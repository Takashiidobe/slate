use crate::ast::*;
use crate::visit::{self, Visitor};
use std::collections::HashMap;
use std::convert::Infallible;

#[derive(Clone, Copy, PartialEq, Eq)]
pub(super) struct SehScope {
    pub(super) node: NodeId,
    pub(super) handler: bool,
    pub(super) finally: bool,
}

pub(super) fn label_scopes(body: &[Stmt]) -> HashMap<String, Vec<SehScope>> {
    let mut collector = Collector::default();
    visit::walk_stmts(&mut collector, body).unwrap_or_else(|never| match never {});
    collector.labels
}

#[derive(Default)]
struct Collector {
    labels: HashMap<String, Vec<SehScope>>,
    scopes: Vec<SehScope>,
}

impl Collector {
    fn within(&mut self, scope: SehScope, body: &Stmt) -> Result<(), Infallible> {
        self.scopes.push(scope);
        let result = self.visit_stmt(body);
        self.scopes.pop();
        result
    }
}

impl Visitor for Collector {
    type Error = Infallible;

    fn visit_stmt(&mut self, stmt: &Stmt) -> Result<(), Self::Error> {
        match &stmt.value {
            StmtKind::NestedFunction(_) => Ok(()),
            StmtKind::Labeled { label, .. } => {
                self.labels.insert(label.value.clone(), self.scopes.clone());
                visit::walk_stmt(self, stmt)
            }
            StmtKind::SehTry { body, handler } => {
                let scope = |handler, finally| SehScope {
                    node: stmt.id,
                    handler,
                    finally,
                };
                self.within(scope(false, false), body)?;
                match handler {
                    SehHandler::Except { filter, body } => {
                        self.visit_expr(filter)?;
                        self.within(scope(true, false), body)
                    }
                    SehHandler::Finally(body) => self.within(scope(true, true), body),
                }
            }
            _ => visit::walk_stmt(self, stmt),
        }
    }
}
