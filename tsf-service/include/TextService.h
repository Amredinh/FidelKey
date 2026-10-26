#pragma once

#include <windows.h>
#include <msctf.h>
#include <msctfattr.h>
#include <ctffunc.h>
#include <fidelkey/engine.h>

// {FIDELKEY-TEXT-SERVICE-CLSID}
// {12345678-1234-1234-1234-123456789012}
DEFINE_GUID(CLSID_FidelKeyTextService, 0x12345678, 0x1234, 0x1234, 0x12, 0x34, 0x12, 0x34, 0x56, 0x78, 0x90, 0x12);

// {FIDELKEY-LANGUAGE-PROFILE-GUID}
DEFINE_GUID(GUID_FidelKeyProfile, 0x87654321, 0x4321, 0x4321, 0x43, 0x21, 0x87, 0x65, 0x43, 0x21, 0x09, 0x87);

class CFidelKeyTextService : public ITfTextInputProcessor,
                             public ITfThreadMgrEventSink,
                             public ITfDisplayAttributeProvider {
public:
    CFidelKeyTextService();
    ~CFidelKeyTextService();

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

    // ITfDisplayAttributeProvider
    STDMETHODIMP GetDisplayAttributeInfo(const TF_DISPLAYATTRIBUTEINFO* pdaInfo);

    // Internal methods
    HRESULT HandleKey(TfClientId tid, ITfContext* pContext, WPARAM wParam, LPARAM lParam, BOOL* pfEaten);
    HRESULT HandleBackspace(TfClientId tid, ITfContext* pContext, BOOL* pfEaten);

private:
    LONG m_cRef;
    TfClientId m_tid;
    ITfThreadMgr* m_pThreadMgr;
    DWORD m_dwThreadMgrEventSinkCookie;
    fidelkey::engine::PhoneticEngine m_engine;
    
    // Helper methods
    HRESULT InsertText(ITfContext* pContext, const std::wstring& text);
    HRESULT GetContextSelection(ITfContext* pContext, ITfRange** ppRange);
};
