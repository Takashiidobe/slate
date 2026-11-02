use std::path::{Path, PathBuf};
use std::process::Command;

fn fixture() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR")).join("tests/fixtures/libc_record_target.c")
}

fn slate() -> Command {
    let mut command = Command::new(env!("CARGO_BIN_EXE_slate"));
    command
        .env("SLATE_SYSROOTS", slate_parser::sysroot::root())
        .env_remove("SLATE_TARGET")
        .env_remove("SLATE_CLANG_ARGS");
    command
}

#[test]
fn libc_records_follow_translation_target() {
    for (target, mapped) in [
        ("x86_64-unknown-linux-gnu", true),
        ("aarch64-apple-darwin", true),
        ("x86_64-apple-darwin", false),
        ("x86_64-unknown-freebsd", false),
        ("armv7-unknown-linux-gnueabi", false),
    ] {
        let output = slate()
            .arg("translate")
            .arg(format!("-target={target}"))
            .arg(fixture())
            .output()
            .expect("translate record fixture");
        assert!(
            output.status.success(),
            "{target}: {}",
            String::from_utf8_lossy(&output.stderr)
        );
        let source = String::from_utf8(output.stdout).expect("Rust output");
        assert_eq!(
            source.contains("slate_support::dirent"),
            mapped,
            "{target}: {source}"
        );
        assert_eq!(
            source.contains("struct dirent {"),
            !mapped,
            "{target}: {source}"
        );
    }
}

#[test]
fn generated_support_defaults_match_abi_baselines() {
    let work = tempfile::tempdir().expect("test directory");
    std::fs::copy(fixture(), work.path().join("main.c")).expect("copy fixture");
    let matrix = work.path().join("matrix.json");
    for target in [
        "x86_64-unknown-freebsd",
        "aarch64-unknown-freebsd",
        "i686-unknown-linux-gnu",
        "armv7-unknown-linux-gnueabihf",
    ] {
        std::fs::write(
            &matrix,
            serde_json::json!({
                "targets": [{"name": "freebsd", "triple": target, "flavor": "clang", "args": if target.contains("linux") { vec!["-D_FILE_OFFSET_BITS=64", "-D_TIME_BITS=64"] } else { vec![] }}],
                "sources": ["main.c"]
            })
            .to_string(),
        )
        .expect("write matrix");
        let project = work.path().join(target);
        let output = slate()
            .args(["translate-project", "--matrix"])
            .arg(&matrix)
            .args(["--matrix-target", "freebsd"])
            .arg(work.path())
            .arg(&project)
            .output()
            .expect("translate FreeBSD project");
        assert!(
            output.status.success(),
            "{}",
            String::from_utf8_lossy(&output.stderr)
        );
        let root = project.join("src/main.rs");
        let mut source = std::fs::read_to_string(&root).expect("generated root");
        if target.ends_with("freebsd") {
            source.push_str("\nconst _: () = { assert!(core::mem::size_of::<slate_support::cpuset_t>() == 128); assert!(slate_support::CPU_SETSIZE == 1024); assert!(core::mem::size_of::<slate_support::tcp_info>() == 256); assert!(core::mem::offset_of!(slate_support::tcp_info, tcpi_total_tlp_bytes) == 168); };\n");
        } else {
            source.push_str("\nconst _: () = { assert!(core::mem::size_of::<slate_support::off_t>() == 8); assert!(core::mem::size_of::<slate_support::time_t>() == 8); };\n");
        }
        std::fs::write(root, source).expect("append ABI assertions");
        let output =
            Command::new(std::env::var_os("SLATE_CARGO").unwrap_or_else(|| "cargo".into()))
                .current_dir(Path::new(env!("CARGO_MANIFEST_DIR")).join("../.."))
                .args(["check", "--quiet", "--target", target, "--manifest-path"])
                .arg(project.join("Cargo.toml"))
                .env("CARGO_TARGET_DIR", work.path().join("cargo-target"))
                .env("RUSTFLAGS", "")
                .env_remove("CARGO_ENCODED_RUSTFLAGS")
                .output()
                .expect("check generated support");
        assert!(
            output.status.success(),
            "{target}: {}",
            String::from_utf8_lossy(&output.stderr)
        );
    }
}
