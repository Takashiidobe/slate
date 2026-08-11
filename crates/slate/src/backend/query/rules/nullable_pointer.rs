use crate::backend::facts::{AstPath, PathSegment};
use crate::backend::rust_ast::{Expr, IndentStmt, Stmt};
use crate::backend::trace::Pass;

use super::super::item::StatementRef;
use super::super::{
    BindingAccess, BindingRef, EditSet, Function, FunctionRef, ItemCaseContext,
    NullablePointerAlias, NullablePointerPlan, QueryRule, Rejection, nullable_pointer_option,
    preferred_option_name, removable_alias_decl, rewrite_nullable_pointer, supported_observation,
    transparent_alias_value,
};

pub(in crate::backend) fn rewrite() -> QueryRule<Function> {
    QueryRule::new(
        Pass::NullablePointer,
        "rewrite_nullable_pointer_chain",
        Function::default(),
    )
    .case("chain", nullable_pointer_case)
}

fn nullable_pointer_case<'db>(
    case: &mut ItemCaseContext<'_, 'db>,
    function: &FunctionRef<'db>,
) -> Result<EditSet, Rejection> {
    let body = case
        .fact(|query| query.function_snapshot(function))?
        .body
        .clone();
    let bindings = case.fact(|query| query.function_bindings(function))?;
    let mut path = Vec::new();
    let plan =
        find_plan(case, function, &bindings, &body, &mut path).ok_or_else(|| case.reject())?;
    case.replace_function_body(function.clone(), rewrite_nullable_pointer(body, plan))
}

fn find_plan<'db>(
    case: &mut ItemCaseContext<'_, 'db>,
    function: &FunctionRef<'db>,
    bindings: &[BindingRef<'db>],
    body: &[IndentStmt],
    path: &mut Vec<PathSegment>,
) -> Option<NullablePointerPlan> {
    for (index, indent) in body.iter().enumerate() {
        path.push(PathSegment::Stmt(index));
        let found = find_plan_in_stmt(case, function, bindings, &indent.stmt, path);
        path.pop();
        if found.is_some() {
            return found;
        }
    }
    for producer_index in 0..body.len() {
        if let Some(plan) = plan_for_producer(case, function, bindings, body, producer_index, path)
        {
            return Some(plan);
        }
    }
    None
}

fn find_plan_in_stmt<'db>(
    case: &mut ItemCaseContext<'_, 'db>,
    function: &FunctionRef<'db>,
    bindings: &[BindingRef<'db>],
    stmt: &Stmt,
    path: &mut Vec<PathSegment>,
) -> Option<NullablePointerPlan> {
    macro_rules! nested {
        ($segment:expr, $body:expr) => {{
            path.push($segment);
            let found = find_plan(case, function, bindings, $body, path);
            path.pop();
            found
        }};
    }
    match stmt {
        Stmt::If {
            then_body,
            else_body,
            ..
        }
        | Stmt::LetIf {
            then_body,
            else_body,
            ..
        } => nested!(PathSegment::Then, then_body).or_else(|| {
            path.push(PathSegment::Else);
            let found = find_plan(case, function, bindings, else_body, path);
            path.pop();
            found
        }),
        Stmt::Loop { body, .. } => nested!(PathSegment::LoopBody, body),
        Stmt::For { body, .. } => nested!(PathSegment::ForBody, body),
        Stmt::Scope { body } => nested!(PathSegment::ScopeBody, body),
        Stmt::LabeledBlock { body, .. } => nested!(PathSegment::LabeledBody, body),
        Stmt::Unsafe { body } => nested!(PathSegment::UnsafeBody, &body.stmts),
        Stmt::While { body, .. } => nested!(PathSegment::WhileBody, &body.stmts),
        Stmt::Block(body) => nested!(PathSegment::BlockBody, &body.stmts),
        Stmt::Match { arms, .. } => {
            for (index, arm) in arms.iter().enumerate() {
                path.push(PathSegment::MatchArm(index));
                let found = find_plan(case, function, bindings, &arm.body, path);
                path.pop();
                if found.is_some() {
                    return found;
                }
            }
            None
        }
        _ => None,
    }
}

