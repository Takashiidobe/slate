use super::*;
use slate_parser::ast::Loc;

pub(super) fn comment(text: &[String]) -> rust::Comment {
    rust::Comment {
        lines: text
            .iter()
            .flat_map(|text| text.lines().map(str::to_owned))
            .collect(),
    }
}

impl FunctionLowerer<'_, '_> {
    pub(super) fn remaining_statement_comments(
        &mut self,
        body: &[Span<ir::Statement>],
    ) -> Vec<Stmt> {
        let mut comments = Vec::new();
        for statement in body {
            if let ir::Statement::Comment(text) = &statement.value
                && self.dependencies.emitted_comments.insert(statement.id)
            {
                comments.push(Stmt::Comment(comment(text)));
            }
            for child in control_flow::children(statement) {
                comments.extend(self.remaining_statement_comments(child));
            }
        }
        comments
    }

    pub(super) fn take_comments(&mut self, start: Loc, end: usize) -> Vec<rust::Comment> {
        self.tables
            .comments
            .iter()
            .filter(|&source| {
                source.expansion.file == start.file
                    && source.expansion.offset >= start.offset
                    && source.expansion.offset < end
                    && self.dependencies.emitted_comments.insert(source.id)
            })
            .map(|source| comment(&source.value))
            .collect()
    }
}

pub(super) fn module_comments(
    module: &ir::Module,
    tables: &Tables<'_>,
    emitted: &HashSet<slate_parser::ast::NodeId>,
    items: Vec<Item>,
) -> Vec<Item> {
    let origins: Vec<_> = items
        .iter()
        .enumerate()
        .filter_map(|(index, item)| {
            let loc = match item {
                Item::Fn(function) => module
                    .functions
                    .iter()
                    .find(|source| tables.names[&source.value.id].rust == function.name.as_str())
                    .map(|source| source.expansion),
                Item::Static { name, .. } => module
                    .globals
                    .iter()
                    .find(|source| tables.bindings.get(&source.variable.id) == Some(name))
                    .map(|source| source.expansion),
                Item::Record(record) => module
                    .types
                    .iter()
                    .find(|source| tables.record_names.get(&source.value.id) == Some(&record.name))
                    .map(|source| source.expansion),
                _ => None,
            }?;
            Some((index, loc))
        })
        .collect();
    let mut comments = BTreeMap::<usize, Vec<&Span<Vec<String>>>>::new();
    for source in &module.comments {
        if emitted.contains(&source.id) {
            continue;
        }
        let loc = source.expansion;
        let next = origins
            .iter()
            .filter(|(_, origin)| origin.file == loc.file && origin.offset >= loc.offset)
            .min_by_key(|(_, origin)| origin.offset)
            .map(|(index, _)| *index);
        let previous = origins
            .iter()
            .filter(|(_, origin)| origin.file == loc.file)
            .max_by_key(|(_, origin)| origin.offset)
            .map(|(index, _)| index + 1);
        comments
            .entry(next.or(previous).unwrap_or(items.len()))
            .or_default()
            .push(source);
    }
    for sources in comments.values_mut() {
        sources.sort_by_key(|source| (source.expansion.file.0, source.expansion.offset));
    }
    let mut output = Vec::new();
    for (index, item) in items.into_iter().enumerate() {
        if let Some(sources) = comments.remove(&index) {
            output.extend(
                sources
                    .into_iter()
                    .map(|source| Item::Comment(comment(&source.value))),
            );
        }
        output.push(item);
    }
    for sources in comments.into_values() {
        output.extend(
            sources
                .into_iter()
                .map(|source| Item::Comment(comment(&source.value))),
        );
    }
    output
}
