use crate::c_side::{CMap, CType, Field};
use crate::rust_side::RustLayout;
use std::collections::{BTreeMap, BTreeSet};
use std::fmt::Write;

const MACRO_REACHED: &[&str] = &["FILE"];

pub struct TargetData {
    pub c: CMap,
    pub rust: BTreeMap<String, RustLayout>,
}

pub enum Outcome {
    Fields(Vec<String>),
    Opaque,
    Excluded,
}

pub struct Entry {
    pub name: String,
    pub outcome: Outcome,
    pub alignment: u64,
    pub aliases: BTreeSet<String>,
    pub notes: Vec<String>,
}

fn reserved(name: &str) -> bool {
    name.starts_with('_')
}

pub fn decide(targets: &BTreeMap<String, TargetData>) -> Vec<Entry> {
    let names: BTreeSet<&String> = targets.values().flat_map(|data| data.c.keys()).collect();
    names
        .into_iter()
        .map(|name| decide_one(name, targets))
        .collect()
}

fn decide_one(name: &str, targets: &BTreeMap<String, TargetData>) -> Entry {
    let present: Vec<(&String, &CType, Option<&RustLayout>)> = targets
        .iter()
        .filter_map(|(target, data)| data.c.get(name).map(|c| (target, c, data.rust.get(name))))
        .collect();
    let mut notes = Vec::new();
    let mut blocked = false;
    let mut common_fields: Option<Vec<String>> = None;
    let mut bad_fields: BTreeSet<String> = BTreeSet::new();

    for (target, c, rust) in &present {
        let Some(rust) = rust else {
            notes.push(format!("{target}: absent from libc"));
            blocked = true;
            continue;
        };
        let reserved_only = c
            .fields
            .as_ref()
            .is_some_and(|fields| fields.iter().all(|field| reserved(&field.name)));
        let opaque_candidate = reserved_only;
        if c.incomplete {
            continue;
        }
        let incomplete_in_libc = rust.size == 0 && reserved_only;
        if incomplete_in_libc {
            notes.push(format!(
                "{target}: libc models it as an incomplete type; pointer use only"
            ));
        } else if rust.size != c.size || rust.align != c.align {
            notes.push(format!(
                "{target}: layout C size={} align={} vs libc size={} align={}",
                c.size, c.align, rust.size, rust.align
            ));
            blocked = true;
        }
        match &c.fields {
            None => {
                notes.push(format!("{target}: C field offsets unavailable"));
                blocked = true;
            }
            Some(fields) => {
                let mut target_fields: Vec<String> = Vec::new();
                for Field {
                    name: field,
                    offset,
                    aggregate,
                } in fields
                {
                    let matches = rust.offsets.get(field) == Some(offset);
                    if field == "<anonymous>" {
                        continue;
                    }
                    if !reserved(field) {
                        if matches {
                            target_fields.push(field.clone());
                        } else {
                            bad_fields.insert(field.clone());
                            let detail = match rust.offsets.get(field) {
                                Some(libc) => format!("C offset {offset} vs libc {libc}"),
                                None => "missing or private in libc".to_string(),
                            };
                            notes.push(format!("{target}: field `{field}`: {detail}"));
                        }
                    } else if !matches && !incomplete_in_libc {
                        if *aggregate && !opaque_candidate {
                            bad_fields.insert(field.clone());
                            notes.push(format!(
                                "{target}: reserved aggregate `{field}` not mirrored by libc; \
                                 header macros reach members through it"
                            ));
                        } else {
                            notes.push(format!(
                                "{target}: reserved field `{field}` not mirrored by libc (ignored)"
                            ));
                        }
                    }
                }
                common_fields = Some(match common_fields {
                    None => target_fields,
                    Some(previous) => previous
                        .into_iter()
                        .filter(|field| target_fields.contains(field))
                        .collect(),
                });
            }
        }
    }

    let outcome = if blocked || !bad_fields.is_empty() {
        Outcome::Excluded
    } else {
        match common_fields {
            Some(fields) if !fields.is_empty() => Outcome::Fields(fields),
            _ => Outcome::Opaque,
        }
    };
    let alignment = present.iter().map(|(_, c, _)| c.align).max().unwrap_or(1);
    let aliases = present
        .iter()
        .filter_map(|(_, c, _)| c.record_name.clone())
        .filter(|record| record != name)
        .collect();
    Entry {
        name: name.to_string(),
        outcome,
        alignment,
        aliases,
        notes,
    }
}

