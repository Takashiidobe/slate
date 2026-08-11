use std::collections::{BTreeMap, BTreeSet};

use crate::backend::facts::StructFieldOwnershipFact;
use crate::backend::rust_ast::{RecordDef, Type};
pub(in crate::backend) fn collect<'a>(
    records: impl Iterator<Item = &'a RecordDef>,
) -> Vec<StructFieldOwnershipFact> {
    let mut candidates: Vec<(String, String, String)> = Vec::new();
    for record in records {
        for field in &record.fields {
            if let Some(pointee) = pointer_to_record(&field.ty) {
                candidates.push((
                    record.name.clone(),
                    field.name.as_str().to_string(),
                    pointee,
                ));
            }
        }
    }

    let mut self_field_count: BTreeMap<String, usize> = BTreeMap::new();
    for (record_name, _field_name, pointee) in &candidates {
        if pointee == record_name {
            *self_field_count.entry(record_name.clone()).or_insert(0) += 1;
        }
    }

    let mut edges: BTreeMap<String, BTreeSet<String>> = BTreeMap::new();
    for (record_name, _field_name, pointee) in &candidates {
        edges
            .entry(record_name.clone())
            .or_default()
            .insert(pointee.clone());
    }

    candidates
        .into_iter()
        .map(|(record_name, field_name, pointee)| {
            let tree_eligible = if pointee == record_name {
                self_field_count.get(&record_name).copied().unwrap_or(0) == 1
            } else {
                !reaches(&edges, &pointee, &record_name, &mut BTreeSet::new())
            };
            StructFieldOwnershipFact {
                record_name,
                field_name,
                tree_eligible,
            }
        })
        .collect()
}

fn reaches(
    edges: &BTreeMap<String, BTreeSet<String>>,
    from: &str,
    target: &str,
    visited: &mut BTreeSet<String>,
) -> bool {
    if from == target {
        return true;
    }
    if !visited.insert(from.to_string()) {
        return false;
    }
    edges
        .get(from)
        .is_some_and(|next| next.iter().any(|n| reaches(edges, n, target, visited)))
}

fn pointer_to_record(ty: &Type) -> Option<String> {
    match ty {
        Type::Ptr { inner, .. } => match &**inner {
            Type::Custom(name) => Some(name.clone()),
            _ => None,
        },
        _ => None,
    }
}
