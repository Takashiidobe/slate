use std::path::Path;
use std::process::Command;

fn matrix_script() -> std::path::PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR")).join("tools/libc-abi-matrix.py")
}

fn append_selectors(command: &mut Command, flag: &str, env_var: &str) {
    if let Ok(values) = std::env::var(env_var) {
        for value in values.split(',').filter(|value| !value.is_empty()) {
            command.arg(format!("{flag}={value}"));
        }
    }
}

#[test]
fn libc_abi_matrix() {
    let mut command = Command::new("python3");
    command.arg(matrix_script());
    if let Ok(libc) = std::env::var("SLATE_LIBC_ABI_LIBC") {
        command.arg(format!("--libc={libc}"));
    }
    append_selectors(&mut command, "--arch", "SLATE_LIBC_ABI_ARCH");
    append_selectors(&mut command, "--family", "SLATE_LIBC_ABI_FAMILY");

    let output = command.output().expect("run tools/libc-abi-matrix.py");
    if !output.status.success() {
        panic!(
            "libc ABI oracle matrix failed ({}):\n--- stdout ---\n{}\n--- stderr ---\n{}",
            output.status,
            String::from_utf8_lossy(&output.stdout),
            String::from_utf8_lossy(&output.stderr),
        );
    }
}
