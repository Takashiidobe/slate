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

    pub fn parse_file(&mut self, path: &Path) -> Vec<PPNode> {
        let canon = path.canonicalize().unwrap_or_else(|_| path.to_path_buf());
        let src = std::fs::read_to_string(&canon)
            .unwrap_or_else(|e| panic!("failed to read {}: {e}", canon.display()));
        let file = self.files.intern(canon.clone(), HeaderKind::User);
        self.open_stack.push(canon);
        let nodes = self.parse_source(&src, file);
        self.open_stack.pop();
        nodes
    }

    pub fn parse_str(&mut self, name: &str, src: &str) -> Vec<PPNode> {
        let file = self.files.intern(PathBuf::from(name), HeaderKind::User);
        self.parse_source(src, file)
    }

    fn parse_source(&mut self, src: &str, file: FileId) -> Vec<PPNode> {
        let lines: Vec<&str> = src.lines().collect();
        let mut pos = 0;
        self.parse_block(&lines, &mut pos, file)
    }

    fn parse_block(&mut self, lines: &[&str], pos: &mut usize, file: FileId) -> Vec<PPNode> {
        let mut nodes = Vec::new();
        let provenance = Provenance {
            file,
            kind: self.files.kind(file),
        };

        while *pos < lines.len() {
            let raw = lines[*pos];
            let trimmed = raw.trim();

            if trimmed.starts_with("#if ") {
                let expression = trimmed.strip_prefix("#if ").unwrap().trim();
                let value = const_expr::evaluate(expression)
                    .unwrap_or_else(|error| panic!("invalid #if expression: {}", error.0));
                *pos += 1;
                let then_body = self.parse_block(lines, pos, file);
                let mut branches = vec![(Condition::Constant(value), then_body)];
                if *pos < lines.len() && lines[*pos].trim() == "#else" {
                    *pos += 1;
                    let else_body = self.parse_block(lines, pos, file);
                    branches.push((
                        Condition::Not(Box::new(Condition::Constant(value))),
                        else_body,
                    ));
                }
                assert_eq!(
                    lines.get(*pos).map(|l| l.trim()),
                    Some("#endif"),
                    "expected #endif"
                );
                *pos += 1;
                nodes.push(PPNode::Conditional(PPConditional { branches }));
            } else if trimmed.starts_with("#ifdef") || trimmed.starts_with("#ifndef") {
                let negate = trimmed.starts_with("#ifndef");
                let name = trimmed
                    .split_whitespace()
                    .nth(1)
                    .expect("macro name after #ifdef/#ifndef")
                    .to_string();
                *pos += 1;

                let then_body = self.parse_block(lines, pos, file);
                let then_cond = if negate {
                    Condition::Not(Box::new(Condition::Defined(name.clone())))
                } else {
                    Condition::Defined(name.clone())
                };
                let mut branches = vec![(then_cond, then_body)];

                if *pos < lines.len() && lines[*pos].trim() == "#else" {
                    *pos += 1;
                    let else_body = self.parse_block(lines, pos, file);
                    let else_cond = if negate {
                        Condition::Defined(name.clone())
                    } else {
                        Condition::Not(Box::new(Condition::Defined(name.clone())))
                    };
                    branches.push((else_cond, else_body));
                }

                assert_eq!(
                    lines.get(*pos).map(|l| l.trim()),
                    Some("#endif"),
                    "expected #endif"
                );
                *pos += 1;

                nodes.push(PPNode::Conditional(PPConditional { branches }));
            } else if let Some(include) = parse_include_directive(trimmed) {
                *pos += 1;
                nodes.extend(self.resolve_and_parse_include(&include, file));
            } else if trimmed == "#else" || trimmed == "#endif" {
                break;
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

        nodes
    }

    fn resolve_and_parse_include(
        &mut self,
        include: &IncludeDirective,
        from: FileId,
    ) -> Vec<PPNode> {
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
        let nodes = self.parse_source(&src, file);
        self.open_stack.pop();
        nodes
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
