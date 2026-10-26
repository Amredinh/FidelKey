# FidelKey Installer

The installer is a custom NSIS script (`fidelkey.nsi`). NSIS was chosen over WiX
and MSIX because TSF registration requires running `regsvr32` on a normal
in-proc COM server, which is not possible inside a sandboxed MSIX package.

## What it installs

- `FidelKey.exe` — Tauri settings app
- `FidelKeyTSF.dll` — TSF text service (registered via `regsvr32 /s`)
- Start Menu shortcut and an optional desktop shortcut
- Add/Remove Programs entry

The DLL registration writes the COM `InprocServer32` key and calls
`ITfInputProcessorProfiles::Register` +
`AddLanguageProfile(..., 0x045E, GUID_FidelKeyProfile, ...)` plus
`ITfCategoryMgr::RegisterCategory(GUID_TFCAT_TIP_KEYBOARD)`.

## Building locally

Inputs must exist before running `makensis`:

```powershell
cmake -S . -B build -DBUILD_TESTS=OFF
cmake --build build --config Release --target FidelKeyTSF

cd settings-app
npm install
npm run tauri build -- --no-bundle

cd ..
New-Item -ItemType Directory -Force installer/dist | Out-Null
Copy-Item build\tsf-service\Release\FidelKeyTSF.dll installer\dist\FidelKeyTSF.dll
Copy-Item settings-app\src-tauri\target\release\FidelKey.exe installer\dist\FidelKey.exe

# Optional but recommended, so the settings app runs on a clean Windows install:
Invoke-WebRequest "https://aka.ms/vs/17/release/vc_redist.x64.exe" -OutFile dist\vc_redist.x64.exe

cd installer
makensis "/DVERSION=0.1.0" "/DSOURCE_EXE=$PWD\dist\FidelKey.exe" "/DSOURCE_DLL=$PWD\dist\FidelKeyTSF.dll" "/DSOURCE_VCREDIST=$PWD\dist\vc_redist.x64.exe" "/DICON_FILE=$PWD\..\settings-app\src-tauri\icons\icon.ico" fidelkey.nsi
```

Output: `installer/FidelKey-Setup-0.1.0.exe`.

GitHub Actions does the same thing in the `installer` job and uploads the setup
executable as an artifact.

## Uninstallation

The uninstaller runs `regsvr32 /u` to remove the COM registration and language
profile before deleting files, shortcuts, and registry entries.

## Requirements and caveats

- Windows 10 1903+ or Windows 11 (64-bit).
- Administrator rights are required for per-machine installation and
  registration.
- The TSF DLL links the C++ runtime statically. The Rust settings binary uses
  the MSVC runtime, so the installer bundles and silently installs
  `vc_redist.x64.exe`.
- WebView2 is required by the settings app; it ships with Windows 11 and current
  Windows 10.
- A sign-out or restart may be needed on some systems before the new keyboard
  appears in the Win+Space list.
- The installer is not code-signed. Windows SmartScreen may warn on first run.
- The traditional Fidel layout is not included in this version.
