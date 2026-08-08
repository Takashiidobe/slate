use crate::fixups::facts::{AnonymousStructFact, AnonymousStructFieldFact};
use crate::rust_ast::RecordDef;
pub(in crate::fixups) fn collect<'a>(
    records: impl Iterator<Item = &'a RecordDef>,
) -> Vec<AnonymousStructFact> {
    records
        .filter(|record| !record.is_union && record.name.starts_with("anon_"))
        .enumerate()
        .map(|(next, record)| AnonymousStructFact {
            original_name: record.name.clone(),
            generated_name: format!("__slate_anonymous_struct_{next}"),
            fields: record
                .fields
                .iter()
                .map(|field| AnonymousStructFieldFact {
                    name: field.name.as_str().to_string(),
                    ty: field.ty.clone(),
                })
                .collect(),
        })
        .collect()
}
