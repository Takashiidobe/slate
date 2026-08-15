// This file implements the C preprocessor.
//
// The preprocessor takes a list of tokens as an input and returns a
// new list of tokens as an output.
//
// The preprocessing language is designed in such a way that that's
// guaranteed to stop even if there is a recursive macro.
// Informally speaking, a macro is applied only once for each token.
// That is, if a macro token T appears in a result of direct or
// indirect macro expansion of T, T won't be expanded any further.
// For example, if T is defined as U, and U is defined as T, then
// token T is expanded to U and then to T and the macro expansion
// stops at that point.
//
// To achieve the above behavior, we attach for each token a set of
// macro names from which the token is expanded. The set is called
// "hideset". Hideset is initially empty, and every time we expand a
// macro, the macro name is added to the resulting tokens' hidesets.
//
// The above macro expansion algorithm is explained in this document
// written by Dave Prossor, which is used as a basis for the
// standard's wording:
// https://github.com/rui314/chibicc/wiki/cpp.algo.pdf

use crate::parse::ast::Type;
use crate::parse::error::{CompileError, CompileResult, SourceLocation};
use crate::parse::lexer::{
    HideSet, Keyword, Punct, Token, TokenKind, convert_pp_tokens, get_base_file, get_input_file,
    tokenize, tokenize_builtin, tokenize_file, tokenize_string_literal,
};
use crate::parse::parser::const_expr;
use chrono::{Datelike, Local, Timelike};
use std::collections::{HashMap, HashSet};
use std::fs;
use std::path::{Path, PathBuf};
use std::sync::{Mutex, OnceLock};

#[derive(Clone, Debug)]
struct MacroParam {
    name: String,
}

#[derive(Clone, Debug)]
struct MacroArg {
    name: String,
    is_va_args: bool,
    tokens: Vec<Token>,
}

type MacroHandler = fn(&Token) -> CompileResult<Token>;

#[derive(Clone, Debug, Default)]
struct Macro {
    name: String,
    is_objlike: bool, // Object-like or function-like
    params: Vec<MacroParam>,
    va_args_name: Option<String>,
    body: Vec<Token>,
    handler: Option<MacroHandler>,
}

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
enum CondCtx {
    Then,
    Elif,
    Else,
}

#[derive(Clone, Debug)]
struct CondIncl {
    ctx: CondCtx,
    tok: Token,
    included: bool,
}

struct Preprocessor {
    tokens: Vec<Token>,
    macros: HashMap<String, Macro>,
    pos: usize, // Current position in token stream
    cmdline_defines: Vec<(String, String)>,
    cmdline_undefs: Vec<String>,
}

static INCLUDE_PATHS: OnceLock<Mutex<Vec<PathBuf>>> = OnceLock::new();
static INCLUDE_CACHE: OnceLock<Mutex<HashMap<String, PathBuf>>> = OnceLock::new();
static INCLUDE_GUARDS: OnceLock<Mutex<HashMap<PathBuf, String>>> = OnceLock::new();
static PRAGMA_ONCE: OnceLock<Mutex<HashSet<PathBuf>>> = OnceLock::new();
static INCLUDE_NEXT_IDX: OnceLock<Mutex<usize>> = OnceLock::new();

fn include_paths_storage() -> &'static Mutex<Vec<PathBuf>> {
    INCLUDE_PATHS.get_or_init(|| Mutex::new(Vec::new()))
}

fn include_cache() -> &'static Mutex<HashMap<String, PathBuf>> {
    INCLUDE_CACHE.get_or_init(|| Mutex::new(HashMap::new()))
}

fn include_guard_cache() -> &'static Mutex<HashMap<PathBuf, String>> {
    INCLUDE_GUARDS.get_or_init(|| Mutex::new(HashMap::new()))
}

fn pragma_once_cache() -> &'static Mutex<HashSet<PathBuf>> {
    PRAGMA_ONCE.get_or_init(|| Mutex::new(HashSet::new()))
}

fn include_next_idx() -> &'static Mutex<usize> {
    INCLUDE_NEXT_IDX.get_or_init(|| Mutex::new(0))
}

fn normalize_path(path: &Path) -> PathBuf {
    path.canonicalize().unwrap_or_else(|_| path.to_path_buf())
}

pub fn set_include_paths(paths: Vec<PathBuf>) {
    *include_paths_storage()
        .lock()
        .expect("include paths lock poisoned") = paths;
}

fn get_include_paths() -> Vec<PathBuf> {
    include_paths_storage()
        .lock()
        .map(|paths| paths.clone())
        .unwrap_or_default()
}

impl Preprocessor {
    fn new(
        tokens: Vec<Token>,
        cmdline_defines: Vec<(String, String)>,
        cmdline_undefs: Vec<String>,
    ) -> Self {
        Self {
            tokens,
            macros: HashMap::new(),
            pos: 0,
            cmdline_defines,
            cmdline_undefs,
        }
    }

    fn peek(&self, offset: usize) -> &Token {
        &self.tokens[self.pos + offset]
    }

    fn cur_tok(&self) -> &Token {
        &self.tokens[self.pos]
    }

    fn error<T>(&self, message: impl Into<String>, location: SourceLocation) -> CompileResult<T> {
        Err(CompileError::at(message, location))
    }

    fn find_macro_name(&self, name: &str) -> Option<&Macro> {
        self.macros.get(name)
    }

    fn find_macro(&self, tok: &Token) -> Option<&Macro> {
        let name = macro_name(tok)?;
        self.find_macro_name(&name)
    }

    fn read_macro_params(&mut self, va_args_name: &mut Option<String>) -> Vec<MacroParam> {
        let mut params = Vec::new();

        while self.pos < self.tokens.len()
            && !matches!(self.cur_tok().kind, TokenKind::Punct(Punct::RParen))
        {
            if !params.is_empty() {
                // Expect comma between parameters
                if !matches!(self.cur_tok().kind, TokenKind::Punct(Punct::Comma)) {
                    // Error: expected comma
                    break;
                }
                self.pos += 1;
            }

            if matches!(self.cur_tok().kind, TokenKind::Punct(Punct::Ellipsis)) {
                *va_args_name = Some("__VA_ARGS__".to_string());
                self.pos += 1;
                if self.pos < self.tokens.len()
                    && matches!(self.cur_tok().kind, TokenKind::Punct(Punct::RParen))
                {
                    self.pos += 1;
                }
                return params;
            }

            // Expect identifier
            let TokenKind::Ident(name) = &self.cur_tok().kind else {
                // Error: expected identifier
                break;
            };
            if self.pos + 1 < self.tokens.len()
                && matches!(self.peek(1).kind, TokenKind::Punct(Punct::Ellipsis))
            {
                *va_args_name = Some(name.clone());
                self.pos += 2;
                if self.pos < self.tokens.len()
                    && matches!(self.cur_tok().kind, TokenKind::Punct(Punct::RParen))
                {
                    self.pos += 1;
                }
                return params;
            }
            params.push(MacroParam { name: name.clone() });
            self.pos += 1;
        }

        // Skip the closing ')'
        if self.pos < self.tokens.len()
            && matches!(self.cur_tok().kind, TokenKind::Punct(Punct::RParen))
        {
            self.pos += 1;
        }

        params
    }

    fn read_macro_definition(&mut self) {
        if self.pos >= self.tokens.len() {
            return;
        };
        let tok = self.cur_tok();
        let Some(macro_name) = macro_name(tok) else {
            // Error: macro name must be an identifier (would need error handling)
            return;
        };
        self.pos += 1;

        // Check if this is a function-like macro (no space before '(')
        if self.pos < self.tokens.len()
            && !self.cur_tok().has_space
            && matches!(self.cur_tok().kind, TokenKind::Punct(Punct::LParen))
        {
            // Function-like macro - read parameters
            self.pos += 1; // skip '('
            let mut va_args_name = None;
            let params = self.read_macro_params(&mut va_args_name);
            let body = self.copy_line();
            self.macros.insert(
                macro_name.clone(),
                Macro {
                    name: macro_name,
                    is_objlike: false,
                    params,
                    va_args_name,
                    body,
                    ..Macro::default()
                },
            );
            return;
        }

        // Object-like macro
        let body = self.copy_line();
        self.macros.insert(
            macro_name.clone(),
            Macro {
                name: macro_name,
                is_objlike: true,
                body,
                ..Macro::default()
            },
        );
    }

