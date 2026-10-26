#include "TextService.h"

CFidelKeyTextService::CFidelKeyTextService()
    : m_cRef(1)
    , m_tid(TF_CLIENTID_NULL)
    , m_pThreadMgr(nullptr)
    , m_dwThreadMgrEventSinkCookie(TF_INVALID_COOKIE)
    , m_pCompositionRange(nullptr)
    , m_pComposition(nullptr)
{
}

CFidelKeyTextService::~CFidelKeyTextService()
{
    if (m_pComposition) {
        m_pComposition->EndComposition(nullptr);
        m_pComposition->Release();
        m_pComposition = nullptr;
    }
    if (m_pCompositionRange) {
        m_pCompositionRange->Release();
        m_pCompositionRange = nullptr;
    }
    if (m_pThreadMgr) {
        m_pThreadMgr->Release();
        m_pThreadMgr = nullptr;
    }
}

STDMETHODIMP CFidelKeyTextService::QueryInterface(REFIID riid, void** ppvObj)
{
    if (ppvObj == nullptr) return E_INVALIDARG;
    *ppvObj = nullptr;

    if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_ITfTextInputProcessor)) {
        *ppvObj = (ITfTextInputProcessor*)this;
    } else if (IsEqualIID(riid, IID_ITfThreadMgrEventSink)) {
        *ppvObj = (ITfThreadMgrEventSink*)this;
    } else if (IsEqualIID(riid, IID_ITfDisplayAttributeProvider)) {
        *ppvObj = (ITfDisplayAttributeProvider*)this;
    }

    if (*ppvObj) {
        AddRef();
        return S_OK;
    }
    return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) CFidelKeyTextService::AddRef()
{
    return InterlockedIncrement(&m_cRef);
}

STDMETHODIMP_(ULONG) CFidelKeyTextService::Release()
{
    LONG cRef = InterlockedDecrement(&m_cRef);
    if (cRef == 0) {
        delete this;
    }
    return cRef;
}

STDMETHODIMP CFidelKeyTextService::Activate(ITfThreadMgr* pThreadMgr, TfClientId tid)
{
    if (pThreadMgr == nullptr) return E_INVALIDARG;

    m_tid = tid;
    m_pThreadMgr = pThreadMgr;
    m_pThreadMgr->AddRef();

    ITfSource* pSource = nullptr;
    if (SUCCEEDED(m_pThreadMgr->QueryInterface(IID_ITfSource, (void**)&pSource))) {
        pSource->AdviseSink(IID_ITfThreadMgrEventSink, (ITfThreadMgrEventSink*)this, &m_dwThreadMgrEventSinkCookie);
        pSource->Release();
    }

    m_engine.Reset();
    return S_OK;
}

STDMETHODIMP CFidelKeyTextService::Deactivate()
{
    if (m_pThreadMgr != nullptr) {
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
    m_engine.Reset();
    return S_OK;
}

STDMETHODIMP CFidelKeyTextService::OnInitDocumentMgr(ITfDocumentMgr* pDocMgr)
{
    return S_OK;
}

STDMETHODIMP CFidelKeyTextService::OnUninitDocumentMgr(ITfDocumentMgr* pDocMgr)
{
    return S_OK;
}

STDMETHODIMP CFidelKeyTextService::OnSetFocus(ITfDocumentMgr* pDocMgrFocus, ITfDocumentMgr* pDocMgrPrevFocus)
{
    m_engine.Reset();
    return S_OK;
}

STDMETHODIMP CFidelKeyTextService::OnPushContext(ITfContext* pContext)
{
    return S_OK;
}

STDMETHODIMP CFidelKeyTextService::OnPopContext(ITfContext* pContext)
{
    return S_OK;
}

STDMETHODIMP CFidelKeyTextService::GetDisplayAttributeInfo(const TF_DISPLAYATTRIBUTEINFO* pdaInfo)
{
    return E_NOTIMPL;
}

HRESULT CFidelKeyTextService::HandleKey(TfClientId tid, ITfContext* pContext, WPARAM wParam, LPARAM lParam, BOOL* pfEaten)
{
    if (pfEaten == nullptr) return E_INVALIDARG;
    *pfEaten = FALSE;

    if (pContext == nullptr) return E_INVALIDARG;

    wchar_t key = (wchar_t)wParam;
    if (key == 0) return S_OK;

    auto result = m_engine.ProcessKey(key);

    if (result.consumed) {
        *pfEaten = TRUE;

        if (result.shouldCommit && !result.output.empty()) {
            InsertText(pContext, result.output);
            m_engine.Reset();
        }
    }

    return S_OK;
}

HRESULT CFidelKeyTextService::HandleBackspace(TfClientId tid, ITfContext* pContext, BOOL* pfEaten)
{
    if (pfEaten == nullptr) return E_INVALIDARG;
    *pfEaten = FALSE;

    if (pContext == nullptr) return E_INVALIDARG;

    auto result = m_engine.ProcessBackspace();

    if (result.consumed) {
        *pfEaten = TRUE;
        if (result.shouldCommit && !result.output.empty()) {
            InsertText(pContext, result.output);
        }
    }

    return S_OK;
}

HRESULT CFidelKeyTextService::InsertText(ITfContext* pContext, const std::wstring& text)
{
    if (pContext == nullptr || text.empty()) return E_INVALIDARG;

    ITfInsertAtSelection* pInsertAtSelection = nullptr;
    HRESULT hr = pContext->QueryInterface(IID_ITfInsertAtSelection, (void**)&pInsertAtSelection);
    if (SUCCEEDED(hr)) {
        ITfRange* pRange = nullptr;
        hr = pInsertAtSelection->InsertTextAtSelection(m_tid, 0, text.c_str(), (LONG)text.length(), &pRange);
        if (pRange) pRange->Release();
        pInsertAtSelection->Release();
    }
    return hr;
}
