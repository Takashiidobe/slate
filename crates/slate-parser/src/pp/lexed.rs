use crate::ast::FileId;
use crate::charset::Charset;
use crate::lexer::{Lexer, LineToken};
use crate::standard_features::StandardFeatures;
use std::cell::RefCell;
use std::collections::HashMap;
use std::rc::Rc;
use std::sync::atomic::{AtomicBool, Ordering};

pub(super) type Lexed = Rc<Vec<LineToken>>;

struct Variant {
    features: StandardFeatures,
    charset: Charset,
    tokens: Lexed,
}

static REUSE: AtomicBool = AtomicBool::new(false);

thread_local! {
    static LEXED: RefCell<HashMap<String, Vec<Variant>>> = RefCell::default();
}

pub fn reuse_lexed_sources() {
    REUSE.store(true, Ordering::Relaxed);
}

pub(super) fn lexed_tokens(
    file: FileId,
    src: &str,
    features: StandardFeatures,
    charset: Charset,
) -> Lexed {
    let lex = || {
        Rc::new(
            Lexer::new(file, src, features)
                .decoding(charset)
                .tokenize_lines(),
        )
    };
    if !REUSE.load(Ordering::Relaxed) {
        return lex();
    }
    LEXED.with_borrow_mut(|sources| {
        let found = sources.get(src).and_then(|variants| {
            variants
                .iter()
                .find(|variant| variant.features == features && variant.charset == charset)
                .map(|variant| variant.tokens.clone())
        });
        found.unwrap_or_else(|| {
            let tokens = lex();
            sources.entry(src.to_string()).or_default().push(Variant {
                features,
                charset,
                tokens: tokens.clone(),
            });
            tokens
        })
    })
}
