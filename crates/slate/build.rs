use std::fmt::Write as _;
use std::path::{Path, PathBuf};

fn vendored_files(dir: &Path, files: &mut Vec<PathBuf>) {
    for entry in std::fs::read_dir(dir).expect("read vendored crate") {
        let path = entry.expect("read vendored entry").path();
        if path.is_dir() {
            vendored_files(&path, files);
        } else {
            files.push(path);
        }
    }
}

fn write_vendored_crate(name: &str, constant: &str) {
    let manifest = PathBuf::from(std::env::var("CARGO_MANIFEST_DIR").expect("manifest dir"));
    let root = manifest.join("vendor").join(name);
    println!("cargo:rerun-if-changed={}", root.display());
    let mut files = Vec::new();
    vendored_files(&root, &mut files);
    files.sort();
    let mut table = format!("const {constant}: &[(&str, &str)] = &[\n");
    for file in &files {
        let relative = file
            .strip_prefix(&root)
            .expect("vendored file under root")
            .display()
            .to_string()
            .replace("Cargo.toml.template", "Cargo.toml");
        writeln!(
            table,
            "    ({relative:?}, include_str!({:?})),",
            file.display().to_string()
        )
        .expect("write table");
    }
    table.push_str("];\n");
    let out = PathBuf::from(std::env::var("OUT_DIR").expect("out dir"));
    std::fs::write(out.join(format!("vendored_{name}.rs")), table).expect("write vendored table");
}

fn main() {
    println!("cargo:rerun-if-changed=build.rs");
    println!(
        "cargo:rustc-env=SLATE_BUILD_TARGET={}",
        std::env::var("TARGET").expect("Cargo did not set TARGET")
    );
    write_vendored_crate("libc", "VENDORED_LIBC");
    write_vendored_crate("mach2", "VENDORED_MACH2");
}
