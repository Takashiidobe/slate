mod support;

use std::path::{Path, PathBuf};

use support::libc_declaration_probe::{
    GeneratedProbe, compile_and_link_oracle_probe, compile_and_link_shim_probe, diff_header_files,
    diff_macro_names, extract_oracle_header_files, extract_oracle_header_functions,
    extract_oracle_header_macros, extract_oracle_header_macros_with_args,
    extract_oracle_header_objects, extract_oracle_header_symbol_names_with_args,
    extract_oracle_type_surface, extract_oracle_type_surface_with_args, extract_shim_header_files,
    extract_shim_header_functions, extract_shim_header_macros,
    extract_shim_header_macros_with_args, extract_shim_header_symbol_names_with_args,
    extract_shim_type_surface, select_cross_checkable_shim_macros,
    select_oracle_object_macro_value_probes, select_shim_object_macro_value_probes,
    write_header_matrix_probe, write_header_object_macro_value_probe, write_header_shim_probe,
    write_type_surface_probe, write_type_surface_probe_with_args,
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
        name: "msvc-x86_64",
        architecture: Architecture::X86_64,
        libc: LibcVariant::Msvc,
        feature_profile: "default",
        fixture_root: "tests/fixtures.libc-static-test/msvc-x86_64",
        manifest: "headers.txt",
        oracle_root: "target/libc-declaration-oracle/msvc-x86_64",
    },
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
        Err(_) => DESCRIPTORS
            .iter()
            .find(|descriptor| descriptor.libc != LibcVariant::Msvc)
            .expect("at least one default declaration matrix descriptor"),
    }
}

