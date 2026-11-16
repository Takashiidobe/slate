use crate::backend::rust_ast::{
    AsmOperand, EnumDef, Expr, Ident, InlineAsm, Item, Path, Program, RecordDef, StructDef,
    Visibility,
};

pub fn hoist_global_asm(
    programs: &mut [(String, Program)],
    root: Option<&str>,
    gated: impl Fn(&str) -> bool,
) -> Vec<Item> {
    let carriers = programs
        .iter()
        .filter(|(stem, program)| {
            !gated(stem) && program.items.iter().any(|item| hoistable(program, item))
        })
        .count();
    if carriers < 2 {
        return Vec::new();
    }
    let mut blocks = Vec::new();
    for (stem, program) in programs.iter_mut().filter(|(stem, _)| !gated(stem)) {
        let module: Vec<Ident> = std::iter::once(Ident::new("crate"))
            .chain(
                (root != Some(stem.as_str())).then(|| Ident::new(format!("__slate_unit_{stem}"))),
            )
            .collect();
        let mut published = Vec::new();
        let hoisted: Vec<bool> = program
            .items
            .iter()
            .map(|item| hoistable(program, item))
            .collect();
        for (item, hoist) in std::mem::take(&mut program.items).into_iter().zip(hoisted) {
            match item {
                Item::GlobalAsm(mut asm) if hoist => {
                    for operand in &mut asm.operands {
                        if let AsmOperand::Sym(expr) = operand
                            && let Expr::Var(name) = expr
                        {
                            published.push(name.as_str().to_owned());
                            *expr = Expr::Path(Path::new(module.iter().cloned().chain([*name])));
                        }
                    }
                    blocks.push(asm);
                }
                item => program.items.push(item),
            }
        }
        if published.is_empty() {
            continue;
        }
        for item in &mut program.items {
            match item {
                Item::Fn(function) if published.contains(&function.name) => {
                    function.vis = Visibility::Pub;
                }
                Item::Static { vis, name, .. } if published.contains(name) => {
                    *vis = Visibility::Pub;
                }
                Item::Record(RecordDef { vis, .. })
                | Item::Struct(StructDef { vis, .. })
                | Item::Enum(EnumDef { vis, .. }) => *vis = Visibility::Pub,
                _ => {}
            }
        }
    }
    concatenate(blocks)
        .into_iter()
        .map(Item::GlobalAsm)
        .collect()
}

fn hoistable(program: &Program, item: &Item) -> bool {
    let Item::GlobalAsm(asm) = item else {
        return false;
    };
    asm.operands.iter().all(|operand| match operand {
        AsmOperand::Sym(Expr::Var(name)) => program.items.iter().any(|item| match item {
            Item::Fn(function) => function.name == name.as_str(),
            Item::Static { name: defined, .. } => defined == name.as_str(),
            _ => false,
        }),
        _ => false,
    })
}

fn concatenate(blocks: Vec<InlineAsm>) -> Vec<InlineAsm> {
    let mut merged: Vec<InlineAsm> = Vec::new();
    for block in blocks {
        match merged.last_mut() {
            Some(last) if last.options == block.options => {
                let template = renumbered(&block.template, last.operands.len());
                last.template = format!("{}\n{template}", last.template);
                last.operands.extend(block.operands);
            }
            _ => {
                let template = renumbered(&block.template, 0);
                merged.push(InlineAsm { template, ..block });
            }
        }
    }
    merged
}

fn renumbered(template: &str, offset: usize) -> String {
    let mut out = String::with_capacity(template.len());
    let mut implicit = 0;
    let mut chars = template.chars().peekable();
    while let Some(c) = chars.next() {
        match c {
            '{' if chars.peek() == Some(&'{') => {
                chars.next();
                out.push_str("{{");
            }
            '}' if chars.peek() == Some(&'}') => {
                chars.next();
                out.push_str("}}");
            }
            '{' => {
                let inner: String = chars.by_ref().take_while(|&c| c != '}').collect();
                let (index, modifier) = match inner.split_once(':') {
                    Some((index, modifier)) => (index, Some(modifier)),
                    None => (inner.as_str(), None),
                };
                let index = if index.is_empty() {
                    implicit += 1;
                    implicit - 1
                } else {
                    index
                        .parse::<usize>()
                        .expect("slate emits positional global_asm! placeholders")
                };
                out.push_str(&format!("{{{}", index + offset));
                if let Some(modifier) = modifier {
                    out.push_str(&format!(":{modifier}"));
                }
                out.push('}');
            }
            c => out.push(c),
        }
    }
    out
}
