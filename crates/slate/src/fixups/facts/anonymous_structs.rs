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

#[cfg(test)]
mod tests {
    use super::*;
    use crate::rust_ast::{Ident, Prim, Program, RecordDef, RecordField, Type, Visibility};

    #[test]
    fn records_anonymous_struct_field_order() {
        let program = Program {
            items: vec![Item::Record(RecordDef {
                comments: vec![],
                vis: Visibility::Private,
                field_vis: Visibility::Private,
                is_union: false,
                allow_non_camel_case: false,
                name: "anon_0".into(),
                packed: false,
                align: None,
                fields: vec![
                    RecordField {
                        comments: vec![],
                        name: Ident::from("x"),
                        ty: Type::Prim(Prim::I32),
                    },
                    RecordField {
                        comments: vec![],
                        name: Ident::from("y"),
                        ty: Type::Prim(Prim::I32),
                    },
                ],
            })],
        };
        let mut facts = FixupFacts::default();

        collect_facts(&program, &mut facts);

        assert_eq!(facts.anonymous_structs.len(), 1);
        assert_eq!(facts.anonymous_structs[0].original_name, "anon_0");
        assert_eq!(
            facts.anonymous_structs[0].generated_name,
            "__slate_anonymous_struct_0"
        );
        assert_eq!(facts.anonymous_structs[0].fields[0].name, "x");
        assert_eq!(facts.anonymous_structs[0].fields[1].name, "y");
    }
}
