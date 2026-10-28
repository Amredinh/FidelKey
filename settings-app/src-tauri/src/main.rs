#![cfg_attr(not(debug_assertions), windows_subsystem = "windows")]

use serde::{Deserialize, Serialize};
use std::env;
use std::fs;
use std::path::{Path, PathBuf};
use std::process::Command;

const TSF_DLL: &str = "FidelKeyTSF.dll";
const APP_EXE: &str = "FidelKey.exe";

#[derive(Serialize, Deserialize, Clone)]
#[serde(default)]
struct Settings {
    layout: String,
    show_typing_test: bool,
}

impl Default for Settings {
    fn default() -> Self {
        Self {
            layout: "phonetic".to_string(),
            show_typing_test: true,
        }
    }
}

fn settings_path() -> PathBuf {
    let base = std::env::var("APPDATA").unwrap_or_else(|_| ".".to_string());
    PathBuf::from(base).join("FidelKey").join("settings.json")
}

#[tauri::command]
fn load_settings() -> Result<Settings, String> {
    let path = settings_path();
    if !path.exists() {
        return Ok(Settings::default());
    }
    let contents = fs::read_to_string(&path).map_err(|e| e.to_string())?;
    serde_json::from_str(&contents).map_err(|e| e.to_string())
}

#[tauri::command]
fn save_settings(settings: Settings) -> Result<(), String> {
    let path = settings_path();
    if let Some(parent) = path.parent() {
        fs::create_dir_all(parent).map_err(|e| e.to_string())?;
    }
    let contents = serde_json::to_string_pretty(&settings).map_err(|e| e.to_string())?;
    fs::write(&path, contents).map_err(|e| e.to_string())
}

#[tauri::command]
fn open_language_settings() -> Result<(), String> {
    std::process::Command::new("cmd")
        .args(["/C", "start", "", "ms-settings:regionlanguage"])
        .spawn()
        .map(|_| ())
        .map_err(|e| e.to_string())
}

#[derive(Serialize)]
struct KeyboardStatus {
    installed: bool,
    packaged: bool,
}

fn install_dir() -> PathBuf {
    let base = env::var("ProgramFiles").unwrap_or_else(|_| r"C:\Program Files".to_string());
    PathBuf::from(base).join("FidelKey")
}

fn exe_dir() -> Option<PathBuf> {
    env::current_exe()
        .ok()
        .and_then(|path| path.parent().map(Path::to_path_buf))
}

fn is_packaged() -> bool {
    env::current_exe()
        .map(|path| {
            path.to_string_lossy()
                .to_ascii_lowercase()
                .contains("\\windowsapps\\")
        })
        .unwrap_or(false)
}

fn keyboard_installed() -> bool {
    if is_packaged() {
        install_dir().join(TSF_DLL).is_file()
    } else {
        exe_dir()
            .map(|dir| dir.join(TSF_DLL).is_file())
            .unwrap_or(false)
    }
}

fn ps_quote(path: &Path) -> String {
    format!("'{}'", path.to_string_lossy().replace('\'', "''"))
}

fn log_error(message: &str) {
    let path = env::temp_dir().join("FidelKey-tsf-error.txt");
    let _ = fs::write(path, message);
}

fn run_elevated(arg: &str) -> Result<(), String> {
    let exe = env::current_exe().map_err(|e| e.to_string())?;
    let script = format!(
        "$p = Start-Process -FilePath {} -ArgumentList '{}' -Verb RunAs -Wait -PassThru; exit $p.ExitCode",
        ps_quote(&exe),
        arg
    );
    let status = Command::new("powershell.exe")
        .args([
            "-NoProfile",
            "-NonInteractive",
            "-ExecutionPolicy",
            "Bypass",
            "-Command",
            &script,
        ])
        .status()
        .map_err(|e| format!("Could not start the Windows elevation prompt: {e}"))?;
    if !status.success() {
        return Err(format!(
            "Administrator approval was cancelled or the keyboard operation failed (code {}).",
            status.code().unwrap_or(-1)
        ));
    }
    Ok(())
}

