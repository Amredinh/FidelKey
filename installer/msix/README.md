# FidelKey MSIX

This folder contains the best-effort Microsoft Store MSIX package for FidelKey.

## What it packages

- `FidelKey.exe` (Tauri settings app)
- `FidelKeyTSF.dll` (Amharic TSF keyboard)
- MSIX tile, Store, and splash assets
- A full-trust Win32 app declaration and the `allowElevation` capability

The Store package cannot register a machine-wide TSF keyboard by itself. On first run the app asks for administrator approval, copies the keyboard to `C:\Program Files\FidelKey`, and calls `regsvr32.exe` to register it. **Remove keyboard** in the app unregisters it and removes those files.

## Build locally

Requires the Windows SDK (`MakeAppx.exe`) and the built `FidelKey.exe` and `FidelKeyTSF.dll`:

```powershell
New-Item -ItemType Directory -Force layout\Assets | Out-Null
Copy-Item ..\..\settings-app\src-tauri\target\release\FidelKey.exe layout\
Copy-Item ..\..\build\tsf-service\Release\FidelKeyTSF.dll layout\
Copy-Item Assets\* layout\Assets\
$manifest = Get-Content AppxManifest.xml -Raw
$manifest = $manifest.Replace("__PUBLISHER__", "CN=Afbix")
$manifest = $manifest.Replace("__VERSION__", "0.1.0.0")
Set-Content layout\AppxManifest.xml -Value $manifest -Encoding UTF8
makeappx.exe pack /d layout /p FidelKey_0.1.0.0_x64.msix /o
```

CI produces the unsigned `FidelKey-MSIX` artifact.

## Store submission

1. Reserve `afbix.fidelkey` in Partner Center.
2. Set the repository variable `MSIX_PUBLISHER` to the exact **Publisher** identity shown by Partner Center (usually `CN=...`). The manifest default `CN=Afbix` will not match a Store identity.
3. Upload the `FidelKey_0.1.0.0_x64.msix` package in the MSIX submission. The Store signs the package after certification.
4. Add the Store listing screenshots and the 300/150/71/1080 px logo exports from `assets/branding/`.

## Known risks

- Microsoft Store certification may reject an app that requests UAC and installs a system-wide input method outside the package. The `allowElevation` capability is a restricted capability.
- The MSIX has no custom uninstall action. If the user uninstalls the package without removing the keyboard first, `C:\Program Files\FidelKey` and the TSF registration may remain.
- The package depends on `Microsoft.VCLibs.140.00.UWPDesktop`; the Store resolves this dependency, but a sideload install needs that framework package.
- TSF installation and activation have not yet been tested on a real Windows machine.
- This package is unsigned in CI. It cannot be sideloaded until it is signed with a trusted certificate; the Store signs accepted submissions.
