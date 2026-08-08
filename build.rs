fn main() {
    println!(
        "cargo:rustc-env=SLATE_BUILD_TARGET={}",
        std::env::var("TARGET").expect("Cargo did not set TARGET")
    );
}