    fn read_macro_arg_one(&mut self, read_rest: bool) -> Vec<Token> {
        let mut arg_tokens = Vec::new();
        let mut level = 0;

        while self.pos < self.tokens.len() {
            if level == 0 && matches!(self.cur_tok().kind, TokenKind::Punct(Punct::RParen)) {
                break;
            }
            if level == 0
                && !read_rest
                && matches!(self.cur_tok().kind, TokenKind::Punct(Punct::Comma))
            {
                break;
            }
            if matches!(self.cur_tok().kind, TokenKind::Eof) {
                // Error: premature end of input
                break;
            }
            if matches!(self.cur_tok().kind, TokenKind::Punct(Punct::LParen)) {
                level += 1;
            } else if matches!(self.cur_tok().kind, TokenKind::Punct(Punct::RParen)) {
                level -= 1;
            }
            arg_tokens.push(self.cur_tok().clone());
            self.pos += 1;
        }

        let eof_source = self
            .tokens
            .get(self.pos)
            .or_else(|| self.tokens.last())
            .expect("tokens");
        arg_tokens.push(new_eof(eof_source));
        arg_tokens
    }

    fn read_macro_args(
        &mut self,
        params: &[MacroParam],
        va_args_name: Option<String>,
    ) -> Vec<MacroArg> {
        // self.pos should be pointing at the macro name, next should be '('
        self.pos += 2; // skip macro name and '('
        let mut args = Vec::new();

        for param in params {
            if !args.is_empty() {
                // Expect comma between arguments
                if self.pos < self.tokens.len()
                    && matches!(self.cur_tok().kind, TokenKind::Punct(Punct::Comma))
                {
                    self.pos += 1;
                }
            }

            let arg_tokens = self.read_macro_arg_one(false);
            args.push(MacroArg {
                name: param.name.clone(),
                is_va_args: false,
                tokens: arg_tokens,
            });
        }

        if let Some(va_args_name) = va_args_name {
            let arg_tokens = if self.pos < self.tokens.len()
                && matches!(self.cur_tok().kind, TokenKind::Punct(Punct::RParen))
            {
                let eof_source = self
                    .tokens
                    .get(self.pos)
                    .or_else(|| self.tokens.last())
                    .expect("tokens");
                vec![new_eof(eof_source)]
            } else {
                if !params.is_empty()
                    && self.pos < self.tokens.len()
                    && matches!(self.cur_tok().kind, TokenKind::Punct(Punct::Comma))
                {
                    self.pos += 1;
                }
                self.read_macro_arg_one(true)
            };
            args.push(MacroArg {
                name: va_args_name,
                is_va_args: true,
                tokens: arg_tokens,
            });
        } else if self.pos < self.tokens.len()
            && !matches!(self.cur_tok().kind, TokenKind::Punct(Punct::RParen))
        {
            // Too many arguments
        }

        // Skip the closing ')'
        if self.pos < self.tokens.len()
            && matches!(self.cur_tok().kind, TokenKind::Punct(Punct::RParen))
        {
            self.pos += 1;
        }

        args
    }

    fn expand_macro(&mut self) -> CompileResult<Option<Vec<Token>>> {
        let tok = self.cur_tok().clone();

        // Check if this macro is in the token's hideset
        let Some(macro_name) = macro_name(&tok) else {
            return Ok(None);
        };
        if tok.hideset.contains(&macro_name) {
            return Ok(None);
        }

        let (is_objlike, params, body, macro_name, handler, va_args_name) = {
            let Some(m) = self.find_macro_name(&macro_name) else {
                return Ok(None);
            };
            (
                m.is_objlike,
                m.params.clone(),
                m.body.clone(),
                m.name.clone(),
                m.handler,
                m.va_args_name.clone(),
            )
        };

        if let Some(handler) = handler {
            let mut expanded = handler(&tok)?;
            expanded.at_bol = tok.at_bol;
            expanded.has_space = tok.has_space;
            let mut result = Vec::new();
            result.extend_from_slice(&self.tokens[..self.pos]);
            result.push(expanded);
            result.extend_from_slice(&self.tokens[self.pos + 1..]);
            return Ok(Some(result));
        }

        // Object-like macro application
        if is_objlike {
            let mut hs = HideSet::default();
            hs.add(macro_name);
            let hs = tok.hideset.union(&hs);
            let mut body = body;
            if body
                .last()
                .is_some_and(|tok| matches!(tok.kind, TokenKind::Eof))
            {
                body.pop();
            }
            hs.add_tokens(&mut body);
            apply_origin(&mut body, origin_location(&tok));

            let mut result = Vec::new();
            result.extend_from_slice(&self.tokens[..self.pos]);
            let start_idx = result.len();
            result.extend(body);
            result.extend_from_slice(&self.tokens[self.pos + 1..]);
            if let Some(first) = result.get_mut(start_idx) {
                first.at_bol = tok.at_bol;
                first.has_space = tok.has_space;
            }
            return Ok(Some(result));
        }

        // If a funclike macro token is not followed by an argument list,
        // treat it as a normal identifier.
        if self.pos + 1 >= self.tokens.len()
            || !matches!(self.peek(1).kind, TokenKind::Punct(Punct::LParen))
        {
            return Ok(None);
        }

        // Function-like macro application - read arguments and substitute
        let saved_pos = self.pos;
        let args = self.read_macro_args(&params, va_args_name);
        let rest_pos = self.pos;
        let rparen_pos = rest_pos.saturating_sub(1);
        let mut hs = tok.hideset.intersection(&self.tokens[rparen_pos].hideset);
        hs.add(macro_name);
        self.pos = saved_pos;
        let mut result = Vec::new();
        result.extend_from_slice(&self.tokens[..saved_pos]);
        let start_idx = result.len();
        let mut expanded = self.subst(&body, &args)?;
        hs.add_tokens(&mut expanded);
        apply_origin(&mut expanded, origin_location(&tok));
        result.extend(expanded);
        result.extend_from_slice(&self.tokens[rest_pos..]);
        if let Some(first) = result.get_mut(start_idx) {
            first.at_bol = tok.at_bol;
            first.has_space = tok.has_space;
        }
        Ok(Some(result))
    }

    fn expand_macros_stream(&mut self) -> CompileResult<Vec<Token>> {
        let mut out = Vec::new();
        self.pos = 0;

        while self.pos < self.tokens.len() {
            if matches!(self.cur_tok().kind, TokenKind::Eof) {
                out.push(self.cur_tok().clone());
                break;
            }

            // Try to expand macros
            if let Some(expanded) = self.expand_macro()? {
                self.tokens = expanded;
                continue;
            }

            out.push(self.cur_tok().clone());
            self.pos += 1;
        }

        Ok(out)
    }

    fn expand_macros_only(&self, tokens: Vec<Token>) -> CompileResult<Vec<Token>> {
        let mut expander = Preprocessor::new(tokens, Vec::new(), Vec::new());
        expander.macros = self.macros.clone();
        expander.expand_macros_stream()
    }

    fn skip_line(&mut self) {
        if self.pos >= self.tokens.len() {
            return;
        }
        if self.cur_tok().at_bol {
            return;
        }
        warn_tok(self.cur_tok(), "extra token");
        while self.pos < self.tokens.len() && !self.cur_tok().at_bol {
            self.pos += 1;
        }
    }

    fn skip_cond_incl2(&mut self) {
        while self.pos < self.tokens.len() {
            let tok = self.cur_tok();
            if matches!(tok.kind, TokenKind::Eof) {
                return;
            }
            if is_hash(tok) && self.pos + 1 < self.tokens.len() {
                let kind = &self.peek(1).kind;
                if is_if_directive(kind)
                    || matches!(
                        kind,
                        TokenKind::Ident(name)
                            if name == "ifdef" || name == "ifndef"
                    )
                {
                    self.pos += 2;
                    self.skip_cond_incl2();
                    continue;
                }
                if matches!(kind, TokenKind::Ident(name) if name == "endif") {
                    self.pos += 2;
                    return;
                }
            }
            self.pos += 1;
        }
    }

    fn skip_cond_incl(&mut self) {
        while self.pos < self.tokens.len() {
            let tok = self.cur_tok();
            if matches!(tok.kind, TokenKind::Eof) {
                return;
            }
            if is_hash(tok) && self.pos + 1 < self.tokens.len() {
                let kind = &self.peek(1).kind;
                if is_if_directive(kind)
                    || matches!(
                        kind,
                        TokenKind::Ident(name)
                            if name == "ifdef" || name == "ifndef"
                    )
                {
                    self.pos += 2;
                    self.skip_cond_incl2();
                    continue;
                }
                if matches!(
                    kind,
                    TokenKind::Ident(name)
                        if name == "elif"
                            || name == "elifdef"
                            || name == "elifndef"
                            || name == "endif"
                ) || is_else_directive(kind)
                {
                    break;
                }
            }
            self.pos += 1;
        }
    }

    fn copy_line(&mut self) -> Vec<Token> {
        let mut out = Vec::new();
        while self.pos < self.tokens.len() && !self.cur_tok().at_bol {
            out.push(self.cur_tok().clone());
            self.pos += 1;
        }
        let eof_source = self
            .tokens
            .get(self.pos)
            .or_else(|| self.tokens.last())
            .expect("tokens");
        out.push(new_eof(eof_source));
        out
    }

