use crate::ast::{
    DeclKind, Declaration, FunctionDefinition, MsStructAction, Pragma, PragmaKind,
    PragmaStackAction, Stmt, StmtKind, TagId, TagSpecifier, TranslationUnit, TypeSpecifier,
};
use crate::ir::{SymbolAttributes, Visibility};
use crate::visit::{self, Visitor};
use std::collections::{HashMap, HashSet};
use std::convert::Infallible;

#[derive(Debug, Clone, Default)]
pub struct Pragmas {
    pack: HashMap<TagId, u64>,
    ms_struct: HashSet<TagId>,
    visibility: HashMap<String, Visibility>,
    weak: HashMap<String, Option<String>>,
    renamed: HashMap<String, String>,
}

impl Pragmas {
    pub fn max_field_alignment(&self, tag: TagId) -> Option<u64> {
        self.pack.get(&tag).copied()
    }

    pub fn is_ms_struct(&self, tag: TagId) -> bool {
        self.ms_struct.contains(&tag)
    }

    pub fn apply(&self, name: &str, symbol: &mut SymbolAttributes) {
        if let (None, Some(visibility)) = (symbol.visibility, self.visibility.get(name)) {
            symbol.visibility = Some(*visibility);
        }
        if let Some(alias) = self.weak.get(name) {
            symbol.weak = true;
            if symbol.alias.is_none() {
                symbol.alias = alias.clone();
            }
        }
        if let (None, Some(target)) = (&symbol.asm_name, self.renamed.get(name)) {
            symbol.asm_name = Some(target.clone());
        }
    }
}

pub fn collect(unit: &TranslationUnit) -> Pragmas {
    let mut walk = Walk {
        unit,
        pack: None,
        pack_stack: Vec::new(),
        ms_struct: false,
        visibility: Vec::new(),
        pragmas: Pragmas::default(),
    };
    for declaration in &unit.decls {
        match &declaration.value {
            DeclKind::Pragma(pragma) => walk.pragma(pragma),
            DeclKind::Function(function) => walk.function(function),
            DeclKind::Declaration(inner) => walk.declaration(inner),
            DeclKind::Comment(_) | DeclKind::StaticAssert(_) | DeclKind::Asm(_) => {}
        }
        for name in declaration.value.names() {
            walk.symbol(name);
        }
    }
    walk.pragmas
}

struct Walk<'a> {
    unit: &'a TranslationUnit,
    pack: Option<u64>,
    pack_stack: Vec<(Option<String>, Option<u64>)>,
    ms_struct: bool,
    visibility: Vec<Visibility>,
    pragmas: Pragmas,
}

