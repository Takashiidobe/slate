fn main() {
    println!("cargo:rerun-if-changed=c");
    let vendor = std::env::var("CARGO_CFG_TARGET_VENDOR").unwrap_or_default();
    let env = std::env::var("CARGO_CFG_TARGET_ENV").unwrap_or_default();
    if vendor == "apple" || env == "msvc" {
        return;
    }
    cc::Build::new()
        .file("c/runtime.c")
        .file("c/data.c")
        .include("c")
        .warnings(false)
        .compile("BlocksRuntime");
}
