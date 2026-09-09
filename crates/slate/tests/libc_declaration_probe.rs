mod support;

use std::path::Path;

use support::libc_declaration_probe::{
    compile_and_link_oracle_probe, compile_and_link_shim_probe, diff_header_files,
    extract_oracle_function, extract_oracle_header_files, extract_oracle_header_functions,
    extract_oracle_header_macros, extract_oracle_header_objects, extract_oracle_type_surface,
    extract_shim_header_files, extract_shim_header_functions,
    select_oracle_object_macro_value_probes, write_header_macro_presence_probe,
    write_header_object_macro_value_probe, write_header_shim_probe, write_object_macro_value_probe,
    write_object_probe, write_oracle_declarations, write_oracle_macro_manifest,
    write_oracle_type_manifest, write_shim_probe, write_type_surface_probe,
};
use support::libc_probe::{arch_from_key, resolve};
use support::libc_shim::{Architecture, LibcVariant};

fn selected_libc() -> LibcVariant {
    match std::env::var("SLATE_LIBC_DECL_LIBC").as_deref() {
        Ok("glibc") | Err(_) => LibcVariant::Glibc,
        Ok("musl") => LibcVariant::Musl,
        Ok(other) => panic!("unknown SLATE_LIBC_DECL_LIBC value: {other}"),
    }
}

fn selected_config() -> (LibcVariant, support::libc_probe::ProbeConfig) {
    let libc = selected_libc();
    let arch_name = std::env::var("SLATE_LIBC_DECL_ARCH").unwrap_or_else(|_| "x86_64".into());
    let arch = arch_from_key(&arch_name)
        .unwrap_or_else(|| panic!("unknown SLATE_LIBC_DECL_ARCH value: {arch_name}"));
    let config = resolve(arch, libc).expect("resolve target oracle");
    (libc, config)
}

fn selected_header() -> String {
    std::env::var("SLATE_LIBC_DECL_HEADER").unwrap_or_else(|_| "fcntl.h".into())
}

fn header_output(
    libc: LibcVariant,
    config: &support::libc_probe::ProbeConfig,
    header: &str,
) -> std::path::PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .join("target/libc-declaration-probe")
        .join(format!(
            "{}-{}-{}",
            libc.name(),
            config.label.replace('/', "-"),
            header.replace(['/', '.'], "_")
        ))
}

