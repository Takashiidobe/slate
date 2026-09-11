use crate::ast::{Condition, Conditional, FileId, HeaderKind, Provenance};
use crate::const_expr;
use crate::files::{Files, SearchPaths, display_path};
use crate::lexer::Token;
use crate::lexer::lex;
use miette::{Diagnostic, NamedSource, SourceSpan};
use std::collections::HashSet;
use std::path::{Path, PathBuf};
use thiserror::Error;

#[derive(Debug, Clone, PartialEq)]
pub enum PPNode {
    Code {
        text: String,
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
    pub replacement: Vec<Token>,
    pub provenance: Provenance,
    pub order: usize,
}

#[derive(Debug, Error, Diagnostic, Clone)]
#[error("{message}")]
pub struct PPError {
    pub message: String,
    #[source_code]
    pub source_code: NamedSource<String>,
    #[label]
    pub span: SourceSpan,
}

#[derive(Debug, Clone, PartialEq, Eq)]
struct PPFailure {
    line: usize,
    message: String,
}

enum IncludeDirective {
    Angled(String),
    Quoted(String),
}

fn parse_include_directive(trimmed: &str) -> Option<IncludeDirective> {
    let rest = trimmed.strip_prefix("#include")?.trim();
    if let Some(inner) = rest.strip_prefix('<').and_then(|s| s.strip_suffix('>')) {
        Some(IncludeDirective::Angled(inner.to_string()))
    } else {
        rest.strip_prefix('"')
            .and_then(|s| s.strip_suffix('"'))
            .map(|inner| IncludeDirective::Quoted(inner.to_string()))
    }
}

pub struct Preprocessor<'a> {
    pub files: Files,
    pub macros: std::collections::HashMap<String, Conditional<MacroDef>>,
    search: &'a SearchPaths,
    open_stack: Vec<PathBuf>,
    macro_order: usize,
}

impl<'a> Preprocessor<'a> {
    pub fn new(search: &'a SearchPaths) -> Self {
        Preprocessor {
            files: Files::new(),
            macros: std::collections::HashMap::new(),
            search,
            open_stack: Vec::new(),
            macro_order: 0,
        }
    }

    pub fn parse_file(&mut self, path: &Path) -> Result<Vec<PPNode>, PPError> {
        let canon = path.canonicalize().unwrap_or_else(|_| path.to_path_buf());
        let src = std::fs::read_to_string(&canon)
            .unwrap_or_else(|e| panic!("failed to read {}: {e}", canon.display()));
        let file = self.files.intern(canon.clone(), HeaderKind::User);
        self.open_stack.push(canon.clone());
        let nodes = self.parse_source(&display_path(&canon), &src, file, Condition::Constant(1))?;
        self.open_stack.pop();
        Ok(nodes)
    }

    pub fn parse_str(&mut self, name: &str, src: &str) -> Result<Vec<PPNode>, PPError> {
        let file = self.files.intern(PathBuf::from(name), HeaderKind::User);
        self.parse_source(name, src, file, Condition::Constant(1))
    }

    fn parse_source(
        &mut self,
        name: &str,
        src: &str,
        file: FileId,
        active: Condition,
    ) -> Result<Vec<PPNode>, PPError> {
        let lines: Vec<&str> = src.lines().collect();
        let mut pos = 0;
        let nodes = self
            .parse_block(&lines, &mut pos, file, &active)
            .map_err(|error| self.with_source(error, name, src))?;
        if pos < lines.len() {
            return Err(self.with_source(
                self.error(pos, "unexpected conditional directive"),
                name,
                src,
            ));
        }
        Ok(nodes)
    }

    fn parse_block(
        &mut self,
        lines: &[&str],
        pos: &mut usize,
        file: FileId,
        active: &Condition,
    ) -> Result<Vec<PPNode>, PPFailure> {
        let mut nodes = Vec::new();
        let provenance = Provenance {
            file,
            kind: self.files.kind(file),
            line: 0,
        };

        while *pos < lines.len() {
            let raw = lines[*pos];
            let trimmed = raw.trim();

            if let Some(condition) = self.parse_opening_condition(trimmed, *pos)? {
                *pos += 1;
                nodes.push(PPNode::Conditional(
                    self.parse_conditional(lines, pos, file, condition, active)?,
                ));
            } else if let Some(include) = parse_include_directive(trimmed) {
                *pos += 1;
                nodes.extend(
                    self.resolve_and_parse_include(&include, file, active)
                        .map_err(|error| self.error(*pos, error.to_string()))?,
                );
            } else if let Some(rest) = trimmed.strip_prefix("#define") {
                self.record_define(rest.trim_start(), file, *pos, active)?;
                *pos += 1;
            } else if trimmed.starts_with("#undef") {
                *pos += 1;
            } else if trimmed == "#else" || trimmed.starts_with("#elif") || trimmed == "#endif" {
                break;
            } else if trimmed.starts_with('#') {
                return Err(self.error(*pos, "unsupported preprocessor directive"));
            } else if trimmed.is_empty() {
                *pos += 1;
            } else {
                let expanded =
                    self.expand_object_macros(&lex(trimmed), &mut HashSet::new(), active);
                nodes.push(PPNode::Code {
                    text: tokens_source(&expanded),
                    provenance: Provenance {
                        line: *pos,
                        ..provenance
                    },
                });
                *pos += 1;
            }
        }

        Ok(nodes)
    }

