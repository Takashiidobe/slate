use std::collections::{HashMap, HashSet};

use slate_parser::ast::{MacroCover, MacroExpansionId, MacroInvocations, Span};
use slate_parser::ir;

use super::*;
use crate::backend::codegen;
use crate::backend::engine::macro_templates;
use crate::backend::rust_ast::MacroOutcome;
use crate::frontend::macro_keep;

pub(super) fn unreached_expansions(
    macros: &MacroInvocations,
    rejections: &HashMap<MacroExpansionId, &'static str>,
    seen: &HashSet<MacroExpansionId>,
) -> Vec<MacroOutcome> {
    macros
        .expansions
        .iter()
        .enumerate()
        .map(|(id, expansion)| (MacroExpansionId(id as u32), expansion))
        .filter(|(id, _)| !seen.contains(id))
        .map(|(id, expansion)| MacroOutcome {
            name: macros.definition(id).name.to_string(),
            inline: Some(if let Some(reason) = rejections.get(&id) {
                reason
            } else if expansion.empty {
                "empty_expansion"
            } else if expansion.spelled.is_some() && expansion.parent.is_some() {
                "inside_argument"
            } else if let Some(reason) = macro_keep::definition_unfit(macros.definition(id)) {
                reason
            } else if expansion
                .argument_uses
                .iter()
                .any(|argument| matches!(argument.cover, MacroCover::Split))
            {
                "split_argument"
            } else if expansion.argument_uses.iter().any(|argument| {
                matches!(
                    argument.cover,
                    MacroCover::Statements(_) | MacroCover::Decl(_)
                )
            }) {
                "statement_argument"
            } else {
                "no_statement_region"
            }),
        })
        .collect()
}

pub(super) struct MacroSite {
    expansion: MacroExpansionId,
    parameters: Vec<String>,
    arguments: Vec<Option<Expr>>,
    place_arguments: Vec<Option<Expr>>,
    call_arguments: Vec<Vec<Expr>>,
}

impl FunctionLowerer<'_, '_> {
    pub(super) fn lower_macro_site(
        &mut self,
        expansion: MacroExpansionId,
        body: &[Span<ir::Statement>],
    ) -> Result<Stmt> {
        self.open_macro_site(expansion);
        let lowered = self.lower_statement_list(body);
        let site = self.close_macro_site();
        let (comments, template): (Vec<Stmt>, Vec<Stmt>) = lowered?
            .into_iter()
            .partition(|stmt| matches!(stmt, Stmt::Comment(_)));
        let call =
            self.finish_macro_site(expansion, site, template, |this, template, params, args| {
                this.check_macro_template(body, None, template, params, args)
            })?;
        let call = Stmt::Expr(call);
        if comments.is_empty() {
            return Ok(call);
        }
        let mut body = comments;
        body.push(call);
        Ok(Stmt::Scope { body })
    }

    pub(super) fn lower_macro_value_site(
        &mut self,
        expansion: MacroExpansionId,
        evaluation: &ir::Evaluation,
    ) -> Result<Expr> {
        if self.macro_template_check {
            let stmts = self.lower_statement_list(&evaluation.statements)?;
            let value = self.lower_value(&evaluation.value)?;
            if stmts.is_empty() {
                return Ok(value);
            }
            return Ok(Expr::Block(Box::new(rust::Block {
                stmts,
                tail: Some(Box::new(value)),
            })));
        }
        self.open_macro_site(expansion);
        let lowered = self
            .lower_statement_list(&evaluation.statements)
            .and_then(|stmts| Ok((stmts, self.lower_value(&evaluation.value)?)));
        let site = self.close_macro_site();
        let (mut template, value) = lowered?;
        template.retain(|stmt| !matches!(stmt, Stmt::Comment(_)));
        template.push(Stmt::Return(Some(value)));
        self.finish_macro_site(expansion, site, template, |this, template, params, args| {
            this.check_macro_template(
                &evaluation.statements,
                Some(&evaluation.value),
                template,
                params,
                args,
            )
        })
    }

    fn open_macro_site(&mut self, expansion: MacroExpansionId) {
        let parameters = self.macro_site_parameters(expansion);
        let count = parameters.len();
        self.macro_sites.push(MacroSite {
            expansion,
            parameters,
            arguments: vec![None; count],
            place_arguments: vec![None; count],
            call_arguments: vec![Vec::new(); count],
        });
    }

