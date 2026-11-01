use std::collections::{HashMap, HashSet};

use slate_parser::ast::{MacroDefinition, MacroExpansionId};
use slate_parser::ir::{
    BindingId, MacroStructureVisitor, Module, Place, PlaceKind, Statement, Type, ValueKind,
    walk_macro_structure,
};

use crate::backend::rust_ast::is_valid_macro_template_base;

#[derive(Default)]
struct OpenExpansion {
    jump_targets: HashSet<BindingId>,
    in_hole: usize,
}

struct Survey<'a> {
    globals: &'a HashSet<BindingId>,
    open: Vec<(MacroExpansionId, OpenExpansion)>,
    declared: HashMap<BindingId, Vec<MacroExpansionId>>,
    seen: Vec<MacroExpansionId>,
    unfit: HashSet<MacroExpansionId>,
    has_jump: bool,
}

impl Survey<'_> {
    fn reject_open(&mut self) {
        self.unfit.extend(self.open.iter().map(|(id, _)| *id));
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
        for (id, open) in &self.open {
            if !open.jump_targets.contains(&target) {
                self.unfit.insert(*id);
            }
        }
    }
}

impl MacroStructureVisitor for Survey<'_> {
    fn statement(&mut self, statement: &Statement) {
        match statement {
            Statement::Let(variable) => {
                self.declare(variable.id);
                if variable.cleanup.is_some() {
                    self.reject_open();
                }
            }
            Statement::Temporary { id, .. } => self.declare(*id),
            Statement::Write { place, .. } if writes_bit_field(place) => self.reject_open(),
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
                self.reject_open();
            }
            Statement::Return(_)
            | Statement::Asm(_)
            | Statement::Case { .. }
            | Statement::Default { .. }
            | Statement::SehTry { .. }
            | Statement::SehLeave(_)
            | Statement::OmpParallel(_)
            | Statement::OmpFor(_) => self.reject_open(),
            _ => {}
        }
    }

    fn value_kind(&mut self, kind: &ValueKind) {
        if matches!(kind, ValueKind::LabelAddress(_) | ValueKind::Seh(_)) {
            self.reject_open();
        }
        if let ValueKind::MacroExpansion {
            expansion,
            evaluation,
        } = kind
            && evaluation.value.ty == Type::Void
        {
            self.unfit.insert(*expansion);
        }
    }

    fn place_kind(&mut self, kind: &PlaceKind) {
        if matches!(
            kind,
            PlaceKind::CompoundLiteral { .. } | PlaceKind::Temporary { .. }
        ) {
            self.reject_open();
        }
    }

    fn enter_expansion(&mut self, expansion: MacroExpansionId) {
        self.seen.push(expansion);
        self.open.push((expansion, OpenExpansion::default()));
    }

    fn leave_expansion(&mut self, _expansion: MacroExpansionId) {
        self.open.pop();
    }

    fn enter_value_hole(&mut self, expansion: MacroExpansionId) {
        for (id, open) in &mut self.open {
            if *id == expansion {
                open.in_hole += 1;
            }
        }
    }

    fn leave_value_hole(&mut self, expansion: MacroExpansionId) {
        for (id, open) in &mut self.open {
            if *id == expansion {
                open.in_hole -= 1;
            }
        }
    }

    fn enter_place_hole(&mut self, expansion: MacroExpansionId, place: &Place) {
        if !plain_place(place) {
            self.unfit.insert(expansion);
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
        let declared = self.declared.get(&binding).map_or(&[][..], Vec::as_slice);
        for (id, open) in &self.open {
            if declared.contains(id) == (open.in_hole > 0) {
                self.unfit.insert(*id);
            }
        }
        for id in declared {
            if !self.open.iter().any(|(open, _)| open == id) {
                self.unfit.insert(*id);
            }
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

pub(super) fn kept_expansions(module: &mut Module) -> HashSet<MacroExpansionId> {
    let mut globals: HashSet<BindingId> = module
        .globals
        .iter()
        .map(|global| global.value.variable.id)
        .collect();
    for function in &module.functions {
        globals.insert(function.value.id);
    }
    let mut unfit = HashSet::new();
    let mut seen = HashSet::new();
    for function in &mut module.functions {
        let Some(body) = &mut function.value.body else {
            continue;
        };
        let mut survey = Survey {
            globals: &globals,
            open: Vec::new(),
            declared: HashMap::new(),
            seen: Vec::new(),
            unfit: HashSet::new(),
            has_jump: false,
        };
        walk_macro_structure(body, &mut survey);
        if survey.has_jump {
            for expansions in survey.declared.values() {
                survey.unfit.extend(expansions.iter().copied());
            }
        }
        unfit.extend(survey.unfit);
        seen.extend(survey.seen);
    }
    seen.into_iter()
        .filter(|id| !unfit.contains(id) && definition_fits(module.macros.definition(*id)))
        .collect()
}

fn definition_fits(definition: &MacroDefinition) -> bool {
    let Some(parameters) = &definition.parameters else {
        return false;
    };
    definition.provenance.system_header.is_none()
        && !definition.variadic
        && !definition.replacement.contains('#')
        && is_valid_macro_template_base(&definition.name)
        && !parameters.iter().any(|name| name == "crate")
}