fn plan_for_producer<'db>(
    case: &mut ItemCaseContext<'_, 'db>,
    function: &FunctionRef<'db>,
    bindings: &[BindingRef<'db>],
    body: &[IndentStmt],
    producer_index: usize,
    body_path: &[PathSegment],
) -> Option<NullablePointerPlan> {
    let Stmt::Let {
        name: option_name,
        mutable: false,
        init: Some(init),
        ..
    } = &body[producer_index].stmt
    else {
        return None;
    };
    let producer = nullable_pointer_option(init)?;
    let producer_statement = StatementRef {
        item_index: function.item_index,
        path: AstPath(stmt_path(body_path, producer_index)),
    };
    let producer_binding = case
        .fact(|query| query.statement_binding(&producer_statement))
        .ok()?;
    let producer_uses = case
        .fact(|query| query.binding_uses(&producer_binding))
        .ok()?;
    let has_reads = producer_uses
        .uses
        .iter()
        .any(|usage| matches!(usage.access, BindingAccess::Read | BindingAccess::ReadWrite));
    if !has_reads {
        return None;
    }
    let aliases = collect_alias_chain(
        case,
        function,
        bindings,
        body,
        body_path,
        producer_index,
        AliasSource {
            name: option_name.clone(),
            binding: producer_binding,
        },
        producer.base_ptr.as_ref(),
    )?;
    let producer_name = option_name.as_str().to_string();
    let alias_names = aliases
        .iter()
        .map(|alias| alias.name.clone())
        .collect::<Vec<_>>();
    let option_name = preferred_option_name(&producer_name, &alias_names);
    Some(NullablePointerPlan {
        container: body_path.to_vec(),
        producer_index,
        producer_name,
        option_name,
        option_expr: producer.option_expr,
        base_ptr: producer.base_ptr,
        aliases,
    })
}

struct AliasSource<'db> {
    name: String,
    binding: BindingRef<'db>,
}

#[expect(
    clippy::too_many_arguments,
    reason = "alias-edge analysis carries function, binding, and two site refs together"
)]
fn collect_alias_chain<'db>(
    case: &mut ItemCaseContext<'_, 'db>,
    function: &FunctionRef<'db>,
    bindings: &[BindingRef<'db>],
    body: &[IndentStmt],
    body_path: &[PathSegment],
    producer_index: usize,
    producer: AliasSource,
    base_ptr: Option<&Expr>,
) -> Option<Vec<NullablePointerAlias>> {
    let mut sources = vec![producer];
    let mut aliases = Vec::new();
    let mut cursor = 0;
    while cursor < sources.len() {
        let source_name = sources[cursor].name.clone();
        let source_binding = sources[cursor].binding.clone();
        let uses = case
            .fact(|query| query.binding_uses(&source_binding))
            .ok()?;
        for usage in &uses.uses {
            if !matches!(usage.access, BindingAccess::Read | BindingAccess::ReadWrite) {
                continue;
            }
            let expression = usage.expression()?;
            let statement = case
                .fact(|query| query.enclosing_statement(expression))
                .ok()?;
            let read_index = direct_stmt_index(body_path, &statement.path)?;
            if read_index <= producer_index || read_index >= body.len() {
                return None;
            }
            let uses_in_statement = case
                .fact(|query| query.binding_uses_in_statement(&source_binding, &statement))
                .ok()?;
            if uses_in_statement.uses.len() != 1 {
                return None;
            }
            let stmt = &body[read_index].stmt;
            if let Some(alias) = alias_edge(
                case,
                function,
                bindings,
                body,
                body_path,
                read_index,
                &source_name,
            )? {
                if !sources.iter().any(|source| source.name == alias.0.name) {
                    aliases.push(NullablePointerAlias {
                        name: alias.0.name.clone(),
                        remove_indices: alias.1.clone(),
                    });
                    sources.push(alias.0);
                }
                continue;
            }
            if !supported_observation(stmt, &source_name, base_ptr) {
                return None;
            }
        }
        cursor += 1;
    }
    Some(aliases)
}