#[test]
#[ignore = "manual header-by-header declaration probe"]
fn emit_oracle_derived_shim_probe() {
    let (libc, config) = selected_config();
    let header = selected_header();
    let symbol = std::env::var("SLATE_LIBC_DECL_SYMBOL").unwrap_or_else(|_| "open".into());
    let output = Path::new(env!("CARGO_MANIFEST_DIR"))
        .join("target/libc-declaration-probe")
        .join(format!("{}-{}", libc.name(), symbol));
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

#[test]
#[ignore = "manual oracle header declaration inventory"]
fn emit_oracle_header_declarations() {
    let (libc, config) = selected_config();
    let header = selected_header();
    let output = header_output(libc, &config, &header);
    let functions = extract_oracle_header_functions(&config, &header, &output)
        .expect("extract oracle header declarations");
    let declarations =
        write_oracle_declarations(&functions, &output).expect("write oracle header declarations");

    println!("oracle header: {header}");
    println!("functions: {}", functions.len());
    println!("generated: {}", declarations.display());
}

#[test]
#[ignore = "manual oracle header shim compliance probe"]
fn verify_oracle_header_shim_compliance() {
    let (libc, config) = selected_config();
    let header = selected_header();
    let output = header_output(libc, &config, &header);
    let functions = extract_oracle_header_functions(&config, &header, &output)
        .expect("extract oracle header declarations");
    let declarations =
        write_oracle_declarations(&functions, &output).expect("write oracle header declarations");
    let probe =
        write_header_shim_probe(&functions, &output).expect("write shim header compliance probe");

    println!("oracle declarations: {}", declarations.display());
    println!("generated: {}", probe.source.display());
    compile_and_link_shim_probe(&config, &probe)
        .expect("compile and link shim header compliance probe");
}

#[test]
fn strings_header_declarations_match_glibc_oracle() {
    let libc = LibcVariant::Glibc;
    let header = "strings.h";
    let config = resolve(Architecture::X86_64, libc).expect("resolve glibc x86_64 oracle");
    let output = header_output(libc, &config, header);
    let functions = extract_oracle_header_functions(&config, header, &output)
        .expect("extract strings.h oracle declarations");
    assert!(
        !functions.is_empty(),
        "strings.h oracle exposed no functions"
    );
    write_oracle_declarations(&functions, &output).expect("write strings.h oracle declarations");
    let probe = write_header_shim_probe(&functions, &output)
        .expect("write strings.h shim compliance probe");
    compile_and_link_shim_probe(&config, &probe)
        .expect("compile and link strings.h shim compliance probe");
}

#[test]
#[ignore = "manual oracle header macro inventory"]
fn emit_oracle_header_macros() {
    let (libc, config) = selected_config();
    let header = selected_header();
    let output = header_output(libc, &config, &header);
    let macros = extract_oracle_header_macros(&config, &header, &output)
        .expect("extract oracle header macros");
    let manifest =
        write_oracle_macro_manifest(&macros, &output).expect("write oracle macro manifest");

    println!("oracle header: {header}");
    println!("macros: {}", macros.len());
    println!("generated: {}", manifest.display());
}

#[test]
#[ignore = "manual oracle header macro presence probe"]
fn verify_oracle_header_macro_presence() {
    let (libc, config) = selected_config();
    let header = selected_header();
    let output = header_output(libc, &config, &header);
    let macros = extract_oracle_header_macros(&config, &header, &output)
        .expect("extract oracle header macros");
    let manifest =
        write_oracle_macro_manifest(&macros, &output).expect("write oracle macro manifest");
    let probe = write_header_macro_presence_probe(&macros, &output)
        .expect("write shim macro presence probe");

    println!("oracle macros: {}", manifest.display());
    println!("generated: {}", probe.source.display());
    compile_and_link_shim_probe(&config, &probe)
        .expect("compile and link shim macro presence probe");
}

#[test]
#[ignore = "manual oracle header macro value probe"]
fn verify_oracle_header_macro_values() {
    let (libc, config) = selected_config();
    let header = selected_header();
    let output = header_output(libc, &config, &header);
    let macros = extract_oracle_header_macros(&config, &header, &output)
        .expect("extract oracle header macros");
    let selected = select_oracle_object_macro_value_probes(&config, &macros, &output)
        .expect("classify oracle object macro value probes");
    let probe = write_header_object_macro_value_probe(&selected, &output)
        .expect("write shim macro value probe");

    println!("value-checked macros: {}", selected.len());
    println!("generated: {}", probe.source.display());
    compile_and_link_shim_probe(&config, &probe).expect("verify shim macro types and values");
}

#[test]
fn fcntl_header_macros_are_collected_from_glibc_oracle() {
    let libc = LibcVariant::Glibc;
    let header = "fcntl.h";
    let config = resolve(Architecture::X86_64, libc).expect("resolve glibc x86_64 oracle");
    let output = header_output(libc, &config, header);
    let macros =
        extract_oracle_header_macros(&config, header, &output).expect("extract fcntl.h macros");
    let manifest =
        write_oracle_macro_manifest(&macros, &output).expect("write fcntl.h oracle macro manifest");
    assert!(manifest.is_file(), "macro manifest was not written");
    let o_rdonly = macros
        .iter()
        .find(|macro_definition| macro_definition.name == "O_RDONLY")
        .expect("fcntl.h oracle did not expose O_RDONLY");
    assert!(matches!(
        o_rdonly.kind,
        support::libc_declaration_probe::MacroKind::ObjectLike
    ));
    assert_eq!(o_rdonly.replacement, "00");
    assert!(!o_rdonly.private);
    assert!(o_rdonly.definition_file.ends_with("fcntl-linux.h"));
    let probe =
        write_object_macro_value_probe(o_rdonly, &output).expect("write O_RDONLY value probe");
    compile_and_link_shim_probe(&config, &probe).expect("verify O_RDONLY macro type and value");
}

#[test]
fn fcntl_header_type_surface_is_collected_from_glibc_oracle() {
    let libc = LibcVariant::Glibc;
    let header = "fcntl.h";
    let config = resolve(Architecture::X86_64, libc).expect("resolve glibc x86_64 oracle");
    let output = header_output(libc, &config, header);
    let surface = extract_oracle_type_surface(&config, header, &output)
        .expect("extract fcntl.h type surface");
    let manifest =
        write_oracle_type_manifest(&surface, &output).expect("write fcntl.h type manifest");
    assert!(manifest.is_file(), "type manifest was not written");
    assert!(
        surface
            .typedefs
            .iter()
            .any(|typedef| typedef.name == "mode_t")
    );
    let flock = surface
        .records
        .iter()
        .find(|record| record.tag == "flock")
        .expect("fcntl.h oracle did not expose struct flock");
    assert!(flock.fields.iter().any(|field| field.name == "l_type"));
    assert!(
        surface
            .enums
            .iter()
            .any(|enumeration| enumeration.tag == "__pid_type")
    );
    let probe = write_type_surface_probe(header, &surface, &output)
        .expect("write fcntl.h type surface probe");
    assert!(probe.source.is_file(), "type surface probe was not written");
}

#[test]
fn unistd_environ_object_matches_glibc_oracle() {
    let libc = LibcVariant::Glibc;
    let config = resolve(Architecture::X86_64, libc).expect("resolve glibc x86_64 oracle");
    let output = header_output(libc, &config, "unistd.h");
    let environ = extract_oracle_header_objects(&config, "unistd.h", &output)
        .expect("extract unistd.h objects")
        .into_iter()
        .find(|object| object.name == "environ")
        .expect("oracle environ object");
    assert_eq!(environ.type_spelling, "char **");
    let probe = write_object_probe(&environ, &output).expect("write environ probe");
    compile_and_link_shim_probe(&config, &probe).expect("verify environ object");
}

#[test]
#[ignore = "manual oracle type surface shim probe"]
fn verify_oracle_header_type_surface() {
    let (libc, config) = selected_config();
    let header = selected_header();
    let output = header_output(libc, &config, &header);
    let surface = extract_oracle_type_surface(&config, &header, &output)
        .expect("extract oracle type surface");
    let probe = write_type_surface_probe(&header, &surface, &output)
        .expect("write shim type surface probe");
    println!("generated: {}", probe.source.display());
    compile_and_link_shim_probe(&config, &probe).expect("verify shim type surface");
}

#[test]
#[ignore = "manual bidirectional header-file diff probe"]
fn diff_shim_header_files_against_oracle() {
    let (libc, config) = selected_config();
    let header = selected_header();
    let output = header_output(libc, &config, &header);
    let oracle_files = extract_oracle_header_files(&config, &header, &output.join("oracle-files"))
        .expect("extract oracle header files");
    let shim_files = extract_shim_header_files(&config, &header, &output.join("shim-files"))
        .expect("extract shim header files");
    let diff = diff_header_files(&oracle_files, &shim_files);

    println!("oracle files: {oracle_files:?}");
    println!("shim files: {shim_files:?}");
    println!("extra in shim: {:?}", diff.extra_in_shim);
    println!("missing from shim: {:?}", diff.missing_from_shim);
}

#[test]
#[ignore = "manual reverse-direction shim-to-oracle probe"]
fn verify_shim_header_declarations_against_oracle() {
    let (libc, config) = selected_config();
    let header = selected_header();
    let output = header_output(libc, &config, &header);
    let functions = extract_shim_header_functions(&config, &header, &output)
        .expect("extract shim header declarations");
    let probe =
        write_header_shim_probe(&functions, &output).expect("write reverse shim compliance probe");

    println!("shim declarations: {}", functions.len());
    println!("generated: {}", probe.source.display());
    compile_and_link_oracle_probe(&config, &probe)
        .expect("verify shim declarations against the real oracle");
}
