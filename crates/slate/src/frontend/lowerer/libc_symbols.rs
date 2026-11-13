use super::*;
use crate::libc_catalog::{self, Kind};

pub(super) struct SupportFunction {
    pub symbol: &'static libc_catalog::Symbol,
    pub shape: String,
}

fn record(name: &str, module: &ir::Module) -> Option<String> {
    let definition = module
        .types
        .iter()
        .find(|definition| definition.name.as_deref() == Some(name))?;
    libc_catalog::record_path(definition, &module.target.triple).map(str::to_string)
}

fn use_symbol(path: &str, alias: &str) -> Item {
    Item::Use {
        vis: rust::Visibility::Private,
        path: rust::Path::new(path.split("::").map(rust::Ident::from)),
        alias: (path.rsplit("::").next() != Some(alias)).then(|| alias.into()),
    }
}

impl Tables<'_> {
    pub(super) fn bind_libc_symbols(&mut self, module: &ir::Module) {
        let target = &module.target.triple;
        let shape = |ty: &ir::Type, c| {
            libc_catalog::shape(
                ty,
                &module.types,
                &module.c_types,
                c,
                &|name| record(name, module),
                module.target.pointer_width,
            )
        };
        for function in &module.functions {
            let intrinsic = module.metadata.get(&function.id).is_some_and(|entries| {
                entries
                    .iter()
                    .any(|(key, value)| key == "c_builtin_header" && value.ends_with("intrin.h"))
            });
            if function.body.is_some()
                || (function.provenance.system_header.is_none() && !intrinsic)
                || self.stdio.functions.contains_key(&function.value.id)
                || self.intrinsics.contains_key(&function.value.id)
                || !matches!(function.linkage, ir::Linkage::External)
            {
                continue;
            }
            let Some(symbol) = libc_catalog::lookup(&function.name, target, Kind::Function) else {
                continue;
            };
            let Some(ty) = libc_catalog::function_type(function) else {
                continue;
            };
            let Some(mut actual) = shape(
                &ty,
                module.c_types.bindings.get(&function.value.id).copied(),
            ) else {
                continue;
            };
            if function.semantics.noreturn
                && let Some((prefix, _)) = actual.rsplit_once(" -> ")
            {
                actual = format!("{prefix} -> !");
            }
            if symbol.shape.is_none_or(|shape| shape == actual)
                && function
                    .symbol
                    .asm_name
                    .as_deref()
                    .is_none_or(|name| name.trim_start_matches('\u{1}') == function.name)
            {
                self.support_functions.insert(
                    function.value.id,
                    SupportFunction {
                        symbol,
                        shape: actual,
                    },
                );
            }
        }
        for global in &module.globals {
            if global.definition
                || global.provenance.system_header.is_none()
                || !matches!(global.linkage, ir::Linkage::External)
                || matches!(global.variable.storage, ir::StorageDuration::Thread)
                || global.symbol.weak
                || global.symbol.alias.is_some()
                || !global.variable.access.is_plain()
            {
                continue;
            }
            let Some(symbol) = libc_catalog::lookup(&global.variable.name, target, Kind::Global)
            else {
                continue;
            };
            if global
                .symbol
                .asm_name
                .as_deref()
                .is_none_or(|name| name.trim_start_matches('\u{1}') == global.variable.name)
            {
                self.support_globals.insert(global.variable.id, symbol);
            }
        }
    }
}

impl ModuleLowerer<'_> {
    pub(super) fn declare_support_function(&mut self, id: BindingId) -> bool {
        let Some(symbol) = self.tables.support_functions.get(&id) else {
            return false;
        };
        self.items.push(use_symbol(
            symbol.symbol.rust_path,
            &self.tables.names[&id].rust,
        ));
        true
    }

    pub(super) fn declare_support_global(&mut self, id: BindingId) -> bool {
        let Some(symbol) = self.tables.support_globals.get(&id) else {
            return false;
        };
        self.items
            .push(use_symbol(symbol.rust_path, &self.tables.bindings[&id]));
        true
    }
}

impl FunctionLowerer<'_, '_> {
    pub(super) fn support_argument(
        &mut self,
        argument: &ir::Value,
        lowered: Expr,
        expected: &str,
    ) -> Result<Expr> {
        let ty = self.tables.resolve_type(&argument.ty);
        let vector = libc_catalog::is_vector_path(expected);
        let expected = rust::Type::Custom(expected.replace("libc::", "slate_support::"));
        if vector
            || matches!(ty, ir::Type::Pointer { pointee, .. } if matches!(self.tables.resolve_type(pointee), ir::Type::Function { .. }))
        {
            return Ok(Expr::Transmute {
                from: self.lower_type(&argument.ty)?,
                to: expected,
                expr: Box::new(lowered),
            });
        }
        if matches!(
            ty,
            ir::Type::Bool | ir::Type::Numeric(_) | ir::Type::Pointer { .. }
        ) {
            Ok(Expr::Cast {
                expr: Box::new(lowered),
                ty: expected,
            })
        } else {
            Ok(lowered)
        }
    }

    pub(super) fn support_constant(
        &mut self,
        expansion: slate_parser::ast::MacroExpansionId,
        value: &ir::Value,
    ) -> Option<Result<Expr>> {
        let definition = self.tables.macros.definition(expansion);
        definition.provenance.system_header?;
        if definition.parameters.is_some() {
            return None;
        }
        let symbol =
            libc_catalog::lookup(&definition.name, &self.tables.target.triple, Kind::Constant)?;
        if !matches!(
            self.tables.resolve_type(&value.ty),
            ir::Type::Numeric(ir::NumericType::Integer { .. }) | ir::Type::Bool
        ) {
            return None;
        }
        Some(self.lower_value_type(value).map(|ty| Expr::Cast {
            expr: Box::new(Expr::Path(rust::Path::new(
                symbol.rust_path.split("::").map(rust::Ident::from),
            ))),
            ty,
        }))
    }
}
