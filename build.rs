fn main() {
    println!(
        "cargo:rustc-env=SLATE_TARGET_ENV={}",
        std::env::var("CARGO_CFG_TARGET_ENV").unwrap_or_default()
    );
}
