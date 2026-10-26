#pragma once

#include <windows.h>
#include <msctf.h>
#include <msctfattr.h>
#include <ctffunc.h>
#include <fidelkey/engine.h>

DEFINE_GUID(CLSID_FidelKeyTextService, 0x12345678, 0x1234, 0x1234, 0x12, 0x34, 0x12, 0x34, 0x56, 0x78, 0x90, 0x12);
DEFINE_GUID(GUID_FidelKeyProfile, 0x87654321, 0x4321, 0x4321, 0x43, 0x21, 0x87, 0x65, 0x43, 0x21, 0x09, 0x87);

class CFidelKeyTextService : public ITfTextInputProcessor,
                             public ITfThreadMgrEventSink,
                             public ITfDisplayAttributeProvider {
public:
    CFidelKeyTextService();
    ~CFidelKeyTextService();

    STDMETHODIMP QueryInterface(REFIID riid, void** ppvObj);
    STDMETHODIMP_(ULONG) AddRef();
    STDMETHODIMP_(ULONG) Release();

    STDMETHODIMP Activate(ITfThreadMgr* pThreadMgr, TfClientId tid);
    STDMETHODIMP Deactivate();

    STDMETHODIMP OnInitDocumentMgr(ITfDocumentMgr* pDocMgr);
    STDMETHODIMP OnUninitDocumentMgr(ITfDocumentMgr* pDocMgr);
    STDMETHODIMP OnSetFocus(ITfDocumentMgr* pDocMgrFocus, ITfDocumentMgr* pDocMgrPrevFocus);
    STDMETHODIMP OnPushContext(ITfContext* pContext);
    STDMETHODIMP OnPopContext(ITfContext* pContext);

    STDMETHODIMP GetDisplayAttributeInfo(const TF_DISPLAYATTRIBUTEINFO* pdaInfo);

    HRESULT HandleKey(TfClientId tid, ITfContext* pContext, WPARAM wParam, LPARAM lParam, BOOL* pfEaten);
    HRESULT HandleBackspace(TfClientId tid, ITfContext* pContext, BOOL* pfEaten);

private:
    LONG m_cRef;
    TfClientId m_tid;
    ITfThreadMgr* m_pThreadMgr;
    DWORD m_dwThreadMgrEventSinkCookie;
    fidelkey::engine::PhoneticEngine m_engine;
    
    HRESULT InsertText(ITfContext* pContext, const std::wstring& text);
    ITfRange* m_pCompositionRange;
    ITfComposition* m_pComposition;
};
