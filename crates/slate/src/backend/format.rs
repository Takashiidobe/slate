use std::io::Write;
use std::path::Path;
use std::process::{Command, Stdio};

pub fn format_rust(source: &str) -> Result<String, String> {
    let rustfmt = std::env::var("SLATE_RUSTFMT").unwrap_or_else(|_| "rustfmt".into());
    let mut child = Command::new(&rustfmt)
        .args(["--emit", "stdout", "--edition", "2024"])
        .stdin(Stdio::piped())
        .stdout(Stdio::piped())
        .stderr(Stdio::piped())
        .spawn()
        .map_err(|error| format!("spawn {rustfmt}: {error}"))?;
    let mut stdin = child.stdin.take().expect("rustfmt stdin is piped");
    stdin
        .write_all(source.as_bytes())
        .map_err(|error| format!("write {rustfmt} stdin: {error}"))?;
    drop(stdin);
    let output = child
        .wait_with_output()
        .map_err(|error| format!("wait for {rustfmt}: {error}"))?;
    if !output.status.success() {
        return Err(format!(
            "{rustfmt} failed:\n{}",
            String::from_utf8_lossy(&output.stderr)
        ));
    }
    String::from_utf8(output.stdout).map_err(|error| format!("decode {rustfmt} output: {error}"))
}

pub fn write_rust(path: &Path, source: &str) -> Result<(), String> {
    let formatted = format_rust(source)?;
    std::fs::write(path, formatted).map_err(|error| format!("write {}: {error}", path.display()))
}
