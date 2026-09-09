mod support;

use std::path::{Path, PathBuf};

use support::libc_declaration_probe::{
    GeneratedProbe, compile_and_link_oracle_probe, compile_and_link_shim_probe, diff_header_files,
    diff_macro_names, extract_oracle_header_files, extract_oracle_header_functions,
    extract_oracle_header_macros, extract_oracle_header_objects, extract_oracle_type_surface,
    extract_shim_header_files, extract_shim_header_functions, extract_shim_header_macros,
    extract_shim_type_surface, select_cross_checkable_shim_macros,
    select_oracle_object_macro_value_probes, select_shim_object_macro_value_probes,
    write_header_matrix_probe, write_header_object_macro_value_probe, write_header_shim_probe,
    write_type_surface_probe,
};
use support::libc_probe::resolve;
use support::libc_shim::{Architecture, LibcVariant};

struct DeclarationMatrixDescriptor {
    name: &'static str,
    architecture: Architecture,
    libc: LibcVariant,
    feature_profile: &'static str,
    fixture_root: &'static str,
    manifest: &'static str,
    oracle_root: &'static str,
}

const DESCRIPTORS: &[DeclarationMatrixDescriptor] = &[
    DeclarationMatrixDescriptor {
        name: "glibc-x86_64",
        architecture: Architecture::X86_64,
        libc: LibcVariant::Glibc,
        feature_profile: "default",
        fixture_root: "tests/fixtures.libc-static-test/glibc-x86_64",
        manifest: "headers.txt",
        oracle_root: "target/libc-declaration-oracle/glibc-x86_64",
    },
    DeclarationMatrixDescriptor {
        name: "musl-x86_64",
        architecture: Architecture::X86_64,
        libc: LibcVariant::Musl,
        feature_profile: "default",
        fixture_root: "tests/fixtures.libc-static-test/musl-x86_64",
        manifest: "headers.txt",
        oracle_root: "target/libc-declaration-oracle/musl-x86_64",
    },
    DeclarationMatrixDescriptor {
        name: "glibc-i386",
        architecture: Architecture::X86,
        libc: LibcVariant::Glibc,
        feature_profile: "default",
        fixture_root: "tests/fixtures.libc-static-test/glibc-i386",
        manifest: "headers.txt",
        oracle_root: "target/libc-declaration-oracle/glibc-i386",
    },
    DeclarationMatrixDescriptor {
        name: "musl-i386",
        architecture: Architecture::X86,
        libc: LibcVariant::Musl,
        feature_profile: "default",
        fixture_root: "tests/fixtures.libc-static-test/musl-i386",
        manifest: "headers.txt",
        oracle_root: "target/libc-declaration-oracle/musl-i386",
    },
    DeclarationMatrixDescriptor {
        name: "glibc-arm",
        architecture: Architecture::Arm,
        libc: LibcVariant::Glibc,
        feature_profile: "default",
        fixture_root: "tests/fixtures.libc-static-test/glibc-arm",
        manifest: "headers.txt",
        oracle_root: "target/libc-declaration-oracle/glibc-arm",
    },
    DeclarationMatrixDescriptor {
        name: "musl-arm",
        architecture: Architecture::Arm,
        libc: LibcVariant::Musl,
        feature_profile: "default",
        fixture_root: "tests/fixtures.libc-static-test/musl-arm",
        manifest: "headers.txt",
        oracle_root: "target/libc-declaration-oracle/musl-arm",
    },
    DeclarationMatrixDescriptor {
        name: "glibc-aarch64",
        architecture: Architecture::Aarch64,
        libc: LibcVariant::Glibc,
        feature_profile: "default",
        fixture_root: "tests/fixtures.libc-static-test/glibc-aarch64",
        manifest: "headers.txt",
        oracle_root: "target/libc-declaration-oracle/glibc-aarch64",
    },
    DeclarationMatrixDescriptor {
        name: "musl-aarch64",
        architecture: Architecture::Aarch64,
        libc: LibcVariant::Musl,
        feature_profile: "default",
        fixture_root: "tests/fixtures.libc-static-test/musl-aarch64",
        manifest: "headers.txt",
        oracle_root: "target/libc-declaration-oracle/musl-aarch64",
    },
];

impl DeclarationMatrixDescriptor {
    fn config(&self) -> support::libc_probe::ProbeConfig {
        resolve(self.architecture, self.libc).unwrap_or_else(|error| {
            panic!(
                "resolve {} declaration matrix ({} feature profile): {error}",
                self.name, self.feature_profile
            )
        })
    }

    fn fixture_root(&self) -> PathBuf {
        Path::new(env!("CARGO_MANIFEST_DIR")).join(self.fixture_root)
    }

