//! lower: combine the CIR Op-tree with the C AST oracle into Rust output.

use crate::c_ast::{EnumConstRef, LayoutQuery, Loc, MacroConst, RecordKind, Unit};
use crate::cir::ir::{Attr, Block, CirOpKind, Module, Op, Region};
use crate::ctx::Ctx;
use crate::function_identity::{CallBinding, FunctionIdentity};
use crate::rust_ast::{
    Abi, AsmDialect, AsmOperand, AsmReg, AtomicOrdering, AtomicPlace, AtomicRmwOp, AtomicType,
    Attr as RustAttr, BinOp, CLibType, Cfg, CrateAttr, Derive, EnumConst, EnumDef, Expr,
    ExprMatchArm, ExternDecl, ExternFnDecl, Feature, FnDef, FnParam, GenericParam, Ident,
    ImplBlock, ImplItem, IndentStmt, InlineAsm, Item, Label, Lint, MatchArm, Method, Path, Pattern,
    Prim, Program, RecordDef, RecordField, Repr, RustValue, SelfKind, StdTrait, Stmt, StructDef,
    StructFields, TraitBound, Type, UnaryOp, UsedKind, Visibility,
};
use std::collections::{BTreeMap, BTreeSet, HashMap, VecDeque};

#[derive(Default, Clone)]
pub struct ProjectInfo {
    pub cross_module: BTreeMap<String, String>,
    pub cross_module_globals: BTreeMap<String, String>,
    pub child_modules: Vec<String>,
    pub shared_records: BTreeSet<String>,
    pub shared_enums: BTreeSet<String>,
    pub shared_type_module: Option<String>,
    pub shared_type_crate: Option<String>,
    pub cross_module_crate: Option<String>,
    pub unsafe_functions: BTreeSet<String>,
    pub crate_features: BTreeSet<Feature>,
    pub emit_pub: bool,
}

const WEAK_ANY_LINKAGE: i64 = 4;
const HIDDEN_VISIBILITY: i64 = 1;
const PROTECTED_VISIBILITY: i64 = 2;

fn linkage_is_external(op: &Op) -> bool {
    matches!(attr_int(op, "linkage").unwrap_or(0), 0 | WEAK_ANY_LINKAGE)
}

fn visibility_allows_export(op: &Op) -> bool {
    attr_int(op, "global_visibility").unwrap_or(0) != HIDDEN_VISIBILITY
}

fn externally_exported(op: &Op) -> bool {
    linkage_is_external(op) && visibility_allows_export(op)
}

fn visibility_is_protected(op: &Op) -> bool {
    attr_int(op, "global_visibility") == Some(PROTECTED_VISIBILITY)
}

fn linkage_is_weak(op: &Op) -> bool {
    attr_int(op, "linkage") == Some(WEAK_ANY_LINKAGE)
}

fn symbol_attrs(
    no_mangle: bool,
    weak_linkage: bool,
    section: Option<&str>,
    used: &[UsedKind],
) -> Vec<RustAttr> {
    let mut attrs = Vec::new();
    if no_mangle {
        attrs.push(RustAttr::NoMangle);
    }
    if weak_linkage {
        attrs.push(RustAttr::WeakLinkage);
    }
    if let Some(section) = section {
        attrs.push(RustAttr::LinkSection(section.to_string()));
    }
    attrs.extend(used.iter().copied().map(RustAttr::Used));
    attrs
}

fn insert_crate_feature(items: &mut [Item], feature: Feature) {
    let Some(Item::CrateAttrs(attrs)) = items.first_mut() else {
        return;
    };
    if !attrs
        .iter()
        .any(|attr| matches!(attr, CrateAttr::Feature(existing) if *existing == feature))
    {
        attrs.insert(0, CrateAttr::Feature(feature));
    }
}

fn region_ends_in_noreturn_call(region: &Region) -> bool {
    if region.blocks.len() != 1 {
        return false;
    }
    let Some(block) = region.blocks.first() else {
        return false;
    };
    let ops: &[Op] = match block.ops.last() {
        Some(last) if matches!(last.kind(), CirOpKind::Return | CirOpKind::Yield) => {
            &block.ops[..block.ops.len() - 1]
        }
        _ => &block.ops,
    };
    match ops.last() {
        Some(op) if op.kind() == CirOpKind::Call => attr_bool(op, "noreturn"),
        Some(op) if op.kind() == CirOpKind::Scope => {
            op.regions.first().is_some_and(region_ends_in_noreturn_call)
        }
        _ => false,
    }
}

fn function_requires_unsafe_contract(op: &Op) -> bool {
    let (params, _) = parse_function_type(attr_str(op, "function_type").unwrap_or(""));
    if !params.iter().any(|ty| ty.starts_with("!cir.ptr<")) {
        return false;
    }

    let mut ops = Vec::new();
    collect_region_ops_recursive(op, &mut ops);
    let mut local_ptrs = BTreeSet::new();
    for op in ops {
        if op.kind() == CirOpKind::Alloca
            || (matches!(op.kind(), CirOpKind::GetMember | CirOpKind::GetElement)
                && op
                    .operands
                    .first()
                    .is_some_and(|base| local_ptrs.contains(base)))
        {
            local_ptrs.extend(op.results.iter().cloned());
        } else if matches!(op.kind(), CirOpKind::Load | CirOpKind::Store)
            && op
                .operands
                .last()
                .is_some_and(|ptr| !local_ptrs.contains(ptr))
        {
            return true;
        }
    }
    false
}

pub fn defined_functions(module: &Module) -> Vec<String> {
    let Some(module_op) = module.ops.iter().find(|op| op.name == "builtin.module") else {
        return Vec::new();
    };
    region_ops(module_op)
        .iter()
        .filter(|op| {
            op.kind() == CirOpKind::Func
                && externally_exported(op)
                && (!region_ops(op).is_empty()
                    || attr_symbol_ref(op, "aliasee").is_some() && !linkage_is_weak(op))
        })
        .filter_map(|op| attr_str(op, "sym_name").map(str::to_string))
        .collect()
}

pub fn declared_functions(module: &Module) -> Vec<String> {
    let Some(module_op) = module.ops.iter().find(|op| op.name == "builtin.module") else {
        return Vec::new();
    };
    region_ops(module_op)
        .iter()
        .filter(|op| op.kind() == CirOpKind::Func && region_ops(op).is_empty())
        .filter_map(|op| attr_str(op, "sym_name").map(str::to_string))
        .collect()
}

pub fn defined_globals(module: &Module) -> Vec<String> {
    let Some(module_op) = module.ops.iter().find(|op| op.name == "builtin.module") else {
        return Vec::new();
    };
    region_ops(module_op)
        .iter()
        .filter(|op| {
            op.kind() == CirOpKind::Global
                && attr_str(op, "initial_value").is_some()
                && externally_exported(op)
        })
        .filter_map(|op| attr_str(op, "sym_name").map(|name| sanitize_ident(name).into_string()))
        .collect()
}

pub fn unsafe_defined_functions(module: &Module) -> BTreeSet<String> {
    let Some(module_op) = module.ops.iter().find(|op| op.name == "builtin.module") else {
        return BTreeSet::new();
    };
    let ops = region_ops(module_op);
    let mut unsafe_functions: BTreeSet<String> = ops
        .iter()
        .filter(|op| {
            op.kind() == CirOpKind::Func && !region_ops(op).is_empty() && externally_exported(op)
        })
        .filter(|op| function_requires_unsafe_contract(op))
        .filter_map(|op| attr_str(op, "sym_name").map(str::to_string))
        .filter(|name| name != "main")
        .collect();
    for op in ops {
        if op.kind() != CirOpKind::Func || !region_ops(op).is_empty() {
            continue;
        }
        let Some(name) = attr_str(op, "sym_name") else {
            continue;
        };
        let Some(target) = attr_symbol_ref(op, "aliasee") else {
            continue;
        };
        if unsafe_functions.contains(target) {
            unsafe_functions.insert(name.to_string());
        }
    }
    unsafe_functions
}

pub fn required_features(module: &Module) -> BTreeSet<Feature> {
    let mut features = BTreeSet::new();
    let Some(module_op) = module.ops.iter().find(|op| op.name == "builtin.module") else {
        return features;
    };
    for op in region_ops(module_op) {
        if op.ty.as_deref().is_some_and(|ty| ty.contains("!cir.f128"))
            || op
                .attrs
                .values()
                .filter_map(Attr::as_str)
                .any(|value| value.contains("!cir.f128"))
        {
            features.insert(Feature::F128);
        }
        if linkage_is_weak(op) && attr_symbol_ref(op, "aliasee").is_none() {
            features.insert(Feature::Linkage);
        }
        if op.kind() == CirOpKind::Asm
            && !op.results.is_empty()
            && attr_str(op, "asm_string").is_some_and(asm_template_has_labels)
        {
            features.insert(Feature::AsmGotoWithOutputs);
        }
        if op.kind() == CirOpKind::Func {
            let function_type = attr_str(op, "function_type").unwrap_or("");
            if function_type_is_variadic(function_type) {
                features.insert(Feature::CVariadic);
            }
        } else if op.kind() == CirOpKind::Global
            && matches!(attr_str(op, "sym_name"), Some("llvm.used"))
        {
            features.insert(Feature::UsedWithArg);
        }
    }
    features
}

pub fn lower(cir: &Module, c: &Unit, ctx: &mut Ctx) -> Program {
    lower_with_project(cir, c, ctx, &ProjectInfo::default())
}

pub fn lower_with_project(cir: &Module, c: &Unit, ctx: &mut Ctx, project: &ProjectInfo) -> Program {
    let mut anon_records = anon_local_records(cir);
    let cir_record_names: BTreeSet<String> = cir
        .aliases
        .values()
        .filter_map(|ty| cir_record_name(ty))
        .map(|name| sanitize_ident(name).into_string())
        .collect();
    let mut anon_record_names: BTreeSet<String> = anon_records
        .iter()
        .map(|record| sanitize_ident(&record.name).into_string())
        .collect();
    for record in &c.anonymous_header_records {
        let name = sanitize_ident(&record.name).into_string();
        if cir_record_names.contains(&name)
            && clib_record_type(&record.name).is_none()
            && anon_record_names.insert(name)
        {
            anon_records.push(record.clone());
        }
    }
    let mut records: BTreeMap<String, crate::c_ast::Record> = c
        .records
        .iter()
        .map(|record| (sanitize_ident(&record.name).into_string(), record.clone()))
        .collect();
    let enums: BTreeMap<String, crate::c_ast::Enum> = c
        .enums
        .iter()
        .map(|enm| (sanitize_ident(&enm.name).into_string(), enm.clone()))
        .collect();
    for record in &anon_records {
        let name = sanitize_ident(&record.name).into_string();
        if record
            .fields
            .iter()
            .any(|field| field.name.starts_with("__bitfield_"))
        {
            records.insert(name, record.clone());
        } else {
            records.entry(name).or_insert_with(|| record.clone());
        }
    }
    reconcile_anonymous_member_types(cir, &mut records);
    let mut lowerer = Lowerer {
        ctx,
        aliases: cir.aliases.clone(),
        call_bindings: c.call_bindings(),
        known_functions: c
            .call_bindings()
            .values()
            .filter_map(|binding| binding.known())
            .map(|known| (known.symbol().to_string(), FunctionIdentity::Known(known)))
            .collect(),
        weak_refs: c
            .weak_refs
            .iter()
            .map(|attribute| (attribute.name.clone(), attribute.target.clone()))
            .collect(),
        external_weak_targets: BTreeSet::new(),
        weak_aliases: BTreeMap::new(),
        records,
        enums,
        anon_records,
        globals: BTreeMap::new(),
        extern_globals: BTreeMap::new(),
        strings: BTreeMap::new(),
        const_arrays: BTreeMap::new(),
        block_addr_globals: BTreeMap::new(),
        const_aggregates: BTreeMap::new(),
        const_zero_globals: BTreeSet::new(),
        used_symbols: BTreeMap::new(),
        externs: BTreeMap::new(),
        extern_returns: BTreeMap::new(),
        long_double_shims: BTreeMap::new(),
        uses_long_double: std::cell::Cell::new(false),
        uses_complex: std::cell::Cell::new(false),
        uses_f128: std::cell::Cell::new(false),
        uses_c_variadic: std::cell::Cell::new(false),
        uses_linkage: std::cell::Cell::new(false),
        uses_thread_local: std::cell::Cell::new(false),
        uses_used_with_arg: std::cell::Cell::new(false),
        uses_asm_goto_outputs: std::cell::Cell::new(false),
        uses_breakpoint: std::cell::Cell::new(false),
        uses_memchr: std::cell::Cell::new(false),
        target_arch: cir
            .ops
            .iter()
            .find_map(|op| attr_str(op, "cir.triple"))
            .and_then(rust_target_arch)
            .map(str::to_string),
        variadic_defs: BTreeSet::new(),
        c_abi_functions: BTreeSet::new(),
        project: project.clone(),
        unsafe_functions: project.unsafe_functions.clone(),
        cross_uses: Vec::new(),
        ctor_calls: Vec::new(),
        dtor_calls: Vec::new(),
        generated_alloca_frames: Vec::new(),
        layout_queries: c
            .functions
            .iter()
            .map(|function| (function.name.clone(), function.layout_queries.clone()))
            .collect(),
        macro_consts: c
            .functions
            .iter()
            .map(|function| (function.name.clone(), function.macro_consts.clone()))
            .collect(),
        enum_consts: c
            .functions
            .iter()
            .map(|function| (function.name.clone(), function.enum_consts.clone()))
            .collect(),
        asm_gotos: c
            .functions
            .iter()
            .map(|function| (function.name.clone(), function.asm_gotos.clone()))
            .collect(),
    };
    lowerer.lower_module(cir, c)
}

pub fn lower_shared_types(
    records: &[crate::c_ast::Record],
    enums: &[crate::c_ast::Enum],
) -> Program {
    let items = std::iter::once(Item::CrateAttrs(vec![CrateAttr::Allow(vec![
        Lint::DeadCode,
        Lint::Unused,
        Lint::NonSnakeCase,
        Lint::NonUpperCaseGlobals,
    ])]))
    .chain(
        enums
            .iter()
            .filter_map(|enm| lower_enum_def(enm, Visibility::Pub).map(Item::Enum)),
    )
    .chain(
        records
            .iter()
            .flat_map(|record| lower_record_def(record, Visibility::Pub, Visibility::Pub, true)),
    )
    .collect();
    Program {
        items,
        ..Program::default()
    }
}

fn lower_enum_def(enm: &crate::c_ast::Enum, vis: Visibility) -> Option<EnumDef> {
    if enm.variants.is_empty() {
        return None;
    }
    Some(EnumDef {
        comments: comments(&enm.comments),
        attrs: enum_attrs(),
        vis,
        name: sanitize_ident(&enm.name).into_string(),
        variants: enm
            .variants
            .iter()
            .map(|variant| EnumConst {
                comments: comments(&variant.comments),
                name: sanitize_ident(&variant.name).into_string(),
                value: variant.value,
            })
            .collect(),
    })
}

fn enum_attrs() -> Vec<RustAttr> {
    vec![
        RustAttr::Repr(vec![Repr::C]),
        RustAttr::Allow(vec![Lint::NonCamelCaseTypes]),
        RustAttr::Derive(vec![
            Derive::Clone,
            Derive::Copy,
            Derive::PartialEq,
            Derive::Eq,
            Derive::Debug,
            Derive::Hash,
        ]),
    ]
}

fn aligned_type(ty: Type, alignment: u32) -> Type {
    Type::Generic {
        name: "aligned::Aligned".into(),
        args: vec![Type::Custom(format!("aligned::A{alignment}")), ty],
    }
}

fn aligned_value(value: Expr, _alignment: u32) -> Expr {
    Expr::TupleStructLit {
        name: "aligned::Aligned".into(),
        fields: vec![value],
    }
}

fn type_alignment(ty: &Type) -> u32 {
    match ty {
        Type::Prim(Prim::Bool | Prim::I8 | Prim::U8) => 1,
        Type::Prim(Prim::I16 | Prim::U16) => 2,
        Type::Prim(Prim::I32 | Prim::U32 | Prim::F32) => 4,
        Type::Prim(Prim::I64 | Prim::U64 | Prim::Isize | Prim::Usize | Prim::F64)
        | Type::Ptr { .. }
        | Type::FnPtr { .. }
        | Type::Ref { .. }
        | Type::VaList => 8,
        Type::CLib(CLibType::MbState) => 4,
        Type::CLib(CLibType::Timespec) => 8,
        Type::Prim(Prim::I128 | Prim::U128 | Prim::F128) | Type::LongDouble => 16,
        Type::Array { elem, .. } => type_alignment(elem),
        Type::Complex(inner) => type_alignment(inner),
        _ => 1,
    }
}

fn lower_record_def(
    record: &crate::c_ast::Record,
    vis: Visibility,
    field_vis: Visibility,
    allow_empty: bool,
) -> Vec<Item> {
    if record.fields.is_empty() && !allow_empty {
        return Vec::new();
    }
    let is_union = record.kind == RecordKind::Union;
    let fields: Vec<RecordField> = record
        .fields
        .iter()
        .map(|field| RecordField {
            comments: comments(&field.comments),
            name: sanitize_ident(&field.name),
            ty: c_record_field_type(&field.ty),
        })
        .collect();
    let name = sanitize_ident(&record.name).into_string();

    if let Some(packed) = record.packed
        && let Some(align) = record.align
    {
        return lower_packed_aligned_wrapper(
            &name, fields, is_union, vis, field_vis, packed, align,
        );
    }

    vec![Item::Record(RecordDef {
        comments: comments(&record.comments),
        vis,
        field_vis,
        is_union,
        allow_non_camel_case: name == "__once_flag",
        name,
        fields,
        packed: record.packed,
        align: record.align,
    })]
}

fn packed_aligned_inner_name(name: &str) -> String {
    format!("{name}__packed")
}

fn lower_packed_aligned_wrapper(
    name: &str,
    fields: Vec<RecordField>,
    is_union: bool,
    vis: Visibility,
    field_vis: Visibility,
    packed: u32,
    align: u32,
) -> Vec<Item> {
    let inner_name = packed_aligned_inner_name(name);
    let inner = Item::Record(RecordDef {
        comments: Vec::new(),
        vis,
        field_vis,
        is_union,
        allow_non_camel_case: false,
        name: inner_name.clone(),
        fields,
        packed: Some(packed),
        align: None,
    });
    let outer = Item::Struct(StructDef {
        attrs: vec![
            RustAttr::Repr(vec![Repr::C, Repr::Align(align)]),
            RustAttr::Derive(vec![Derive::Clone, Derive::Copy]),
        ],
        vis,
        generics: vec![],
        name: name.to_string(),
        fields: StructFields::Tuple(vec![Type::Custom(inner_name.clone())]),
    });
    let self_field = |mutable| Expr::Ref {
        mutable,
        expr: Box::new(Expr::Field {
            base: Box::new(Expr::Var("self".into())),
            field: "0".into(),
        }),
    };
    let deref = Item::Impl(ImplBlock {
        generics: vec![],
        trait_: Some(StdTrait::Deref),
        self_ty: Type::Custom(name.to_string()),
        items: vec![
            ImplItem::AssocType {
                name: "Target".into(),
                ty: Type::Custom(inner_name.clone()),
            },
            ImplItem::Method(Method {
                name: "deref".into(),
                self_kind: SelfKind::Ref,
                params: vec![],
                ret: Some(Type::Ref {
                    mutable: false,
                    inner: Box::new(Type::Custom(inner_name.clone())),
                }),
                body: self_field(false),
            }),
        ],
    });
    let deref_mut = Item::Impl(ImplBlock {
        generics: vec![],
        trait_: Some(StdTrait::DerefMut),
        self_ty: Type::Custom(name.to_string()),
        items: vec![ImplItem::Method(Method {
            name: "deref_mut".into(),
            self_kind: SelfKind::RefMut,
            params: vec![],
            ret: Some(Type::Ref {
                mutable: true,
                inner: Box::new(Type::Custom(inner_name)),
            }),
            body: self_field(true),
        })],
    });
    vec![inner, outer, deref, deref_mut]
}

fn record_lit_name(record: &crate::c_ast::Record) -> String {
    let name = sanitize_ident(&record.name).into_string();
    if record.packed.is_some() && record.align.is_some() {
        packed_aligned_inner_name(&name)
    } else {
        name
    }
}

fn wrap_record_lit(record: &crate::c_ast::Record, lit: Expr) -> Expr {
    if record.packed.is_some() && record.align.is_some() {
        Expr::TupleStructLit {
            name: sanitize_ident(&record.name).into_string(),
            fields: vec![lit],
        }
    } else {
        lit
    }
}

fn comments(lines: &[String]) -> Vec<crate::rust_ast::Comment> {
    if lines.is_empty() {
        Vec::new()
    } else {
        vec![crate::rust_ast::Comment {
            lines: lines.to_vec(),
        }]
    }
}

struct Lowerer<'a> {
    ctx: &'a mut Ctx,
    aliases: BTreeMap<String, String>,
    call_bindings: HashMap<Loc, CallBinding>,
    known_functions: BTreeMap<String, FunctionIdentity>,
    weak_refs: BTreeMap<String, String>,
    external_weak_targets: BTreeSet<String>,
    weak_aliases: BTreeMap<String, String>,
    records: BTreeMap<String, crate::c_ast::Record>,
    enums: BTreeMap<String, crate::c_ast::Enum>,
    anon_records: Vec<crate::c_ast::Record>,
    globals: BTreeMap<String, GlobalVar>,
    extern_globals: BTreeMap<String, ExternGlobal>,
    strings: BTreeMap<String, Vec<u8>>,
    const_arrays: BTreeMap<String, Vec<Expr>>,
    block_addr_globals: BTreeMap<String, Vec<String>>,
    const_aggregates: BTreeMap<String, String>,
    const_zero_globals: BTreeSet<String>,
    used_symbols: BTreeMap<String, Vec<UsedKind>>,
    externs: BTreeMap<String, Vec<Type>>,
    extern_returns: BTreeMap<String, Option<String>>,
    long_double_shims: BTreeMap<String, ExternFnDecl>,
    uses_long_double: std::cell::Cell<bool>,
    uses_complex: std::cell::Cell<bool>,
    uses_f128: std::cell::Cell<bool>,
    uses_c_variadic: std::cell::Cell<bool>,
    uses_linkage: std::cell::Cell<bool>,
    uses_thread_local: std::cell::Cell<bool>,
    uses_used_with_arg: std::cell::Cell<bool>,
    uses_asm_goto_outputs: std::cell::Cell<bool>,
    uses_breakpoint: std::cell::Cell<bool>,
    uses_memchr: std::cell::Cell<bool>,
    target_arch: Option<String>,
    variadic_defs: BTreeSet<String>,
    c_abi_functions: BTreeSet<String>,
    project: ProjectInfo,
    unsafe_functions: BTreeSet<String>,
    cross_uses: Vec<Item>,
    ctor_calls: Vec<String>,
    dtor_calls: Vec<String>,
    generated_alloca_frames: Vec<StructDef>,
    layout_queries: BTreeMap<String, Vec<LayoutQuery>>,
    macro_consts: BTreeMap<String, Vec<MacroConst>>,
    enum_consts: BTreeMap<String, Vec<EnumConstRef>>,
    asm_gotos: BTreeMap<String, Vec<crate::c_ast::AsmGoto>>,
}

struct FunctionLowerer<'a, 'b> {
    parent: &'a mut Lowerer<'b>,
    values: BTreeMap<String, Val>,
    const_int_values: BTreeMap<String, i128>,
    function_pointer_null_values: BTreeSet<String>,
    slots: BTreeMap<String, String>,
    slot_places: BTreeMap<String, Expr>,
    aligned_slots: BTreeSet<String>,
    slot_types: BTreeMap<String, Type>,
    member_ptrs: BTreeMap<String, MemberPtr>,
    element_ptrs: BTreeMap<String, ElementPtr>,
    block_addr_element_ptrs: BTreeMap<String, BlockAddrElementPtr>,
    indirect_target_values: BTreeMap<String, Expr>,
    temp_counter: usize,
    indent: usize,
    body: Vec<IndentStmt>,
    is_main: bool,
    loop_stack: Vec<LoopFrame>,
    label_counter: usize,
    dispatch: Option<DispatchCtx>,
    hoisted: BTreeSet<String>,
    declared_local_names: BTreeSet<String>,
    forward_allocas: BTreeSet<String>,
    forward_values: BTreeMap<String, Expr>,
    immutable_temps: BTreeSet<String>,
    va_places: BTreeMap<String, String>,
    va_args_param: Option<String>,
    layout_queries: VecDeque<LayoutQuery>,
    macro_consts: VecDeque<MacroConst>,
    enum_consts: VecDeque<EnumConstRef>,
    macro_arith_values: BTreeMap<String, i128>,
    asm_outputs: BTreeMap<String, Vec<Expr>>,
    asm_gotos: VecDeque<crate::c_ast::AsmGoto>,
    asm_output_places: BTreeMap<String, Expr>,
}

struct DispatchCtx {
    loop_label: Label,
    state_var: String,
    label_to_state: BTreeMap<String, usize>,
    block_to_state: BTreeMap<String, usize>,
}

struct LoopFrame {
    break_label: Option<Label>,
    continue_label: Option<Label>,
    is_loop: bool,
}

struct SwitchCase<'a> {
    patterns: Vec<Pattern>,
    is_default: bool,
    region: &'a Region,
}

#[derive(Debug, Clone)]
struct MemberPtr {
    base: Expr,
    field: String,
    field_ty: Option<Type>,
    unsafe_access: bool,
    bitfield_storage: bool,
}

#[derive(Debug, Clone)]
struct ElementPtr {
    base: Expr,
    index: Expr,
    unsafe_access: bool,
    unbounded: bool,
}

#[derive(Debug, Clone)]
struct BlockAddrElementPtr {
    labels: Vec<String>,
    index: Expr,
}

#[derive(Debug, Clone)]
struct GlobalVar {
    name: String,
    ty: Type,
    init: Expr,
    alignment: Option<u32>,
    thread_local: bool,
    external: bool,
    weak: bool,
    section: Option<String>,
    used: Vec<UsedKind>,
}

#[derive(Debug, Clone)]
struct ExternGlobal {
    ty: Type,
    thread_local: bool,
}

#[derive(Debug, Clone)]
enum Val {
    Expr(Expr),
    Global(String),
}

impl Val {
    fn to_expr(&self, strings: &BTreeMap<String, Vec<u8>>) -> Expr {
        match self {
            Val::Expr(e) => e.clone(),
            Val::Global(name) => match strings.get(name) {
                Some(bytes) => Expr::Cast {
                    expr: Box::new(Expr::MethodCall {
                        recv: Box::new(Expr::ByteStr(bytes.clone())),
                        method: "as_ptr".into(),
                        args: Vec::new(),
                    }),
                    ty: Type::parse("*mut libc::c_char"),
                },
                None => Expr::Var(name.clone().into()),
            },
        }
    }
}

impl<'a> Lowerer<'a> {
    fn call_binding(&self, op: &Op, direct: bool) -> CallBinding {
        if !direct {
            return CallBinding::Indirect;
        }
        op.loc
            .as_deref()
            .and_then(|raw| self.resolve_loc(raw))
            .and_then(|loc| self.call_bindings.get(&loc).cloned())
            .unwrap_or_else(|| CallBinding::direct_unknown(None))
    }

    fn resolve_loc(&self, raw: &str) -> Option<Loc> {
        let mut raw = raw.trim();
        for _ in 0..4 {
            let inner = raw.strip_prefix("loc(")?.strip_suffix(')')?.trim();
            if inner.starts_with('#') {
                raw = self.aliases.get(inner)?.as_str();
                continue;
            }
            let (_, line_col) = inner.rsplit_once("\":")?;
            let (line, col) = line_col.split_once(':')?;
            return Some(Loc {
                line: line.parse().ok()?,
                col: col.parse().ok()?,
            });
        }
        None
    }

    fn lower_module(&mut self, module: &Module, c: &Unit) -> Program {
        let mut items = vec![Item::CrateAttrs(vec![CrateAttr::Allow(vec![
            Lint::DeadCode,
            Lint::Unused,
            Lint::NonSnakeCase,
            Lint::NonUpperCaseGlobals,
            Lint::ArithmeticOverflow,
        ])])];

        for enm in &c.enums {
            let name = sanitize_ident(&enm.name).into_string();
            if self.project.shared_enums.contains(&name) {
                continue;
            }
            if let Some(item) = self.lower_enum(enm) {
                items.push(item);
            }
        }
        for record in &c.records {
            let name = sanitize_ident(&record.name).into_string();
            if self.project.shared_records.contains(&name) {
                continue;
            }
            let record = self
                .records
                .get(&name)
                .cloned()
                .unwrap_or_else(|| record.clone());
            items.extend(self.lower_record(&record));
        }
        for record in self.anon_records.clone() {
            let name = sanitize_ident(&record.name).into_string();
            let record = self.records.get(&name).cloned().unwrap_or(record);
            if !record.fields.is_empty() {
                items.extend(self.lower_record(&record));
            }
        }
        items.extend(self.standard_record_defs());

        let Some(module_op) = module.ops.iter().find(|op| op.name == "builtin.module") else {
            self.ctx.diagnostics.error("lower: no builtin.module op");
            return Program {
                items,
                ..Program::default()
            };
        };

        let ops = region_ops(module_op);
        self.weak_aliases = ops
            .iter()
            .filter(|op| op.kind() == CirOpKind::Func && linkage_is_weak(op))
            .filter_map(|op| {
                Some((
                    attr_str(op, "sym_name")?.to_string(),
                    attr_symbol_ref(op, "aliasee")?.to_string(),
                ))
            })
            .collect();
        self.external_weak_targets = self
            .weak_refs
            .values()
            .filter(|target| {
                !ops.iter().any(|op| {
                    op.kind() == CirOpKind::Func
                        && attr_str(op, "sym_name") == Some(target.as_str())
                })
            })
            .cloned()
            .collect();
        self.c_abi_functions = c_abi_function_targets(module_op);
        let mut assembly_strings = Vec::new();
        collect_assembly_strings(module_op, &mut assembly_strings);
        let asm_referenced_globals: BTreeSet<String> = ops
            .iter()
            .filter(|op| op.kind() == CirOpKind::Global)
            .filter_map(|op| attr_str(op, "sym_name"))
            .filter(|name| {
                assembly_strings
                    .iter()
                    .any(|assembly| assembly_mentions_symbol(assembly, name))
            })
            .map(|name| sanitize_ident(name).into_string())
            .collect();
        items.extend(lower_module_asm(module_op, &mut self.ctx.diagnostics));
        items.extend(lower_weak_alias_asm(
            module_op,
            &self.weak_aliases,
            &mut self.ctx.diagnostics,
        ));
        let has_main = ops.iter().any(|op| {
            op.kind() == CirOpKind::Func
                && attr_str(op, "sym_name") == Some("main")
                && !region_ops(op).is_empty()
        });
        let hooks = collect_lifecycle_hooks(&ops, has_main, &mut self.ctx.diagnostics);
        self.ctor_calls = hooks.ctors;
        self.dtor_calls = hooks.dtors;
        self.used_symbols = collect_used_symbols(&ops);
        for op in &ops {
            if op.kind() == CirOpKind::Global {
                self.collect_global(op);
            }
        }
        for global in self.globals.values() {
            let global_external_def = self.project.emit_pub && global.external;
            let global_vis = if global_external_def {
                Visibility::Pub
            } else {
                Visibility::Private
            };
            if global
                .used
                .iter()
                .any(|kind| !matches!(kind, UsedKind::Plain))
            {
                self.uses_used_with_arg.set(true);
            }
            if global.weak {
                self.uses_linkage.set(true);
            }
            let ty = global
                .alignment
                .map(|alignment| aligned_type(global.ty.clone(), alignment))
                .unwrap_or_else(|| global.ty.clone());
            let init = global
                .alignment
                .map(|alignment| aligned_value(global.init.clone(), alignment))
                .unwrap_or_else(|| global.init.clone());
            let mut attrs = symbol_attrs(
                global_external_def || asm_referenced_globals.contains(&global.name),
                global.weak,
                global.section.as_deref(),
                &global.used,
            );
            if global.thread_local {
                attrs.push(RustAttr::ThreadLocal);
                self.uses_thread_local.set(true);
            }
            items.push(Item::Static {
                attrs,
                vis: global_vis,
                mutable: true,
                name: global.name.clone(),
                ty,
                init,
            });
        }

        let mut extern_decls = Vec::new();
        for (name, global) in &self.extern_globals {
            if let Some(module) = self.project.cross_module_globals.get(name) {
                let root = self
                    .project
                    .cross_module_crate
                    .as_deref()
                    .unwrap_or("crate");
                self.cross_uses.push(Item::Use {
                    path: Path::new([
                        Ident::from(root),
                        Ident::from(module.as_str()),
                        Ident::from(name.as_str()),
                    ]),
                });
                continue;
            }
            extern_decls.push(ExternDecl::Static {
                attrs: if global.thread_local {
                    vec![RustAttr::ThreadLocal]
                } else {
                    Vec::new()
                },
                mutable: true,
                name: name.clone(),
                ty: global.ty.clone(),
            });
        }
        let mut emitted_weak_targets = BTreeSet::new();
        for op in &ops {
            if op.kind() != CirOpKind::Func || !region_ops(op).is_empty() {
                continue;
            }
            if attr_symbol_ref(op, "aliasee").is_some() {
                if linkage_is_weak(op) {
                    let Some(name) = attr_str(op, "sym_name") else {
                        continue;
                    };
                    let function_type = attr_str(op, "function_type").unwrap_or("");
                    let (decl, params, ret) = self.extern_fn_signature(name, function_type);
                    if decl.variadic {
                        self.uses_c_variadic.set(true);
                    }
                    self.externs.insert(name.to_string(), params);
                    self.extern_returns.insert(name.to_string(), ret);
                    extern_decls.push(ExternDecl::Fn(decl));
                }
                continue;
            }
            let Some(name) = attr_str(op, "sym_name") else {
                continue;
            };
            if let Some(target) = self.weak_refs.get(name).cloned() {
                if self.external_weak_targets.contains(&target)
                    && emitted_weak_targets.insert(target.clone())
                {
                    let function_type = attr_str(op, "function_type").unwrap_or("");
                    let (decl, params, ret) = self.extern_fn_signature(&target, function_type);
                    if decl.variadic {
                        self.ctx.diagnostics.error(format!(
                            "lower: variadic weakref alias `{name}` to external target `{target}`"
                        ));
                    } else {
                        self.externs.insert(target.clone(), params);
                        self.extern_returns.insert(target.clone(), ret);
                        extern_decls.push(ExternDecl::Static {
                            attrs: vec![RustAttr::ExternWeakLinkage],
                            mutable: false,
                            name: target,
                            ty: Type::FnPtr {
                                abi: Abi::C,
                                params: decl.params.into_iter().map(|param| param.ty).collect(),
                                ret: Box::new(decl.ret.unwrap_or(Type::Unit)),
                            },
                        });
                        self.uses_linkage.set(true);
                    }
                }
                continue;
            }
            if is_complex_runtime_call(name) {
                continue;
            }
            if let Some(module) = self.project.cross_module.get(name) {
                let root = self
                    .project
                    .cross_module_crate
                    .as_deref()
                    .unwrap_or("crate");
                self.cross_uses.push(Item::Use {
                    path: Path::new([
                        Ident::from(root),
                        Ident::from(module.as_str()),
                        Ident::from(name),
                    ]),
                });
                continue;
            }
            let function_type = attr_str(op, "function_type").unwrap_or("");
            let (mut decl, params, ret) = self.extern_fn_signature(name, function_type);
            if attr_bool(op, "noreturn") {
                decl.ret = Some(Type::Never);
            }
            self.externs.insert(name.to_string(), params);
            self.extern_returns.insert(name.to_string(), ret.clone());
            if name == "strtold" && ret.as_deref() == Some(LONG_DOUBLE_TY) {
                self.long_double_shims
                    .entry("__slate_strtold".into())
                    .or_insert_with(|| ExternFnDecl {
                        identity: crate::function_identity::FunctionIdentity::Unknown,
                        name: "__slate_strtold".into(),
                        params: vec![
                            FnParam {
                                name: "_0".into(),
                                mutable: false,
                                ty: Type::Ptr {
                                    mutable: true,
                                    inner: Box::new(Type::Prim(Prim::I8)),
                                },
                            },
                            FnParam {
                                name: "_1".into(),
                                mutable: false,
                                ty: Type::Ptr {
                                    mutable: true,
                                    inner: Box::new(Type::Ptr {
                                        mutable: true,
                                        inner: Box::new(Type::Prim(Prim::I8)),
                                    }),
                                },
                            },
                            FnParam {
                                name: "_2".into(),
                                mutable: false,
                                ty: Type::Ptr {
                                    mutable: true,
                                    inner: Box::new(Type::Prim(Prim::F64)),
                                },
                            },
                        ],
                        variadic: false,
                        ret: None,
                    });
            } else {
                extern_decls.push(ExternDecl::Fn(decl));
            }
        }
        if !extern_decls.is_empty() {
            items.push(Item::ExternBlock {
                abi: "C".into(),
                decls: extern_decls,
            });
        }

        for op in &ops {
            if op.kind() == CirOpKind::Func
                && !region_ops(op).is_empty()
                && attr_str(op, "sym_name").is_some_and(|name| name != "main")
                && function_type_is_variadic(attr_str(op, "function_type").unwrap_or(""))
            {
                self.variadic_defs
                    .insert(attr_str(op, "sym_name").unwrap().to_string());
            }
        }
        self.unsafe_functions
            .extend(unsafe_defined_functions(module));

        for op in &ops {
            if op.kind() != CirOpKind::Func {
                continue;
            }
            let item = if region_ops(op).is_empty() {
                self.lower_func_alias(op, &ops)
            } else {
                self.lower_func(op)
            };
            if let Some(item) = item {
                items.push(item);
            }
        }

        let storage_items: Vec<Item> = self
            .generated_alloca_frames
            .iter()
            .cloned()
            .map(Item::Struct)
            .collect();
        items.splice(1..1, storage_items);

        if !self.long_double_shims.is_empty() {
            items.push(Item::ExternBlock {
                abi: "C".into(),
                decls: self
                    .long_double_shims
                    .values()
                    .cloned()
                    .map(ExternDecl::Fn)
                    .collect(),
            });
        }
        if self.uses_long_double.get() {
            items.splice(1..1, long_double_prelude());
        }
        if self.uses_complex.get() {
            items.splice(1..1, complex_prelude());
        }
        if self.uses_memchr.get() {
            items.splice(1..1, vec![memchr_prelude()]);
        }

        let mut wiring: Vec<Item> = self
            .project
            .child_modules
            .iter()
            .map(|name| Item::Mod {
                name: Ident::from(name.as_str()),
            })
            .collect();
        if let Some(module) = &self.project.shared_type_module {
            let root = self.project.shared_type_crate.as_deref().unwrap_or("crate");
            for enm in &self.project.shared_enums {
                wiring.push(Item::Use {
                    path: Path::new([
                        Ident::from(root),
                        Ident::from(module.as_str()),
                        Ident::from(enm.as_str()),
                    ]),
                });
            }
            for record in &self.project.shared_records {
                wiring.push(Item::Use {
                    path: Path::new([
                        Ident::from(root),
                        Ident::from(module.as_str()),
                        Ident::from(record.as_str()),
                    ]),
                });
            }
        }
        wiring.append(&mut self.cross_uses);
        for (offset, item) in wiring.into_iter().enumerate() {
            items.insert(1 + offset, item);
        }

        if self.uses_c_variadic.get() {
            insert_crate_feature(&mut items, Feature::CVariadic);
        }
        if self.uses_f128.get() {
            insert_crate_feature(&mut items, Feature::F128);
        }
        if self.uses_linkage.get() {
            insert_crate_feature(&mut items, Feature::Linkage);
        }
        if self.uses_thread_local.get() {
            insert_crate_feature(&mut items, Feature::ThreadLocal);
        }
        if self.uses_used_with_arg.get() {
            insert_crate_feature(&mut items, Feature::UsedWithArg);
        }
        if self.uses_asm_goto_outputs.get() {
            insert_crate_feature(&mut items, Feature::AsmGotoWithOutputs);
        }
        if self.uses_breakpoint.get() {
            insert_crate_feature(&mut items, Feature::Breakpoint);
        }
        for feature in &self.project.crate_features {
            insert_crate_feature(&mut items, *feature);
        }

        Program {
            items,
            shims: self.long_double_shims.values().cloned().collect(),
        }
    }

    fn collect_global(&mut self, op: &Op) {
        let Some(name) = attr_str(op, "sym_name") else {
            return;
        };
        if matches!(name, "llvm.compiler.used" | "llvm.used") {
            return;
        }
        if let Some(target) = attr_symbol_ref(op, "aliasee") {
            self.ctx.diagnostics.error(
                format!(
                    "lower: unsupported global alias `{name}` to `{target}`; Rust has no faithful static alias representation"
                ));
            return;
        }
        let rust_name = sanitize_ident(name).into_string();
        let ty = attr_str(op, "sym_type").map(|ty| self.rust_type(ty));
        let alignment = ty.as_ref().and_then(|ty| {
            attr_int(op, "alignment")
                .and_then(|alignment| u32::try_from(alignment).ok())
                .filter(|alignment| {
                    *alignment > type_alignment(ty) && !matches!(ty, Type::Custom(_))
                })
        });
        let weak = linkage_is_weak(op);
        let thread_local = op.attrs.contains_key("tls_model");
        if thread_local {
            self.uses_thread_local.set(true);
        }
        self.warn_protected_visibility(op, name);
        let section = attr_str(op, "section").map(str::to_owned);
        let used = self
            .used_symbols
            .get(&rust_name)
            .cloned()
            .unwrap_or_default();
        let is_c_global = !name.starts_with("__") && !name.starts_with(".str");
        let Some(raw) = attr_str(op, "initial_value") else {
            let Some(ty) = ty else {
                return;
            };
            self.extern_globals
                .insert(rust_name, ExternGlobal { ty, thread_local });
            return;
        };
        if let Some(labels) = parse_cir_block_addr_labels(raw) {
            self.block_addr_globals.insert(name.to_string(), labels);
            if is_c_global && let Some(ty) = ty {
                let init = self.default_value_expr(&ty);
                self.globals.insert(
                    rust_name.clone(),
                    GlobalVar {
                        name: rust_name,
                        ty,
                        init,
                        alignment,
                        thread_local,
                        external: externally_exported(op),
                        weak,
                        section: section.clone(),
                        used: used.clone(),
                    },
                );
            }
        } else if let Some(mut bytes) = parse_cir_const_array(raw) {
            if is_c_global && let Some(ty) = ty {
                let len = type_array_len(&ty)
                    .and_then(|len| usize::try_from(len).ok())
                    .unwrap_or(bytes.len());
                let elems = byte_array_elems(&bytes, &ty);
                self.globals.insert(
                    rust_name.clone(),
                    GlobalVar {
                        name: rust_name,
                        ty,
                        init: render_array_literal_expr(
                            &elems,
                            len,
                            Expr::Value(RustValue::I64(0)),
                        ),
                        alignment,
                        thread_local,
                        external: externally_exported(op),
                        weak,
                        section: section.clone(),
                        used: used.clone(),
                    },
                );
            } else {
                bytes.push(0);
                self.strings.insert(name.to_string(), bytes);
            }
        } else if let Some(elems) = parse_cir_const_array_elems(raw) {
            if is_c_global && let Some(ty) = ty {
                if let Some((_, len)) = parse_rust_array_type(&ty.render()) {
                    self.globals.insert(
                        rust_name.clone(),
                        GlobalVar {
                            name: rust_name,
                            ty,
                            init: render_array_literal_expr(
                                &elems,
                                len as usize,
                                Expr::Value(RustValue::I64(0)),
                            ),
                            alignment,
                            thread_local,
                            external: externally_exported(op),
                            weak,
                            section: section.clone(),
                            used: used.clone(),
                        },
                    );
                }
            } else {
                self.const_arrays.insert(name.to_string(), elems);
            }
        } else if is_cir_aggregate_init(raw) {
            if is_c_global && let Some(ty) = ty {
                if let Some(init) = self.render_const_value_expr(&ty, raw) {
                    self.globals.insert(
                        rust_name.clone(),
                        GlobalVar {
                            name: rust_name,
                            ty,
                            init,
                            alignment,
                            thread_local,
                            external: externally_exported(op),
                            weak,
                            section: section.clone(),
                            used: used.clone(),
                        },
                    );
                }
            } else {
                self.const_aggregates
                    .insert(name.to_string(), raw.to_string());
            }
        } else if raw.trim_start().starts_with("#cir.zero")
            && let Some((elem, len)) = parse_cir_array_type(attr_str(op, "sym_type").unwrap_or(""))
        {
            if is_c_global && let Some(ty) = ty {
                self.globals.insert(
                    rust_name.clone(),
                    GlobalVar {
                        name: rust_name,
                        ty,
                        init: Expr::ArrayRepeat {
                            elem: Box::new(self.default_value_expr(&self.rust_type(&elem))),
                            len: len as usize,
                        },
                        alignment,
                        thread_local,
                        external: externally_exported(op),
                        weak,
                        section: section.clone(),
                        used: used.clone(),
                    },
                );
            } else if elem == "!s8i" && name.starts_with(".str") {
                self.strings.insert(name.to_string(), vec![0; len as usize]);
            } else {
                self.const_arrays.insert(name.to_string(), Vec::new());
            }
        } else if raw.trim_start().starts_with("#cir.zero") {
            if is_c_global && let Some(ty) = ty {
                self.globals.insert(
                    rust_name.clone(),
                    GlobalVar {
                        name: rust_name,
                        init: self.default_value_expr(&ty),
                        ty,
                        alignment,
                        thread_local,
                        external: externally_exported(op),
                        weak,
                        section: section.clone(),
                        used: used.clone(),
                    },
                );
            } else {
                self.const_zero_globals.insert(name.to_string());
            }
        } else if let Some(target) = parse_cir_global_view(raw) {
            if is_c_global
                && let Some(ty) = ty
                && let Some(init) = self.global_view_init_expr(target, &ty)
            {
                self.globals.insert(
                    rust_name.clone(),
                    GlobalVar {
                        name: rust_name,
                        ty,
                        init,
                        alignment,
                        thread_local,
                        external: externally_exported(op),
                        weak,
                        section: section.clone(),
                        used: used.clone(),
                    },
                );
            }
        } else if let Some(ty) = ty
            && let Some(init) = self.render_const_value_expr(&ty, raw)
        {
            let external = externally_exported(op);
            self.globals.insert(
                rust_name.clone(),
                GlobalVar {
                    name: rust_name,
                    ty,
                    init,
                    alignment,
                    thread_local,
                    external,
                    weak,
                    section,
                    used,
                },
            );
        }
    }

    fn lower_func_alias(&mut self, op: &Op, ops: &[&Op]) -> Option<Item> {
        let name = attr_str(op, "sym_name")?;
        let target = attr_symbol_ref(op, "aliasee")?;
        if linkage_is_weak(op) {
            return None;
        }
        let target_op = ops.iter().find(|candidate| {
            candidate.kind() == CirOpKind::Func
                && attr_str(candidate, "sym_name") == Some(target)
                && !region_ops(candidate).is_empty()
        });
        if target_op.is_none() {
            self.ctx.diagnostics.error(format!(
                "lower: unsupported function alias `{name}` to external target `{target}`"
            ));
            return None;
        }

        let function_type = attr_str(op, "function_type").unwrap_or("");
        let (decl, _, _) = self.extern_fn_signature(name, function_type);
        if decl.variadic {
            self.ctx.diagnostics.error(format!(
                "lower: unsupported variadic function alias `{name}` to `{target}`"
            ));
            return None;
        }

        let external_def = self.project.emit_pub && externally_exported(op);
        self.warn_protected_visibility(op, name);
        let attrs = symbol_attrs(
            external_def,
            linkage_is_weak(op),
            attr_str(op, "section"),
            &[],
        );
        if linkage_is_weak(op) {
            self.uses_linkage.set(true);
        }
        let args = decl
            .params
            .iter()
            .map(|param| Expr::Var(param.name.clone().into()))
            .collect();
        let mut call = Expr::Call {
            binding: crate::function_identity::CallBinding::Generated,
            func: Box::new(Expr::Var(target.into())),
            args,
        };
        let unsafe_ =
            self.unsafe_functions.contains(name) || self.unsafe_functions.contains(target);
        if unsafe_ {
            call = FunctionLowerer::unsafe_expr(call);
        }
        let stmt = if decl.ret.is_some() {
            Stmt::Return(Some(call))
        } else {
            Stmt::Expr(call)
        };

        Some(Item::Fn(FnDef {
            attrs,
            vis: if external_def {
                Visibility::Pub
            } else {
                Visibility::Private
            },
            unsafe_,
            abi: if external_def || self.c_abi_functions.contains(name) {
                Some(Abi::C)
            } else {
                None
            },
            name: name.to_string(),
            params: decl.params,
            ret: decl.ret,
            body: vec![IndentStmt { depth: 1, stmt }],
        }))
    }

    fn lower_enum(&mut self, enm: &crate::c_ast::Enum) -> Option<Item> {
        lower_enum_def(enm, Visibility::Private).map(Item::Enum)
    }

    fn warn_protected_visibility(&mut self, op: &Op, name: &str) {
        if visibility_is_protected(op) {
            self.ctx.diagnostics.warn(
                format!(
                    "lower: protected visibility on `{name}` has no faithful Rust representation; falling back to default exported visibility"
                ));
        }
    }

    fn lower_record(&mut self, record: &crate::c_ast::Record) -> Vec<Item> {
        let storage_record;
        let record = if let Some(fields) = self.bitfield_storage_fields(record) {
            storage_record = crate::c_ast::Record {
                fields,
                ..record.clone()
            };
            &storage_record
        } else {
            record
        };
        lower_record_def(record, Visibility::Private, Visibility::Private, true)
    }

    fn bitfield_storage_fields(
        &self,
        record: &crate::c_ast::Record,
    ) -> Option<Vec<crate::c_ast::Decl>> {
        if record.fields.is_empty() || record.fields.iter().all(|field| field.bit_width.is_none()) {
            return None;
        }
        let expanded = self.aliases.values().find(|ty| {
            cir_record_name(ty).is_some_and(|name| {
                sanitize_ident(name).as_str() == sanitize_ident(&record.name).as_str()
            })
        })?;
        let open = expanded.find('{')?;
        let close = expanded.rfind('}')?;
        let preserve_field_names = record
            .fields
            .iter()
            .any(|field| field.name.starts_with("__bitfield_"));
        Some(
            split_top_level(&expanded[open + 1..close], ',')
                .into_iter()
                .map(str::trim)
                .filter(|ty| !ty.is_empty())
                .enumerate()
                .map(|(index, ty)| crate::c_ast::Decl {
                    name: if preserve_field_names {
                        record
                            .fields
                            .get(index)
                            .map(|field| field.name.clone())
                            .unwrap_or_else(|| format!("__bitfield_{index}"))
                    } else {
                        format!("__bitfield_{index}")
                    },
                    comments: Vec::new(),
                    ty: cir_type_to_ctype(ty, &self.aliases),
                    bit_width: None,
                })
                .collect(),
        )
    }

    fn standard_record_defs(&self) -> Vec<Item> {
        let mut out = Vec::new();
        for name in ["div_t", "ldiv_t", "lldiv_t", "imaxdiv_t", "tm", "lconv"] {
            if self.records.contains_key(name) {
                continue;
            }
            if self
                .aliases
                .values()
                .any(|ty| cir_record_name(ty) == Some(name))
            {
                out.push(Item::Record(standard_record_def(name)));
            }
        }
        out
    }

    fn lower_func(&mut self, op: &Op) -> Option<Item> {
        let name = attr_str(op, "sym_name")?;
        let weak_alias_target = self.weak_aliases.values().any(|target| target == name);
        let function_type = attr_str(op, "function_type").unwrap_or("");
        let (param_types, ret_ty) = parse_function_type(function_type);
        let entry = op.regions.first()?.blocks.first()?;
        let is_main = name == "main";
        let is_variadic = !is_main && function_type_is_variadic(function_type);

        let mut params = entry
            .args
            .iter()
            .enumerate()
            .map(|(i, (arg, ty))| {
                let ty = param_types.get(i).map(String::as_str).unwrap_or(ty);
                FnParam {
                    name: arg.clone(),
                    mutable: false,
                    ty: self.rust_type(ty),
                }
            })
            .collect::<Vec<_>>();

        let va_args_param = if is_variadic {
            let param = "__slate_va_args".to_string();
            params.push(FnParam {
                name: param.clone(),
                mutable: true,
                ty: Type::Variadic,
            });
            Some(param)
        } else {
            None
        };

        let (vis, abi, ret, prelude) = if is_main {
            params.clear();
            let mut prelude = self.main_arg_bindings(entry);
            prelude.extend(
                self.ctor_calls
                    .iter()
                    .map(|name| hook_call_stmt(name, &self.unsafe_functions)),
            );
            (Visibility::Private, None, None, prelude)
        } else {
            let external_def =
                self.project.emit_pub && externally_exported(op) || weak_alias_target;
            let vis = if external_def {
                Visibility::Pub
            } else {
                Visibility::Private
            };
            let abi = if external_def || is_variadic || self.c_abi_functions.contains(name) {
                Some(Abi::C)
            } else {
                None
            };
            if is_variadic {
                self.uses_c_variadic.set(true);
                self.variadic_defs.insert(name.to_string());
            }
            let ret = Some(self.rust_type(ret_ty.as_deref().unwrap_or("()")));
            (vis, abi, ret, Vec::<Stmt>::new())
        };

        let diverges = !is_main
            && attr_bool(op, "noreturn")
            && op.regions.first().is_some_and(region_ends_in_noreturn_call);
        if !is_main && attr_bool(op, "noreturn") && !diverges {
            self.ctx.diagnostics.warn(
                format!(
                    "lower: __attribute__((noreturn)) on `{name}` does not structurally prove divergence; keeping its declared return type"
                ));
        }
        let ret = if diverges { Some(Type::Never) } else { ret };

        let attrs = symbol_attrs(
            !is_main && (self.project.emit_pub && externally_exported(op) || weak_alias_target),
            linkage_is_weak(op),
            attr_str(op, "section"),
            &[],
        );
        self.warn_protected_visibility(op, name);
        if linkage_is_weak(op) {
            self.uses_linkage.set(true);
        }
        let unsafe_ = is_variadic
            || self.unsafe_functions.contains(name) && !self.c_abi_functions.contains(name);
        let layout_queries: VecDeque<_> = self
            .layout_queries
            .get(name)
            .cloned()
            .unwrap_or_default()
            .into();
        let macro_consts: VecDeque<_> = self
            .macro_consts
            .get(name)
            .cloned()
            .unwrap_or_default()
            .into();
        let enum_consts: VecDeque<_> = self
            .enum_consts
            .get(name)
            .cloned()
            .unwrap_or_default()
            .into();
        let asm_gotos: VecDeque<_> = self.asm_gotos.get(name).cloned().unwrap_or_default().into();
        let mut f = FunctionLowerer {
            parent: self,
            values: BTreeMap::new(),
            const_int_values: BTreeMap::new(),
            function_pointer_null_values: BTreeSet::new(),
            slots: BTreeMap::new(),
            slot_places: BTreeMap::new(),
            aligned_slots: BTreeSet::new(),
            slot_types: BTreeMap::new(),
            member_ptrs: BTreeMap::new(),
            element_ptrs: BTreeMap::new(),
            block_addr_element_ptrs: BTreeMap::new(),
            indirect_target_values: BTreeMap::new(),
            temp_counter: 0,
            indent: 1,
            body: Vec::new(),
            is_main,
            loop_stack: Vec::new(),
            label_counter: 0,
            dispatch: None,
            hoisted: BTreeSet::new(),
            declared_local_names: BTreeSet::new(),
            forward_allocas: forwardable_temp_allocas(op.regions.first()?),
            forward_values: BTreeMap::new(),
            immutable_temps: BTreeSet::new(),
            va_places: BTreeMap::new(),
            va_args_param,
            layout_queries,
            macro_consts,
            enum_consts,
            macro_arith_values: BTreeMap::new(),
            asm_outputs: BTreeMap::new(),
            asm_gotos,
            asm_output_places: BTreeMap::new(),
        };

        for stmt in prelude {
            f.push_stmt(stmt);
        }
        for (arg, _) in &entry.args {
            f.immutable_temps.insert(arg.clone());
            f.values
                .insert(arg.clone(), Val::Expr(Expr::Var(arg.clone().into())));
        }
        let body = op.regions.first().unwrap();
        if body.blocks.len() > 1 {
            let returns_value = !matches!(ret, None | Some(Type::Unit));
            f.lower_dispatch(body, returns_value);
        } else {
            f.lower_block(entry);
        }
        if diverges && matches!(f.body.last().map(|s| &s.stmt), Some(Stmt::Return(None))) {
            f.body.pop();
        }
        Some(Item::Fn(FnDef {
            attrs,
            vis,
            unsafe_,
            abi,
            name: name.to_string(),
            params,
            ret,
            body: f.body,
        }))
    }

    fn main_arg_bindings(&self, entry: &Block) -> Vec<Stmt> {
        if entry.args.is_empty() {
            return Vec::new();
        }

        let call = |path: &str, args: Vec<Expr>| Expr::Call {
            binding: crate::function_identity::CallBinding::Generated,
            func: Box::new(Expr::Var(path.into())),
            args,
        };
        let method = |recv: Expr, name: &str, args: Vec<Expr>| Expr::MethodCall {
            recv: Box::new(recv),
            method: name.into(),
            args,
        };
        let char_ptr = Type::Ptr {
            mutable: true,
            inner: Box::new(Type::Prim(Prim::I8)),
        };

        let storage_init = method(
            method(
                call("std::env::args", vec![]),
                "map",
                vec![Expr::Closure {
                    params: vec!["arg".into()],
                    body: Box::new(method(
                        call("std::ffi::CString::new", vec![Expr::Var("arg".into())]),
                        "unwrap",
                        vec![],
                    )),
                }],
            ),
            "collect",
            vec![],
        );
        let ptrs_init = method(
            method(
                method(Expr::Var("__slate_argv_storage".into()), "iter", vec![]),
                "map",
                vec![Expr::Closure {
                    params: vec!["arg".into()],
                    body: Box::new(Expr::Cast {
                        expr: Box::new(method(Expr::Var("arg".into()), "as_ptr", vec![])),
                        ty: char_ptr.clone(),
                    }),
                }],
            ),
            "collect",
            vec![],
        );

        let mut stmts = vec![
            Stmt::Let {
                name: "__slate_argv_storage".into(),
                mutable: true,
                ty: Some(Type::Generic {
                    name: "Vec".into(),
                    args: vec![Type::Custom("std::ffi::CString".into())],
                }),
                init: Some(storage_init),
            },
            Stmt::Let {
                name: "__slate_argv_ptrs".into(),
                mutable: true,
                ty: Some(Type::Generic {
                    name: "Vec".into(),
                    args: vec![char_ptr],
                }),
                init: Some(ptrs_init),
            },
            Stmt::Expr(method(
                Expr::Var("__slate_argv_ptrs".into()),
                "push",
                vec![Expr::Value(RustValue::NullPtr)],
            )),
        ];

        for (i, (arg, ty)) in entry.args.iter().enumerate() {
            let value = match i {
                0 => Expr::Cast {
                    expr: Box::new(method(
                        Expr::Var("__slate_argv_storage".into()),
                        "len",
                        vec![],
                    )),
                    ty: Type::Prim(Prim::I32),
                },
                1 => method(Expr::Var("__slate_argv_ptrs".into()), "as_mut_ptr", vec![]),
                _ => Expr::Value(RustValue::NullPtr),
            };
            stmts.push(Stmt::Let {
                name: sanitize_ident(arg).into_string(),
                mutable: false,
                ty: Some(self.rust_type(ty)),
                init: Some(value),
            });
        }
        stmts
    }

    /// Build a Rust `extern "C"` signature for a body-less C declaration,
    /// returning `(line, fixed_param_rust_types, return_type)`. Trailing `...` becomes a Rust
    /// variadic; a missing return arrow means the C function returns `void`.
    fn extern_fn_signature(
        &self,
        name: &str,
        function_type: &str,
    ) -> (ExternFnDecl, Vec<Type>, Option<String>) {
        let inner = function_type
            .strip_prefix("!cir.func<")
            .and_then(|s| s.strip_suffix('>'))
            .unwrap_or("");
        let (params_str, ret) = match split_top_level_arrow(inner) {
            Some((params, ret)) => (params.trim(), Some(ret.trim())),
            None => (inner.trim(), None),
        };
        let params_str = params_str.trim_start_matches('(').trim_end_matches(')');

        let mut params = Vec::new();
        let mut param_types = Vec::new();
        let mut variadic = false;
        for (i, raw) in split_top_level(params_str, ',')
            .into_iter()
            .map(str::trim)
            .filter(|s| !s.is_empty())
            .enumerate()
        {
            if raw == "..." {
                variadic = true;
            } else {
                let ty = self.rust_type(raw);
                params.push(FnParam {
                    name: format!("_{i}"),
                    mutable: false,
                    ty: ty.clone(),
                });
                param_types.push(ty);
            }
        }
        let ret_ast = match ret {
            Some(ret) if ret != "()" => Some(self.rust_type(ret)),
            _ => None,
        };
        let ret_ty = ret_ast.as_ref().map(Type::render);
        let decl = ExternFnDecl {
            name: name.into(),
            identity: self
                .known_functions
                .get(canonical_c23_libc_symbol(name))
                .copied()
                .or_else(|| c23_redirected_function(name))
                .unwrap_or(FunctionIdentity::Unknown),
            params,
            variadic,
            ret: ret_ast,
        };
        (decl, param_types, ret_ty)
    }

    fn rust_type(&self, cir_ty: &str) -> Type {
        let ty = rust_type_with_aliases(cir_ty, &self.aliases);
        if type_mentions_long_double(&ty) {
            self.uses_long_double.set(true);
        }
        if type_mentions_complex(&ty) {
            self.uses_complex.set(true);
        }
        if type_mentions_f128(&ty) {
            self.uses_f128.set(true);
        }
        ty
    }

    fn cir_type_is_union(&self, ty: &str) -> bool {
        let ty = self.expand_alias(ty);
        if ty.starts_with("!cir.union<") {
            return true;
        }
        cir_record_name(ty)
            .and_then(|name| self.records.get(sanitize_ident(name).as_str()))
            .is_some_and(|record| record.kind == RecordKind::Union)
    }

    fn expand_alias<'b>(&'b self, ty: &'b str) -> &'b str {
        let mut ty = ty.trim();
        let mut seen = BTreeSet::new();
        while let Some(expanded) = self.aliases.get(ty) {
            if !seen.insert(ty.to_string()) {
                break;
            }
            ty = expanded.trim();
        }
        ty
    }

    fn record_field_type(&self, ty: &crate::c_ast::CType) -> Type {
        if ctype_uses_long_double(ty) {
            self.uses_long_double.set(true);
        }
        c_record_field_type(ty)
    }

    fn default_value_expr(&self, ty: &Type) -> Expr {
        match ty {
            Type::Custom(name) => {
                if let Some(enm) = self.enums.get(name)
                    && let Some(variant) = enm.variants.first()
                {
                    return Expr::Path(Path::new([
                        Ident::from(name.as_str()),
                        Ident::from(sanitize_ident(&variant.name).as_str()),
                    ]));
                }
                if let Some(record) = self.records.get(name) {
                    if let Some(fields) = self.bitfield_storage_fields(record) {
                        return Expr::StructLit {
                            name: record_lit_name(record),
                            fields: fields
                                .iter()
                                .map(|field| {
                                    (
                                        field.name.clone(),
                                        self.default_value_expr(&c_record_field_type(&field.ty)),
                                    )
                                })
                                .collect(),
                        };
                    }
                    match record.kind {
                        RecordKind::Struct => {
                            let fields = record
                                .fields
                                .iter()
                                .map(|field| {
                                    (
                                        sanitize_ident(&field.name).into_string(),
                                        self.default_value_expr(&c_record_field_type(&field.ty)),
                                    )
                                })
                                .collect();
                            return wrap_record_lit(
                                record,
                                Expr::StructLit {
                                    name: record_lit_name(record),
                                    fields,
                                },
                            );
                        }
                        RecordKind::Union => {
                            if let Some(field) = record.fields.first() {
                                return wrap_record_lit(
                                    record,
                                    Expr::StructLit {
                                        name: record_lit_name(record),
                                        fields: vec![(
                                            sanitize_ident(&field.name).into_string(),
                                            self.default_value_expr(&c_record_field_type(
                                                &field.ty,
                                            )),
                                        )],
                                    },
                                );
                            }
                        }
                    }
                }
                standard_record_default_expr(name).unwrap_or_else(|| default_value_for_type(ty))
            }
            Type::LongDouble => Expr::Call {
                binding: crate::function_identity::CallBinding::Generated,
                func: Box::new(Expr::Var(LONG_DOUBLE_TY.into())),
                args: vec![Expr::Value(RustValue::Float(0.0))],
            },
            Type::Complex(inner) => {
                let d = default_value_for_type(inner);
                Expr::StructLit {
                    name: "Complex".into(),
                    fields: vec![("re".into(), d.clone()), ("im".into(), d)],
                }
            }
            Type::Array { elem, len } => Expr::ArrayRepeat {
                elem: Box::new(self.default_value_expr(elem)),
                len: *len as usize,
            },
            Type::CLib(CLibType::Timespec) => Expr::StructLit {
                name: "libc::timespec".into(),
                fields: vec![
                    ("tv_sec".into(), Expr::Value(RustValue::I64(0))),
                    ("tv_nsec".into(), Expr::Value(RustValue::I64(0))),
                ],
            },
            Type::CLib(CLibType::MbState) => Expr::Unsafe(Box::new(crate::rust_ast::Block {
                stmts: Vec::new(),
                tail: Some(Box::new(Expr::Call {
                    binding: crate::function_identity::CallBinding::Generated,
                    func: Box::new(Expr::Var("std::mem::zeroed::<libc::mbstate_t>".into())),
                    args: Vec::new(),
                })),
            })),
            _ => default_value_for_type(ty),
        }
    }

    fn type_is_enum(&self, ty: &Type) -> bool {
        matches!(ty, Type::Custom(name) if self.enums.contains_key(name))
    }

    fn type_is_enum_ptr(&self, ty: &Type) -> bool {
        matches!(ty, Type::Ptr { inner, .. } if self.type_is_enum(inner))
    }

    fn layout_query_value(&self, query: &LayoutQuery) -> Option<i128> {
        match query {
            LayoutQuery::Size(ty) => c_layout(ty, &self.records).map(|layout| layout.size),
            LayoutQuery::Align(ty) => c_layout(ty, &self.records).map(|layout| layout.align),
            LayoutQuery::Offset { record, field } => {
                record_field_offset(record, field, &self.records)
            }
        }
        .map(i128::from)
    }

    fn layout_query_expr(&self, query: &LayoutQuery) -> Option<Expr> {
        match query {
            LayoutQuery::Size(ty @ crate::c_ast::CType::Record(_)) => {
                Some(layout_call("size_of", &c_type_to_type(ty)))
            }
            LayoutQuery::Align(ty @ crate::c_ast::CType::Record(_)) => {
                Some(layout_call("align_of", &c_type_to_type(ty)))
            }
            LayoutQuery::Size(_) | LayoutQuery::Align(_) => None,
            LayoutQuery::Offset { record, field } => {
                let source_record = self.records.get(&sanitize_ident(record).into_string())?;
                if source_record.packed.is_some() && source_record.align.is_some() {
                    return None;
                }
                let record = sanitize_ident(record).into_string();
                Some(Expr::Macro {
                    name: "std::mem::offset_of".into(),
                    args: vec![Expr::Var(record.into()), Expr::Var(sanitize_ident(field))],
                })
            }
        }
    }

    fn render_const_value_expr(&self, ty: &Type, raw: &str) -> Option<Expr> {
        let raw = raw.trim();
        if let Some((re, im)) = parse_cir_const_complex(raw) {
            Some(complex_const_expr(Some(ty), re, im))
        } else if raw.starts_with("#cir.const_record<") {
            let Type::Custom(name) = ty else {
                return None;
            };
            let record = self.records.get(name)?;
            let open = raw.find('{')?;
            let close = raw.rfind('}')?;
            let elems = split_top_level(&raw[open + 1..close], ',');
            match record.kind {
                RecordKind::Struct => {
                    if let Some(storage_fields) = self.bitfield_storage_fields(record) {
                        let fields = storage_fields
                            .iter()
                            .enumerate()
                            .map(|(i, field)| {
                                let field_ty = c_record_field_type(&field.ty);
                                let value = elems
                                    .get(i)
                                    .and_then(|e| self.render_const_value_expr(&field_ty, e.trim()))
                                    .unwrap_or_else(|| self.default_value_expr(&field_ty));
                                (field.name.clone(), value)
                            })
                            .collect();
                        return Some(Expr::StructLit {
                            name: record_lit_name(record),
                            fields,
                        });
                    }
                    let fields = record
                        .fields
                        .iter()
                        .enumerate()
                        .map(|(i, field)| {
                            let field_ty = c_record_field_type(&field.ty);
                            let value = elems
                                .get(i)
                                .and_then(|e| self.render_const_value_expr(&field_ty, e.trim()))
                                .unwrap_or_else(|| self.default_value_expr(&field_ty));
                            (sanitize_ident(&field.name).into_string(), value)
                        })
                        .collect();
                    Some(wrap_record_lit(
                        record,
                        Expr::StructLit {
                            name: record_lit_name(record),
                            fields,
                        },
                    ))
                }
                RecordKind::Union => {
                    let field = record.fields.first()?;
                    let field_ty = c_record_field_type(&field.ty);
                    let value = elems
                        .first()
                        .and_then(|e| self.render_const_value_expr(&field_ty, e.trim()))
                        .unwrap_or_else(|| self.default_value_expr(&field_ty));
                    Some(wrap_record_lit(
                        record,
                        Expr::StructLit {
                            name: record_lit_name(record),
                            fields: vec![(sanitize_ident(&field.name).into_string(), value)],
                        },
                    ))
                }
            }
        } else if raw.starts_with("#cir.const_array<[") {
            let Type::Array { elem, len } = ty else {
                return None;
            };
            let len = *len as usize;
            let open = raw.find('[')?;
            let close = raw.rfind(']')?;
            let mut out: Vec<Expr> = split_top_level(&raw[open + 1..close], ',')
                .into_iter()
                .map(|e| e.trim().to_string())
                .filter(|e| !e.is_empty())
                .take(len)
                .map(|e| {
                    self.render_const_value_expr(elem, &e)
                        .unwrap_or_else(|| self.default_value_expr(elem))
                })
                .collect();
            out.resize(len, self.default_value_expr(elem));
            Some(Expr::ArrayLit(out))
        } else if raw.starts_with("#cir.zero") {
            Some(self.default_value_expr(ty))
        } else if let Type::Custom(name) = ty
            && let Some(enm) = self.enums.get(name)
            && let Some(value) = parse_cir_int(raw)
            && let Some(variant) = enm
                .variants
                .iter()
                .find(|variant| i128::from(variant.value) == value)
        {
            Some(Expr::Path(Path::new([
                Ident::from(name.as_str()),
                Ident::from(sanitize_ident(&variant.name).as_str()),
            ])))
        } else if let Some(target) = parse_cir_global_view(raw) {
            self.global_view_init_expr(target, ty)
        } else {
            parse_cir_scalar_expr(raw)
        }
    }

    fn global_view_init_expr(&self, target: &str, ty: &Type) -> Option<Expr> {
        if let Some(bytes) = self.strings.get(target) {
            return Some(Expr::Cast {
                expr: Box::new(Expr::MethodCall {
                    recv: Box::new(Expr::ByteStr(bytes.clone())),
                    method: "as_ptr".into(),
                    args: Vec::new(),
                }),
                ty: ty.clone(),
            });
        }
        let Type::Ptr { mutable, .. } = ty else {
            return None;
        };
        Some(Expr::Cast {
            expr: Box::new(Expr::AddrOf {
                mutable: *mutable,
                expr: Box::new(Expr::Var(sanitize_ident(target).into_string().into())),
            }),
            ty: ty.clone(),
        })
    }
}

fn clib_record_type(name: &str) -> Option<CLibType> {
    Some(match name {
        "FILE" => CLibType::File,
        "mbstate_t" => CLibType::MbState,
        "pthread_attr_t" => CLibType::PthreadAttr,
        "timespec" => CLibType::Timespec,
        _ => return None,
    })
}

fn c_type_to_type(ty: &crate::c_ast::CType) -> Type {
    use crate::c_ast::CType;
    let ptr = |inner: &CType| Type::Ptr {
        mutable: true,
        inner: Box::new(c_type_to_type(inner)),
    };
    match ty {
        CType::Void => Type::Unit,
        CType::Bool => Type::Prim(Prim::Bool),
        CType::Int { signed, bits } => Type::Prim(match (signed, bits) {
            (true, 8) => Prim::I8,
            (false, 8) => Prim::U8,
            (true, 16) => Prim::I16,
            (false, 16) => Prim::U16,
            (false, 32) => Prim::U32,
            (true, 64) => Prim::I64,
            (false, 64) => Prim::U64,
            (true, 128) => Prim::I128,
            (false, 128) => Prim::U128,
            _ => Prim::I32,
        }),
        CType::Float { bits: 32 } => Type::Prim(Prim::F32),
        CType::Float { bits: 80 } => Type::LongDouble,
        CType::Float { bits: 128 } => Type::Prim(Prim::F128),
        CType::Float { .. } => Type::Prim(Prim::F64),
        CType::Ptr(inner) if matches!(&**inner, CType::Void) => Type::Ptr {
            mutable: true,
            inner: Box::new(Type::CLib(CLibType::Void)),
        },
        CType::Ptr(inner) => ptr(inner),
        CType::FuncPtr { ret, params } => Type::FnPtr {
            abi: Abi::C,
            params: params.iter().map(c_type_to_type).collect(),
            ret: Box::new(c_type_to_type(ret)),
        },
        CType::Array(inner, Some(len)) => Type::Array {
            elem: Box::new(c_type_to_type(inner)),
            len: *len,
        },
        CType::Array(inner, None) => ptr(inner),
        CType::Record(name) => clib_record_type(name)
            .map(Type::CLib)
            .unwrap_or_else(|| Type::Custom(sanitize_ident(name).into_string())),
        CType::Enum(name) => Type::Custom(sanitize_ident(name).into_string()),
    }
}

fn c_record_field_type(ty: &crate::c_ast::CType) -> Type {
    match ty {
        crate::c_ast::CType::Array(inner, None) => Type::Array {
            elem: Box::new(c_type_to_type(inner)),
            len: 0,
        },
        _ => c_type_to_type(ty),
    }
}

#[derive(Clone, Copy)]
struct CLayout {
    size: u64,
    align: u64,
}

fn layout_call(name: &str, ty: &Type) -> Expr {
    Expr::Call {
        binding: crate::function_identity::CallBinding::Generated,
        func: Box::new(Expr::Var(
            format!("std::mem::{name}::<{}>", ty.render()).into(),
        )),
        args: Vec::new(),
    }
}

fn c_layout(
    ty: &crate::c_ast::CType,
    records: &BTreeMap<String, crate::c_ast::Record>,
) -> Option<CLayout> {
    use crate::c_ast::CType;
    match ty {
        CType::Void => Some(CLayout { size: 0, align: 1 }),
        CType::Bool => Some(CLayout { size: 1, align: 1 }),
        CType::Int { bits, .. } => scalar_layout(*bits),
        CType::Float { bits: 80 } => Some(CLayout {
            size: 16,
            align: 16,
        }),
        CType::Float { bits } => scalar_layout(*bits),
        CType::Ptr(_) | CType::FuncPtr { .. } => Some(CLayout { size: 8, align: 8 }),
        CType::Array(elem, Some(len)) => {
            let elem = c_layout(elem, records)?;
            Some(CLayout {
                size: align_to(elem.size, elem.align) * len,
                align: elem.align,
            })
        }
        CType::Array(elem, None) => {
            let elem = c_layout(elem, records)?;
            Some(CLayout {
                size: 0,
                align: elem.align,
            })
        }
        CType::Record(name) => record_layout(name, records),
        CType::Enum(_) => scalar_layout(32),
    }
}

fn scalar_layout(bits: u32) -> Option<CLayout> {
    let bytes = u64::from(bits).checked_div(8)?;
    let bytes = bytes.max(1);
    Some(CLayout {
        size: bytes,
        align: bytes,
    })
}

fn record_layout(name: &str, records: &BTreeMap<String, crate::c_ast::Record>) -> Option<CLayout> {
    let record = records.get(&sanitize_ident(name).into_string())?;
    let natural_align = record_natural_align(record, records)?;
    let align = record
        .align
        .map(u64::from)
        .unwrap_or(natural_align)
        .max(natural_align);
    match record.kind {
        RecordKind::Struct => {
            let mut offset = 0;
            for field in &record.fields {
                let field_layout = c_layout(&field.ty, records)?;
                let field_align = record.packed.map_or(field_layout.align, |packed| {
                    field_layout.align.min(u64::from(packed))
                });
                offset = align_to(offset, field_align);
                offset += field_layout.size;
            }
            Some(CLayout {
                size: align_to(offset, align),
                align,
            })
        }
        RecordKind::Union => {
            let size = record
                .fields
                .iter()
                .filter_map(|field| c_layout(&field.ty, records).map(|layout| layout.size))
                .max()
                .unwrap_or(0);
            Some(CLayout {
                size: align_to(size, align),
                align,
            })
        }
    }
}

fn record_natural_align(
    record: &crate::c_ast::Record,
    records: &BTreeMap<String, crate::c_ast::Record>,
) -> Option<u64> {
    record
        .fields
        .iter()
        .map(|field| {
            c_layout(&field.ty, records).map(|layout| {
                record
                    .packed
                    .map_or(layout.align, |packed| layout.align.min(u64::from(packed)))
            })
        })
        .max()
        .unwrap_or(Some(1))
}

fn record_field_offset(
    record_name: &str,
    field_name: &str,
    records: &BTreeMap<String, crate::c_ast::Record>,
) -> Option<u64> {
    let record = records.get(&sanitize_ident(record_name).into_string())?;
    if record.kind != RecordKind::Struct {
        return None;
    }
    let mut offset = 0;
    for field in &record.fields {
        let field_layout = c_layout(&field.ty, records)?;
        let field_align = record.packed.map_or(field_layout.align, |packed| {
            field_layout.align.min(u64::from(packed))
        });
        offset = align_to(offset, field_align);
        if field.name == field_name {
            return Some(offset);
        }
        offset += field_layout.size;
    }
    None
}

fn align_to(value: u64, align: u64) -> u64 {
    if align <= 1 {
        value
    } else {
        value.div_ceil(align) * align
    }
}

fn ctype_uses_long_double(ty: &crate::c_ast::CType) -> bool {
    use crate::c_ast::CType;
    match ty {
        CType::Float { bits: 80 } => true,
        CType::Ptr(inner) | CType::Array(inner, _) => ctype_uses_long_double(inner),
        CType::FuncPtr { ret, params } => {
            ctype_uses_long_double(ret) || params.iter().any(ctype_uses_long_double)
        }
        CType::Enum(_) => false,
        _ => false,
    }
}

impl<'a, 'b> FunctionLowerer<'a, 'b> {
    fn lower_block(&mut self, block: &Block) {
        let mut index = 0;
        while index < block.ops.len() {
            let op = &block.ops[index];
            if op.kind() == CirOpKind::Alloca {
                let end = block.ops[index..]
                    .iter()
                    .take_while(|candidate| {
                        candidate.kind() == CirOpKind::Alloca && candidate.loc == op.loc
                    })
                    .count()
                    + index;
                if end - index > 1 && self.alloca_group_is_lowerable(&block.ops[index..end]) {
                    self.lower_alloca_group(&block.ops[index..end]);
                    index = end;
                    continue;
                }
            }
            self.asm_output_places.clear();
            if op.kind() == CirOpKind::Asm
                && attr_str(op, "asm_string").is_some_and(asm_template_has_labels)
            {
                for result in &op.results {
                    if let Some(store) = block.ops[index + 1..].iter().find(|candidate| {
                        candidate.kind() == CirOpKind::Store
                            && candidate.operands.first() == Some(result)
                    }) && let Some(pointer) = store.operands.get(1)
                        && let Some(place) = self.place_expr(pointer)
                    {
                        self.asm_output_places.insert(result.clone(), place);
                    }
                }
            }
            self.lower_op(op);
            index += 1;
        }
    }

    fn lower_region_ops(&mut self, region: &Region) {
        for block in &region.blocks {
            self.lower_block(block);
        }
    }

    fn lower_op(&mut self, op: &Op) {
        match op.kind() {
            CirOpKind::Alloca => self.lower_alloca(op),
            CirOpKind::Store => self.lower_store(op),
            CirOpKind::Copy => self.lower_copy(op),
            CirOpKind::Load => self.lower_load(op),
            CirOpKind::Const => self.lower_const(op),
            CirOpKind::AddOverflow => self.lower_overflow_arith(op, "overflowing_add"),
            CirOpKind::SubOverflow => self.lower_overflow_arith(op, "overflowing_sub"),
            CirOpKind::MulOverflow => self.lower_overflow_arith(op, "overflowing_mul"),
            CirOpKind::DivOverflow => self.lower_overflow_arith(op, "overflowing_div"),
            CirOpKind::RemOverflow => self.lower_overflow_arith(op, "overflowing_rem"),
            CirOpKind::Add => self.lower_int_arith(op, BinOp::Add),
            CirOpKind::Sub => self.lower_int_arith(op, BinOp::Sub),
            CirOpKind::Mul => self.lower_int_arith(op, BinOp::Mul),
            CirOpKind::Div => self.lower_int_arith(op, BinOp::Div),
            CirOpKind::Rem => self.lower_int_arith(op, BinOp::Rem),
            CirOpKind::And => self.lower_int_arith(op, BinOp::BitAnd),
            CirOpKind::Asm => self.lower_asm(op),
            CirOpKind::Or => self.lower_int_arith(op, BinOp::BitOr),
            CirOpKind::Xor => self.lower_int_arith(op, BinOp::BitXor),
            CirOpKind::Bitreverse => self.lower_unary_method(op, "reverse_bits"),
            CirOpKind::BlockAddress => self.lower_opaque_pointer(op, true),
            CirOpKind::ByteSwap => self.lower_unary_method(op, "swap_bytes"),
            CirOpKind::Clrsb => self.lower_clrsb(op),
            CirOpKind::Clz => self.lower_unary_method(op, "leading_zeros"),
            CirOpKind::Ctz => self.lower_unary_method(op, "trailing_zeros"),
            CirOpKind::Ffs => self.lower_ffs(op),
            CirOpKind::IsConstant => self.lower_is_constant(op),
            CirOpKind::Objsize => self.lower_objsize(op),
            CirOpKind::Parity => self.lower_parity(op),
            CirOpKind::Popcount => self.lower_unary_method(op, "count_ones"),
            CirOpKind::Rotate => self.lower_rotate(op),
            CirOpKind::Shift => self.lower_shift(op),
            CirOpKind::Not => self.lower_not(op),
            CirOpKind::Minus | CirOpKind::Fneg => self.lower_neg(op),
            CirOpKind::Abs => self.lower_abs(op),
            CirOpKind::Acos => self.lower_unary_method(op, "acos"),
            CirOpKind::Asin => self.lower_unary_method(op, "asin"),
            CirOpKind::Atan => self.lower_unary_method(op, "atan"),
            CirOpKind::Atan2 => self.lower_binary_method(op, "atan2"),
            CirOpKind::Assume => self.lower_assume(op),
            CirOpKind::Ceil => self.lower_unary_method(op, "ceil"),
            CirOpKind::ClearCache => {}
            CirOpKind::Copysign => self.lower_binary_method(op, "copysign"),
            CirOpKind::Cos => {
                self.lower_known_unary_method(op, crate::function_identity::Known::Cos, "cos")
            }
            CirOpKind::Exp => {
                self.lower_known_unary_method(op, crate::function_identity::Known::Exp, "exp")
            }
            CirOpKind::Exp2 => {
                self.lower_known_unary_method(op, crate::function_identity::Known::Exp2, "exp2")
            }
            CirOpKind::Expect => self.lower_expect(op),
            CirOpKind::Fabs => self.lower_unary_method(op, "abs"),
            CirOpKind::Fma => self.lower_ternary_method(op, "mul_add"),
            CirOpKind::Fmaximum => self.lower_binary_method(op, "max"),
            CirOpKind::Fminimum => self.lower_binary_method(op, "min"),
            CirOpKind::Fmod => {
                self.lower_known_binary(op, crate::function_identity::Known::Fmod, BinOp::Rem)
            }
            CirOpKind::Floor => self.lower_unary_method(op, "floor"),
            CirOpKind::Fmaxnum => self.lower_binary_method(op, "max"),
            CirOpKind::Fminnum => self.lower_binary_method(op, "min"),
            CirOpKind::IsFpClass => self.lower_is_fp_class(op),
            CirOpKind::Llrint => self.lower_unary_cast_method(op, "round_ties_even"),
            CirOpKind::Llround => self.lower_known_unary_cast_method(
                op,
                crate::function_identity::Known::Llround,
                "round",
            ),
            CirOpKind::Log => {
                self.lower_known_unary_method(op, crate::function_identity::Known::Log, "ln")
            }
            CirOpKind::Log10 => {
                self.lower_known_unary_method(op, crate::function_identity::Known::Log10, "log10")
            }
            CirOpKind::Log2 => {
                self.lower_known_unary_method(op, crate::function_identity::Known::Log2, "log2")
            }
            CirOpKind::Lrint => self.lower_unary_cast_method(op, "round_ties_even"),
            CirOpKind::Lround => self.lower_known_unary_cast_method(
                op,
                crate::function_identity::Known::Lround,
                "round",
            ),
            CirOpKind::Modf => self.lower_modf(op),
            CirOpKind::Nearbyint => self.lower_unary_method(op, "round_ties_even"),
            CirOpKind::Pow => {
                self.lower_known_binary_method(op, crate::function_identity::Known::Pow, "powf")
            }
            CirOpKind::Prefetch => {}
            CirOpKind::Rint => self.lower_unary_method(op, "round_ties_even"),
            CirOpKind::Round => self.lower_unary_method(op, "round"),
            CirOpKind::Roundeven => self.lower_unary_method(op, "round_ties_even"),
            CirOpKind::Signbit => self.lower_signbit(op),
            CirOpKind::Sin => {
                self.lower_known_unary_method(op, crate::function_identity::Known::Sin, "sin")
            }
            CirOpKind::Sqrt => {
                self.lower_known_unary_method(op, crate::function_identity::Known::Sqrt, "sqrt")
            }
            CirOpKind::FrameAddress => self.lower_opaque_pointer(op, true),
            CirOpKind::ReturnAddress => self.lower_opaque_pointer(op, true),
            CirOpKind::Stacksave => self.lower_opaque_pointer(op, false),
            CirOpKind::Stackrestore => {}
            CirOpKind::Tan => {
                self.lower_known_unary_method(op, crate::function_identity::Known::Tan, "tan")
            }
            CirOpKind::Trap => self.lower_trap(),
            CirOpKind::Trunc => self.lower_unary_method(op, "trunc"),
            CirOpKind::Unreachable => self.lower_unreachable(),
            CirOpKind::Fadd => self.lower_binary(op, BinOp::Add),
            CirOpKind::Fsub => self.lower_binary(op, BinOp::Sub),
            CirOpKind::Fmul => self.lower_binary(op, BinOp::Mul),
            CirOpKind::Fdiv => self.lower_binary(op, BinOp::Div),
            CirOpKind::ComplexAdd => self.lower_binary(op, BinOp::Add),
            CirOpKind::ComplexSub => self.lower_binary(op, BinOp::Sub),
            CirOpKind::ComplexMul => self.lower_complex_mul(op),
            CirOpKind::ComplexDiv => self.lower_complex_div(op),
            CirOpKind::ComplexConj => self.lower_complex_conj(op),
            CirOpKind::ComplexCreate => self.lower_complex_create(op),
            CirOpKind::ComplexReal => self.lower_complex_part(op, "re"),
            CirOpKind::ComplexImag => self.lower_complex_part(op, "im"),
            CirOpKind::ComplexRealPtr => self.lower_complex_part_ptr(op, "re"),
            CirOpKind::ComplexImagPtr => self.lower_complex_part_ptr(op, "im"),
            CirOpKind::Inc => self.lower_step(op, BinOp::Add),
            CirOpKind::Dec => self.lower_step(op, BinOp::Sub),
            CirOpKind::Cmp => self.lower_cmp(op),
            CirOpKind::Select => self.lower_select(op),
            CirOpKind::Ternary => self.lower_ternary(op),
            CirOpKind::GetGlobal => self.lower_get_global(op),
            CirOpKind::GetMember => self.lower_get_member(op),
            CirOpKind::ExtractMember => self.lower_extract_member(op),
            CirOpKind::InsertMember => self.lower_insert_member(op),
            CirOpKind::GetBitfield => self.lower_get_bitfield(op),
            CirOpKind::SetBitfield => self.lower_set_bitfield(op),
            CirOpKind::GetElement => self.lower_get_element(op),
            CirOpKind::Cast => self.lower_cast(op),
            CirOpKind::PtrStride => self.lower_ptr_stride(op),
            CirOpKind::PtrDiff => self.lower_ptr_diff(op),
            CirOpKind::Call => self.lower_call(op),
            CirOpKind::LibcMemcpy => self.lower_mem_copy(op, false),
            CirOpKind::LibcMemmove => self.lower_mem_copy(op, true),
            CirOpKind::LibcMemset => self.lower_mem_set(op),
            CirOpKind::LibcMemchr => self.lower_mem_chr(op),
            CirOpKind::VaStart => self.lower_va_start(op),
            CirOpKind::VaArg => self.lower_va_arg(op),
            // `VaList` drops on scope exit.
            CirOpKind::VaEnd => {}
            CirOpKind::AtomicFetch => self.lower_atomic_fetch(op),
            CirOpKind::AtomicXchg => self.lower_atomic_xchg(op),
            CirOpKind::AtomicCmpxchg => self.lower_atomic_cmpxchg(op),
            CirOpKind::AtomicTestAndSet => self.lower_atomic_test_and_set(op),
            CirOpKind::AtomicClear => self.lower_atomic_clear(op),
            CirOpKind::AtomicFence => self.lower_atomic_fence(op),
            CirOpKind::Return => self.lower_return(op),
            CirOpKind::Scope => self.lower_scope(op),
            CirOpKind::CleanupScope => self.lower_cleanup_scope(op),
            CirOpKind::If => self.lower_if(op),
            CirOpKind::Switch => self.lower_switch(op),
            CirOpKind::For => self.lower_for(op),
            CirOpKind::While => self.lower_while(op),
            CirOpKind::Do => self.lower_do(op),
            CirOpKind::Break => self.lower_break(),
            CirOpKind::Continue => self.lower_continue(),
            CirOpKind::Goto => self.lower_goto(op),
            CirOpKind::Br => self.lower_br(op),
            CirOpKind::IndirectBr => self.lower_indirect_br(op),
            CirOpKind::VecExtract => self.lower_vec_extract(op),
            CirOpKind::VecInsert => self.lower_vec_insert(op),
            CirOpKind::VecShuffle => self.lower_vec_shuffle(op),
            CirOpKind::EhSetjmp => self.lower_eh_setjmp(op),
            CirOpKind::CallLlvmIntrinsic => self.lower_llvm_intrinsic(op),
            CirOpKind::Label => {}
            CirOpKind::Yield | CirOpKind::Condition => {}
            _ => {
                let name = op.name.as_str();
                self.parent
                    .ctx
                    .diagnostics
                    .warn(format!("lower: unsupported CIR op {name}"));
                self.emit_todo(name);
            }
        }
    }

    fn unique_local_name(&mut self, base: String) -> String {
        if !self.parent.globals.contains_key(&base)
            && self.declared_local_names.insert(base.clone())
        {
            return base;
        }
        let mut n = 2;
        loop {
            let candidate = format!("{base}{n}");
            if !self.parent.globals.contains_key(&candidate)
                && self.declared_local_names.insert(candidate.clone())
            {
                return candidate;
            }
            n += 1;
        }
    }

    fn alloca_group_is_lowerable(&self, ops: &[Op]) -> bool {
        ops.iter().all(|op| {
            op.operands.is_empty()
                && op
                    .results
                    .first()
                    .is_some_and(|result| !self.forward_allocas.contains(result))
                && !op
                    .ty
                    .as_deref()
                    .is_some_and(|ty| ty.contains("__va_list_tag"))
                && !matches!(
                    self.pointee_type(op.ty.as_deref().unwrap_or("")),
                    Some(Type::Custom(_))
                )
        })
    }

    fn lower_alloca_group(&mut self, ops: &[Op]) {
        let frame_index = self.parent.generated_alloca_frames.len();
        let frame_name = format!("__SlateAllocaFrame{frame_index}");
        let frame_var = self.unique_local_name(format!("__slate_alloca_frame{frame_index}"));
        let mut fields = Vec::with_capacity(ops.len());
        let mut init = Vec::with_capacity(ops.len());
        let mut places = Vec::with_capacity(ops.len());

        for (field_index, op) in ops.iter().rev().enumerate() {
            let Some(result) = op.results.first() else {
                return;
            };
            let ty = self
                .pointee_type(op.ty.as_deref().unwrap_or(""))
                .unwrap_or(Type::Prim(Prim::I32));
            let alignment = attr_int(op, "alignment")
                .and_then(|alignment| u32::try_from(alignment).ok())
                .unwrap_or_else(|| type_alignment(&ty));
            let over_aligned = alignment > type_alignment(&ty) && !matches!(ty, Type::Custom(_));
            if over_aligned {
                fields.push(aligned_type(ty.clone(), alignment));
                init.push(aligned_value(self.default_value_expr(&ty), alignment));
            } else {
                fields.push(ty.clone());
                init.push(self.default_value_expr(&ty));
            }
            let field = Expr::TupleField {
                base: Box::new(Expr::Var(frame_var.clone().into())),
                index: field_index,
            };
            places.push((
                result.clone(),
                ty,
                if over_aligned {
                    Expr::Unary {
                        op: UnaryOp::Deref,
                        expr: Box::new(field),
                    }
                } else {
                    field
                },
            ));
            if over_aligned {
                self.aligned_slots.insert(result.clone());
            }
        }

        self.parent.generated_alloca_frames.push(StructDef {
            attrs: vec![RustAttr::Repr(vec![Repr::C])],
            vis: Visibility::Private,
            generics: Vec::new(),
            name: frame_name.clone(),
            fields: StructFields::Tuple(fields),
        });
        self.push_stmt(Stmt::Let {
            name: frame_var,
            mutable: true,
            ty: Some(Type::Custom(frame_name)),
            init: Some(Expr::TupleStructLit {
                name: self
                    .parent
                    .generated_alloca_frames
                    .last()
                    .expect("generated alloca frame")
                    .name
                    .clone(),
                fields: init,
            }),
        });
        for (result, ty, place) in places {
            self.slot_types.insert(result.clone(), ty);
            self.slot_places.insert(result, place);
        }
    }

    fn lower_alloca(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        // a forwarded compiler temp carries one SSA value: its single store
        // records the value and its single load reads it back, so no local.
        if self.forward_allocas.contains(result) {
            if let Some(ty) = self.pointee_type(op.ty.as_deref().unwrap_or("")) {
                self.slot_types.insert(result.clone(), ty);
            }
            return;
        }
        // hoisted allocas were already declared above the dispatch loop.
        if self.hoisted.contains(result) && self.dispatch.is_some() {
            return;
        }
        let name = self.unique_local_name(
            sanitize_ident(attr_str(op, "name").unwrap_or(result)).into_string(),
        );
        if let Some(count) = op.operands.first() {
            let ty = self
                .pointee_type(op.ty.as_deref().unwrap_or(""))
                .unwrap_or(Type::Prim(Prim::I32));
            self.values.insert(
                result.clone(),
                Val::Expr(Expr::MethodCall {
                    recv: Box::new(Expr::Var(name.clone().into())),
                    method: "as_mut_ptr".into(),
                    args: Vec::new(),
                }),
            );
            self.push_stmt(Stmt::Let {
                name,
                mutable: true,
                ty: Some(Type::Generic {
                    name: "Vec".into(),
                    args: vec![ty.clone()],
                }),
                init: Some(Expr::VecRepeat {
                    elem: Box::new(self.parent.default_value_expr(&ty)),
                    len: Box::new(Expr::Cast {
                        expr: Box::new(self.operand_expr(count)),
                        ty: Type::Prim(Prim::Usize),
                    }),
                }),
            });
            return;
        }
        // a `va_list` local becomes a Rust `VaList`, assigned by `va_start`.
        if op
            .ty
            .as_deref()
            .is_some_and(|ty| ty.contains("__va_list_tag"))
        {
            self.slots.insert(result.clone(), name.clone());
            self.va_places.insert(result.clone(), name.clone());
            self.push_stmt(Stmt::Let {
                name,
                mutable: true,
                ty: Some(Type::VaList),
                init: None,
            });
            return;
        }
        let ty = self
            .pointee_type(op.ty.as_deref().unwrap_or(""))
            .unwrap_or(Type::Prim(Prim::I32));
        let alignment = attr_int(op, "alignment")
            .and_then(|alignment| u32::try_from(alignment).ok())
            .filter(|alignment| *alignment > type_alignment(&ty) && !matches!(ty, Type::Custom(_)));
        self.slots.insert(result.clone(), name.clone());
        self.slot_types.insert(result.clone(), ty.clone());
        let init = self.default_value_expr(&ty);
        if let Some(alignment) = alignment {
            self.aligned_slots.insert(result.clone());
            self.slot_places.insert(
                result.clone(),
                Expr::Unary {
                    op: UnaryOp::Deref,
                    expr: Box::new(Expr::Var(name.clone().into())),
                },
            );
            self.push_stmt(Stmt::Let {
                name,
                mutable: true,
                ty: Some(aligned_type(ty, alignment)),
                init: Some(aligned_value(init, alignment)),
            });
            return;
        }
        self.push_stmt(Stmt::Let {
            name,
            mutable: true,
            ty: Some(ty),
            init: Some(init),
        });
    }

    fn lower_store(&mut self, op: &Op) {
        if op.operands.len() < 2 {
            return;
        }
        if let Some(outputs) = self.asm_outputs.get(&op.operands[0]).cloned() {
            self.asm_outputs.insert(op.operands[1].clone(), outputs);
            return;
        }
        let operand_types = op_operand_types(op.ty.as_deref().unwrap_or(""));
        let value_ty = operand_types.first().copied();
        let mut value = if value_ty.is_some_and(is_cir_function_pointer_type) {
            self.function_pointer_operand_expr(&op.operands[0])
        } else if value_ty.is_some_and(|ty| ty.starts_with("!cir.ptr<")) {
            self.pointer_operand_expr(&op.operands[0])
        } else {
            self.operand_expr(&op.operands[0])
        };
        let ptr = &op.operands[1];
        value = self.coerce_store_value(ptr, value, &op.operands[0]);
        if self.forward_allocas.contains(ptr) {
            let value = self.forward_safe_value(value, value_ty);
            self.forward_values.insert(ptr.clone(), value);
            return;
        }
        if !attr_bool(op, "is_volatile") && self.try_atomic_store(op, ptr, value_ty, value.clone())
        {
            return;
        }
        if attr_bool(op, "is_volatile") {
            self.push_stmt(Stmt::Expr(Self::unsafe_expr(Expr::Call {
                binding: crate::function_identity::CallBinding::Generated,
                func: Box::new(Expr::Path(Path::new(
                    ["std", "ptr", "write_volatile"].map(Ident::from),
                ))),
                args: vec![self.store_address_expr(ptr), value],
            })));
        } else if let Some(target) = self.place_expr(ptr) {
            if self.ptr_requires_unsafe(ptr) {
                self.push_unsafe_assign(target, value);
            } else {
                self.push_assign(target, value);
            }
        } else {
            self.push_unsafe_assign(
                Expr::Unary {
                    op: UnaryOp::Deref,
                    expr: Box::new(self.pointer_operand_expr(ptr)),
                },
                value,
            );
        }
    }

    fn lower_copy(&mut self, op: &Op) {
        if op.operands.len() < 2 {
            return;
        }
        let dst = op.operands[0].clone();
        let src = op.operands[1].clone();
        let Some(value) = self.copy_source_value(&dst, &src) else {
            // opaque aggregate copy: fall back to a one-element memcpy.
            let d = self.pointer_operand_expr(&dst);
            let s = self.pointer_operand_expr(&src);
            self.push_stmt(Stmt::Expr(Expr::CopyNonoverlapping {
                src: Box::new(s),
                dst: Box::new(d),
                count: 1,
            }));
            return;
        };
        if let Some(target) = self.place_expr(&dst) {
            if self.ptr_requires_unsafe(&dst) {
                self.push_unsafe_assign(target, value);
            } else {
                self.push_assign(target, value);
            }
        } else {
            self.push_unsafe_assign(
                Expr::Unary {
                    op: UnaryOp::Deref,
                    expr: Box::new(self.pointer_operand_expr(&dst)),
                },
                value,
            );
        }
    }

    /// Resolve the by-value source of a `cir.copy`: a numeric/char const global
    /// renders to an array literal (padded to the destination length), while an
    /// aggregate local relies on the `Copy` derive of arrays and `#[repr(C)]`
    /// structs. Returns `None` when the source is opaque (raw pointer copy).
    fn copy_source_value(&self, dst: &str, src: &str) -> Option<Expr> {
        let dst_len = self
            .slot_types
            .get(dst)
            .and_then(type_array_len)
            .map(|len| len as usize);
        match self.values.get(src) {
            Some(Val::Global(name)) => {
                if let Some(bytes) = self.parent.strings.get(name) {
                    let ty = self.slot_types.get(dst)?;
                    let elems = byte_array_elems(bytes, ty);
                    Some(render_array_literal_expr(
                        &elems,
                        dst_len.unwrap_or(elems.len()),
                        Expr::Value(RustValue::I64(0)),
                    ))
                } else if let Some(elems) = self.parent.const_arrays.get(name) {
                    Some(render_array_literal_expr(
                        elems,
                        dst_len.unwrap_or(elems.len()),
                        Expr::Value(RustValue::I64(0)),
                    ))
                } else if let Some(raw) = self.parent.const_aggregates.get(name) {
                    let ty = self.slot_types.get(dst)?;
                    self.render_const_value_expr(ty, raw)
                } else if self.parent.const_zero_globals.contains(name) {
                    self.slot_types
                        .get(dst)
                        .map(|ty| self.default_value_expr(ty))
                } else {
                    None
                }
            }
            _ => self.slot_place(src),
        }
    }

    fn render_const_value_expr(&self, ty: &Type, raw: &str) -> Option<Expr> {
        self.parent.render_const_value_expr(ty, raw)
    }

    fn lower_load(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        let Some(ptr) = op.operands.first() else {
            return;
        };
        if let Some(value) = self.forward_values.get(ptr) {
            self.values.insert(result.clone(), Val::Expr(value.clone()));
            return;
        }
        if let Some(expr) = self.block_addr_dispatch_expr(ptr) {
            self.indirect_target_values.insert(result.clone(), expr);
            self.lower_opaque_pointer(op, true);
            return;
        }
        let mut value = if attr_bool(op, "is_volatile") {
            Self::unsafe_expr(Expr::Call {
                binding: crate::function_identity::CallBinding::Generated,
                func: Box::new(Expr::Path(Path::new(
                    ["std", "ptr", "read_volatile"].map(Ident::from),
                ))),
                args: vec![self.load_address_expr(ptr)],
            })
        } else if let Some(atomic) = self.atomic_load_expr(op, ptr) {
            atomic
        } else if let Some(global) = self.global_place(ptr) {
            Self::unsafe_expr(global)
        } else if let Some(member) = self.member_ptrs.get(ptr) {
            let place = Expr::Field {
                base: Box::new(member.base.clone()),
                field: member.field.clone(),
            };
            if member.unsafe_access {
                Self::unsafe_expr(place)
            } else {
                place
            }
        } else if let Some(element) = self.element_ptrs.get(ptr) {
            let place = self.element_place_expr(element);
            if element.unsafe_access {
                Self::unsafe_expr(place)
            } else {
                place
            }
        } else if let Some(slot) = self.slot_place(ptr) {
            slot
        } else {
            Self::unsafe_deref_expr(self.operand_expr(ptr))
        };
        if let Some(member_ty) = self
            .member_ptrs
            .get(ptr)
            .and_then(|member| member.field_ty.as_ref())
            && let Some(result_ty) = op_result_type(op).map(|ty| self.parent.rust_type(ty))
            && ((self.parent.type_is_enum(member_ty) && matches!(result_ty, Type::Prim(_)))
                || (self.parent.type_is_enum_ptr(member_ty)
                    && matches!(result_ty, Type::Ptr { .. })))
        {
            value = Expr::Cast {
                expr: Box::new(value),
                ty: result_ty,
            };
        }
        self.materialize_expr(result, value, op_result_type(op));
    }

    fn block_addr_dispatch_expr(&self, ptr: &str) -> Option<Expr> {
        let element = self.block_addr_element_ptrs.get(ptr)?;
        let dispatch = self.dispatch.as_ref()?;
        let states: Option<Vec<Expr>> = element
            .labels
            .iter()
            .map(|label| {
                dispatch
                    .label_to_state
                    .get(label)
                    .map(|state| Expr::Value(RustValue::I64(*state as i64)))
            })
            .collect();
        Some(Expr::Index {
            base: Box::new(Expr::ArrayLit(states?)),
            index: Box::new(Expr::Cast {
                expr: Box::new(element.index.clone()),
                ty: Type::Prim(Prim::Usize),
            }),
        })
    }

    fn load_address_expr(&self, ptr: &str) -> Expr {
        self.address_expr(ptr, false)
    }

    fn store_address_expr(&self, ptr: &str) -> Expr {
        self.address_expr(ptr, true)
    }

    fn address_expr(&self, ptr: &str, mutable: bool) -> Expr {
        let addr_of = |expr: Expr| Expr::AddrOf {
            mutable,
            expr: Box::new(expr),
        };
        if let Some(member) = self.member_ptrs.get(ptr) {
            let place = addr_of(Expr::Field {
                base: Box::new(member.base.clone()),
                field: member.field.clone(),
            });
            if member.unsafe_access {
                Self::unsafe_expr(place)
            } else {
                place
            }
        } else if let Some(element) = self.element_ptrs.get(ptr) {
            let place = addr_of(self.element_place_expr(element));
            if element.unsafe_access {
                Self::unsafe_expr(place)
            } else {
                place
            }
        } else if let Some(slot) = self.slot_place(ptr) {
            addr_of(slot)
        } else if let Some(global) = self.global_place(ptr) {
            addr_of(global)
        } else {
            self.operand_expr(ptr)
        }
    }

    fn global_name(&self, ptr: &str) -> Option<String> {
        let Some(Val::Global(name)) = self.values.get(ptr) else {
            return None;
        };
        let name = sanitize_ident(name).into_string();
        (self.parent.globals.contains_key(&name) || self.parent.extern_globals.contains_key(&name))
            .then_some(name)
    }

    fn global_place(&self, ptr: &str) -> Option<Expr> {
        let name = self.global_name(ptr)?;
        let base = Expr::Var(name.clone().into());
        self.parent
            .globals
            .get(&name)
            .and_then(|global| global.alignment)
            .map(|_| Expr::Unary {
                op: UnaryOp::Deref,
                expr: Box::new(base.clone()),
            })
            .or(Some(base))
    }

    fn lower_const(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        let raw = attr_str(op, "value").unwrap_or("");
        // MLIR may print a const as an attribute alias (e.g. `#false`); expand it.
        let raw = self
            .parent
            .aliases
            .get(raw)
            .cloned()
            .unwrap_or_else(|| raw.to_string());
        let raw = raw.as_str();
        if let Some(value) = parse_cir_int(raw) {
            self.const_int_values.insert(result.clone(), value);
        }
        let result_ty = op_result_type(op);
        if let Some(value) = parse_cir_int(raw)
            && let Some(expr) = self.next_layout_query_expr(value, result_ty)
        {
            self.materialize_expr(result, expr, result_ty);
            return;
        }
        if let Some(value) = parse_cir_int(raw)
            && let Some(expr) = self.next_macro_const_expr(value, result_ty)
        {
            self.materialize_expr(result, expr, result_ty);
            return;
        }
        if let Some(value) = parse_cir_int(raw)
            && let Some(expr) = self.next_enum_const_expr(op, value, result_ty)
        {
            self.materialize_expr(result, expr, result_ty);
            return;
        }
        if let Some(value) = parse_cir_const_vector(raw) {
            self.materialize_expr(result, value, result_ty);
            return;
        }
        if let Some((re, im)) = parse_cir_const_complex(raw) {
            let rust_ty = result_ty.map(|ty| self.parent.rust_type(ty));
            self.materialize_expr(
                result,
                complex_const_expr(rust_ty.as_ref(), re, im),
                result_ty,
            );
            return;
        }
        if let Some(target) = parse_cir_global_view(raw)
            && let Some(result_ty) = result_ty
            && let Some(value) = self
                .parent
                .global_view_init_expr(target, &self.parent.rust_type(result_ty))
        {
            self.materialize_expr(result, value, Some(result_ty));
            return;
        }
        if let Some(b) = parse_cir_bool(raw) {
            self.materialize_expr(result, Expr::Value(RustValue::Bool(b)), result_ty);
            return;
        }
        if raw.starts_with("#cir.ptr<null>") {
            let value = if result_ty.is_some_and(is_cir_function_pointer_type) {
                self.function_pointer_null_values.insert(result.clone());
                Expr::Value(RustValue::None)
            } else if result_ty.is_some_and(|ty| self.parent.expand_alias(ty) == "!cir.ptr<!void>")
            {
                Expr::Cast {
                    expr: Box::new(Expr::Value(RustValue::NullPtr)),
                    ty: self.parent.rust_type(result_ty.unwrap()),
                }
            } else {
                Expr::Value(RustValue::NullPtr)
            };
            self.materialize_expr(result, value, result_ty);
            return;
        }
        let value = if result_ty == Some("!cir.f128") {
            parse_cir_f128_expr(raw).unwrap_or_else(|| Expr::HexFloat("0.0f128".into()))
        } else if result_ty.is_some_and(is_long_double) {
            self.next_long_double_macro_const_expr(op)
                .unwrap_or_else(|| {
                    let value = parse_cir_fp_expr(raw)
                        .or_else(|| parse_cir_int(raw).map(int_value_expr))
                        .unwrap_or(Expr::Value(RustValue::Float(0.0)));
                    Expr::Call {
                        binding: crate::function_identity::CallBinding::Generated,
                        func: Box::new(Expr::Var(LONG_DOUBLE_TY.into())),
                        args: vec![value],
                    }
                })
        } else {
            parse_cir_scalar_expr(raw).unwrap_or(Expr::Value(RustValue::I64(0)))
        };
        self.materialize_expr(result, value, result_ty);
    }

    fn next_layout_query_expr(&mut self, value: i128, result_ty: Option<&str>) -> Option<Expr> {
        let query = self.layout_queries.front()?;
        let expected = self.parent.layout_query_value(query)?;
        if expected != value {
            return None;
        }
        let expr = self.parent.layout_query_expr(query);
        self.layout_queries.pop_front();
        let mut expr = expr?;
        if let Some(result_ty) = result_ty {
            expr = Expr::Cast {
                expr: Box::new(expr),
                ty: self.parent.rust_type(result_ty),
            };
        }
        Some(expr)
    }

    fn next_macro_const_expr(&mut self, value: i128, result_ty: Option<&str>) -> Option<Expr> {
        let macro_const = self.macro_consts.front()?;
        let known = crate::macros::lookup(&macro_const.name)?;
        let crate::macros::MacroValue::Integer { source, rust_path } = known.value else {
            return None;
        };
        if source != value {
            return None;
        }
        self.macro_consts.pop_front();
        let mut expr = Expr::Var(rust_path.into());
        if let Some(result_ty) = result_ty {
            expr = Expr::Cast {
                expr: Box::new(expr),
                ty: self.parent.rust_type(result_ty),
            };
        }
        Some(expr)
    }

    fn next_enum_const_expr(
        &mut self,
        op: &Op,
        value: i128,
        result_ty: Option<&str>,
    ) -> Option<Expr> {
        let enum_const = self.enum_consts.front()?;
        if enum_const.value as i128 != value {
            return None;
        }
        if op
            .loc
            .as_deref()
            .and_then(|raw| self.parent.resolve_loc(raw))
            != Some(enum_const.loc)
        {
            return None;
        }
        let enum_const = self.enum_consts.pop_front()?;
        let mut expr = Expr::Path(Path::new([
            sanitize_ident(&enum_const.enum_name),
            sanitize_ident(&enum_const.constant_name),
        ]));
        if let Some(result_ty) = result_ty {
            expr = Expr::Cast {
                expr: Box::new(expr),
                ty: self.parent.rust_type(result_ty),
            };
        }
        Some(expr)
    }

    fn next_long_double_macro_const_expr(&mut self, op: &Op) -> Option<Expr> {
        let macro_const = self.macro_consts.front()?;
        let known = crate::macros::lookup(&macro_const.name)?;
        let crate::macros::MacroValue::LongDouble { rust_bits, .. } = known.value else {
            return None;
        };
        if op
            .loc
            .as_deref()
            .and_then(|raw| self.parent.resolve_loc(raw))
            != Some(macro_const.loc)
        {
            return None;
        }
        self.macro_consts.pop_front();
        Some(Expr::Call {
            binding: crate::function_identity::CallBinding::Generated,
            func: Box::new(Expr::Var(LONG_DOUBLE_TY.into())),
            args: vec![Expr::Call {
                binding: crate::function_identity::CallBinding::Generated,
                func: Box::new(Expr::Var("f64::from_bits".into())),
                args: vec![Expr::Value(RustValue::I64(rust_bits as i64))],
            }],
        })
    }

    fn lower_complex_create(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        if op.operands.len() < 2 {
            return;
        }
        self.materialize_expr(
            result,
            Expr::StructLit {
                name: "Complex".into(),
                fields: vec![
                    ("re".into(), self.operand_expr(&op.operands[0])),
                    ("im".into(), self.operand_expr(&op.operands[1])),
                ],
            },
            op_result_type(op),
        );
    }

    fn lower_complex_part(&mut self, op: &Op, field: &str) {
        let Some(result) = op.results.first() else {
            return;
        };
        let Some(src) = op.operands.first() else {
            return;
        };
        self.materialize_expr(
            result,
            Expr::Field {
                base: Box::new(self.operand_expr(src)),
                field: field.into(),
            },
            op_result_type(op),
        );
    }

    fn lower_complex_part_ptr(&mut self, op: &Op, field: &str) {
        let Some(result) = op.results.first() else {
            return;
        };
        let Some(base_ptr) = op.operands.first() else {
            return;
        };
        let base = self.place_or_deref_expr(base_ptr);
        let unsafe_access =
            self.place_expr(base_ptr).is_none() || self.ptr_requires_unsafe(base_ptr);
        self.member_ptrs.insert(
            result.clone(),
            MemberPtr {
                base,
                field: field.into(),
                field_ty: op_result_type(op)
                    .and_then(cir_ptr_pointee)
                    .map(|ty| self.parent.rust_type(ty)),
                unsafe_access,
                bitfield_storage: false,
            },
        );
    }

    fn lower_complex_mul(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        if op.operands.len() < 2 {
            return;
        }
        if let Some(call) = self.complex_runtime_binop(op, "__mulsc3", "__muldc3") {
            self.materialize_expr(result, call, op_result_type(op));
            return;
        }
        let lhs = self.operand_expr(&op.operands[0]);
        let rhs = self.operand_expr(&op.operands[1]);
        let field = |base: Expr, name: &str| Expr::Field {
            base: Box::new(base),
            field: name.into(),
        };
        let ac = Expr::Binary {
            op: BinOp::Mul,
            lhs: Box::new(field(lhs.clone(), "re")),
            rhs: Box::new(field(rhs.clone(), "re")),
        };
        let bd = Expr::Binary {
            op: BinOp::Mul,
            lhs: Box::new(field(lhs.clone(), "im")),
            rhs: Box::new(field(rhs.clone(), "im")),
        };
        let ad = Expr::Binary {
            op: BinOp::Mul,
            lhs: Box::new(field(lhs.clone(), "re")),
            rhs: Box::new(field(rhs.clone(), "im")),
        };
        let bc = Expr::Binary {
            op: BinOp::Mul,
            lhs: Box::new(field(lhs, "im")),
            rhs: Box::new(field(rhs, "re")),
        };
        self.materialize_expr(
            result,
            Expr::StructLit {
                name: "Complex".into(),
                fields: vec![
                    (
                        "re".into(),
                        Expr::Binary {
                            op: BinOp::Sub,
                            lhs: Box::new(ac),
                            rhs: Box::new(bd),
                        },
                    ),
                    (
                        "im".into(),
                        Expr::Binary {
                            op: BinOp::Add,
                            lhs: Box::new(ad),
                            rhs: Box::new(bc),
                        },
                    ),
                ],
            },
            op_result_type(op),
        );
    }

    fn lower_complex_div(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        if op.operands.len() < 2 {
            return;
        }
        if let Some(call) = self.complex_runtime_binop(op, "__divsc3", "__divdc3") {
            self.materialize_expr(result, call, op_result_type(op));
            return;
        }
        let lhs = self.operand_expr(&op.operands[0]);
        let rhs = self.operand_expr(&op.operands[1]);
        let field = |base: Expr, name: &str| Expr::Field {
            base: Box::new(base),
            field: name.into(),
        };
        let c = field(rhs.clone(), "re");
        let d = field(rhs.clone(), "im");
        let denom = self.next_temp();
        self.push_stmt(Stmt::Let {
            name: denom.clone(),
            mutable: false,
            ty: None,
            init: Some(Expr::Binary {
                op: BinOp::Add,
                lhs: Box::new(Expr::Binary {
                    op: BinOp::Mul,
                    lhs: Box::new(c.clone()),
                    rhs: Box::new(c),
                }),
                rhs: Box::new(Expr::Binary {
                    op: BinOp::Mul,
                    lhs: Box::new(d.clone()),
                    rhs: Box::new(d),
                }),
            }),
        });
        let denom_expr = || Expr::Var(denom.clone().into());
        let ac = Expr::Binary {
            op: BinOp::Mul,
            lhs: Box::new(field(lhs.clone(), "re")),
            rhs: Box::new(field(rhs.clone(), "re")),
        };
        let bd = Expr::Binary {
            op: BinOp::Mul,
            lhs: Box::new(field(lhs.clone(), "im")),
            rhs: Box::new(field(rhs.clone(), "im")),
        };
        let bc = Expr::Binary {
            op: BinOp::Mul,
            lhs: Box::new(field(lhs.clone(), "im")),
            rhs: Box::new(field(rhs.clone(), "re")),
        };
        let ad = Expr::Binary {
            op: BinOp::Mul,
            lhs: Box::new(field(lhs, "re")),
            rhs: Box::new(field(rhs, "im")),
        };
        self.materialize_expr(
            result,
            Expr::StructLit {
                name: "Complex".into(),
                fields: vec![
                    (
                        "re".into(),
                        Expr::Binary {
                            op: BinOp::Div,
                            lhs: Box::new(Expr::Binary {
                                op: BinOp::Add,
                                lhs: Box::new(ac),
                                rhs: Box::new(bd),
                            }),
                            rhs: Box::new(denom_expr()),
                        },
                    ),
                    (
                        "im".into(),
                        Expr::Binary {
                            op: BinOp::Div,
                            lhs: Box::new(Expr::Binary {
                                op: BinOp::Sub,
                                lhs: Box::new(bc),
                                rhs: Box::new(ad),
                            }),
                            rhs: Box::new(denom_expr()),
                        },
                    ),
                ],
            },
            op_result_type(op),
        );
    }

    fn lower_complex_conj(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        let Some(src) = op.operands.first() else {
            return;
        };
        let src = self.operand_expr(src);
        self.materialize_expr(
            result,
            Expr::StructLit {
                name: "Complex".into(),
                fields: vec![
                    (
                        "re".into(),
                        Expr::Field {
                            base: Box::new(src.clone()),
                            field: "re".into(),
                        },
                    ),
                    (
                        "im".into(),
                        Expr::Unary {
                            op: UnaryOp::Neg,
                            expr: Box::new(Expr::Field {
                                base: Box::new(src),
                                field: "im".into(),
                            }),
                        },
                    ),
                ],
            },
            op_result_type(op),
        );
    }

    fn complex_runtime_binop(&mut self, op: &Op, f32_name: &str, f64_name: &str) -> Option<Expr> {
        let inner = op_result_type(op).and_then(cir_complex_inner)?;
        let name = if inner == "!cir.float" {
            f32_name
        } else if inner == "!cir.double" {
            f64_name
        } else {
            return None;
        };
        let lhs = self.operand_expr(&op.operands[0]);
        let rhs = self.operand_expr(&op.operands[1]);
        let part = |base: Expr, field: &str| Expr::Field {
            base: Box::new(base),
            field: field.into(),
        };
        self.parent.uses_complex.set(true);
        Some(Self::unsafe_expr(Expr::Call {
            binding: crate::function_identity::CallBinding::Generated,
            func: Box::new(Expr::Var(name.into())),
            args: vec![
                part(lhs.clone(), "re"),
                part(lhs, "im"),
                part(rhs.clone(), "re"),
                part(rhs, "im"),
            ],
        }))
    }

    // cir.select(cond, t, f) is a pure value pick; all three operands are already
    // materialized, so it collapses to a Rust `if` expression.
    fn lower_select(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        if op.operands.len() < 3 {
            return;
        }
        let cond = self.operand_expr(&op.operands[0]);
        let t = self.operand_expr(&op.operands[1]);
        let f = self.operand_expr(&op.operands[2]);
        self.materialize_expr(
            result,
            Expr::If {
                cond: Box::new(cond),
                then_expr: Box::new(t),
                else_expr: Box::new(f),
            },
            op_result_type(op),
        );
    }

    // cir.ternary has two value-yielding regions; clang emits it for the NaN-recovery
    // arm of complex `*` (the taken branch calls __muldc3). Lower to an `if` whose
    // block bodies run each region's ops and tail-yield the region result.
    fn lower_ternary(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        let Some(cond) = op.operands.first() else {
            return;
        };
        if op.regions.len() < 2 {
            self.emit_todo("cir.ternary");
            return;
        }
        let cond = self.operand_expr(cond);
        let name = self.next_temp();
        let ty = op_result_type(op)
            .map(|ty| self.parent.rust_type(ty))
            .unwrap_or(Type::Prim(Prim::I32));
        let (then_body, then_value) = self.lower_yield_region(&op.regions[0]);
        let (else_body, else_value) = self.lower_yield_region(&op.regions[1]);
        self.push_stmt(Stmt::LetIf {
            name: name.clone(),
            mutable: false,
            ty: Some(ty),
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
        });
        self.values
            .insert(result.to_string(), Val::Expr(Expr::Var(name.into())));
    }

    // Lower every op in a region, capturing the terminating cir.yield's operand as
    // the region's tail value instead of lowering the yield itself.
    fn lower_yield_region(&mut self, region: &Region) -> (Vec<IndentStmt>, Expr) {
        let mut yielded = Expr::Todo("cir.yield".into());
        let body = self.capture_body(|this| {
            for block in &region.blocks {
                for op in &block.ops {
                    if op.kind() == CirOpKind::Yield {
                        if let Some(operand) = op.operands.first() {
                            yielded = this.operand_expr(operand);
                        }
                    } else {
                        this.lower_op(op);
                    }
                }
            }
        });
        (body, yielded)
    }

    fn lower_binary(&mut self, op: &Op, rust_op: BinOp) {
        let Some(result) = op.results.first() else {
            return;
        };
        if op.operands.len() < 2 {
            return;
        }
        let lhs = self.operand_expr(&op.operands[0]);
        let rhs = self.operand_expr(&op.operands[1]);
        self.materialize_expr(
            result,
            Expr::Binary {
                op: rust_op,
                lhs: Box::new(lhs),
                rhs: Box::new(rhs),
            },
            op_result_type(op),
        );
    }

    fn lower_known_binary(
        &mut self,
        op: &Op,
        known: crate::function_identity::Known,
        rust_op: BinOp,
    ) {
        if self.lower_known_libc_op(op, known) {
            self.lower_binary(op, rust_op);
        }
    }

    // The batch crate builds with `overflow-checks = false`, so plain `+`/`-`/`*`
    // wrap two's-complement just like clang's `-O0` C — no `wrapping_*` needed.
    // `/` and `%` still trap on div-by-zero and INT_MIN/-1 on both sides, so the
    // generator avoids those.
    fn lower_int_arith(&mut self, op: &Op, rust_op: BinOp) {
        let Some(result) = op.results.first() else {
            return;
        };
        if op.operands.len() < 2 {
            return;
        }
        if let Some((_, len)) = op_result_type(op).and_then(parse_cir_vector_type) {
            self.materialize_expr(
                result,
                self.vector_binary_expr(&op.operands[0], &op.operands[1], len, rust_op),
                op_result_type(op),
            );
            return;
        }
        let ty = op_result_type(op);
        if let Some(folded) = self.fold_int_arith(&op.operands[0], &op.operands[1], rust_op) {
            self.macro_arith_values.insert(result.clone(), folded);
            if let Some(expr) = self.next_macro_const_expr(folded, ty) {
                self.materialize_expr(result, expr, ty);
                return;
            }
        }
        let lhs = self.operand_expr(&op.operands[0]);
        let rhs = self.operand_expr(&op.operands[1]);
        self.materialize_expr(
            result,
            Expr::Binary {
                op: rust_op,
                lhs: Box::new(lhs),
                rhs: Box::new(rhs),
            },
            ty,
        );
    }

    fn known_arith_value(&self, operand: &str) -> Option<i128> {
        self.const_int_values
            .get(operand)
            .copied()
            .or_else(|| self.macro_arith_values.get(operand).copied())
    }

    fn fold_int_arith(&self, lhs: &str, rhs: &str, op: BinOp) -> Option<i128> {
        let l = self.known_arith_value(lhs)?;
        let r = self.known_arith_value(rhs)?;
        match op {
            BinOp::Add => Some(l + r),
            BinOp::Sub => Some(l - r),
            BinOp::Mul => Some(l * r),
            _ => None,
        }
    }

    fn lower_overflow_arith(&mut self, op: &Op, rust_method: &str) {
        if op.results.len() < 2 || op.operands.len() < 2 {
            return;
        }
        let lhs = self.operand_expr(&op.operands[0]);
        let rhs = self.operand_expr(&op.operands[1]);
        let result_types = op_result_types(op);
        let pair = self.next_temp();
        self.push_stmt(Stmt::Let {
            name: pair.clone(),
            mutable: false,
            ty: None,
            init: Some(Expr::MethodCall {
                recv: Box::new(lhs),
                method: rust_method.to_string(),
                args: vec![rhs],
            }),
        });
        self.materialize_expr(
            &op.results[0],
            Expr::Field {
                base: Box::new(Expr::Var(pair.clone().into())),
                field: "0".into(),
            },
            result_types.first().copied(),
        );
        self.materialize_expr(
            &op.results[1],
            Expr::Field {
                base: Box::new(Expr::Var(pair.into())),
                field: "1".into(),
            },
            result_types.get(1).copied(),
        );
    }

    fn lower_step(&mut self, op: &Op, rust_op: BinOp) {
        let Some(result) = op.results.first() else {
            return;
        };
        let Some(value) = op.operands.first() else {
            return;
        };
        let value = self.operand_expr(value);
        let ty = op_result_type(op);
        self.materialize_expr(
            result,
            Expr::Binary {
                op: rust_op,
                lhs: Box::new(value),
                rhs: Box::new(Expr::Value(RustValue::I64(1))),
            },
            ty,
        );
    }

    // cir.shift carries the isShiftleft unit attr for `<<`; its absence means `>>`.
    // Rust's `>>` is arithmetic on signed and logical on unsigned, matching C by type.
    fn lower_shift(&mut self, op: &Op) {
        let rust_op = if attr_bool(op, "isShiftleft") {
            BinOp::Shl
        } else {
            BinOp::Shr
        };
        self.lower_int_arith(op, rust_op);
    }

    // cir.not is C's unary `~`; Rust spells integer bitwise complement `!`.
    fn lower_not(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        let Some(value) = op.operands.first() else {
            return;
        };
        let value = self.operand_expr(value);
        let ty = op_result_type(op);
        self.materialize_expr(
            result,
            Expr::Unary {
                op: UnaryOp::Not,
                expr: Box::new(value),
            },
            ty,
        );
    }

    fn lower_neg(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        let Some(value) = op.operands.first() else {
            return;
        };
        let value = self.operand_expr(value);
        let result_ty = op_result_type(op);
        let operand_ty = op_operand_types(op.ty.as_deref().unwrap_or(""))
            .into_iter()
            .next()
            .unwrap_or("");
        let rust_ty = result_ty
            .map(|ty| self.parent.rust_type(ty))
            .unwrap_or(Type::Prim(Prim::I32));
        if type_mentions_long_double(&rust_ty) {
            self.materialize_expr(
                result,
                Expr::Call {
                    binding: crate::function_identity::CallBinding::Generated,
                    func: Box::new(Expr::Var(LONG_DOUBLE_TY.into())),
                    args: vec![Expr::Unary {
                        op: UnaryOp::Neg,
                        expr: Box::new(Expr::Field {
                            base: Box::new(value),
                            field: "0".into(),
                        }),
                    }],
                },
                result_ty,
            );
            return;
        }
        let expr = if operand_ty == "!cir.bool" {
            Expr::Unary {
                op: UnaryOp::Neg,
                expr: Box::new(Expr::Cast {
                    expr: Box::new(value),
                    ty: rust_ty,
                }),
            }
        } else {
            Expr::Unary {
                op: UnaryOp::Neg,
                expr: Box::new(value),
            }
        };
        self.materialize_expr(result, expr, result_ty);
    }

    fn lower_abs(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        let Some(value) = op.operands.first() else {
            return;
        };
        let value = self.operand_expr(value);
        let result_ty = op_result_type(op);
        let rust_ty = result_ty
            .map(|ty| self.parent.rust_type(ty))
            .unwrap_or(Type::Prim(Prim::I32));
        let expr = if matches!(
            &rust_ty,
            Type::Prim(Prim::I8 | Prim::I16 | Prim::I32 | Prim::I64)
        ) {
            Expr::MethodCall {
                recv: Box::new(value),
                method: "wrapping_abs".into(),
                args: vec![],
            }
        } else if type_mentions_long_double(&rust_ty) {
            Expr::Call {
                binding: crate::function_identity::CallBinding::Generated,
                func: Box::new(Expr::Var(LONG_DOUBLE_TY.into())),
                args: vec![Expr::MethodCall {
                    recv: Box::new(Expr::Field {
                        base: Box::new(value),
                        field: "0".into(),
                    }),
                    method: "abs".into(),
                    args: vec![],
                }],
            }
        } else {
            Expr::MethodCall {
                recv: Box::new(value),
                method: "abs".into(),
                args: vec![],
            }
        };
        self.materialize_expr(result, expr, result_ty);
    }

    fn lower_unary_method(&mut self, op: &Op, method: &str) {
        let Some(result) = op.results.first() else {
            return;
        };
        let Some(value) = op.operands.first() else {
            return;
        };
        let value = self.operand_expr(value);
        let result_ty = op_result_type(op);
        let rust_ty = result_ty
            .map(|ty| self.parent.rust_type(ty))
            .unwrap_or(Type::Prim(Prim::F64));
        let expr = if type_mentions_long_double(&rust_ty) {
            Expr::Call {
                binding: crate::function_identity::CallBinding::Generated,
                func: Box::new(Expr::Var(LONG_DOUBLE_TY.into())),
                args: vec![Expr::MethodCall {
                    recv: Box::new(Expr::Field {
                        base: Box::new(value),
                        field: "0".into(),
                    }),
                    method: method.into(),
                    args: vec![],
                }],
            }
        } else {
            Expr::MethodCall {
                recv: Box::new(value),
                method: method.into(),
                args: vec![],
            }
        };
        self.materialize_expr(result, expr, result_ty);
    }

    fn lower_known_unary_method(
        &mut self,
        op: &Op,
        known: crate::function_identity::Known,
        method: &str,
    ) {
        if self.lower_known_libc_op(op, known) {
            self.lower_unary_method(op, method);
        }
    }

    fn lower_unary_cast_method(&mut self, op: &Op, method: &str) {
        let Some(result) = op.results.first() else {
            return;
        };
        let Some(value) = op.operands.first() else {
            return;
        };
        let result_ty = op_result_type(op);
        let ty = result_ty
            .map(|ty| self.parent.rust_type(ty))
            .unwrap_or(Type::Prim(Prim::I64));
        let expr = Expr::Cast {
            expr: Box::new(Expr::MethodCall {
                recv: Box::new(self.operand_expr(value)),
                method: method.into(),
                args: Vec::new(),
            }),
            ty,
        };
        self.materialize_expr(result, expr, result_ty);
    }

    fn lower_known_unary_cast_method(
        &mut self,
        op: &Op,
        known: crate::function_identity::Known,
        method: &str,
    ) {
        if self.lower_known_libc_op(op, known) {
            self.lower_unary_cast_method(op, method);
        }
    }

    fn lower_parity(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        let Some(value) = op.operands.first() else {
            return;
        };
        let expr = Expr::Binary {
            op: BinOp::BitAnd,
            lhs: Box::new(Expr::MethodCall {
                recv: Box::new(self.operand_expr(value)),
                method: "count_ones".into(),
                args: Vec::new(),
            }),
            rhs: Box::new(Expr::Value(RustValue::I64(1))),
        };
        self.materialize_expr(result, expr, op_result_type(op));
    }

    fn lower_rotate(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        if op.operands.len() < 2 {
            return;
        }
        let method = if attr_bool(op, "rotateLeft") {
            "rotate_left"
        } else {
            "rotate_right"
        };
        let expr = Expr::MethodCall {
            recv: Box::new(self.operand_expr(&op.operands[0])),
            method: method.into(),
            args: vec![self.operand_expr(&op.operands[1])],
        };
        self.materialize_expr(result, expr, op_result_type(op));
    }

    fn lower_expect(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        let Some(value) = op.operands.first() else {
            return;
        };
        self.materialize_expr(result, self.operand_expr(value), op_result_type(op));
    }

    fn lower_assume(&mut self, op: &Op) {
        let Some(cond) = op.operands.first() else {
            return;
        };
        self.push_stmt(Stmt::Expr(Self::unsafe_expr(Expr::Call {
            binding: crate::function_identity::CallBinding::Generated,
            func: Box::new(Expr::Path(Path::new(
                ["core", "hint", "assert_unchecked"].map(Ident::from),
            ))),
            args: vec![self.operand_expr(cond)],
        })));
    }

    fn lower_asm(&mut self, op: &Op) {
        if op.results.is_empty()
            && op.operands.is_empty()
            && let Some(raw) = attr_str(op, "asm_string")
            && !asm_template_has_placeholders(raw)
        {
            let Ok(template) = String::from_utf8(decode_cir_string(raw)) else {
                self.parent
                    .ctx
                    .diagnostics
                    .error("lower: inline assembly template is not valid UTF-8");
                return;
            };
            self.push_stmt(Self::unsafe_stmt(Stmt::Expr(asm_macro_expr(
                template.replace("$$", "$"),
                cir_asm_dialect(op),
            ))));
            return;
        }
        if self.lower_extended_asm(op) {
            return;
        }
        let Some(result) = op.results.first() else {
            return;
        };
        let expr = op
            .operands
            .first()
            .map(|operand| self.operand_expr(operand))
            .unwrap_or_else(|| {
                op_result_type(op)
                    .map(|ty| self.parent.rust_type(ty))
                    .map(|ty| self.default_value_expr(&ty))
                    .unwrap_or(Expr::Value(RustValue::I64(0)))
            });
        self.materialize_expr(result, expr, op_result_type(op));
    }

    fn lower_extended_asm(&mut self, op: &Op) -> bool {
        let Some(raw_template) = attr_str(op, "asm_string") else {
            return false;
        };
        let Ok(template) = String::from_utf8(decode_cir_string(raw_template)) else {
            self.parent
                .ctx
                .diagnostics
                .error("lower: inline assembly template is not valid UTF-8");
            return true;
        };
        let label_count = asm_template_label_count(&template);
        let asm_goto = if label_count == 0 {
            None
        } else {
            let Some(asm_goto) = self.asm_gotos.pop_front() else {
                self.parent
                    .ctx
                    .diagnostics
                    .error("lower: asm goto labels are missing from the Clang AST");
                return true;
            };
            Some(asm_goto)
        };
        if asm_goto
            .as_ref()
            .is_some_and(|asm_goto| asm_goto.labels.len() != label_count)
        {
            self.parent
                .ctx
                .diagnostics
                .error("lower: asm goto label count differs between CIR and the Clang AST");
            return true;
        }
        let Some(raw_constraints) = attr_str(op, "constraints") else {
            return false;
        };
        let constraints = raw_constraints
            .split(',')
            .map(str::trim)
            .take_while(|constraint| !constraint.starts_with("~{"))
            .collect::<Vec<_>>();
        let Some(output_count) = constraints.len().checked_sub(op.operands.len()) else {
            return false;
        };
        if output_count != op.results.len() && !(op.results.len() == 1 && output_count > 1) {
            return false;
        }
        if constraints[..output_count]
            .iter()
            .any(|constraint| !matches!(*constraint, "=r" | "=&r"))
        {
            return false;
        }
        let mut tied_outputs = vec![None; output_count];
        for (operand_index, constraint) in constraints[output_count..].iter().enumerate() {
            if let Ok(output_index) = constraint.parse::<usize>() {
                if output_index >= output_count || tied_outputs[output_index].is_some() {
                    return false;
                }
                tied_outputs[output_index] = Some(operand_index);
            } else if !matches!(*constraint, "r" | "i") {
                return false;
            }
        }
        let result_types = if output_count == 0 {
            Vec::new()
        } else {
            let Some(result_types) = asm_output_types(op, &self.parent.aliases, output_count)
                .map(|types| types.into_iter().map(str::to_string).collect::<Vec<_>>())
            else {
                return false;
            };
            result_types
        };
        let operand_types = op.ty.as_deref().map(op_operand_types).unwrap_or_default();
        if operand_types.len() != op.operands.len() {
            return false;
        }
        if constraints[output_count..]
            .iter()
            .zip(&op.operands)
            .any(|(constraint, operand)| {
                *constraint == "i" && self.known_arith_value(operand).is_none()
            })
        {
            return false;
        }
        let mut slot_to_rust = vec![0; constraints.len() + label_count];
        for (output_index, slot) in slot_to_rust.iter_mut().take(output_count).enumerate() {
            *slot = output_index;
        }
        let mut next_rust_operand = output_count;
        for (operand_index, constraint) in constraints[output_count..].iter().enumerate() {
            let slot = output_count + operand_index;
            if let Ok(output_index) = constraint.parse::<usize>() {
                slot_to_rust[slot] = output_index;
            } else {
                slot_to_rust[slot] = next_rust_operand;
                next_rust_operand += 1;
            }
        }
        for slot in &mut slot_to_rust[constraints.len()..] {
            *slot = next_rust_operand;
            next_rust_operand += 1;
        }
        let mut template_constraints = constraints.clone();
        template_constraints.extend(std::iter::repeat_n("X", label_count));
        let mut template_types = result_types
            .iter()
            .map(String::as_str)
            .chain(operand_types.iter().copied())
            .collect::<Vec<_>>();
        template_types.extend(std::iter::repeat_n("()", label_count));
        let Some(template) = translate_asm_template(
            &template,
            &slot_to_rust,
            &template_constraints,
            &template_types,
        ) else {
            return false;
        };
        let mut operands = Vec::new();
        let mut output_exprs = Vec::new();
        let mut output_names = Vec::new();
        if asm_goto.is_some()
            && output_count > 0
            && (op.results.len() != output_count
                || op
                    .results
                    .iter()
                    .any(|result| !self.asm_output_places.contains_key(result)))
        {
            self.parent
                .ctx
                .diagnostics
                .error("lower: asm goto output does not have a direct CIR destination");
            return true;
        }
        for (output_index, constraint) in constraints[..output_count].iter().enumerate() {
            let direct_output = op
                .results
                .get(output_index)
                .and_then(|result| self.asm_output_places.get(result))
                .cloned();
            let (output, output_name) = if let Some(output) = direct_output {
                (output, None)
            } else {
                let name = self.next_temp();
                self.push_stmt(Stmt::Let {
                    name: name.clone(),
                    mutable: false,
                    ty: Some(self.parent.rust_type(&result_types[output_index])),
                    init: None,
                });
                (Expr::Var(name.clone().into()), Some(name))
            };
            if let Some(operand_index) = tied_outputs[output_index] {
                operands.push(AsmOperand::InOut {
                    reg: AsmReg::Class("reg".into()),
                    late: *constraint == "=r",
                    input: self.operand_expr(&op.operands[operand_index]),
                    output,
                });
            } else {
                operands.push(AsmOperand::Out {
                    reg: AsmReg::Class("reg".into()),
                    late: *constraint == "=r",
                    value: output,
                });
            }
            output_exprs.push(
                operands
                    .last()
                    .and_then(|operand| match operand {
                        AsmOperand::Out { value, .. } => Some(value.clone()),
                        AsmOperand::InOut { output, .. } => Some(output.clone()),
                        _ => None,
                    })
                    .unwrap(),
            );
            output_names.push(output_name);
        }
        for (operand_index, constraint) in constraints[output_count..].iter().enumerate() {
            if constraint.parse::<usize>().is_ok() {
                continue;
            }
            let value = if *constraint == "i" {
                let value = self.known_arith_value(&op.operands[operand_index]).unwrap();
                AsmOperand::Const(int_value_expr(value))
            } else {
                AsmOperand::In {
                    reg: AsmReg::Class("reg".into()),
                    value: self.operand_expr(&op.operands[operand_index]),
                }
            };
            operands.push(value);
        }
        if let Some(asm_goto) = asm_goto {
            let Some(dispatch) = self.dispatch.as_ref() else {
                self.parent
                    .ctx
                    .diagnostics
                    .error("lower: asm goto requires dispatch control flow");
                return true;
            };
            for label in asm_goto.labels {
                let Some(state) = dispatch.label_to_state.get(&label).copied() else {
                    self.parent.ctx.diagnostics.error(format!(
                        "lower: asm goto target `{label}` is missing from CIR"
                    ));
                    return true;
                };
                operands.push(AsmOperand::Label {
                    state: Expr::Var(dispatch.state_var.clone().into()),
                    value: Expr::Value(RustValue::I64(state as i64)),
                    destination: dispatch.loop_label.clone(),
                });
            }
            if output_count > 0 {
                self.parent.uses_asm_goto_outputs.set(true);
            }
        }
        self.push_stmt(Self::unsafe_stmt(Stmt::InlineAsm(InlineAsm {
            template,
            dialect: cir_asm_dialect(op),
            operands,
            raw: false,
        })));
        if output_count == op.results.len() {
            for ((result, output), name) in op.results.iter().zip(output_exprs).zip(output_names) {
                self.values.insert(result.clone(), Val::Expr(output));
                if let Some(name) = name {
                    self.immutable_temps.insert(name);
                }
            }
        } else {
            self.asm_outputs.insert(op.results[0].clone(), output_exprs);
            self.immutable_temps
                .extend(output_names.into_iter().flatten());
        }
        true
    }

    fn lower_eh_setjmp(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        self.materialize_expr(result, Expr::Value(RustValue::I64(0)), op_result_type(op));
    }

    fn lower_unsupported_value(&mut self, op: &Op, note: &str) {
        let Some(result) = op.results.first() else {
            self.push_stmt(Stmt::Expr(Expr::Macro {
                name: "panic".into(),
                args: vec![Expr::Str(format!("unsupported CIR op: {note}"))],
            }));
            return;
        };
        self.materialize_expr(
            result,
            Expr::Macro {
                name: "panic".into(),
                args: vec![Expr::Str(format!("unsupported CIR op: {note}"))],
            },
            op_result_type(op),
        );
    }

    fn lower_llvm_intrinsic(&mut self, op: &Op) {
        let Some(name) = attr_str(op, "intrinsic_name") else {
            self.lower_unsupported_value(op, "cir.call_llvm_intrinsic");
            return;
        };
        if name == "debugtrap" {
            self.parent.uses_breakpoint.set(true);
            self.push_stmt(Stmt::Expr(Expr::Call {
                binding: crate::function_identity::CallBinding::Generated,
                func: Box::new(Expr::Path(Path::new(
                    ["core", "arch", "breakpoint"].map(Ident::from),
                ))),
                args: Vec::new(),
            }));
        } else if !self.lower_x86_intrinsic(op, name) {
            self.lower_unsupported_value(op, "cir.call_llvm_intrinsic");
        }
    }

    fn lower_x86_intrinsic(&mut self, op: &Op, name: &str) -> bool {
        let function = match name {
            "x86.sse2.pause" => "_mm_pause",
            "x86.sse2.lfence" => "_mm_lfence",
            "x86.sse2.mfence" => "_mm_mfence",
            "x86.sse.sfence" => "_mm_sfence",
            "x86.rdtsc" => "_rdtsc",
            "x86.sse42.crc32.32.8" => "_mm_crc32_u8",
            "x86.sse42.crc32.32.16" => "_mm_crc32_u16",
            "x86.sse42.crc32.32.32" => "_mm_crc32_u32",
            "x86.sse42.crc32.64.64" => "_mm_crc32_u64",
            "x86.rdtscp" => return self.lower_x86_rdtscp(op),
            _ => return false,
        };
        let Some(call) = self.x86_intrinsic_op_call(function, op) else {
            return false;
        };
        let call = Self::unsafe_expr(call);
        if matches!(
            name,
            "x86.sse2.pause" | "x86.sse2.lfence" | "x86.sse2.mfence" | "x86.sse.sfence"
        ) {
            self.push_stmt(Stmt::Expr(call));
        } else if let Some(result) = op.results.first() {
            self.materialize_expr(result, call, op_result_type(op));
        }
        true
    }

    fn lower_x86_rdtscp(&mut self, op: &Op) -> bool {
        let Some(result) = op.results.first() else {
            return false;
        };
        let Some(result_ty) = op_result_type(op) else {
            return false;
        };
        let Type::Custom(record_name) = self.parent.rust_type(result_ty) else {
            return false;
        };
        let Some(record) = self.parent.records.get(&record_name) else {
            return false;
        };
        let [counter_field, auxiliary_field] = record.fields.as_slice() else {
            return false;
        };
        let counter_field = sanitize_ident(&counter_field.name).into_string();
        let auxiliary_field = sanitize_ident(&auxiliary_field.name).into_string();
        let auxiliary = self.unique_local_name("__slate_rdtscp_aux".into());
        let counter = self.unique_local_name("__slate_rdtscp_counter".into());
        let Some(call) = self.x86_intrinsic_call(
            "__rdtscp",
            &[Expr::Ref {
                mutable: true,
                expr: Box::new(Expr::Var(auxiliary.clone().into())),
            }],
        ) else {
            return false;
        };
        let expr = Expr::Block(Box::new(crate::rust_ast::Block {
            stmts: vec![
                IndentStmt {
                    depth: 1,
                    stmt: Stmt::Let {
                        name: auxiliary.clone(),
                        mutable: true,
                        ty: Some(Type::Prim(Prim::U32)),
                        init: Some(Expr::Value(RustValue::I64(0))),
                    },
                },
                IndentStmt {
                    depth: 1,
                    stmt: Stmt::Let {
                        name: counter.clone(),
                        mutable: false,
                        ty: Some(Type::Prim(Prim::U64)),
                        init: Some(Self::unsafe_expr(call)),
                    },
                },
            ],
            tail: Some(Box::new(Expr::StructLit {
                name: record_name,
                fields: vec![
                    (counter_field, Expr::Var(counter.into())),
                    (auxiliary_field, Expr::Var(auxiliary.into())),
                ],
            })),
        }));
        self.materialize_expr(result, expr, Some(result_ty));
        true
    }

    fn x86_intrinsic_op_call(&self, function: &str, op: &Op) -> Option<Expr> {
        let args = op
            .operands
            .iter()
            .map(|operand| self.operand_expr(operand))
            .collect::<Vec<_>>();
        self.x86_intrinsic_call(function, &args)
    }

    fn x86_intrinsic_call(&self, function: &str, args: &[Expr]) -> Option<Expr> {
        let arch = match self.parent.target_arch.as_deref()? {
            "x86" => "x86",
            "x86_64" => "x86_64",
            _ => return None,
        };
        Some(Expr::Call {
            binding: CallBinding::Generated,
            func: Box::new(Expr::Path(Path::new(
                ["core", "arch", arch, function].map(Ident::from),
            ))),
            args: args.to_vec(),
        })
    }

    fn vector_binary_expr(&self, lhs: &str, rhs: &str, len: u64, op: BinOp) -> Expr {
        let lhs = self.operand_expr(lhs);
        let rhs = self.operand_expr(rhs);
        Expr::ArrayLit(
            (0..len)
                .map(|i| Expr::Binary {
                    op,
                    lhs: Box::new(vector_index_expr(lhs.clone(), i)),
                    rhs: Box::new(vector_index_expr(rhs.clone(), i)),
                })
                .collect(),
        )
    }

    fn lower_vec_extract(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        if op.operands.len() < 2 {
            return;
        }
        self.materialize_expr(
            result,
            Expr::Index {
                base: Box::new(self.operand_expr(&op.operands[0])),
                index: Box::new(Expr::Cast {
                    expr: Box::new(self.operand_expr(&op.operands[1])),
                    ty: Type::Prim(Prim::Usize),
                }),
            },
            op_result_type(op),
        );
    }

    fn lower_vec_insert(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        if op.operands.len() < 3 {
            return;
        }
        let Some((_, len)) = op_result_type(op).and_then(parse_cir_vector_type) else {
            self.lower_unsupported_value(op, "cir.vec.insert");
            return;
        };
        let Some(index) = self
            .const_int_values
            .get(&op.operands[2])
            .and_then(|i| u64::try_from(*i).ok())
            .filter(|i| *i < len)
        else {
            self.lower_unsupported_value(op, "cir.vec.insert dynamic index");
            return;
        };
        let base = self.operand_expr(&op.operands[0]);
        let value = self.operand_expr(&op.operands[1]);
        self.materialize_expr(
            result,
            Expr::ArrayLit(
                (0..len)
                    .map(|i| {
                        if i == index {
                            value.clone()
                        } else {
                            vector_index_expr(base.clone(), i)
                        }
                    })
                    .collect(),
            ),
            op_result_type(op),
        );
    }

    fn lower_vec_shuffle(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        if op.operands.len() < 2 {
            return;
        }
        let Some((_, len)) = op_result_type(op).and_then(parse_cir_vector_type) else {
            self.lower_unsupported_value(op, "cir.vec.shuffle");
            return;
        };
        let indices = attr_int_array(op, "indices").unwrap_or_default();
        if indices.len() != len as usize {
            self.lower_unsupported_value(op, "cir.vec.shuffle indices");
            return;
        }
        let lhs = self.operand_expr(&op.operands[0]);
        let rhs = self.operand_expr(&op.operands[1]);
        self.materialize_expr(
            result,
            Expr::ArrayLit(
                indices
                    .into_iter()
                    .map(|index| {
                        if index < len {
                            vector_index_expr(lhs.clone(), index)
                        } else {
                            vector_index_expr(rhs.clone(), index - len)
                        }
                    })
                    .collect(),
            ),
            op_result_type(op),
        );
    }

    fn lower_trap(&mut self) {
        self.push_stmt(Stmt::Expr(Expr::Call {
            binding: crate::function_identity::CallBinding::Generated,
            func: Box::new(Expr::Path(Path::new(
                ["std", "process", "abort"].map(Ident::from),
            ))),
            args: Vec::new(),
        }));
    }

    fn lower_unreachable(&mut self) {
        self.push_stmt(Stmt::Expr(Expr::Macro {
            name: "unreachable".into(),
            args: Vec::new(),
        }));
    }

    fn lower_opaque_pointer(&mut self, op: &Op, non_null: bool) {
        let Some(result) = op.results.first() else {
            return;
        };
        let result_ty = op_result_type(op);
        let ty = result_ty
            .map(|ty| self.parent.rust_type(ty))
            .unwrap_or(Type::Ptr {
                mutable: true,
                inner: Box::new(Type::CLib(CLibType::Void)),
            });
        let addr = if non_null { 1 } else { 0 };
        self.materialize_expr(
            result,
            Expr::Cast {
                expr: Box::new(Expr::Value(RustValue::Usize(addr))),
                ty,
            },
            result_ty,
        );
    }

    fn lower_is_constant(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        let Some(value) = op.operands.first() else {
            return;
        };
        let is_constant = self.const_int_values.contains_key(value)
            || self.values.get(value).is_some_and(|value| match value {
                Val::Expr(expr) => matches!(
                    expr,
                    Expr::Value(_)
                        | Expr::Str(_)
                        | Expr::HexFloat(_)
                        | Expr::ByteStr(_)
                        | Expr::CStr(_)
                        | Expr::Path(_)
                ),
                Val::Global(_) => true,
            });
        let result_ty = op_result_type(op);
        let expr = self.bool_or_int_literal(is_constant, result_ty);
        self.materialize_expr(result, expr, result_ty);
    }

    fn lower_objsize(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        let result_ty = op_result_type(op);
        let expr = if attr_bool(op, "min") {
            self.zero_literal(result_ty)
        } else {
            self.max_literal(result_ty)
        };
        self.materialize_expr(result, expr, result_ty);
    }

    fn bool_or_int_literal(&self, value: bool, cir_ty: Option<&str>) -> Expr {
        match cir_ty.map(|ty| self.parent.rust_type(ty)) {
            Some(Type::Prim(Prim::Bool)) => Expr::Value(RustValue::Bool(value)),
            _ => Expr::Value(RustValue::I64(if value { 1 } else { 0 })),
        }
    }

    fn zero_literal(&self, cir_ty: Option<&str>) -> Expr {
        match cir_ty.map(|ty| self.parent.rust_type(ty)) {
            Some(Type::Prim(Prim::Bool)) => Expr::Value(RustValue::Bool(false)),
            _ => Expr::Value(RustValue::I64(0)),
        }
    }

    fn max_literal(&self, cir_ty: Option<&str>) -> Expr {
        match cir_ty.map(|ty| self.parent.rust_type(ty)) {
            Some(Type::Prim(Prim::Bool)) => Expr::Value(RustValue::Bool(true)),
            Some(Type::Prim(prim)) => Expr::Path(Path::new([
                Ident::from(prim.spelling()),
                Ident::from("MAX"),
            ])),
            _ => Expr::Value(RustValue::I64(-1)),
        }
    }

    fn lower_ffs(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        let Some(value) = op.operands.first() else {
            return;
        };
        let result_ty = op_result_type(op);
        let ty = result_ty
            .map(|ty| self.parent.rust_type(ty))
            .unwrap_or(Type::Prim(Prim::I32));
        let value = self.operand_expr(value);
        let expr = Expr::If {
            cond: Box::new(Expr::Binary {
                op: BinOp::Eq,
                lhs: Box::new(value.clone()),
                rhs: Box::new(Expr::Value(RustValue::I64(0))),
            }),
            then_expr: Box::new(Expr::Value(RustValue::I64(0))),
            else_expr: Box::new(Expr::Binary {
                op: BinOp::Add,
                lhs: Box::new(Expr::Cast {
                    expr: Box::new(Expr::MethodCall {
                        recv: Box::new(value),
                        method: "trailing_zeros".into(),
                        args: Vec::new(),
                    }),
                    ty,
                }),
                rhs: Box::new(Expr::Value(RustValue::I64(1))),
            }),
        };
        self.materialize_expr(result, expr, result_ty);
    }

    fn lower_clrsb(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        let Some(value) = op.operands.first() else {
            return;
        };
        let result_ty = op_result_type(op);
        let ty = result_ty
            .map(|ty| self.parent.rust_type(ty))
            .unwrap_or(Type::Prim(Prim::I32));
        let value = self.operand_expr(value);
        let sign_stripped = Expr::If {
            cond: Box::new(Expr::Binary {
                op: BinOp::Lt,
                lhs: Box::new(value.clone()),
                rhs: Box::new(Expr::Value(RustValue::I64(0))),
            }),
            then_expr: Box::new(Expr::Unary {
                op: UnaryOp::Not,
                expr: Box::new(value.clone()),
            }),
            else_expr: Box::new(value),
        };
        let expr = Expr::Binary {
            op: BinOp::Sub,
            lhs: Box::new(Expr::Cast {
                expr: Box::new(Expr::MethodCall {
                    recv: Box::new(sign_stripped),
                    method: "leading_zeros".into(),
                    args: Vec::new(),
                }),
                ty,
            }),
            rhs: Box::new(Expr::Value(RustValue::I64(1))),
        };
        self.materialize_expr(result, expr, result_ty);
    }

    fn lower_binary_method(&mut self, op: &Op, method: &str) {
        let Some(result) = op.results.first() else {
            return;
        };
        if op.operands.len() < 2 {
            return;
        }
        let lhs = self.operand_expr(&op.operands[0]);
        let rhs = self.operand_expr(&op.operands[1]);
        let result_ty = op_result_type(op);
        let rust_ty = result_ty
            .map(|ty| self.parent.rust_type(ty))
            .unwrap_or(Type::Prim(Prim::F64));
        let expr = if type_mentions_long_double(&rust_ty) {
            Expr::Call {
                binding: crate::function_identity::CallBinding::Generated,
                func: Box::new(Expr::Var(LONG_DOUBLE_TY.into())),
                args: vec![Expr::MethodCall {
                    recv: Box::new(Expr::Field {
                        base: Box::new(lhs),
                        field: "0".into(),
                    }),
                    method: method.into(),
                    args: vec![Expr::Field {
                        base: Box::new(rhs),
                        field: "0".into(),
                    }],
                }],
            }
        } else {
            Expr::MethodCall {
                recv: Box::new(lhs),
                method: method.into(),
                args: vec![rhs],
            }
        };
        self.materialize_expr(result, expr, result_ty);
    }

    fn lower_known_binary_method(
        &mut self,
        op: &Op,
        known: crate::function_identity::Known,
        method: &str,
    ) {
        if self.lower_known_libc_op(op, known) {
            self.lower_binary_method(op, method);
        }
    }

    fn lower_ternary_method(&mut self, op: &Op, method: &str) {
        let Some(result) = op.results.first() else {
            return;
        };
        if op.operands.len() < 3 {
            return;
        }
        let a = self.operand_expr(&op.operands[0]);
        let b = self.operand_expr(&op.operands[1]);
        let c = self.operand_expr(&op.operands[2]);
        let result_ty = op_result_type(op);
        let rust_ty = result_ty
            .map(|ty| self.parent.rust_type(ty))
            .unwrap_or(Type::Prim(Prim::F64));
        let expr = if type_mentions_long_double(&rust_ty) {
            Expr::Call {
                binding: crate::function_identity::CallBinding::Generated,
                func: Box::new(Expr::Var(LONG_DOUBLE_TY.into())),
                args: vec![Expr::MethodCall {
                    recv: Box::new(Expr::Field {
                        base: Box::new(a),
                        field: "0".into(),
                    }),
                    method: method.into(),
                    args: vec![
                        Expr::Field {
                            base: Box::new(b),
                            field: "0".into(),
                        },
                        Expr::Field {
                            base: Box::new(c),
                            field: "0".into(),
                        },
                    ],
                }],
            }
        } else {
            Expr::MethodCall {
                recv: Box::new(a),
                method: method.into(),
                args: vec![b, c],
            }
        };
        self.materialize_expr(result, expr, result_ty);
    }

    fn lower_signbit(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        let Some(value) = op.operands.first() else {
            return;
        };
        let operand_ty = op
            .ty
            .as_deref()
            .and_then(|ty| op_operand_types(ty).into_iter().next());
        let value = self.float_predicate_operand_expr(value, operand_ty);
        self.materialize_expr(
            result,
            Expr::MethodCall {
                recv: Box::new(value),
                method: "is_sign_negative".into(),
                args: vec![],
            },
            op_result_type(op),
        );
    }

    fn lower_is_fp_class(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        let Some(value) = op.operands.first() else {
            return;
        };
        let Some(flags) = attr_int(op, "flags") else {
            return;
        };
        let operand_ty = op
            .ty
            .as_deref()
            .and_then(|ty| op_operand_types(ty).into_iter().next());
        let value = self.float_predicate_operand_expr(value, operand_ty);
        let mut parts = Vec::new();
        if flags & 0x3 != 0 {
            parts.push(Expr::MethodCall {
                recv: Box::new(value.clone()),
                method: "is_nan".into(),
                args: vec![],
            });
        }
        if flags & 0x4 != 0 {
            parts.push(Expr::Binary {
                op: BinOp::Eq,
                lhs: Box::new(value.clone()),
                rhs: Box::new(Expr::Path(Path::new(
                    ["f64", "NEG_INFINITY"].map(Ident::from),
                ))),
            });
        }
        if flags & 0x8 != 0 {
            parts.push(Self::and_expr(
                Expr::MethodCall {
                    recv: Box::new(value.clone()),
                    method: "is_normal".into(),
                    args: vec![],
                },
                Expr::MethodCall {
                    recv: Box::new(value.clone()),
                    method: "is_sign_negative".into(),
                    args: vec![],
                },
            ));
        }
        if flags & 0x10 != 0 {
            parts.push(Self::and_expr(
                Expr::MethodCall {
                    recv: Box::new(value.clone()),
                    method: "is_subnormal".into(),
                    args: vec![],
                },
                Expr::MethodCall {
                    recv: Box::new(value.clone()),
                    method: "is_sign_negative".into(),
                    args: vec![],
                },
            ));
        }
        if flags & 0x20 != 0 {
            parts.push(Self::and_expr(
                Expr::Binary {
                    op: BinOp::Eq,
                    lhs: Box::new(value.clone()),
                    rhs: Box::new(Expr::Value(RustValue::Float(0.0))),
                },
                Expr::MethodCall {
                    recv: Box::new(value.clone()),
                    method: "is_sign_negative".into(),
                    args: vec![],
                },
            ));
        }
        if flags & 0x40 != 0 {
            parts.push(Self::and_expr(
                Expr::Binary {
                    op: BinOp::Eq,
                    lhs: Box::new(value.clone()),
                    rhs: Box::new(Expr::Value(RustValue::Float(0.0))),
                },
                Expr::Unary {
                    op: UnaryOp::Not,
                    expr: Box::new(Expr::MethodCall {
                        recv: Box::new(value.clone()),
                        method: "is_sign_negative".into(),
                        args: vec![],
                    }),
                },
            ));
        }
        if flags & 0x80 != 0 {
            parts.push(Self::and_expr(
                Expr::MethodCall {
                    recv: Box::new(value.clone()),
                    method: "is_subnormal".into(),
                    args: vec![],
                },
                Expr::Unary {
                    op: UnaryOp::Not,
                    expr: Box::new(Expr::MethodCall {
                        recv: Box::new(value.clone()),
                        method: "is_sign_negative".into(),
                        args: vec![],
                    }),
                },
            ));
        }
        if flags & 0x100 != 0 {
            parts.push(Self::and_expr(
                Expr::MethodCall {
                    recv: Box::new(value.clone()),
                    method: "is_normal".into(),
                    args: vec![],
                },
                Expr::Unary {
                    op: UnaryOp::Not,
                    expr: Box::new(Expr::MethodCall {
                        recv: Box::new(value.clone()),
                        method: "is_sign_negative".into(),
                        args: vec![],
                    }),
                },
            ));
        }
        if flags & 0x200 != 0 {
            parts.push(Expr::Binary {
                op: BinOp::Eq,
                lhs: Box::new(value),
                rhs: Box::new(Expr::Path(Path::new(["f64", "INFINITY"].map(Ident::from)))),
            });
        }
        let expr = if parts.is_empty() {
            Expr::Value(RustValue::Bool(false))
        } else {
            Self::or_exprs(parts)
        };
        self.materialize_expr(result, expr, op_result_type(op));
    }

    fn float_predicate_operand_expr(&self, operand: &str, ty: Option<&str>) -> Expr {
        let value = self.operand_expr(operand);
        match ty {
            Some(ty) if is_long_double(ty) => Expr::Field {
                base: Box::new(value),
                field: "0".into(),
            },
            Some("!cir.float") => Expr::Cast {
                expr: Box::new(value),
                ty: Type::Prim(Prim::F64),
            },
            _ => value,
        }
    }

    fn lower_modf(&mut self, op: &Op) {
        if op.results.len() < 2 {
            return;
        }
        let Some(value) = op.operands.first() else {
            return;
        };
        let value = self.operand_expr(value);
        let result_types = op_result_types(op);
        self.materialize_expr(
            &op.results[0],
            Expr::MethodCall {
                recv: Box::new(value.clone()),
                method: "fract".into(),
                args: vec![],
            },
            result_types.first().copied(),
        );
        self.materialize_expr(
            &op.results[1],
            Expr::MethodCall {
                recv: Box::new(value),
                method: "trunc".into(),
                args: vec![],
            },
            result_types.get(1).copied(),
        );
    }

    fn lower_cmp(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        if op.operands.len() < 2 {
            return;
        }
        if let Some(expr) = self.lower_function_pointer_null_cmp(op) {
            self.materialize_expr(result, expr, Some("!cir.bool"));
            return;
        }
        let operand_types = op_operand_types(op.ty.as_deref().unwrap_or(""));
        let concrete_function_symbols = operand_types
            .first()
            .zip(operand_types.get(1))
            .is_some_and(|(lhs, rhs)| {
                is_cir_function_pointer_type(lhs)
                    && is_cir_function_pointer_type(rhs)
                    && matches!(self.values.get(&op.operands[0]), Some(Val::Global(_)))
                    && matches!(self.values.get(&op.operands[1]), Some(Val::Global(_)))
            });
        let lhs = if concrete_function_symbols {
            self.function_pointer_byte_operand_expr(&op.operands[0])
        } else {
            operand_types.first().map_or_else(
                || self.operand_expr(&op.operands[0]),
                |ty| self.call_arg_expr(&op.operands[0], ty),
            )
        };
        let rhs = if concrete_function_symbols {
            self.function_pointer_byte_operand_expr(&op.operands[1])
        } else {
            operand_types.get(1).map_or_else(
                || self.operand_expr(&op.operands[1]),
                |ty| self.call_arg_expr(&op.operands[1], ty),
            )
        };
        let cmp = match attr_int(op, "kind") {
            Some(0) => BinOp::Lt,
            Some(1) => BinOp::Le,
            Some(2) => BinOp::Gt,
            Some(3) => BinOp::Ge,
            Some(4) => BinOp::Eq,
            Some(5) => BinOp::Ne,
            _ => BinOp::Le,
        };
        self.materialize_expr(
            result,
            Expr::Binary {
                op: cmp,
                lhs: Box::new(lhs),
                rhs: Box::new(rhs),
            },
            Some("!cir.bool"),
        );
    }

    fn lower_function_pointer_null_cmp(&self, op: &Op) -> Option<Expr> {
        let kind = attr_int(op, "kind")?;
        let (nonnull_operand, method) = match (
            self.is_function_pointer_null_operand(&op.operands[0]),
            self.is_function_pointer_null_operand(&op.operands[1]),
            kind,
        ) {
            (false, true, 4) => (&op.operands[0], "is_none"),
            (false, true, 5) => (&op.operands[0], "is_some"),
            (true, false, 4) => (&op.operands[1], "is_none"),
            (true, false, 5) => (&op.operands[1], "is_some"),
            _ => return None,
        };
        Some(Expr::MethodCall {
            recv: Box::new(self.function_pointer_operand_expr(nonnull_operand)),
            method: method.into(),
            args: Vec::new(),
        })
    }

    fn lower_get_global(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        let name = attr_str(op, "name")
            .unwrap_or("")
            .trim_start_matches('@')
            .trim_matches('"')
            .to_string();
        let name = self.parent.weak_refs.get(&name).cloned().unwrap_or(name);
        self.values.insert(result.clone(), Val::Global(name));
    }

    fn place_expr(&self, ptr: &str) -> Option<Expr> {
        if let Some(member) = self.member_ptrs.get(ptr) {
            Some(Expr::Field {
                base: Box::new(member.base.clone()),
                field: member.field.clone(),
            })
        } else if let Some(element) = self.element_ptrs.get(ptr) {
            Some(self.element_place_expr(element))
        } else if let Some(slot) = self.slot_place(ptr) {
            Some(slot)
        } else {
            self.global_place(ptr)
        }
    }

    fn place_or_deref_expr(&self, ptr: &str) -> Expr {
        if self.aligned_slots.contains(ptr)
            && let Some(place) = self.slot_receiver(ptr)
        {
            return place;
        }
        self.place_expr(ptr).unwrap_or_else(|| Expr::Unary {
            op: UnaryOp::Deref,
            expr: Box::new(self.pointer_operand_expr(ptr)),
        })
    }

    fn lower_get_member(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        let Some(base_ptr) = op.operands.first() else {
            return;
        };
        if let Some(outputs) = self.asm_outputs.get(base_ptr)
            && let Some(output) = aggregate_member_index(op).and_then(|index| outputs.get(index))
        {
            self.forward_values.insert(result.clone(), output.clone());
            return;
        }
        let base = self.place_or_deref_expr(base_ptr);
        let raw_field = attr_str(op, "name").unwrap_or(result);
        let logical_field = if raw_field.is_empty() {
            aggregate_member_index(op)
                .map(|index| format!("__slate_anon_{index}"))
                .unwrap_or_else(|| sanitize_ident(result).into_string())
        } else {
            sanitize_ident(raw_field).into_string()
        };
        let storage = self.bitfield_storage_member(op);
        let field = storage
            .as_ref()
            .map(|(field, _)| field.clone())
            .unwrap_or_else(|| logical_field.clone());
        let field_ty = storage.as_ref().map(|(_, ty)| ty.clone()).or_else(|| {
            self.member_field_type(base_ptr, &logical_field)
                .or_else(|| self.member_field_type_from_op(op, &logical_field))
        });
        let unsafe_access = self.place_expr(base_ptr).is_none()
            || self.ptr_requires_unsafe(base_ptr)
            || self.op_base_is_union(op);
        self.member_ptrs.insert(
            result.clone(),
            MemberPtr {
                base,
                field,
                field_ty,
                unsafe_access,
                bitfield_storage: storage.is_some(),
            },
        );
    }

    fn bitfield_storage_member(&self, op: &Op) -> Option<(String, Type)> {
        let record_name = op
            .ty
            .as_deref()
            .and_then(split_top_level_arrow)
            .and_then(|(inputs, _)| inputs.trim().strip_prefix('(')?.strip_suffix(')'))
            .and_then(|inputs| split_top_level(inputs, ',').first().copied())
            .and_then(cir_ptr_pointee)
            .map(|ty| {
                self.parent
                    .aliases
                    .get(ty)
                    .map(String::as_str)
                    .unwrap_or(ty)
            })
            .and_then(cir_record_name)?;
        let record = self
            .parent
            .records
            .get(&sanitize_ident(record_name).into_string())?;
        let fields = self.parent.bitfield_storage_fields(record)?;
        let index = aggregate_member_index(op)?;
        let field = fields.get(index)?;
        let ty = op_result_type(op)
            .and_then(cir_ptr_pointee)
            .map(|ty| self.parent.rust_type(ty))?;
        Some((sanitize_ident(&field.name).into_string(), ty))
    }

    fn lower_extract_member(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        let Some(base) = op.operands.first() else {
            return;
        };
        let Some(field) = self.value_member_field(op, 0) else {
            self.emit_todo("cir.extract_member");
            return;
        };
        self.materialize_expr(
            result,
            Expr::Field {
                base: Box::new(self.operand_expr(base)),
                field,
            },
            op_result_type(op),
        );
    }

    fn lower_insert_member(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        if op.operands.len() < 2 {
            return;
        }
        let Some(index) = aggregate_member_index(op) else {
            self.emit_todo("cir.insert_member");
            return;
        };
        let Some(record_name) = op_result_type(op)
            .map(|ty| self.parent.rust_type(ty))
            .and_then(|ty| match ty {
                Type::Custom(name) => Some(name),
                _ => None,
            })
        else {
            self.emit_todo("cir.insert_member");
            return;
        };
        let Some(record) = self.parent.records.get(&record_name) else {
            self.emit_todo("cir.insert_member");
            return;
        };
        if record.kind != RecordKind::Struct {
            self.emit_todo("cir.insert_member");
            return;
        }
        if index >= record.fields.len() {
            self.emit_todo("cir.insert_member");
            return;
        }
        let base = self.operand_expr(&op.operands[0]);
        let value = self.operand_expr(&op.operands[1]);
        let fields = record
            .fields
            .iter()
            .enumerate()
            .map(|(i, field)| {
                let field = sanitize_ident(&field.name).into_string();
                let expr = if i == index {
                    value.clone()
                } else {
                    Expr::Field {
                        base: Box::new(base.clone()),
                        field: field.clone(),
                    }
                };
                (field, expr)
            })
            .collect();
        self.materialize_expr(
            result,
            wrap_record_lit(
                record,
                Expr::StructLit {
                    name: record_lit_name(record),
                    fields,
                },
            ),
            op_result_type(op),
        );
    }

    fn value_member_field(&self, op: &Op, operand_index: usize) -> Option<String> {
        let index = aggregate_member_index(op)?;
        let record_ty = op_operand_types(op.ty.as_deref()?)
            .get(operand_index)
            .copied()?;
        let Type::Custom(record_name) = self.parent.rust_type(record_ty) else {
            return None;
        };
        self.parent
            .records
            .get(&record_name)?
            .fields
            .get(index)
            .map(|field| sanitize_ident(&field.name).into_string())
    }

    fn coerce_store_value(&self, ptr: &str, value: Expr, value_operand: &str) -> Expr {
        if self
            .slot_types
            .get(ptr)
            .is_some_and(|ty| matches!(ty, Type::FnPtr { .. }))
            && self.is_function_pointer_none_expr(&value)
        {
            return Expr::Value(RustValue::None);
        }
        let Some(member) = self.member_ptrs.get(ptr) else {
            return value;
        };
        let Some(field_ty) = &member.field_ty else {
            return value;
        };
        if let Type::Custom(enum_name) = field_ty
            && let Some(enm) = self.parent.enums.get(enum_name)
        {
            if let Some(value_int) = self.store_int_value(&value, value_operand)
                && let Some(variant) = enm
                    .variants
                    .iter()
                    .find(|variant| i128::from(variant.value) == value_int)
            {
                return Expr::Path(Path::new([
                    Ident::from(enum_name.as_str()),
                    Ident::from(sanitize_ident(&variant.name).as_str()),
                ]));
            }
            return Self::unsafe_expr(Expr::Call {
                binding: crate::function_identity::CallBinding::Generated,
                func: Box::new(Expr::Path(Path::new(
                    ["std", "mem", "transmute"].map(Ident::from),
                ))),
                args: vec![value],
            });
        }
        if self.parent.type_is_enum_ptr(field_ty) {
            return Expr::Cast {
                expr: Box::new(value),
                ty: field_ty.clone(),
            };
        }
        value
    }

    fn store_int_value(&self, value: &Expr, value_operand: &str) -> Option<i128> {
        expr_int_value(value).or_else(|| {
            if let Some(value) = self.const_int_values.get(value_operand) {
                return Some(*value);
            }
            let Val::Expr(expr) = self.values.get(value_operand)? else {
                return None;
            };
            expr_int_value(expr)
        })
    }

    fn member_field_type(&self, base_ptr: &str, field: &str) -> Option<Type> {
        if let Some(Type::Custom(record_name)) = self.slot_types.get(base_ptr) {
            return self.record_field_type_by_name(record_name, field);
        }
        if let Some(Type::Custom(record_name)) = self
            .member_ptrs
            .get(base_ptr)
            .and_then(|member| member.field_ty.as_ref())
        {
            return self.record_field_type_by_name(record_name, field);
        }
        None
    }

    fn member_field_type_from_op(&self, op: &Op, field: &str) -> Option<Type> {
        let base_ty = op_operand_types(op.ty.as_deref()?).first().copied()?;
        let record_name = cir_ptr_pointee(base_ty).and_then(cir_record_name)?;
        self.record_field_type_by_name(&sanitize_ident(record_name).into_string(), field)
    }

    fn record_field_type_by_name(&self, record_name: &str, field: &str) -> Option<Type> {
        let record = self.parent.records.get(record_name)?;
        record
            .fields
            .iter()
            .find(|candidate| sanitize_ident(&candidate.name).as_str() == field)
            .map(|candidate| self.parent.record_field_type(&candidate.ty))
    }

    fn lower_set_bitfield(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        if op.operands.len() < 2 {
            return;
        }
        let ty = op_result_type(op);
        let value = self.operand_expr(&op.operands[1]);
        let value = ty.map_or(value.clone(), |ty| Expr::Cast {
            expr: Box::new(value),
            ty: self.parent.rust_type(ty),
        });
        let trunc = self.truncate_bitfield_expr(op, value, ty);
        self.materialize_expr(result, trunc, ty);
        let stored = self.operand_expr(result);
        let (place, needs_unsafe) = self.bitfield_place(&op.operands[0]);
        let stored = if let Some(member) = self.member_ptrs.get(&op.operands[0])
            && member.bitfield_storage
            && let Some(field_ty) = &member.field_ty
            && let Some((size, offset)) = self.bitfield_size_offset(op)
        {
            let mask = bitfield_mask(size);
            let shifted_mask = mask << offset;
            let retained = Expr::Binary {
                op: BinOp::BitAnd,
                lhs: Box::new(place.clone()),
                rhs: Box::new(Expr::Unary {
                    op: UnaryOp::Not,
                    expr: Box::new(Expr::Value(RustValue::U128(shifted_mask))),
                }),
            };
            let inserted = Expr::Binary {
                op: BinOp::Shl,
                lhs: Box::new(Expr::Binary {
                    op: BinOp::BitAnd,
                    lhs: Box::new(Expr::Cast {
                        expr: Box::new(stored),
                        ty: field_ty.clone(),
                    }),
                    rhs: Box::new(Expr::Value(RustValue::U128(mask))),
                }),
                rhs: Box::new(int_value_expr(i128::from(offset))),
            };
            Expr::Binary {
                op: BinOp::BitOr,
                lhs: Box::new(retained),
                rhs: Box::new(inserted),
            }
        } else {
            stored
        };
        if needs_unsafe {
            self.push_unsafe_assign(place, stored);
        } else {
            self.push_assign(place, stored);
        }
    }

    fn lower_get_bitfield(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        let Some(ptr) = op.operands.first() else {
            return;
        };
        let ty = op_result_type(op);
        let (place, needs_unsafe) = self.bitfield_place(ptr);
        let mut read = if needs_unsafe {
            Self::unsafe_expr(place)
        } else {
            place
        };
        if self
            .member_ptrs
            .get(ptr)
            .is_some_and(|member| member.bitfield_storage)
            && let Some((size, offset)) = self.bitfield_size_offset(op)
        {
            read = Expr::Binary {
                op: BinOp::BitAnd,
                lhs: Box::new(Expr::Binary {
                    op: BinOp::Shr,
                    lhs: Box::new(read),
                    rhs: Box::new(int_value_expr(i128::from(offset))),
                }),
                rhs: Box::new(Expr::Value(RustValue::U128(bitfield_mask(size)))),
            };
            if let Some(ty) = ty {
                read = Expr::Cast {
                    expr: Box::new(read),
                    ty: self.parent.rust_type(ty),
                };
            }
        }
        let expr = self.truncate_bitfield_expr(op, read, ty);
        self.materialize_expr(result, expr, ty);
    }

    fn bitfield_place(&self, ptr: &str) -> (Expr, bool) {
        match self.place_expr(ptr) {
            Some(place) => (place, self.ptr_requires_unsafe(ptr)),
            None => (
                Expr::Unary {
                    op: UnaryOp::Deref,
                    expr: Box::new(self.pointer_operand_expr(ptr)),
                },
                true,
            ),
        }
    }

    // shift up then arithmetic-shift down masks to `size` bits, sign-extending signed types.
    fn truncate_bitfield_expr(&self, op: &Op, expr: Expr, ty: Option<&str>) -> Expr {
        let bits = ty
            .map(|ty| self.parent.rust_type(ty))
            .and_then(|t| int_bits(&t.render()));
        match (self.bitfield_size(op), bits) {
            (Some(size), Some(bits)) if size < bits => {
                let sh = Box::new(Expr::Value(RustValue::I64((bits - size) as i64)));
                Expr::Binary {
                    op: BinOp::Shr,
                    lhs: Box::new(Expr::Binary {
                        op: BinOp::Shl,
                        lhs: Box::new(expr),
                        rhs: sh.clone(),
                    }),
                    rhs: sh,
                }
            }
            _ => expr,
        }
    }

    fn bitfield_size(&self, op: &Op) -> Option<u32> {
        self.bitfield_size_offset(op).map(|(size, _)| size)
    }

    fn bitfield_size_offset(&self, op: &Op) -> Option<(u32, u32)> {
        let raw = attr_str(op, "bitfield_info")?;
        let resolved = self.parent.aliases.get(raw).map_or(raw, String::as_str);
        Some((
            bitfield_info_number(resolved, "size = ")?,
            bitfield_info_number(resolved, "offset = ")?,
        ))
    }

    fn lower_get_element(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        if op.operands.len() < 2 {
            return;
        }
        let base_ptr = &op.operands[0];
        let index = self.operand_expr(&op.operands[1]);
        let unbounded = self
            .member_ptrs
            .get(base_ptr)
            .is_some_and(|member| matches!(&member.field_ty, Some(Type::Array { len: 0, .. })));
        if let Some(Val::Global(name)) = self.values.get(base_ptr).cloned() {
            if let Some(labels) = self.parent.block_addr_globals.get(&name) {
                self.block_addr_element_ptrs.insert(
                    result.clone(),
                    BlockAddrElementPtr {
                        labels: labels.clone(),
                        index: index.clone(),
                    },
                );
            }
            let elem_ty = op_result_type(op)
                .and_then(cir_ptr_inner)
                .map(|ty| self.parent.rust_type(ty));
            let declared_len = op_operand_types(op.ty.as_deref().unwrap_or(""))
                .into_iter()
                .next()
                .and_then(cir_ptr_inner)
                .and_then(parse_cir_array_type)
                .map(|(_, len)| len as usize);
            if let Some(base) = self.global_array_literal_expr(&name, elem_ty, declared_len) {
                self.element_ptrs.insert(
                    result.clone(),
                    ElementPtr {
                        base,
                        index,
                        unsafe_access: false,
                        unbounded: false,
                    },
                );
                return;
            }
        }
        let base = self.place_or_deref_expr(base_ptr);
        let unsafe_access =
            unbounded || self.place_expr(base_ptr).is_none() || self.ptr_requires_unsafe(base_ptr);
        self.element_ptrs.insert(
            result.clone(),
            ElementPtr {
                base,
                index,
                unsafe_access,
                unbounded,
            },
        );
    }

    fn global_array_literal_expr(
        &self,
        name: &str,
        elem_ty: Option<Type>,
        declared_len: Option<usize>,
    ) -> Option<Expr> {
        if let Some(bytes) = self.parent.strings.get(name) {
            let elem_ty = elem_ty.unwrap_or(Type::Prim(Prim::I8));
            let len = declared_len.unwrap_or(bytes.len());
            let elems = byte_array_elems(
                bytes,
                &Type::Array {
                    elem: Box::new(elem_ty),
                    len: len as u64,
                },
            );
            Some(render_array_literal_expr(
                &elems,
                len,
                Expr::Value(RustValue::I64(0)),
            ))
        } else if let Some(elems) = self.parent.const_arrays.get(name) {
            let len = declared_len.unwrap_or(elems.len());
            Some(render_array_literal_expr(
                elems,
                len,
                Expr::Value(RustValue::I64(0)),
            ))
        } else {
            None
        }
    }

    fn ptr_requires_unsafe(&self, ptr: &str) -> bool {
        self.global_name(ptr).is_some()
            || self
                .member_ptrs
                .get(ptr)
                .is_some_and(|member| member.unsafe_access)
            || self
                .element_ptrs
                .get(ptr)
                .is_some_and(|element| element.unsafe_access)
    }

    fn op_base_is_union(&self, op: &Op) -> bool {
        op.ty
            .as_deref()
            .and_then(|ty| op_operand_types(ty).into_iter().next())
            .and_then(cir_ptr_inner)
            .is_some_and(|ty| self.parent.cir_type_is_union(ty))
    }

    fn lower_cast(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        let Some(src) = op.operands.first() else {
            return;
        };
        if let Some(slot) = self.va_places.get(src).cloned() {
            self.va_places.insert(result.clone(), slot.clone());
            self.values
                .insert(result.clone(), Val::Expr(Expr::Var(slot.into())));
            return;
        }
        let result_ty = op_result_type(op).unwrap_or("");
        let operand_ty = op_operand_types(op.ty.as_deref().unwrap_or(""))
            .into_iter()
            .next()
            .unwrap_or("");
        let value = match self.values.get(src).cloned() {
            Some(Val::Global(name))
                if result_ty.starts_with("!cir.ptr<")
                    && self.parent.strings.contains_key(&name) =>
            {
                let bytes = self.parent.strings[&name].clone();
                Val::Expr(Expr::Cast {
                    expr: Box::new(Expr::MethodCall {
                        recv: Box::new(Expr::ByteStr(bytes)),
                        method: "as_ptr".into(),
                        args: Vec::new(),
                    }),
                    ty: self.parent.rust_type(result_ty),
                })
            }
            Some(Val::Global(name))
                if result_ty.starts_with("!cir.ptr<")
                    && self.parent.const_arrays.contains_key(&name) =>
            {
                let elems = &self.parent.const_arrays[&name];
                let (elem_ty, len) = cir_ptr_inner(operand_ty)
                    .and_then(parse_cir_array_type)
                    .map_or((Type::Prim(Prim::I32), elems.len()), |(elem, len)| {
                        (self.parent.rust_type(&elem), len as usize)
                    });
                let mut typed: Vec<Expr> = elems.clone();
                if let Some(first) = typed.first_mut() {
                    *first = Expr::Cast {
                        expr: Box::new(first.clone()),
                        ty: elem_ty,
                    };
                }
                let ptr_ty = self.parent.rust_type(result_ty);
                Val::Expr(Expr::Cast {
                    expr: Box::new(Expr::ArrayPtr {
                        array: Box::new(render_array_literal_expr(
                            &typed,
                            len,
                            Expr::Value(RustValue::I64(0)),
                        )),
                        mutable: false,
                    }),
                    ty: ptr_ty,
                })
            }
            Some(Val::Global(_)) if !result_ty.starts_with("!cir.ptr<") => {
                let Some(name) = self.global_name(src) else {
                    self.emit_todo("cir.cast (global ptrtoint)");
                    return;
                };
                Val::Expr(Expr::Cast {
                    expr: Box::new(Expr::AddrOf {
                        mutable: false,
                        expr: Box::new(Expr::Var(name.into())),
                    }),
                    ty: self.parent.rust_type(result_ty),
                })
            }
            Some(Val::Global(name)) => Val::Global(name),
            _ if self
                .slot_types
                .get(src)
                .is_some_and(|ty| matches!(ty, Type::Array { .. })) =>
            {
                let array_ptr = Expr::ArrayPtr {
                    array: Box::new(
                        self.slot_receiver(src)
                            .unwrap_or_else(|| self.operand_expr(src)),
                    ),
                    mutable: true,
                };
                if result_ty.starts_with("!cir.ptr<") {
                    Val::Expr(array_ptr)
                } else {
                    Val::Expr(Expr::Cast {
                        expr: Box::new(array_ptr),
                        ty: self.parent.rust_type(result_ty),
                    })
                }
            }
            _ if is_long_double(result_ty) && !is_long_double(operand_ty) => {
                Val::Expr(Expr::Call {
                    binding: crate::function_identity::CallBinding::Generated,
                    func: Box::new(Expr::Var(LONG_DOUBLE_TY.into())),
                    args: vec![Expr::Cast {
                        expr: Box::new(self.operand_expr(src)),
                        ty: crate::rust_ast::Type::Prim(Prim::F64),
                    }],
                })
            }
            _ if is_long_double(operand_ty) && result_ty == "!cir.bool" => {
                Val::Expr(Expr::Binary {
                    op: BinOp::Ne,
                    lhs: Box::new(Expr::TupleField {
                        base: Box::new(self.operand_expr(src)),
                        index: 0,
                    }),
                    rhs: Box::new(Expr::Value(RustValue::Float(0.0))),
                })
            }
            _ if is_long_double(operand_ty) && !is_long_double(result_ty) => {
                Val::Expr(Expr::Cast {
                    expr: Box::new(Expr::TupleField {
                        base: Box::new(self.operand_expr(src)),
                        index: 0,
                    }),
                    ty: self.parent.rust_type(result_ty),
                })
            }
            _ if result_ty.starts_with("!cir.ptr<") && operand_ty.starts_with("!cir.ptr<") => {
                Val::Expr(Expr::Cast {
                    expr: Box::new(self.pointer_operand_expr(src)),
                    ty: self.parent.rust_type(result_ty),
                })
            }
            _ if operand_ty.starts_with("!cir.ptr<")
                && result_ty != "!cir.bool"
                && !is_cir_function_pointer_type(operand_ty) =>
            {
                Val::Expr(Expr::Cast {
                    expr: Box::new(self.pointer_operand_expr(src)),
                    ty: self.parent.rust_type(result_ty),
                })
            }
            _ if result_ty.starts_with("!cir.ptr<!cir.func<") => {
                let ptr_ty = self.parent.rust_type(result_ty);
                Val::Expr(Expr::Transmute {
                    from: Type::Prim(Prim::Usize),
                    to: ptr_ty,
                    expr: Box::new(Expr::Cast {
                        expr: Box::new(self.operand_expr(src)),
                        ty: Type::Prim(Prim::Usize),
                    }),
                })
            }
            _ if result_ty == "!cir.bool" && is_cir_function_pointer_type(operand_ty) => {
                Val::Expr(Expr::MethodCall {
                    recv: Box::new(self.function_pointer_operand_expr(src)),
                    method: "is_some".into(),
                    args: Vec::new(),
                })
            }
            _ if result_ty == "!cir.bool" && operand_ty != "!cir.bool" => Val::Expr(Expr::Binary {
                op: BinOp::Ne,
                lhs: Box::new(self.operand_expr(src)),
                rhs: Box::new(zero_for_cir_type(operand_ty)),
            }),
            _ if result_ty == operand_ty => Val::Expr(self.operand_expr(src)),
            _ => Val::Expr(Expr::Cast {
                expr: Box::new(self.operand_expr(src)),
                ty: self.parent.rust_type(result_ty),
            }),
        };
        self.values.insert(result.clone(), value);
    }

    fn lower_ptr_diff(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        if op.operands.len() < 2 {
            return;
        }
        let operand_types = op_operand_types(op.ty.as_deref().unwrap_or(""));
        let lhs = if operand_types
            .first()
            .is_some_and(|ty| is_cir_function_pointer_type(ty))
        {
            self.function_pointer_byte_operand_expr(&op.operands[0])
        } else {
            self.pointer_operand_expr(&op.operands[0])
        };
        let rhs = if operand_types
            .get(1)
            .is_some_and(|ty| is_cir_function_pointer_type(ty))
        {
            self.function_pointer_byte_operand_expr(&op.operands[1])
        } else {
            self.pointer_operand_expr(&op.operands[1])
        };
        let ty = op_result_type(op)
            .map(|ty| self.parent.rust_type(ty))
            .unwrap_or(Type::Prim(Prim::I64));
        self.materialize_expr(
            result,
            Self::unsafe_expr(Expr::Cast {
                expr: Box::new(Expr::MethodCall {
                    recv: Box::new(lhs),
                    method: "offset_from".into(),
                    args: vec![rhs],
                }),
                ty,
            }),
            op_result_type(op),
        );
    }

    fn lower_ptr_stride(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        if op.operands.len() < 2 {
            return;
        }
        let operand_types = op_operand_types(op.ty.as_deref().unwrap_or(""));
        let function_pointer_stride = operand_types
            .first()
            .is_some_and(|ty| is_cir_function_pointer_type(ty))
            && op_result_type(op).is_some_and(is_cir_function_pointer_type);
        let base = if function_pointer_stride {
            self.function_pointer_byte_operand_expr(&op.operands[0])
        } else {
            self.pointer_operand_expr(&op.operands[0])
        };
        let index = self.operand_expr(&op.operands[1]);
        let (method, args) = self.ptr_stride_method_and_args(op, index);
        let stride = Self::unsafe_expr(Expr::MethodCall {
            recv: Box::new(base),
            method,
            args,
        });
        let value = if function_pointer_stride {
            Expr::Transmute {
                from: Type::Ptr {
                    mutable: false,
                    inner: Box::new(Type::Prim(Prim::U8)),
                },
                to: self
                    .parent
                    .rust_type(op_result_type(op).expect("checked above")),
                expr: Box::new(stride),
            }
        } else {
            stride
        };
        self.values.insert(result.clone(), Val::Expr(value));
    }

    fn ptr_stride_method_and_args(&self, op: &Op, index: Expr) -> (String, Vec<Expr>) {
        if let Some(index_operand) = op.operands.get(1) {
            if let Some(value) = self.const_int_values.get(index_operand)
                && *value >= 0
            {
                return ("add".into(), vec![int_value_expr(*value)]);
            }
            if op_operand_types(op.ty.as_deref().unwrap_or(""))
                .get(1)
                .is_some_and(|ty| self.cir_int_is_unsigned(ty))
            {
                return (
                    "add".into(),
                    vec![Expr::Cast {
                        expr: Box::new(index),
                        ty: Type::Prim(Prim::Usize),
                    }],
                );
            }
        }
        (
            "offset".into(),
            vec![Expr::Cast {
                expr: Box::new(index),
                ty: Type::Prim(Prim::Isize),
            }],
        )
    }

    fn cir_int_is_unsigned(&self, ty: &str) -> bool {
        let resolved = self.parent.aliases.get(ty).map_or(ty, String::as_str);
        if let Some((signed, _)) = parse_cir_int_type(resolved) {
            return !signed;
        }
        resolved
            .trim()
            .strip_prefix("!cir.int<")
            .and_then(|rest| rest.split(',').next())
            .is_some_and(|sign| sign.trim() == "u")
    }

    fn lower_call(&mut self, op: &Op) {
        let operand_types = op_operand_types(op.ty.as_deref().unwrap_or(""));
        let direct_callee =
            attr_str(op, "callee").map(|callee| callee.trim_start_matches('@').to_string());
        let weak_ref_target = direct_callee
            .as_ref()
            .and_then(|callee| self.parent.weak_refs.get(callee))
            .cloned();
        let external_weak_call = weak_ref_target
            .as_ref()
            .is_some_and(|target| self.parent.external_weak_targets.contains(target));
        let direct_callee = weak_ref_target.clone().or(direct_callee);
        let mut binding = if weak_ref_target.is_some() {
            crate::function_identity::CallBinding::Generated
        } else {
            self.parent.call_binding(op, direct_callee.is_some())
        };
        if binding.known().is_none()
            && let Some(callee) = direct_callee.as_deref()
            && let Some(identity @ FunctionIdentity::Known(_)) = self
                .parent
                .known_functions
                .get(canonical_c23_libc_symbol(callee))
                .copied()
                .or_else(|| c23_redirected_function(callee))
        {
            binding = crate::function_identity::CallBinding::Direct {
                identity,
                canonical_type: match binding {
                    crate::function_identity::CallBinding::Direct { canonical_type, .. } => {
                        canonical_type
                    }
                    crate::function_identity::CallBinding::Indirect
                    | crate::function_identity::CallBinding::Generated => None,
                },
            };
        }
        let (callee_name, callee_expr, arg_operands, arg_types) =
            if let Some(callee) = direct_callee {
                let callee_expr = if external_weak_call {
                    Expr::MethodCall {
                        recv: Box::new(Expr::Var(callee.clone().into())),
                        method: "unwrap".into(),
                        args: vec![],
                    }
                } else {
                    Expr::Var(callee.clone().into())
                };
                (
                    callee.clone(),
                    callee_expr,
                    op.operands.as_slice(),
                    operand_types.as_slice(),
                )
            } else {
                let Some((callee_operand, arg_operands)) = op.operands.split_first() else {
                    return;
                };
                (
                    String::new(),
                    Expr::MethodCall {
                        recv: Box::new(self.operand_expr(callee_operand)),
                        method: "unwrap".into(),
                        args: vec![],
                    },
                    arg_operands,
                    operand_types.get(1..).unwrap_or(&[]),
                )
            };
        let args = arg_operands
            .iter()
            .zip(arg_types.iter().copied())
            .map(|(operand, ty)| self.call_arg_expr(operand, ty))
            .collect::<Vec<_>>();
        // long-double libc shims use custom helper ABIs, so they stay on raw paths.
        if callee_name == "strtold"
            && self
                .parent
                .extern_returns
                .get(&callee_name)
                .and_then(|ret| ret.as_deref())
                == Some(LONG_DOUBLE_TY)
        {
            if let Some(result) = op.results.first() {
                let name = self.next_temp();
                let a0 = args
                    .first()
                    .cloned()
                    .unwrap_or(Expr::Value(RustValue::NullPtr));
                let a1 = args
                    .get(1)
                    .cloned()
                    .unwrap_or(Expr::Value(RustValue::NullPtr));
                self.push_stmt(Stmt::Let {
                    name: name.clone(),
                    mutable: true,
                    ty: Some(Type::Prim(Prim::F64)),
                    init: Some(Expr::Value(RustValue::Float(0.0))),
                });
                let i8_ptr = Type::Ptr {
                    mutable: true,
                    inner: Box::new(Type::Prim(Prim::I8)),
                };
                let i8_ptr_ptr = Type::Ptr {
                    mutable: true,
                    inner: Box::new(i8_ptr.clone()),
                };
                let call = Expr::Call {
                    binding: crate::function_identity::CallBinding::Generated,
                    func: Box::new(Expr::Var("__slate_strtold".into())),
                    args: vec![
                        Expr::Cast {
                            expr: Box::new(a0),
                            ty: i8_ptr,
                        },
                        Expr::Cast {
                            expr: Box::new(a1),
                            ty: i8_ptr_ptr,
                        },
                        Expr::AddrOf {
                            mutable: true,
                            expr: Box::new(Expr::Var(name.clone().into())),
                        },
                    ],
                };
                self.push_stmt(Self::unsafe_stmt(Stmt::Expr(call)));
                self.values.insert(
                    result.to_string(),
                    Val::Expr(Expr::Call {
                        binding: crate::function_identity::CallBinding::Generated,
                        func: Box::new(Expr::Var(LONG_DOUBLE_TY.into())),
                        args: vec![Expr::Var(name.into())],
                    }),
                );
            }
            return;
        }
        if self.try_fixed_long_double_call_shim(op, &callee_name, &args, arg_types)
            || self.try_format_call_shims(op, &callee_name, &args, arg_types)
        {
            return;
        }
        let call = Expr::Call {
            binding,
            func: Box::new(callee_expr),
            args: if let Some(param_types) = self.parent.externs.get(&callee_name).cloned() {
                args.into_iter()
                    .enumerate()
                    .map(|(i, arg)| match param_types.get(i) {
                        Some(_)
                            if arg_types
                                .get(i)
                                .is_some_and(|t| is_cir_function_pointer_type(t)) =>
                        {
                            arg
                        }
                        Some(ty) => Expr::Cast {
                            expr: Box::new(arg),
                            ty: ty.clone(),
                        },
                        None => arg,
                    })
                    .collect()
            } else {
                args
            },
        };
        let expr = if is_complex_runtime_call(&callee_name) {
            self.parent.uses_complex.set(true);
            Self::unsafe_expr(call)
        } else if self.parent.externs.contains_key(&callee_name)
            || self.parent.variadic_defs.contains(&callee_name)
            || self.parent.unsafe_functions.contains(&callee_name)
        {
            Self::unsafe_expr(call)
        } else {
            call
        };

        if let Some(result) = op.results.first() {
            self.materialize_expr(result, expr, op_result_type(op));
        } else {
            self.push_stmt(Stmt::Expr(expr));
        }
    }

    fn try_format_call_shims(
        &mut self,
        op: &Op,
        callee_name: &str,
        args: &[Expr],
        arg_types: &[&str],
    ) -> bool {
        let Some(fmt_index) = self.format_string_arg_index(callee_name, arg_types) else {
            return false;
        };
        self.try_long_double_i32_shim(op, callee_name, args, arg_types, fmt_index + 1)
    }

    fn try_fixed_long_double_call_shim(
        &mut self,
        op: &Op,
        callee_name: &str,
        args: &[Expr],
        arg_types: &[&str],
    ) -> bool {
        let Some(fixed) = self.parent.externs.get(callee_name) else {
            return false;
        };
        if args.len() != fixed.len() || op_result_type(op) != Some("!s32i") {
            return false;
        }
        self.try_long_double_i32_shim(op, callee_name, args, arg_types, 0)
    }

    fn format_string_arg_index(&self, callee_name: &str, arg_types: &[&str]) -> Option<usize> {
        let fixed = self.parent.externs.get(callee_name)?;
        if arg_types.len() <= fixed.len() {
            return None;
        }
        let fmt_index = fixed.len().checked_sub(1)?;
        is_format_string_arg(arg_types.get(fmt_index)?).then_some(fmt_index)
    }

    fn try_long_double_i32_shim(
        &mut self,
        op: &Op,
        callee_name: &str,
        args: &[Expr],
        arg_types: &[&str],
        prefix: usize,
    ) -> bool {
        if !arg_types[prefix..].iter().any(|ty| is_long_double(ty)) {
            return false;
        }
        let param_types: Vec<Type> = arg_types
            .iter()
            .map(|ty| {
                if is_long_double(ty) {
                    Type::Prim(Prim::F64)
                } else {
                    self.parent.rust_type(ty)
                }
            })
            .collect();
        let tags: Vec<String> = arg_types
            .iter()
            .zip(param_types.iter())
            .map(|(ty, param_ty)| {
                if is_long_double(ty) {
                    "ld".to_string()
                } else {
                    long_double_shim_type_tag(param_ty)
                }
            })
            .collect();
        let shim_name = format!("__slate_{callee_name}__{}", tags.join("_"));
        self.parent
            .long_double_shims
            .entry(shim_name.clone())
            .or_insert_with(|| ExternFnDecl {
                identity: crate::function_identity::FunctionIdentity::Unknown,
                name: shim_name.clone(),
                params: param_types
                    .iter()
                    .enumerate()
                    .map(|(i, ty)| FnParam {
                        name: format!("_{i}"),
                        mutable: false,
                        ty: ty.clone(),
                    })
                    .collect(),
                variadic: false,
                ret: Some(Type::Prim(Prim::I32)),
            });
        let call_args = args
            .iter()
            .zip(arg_types.iter())
            .enumerate()
            .map(|(i, (arg, ty))| {
                if is_long_double(ty) {
                    Expr::Field {
                        base: Box::new(arg.clone()),
                        field: "0".into(),
                    }
                } else {
                    Expr::Cast {
                        expr: Box::new(arg.clone()),
                        ty: param_types[i].clone(),
                    }
                }
            })
            .collect();
        let expr = Self::unsafe_expr(Expr::Call {
            binding: crate::function_identity::CallBinding::Generated,
            func: Box::new(Expr::Var(shim_name.into())),
            args: call_args,
        });
        if let Some(result) = op.results.first() {
            self.materialize_expr(result, expr, op_result_type(op));
        } else {
            self.push_stmt(Stmt::Expr(expr));
        }
        true
    }

    fn byte_ptr_operand(&self, operand: &str, mutable: bool) -> Expr {
        Expr::Cast {
            expr: Box::new(self.pointer_operand_expr(operand)),
            ty: Type::Ptr {
                mutable,
                inner: Box::new(Type::Prim(Prim::U8)),
            },
        }
    }

    fn usize_operand(&self, operand: &str) -> Expr {
        Expr::Cast {
            expr: Box::new(self.operand_expr(operand)),
            ty: Type::Prim(Prim::Usize),
        }
    }

    // cir.libc.memcpy/memmove: (dst, src, len). memmove keeps overlapping copy
    // semantics; both operate byte-wise via *u8 pointers.
    fn lower_mem_copy(&mut self, op: &Op, overlapping: bool) {
        let known = if overlapping {
            crate::function_identity::Known::MemMove
        } else {
            crate::function_identity::Known::MemCpy
        };
        if !self.lower_known_libc_op(op, known) {
            return;
        }
        if op.operands.len() < 3 {
            return;
        }
        let dst = Self::without_empty_unsafe(self.byte_ptr_operand(&op.operands[0], true));
        let src = Self::without_empty_unsafe(self.byte_ptr_operand(&op.operands[1], false));
        let count = self.usize_operand(&op.operands[2]);
        self.push_stmt(Stmt::Expr(Self::unsafe_expr(Expr::PtrCopy {
            src: Box::new(src),
            dst: Box::new(dst),
            count: Box::new(count),
            overlapping,
        })));
    }

    // cir.libc.memset: (dst, val:u8, len); the alignment attr carries no runtime
    // meaning here.
    fn lower_mem_set(&mut self, op: &Op) {
        if !self.lower_known_libc_op(op, crate::function_identity::Known::MemSet) {
            return;
        }
        if op.operands.len() < 3 {
            return;
        }
        let dst = Self::without_empty_unsafe(self.byte_ptr_operand(&op.operands[0], true));
        let val = Expr::Cast {
            expr: Box::new(self.operand_expr(&op.operands[1])),
            ty: Type::Prim(Prim::U8),
        };
        let count = self.usize_operand(&op.operands[2]);
        self.push_stmt(Stmt::Expr(Self::unsafe_expr(Expr::WriteBytes {
            dst: Box::new(dst),
            val: Box::new(val),
            count: Box::new(count),
        })));
    }

    // cir.libc.memchr: (src, pattern:i32, len:u64) -> void*. Backed by a prelude
    // helper so the byte scan stays a single structured call site.
    fn lower_mem_chr(&mut self, op: &Op) {
        if !self.lower_known_libc_op(op, crate::function_identity::Known::MemChr) {
            return;
        }
        let Some(result) = op.results.first() else {
            return;
        };
        if op.operands.len() < 3 {
            return;
        }
        self.parent.uses_memchr.set(true);
        let src = self.pointer_operand_expr(&op.operands[0]);
        let pattern = Expr::Cast {
            expr: Box::new(self.operand_expr(&op.operands[1])),
            ty: Type::Prim(Prim::I32),
        };
        let len = self.usize_operand(&op.operands[2]);
        let call = Expr::Call {
            binding: crate::function_identity::CallBinding::Generated,
            func: Box::new(Expr::Var("__slate_memchr".into())),
            args: vec![src, pattern, len],
        };
        self.materialize_expr(result, call, op_result_type(op));
    }

    fn lower_known_libc_op(&mut self, op: &Op, known: crate::function_identity::Known) -> bool {
        let binding = self.parent.call_binding(op, true);
        if binding.known() == Some(known)
            || matches!(
                binding,
                crate::function_identity::CallBinding::Direct {
                    identity: FunctionIdentity::Unknown,
                    canonical_type: None,
                }
            )
            || self.parent.known_functions.get(known.symbol())
                == Some(&FunctionIdentity::Known(known))
        {
            return true;
        }
        let args = op
            .operands
            .iter()
            .map(|operand| self.operand_expr(operand))
            .collect();
        let call = Expr::Call {
            binding,
            func: Box::new(Expr::Var(known.symbol().into())),
            args,
        };
        let expr = if self.parent.externs.contains_key(known.symbol()) {
            Self::unsafe_expr(call)
        } else {
            call
        };
        if let Some(result) = op.results.first() {
            self.materialize_expr(result, expr, op_result_type(op));
        } else {
            self.push_stmt(Stmt::Expr(expr));
        }
        false
    }

    // Atomic ops lower to real `std::sync::atomic` operations viewed through
    // `AtomicN::from_ptr(store_address(ptr))`, so the existing integer slot is
    // accessed atomically without changing its storage. Integer/bool types map
    // to an atomic wrapper; float/pointer atomics fall back to a non-atomic RMW
    // (std has no atomic float, and atomic pointers need a different shape).
    fn atomic_rust_type(&self, op: &Op) -> Type {
        op_result_type(op)
            .map(|ty| self.parent.rust_type(ty))
            .unwrap_or(Type::Prim(Prim::I32))
    }

    fn lower_atomic_fetch(&mut self, op: &Op) {
        let Some(result) = op.results.first().cloned() else {
            return;
        };
        if op.operands.len() < 2 {
            return;
        }
        let val = self.operand_expr(&op.operands[1]);
        let ty = self.atomic_rust_type(op);
        let binop = attr_int(op, "binop").unwrap_or(0);
        let Some(atomic_ty) = atomic_type(&ty) else {
            // float/pointer atomic: non-atomic read-modify-write fallback.
            self.lower_atomic_fetch_nonatomic(op, &result, val, ty, binop);
            return;
        };
        let fetched = Self::unsafe_expr(Expr::AtomicFetch {
            ty: atomic_ty,
            op: atomic_rmw_op(binop),
            place: AtomicPlace::Ptr(Box::new(self.store_address_expr(&op.operands[0]))),
            value: Box::new(val.clone()),
            ordering: rust_ordering(attr_int(op, "mem_order").unwrap_or(5)),
        });
        if attr_bool(op, "fetch_first") {
            self.materialize_expr(&result, fetched, op_result_type(op));
        } else {
            let old = self.next_temp();
            self.push_stmt(Stmt::Let {
                name: old.clone(),
                mutable: false,
                ty: Some(ty),
                init: Some(fetched),
            });
            let new = atomic_combine(binop, Expr::Var(old.into()), val);
            self.materialize_expr(&result, new, op_result_type(op));
        }
    }

    fn lower_atomic_fetch_nonatomic(
        &mut self,
        op: &Op,
        result: &str,
        val: Expr,
        ty: Type,
        binop: i64,
    ) {
        let addr = self.store_address_expr(&op.operands[0]);
        let old = self.next_temp();
        self.push_stmt(Stmt::Let {
            name: old.clone(),
            mutable: false,
            ty: Some(ty.clone()),
            init: Some(Self::unsafe_expr(Expr::Unary {
                op: UnaryOp::Deref,
                expr: Box::new(addr.clone()),
            })),
        });
        let new = self.next_temp();
        self.push_stmt(Stmt::Let {
            name: new.clone(),
            mutable: false,
            ty: Some(ty),
            init: Some(atomic_combine(binop, Expr::Var(old.clone().into()), val)),
        });
        self.push_unsafe_assign(
            Expr::Unary {
                op: UnaryOp::Deref,
                expr: Box::new(addr),
            },
            Expr::Var(new.clone().into()),
        );
        let bound = if attr_bool(op, "fetch_first") {
            old
        } else {
            new
        };
        self.immutable_temps.insert(bound.clone());
        self.values
            .insert(result.to_string(), Val::Expr(Expr::Var(bound.into())));
    }

    fn lower_atomic_xchg(&mut self, op: &Op) {
        let Some(result) = op.results.first().cloned() else {
            return;
        };
        if op.operands.len() < 2 {
            return;
        }
        let val = self.operand_expr(&op.operands[1]);
        let ty = self.atomic_rust_type(op);
        if let Some(atomic_ty) = atomic_type(&ty) {
            let expr = Self::unsafe_expr(Expr::AtomicSwap {
                ty: atomic_ty,
                place: AtomicPlace::Ptr(Box::new(self.store_address_expr(&op.operands[0]))),
                value: Box::new(val),
                ordering: rust_ordering(attr_int(op, "mem_order").unwrap_or(5)),
            });
            self.materialize_expr(&result, expr, op_result_type(op));
            return;
        }
        let addr = self.store_address_expr(&op.operands[0]);
        let old = self.next_temp();
        self.push_stmt(Stmt::Let {
            name: old.clone(),
            mutable: false,
            ty: Some(ty),
            init: Some(Self::unsafe_expr(Expr::Unary {
                op: UnaryOp::Deref,
                expr: Box::new(addr.clone()),
            })),
        });
        self.push_unsafe_assign(
            Expr::Unary {
                op: UnaryOp::Deref,
                expr: Box::new(addr),
            },
            val,
        );
        self.immutable_temps.insert(old.clone());
        self.values.insert(result, Val::Expr(Expr::Var(old.into())));
    }

    fn lower_atomic_test_and_set(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        let Some(ptr) = op.operands.first() else {
            return;
        };
        let old = Self::unsafe_expr(Expr::AtomicSwap {
            ty: AtomicType::I8,
            place: AtomicPlace::Ptr(Box::new(self.store_address_expr(ptr))),
            value: Box::new(Expr::Value(RustValue::I64(1))),
            ordering: rust_ordering(attr_int(op, "mem_order").unwrap_or(5)),
        });
        self.materialize_expr(
            result,
            Expr::Binary {
                op: BinOp::Ne,
                lhs: Box::new(old),
                rhs: Box::new(Expr::Value(RustValue::I64(0))),
            },
            op_result_type(op),
        );
    }

    fn lower_atomic_clear(&mut self, op: &Op) {
        let Some(ptr) = op.operands.first() else {
            return;
        };
        self.push_stmt(Stmt::Expr(Self::unsafe_expr(Expr::AtomicStore {
            ty: AtomicType::I8,
            place: AtomicPlace::Ptr(Box::new(self.store_address_expr(ptr))),
            value: Box::new(Expr::Value(RustValue::I64(0))),
            ordering: store_ordering(attr_int(op, "mem_order").unwrap_or(5)),
        })));
    }

    fn lower_atomic_cmpxchg(&mut self, op: &Op) {
        if op.operands.len() < 3 || op.results.len() < 2 {
            return;
        }
        let expected = self.operand_expr(&op.operands[1]);
        let desired = self.operand_expr(&op.operands[2]);
        let ty = op_result_types(op)
            .first()
            .map(|ty| self.parent.rust_type(ty))
            .unwrap_or(Type::Prim(Prim::I32));
        if let Some(atomic_ty) = atomic_type(&ty) {
            let res = self.next_temp();
            self.push_stmt(Stmt::Let {
                name: res.clone(),
                mutable: false,
                ty: Some(Type::Generic {
                    name: "Result".into(),
                    args: vec![ty.clone(), ty.clone()],
                }),
                init: Some(Self::unsafe_expr(Expr::AtomicCompareExchange {
                    ty: atomic_ty,
                    place: AtomicPlace::Ptr(Box::new(self.store_address_expr(&op.operands[0]))),
                    expected: Box::new(expected),
                    desired: Box::new(desired),
                    success: rust_ordering(attr_int(op, "succ_order").unwrap_or(5)),
                    failure: load_ordering(attr_int(op, "fail_order").unwrap_or(5)),
                })),
            });
            let old = self.next_temp();
            self.push_stmt(Stmt::Let {
                name: old.clone(),
                mutable: false,
                ty: Some(ty.clone()),
                init: Some(Expr::Match {
                    expr: Box::new(Expr::Var(res.clone().into())),
                    arms: vec![
                        ExprMatchArm {
                            pattern: Pattern::TupleStruct {
                                name: "Ok".into(),
                                fields: vec![Pattern::Binding("v".into())],
                            },
                            value: Expr::Var("v".into()),
                        },
                        ExprMatchArm {
                            pattern: Pattern::TupleStruct {
                                name: "Err".into(),
                                fields: vec![Pattern::Binding("v".into())],
                            },
                            value: Expr::Var("v".into()),
                        },
                    ],
                }),
            });
            let ok = self.next_temp();
            self.push_stmt(Stmt::Let {
                name: ok.clone(),
                mutable: false,
                ty: Some(Type::Prim(Prim::Bool)),
                init: Some(Expr::MethodCall {
                    recv: Box::new(Expr::Var(res.into())),
                    method: "is_ok".into(),
                    args: vec![],
                }),
            });
            self.immutable_temps.insert(old.clone());
            self.immutable_temps.insert(ok.clone());
            self.values
                .insert(op.results[0].clone(), Val::Expr(Expr::Var(old.into())));
            self.values
                .insert(op.results[1].clone(), Val::Expr(Expr::Var(ok.into())));
            return;
        }
        let addr = self.store_address_expr(&op.operands[0]);
        let old = self.next_temp();
        let ok = self.next_temp();
        self.push_stmt(Stmt::Let {
            name: old.clone(),
            mutable: false,
            ty: Some(ty),
            init: Some(Self::unsafe_expr(Expr::Unary {
                op: UnaryOp::Deref,
                expr: Box::new(addr.clone()),
            })),
        });
        self.push_stmt(Stmt::Let {
            name: ok.clone(),
            mutable: false,
            ty: Some(Type::Prim(Prim::Bool)),
            init: Some(Expr::Binary {
                op: BinOp::Eq,
                lhs: Box::new(Expr::Var(old.clone().into())),
                rhs: Box::new(expected),
            }),
        });
        self.push_stmt(Stmt::If {
            cond: Expr::Var(ok.clone().into()),
            then_body: vec![Self::indent_stmt(Self::unsafe_stmt(Self::assign_stmt(
                Expr::Unary {
                    op: UnaryOp::Deref,
                    expr: Box::new(addr),
                },
                desired,
            )))],
            else_body: Vec::new(),
        });
        self.immutable_temps.insert(old.clone());
        self.immutable_temps.insert(ok.clone());
        self.values
            .insert(op.results[0].clone(), Val::Expr(Expr::Var(old.into())));
        self.values
            .insert(op.results[1].clone(), Val::Expr(Expr::Var(ok.into())));
    }

    fn lower_atomic_fence(&mut self, op: &Op) {
        let ordering = attr_int(op, "ordering").unwrap_or(5);
        if ordering == 0 {
            return;
        }
        self.push_stmt(Stmt::Expr(Expr::AtomicFence {
            ordering: rust_ordering(ordering),
        }));
    }

    fn atomic_load_expr(&self, op: &Op, ptr: &str) -> Option<Expr> {
        let mem_order = attr_int(op, "mem_order")?;
        let ty = op_result_type(op).map(|ty| self.parent.rust_type(ty))?;
        let atomic_ty = atomic_type(&ty)?;
        Some(Self::unsafe_expr(Expr::AtomicLoad {
            ty: atomic_ty,
            place: AtomicPlace::Ptr(Box::new(self.store_address_expr(ptr))),
            ordering: load_ordering(mem_order),
        }))
    }

    fn try_atomic_store(
        &mut self,
        op: &Op,
        ptr: &str,
        value_ty: Option<&str>,
        value: Expr,
    ) -> bool {
        let Some(mem_order) = attr_int(op, "mem_order") else {
            return false;
        };
        let Some(wrapper) = value_ty
            .map(|ty| self.parent.rust_type(ty))
            .as_ref()
            .and_then(atomic_type)
        else {
            return false;
        };
        self.push_stmt(Stmt::Expr(Self::unsafe_expr(Expr::AtomicStore {
            ty: wrapper,
            place: AtomicPlace::Ptr(Box::new(self.store_address_expr(ptr))),
            value: Box::new(value),
            ordering: store_ordering(mem_order),
        })));
        true
    }

    fn lower_va_start(&mut self, op: &Op) {
        let Some(ptr) = op.operands.first() else {
            return;
        };
        let Some(slot) = self.va_places.get(ptr).cloned() else {
            return;
        };
        let args = self
            .va_args_param
            .clone()
            .unwrap_or_else(|| "__slate_va_args".into());
        self.push_assign(
            Expr::Var(slot.into()),
            Expr::MethodCall {
                recv: Box::new(Expr::Var(args.into())),
                method: "clone".into(),
                args: vec![],
            },
        );
    }

    fn lower_va_arg(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        let Some(ptr) = op.operands.first() else {
            return;
        };
        let Some(slot) = self.va_places.get(ptr).cloned() else {
            return;
        };
        let ty = op_result_type(op)
            .map(|ty| self.parent.rust_type(ty))
            .unwrap_or(Type::Prim(Prim::I32));
        self.materialize_expr(
            result,
            Self::unsafe_expr(Expr::MethodCallGeneric {
                recv: Box::new(Expr::Var(slot.into())),
                method: "next_arg".into(),
                type_args: vec![ty],
                args: vec![],
            }),
            op_result_type(op),
        );
    }

    fn lower_return(&mut self, op: &Op) {
        let value = op
            .operands
            .first()
            .map(|operand| self.operand_expr(operand));
        if self.is_main {
            let code = value.unwrap_or(Expr::Value(RustValue::I64(0)));
            let dtor_stmts: Vec<Stmt> = self
                .parent
                .dtor_calls
                .iter()
                .map(|name| hook_call_stmt(name, &self.parent.unsafe_functions))
                .collect();
            for stmt in dtor_stmts {
                self.push_stmt(stmt);
            }
            self.push_stmt(Stmt::Expr(Expr::Call {
                binding: crate::function_identity::CallBinding::Generated,
                func: Box::new(Expr::Path(Path::new(
                    ["std", "process", "exit"].map(Ident::from),
                ))),
                args: vec![Expr::Cast {
                    expr: Box::new(code),
                    ty: Type::Prim(Prim::I32),
                }],
            }));
        } else if let Some(value) = value {
            self.push_stmt(Stmt::Return(Some(value)));
        } else {
            self.push_stmt(Stmt::Return(None));
        }
    }

    fn lower_scope(&mut self, op: &Op) {
        let body = self.capture_body(|this| {
            for region in &op.regions {
                this.lower_region_ops(region);
            }
        });
        self.push_stmt(Stmt::Scope { body });
    }

    fn lower_cleanup_scope(&mut self, op: &Op) {
        let Some(cleanup) = op.regions.get(1) else {
            self.emit_todo("cir.cleanup.scope");
            return;
        };
        let mut saw_stackrestore = false;
        for block in &cleanup.blocks {
            for cleanup_op in &block.ops {
                match cleanup_op.kind() {
                    CirOpKind::Load | CirOpKind::Yield => {}
                    CirOpKind::Stackrestore => saw_stackrestore = true,
                    _ => {
                        self.emit_todo("cir.cleanup.scope");
                        return;
                    }
                }
            }
        }
        if !saw_stackrestore {
            self.emit_todo("cir.cleanup.scope");
            return;
        }
        let body = self.capture_body(|this| {
            if let Some(region) = op.regions.first() {
                this.lower_region_ops(region);
            }
        });
        self.push_stmt(Stmt::Scope { body });
    }

    fn lower_if(&mut self, op: &Op) {
        let Some(cond) = op.operands.first() else {
            self.emit_todo("cir.if");
            return;
        };
        let cond = self.operand_expr(cond);
        let then_body = self.capture_body(|this| {
            if let Some(region) = op.regions.first() {
                this.lower_region_ops(region);
            }
        });
        let has_else = op
            .regions
            .get(1)
            .is_some_and(|region| region.blocks.iter().any(|block| !block.ops.is_empty()));
        let else_body = if has_else {
            self.capture_body(|this| this.lower_region_ops(&op.regions[1]))
        } else {
            Vec::new()
        };
        self.push_stmt(Stmt::If {
            cond,
            then_body,
            else_body,
        });
    }

    fn not_expr(expr: Expr) -> Expr {
        Expr::Unary {
            op: UnaryOp::Not,
            expr: Box::new(expr),
        }
    }

    fn break_stmt(label: Option<Label>) -> IndentStmt {
        IndentStmt {
            depth: 0,
            stmt: Stmt::Break(label),
        }
    }

    fn indent_stmt(stmt: Stmt) -> IndentStmt {
        IndentStmt { depth: 0, stmt }
    }

    fn guard_break(cond: Expr, label: Option<Label>) -> Stmt {
        Stmt::If {
            cond: Self::not_expr(cond),
            then_body: vec![Self::break_stmt(label)],
            else_body: Vec::new(),
        }
    }

    fn capture_body<F>(&mut self, f: F) -> Vec<IndentStmt>
    where
        F: FnOnce(&mut Self),
    {
        let outer_body = std::mem::take(&mut self.body);
        let outer_indent = self.indent;
        self.indent = 0;
        f(self);
        let body = std::mem::take(&mut self.body);
        self.body = outer_body;
        self.indent = outer_indent;
        body
    }

    fn lower_condition_region_expr(&mut self, region: &Region) -> Expr {
        let mut condition = Expr::Value(RustValue::Bool(true));
        for block in &region.blocks {
            for op in &block.ops {
                if op.kind() == CirOpKind::Condition {
                    if let Some(operand) = op.operands.first() {
                        condition = self.operand_expr(operand);
                    }
                } else {
                    self.lower_op(op);
                }
            }
        }
        condition
    }

    fn lower_for_loop_body(
        &mut self,
        op: &Op,
        break_label: Option<Label>,
        continue_label: Option<Label>,
    ) -> Vec<IndentStmt> {
        self.capture_body(|this| {
            let cond = this.lower_condition_region_expr(&op.regions[0]);
            this.push_stmt(Self::guard_break(cond, None));
            if let Some(label) = &continue_label {
                this.loop_stack.push(LoopFrame {
                    break_label: break_label.clone(),
                    continue_label: continue_label.clone(),
                    is_loop: true,
                });
                let body = this.capture_body(|this| this.lower_region_ops(&op.regions[1]));
                this.loop_stack.pop();
                this.push_stmt(Stmt::LabeledBlock {
                    label: label.clone(),
                    body,
                });
            } else {
                this.loop_stack.push(LoopFrame {
                    break_label,
                    continue_label: None,
                    is_loop: true,
                });
                this.lower_region_ops(&op.regions[1]);
                this.loop_stack.pop();
            }
            this.lower_region_ops(&op.regions[2]);
        })
    }

    fn lower_do_loop_body(
        &mut self,
        op: &Op,
        break_label: Option<Label>,
        continue_label: Option<Label>,
    ) -> Vec<IndentStmt> {
        self.capture_body(|this| {
            if let Some(label) = &continue_label {
                let block = this.capture_body(|this| {
                    this.loop_stack.push(LoopFrame {
                        break_label: break_label.clone(),
                        continue_label: continue_label.clone(),
                        is_loop: true,
                    });
                    this.lower_region_ops(&op.regions[0]);
                    this.loop_stack.pop();
                });
                this.push_stmt(Stmt::LabeledBlock {
                    label: label.clone(),
                    body: block,
                });
            } else {
                this.loop_stack.push(LoopFrame {
                    break_label: break_label.clone(),
                    continue_label: None,
                    is_loop: true,
                });
                this.lower_region_ops(&op.regions[0]);
                this.loop_stack.pop();
            }
            let cond = this.lower_condition_region_expr(&op.regions[1]);
            this.push_stmt(Self::guard_break(cond, break_label));
        })
    }

    fn lower_while_loop_body(&mut self, op: &Op) -> Vec<IndentStmt> {
        self.capture_body(|this| {
            let cond = this.lower_condition_region_expr(&op.regions[0]);
            this.push_stmt(Self::guard_break(cond, None));
            this.loop_stack.push(LoopFrame {
                break_label: None,
                continue_label: None,
                is_loop: true,
            });
            this.lower_region_ops(&op.regions[1]);
            this.loop_stack.pop();
        })
    }

    fn lower_switch(&mut self, op: &Op) {
        let Some(selector) = op.operands.first() else {
            self.emit_todo("cir.switch");
            return;
        };
        let Some(region) = op.regions.first() else {
            self.emit_todo("cir.switch");
            return;
        };
        let cases: Vec<_> = region
            .blocks
            .iter()
            .flat_map(|block| &block.ops)
            .filter(|op| op.kind() == CirOpKind::Case)
            .filter_map(switch_case)
            .collect();
        if cases.is_empty() {
            return;
        }

        let n = self.label_counter;
        self.label_counter += 1;
        let label = Label::new(format!("__switch{n}"));
        let selector_name = format!("__switch_value{n}");
        let case_name = format!("__switch_case{n}");
        let default_index = cases.iter().position(|case| case.is_default);
        let fallback = default_index.map(|index| index as i64).unwrap_or(-1);
        let selector = self.operand_expr(selector);

        let mut selector_arms = Vec::new();
        for (index, case) in cases.iter().enumerate() {
            for pattern in &case.patterns {
                selector_arms.push(ExprMatchArm {
                    pattern: pattern.clone(),
                    value: Expr::Value(RustValue::I64(index as i64)),
                });
            }
        }
        selector_arms.push(ExprMatchArm {
            pattern: Pattern::Wildcard,
            value: Expr::Value(RustValue::I64(fallback)),
        });

        let mut case_arms = Vec::new();
        self.loop_stack.push(LoopFrame {
            break_label: Some(label.clone()),
            continue_label: None,
            is_loop: false,
        });
        for (index, case) in cases.iter().enumerate() {
            let mut body = self.capture_body(|this| this.lower_region_ops(case.region));
            if !region_ends_control_flow(case.region) {
                if index + 1 < cases.len() {
                    body.push(Self::indent_stmt(Self::assign_stmt(
                        Expr::Var(case_name.clone().into()),
                        Expr::Value(RustValue::I64((index + 1) as i64)),
                    )));
                    body.push(Self::indent_stmt(Stmt::Continue(Some(label.clone()))));
                } else {
                    body.push(Self::indent_stmt(Stmt::Break(Some(label.clone()))));
                }
            }
            case_arms.push(MatchArm {
                pattern: int_pattern(index as i128),
                body,
            });
        }
        self.loop_stack.pop();
        case_arms.push(MatchArm {
            pattern: Pattern::Wildcard,
            body: vec![Self::indent_stmt(Stmt::Break(Some(label.clone())))],
        });

        let body = vec![
            Self::indent_stmt(Stmt::Let {
                name: selector_name.clone(),
                mutable: false,
                ty: None,
                init: Some(selector),
            }),
            Self::indent_stmt(Stmt::Let {
                name: case_name.clone(),
                mutable: true,
                ty: Some(Type::Prim(Prim::I32)),
                init: Some(Expr::Match {
                    expr: Box::new(Expr::Var(selector_name.into())),
                    arms: selector_arms,
                }),
            }),
            Self::indent_stmt(Stmt::Loop {
                label: Some(label),
                body: vec![Self::indent_stmt(Stmt::Match {
                    expr: Expr::Var(case_name.into()),
                    arms: case_arms,
                })],
            }),
        ];
        self.push_stmt(Stmt::Scope { body });
    }

    fn lower_for(&mut self, op: &Op) {
        if op.regions.len() < 3 {
            self.emit_todo("cir.for");
            return;
        }
        let (break_label, continue_label) = if region_has_direct_continue(&op.regions[1]) {
            let n = self.label_counter;
            self.label_counter += 1;
            (
                Some(Label::new(format!("__loop{n}"))),
                Some(Label::new(format!("__continue{n}"))),
            )
        } else {
            (None, None)
        };
        let body = self.lower_for_loop_body(op, break_label.clone(), continue_label);
        self.push_stmt(Stmt::Loop {
            label: break_label,
            body,
        });
    }

    fn lower_while(&mut self, op: &Op) {
        if op.regions.len() < 2 {
            self.emit_todo("cir.while");
            return;
        }
        let body = self.lower_while_loop_body(op);
        self.push_stmt(Stmt::Loop { label: None, body });
    }

    fn lower_do(&mut self, op: &Op) {
        if op.regions.len() < 2 {
            self.emit_todo("cir.do");
            return;
        }
        let (break_label, continue_label) = if region_has_direct_continue(&op.regions[0]) {
            let n = self.label_counter;
            self.label_counter += 1;
            (
                Some(Label::new(format!("__loop{n}"))),
                Some(Label::new(format!("__continue{n}"))),
            )
        } else {
            (None, None)
        };
        let body = self.lower_do_loop_body(op, break_label.clone(), continue_label);
        self.push_stmt(Stmt::Loop {
            label: break_label,
            body,
        });
    }

    fn lower_break(&mut self) {
        let label = self.loop_stack.last().and_then(|f| f.break_label.clone());
        self.push_stmt(Stmt::Break(label));
    }

    fn lower_continue(&mut self) {
        let label = self
            .loop_stack
            .iter()
            .rev()
            .find(|frame| frame.is_loop)
            .and_then(|f| f.continue_label.clone());
        match label {
            Some(label) => self.push_stmt(Stmt::Break(Some(label))),
            None => self.push_stmt(Stmt::Continue(None)),
        }
    }

    fn lower_dispatch(&mut self, body: &Region, returns_value: bool) {
        let n = self.label_counter;
        self.label_counter += 1;
        let loop_label = Label::new(format!("__dispatch{n}"));
        let state_var = format!("__state{n}");

        let mut label_to_state = BTreeMap::new();
        let mut block_to_state = BTreeMap::new();
        for (i, block) in body.blocks.iter().enumerate() {
            let key = block.label.clone().unwrap_or_else(|| format!("bb{i}"));
            block_to_state.insert(key, i);
            for op in &block.ops {
                if op.kind() == CirOpKind::Label
                    && let Some(label) = attr_str(op, "label")
                {
                    label_to_state.insert(label.to_string(), i);
                }
            }
        }
        self.dispatch = Some(DispatchCtx {
            loop_label: loop_label.clone(),
            state_var: state_var.clone(),
            label_to_state,
            block_to_state,
        });

        for block in &body.blocks {
            for op in &block.ops {
                if op.kind() == CirOpKind::Alloca {
                    self.lower_alloca(op);
                    if let Some(result) = op.results.first() {
                        self.hoisted.insert(result.clone());
                    }
                }
            }
        }

        let mut arms = Vec::new();
        for (i, block) in body.blocks.iter().enumerate() {
            let mut body = self.capture_body(|this| this.lower_block(block));
            if !block_diverges(block) {
                body.push(Self::indent_stmt(Self::assign_stmt(
                    Expr::Var(state_var.clone().into()),
                    Expr::Value(RustValue::I64((i + 1) as i64)),
                )));
                body.push(Self::indent_stmt(Stmt::Continue(Some(loop_label.clone()))));
            }
            arms.push(MatchArm {
                pattern: int_pattern(i as i128),
                body,
            });
        }
        let fallthrough = if returns_value {
            Stmt::Expr(Expr::Macro {
                name: "unreachable".into(),
                args: Vec::new(),
            })
        } else {
            Stmt::Break(Some(loop_label.clone()))
        };
        arms.push(MatchArm {
            pattern: Pattern::Wildcard,
            body: vec![Self::indent_stmt(fallthrough)],
        });
        self.push_stmt(Stmt::Let {
            name: state_var.clone(),
            mutable: true,
            ty: Some(Type::Prim(Prim::I32)),
            init: Some(Expr::Value(RustValue::I64(0))),
        });
        self.push_stmt(Stmt::Loop {
            label: Some(loop_label),
            body: vec![Self::indent_stmt(Stmt::Match {
                expr: Expr::Var(state_var.into()),
                arms,
            })],
        });
        self.dispatch = None;
    }

    fn lower_goto(&mut self, op: &Op) {
        let Some(dispatch) = &self.dispatch else {
            return;
        };
        let target = attr_str(op, "label")
            .and_then(|l| dispatch.label_to_state.get(l))
            .map(|state| {
                (
                    *state,
                    dispatch.state_var.clone(),
                    dispatch.loop_label.clone(),
                )
            });
        match target {
            Some((state, state_var, loop_label)) => {
                self.push_assign(
                    Expr::Var(state_var.into()),
                    Expr::Value(RustValue::I64(state as i64)),
                );
                self.push_stmt(Stmt::Continue(Some(loop_label)));
            }
            None => self.emit_todo("cir.goto: unknown label"),
        }
    }

    fn lower_br(&mut self, op: &Op) {
        let Some(dispatch) = &self.dispatch else {
            return;
        };
        if let Some(target) = op
            .operands
            .first()
            .and_then(|operand| self.indirect_target_values.get(operand))
        {
            let state_var = dispatch.state_var.clone();
            let loop_label = dispatch.loop_label.clone();
            let target = target.clone();
            self.push_assign(Expr::Var(state_var.into()), target);
            self.push_stmt(Stmt::Continue(Some(loop_label)));
            return;
        }
        let target = op
            .successors
            .first()
            .and_then(|bb| dispatch.block_to_state.get(bb))
            .map(|state| {
                (
                    *state,
                    dispatch.state_var.clone(),
                    dispatch.loop_label.clone(),
                )
            });
        match target {
            Some((state, state_var, loop_label)) => {
                self.push_assign(
                    Expr::Var(state_var.into()),
                    Expr::Value(RustValue::I64(state as i64)),
                );
                self.push_stmt(Stmt::Continue(Some(loop_label)));
            }
            None => self.emit_todo("cir.br: unknown successor"),
        }
    }

    fn lower_indirect_br(&mut self, op: &Op) {
        let Some(dispatch) = &self.dispatch else {
            return;
        };
        if let Some(target) = op
            .operands
            .first()
            .and_then(|operand| self.indirect_target_values.get(operand))
        {
            let state_var = dispatch.state_var.clone();
            let loop_label = dispatch.loop_label.clone();
            let target = target.clone();
            self.push_assign(Expr::Var(state_var.into()), target);
            self.push_stmt(Stmt::Continue(Some(loop_label)));
        } else {
            self.lower_unreachable();
        }
    }

    fn materialize_expr(&mut self, result: &str, expr: Expr, cir_ty: Option<&str>) {
        let name = self.next_temp();
        let ty = cir_ty
            .map(|ty| self.parent.rust_type(ty))
            .unwrap_or(Type::Prim(Prim::I32));
        self.push_stmt(Stmt::Let {
            name: name.clone(),
            mutable: false,
            ty: Some(ty),
            init: Some(expr),
        });
        self.immutable_temps.insert(name.clone());
        self.values
            .insert(result.to_string(), Val::Expr(Expr::Var(name.into())));
    }

    fn forward_safe_value(&mut self, value: Expr, cir_ty: Option<&str>) -> Expr {
        let stable = match &value {
            Expr::Value(_) => true,
            Expr::Var(name) => self.immutable_temps.contains(name.as_str()),
            _ => false,
        };
        if stable {
            return value;
        }
        let name = self.next_temp();
        self.push_stmt(Stmt::Let {
            name: name.clone(),
            mutable: false,
            ty: cir_ty.map(|ty| self.parent.rust_type(ty)),
            init: Some(value),
        });
        self.immutable_temps.insert(name.clone());
        Expr::Var(name.into())
    }

    fn operand_expr(&self, operand: &str) -> Expr {
        if let Some(val) = self.values.get(operand) {
            return val.to_expr(&self.parent.strings);
        }
        if let Some(slot) = self.slot_place(operand) {
            return slot;
        }
        Expr::Var(sanitize_ident(operand))
    }

    fn slot_place(&self, operand: &str) -> Option<Expr> {
        self.slot_places.get(operand).cloned().or_else(|| {
            self.slots
                .get(operand)
                .map(|slot| Expr::Var(slot.clone().into()))
        })
    }

    fn slot_receiver(&self, operand: &str) -> Option<Expr> {
        let place = self.slot_place(operand)?;
        if !self.aligned_slots.contains(operand) {
            return Some(place);
        }
        match place {
            Expr::Unary {
                op: UnaryOp::Deref,
                expr,
            } => Some(*expr),
            _ => Some(place),
        }
    }

    fn element_place_expr(&self, element: &ElementPtr) -> Expr {
        let index = Expr::Cast {
            expr: Box::new(element.index.clone()),
            ty: Type::Prim(Prim::Usize),
        };
        if element.unbounded {
            return Expr::Unary {
                op: UnaryOp::Deref,
                expr: Box::new(Expr::MethodCall {
                    recv: Box::new(Expr::ArrayPtr {
                        array: Box::new(element.base.clone()),
                        mutable: true,
                    }),
                    method: "add".into(),
                    args: vec![index],
                }),
            };
        }
        Expr::Index {
            base: Box::new(element.base.clone()),
            index: Box::new(index),
        }
    }

    fn pointer_operand_expr(&self, operand: &str) -> Expr {
        if self.member_ptrs.contains_key(operand) || self.element_ptrs.contains_key(operand) {
            return self.store_address_expr(operand);
        }
        if self.global_name(operand).is_some() {
            return self.store_address_expr(operand);
        }
        if let Some(value) = self.values.get(operand) {
            return value.to_expr(&self.parent.strings);
        }
        if let Some(slot) = self.slot_place(operand) {
            return if self
                .slot_types
                .get(operand)
                .is_some_and(|ty| matches!(ty, Type::Array { .. }))
            {
                Expr::MethodCall {
                    recv: Box::new(self.slot_receiver(operand).unwrap_or(slot)),
                    method: "as_mut_ptr".into(),
                    args: vec![],
                }
            } else {
                Expr::AddrOf {
                    mutable: true,
                    expr: Box::new(slot),
                }
            };
        }
        Expr::Var(sanitize_ident(operand))
    }

    fn function_pointer_operand_expr(&self, operand: &str) -> Expr {
        if self.function_pointer_null_values.contains(operand) {
            return Expr::Value(RustValue::None);
        }
        match self.values.get(operand) {
            Some(Val::Global(name)) if !self.parent.strings.contains_key(name) => Expr::Call {
                binding: crate::function_identity::CallBinding::Generated,
                func: Box::new(Expr::Var("Some".into())),
                args: vec![Expr::Var(sanitize_ident(name))],
            },
            Some(Val::Expr(expr)) if self.is_function_pointer_none_expr(expr) => {
                Expr::Value(RustValue::None)
            }
            Some(value) => value.to_expr(&self.parent.strings),
            None => self.operand_expr(operand),
        }
    }

    fn function_pointer_byte_operand_expr(&self, operand: &str) -> Expr {
        Expr::Cast {
            expr: Box::new(Expr::MethodCall {
                recv: Box::new(self.function_pointer_operand_expr(operand)),
                method: "unwrap".into(),
                args: Vec::new(),
            }),
            ty: Type::Ptr {
                mutable: false,
                inner: Box::new(Type::Prim(Prim::U8)),
            },
        }
    }

    fn is_function_pointer_null_operand(&self, operand: &str) -> bool {
        if self.function_pointer_null_values.contains(operand) {
            return true;
        }
        matches!(
            self.values.get(operand),
            Some(Val::Expr(expr)) if self.is_function_pointer_none_expr(expr)
        )
    }

    fn is_function_pointer_none_expr(&self, expr: &Expr) -> bool {
        matches!(
            expr,
            Expr::Value(RustValue::None) | Expr::Value(RustValue::NullPtr)
        )
    }

    fn call_arg_expr(&self, operand: &str, ty: &str) -> Expr {
        if is_cir_function_pointer_type(ty) {
            self.function_pointer_operand_expr(operand)
        } else if ty.starts_with("!cir.ptr<") {
            self.pointer_operand_expr(operand)
        } else {
            self.operand_expr(operand)
        }
    }

    fn next_temp(&mut self) -> String {
        let name = format!("_v{}", self.temp_counter);
        self.temp_counter += 1;
        name
    }

    fn emit_todo(&mut self, note: &str) {
        self.push_stmt(Stmt::Expr(Expr::Todo(note.to_string())));
    }

    fn push_stmt(&mut self, stmt: Stmt) {
        self.body.push(IndentStmt {
            depth: self.indent,
            stmt,
        });
    }

    fn unsafe_expr(value: Expr) -> Expr {
        Expr::Unsafe(Box::new(crate::rust_ast::Block {
            stmts: Vec::new(),
            tail: Some(Box::new(value)),
        }))
    }

    fn unsafe_deref_expr(value: Expr) -> Expr {
        match value {
            Expr::Unsafe(block) if block.stmts.is_empty() && block.tail.is_some() => {
                Self::unsafe_expr(Expr::Unary {
                    op: UnaryOp::Deref,
                    expr: block.tail.expect("checked above"),
                })
            }
            value => Self::unsafe_expr(Expr::Unary {
                op: UnaryOp::Deref,
                expr: Box::new(value),
            }),
        }
    }

    fn without_empty_unsafe(value: Expr) -> Expr {
        match value {
            Expr::Unsafe(block) if block.stmts.is_empty() && block.tail.is_some() => {
                Self::without_empty_unsafe(*block.tail.expect("checked above"))
            }
            Expr::Cast { expr, ty } => Expr::Cast {
                expr: Box::new(Self::without_empty_unsafe(*expr)),
                ty,
            },
            value => value,
        }
    }

    fn and_expr(lhs: Expr, rhs: Expr) -> Expr {
        Expr::Binary {
            op: BinOp::And,
            lhs: Box::new(lhs),
            rhs: Box::new(rhs),
        }
    }

    fn or_exprs(mut exprs: Vec<Expr>) -> Expr {
        let first = exprs.remove(0);
        exprs.into_iter().fold(first, |lhs, rhs| Expr::Binary {
            op: BinOp::Or,
            lhs: Box::new(lhs),
            rhs: Box::new(rhs),
        })
    }

    fn unsafe_stmt(stmt: Stmt) -> Stmt {
        Stmt::Unsafe {
            body: crate::rust_ast::Block {
                stmts: vec![IndentStmt { depth: 0, stmt }],
                tail: None,
            },
        }
    }

    fn assign_stmt(target: Expr, value: Expr) -> Stmt {
        Stmt::Assign { target, value }
    }

    fn push_assign(&mut self, target: Expr, value: Expr) {
        self.push_stmt(Self::assign_stmt(target, value));
    }

    fn push_unsafe_assign(&mut self, target: Expr, value: Expr) {
        self.push_stmt(Self::unsafe_stmt(Self::assign_stmt(target, value)));
    }

    fn pointee_type(&self, ty: &str) -> Option<Type> {
        let ret = op_type_return(ty)?;
        ret.strip_prefix("!cir.ptr<")
            .and_then(|s| s.strip_suffix('>'))
            .map(|ty| self.parent.rust_type(ty))
    }

    fn default_value_expr(&self, ty: &Type) -> Expr {
        self.parent.default_value_expr(ty)
    }
}

/// Constructor/destructor call order for one translation unit.
struct LifecycleHooks {
    /// `__attribute__((constructor))` functions, ascending priority (ties in
    /// declaration order) — matches `.init_array` execution order.
    ctors: Vec<String>,
    /// `__attribute__((destructor))` functions, in the reverse of their own
    /// ascending-priority/declaration-order build list — matches how
    /// `.fini_array` is built like `.init_array` but run back to front.
    dtors: Vec<String>,
}

/// Scans top-level `cir.func` ops for `global_ctor_priority`/`global_dtor_priority`
/// (present whenever the source had `__attribute__((constructor))`/`(destructor)`,
/// defaulting to priority 65535 when none was given) and orders the hooks the way
/// glibc's `.init_array`/`.fini_array` would run them. Hooks this TU can't wire up
/// (no `main` to splice into, or a non-`void(void)` signature) are diagnosed and
/// dropped rather than silently ignored.
fn collect_lifecycle_hooks(
    ops: &[&Op],
    has_main: bool,
    diagnostics: &mut crate::ctx::Diagnostics,
) -> LifecycleHooks {
    let mut ctors: Vec<(i64, String)> = Vec::new();
    let mut dtors: Vec<(i64, String)> = Vec::new();
    for op in ops {
        if op.kind() != CirOpKind::Func || region_ops(op).is_empty() {
            continue;
        }
        let is_ctor = op.attrs.contains_key("global_ctor_priority");
        let is_dtor = op.attrs.contains_key("global_dtor_priority");
        if !is_ctor && !is_dtor {
            continue;
        }
        let Some(name) = attr_str(op, "sym_name") else {
            continue;
        };
        if function_type_has_params(attr_str(op, "function_type").unwrap_or("")) {
            diagnostics.warn(
                format!(
                    "lower: __attribute__((constructor/destructor)) on `{name}` with a non-void(void) signature is not supported; hook dropped"
                ));
            continue;
        }
        if !has_main {
            diagnostics.warn(
                format!(
                    "lower: __attribute__((constructor/destructor)) on `{name}` needs a `main` in this translation unit to splice into; hook dropped"
                ));
            continue;
        }
        if is_ctor {
            ctors.push((
                attr_int(op, "global_ctor_priority").unwrap_or(65535),
                name.to_string(),
            ));
        }
        if is_dtor {
            dtors.push((
                attr_int(op, "global_dtor_priority").unwrap_or(65535),
                name.to_string(),
            ));
        }
    }
    ctors.sort_by_key(|(prio, _)| *prio);
    dtors.sort_by_key(|(prio, _)| *prio);
    let mut dtor_order: Vec<String> = dtors.into_iter().map(|(_, name)| name).collect();
    dtor_order.reverse();
    LifecycleHooks {
        ctors: ctors.into_iter().map(|(_, name)| name).collect(),
        dtors: dtor_order,
    }
}

/// A no-arg call to a locally defined function, wrapped in `unsafe {}` when the
/// callee requires it (mirrors the wrapping `lower_call` applies at call sites).
fn hook_call_stmt(name: &str, unsafe_functions: &BTreeSet<String>) -> Stmt {
    let call = Expr::Call {
        binding: crate::function_identity::CallBinding::Generated,
        func: Box::new(Expr::Var(name.to_string().into())),
        args: Vec::new(),
    };
    let call = if unsafe_functions.contains(name) {
        Expr::Unsafe(Box::new(crate::rust_ast::Block {
            stmts: Vec::new(),
            tail: Some(Box::new(call)),
        }))
    } else {
        call
    };
    Stmt::Expr(call)
}

fn region_ops(op: &Op) -> Vec<&Op> {
    op.regions
        .iter()
        .flat_map(|region| region.blocks.iter())
        .flat_map(|block| block.ops.iter())
        .collect()
}

fn collect_used_symbols(ops: &[&Op]) -> BTreeMap<String, Vec<UsedKind>> {
    let mut flags = BTreeMap::<String, (bool, bool)>::new();
    for op in ops {
        if op.kind() != CirOpKind::Global {
            continue;
        }
        let Some(kind) = (match attr_str(op, "sym_name") {
            Some("llvm.compiler.used") => Some(UsedKind::Compiler),
            Some("llvm.used") => Some(UsedKind::Linker),
            _ => None,
        }) else {
            continue;
        };
        let Some(init) = attr_str(op, "initial_value") else {
            continue;
        };
        for symbol in parse_cir_global_view_array(init) {
            let name = sanitize_ident(symbol).into_string();
            let entry = flags.entry(name).or_default();
            match kind {
                UsedKind::Compiler => entry.0 = true,
                UsedKind::Linker => entry.1 = true,
                UsedKind::Plain => {}
            }
        }
    }

    flags
        .into_iter()
        .map(|(name, (compiler, linker))| {
            let mut kinds = Vec::new();
            if compiler {
                kinds.push(UsedKind::Plain);
            }
            if linker {
                kinds.push(UsedKind::Linker);
            }
            (name, kinds)
        })
        .collect()
}

fn collect_region_ops_recursive<'a>(op: &'a Op, out: &mut Vec<&'a Op>) {
    for child in region_ops(op) {
        out.push(child);
        collect_region_ops_recursive(child, out);
    }
}

fn c_abi_function_targets(op: &Op) -> BTreeSet<String> {
    let mut ops = Vec::new();
    collect_region_ops_recursive(op, &mut ops);
    ops.into_iter()
        .filter(|op| op.kind() == CirOpKind::GetGlobal)
        .filter(|op| op_result_type(op).is_some_and(is_cir_function_pointer_type))
        .filter_map(|op| attr_symbol_ref(op, "name").map(str::to_string))
        .collect()
}

fn attr_str<'a>(op: &'a Op, key: &str) -> Option<&'a str> {
    op.attrs.get(key).and_then(Attr::as_str)
}

fn collect_assembly_strings<'a>(op: &'a Op, out: &mut Vec<&'a str>) {
    if op.kind() == CirOpKind::Asm
        && let Some(template) = attr_str(op, "asm_string")
    {
        out.push(template);
    }
    if let Some(Attr::Array(values)) = op.attrs.get("cir.module_asm") {
        out.extend(values.iter().filter_map(Attr::as_str));
    }
    for region in &op.regions {
        for block in &region.blocks {
            for child in &block.ops {
                collect_assembly_strings(child, out);
            }
        }
    }
}

fn assembly_mentions_symbol(assembly: &str, symbol: &str) -> bool {
    assembly.match_indices(symbol).any(|(start, _)| {
        let before = assembly[..start].chars().next_back();
        let after = assembly[start + symbol.len()..].chars().next();
        !before.is_some_and(is_asm_symbol_char) && !after.is_some_and(is_asm_symbol_char)
    })
}

fn is_asm_symbol_char(ch: char) -> bool {
    ch.is_ascii_alphanumeric() || matches!(ch, '_' | '.' | '$')
}

fn lower_module_asm(module_op: &Op, diagnostics: &mut crate::ctx::Diagnostics) -> Vec<Item> {
    let Some(Attr::Array(values)) = module_op.attrs.get("cir.module_asm") else {
        return Vec::new();
    };
    let mut templates = Vec::new();
    for raw in values.iter().filter_map(Attr::as_str) {
        match String::from_utf8(decode_cir_string(raw)) {
            Ok(template) if !template.is_empty() => templates.push(template),
            Ok(_) => {}
            Err(_) => diagnostics.error("lower: file-scope assembly template is not valid UTF-8"),
        }
    }
    if templates.is_empty() {
        return Vec::new();
    }
    let Some(triple) = attr_str(module_op, "cir.triple") else {
        diagnostics.error("lower: file-scope assembly has no CIR target triple");
        return Vec::new();
    };
    let Some(target_arch) = rust_target_arch(triple) else {
        diagnostics.error(format!(
            "lower: unsupported file-scope assembly target `{triple}`"
        ));
        return Vec::new();
    };
    let dialect = matches!(target_arch, "x86" | "x86_64").then_some(AsmDialect::Att);
    templates
        .iter()
        .map(|template| Item::Cfg {
            cfg: Cfg::Opt {
                key: "target_arch".into(),
                value: target_arch.into(),
            },
            item: Box::new(Item::Macro {
                name: "core::arch::global_asm".into(),
                args: asm_macro_args(template.clone(), dialect),
            }),
        })
        .collect()
}

fn lower_weak_alias_asm(
    module_op: &Op,
    aliases: &BTreeMap<String, String>,
    diagnostics: &mut crate::ctx::Diagnostics,
) -> Vec<Item> {
    if aliases.is_empty() {
        return Vec::new();
    }
    let Some(triple) = attr_str(module_op, "cir.triple") else {
        diagnostics.error("lower: weak aliases require a CIR target triple");
        return Vec::new();
    };
    let Some(target_arch) = rust_target_arch(triple) else {
        diagnostics.error(format!("lower: unsupported weak alias target `{triple}`"));
        return Vec::new();
    };
    let dialect = matches!(target_arch, "x86" | "x86_64").then_some(AsmDialect::Att);
    aliases
        .iter()
        .map(|(name, target)| Item::Cfg {
            cfg: Cfg::Opt {
                key: "target_arch".into(),
                value: target_arch.into(),
            },
            item: Box::new(Item::Macro {
                name: "core::arch::global_asm".into(),
                args: asm_macro_args(format!(".weak {name}\n.set {name}, {target}"), dialect),
            }),
        })
        .collect()
}

fn rust_target_arch(triple: &str) -> Option<&'static str> {
    let arch = triple.split('-').next()?;
    match arch {
        "x86_64" | "x86_64h" => Some("x86_64"),
        "i386" | "i486" | "i586" | "i686" => Some("x86"),
        "aarch64" | "aarch64_be" | "arm64" => Some("aarch64"),
        arch if arch.starts_with("arm") || arch.starts_with("thumb") => Some("arm"),
        "powerpc" => Some("powerpc"),
        "powerpc64" | "powerpc64le" => Some("powerpc64"),
        "riscv32" | "riscv32gc" | "riscv32imac" => Some("riscv32"),
        "riscv64" | "riscv64gc" | "riscv64imac" => Some("riscv64"),
        "s390x" => Some("s390x"),
        "wasm32" => Some("wasm32"),
        "wasm64" => Some("wasm64"),
        "mips" | "mipsel" => Some("mips"),
        "mips64" | "mips64el" => Some("mips64"),
        "sparc" => Some("sparc"),
        "sparc64" => Some("sparc64"),
        "loongarch64" => Some("loongarch64"),
        "hexagon" => Some("hexagon"),
        "bpfeb" | "bpfel" => Some("bpf"),
        "nvptx64" => Some("nvptx64"),
        "xtensa" => Some("xtensa"),
        _ => None,
    }
}

fn cir_asm_dialect(op: &Op) -> Option<AsmDialect> {
    match attr_int(op, "asm_flavor") {
        Some(0) => Some(AsmDialect::Att),
        Some(1) => Some(AsmDialect::Intel),
        _ => None,
    }
}

fn asm_macro_args(template: String, dialect: Option<AsmDialect>) -> Vec<Expr> {
    let mut options = Vec::new();
    if matches!(dialect, Some(AsmDialect::Att)) {
        options.push(Expr::Var("att_syntax".into()));
    }
    options.push(Expr::Var("raw".into()));
    vec![
        Expr::Str(template),
        Expr::Call {
            binding: CallBinding::Generated,
            func: Box::new(Expr::Var("options".into())),
            args: options,
        },
    ]
}

fn asm_macro_expr(template: String, dialect: Option<AsmDialect>) -> Expr {
    Expr::Macro {
        name: "core::arch::asm".into(),
        args: asm_macro_args(template, dialect),
    }
}

fn asm_template_has_placeholders(template: &str) -> bool {
    let mut chars = template.chars().peekable();
    while let Some(ch) = chars.next() {
        if ch != '$' {
            continue;
        }
        if chars.peek() == Some(&'$') {
            chars.next();
        } else {
            return true;
        }
    }
    false
}

fn asm_template_has_labels(template: &str) -> bool {
    asm_template_label_count(template) != 0
}

fn asm_template_label_count(template: &str) -> usize {
    let mut slots = BTreeSet::new();
    let mut chars = template.chars().peekable();
    while let Some(ch) = chars.next() {
        if ch != '$' || chars.peek() != Some(&'{') {
            continue;
        }
        chars.next();
        let body = chars
            .by_ref()
            .take_while(|next| *next != '}')
            .collect::<String>();
        if let Some((slot, "l")) = body.split_once(':')
            && let Ok(slot) = slot.parse::<usize>()
        {
            slots.insert(slot);
        }
    }
    slots.len()
}

fn translate_asm_template(
    template: &str,
    slot_to_rust: &[usize],
    constraints: &[&str],
    types: &[&str],
) -> Option<String> {
    let mut translated = String::new();
    let mut referenced_operands = BTreeSet::new();
    let mut chars = template.char_indices().peekable();
    while let Some((_, ch)) = chars.next() {
        if ch != '$' {
            translated.push(ch);
            continue;
        }
        if chars.peek().is_some_and(|(_, next)| *next == '$') {
            chars.next();
            translated.push('$');
            continue;
        }
        let (slot, suppress_modifier) = if chars.peek().is_some_and(|(_, next)| *next == '{') {
            chars.next();
            let mut body = String::new();
            let mut closed = false;
            for (_, next) in chars.by_ref() {
                if next == '}' {
                    closed = true;
                    break;
                }
                body.push(next);
            }
            if !closed {
                return None;
            }
            let (slot, modifier) = body
                .split_once(':')
                .map(|(slot, modifier)| (slot, Some(modifier)))
                .unwrap_or((body.as_str(), None));
            (
                slot.parse::<usize>().ok()?,
                matches!(modifier, Some("c" | "l")),
            )
        } else {
            let mut digits = String::new();
            while let Some((_, next)) = chars.peek() {
                if !next.is_ascii_digit() {
                    break;
                }
                digits.push(*next);
                chars.next();
            }
            if digits.is_empty() {
                return None;
            }
            (digits.parse::<usize>().ok()?, false)
        };
        let rust_slot = *slot_to_rust.get(slot)?;
        let constraint = *constraints.get(slot)?;
        referenced_operands.insert(rust_slot);
        translated.push('{');
        translated.push_str(&rust_slot.to_string());
        if !suppress_modifier
            && (constraint.contains('r')
                || constraint
                    .parse::<usize>()
                    .ok()
                    .and_then(|output| constraints.get(output))
                    .is_some_and(|output| output.contains('r')))
            && let Some(modifier) = rust_asm_register_modifier(types.get(slot).copied()?)
        {
            translated.push(':');
            translated.push(modifier);
        }
        translated.push('}');
    }
    for rust_slot in 0..slot_to_rust.iter().copied().max()?.saturating_add(1) {
        if referenced_operands.contains(&rust_slot) {
            continue;
        }
        let source_slot = slot_to_rust
            .iter()
            .position(|mapped| *mapped == rust_slot)?;
        translated.push_str("\n/* {");
        translated.push_str(&rust_slot.to_string());
        let constraint = constraints[source_slot];
        if (constraint.contains('r')
            || constraint
                .parse::<usize>()
                .ok()
                .and_then(|output| constraints.get(output))
                .is_some_and(|output| output.contains('r')))
            && let Some(modifier) = rust_asm_register_modifier(types.get(source_slot).copied()?)
        {
            translated.push(':');
            translated.push(modifier);
        }
        translated.push_str("} */");
    }
    Some(translated)
}

fn rust_asm_register_modifier(cir_ty: &str) -> Option<char> {
    let bits = cir_ty
        .trim()
        .strip_prefix("!s")
        .or_else(|| cir_ty.trim().strip_prefix("!u"))?
        .strip_suffix('i')?
        .parse::<u16>()
        .ok()?;
    match bits {
        8 => Some('l'),
        16 => Some('x'),
        32 => Some('e'),
        64 => Some('r'),
        _ => None,
    }
}

fn attr_symbol_ref<'a>(op: &'a Op, key: &str) -> Option<&'a str> {
    attr_str(op, key)
        .and_then(|value| value.trim().strip_prefix('@'))
        .map(|value| value.trim_matches('"'))
}

fn attr_int(op: &Op, key: &str) -> Option<i64> {
    op.attrs.get(key).and_then(Attr::as_int)
}

fn attr_int_array(op: &Op, key: &str) -> Option<Vec<u64>> {
    let Attr::Array(values) = op.attrs.get(key)? else {
        return None;
    };
    values
        .iter()
        .map(|value| {
            value
                .as_int()
                .and_then(|value| u64::try_from(value).ok())
                .or_else(|| {
                    value
                        .as_str()
                        .and_then(parse_cir_int)
                        .and_then(|value| u64::try_from(value).ok())
                })
        })
        .collect()
}

fn aggregate_member_index(op: &Op) -> Option<usize> {
    attr_int(op, "index")
        .or_else(|| attr_int(op, "index_attr"))
        .and_then(|index| usize::try_from(index).ok())
}

fn attr_bool(op: &Op, key: &str) -> bool {
    op.attrs.contains_key(key)
}

/// Alloca results for clang-generated temps (`.atomictmp`, `atomic-temp`,
/// `cmpxchg.bool` — names no C identifier can spell) whose only uses are one
/// plain store followed by one plain load in the same block. Such a slot
/// carries a single SSA value, so the lowerer forwards it instead of declaring
/// one shadowed named local per call site.
fn forwardable_temp_allocas(body: &Region) -> BTreeSet<String> {
    #[derive(Default)]
    struct Uses {
        stores: usize,
        loads: usize,
        store_at: Option<(usize, usize)>,
        load_at: Option<(usize, usize)>,
        escapes: bool,
    }
    fn plain_access(op: &Op) -> bool {
        attr_int(op, "mem_order").is_none() && !attr_bool(op, "is_volatile")
    }
    fn walk(blocks: &[Block], next_block: &mut usize, uses: &mut BTreeMap<String, Uses>) {
        for block in blocks {
            let block_id = *next_block;
            *next_block += 1;
            for (pos, op) in block.ops.iter().enumerate() {
                if op.kind() == CirOpKind::Alloca
                    && let Some(name) = op.attrs.get("name").and_then(Attr::as_str)
                    && name.chars().any(|c| !c.is_ascii_alphanumeric() && c != '_')
                    && let Some(result) = op.results.first()
                {
                    uses.entry(result.clone()).or_default();
                }
                for (i, operand) in op.operands.iter().enumerate() {
                    let Some(u) = uses.get_mut(operand) else {
                        continue;
                    };
                    match op.kind() {
                        CirOpKind::Store if i == 1 && plain_access(op) => {
                            u.stores += 1;
                            u.store_at = Some((block_id, pos));
                        }
                        CirOpKind::Load if i == 0 && plain_access(op) => {
                            u.loads += 1;
                            u.load_at = Some((block_id, pos));
                        }
                        _ => u.escapes = true,
                    }
                }
                for region in &op.regions {
                    walk(&region.blocks, next_block, uses);
                }
            }
        }
    }
    let mut uses = BTreeMap::new();
    walk(&body.blocks, &mut 0, &mut uses);
    uses.into_iter()
        .filter(|(_, u)| {
            !u.escapes
                && u.stores == 1
                && u.loads == 1
                && matches!(
                    (u.store_at, u.load_at),
                    (Some((sb, sp)), Some((lb, lp))) if sb == lb && sp < lp
                )
        })
        .map(|(result, _)| result)
        .collect()
}

fn atomic_type(rust_ty: &Type) -> Option<AtomicType> {
    Some(match rust_ty {
        Type::Prim(Prim::I8) => AtomicType::I8,
        Type::Prim(Prim::U8) => AtomicType::U8,
        Type::Prim(Prim::I16) => AtomicType::I16,
        Type::Prim(Prim::U16) => AtomicType::U16,
        Type::Prim(Prim::I32) => AtomicType::I32,
        Type::Prim(Prim::U32) => AtomicType::U32,
        Type::Prim(Prim::I64) => AtomicType::I64,
        Type::Prim(Prim::U64) => AtomicType::U64,
        Type::Prim(Prim::Isize) => AtomicType::Isize,
        Type::Prim(Prim::Usize) => AtomicType::Usize,
        Type::Prim(Prim::Bool) => AtomicType::Bool,
        _ => return None,
    })
}

fn atomic_rmw_op(binop: i64) -> AtomicRmwOp {
    match binop {
        0 => AtomicRmwOp::Add,
        1 => AtomicRmwOp::Sub,
        2 => AtomicRmwOp::And,
        3 => AtomicRmwOp::Xor,
        4 => AtomicRmwOp::Or,
        5 => AtomicRmwOp::Nand,
        6 => AtomicRmwOp::Max,
        _ => AtomicRmwOp::Min,
    }
}

// combine old value and operand to the value an atomic fetch op stores.
fn atomic_combine(binop: i64, old: Expr, val: Expr) -> Expr {
    match binop {
        0 => Expr::Binary {
            op: BinOp::Add,
            lhs: Box::new(old),
            rhs: Box::new(val),
        },
        1 => Expr::Binary {
            op: BinOp::Sub,
            lhs: Box::new(old),
            rhs: Box::new(val),
        },
        2 => Expr::Binary {
            op: BinOp::BitAnd,
            lhs: Box::new(old),
            rhs: Box::new(val),
        },
        3 => Expr::Binary {
            op: BinOp::BitXor,
            lhs: Box::new(old),
            rhs: Box::new(val),
        },
        4 => Expr::Binary {
            op: BinOp::BitOr,
            lhs: Box::new(old),
            rhs: Box::new(val),
        },
        5 => Expr::Unary {
            op: UnaryOp::Not,
            expr: Box::new(Expr::Binary {
                op: BinOp::BitAnd,
                lhs: Box::new(old),
                rhs: Box::new(val),
            }),
        },
        6 => Expr::MethodCall {
            recv: Box::new(old),
            method: "max".into(),
            args: vec![val],
        },
        _ => Expr::MethodCall {
            recv: Box::new(old),
            method: "min".into(),
            args: vec![val],
        },
    }
}

// C `memory_order` (0 relaxed,1 consume,2 acquire,3 release,4 acq_rel,5 seq_cst)
// mapped to a fully-qualified Rust `Ordering`. Rust lacks Consume; use Acquire.
fn rust_ordering(mem_order: i64) -> AtomicOrdering {
    match mem_order {
        0 => AtomicOrdering::Relaxed,
        1 | 2 => AtomicOrdering::Acquire,
        3 => AtomicOrdering::Release,
        4 => AtomicOrdering::AcqRel,
        _ => AtomicOrdering::SeqCst,
    }
}

// loads reject Release/AcqRel; clamp to a load-valid ordering.
fn load_ordering(mem_order: i64) -> AtomicOrdering {
    match mem_order {
        0 => AtomicOrdering::Relaxed,
        1 | 2 => AtomicOrdering::Acquire,
        _ => AtomicOrdering::SeqCst,
    }
}

// stores reject Acquire/AcqRel; clamp to a store-valid ordering.
fn store_ordering(mem_order: i64) -> AtomicOrdering {
    match mem_order {
        0 => AtomicOrdering::Relaxed,
        3 => AtomicOrdering::Release,
        _ => AtomicOrdering::SeqCst,
    }
}

fn switch_case(op: &Op) -> Option<SwitchCase<'_>> {
    let kind = attr_int(op, "kind");
    let is_default = kind == Some(0);
    let values = match op.attrs.get("value") {
        Some(Attr::Array(values)) => values
            .iter()
            .filter_map(|value| match value {
                Attr::Int(n) => Some(*n),
                Attr::Raw(raw) => parse_cir_int(raw).map(|n| n as i64),
                _ => None,
            })
            .collect(),
        _ => Vec::new(),
    };
    let patterns = if kind == Some(3) {
        let [start, end] = values.as_slice() else {
            return None;
        };
        vec![Pattern::InclusiveRange {
            start: i128::from(*start),
            end: i128::from(*end),
        }]
    } else {
        values
            .into_iter()
            .map(|value| int_pattern(i128::from(value)))
            .collect()
    };
    let region = op.regions.first()?;
    Some(SwitchCase {
        patterns,
        is_default,
        region,
    })
}

/// Whether a dispatch block ends in its own control transfer (so the dispatch
/// loop must not append a fall-through to the next state).
fn block_diverges(block: &Block) -> bool {
    block.ops.last().is_some_and(|op| {
        matches!(
            op.kind(),
            CirOpKind::Return
                | CirOpKind::Br
                | CirOpKind::IndirectBr
                | CirOpKind::Goto
                | CirOpKind::Trap
                | CirOpKind::Unreachable
        )
    })
}

fn region_ends_control_flow(region: &Region) -> bool {
    region
        .blocks
        .iter()
        .rev()
        .flat_map(|block| block.ops.iter().rev())
        .find(|op| op.kind() != CirOpKind::Yield)
        .is_some_and(|op| {
            matches!(
                op.kind(),
                CirOpKind::Break
                    | CirOpKind::Continue
                    | CirOpKind::Return
                    | CirOpKind::Trap
                    | CirOpKind::Unreachable
            )
        })
}

fn op_result_type(op: &Op) -> Option<&str> {
    op.ty
        .as_deref()
        .and_then(split_top_level_arrow)
        .map(|(_, ret)| ret.trim())
}

// `u32` -> 32; None for bool/isize/usize/non-integers (no fixed width to mask to).
fn int_bits(rust_ty: &str) -> Option<u32> {
    rust_ty
        .strip_prefix('i')
        .or_else(|| rust_ty.strip_prefix('u'))?
        .parse()
        .ok()
}

fn op_result_types(op: &Op) -> Vec<&str> {
    let Some(ret) = op_result_type(op) else {
        return Vec::new();
    };
    let ret = ret.trim();
    if ret.starts_with('(') && ret.ends_with(')') {
        split_top_level(&ret[1..ret.len() - 1], ',')
            .into_iter()
            .map(str::trim)
            .filter(|ty| !ty.is_empty())
            .collect()
    } else {
        vec![ret]
    }
}

fn asm_output_types<'a>(
    op: &'a Op,
    aliases: &'a BTreeMap<String, String>,
    output_count: usize,
) -> Option<Vec<&'a str>> {
    let direct = op_result_types(op);
    if direct.len() == output_count {
        return Some(direct);
    }
    let aggregate = aliases.get(op_result_type(op)?)?.trim();
    let fields = aggregate
        .strip_prefix("!cir.struct<{")?
        .strip_suffix("}>")?;
    let fields = split_top_level(fields, ',')
        .into_iter()
        .map(str::trim)
        .filter(|field| !field.is_empty())
        .collect::<Vec<_>>();
    (fields.len() == output_count).then_some(fields)
}

fn op_operand_types(ty: &str) -> Vec<&str> {
    let Some((params, _)) = split_top_level_arrow(ty) else {
        return Vec::new();
    };
    let params = params.trim().trim_start_matches('(').trim_end_matches(')');
    split_top_level(params, ',')
        .into_iter()
        .map(str::trim)
        .filter(|ty| !ty.is_empty())
        .collect()
}

fn cir_ptr_inner(ty: &str) -> Option<&str> {
    ty.trim()
        .strip_prefix("!cir.ptr<")
        .and_then(|ty| ty.strip_suffix('>'))
        .map(str::trim)
}

fn parse_function_type(s: &str) -> (Vec<String>, Option<String>) {
    let Some(inner) = s
        .strip_prefix("!cir.func<")
        .and_then(|s| s.strip_suffix('>'))
    else {
        return (Vec::new(), None);
    };
    let Some((params, ret)) = split_top_level_arrow(inner) else {
        return (Vec::new(), None);
    };
    let params = params.trim().trim_start_matches('(').trim_end_matches(')');
    let params = split_top_level(params, ',')
        .into_iter()
        .map(str::trim)
        .filter(|s| !s.is_empty() && *s != "...")
        .map(str::to_string)
        .collect();
    (params, Some(ret.trim().to_string()))
}

/// Whether a `!cir.func<..>` type declares any parameters. Void-returning
/// functions have no `->` in the printed type, so this can't reuse
/// `parse_function_type` (which treats a missing arrow as "nothing parsed").
fn function_type_has_params(s: &str) -> bool {
    let Some(inner) = s
        .strip_prefix("!cir.func<")
        .and_then(|s| s.strip_suffix('>'))
    else {
        return false;
    };
    let params = split_top_level_arrow(inner).map_or(inner, |(params, _)| params);
    let params = params.trim().trim_start_matches('(').trim_end_matches(')');
    split_top_level(params, ',')
        .into_iter()
        .any(|s| !s.trim().is_empty())
}

/// Whether a `!cir.func<..>` type ends its parameter list with `...`.
fn function_type_is_variadic(s: &str) -> bool {
    let Some(inner) = s
        .strip_prefix("!cir.func<")
        .and_then(|s| s.strip_suffix('>'))
    else {
        return false;
    };
    let params = split_top_level_arrow(inner).map_or(inner, |(params, _)| params);
    let params = params.trim().trim_start_matches('(').trim_end_matches(')');
    split_top_level(params, ',')
        .into_iter()
        .any(|s| s.trim() == "...")
}

const LONG_DOUBLE_TY: &str = "LongDouble";

fn long_double_ty() -> Type {
    Type::LongDouble
}

fn long_double_field(base: &str) -> Expr {
    Expr::Field {
        base: Box::new(Expr::Var(base.into())),
        field: "0".into(),
    }
}

fn long_double_op_impl(trait_: StdTrait, params: Vec<FnParam>, arg: Expr) -> Item {
    let method = Method {
        name: trait_.method().into(),
        self_kind: SelfKind::Value,
        params,
        ret: Some(long_double_ty()),
        body: Expr::Call {
            binding: crate::function_identity::CallBinding::Generated,
            func: Box::new(Expr::Var(LONG_DOUBLE_TY.into())),
            args: vec![arg],
        },
    };
    Item::Impl(ImplBlock {
        generics: vec![],
        trait_: Some(trait_),
        self_ty: long_double_ty(),
        items: vec![
            ImplItem::AssocType {
                name: "Output".into(),
                ty: long_double_ty(),
            },
            ImplItem::Method(method),
        ],
    })
}

fn long_double_binary(trait_: StdTrait, op: BinOp) -> Item {
    let arg = Expr::Binary {
        op,
        lhs: Box::new(long_double_field("self")),
        rhs: Box::new(long_double_field("o")),
    };
    let o = FnParam {
        name: "o".into(),
        mutable: false,
        ty: long_double_ty(),
    };
    long_double_op_impl(trait_, vec![o], arg)
}

// x86-64 SysV wants size 16 / align 16 for long double; align(16) on an f64
// newtype gives that layout while arithmetic stays f64-precision (tier 1).
fn long_double_prelude() -> Vec<Item> {
    let neg_arg = Expr::Unary {
        op: UnaryOp::Neg,
        expr: Box::new(long_double_field("self")),
    };
    vec![
        Item::Struct(StructDef {
            attrs: vec![
                RustAttr::Repr(vec![Repr::C, Repr::Align(16)]),
                RustAttr::Derive(vec![
                    Derive::Clone,
                    Derive::Copy,
                    Derive::PartialEq,
                    Derive::PartialOrd,
                ]),
            ],
            vis: Visibility::Private,
            generics: vec![],
            name: LONG_DOUBLE_TY.into(),
            fields: StructFields::Tuple(vec![Type::Prim(Prim::F64)]),
        }),
        long_double_binary(StdTrait::Add, BinOp::Add),
        long_double_binary(StdTrait::Sub, BinOp::Sub),
        long_double_binary(StdTrait::Mul, BinOp::Mul),
        long_double_binary(StdTrait::Div, BinOp::Div),
        long_double_op_impl(StdTrait::Neg, vec![], neg_arg),
    ]
}

fn is_long_double(ty: &str) -> bool {
    ty.starts_with("!cir.long_double")
}

fn is_format_string_arg(ty: &str) -> bool {
    matches!(ty, "!cir.ptr<!s8i>" | "!cir.ptr<!u8i>")
}

fn long_double_shim_type_tag(ty: &Type) -> String {
    match ty {
        Type::Prim(Prim::I8) => "i8".into(),
        Type::Prim(Prim::U8) => "u8".into(),
        Type::Prim(Prim::I16) => "i16".into(),
        Type::Prim(Prim::U16) => "u16".into(),
        Type::Prim(Prim::I32) => "i32".into(),
        Type::Prim(Prim::U32) => "u32".into(),
        Type::Prim(Prim::I64) => "i64".into(),
        Type::Prim(Prim::U64) => "u64".into(),
        Type::Prim(Prim::I128) => "i128".into(),
        Type::Prim(Prim::U128) => "u128".into(),
        Type::Prim(Prim::Isize) => "isize".into(),
        Type::Prim(Prim::Usize) => "usize".into(),
        Type::Prim(Prim::F32) => "f32".into(),
        Type::Prim(Prim::F64) => "f64".into(),
        Type::Prim(Prim::Bool) => "bool".into(),
        Type::Ptr { inner, .. } => format!("p{}", long_double_shim_type_tag(inner)),
        _ => "x".into(),
    }
}

// clang lowers complex `*`/`/` to the libgcc runtime (__mul?c3/__div?c3), reached
// directly for `/` and via a NaN-recovery branch for `*`. We call the same symbols
// so results are bit-identical; #[repr(C)] {re, im} matches the return ABI.
fn is_complex_runtime_call(name: &str) -> bool {
    matches!(name, "__muldc3" | "__divdc3" | "__mulsc3" | "__divsc3")
}

fn complex_ty(inner: Type) -> Type {
    Type::Complex(Box::new(inner))
}

fn cir_complex_inner(ty: &str) -> Option<&str> {
    ty.strip_prefix("!cir.complex<")?.strip_suffix('>')
}

fn complex_binop_impl(trait_: StdTrait, op: BinOp) -> Item {
    let field = |base: &str, field: &str| Expr::Field {
        base: Box::new(Expr::Var(base.into())),
        field: field.into(),
    };
    let component = |name: &str| {
        (
            name.to_string(),
            Expr::Binary {
                op,
                lhs: Box::new(field("self", name)),
                rhs: Box::new(field("o", name)),
            },
        )
    };
    let method = Method {
        name: trait_.method().into(),
        self_kind: SelfKind::Value,
        params: vec![FnParam {
            name: "o".into(),
            mutable: false,
            ty: complex_ty(Type::TyVar("T".into())),
        }],
        ret: Some(complex_ty(Type::TyVar("T".into()))),
        body: Expr::StructLit {
            name: "Complex".into(),
            fields: vec![component("re"), component("im")],
        },
    };
    Item::Impl(ImplBlock {
        generics: vec![GenericParam {
            name: "T".into(),
            bounds: vec![TraitBound {
                trait_,
                assoc: vec![("Output".into(), Type::TyVar("T".into()))],
            }],
        }],
        trait_: Some(trait_),
        self_ty: complex_ty(Type::TyVar("T".into())),
        items: vec![
            ImplItem::AssocType {
                name: "Output".into(),
                ty: complex_ty(Type::TyVar("T".into())),
            },
            ImplItem::Method(method),
        ],
    })
}

fn complex_runtime_decl(name: &str, prim: Prim) -> ExternDecl {
    let param = |n: &str| FnParam {
        name: n.into(),
        mutable: false,
        ty: Type::Prim(prim),
    };
    ExternDecl::Fn(ExternFnDecl {
        identity: crate::function_identity::FunctionIdentity::Unknown,
        name: name.into(),
        params: vec![param("a"), param("b"), param("c"), param("d")],
        variadic: false,
        ret: Some(complex_ty(Type::Prim(prim))),
    })
}

// C `_Complex` has no native Rust type; a #[repr(C)] pair matches its two-scalar
// layout, and the extern runtime routines back `*`/`/`.
fn complex_prelude() -> Vec<Item> {
    vec![
        Item::Struct(StructDef {
            attrs: vec![
                RustAttr::Repr(vec![Repr::C]),
                RustAttr::Derive(vec![Derive::Clone, Derive::Copy, Derive::PartialEq]),
            ],
            vis: Visibility::Private,
            generics: vec![GenericParam {
                name: "T".into(),
                bounds: vec![],
            }],
            name: "Complex".into(),
            fields: StructFields::Named(vec![
                ("re".into(), Type::TyVar("T".into())),
                ("im".into(), Type::TyVar("T".into())),
            ]),
        }),
        complex_binop_impl(StdTrait::Add, BinOp::Add),
        complex_binop_impl(StdTrait::Sub, BinOp::Sub),
        Item::ExternBlock {
            abi: "C".into(),
            decls: vec![
                complex_runtime_decl("__muldc3", Prim::F64),
                complex_runtime_decl("__divdc3", Prim::F64),
                complex_runtime_decl("__mulsc3", Prim::F32),
                complex_runtime_decl("__divsc3", Prim::F32),
            ],
        },
    ]
}

// C `memchr` has no direct std equivalent; this byte scan matches its
// `(unsigned char)c` comparison and returns a raw pointer to the first hit.
fn memchr_prelude() -> Item {
    let void_ptr = |mutable| Type::Ptr {
        mutable,
        inner: Box::new(Type::CLib(CLibType::Void)),
    };
    let u8_const_ptr = Type::Ptr {
        mutable: false,
        inner: Box::new(Type::Prim(Prim::U8)),
    };
    let var = |name: &str| Expr::Var(name.into());
    let byte_at = || Expr::MethodCall {
        recv: Box::new(var("bytes")),
        method: "add".into(),
        args: vec![var("i")],
    };

    let hit = Stmt::If {
        cond: Expr::Binary {
            op: BinOp::Eq,
            lhs: Box::new(FunctionLowerer::unsafe_expr(Expr::Unary {
                op: UnaryOp::Deref,
                expr: Box::new(byte_at()),
            })),
            rhs: Box::new(var("b")),
        },
        then_body: vec![IndentStmt {
            depth: 0,
            stmt: Stmt::Return(Some(Expr::Cast {
                expr: Box::new(FunctionLowerer::unsafe_expr(byte_at())),
                ty: void_ptr(true),
            })),
        }],
        else_body: Vec::new(),
    };
    let step = Stmt::CompoundAssign {
        target: var("i"),
        op: BinOp::Add,
        value: Expr::Value(RustValue::I64(1)),
    };
    let scan = Stmt::While {
        cond: Expr::Binary {
            op: BinOp::Lt,
            lhs: Box::new(var("i")),
            rhs: Box::new(var("n")),
        },
        body: crate::rust_ast::Block {
            stmts: vec![
                IndentStmt {
                    depth: 0,
                    stmt: hit,
                },
                IndentStmt {
                    depth: 0,
                    stmt: step,
                },
            ],
            tail: None,
        },
    };

    let body = vec![
        IndentStmt {
            depth: 1,
            stmt: Stmt::Let {
                name: "b".into(),
                mutable: false,
                ty: Some(Type::Prim(Prim::U8)),
                init: Some(Expr::Cast {
                    expr: Box::new(var("c")),
                    ty: Type::Prim(Prim::U8),
                }),
            },
        },
        IndentStmt {
            depth: 1,
            stmt: Stmt::Let {
                name: "bytes".into(),
                mutable: false,
                ty: Some(u8_const_ptr.clone()),
                init: Some(Expr::Cast {
                    expr: Box::new(var("s")),
                    ty: u8_const_ptr,
                }),
            },
        },
        IndentStmt {
            depth: 1,
            stmt: Stmt::Let {
                name: "i".into(),
                mutable: true,
                ty: Some(Type::Prim(Prim::Usize)),
                init: Some(Expr::Value(RustValue::I64(0))),
            },
        },
        IndentStmt {
            depth: 1,
            stmt: scan,
        },
        IndentStmt {
            depth: 1,
            stmt: Stmt::Return(Some(Expr::Value(RustValue::NullPtr))),
        },
    ];

    Item::Fn(FnDef {
        attrs: Vec::new(),
        vis: Visibility::Private,
        unsafe_: false,
        abi: None,
        name: "__slate_memchr".into(),
        params: vec![
            FnParam {
                name: "s".into(),
                mutable: false,
                ty: void_ptr(false),
            },
            FnParam {
                name: "c".into(),
                mutable: false,
                ty: Type::Prim(Prim::I32),
            },
            FnParam {
                name: "n".into(),
                mutable: false,
                ty: Type::Prim(Prim::Usize),
            },
        ],
        ret: Some(void_ptr(true)),
        body,
    })
}

fn rust_type(cir_ty: &str) -> Type {
    rust_type_with_aliases(cir_ty, &BTreeMap::new())
}

fn canonical_c23_libc_symbol(name: &str) -> &str {
    match name {
        "__isoc23_strtol" => "strtol",
        "__isoc23_strtoul" => "strtoul",
        "__isoc23_strtod" => "strtod",
        _ => name,
    }
}

fn c23_redirected_function(name: &str) -> Option<FunctionIdentity> {
    Some(FunctionIdentity::Known(match name {
        "__isoc23_strtol" => crate::function_identity::Known::StrTol,
        "__isoc23_strtoul" => crate::function_identity::Known::StrToul,
        "__isoc23_strtod" => crate::function_identity::Known::StrTod,
        _ => return None,
    }))
}

// True if the region contains a `cir.continue` that targets the enclosing loop,
// i.e. one not swallowed by a nested loop. `cir.if`/`cir.scope`/`cir.switch`
// bodies are transparent, so we recurse through them.
fn region_has_direct_continue(region: &Region) -> bool {
    region
        .blocks
        .iter()
        .any(|block| ops_have_direct_continue(&block.ops))
}

fn ops_have_direct_continue(ops: &[Op]) -> bool {
    ops.iter().any(|op| match op.kind() {
        CirOpKind::Continue => true,
        CirOpKind::For | CirOpKind::While | CirOpKind::Do => false,
        _ => op.regions.iter().any(region_has_direct_continue),
    })
}

fn rust_type_with_aliases(cir_ty: &str, aliases: &BTreeMap<String, String>) -> Type {
    let ty = cir_ty.trim();
    if let Some(expanded) = aliases.get(ty) {
        if (expanded.starts_with("!cir.struct<{") || expanded.starts_with("!cir.union<{"))
            && let Some(name) = ty.strip_prefix("!rec_")
        {
            return Type::Custom(sanitize_ident(name).into_string());
        }
        return rust_type_with_aliases(expanded, aliases);
    }
    if ty == "()" || ty.is_empty() {
        Type::Unit
    } else if ty == "!void" || ty == "!cir.void" {
        Type::CLib(CLibType::Void)
    } else if ty == "!cir.bool" {
        Type::Prim(Prim::Bool)
    } else if ty == "!s32i" || ty == "!cir.int<s, 32>" {
        Type::Prim(Prim::I32)
    } else if ty == "!u32i" || ty == "!cir.int<u, 32>" {
        Type::Prim(Prim::U32)
    } else if ty == "!s16i" || ty == "!cir.int<s, 16>" {
        Type::Prim(Prim::I16)
    } else if ty == "!u16i" || ty == "!cir.int<u, 16>" {
        Type::Prim(Prim::U16)
    } else if ty == "!s8i" || ty == "!cir.int<s, 8>" {
        Type::Prim(Prim::I8)
    } else if ty == "!u8i" || ty == "!cir.int<u, 8>" {
        Type::Prim(Prim::U8)
    } else if ty == "!s64i" || ty == "!cir.int<s, 64>" {
        Type::Prim(Prim::I64)
    } else if ty == "!u64i" || ty == "!cir.int<u, 64>" {
        Type::Prim(Prim::U64)
    } else if ty == "!s128i"
        || ty == "!cir.int<s, 128>"
        || ty == "!s128i_bitint"
        || ty == "!cir.int<s, 128, bitint>"
    {
        Type::Prim(Prim::I128)
    } else if ty == "!u128i"
        || ty == "!cir.int<u, 128>"
        || ty == "!u128i_bitint"
        || ty == "!cir.int<u, 128, bitint>"
    {
        Type::Prim(Prim::U128)
    } else if ty == "!cir.float" {
        Type::Prim(Prim::F32)
    } else if ty == "!cir.double" {
        Type::Prim(Prim::F64)
    } else if ty == "!cir.f128" {
        Type::Prim(Prim::F128)
    } else if is_long_double(ty) {
        Type::LongDouble
    } else if let Some(inner) = ty
        .strip_prefix("!cir.complex<")
        .and_then(|s| s.strip_suffix('>'))
    {
        Type::Complex(Box::new(rust_type_with_aliases(inner, aliases)))
    } else if let Some((inner, len)) = parse_cir_vector_type(ty) {
        Type::Array {
            elem: Box::new(rust_type_with_aliases(&inner, aliases)),
            len,
        }
    } else if let Some(inner) = ty
        .strip_prefix("!cir.ptr<")
        .and_then(|s| s.strip_suffix('>'))
    {
        if let Some(fn_ty) = cir_fn_type_to_type(inner, aliases) {
            fn_ty
        } else {
            Type::Ptr {
                mutable: true,
                inner: Box::new(rust_type_with_aliases(inner, aliases)),
            }
        }
    } else if let Some((inner, len)) = parse_cir_array_type(ty) {
        Type::Array {
            elem: Box::new(rust_type_with_aliases(&inner, aliases)),
            len,
        }
    } else if let Some(name) = cir_record_name(ty) {
        clib_record_type(name)
            .map(Type::CLib)
            .unwrap_or_else(|| Type::Custom(sanitize_ident(name).into_string()))
    } else {
        Type::Prim(Prim::I32)
    }
}

fn cir_fn_type_to_type(ty: &str, aliases: &BTreeMap<String, String>) -> Option<Type> {
    let inner = ty
        .trim()
        .strip_prefix("!cir.func<")
        .and_then(|s| s.strip_suffix('>'))?;
    let (params, ret) = split_top_level_arrow(inner).unwrap_or((inner, "()"));
    let params = params.trim().trim_start_matches('(').trim_end_matches(')');
    let params = split_top_level(params, ',')
        .into_iter()
        .map(str::trim)
        .filter(|s| !s.is_empty() && *s != "...")
        .map(|param| rust_type_with_aliases(param, aliases))
        .collect::<Vec<_>>();
    let ret = rust_type_with_aliases(ret.trim(), aliases);
    Some(Type::FnPtr {
        abi: Abi::C,
        params,
        ret: Box::new(ret),
    })
}

fn type_mentions_long_double(ty: &Type) -> bool {
    match ty {
        Type::LongDouble => true,
        Type::Complex(inner) => type_mentions_long_double(inner),
        Type::Generic { args, .. } => args.iter().any(type_mentions_long_double),
        Type::Ref { inner, .. } => type_mentions_long_double(inner),
        Type::Slice(elem) => type_mentions_long_double(elem),
        Type::Ptr { inner, .. } => type_mentions_long_double(inner),
        Type::Array { elem, .. } => type_mentions_long_double(elem),
        Type::FnPtr { params, ret, .. } => {
            params.iter().any(type_mentions_long_double) || type_mentions_long_double(ret)
        }
        Type::Prim(_)
        | Type::Custom(_)
        | Type::TyVar(_)
        | Type::CLib(_)
        | Type::VaList
        | Type::Str
        | Type::Unit
        | Type::Variadic
        | Type::Never => false,
    }
}

fn type_mentions_complex(ty: &Type) -> bool {
    match ty {
        Type::Complex(_) => true,
        Type::Ref { inner, .. } => type_mentions_complex(inner),
        Type::Slice(elem) => type_mentions_complex(elem),
        Type::Ptr { inner, .. } => type_mentions_complex(inner),
        Type::Array { elem, .. } => type_mentions_complex(elem),
        Type::FnPtr { params, ret, .. } => {
            params.iter().any(type_mentions_complex) || type_mentions_complex(ret)
        }
        Type::Generic { args, .. } => args.iter().any(type_mentions_complex),
        Type::Prim(_)
        | Type::Custom(_)
        | Type::LongDouble
        | Type::TyVar(_)
        | Type::CLib(_)
        | Type::VaList
        | Type::Str
        | Type::Unit
        | Type::Variadic
        | Type::Never => false,
    }
}

fn type_mentions_f128(ty: &Type) -> bool {
    match ty {
        Type::Prim(Prim::F128) => true,
        Type::Complex(inner) | Type::Ref { inner, .. } | Type::Slice(inner) => {
            type_mentions_f128(inner)
        }
        Type::Ptr { inner, .. } | Type::Array { elem: inner, .. } => type_mentions_f128(inner),
        Type::FnPtr { params, ret, .. } => {
            params.iter().any(type_mentions_f128) || type_mentions_f128(ret)
        }
        Type::Generic { args, .. } => args.iter().any(type_mentions_f128),
        _ => false,
    }
}

fn is_cir_function_pointer_type(ty: &str) -> bool {
    ty.trim()
        .strip_prefix("!cir.ptr<")
        .and_then(|s| s.strip_suffix('>'))
        .is_some_and(|inner| inner.trim().starts_with("!cir.func<"))
}

fn parse_cir_array_type(ty: &str) -> Option<(String, u64)> {
    let inner = ty
        .trim()
        .strip_prefix("!cir.array<")
        .and_then(|s| s.strip_suffix('>'))?;
    let (element, len) = inner.rsplit_once(" x ")?;
    Some((element.trim().to_string(), len.trim().parse().ok()?))
}

fn parse_cir_vector_type(ty: &str) -> Option<(String, u64)> {
    let inner = ty
        .trim()
        .strip_prefix("!cir.vector<")
        .and_then(|s| s.strip_suffix('>'))?;
    let (len, element) = inner.split_once(" x ")?;
    Some((element.trim().to_string(), len.trim().parse().ok()?))
}

fn parse_rust_array_type(ty: &str) -> Option<(&str, u64)> {
    let inner = ty
        .trim()
        .strip_prefix('[')
        .and_then(|s| s.strip_suffix(']'))?;
    let (element, len) = inner.rsplit_once(';')?;
    Some((element.trim(), len.trim().parse().ok()?))
}

fn cir_record_name(ty: &str) -> Option<&str> {
    if let Some(name) = ty.strip_prefix("!rec_") {
        return Some(name);
    }
    let rest = ty
        .strip_prefix("!cir.union<\"")
        .or_else(|| ty.strip_prefix("!cir.struct<\""))?;
    rest.split_once('"').map(|(name, _)| name)
}

fn op_type_return(ty: &str) -> Option<&str> {
    split_top_level_arrow(ty).map(|(_, ret)| ret.trim())
}

fn cir_ptr_pointee(ty: &str) -> Option<&str> {
    ty.trim()
        .strip_prefix("!cir.ptr<")
        .and_then(|s| s.strip_suffix('>'))
}

fn anon_alias_key(ty: &str, aliases: &BTreeMap<String, String>) -> Option<String> {
    let ty = ty.trim();
    let expanded = aliases.get(ty)?;
    let name = cir_record_name(expanded).or_else(|| ty.strip_prefix("!rec_"))?;
    (name.starts_with("anon.") || name.starts_with("anon_") || name == "__once_flag")
        .then(|| ty.to_string())
}

fn collect_anon_alias_keys(
    ty: &str,
    aliases: &BTreeMap<String, String>,
    out: &mut BTreeSet<String>,
) {
    collect_anon_alias_keys_inner(ty, aliases, out, &mut BTreeSet::new());
}

fn collect_anon_alias_keys_inner(
    ty: &str,
    aliases: &BTreeMap<String, String>,
    out: &mut BTreeSet<String>,
    seen: &mut BTreeSet<String>,
) {
    let ty = ty.trim();
    if !seen.insert(ty.to_string()) {
        return;
    }
    if let Some(key) = anon_alias_key(ty, aliases) {
        out.insert(key);
    }
    if let Some(inner) = cir_ptr_pointee(ty) {
        collect_anon_alias_keys_inner(inner, aliases, out, seen);
    } else if let Some((elem, _)) = parse_cir_array_type(ty) {
        collect_anon_alias_keys_inner(&elem, aliases, out, seen);
    } else if let Some(expanded) = aliases.get(ty)
        && let (Some(open), Some(close)) = (expanded.find('{'), expanded.rfind('}'))
    {
        for field_ty in split_top_level(&expanded[open + 1..close], ',') {
            collect_anon_alias_keys_inner(field_ty, aliases, out, seen);
        }
    }
}

fn parse_cir_int_type(ty: &str) -> Option<(bool, u32)> {
    let ty = ty.trim();
    if let Some(rest) = ty
        .strip_prefix("!cir.int<")
        .and_then(|s| s.strip_suffix('>'))
    {
        let mut parts = rest.split(',').map(str::trim);
        let signed = match parts.next()? {
            "s" => true,
            "u" => false,
            _ => return None,
        };
        return Some((signed, parts.next()?.parse().ok()?));
    }
    let rest = ty.strip_prefix('!')?;
    let rest = rest.strip_suffix("_bitint").unwrap_or(rest);
    let signed = match rest.as_bytes().first()? {
        b's' => true,
        b'u' => false,
        _ => return None,
    };
    let bits = rest[1..].strip_suffix('i')?.parse().ok()?;
    Some((signed, bits))
}

fn cir_type_to_ctype(ty: &str, aliases: &BTreeMap<String, String>) -> crate::c_ast::CType {
    use crate::c_ast::CType;
    let ty = ty.trim();
    if let Some(inner) = cir_ptr_pointee(ty) {
        return CType::Ptr(Box::new(cir_type_to_ctype(inner, aliases)));
    }
    match ty {
        "!cir.void" | "!void" => return CType::Void,
        "!cir.bool" => return CType::Bool,
        "!cir.float" => return CType::Float { bits: 32 },
        "!cir.double" => return CType::Float { bits: 64 },
        _ => {}
    }
    if let Some((signed, bits)) = parse_cir_int_type(ty) {
        return CType::Int { signed, bits };
    }
    if let Some((elem, len)) = parse_cir_array_type(ty) {
        return CType::Array(Box::new(cir_type_to_ctype(&elem, aliases)), Some(len));
    }
    // resolve records through the alias table so anon fields keep their dotted name.
    if let Some(name) = aliases
        .get(ty)
        .and_then(|expanded| cir_record_name(expanded))
        .or_else(|| cir_record_name(ty))
    {
        return CType::Record(name.to_string());
    }
    CType::Int {
        signed: true,
        bits: 32,
    }
}

fn reconcile_anonymous_member_types(
    module: &Module,
    records: &mut BTreeMap<String, crate::c_ast::Record>,
) {
    for record in records.values_mut() {
        let Some(expanded) = module.aliases.values().find(|expanded| {
            cir_record_name(expanded).is_some_and(|name| {
                sanitize_ident(name).as_str() == sanitize_ident(&record.name).as_str()
            })
        }) else {
            continue;
        };
        let (Some(open), Some(close)) = (expanded.find('{'), expanded.rfind('}')) else {
            continue;
        };
        let field_types = split_top_level(&expanded[open + 1..close], ',');
        if field_types.len() != record.fields.len() {
            continue;
        }
        for (index, field) in record.fields.iter_mut().enumerate() {
            if field.name == format!("__slate_anon_{index}") {
                field.ty = cir_type_to_ctype(field_types[index], &module.aliases);
            }
        }
    }
}

fn collect_anon_record_info(
    ops: &[Op],
    aliases: &BTreeMap<String, String>,
    needed: &mut BTreeSet<String>,
    field_names: &mut BTreeMap<(String, i64), String>,
) {
    for op in ops {
        match op.kind() {
            CirOpKind::Alloca => {
                if let Some(ty) = op
                    .ty
                    .as_deref()
                    .and_then(op_type_return)
                    .and_then(cir_ptr_pointee)
                {
                    collect_anon_alias_keys(ty, aliases, needed);
                }
            }
            CirOpKind::GetMember => {
                if let (Some(key), Some(index), Some(name)) = (
                    op.ty
                        .as_deref()
                        .and_then(split_top_level_arrow)
                        .and_then(|(inputs, _)| inputs.trim().strip_prefix('(')?.strip_suffix(')'))
                        .and_then(|inputs| split_top_level(inputs, ',').first().copied())
                        .and_then(cir_ptr_pointee)
                        .and_then(|pointee| anon_alias_key(pointee, aliases)),
                    op.attrs.get("index_attr").and_then(Attr::as_int),
                    op.attrs.get("name").and_then(Attr::as_str),
                ) {
                    field_names.insert((key, index), name.to_string());
                }
            }
            CirOpKind::Global => {
                if let Some(ty) = attr_str(op, "sym_type") {
                    collect_anon_alias_keys(ty, aliases, needed);
                }
            }
            CirOpKind::CallLlvmIntrinsic => {
                if let Some(ty) = op_result_type(op) {
                    collect_anon_alias_keys(ty, aliases, needed);
                }
            }
            _ => {}
        }
        for region in &op.regions {
            for block in &region.blocks {
                collect_anon_record_info(&block.ops, aliases, needed, field_names);
            }
        }
    }
}

fn collect_anon_bitfield_slots(
    ops: &[Op],
    aliases: &BTreeMap<String, String>,
    member_slots: &mut BTreeMap<String, (String, i64)>,
    bitfield_slots: &mut BTreeSet<(String, i64)>,
) {
    for op in ops {
        if op.kind() == CirOpKind::GetMember
            && let (Some(result), Some(key), Some(index)) = (
                op.results.first(),
                op.ty
                    .as_deref()
                    .and_then(split_top_level_arrow)
                    .and_then(|(inputs, _)| inputs.trim().strip_prefix('(')?.strip_suffix(')'))
                    .and_then(|inputs| split_top_level(inputs, ',').first().copied())
                    .and_then(cir_ptr_pointee)
                    .and_then(|pointee| anon_alias_key(pointee, aliases)),
                op.attrs.get("index_attr").and_then(Attr::as_int),
            )
        {
            member_slots.insert(result.clone(), (key, index));
        }
        if matches!(op.kind(), CirOpKind::GetBitfield | CirOpKind::SetBitfield)
            && let Some(slot) = op
                .operands
                .first()
                .and_then(|operand| member_slots.get(operand))
        {
            bitfield_slots.insert(slot.clone());
        }
        for region in &op.regions {
            for block in &region.blocks {
                collect_anon_bitfield_slots(&block.ops, aliases, member_slots, bitfield_slots);
            }
        }
    }
}

pub fn anon_local_records(module: &Module) -> Vec<crate::c_ast::Record> {
    let mut needed = BTreeSet::new();
    let mut field_names = BTreeMap::new();
    let mut bitfield_slots = BTreeSet::new();
    let mut member_slots = BTreeMap::new();
    collect_anon_bitfield_slots(
        &module.ops,
        &module.aliases,
        &mut member_slots,
        &mut bitfield_slots,
    );
    collect_anon_record_info(&module.ops, &module.aliases, &mut needed, &mut field_names);

    let mut frontier: Vec<String> = needed.iter().cloned().collect();
    while let Some(key) = frontier.pop() {
        let Some(expanded) = module.aliases.get(&key) else {
            continue;
        };
        let (Some(open), Some(close)) = (expanded.find('{'), expanded.rfind('}')) else {
            continue;
        };
        for field_ty in split_top_level(&expanded[open + 1..close], ',') {
            let mut field_keys = BTreeSet::new();
            collect_anon_alias_keys(field_ty.trim(), &module.aliases, &mut field_keys);
            for field_key in field_keys {
                if needed.insert(field_key.clone()) {
                    frontier.push(field_key);
                }
            }
        }
    }

    let mut records = Vec::new();
    for key in &needed {
        let Some(expanded) = module.aliases.get(key) else {
            continue;
        };
        let Some(name) = cir_record_name(expanded).or_else(|| key.strip_prefix("!rec_")) else {
            continue;
        };
        let is_union = expanded.trim_start().starts_with("!cir.union");
        let (Some(open), Some(close)) = (expanded.find('{'), expanded.rfind('}')) else {
            continue;
        };
        let fields = split_top_level(&expanded[open + 1..close], ',')
            .iter()
            .map(|s| s.trim())
            .filter(|s| !s.is_empty())
            .enumerate()
            .map(|(i, field_ty)| crate::c_ast::Decl {
                name: if bitfield_slots.contains(&(key.clone(), i as i64)) {
                    format!("__bitfield_{i}")
                } else {
                    field_names
                        .get(&(key.clone(), i as i64))
                        .cloned()
                        .unwrap_or_else(|| format!("f{i}"))
                },
                comments: Vec::new(),
                ty: cir_type_to_ctype(field_ty, &module.aliases),
                bit_width: bitfield_slots
                    .contains(&(key.clone(), i as i64))
                    .then_some(0),
            })
            .collect();
        records.push(crate::c_ast::Record {
            name: name.to_string(),
            comments: Vec::new(),
            kind: if is_union {
                RecordKind::Union
            } else {
                RecordKind::Struct
            },
            fields,
            packed: None,
            align: None,
        });
    }
    records
}

fn type_array_len(ty: &Type) -> Option<u64> {
    match ty {
        Type::Array { len, .. } => Some(*len),
        _ => None,
    }
}

fn byte_array_elems(bytes: &[u8], ty: &Type) -> Vec<Expr> {
    let signed = matches!(
        ty,
        Type::Array {
            elem,
            ..
        } if matches!(&**elem, Type::Prim(Prim::I8))
    );
    bytes
        .iter()
        .map(|byte| {
            let value = if signed {
                i64::from(i8::from_ne_bytes([*byte]))
            } else {
                i64::from(*byte)
            };
            Expr::Value(RustValue::I64(value))
        })
        .collect()
}

fn standard_record_def(name: &str) -> RecordDef {
    let i8_ty = || Type::Prim(Prim::I8);
    let i32_ty = || Type::Prim(Prim::I32);
    let i64_ty = || Type::Prim(Prim::I64);
    let i8_ptr_ty = || Type::Ptr {
        mutable: true,
        inner: Box::new(Type::Prim(Prim::I8)),
    };
    let fields: Vec<RecordField> = match name {
        "div_t" => vec![("quot".into(), i32_ty()), ("rem".into(), i32_ty())],
        "ldiv_t" | "lldiv_t" | "imaxdiv_t" => {
            vec![("quot".into(), i64_ty()), ("rem".into(), i64_ty())]
        }
        "tm" => vec![
            ("tm_sec".into(), i32_ty()),
            ("tm_min".into(), i32_ty()),
            ("tm_hour".into(), i32_ty()),
            ("tm_mday".into(), i32_ty()),
            ("tm_mon".into(), i32_ty()),
            ("tm_year".into(), i32_ty()),
            ("tm_wday".into(), i32_ty()),
            ("tm_yday".into(), i32_ty()),
            ("tm_isdst".into(), i32_ty()),
            ("tm_gmtoff".into(), i64_ty()),
            ("tm_zone".into(), i8_ptr_ty()),
        ],
        "lconv" => vec![
            ("decimal_point".into(), i8_ptr_ty()),
            ("thousands_sep".into(), i8_ptr_ty()),
            ("grouping".into(), i8_ptr_ty()),
            ("int_curr_symbol".into(), i8_ptr_ty()),
            ("currency_symbol".into(), i8_ptr_ty()),
            ("mon_decimal_point".into(), i8_ptr_ty()),
            ("mon_thousands_sep".into(), i8_ptr_ty()),
            ("mon_grouping".into(), i8_ptr_ty()),
            ("positive_sign".into(), i8_ptr_ty()),
            ("negative_sign".into(), i8_ptr_ty()),
            ("int_frac_digits".into(), i8_ty()),
            ("frac_digits".into(), i8_ty()),
            ("p_cs_precedes".into(), i8_ty()),
            ("p_sep_by_space".into(), i8_ty()),
            ("n_cs_precedes".into(), i8_ty()),
            ("n_sep_by_space".into(), i8_ty()),
            ("p_sign_posn".into(), i8_ty()),
            ("n_sign_posn".into(), i8_ty()),
            ("int_p_cs_precedes".into(), i8_ty()),
            ("int_p_sep_by_space".into(), i8_ty()),
            ("int_n_cs_precedes".into(), i8_ty()),
            ("int_n_sep_by_space".into(), i8_ty()),
            ("int_p_sign_posn".into(), i8_ty()),
            ("int_n_sign_posn".into(), i8_ty()),
        ],
        _ => Vec::new(),
    }
    .into_iter()
    .map(|(name, ty)| RecordField {
        comments: Vec::new(),
        name,
        ty,
    })
    .collect();
    RecordDef {
        comments: Vec::new(),
        vis: Visibility::Private,
        field_vis: Visibility::Private,
        is_union: false,
        allow_non_camel_case: true,
        name: name.to_string(),
        fields,
        packed: None,
        align: None,
    }
}

fn standard_record_default_expr(ty: &str) -> Option<Expr> {
    use RustValue::{I64, NullPtr};
    let fields = match ty {
        "div_t" => vec![("quot", I64(0)), ("rem", I64(0))],
        "ldiv_t" => vec![("quot", I64(0)), ("rem", I64(0))],
        "lldiv_t" => vec![("quot", I64(0)), ("rem", I64(0))],
        "imaxdiv_t" => vec![("quot", I64(0)), ("rem", I64(0))],
        "tm" => vec![
            ("tm_sec", I64(0)),
            ("tm_min", I64(0)),
            ("tm_hour", I64(0)),
            ("tm_mday", I64(0)),
            ("tm_mon", I64(0)),
            ("tm_year", I64(0)),
            ("tm_wday", I64(0)),
            ("tm_yday", I64(0)),
            ("tm_isdst", I64(0)),
            ("tm_gmtoff", I64(0)),
            ("tm_zone", NullPtr),
        ],
        "lconv" => vec![
            ("decimal_point", NullPtr),
            ("thousands_sep", NullPtr),
            ("grouping", NullPtr),
            ("int_curr_symbol", NullPtr),
            ("currency_symbol", NullPtr),
            ("mon_decimal_point", NullPtr),
            ("mon_thousands_sep", NullPtr),
            ("mon_grouping", NullPtr),
            ("positive_sign", NullPtr),
            ("negative_sign", NullPtr),
            ("int_frac_digits", I64(0)),
            ("frac_digits", I64(0)),
            ("p_cs_precedes", I64(0)),
            ("p_sep_by_space", I64(0)),
            ("n_cs_precedes", I64(0)),
            ("n_sep_by_space", I64(0)),
            ("p_sign_posn", I64(0)),
            ("n_sign_posn", I64(0)),
            ("int_p_cs_precedes", I64(0)),
            ("int_p_sep_by_space", I64(0)),
            ("int_n_cs_precedes", I64(0)),
            ("int_n_sep_by_space", I64(0)),
            ("int_p_sign_posn", I64(0)),
            ("int_n_sign_posn", I64(0)),
        ],
        _ => return None,
    };
    Some(Expr::StructLit {
        name: ty.into(),
        fields: fields
            .into_iter()
            .map(|(name, value)| (name.into(), Expr::Value(value)))
            .collect(),
    })
}

fn zero_for_cir_type(ty: &str) -> Expr {
    default_value_for_type(&rust_type(ty))
}

fn default_value_for_type(ty: &Type) -> Expr {
    match ty {
        Type::Prim(Prim::Bool) => Expr::Value(RustValue::Bool(false)),
        Type::Prim(Prim::F32 | Prim::F64) => Expr::Value(RustValue::Float(0.0)),
        Type::Prim(Prim::F128) => Expr::HexFloat("0.0f128".into()),
        Type::Ptr { .. } => Expr::Value(RustValue::NullPtr),
        Type::FnPtr { .. } => Expr::Value(RustValue::None),
        _ => Expr::Value(RustValue::I64(0)),
    }
}

fn expr_int_value(expr: &Expr) -> Option<i128> {
    match expr {
        Expr::Value(RustValue::I64(value)) => Some(i128::from(*value)),
        Expr::Value(RustValue::I128(value)) => Some(*value),
        _ => None,
    }
}

fn bitfield_info_number(info: &str, key: &str) -> Option<u32> {
    let rest = info.split(key).nth(1)?;
    let end = rest
        .find(|c: char| !c.is_ascii_digit())
        .unwrap_or(rest.len());
    rest[..end].parse().ok()
}

fn bitfield_mask(size: u32) -> u128 {
    if size == 0 {
        0
    } else {
        u128::MAX >> (128 - size.min(128))
    }
}

fn render_array_literal_expr(elems: &[Expr], len: usize, default: Expr) -> Expr {
    let mut out: Vec<Expr> = elems.iter().take(len).cloned().collect();
    out.resize(len, default);
    Expr::ArrayLit(out)
}

fn vector_index_expr(base: Expr, index: u64) -> Expr {
    Expr::Index {
        base: Box::new(base),
        index: Box::new(Expr::Value(RustValue::Usize(index as usize))),
    }
}

fn parse_cir_scalar_expr(s: &str) -> Option<Expr> {
    parse_cir_int(s)
        .map(int_value_expr)
        .or_else(|| parse_cir_uint128(s).map(|n| Expr::Value(RustValue::U128(n))))
        .or_else(|| parse_cir_fp_expr(s))
        .or_else(|| parse_cir_bool(s).map(|b| Expr::Value(RustValue::Bool(b))))
        .or_else(|| {
            s.trim_start()
                .starts_with("#cir.ptr<null>")
                .then_some(Expr::Value(RustValue::NullPtr))
        })
}

fn parse_cir_const_vector(s: &str) -> Option<Expr> {
    let s = s.trim_start();
    let start = s.find("#cir.const_vector<[")?;
    let rest = &s[start + "#cir.const_vector<[".len()..];
    let close = rest.find("]>")?;
    Some(Expr::ArrayLit(
        split_top_level(&rest[..close], ',')
            .into_iter()
            .map(str::trim)
            .filter(|part| !part.is_empty())
            .map(parse_cir_scalar_expr)
            .collect::<Option<Vec<_>>>()?,
    ))
}

fn parse_cir_global_view(s: &str) -> Option<&str> {
    let s = s.trim_start().strip_prefix("#cir.global_view<@")?;
    let end = s.find('>')?;
    Some(s[..end].trim_matches('"'))
}

fn parse_cir_global_view_array(s: &str) -> Vec<&str> {
    let s = s.trim_start();
    if !s.starts_with("#cir.const_array<[") {
        return Vec::new();
    }
    let Some(open) = s.find('[') else {
        return Vec::new();
    };
    let Some(close) = s.rfind(']') else {
        return Vec::new();
    };
    split_top_level(&s[open + 1..close], ',')
        .into_iter()
        .filter_map(parse_cir_global_view)
        .collect()
}

fn int_value_expr(n: i128) -> Expr {
    Expr::Value(match i64::try_from(n) {
        Ok(n) => RustValue::I64(n),
        Err(_) => RustValue::I128(n),
    })
}

fn int_pattern(n: i128) -> Pattern {
    match i64::try_from(n) {
        Ok(n) => Pattern::I64(n),
        Err(_) => Pattern::I128(n),
    }
}

fn parse_cir_fp_expr(s: &str) -> Option<Expr> {
    parse_cir_fp(s).map(fp_literal_expr)
}

fn parse_cir_f128_expr(s: &str) -> Option<Expr> {
    let fp = cir_fp_text(s)?;
    if let Some(hex) = fp.strip_prefix("0x").or_else(|| fp.strip_prefix("0X")) {
        let bits = u128::from_str_radix(hex, 16).ok()?;
        Some(Expr::HexFloat(format!("f128::from_bits(0x{bits:032x})")))
    } else {
        Some(Expr::HexFloat(format!("{fp}f128")))
    }
}

fn fp_literal_expr(fp: String) -> Expr {
    fp.parse::<f64>()
        .map(|n| Expr::Value(RustValue::Float(n)))
        .unwrap_or_else(|_| Expr::HexFloat(fp))
}

fn typed_fp_literal_expr(ty: Option<&Type>, fp: String) -> Expr {
    let value = fp_literal_expr(fp);
    if matches!(ty, Some(Type::LongDouble)) {
        Expr::Call {
            binding: crate::function_identity::CallBinding::Generated,
            func: Box::new(Expr::Var(LONG_DOUBLE_TY.into())),
            args: vec![value],
        }
    } else {
        value
    }
}

enum CirComplexComponent {
    Int(i128),
    Uint(u128),
    Float(String),
}

fn complex_const_expr(ty: Option<&Type>, re: CirComplexComponent, im: CirComplexComponent) -> Expr {
    let inner = match ty {
        Some(Type::Complex(inner)) => Some(inner.as_ref()),
        _ => None,
    };
    Expr::StructLit {
        name: "Complex".into(),
        fields: vec![
            ("re".into(), complex_component_expr(inner, re)),
            ("im".into(), complex_component_expr(inner, im)),
        ],
    }
}

fn complex_component_expr(ty: Option<&Type>, component: CirComplexComponent) -> Expr {
    match component {
        CirComplexComponent::Int(value) => int_value_expr(value),
        CirComplexComponent::Uint(value) => Expr::Value(RustValue::U128(value)),
        CirComplexComponent::Float(value) => typed_fp_literal_expr(ty, value),
    }
}

fn cir_int_digits(s: &str) -> Option<&str> {
    let start = s.find("#cir.int<")? + "#cir.int<".len();
    let rest = &s[start..];
    let end = rest.find('>')?;
    Some(&rest[..end])
}

// i128 so a full-range `!u64i` value (e.g. SIG_ERR = (void(*)(int))-1, which CIR
// prints as the unsigned bit pattern 18446744073709551615) survives as a valid
// unsigned literal rather than overflowing i64 and collapsing to 0.
fn parse_cir_int(s: &str) -> Option<i128> {
    cir_int_digits(s)?.parse().ok()
}

// separate from parse_cir_int: a u128 constant above i128::MAX (e.g. near
// UINT128_MAX) doesn't fit in i128, so this is the fallback for scalar-expr
// construction, not for callers that need a plain i128 (offsets, lengths, ...).
fn parse_cir_uint128(s: &str) -> Option<u128> {
    cir_int_digits(s)?.parse().ok()
}

fn parse_cir_bool(s: &str) -> Option<bool> {
    let start = s.find("#cir.bool<")? + "#cir.bool<".len();
    let rest = &s[start..];
    let end = rest.find('>')?;
    match rest[..end].trim() {
        "true" => Some(true),
        "false" => Some(false),
        _ => None,
    }
}

fn parse_cir_fp(s: &str) -> Option<String> {
    let text = cir_fp_text(s)?;
    if text.starts_with("0x") || text.starts_with("0X") {
        let bits = u64::from_str_radix(&text[2..], 16).ok()?;
        return match text.len() - 2 {
            8 => Some(format!("f32::from_bits(0x{bits:08x})")),
            16 => Some(format!("f64::from_bits(0x{bits:016x})")),
            _ => None,
        };
    }
    Some(text.to_string())
}

fn cir_fp_text(s: &str) -> Option<&str> {
    let start = s.find("#cir.fp<")? + "#cir.fp<".len();
    let rest = &s[start..];
    let end = rest.find('>')?;
    Some(rest[..end].trim())
}

fn parse_cir_const_complex(s: &str) -> Option<(CirComplexComponent, CirComplexComponent)> {
    let start = s.find("#cir.const_complex<")? + "#cir.const_complex<".len();
    let inner = &s[start..];
    let parts = split_top_level(inner, ',');
    let re = parse_cir_complex_component(parts.first()?.trim())?;
    let im = parse_cir_complex_component(parts.get(1)?.trim())?;
    Some((re, im))
}

fn parse_cir_complex_component(s: &str) -> Option<CirComplexComponent> {
    parse_cir_int(s)
        .map(CirComplexComponent::Int)
        .or_else(|| parse_cir_uint128(s).map(CirComplexComponent::Uint))
        .or_else(|| parse_cir_fp(s).map(CirComplexComponent::Float))
}

fn parse_cir_const_array(s: &str) -> Option<Vec<u8>> {
    let s = s.trim_start();
    if !s.starts_with("#cir.const_array<\"") {
        return None;
    }
    let start = s.find('"')? + 1;
    let rest = &s[start..];
    let end = rest.find('"')?;
    Some(decode_cir_string(&rest[..end]))
}

/// Parse the numeric form `#cir.const_array<[#cir.int<1> : !s32i, ...]>` into
/// per-element Rust literals. Returns `None` for the string form (handled by
/// [`parse_cir_const_array`]) or any element we cannot render.
fn parse_cir_const_array_elems(s: &str) -> Option<Vec<Expr>> {
    let s = s.trim_start();
    if !s.starts_with("#cir.const_array<[") {
        return None;
    }
    let open = s.find('[')?;
    let close = s.rfind(']')?;
    let inner = &s[open + 1..close];
    split_top_level(inner, ',')
        .into_iter()
        .map(str::trim)
        .filter(|part| !part.is_empty())
        .map(|part| {
            if is_cir_aggregate_init(part) {
                return None; // array of aggregates → render_const_value handles it
            }
            parse_cir_scalar_expr(part)
        })
        .collect()
}

fn parse_cir_block_addr_labels(s: &str) -> Option<Vec<String>> {
    let s = s.trim_start();
    if !s.starts_with("#cir.const_array<[") {
        return None;
    }
    let open = s.find('[')?;
    let close = s.rfind(']')?;
    let mut labels = Vec::new();
    for part in split_top_level(&s[open + 1..close], ',') {
        let part = part.trim();
        if part.is_empty() {
            continue;
        }
        if !part.contains("#cir.block_addr_info<") {
            return None;
        }
        let start = part.find('"')? + 1;
        let rest = &part[start..];
        let end = rest.find('"')?;
        labels.push(rest[..end].to_string());
    }
    (!labels.is_empty()).then_some(labels)
}

/// A `cir.global` initializer that is a struct/union or nested-aggregate array,
/// rendered on demand by [`FunctionLowerer::render_const_value_expr`].
fn is_cir_aggregate_init(raw: &str) -> bool {
    let raw = raw.trim_start();
    raw.starts_with("#cir.const_record<") || raw.starts_with("#cir.const_array<[")
}

fn decode_cir_string(s: &str) -> Vec<u8> {
    let mut bytes = Vec::new();
    let mut chars = s.chars().peekable();
    while let Some(c) = chars.next() {
        if c != '\\' {
            bytes.push(c as u8);
            continue;
        }
        if chars.peek() == Some(&'\\') {
            chars.next();
            bytes.push(b'\\');
            continue;
        }
        let mut hex = String::new();
        while hex.len() < 2 && chars.peek().is_some_and(|c| c.is_ascii_hexdigit()) {
            hex.push(chars.next().unwrap());
        }
        if hex.is_empty() {
            bytes.push(b'\\');
        } else if let Ok(value) = u8::from_str_radix(&hex, 16) {
            bytes.push(value);
        }
    }
    bytes
}

fn sanitize_ident(s: &str) -> Ident {
    let mut out = String::new();
    for (i, c) in s.chars().enumerate() {
        if (i == 0 && (c.is_ascii_alphabetic() || c == '_'))
            || (i > 0 && (c.is_ascii_alphanumeric() || c == '_'))
        {
            out.push(c);
        } else {
            out.push('_');
        }
    }
    if out.is_empty() {
        return Ident::from("_tmp");
    }
    // `crate`/`self`/`Self`/`super` can't be raw identifiers, so mangle them instead.
    if matches!(out.as_str(), "crate" | "self" | "Self" | "super") {
        out.push('_');
    } else if is_rust_keyword(&out) {
        out = format!("r#{out}");
    }
    Ident::from(out)
}

fn is_rust_keyword(s: &str) -> bool {
    matches!(
        s,
        "as" | "break"
            | "const"
            | "continue"
            | "crate"
            | "else"
            | "enum"
            | "extern"
            | "false"
            | "fn"
            | "for"
            | "if"
            | "impl"
            | "in"
            | "let"
            | "loop"
            | "match"
            | "mod"
            | "move"
            | "mut"
            | "pub"
            | "ref"
            | "return"
            | "self"
            | "Self"
            | "static"
            | "struct"
            | "super"
            | "trait"
            | "true"
            | "type"
            | "unsafe"
            | "use"
            | "where"
            | "while"
            | "async"
            | "await"
            | "dyn"
            | "abstract"
            | "become"
            | "box"
            | "do"
            | "final"
            | "macro"
            | "override"
            | "priv"
            | "typeof"
            | "unsized"
            | "virtual"
            | "yield"
            | "try"
    )
}

fn split_top_level_arrow(s: &str) -> Option<(&str, &str)> {
    let mut angle = 0usize;
    let mut paren = 0usize;
    let bytes = s.as_bytes();
    let mut i = 0usize;
    while i + 1 < bytes.len() {
        match bytes[i] as char {
            '<' => angle += 1,
            '>' => angle = angle.saturating_sub(1),
            '(' => paren += 1,
            ')' => paren = paren.saturating_sub(1),
            '-' if bytes[i + 1] == b'>' && angle == 0 && paren == 0 => {
                return Some((&s[..i], &s[i + 2..]));
            }
            _ => {}
        }
        i += 1;
    }
    None
}

fn split_top_level(s: &str, delimiter: char) -> Vec<&str> {
    let mut parts = Vec::new();
    let mut start = 0usize;
    let mut angle = 0usize;
    let mut paren = 0usize;
    for (i, c) in s.char_indices() {
        match c {
            '<' => angle += 1,
            '>' => angle = angle.saturating_sub(1),
            '(' => paren += 1,
            ')' => paren = paren.saturating_sub(1),
            c if c == delimiter && angle == 0 && paren == 0 => {
                parts.push(&s[start..i]);
                start = i + c.len_utf8();
            }
            _ => {}
        }
    }
    parts.push(&s[start..]);
    parts
}