impl Walk<'_> {
    fn pragma(&mut self, pragma: &Pragma) {
        match &pragma.kind {
            PragmaKind::Pack {
                action,
                label,
                alignment,
            } => {
                let requested = pack_request(alignment.as_ref());
                match action {
                    PragmaStackAction::Push => {
                        self.pack_stack.push((label.clone(), self.pack));
                        if let (true, Some(value)) = (alignment.is_some(), requested) {
                            self.pack = value;
                        }
                    }
                    PragmaStackAction::Pop => {
                        let entry = match label {
                            Some(label) => self
                                .pack_stack
                                .iter()
                                .rposition(|(slot, _)| slot.as_ref() == Some(label))
                                .map(|index| self.pack_stack.split_off(index).swap_remove(0)),
                            None => self.pack_stack.pop(),
                        };
                        if let Some((_, previous)) = entry {
                            self.pack = previous;
                        }
                    }
                    PragmaStackAction::Set => {
                        if let Some(value) = requested {
                            self.pack = value;
                        }
                    }
                    PragmaStackAction::Show => {}
                }
            }
            PragmaKind::MsStruct { action } => {
                self.ms_struct = match action {
                    MsStructAction::On => true,
                    // reset restores the -mms-bitfields default, which is off
                    // for every target this front end models
                    MsStructAction::Off | MsStructAction::Reset => false,
                };
            }
            PragmaKind::Visibility { action, visibility } => match action {
                PragmaStackAction::Push => {
                    if let Some(value) = visibility.as_deref().and_then(visibility_named) {
                        self.visibility.push(value);
                    }
                }
                PragmaStackAction::Pop => {
                    self.visibility.pop();
                }
                PragmaStackAction::Set | PragmaStackAction::Show => {}
            },
            PragmaKind::Weak { name, alias } => {
                self.pragmas.weak.insert(name.clone(), alias.clone());
            }
            PragmaKind::Opaque(text) => {
                if let ["redefine_extname", from, to] =
                    text.split_whitespace().collect::<Vec<_>>()[..]
                {
                    self.pragmas
                        .renamed
                        .insert(from.to_string(), to.to_string());
                }
            }
            PragmaKind::Stdc { .. } | PragmaKind::FloatControl { .. } => {}
        }
    }

    fn symbol(&mut self, name: &str) {
        if let Some(visibility) = self.visibility.last() {
            self.pragmas
                .visibility
                .entry(name.to_string())
                .or_insert(*visibility);
        }
    }

    fn record_tags(&mut self, collect: impl FnOnce(&mut TagCollector<'_>)) {
        let mut tags = TagCollector {
            unit: self.unit,
            found: Vec::new(),
        };
        collect(&mut tags);
        for tag in tags.found {
            if let Some(alignment) = self.pack {
                self.pragmas.pack.insert(tag, alignment);
            }
            if self.ms_struct {
                self.pragmas.ms_struct.insert(tag);
            }
        }
    }

    fn declaration(&mut self, declaration: &Declaration) {
        self.record_tags(|tags| {
            let _ = tags.visit_declaration(declaration);
        });
    }

    fn function(&mut self, function: &FunctionDefinition) {
        self.record_tags(|tags| {
            let _ = visit::walk_specifiers(tags, &function.specifiers);
            let _ = tags.visit_declarator(&function.declarator);
        });
        self.statements(&function.body);
    }

    fn statements(&mut self, body: &[Stmt]) {
        for statement in body {
            self.statement(statement);
        }
    }

    fn statement(&mut self, statement: &Stmt) {
        match &statement.value {
            StmtKind::Pragma(pragma) => self.pragma(pragma),
            StmtKind::Decl(declaration) => {
                self.declaration(declaration);
                for name in declaration.names() {
                    self.symbol(name);
                }
            }
            StmtKind::NestedFunction(function) => self.function(function),
            StmtKind::Block(body) => self.statements(body),
            StmtKind::Attributed { body, .. }
            | StmtKind::Labeled { body, .. }
            | StmtKind::SwitchLabel { body, .. }
            | StmtKind::While { body, .. }
            | StmtKind::DoWhile { body, .. }
            | StmtKind::Switch { body, .. } => self.statement(body),
            StmtKind::If {
                then_branch,
                else_branch,
                ..
            } => {
                self.statement(then_branch);
                if let Some(branch) = else_branch {
                    self.statement(branch);
                }
            }
            StmtKind::For { init, body, .. } => {
                if let Some(init) = init {
                    self.statement(init);
                }
                self.statement(body);
            }
            StmtKind::Comment(_)
            | StmtKind::Null
            | StmtKind::Return(_)
            | StmtKind::ReturnVoid
            | StmtKind::Expr(_)
            | StmtKind::StaticAssert(_)
            | StmtKind::Attribute(_)
            | StmtKind::LocalLabelDecl(_)
            | StmtKind::Asm(_)
            | StmtKind::Goto(_)
            | StmtKind::ComputedGoto(_)
            | StmtKind::Break
            | StmtKind::Continue => {}
        }
    }
}

fn pack_request(alignment: Option<&crate::ast::Expr>) -> Option<Option<u64>> {
    let Some(expression) = alignment else {
        return Some(None);
    };
    let value = crate::const_expr::Parser::evaluate_ast(expression)
        .ok()
        .and_then(|value| u64::try_from(value).ok())?;
    match value {
        0 => Some(None),
        1 | 2 | 4 | 8 | 16 => Some(Some(value)),
        // a pack that is not a small power of two is diagnosed and ignored,
        // leaving the alignment already in effect alone
        _ => None,
    }
}

fn visibility_named(name: &str) -> Option<Visibility> {
    match name {
        "default" => Some(Visibility::Default),
        "hidden" => Some(Visibility::Hidden),
        "protected" => Some(Visibility::Protected),
        "internal" => Some(Visibility::Internal),
        _ => None,
    }
}

struct TagCollector<'a> {
    unit: &'a TranslationUnit,
    found: Vec<TagId>,
}

impl Visitor for TagCollector<'_> {
    type Error = Infallible;

    fn visit_type_specifier(&mut self, ty: &TypeSpecifier) -> Result<(), Self::Error> {
        if let TypeSpecifier::Tag(TagSpecifier::Definition(id)) = ty
            && !self.found.contains(id)
        {
            self.found.push(*id);
            if let Some(tag) = self.unit.tag(*id) {
                visit::walk_tag_definition(self, tag)?;
            }
        }
        visit::walk_type_specifier(self, ty)
    }
}