    fn parse_opening_condition(
        &self,
        trimmed: &str,
        line: usize,
    ) -> Result<Option<Condition>, PPFailure> {
        if let Some(expression) = trimmed.strip_prefix("#if ") {
            let value = const_expr::Parser::evaluate(&lex(expression.trim()))
                .map_err(|error| self.error(line, format!("invalid #if expression: {error}")))?;
            return Ok(Some(Condition::Constant(value)));
        }
        for (directive, negate) in [("#ifdef", false), ("#ifndef", true)] {
            if let Some(rest) = trimmed.strip_prefix(directive) {
                let name = rest.trim();
                if name.is_empty() || name.split_whitespace().count() != 1 {
                    return Err(self.error(line, format!("expected macro name after {directive}")));
                }
                let condition = Condition::Defined(name.to_string());
                return Ok(Some(if negate {
                    Condition::Not(Box::new(condition))
                } else {
                    condition
                }));
            }
        }
        Ok(None)
    }

    fn parse_conditional(
        &mut self,
        lines: &[&str],
        pos: &mut usize,
        file: FileId,
        first_condition: Condition,
        active: &Condition,
    ) -> Result<PPConditional, PPFailure> {
        let mut branches = Vec::new();
        let mut prior = vec![first_condition.clone()];
        let first_active = conjunction(active, &first_condition);
        let first_body = self.parse_block(lines, pos, file, &first_active)?;
        branches.push((first_condition, first_body));
        let mut saw_else = false;

        loop {
            let Some(directive) = lines.get(*pos).map(|line| line.trim()) else {
                return Err(self.error(*pos, "expected #endif"));
            };
            if directive == "#endif" {
                *pos += 1;
                return Ok(PPConditional { branches });
            }
            if directive == "#else" {
                if saw_else {
                    return Err(self.error(*pos, "multiple #else directives"));
                }
                saw_else = true;
                *pos += 1;
                let excluded = prior
                    .clone()
                    .into_iter()
                    .reduce(|left, right| Condition::Or(Box::new(left), Box::new(right)))
                    .expect("conditional has an initial branch");
                let else_condition = Condition::Not(Box::new(excluded));
                let else_active = conjunction(active, &else_condition);
                let else_body = self.parse_block(lines, pos, file, &else_active)?;
                branches.push((else_condition, else_body));
                continue;
            }
            if let Some(expression) = directive.strip_prefix("#elif ") {
                if saw_else {
                    return Err(self.error(*pos, "#elif after #else"));
                }
                let value =
                    const_expr::Parser::evaluate(&lex(expression.trim())).map_err(|error| {
                        self.error(*pos, format!("invalid #elif expression: {error}"))
                    })?;
                let condition = Condition::Constant(value);
                let excluded = prior
                    .iter()
                    .cloned()
                    .reduce(|left, right| Condition::Or(Box::new(left), Box::new(right)))
                    .expect("conditional has an initial branch");
                let branch_condition = Condition::And(
                    Box::new(Condition::Not(Box::new(excluded))),
                    Box::new(condition),
                );
                prior.push(branch_condition.clone());
                *pos += 1;
                let body =
                    self.parse_block(lines, pos, file, &conjunction(active, &branch_condition))?;
                branches.push((branch_condition, body));
                continue;
            }
            return Err(self.error(*pos, "expected #elif, #else, or #endif"));
        }
    }

    fn error(&self, line: usize, message: impl Into<String>) -> PPFailure {
        PPFailure {
            line,
            message: message.into(),
        }
    }