pub fn table(entries: &[Entry]) -> (String, Vec<String>) {
    let accepted: BTreeSet<&str> = entries
        .iter()
        .filter(|entry| !matches!(entry.outcome, Outcome::Excluded))
        .map(|entry| entry.name.as_str())
        .collect();
    let mut rows: BTreeMap<String, (&Entry, String)> = BTreeMap::new();
    let mut conflicts = Vec::new();
    for entry in entries {
        if matches!(entry.outcome, Outcome::Excluded) {
            continue;
        }
        rows.insert(entry.name.clone(), (entry, entry.name.clone()));
    }
    for entry in entries {
        if matches!(entry.outcome, Outcome::Excluded) {
            continue;
        }
        if MACRO_REACHED.contains(&entry.name.as_str()) {
            continue;
        }
        for alias in &entry.aliases {
            if accepted.contains(alias.as_str()) {
                continue;
            }
            match rows.get(alias) {
                Some((_, target)) if target != &entry.name => conflicts.push(format!(
                    "alias `{alias}` wanted by both `{target}` and `{}`",
                    entry.name
                )),
                _ => {
                    rows.insert(alias.clone(), (entry, entry.name.clone()));
                }
            }
        }
    }
    let mut out = String::from("pub const CLIB_RECORD_TYPES: &[CLibType] = &[\n");
    for (c_name, (entry, rust_name)) in &rows {
        let initializer = match &entry.outcome {
            Outcome::Fields(fields) => format!(
                "CLibInitializer::Fields(&[{}])",
                fields
                    .iter()
                    .map(|field| format!("{field:?}"))
                    .collect::<Vec<_>>()
                    .join(", ")
            ),
            _ => "CLibInitializer::ScalarZero".to_string(),
        };
        let _ = writeln!(
            out,
            "    CLibType::new({c_name:?}, \"slate_support::{rust_name}\", {}, {initializer}),",
            entry.alignment
        );
    }
    out.push_str("];\n");
    (out, conflicts)
}

pub fn report(
    entries: &[Entry],
    targets: &BTreeMap<String, TargetData>,
    conflicts: &[String],
) -> String {
    let mut out = String::from("# slate-libc-gen report\n\n");
    let _ = writeln!(
        out,
        "Targets: {}\n",
        targets.keys().cloned().collect::<Vec<_>>().join(", ")
    );
    let accepted = entries
        .iter()
        .filter(|e| !matches!(e.outcome, Outcome::Excluded))
        .count();
    let _ = writeln!(
        out,
        "{} candidate names, {} accepted, {} excluded.\n",
        entries.len(),
        accepted,
        entries.len() - accepted
    );
    if !conflicts.is_empty() {
        out.push_str("## Alias conflicts\n\n");
        for conflict in conflicts {
            let _ = writeln!(out, "- {conflict}");
        }
        out.push('\n');
    }
    out.push_str("## Accepted\n\n");
    for entry in entries
        .iter()
        .filter(|e| !matches!(e.outcome, Outcome::Excluded))
    {
        let kind = match &entry.outcome {
            Outcome::Fields(fields) => format!("fields {fields:?}"),
            _ => "opaque".to_string(),
        };
        let _ = writeln!(out, "- `{}`: {kind}", entry.name);
        for note in &entry.notes {
            let _ = writeln!(out, "  - {note}");
        }
    }
    out.push_str("\n## Excluded\n\n");
    for entry in entries
        .iter()
        .filter(|e| matches!(e.outcome, Outcome::Excluded))
    {
        let _ = writeln!(out, "- `{}`", entry.name);
        for note in &entry.notes {
            let _ = writeln!(out, "  - {note}");
        }
    }
    out
}
