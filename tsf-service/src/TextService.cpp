#include "TextService.h"

#include <new>

using fidelkey::engine::ConversionResult;

namespace {

class CCompositionEditSession : public ITfEditSession {
public:
    CCompositionEditSession(CFidelKeyTextService* service, ITfContext* context,
                            CFidelKeyTextService::EditOp op, const std::wstring& text)
        : m_cRef(1), m_service(service), m_context(context), m_op(op), m_text(text) {
        m_service->AddRef();
        m_context->AddRef();
    }

    STDMETHODIMP QueryInterface(REFIID riid, void** ppv) {
        if (ppv == nullptr) return E_INVALIDARG;
        *ppv = nullptr;
        if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_ITfEditSession)) {
            *ppv = static_cast<ITfEditSession*>(this);
            AddRef();
            return S_OK;
        }
        return E_NOINTERFACE;
    }

    STDMETHODIMP_(ULONG) AddRef() { return InterlockedIncrement(&m_cRef); }

    STDMETHODIMP_(ULONG) Release() {
        LONG cRef = InterlockedDecrement(&m_cRef);
        if (cRef == 0) delete this;
        return cRef;
    }

    STDMETHODIMP DoEditSession(TfEditCookie ec) {
        return m_service->ApplyEdit(ec, m_context, m_op, m_text);
    }

private:
    ~CCompositionEditSession() {
        m_context->Release();
        m_service->Release();
    }

    LONG m_cRef;
    CFidelKeyTextService* m_service;
    ITfContext* m_context;
    CFidelKeyTextService::EditOp m_op;
    std::wstring m_text;
};

bool IsModifierKey(WPARAM vk) {
    switch (vk) {
    case VK_SHIFT:
    case VK_LSHIFT:
    case VK_RSHIFT:
    case VK_CONTROL:
    case VK_LCONTROL:
    case VK_RCONTROL:
    case VK_MENU:
    case VK_LMENU:
    case VK_RMENU:
    case VK_CAPITAL:
    case VK_LWIN:
    case VK_RWIN:
    case VK_NUMLOCK:
    case VK_SCROLL:
    case VK_SNAPSHOT:
        return true;
    default:
        return false;
    }
}

bool IsLetterKey(WPARAM vk) {
    return vk >= 'A' && vk <= 'Z';
}

bool IsControlDown() {
    return (GetKeyState(VK_CONTROL) & 0x8000) != 0;
}

bool IsAltDown() {
    return (GetKeyState(VK_MENU) & 0x8000) != 0;
}

bool IsShiftDown() {
    return (GetKeyState(VK_SHIFT) & 0x8000) != 0;
}

wchar_t LetterFromVk(WPARAM vk) {
    bool upper = IsShiftDown();
    if ((GetKeyState(VK_CAPITAL) & 1) != 0) {
        upper = !upper;
    }
    wchar_t lower = static_cast<wchar_t>(vk - 'A' + 'a');
    return upper ? static_cast<wchar_t>(vk) : lower;
}

} // namespace

CFidelKeyTextService::CFidelKeyTextService()
    : m_cRef(1),
      m_tid(TF_CLIENTID_NULL),
      m_pThreadMgr(nullptr),
      m_dwThreadMgrEventSinkCookie(TF_INVALID_COOKIE),
      m_pComposition(nullptr) {
}

CFidelKeyTextService::~CFidelKeyTextService() {
    if (m_pComposition != nullptr) {
        m_pComposition->Release();
        m_pComposition = nullptr;
    }
    if (m_pThreadMgr != nullptr) {
        m_pThreadMgr->Release();
        m_pThreadMgr = nullptr;
    }
}

STDMETHODIMP CFidelKeyTextService::QueryInterface(REFIID riid, void** ppvObj) {
    if (ppvObj == nullptr) return E_INVALIDARG;
    *ppvObj = nullptr;

    if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_ITfTextInputProcessor)) {
        *ppvObj = static_cast<ITfTextInputProcessor*>(this);
    } else if (IsEqualIID(riid, IID_ITfThreadMgrEventSink)) {
        *ppvObj = static_cast<ITfThreadMgrEventSink*>(this);
    } else if (IsEqualIID(riid, IID_ITfKeyEventSink)) {
        *ppvObj = static_cast<ITfKeyEventSink*>(this);
    } else if (IsEqualIID(riid, IID_ITfCompositionSink)) {
        *ppvObj = static_cast<ITfCompositionSink*>(this);
    }

    if (*ppvObj != nullptr) {
        AddRef();
        return S_OK;
    }
    return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) CFidelKeyTextService::AddRef() {
    return InterlockedIncrement(&m_cRef);
}

STDMETHODIMP_(ULONG) CFidelKeyTextService::Release() {
    LONG cRef = InterlockedDecrement(&m_cRef);
    if (cRef == 0) {
        delete this;
    }
    return cRef;
}

