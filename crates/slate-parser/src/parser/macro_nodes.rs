use super::input::{ParserInput, TokenRange};
use crate::ast::*;
use crate::lexer::Token;
use crate::visit::{self, Visitor};
use std::collections::{HashMap, HashSet};
use std::convert::Infallible;

#[derive(Clone, Copy, PartialEq, Eq)]
struct RunKey {
    expansion: MacroExpansionId,
    argument: Option<usize>,
}

#[derive(Clone, Copy)]
enum NodeKind {
    Expr,
    Stmt,
    Decl,
}

#[derive(Clone, Copy)]
struct IndexedStatement {
    id: NodeId,
    end: usize,
    block: Option<NodeId>,
}

struct NodeIndex<'i> {
    ranges: &'i HashMap<NodeId, TokenRange>,
    nodes: HashMap<TokenRange, (NodeId, NodeKind)>,
    statements: HashMap<usize, IndexedStatement>,
    callees: HashSet<NodeId>,
    call_arguments: HashMap<TokenRange, NodeId>,
    block: Option<NodeId>,
}

impl NodeIndex<'_> {
    fn record(&mut self, id: NodeId, kind: NodeKind) -> Option<TokenRange> {
        let range = *self.ranges.get(&id)?;
        self.nodes.entry(range).or_insert((id, kind));
        Some(range)
    }
}

impl Visitor for NodeIndex<'_> {
    type Error = Infallible;

    fn visit_decl(&mut self, decl: &Decl) -> Result<(), Infallible> {
        self.record(decl.id, NodeKind::Decl);
        self.block = matches!(decl.value, DeclKind::Function(_)).then_some(decl.id);
        visit::walk_decl(self, decl)
    }

    fn visit_stmt(&mut self, stmt: &Stmt) -> Result<(), Infallible> {
        if let Some((start, end)) = self.record(stmt.id, NodeKind::Stmt) {
            self.statements.entry(start).or_insert(IndexedStatement {
                id: stmt.id,
                end,
                block: self.block,
            });
        }
        let inner = matches!(stmt.value, StmtKind::Block(_)).then_some(stmt.id);
        let outer = std::mem::replace(&mut self.block, inner);
        let walked = visit::walk_stmt(self, stmt);
        self.block = outer;
        walked
    }

    fn visit_expr(&mut self, expr: &Expr) -> Result<(), Infallible> {
        self.record(expr.id, NodeKind::Expr);
        if let ExprKind::Call { callee, .. } = &expr.value {
            if let (Some(&(_, open)), Some(&(_, close))) =
                (self.ranges.get(&callee.id), self.ranges.get(&expr.id))
            {
                self.call_arguments.entry((open, close)).or_insert(expr.id);
            }
            let mut current = callee;
            self.callees.insert(current.id);
            while let ExprKind::Paren(inner) = &current.value {
                current = inner;
                self.callees.insert(current.id);
            }
        }
        visit::walk_expr(self, expr)
    }
}

pub(super) fn map_macro_nodes(
    input: &ParserInput,
    unit: &TranslationUnit,
    macros: &mut MacroInvocations,
) {
    if macros.expansions.is_empty() {
        return;
    }
    let ranges = input.node_ranges();
    let mut index = NodeIndex {
        ranges: &ranges,
        nodes: HashMap::new(),
        statements: HashMap::new(),
        callees: HashSet::new(),
        call_arguments: HashMap::new(),
        block: None,
    };
    let Ok(()) = index.visit_translation_unit(unit);
    let types = input.type_ranges();
    let classifier = Classifier {
        tokens: &input.tokens,
        index: &index,
        types: &types,
    };
    let mut runs = token_runs(&input.tokens);
    runs.sort_by_key(|(key, (start, _))| (key.argument, *start));
    let mut expansion_runs = vec![Vec::new(); macros.expansions.len()];
    for (key, range) in runs {
        match key.argument {
            None => expansion_runs[key.expansion.0 as usize].push(range),
            Some(argument) => macros.expansions[key.expansion.0 as usize]
                .argument_uses
                .push(MacroArgumentUse {
                    argument,
                    cover: classifier.classify(range),
                }),
        }
    }
    for (expansion, runs) in macros.expansions.iter_mut().zip(expansion_runs) {
        expansion.cover = match runs.as_slice() {
            [] => None,
            [only] => Some(classifier.classify(*only)),
            _ => Some(MacroCover::Split),
        };
    }
}

