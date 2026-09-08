mod support;

use std::path::{Path, PathBuf};

use support::libc_declaration_probe::{
    GeneratedProbe, compile_and_link_shim_probe, extract_oracle_header_functions,
    extract_oracle_header_macros, extract_oracle_header_objects, extract_oracle_type_surface,
    write_header_matrix_probe,
};
use support::libc_probe::resolve;
use support::libc_shim::{Architecture, LibcVariant};

fn headers() -> Vec<String> {
    std::fs::read_to_string(fixture_root().join("headers.txt"))
        .expect("read glibc x86_64 declaration header manifest")
        .lines()
        .map(str::trim)
        .filter(|line| !line.is_empty() && !line.starts_with('#'))
        .map(str::to_string)
        .collect()
}

fn fixture_root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR")).join("tests/fixtures.libc-static-test/glibc-x86_64")
}

fn header_directory(header: &str) -> String {
    header.replace(['/', '.'], "_")
}

fn fixture_source(header: &str) -> PathBuf {
    fixture_root()
        .join(header_directory(header))
        .join("shim-header-matrix.c")
}

#[test]
#[ignore = "regenerate glibc x86_64 declaration matrix fixtures"]
fn generate_glibc_x86_64_declaration_matrix_fixtures() {
    let config =
        resolve(Architecture::X86_64, LibcVariant::Glibc).expect("resolve glibc x86_64 oracle");
    let oracle_root =
        Path::new(env!("CARGO_MANIFEST_DIR")).join("target/libc-declaration-oracle/glibc-x86_64");
    for header in headers() {
        let oracle_output = oracle_root.join(header_directory(&header));
        let fixture_output = fixture_root().join(header_directory(&header));
        let functions = extract_oracle_header_functions(&config, &header, &oracle_output)
            .unwrap_or_else(|error| panic!("extract {header} functions: {error}"));
        let objects = extract_oracle_header_objects(&config, &header, &oracle_output)
            .unwrap_or_else(|error| panic!("extract {header} objects: {error}"));
        let surface = extract_oracle_type_surface(&config, &header, &oracle_output)
            .unwrap_or_else(|error| panic!("extract {header} type surface: {error}"));
        let macros = extract_oracle_header_macros(&config, &header, &oracle_output)
            .unwrap_or_else(|error| panic!("extract {header} macros: {error}"));
        write_header_matrix_probe(
            &header,
            &functions,
            &objects,
            &surface,
            &macros,
            &fixture_output,
        )
        .unwrap_or_else(|error| panic!("generate {header} fixture: {error}"));
    }
}

#[test]
fn glibc_x86_64_declaration_matrix() {
    let config =
        resolve(Architecture::X86_64, LibcVariant::Glibc).expect("resolve glibc x86_64 oracle");
    let root =
        Path::new(env!("CARGO_MANIFEST_DIR")).join("target/libc-declaration-matrix/glibc-x86_64");
    let mut failures = Vec::new();
    for header in headers() {
        let output = root.join(header_directory(&header));
        let source = fixture_source(&header);
        let result = if !source.is_file() {
            Err(format!(
                "missing generated fixture {}; run generate_glibc_x86_64_declaration_matrix_fixtures",
                source.display()
            ))
        } else {
            std::fs::create_dir_all(&output)
                .map_err(|error| format!("create {}: {error}", output.display()))
                .and_then(|()| {
                    compile_and_link_shim_probe(
                        &config,
                        &GeneratedProbe {
                            source,
                            object: output.join("shim-header-matrix.o"),
                            executable: output.join("shim-header-matrix"),
                        },
                    )
                })
        };
        if let Err(error) = result {
            failures.push(format!("{header}:\n{error}"));
        }
    }
    if !failures.is_empty() {
        panic!(
            "glibc x86_64 declaration matrix failed:\n\n{}",
            failures.join("\n\n")
        );
    }
}
