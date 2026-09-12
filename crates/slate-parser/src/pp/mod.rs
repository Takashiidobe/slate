mod condition;
mod define;
mod error;
mod expand;
mod include;
mod syntax;

use crate::ast::{Condition, Conditional, FileId, HeaderKind, Loc, Provenance, Span};
use crate::const_expr;
use crate::files::{Files, SearchPaths};
use crate::lexer::{Lexer, Token, TokenSpanExt};
use condition::{
    conjunction, difference, intersect, is_satisfiable, is_statically_false, negate, normalize,
};
use define::PushedMacro;
pub use error::{DirectiveDiagnostic, DirectiveErrors, PPError};
use error::{PPErrorKind, PPFailure};
use expand::defined_operands;
use include::{include_target, read_source};
use miette::Severity;
use std::collections::{HashMap, HashSet};
use std::path::{Path, PathBuf};
use syntax::{
    Directive, DirectiveName, IfSection, Item, controlling_macro, directive_spelling, identifier,
};

pub type PPNode = Span<PPNodeKind>;

#[derive(Debug, Clone, PartialEq)]
pub enum PPNodeKind {
    Comment {
        text: String,
        provenance: Provenance,
    },
    Code {
        text: String,
        tokens: Vec<Span<Token>>,
        provenance: Provenance,
    },
    Conditional(PPConditional),
}

#[derive(Debug, Clone, PartialEq)]
pub struct PPConditional {
    pub branches: Vec<(Condition, Vec<PPNode>)>,
}

#[derive(Debug, Clone, PartialEq)]
pub struct MacroDef {
    pub parameters: Option<Vec<String>>,
    pub variadic: bool,
    pub replacement: Vec<Span<Token>>,
}

#[derive(Debug, Clone, PartialEq)]
pub struct MacroEntry {
    pub definition: Option<MacroDef>,
    pub provenance: Provenance,
    pub order: usize,
}

pub struct Preprocessor<'a> {
    pub files: Files,
    pub macros: HashMap<String, Conditional<MacroEntry>>,
    pub main_file: Option<FileId>,
    search: &'a SearchPaths,
    open_stack: Vec<PathBuf>,
    sources: HashMap<FileId, String>,
    line_starts: HashMap<FileId, Vec<usize>>,
    pragma_once: HashMap<PathBuf, Vec<Condition>>,
    pushed_macros: HashMap<String, Vec<PushedMacro>>,
    configuration_names: HashSet<String>,
    pub predefined_macros: Vec<String>,
    pub directive_diagnostics: Vec<DirectiveDiagnostic>,
    macro_order: usize,
}

const BUILTIN_PREDEFINES: [(&str, &str); 2] = [
    (
        "<clang-x86_64-linux-gnu-predefines>",
        include_str!("../predefines/clang_x86_64_linux_gnu.h"),
    ),
    (
        "<slate-target-defaults>",
        include_str!("../predefines/slate_target_defaults.h"),
    ),
];

impl<'a> Preprocessor<'a> {
    pub fn new(search: &'a SearchPaths) -> Self {
        let mut pp = Preprocessor {
            files: Files::new(),
            macros: HashMap::new(),
            main_file: None,
            search,
            open_stack: Vec::new(),
            sources: HashMap::new(),
            line_starts: HashMap::new(),
            pragma_once: HashMap::new(),
            pushed_macros: HashMap::new(),
            configuration_names: HashSet::new(),
            predefined_macros: Vec::new(),
            directive_diagnostics: Vec::new(),
            macro_order: 0,
        };
        pp.seed_builtin_macros();
        pp
    }

    fn seed_builtin_macros(&mut self) {
        for (name, source) in BUILTIN_PREDEFINES {
            let file = self.files.intern(PathBuf::from(name), HeaderKind::System);
            let nodes = self
                .parse_source(source, file, Condition::Constant(1))
                .expect("builtin predefines must parse cleanly");
            debug_assert!(
                nodes.is_empty(),
                "predefines should only contain #define directives"
            );
        }
        self.predefined_macros = self.macros.keys().cloned().collect();
        self.predefined_macros.sort();
    }

    pub fn set_configuration_names(&mut self, names: impl IntoIterator<Item = String>) {
        self.configuration_names = names.into_iter().collect();
    }

