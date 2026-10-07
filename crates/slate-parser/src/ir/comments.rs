use super::Comment;
use crate::ast::{CommentAttach, CommentGroup, FileId, NodeId, Span};
use std::collections::HashMap;

#[derive(Debug, Clone, Default)]
pub struct Comments {
    pub owned: HashMap<NodeId, OwnedComments>,
    pub detached: Vec<Detached>,
    pub order: Vec<(NodeId, FileId)>,
}

#[derive(Debug, Clone, Default)]
pub struct OwnedComments {
    pub leading: Vec<Span<Comment>>,
    pub trailing: Vec<Span<Comment>>,
    pub inner: Vec<Span<Comment>>,
}

impl OwnedComments {
    pub fn is_empty(&self) -> bool {
        self.leading.is_empty() && self.trailing.is_empty() && self.inner.is_empty()
    }

    pub fn iter(&self) -> impl Iterator<Item = &Span<Comment>> {
        self.leading.iter().chain(&self.trailing).chain(&self.inner)
    }
}

#[derive(Debug, Clone)]
pub struct Detached {
    pub comment: Span<Comment>,
    pub position: usize,
}

pub enum Sibling {
    Comment(Span<Comment>),
    Node { first: NodeId, last: NodeId },
    Skipped,
}

impl Comments {
    pub fn own(&mut self, id: NodeId, comments: impl IntoIterator<Item = Span<Comment>>) {
        for comment in comments {
            let owned = self.owned.entry(id).or_default();
            match comment.attach {
                CommentAttach::Trailing => owned.trailing.push(comment),
                CommentAttach::Leading | CommentAttach::Detached => owned.leading.push(comment),
            }
        }
    }

    pub fn own_groups(&mut self, id: NodeId, groups: &[Span<CommentGroup>]) {
        self.own(
            id,
            groups
                .iter()
                .map(|group| group.derive(Comment::from(&group.value))),
        );
    }

    pub fn distribute(&mut self, siblings: Vec<Sibling>) -> Vec<(usize, Span<Comment>)> {
        let count = siblings.len();
        let mut unowned = Vec::new();
        let mut pending = Vec::new();
        let mut previous = None;
        for (index, sibling) in siblings.into_iter().enumerate() {
            match sibling {
                Sibling::Comment(comment) => match (comment.attach, previous) {
                    (CommentAttach::Leading, _) => pending.push(comment),
                    (CommentAttach::Trailing, Some(owner)) => {
                        self.owned.entry(owner).or_default().trailing.push(comment);
                    }
                    _ => unowned.push((index, comment)),
                },
                Sibling::Node { first, last } => {
                    if !pending.is_empty() {
                        self.owned
                            .entry(first)
                            .or_default()
                            .leading
                            .append(&mut pending);
                    }
                    previous = Some(last);
                }
                Sibling::Skipped => previous = None,
            }
        }
        unowned.extend(pending.into_iter().map(|comment| (count, comment)));
        unowned
    }
}