    fn oracle_root(&self) -> PathBuf {
        Path::new(env!("CARGO_MANIFEST_DIR")).join(self.oracle_root)
    }

    fn headers(&self) -> Vec<String> {
        std::fs::read_to_string(self.fixture_root().join(self.manifest))
            .unwrap_or_else(|error| {
                panic!(
                    "read {} declaration header manifest {}: {error}",
                    self.name, self.manifest
                )
            })
            .lines()
            .map(str::trim)
            .filter(|line| !line.is_empty() && !line.starts_with('#'))
            .map(str::to_string)
            .collect()
    }

    fn fixture_source(&self, header: &str) -> PathBuf {
        self.fixture_root()
            .join(header_directory(header))
            .join("shim-header-matrix.c")
    }
}

fn header_directory(header: &str) -> String {
    header.replace(['/', '.'], "_")
}

#[test]
fn header_visibility_failure_lists_missing_and_extra_headers() {
    let oracle = ["features.h".to_string(), "sys/types.h".to_string()]
        .into_iter()
        .collect();
    let shim = ["features.h".to_string(), "bits/types.h".to_string()]
        .into_iter()
        .collect();

    let failure = header_visibility_failure("example.h", &oracle, &shim).unwrap();

    assert_eq!(
        failure,
        "header visibility mismatch for example.h:\nmissing from shim: [\"sys/types.h\"]\nextra in shim: [\"bits/types.h\"]"
    );
}

fn header_visibility_failure(
    header: &str,
    oracle: &std::collections::BTreeSet<String>,
    shim: &std::collections::BTreeSet<String>,
) -> Option<String> {
    let diff = diff_header_files(oracle, shim);
    let missing_from_shim = diff.missing_from_shim;
    let extra_in_shim = diff.extra_in_shim;
    if missing_from_shim.is_empty() && extra_in_shim.is_empty() {
        return None;
    }
    Some(format!(
        "header visibility mismatch for {header}:\nmissing from shim: {:?}\nextra in shim: {:?}",
        missing_from_shim, extra_in_shim
    ))
}

fn selected_descriptor() -> &'static DeclarationMatrixDescriptor {
    match std::env::var("SLATE_LIBC_DECL_TARGET") {
        Ok(name) => DESCRIPTORS
            .iter()
            .find(|descriptor| descriptor.name == name)
            .unwrap_or_else(|| {
                panic!(
                    "unknown SLATE_LIBC_DECL_TARGET {name}; expected one of: {}",
                    DESCRIPTORS
                        .iter()
                        .map(|descriptor| descriptor.name)
                        .collect::<Vec<_>>()
                        .join(", ")
                )
            }),
        Err(_) => &DESCRIPTORS[0],
    }
}

fn enabled_descriptors() -> impl Iterator<Item = &'static DeclarationMatrixDescriptor> {
    match std::env::var("SLATE_LIBC_DECL_TARGET") {
        Ok(_) => vec![selected_descriptor()].into_iter(),
        Err(_) => DESCRIPTORS.iter().collect::<Vec<_>>().into_iter(),
    }
}

#[test]
#[ignore = "regenerate declaration matrix fixtures for the selected target"]
fn generate_declaration_matrix_fixtures() {
    let descriptor = selected_descriptor();
    let config = descriptor.config();
    for header in descriptor.headers() {
        let oracle_output = descriptor.oracle_root().join(header_directory(&header));
        let fixture_output = descriptor.fixture_root().join(header_directory(&header));
        let functions = extract_oracle_header_functions(&config, &header, &oracle_output)
            .unwrap_or_else(|error| {
                panic!("extract {} {header} functions: {error}", descriptor.name)
            });
        let objects = extract_oracle_header_objects(&config, &header, &oracle_output)
            .unwrap_or_else(|error| {
                panic!("extract {} {header} objects: {error}", descriptor.name)
            });
        let surface =
            extract_oracle_type_surface(&config, &header, &oracle_output).unwrap_or_else(|error| {
                panic!("extract {} {header} type surface: {error}", descriptor.name)
            });
        let macros = extract_oracle_header_macros(&config, &header, &oracle_output)
            .unwrap_or_else(|error| panic!("extract {} {header} macros: {error}", descriptor.name));
        let result = write_header_matrix_probe(
            &header,
            &functions,
            &objects,
            &surface,
            &macros,
            &fixture_output,
        );
        if let Err(error) = result {
            if error == format!("{header} has no matrix probe strategy") {
                continue;
            }
            panic!("generate {} {header} fixture: {error}", descriptor.name);
        }
    }
}

