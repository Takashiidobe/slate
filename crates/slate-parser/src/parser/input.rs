use super::undo::UndoLog;
use crate::ast::{Comment, CommentAttach, CommentGroup, NodeId, Span, SpanRangeIndex, TypeName};
use crate::lexer::Token;
use crate::pp::{CommentFollower, CommentLayout, CommentPlacement, PPNode, PPNodeKind};
use std::cell::RefCell;
use std::collections::{BTreeMap, HashMap, HashSet};

#[derive(Clone)]
pub(super) enum Annotation {
    Comment(CommentGroup),
    Pragma(Vec<Span<Token>>),
}

pub(super) type Annotations = BTreeMap<usize, Vec<Span<Annotation>>>;

#[derive(Default)]
pub(super) struct ParserInput {
    pub tokens: Vec<Span<Token>>,
    pub(super) annotations: RefCell<Annotations>,
    annotation_undo: RefCell<UndoLog<(usize, Vec<Span<Annotation>>)>>,
    span_ranges: SpanRangeIndex,
    node_ranges: RefCell<HashMap<NodeId, TokenRange>>,
    type_ranges: RefCell<HashSet<TokenRange>>,
    type_names: RefCell<HashMap<TokenRange, TypeName>>,
}

pub(super) type TokenRange = (usize, usize);

