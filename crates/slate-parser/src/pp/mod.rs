mod condition;
mod error;
mod expand;
mod include;

use crate::ast::{Condition, Conditional, FileId, HeaderKind, Loc, Provenance, Span};
use crate::const_expr;
use crate::files::{Files, SearchPaths};
use crate::lexer::{Lexer, Token, TokenSpanExt};
use std::collections::{HashMap, HashSet};
use std::path::{Path, PathBuf};

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

#[derive(Clone)]
struct Comment {
    text: String,
    loc: Loc,
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
    pub provenance: Provenance,
    pub order: usize,
}

use condition::{conjunction, is_statically_false, replace_subterm, simplify_condition};
pub use error::PPError;
use error::{PPErrorKind, PPFailure};
use include::{parse_include_directive, read_source};

fn normalize_directive(raw: &str) -> String {
    let trimmed = raw.trim();
    match trimmed.strip_prefix('#') {
        Some(rest) => format!("#{}", rest.trim_start()),
        None => trimmed.to_string(),
    }
}

pub struct Preprocessor<'a> {
    pub files: Files,
    pub macros: std::collections::HashMap<String, Conditional<MacroDef>>,
    pub main_file: Option<FileId>,
    search: &'a SearchPaths,
    open_stack: Vec<PathBuf>,
    comments: HashMap<(FileId, usize), Vec<Comment>>,
    line_offsets: HashMap<(FileId, usize), usize>,
    sources: HashMap<FileId, String>,
    macro_order: usize,
}

const CLANG_X86_64_LINUX_GNU_PREDEFINES: &str =
    include_str!("../predefines/clang_x86_64_linux_gnu.h");

