#include "TextService.h"

#include <objbase.h>

#include <cwchar>
#include <new>
#include <string>

extern HINSTANCE g_hInstance;

namespace {

HRESULT GetModulePath(wchar_t* path, DWORD cchPath);

class CFidelKeyClassFactory : public IClassFactory {
public:
    CFidelKeyClassFactory() : m_cRef(1) {}

    STDMETHODIMP QueryInterface(REFIID riid, void** ppv) {
        if (ppv == nullptr) return E_INVALIDARG;
        *ppv = nullptr;
        if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IClassFactory)) {
            *ppv = static_cast<IClassFactory*>(this);
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

        CFidelKeyTextService* pTextService = new (std::nothrow) CFidelKeyTextService();
        if (pTextService == nullptr) return E_OUTOFMEMORY;

        HRESULT hr = pTextService->QueryInterface(riid, ppv);
        pTextService->Release();
        return hr;
    }

    STDMETHODIMP LockServer(BOOL fLock) { return S_OK; }

private:
    LONG m_cRef;
};

HRESULT RegisterComServer(const CLSID& clsid, const wchar_t* dllPath) {
    wchar_t clsidString[64] = {};
    if (StringFromGUID2(clsid, clsidString, 64) == 0) return E_FAIL;

    const std::wstring clsidKey = std::wstring(L"CLSID\\") + clsidString;
    const wchar_t* description = L"FidelKey Amharic Input Method";

    HKEY hClsid = nullptr;
    LSTATUS status = RegCreateKeyExW(HKEY_CLASSES_ROOT, clsidKey.c_str(), 0, nullptr, 0,
                                     KEY_WRITE, nullptr, &hClsid, nullptr);
    if (status != ERROR_SUCCESS) return HRESULT_FROM_WIN32(status);

    RegSetValueExW(hClsid, nullptr, 0, REG_SZ, reinterpret_cast<const BYTE*>(description),
                   static_cast<DWORD>((wcslen(description) + 1) * sizeof(wchar_t)));

    HKEY hInproc = nullptr;
    status = RegCreateKeyExW(hClsid, L"InprocServer32", 0, nullptr, 0, KEY_WRITE, nullptr,
                             &hInproc, nullptr);
    if (status == ERROR_SUCCESS) {
        RegSetValueExW(hInproc, nullptr, 0, REG_SZ, reinterpret_cast<const BYTE*>(dllPath),
                       static_cast<DWORD>((wcslen(dllPath) + 1) * sizeof(wchar_t)));
        const wchar_t* threadingModel = L"Apartment";
        RegSetValueExW(hInproc, L"ThreadingModel", 0, REG_SZ,
                       reinterpret_cast<const BYTE*>(threadingModel),
                       static_cast<DWORD>((wcslen(threadingModel) + 1) * sizeof(wchar_t)));
        RegCloseKey(hInproc);
    }

    RegCloseKey(hClsid);

    if (status != ERROR_SUCCESS) return HRESULT_FROM_WIN32(status);
    return S_OK;
}

HRESULT RegisterTextService(const wchar_t* dllPath) {
    HRESULT hr = RegisterComServer(CLSID_FidelKeyTextService, dllPath);
    if (FAILED(hr)) return hr;

    ITfInputProcessorProfiles* profiles = nullptr;
    hr = CoCreateInstance(CLSID_TF_InputProcessorProfiles, nullptr, CLSCTX_INPROC_SERVER,
                          IID_ITfInputProcessorProfiles, (void**)&profiles);
    if (FAILED(hr)) return hr;

    hr = profiles->Register(CLSID_FidelKeyTextService);
    if (SUCCEEDED(hr)) {
        const wchar_t* description = L"FidelKey Amharic (Phonetic)";
        // Amharic (Ethiopia) language id.
        const LANGID amharic = 0x045E;

        // Use the installed settings executable as the language-bar icon source.
        std::wstring iconPath;
        wchar_t modulePath[MAX_PATH] = {};
        if (SUCCEEDED(GetModulePath(modulePath, MAX_PATH))) {
            iconPath.assign(modulePath);
            const size_t slash = iconPath.find_last_of(L"\\/");
            if (slash != std::wstring::npos) {
                iconPath.resize(slash + 1);
                iconPath += L"FidelKey.exe";
                if (GetFileAttributesW(iconPath.c_str()) == INVALID_FILE_ATTRIBUTES) {
                    iconPath.clear();
                }
            } else {
                iconPath.clear();
            }
        }

        hr = profiles->AddLanguageProfile(
            CLSID_FidelKeyTextService, amharic, GUID_FidelKeyProfile,
            description, static_cast<ULONG>(wcslen(description)),
            iconPath.empty() ? nullptr : iconPath.c_str(),
            static_cast<ULONG>(iconPath.size()), 0);
    }
    profiles->Release();

    ITfCategoryMgr* categories = nullptr;
    if (SUCCEEDED(CoCreateInstance(CLSID_TF_CategoryMgr, nullptr, CLSCTX_INPROC_SERVER,
                                   IID_ITfCategoryMgr, (void**)&categories))) {
        categories->RegisterCategory(CLSID_FidelKeyTextService, GUID_TFCAT_TIP_KEYBOARD,
                                     CLSID_FidelKeyTextService);
        categories->Release();
    }

    return hr;
}

