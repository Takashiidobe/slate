use crate::backend::rust_ast::{
    AsmOperand, Attr, EnumDef, Expr, Ident, InlineAsm, Item, Path, Program, RecordDef, StructDef,
    Type, Visibility,
};
use std::collections::{HashMap, HashSet};

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
        let module = module_path(stem, root);
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
                _ => {}
            }
        }
        publish_types(program);
    }
    concatenate(blocks)
        .into_iter()
        .map(Item::GlobalAsm)
        .collect()
}

pub fn hoist_section_statics(
    programs: &mut [(String, Program)],
    root: Option<&str>,
    gated: impl Fn(&str) -> bool,
) -> Vec<Item> {
    let mut contributors: HashMap<String, HashSet<usize>> = HashMap::new();
    for (index, (stem, program)) in programs.iter().enumerate() {
        if gated(stem) {
            continue;
        }
        for section in program.items.iter().filter_map(placed_section) {
            contributors
                .entry(section.to_owned())
                .or_default()
                .insert(index);
        }
    }
    let shared: HashSet<String> = contributors
        .into_iter()
        .filter(|(_, units)| units.len() > 1)
        .map(|(section, _)| section)
        .collect();
    if shared.is_empty() {
        return Vec::new();
    }
    let mut moved = Vec::new();
    for (stem, program) in programs.iter_mut().filter(|(stem, _)| !gated(stem)) {
        let module = module_path(stem, root);
        let mut unit_items = Vec::new();
        for item in &mut program.items {
            if !placed_section(item).is_some_and(|section| shared.contains(section)) {
                continue;
            }
            let index = moved.len();
            let root_name = format!("__slate_link_order_{index}");
            let type_name = format!("__slate_link_order_type_{index}");
            let init_name = format!("__slate_link_order_init_{index}");
            let Item::Static {
                attrs,
                mutable,
                name,
                ty,
                init,
                ..
            } = std::mem::replace(
                item,
                Item::Use {
                    vis: Visibility::Pub,
                    path: Path::new([Ident::new("crate"), Ident::new(&root_name)]),
                    alias: None,
                },
            )
            else {
                unreachable!("placed_section only matches statics")
            };
            if let Item::Use { alias, .. } = item {
                *alias = Some(Ident::new(&name));
            }
            let qualified = |name: &str| {
                module
                    .iter()
                    .map(Ident::as_str)
                    .chain([name])
                    .collect::<Vec<_>>()
                    .join("::")
            };
            moved.push(Item::Static {
                comments: Vec::new(),
                attrs,
                vis: Visibility::Pub,
                mutable,
                name: root_name,
                ty: Type::Custom(qualified(&type_name)),
                init: Expr::Path(Path::new(
                    module.iter().cloned().chain([Ident::new(&init_name)]),
                )),
            });
            unit_items.push(Item::TypeAlias {
                vis: Visibility::Pub,
                name: type_name.clone(),
                ty,
            });
            unit_items.push(Item::Const {
                attrs: Vec::new(),
                vis: Visibility::Pub,
                name: init_name,
                ty: Type::Custom(type_name),
                init,
            });
        }
        if !unit_items.is_empty() {
            program.items.extend(unit_items);
            publish_types(program);
        }
    }
    moved
}

fn placed_section(item: &Item) -> Option<&str> {
    let Item::Static { attrs, .. } = item else {
        return None;
    };
    if attrs.contains(&Attr::ThreadLocal) {
        return None;
    }
    attrs.iter().find_map(|attr| match attr {
        Attr::LinkSection(section) => Some(section.as_str()),
        _ => None,
    })
}

fn module_path(stem: &str, root: Option<&str>) -> Vec<Ident> {
    std::iter::once(Ident::new("crate"))
        .chain((root != Some(stem)).then(|| Ident::new(format!("__slate_unit_{stem}"))))
        .collect()
}

fn publish_types(program: &mut Program) {
    for item in &mut program.items {
        if let Item::Record(RecordDef { vis, .. })
        | Item::Struct(StructDef { vis, .. })
        | Item::Enum(EnumDef { vis, .. }) = item
        {
            *vis = Visibility::Pub;
        }
    }
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
