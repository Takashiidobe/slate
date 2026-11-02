use super::expression::Lowerer;
use super::numeric::ResolveError;
use super::operand::Operand;
use crate::ast::{BlockLiteral as AstBlock, Expr, ExprKind, NodeId};
use crate::ir::*;
use crate::visit::{self, Visitor};
use std::collections::HashMap;

struct ReferenceScan<'a> {
    references: &'a HashMap<NodeId, BindingId>,
    found: Vec<BindingId>,
}

impl Visitor for ReferenceScan<'_> {
    type Error = std::convert::Infallible;

    fn visit_expr(&mut self, expr: &Expr) -> Result<(), Self::Error> {
        if matches!(expr.value, ExprKind::Identifier(_))
            && let Some(id) = self.references.get(&expr.id)
            && !self.found.contains(id)
        {
            self.found.push(*id);
        }
        visit::walk_expr(self, expr)
    }
}

impl Lowerer {
    pub(super) fn block_literal(
        &mut self,
        e: &Expr,
        block: &AstBlock,
    ) -> Result<Operand, ResolveError> {
        let signature = *self
            .types
            .block_signatures
            .get(&e.id)
            .ok_or(ResolveError::Internal("block literal was not checked"))?;
        let (returned, ..) = self
            .types
            .ctypes
            .function_parts(signature)
            .ok_or(ResolveError::Internal("block signature"))?;
        let ty = self.types.object_type(signature, "void block type")?;
        let Type::Function { return_type, .. } = &ty else {
            return Err(ResolveError::Internal("block signature"));
        };
        let return_type = return_type.as_ref().map(|ty| (**ty).clone());
        let return_c = return_type.as_ref().map(|_| returned);
        let params = block
            .signature
            .declarator
            .function_parameters()
            .ok_or(ResolveError::Internal("missing block parameters"))?;
        let target = self.function_target.clone();
        let abi = Some(self.c_abi_signature(signature, &ty, None, &target)?);
        let enclosing = (
            std::mem::replace(&mut self.return_type, return_c),
            std::mem::take(&mut self.break_targets),
            std::mem::take(&mut self.continue_targets),
            std::mem::take(&mut self.switches),
            std::mem::take(&mut self.seh_leave_targets),
        );
        let mut prologue = Vec::new();
        let lowered = self
            .parameters(params, Some(&mut prologue))
            .and_then(|parameters| {
                let body = self.compound(|lower| lower.statements(&block.body, return_c))?;
                prologue.extend(body);
                Ok((parameters, prologue))
            });
        (
            self.return_type,
            self.break_targets,
            self.continue_targets,
            self.switches,
            self.seh_leave_targets,
        ) = enclosing;
        let (parameters, body) = lowered?;
        let captures = self.block_captures(e.id, block);
        let id = self.fresh();
        let function = Function {
            id,
            name: format!("block_{}", id.0),
            parameters,
            return_type: return_type.clone(),
            abi,
            linkage: Linkage::Internal,
            symbol: SymbolAttributes::default(),
            semantics: Default::default(),
            body: Some(body),
            fallthrough: Some(if return_type.is_none() {
                Fallthrough::ReturnVoid
            } else {
                Fallthrough::UndefinedIfUsed
            }),
        };
        let c = self.types.ctypes.block_pointer(signature);
        Ok(self.operand(
            e,
            c,
            ValueKind::Block(Box::new(BlockLiteral { function, captures })),
        ))
    }

    fn block_captures(&self, node: NodeId, block: &AstBlock) -> Vec<BlockCapture> {
        let Some(range) = self.names.block_ranges.get(&node) else {
            return Vec::new();
        };
        let mut scan = ReferenceScan {
            references: &self.types.references,
            found: Vec::new(),
        };
        let Ok(()) = visit::walk_stmts(&mut scan, &block.body);
        scan.found
            .into_iter()
            .filter(|id| id.0 < range.start && self.is_local_object(*id))
            .map(|id| BlockCapture {
                id,
                by_reference: self.types.block_variables.contains(&id),
            })
            .collect()
    }

    fn is_local_object(&self, id: BindingId) -> bool {
        let object = self
            .names
            .bindings
            .get(id.0 as usize)
            .is_some_and(|binding| {
                matches!(binding.kind, BindingKind::Object | BindingKind::Parameter)
            });
        object
            && self.types.entities.linkage(id).is_none()
            && !matches!(
                self.types.entities.storage(id),
                Some(StorageDuration::Static | StorageDuration::Thread)
            )
    }
}
