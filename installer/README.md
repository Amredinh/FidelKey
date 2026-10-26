# FidelKey Installer Configuration

## Requirements
- Install native TSF service DLL
- Install settings application
- Register TSF service with Windows
- Create Start Menu shortcut
- Support uninstallation

## Options
1. **WiX Toolset** - Most flexible for Windows desktop apps
2. **MSIX** - Modern, sandboxed, but has limitations for TSF registration
3. **Inno Setup** - Simple, script-based

## Recommended: WiX Toolset
- Better control over registration
- Supports native DLL registration
- Creates proper Windows Installer

## Installation Steps
1. Copy TSF service DLL to system directory
2. Register DLL with regsvr32
3. Copy settings app to Program Files
4. Create Start Menu shortcut
5. Add to Add/Remove Programs

## Uninstallation Steps
1. Unregister DLL with regsvr32 /u
2. Remove files
3. Remove shortcuts
4. Clean up registry

## Windows Requirements
- Windows 10 1903+ or Windows 11
- .NET Framework not required
- Visual C++ Redistributable may be needed

## Distribution
- GitHub Releases
- Microsoft Store (future)