fn token_runs(tokens: &[Span<Token>]) -> Vec<(RunKey, TokenRange)> {
    let mut runs = Vec::new();
    let mut open: Vec<(RunKey, usize)> = Vec::new();
    let mut current: Vec<RunKey> = Vec::new();
    let mut previous: Option<&std::sync::Arc<MacroNesting>> = None;
    for (position, token) in tokens.iter().enumerate() {
        let nesting = token.macro_nesting.as_ref();
        let unchanged = match (previous, nesting) {
            (None, None) => true,
            (Some(left), Some(right)) => std::sync::Arc::ptr_eq(left, right),
            _ => false,
        };
        if unchanged {
            continue;
        }
        previous = nesting;
        current.clear();
        for link in nesting.iter().flat_map(|nesting| nesting.iter()) {
            let whole = RunKey {
                expansion: link.expansion,
                argument: None,
            };
            if !current.contains(&whole) {
                current.push(whole);
            }
            if link.argument.is_some() {
                current.push(RunKey {
                    expansion: link.expansion,
                    argument: link.argument,
                });
            }
        }
        open.retain(|(key, start)| {
            let stays = current.contains(key);
            if !stays {
                runs.push((*key, (*start, position)));
            }
            stays
        });
        for key in &current {
            if !open.iter().any(|(existing, _)| existing == key) {
                open.push((*key, position));
            }
        }
    }
    runs.extend(
        open.iter()
            .map(|(key, start)| (*key, (*start, tokens.len()))),
    );
    runs
}

struct Classifier<'c> {
    tokens: &'c [Span<Token>],
    index: &'c NodeIndex<'c>,
    types: &'c HashSet<TokenRange>,
}

impl Classifier<'_> {
    fn classify(&self, range: TokenRange) -> MacroCover {
        if let Some(&(id, kind)) = self.index.nodes.get(&range) {
            return match kind {
                NodeKind::Expr if self.index.callees.contains(&id) => MacroCover::Callee(id),
                NodeKind::Expr => MacroCover::Expr(id),
                NodeKind::Stmt => MacroCover::Statements(vec![id]),
                NodeKind::Decl => MacroCover::Decl(id),
            };
        }
        if let Some(&id) = self.index.call_arguments.get(&range) {
            return MacroCover::CallArguments(id);
        }
        if self.types.contains(&range) {
            return MacroCover::Type;
        }
        let closed = self.closed_by_semicolon(range);
        if let Some((_, end)) = closed
            && let Some(&(id, NodeKind::Decl)) = self.index.nodes.get(&(range.0, end))
        {
            return MacroCover::Decl(id);
        }
        if let Some(statements) = self.statements_tiling(range, closed.is_some()) {
            return MacroCover::Statements(statements);
        }
        if range.1 == range.0 + 1 && matches!(self.tokens[range.0].value, Token::Ident(_)) {
            return MacroCover::Name;
        }
        MacroCover::Split
    }

    fn closed_by_semicolon(&self, (start, end): TokenRange) -> Option<TokenRange> {
        matches!(self.tokens.get(end)?.value, Token::Semi).then_some((start, end + 1))
    }

    fn statements_tiling(&self, (start, end): TokenRange, closed: bool) -> Option<Vec<NodeId>> {
        let limit = end + usize::from(closed);
        let mut statements = Vec::new();
        let mut block = None;
        let mut position = start;
        while position < end {
            let statement = self.index.statements.get(&position)?;
            if statement.end > limit {
                return None;
            }
            if !statements.is_empty() && (statement.block.is_none() || statement.block != block) {
                return None;
            }
            block = statement.block;
            statements.push(statement.id);
            position = statement.end;
        }
        (!statements.is_empty()).then_some(statements)
    }
}