    pub fn parse_file(&mut self, path: &Path) -> Result<Vec<PPNode>, PPError> {
        let canon = path.canonicalize().unwrap_or_else(|_| path.to_path_buf());
        let src =
            read_source(&canon).map_err(|kind| self.render_error(PPFailure::unlocated(kind)))?;
        let file = self.files.intern(canon.clone(), HeaderKind::User);
        self.main_file = Some(file);
        self.open_stack.push(canon);
        let nodes = self
            .parse_source(&src, file, Condition::Constant(1))
            .map_err(|failure| self.render_error(failure))?;
        self.open_stack.pop();
        Ok(nodes)
    }

    pub fn parse_str(&mut self, name: &str, src: &str) -> Result<Vec<PPNode>, PPError> {
        let file = self.files.intern(PathBuf::from(name), HeaderKind::User);
        self.main_file = Some(file);
        self.parse_source(src, file, Condition::Constant(1))
            .map_err(|failure| self.render_error(failure))
    }

    fn parse_source(
        &mut self,
        src: &str,
        file: FileId,
        active: Condition,
    ) -> Result<Vec<PPNode>, PPFailure> {
        self.sources.insert(file, src.to_string());
        self.line_starts.insert(
            file,
            std::iter::once(0)
                .chain(src.match_indices('\n').map(|(index, _)| index + 1))
                .collect(),
        );
        let tokens = Lexer::new(file, src).with_newlines().tokenize();
        let items = syntax::parse(src, tokens)?;
        match controlling_macro(src, &items) {
            Some((index, section, guard)) => {
                self.walk_guarded(&items, index, section, &guard, &active)
            }
            None => self.walk_group(&items, &active),
        }
    }

    fn walk_guarded(
        &mut self,
        items: &[Item],
        index: usize,
        section: &IfSection,
        guard: &str,
        active: &Condition,
    ) -> Result<Vec<PPNode>, PPFailure> {
        let mut nodes = self.walk_group(&items[..index], active)?;
        let unguarded = normalize(&negate(&self.defined_condition(guard, false)));
        let branch_active = intersect(active, &unguarded);
        if is_satisfiable(&branch_active) {
            let body = section
                .branches
                .first()
                .map_or(&[][..], |branch| branch.body.as_slice());
            let body_nodes = self.walk_group(body, &branch_active)?;
            if is_satisfiable(&difference(active, &unguarded)) {
                nodes.push(conditional_node(section, vec![(unguarded, body_nodes)]));
            } else {
                nodes.extend(body_nodes);
            }
        }
        nodes.extend(self.walk_group(&items[index + 1..], active)?);
        Ok(nodes)
    }

    fn source(&self, file: FileId) -> &str {
        self.sources.get(&file).map_or("", String::as_str)
    }

    fn provenance(&self, loc: Loc) -> Provenance {
        let line = self.line_starts.get(&loc.file).map_or(0, |starts| {
            starts
                .partition_point(|&start| start <= loc.offset)
                .saturating_sub(1)
        });
        Provenance {
            file: loc.file,
            kind: self.files.kind(loc.file),
            line,
        }
    }

    fn walk_group(&mut self, items: &[Item], active: &Condition) -> Result<Vec<PPNode>, PPFailure> {
        let mut nodes = Vec::new();
        for item in items {
            match item {
                Item::Comment(comment) => nodes.push(Span::new(
                    PPNodeKind::Comment {
                        text: comment.value.clone(),
                        provenance: self.provenance(comment.spelling),
                    },
                    comment.spelling,
                    comment.spelling,
                )),
                Item::Text(tokens) => nodes.push(self.expand_line(tokens, active)),
                Item::Conditional(section) => nodes.push(self.walk_conditional(section, active)?),
                Item::Directive(directive) => match directive.name {
                    DirectiveName::Define => self.record_define(directive, active)?,
                    DirectiveName::Include | DirectiveName::IncludeNext => {
                        let include = include_target(self.source(directive.loc.file), directive)?;
                        nodes.extend(self.resolve_and_parse_include(
                            &include,
                            directive.arguments_loc(),
                            active,
                        )?);
                    }
                    DirectiveName::Undef => self.record_undef(directive, active)?,
                    DirectiveName::Pragma => self.record_pragma(directive, active),
                    DirectiveName::Error => {
                        self.record_directive_diagnostic(directive, Severity::Error, active)
                    }
                    DirectiveName::Warning => {
                        self.record_directive_diagnostic(directive, Severity::Warning, active)
                    }
                    DirectiveName::Line
                    | DirectiveName::LineMarker
                    | DirectiveName::Ident
                    | DirectiveName::Null => {}
                    _ => {
                        return Err(PPFailure::at(
                            directive.name_loc,
                            PPErrorKind::UnsupportedDirective,
                        ));
                    }
                },
            }
        }
        Ok(nodes)
    }