    fn read_line_marker(&mut self, start: &Token) -> CompileResult<()> {
        let line_tokens = self.copy_line();
        let mut tokens = self.expand_macros_only(line_tokens)?;
        convert_pp_tokens(&mut tokens)?;

        let first = tokens
            .first()
            .ok_or_else(|| CompileError::new("invalid line marker"))?;
        let (value, ty) = match &first.kind {
            TokenKind::Num { value, ty, .. } => (*value, ty),
            _ => {
                return self.error("invalid line marker", first.location);
            }
        };
        if *ty != Type::Int {
            return self.error("invalid line marker", first.location);
        }

        let line_delta = value - start.location.line as i64;
        let mut display_name = None;

        if let Some(tok) = tokens.get(1) {
            if matches!(tok.kind, TokenKind::Eof) {
                crate::parse::lexer::set_line_marker(
                    start.location.file_no,
                    line_delta as i32,
                    display_name,
                );
                return Ok(());
            }

            if matches!(tok.kind, TokenKind::Str { .. }) {
                display_name = Some(raw_string_literal(tok));
            } else {
                return self.error("filename expected", tok.location);
            }
        }

        crate::parse::lexer::set_line_marker(
            start.location.file_no,
            line_delta as i32,
            display_name,
        );
        Ok(())
    }

    fn read_const_expr(&mut self) -> CompileResult<Vec<Token>> {
        let tokens = self.copy_line();
        let mut out = Vec::new();
        let mut i = 0;

        while i < tokens.len() {
            let tok = &tokens[i];
            if matches!(tok.kind, TokenKind::Eof) {
                out.push(tok.clone());
                break;
            }

            if let TokenKind::Ident(name) = &tok.kind
                && name == "defined"
            {
                let start = tok.clone();
                let mut idx = i + 1;
                let mut has_paren = false;

                if idx < tokens.len() && matches!(tokens[idx].kind, TokenKind::Punct(Punct::LParen))
                {
                    has_paren = true;
                    idx += 1;
                }

                let name_tok = tokens.get(idx).ok_or_else(|| {
                    CompileError::at("macro name must be an identifier", start.location)
                })?;
                let TokenKind::Ident(macro_name) = &name_tok.kind else {
                    return self.error("macro name must be an identifier", start.location);
                };
                let defined = self.find_macro_name(macro_name).is_some();
                idx += 1;

                if has_paren {
                    if idx >= tokens.len()
                        || !matches!(tokens[idx].kind, TokenKind::Punct(Punct::RParen))
                    {
                        return self.error("expected ')'", start.location);
                    }
                    idx += 1;
                }

                out.push(new_num_token(if defined { 1 } else { 0 }, &start));
                i = idx;
                continue;
            }

            out.push(tok.clone());
            i += 1;
        }

        Ok(out)
    }

    fn eval_const_expr(&mut self) -> CompileResult<i64> {
        let start = self
            .tokens
            .get(self.pos.saturating_sub(1))
            .or_else(|| self.tokens.get(self.pos))
            .cloned()
            .ok_or_else(|| CompileError::new("no expression"))?;
        let expr_tokens = self.read_const_expr()?;

        // Expand macros in the expression
        let mut expr_tokens = self.expand_macros_only(expr_tokens)?;
        expr_tokens = self.replace_macro_with_zero(expr_tokens, "__has_include")?;
        expr_tokens = self.replace_macro_with_zero(expr_tokens, "__has_include_next")?;
        expr_tokens = self.replace_macro_with_zero(expr_tokens, "__has_feature")?;
        expr_tokens = self.replace_macro_with_zero(expr_tokens, "__building_module")?;

        // Convert pp-numbers to regular numbers
        convert_pp_tokens(&mut expr_tokens)?;

        for tok in &mut expr_tokens {
            if matches!(tok.kind, TokenKind::Ident(_)) {
                *tok = new_num_token(0, tok);
            }
        }

        if matches!(
            expr_tokens.first().map(|tok| &tok.kind),
            Some(TokenKind::Eof)
        ) {
            self.error("no expression", start.location)?;
        }

        let value = const_expr(&expr_tokens)?;
        Ok(value)
    }

    fn replace_macro_with_zero(
        &mut self,
        tokens: Vec<Token>,
        macro_name: &str,
    ) -> CompileResult<Vec<Token>> {
        let mut out = Vec::with_capacity(tokens.len());
        let mut i = 0;

        while i < tokens.len() {
            let tok = &tokens[i];
            let TokenKind::Ident(name) = &tok.kind else {
                out.push(tok.clone());
                i += 1;
                continue;
            };

            if name != macro_name {
                out.push(tok.clone());
                i += 1;
                continue;
            }

            if i + 1 >= tokens.len()
                || !matches!(tokens[i + 1].kind, TokenKind::Punct(Punct::LParen))
            {
                return self.error("expected '('", tok.location);
            }
            i += 2;

            let mut depth = 1usize;
            while i < tokens.len() && depth > 0 {
                match tokens[i].kind {
                    TokenKind::Punct(Punct::LParen) => depth += 1,
                    TokenKind::Punct(Punct::RParen) => depth -= 1,
                    _ => {}
                }
                i += 1;
            }

            if depth != 0 {
                return self.error("expected ')'", tok.location);
            }

            out.push(new_num_token(0, tok));
        }

        Ok(out)
    }
}

fn find_arg<'a>(args: &'a [MacroArg], tok: &Token) -> Option<&'a MacroArg> {
    let TokenKind::Ident(name) = &tok.kind else {
        return None;
    };
    args.iter().find(|arg| &arg.name == name)
}

fn has_varargs(args: &[MacroArg]) -> bool {
    args.iter().any(|arg| {
        arg.is_va_args
            && arg
                .tokens
                .first()
                .is_some_and(|tok| !matches!(tok.kind, TokenKind::Eof))
    })
}

fn macro_name(tok: &Token) -> Option<String> {
    match &tok.kind {
        TokenKind::Ident(name) => Some(name.clone()),
        TokenKind::Keyword(keyword) => Some(keyword.to_string()),
        _ => None,
    }
}

fn is_if_directive(kind: &TokenKind) -> bool {
    matches!(kind, TokenKind::Ident(name) if name == "if")
        || matches!(kind, TokenKind::Keyword(Keyword::If))
}

fn is_else_directive(kind: &TokenKind) -> bool {
    matches!(kind, TokenKind::Ident(name) if name == "else")
        || matches!(kind, TokenKind::Keyword(Keyword::Else))
}

fn token_text(tok: &Token) -> String {
    if tok.len != 0
        && let Some(file) = get_input_file(tok.location.file_no)
    {
        let bytes = file.contents.as_bytes();
        let start = tok.location.byte;
        let end = start.saturating_add(tok.len);
        if end <= bytes.len() {
            return String::from_utf8_lossy(&bytes[start..end]).into_owned();
        }
    }

    match &tok.kind {
        TokenKind::Keyword(keyword) => keyword.to_string(),
        TokenKind::Ident(name) => name.clone(),
        TokenKind::Punct(punct) => punct.to_string(),
        TokenKind::Num { value, .. } => value.to_string(),
        TokenKind::PPNum => tok.text(),
        TokenKind::Str { bytes, .. } => {
            let inner = String::from_utf8_lossy(bytes)
                .trim_end_matches('\0')
                .replace('\\', "\\\\")
                .replace('"', "\\\"");
            format!("\"{inner}\"")
        }
        TokenKind::Eof => String::new(),
    }
}

fn raw_string_literal(tok: &Token) -> String {
    if tok.len >= 2
        && let Some(file) = get_input_file(tok.location.file_no)
    {
        let bytes = file.contents.as_bytes();
        let start = tok.location.byte;
        let end = start.saturating_add(tok.len);
        if end <= bytes.len()
            && start < end.saturating_sub(1)
            && bytes.get(start) == Some(&b'"')
            && bytes.get(end.saturating_sub(1)) == Some(&b'"')
        {
            return String::from_utf8_lossy(&bytes[start + 1..end - 1]).into_owned();
        }
    }

    if let TokenKind::Str { bytes, .. } = &tok.kind {
        let slice = bytes.strip_suffix(&[0]).unwrap_or(bytes.as_slice());
        return String::from_utf8_lossy(slice).into_owned();
    }

    String::new()
}

fn origin_location(tok: &Token) -> SourceLocation {
    tok.origin.unwrap_or(tok.location)
}

