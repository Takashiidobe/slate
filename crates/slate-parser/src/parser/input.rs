use crate::ast::{Comment, CommentGroup, Span};
use crate::lexer::Token;
use crate::pp::{PPNode, PPNodeKind};
use std::cell::RefCell;
use std::collections::BTreeMap;

#[derive(Clone)]
pub(super) enum Annotation {
    Comment(CommentGroup),
    Pragma(Vec<Span<Token>>),
}

#[derive(Default)]
pub(super) struct ParserInput {
    pub tokens: Vec<Span<Token>>,
    annotations: RefCell<BTreeMap<usize, Vec<Span<Annotation>>>>,
}

impl ParserInput {
    pub fn new(nodes: Vec<PPNode>) -> Self {
        let mut input = Self::default();
        let annotations = input.annotations.get_mut();
        for node in nodes {
            match node.value {
                PPNodeKind::Code { tokens, .. } => input.tokens.extend(tokens),
                PPNodeKind::Comment { ref text, .. } => {
                    let entries = annotations.entry(input.tokens.len()).or_default();
                    if let Some(previous) = entries.last_mut()
                        && previous.expansion.file == node.expansion.file
                        && let Annotation::Comment(group) = &mut previous.value
                    {
                        group.comment.text.push(text.clone());
                        group.comment.loc = group.comment.loc.through(node.expansion);
                        previous.spelling = previous.spelling.through(node.spelling);
                        previous.expansion = previous.expansion.through(node.expansion);
                    } else {
                        let group = CommentGroup {
                            comment: Comment {
                                text: vec![text.clone()],
                                loc: node.expansion,
                            },
                        };
                        entries.push(node.map(|_| Annotation::Comment(group)));
                    }
                }
                PPNodeKind::Pragma { ref tokens, .. } => {
                    let tokens = tokens.clone();
                    annotations
                        .entry(input.tokens.len())
                        .or_default()
                        .push(node.map(|_| Annotation::Pragma(tokens)));
                }
            }
        }
        input
    }

    fn position(&self, tokens: &[Span<Token>], index: usize) -> Option<usize> {
        // macro-expanded tokens can share node ids, so locate the borrowed buffer instead.
        let address = tokens.as_ptr() as usize;
        let base = self.tokens.as_ptr() as usize;
        let end = base + self.tokens.len() * std::mem::size_of::<Span<Token>>();
        if address >= base && address <= end
            && index <= tokens.len()
            && std::mem::size_of_val(tokens) <= end - address
        {
            return Some((address - base) / std::mem::size_of::<Span<Token>>() + index);
        }
        None
    }

    pub fn take(&self, tokens: &[Span<Token>], start: usize, end: usize) -> Vec<Span<Annotation>> {
        self.take_matching(tokens, start, end, |_| true)
    }

    pub fn take_comments(
        &self,
        tokens: &[Span<Token>],
        start: usize,
        end: usize,
    ) -> Vec<Span<Annotation>> {
        self.take_matching(tokens, start, end, |annotation| {
            matches!(annotation, Annotation::Comment(_))
        })
    }

    fn take_matching(
        &self,
        tokens: &[Span<Token>],
        start: usize,
        end: usize,
        matches: impl Fn(&Annotation) -> bool,
    ) -> Vec<Span<Annotation>> {
        let Some(start) = self.position(tokens, start) else {
            return Vec::new();
        };
        let Some(end) = self.position(tokens, end) else {
            return Vec::new();
        };
        if start > end {
            return Vec::new();
        }
        let mut annotations = self.annotations.borrow_mut();
        let positions: Vec<_> = annotations
            .range(start..=end)
            .map(|(position, _)| *position)
            .collect();
        let mut result = Vec::new();
        for position in positions {
            let (taken, remaining): (Vec<_>, Vec<_>) = annotations
                .remove(&position)
                .unwrap_or_default()
                .into_iter()
                .partition(|annotation| matches(&annotation.value));
            result.extend(taken);
            if !remaining.is_empty() {
                annotations.insert(position, remaining);
            }
        }
        result
    }

    pub fn take_remaining(&self) -> Vec<Span<Annotation>> {
        std::mem::take(&mut *self.annotations.borrow_mut())
            .into_values()
            .flatten()
            .collect()
    }
}
