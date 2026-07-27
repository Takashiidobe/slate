use crate::fixups::facts::{AnonymousStructFact, AnonymousStructFieldFact, FixupFacts};
use crate::rust_ast::{Item, Program};

pub(super) fn collect_facts(program: &Program, facts: &mut FixupFacts) {
    facts.anonymous_structs.clear();
    let mut next = 0usize;
    for item in &program.items {
        let Item::Record(record) = item else {
            continue;
        };
        if record.is_union || !record.name.starts_with("anon_") {
            continue;
        }
        facts.anonymous_structs.push(AnonymousStructFact {
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
        });
        next += 1;
    }
}
