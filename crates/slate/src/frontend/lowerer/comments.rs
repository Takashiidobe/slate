use super::*;
use slate_parser::ast::{CommentAttach, FileId, NodeId};

pub(super) fn comment(source: &ir::Comment) -> rust::Comment {
    lowered(source, false)
}

fn lowered(source: &ir::Comment, doc: bool) -> rust::Comment {
    rust::Comment {
        lines: comment_text::normalize(&source.text),
        attach: match source.attach {
            CommentAttach::Leading => rust::CommentAttach::Leading,
            CommentAttach::Trailing => rust::CommentAttach::Trailing,
            CommentAttach::Detached => rust::CommentAttach::Detached,
        },
        doc: doc || source.doc,
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
        let promote = self.tables.promote_docs;
        owned
            .leading
            .iter()
            .chain(&owned.trailing)
            .map(|source| lowered(source, promote))
            .filter(|comment| !comment.lines.is_empty())
            .collect()
    }

    pub(super) fn claim_inner_comments(&self, owner: NodeId) -> Vec<rust::Comment> {
        self.tables
            .comments
            .owned
            .get(&owner)
            .map(|owned| {
                owned
                    .inner
                    .iter()
                    .map(|source| comment(source))
                    .filter(|comment| !comment.lines.is_empty())
                    .collect()
            })
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
    let rank: HashMap<NodeId, usize> = module
        .comments
        .order
        .iter()
        .enumerate()
        .map(|(position, (node, _))| (*node, position))
        .collect();
    let slots: Vec<usize> = nodes
        .iter()
        .enumerate()
        .filter(|(_, node)| node.is_some_and(|node| rank.contains_key(&node)))
        .map(|(index, _)| index)
        .collect();
    let mut sorted = slots.clone();
    sorted.sort_by_key(|&index| rank[&nodes[index].expect("ranked item has a node")]);
    let mut items: Vec<Option<Item>> = items.into_iter().map(Some).collect();
    let mut nodes = nodes;
    let moved: Vec<(Option<Item>, Option<NodeId>)> = sorted
        .iter()
        .map(|&index| (items[index].take(), nodes[index]))
        .collect();
    for (&slot, (item, node)) in slots.iter().zip(moved) {
        items[slot] = item;
        nodes[slot] = node;
    }
    let items: Vec<Item> = items.into_iter().flatten().collect();
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
    let mut first_offsets = HashMap::<FileId, usize>::new();
    for source in module
        .comments
        .detached
        .iter()
        .map(|detached| &detached.comment)
        .chain(
            module
                .comments
                .owned
                .values()
                .flat_map(|owned| owned.iter()),
        )
    {
        let loc = source.expansion;
        let first = first_offsets.entry(loc.file).or_insert(loc.offset);
        *first = (*first).min(loc.offset);
    }
    let mut prologue = Vec::new();
    let mut placed = BTreeMap::<usize, Vec<rust::Comment>>::new();
    for detached in &module.comments.detached {
        let loc = detached.comment.expansion;
        let lowered = comment(&detached.comment.value);
        if first_offsets.get(&loc.file) == Some(&loc.offset)
            && !order[..detached.position]
                .iter()
                .any(|(_, file)| *file == loc.file)
        {
            prologue.push(rust::Comment {
                attach: rust::CommentAttach::Prologue,
                doc: true,
                ..lowered
            });
            continue;
        }
        placed
            .entry(slot(detached.position, loc.file))
            .or_default()
            .push(lowered);
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
    let prologue_after = usize::from(matches!(items.first(), Some(Item::CrateAttrs(_))));
    let mut prologue = Some(prologue);
    for (index, item) in items.into_iter().enumerate() {
        if index == prologue_after {
            output.extend(prologue.take().into_iter().flatten().map(Item::Comment));
        }
        if let Some(comments) = placed.remove(&index) {
            output.extend(comments.into_iter().map(Item::Comment));
        }
        output.push(item);
    }
    output.extend(prologue.into_iter().flatten().map(Item::Comment));
    for comments in placed.into_values() {
        output.extend(comments.into_iter().map(Item::Comment));
    }
    output.retain(|item| !matches!(item, Item::Comment(comment) if comment.lines.is_empty()));
    output
}