    fn expand_line(&self, source_tokens: &[Span<Token>], active: &Condition) -> PPNode {
        let loc = Span::cover((), source_tokens).spelling;
        let provenance = self.provenance(loc);
        let code = |condition: &Condition| {
            let expanded = Self::strip_pragma_operator(&self.expand_macros(
                source_tokens,
                &mut HashSet::new(),
                condition,
            ));
            Span::new(
                PPNodeKind::Code {
                    text: tokens_source(expanded.values()),
                    tokens: expanded,
                    provenance,
                },
                loc,
                loc,
            )
        };
        match self.divergent_macro_conditions(source_tokens, active) {
            Some(conditions) => Span::new(
                PPNodeKind::Conditional(PPConditional {
                    branches: conditions
                        .into_iter()
                        .map(|condition| {
                            let node = code(&condition);
                            (condition, vec![node])
                        })
                        .collect(),
                }),
                loc,
                loc,
            ),
            None => code(active),
        }
    }

    fn walk_conditional(
        &mut self,
        section: &IfSection,
        active: &Condition,
    ) -> Result<PPNode, PPFailure> {
        let mut branches = Vec::new();
        let mut excluded: Option<Condition> = None;
        for branch in &section.branches {
            let directive = &branch.directive;
            let test = match directive.name {
                DirectiveName::Ifdef | DirectiveName::Elifdef => {
                    let (name, _) =
                        self.macro_name(directive, directive_spelling(directive.name))?;
                    Some(self.defined_condition(&name, true))
                }
                DirectiveName::Ifndef | DirectiveName::Elifndef => {
                    let (name, _) =
                        self.macro_name(directive, directive_spelling(directive.name))?;
                    Some(normalize(&negate(&self.defined_condition(&name, true))))
                }
                DirectiveName::Else => None,
                _ => Some(self.evaluate_condition(
                    directive,
                    directive_spelling(directive.name),
                    active,
                )?),
            };
            let condition = match (excluded.take(), test) {
                (None, test) => test.unwrap_or(Condition::Constant(1)),
                (Some(prior), None) => {
                    let condition = Condition::Not(Box::new(prior.clone()));
                    excluded = Some(prior);
                    condition
                }
                (Some(prior), Some(test)) => {
                    let condition = Condition::And(
                        Box::new(Condition::Not(Box::new(prior.clone()))),
                        Box::new(test),
                    );
                    excluded = Some(Condition::Or(Box::new(prior), Box::new(condition.clone())));
                    condition
                }
            };
            if excluded.is_none() {
                excluded = Some(condition.clone());
            }
            let branch_active = conjunction(active, &condition);
            let body = if is_statically_false(&branch_active) {
                Vec::new()
            } else {
                self.walk_group(&branch.body, &branch_active)?
            };
            branches.push((condition, body));
        }
        Ok(conditional_node(section, branches))
    }

    fn evaluate_condition(
        &self,
        directive: &Directive,
        name: &'static str,
        active: &Condition,
    ) -> Result<Condition, PPFailure> {
        let expandable = Self::expandable_tokens(&directive.arguments);
        let mut cases = self
            .divergent_macro_conditions(&expandable, active)
            .unwrap_or_else(|| vec![active.clone()]);
        for operand in defined_operands(&directive.arguments) {
            let defined = self.defined_condition(&operand, true);
            cases = cases
                .iter()
                .flat_map(|case| [intersect(case, &defined), difference(case, &defined)])
                .filter(is_satisfiable)
                .collect();
        }
        if let [case] = cases.as_slice() {
            return self
                .evaluate_expanded_condition(directive, name, case)
                .map(Condition::Constant);
        }
        let mut combined = Condition::Constant(0);
        for case in cases {
            let value = self.evaluate_expanded_condition(directive, name, &case)?;
            combined = Condition::Or(
                Box::new(combined),
                Box::new(Condition::And(
                    Box::new(case),
                    Box::new(Condition::Constant(value)),
                )),
            );
        }
        Ok(normalize(&combined))
    }