fn enabled_descriptors() -> impl Iterator<Item = &'static DeclarationMatrixDescriptor> {
    match std::env::var("SLATE_LIBC_DECL_TARGET") {
        Ok(_) => vec![selected_descriptor()].into_iter(),
        Err(_) => DESCRIPTORS
            .iter()
            .filter(|descriptor| descriptor.libc != LibcVariant::Msvc)
            .collect::<Vec<_>>()
            .into_iter(),
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
            matches!(descriptor.libc, LibcVariant::Darwin | LibcVariant::Msvc),
            true,
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
                        if descriptor.libc != LibcVariant::Msvc
                            && let Some(failure) =
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

const C11_HEADERS: &[&str] = &[
    "assert.h",
    "ctype.h",
    "errno.h",
    "float.h",
    "limits.h",
    "locale.h",
    "math.h",
    "setjmp.h",
    "signal.h",
    "stdarg.h",
    "stddef.h",
    "stdio.h",
    "stdlib.h",
    "string.h",
    "time.h",
    "iso646.h",
    "wchar.h",
    "wctype.h",
    "stdbool.h",
    "stdint.h",
    "inttypes.h",
    "complex.h",
    "fenv.h",
    "tgmath.h",
    "stdalign.h",
    "stdatomic.h",
    "stdnoreturn.h",
    "threads.h",
    "uchar.h",
];

const C23_HEADERS: &[&str] = &["stdbit.h", "stdckdint.h"];

const FEATURE_HEADERS: &[&str] = &[
    "aio.h",
    "arpa/inet.h",
    "dirent.h",
    "fcntl.h",
    "netdb.h",
    "poll.h",
    "pthread.h",
    "pwd.h",
    "sched.h",
    "strings.h",
    "sys/mman.h",
    "sys/resource.h",
    "sys/select.h",
    "sys/socket.h",
    "sys/stat.h",
    "sys/time.h",
    "sys/types.h",
    "sys/uio.h",
    "sys/wait.h",
    "termios.h",
    "unistd.h",
    "wait.h",
];

const LARGE_FILE_HEADERS: &[&str] = &[
    "dirent.h",
    "fcntl.h",
    "pthread.h",
    "stdio.h",
    "stdlib.h",
    "sys/stat.h",
    "sys/time.h",
    "sys/types.h",
    "time.h",
    "unistd.h",
];

const FEATURE_MODES: &[(&str, &[&str])] = &[
    ("c89", &["-std=c89"]),
    ("iso9899-1990", &["-std=iso9899:1990"]),
    ("iso9899-199409", &["-std=iso9899:199409"]),
    ("gnu89", &["-std=gnu89"]),
    ("gnu89-default", &["-std=gnu89", "-D_DEFAULT_SOURCE"]),
    ("gnu89-gnu", &["-std=gnu89", "-D_GNU_SOURCE"]),
    ("c99", &["-std=c99"]),
    ("iso9899-1999", &["-std=iso9899:1999"]),
    ("gnu99", &["-std=gnu99"]),
    ("gnu99-default", &["-std=gnu99", "-D_DEFAULT_SOURCE"]),
    ("gnu99-gnu", &["-std=gnu99", "-D_GNU_SOURCE"]),
    ("c11", &["-std=c11"]),
    ("iso9899-2011", &["-std=iso9899:2011"]),
    ("gnu11", &["-std=gnu11"]),
    ("gnu11-default", &["-std=gnu11", "-D_DEFAULT_SOURCE"]),
    ("gnu11-gnu", &["-std=gnu11", "-D_GNU_SOURCE"]),
    ("c17", &["-std=c17"]),
    ("iso9899-2017", &["-std=iso9899:2017"]),
    ("gnu17", &["-std=gnu17"]),
    ("gnu17-default", &["-std=gnu17", "-D_DEFAULT_SOURCE"]),
    ("gnu17-gnu", &["-std=gnu17", "-D_GNU_SOURCE"]),
    ("c23", &["-std=c23"]),
    ("c23-isoc23", &["-std=c23", "-D_ISOC23_SOURCE"]),
    ("c23-isoc2x", &["-std=c23", "-D_ISOC2X_SOURCE"]),
    ("gnu23", &["-std=gnu23"]),
    ("gnu23-default", &["-std=gnu23", "-D_DEFAULT_SOURCE"]),
    ("gnu23-gnu", &["-std=gnu23", "-D_GNU_SOURCE"]),
    ("posix-source", &["-std=c17", "-D_POSIX_SOURCE"]),
    ("posix-1", &["-std=c17", "-D_POSIX_C_SOURCE=1"]),
    ("posix-2", &["-std=c17", "-D_POSIX_C_SOURCE=2"]),
    ("posix-199309", &["-std=c17", "-D_POSIX_C_SOURCE=199309L"]),
    ("posix-199506", &["-std=c17", "-D_POSIX_C_SOURCE=199506L"]),
    ("posix-200112", &["-std=c17", "-D_POSIX_C_SOURCE=200112L"]),
    ("posix-200809", &["-std=c17", "-D_POSIX_C_SOURCE=200809L"]),
    ("posix-202405", &["-std=c17", "-D_POSIX_C_SOURCE=202405L"]),
    ("xopen-500", &["-std=c17", "-D_XOPEN_SOURCE=500"]),
    ("xopen-600", &["-std=c17", "-D_XOPEN_SOURCE=600"]),
    ("xopen-700", &["-std=c17", "-D_XOPEN_SOURCE=700"]),
    ("xopen-800", &["-std=c17", "-D_XOPEN_SOURCE=800"]),
    ("c17-default-source", &["-std=c17", "-D_DEFAULT_SOURCE"]),
    ("c17-gnu-source", &["-std=c17", "-D_GNU_SOURCE"]),
    ("c23-default-source", &["-std=c23", "-D_DEFAULT_SOURCE"]),
    ("c23-gnu-source", &["-std=c23", "-D_GNU_SOURCE"]),
    ("largefile-source", &["-std=c17", "-D_LARGEFILE_SOURCE"]),
    ("largefile64-source", &["-std=c17", "-D_LARGEFILE64_SOURCE"]),
    ("file-offset-64", &["-std=c17", "-D_FILE_OFFSET_BITS=64"]),
    (
        "file-time-64",
        &["-std=c17", "-D_FILE_OFFSET_BITS=64", "-D_TIME_BITS=64"],
    ),
];

#[test]
#[ignore = "known shim visibility gaps; run manually to inspect the full report"]
fn feature_visibility_matrix() {
    let targets = [
        (Architecture::X86_64, LibcVariant::Glibc, "glibc-x86_64"),
        (Architecture::X86_64, LibcVariant::Musl, "musl-x86_64"),
    ];
    let selected = std::env::var("SLATE_LIBC_FEATURE_PROFILE").ok();
    assert!(
        selected
            .as_deref()
            .is_none_or(|selected| FEATURE_MODES.iter().any(|(name, _)| *name == selected)),
        "unknown SLATE_LIBC_FEATURE_PROFILE {selected:?}"
    );
    let mut failures = Vec::new();
    let mut extras = Vec::new();
    let mut macro_extras = Vec::new();
    for (arch, libc, target) in targets {
        let config = resolve(arch, libc)
            .unwrap_or_else(|error| panic!("resolve {target} feature visibility: {error}"));
        for (mode, args) in FEATURE_MODES {
            if selected
                .as_deref()
                .is_some_and(|selected| selected != *mode)
            {
                continue;
            }
            let root = Path::new(env!("CARGO_MANIFEST_DIR"))
                .join("target/libc-feature-visibility")
                .join(target)
                .join(mode);
            let mut headers = C11_HEADERS.to_vec();
            if mode.contains("23") && libc == LibcVariant::Glibc {
                headers.extend_from_slice(C23_HEADERS);
            }
            if mode.starts_with("largefile") || mode.starts_with("file-") {
                headers.extend_from_slice(LARGE_FILE_HEADERS);
            } else if mode.starts_with("posix-")
                || mode.starts_with("xopen-")
                || mode.ends_with("default-source")
                || mode.ends_with("gnu-source")
            {
                headers.extend_from_slice(FEATURE_HEADERS);
            }
            headers.sort_unstable();
            headers.dedup();
            for header in headers {
                let output = root.join(header_directory(header));
                let result = extract_oracle_header_symbol_names_with_args(
                    &config,
                    header,
                    &output.join("oracle"),
                    args,
                )
                .and_then(|oracle| {
                    let shim = extract_shim_header_symbol_names_with_args(
                        &config,
                        header,
                        &output.join("shim"),
                        args,
                    )?;
                    let diff = diff_header_files(&oracle, &shim);
                    if diff.extra_in_shim.is_empty() {
                        Ok(())
                    } else {
                        for symbol in &diff.extra_in_shim {
                            extras.push(format!("{target}\t{mode}\t{header}\t{symbol}"));
                        }
                        Err(format!("{} extra symbol(s)", diff.extra_in_shim.len()))
                    }
                });
                if let Err(error) = result {
                    failures.push(format!("{target} {mode} {header}: {error}"));
                }
                if (mode.starts_with("largefile") || mode.starts_with("file-"))
                    && LARGE_FILE_HEADERS.contains(&header)
                {
                    let type_result = extract_oracle_type_surface_with_args(
                        &config,
                        header,
                        &output.join("oracle-types"),
                        args,
                    )
                    .and_then(
                        |oracle| match write_type_surface_probe_with_args(
                            header,
                            &oracle,
                            args,
                            &output.join("type-probe"),
                        ) {
                            Ok(probe) => compile_and_link_shim_probe(&config, &probe),
                            Err(error) if error.contains("no safe type-surface checks") => Ok(()),
                            Err(error) => Err(error),
                        },
                    );
                    if let Err(error) = type_result {
                        failures.push(format!("{target} {mode} {header} types: {error}"));
                    }
                }
                let macro_result = extract_oracle_header_macros_with_args(
                    &config,
                    header,
                    &output.join("oracle-macros"),
                    args,
                )
                .and_then(|oracle| {
                    let shim = extract_shim_header_macros_with_args(
                        &config,
                        header,
                        &output.join("shim-macros"),
                        args,
                    )?;
                    let diff = diff_macro_names(&oracle, &shim);
                    if diff.extra_in_shim.is_empty() {
                        Ok(())
                    } else {
                        for macro_name in &diff.extra_in_shim {
                            macro_extras.push(format!("{target}\t{mode}\t{header}\t{macro_name}"));
                        }
                        Err(format!("{} extra macro(s)", diff.extra_in_shim.len()))
                    }
                });
                if let Err(error) = macro_result {
                    failures.push(format!("{target} {mode} {header} macros: {error}"));
                }
            }
        }
    }
    let report = Path::new(env!("CARGO_MANIFEST_DIR"))
        .join("target/libc-feature-visibility/extra-symbols.tsv");
    std::fs::write(&report, extras.join("\n") + "\n")
        .unwrap_or_else(|error| panic!("write {}: {error}", report.display()));
    let macro_report = Path::new(env!("CARGO_MANIFEST_DIR"))
        .join("target/libc-feature-visibility/extra-macros.tsv");
    std::fs::write(&macro_report, macro_extras.join("\n") + "\n")
        .unwrap_or_else(|error| panic!("write {}: {error}", macro_report.display()));
    assert!(
        failures.is_empty(),
        "libc feature visibility mismatches ({} extra symbols, {} extra macros; reports at {} and {}):\n{}",
        extras.len(),
        macro_extras.len(),
        report.display(),
        macro_report.display(),
        failures.join("\n")
    );
}
