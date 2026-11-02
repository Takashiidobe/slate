use crate::c_side::{CMap, CType, Field};
use crate::rust_side::RustLayout;
use std::collections::{BTreeMap, BTreeSet};
use std::fmt::Write;

const MACRO_REACHED: &[&str] = &["FILE"];

pub struct TargetData {
    pub c: CMap,
    pub rust: BTreeMap<String, RustLayout>,
}

#[derive(PartialEq, Eq)]
pub enum Outcome {
    Fields(Vec<String>),
    Opaque,
    Excluded,
}

pub struct Entry {
    pub name: String,
    pub targets: BTreeSet<String>,
    pub outcome: Outcome,
    pub alignment: u64,
    pub layout_key: u64,
    pub aliases: BTreeSet<String>,
    pub notes: Vec<String>,
}

fn reserved(name: &str) -> bool {
    name.starts_with('_')
}

pub fn decide(targets: &BTreeMap<String, TargetData>) -> Vec<Entry> {
    let names: BTreeSet<&String> = targets.values().flat_map(|data| data.c.keys()).collect();
    let mut entries: Vec<Entry> = Vec::new();
    for name in names {
        for (target, data) in targets {
            let Some(c) = data.c.get(name) else {
                continue;
            };
            let entry = decide_one(name, target, c, data.rust.get(name));
            if let Some(previous) = entries.iter_mut().find(|previous| {
                previous.name == entry.name
                    && previous.outcome == entry.outcome
                    && previous.alignment == entry.alignment
                    && previous.layout_key == entry.layout_key
                    && previous.aliases == entry.aliases
            }) {
                previous.targets.extend(entry.targets);
                previous.notes.extend(entry.notes);
            } else {
                entries.push(entry);
            }
        }
    }
    entries
}

fn decide_one(name: &str, target: &str, c: &CType, rust: Option<&RustLayout>) -> Entry {
    let present = [(target, c, rust)];
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
                    bitfield,
                    ..
                } in fields
                {
                    let matches = !*bitfield && rust.offsets.get(field) == Some(offset);
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
        targets: BTreeSet::from([target.to_string()]),
        outcome,
        alignment,
        layout_key: {
            let fields = c
                .fields
                .iter()
                .flatten()
                .map(|field| {
                    (
                        format!(
                            "{}:{}",
                            field.name,
                            slate::libc_catalog::field_type_key(&field.shape)
                        ),
                        field.offset,
                        field.bitfield,
                    )
                })
                .collect::<Vec<_>>();
            slate::libc_catalog::layout_key(
                c.size,
                c.align,
                fields
                    .iter()
                    .map(|(name, offset, bits)| (name.as_str(), *offset, *bits)),
            )
        },
        aliases,
        notes,
    }
}

pub fn table(entries: &[Entry]) -> (String, Vec<String>) {
    let accepted: BTreeSet<(&str, &str)> = entries
        .iter()
        .filter(|entry| !matches!(entry.outcome, Outcome::Excluded))
        .flat_map(|entry| {
            entry
                .targets
                .iter()
                .map(|target| (target.as_str(), entry.name.as_str()))
        })
        .collect();
    let mut rows: BTreeMap<String, BTreeMap<String, BTreeSet<String>>> = BTreeMap::new();
    let mut aliases: BTreeMap<(&str, &str), &str> = BTreeMap::new();
    let mut conflicts = Vec::new();
    for entry in entries {
        if matches!(entry.outcome, Outcome::Excluded) {
            continue;
        }
        for target in &entry.targets {
            let mut names = vec![&entry.name];
            if !MACRO_REACHED.contains(&entry.name.as_str()) {
                for alias in &entry.aliases {
                    if accepted.contains(&(target.as_str(), alias.as_str())) {
                        continue;
                    }
                    match aliases.get(&(target.as_str(), alias.as_str())) {
                        Some(previous) if *previous != entry.name => {
                            conflicts.push(format!(
                                "{target}: alias `{alias}` wanted by both `{previous}` and `{}`",
                                entry.name
                            ));
                            continue;
                        }
                        _ => {
                            aliases.insert((target, alias), &entry.name);
                        }
                    }
                    names.push(alias);
                }
            }
            for name in names {
                rows.entry(entry.name.clone())
                    .or_default()
                    .entry(name.clone())
                    .or_default()
                    .insert(target.clone());
            }
        }
    }
    let mut out = String::from("pub const CLIB_RECORD_TYPES: &[CLibRecord] = &[\n");
    for (rust_name, names) in rows {
        let _ = writeln!(
            out,
            "    CLibRecord {{ rust_path: \"slate_support::{rust_name}\", aliases: &["
        );
        let mut groups: BTreeMap<BTreeSet<String>, Vec<String>> = BTreeMap::new();
        for (name, targets) in names {
            groups.entry(targets).or_default().push(name);
        }
        for (targets, names) in groups {
            let targets = targets.into_iter().collect::<Vec<_>>();
            let _ = writeln!(
                out,
                "        CLibAliases {{ names: &{names:?}, targets: &{targets:?} }},"
            );
        }
        out.push_str("    ] },\n");
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
        "{} target groups, {} accepted, {} excluded.\n",
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
        let _ = writeln!(
            out,
            "- `{}`: {kind} on {}",
            entry.name,
            entry.targets.iter().cloned().collect::<Vec<_>>().join(", ")
        );
        for note in &entry.notes {
            let _ = writeln!(out, "  - {note}");
        }
    }
    out.push_str("\n## Excluded\n\n");
    for entry in entries
        .iter()
        .filter(|e| matches!(e.outcome, Outcome::Excluded))
    {
        let _ = writeln!(
            out,
            "- `{}` on {}",
            entry.name,
            entry.targets.iter().cloned().collect::<Vec<_>>().join(", ")
        );
        for note in &entry.notes {
            let _ = writeln!(out, "  - {note}");
        }
    }
    out
}
