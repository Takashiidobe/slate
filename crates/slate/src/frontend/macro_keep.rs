use foldhash::{HashMap, HashMapExt, HashSet, HashSetExt};

use slate_parser::ast::{MacroCover, MacroDefinition, MacroExpansionId, MacroInvocations};
use slate_parser::ir::{
    BindingId, MacroStructureVisitor, Module, Place, PlaceKind, Statement, Type, ValueKind,
    walk_macro_structure,
};

use crate::backend::rust_ast::is_valid_macro_template_base;

const MAX_LIFTED_NESTING: usize = 64;

#[derive(Default)]
struct OpenExpansion {
    jump_targets: HashSet<BindingId>,
    in_hole: usize,
    inside_outer_hole: usize,
}

struct Survey<'a> {
    globals: &'a HashSet<BindingId>,
    macros: &'a MacroInvocations,
    open: Vec<(MacroExpansionId, OpenExpansion)>,
    declared: HashMap<BindingId, Vec<MacroExpansionId>>,
    seen: Vec<MacroExpansionId>,
    unfit: HashMap<MacroExpansionId, &'static str>,
    has_jump: bool,
    reserves_storage: HashSet<MacroExpansionId>,
}

impl Survey<'_> {
    fn reserve_storage(&mut self) {
        self.reserves_storage
            .extend(self.open.iter().map(|(id, _)| *id));
    }

    fn reject(&mut self, expansion: MacroExpansionId, reason: &'static str) {
        self.unfit.entry(expansion).or_insert(reason);
    }

    fn reject_open(&mut self, reason: &'static str) {
        for position in 0..self.open.len() {
            self.reject(self.open[position].0, reason);
        }
    }

    fn declare(&mut self, binding: BindingId) {
        let open: Vec<_> = self
            .open
            .iter()
            .filter(|(_, open)| open.in_hole == 0)
            .map(|(id, _)| *id)
            .collect();
        self.declared.entry(binding).or_default().extend(open);
    }

    fn jump(&mut self, target: BindingId) {
        for position in 0..self.open.len() {
            let (id, open) = &self.open[position];
            if !open.jump_targets.contains(&target) {
                self.reject(*id, "break_out_of_macro");
            }
        }
    }
}

impl MacroStructureVisitor for Survey<'_> {
    fn statement(&mut self, statement: &Statement) {
        match statement {
            Statement::Let(variable) => {
                self.declare(variable.id);
                if variable.byref {
                    self.reserve_storage();
                }
                if variable.cleanup.is_some() {
                    self.reject_open("cleanup");
                }
            }
            Statement::Temporary { id, .. } => self.declare(*id),
            Statement::Write { place, .. } if writes_bit_field(place) => {
                self.reject_open("bit_field_write")
            }
            Statement::While { id, .. }
            | Statement::DoWhile { id, .. }
            | Statement::For { id, .. }
            | Statement::Switch { id, .. } => {
                for (_, open) in &mut self.open {
                    open.jump_targets.insert(*id);
                }
            }
            Statement::Break(target) | Statement::Continue(target) => self.jump(*target),
            Statement::Goto(_) | Statement::Label { .. } | Statement::ComputedGoto(_) => {
                self.has_jump = true;
                self.reject_open("control_transfer");
            }
            Statement::Asm(asm) => {
                self.has_jump |= !asm.labels.is_empty();
                self.reject_open("control_transfer");
            }
            Statement::Case { .. }
            | Statement::Default { .. }
            | Statement::SehTry { .. }
            | Statement::SehLeave(_)
            | Statement::OmpParallel(_)
            | Statement::OmpFor(_) => self.reject_open("control_transfer"),
            _ => {}
        }
    }

    fn value_kind(&mut self, kind: &ValueKind) {
        if matches!(kind, ValueKind::LabelAddress(_) | ValueKind::Seh(_)) {
            self.reject_open("control_transfer");
        }
        if matches!(kind, ValueKind::Block(_)) {
            self.reserve_storage();
        }
        if let ValueKind::MacroExpansion {
            expansion,
            evaluation,
        } = kind
            && evaluation.value.ty == Type::Void
        {
            self.reject(*expansion, "void_value");
        }
    }

    fn place_kind(&mut self, kind: &PlaceKind) {
        if matches!(
            kind,
            PlaceKind::CompoundLiteral { .. } | PlaceKind::Temporary { .. }
        ) {
            self.reject_open("compound_literal_or_temporary");
        }
    }

    fn enter_expansion(&mut self, expansion: MacroExpansionId) {
        self.seen.push(expansion);
        if !self.open.is_empty()
            && self
                .macros
                .expansion(expansion)
                .argument_uses
                .iter()
                .any(|using| match &using.cover {
                    MacroCover::Name(spelled) => !self.open.iter().any(|(outer, _)| {
                        self.macros
                            .expansion(*outer)
                            .argument_uses
                            .iter()
                            .any(|outer_use| matches!(&outer_use.cover, MacroCover::Name(name) if name == spelled))
                    }),
                    _ => false,
                })
        {
            self.reject(expansion, "name_argument_in_template");
        }
        if self.open.len() >= MAX_LIFTED_NESTING {
            self.reject(expansion, "nesting_beyond_recursion_limit");
        }
        self.open.push((expansion, OpenExpansion::default()));
    }

    fn leave_expansion(&mut self, _expansion: MacroExpansionId) {
        self.open.pop();
    }

    fn enter_value_hole(&mut self, expansion: MacroExpansionId) {
        let mut seen = false;
        for (id, open) in &mut self.open {
            if *id == expansion {
                open.in_hole += 1;
                seen = true;
            } else if seen {
                open.inside_outer_hole += 1;
            }
        }
    }

    fn leave_value_hole(&mut self, expansion: MacroExpansionId) {
        let mut seen = false;
        for (id, open) in &mut self.open {
            if *id == expansion {
                open.in_hole -= 1;
                seen = true;
            } else if seen {
                open.inside_outer_hole -= 1;
            }
        }
    }

    fn enter_place_hole(&mut self, expansion: MacroExpansionId, place: &Place) {
        if !plain_place(place) {
            self.reject(expansion, "unplain_place");
        }
        self.enter_value_hole(expansion);
    }

    fn leave_place_hole(&mut self, expansion: MacroExpansionId) {
        self.leave_value_hole(expansion);
    }

    fn binding(&mut self, binding: BindingId) {
        if self.globals.contains(&binding) {
            return;
        }
        let declared = self.declared.get(&binding).cloned().unwrap_or_default();
        for position in 0..self.open.len() {
            let (id, open) = &self.open[position];
            if declared.contains(id) && open.in_hole > 0 {
                self.reject(*id, "argument_reads_macro_local");
            } else if !declared.contains(id) && open.in_hole == 0 && open.inside_outer_hole > 0 {
                self.reject(*id, "reads_outer_argument");
            }
        }
        for id in declared {
            if !self.open.iter().any(|(open, _)| *open == id) {
                self.reject(id, "local_used_after_macro");
            }
        }
    }
}

