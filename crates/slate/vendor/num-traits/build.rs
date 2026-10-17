fn main() {
    let ac = autocfg::new();

    ac.emit_expression_cfg("1f64.total_cmp(&2f64)", "has_total_cmp"); // 1.62

    autocfg::emit_possibility("has_f16");
    if ac
        .probe_raw("#![feature(f16)]\npub fn f(x: f16) -> f16 { x + x }")
        .is_ok()
    {
        autocfg::emit("has_f16");
    }

    autocfg::rerun_path("build.rs");
}