fn alias_edge<'db>(
    case: &mut ItemCaseContext<'_, 'db>,
    function: &FunctionRef<'db>,
    bindings: &[BindingRef<'db>],
    body: &[IndentStmt],
    body_path: &[PathSegment],
    stmt_index: usize,
    source_name: &str,
) -> Option<Option<(AliasSource<'db>, Vec<usize>)>> {
    match &body[stmt_index].stmt {
        Stmt::Assign { target, value } => {
            let Expr::Var(alias_name) = target else {
                return Some(None);
            };
            if !transparent_alias_value(value, source_name) {
                return Some(None);
            }
            let binding = declared_before(bindings, body_path, alias_name.as_str(), stmt_index)?;
            if !all_writes_within(case, &binding, body_path, stmt_index)? {
                return None;
            }
            let mut remove_indices = vec![stmt_index];
            let def_index = direct_stmt_index(body_path, &binding.definition)
                .filter(|index| *index < stmt_index);
            if let Some(def_index) = def_index {
                if !removable_alias_decl(&body[def_index].stmt, alias_name.as_str()) {
                    return None;
                }
                remove_indices.push(def_index);
            }
            Some(Some((
                AliasSource {
                    name: alias_name.to_string(),
                    binding,
                },
                remove_indices,
            )))
        }
        Stmt::Let {
            name,
            init: Some(init),
            ..
        } => {
            if !transparent_alias_value(init, source_name) {
                return Some(None);
            }
            let statement = StatementRef {
                item_index: function.item_index,
                path: AstPath(stmt_path(body_path, stmt_index)),
            };
            let binding = case
                .fact(|query| query.statement_binding(&statement))
                .ok()?;
            if !all_writes_within(case, &binding, body_path, stmt_index)? {
                return None;
            }
            Some(Some((
                AliasSource {
                    name: name.clone(),
                    binding,
                },
                vec![stmt_index],
            )))
        }
        _ => Some(None),
    }
}

fn all_writes_within<'db>(
    case: &mut ItemCaseContext<'_, '_>,
    binding: &BindingRef<'db>,
    body_path: &[PathSegment],
    stmt_index: usize,
) -> Option<bool> {
    let statement = StatementRef {
        item_index: binding.item_index,
        path: AstPath(stmt_path(body_path, stmt_index)),
    };
    let total = case.fact(|query| query.binding_uses(binding)).ok()?;
    let here = case
        .fact(|query| query.binding_uses_in_statement(binding, &statement))
        .ok()?;
    let total_writes = total
        .uses
        .iter()
        .filter(|usage| {
            matches!(
                usage.access,
                BindingAccess::Write | BindingAccess::ReadWrite
            )
        })
        .count();
    let writes_here = here
        .uses
        .iter()
        .filter(|usage| {
            matches!(
                usage.access,
                BindingAccess::Write | BindingAccess::ReadWrite
            )
        })
        .count();
    Some(total_writes == writes_here)
}

fn declared_before<'db>(
    bindings: &[BindingRef<'db>],
    container: &[PathSegment],
    name: &str,
    before_index: usize,
) -> Option<BindingRef<'db>> {
    bindings
        .iter()
        .filter(|binding| binding.name == name)
        .filter_map(|binding| {
            let rest = binding.definition.0.strip_prefix(container)?;
            let [PathSegment::Stmt(index)] = rest else {
                return None;
            };
            (*index < before_index).then_some((*index, binding))
        })
        .max_by_key(|(index, _)| *index)
        .map(|(_, binding)| binding.clone())
}

fn stmt_path(body_path: &[PathSegment], index: usize) -> Vec<PathSegment> {
    let mut path = body_path.to_vec();
    path.push(PathSegment::Stmt(index));
    path
}

fn direct_stmt_index(body_path: &[PathSegment], path: &AstPath) -> Option<usize> {
    let rest = path.0.strip_prefix(body_path)?;
    match rest {
        [PathSegment::Stmt(index)] | [PathSegment::Stmt(index), ..] => Some(*index),
        _ => None,
    }
}
