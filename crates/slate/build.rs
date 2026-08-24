fn main() {
    enforce_typed_lowering_boundary();
    println!(
        "cargo:rustc-env=SLATE_BUILD_TARGET={}",
        std::env::var("TARGET").expect("Cargo did not set TARGET")
    );
}

fn enforce_typed_lowering_boundary() {
    let mut paths = vec![std::path::PathBuf::from("src/frontend/lowerer.rs")];
    paths.extend(
        std::fs::read_dir("src/frontend/lowerer")
            .expect("read frontend lowerer directory")
            .filter_map(Result::ok)
            .map(|entry| entry.path())
            .filter(|path| path.extension().is_some_and(|extension| extension == "rs")),
    );
    for path in paths {
        println!("cargo:rerun-if-changed={}", path.display());
        let source = std::fs::read_to_string(&path).expect("read frontend lowerer source");
        for forbidden in ["Operation", "GenericModule", ".generic", ".mnemonic("] {
            assert!(
                !source.contains(forbidden),
                "{} crosses the typed clang-ir boundary with `{forbidden}`",
                path.display()
            );
        }
    }
}