impl ParserInput {
    pub fn new(nodes: Vec<PPNode>) -> Self {
        let mut input = Self::default();
        let annotations = input.annotations.get_mut();
        let mut open: Option<(CommentLayout, CommentLayout)> = None;
        for node in nodes {
            let layouts = open.take();
            match node.value {
                PPNodeKind::Code { tokens, .. } => input.tokens.extend(tokens),
                PPNodeKind::Comment {
                    ref text, layout, ..
                } => {
                    let entries = annotations.entry(input.tokens.len()).or_default();
                    if let Some((first, last)) = layouts
                        && joins(first, last, layout)
                        && let Some(previous) = entries.last_mut()
                        && previous.expansion.file == node.expansion.file
                        && let Annotation::Comment(group) = &mut previous.value
                    {
                        group.comment.text.push(text.clone());
                        group.comment.loc = group.comment.loc.through(node.expansion);
                        group.attach = attachment(first, layout);
                        previous.spelling = previous.spelling.through(node.spelling);
                        previous.expansion = previous.expansion.through(node.expansion);
                        open = Some((first, layout));
                    } else {
                        let group = CommentGroup {
                            comment: Comment {
                                text: vec![text.clone()],
                                loc: node.expansion,
                            },
                            attach: attachment(layout, layout),
                            doc: is_doc_comment(text),
                        };
                        entries.push(node.map(|_| Annotation::Comment(group)));
                        open = Some((layout, layout));
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
        input.span_ranges = SpanRangeIndex::new(&input.tokens);
        input
    }

    pub fn cover_tokens<T>(
        &self,
        value: T,
        tokens: &[Span<Token>],
        start: usize,
        end: usize,
    ) -> Span<T> {
        let Some(base) = self.position(tokens, 0) else {
            return match tokens.get(start..end) {
                Some(range) => Span::cover(value, range),
                None => Span::cover(value, &[] as &[Span<Token>]),
            };
        };
        let span = self
            .span_ranges
            .cover(value, &self.tokens, base + start, base + end);
        if self.starts_in_macro(base + start, base + end) {
            self.node_ranges
                .borrow_mut()
                .insert(span.id, (base + start, base + end));
        }
        span
    }

    fn starts_in_macro(&self, start: usize, end: usize) -> bool {
        start < end
            && self
                .tokens
                .get(start)
                .is_some_and(|token| token.macro_nesting.is_some())
    }

    pub fn note_type_range(&self, tokens: &[Span<Token>], start: usize, end: usize) {
        let Some(base) = self.position(tokens, 0) else {
            return;
        };
        if self.starts_in_macro(base + start, base + end) {
            self.type_ranges
                .borrow_mut()
                .insert((base + start, base + end));
        }
    }

    pub fn note_type_name(
        &self,
        tokens: &[Span<Token>],
        start: usize,
        end: usize,
        type_name: impl FnOnce() -> TypeName,
    ) {
        let Some(base) = self.position(tokens, 0) else {
            return;
        };
        let defines_or_computes = tokens[start..end]
            .iter()
            .any(|token| matches!(token.value, Token::LBrace | Token::LParen));
        if !defines_or_computes && self.starts_in_macro(base + start, base + end) {
            self.type_names
                .borrow_mut()
                .insert((base + start, base + end), type_name());
        }
    }

    pub(super) fn type_names(&self) -> std::cell::Ref<'_, HashMap<TokenRange, TypeName>> {
        self.type_names.borrow()
    }

    pub(super) fn node_ranges(&self) -> std::cell::Ref<'_, HashMap<NodeId, TokenRange>> {
        self.node_ranges.borrow()
    }

    pub(super) fn type_ranges(&self) -> std::cell::Ref<'_, HashSet<TokenRange>> {
        self.type_ranges.borrow()
    }

    pub fn has_token_slice(&self, tokens: &[Span<Token>]) -> bool {
        self.position(tokens, 0).is_some()
    }

    fn position(&self, tokens: &[Span<Token>], index: usize) -> Option<usize> {
        // macro-expanded tokens can share node ids, so locate the borrowed buffer instead.
        let address = tokens.as_ptr() as usize;
        let base = self.tokens.as_ptr() as usize;
        let end = base + self.tokens.len() * std::mem::size_of::<Span<Token>>();
        if address >= base
            && address <= end
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

    pub fn take_first_pragma(
        &self,
        tokens: &[Span<Token>],
        start: usize,
        matches: impl Fn(&[Span<Token>]) -> bool,
    ) -> Option<Span<Vec<Span<Token>>>> {
        let position = self.position(tokens, start)?;
        let mut annotations = self.annotations.borrow_mut();
        let entries = annotations.get_mut(&position)?;
        let index = entries.iter().position(
            |annotation| matches!(&annotation.value, Annotation::Pragma(tokens) if matches(tokens)),
        )?;
        self.annotation_undo
            .borrow_mut()
            .record(|| (position, entries.clone()));
        let taken = entries.remove(index);
        if entries.is_empty() {
            annotations.remove(&position);
        }
        match taken.value {
            Annotation::Pragma(ref tokens) => Some(taken.derive(tokens.clone())),
            Annotation::Comment(_) => None,
        }
    }

    pub(super) fn take_matching(
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
        let mut undo = self.annotation_undo.borrow_mut();
        let positions: Vec<_> = annotations
            .range(start..=end)
            .map(|(position, _)| *position)
            .collect();
        let mut result = Vec::new();
        for position in positions {
            let original = annotations.remove(&position).unwrap_or_default();
            undo.record(|| (position, original.clone()));
            let (taken, remaining): (Vec<_>, Vec<_>) = original
                .into_iter()
                .partition(|annotation| matches(&annotation.value));
            result.extend(taken);
            if !remaining.is_empty() {
                annotations.insert(position, remaining);
            }
        }
        result
    }

    pub fn mark_annotations(&self) -> usize {
        self.annotation_undo.borrow_mut().mark()
    }

    pub fn commit_annotations(&self) {
        self.annotation_undo.borrow_mut().commit();
    }

    pub fn rollback_annotations(&self, mark: usize) {
        let undone = self.annotation_undo.borrow_mut().rollback(mark);
        let mut annotations = self.annotations.borrow_mut();
        for (position, original) in undone {
            annotations.insert(position, original);
        }
    }

    pub fn replace_tokens(&mut self, tokens: Vec<Span<Token>>, positions: &[usize]) {
        let annotations = std::mem::take(self.annotations.get_mut());
        let remapped = self.annotations.get_mut();
        for (position, entries) in annotations {
            remapped
                .entry(positions[position])
                .or_default()
                .extend(entries);
        }
        self.tokens = tokens;
        self.span_ranges = SpanRangeIndex::new(&self.tokens);
    }

    pub fn drop_c23_va_start_extra_arguments(&mut self) {
        let mut dropped = vec![false; self.tokens.len()];
        let mut any = false;
        for start in 0..self.tokens.len() {
            let is_va_start = matches!(
                &self.tokens[start].value,
                Token::Ident(name) if name == "__builtin_c23_va_start"
            );
            if !is_va_start
                || self.tokens.get(start + 1).map(|token| &token.value) != Some(&Token::LParen)
            {
                continue;
            }
            let mut depth = 0usize;
            let mut comma = None;
            for (position, token) in self.tokens.iter().enumerate().skip(start + 2) {
                match token.value {
                    Token::LParen => depth += 1,
                    Token::RParen if depth == 0 => {
                        if let Some(comma) = comma {
                            dropped[comma..position].fill(true);
                            any = true;
                        }
                        break;
                    }
                    Token::RParen => depth -= 1,
                    Token::Comma if depth == 0 && comma.is_none() => comma = Some(position),
                    _ => {}
                }
            }
        }
        if !any {
            return;
        }
        let mut kept = Vec::with_capacity(self.tokens.len());
        let mut positions = Vec::with_capacity(self.tokens.len() + 1);
        for (token, dropped) in self.tokens.iter().zip(dropped) {
            positions.push(kept.len());
            if !dropped {
                kept.push(token.clone());
            }
        }
        positions.push(kept.len());
        self.replace_tokens(kept, &positions);
    }

    pub fn take_remaining(&self) -> Vec<Span<Annotation>> {
        std::mem::take(&mut *self.annotations.borrow_mut())
            .into_values()
            .flatten()
            .collect()
    }
}

fn joins(first: CommentLayout, last: CommentLayout, next: CommentLayout) -> bool {
    match last.follower {
        CommentFollower::SameLineComment => true,
        CommentFollower::NextLineComment => match first.placement {
            CommentPlacement::OwnLine => next.placement == CommentPlacement::OwnLine,
            CommentPlacement::AfterCode => {
                next.placement == CommentPlacement::OwnLine && next.column == first.column
            }
            CommentPlacement::Directive => false,
        },
        CommentFollower::Code | CommentFollower::Separated => false,
    }
}

fn attachment(first: CommentLayout, last: CommentLayout) -> CommentAttach {
    match (first.placement, last.follower) {
        (CommentPlacement::AfterCode, _) => CommentAttach::Trailing,
        (CommentPlacement::OwnLine, CommentFollower::Code) => CommentAttach::Leading,
        _ => CommentAttach::Detached,
    }
}

fn is_doc_comment(text: &str) -> bool {
    let marked = |prefix: &str, repeat: char| {
        text.strip_prefix(prefix)
            .is_some_and(|rest| !rest.starts_with(repeat) && rest != "/")
    };
    marked("/**", '*') || marked("///", '/') || text.starts_with("/*!") || text.starts_with("//!")
}
