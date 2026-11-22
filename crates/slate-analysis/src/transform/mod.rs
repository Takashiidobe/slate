mod const_fold;
mod dead_temporaries;
mod decl_init;
mod do_while_zero;
mod inline_temporaries;
mod labeled_blocks;
mod structure_goto;
mod truth_canonicalize;
mod truth_locals;

use crate::def_use::captured;
use foldhash::HashSet;
use slate_parser::ast::NodeId;
use slate_parser::ir::*;
pub use truth_canonicalize::negation;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Slot {
    BeforeMacroKeep,
    AfterMacroStrip,
}

pub struct Pass {
    pub name: &'static str,
    pub slots: &'static [Slot],
    decls: Option<fn(&mut Module)>,
    function: Option<fn(&mut Function, &mut PassContext)>,
}

pub(crate) struct PassContext<'a> {
    pub module: &'a mut Module,
    pub captured: &'a HashSet<BindingId>,
    pub visible: &'a HashSet<String>,
}

impl PassContext<'_> {
    pub fn retire(&mut self, nodes: impl IntoIterator<Item = NodeId>) {
        for id in nodes {
            self.module.c_types.nodes.remove(&id);
            self.module.metadata.remove(&id);
        }
    }
}

pub static PASSES: &[Pass] = &[
    Pass {
        name: "structure-goto",
        slots: &[Slot::AfterMacroStrip],
        decls: None,
        function: Some(structure_goto::transform),
    },
    Pass {
        name: "truth-canonicalize",
        slots: &[Slot::AfterMacroStrip],
        decls: Some(truth_canonicalize::globals),
        function: Some(truth_canonicalize::transform),
    },
    Pass {
        name: "inline-temps",
        slots: &[Slot::BeforeMacroKeep, Slot::AfterMacroStrip],
        decls: None,
        function: Some(inline_temporaries::transform),
    },
    Pass {
        name: "const-fold",
        slots: &[Slot::AfterMacroStrip],
        decls: Some(const_fold::globals),
        function: Some(const_fold::transform),
    },
    Pass {
        name: "do-while-zero",
        slots: &[Slot::AfterMacroStrip],
        decls: None,
        function: Some(do_while_zero::transform),
    },
    Pass {
        name: "labeled-blocks",
        slots: &[Slot::AfterMacroStrip],
        decls: None,
        function: Some(labeled_blocks::transform),
    },
    Pass {
        name: "dead-temporaries",
        slots: &[Slot::AfterMacroStrip],
        decls: None,
        function: Some(dead_temporaries::transform),
    },
    Pass {
        name: "decl-init",
        slots: &[Slot::AfterMacroStrip],
        decls: None,
        function: Some(decl_init::transform),
    },
    Pass {
        name: "truth-locals",
        slots: &[Slot::AfterMacroStrip],
        decls: None,
        function: Some(truth_locals::transform),
    },
];

fn run_functions(module: &mut Module, run: fn(&mut Function, &mut PassContext)) {
    let mut captured: HashSet<BindingId> = module
        .types
        .iter()
        .filter_map(|definition| match &definition.value.kind {
            TypeDefinitionKind::Record {
                fields: Some(fields),
                ..
            } => Some(fields),
            _ => None,
        })
        .flatten()
        .flat_map(|field| captured(&field.ty))
        .collect();
    captured.extend(module.macro_hidden_arguments.values().flatten().copied());
    let visible: HashSet<String> = module
        .globals
        .iter()
        .map(|global| global.value.variable.name.clone())
        .chain(
            module
                .functions
                .iter()
                .map(|function| function.value.name.clone()),
        )
        .collect();
    let mut functions = std::mem::take(&mut module.functions);
    for function in &mut functions {
        run_scoped(&mut function.value, module, &captured, &visible, run);
    }
    module.functions = functions;
}

fn run_scoped(
    function: &mut Function,
    module: &mut Module,
    captured: &HashSet<BindingId>,
    visible: &HashSet<String>,
    run: fn(&mut Function, &mut PassContext),
) {
    if function.body.is_none() {
        return;
    }
    run(
        function,
        &mut PassContext {
            module,
            captured,
            visible,
        },
    );
    let Some(body) = &mut function.body else {
        return;
    };
    let mut scope = Scope::default();
    walk_function_body(body, &mut scope);
    if !scope.inner {
        return;
    }
    let mut names = visible.clone();
    scope.collect = true;
    walk_function_body(body, &mut scope);
    names.extend(scope.names);
    if let Parameters::Prototype { fixed, .. } = &function.parameters {
        names.extend(
            fixed
                .iter()
                .filter_map(|parameter| parameter.value.name.clone()),
        );
    }
    for_each_inner_function(body, &mut |inner| {
        run_scoped(inner, module, captured, &names, run);
    });
}

#[derive(Default)]
struct Scope {
    collect: bool,
    names: Vec<String>,
    inner: bool,
}

impl Visitor for Scope {
    fn statement(&mut self, statement: &Statement) {
        match statement {
            Statement::Let(variable) if self.collect && !variable.name.is_empty() => {
                self.names.push(variable.name.clone());
            }
            Statement::NestedFunction(_) => self.inner = true,
            #[expect(
                slate::sensitive_wildcard,
                reason = "only declarations and inner functions shape an inner function's scope"
            )]
            _ => {}
        }
    }

    fn value_kind(&mut self, kind: &ValueKind) {
        if matches!(kind, ValueKind::Block(_)) {
            self.inner = true;
        }
    }
}

#[derive(Clone)]
pub struct Passes(Vec<&'static Pass>);

impl Default for Passes {
    fn default() -> Self {
        Self(PASSES.iter().collect())
    }
}

impl Passes {
    pub fn none() -> Self {
        Self(Vec::new())
    }

    pub fn named(names: &str) -> Result<Self, String> {
        if names == "all" {
            return Ok(Self::default());
        }
        names
            .split(',')
            .filter(|name| !name.is_empty())
            .map(|name| {
                PASSES.iter().find(|pass| pass.name == name).ok_or_else(|| {
                    let known: Vec<_> = PASSES.iter().map(|pass| pass.name).collect();
                    format!("unknown pass `{name}` (known: all, {})", known.join(", "))
                })
            })
            .collect::<Result<_, _>>()
            .map(Self)
    }

    pub fn run(&self, module: &mut Module, slot: Slot) {
        for pass in PASSES {
            if pass.slots.contains(&slot)
                && self.0.iter().any(|selected| std::ptr::eq(*selected, pass))
            {
                if let Some(decls) = pass.decls {
                    decls(module);
                }
                if let Some(function) = pass.function {
                    run_functions(module, function);
                }
            }
        }
    }
}
