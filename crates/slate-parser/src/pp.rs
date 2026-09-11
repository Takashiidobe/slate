use crate::ast::{Condition, FileId, HeaderKind, Provenance};
use crate::const_expr;
use crate::files::{Files, SearchPaths};
use std::path::{Path, PathBuf};

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

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct PPError {
    pub line: usize,
    pub message: String,
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
    search: &'a SearchPaths,
    open_stack: Vec<PathBuf>,
}

impl<'a> Preprocessor<'a> {
    pub fn new(search: &'a SearchPaths) -> Self {
        Preprocessor {
            files: Files::new(),
            search,
            open_stack: Vec::new(),
        }
    }

    pub fn parse_file(&mut self, path: &Path) -> Result<Vec<PPNode>, PPError> {
        let canon = path.canonicalize().unwrap_or_else(|_| path.to_path_buf());
        let src = std::fs::read_to_string(&canon)
            .unwrap_or_else(|e| panic!("failed to read {}: {e}", canon.display()));
        let file = self.files.intern(canon.clone(), HeaderKind::User);
        self.open_stack.push(canon);
        let nodes = self.parse_source(&src, file)?;
        self.open_stack.pop();
        Ok(nodes)
    }

    pub fn parse_str(&mut self, name: &str, src: &str) -> Result<Vec<PPNode>, PPError> {
        let file = self.files.intern(PathBuf::from(name), HeaderKind::User);
        self.parse_source(src, file)
    }

    fn parse_source(&mut self, src: &str, file: FileId) -> Result<Vec<PPNode>, PPError> {
        let lines: Vec<&str> = src.lines().collect();
        let mut pos = 0;
        let nodes = self.parse_block(&lines, &mut pos, file)?;
        if pos < lines.len() {
            return Err(self.error(pos, "unexpected conditional directive"));
        }
        Ok(nodes)
    }

    fn parse_block(
        &mut self,
        lines: &[&str],
        pos: &mut usize,
        file: FileId,
    ) -> Result<Vec<PPNode>, PPError> {
        let mut nodes = Vec::new();
        let provenance = Provenance {
            file,
            kind: self.files.kind(file),
        };

        while *pos < lines.len() {
            let raw = lines[*pos];
            let trimmed = raw.trim();

            if let Some(condition) = self.parse_opening_condition(trimmed, *pos)? {
                *pos += 1;
                nodes.push(PPNode::Conditional(
                    self.parse_conditional(lines, pos, file, condition)?,
                ));
            } else if let Some(include) = parse_include_directive(trimmed) {
                *pos += 1;
                nodes.extend(self.resolve_and_parse_include(&include, file)?);
            } else if trimmed == "#else" || trimmed.starts_with("#elif") || trimmed == "#endif" {
                break;
            } else if trimmed.starts_with('#') {
                return Err(self.error(*pos, "unsupported preprocessor directive"));
            } else if trimmed.is_empty() {
                *pos += 1;
            } else {
                nodes.push(PPNode::Code {
                    text: trimmed.to_string(),
                    provenance,
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
    ) -> Result<Option<Condition>, PPError> {
        if let Some(expression) = trimmed.strip_prefix("#if ") {
            let value = const_expr::evaluate(expression.trim()).map_err(|error| {
                self.error(line, format!("invalid #if expression: {}", error.0))
            })?;
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
    ) -> Result<PPConditional, PPError> {
        let mut branches = Vec::new();
        let mut prior = vec![first_condition.clone()];
        let first_body = self.parse_block(lines, pos, file)?;
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
                let else_body = self.parse_block(lines, pos, file)?;
                let excluded = prior
                    .clone()
                    .into_iter()
                    .reduce(|left, right| Condition::Or(Box::new(left), Box::new(right)))
                    .expect("conditional has an initial branch");
                branches.push((Condition::Not(Box::new(excluded)), else_body));
                continue;
            }
            if let Some(expression) = directive.strip_prefix("#elif ") {
                if saw_else {
                    return Err(self.error(*pos, "#elif after #else"));
                }
                let value = const_expr::evaluate(expression.trim()).map_err(|error| {
                    self.error(*pos, format!("invalid #elif expression: {}", error.0))
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
                let body = self.parse_block(lines, pos, file)?;
                branches.push((branch_condition, body));
                continue;
            }
            return Err(self.error(*pos, "expected #elif, #else, or #endif"));
        }
    }

    fn error(&self, line: usize, message: impl Into<String>) -> PPError {
        PPError {
            line,
            message: message.into(),
        }
    }

    fn resolve_and_parse_include(
        &mut self,
        include: &IncludeDirective,
        from: FileId,
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
        self.open_stack.push(resolved);
        let nodes = self.parse_source(&src, file)?;
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
