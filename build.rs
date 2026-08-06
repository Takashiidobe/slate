fn main() {
    println!(
        "cargo:rustc-env=SLATE_TARGET_ENV={}",
        std::env::var("CARGO_CFG_TARGET_ENV").unwrap_or_default()
    );
    println!(
        "cargo:rustc-env=SLATE_TARGET_ARCH={}",
        std::env::var("CARGO_CFG_TARGET_ARCH").unwrap_or_default()
    );
    println!(
        "cargo:rustc-env=SLATE_TARGET_ENDIAN={}",
        std::env::var("CARGO_CFG_TARGET_ENDIAN").unwrap_or_default()
    );
}