    fn record_define(
        &mut self,
        rest: &str,
        file: FileId,
        line: usize,
        condition: &Condition,
    ) -> Result<(), PPFailure> {
        let name_end = rest
            .find(|character: char| !character.is_ascii_alphanumeric() && character != '_')
            .unwrap_or(rest.len());
        if name_end == 0 {
            return Err(self.error(line, "expected macro name after #define"));
        }
        let name = rest[..name_end].to_string();
        let after_name = &rest[name_end..];
        let (parameters, variadic, replacement_text) = if after_name.starts_with('(') {
            let Some(close) = after_name.find(')') else {
                return Err(self.error(line, "expected `)` after macro parameters"));
            };
            let parameter_text = &after_name[1..close];
            let mut parameters = Vec::new();
            let mut variadic = false;
            for parameter in parameter_text
                .split(',')
                .map(str::trim)
                .filter(|p| !p.is_empty())
            {
                if parameter == "..." {
                    variadic = true;
                } else {
                    parameters.push(parameter.to_string());
                }
            }
            (Some(parameters), variadic, &after_name[close + 1..])
        } else {
            (None, false, after_name)
        };
        self.macros
            .entry(name)
            .or_insert_with(|| Conditional {
                branches: Vec::new(),
            })
            .branches
            .push((
                condition.clone(),
                MacroDef {
                    parameters,
                    variadic,
                    replacement: lex(replacement_text.trim()),
                    provenance: Provenance {
                        file,
                        kind: self.files.kind(file),
                        line,
                    },
                    order: self.macro_order,
                },
            ));
        self.macro_order += 1;
        Ok(())
    }

    fn expand_object_macros(
        &self,
        tokens: &[Token],
        disabled: &mut HashSet<String>,
        active: &Condition,
    ) -> Vec<Token> {
        let mut expanded = Vec::new();
        for token in tokens {
            let Token::Ident(name) = token else {
                expanded.push(token.clone());
                continue;
            };
            let Some(macro_def) = self.macros.get(name).and_then(|conditional| {
                conditional
                    .branches
                    .iter()
                    .rev()
                    .find(|(condition, _)| condition == active)
                    .map(|(_, definition)| definition.clone())
            }) else {
                expanded.push(token.clone());
                continue;
            };
            if macro_def.parameters.is_some() || !disabled.insert(name.clone()) {
                expanded.push(token.clone());
                continue;
            }
            expanded.extend(self.expand_object_macros(&macro_def.replacement, disabled, active));
            disabled.remove(name);
        }
        expanded
    }

    fn with_source(&self, error: PPFailure, name: &str, source: &str) -> PPError {
        let offset: usize = source
            .lines()
            .take(error.line)
            .map(|line| line.len() + 1)
            .sum();
        PPError {
            message: error.message,
            source_code: NamedSource::new(name, source.to_string()).with_language("C"),
            span: SourceSpan::new(offset.into(), 1),
        }
    }

    fn resolve_and_parse_include(
        &mut self,
        include: &IncludeDirective,
        from: FileId,
        active: &Condition,
    ) -> Result<Vec<PPNode>, PPError> {
        let (resolved, kind) = self.resolve_include(include, from);
        assert!(
            !self.open_stack.contains(&resolved),
            "include cycle detected: {}",
            resolved.display()
        );

        let src = std::fs::read_to_string(&resolved)
            .unwrap_or_else(|e| panic!("failed to read {}: {e}", resolved.display()));
        let file = self.files.intern(resolved.clone(), kind);
        self.open_stack.push(resolved.clone());
        let nodes = self.parse_source(&display_path(&resolved), &src, file, active.clone())?;
        self.open_stack.pop();
        Ok(nodes)
    }

    fn resolve_include(&self, include: &IncludeDirective, from: FileId) -> (PathBuf, HeaderKind) {
        match include {
            IncludeDirective::Angled(name) => self
                .search
                .system
                .iter()
                .map(|dir| dir.join(name))
                .find(|candidate| candidate.is_file())
                .map(|path| (path, HeaderKind::System))
                .unwrap_or_else(|| panic!("system header not found in search path: <{name}>")),
            IncludeDirective::Quoted(name) => {
                let same_dir = self.files.path(from).parent().map(|dir| dir.join(name));
                same_dir
                    .filter(|c| c.is_file())
                    .map(|path| (path, HeaderKind::User))
                    .or_else(|| {
                        self.search
                            .user
                            .iter()
                            .map(|dir| dir.join(name))
                            .find(|c| c.is_file())
                            .map(|path| (path, HeaderKind::User))
                    })
                    .or_else(|| {
                        self.search
                            .system
                            .iter()
                            .map(|dir| dir.join(name))
                            .find(|c| c.is_file())
                            .map(|path| (path, HeaderKind::System))
                    })
                    .unwrap_or_else(|| panic!("header not found in search path: \"{name}\""))
            }
        }
    }
}

fn conjunction(active: &Condition, branch: &Condition) -> Condition {
    Condition::And(Box::new(active.clone()), Box::new(branch.clone()))
}

fn tokens_source(tokens: &[Token]) -> String {
    tokens
        .iter()
        .map(String::from)
        .collect::<Vec<_>>()
        .join(" ")
}
