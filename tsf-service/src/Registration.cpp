#include <windows.h>
#include <msctf.h>

extern "C" __declspec(dllexport) HRESULT DllRegisterServer(void) {
    // Basic registration - full implementation needed
    return S_OK;
}

extern "C" __declspec(dllexport) HRESULT DllUnregisterServer(void) {
    return S_OK;
}

extern "C" __declspec(dllexport) HRESULT DllCanUnloadNow(void) {
    return S_FALSE;
}

extern "C" __declspec(dllexport) HRESULT DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv) {
    if (ppv) *ppv = nullptr;
    return CLASS_E_CLASSNOTAVAILABLE;
}