STDMETHODIMP CFidelKeyTextService::Activate(ITfThreadMgr* pThreadMgr, TfClientId tid) {
    if (pThreadMgr == nullptr) return E_INVALIDARG;

    m_tid = tid;
    m_pThreadMgr = pThreadMgr;
    m_pThreadMgr->AddRef();

    ITfSource* pSource = nullptr;
    if (SUCCEEDED(m_pThreadMgr->QueryInterface(IID_ITfSource, (void**)&pSource))) {
        pSource->AdviseSink(IID_ITfThreadMgrEventSink, static_cast<ITfThreadMgrEventSink*>(this),
                            &m_dwThreadMgrEventSinkCookie);
        pSource->Release();
    }

    ITfKeystrokeMgr* pKeystrokeMgr = nullptr;
    if (SUCCEEDED(m_pThreadMgr->QueryInterface(IID_ITfKeystrokeMgr, (void**)&pKeystrokeMgr))) {
        pKeystrokeMgr->AdviseKeyEventSink(tid, static_cast<ITfKeyEventSink*>(this), TRUE);
        pKeystrokeMgr->Release();
    }

    m_engine.Reset();
    return S_OK;
}

STDMETHODIMP CFidelKeyTextService::Deactivate() {
    if (m_pThreadMgr != nullptr) {
        ITfKeystrokeMgr* pKeystrokeMgr = nullptr;
        if (SUCCEEDED(m_pThreadMgr->QueryInterface(IID_ITfKeystrokeMgr, (void**)&pKeystrokeMgr))) {
            pKeystrokeMgr->UnadviseKeyEventSink(m_tid);
            pKeystrokeMgr->Release();
        }

        ITfSource* pSource = nullptr;
        if (SUCCEEDED(m_pThreadMgr->QueryInterface(IID_ITfSource, (void**)&pSource))) {
            if (m_dwThreadMgrEventSinkCookie != TF_INVALID_COOKIE) {
                pSource->UnadviseSink(m_dwThreadMgrEventSinkCookie);
                m_dwThreadMgrEventSinkCookie = TF_INVALID_COOKIE;
            }
            pSource->Release();
        }

        m_pThreadMgr->Release();
        m_pThreadMgr = nullptr;
    }

    if (m_pComposition != nullptr) {
        m_pComposition->Release();
        m_pComposition = nullptr;
    }

    m_engine.Reset();
    m_tid = TF_CLIENTID_NULL;
    return S_OK;
}

STDMETHODIMP CFidelKeyTextService::OnInitDocumentMgr(ITfDocumentMgr* pDocMgr) {
    return S_OK;
}

STDMETHODIMP CFidelKeyTextService::OnUninitDocumentMgr(ITfDocumentMgr* pDocMgr) {
    return S_OK;
}

STDMETHODIMP CFidelKeyTextService::OnSetFocus(ITfDocumentMgr* pDocMgrFocus,
                                              ITfDocumentMgr* pDocMgrPrevFocus) {
    return S_OK;
}

STDMETHODIMP CFidelKeyTextService::OnPushContext(ITfContext* pContext) {
    return S_OK;
}

STDMETHODIMP CFidelKeyTextService::OnPopContext(ITfContext* pContext) {
    return S_OK;
}

STDMETHODIMP CFidelKeyTextService::OnSetFocus(BOOL fForeground) {
    return S_OK;
}

STDMETHODIMP CFidelKeyTextService::OnTestKeyDown(ITfContext* pContext, WPARAM wParam,
                                                 LPARAM lParam, BOOL* pfEaten) {
    if (pfEaten == nullptr) return E_INVALIDARG;
    *pfEaten = FALSE;

    if (IsControlDown() || IsAltDown()) {
        if (m_engine.HasComposition() && pContext != nullptr) {
            FinalizeComposition(pContext);
        }
        return S_OK;
    }

    if (IsLetterKey(wParam)) {
        *pfEaten = TRUE;
        return S_OK;
    }

    if (wParam == VK_BACK) {
        if (m_engine.HasComposition()) {
            *pfEaten = TRUE;
        }
        return S_OK;
    }

    // Any other non-modifier key ends the current word; the key itself is left
    // for the application so space, punctuation, Enter etc. behave normally.
    if (m_engine.HasComposition() && pContext != nullptr && !IsModifierKey(wParam)) {
        FinalizeComposition(pContext);
    }
    return S_OK;
}

STDMETHODIMP CFidelKeyTextService::OnKeyDown(ITfContext* pContext, WPARAM wParam,
                                             LPARAM lParam, BOOL* pfEaten) {
    if (pfEaten == nullptr) return E_INVALIDARG;
    *pfEaten = FALSE;
    if (pContext == nullptr) return E_INVALIDARG;

    if (IsControlDown() || IsAltDown()) return S_OK;

    if (IsLetterKey(wParam)) {
        if (HandleLetter(pContext, LetterFromVk(wParam)) == S_OK) {
            *pfEaten = TRUE;
        }
        return S_OK;
    }

    if (wParam == VK_BACK) {
        if (HandleBackspace(pContext) == S_OK) {
            *pfEaten = TRUE;
        }
    }
    return S_OK;
}

STDMETHODIMP CFidelKeyTextService::OnTestKeyUp(ITfContext* pContext, WPARAM wParam,
                                               LPARAM lParam, BOOL* pfEaten) {
    if (pfEaten == nullptr) return E_INVALIDARG;
    *pfEaten = FALSE;
    return S_OK;
}