    fn macro_site_parameters(&self, expansion: MacroExpansionId) -> Vec<String> {
        let mut parameters = self
            .tables
            .macros
            .definition(expansion)
            .parameters
            .clone()
            .unwrap_or_default();
        for binding in self
            .tables
            .macro_hidden_arguments
            .get(&expansion)
            .into_iter()
            .flatten()
        {
            let spelled = self.binding_name(*binding);
            let base = match spelled.strip_prefix("r#") {
                Some(raw) => raw,
                None if spelled.starts_with("__v") => "__v",
                None => &spelled,
            };
            let mut name = path_keyword_free(base);
            let mut suffix = 1;
            while parameters.contains(&name) {
                suffix += 1;
                name = format!("{base}__{suffix}");
            }
            parameters.push(name);
        }
        parameters
    }

    fn close_macro_site(&mut self) -> MacroSite {
        self.macro_sites.pop().expect("opened by the caller")
    }

    fn finish_macro_site(
        &mut self,
        expansion: MacroExpansionId,
        site: MacroSite,
        mut template: Vec<Stmt>,
        check: impl FnOnce(&mut Self, &[Stmt], &[rust::MacroParam], &[Expr]) -> Result<()>,
    ) -> Result<Expr> {
        let definition = self.tables.macros.definition(expansion);
        macro_templates::normalize_locals(&mut template);
        let mut params = Vec::new();
        let mut args = Vec::new();
        for (index, ((name, argument), place)) in site
            .parameters
            .iter()
            .zip(site.arguments)
            .zip(site.place_arguments)
            .enumerate()
        {
            for (element, value) in site.call_arguments[index].iter().enumerate() {
                let element = macro_templates::call_argument_parameter(name, element);
                if macro_templates::mentions_hole(&mut template, &element) {
                    params.push(rust::MacroParam {
                        name: element,
                        shape: rust::MacroParamShape::Expr,
                    });
                    args.push(value.clone());
                }
            }
            if let Some((ty, c)) = self
                .tables
                .c_types
                .macro_type_arguments
                .get(&(expansion, index))
                && let Ok(concrete) = self.lower_c_type(ty, Some(*c))
                && macro_templates::make_type_parameter(&mut template, name, &concrete)
            {
                params.push(rust::MacroParam {
                    name: name.clone(),
                    shape: rust::MacroParamShape::Type,
                });
                args.push(Expr::MacroType(concrete));
                continue;
            }
            let name_argument = self
                .tables
                .macros
                .expansion(expansion)
                .argument_uses
                .iter()
                .find_map(|using| match &using.cover {
                    MacroCover::Name(spelled) if using.argument == index => Some(spelled),
                    _ => None,
                });
            if let Some(spelled) = name_argument
                && macro_templates::make_field_parameter(
                    &mut template,
                    name,
                    &path_keyword_free(spelled),
                )
            {
                params.push(rust::MacroParam {
                    name: name.clone(),
                    shape: rust::MacroParamShape::Ident,
                });
                args.push(Expr::MacroName(path_keyword_free(spelled)));
                continue;
            }
            let value = match (argument, place) {
                (argument, Some(place)) => {
                    let read_is_unsafe = matches!(&argument, Some(Expr::Unsafe(_)));
                    macro_templates::make_place_parameter(&mut template, name, read_is_unsafe);
                    Expr::AddrOf {
                        mutable: macro_templates::parameter_is_written(&template, name),
                        expr: Box::new(place),
                    }
                }
                (Some(argument), None) => argument,
                (None, None) => continue,
            };
            params.push(rust::MacroParam {
                name: name.clone(),
                shape: rust::MacroParamShape::Expr,
            });
            args.push(value);
        }
        if std::env::var_os("SLATE_CHECK_MACRO_TEMPLATES").is_some() {
            check(self, &template, &params, &args)?;
        }
        let mut printed = format!("{params:?}");
        printed.extend(template.iter().map(codegen::stmt_to_string));
        let name = rust::macro_template_name(&definition.name, &printed);
        self.dependencies
            .macro_templates
            .entry(name.clone())
            .or_insert_with(|| Item::MacroRules {
                name: name.clone(),
                params,
                body: template,
            });
        self.dependencies.macro_expansions_seen.insert(expansion);
        self.dependencies.macro_outcomes.push(MacroOutcome {
            name: definition.name.to_string(),
            inline: None,
        });
        Ok(Expr::Macro { name, args })
    }