fn install_tsf() -> Result<(), String> {
    let src_dir = exe_dir().ok_or("Could not locate the application directory.")?;
    let dll_src = src_dir.join(TSF_DLL);
    if !dll_src.is_file() {
        return Err(format!("The package is missing {TSF_DLL}."));
    }

    let dst_dir = install_dir();
    fs::create_dir_all(&dst_dir)
        .map_err(|e| format!("Could not create {}: {e}", dst_dir.display()))?;
    fs::copy(&dll_src, dst_dir.join(TSF_DLL))
        .map_err(|e| format!("Could not copy the keyboard DLL: {e}"))?;

    let exe_src = env::current_exe().map_err(|e| e.to_string())?;
    fs::copy(&exe_src, dst_dir.join(APP_EXE))
        .map_err(|e| format!("Could not copy the FidelKey executable: {e}"))?;

    let windir = env::var("WINDIR").unwrap_or_else(|_| r"C:\Windows".to_string());
    let regsvr32 = PathBuf::from(windir)
        .join("System32")
        .join("regsvr32.exe");
    let status = Command::new(&regsvr32)
        .arg("/s")
        .arg(dst_dir.join(TSF_DLL))
        .status()
        .map_err(|e| format!("Could not run regsvr32.exe: {e}"))?;
    if !status.success() {
        return Err(format!(
            "Windows could not register the keyboard (regsvr32 code {}).",
            status.code().unwrap_or(-1)
        ));
    }
    Ok(())
}

fn uninstall_tsf() -> Result<(), String> {
    let dst_dir = install_dir();
    let dll = dst_dir.join(TSF_DLL);
    if dll.is_file() {
        let windir = env::var("WINDIR").unwrap_or_else(|_| r"C:\Windows".to_string());
        let regsvr32 = PathBuf::from(windir)
            .join("System32")
            .join("regsvr32.exe");
        let status = Command::new(&regsvr32)
            .args(["/u", "/s"])
            .arg(&dll)
            .status()
            .map_err(|e| format!("Could not run regsvr32.exe: {e}"))?;
        if !status.success() {
            return Err(format!(
                "Windows could not unregister the keyboard (regsvr32 code {}).",
                status.code().unwrap_or(-1)
            ));
        }
    }

    let _ = fs::remove_file(dst_dir.join(APP_EXE));
    let _ = fs::remove_file(&dll);
    let _ = fs::remove_dir(&dst_dir);
    Ok(())
}

#[tauri::command]
fn keyboard_status() -> KeyboardStatus {
    KeyboardStatus {
        installed: keyboard_installed(),
        packaged: is_packaged(),
    }
}

#[tauri::command]
fn install_keyboard() -> Result<(), String> {
    if keyboard_installed() {
        return Ok(());
    }
    if !is_packaged() {
        return Err("Run the FidelKey installer to add the keyboard.".to_string());
    }
    run_elevated("--install-tsf")?;
    if !keyboard_installed() {
        return Err("The keyboard files were not found after installation.".to_string());
    }
    Ok(())
}

#[tauri::command]
fn uninstall_keyboard() -> Result<(), String> {
    if !is_packaged() {
        return Err("Use the FidelKey uninstaller to remove the keyboard.".to_string());
    }
    if !keyboard_installed() {
        return Ok(());
    }
    run_elevated("--uninstall-tsf")?;
    if keyboard_installed() {
        return Err("The keyboard files are still present.".to_string());
    }
    Ok(())
}

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.iter().any(|arg| arg == "--install-tsf") {
        if let Err(error) = install_tsf() {
            log_error(&error);
            std::process::exit(1);
        }
        std::process::exit(0);
    }
    if args.iter().any(|arg| arg == "--uninstall-tsf") {
        if let Err(error) = uninstall_tsf() {
            log_error(&error);
            std::process::exit(1);
        }
        std::process::exit(0);
    }

    tauri::Builder::default()
        .invoke_handler(tauri::generate_handler![
            load_settings,
            save_settings,
            open_language_settings,
            keyboard_status,
            install_keyboard,
            uninstall_keyboard
        ])
        .run(tauri::generate_context!())
        .expect("error while running FidelKey");
}
