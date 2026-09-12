use crate::ast::{Condition, Conditional, FileId, HeaderKind, Provenance, Span};
use crate::const_expr;
use crate::files::{Files, SearchPaths, display_path};
use crate::lexer::{Lexer, Token};
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
    Next(String),
}

fn normalize_directive(raw: &str) -> String {
    let trimmed = raw.trim();
    match trimmed.strip_prefix('#') {
        Some(rest) => format!("#{}", rest.trim_start()),
        None => trimmed.to_string(),
    }
}

fn parse_include_directive(trimmed: &str) -> Option<IncludeDirective> {
    if let Some(rest) = trimmed.strip_prefix("#include_next") {
        let rest = rest.trim();
        let name = rest
            .strip_prefix('<')
            .and_then(|s| s.strip_suffix('>'))
            .or_else(|| rest.strip_prefix('"').and_then(|s| s.strip_suffix('"')))?;
        return Some(IncludeDirective::Next(name.to_string()));
    }
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
    pub main_file: Option<FileId>,
    search: &'a SearchPaths,
    open_stack: Vec<PathBuf>,
    macro_order: usize,
}

const CLANG_X86_64_LINUX_GNU_PREDEFINES: &str = include_str!("predefines/clang_x86_64_linux_gnu.h");

impl<'a> Preprocessor<'a> {
    pub fn new(search: &'a SearchPaths) -> Self {
        let mut pp = Preprocessor {
            files: Files::new(),
            macros: std::collections::HashMap::new(),
            main_file: None,
            search,
            open_stack: Vec::new(),
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
                "<predefines>",
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
        let src = std::fs::read_to_string(&canon)
            .unwrap_or_else(|e| panic!("failed to read {}: {e}", canon.display()));
        let file = self.files.intern(canon.clone(), HeaderKind::User);
        self.main_file = Some(file);
        self.open_stack.push(canon.clone());
        let nodes = self.parse_source(&display_path(&canon), &src, file, Condition::Constant(1))?;
        self.open_stack.pop();
        Ok(nodes)
    }

    pub fn parse_str(&mut self, name: &str, src: &str) -> Result<Vec<PPNode>, PPError> {
        let file = self.files.intern(PathBuf::from(name), HeaderKind::User);
        self.main_file = Some(file);
        self.parse_source(name, src, file, Condition::Constant(1))
    }

    fn parse_source(
        &mut self,
        name: &str,
        src: &str,
        file: FileId,
        active: Condition,
    ) -> Result<Vec<PPNode>, PPError> {
        let uncommented = strip_comments(src);
        let spliced = splice_continuations(&uncommented);
        let lines: Vec<&str> = spliced.lines().collect();
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
            let trimmed = normalize_directive(raw);
            let trimmed = trimmed.as_str();

            if let Some(condition) = self.parse_opening_condition(trimmed, *pos)? {
                *pos += 1;
                nodes.push(PPNode::Conditional(
                    self.parse_conditional(lines, pos, file, condition, active)?,
                ));
            } else if let Some(include) = parse_include_directive(trimmed) {
                *pos += 1;
                nodes.extend(
                    self.resolve_and_parse_include(&include, file, active)
                        .map_err(|error| {
                            eprintln!("DEBUG nested: {error:?}");
                            self.error(*pos, error.to_string())
                        })?,
                );
            } else if let Some(rest) = trimmed.strip_prefix("#define") {
                self.record_define(rest.trim_start(), file, *pos, active)?;
                *pos += 1;
            } else if trimmed.starts_with("#undef") || trimmed.starts_with("#error") {
                *pos += 1;
            } else if trimmed == "#else" || trimmed.starts_with("#elif") || trimmed == "#endif" {
                break;
            } else if trimmed.starts_with('#') {
                return Err(self.error(*pos, "unsupported preprocessor directive"));
            } else if trimmed.is_empty() {
                *pos += 1;
            } else {
                let tokens = lex(trimmed);
                let provenance = Provenance {
                    line: *pos,
                    ..provenance
                };
                if let Some(conditions) = self.divergent_macro_conditions(&tokens, active) {
                    nodes.push(PPNode::Conditional(PPConditional {
                        branches: conditions
                            .into_iter()
                            .map(|condition| {
                                let expanded = Self::strip_pragma_operator(&self.expand_macros(
                                    &tokens,
                                    &mut HashSet::new(),
                                    &condition,
                                ));
                                (
                                    condition,
                                    vec![PPNode::Code {
                                        text: tokens_source(&expanded),
                                        provenance,
                                    }],
                                )
                            })
                            .collect(),
                    }));
                } else {
                    let expanded = Self::strip_pragma_operator(&self.expand_macros(
                        &tokens,
                        &mut HashSet::new(),
                        active,
                    ));
                    nodes.push(PPNode::Code {
                        text: tokens_source(&expanded),
                        provenance,
                    });
                }
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
            let value = const_expr::Parser::evaluate_with_defined(
                &lex_spanned(expression.trim()),
                &|name| self.macros.contains_key(name),
            )
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

    fn parse_body_or_skip(
        &mut self,
        lines: &[&str],
        pos: &mut usize,
        file: FileId,
        active: &Condition,
    ) -> Result<Vec<PPNode>, PPFailure> {
        if is_statically_false(active) {
            skip_block(lines, pos);
            return Ok(Vec::new());
        }
        self.parse_block(lines, pos, file, active)
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
        let order_before = self.macro_order;
        let first_body = self.parse_body_or_skip(lines, pos, file, &first_active)?;
        self.concretize_guard(&first_condition, order_before);
        branches.push((first_condition, first_body));
        let mut saw_else = false;

        loop {
            let Some(directive) = lines.get(*pos).map(|line| normalize_directive(line)) else {
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
                let order_before = self.macro_order;
                let else_body = self.parse_body_or_skip(lines, pos, file, &else_active)?;
                self.concretize_guard(&else_condition, order_before);
                branches.push((else_condition, else_body));
                continue;
            }
            if let Some(expression) = directive.strip_prefix("#elif ") {
                if saw_else {
                    return Err(self.error(*pos, "#elif after #else"));
                }
                let value = const_expr::Parser::evaluate_with_defined(
                    &lex_spanned(expression.trim()),
                    &|name| self.macros.contains_key(name),
                )
                .map_err(|error| self.error(*pos, format!("invalid #elif expression: {error}")))?;
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
                let branch_active = conjunction(active, &branch_condition);
                let order_before = self.macro_order;
                let body = self.parse_body_or_skip(lines, pos, file, &branch_active)?;
                self.concretize_guard(&branch_condition, order_before);
                branches.push((branch_condition, body));
                continue;
            }
            return Err(self.error(*pos, "expected #elif, #else, or #endif"));
        }
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
            .any(|(_, def)| def.order >= order_from);
        if !guard_defined_here {
            return;
        }
        for conditional in self.macros.values_mut() {
            for (condition, definition) in conditional.branches.iter_mut() {
                if definition.order >= order_from {
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

    fn strip_pragma_operator(tokens: &[Token]) -> Vec<Token> {
        let mut result = Vec::with_capacity(tokens.len());
        let mut i = 0;
        while i < tokens.len() {
            if tokens[i] == Token::Ident("_Pragma".to_string())
                && tokens.get(i + 1) == Some(&Token::LParen)
                && matches!(tokens.get(i + 2), Some(Token::StringLit(_)))
                && tokens.get(i + 3) == Some(&Token::RParen)
            {
                i += 4;
                continue;
            }
            result.push(tokens[i].clone());
            i += 1;
        }
        result
    }

    fn expand_macros(
        &self,
        tokens: &[Token],
        disabled: &mut HashSet<String>,
        active: &Condition,
    ) -> Vec<Token> {
        let mut expanded = Vec::new();
        let mut i = 0;
        while i < tokens.len() {
            let token = &tokens[i];
            let Token::Ident(name) = token else {
                expanded.push(token.clone());
                i += 1;
                continue;
            };
            let Some(macro_def) = self.macros.get(name).and_then(|conditional| {
                conditional
                    .branches
                    .iter()
                    .rev()
                    .find(|(condition, _)| condition == active || is_statically_true(condition))
                    .map(|(_, definition)| definition.clone())
            }) else {
                expanded.push(token.clone());
                i += 1;
                continue;
            };
            if !disabled.insert(name.clone()) {
                expanded.push(token.clone());
                i += 1;
                continue;
            }
            let Some(parameters) = macro_def.parameters.as_ref() else {
                expanded.extend(self.expand_macros(&macro_def.replacement, disabled, active));
                disabled.remove(name);
                i += 1;
                continue;
            };
            let Some((arguments, end)) = invocation_arguments(tokens, i + 1) else {
                disabled.remove(name);
                expanded.push(token.clone());
                i += 1;
                continue;
            };
            if !macro_def.variadic && arguments.len() != parameters.len()
                || macro_def.variadic && arguments.len() < parameters.len()
            {
                disabled.remove(name);
                expanded.push(token.clone());
                i += 1;
                continue;
            }
            let replacement = substitute_function_macro(
                &macro_def, parameters, &arguments, self, disabled, active,
            );
            expanded.extend(self.expand_macros(&replacement, disabled, active));
            disabled.remove(name);
            i = end;
        }
        expanded
    }

    fn divergent_macro_conditions(
        &self,
        tokens: &[Token],
        active: &Condition,
    ) -> Option<Vec<Condition>> {
        for (index, token) in tokens.iter().enumerate() {
            let Token::Ident(name) = token else {
                continue;
            };
            let Some(conditional) = self.macros.get(name) else {
                continue;
            };
            if conditional
                .branches
                .iter()
                .any(|(condition, _)| condition == active)
            {
                continue;
            }
            if conditional
                .branches
                .iter()
                .all(|(_, definition)| definition.parameters.is_some())
                && tokens.get(index + 1) != Some(&Token::LParen)
            {
                continue;
            }
            let mut conditions = Vec::new();
            for (condition, _) in &conditional.branches {
                if !conditions.contains(condition) {
                    conditions.push(condition.clone());
                }
            }
            if conditions.len() > 1 {
                return Some(conditions);
            }
        }
        None
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
            IncludeDirective::Next(name) => {
                let current_dir = self.files.path(from).parent();
                let start = current_dir
                    .and_then(|dir| {
                        self.search
                            .system
                            .iter()
                            .position(|candidate| candidate == dir)
                    })
                    .map_or(0, |index| index + 1);
                self.search.system[start..]
                    .iter()
                    .map(|dir| dir.join(name))
                    .find(|candidate| candidate.is_file())
                    .map(|path| (path, HeaderKind::System))
                    .unwrap_or_else(|| {
                        panic!("system header not found via #include_next in search path: <{name}>")
                    })
            }
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

fn strip_comments(src: &str) -> String {
    let mut out = String::with_capacity(src.len());
    let mut chars = src.chars().peekable();
    let mut in_string: Option<char> = None;
    while let Some(c) = chars.next() {
        if let Some(quote) = in_string {
            out.push(c);
            if c == '\\' {
                if let Some(next) = chars.next() {
                    out.push(next);
                }
            } else if c == quote {
                in_string = None;
            }
            continue;
        }
        match c {
            '"' | '\'' => {
                in_string = Some(c);
                out.push(c);
            }
            '/' if chars.peek() == Some(&'/') => {
                chars.next();
                for ch in chars.by_ref() {
                    if ch == '\n' {
                        out.push('\n');
                        break;
                    }
                }
            }
            '/' if chars.peek() == Some(&'*') => {
                chars.next();
                let mut prev = '\0';
                for ch in chars.by_ref() {
                    if prev == '*' && ch == '/' {
                        break;
                    }
                    if ch == '\n' {
                        out.push('\n');
                    }
                    prev = ch;
                }
            }
            _ => out.push(c),
        }
    }
    out
}

fn splice_continuations(src: &str) -> String {
    let mut logical: Vec<String> = src.lines().map(str::to_string).collect();
    for i in 0..logical.len() {
        let mut next_line = i + 1;
        while logical[i].ends_with('\\') && next_line < logical.len() {
            logical[i].pop();
            let next = std::mem::take(&mut logical[next_line]);
            logical[i].push_str(&next);
            next_line += 1;
        }
    }
    logical.join("\n")
}

fn is_statically_false(condition: &Condition) -> bool {
    match condition {
        Condition::Constant(0) => true,
        Condition::And(left, right) => is_statically_false(left) || is_statically_false(right),
        Condition::Or(left, right) => is_statically_false(left) && is_statically_false(right),
        Condition::Not(inner) => is_statically_true(inner),
        _ => false,
    }
}

fn is_statically_true(condition: &Condition) -> bool {
    match condition {
        Condition::Constant(value) => *value != 0,
        Condition::And(left, right) => is_statically_true(left) && is_statically_true(right),
        Condition::Or(left, right) => is_statically_true(left) || is_statically_true(right),
        Condition::Not(inner) => is_statically_false(inner),
        _ => false,
    }
}

fn skip_block(lines: &[&str], pos: &mut usize) {
    let mut depth = 0usize;
    while *pos < lines.len() {
        let trimmed = normalize_directive(lines[*pos]);
        let trimmed = trimmed.as_str();
        if trimmed.starts_with("#if") {
            depth += 1;
            *pos += 1;
        } else if trimmed == "#endif" {
            if depth == 0 {
                return;
            }
            depth -= 1;
            *pos += 1;
        } else if depth == 0 && (trimmed == "#else" || trimmed.starts_with("#elif")) {
            return;
        } else {
            *pos += 1;
        }
    }
}

fn conjunction(active: &Condition, branch: &Condition) -> Condition {
    simplify_condition(&Condition::And(
        Box::new(active.clone()),
        Box::new(branch.clone()),
    ))
}

fn simplify_condition(condition: &Condition) -> Condition {
    match condition {
        Condition::And(left, right) => {
            let left = simplify_condition(left);
            let right = simplify_condition(right);
            match (&left, &right) {
                (Condition::Constant(0), _) | (_, Condition::Constant(0)) => Condition::Constant(0),
                (Condition::Constant(v), _) if *v != 0 => right,
                (_, Condition::Constant(v)) if *v != 0 => left,
                _ => Condition::And(Box::new(left), Box::new(right)),
            }
        }
        Condition::Or(left, right) => {
            let left = simplify_condition(left);
            let right = simplify_condition(right);
            match (&left, &right) {
                (Condition::Constant(v), _) if *v != 0 => Condition::Constant(1),
                (_, Condition::Constant(v)) if *v != 0 => Condition::Constant(1),
                (Condition::Constant(0), _) => right,
                (_, Condition::Constant(0)) => left,
                _ => Condition::Or(Box::new(left), Box::new(right)),
            }
        }
        Condition::Not(inner) => match simplify_condition(inner) {
            Condition::Constant(v) => Condition::Constant(if v != 0 { 0 } else { 1 }),
            other => Condition::Not(Box::new(other)),
        },
        Condition::Defined(_) | Condition::Constant(_) => condition.clone(),
    }
}

fn replace_subterm(
    condition: &Condition,
    target: &Condition,
    replacement: &Condition,
) -> Condition {
    if condition == target {
        return replacement.clone();
    }
    match condition {
        Condition::Not(inner) => {
            Condition::Not(Box::new(replace_subterm(inner, target, replacement)))
        }
        Condition::And(left, right) => Condition::And(
            Box::new(replace_subterm(left, target, replacement)),
            Box::new(replace_subterm(right, target, replacement)),
        ),
        Condition::Or(left, right) => Condition::Or(
            Box::new(replace_subterm(left, target, replacement)),
            Box::new(replace_subterm(right, target, replacement)),
        ),
        Condition::Defined(_) | Condition::Constant(_) => condition.clone(),
    }
}

fn invocation_arguments(tokens: &[Token], start: usize) -> Option<(Vec<Vec<Token>>, usize)> {
    if tokens.get(start) != Some(&Token::LParen) {
        return None;
    }
    let mut arguments = Vec::new();
    let mut current = Vec::new();
    let mut depth = 0;
    let mut i = start + 1;
    while i < tokens.len() {
        match &tokens[i] {
            Token::LParen => {
                depth += 1;
                current.push(tokens[i].clone());
            }
            Token::RParen if depth == 0 => {
                if !current.is_empty() || !arguments.is_empty() {
                    arguments.push(current);
                }
                return Some((arguments, i + 1));
            }
            Token::RParen => {
                depth -= 1;
                current.push(tokens[i].clone());
            }
            Token::Comma if depth == 0 => {
                arguments.push(std::mem::take(&mut current));
            }
            _ => current.push(tokens[i].clone()),
        }
        i += 1;
    }
    None
}

fn substitute_function_macro(
    definition: &MacroDef,
    parameters: &[String],
    arguments: &[Vec<Token>],
    preprocessor: &Preprocessor<'_>,
    disabled: &mut HashSet<String>,
    active: &Condition,
) -> Vec<Token> {
    let expanded_arguments = arguments
        .iter()
        .map(|argument| preprocessor.expand_macros(argument, disabled, active))
        .collect::<Vec<_>>();
    let mut output = Vec::new();
    let mut i = 0;
    while i < definition.replacement.len() {
        let token = &definition.replacement[i];
        if *token == Token::Hash
            && i + 1 < definition.replacement.len()
            && let Token::Ident(name) = &definition.replacement[i + 1]
        {
            let argument = if name == "__VA_ARGS__" {
                Some(variadic_tokens(arguments, parameters.len()))
            } else {
                macro_argument(name, parameters, arguments).map(<[Token]>::to_vec)
            };
            if let Some(argument) = argument {
                output.push(Token::StringLit(tokens_source(&argument)));
                i += 2;
                continue;
            }
        }
        if *token == Token::HashHash && i + 1 < definition.replacement.len() {
            let Some(left) = output.pop() else {
                i += 1;
                continue;
            };
            let right_tokens = replacement_tokens(
                &definition.replacement[i + 1],
                parameters,
                arguments,
                &expanded_arguments,
                false,
            );
            if let Some(right) = right_tokens.first() {
                let pasted = lex(&format!("{}{}", String::from(&left), String::from(right)));
                if pasted.len() == 1 {
                    output.push(pasted[0].clone());
                    output.extend(right_tokens.into_iter().skip(1));
                } else {
                    output.push(left);
                    output.extend(right_tokens);
                }
            } else {
                output.push(left);
            }
            i += 2;
            continue;
        }
        output.extend(replacement_tokens(
            token,
            parameters,
            arguments,
            &expanded_arguments,
            i + 1 >= definition.replacement.len()
                || definition.replacement[i + 1] != Token::HashHash,
        ));
        i += 1;
    }
    output
}

fn macro_argument<'a>(
    name: &str,
    parameters: &[String],
    arguments: &'a [Vec<Token>],
) -> Option<&'a [Token]> {
    if name == "__VA_ARGS__" {
        return None;
    }
    parameters
        .iter()
        .position(|parameter| parameter == name)
        .and_then(|index| arguments.get(index).map(Vec::as_slice))
}

fn replacement_tokens(
    token: &Token,
    parameters: &[String],
    arguments: &[Vec<Token>],
    expanded_arguments: &[Vec<Token>],
    prescan: bool,
) -> Vec<Token> {
    let Token::Ident(name) = token else {
        return vec![token.clone()];
    };
    if name == "__VA_ARGS__" {
        return variadic_tokens(
            if prescan {
                expanded_arguments
            } else {
                arguments
            },
            parameters.len(),
        );
    }
    parameters
        .iter()
        .position(|parameter| parameter == name)
        .and_then(|index| {
            if prescan {
                expanded_arguments.get(index).cloned()
            } else {
                arguments.get(index).cloned()
            }
        })
        .unwrap_or_else(|| vec![token.clone()])
}

fn variadic_tokens(arguments: &[Vec<Token>], fixed: usize) -> Vec<Token> {
    arguments
        .iter()
        .skip(fixed)
        .enumerate()
        .flat_map(|(index, argument)| {
            let separator = (index != 0).then_some(Token::Comma);
            separator.into_iter().chain(argument.iter().cloned())
        })
        .collect()
}

fn tokens_source(tokens: &[Token]) -> String {
    tokens
        .iter()
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