    pub(super) fn lower_macro_hole(
        &mut self,
        expansion: MacroExpansionId,
        argument: usize,
        value: &ir::Value,
    ) -> Result<Expr> {
        let is_call_argument = self
            .tables
            .macros
            .expansion(expansion)
            .argument_uses
            .iter()
            .any(|using| {
                using.argument == argument && matches!(using.cover, MacroCover::CallArguments(_))
            });
        let mut element = 0;
        let parameter = self.lower_hole_argument(expansion, argument, |this, site| {
            let found = this.lower_value(value)?;
            if is_call_argument {
                element = site.call_arguments[argument].len();
                site.call_arguments[argument].push(found);
            } else {
                site.arguments[argument].get_or_insert(found);
            }
            Ok(())
        })?;
        if is_call_argument {
            return Ok(macro_templates::hole(
                &macro_templates::call_argument_parameter(&parameter, element),
            ));
        }
        Ok(macro_templates::hole(&parameter))
    }

    pub(super) fn lower_macro_place_hole(
        &mut self,
        expansion: MacroExpansionId,
        argument: usize,
        place: &ir::Place,
    ) -> Result<Expr> {
        let parameter = self.lower_hole_argument(expansion, argument, |this, site| {
            let found = this.lower_place(place)?;
            site.place_arguments[argument].get_or_insert(found);
            Ok(())
        })?;
        Ok(macro_templates::place_hole(&parameter))
    }

    fn lower_hole_argument(
        &mut self,
        expansion: MacroExpansionId,
        argument: usize,
        lower: impl FnOnce(&mut Self, &mut MacroSite) -> Result<()>,
    ) -> Result<String> {
        let position = self
            .macro_sites
            .iter()
            .rposition(|site| site.expansion == expansion)
            .ok_or(Invariant::MacroHoleOutsideExpansion)?;
        let parameter = self.macro_sites[position]
            .parameters
            .get(argument)
            .ok_or(Invariant::MacroHoleOutsideExpansion)?
            .clone();
        let inner_sites = self.macro_sites.split_off(position + 1);
        let mut site = self.macro_sites.pop().expect("found above");
        let result = lower(self, &mut site);
        self.macro_sites.push(site);
        self.macro_sites.extend(inner_sites);
        result.map(|()| parameter)
    }

    fn check_macro_template(
        &mut self,
        body: &[Span<ir::Statement>],
        result: Option<&ir::Value>,
        template: &[Stmt],
        params: &[rust::MacroParam],
        args: &[Expr],
    ) -> Result<()> {
        if !self.macro_sites.is_empty() || self.macro_template_check {
            return Ok(());
        }
        let mut inline_body = body.to_vec();
        ir::strip_all_macro_structure(&mut inline_body);
        let outer = std::mem::replace(&mut self.macro_template_check, true);
        let lowered = self
            .lower_statement_list(&inline_body)
            .and_then(|mut stmts| {
                if let Some(result) = result {
                    let mut result = result.clone();
                    ir::strip_all_macro_structure_in_value(&mut result);
                    stmts.push(Stmt::Return(Some(self.lower_value(&result)?)));
                }
                Ok(stmts)
            });
        self.macro_template_check = outer;
        let mut separately: Vec<Stmt> = lowered?
            .into_iter()
            .filter(|stmt| !matches!(stmt, Stmt::Comment(_)))
            .collect();
        macro_templates::collapse_address_of_deref(&mut separately);
        macro_templates::erase_binding_names(&mut separately);
        let mut filled = template.to_vec();
        macro_templates::fill_holes(&mut filled, params, args);
        macro_templates::expand_template_calls(&mut filled, &self.dependencies.macro_templates);
        macro_templates::collapse_address_of_deref(&mut filled);
        macro_templates::erase_binding_names(&mut filled);
        if filled != separately {
            let printed = |stmts: &[Stmt]| {
                stmts
                    .iter()
                    .map(codegen::stmt_to_string)
                    .collect::<Vec<_>>()
                    .join("\n")
            };
            let difference = format!("{}\n---\n{}", printed(&filled), printed(&separately));
            eprintln!("macro template mismatch in {}\n{difference}", self.owner);
            return Err(Invariant::MacroTemplateMismatch(self.owner.clone(), difference).into());
        }
        Ok(())
    }
}