pub fn search_include_paths(filename: &str) -> Option<PathBuf> {
    if Path::new(filename).is_absolute() {
        return Some(PathBuf::from(filename));
    }

    if let Ok(cache) = include_cache().lock()
        && let Some(cached) = cache.get(filename)
    {
        return Some(cached.clone());
    }

    let include_paths = get_include_paths();
    for (idx, path) in include_paths.iter().enumerate() {
        let candidate = path.join(filename);
        if candidate.exists() {
            if let Ok(mut cache) = include_cache().lock() {
                cache.insert(filename.to_string(), candidate.clone());
            }
            if let Ok(mut idx_cache) = include_next_idx().lock() {
                *idx_cache = idx + 1;
            }
            return Some(candidate);
        }
    }

    None
}

fn search_include_next(filename: &str) -> Option<PathBuf> {
    let include_paths = get_include_paths();
    let start_idx = include_next_idx().lock().map(|idx| *idx).unwrap_or(0);

    for (idx, path) in include_paths.iter().enumerate().skip(start_idx) {
        let candidate = path.join(filename);
        if candidate.exists() {
            if let Ok(mut idx_cache) = include_next_idx().lock() {
                *idx_cache = idx + 1;
            }
            return Some(candidate);
        }
    }

    if let Ok(mut idx_cache) = include_next_idx().lock() {
        *idx_cache = include_paths.len();
    }
    None
}

fn quote_string(input: &str) -> String {
    let mut out = String::with_capacity(input.len() + 2);
    out.push('"');
    for ch in input.chars() {
        if ch == '\\' || ch == '"' {
            out.push('\\');
        }
        out.push(ch);
    }
    out.push('"');
    out
}

fn join_tokens(tokens: &[Token]) -> String {
    let mut parts = Vec::new();
    for (idx, tok) in tokens
        .iter()
        .take_while(|tok| !matches!(tok.kind, TokenKind::Eof))
        .enumerate()
    {
        if idx > 0 && tok.has_space {
            parts.push(" ".to_string());
        }
        parts.push(token_text(tok));
    }
    parts.join("")
}

fn is_hash(tok: &Token) -> bool {
    tok.at_bol && matches!(tok.kind, TokenKind::Punct(Punct::Hash))
}

fn detect_include_guard(tokens: &[Token]) -> Option<String> {
    let mut idx = 0;
    if tokens.len() < 4 {
        return None;
    }
    if !is_hash(&tokens[idx]) {
        return None;
    }
    let TokenKind::Ident(ifndef_name) = &tokens[idx + 1].kind else {
        return None;
    };
    if ifndef_name != "ifndef" {
        return None;
    }
    let TokenKind::Ident(macro_name) = &tokens[idx + 2].kind else {
        return None;
    };
    let macro_name = macro_name.clone();
    idx += 3;

    if idx + 2 >= tokens.len() {
        return None;
    }
    if !is_hash(&tokens[idx]) {
        return None;
    }
    let TokenKind::Ident(define_name) = &tokens[idx + 1].kind else {
        return None;
    };
    if define_name != "define" {
        return None;
    }
    let TokenKind::Ident(define_macro) = &tokens[idx + 2].kind else {
        return None;
    };
    if define_macro != &macro_name {
        return None;
    }

    while idx < tokens.len() {
        let tok = &tokens[idx];
        if !is_hash(tok) {
            idx += 1;
            continue;
        }
        if idx + 2 < tokens.len() {
            let kind = &tokens[idx + 1].kind;
            if is_if_directive(kind)
                || matches!(kind, TokenKind::Ident(name) if name == "ifdef" || name == "ifndef")
            {
                idx = skip_cond_incl_tokens(tokens, idx + 1);
                continue;
            }
            if matches!(kind, TokenKind::Ident(name) if name == "endif")
                && matches!(tokens[idx + 2].kind, TokenKind::Eof)
            {
                return Some(macro_name);
            }
        }
        idx += 1;
    }
    None
}

fn skip_cond_incl_tokens(tokens: &[Token], mut idx: usize) -> usize {
    let mut depth = 0usize;
    while idx < tokens.len() {
        let tok = &tokens[idx];
        if matches!(tok.kind, TokenKind::Eof) {
            return idx;
        }
        if is_hash(tok) && idx + 1 < tokens.len() {
            let kind = &tokens[idx + 1].kind;
            if is_if_directive(kind)
                || matches!(kind, TokenKind::Ident(name) if name == "ifdef" || name == "ifndef")
            {
                depth += 1;
                idx += 2;
                continue;
            }
            if matches!(kind, TokenKind::Ident(name) if name == "endif") {
                if depth == 0 {
                    return idx + 2;
                }
                depth = depth.saturating_sub(1);
                idx += 2;
                continue;
            }
        }
        idx += 1;
    }
    idx
}

fn new_eof(tok: &Token) -> Token {
    let mut eof = tok.clone();
    eof.kind = TokenKind::Eof;
    eof.len = 0;
    eof
}

fn new_num_token(value: i64, tmpl: &Token) -> Token {
    let mut tok = tmpl.clone();
    tok.kind = TokenKind::Num {
        value,
        fval: 0.0,
        ty: Type::Int,
    };
    tok.len = 1;
    tok
}

fn new_punct_token(punct: Punct, tmpl: &Token) -> Token {
    let mut tok = tmpl.clone();
    tok.kind = TokenKind::Punct(punct);
    tok.len = 1;
    tok
}

fn apply_origin(tokens: &mut [Token], origin: SourceLocation) {
    for tok in tokens {
        if matches!(tok.kind, TokenKind::Eof) {
            break;
        }
        tok.origin = Some(origin);
    }
}

fn new_str_token_value(value: &str, tmpl: &Token, location: SourceLocation) -> Token {
    let mut bytes = value.as_bytes().to_vec();
    bytes.push(0);
    let ty = Type::Array {
        base: Box::new(Type::Char),
        len: bytes.len() as i32,
    };
    Token {
        kind: TokenKind::Str { bytes, ty },
        location,
        at_bol: tmpl.at_bol,
        has_space: tmpl.has_space,
        len: tmpl.len,
        hideset: tmpl.hideset.clone(),
        origin: tmpl.origin,
        line_delta: tmpl.line_delta,
    }
}

fn file_macro(tok: &Token) -> CompileResult<Token> {
    let origin = origin_location(tok);
    let filename = get_input_file(origin.file_no)
        .map(|file| file.display_name)
        .unwrap_or_default();
    Ok(new_str_token_value(&filename, tok, origin))
}

fn line_macro(tok: &Token) -> CompileResult<Token> {
    let origin = origin_location(tok);
    let line_delta = get_input_file(origin.file_no)
        .map(|file| file.line_delta)
        .unwrap_or(0);
    let mut out = new_num_token(origin.line as i64 + line_delta as i64, tok);
    out.location = origin;
    Ok(out)
}

/// __COUNTER__ is expanded to serial values starting from 0.
fn counter_macro(tok: &Token) -> CompileResult<Token> {
    static COUNTER: Mutex<i64> = Mutex::new(0);
    let mut counter = COUNTER.lock().unwrap();
    let value = *counter;
    *counter += 1;
    Ok(new_num_token(value, tok))
}

/// __TIMESTAMP__ is expanded to the last modification time of the current file.
fn timestamp_macro(tok: &Token) -> CompileResult<Token> {
    let origin = origin_location(tok);
    let fallback = "??? ??? ?? ??:??:?? ????";
    let Some(file) = get_input_file(origin.file_no) else {
        return Ok(new_str_token_value(fallback, tok, origin));
    };
    let Ok(metadata) = fs::metadata(&file.name) else {
        return Ok(new_str_token_value(fallback, tok, origin));
    };
    let Ok(modified) = metadata.modified() else {
        return Ok(new_str_token_value(fallback, tok, origin));
    };
    let timestamp: chrono::DateTime<Local> = modified.into();
    let formatted = timestamp.format("%a %b %e %H:%M:%S %Y").to_string();
    Ok(new_str_token_value(&formatted, tok, origin))
}

fn base_file_macro(tok: &Token) -> CompileResult<Token> {
    let origin = origin_location(tok);
    let base_file = get_base_file().unwrap_or_default();
    Ok(new_str_token_value(&base_file, tok, origin))
}

fn warn_tok(tok: &Token, message: &str) {
    let header = format!("warning: {message}");
    eprintln!("{header}");

    let Some(file) = get_input_file(tok.location.file_no) else {
        return;
    };

    let display_line = (tok.location.line as i64 + tok.line_delta as i64).max(1) as usize;
    let line_text = line_at_byte(&file.contents, tok.location.byte);
    let width = display_line.to_string().len().max(3);

    eprintln!(
        "  --> {}:{}:{}",
        file.display_name, display_line, tok.location.column
    );
    eprintln!("{:>width$} |", "", width = width);
    if let Some(text) = line_text {
        eprintln!("{:>width$} | {}", display_line, text, width = width);
        let caret_width =
            crate::parse::lexer::display_width(text, tok.location.column.saturating_sub(1));
        let caret_pad = " ".repeat(caret_width);
        eprintln!("{:>width$} | {}^", "", caret_pad, width = width);
    }
}

