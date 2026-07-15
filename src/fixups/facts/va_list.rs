use crate::fixups::facts::{
    AstPath, BindingId, BindingKind, FixupFacts, FunctionId, PathSegment, VaListAliasFact,
};
use crate::fixups::support::walk;
use crate::rust_ast::{Expr, Ident, IndentStmt, Item, Program, Stmt, Type};

pub(in crate::fixups) fn collect_facts(program: &Program, facts: &mut FixupFacts) {
    facts.va_list_aliases.clear();
    let mut all = Vec::new();
    for (item_index, item) in program.items.iter().enumerate() {
        let Item::Fn(f) = item else {
            continue;
        };
        let Some(function) = facts.function_by_item_index(item_index) else {
            continue;
        };
        let variadic_params = f
            .params
            .iter()
            .enumerate()
            .filter(|(_, param)| matches!(param.ty, Type::Variadic))
            .collect::<Vec<_>>();
        let [(param_index, param)] = variadic_params.as_slice() else {
            continue;
        };
        let Some(param_binding) = facts.binding_by_param_index(function, *param_index) else {
            continue;
        };
        let mut collector = Collector {
            function,
            facts,
            param_name: Ident::new(param.name.as_str()),
            param_binding,
            aliases: Vec::new(),
        };
        collector.body(&f.body, &mut Vec::new());
        all.extend(collector.aliases);
    }
    facts.va_list_aliases = all;
}

struct Collector<'a> {
    function: FunctionId,
    facts: &'a FixupFacts,
    param_name: Ident,
    param_binding: BindingId,
    aliases: Vec<VaListAliasFact>,
}

impl Collector<'_> {
    fn body(&mut self, body: &[IndentStmt], path: &mut Vec<PathSegment>) {
        for (decl_index, decl) in body.iter().enumerate() {
            if !is_va_list_decl(&decl.stmt) {
                continue;
            }
            for (assign_index, assign) in body.iter().enumerate() {
                if let Some(alias) =
                    self.alias_candidate(&decl.stmt, decl_index, &assign.stmt, assign_index, path)
                {
                    self.aliases.push(alias);
                }
            }
        }
        for (index, indent) in body.iter().enumerate() {
            walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
                walk::nested_bodies_with_path(&indent.stmt, path, &mut |body, path| {
                    self.body(body, path);
                });
            });
        }
    }

    fn alias_candidate(
        &self,
        decl: &Stmt,
        decl_index: usize,
        assign: &Stmt,
        assign_index: usize,
        parent_path: &[PathSegment],
    ) -> Option<VaListAliasFact> {
        let Stmt::Let {
            name,
            ty: Some(Type::VaList),
            init: None,
            ..
        } = decl
        else {
            return None;
        };
        let Stmt::Assign { target, value } = assign else {
            return None;
        };
        if !matches!(target, Expr::Var(target) if target.as_str() == name) {
            return None;
        }
        if !is_clone_of(value, &self.param_name) {
            return None;
        }

        let mut local_decl_path = parent_path.to_vec();
        local_decl_path.push(PathSegment::Stmt(decl_index));
        let local_decl_path = AstPath(local_decl_path);
        let local =
            self.facts
                .binding_by_local_path(self.function, name.as_str(), &local_decl_path)?;

        let mut clone_assign_path = parent_path.to_vec();
        clone_assign_path.push(PathSegment::Stmt(assign_index));
        let clone_assign_path = AstPath(clone_assign_path);

        if !self.binding_is_unique_clone_source(self.param_binding, &clone_assign_path) {
            return None;
        }
        if !self.binding_is_written_once_at(local, &clone_assign_path) {
            return None;
        }
        if self.name_conflicts_with_param(name.as_str()) {
            return None;
        }

        Some(VaListAliasFact {
            function: self.function,
            param: self.param_binding,
            local,
            local_decl_path,
            clone_assign_path,
        })
    }

    fn binding_is_unique_clone_source(&self, binding: BindingId, path: &AstPath) -> bool {
        self.facts.def_use(binding).is_some_and(|def_use| {
            def_use.writes.is_empty()
                && !def_use.reads.is_empty()
                && def_use.reads.iter().all(|read| read == path)
        })
    }

    fn binding_is_written_once_at(&self, binding: BindingId, path: &AstPath) -> bool {
        self.facts.def_use(binding).is_some_and(|def_use| {
            def_use.writes.iter().all(|write| write == path)
                && def_use.writes.iter().any(|write| write == path)
        })
    }

    fn name_conflicts_with_param(&self, name: &str) -> bool {
        self.facts.bindings.iter().any(|binding| {
            binding.function == self.function
                && binding.id != self.param_binding
                && binding.name == name
                && matches!(binding.kind, BindingKind::Param { .. })
        })
    }
}

fn is_va_list_decl(stmt: &Stmt) -> bool {
    matches!(
        stmt,
        Stmt::Let {
            ty: Some(Type::VaList),
            init: None,
            ..
        }
    )
}

fn is_clone_of(expr: &Expr, param: &Ident) -> bool {
    matches!(
        expr,
        Expr::MethodCall { recv, method, args }
            if method == "clone"
                && args.is_empty()
                && matches!(&**recv, Expr::Var(name) if name == param)
    )
}