STDMETHODIMP CFidelKeyTextService::OnKeyUp(ITfContext* pContext, WPARAM wParam,
                                           LPARAM lParam, BOOL* pfEaten) {
    if (pfEaten == nullptr) return E_INVALIDARG;
    *pfEaten = FALSE;
    return S_OK;
}

STDMETHODIMP CFidelKeyTextService::OnPreservedKey(ITfContext* pContext, REFGUID rguid,
                                                  BOOL* pfEaten) {
    if (pfEaten == nullptr) return E_INVALIDARG;
    *pfEaten = FALSE;
    return S_OK;
}

STDMETHODIMP CFidelKeyTextService::OnCompositionTerminated(TfEditCookie ecWrite,
                                                           ITfComposition* pComposition) {
    if (m_pComposition == pComposition) {
        m_pComposition->Release();
        m_pComposition = nullptr;
    }
    m_engine.Reset();
    return S_OK;
}

HRESULT CFidelKeyTextService::HandleLetter(ITfContext* pContext, wchar_t ch) {
    ConversionResult result = m_engine.ProcessKey(ch);
    if (!result.consumed) return S_FALSE;

    if (!m_engine.HasComposition()) {
        return S_FALSE;
    }

    std::wstring display = m_engine.GetComposition();
    HRESULT hr = ShowComposition(pContext, display);
    if (FAILED(hr)) {
        m_engine.Reset();
        return hr;
    }
    return S_OK;
}

HRESULT CFidelKeyTextService::HandleBackspace(ITfContext* pContext) {
    ConversionResult result = m_engine.ProcessBackspace();
    if (!result.consumed) return S_FALSE;

    if (m_engine.HasComposition()) {
        return ShowComposition(pContext, m_engine.GetComposition());
    }

    if (m_pComposition != nullptr) {
        return ApplyComposition(pContext, EditOp::End, L"");
    }
    return S_OK;
}

HRESULT CFidelKeyTextService::ShowComposition(ITfContext* pContext, const std::wstring& text) {
    if (text.empty()) {
        if (m_pComposition != nullptr) {
            return ApplyComposition(pContext, EditOp::End, L"");
        }
        return S_OK;
    }
    EditOp op = (m_pComposition != nullptr) ? EditOp::Update : EditOp::Start;
    return ApplyComposition(pContext, op, text);
}

void CFidelKeyTextService::FinalizeComposition(ITfContext* pContext) {
    if (!m_engine.HasComposition()) return;
    ConversionResult result = m_engine.Commit();
    if (m_pComposition != nullptr) {
        ApplyComposition(pContext, EditOp::End, result.output);
    }
}

HRESULT CFidelKeyTextService::ApplyComposition(ITfContext* pContext, EditOp op,
                                               const std::wstring& text) {
    if (pContext == nullptr) return E_INVALIDARG;

    CCompositionEditSession* session = new (std::nothrow) CCompositionEditSession(
        this, pContext, op, text);
    if (session == nullptr) return E_OUTOFMEMORY;

    HRESULT sessionResult = E_FAIL;
    HRESULT hr = pContext->RequestEditSession(
        m_tid, session, TF_ES_SYNC | TF_ES_READWRITE, &sessionResult);
    session->Release();

    if (FAILED(hr)) return hr;
    return sessionResult;
}

HRESULT CFidelKeyTextService::ApplyEdit(TfEditCookie ec, ITfContext* pContext, EditOp op,
                                        const std::wstring& text) {
    if (pContext == nullptr) return E_INVALIDARG;

    if (op == EditOp::Start) {
        ITfComposition* composition = nullptr;
        HRESULT hr = pContext->StartComposition(ec, &composition);
        if (FAILED(hr) || composition == nullptr) {
            return FAILED(hr) ? hr : E_FAIL;
        }

        composition->SetSink(static_cast<ITfCompositionSink*>(this));

        ITfRange* range = nullptr;
        if (SUCCEEDED(composition->GetRange(&range)) && range != nullptr) {
            if (!text.empty()) {
                range->SetText(ec, 0, text.c_str(), static_cast<LONG>(text.size()));
            }
            range->Release();
        }

        m_pComposition = composition;
        return S_OK;
    }

    if (op == EditOp::Update) {
        if (m_pComposition == nullptr) return S_OK;
        ITfRange* range = nullptr;
        HRESULT hr = m_pComposition->GetRange(&range);
        if (FAILED(hr) || range == nullptr) return hr;
        hr = range->SetText(ec, 0, text.c_str(), static_cast<LONG>(text.size()));
        range->Release();
        return hr;
    }

    // EditOp::End
    ITfComposition* composition = m_pComposition;
    m_pComposition = nullptr;
    if (composition == nullptr) return S_OK;

    ITfRange* range = nullptr;
    if (SUCCEEDED(composition->GetRange(&range)) && range != nullptr) {
        range->SetText(ec, 0, text.c_str(), static_cast<LONG>(text.size()));
        range->Release();
    }

    composition->EndComposition(ec);
    composition->Release();
    return S_OK;
}
