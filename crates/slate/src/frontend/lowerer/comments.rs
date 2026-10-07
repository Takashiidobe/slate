use super::*;
use slate_parser::ast::{CommentAttach, FileId, NodeId};

pub(super) fn comment(source: &ir::Comment) -> rust::Comment {
    rust::Comment {
        lines: source
            .text
            .iter()
            .flat_map(|text| text.lines().map(str::to_owned))
            .collect(),
        attach: match source.attach {
            CommentAttach::Leading => rust::CommentAttach::Leading,
            CommentAttach::Trailing => rust::CommentAttach::Trailing,
            CommentAttach::Detached => rust::CommentAttach::Detached,
        },
        doc: source.doc,
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

    pub(super) fn claim_comments(&mut self, owner: NodeId) -> Vec<rust::Comment> {
        let Some(owned) = self.tables.comments.owned.get(&owner) else {
            return Vec::new();
        };
        self.dependencies.emitted_comments.insert(owner);
        owned
            .leading
            .iter()
            .chain(&owned.trailing)
            .map(|source| comment(source))
            .collect()
    }

    pub(super) fn claim_inner_comments(&self, owner: NodeId) -> Vec<rust::Comment> {
        self.tables
            .comments
            .owned
            .get(&owner)
            .map(|owned| owned.inner.iter().map(|source| comment(source)).collect())
            .unwrap_or_default()
    }
}

pub(super) fn module_comments(
    module: &ir::Module,
    tables: &Tables<'_>,
    emitted: &HashSet<NodeId>,
    items: Vec<Item>,
) -> Vec<Item> {
    let nodes: Vec<Option<NodeId>> = items
        .iter()
        .map(|item| match item {
            Item::Fn(function) => module
                .functions
                .iter()
                .find(|source| tables.names[&source.value.id].rust == function.name.as_str())
                .map(|source| source.id),
            Item::Static { name, .. } => module
                .globals
                .iter()
                .find(|source| tables.bindings.get(&source.variable.id) == Some(name))
                .map(|source| source.id),
            Item::Record(record) => module
                .types
                .iter()
                .find(|source| tables.record_names.get(&source.value.id) == Some(&record.name))
                .map(|source| source.id),
            _ => None,
        })
        .collect();
    let index_of: HashMap<NodeId, usize> = nodes
        .iter()
        .enumerate()
        .filter_map(|(index, node)| Some((((*node)?), index)))
        .collect();
    let order = &module.comments.order;
    let slot = |position: usize, file: FileId| -> usize {
        let emitted_in_file = |(node, node_file): &(NodeId, FileId)| {
            index_of.get(node).filter(|_| *node_file == file).copied()
        };
        order[position..]
            .iter()
            .find_map(emitted_in_file)
            .or_else(|| {
                order[..position]
                    .iter()
                    .rev()
                    .find_map(emitted_in_file)
                    .map(|index| index + 1)
            })
            .unwrap_or(0)
    };
    let mut placed = BTreeMap::<usize, Vec<rust::Comment>>::new();
    for detached in &module.comments.detached {
        let file = detached.comment.expansion.file;
        placed
            .entry(slot(detached.position, file))
            .or_default()
            .push(comment(&detached.comment.value));
    }
    for (position, (node, file)) in order.iter().enumerate() {
        if emitted.contains(node) {
            continue;
        }
        let Some(owned) = module.comments.owned.get(node) else {
            continue;
        };
        placed.entry(slot(position, *file)).or_default().extend(
            owned
                .leading
                .iter()
                .chain(&owned.trailing)
                .chain(&owned.inner)
                .map(|source| comment(&source.value)),
        );
    }
    let mut output = Vec::new();
    for (index, item) in items.into_iter().enumerate() {
        if let Some(comments) = placed.remove(&index) {
            output.extend(comments.into_iter().map(Item::Comment));
        }
        output.push(item);
    }
    for comments in placed.into_values() {
        output.extend(comments.into_iter().map(Item::Comment));
    }
    output
}
