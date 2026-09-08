mod support;

use std::path::Path;

use support::libc_declaration_probe::{
    compile_and_link_shim_probe, extract_oracle_function, write_shim_probe,
};
use support::libc_probe::{arch_from_key, arch_key, resolve};
use support::libc_shim::LibcVariant;

fn selected_libc() -> LibcVariant {
    match std::env::var("SLATE_LIBC_DECL_LIBC").as_deref() {
        Ok("glibc") | Err(_) => LibcVariant::Glibc,
        Ok("musl") => LibcVariant::Musl,
        Ok(other) => panic!("unknown SLATE_LIBC_DECL_LIBC value: {other}"),
    }
}

#[test]
#[ignore = "manual header-by-header declaration probe"]
fn emit_oracle_derived_shim_probe() {
    let libc = selected_libc();
    let arch_name = std::env::var("SLATE_LIBC_DECL_ARCH").unwrap_or_else(|_| "x86_64".into());
    let arch = arch_from_key(&arch_name)
        .unwrap_or_else(|| panic!("unknown SLATE_LIBC_DECL_ARCH value: {arch_name}"));
    let header = std::env::var("SLATE_LIBC_DECL_HEADER").unwrap_or_else(|_| "fcntl.h".into());
    let symbol = std::env::var("SLATE_LIBC_DECL_SYMBOL").unwrap_or_else(|_| "open".into());
    let config = resolve(arch, libc).expect("resolve target oracle");
    let output = Path::new(env!("CARGO_MANIFEST_DIR"))
        .join("target/libc-declaration-probe")
        .join(format!("{}-{}-{}", libc.name(), arch_key(arch), symbol));
    let function = extract_oracle_function(&config, &header, &symbol, &output)
        .expect("extract oracle declaration");
    let probe = write_shim_probe(&function, &output).expect("write shim declaration probe");

    println!("oracle: {function:#?}");
    println!("generated: {}", probe.source.display());
    println!(
        "{}",
        std::fs::read_to_string(&probe.source).expect("read generated shim declaration probe")
    );

    compile_and_link_shim_probe(&config, &probe).expect("compile and link shim declaration probe");
}
