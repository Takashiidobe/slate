mod support;

use std::path::{Path, PathBuf};

use support::libc_declaration_probe::{
    GeneratedProbe, compile_and_link_shim_probe, extract_oracle_header_functions,
    extract_oracle_header_macros, extract_oracle_header_objects, extract_oracle_type_surface,
    write_header_matrix_probe,
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
        write_header_matrix_probe(
            &header,
            &functions,
            &objects,
            &surface,
            &macros,
            &fixture_output,
        )
        .unwrap_or_else(|error| panic!("generate {} {header} fixture: {error}", descriptor.name));
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