fn line_at_byte(contents: &str, byte: usize) -> Option<&str> {
    if contents.is_empty() {
        return None;
    }
    let bytes = contents.as_bytes();
    let pos = byte.min(bytes.len());
    let start = bytes[..pos]
        .iter()
        .rposition(|&b| b == b'\n')
        .map(|idx| idx + 1)
        .unwrap_or(0);
    let end = bytes[pos..]
        .iter()
        .position(|&b| b == b'\n')
        .map(|idx| pos + idx)
        .unwrap_or(bytes.len());
    Some(&contents[start..end])
}

fn join_adjacent_string_literals(tokens: &mut Vec<Token>) -> CompileResult<()> {
    #[derive(Clone, Copy, Debug, PartialEq, Eq)]
    enum StringKind {
        None,
        Utf8,
        Utf16,
        Utf32,
        Wide,
    }

    fn get_string_kind(tok: &Token) -> StringKind {
        let text = tok.text();
        if text.starts_with("u8") {
            return StringKind::Utf8;
        }
        match text.as_bytes().first().copied() {
            Some(b'"') => StringKind::None,
            Some(b'u') => StringKind::Utf16,
            Some(b'U') => StringKind::Utf32,
            Some(b'L') => StringKind::Wide,
            _ => StringKind::None,
        }
    }

    fn base_type(tok: &Token) -> Option<Type> {
        match &tok.kind {
            TokenKind::Str {
                ty: Type::Array { base, .. },
                ..
            } => Some((**base).clone()),
            _ => None,
        }
    }

    let mut i = 0;
    while i < tokens.len() {
        if !matches!(tokens[i].kind, TokenKind::Str { .. }) {
            i += 1;
            continue;
        }

        let mut j = i + 1;
        while j < tokens.len() && matches!(tokens[j].kind, TokenKind::Str { .. }) {
            j += 1;
        }

        let mut kind = StringKind::None;
        let mut base = None::<Type>;
        for tok in &tokens[i..j] {
            let k = get_string_kind(tok);
            if kind == StringKind::None && k != StringKind::None {
                kind = k;
                base = base_type(tok);
                continue;
            }
            if k != StringKind::None && k != kind {
                return Err(CompileError::at(
                    "unsupported non-standard concatenation of string literals",
                    tok.location,
                ));
            }
        }

        if let Some(base) = base.clone()
            && base.size() > 1
        {
            for tok in tokens.iter_mut().take(j).skip(i) {
                let needs_convert = base_type(tok).map(|ty| ty.size() == 1).unwrap_or(false);
                if needs_convert {
                    let converted = tokenize_string_literal(&*tok, &base)?;
                    *tok = Token {
                        kind: converted.kind,
                        location: tok.location,
                        at_bol: tok.at_bol,
                        has_space: tok.has_space,
                        len: converted.len,
                        hideset: tok.hideset.clone(),
                        origin: tok.origin,
                        line_delta: tok.line_delta,
                    };
                }
            }
        }

        i = j;
    }

    let mut i = 0;
    while i + 1 < tokens.len() {
        if !matches!(tokens[i].kind, TokenKind::Str { .. })
            || !matches!(tokens[i + 1].kind, TokenKind::Str { .. })
        {
            i += 1;
            continue;
        }

        let mut j = i + 1;
        while j < tokens.len() && matches!(tokens[j].kind, TokenKind::Str { .. }) {
            j += 1;
        }

        let (base, base_size) = base_type(&tokens[i])
            .map(|ty| {
                let size = ty.size() as usize;
                (ty, size)
            })
            .unwrap_or((Type::Char, 1));
        let base_size = base_size.max(1);
        let suffix = vec![0u8; base_size];

        let mut combined = Vec::new();
        for tok in &tokens[i..j] {
            let TokenKind::Str { bytes, .. } = &tok.kind else {
                continue;
            };
            let slice = if bytes.len() >= base_size && bytes[bytes.len() - base_size..] == suffix {
                &bytes[..bytes.len() - base_size]
            } else {
                bytes.as_slice()
            };
            combined.extend_from_slice(slice);
        }
        combined.extend_from_slice(&suffix);
        let ty = Type::Array {
            base: Box::new(base),
            len: (combined.len() / base_size) as i32,
        };
        let location = tokens[i].location;
        let at_bol = tokens[i].at_bol;
        let has_space = tokens[i].has_space;
        let hideset = tokens[i].hideset.clone();
        let origin = tokens[i].origin;
        let line_delta = tokens[i].line_delta;
        tokens[i] = Token {
            kind: TokenKind::Str {
                bytes: combined,
                ty,
            },
            location,
            at_bol,
            has_space,
            len: 0,
            hideset,
            origin,
            line_delta,
        };
        tokens.drain(i + 1..j);
        i += 1;
    }
    Ok(())
}