    fn evaluate_expanded_condition(
        &self,
        directive: &Directive,
        name: &'static str,
        active: &Condition,
    ) -> Result<i64, PPFailure> {
        let expanded = self.expand_condition(&directive.arguments, active);
        const_expr::Parser::evaluate_with_defined(&expanded, &|macro_name| {
            self.is_defined(macro_name, active)
        })
        .map_err(|located| {
            let loc = match located.token {
                Some(index) => expanded
                    .get(index)
                    .map_or_else(|| directive.end_loc(), |token| token.expansion),
                None => directive.arguments_loc(),
            };
            PPFailure::at(
                loc,
                PPErrorKind::InvalidExpression {
                    directive: name,
                    message: located.error.to_string(),
                },
            )
        })
    }

    fn record_directive_diagnostic(
        &mut self,
        directive: &Directive,
        severity: Severity,
        active: &Condition,
    ) {
        let keyword = if severity == Severity::Warning {
            "#warning"
        } else {
            "#error"
        };
        let message = if directive.arguments.is_empty() {
            keyword.to_string()
        } else {
            format!("{keyword} {}", self.spelling(directive.arguments_loc()))
        };
        let error = self.render_error(PPFailure::at(
            directive.loc,
            PPErrorKind::Directive(message),
        ));
        self.directive_diagnostics.push(DirectiveDiagnostic {
            severity,
            condition: active.clone(),
            loc: directive.loc,
            error,
        });
    }

    fn record_pragma(&mut self, directive: &Directive, active: &Condition) {
        let pragma = directive
            .arguments
            .first()
            .and_then(|token| identifier(self.source(directive.loc.file), token));
        match pragma.as_deref() {
            Some("push_macro") => self.push_macro(directive, active),
            Some("pop_macro") => self.pop_macro(directive, active),
            _ => {}
        }
        if pragma.as_deref() == Some("once") {
            let path = self.files.path(directive.loc.file);
            let key = path.canonicalize().unwrap_or_else(|_| path.to_path_buf());
            self.pragma_once
                .entry(key)
                .or_default()
                .push(active.clone());
        }
    }

    fn is_defined(&self, name: &str, active: &Condition) -> bool {
        !is_satisfiable(&difference(active, &self.defined_condition(name, true)))
    }

    fn defined_condition(&self, name: &str, configurable: bool) -> Condition {
        let mut defined = Condition::Constant(0);
        for (case, entry) in self.macro_cases(name, &Condition::Constant(1)) {
            let case_defined = match entry {
                Some(entry) if entry.definition.is_some() => case,
                None if configurable && self.configuration_names.contains(name) => {
                    intersect(&case, &Condition::Defined(name.to_string()))
                }
                _ => continue,
            };
            defined = Condition::Or(Box::new(defined), Box::new(case_defined));
        }
        normalize(&defined)
    }

    fn spelling(&self, loc: Loc) -> &str {
        self.source(loc.file)
            .get(loc.offset..loc.offset + loc.length)
            .unwrap_or_default()
    }
}

fn conditional_node(section: &IfSection, branches: Vec<(Condition, Vec<PPNode>)>) -> PPNode {
    let loc = section
        .branches
        .first()
        .map_or(section.endif, |branch| branch.directive.loc)
        .through(section.endif);
    Span::new(
        PPNodeKind::Conditional(PPConditional { branches }),
        loc,
        loc,
    )
}

fn tokens_source<'a>(tokens: impl IntoIterator<Item = &'a Token>) -> String {
    tokens
        .into_iter()
        .map(String::from)
        .collect::<Vec<_>>()
        .join(" ")
}

fn lex(src: &str) -> Vec<Token> {
    lex_spanned(src)
        .into_iter()
        .map(|span| span.value)
        .collect()
}

fn lex_spanned(src: &str) -> Vec<Span<Token>> {
    Lexer::new(FileId(0), src).tokenize()
}
