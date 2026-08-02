use std::collections::{BTreeMap, BTreeSet};

use crate::fixups::trace::Pass;

use super::super::{Definition, DefinitionKind, EditSet, Field, QueryRule, ReferenceDomain};

fn type_definition_matcher() -> Definition {
    Definition {
        kind: Field::predicate(|kind: &DefinitionKind, _: &()| {
            matches!(
                kind,
                DefinitionKind::Struct | DefinitionKind::Record | DefinitionKind::Enum
            )
        }),
        ..Default::default()
    }
}

pub(in crate::fixups) fn rewrite() -> QueryRule<Definition> {
    QueryRule::new(
        Pass::UnusedItems,
        "prune_unused_type_definition",
        type_definition_matcher(),
    )
    .case("unreachable", |case, definition| {
        let domain = case.fact(|query| query.reference_domain())?;
        let live = live_type_definitions(&domain);
        case.require(!live.contains(&definition.location.item_index()))?;
        Ok(EditSet::delete_definition(definition.clone()))
    })
}

fn live_type_definitions(domain: &ReferenceDomain) -> BTreeSet<usize> {
    let candidates = domain
        .definitions
        .iter()
        .filter(|definition| {
            matches!(
                definition.kind,
                DefinitionKind::Struct | DefinitionKind::Record | DefinitionKind::Enum
            )
        })
        .map(|definition| (definition.location.item_index(), definition))
        .collect::<BTreeMap<_, _>>();
    let symbols = candidates
        .iter()
        .flat_map(|(item_index, definition)| {
            definition
                .symbols
                .iter()
                .map(move |symbol| (symbol.as_str(), *item_index))
        })
        .collect::<BTreeMap<_, _>>();
    let references = domain
        .items
        .iter()
        .map(|item| (item.item_index, &item.symbols))
        .collect::<BTreeMap<_, _>>();
    let mut live = BTreeSet::new();
    for item in &domain.items {
        if !candidates.contains_key(&item.item_index) {
            mark_referenced_types(&item.symbols, &symbols, &mut live);
        }
    }
    let mut pending = live.iter().copied().collect::<Vec<_>>();
    while let Some(item_index) = pending.pop() {
        let before = live.len();
        if let Some(item_references) = references.get(&item_index) {
            mark_referenced_types(item_references, &symbols, &mut live);
        }
        if live.len() != before {
            pending = live.iter().copied().collect();
        }
    }
    live
}

fn mark_referenced_types(
    references: &BTreeSet<String>,
    symbols: &BTreeMap<&str, usize>,
    live: &mut BTreeSet<usize>,
) {
    for reference in references {
        if let Some(item_index) = symbols.get(reference.as_str()) {
            live.insert(*item_index);
        }
    }
}
