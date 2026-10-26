#include <windows.h>
#include <msctf.h>
#include "TextService.h"

// Class factory
class CFidelKeyClassFactory : public IClassFactory {
public:
    CFidelKeyClassFactory() : m_cRef(1) {}
    
    STDMETHODIMP QueryInterface(REFIID riid, void** ppv) {
        if (ppv == nullptr) return E_INVALIDARG;
        *ppv = nullptr;
        if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IClassFactory)) {
            *ppv = (IClassFactory*)this;
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
    
    STDMETHODIMP CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppv) {
        if (pUnkOuter != nullptr) return CLASS_E_NOAGGREGATION;
        if (ppv == nullptr) return E_INVALIDARG;
        *ppv = nullptr;
        
        CFidelKeyTextService* pTextService = new CFidelKeyTextService();
        if (pTextService == nullptr) return E_OUTOFMEMORY;
        
        HRESULT hr = pTextService->QueryInterface(riid, ppv);
        pTextService->Release();
        return hr;
    }
    
    STDMETHODIMP LockServer(BOOL fLock) {
        return S_OK;
    }
    
private:
    LONG m_cRef;
};

extern "C" __declspec(dllexport) HRESULT DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    if (ppv == nullptr) return E_INVALIDARG;
    *ppv = nullptr;
    
    if (!IsEqualCLSID(rclsid, CLSID_FidelKeyTextService)) {
        return CLASS_E_CLASSNOTAVAILABLE;
    }
    
    CFidelKeyClassFactory* pFactory = new CFidelKeyClassFactory();
    if (pFactory == nullptr) return E_OUTOFMEMORY;
    
    HRESULT hr = pFactory->QueryInterface(riid, ppv);
    pFactory->Release();
    return hr;
}

extern "C" __declspec(dllexport) HRESULT DllCanUnloadNow(void)
{
    return S_FALSE;
}

extern "C" __declspec(dllexport) HRESULT DllRegisterServer(void)
{
    // Registration implementation
    // In full implementation, register CLSID, profile, etc.
    return S_OK;
}

extern "C" __declspec(dllexport) HRESULT DllUnregisterServer(void)
{
    return S_OK;
}
