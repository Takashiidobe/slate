fn main() {
    let ac = autocfg::new();

    autocfg::emit_possibility("has_f16");
    if ac
        .probe_raw("#![feature(f16)]\npub fn f(x: f16) -> f16 { x + x }")
        .is_ok()
    {
        autocfg::emit("has_f16");
    }

    autocfg::emit_possibility("has_f128");
    if ac
        .probe_raw("#![feature(f128)]\npub fn f(x: f128) -> f128 { x + x }")
        .is_ok()
    {
        autocfg::emit("has_f128");
    }

    autocfg::rerun_path("build.rs");
}