HRESULT UnregisterTextService() {
    ITfInputProcessorProfiles* profiles = nullptr;
    if (SUCCEEDED(CoCreateInstance(CLSID_TF_InputProcessorProfiles, nullptr,
                                   CLSCTX_INPROC_SERVER, IID_ITfInputProcessorProfiles,
                                   (void**)&profiles))) {
        const LANGID amharic = 0x045E;
        profiles->RemoveLanguageProfile(CLSID_FidelKeyTextService, amharic, GUID_FidelKeyProfile);
        profiles->Unregister(CLSID_FidelKeyTextService);
        profiles->Release();
    }

    ITfCategoryMgr* categories = nullptr;
    if (SUCCEEDED(CoCreateInstance(CLSID_TF_CategoryMgr, nullptr, CLSCTX_INPROC_SERVER,
                                   IID_ITfCategoryMgr, (void**)&categories))) {
        categories->UnregisterCategory(CLSID_FidelKeyTextService, GUID_TFCAT_TIP_KEYBOARD,
                                       CLSID_FidelKeyTextService);
        categories->Release();
    }

    wchar_t clsidString[64] = {};
    if (StringFromGUID2(CLSID_FidelKeyTextService, clsidString, 64) != 0) {
        const std::wstring clsidKey = std::wstring(L"CLSID\\") + clsidString;
        RegDeleteTreeW(HKEY_CLASSES_ROOT, clsidKey.c_str());
    }

    return S_OK;
}

HRESULT GetModulePath(wchar_t* path, DWORD cchPath) {
    if (g_hInstance == nullptr) return E_FAIL;
    DWORD copied = GetModuleFileNameW(g_hInstance, path, cchPath);
    if (copied == 0 || copied >= cchPath) return HRESULT_FROM_WIN32(GetLastError());
    return S_OK;
}

} // namespace

extern "C" HRESULT STDAPICALLTYPE DllGetClassObject(REFCLSID rclsid, REFIID riid,
                                                            LPVOID* ppv) {
    if (ppv == nullptr) return E_INVALIDARG;
    *ppv = nullptr;

    if (!IsEqualCLSID(rclsid, CLSID_FidelKeyTextService)) {
        return CLASS_E_CLASSNOTAVAILABLE;
    }

    CFidelKeyClassFactory* pFactory = new (std::nothrow) CFidelKeyClassFactory();
    if (pFactory == nullptr) return E_OUTOFMEMORY;

    HRESULT hr = pFactory->QueryInterface(riid, ppv);
    pFactory->Release();
    return hr;
}

extern "C" HRESULT STDAPICALLTYPE DllCanUnloadNow(void) {
    return S_FALSE;
}

extern "C" HRESULT STDAPICALLTYPE DllRegisterServer(void) {
    wchar_t dllPath[MAX_PATH] = {};
    HRESULT hr = GetModulePath(dllPath, MAX_PATH);
    if (FAILED(hr)) return hr;

    HRESULT coInit = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    bool shouldUninitialize = SUCCEEDED(coInit);
    if (FAILED(coInit) && coInit != RPC_E_CHANGED_MODE) return coInit;

    hr = RegisterTextService(dllPath);

    if (shouldUninitialize) CoUninitialize();
    return hr;
}

extern "C" HRESULT STDAPICALLTYPE DllUnregisterServer(void) {
    HRESULT coInit = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    bool shouldUninitialize = SUCCEEDED(coInit);
    if (FAILED(coInit) && coInit != RPC_E_CHANGED_MODE) return coInit;

    HRESULT hr = UnregisterTextService();

    if (shouldUninitialize) CoUninitialize();
    return hr;
}