impl Preprocessor {
    fn preprocess_tokens(&mut self) -> CompileResult<Vec<Token>> {
        let mut out = Vec::with_capacity(self.tokens.len());
        self.pos = 0;
        let mut cond_incl = vec![];

        while self.pos < self.tokens.len() {
            if let Some(file) = get_input_file(self.cur_tok().location.file_no) {
                self.tokens[self.pos].line_delta = file.line_delta;
            }
            // Try to expand macros
            if let Some(expanded) = self.expand_macro()? {
                self.tokens = expanded;
                continue;
            }
            let tok = self.cur_tok();
            if matches!(tok.kind, TokenKind::Eof) {
                out.push(tok.clone());
                break;
            }

            if !is_hash(tok) {
                let mut tok = tok.clone();
                if let Some(file) = get_input_file(tok.location.file_no) {
                    tok.line_delta = file.line_delta;
                }
                out.push(tok);
                self.pos += 1;
                continue;
            }

            let start = tok.clone();
            self.pos += 1;
            if self.pos >= self.tokens.len() {
                break;
            }

            if let TokenKind::Ident(name) = &self.cur_tok().kind
                && name == "include"
            {
                self.pos += 1;
                if self.pos >= self.tokens.len() {
                    let loc = self.tokens[self.pos - 1].location;
                    self.error("expected a filename", loc)?;
                    unreachable!("expected error to return");
                }
                let (filename, is_dquote, filename_tok) = self.read_include_filename()?;
                let filename_path = Path::new(&filename);

                if is_dquote && !filename_path.is_absolute() {
                    let base = get_input_file(start.location.file_no)
                        .map(|file| file.name)
                        .unwrap_or_else(|| Path::new(".").to_path_buf());
                    let dir = base.parent().unwrap_or(Path::new("."));
                    let local_path = dir.join(filename_path);
                    if local_path.exists() {
                        self.include_file(&mut out, &local_path, &filename_tok)?;
                        continue;
                    }
                }

                let include_path =
                    search_include_paths(&filename).unwrap_or_else(|| filename_path.to_path_buf());
                self.include_file(&mut out, &include_path, &filename_tok)?;
                continue;
            }

            if let TokenKind::Ident(name) = &self.cur_tok().kind
                && name == "include_next"
            {
                self.pos += 1;
                if self.pos >= self.tokens.len() {
                    let loc = self.tokens[self.pos - 1].location;
                    self.error("expected a filename", loc)?;
                    unreachable!("expected error to return");
                }
                let (filename, _is_dquote, filename_tok) = self.read_include_filename()?;
                let filename_path = Path::new(&filename);
                let include_path =
                    search_include_next(&filename).unwrap_or_else(|| filename_path.to_path_buf());
                self.include_file(&mut out, &include_path, &filename_tok)?;
                continue;
            }

            if let TokenKind::Ident(name) = &self.cur_tok().kind
                && name == "embed"
            {
                self.pos += 1;
                if self.pos >= self.tokens.len() {
                    let loc = self.tokens[self.pos - 1].location;
                    self.error("expected a filename", loc)?;
                    unreachable!("expected error to return");
                }
                let (filename, is_dquote, filename_tok) = self.read_include_filename()?;
                let filename_path = Path::new(&filename);

                let mut embed_path = None;
                if is_dquote
                    && !filename_path.is_absolute()
                    && let Some(file) = get_input_file(start.location.file_no)
                {
                    let dir = Path::new(&file.name)
                        .parent()
                        .unwrap_or_else(|| Path::new("."));
                    let local_path = dir.join(filename_path);
                    if local_path.exists() {
                        embed_path = Some(local_path);
                    }
                }

                let embed_path = embed_path.unwrap_or_else(|| {
                    search_include_paths(&filename).unwrap_or_else(|| filename_path.to_path_buf())
                });
                let bytes = std::fs::read(&embed_path)
                    .map_err(|err| CompileError::at(err.to_string(), filename_tok.location))?;
                for (idx, byte) in bytes.iter().enumerate() {
                    out.push(new_num_token(*byte as i64, &filename_tok));
                    if idx + 1 < bytes.len() {
                        let mut comma = new_punct_token(Punct::Comma, &filename_tok);
                        comma.has_space = true;
                        out.push(comma);
                    }
                }
                continue;
            }

            if let TokenKind::Ident(name) = &self.cur_tok().kind
                && name == "define"
            {
                self.pos += 1;
                if self.pos >= self.tokens.len() {
                    self.error("macro name must be an identifier", start.location)?;
                    unreachable!("expected error to return");
                }
                let tok = self.cur_tok();
                if macro_name(tok).is_none() {
                    self.error("macro name must be an identifier", tok.location)?;
                    unreachable!("expected error to return");
                }
                self.read_macro_definition();
                continue;
            }

            if let TokenKind::Ident(name) = &self.cur_tok().kind
                && name == "undef"
            {
                self.pos += 1;
                if self.pos >= self.tokens.len() {
                    self.error("macro name must be an identifier", start.location)?;
                    unreachable!("expected error to return");
                }
                let tok = self.cur_tok();
                let Some(macro_name) = macro_name(tok) else {
                    self.error("macro name must be an identifier", tok.location)?;
                    unreachable!("expected error to return");
                };
                self.pos += 1;
                self.skip_line();
                self.undef_macro_internal(&macro_name);
                continue;
            }

            if let TokenKind::Ident(name) = &self.cur_tok().kind
                && (name == "ifdef" || name == "ifndef")
            {
                let is_ifdef = name == "ifdef";
                self.pos += 1;
                let defined = self.find_macro(self.cur_tok()).is_some();
                let included = if is_ifdef { defined } else { !defined };
                cond_incl.push(CondIncl {
                    ctx: CondCtx::Then,
                    tok: start,
                    included,
                });
                self.pos += 1;
                self.skip_line();
                if !included {
                    self.skip_cond_incl();
                }
                continue;
            }

            if is_if_directive(&self.cur_tok().kind) {
                self.pos += 1;
                let value = self.eval_const_expr()?;
                cond_incl.push(CondIncl {
                    ctx: CondCtx::Then,
                    tok: start,
                    included: value != 0,
                });
                if value == 0 {
                    self.skip_cond_incl();
                }
                continue;
            }

            if is_else_directive(&self.cur_tok().kind) {
                let Some(last) = cond_incl.last_mut() else {
                    self.error("stray #else", start.location)?;
                    unreachable!("expected error to return");
                };
                if last.ctx == CondCtx::Else {
                    self.error("stray #else", start.location)?;
                    unreachable!("expected error to return");
                }
                last.ctx = CondCtx::Else;
                self.pos += 1;
                self.skip_line();
                if last.included {
                    self.skip_cond_incl();
                }
                continue;
            }

            if let TokenKind::Ident(name) = &self.cur_tok().kind
                && name == "elif"
            {
                let Some(last) = cond_incl.last_mut() else {
                    self.error("stray #elif", start.location)?;
                    unreachable!("expected error to return");
                };
                if last.ctx == CondCtx::Else {
                    self.error("stray #elif", start.location)?;
                    unreachable!("expected error to return");
                }
                last.ctx = CondCtx::Elif;

                self.pos += 1;
                let value = self.eval_const_expr()?;
                if !last.included && value != 0 {
                    last.included = true;
                } else {
                    self.skip_cond_incl();
                }
                continue;
            }

            if let TokenKind::Ident(name) = &self.cur_tok().kind
                && (name == "elifdef" || name == "elifndef")
            {
                let Some(last) = cond_incl.last_mut() else {
                    self.error("stray #elif", start.location)?;
                    unreachable!("expected error to return");
                };
                if last.ctx == CondCtx::Else {
                    self.error("stray #elif", start.location)?;
                    unreachable!("expected error to return");
                }
                last.ctx = CondCtx::Elif;

                let is_elifdef = name == "elifdef";
                self.pos += 1;
                let defined = self.find_macro(self.cur_tok()).is_some();
                let included = if is_elifdef { defined } else { !defined };
                self.pos += 1;
                self.skip_line();

                if !last.included && included {
                    last.included = true;
                } else {
                    self.skip_cond_incl();
                }
                continue;
            }

            if let TokenKind::Ident(name) = &self.cur_tok().kind
                && name == "endif"
            {
                if cond_incl.is_empty() {
                    self.error("stray #endif", start.location)?;
                }
                cond_incl.pop();
                self.pos += 1;
                self.skip_line();
                continue;
            }

            if let TokenKind::Ident(name) = &self.cur_tok().kind
                && name == "line"
            {
                self.pos += 1;
                self.read_line_marker(&start)?;
                continue;
            }

            if matches!(self.cur_tok().kind, TokenKind::PPNum) {
                self.read_line_marker(&start)?;
                continue;
            }

            if let TokenKind::Ident(name) = &self.cur_tok().kind
                && name == "pragma"
            {
                if matches!(self.peek(1).kind, TokenKind::Ident(ref ident) if ident == "once")
                    && let Some(file) = get_input_file(start.location.file_no)
                    && let Ok(mut cache) = pragma_once_cache().lock()
                {
                    cache.insert(normalize_path(&file.name));
                    self.pos += 2;
                    self.skip_line();
                    continue;
                }
                self.pos += 1;
                self.skip_line();
                continue;
            }

            if let TokenKind::Ident(name) = &self.cur_tok().kind
                && name == "warning"
            {
                self.pos += 1;
                let line_tokens = self.copy_line();
                let tokens = self.expand_macros_only(line_tokens)?;
                let message = join_tokens(&tokens);
                let message = message.trim();
                let msg = if message.is_empty() {
                    "warning"
                } else {
                    message
                };
                warn_tok(&start, msg);
                continue;
            }

            if let TokenKind::Ident(name) = &self.cur_tok().kind
                && name == "error"
            {
                let error_location = start.location;
                self.pos += 1;
                let line_tokens = self.copy_line();
                let tokens = self.expand_macros_only(line_tokens)?;
                let message = join_tokens(&tokens);
                let message = message.trim();
                let msg = if message.is_empty() { "error" } else { message };
                self.error(msg, error_location)?;
            }

            if self.cur_tok().at_bol {
                continue;
            }

            self.error("invalid preprocessor directive", self.cur_tok().location)?;
            unreachable!("expected error to return");
        }

        if let Some(start) = cond_incl.first() {
            self.error("unterminated conditional directive", start.tok.location)?;
        }

        Ok(out)
    }

    fn read_include_filename(&mut self) -> CompileResult<(String, bool, Token)> {
        let line_tokens = self.copy_line();
        let Some(first) = line_tokens.first() else {
            return Err(CompileError::new("expected a filename"));
        };
        if matches!(first.kind, TokenKind::Eof) {
            return self.error("expected a filename", first.location);
        }

        let tokens = if matches!(first.kind, TokenKind::Ident(_)) {
            self.expand_macros_only(line_tokens)?
        } else {
            line_tokens
        };

        let (filename, is_dquote, consumed_idx, name_tok) = self.parse_include_filename(&tokens)?;
        if let Some(extra) = tokens.get(consumed_idx)
            && !matches!(extra.kind, TokenKind::Eof)
        {
            warn_tok(extra, "extra token");
        }
        Ok((filename, is_dquote, name_tok))
    }

    fn parse_include_filename(
        &self,
        tokens: &[Token],
    ) -> CompileResult<(String, bool, usize, Token)> {
        let Some(first) = tokens.first() else {
            return Err(CompileError::new("expected a filename"));
        };
        match &first.kind {
            TokenKind::Str { .. } => {
                let filename = raw_string_literal(first);
                Ok((filename, true, 1, first.clone()))
            }
            TokenKind::Punct(Punct::Less) => {
                let mut idx = 1;
                while idx < tokens.len() {
                    let tok = &tokens[idx];
                    if matches!(tok.kind, TokenKind::Punct(Punct::Greater)) {
                        let filename = join_tokens(&tokens[1..idx]);
                        return Ok((filename, false, idx + 1, first.clone()));
                    }
                    if tok.at_bol || matches!(tok.kind, TokenKind::Eof) {
                        return self.error("expected '>'", tok.location);
                    }
                    idx += 1;
                }
                self.error("expected '>'", first.location)
            }
            _ => self.error("expected a filename", first.location),
        }
    }