struct CallerLocals<'a> {
    globals: &'a HashSet<BindingId>,
    macros: &'a MacroInvocations,
    open: Vec<(MacroExpansionId, usize)>,
    declared: HashMap<BindingId, Vec<MacroExpansionId>>,
    hidden: HashMap<MacroExpansionId, Vec<BindingId>>,
}

impl CallerLocals<'_> {
    fn declare(&mut self, binding: BindingId) {
        let open = self
            .open
            .iter()
            .filter(|(_, in_hole)| *in_hole == 0)
            .map(|(id, _)| *id);
        self.declared.entry(binding).or_default().extend(open);
    }

    fn hole(&mut self, expansion: MacroExpansionId, change: impl Fn(&mut usize)) {
        for (id, in_hole) in &mut self.open {
            if *id == expansion {
                change(in_hole);
            }
        }
    }

    fn hidden_argument(&mut self, expansion: MacroExpansionId, binding: BindingId) -> usize {
        let parameters = self
            .macros
            .definition(expansion)
            .parameters
            .as_ref()
            .map_or(0, Vec::len);
        let hidden = self.hidden.entry(expansion).or_default();
        let position = hidden
            .iter()
            .position(|candidate| *candidate == binding)
            .unwrap_or_else(|| {
                hidden.push(binding);
                hidden.len() - 1
            });
        parameters + position
    }
}

impl MacroStructureVisitor for CallerLocals<'_> {
    fn statement(&mut self, statement: &Statement) {
        match statement {
            Statement::Let(variable) => self.declare(variable.id),
            Statement::Temporary { id, .. } => self.declare(*id),
            _ => {}
        }
    }

    fn enter_expansion(&mut self, expansion: MacroExpansionId) {
        self.open.push((expansion, 0));
    }

    fn leave_expansion(&mut self, _expansion: MacroExpansionId) {
        self.open.pop();
    }

    fn enter_value_hole(&mut self, expansion: MacroExpansionId) {
        self.hole(expansion, |in_hole| *in_hole += 1);
    }

    fn leave_value_hole(&mut self, expansion: MacroExpansionId) {
        self.hole(expansion, |in_hole| *in_hole -= 1);
    }

    fn enter_place_hole(&mut self, expansion: MacroExpansionId, _place: &Place) {
        self.enter_value_hole(expansion);
    }

    fn leave_place_hole(&mut self, expansion: MacroExpansionId) {
        self.leave_value_hole(expansion);
    }

    fn rewrite_place(&mut self, place: &mut Place) {
        let PlaceKind::Binding(binding) = place.kind else {
            return;
        };
        if self.globals.contains(&binding) {
            return;
        }
        let free: Vec<_> = self
            .open
            .iter()
            .filter(|(id, in_hole)| {
                *in_hole == 0
                    && !self
                        .declared
                        .get(&binding)
                        .is_some_and(|declared| declared.contains(id))
            })
            .map(|(id, _)| *id)
            .collect();
        for expansion in free {
            let argument = self.hidden_argument(expansion, binding);
            *place = Place {
                ty: place.ty.clone(),
                access: place.access,
                kind: PlaceKind::MacroArg {
                    expansion,
                    argument,
                    place: Box::new(place.clone()),
                },
            };
        }
    }
}

