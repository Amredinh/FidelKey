#pragma once

#include <windows.h>
#include <msctf.h>

#include <string>

#include <fidelkey/engine.h>

// {12345678-1234-1234-1234-123456789012}
inline const CLSID CLSID_FidelKeyTextService = {
    0x12345678, 0x1234, 0x1234, {0x12, 0x34, 0x12, 0x34, 0x56, 0x78, 0x90, 0x12}};

// {87654321-4321-4321-4321-876543210987}
inline const GUID GUID_FidelKeyProfile = {
    0x87654321, 0x4321, 0x4321, {0x43, 0x21, 0x87, 0x65, 0x43, 0x21, 0x09, 0x87}};

class CFidelKeyTextService : public ITfTextInputProcessor,
                             public ITfThreadMgrEventSink,
                             public ITfKeyEventSink,
                             public ITfCompositionSink {
public:
    CFidelKeyTextService();

    // IUnknown
    STDMETHODIMP QueryInterface(REFIID riid, void** ppvObj);
    STDMETHODIMP_(ULONG) AddRef();
    STDMETHODIMP_(ULONG) Release();

    // ITfTextInputProcessor
    STDMETHODIMP Activate(ITfThreadMgr* pThreadMgr, TfClientId tid);
    STDMETHODIMP Deactivate();

    // ITfThreadMgrEventSink
    STDMETHODIMP OnInitDocumentMgr(ITfDocumentMgr* pDocMgr);
    STDMETHODIMP OnUninitDocumentMgr(ITfDocumentMgr* pDocMgr);
    STDMETHODIMP OnSetFocus(ITfDocumentMgr* pDocMgrFocus, ITfDocumentMgr* pDocMgrPrevFocus);
    STDMETHODIMP OnPushContext(ITfContext* pContext);
    STDMETHODIMP OnPopContext(ITfContext* pContext);

    // ITfKeyEventSink
    STDMETHODIMP OnSetFocus(BOOL fForeground);
    STDMETHODIMP OnTestKeyDown(ITfContext* pContext, WPARAM wParam, LPARAM lParam, BOOL* pfEaten);
    STDMETHODIMP OnKeyDown(ITfContext* pContext, WPARAM wParam, LPARAM lParam, BOOL* pfEaten);
    STDMETHODIMP OnTestKeyUp(ITfContext* pContext, WPARAM wParam, LPARAM lParam, BOOL* pfEaten);
    STDMETHODIMP OnKeyUp(ITfContext* pContext, WPARAM wParam, LPARAM lParam, BOOL* pfEaten);
    STDMETHODIMP OnPreservedKey(ITfContext* pContext, REFGUID rguid, BOOL* pfEaten);

    // ITfCompositionSink
    STDMETHODIMP OnCompositionTerminated(TfEditCookie ecWrite, ITfComposition* pComposition);

    // Called from the edit-session helper.
    enum class EditOp { Start, Update, End };
    HRESULT ApplyEdit(TfEditCookie ec, ITfContext* pContext, EditOp op, const std::wstring& text);

private:
    ~CFidelKeyTextService();

    HRESULT ApplyComposition(ITfContext* pContext, EditOp op, const std::wstring& text);
    HRESULT ShowComposition(ITfContext* pContext, const std::wstring& text);
    void FinalizeComposition(ITfContext* pContext);
    HRESULT HandleLetter(ITfContext* pContext, wchar_t ch);
    HRESULT HandleBackspace(ITfContext* pContext);

    LONG m_cRef;
    TfClientId m_tid;
    ITfThreadMgr* m_pThreadMgr;
    DWORD m_dwThreadMgrEventSinkCookie;
    ITfComposition* m_pComposition;
    fidelkey::engine::PhoneticEngine m_engine;
};