    fn include_file(
        &mut self,
        out: &mut Vec<Token>,
        include_path: &Path,
        filename_tok: &Token,
    ) -> CompileResult<()> {
        let cache_key = normalize_path(include_path);
        if let Ok(cache) = pragma_once_cache().lock()
            && cache.contains(&cache_key)
        {
            return Ok(());
        }

        if let Ok(cache) = include_guard_cache().lock()
            && let Some(guard_name) = cache.get(&cache_key)
            && self.macros.contains_key(guard_name)
        {
            return Ok(());
        }

        let included = tokenize_file(include_path)
            .map_err(|err| CompileError::at(err.message().to_string(), filename_tok.location))?;

        if let Some(guard_name) = detect_include_guard(&included)
            && let Ok(mut cache) = include_guard_cache().lock()
        {
            cache.insert(cache_key, guard_name);
        }

        let mut included_pp = Preprocessor::new(
            included,
            self.cmdline_defines.clone(),
            self.cmdline_undefs.clone(),
        );
        included_pp.macros = self.macros.clone();
        let included_tokens = included_pp.preprocess_tokens()?;
        self.macros = included_pp.macros;
        for inc in included_tokens {
            if !matches!(inc.kind, TokenKind::Eof) {
                out.push(inc);
            }
        }
        Ok(())
    }

    fn new_str_token(&self, text: &str, tmpl: &Token) -> CompileResult<Token> {
        let mut tokens = tokenize(text, tmpl.location.file_no)?;
        let mut tok = tokens
            .drain(..)
            .next()
            .ok_or_else(|| CompileError::new("failed to tokenize string"))?;
        tok.location = tmpl.location;
        tok.at_bol = tmpl.at_bol;
        tok.has_space = tmpl.has_space;
        tok.origin = tmpl.origin;
        Ok(tok)
    }

    fn stringize(&self, hash: &Token, arg: &[Token]) -> CompileResult<Token> {
        let joined = join_tokens(arg);
        let quoted = quote_string(&joined);
        self.new_str_token(&quoted, hash)
    }

    fn paste(&self, lhs: &Token, rhs: &Token) -> CompileResult<Token> {
        let buf = format!("{}{}", token_text(lhs), token_text(rhs));
        let mut tokens = tokenize_builtin("<paste>", &buf)?;
        convert_pp_tokens(&mut tokens)?;
        let mut iter = tokens.into_iter();
        let mut tok = iter
            .next()
            .ok_or_else(|| CompileError::new("failed to tokenize pasted token"))?;
        let next = iter
            .next()
            .ok_or_else(|| CompileError::new("failed to tokenize pasted token"))?;
        if !matches!(next.kind, TokenKind::Eof) || iter.next().is_some() {
            return self.error(
                format!("pasting forms '{buf}', an invalid token"),
                lhs.location,
            );
        }
        tok.location = lhs.location;
        tok.at_bol = lhs.at_bol;
        tok.has_space = lhs.has_space;
        tok.origin = lhs.origin;
        tok.len = 0;
        Ok(tok)
    }

    fn subst(&self, body: &[Token], args: &[MacroArg]) -> CompileResult<Vec<Token>> {
        let mut result = Vec::new();
        let mut i = 0;

        while i < body.len() {
            let tok = &body[i];
            if matches!(tok.kind, TokenKind::Eof) {
                break;
            }

            if let TokenKind::Ident(name) = &tok.kind
                && name == "__VA_OPT__"
                && matches!(
                    body.get(i + 1).map(|tok| &tok.kind),
                    Some(TokenKind::Punct(Punct::LParen))
                )
            {
                let mut tokens = Vec::new();
                let mut idx = i + 2;
                let mut level = 0i32;
                while idx < body.len() {
                    let cur = &body[idx];
                    match cur.kind {
                        TokenKind::Punct(Punct::LParen) => {
                            level += 1;
                            tokens.push(cur.clone());
                        }
                        TokenKind::Punct(Punct::RParen) => {
                            if level == 0 {
                                break;
                            }
                            level -= 1;
                            tokens.push(cur.clone());
                        }
                        TokenKind::Eof => {
                            return self.error("unterminated __VA_OPT__", cur.location);
                        }
                        _ => tokens.push(cur.clone()),
                    }
                    idx += 1;
                }

                if idx >= body.len() || !matches!(body[idx].kind, TokenKind::Punct(Punct::RParen)) {
                    return self.error("unterminated __VA_OPT__", tok.location);
                }

                if has_varargs(args) {
                    result.extend(tokens);
                }
                i = idx + 1;
                continue;
            }

            if matches!(tok.kind, TokenKind::Punct(Punct::Hash)) {
                let Some(next) = body.get(i + 1) else {
                    self.error("'#' is not followed by a macro parameter", tok.location)?;
                    unreachable!("expected error to return");
                };
                let Some(arg) = find_arg(args, next) else {
                    self.error("'#' is not followed by a macro parameter", next.location)?;
                    unreachable!("expected error to return");
                };
                let stringized = self.stringize(tok, &arg.tokens)?;
                result.push(stringized);
                i += 2;
                continue;
            }

            if matches!(tok.kind, TokenKind::Punct(Punct::HashHash)) {
                if result.is_empty() {
                    self.error(
                        "'##' cannot appear at start of macro expansion",
                        tok.location,
                    )?;
                    unreachable!("expected error to return");
                }
                let Some(next) = body.get(i + 1) else {
                    self.error("'##' cannot appear at end of macro expansion", tok.location)?;
                    unreachable!("expected error to return");
                };
                if matches!(next.kind, TokenKind::Eof) {
                    self.error("'##' cannot appear at end of macro expansion", tok.location)?;
                    unreachable!("expected error to return");
                }

                if let Some(arg) = find_arg(args, next) {
                    let is_empty = arg
                        .tokens
                        .first()
                        .is_some_and(|tok| matches!(tok.kind, TokenKind::Eof));
                    if !is_empty {
                        let pasted =
                            self.paste(result.last().expect("pasted lhs"), &arg.tokens[0])?;
                        *result.last_mut().expect("pasted lhs") = pasted;
                        for tok in arg.tokens.iter().skip(1) {
                            if matches!(tok.kind, TokenKind::Eof) {
                                break;
                            }
                            result.push(tok.clone());
                        }
                    }
                    i += 2;
                    continue;
                }

                let pasted = self.paste(result.last().expect("pasted lhs"), next)?;
                *result.last_mut().expect("pasted lhs") = pasted;
                i += 2;
                continue;
            }

            if matches!(tok.kind, TokenKind::Punct(Punct::Comma))
                && matches!(
                    body.get(i + 1).map(|tok| &tok.kind),
                    Some(TokenKind::Punct(Punct::HashHash))
                )
                && let Some(next) = body.get(i + 2)
                && let Some(arg) = find_arg(args, next)
                && arg.is_va_args
            {
                let is_empty = arg
                    .tokens
                    .first()
                    .is_some_and(|tok| matches!(tok.kind, TokenKind::Eof));
                if is_empty {
                    i += 3;
                } else {
                    result.push(tok.clone());
                    i += 2;
                }
                continue;
            }

            if let Some(arg) = find_arg(args, tok)
                && let Some(next) = body.get(i + 1)
                && matches!(next.kind, TokenKind::Punct(Punct::HashHash))
            {
                let rhs = body.get(i + 2).ok_or_else(|| {
                    CompileError::at(
                        "'##' cannot appear at end of macro expansion",
                        next.location,
                    )
                })?;
                let is_empty = arg
                    .tokens
                    .first()
                    .is_some_and(|tok| matches!(tok.kind, TokenKind::Eof));
                if is_empty {
                    if let Some(arg2) = find_arg(args, rhs) {
                        for tok in &arg2.tokens {
                            if matches!(tok.kind, TokenKind::Eof) {
                                break;
                            }
                            result.push(tok.clone());
                        }
                    } else {
                        result.push(rhs.clone());
                    }
                    i += 3;
                    continue;
                }

                for tok in &arg.tokens {
                    if matches!(tok.kind, TokenKind::Eof) {
                        break;
                    }
                    result.push(tok.clone());
                }
                i += 1;
                continue;
            }

            // Check if this token is a parameter that should be substituted.
            if let Some(arg) = find_arg(args, tok) {
                // Macro arguments are completely macro-expanded before substitution.
                let mut expanded = self.expand_macros_only(arg.tokens.clone())?;
                if let Some(first) = expanded.first_mut()
                    && !matches!(first.kind, TokenKind::Eof)
                {
                    first.at_bol = tok.at_bol;
                    first.has_space = tok.has_space;
                }
                for exp_tok in &expanded {
                    if matches!(exp_tok.kind, TokenKind::Eof) {
                        break;
                    }
                    result.push(exp_tok.clone());
                }
            } else {
                // Not a parameter, just copy the token.
                result.push(tok.clone());
            }

            i += 1;
        }

        Ok(result)
    }

    fn define_macro(&mut self, name: &str, body: &str) -> CompileResult<()> {
        let tokens = tokenize_builtin("<built-in>", body)?;
        self.macros.insert(
            name.to_string(),
            Macro {
                name: name.to_string(),
                is_objlike: true,
                body: tokens,
                ..Macro::default()
            },
        );
        Ok(())
    }

