#include <windows.h>

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

// DLL exports for registration
extern "C" __declspec(dllexport) HRESULT DllRegisterServer(void);
extern "C" __declspec(dllexport) HRESULT DllUnregisterServer(void);
extern "C" __declspec(dllexport) HRESULT DllCanUnloadNow(void);
extern "C" __declspec(dllexport) HRESULT DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv);