fn writes_bit_field(place: &Place) -> bool {
    match &place.kind {
        PlaceKind::MacroArg { place, .. } => writes_bit_field(place),
        PlaceKind::Field { bits, .. } => bits.is_some(),
        _ => false,
    }
}

fn plain_place(place: &Place) -> bool {
    if place.access.volatile || place.access.atomic || place.access.align.is_some() {
        return false;
    }
    match &place.kind {
        PlaceKind::MacroArg { place, .. } => plain_place(place),
        PlaceKind::Binding(_) | PlaceKind::Deref(_) | PlaceKind::Index { .. } => true,
        PlaceKind::Field {
            base, bits: None, ..
        } => plain_place(base),
        _ => false,
    }
}

fn global_bindings(module: &Module) -> HashSet<BindingId> {
    let mut globals: HashSet<BindingId> = module
        .globals
        .iter()
        .map(|global| global.value.variable.id)
        .collect();
    for function in &module.functions {
        globals.insert(function.value.id);
    }
    globals
}

pub(super) fn pass_caller_locals(module: &mut Module) {
    let globals = global_bindings(module);
    let mut hidden = HashMap::new();
    for function in &mut module.functions {
        let Some(body) = &mut function.value.body else {
            continue;
        };
        let mut locals = CallerLocals {
            globals: &globals,
            macros: &module.macros,
            open: Vec::new(),
            declared: HashMap::new(),
            hidden: std::mem::take(&mut hidden),
        };
        walk_macro_structure(body, &mut locals);
        hidden = locals.hidden;
    }
    module.macro_hidden_arguments = hidden;
}

pub(super) fn kept_expansions(module: &mut Module) -> HashSet<MacroExpansionId> {
    let globals = global_bindings(module);
    let mut rejections = HashMap::new();
    let mut seen = HashSet::new();
    for function in &mut module.functions {
        let Some(body) = &mut function.value.body else {
            continue;
        };
        let mut survey = Survey {
            globals: &globals,
            macros: &module.macros,
            open: Vec::new(),
            declared: HashMap::new(),
            seen: Vec::new(),
            unfit: HashMap::new(),
            has_jump: false,
            reserves_storage: HashSet::new(),
        };
        walk_macro_structure(body, &mut survey);
        if survey.has_jump {
            for expansion in std::mem::take(&mut survey.reserves_storage) {
                survey.reject(expansion, "storage_in_dispatch_function");
            }
        }
        rejections.extend(survey.unfit);
        seen.extend(survey.seen);
    }
    for id in &seen {
        if let Some(reason) = definition_unfit(module.macros.definition(*id)) {
            rejections.entry(*id).or_insert(reason);
        }
    }
    let mut kept: HashSet<_> = seen
        .into_iter()
        .filter(|id| !rejections.contains_key(id))
        .collect();
    for constant in kept_constants(module) {
        rejections.remove(&constant);
        kept.insert(constant);
    }
    module.macro_rejections = rejections;
    kept
}

pub(super) fn definition_unfit(definition: &MacroDefinition) -> Option<&'static str> {
    let Some(parameters) = &definition.parameters else {
        return Some("not_function_like");
    };
    if definition.provenance.system_header.is_some() {
        Some("system_header")
    } else if definition.variadic {
        Some("variadic")
    } else if definition.replacement.contains('#') {
        Some("stringify_or_paste")
    } else if !is_valid_macro_template_base(&definition.name) {
        Some("reserved_macro_name")
    } else if parameters.iter().any(|name| name == "crate") {
        Some("reserved_param_name")
    } else {
        None
    }
}

pub(super) fn kept_constants(module: &mut Module) -> HashSet<MacroExpansionId> {
    module
        .macros
        .expansions
        .iter()
        .enumerate()
        .filter_map(|(index, expansion)| {
            let id = MacroExpansionId(index as u32);
            let definition = module.macros.definition(id);
            (definition.parameters.is_none()
                && definition.provenance.system_header.is_some()
                && matches!(
                    expansion.cover,
                    Some(slate_parser::ast::MacroCover::Expr(_))
                )
                && crate::libc_catalog::lookup(
                    &definition.name,
                    &module.target.triple,
                    crate::libc_catalog::Kind::Constant,
                )
                .is_some())
            .then_some(id)
        })
        .collect()
}
