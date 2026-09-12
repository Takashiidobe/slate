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
    conjunction, is_statically_false, is_statically_true, replace_subterm, simplify_condition,
};
pub use error::PPError;
use error::{PPErrorKind, PPFailure};
use include::{include_target, read_source};
use std::collections::{HashMap, HashSet};
use std::path::{Path, PathBuf};
use syntax::{Directive, DirectiveName, IfSection, Item};

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
    macro_order: usize,
}

const CLANG_X86_64_LINUX_GNU_PREDEFINES: &str =
    include_str!("../predefines/clang_x86_64_linux_gnu.h");

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
            macro_order: 0,
        };
        pp.seed_builtin_macros();
        pp
    }

    fn seed_builtin_macros(&mut self) {
        let file = self.files.intern(
            PathBuf::from("<clang-x86_64-linux-gnu-predefines>"),
            HeaderKind::System,
        );
        let nodes = self
            .parse_source(
                CLANG_X86_64_LINUX_GNU_PREDEFINES,
                file,
                Condition::Constant(1),
            )
            .expect("builtin predefines must parse cleanly");
        debug_assert!(
            nodes.is_empty(),
            "predefines should only contain #define directives"
        );
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
        self.walk_group(&items, &active)
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
                    DirectiveName::Error if is_statically_true(active) => {
                        let message = self.spelling(directive.arguments_loc());
                        return Err(PPFailure::at(
                            directive.loc,
                            PPErrorKind::ErrorDirective(message.to_string()),
                        ));
                    }
                    DirectiveName::Error | DirectiveName::Null => {}
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
            let condition = match (excluded.take(), directive.name) {
                (None, DirectiveName::Ifdef) => {
                    Condition::Defined(self.macro_name(directive, "#ifdef")?.0)
                }
                (None, DirectiveName::Ifndef) => Condition::Not(Box::new(Condition::Defined(
                    self.macro_name(directive, "#ifndef")?.0,
                ))),
                (None, _) => self.evaluate_condition(directive, "#if", active)?,
                (Some(prior), DirectiveName::Else) => {
                    let condition = Condition::Not(Box::new(prior.clone()));
                    excluded = Some(prior);
                    condition
                }
                (Some(prior), _) => {
                    let condition = Condition::And(
                        Box::new(Condition::Not(Box::new(prior.clone()))),
                        Box::new(self.evaluate_condition(directive, "#elif", active)?),
                    );
                    excluded = Some(Condition::Or(Box::new(prior), Box::new(condition.clone())));
                    condition
                }
            };
            if excluded.is_none() {
                excluded = Some(condition.clone());
            }
            let branch_active = conjunction(active, &condition);
            let order_before = self.macro_order;
            let body = if is_statically_false(&branch_active) {
                Vec::new()
            } else {
                self.walk_group(&branch.body, &branch_active)?
            };
            self.concretize_guard(&condition, order_before);
            branches.push((condition, body));
        }
        let loc = section
            .branches
            .first()
            .map_or(section.endif, |branch| branch.directive.loc)
            .through(section.endif);
        Ok(Span::new(
            PPNodeKind::Conditional(PPConditional { branches }),
            loc,
            loc,
        ))
    }

    fn evaluate_condition(
        &self,
        directive: &Directive,
        name: &'static str,
        active: &Condition,
    ) -> Result<Condition, PPFailure> {
        const_expr::Parser::evaluate_with_defined(&directive.arguments, &|macro_name| {
            self.is_defined(macro_name, active)
        })
        .map(Condition::Constant)
        .map_err(|located| {
            let loc = match located.token {
                Some(index) => directive
                    .arguments
                    .get(index)
                    .map_or_else(|| directive.end_loc(), |token| token.spelling),
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

    fn is_defined(&self, name: &str, active: &Condition) -> bool {
        match self.visible_entry(name, active) {
            Some(entry) => entry.definition.is_some(),
            None => self.macros.get(name).is_some_and(|conditional| {
                conditional
                    .branches
                    .iter()
                    .any(|(_, entry)| entry.definition.is_some())
            }),
        }
    }

    fn spelling(&self, loc: Loc) -> &str {
        self.source(loc.file)
            .get(loc.offset..loc.offset + loc.length)
            .unwrap_or_default()
    }

    fn concretize_guard(&mut self, branch_condition: &Condition, order_from: usize) {
        let Condition::Not(inner) = branch_condition else {
            return;
        };
        let Condition::Defined(name) = inner.as_ref() else {
            return;
        };
        let Some(conditional) = self.macros.get(name) else {
            return;
        };
        let guard_defined_here = conditional
            .branches
            .iter()
            .any(|(_, entry)| entry.definition.is_some() && entry.order >= order_from);
        if !guard_defined_here {
            return;
        }
        for conditional in self.macros.values_mut() {
            for (condition, entry) in conditional.branches.iter_mut() {
                if entry.order >= order_from {
                    let rewritten = simplify_condition(&replace_subterm(
                        condition,
                        branch_condition,
                        &Condition::Constant(1),
                    ));
                    if &rewritten != condition {
                        *condition = rewritten;
                    }
                }
            }
        }
    }
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
