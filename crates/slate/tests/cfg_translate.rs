mod support;

use std::path::{Path, PathBuf};
use std::process::Command;

fn cfg_fixtures_dir() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR")).join("tests/fixtures.cfg")
}

fn translate_cfg(name: &str) -> String {
    let src = cfg_fixtures_dir().join(name);
    let out = Command::new(env!("CARGO_BIN_EXE_slate"))
        .arg("translate-cfg")
        .arg(&src)
        .output()
        .expect("run slate translate-cfg");
    assert!(
        out.status.success(),
        "translate-cfg failed for {name}:\n{}",
        String::from_utf8_lossy(&out.stderr)
    );
    String::from_utf8(out.stdout).expect("generated Rust is utf8")
}

fn write_generated(name: &str, rust: &str) -> PathBuf {
    let out_dir = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/cfg-translate-generated");
    std::fs::create_dir_all(&out_dir).expect("create cfg translate output dir");
    let out = out_dir.join(name).with_extension("rs");
    std::fs::write(&out, rust).expect("write generated cfg Rust");
    out
}

#[test]
fn translates_os_macro_variants_to_cfg_items() {
    let rust = translate_cfg("os_targets.c");

    assert!(rust.contains("#[cfg(windows)]\nfn os_code() -> i32"));
    assert!(rust.contains("#[cfg(target_os = \"linux\")]\nfn os_code() -> i32"));
    assert!(rust.contains("#[cfg(target_vendor = \"apple\")]\nfn os_code() -> i32"));
    assert!(rust.contains(
        "#[cfg(not(any(windows, target_os = \"linux\", target_vendor = \"apple\")))]\nfn os_code() -> i32"
    ));
    assert!(rust.contains("i32 = 10;"));
    assert!(rust.contains("i32 = 20;"));
    assert!(rust.contains("i32 = 30;"));
    assert!(rust.contains("i32 = 40;"));
    assert_eq!(rust.matches("fn main()").count(), 1);
}

#[test]
fn translates_arch_macro_variants_to_cfg_items() {
    let rust = translate_cfg("arch_targets.c");

    assert!(rust.contains("#[cfg(target_arch = \"x86_64\")]\nfn arch_code() -> i32"));
    assert!(rust.contains("#[cfg(target_arch = \"aarch64\")]\nfn arch_code() -> i32"));
    assert!(rust.contains("#[cfg(target_arch = \"arm\")]\nfn arch_code() -> i32"));
    assert!(rust.contains(
        "#[cfg(not(any(target_arch = \"x86_64\", target_arch = \"aarch64\", target_arch = \"arm\")))]\nfn arch_code() -> i32"
    ));
    assert!(rust.contains("i32 = 64;"));
    assert!(rust.contains("i32 = 128;"));
    assert!(rust.contains("i32 = 32;"));
    assert!(rust.contains("i32 = 0;"));
    assert_eq!(rust.matches("fn main()").count(), 1);
}

#[test]
fn translates_ndebug_variants_to_debug_assertion_cfg_items() {
    let rust = translate_cfg("ndebug.c");

    assert!(rust.contains("#[cfg(not(debug_assertions))]\nfn debug_code() -> i32"));
    assert!(rust.contains("#[cfg(debug_assertions)]\nfn debug_code() -> i32"));
    assert!(rust.contains("i32 = 0;"));
    assert!(rust.contains("i32 = 1;"));
    assert_eq!(rust.matches("fn main()").count(), 1);
}

#[test]
fn cfg_translated_fixtures_compile_for_current_host() {
    let work_dir = Path::new(env!("CARGO_MANIFEST_DIR")).join("target/cfg-translate-compile");
    for name in ["os_targets.c", "arch_targets.c", "ndebug.c"] {
        let rust = translate_cfg(name);
        let rs = write_generated(name, &rust);
        let package = format!("cfg_{}", name.trim_end_matches(".c"));
        support::compile_rs_cargo(&rs, &work_dir, &package)
            .unwrap_or_else(|err| panic!("generated cfg Rust did not compile for {name}:\n{err}"));
    }
}