    fn undef_macro_internal(&mut self, name: &str) {
        self.macros.remove(name);
    }

    fn add_builtin(&mut self, name: &str, handler: MacroHandler) {
        self.macros.insert(
            name.to_string(),
            Macro {
                name: name.to_string(),
                is_objlike: true,
                handler: Some(handler),
                ..Macro::default()
            },
        );
    }

    /// __DATE__ is expanded to the current date, e.g. "Jan  1 2026".
    /// The format is always "Mmm DD YYYY" (11 characters).
    fn format_date() -> String {
        let now = Local::now();
        let month_names = [
            "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec",
        ];
        let month = month_names[now.month0() as usize];
        format!("\"{} {:2} {}\"", month, now.day(), now.year())
    }

    /// __TIME__ is expanded to the current time, e.g. "12:34:56".
    /// The format is always "HH:MM:SS" (8 characters).
    fn format_time() -> String {
        let now = Local::now();
        format!(
            "\"{:02}:{:02}:{:02}\"",
            now.hour(),
            now.minute(),
            now.second()
        )
    }

    fn init_macros(&mut self) -> CompileResult<()> {
        self.define_macro("_LP64", "1")?;
        self.define_macro("__C99_MACRO_WITH_VA_ARGS", "1")?;
        self.define_macro("__ELF__", "1")?;
        self.define_macro("__LP64__", "1")?;
        self.define_macro("__SIZEOF_DOUBLE__", "8")?;
        self.define_macro("__SIZEOF_FLOAT__", "4")?;
        self.define_macro("__SIZEOF_INT__", "4")?;
        self.define_macro("__SIZEOF_LONG_DOUBLE__", "8")?;
        self.define_macro("__SIZEOF_LONG_LONG__", "8")?;
        self.define_macro("__SIZEOF_LONG__", "8")?;
        self.define_macro("__SIZEOF_POINTER__", "8")?;
        self.define_macro("__SIZEOF_PTRDIFF_T__", "8")?;
        self.define_macro("__SIZEOF_SHORT__", "2")?;
        self.define_macro("__SIZEOF_SIZE_T__", "8")?;
        self.define_macro("__SIZE_TYPE__", "unsigned long")?;
        self.define_macro("__PTRDIFF_TYPE__", "long")?;
        self.define_macro("__INTMAX_TYPE__", "long")?;
        self.define_macro("__INTPTR_TYPE__", "long")?;
        self.define_macro("__UINTMAX_TYPE__", "unsigned long")?;
        self.define_macro("__UINTPTR_TYPE__", "unsigned long")?;
        self.define_macro("__WCHAR_TYPE__", "int")?;
        self.define_macro("__WINT_TYPE__", "unsigned int")?;
        self.define_macro("__CHAR16_TYPE__", "unsigned short")?;
        self.define_macro("__CHAR32_TYPE__", "unsigned int")?;
        self.define_macro("__INT8_TYPE__", "signed char")?;
        self.define_macro("__INT16_TYPE__", "short")?;
        self.define_macro("__INT32_TYPE__", "int")?;
        self.define_macro("__INT64_TYPE__", "long")?;
        self.define_macro("__UINT8_TYPE__", "unsigned char")?;
        self.define_macro("__UINT16_TYPE__", "unsigned short")?;
        self.define_macro("__UINT32_TYPE__", "unsigned int")?;
        self.define_macro("__UINT64_TYPE__", "unsigned long")?;
        self.define_macro("__INT_FAST8_TYPE__", "signed char")?;
        self.define_macro("__INT_FAST16_TYPE__", "short")?;
        self.define_macro("__INT_FAST32_TYPE__", "int")?;
        self.define_macro("__INT_FAST64_TYPE__", "long")?;
        self.define_macro("__UINT_FAST8_TYPE__", "unsigned char")?;
        self.define_macro("__UINT_FAST16_TYPE__", "unsigned short")?;
        self.define_macro("__UINT_FAST32_TYPE__", "unsigned int")?;
        self.define_macro("__UINT_FAST64_TYPE__", "unsigned long")?;
        self.define_macro("__INT_LEAST8_TYPE__", "signed char")?;
        self.define_macro("__INT_LEAST16_TYPE__", "short")?;
        self.define_macro("__INT_LEAST32_TYPE__", "int")?;
        self.define_macro("__INT_LEAST64_TYPE__", "long")?;
        self.define_macro("__UINT_LEAST8_TYPE__", "unsigned char")?;
        self.define_macro("__UINT_LEAST16_TYPE__", "unsigned short")?;
        self.define_macro("__UINT_LEAST32_TYPE__", "unsigned int")?;
        self.define_macro("__UINT_LEAST64_TYPE__", "unsigned long")?;
        // C11 char16_t and char32_t underlying types
        self.define_macro("__CHAR16_TYPE__", "unsigned short")?;
        self.define_macro("__CHAR32_TYPE__", "unsigned int")?;
        self.define_macro("__STDC_HOSTED__", "1")?;
        self.define_macro("__STDC_VERSION__", "201710L")?;
        self.define_macro("__STDC__", "1")?;
        // Optional feature support macros
        self.define_macro("__STDC_UTF_16__", "1")?;
        self.define_macro("__STDC_UTF_32__", "1")?;
        self.define_macro("__STDC_IEC_559__", "1")?; // IEEE 754 floating point
        self.define_macro("__STDC_IEC_559_COMPLEX__", "1")?; // IEC 60559 complex support
        self.define_macro("__USER_LABEL_PREFIX__", "")?;
        self.define_macro("__alignof__", "_Alignof")?;
        self.define_macro("static_assert", "_Static_assert")?;
        self.define_macro("__amd64", "1")?;
        self.define_macro("__amd64__", "1")?;
        self.define_macro("__chibicc__", "1")?;
        self.define_macro("__const__", "const")?;
        self.define_macro("__gnu_linux__", "1")?;
        self.define_macro("__inline__", "inline")?;
        self.define_macro("__linux", "1")?;
        self.define_macro("__linux__", "1")?;
        self.define_macro("__signed__", "signed")?;
        self.define_macro("__typeof__", "typeof")?;
        self.define_macro("__unix", "1")?;
        self.define_macro("__unix__", "1")?;
        self.define_macro("__volatile__", "volatile")?;
        self.define_macro("__x86_64", "1")?;
        self.define_macro("__x86_64__", "1")?;
        self.define_macro("linux", "1")?;
        self.define_macro("unix", "1")?;
        // GCC/clang predefine these as the __ATOMIC_* memory-order constants used
        // by the low-level __atomic_* builtins (distinct from <stdatomic.h>'s
        // memory_order enum).
        self.define_macro("__ATOMIC_RELAXED", "0")?;
        self.define_macro("__ATOMIC_CONSUME", "1")?;
        self.define_macro("__ATOMIC_ACQUIRE", "2")?;
        self.define_macro("__ATOMIC_RELEASE", "3")?;
        self.define_macro("__ATOMIC_ACQ_REL", "4")?;
        self.define_macro("__ATOMIC_SEQ_CST", "5")?;
        self.add_builtin("__FILE__", file_macro);
        self.add_builtin("__LINE__", line_macro);
        self.add_builtin("__COUNTER__", counter_macro);
        self.add_builtin("__TIMESTAMP__", timestamp_macro);
        self.add_builtin("__BASE_FILE__", base_file_macro);
        self.define_macro("__DATE__", &Self::format_date())?;
        self.define_macro("__TIME__", &Self::format_time())?;

        // Apply command-line defined macros
        let cmdline_defines = self.cmdline_defines.clone();
        for (name, value) in cmdline_defines {
            self.define_macro(&name, &value)?;
        }

        // Apply command-line undefined macros
        let cmdline_undefs = self.cmdline_undefs.clone();
        for name in cmdline_undefs {
            self.undef_macro_internal(&name);
        }

        Ok(())
    }
}

/// Entry point of the preprocessor.
pub fn preprocess(
    tokens: Vec<Token>,
    cmdline_defines: Vec<(String, String)>,
    cmdline_undefs: Vec<String>,
) -> CompileResult<Vec<Token>> {
    let mut preprocessor = Preprocessor::new(tokens, cmdline_defines, cmdline_undefs);
    preprocessor.init_macros()?;
    let mut tokens = preprocessor.preprocess_tokens()?;
    convert_pp_tokens(&mut tokens)?;
    join_adjacent_string_literals(&mut tokens)?;
    for tok in &mut tokens {
        if tok.line_delta != 0 {
            let line = tok.location.line as i64 + tok.line_delta as i64;
            tok.location.line = line.max(1) as usize;
        }
    }
    Ok(tokens)
}
