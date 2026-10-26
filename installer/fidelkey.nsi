!include "MUI2.nsh"
!include "LogicLib.nsh"

!ifndef VERSION
  !define VERSION "0.1.0"
!endif

!ifndef SOURCE_EXE
  !define SOURCE_EXE "..\FidelKey.exe"
!endif

!ifndef SOURCE_DLL
  !define SOURCE_DLL "..\FidelKeyTSF.dll"
!endif

!ifdef ICON_FILE
  !define MUI_ICON "${ICON_FILE}"
  !define MUI_UNICON "${ICON_FILE}"
!endif

Name "FidelKey ${VERSION}"
OutFile "FidelKey-Setup-${VERSION}.exe"
InstallDir "$PROGRAMFILES64\FidelKey"
InstallDirRegKey HKLM "Software\FidelKey" "InstallLocation"
RequestExecutionLevel admin
Unicode true

SetCompressor /SOLID lzma
SetShellVarContext all
SetRegView 64

VIProductVersion "${VERSION}.0"
VIAddVersionKey "ProductName" "FidelKey"
VIAddVersionKey "FileDescription" "FidelKey Amharic Keyboard Setup"
VIAddVersionKey "FileVersion" "${VERSION}"
VIAddVersionKey "ProductVersion" "${VERSION}"
VIAddVersionKey "LegalCopyright" "FidelKey contributors"

!define MUI_ABORTWARNING
!define MUI_FINISHPAGE_RUN "$INSTDIR\FidelKey.exe"
!define MUI_FINISHPAGE_RUN_TEXT "Open FidelKey settings"

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

!insertmacro MUI_LANGUAGE "English"

Section "FidelKey" SecMain
  SectionIn RO

  SetOutPath "$INSTDIR"
  SetOverwrite on

  File /oname=FidelKey.exe "${SOURCE_EXE}"
  File /oname=FidelKeyTSF.dll "${SOURCE_DLL}"

  ; The settings app needs the VC++ runtime. The TSF DLL itself is /MT.
  !ifdef SOURCE_VCREDIST
    File /oname=vc_redist.x64.exe "${SOURCE_VCREDIST}"
    ExecWait '"$INSTDIR\vc_redist.x64.exe" /install /quiet /norestart'
    Delete "$INSTDIR\vc_redist.x64.exe"
  !endif

  ; Register the TSF text service (COM + input processor profile).
  ExecWait '"$SYSDIR\regsvr32.exe" /s "$INSTDIR\FidelKeyTSF.dll"' $0
  ${If} $0 != 0
    MessageBox MB_ICONEXCLAMATION|MB_OK "FidelKey was installed, but the keyboard could not be registered (code $0). Try running the installer as administrator."
  ${EndIf}

  WriteUninstaller "$INSTDIR\Uninstall.exe"

  CreateDirectory "$SMPROGRAMS\FidelKey"
  CreateShortcut "$SMPROGRAMS\FidelKey\FidelKey Settings.lnk" "$INSTDIR\FidelKey.exe"
  CreateShortcut "$DESKTOP\FidelKey Settings.lnk" "$INSTDIR\FidelKey.exe"

  WriteRegStr HKLM "Software\FidelKey" "InstallLocation" "$INSTDIR"

  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\FidelKey" "DisplayName" "FidelKey Amharic Keyboard"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\FidelKey" "DisplayVersion" "${VERSION}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\FidelKey" "Publisher" "FidelKey"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\FidelKey" "DisplayIcon" "$INSTDIR\FidelKey.exe"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\FidelKey" "InstallLocation" "$INSTDIR"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\FidelKey" "UninstallString" '"$INSTDIR\Uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\FidelKey" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\FidelKey" "NoRepair" 1
SectionEnd

Section "Uninstall"
  ExecWait '"$SYSDIR\regsvr32.exe" /u /s "$INSTDIR\FidelKeyTSF.dll"'

  Delete "$INSTDIR\FidelKey.exe"
  Delete "$INSTDIR\FidelKeyTSF.dll"
  Delete "$INSTDIR\Uninstall.exe"
  RMDir "$INSTDIR"

  Delete "$SMPROGRAMS\FidelKey\FidelKey Settings.lnk"
  RMDir "$SMPROGRAMS\FidelKey"
  Delete "$DESKTOP\FidelKey Settings.lnk"

  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\FidelKey"
  DeleteRegKey HKLM "Software\FidelKey"
SectionEnd
