mod support;

use support::libc_probe::{self, ABI_FAMILIES};
use support::libc_shim::{Architecture, LibcVariant};

fn selected_libcs() -> Vec<LibcVariant> {
    match std::env::var("SLATE_LIBC_ABI_LIBC").ok().as_deref() {
        Some("musl") => vec![LibcVariant::Musl],
        Some("glibc") => vec![LibcVariant::Glibc],
        Some("freebsd") => vec![LibcVariant::FreeBsd],
        Some(other) => panic!("unknown SLATE_LIBC_ABI_LIBC value: {other}"),
        None => vec![LibcVariant::Musl, LibcVariant::Glibc],
    }
}

fn selected_arches() -> Vec<Architecture> {
    match std::env::var("SLATE_LIBC_ABI_ARCH") {
        Ok(value) => value
            .split(',')
            .filter(|value| !value.is_empty())
            .map(|value| {
                libc_probe::arch_from_key(value)
                    .unwrap_or_else(|| panic!("unknown SLATE_LIBC_ABI_ARCH value: {value}"))
            })
            .collect(),
        Err(_) => libc_probe::ARCHES.to_vec(),
    }
}

fn selected_families() -> Option<Vec<String>> {
    std::env::var("SLATE_LIBC_ABI_FAMILY").ok().map(|value| {
        value
            .split(',')
            .filter(|value| !value.is_empty())
            .map(|value| {
                assert!(
                    ABI_FAMILIES.contains(&value),
                    "unknown SLATE_LIBC_ABI_FAMILY value: {value}"
                );
                value.to_string()
            })
            .collect()
    })
}

type AbiRecord = (String, Option<i64>, Option<i64>);

enum Outcome {
    Ran(Vec<AbiRecord>),
    CompiledOnly,
}

fn evaluate(
    config: &libc_probe::ProbeConfig,
    families: Option<&[String]>,
) -> Result<Outcome, String> {
    if !config.can_execute {
        libc_probe::compile_and_link_probe(config, false)?;
        libc_probe::compile_and_link_probe(config, true)?;
        return Ok(Outcome::CompiledOnly);
    }
    let oracle = libc_probe::run_probe(config, false)?;
    let candidate = libc_probe::run_probe(config, true)?;
    let mut records = libc_probe::compare_records(&oracle, &candidate);
    if let Some(families) = families {
        records.retain(|(name, _, _)| {
            families
                .iter()
                .any(|family| libc_probe::in_family(name, family))
        });
        if records.is_empty() {
            return Err("selected ABI family has no probe records".to_string());
        }
    }
    Ok(Outcome::Ran(records))
}

#[test]
fn libc_abi_matrix() {
    let families = selected_families();
    let mut skipped = Vec::new();
    let mut failures = Vec::new();
    let mut targets_passed = 0usize;
    let mut targets_failed = 0usize;
    let mut records_passed = 0usize;
    let mut records_failed = 0usize;

    for libc in selected_libcs() {
        for arch in selected_arches() {
            let label = format!("{}/{}", libc.name(), libc_probe::arch_key(arch));
            let config = match libc_probe::resolve(arch, libc) {
                Ok(config) => config,
                Err(reason) => {
                    eprintln!("SKIP {label}: {reason}");
                    skipped.push(label);
                    continue;
                }
            };
            match evaluate(&config, families.as_deref()) {
                Ok(Outcome::CompiledOnly) => {
                    println!(
                        "PASS {label} (compile+link only; runtime values unverified on this host)"
                    );
                    targets_passed += 1;
                }
                Ok(Outcome::Ran(records)) => {
                    let mismatches: Vec<&AbiRecord> = records
                        .iter()
                        .filter(|(_, expected, actual)| expected != actual)
                        .collect();
                    records_passed += records.len() - mismatches.len();
                    records_failed += mismatches.len();
                    for (name, expected, actual) in &records {
                        if expected == actual {
                            println!("  PASS {name}");
                        } else {
                            println!("  FAIL {name}: oracle={expected:?} candidate={actual:?}");
                        }
                    }
                    if mismatches.is_empty() {
                        println!("PASS {label}");
                        targets_passed += 1;
                    } else {
                        println!("FAIL {label}");
                        targets_failed += 1;
                        let detail = mismatches
                            .iter()
                            .map(|(name, expected, actual)| {
                                format!("{name}: oracle={expected:?} candidate={actual:?}")
                            })
                            .collect::<Vec<_>>()
                            .join("\n");
                        failures.push(format!("{label}:\n{detail}"));
                    }
                }
                Err(error) => {
                    eprintln!("FAIL {label}: {error}");
                    targets_failed += 1;
                    failures.push(format!("{label}: {error}"));
                }
            }
        }
    }

    println!(
        "SUMMARY targets_passed={targets_passed} targets_failed={targets_failed} \
         targets_skipped={} records_passed={records_passed} records_failed={records_failed}",
        skipped.len()
    );

    assert!(
        failures.is_empty(),
        "libc ABI oracle matrix failed:\n\n{}",
        failures.join("\n\n")
    );
}