#[test]
fn declaration_matrices() {
    let mut matrix_failures = Vec::new();
    for descriptor in enabled_descriptors() {
        let config = descriptor.config();
        let root = Path::new(env!("CARGO_MANIFEST_DIR"))
            .join("target/libc-declaration-matrix")
            .join(descriptor.name);
        let mut failures = Vec::new();
        for header in descriptor.headers() {
            let output = root.join(header_directory(&header));
            let source = descriptor.fixture_source(&header);
            let result = if !source.is_file() {
                Err(format!(
                    "missing generated fixture {}; run generate_declaration_matrix_fixtures with SLATE_LIBC_DECL_TARGET={}",
                    source.display(),
                    descriptor.name
                ))
            } else {
                std::fs::create_dir_all(&output)
                    .map_err(|error| format!("create {}: {error}", output.display()))
                    .and_then(|()| {
                        let oracle_files = extract_oracle_header_files(
                            &config,
                            &header,
                            &output.join("oracle-files"),
                        )?;
                        let shim_files = extract_shim_header_files(
                            &config,
                            &header,
                            &output.join("shim-files"),
                        )?;
                        if let Some(failure) =
                            header_visibility_failure(&header, &oracle_files, &shim_files)
                        {
                            return Err(failure);
                        }
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
            matrix_failures.push(format!(
                "{} ({} feature profile):\n{}",
                descriptor.name,
                descriptor.feature_profile,
                failures.join("\n\n")
            ));
        }
    }
    if !matrix_failures.is_empty() {
        panic!(
            "libc declaration matrices failed:\n\n{}",
            matrix_failures.join("\n\n")
        );
    }
}

const BIDIRECTIONAL_HEADERS: &[&str] = &["arpa/nameser.h"];

fn run_bidirectional_checks(
    config: &support::libc_probe::ProbeConfig,
    header: &str,
    root: &Path,
) -> Result<(), String> {
    let output = root.join(header.replace(['/', '.'], "_"));

    let oracle_files = extract_oracle_header_files(config, header, &output.join("oracle-files"))?;
    let shim_files = extract_shim_header_files(config, header, &output.join("shim-files"))?;
    let file_diff = diff_header_files(&oracle_files, &shim_files);
    if !file_diff.extra_in_shim.is_empty() {
        return Err(format!(
            "shim leaks extra headers for {header}: {:?}",
            file_diff.extra_in_shim
        ));
    }

    let shim_functions = extract_shim_header_functions(config, header, &output)?;
    if let Ok(probe) = write_header_shim_probe(&shim_functions, &output) {
        compile_and_link_oracle_probe(config, &probe)?;
    }

    let shim_surface = extract_shim_type_surface(config, header, &output)?;
    if let Ok(probe) = write_type_surface_probe(header, &shim_surface, &output) {
        compile_and_link_oracle_probe(config, &probe)?;
    }

    let oracle_macros = extract_oracle_header_macros(config, header, &output.join("oracle"))?;
    let shim_macros = extract_shim_header_macros(config, header, &output.join("shim"))?;
    let macro_diff = diff_macro_names(&oracle_macros, &shim_macros);
    if !macro_diff.missing_from_shim.is_empty() {
        eprintln!(
            "note: macros reported missing from shim for {header} (often transitively-leaked \
             oracle bits/* macros unrelated to this header, a known heuristic limitation): {:?}",
            macro_diff.missing_from_shim
        );
    }

    let oracle_classified =
        select_oracle_object_macro_value_probes(config, &oracle_macros, &output.join("oracle"))?;
    let shim_classified =
        select_shim_object_macro_value_probes(config, &shim_macros, &output.join("shim"))?;
    let cross_checkable = select_cross_checkable_shim_macros(&oracle_classified, &shim_classified);
    if !cross_checkable.is_empty() {
        let probe = write_header_object_macro_value_probe(&cross_checkable, &output.join("cross"))?;
        compile_and_link_oracle_probe(config, &probe)?;
    }

    Ok(())
}

#[test]
fn bidirectional_declaration_checks() {
    let targets = [
        (Architecture::X86_64, LibcVariant::Glibc, "glibc-x86_64"),
        (Architecture::X86_64, LibcVariant::Musl, "musl-x86_64"),
    ];
    let mut failures = Vec::new();
    for (arch, libc, name) in targets {
        let config = resolve(arch, libc)
            .unwrap_or_else(|error| panic!("resolve {name} bidirectional check: {error}"));
        let root = Path::new(env!("CARGO_MANIFEST_DIR"))
            .join("target/libc-declaration-bidirectional")
            .join(name);
        for header in BIDIRECTIONAL_HEADERS {
            if let Err(error) = run_bidirectional_checks(&config, header, &root) {
                failures.push(format!("{name} {header}:\n{error}"));
            }
        }
    }
    if !failures.is_empty() {
        panic!(
            "bidirectional libc declaration checks failed:\n\n{}",
            failures.join("\n\n")
        );
    }
}