impl<'a> Preprocessor<'a> {
    pub fn new(search: &'a SearchPaths) -> Self {
        let mut pp = Preprocessor {
            files: Files::new(),
            macros: std::collections::HashMap::new(),
            main_file: None,
            search,
            open_stack: Vec::new(),
            comments: HashMap::new(),
            line_offsets: HashMap::new(),
            sources: HashMap::new(),
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
        self.collect_comments(src, file);
        self.collect_line_offsets(src, file);
        let uncommented = strip_comments(src);
        let spliced = splice_continuations(&uncommented);
        let lines: Vec<&str> = spliced.lines().collect();
        let mut pos = 0;
        let nodes = self.parse_block(&lines, &mut pos, file, &active)?;
        if pos < lines.len() {
            return Err(PPFailure::at(
                self.line_loc(&lines, file, pos),
                PPErrorKind::UnexpectedConditional,
            ));
        }
        Ok(nodes)
    }

    fn collect_comments(&mut self, src: &str, file: FileId) {
        for token in Lexer::new(file, src).tokenize() {
            let Token::Comment(text) = token.value else {
                continue;
            };
            let line = src[..token.spelling.offset]
                .bytes()
                .filter(|byte| *byte == b'\n')
                .count();
            self.comments
                .entry((file, line))
                .or_default()
                .push(Comment {
                    text,
                    loc: token.spelling,
                });
        }
    }

    fn collect_line_offsets(&mut self, src: &str, file: FileId) {
        let mut offset = 0;
        for (line, text) in src.split_inclusive('\n').enumerate() {
            self.line_offsets.insert((file, line), offset);
            offset += text.len();
        }
    }

    fn parse_block(
        &mut self,
        lines: &[&str],
        pos: &mut usize,
        file: FileId,
        active: &Condition,
    ) -> Result<Vec<PPNode>, PPFailure> {
        let mut nodes = Vec::new();
        while *pos < lines.len() {
            let raw = lines[*pos];
            let trimmed = normalize_directive(raw);
            let trimmed = trimmed.as_str();
            let provenance = Provenance {
                file,
                kind: self.files.kind(file),
                line: *pos,
            };
            if let Some(comments) = self.comments.get(&(file, *pos)) {
                nodes.extend(comments.iter().cloned().map(|comment| {
                    Span::new(
                        PPNodeKind::Comment {
                            text: comment.text,
                            provenance,
                        },
                        comment.loc,
                        comment.loc,
                    )
                }));
            }
            if raw.trim_start().starts_with("//") {
                *pos += 1;
                continue;
            }

            let directive_loc = self.line_loc(lines, file, *pos);
            if let Some(condition) = self.parse_opening_condition(trimmed, directive_loc)? {
                *pos += 1;
                let conditional =
                    self.parse_conditional(lines, pos, directive_loc, condition, active)?;
                let end = self.line_loc(lines, file, pos.saturating_sub(1));
                let loc = directive_loc.through(end);
                nodes.push(Span::new(PPNodeKind::Conditional(conditional), loc, loc));
            } else if let Some(include) = parse_include_directive(trimmed) {
                *pos += 1;
                nodes.extend(self.resolve_and_parse_include(&include, directive_loc, active)?);
            } else if let Some(rest) = trimmed.strip_prefix("#define") {
                self.record_define(rest.trim_start(), raw, directive_loc, *pos, active)?;
                *pos += 1;
            } else if trimmed.starts_with("#undef") || trimmed.starts_with("#error") {
                *pos += 1;
            } else if trimmed == "#else" || trimmed.starts_with("#elif") || trimmed == "#endif" {
                break;
            } else if trimmed.starts_with('#') {
                return Err(PPFailure::at(
                    directive_loc,
                    PPErrorKind::UnsupportedDirective,
                ));
            } else if trimmed.is_empty() {
                *pos += 1;
            } else {
                let offset = self.line_offsets.get(&(file, *pos)).copied().unwrap_or(0)
                    + raw.find(trimmed).unwrap_or(0);
                let source_tokens = Lexer::with_offset(file, trimmed, offset).tokenize();
                if let Some(conditions) = self.divergent_macro_conditions(&source_tokens, active) {
                    let loc = source_tokens_loc(&source_tokens);
                    nodes.push(Span::new(
                        PPNodeKind::Conditional(PPConditional {
                            branches: conditions
                                .into_iter()
                                .map(|condition| {
                                    let expanded =
                                        Self::strip_pragma_operator(&self.expand_macros(
                                            &source_tokens,
                                            &mut HashSet::new(),
                                            &condition,
                                        ));
                                    (
                                        condition,
                                        vec![Span::new(
                                            PPNodeKind::Code {
                                                text: tokens_source(expanded.values()),
                                                tokens: expanded,
                                                provenance,
                                            },
                                            loc,
                                            loc,
                                        )],
                                    )
                                })
                                .collect(),
                        }),
                        loc,
                        loc,
                    ));
                } else {
                    let expanded = Self::strip_pragma_operator(&self.expand_macros(
                        &source_tokens,
                        &mut HashSet::new(),
                        active,
                    ));
                    let loc = source_tokens_loc(&source_tokens);
                    nodes.push(Span::new(
                        PPNodeKind::Code {
                            text: tokens_source(expanded.values()),
                            tokens: expanded,
                            provenance,
                        },
                        loc,
                        loc,
                    ));
                }
                *pos += 1;
            }
        }

        Ok(nodes)
    }

    fn line_loc(&self, lines: &[&str], file: FileId, line: usize) -> Loc {
        Loc::new(
            file,
            self.line_offsets.get(&(file, line)).copied().unwrap_or(0),
            lines.get(line).map_or(0, |text| text.len()),
        )
    }

    fn parse_opening_condition(
        &self,
        trimmed: &str,
        loc: Loc,
    ) -> Result<Option<Condition>, PPFailure> {
        if let Some(expression) = trimmed.strip_prefix("#if ") {
            let value = const_expr::Parser::evaluate_with_defined(
                &lex_spanned(expression.trim()),
                &|name| self.macros.contains_key(name),
            )
            .map_err(|error| {
                PPFailure::at(
                    loc,
                    PPErrorKind::InvalidExpression {
                        directive: "#if",
                        message: error.to_string(),
                    },
                )
            })?;
            return Ok(Some(Condition::Constant(value)));
        }
        for (directive, negate) in [("#ifdef", false), ("#ifndef", true)] {
            if let Some(rest) = trimmed.strip_prefix(directive) {
                let name = rest.trim();
                if name.is_empty() || name.split_whitespace().count() != 1 {
                    return Err(PPFailure::at(
                        loc,
                        PPErrorKind::ExpectedMacroName(directive),
                    ));
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
        opening: Loc,
        first_condition: Condition,
        active: &Condition,
    ) -> Result<PPConditional, PPFailure> {
        let file = opening.file;
        let mut branches = Vec::new();
        let mut excluded = first_condition.clone();
        let first_active = conjunction(active, &first_condition);
        let order_before = self.macro_order;
        let first_body = self.parse_body_or_skip(lines, pos, file, &first_active)?;
        self.concretize_guard(&first_condition, order_before);
        branches.push((first_condition, first_body));
        let mut saw_else = false;

        loop {
            let Some(directive) = lines.get(*pos).map(|line| normalize_directive(line)) else {
                return Err(PPFailure::at(opening, PPErrorKind::UnterminatedConditional));
            };
            let directive_loc = self.line_loc(lines, file, *pos);
            if directive == "#endif" {
                *pos += 1;
                return Ok(PPConditional { branches });
            }
            if directive == "#else" {
                if saw_else {
                    return Err(PPFailure::at(directive_loc, PPErrorKind::MultipleElse));
                }
                saw_else = true;
                *pos += 1;
                let else_condition = Condition::Not(Box::new(excluded.clone()));
                let else_active = conjunction(active, &else_condition);
                let order_before = self.macro_order;
                let else_body = self.parse_body_or_skip(lines, pos, file, &else_active)?;
                self.concretize_guard(&else_condition, order_before);
                branches.push((else_condition, else_body));
                continue;
            }
            if let Some(expression) = directive.strip_prefix("#elif ") {
                if saw_else {
                    return Err(PPFailure::at(directive_loc, PPErrorKind::ElifAfterElse));
                }
                let value = const_expr::Parser::evaluate_with_defined(
                    &lex_spanned(expression.trim()),
                    &|name| self.macros.contains_key(name),
                )
                .map_err(|error| {
                    PPFailure::at(
                        directive_loc,
                        PPErrorKind::InvalidExpression {
                            directive: "#elif",
                            message: error.to_string(),
                        },
                    )
                })?;
                let condition = Condition::Constant(value);
                let branch_condition = Condition::And(
                    Box::new(Condition::Not(Box::new(excluded.clone()))),
                    Box::new(condition),
                );
                excluded = Condition::Or(Box::new(excluded), Box::new(branch_condition.clone()));
                *pos += 1;
                let branch_active = conjunction(active, &branch_condition);
                let order_before = self.macro_order;
                let body = self.parse_body_or_skip(lines, pos, file, &branch_active)?;
                self.concretize_guard(&branch_condition, order_before);
                branches.push((branch_condition, body));
                continue;
            }
            return Err(PPFailure::at(
                directive_loc,
                PPErrorKind::ExpectedConditionalDirective,
            ));
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

    fn record_define(
        &mut self,
        rest: &str,
        raw: &str,
        directive: Loc,
        line: usize,
        condition: &Condition,
    ) -> Result<(), PPFailure> {
        let file = directive.file;
        let name_end = rest
            .find(|character: char| !character.is_ascii_alphanumeric() && character != '_')
            .unwrap_or(rest.len());
        if name_end == 0 {
            return Err(PPFailure::at(
                directive,
                PPErrorKind::ExpectedMacroName("#define"),
            ));
        }
        let name = rest[..name_end].to_string();
        let after_name = &rest[name_end..];
        let (parameters, variadic, replacement_text) = if after_name.starts_with('(') {
            let Some(close) = after_name.find(')') else {
                return Err(PPFailure::at(
                    directive,
                    PPErrorKind::ExpectedParametersClose,
                ));
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
        let replacement_text = replacement_text.trim();
        let replacement_offset = self.line_offsets.get(&(file, line)).copied().unwrap_or(0)
            + raw.find(replacement_text).unwrap_or(raw.len());
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
                    replacement: Lexer::with_offset(file, replacement_text, replacement_offset)
                        .tokenize(),
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

fn source_tokens_loc(tokens: &[Span<Token>]) -> Loc {
    let (Some(first), Some(last)) = (tokens.first(), tokens.last()) else {
        return Loc::new(FileId(0), 0, 0);
    };
    first.spelling.through(last.spelling)
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
