//! lower: combine the typed CIR tree with the C AST oracle into Rust output.

use crate::backend::rust_ast::{
    Abi, AsmDialect, AsmOperand, AsmReg, AtomicOrdering, AtomicPlace, AtomicRmwOp, AtomicType,
    Attr as RustAttr, AttrArg, BinOp, CLIB_RECORD_TYPES, CLibInitializer, CLibType, Cfg, Comment,
    CrateAttr, Derive, EnumConst, EnumDef, Expr, ExprMatchArm, ExternDecl, ExternFnDecl, Feature,
    FnDef, FnParam, Ident, ImplBlock, ImplItem, IndentStmt, InlineAsm, InlineHint, Item, Label,
    Lint, MatchArm, Method, Path, Pattern, Prim, Program, Raw, RecordDef, RecordField, Repr,
    RustValue, SelfKind, StdTrait, Stmt, StructDef, StructField, StructFields, SupportModule,
    TraitRef, Type, UnaryOp, UsedKind, Visibility,
};
use crate::ctx::Ctx;
use crate::frontend::c_ast::{
    CType, EnumConstRef, FloatingLiteralFact, FloatingLiteralLoc, LayoutQuery, Loc, MacroConst,
    RecordKind, SourcePoint, Unit,
};
use crate::frontend::function_abi::repair_function_signature;
use crate::function_identity::{CallBinding, FunctionIdentity, Known};
use clang_ir::ast::SourceLocation;
use clang_ir::ast::{Attribute as Attr, Type as CirType};
use clang_ir::enums::CmpOpKind;
use clang_ir::model::Module;
use clang_ir::model::{
    Function as CirFunction, Global as CirGlobal, GlobalLinkageKind, MemOrder, Op, VisibilityKind,
    instruction as inst,
};
use std::collections::{BTreeMap, BTreeSet, HashMap, VecDeque};

mod analysis;
mod arithmetic;
mod asm;
mod atomic;
mod bitfields;
mod builtins;
mod calls;
mod cir_ops;
mod constants;
mod control_flow;
mod dispatch;
mod expressions;
mod function_setup;
mod intrinsics;
mod intrinsics_table;
mod memory;
mod module_index;
mod op_utils;
mod record_analysis;
mod runtime_support;
mod storage;
mod types;
mod values;

use analysis::*;
use asm::*;
use atomic::*;
use bitfields::*;
use cir_ops::*;
use constants::*;
use module_index::*;
pub use module_index::{
    address_taken_functions, always_inline_functions, declared_functions, declared_globals,
    defined_functions, defined_globals, required_features, shim_records_for_module,
    target_feature_functions, unsafe_defined_functions,
};
use op_utils::*;
pub use record_analysis::anon_local_records;
use record_analysis::*;
use runtime_support::*;
use types::*;

#[derive(Default, Clone)]
pub struct ProjectInfo {
    pub cross_module: BTreeMap<String, String>,
    pub cross_module_globals: BTreeMap<String, String>,
    pub child_modules: Vec<String>,
    pub shared_records: BTreeSet<String>,
    pub shared_enums: BTreeSet<String>,
    pub shared_type_module: Option<String>,
    pub shared_type_crate: Option<String>,
    pub shared_long_double: bool,
    pub cross_module_crate: Option<String>,
    pub unsafe_functions: BTreeSet<String>,
    pub crate_features: BTreeSet<Feature>,
    pub emit_pub: bool,
    pub cross_referenced_functions: BTreeSet<String>,
    pub cross_referenced_globals: BTreeSet<String>,
    pub address_taken_functions: BTreeSet<String>,
}

pub fn lower(cir: &Module, c: &Unit, ctx: &mut Ctx) -> Program {
    lower_with_project(cir, c, ctx, &ProjectInfo::default())
}

pub fn lower_with_project(cir: &Module, c: &Unit, ctx: &mut Ctx, project: &ProjectInfo) -> Program {
    let mut anon_records = anon_local_records(cir);
    let shim_records = shim_records_for_module(cir, c);
    let cir_record_names: BTreeSet<String> = cir
        .type_aliases
        .values()
        .filter_map(slate_record_name)
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
    for record in &shim_records {
        let name = sanitize_ident(&record.name).into_string();
        if !project.shared_records.contains(&name) && anon_record_names.insert(name) {
            anon_records.push(record.clone());
        }
    }
    let mut records: BTreeMap<String, crate::frontend::c_ast::Record> = c
        .records
        .iter()
        .map(|record| (sanitize_ident(&record.name).into_string(), record.clone()))
        .collect();
    let local_record_candidates: Vec<crate::frontend::c_ast::Record> = c
        .records
        .iter()
        .chain(&c.anonymous_header_records)
        .chain(&c.named_header_records)
        .cloned()
        .collect();
    let local_collisions = resolve_local_record_collisions(cir, &local_record_candidates);
    records.extend(local_collisions);
    let enums: BTreeMap<String, crate::frontend::c_ast::Enum> = c
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
            || (records
                .get(&name)
                .is_some_and(|existing| existing.fields.is_empty())
                && !record.fields.is_empty())
        {
            records.insert(name, record.clone());
        } else {
            records.entry(name).or_insert_with(|| record.clone());
        }
    }
    for record in shim_records {
        records
            .entry(sanitize_ident(&record.name).into_string())
            .or_insert(record);
    }
    for record in reconcile_anonymous_member_types(cir, &mut records, &c.anonymous_header_records) {
        let name = sanitize_ident(&record.name).into_string();
        if anon_record_names.insert(name) {
            anon_records.push(record);
        }
    }
    widen_flexible_array_members(cir, &mut records);
    let record_defs = required_record_defs(cir, c, &mut records, &project.shared_records);
    let global_rust_names = allocate_global_rust_names(cir, &records, &enums);
    let mut declaration_comments: BTreeMap<(String, String), Vec<Comment>> = BTreeMap::new();
    for declaration in &c.declaration_comments {
        let Some(name) = &declaration.name else {
            continue;
        };
        let name = sanitize_ident(name).into_string();
        let comment = Comment {
            lines: declaration.lines.clone(),
        };
        if declaration.kind != "FunctionDecl"
            && let Some(function) = &declaration.function
        {
            declaration_comments
                .entry((
                    "FunctionDecl".into(),
                    sanitize_ident(function).into_string(),
                ))
                .or_default()
                .push(comment);
        } else {
            declaration_comments
                .entry((declaration.kind.clone(), name))
                .or_default()
                .push(comment);
        }
    }
    let mut lowerer = Lowerer {
        ctx,
        aliases: cir.type_aliases.clone(),
        attr_aliases: cir.attr_aliases.clone(),
        call_bindings: c.call_bindings(),
        known_functions: c
            .call_bindings()
            .values()
            .filter_map(|binding| binding.known())
            .map(|known| (known.symbol().to_string(), FunctionIdentity::Known(known)))
            .collect(),
        function_types: c
            .function_types
            .iter()
            .map(|(name, ty)| (name.clone(), ty.clone()))
            .collect(),
        weak_refs: c
            .weak_refs
            .iter()
            .map(|attribute| (attribute.name.clone(), attribute.target.clone()))
            .collect(),
        naked_functions: c.naked_functions.clone(),
        external_weak_targets: BTreeSet::new(),
        weak_aliases: BTreeMap::new(),
        records,
        record_defs,
        enums,
        declaration_comments,
        global_rust_names,
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
        llvm_intrinsic_shims: BTreeMap::new(),
        uses_long_double: std::cell::Cell::new(false),
        uses_fenv_shims: std::cell::Cell::new(false),
        uses_complex: std::cell::Cell::new(false),
        uses_f128: std::cell::Cell::new(false),
        uses_f16: std::cell::Cell::new(false),
        uses_c_variadic: std::cell::Cell::new(false),
        uses_linkage: std::cell::Cell::new(false),
        uses_thread_local: std::cell::Cell::new(false),
        uses_used_with_arg: std::cell::Cell::new(false),
        uses_asm_goto_outputs: std::cell::Cell::new(false),
        uses_llvm_intrinsics: std::cell::Cell::new(false),
        uses_portable_simd: std::cell::Cell::new(false),
        uses_memchr: std::cell::Cell::new(false),
        synthetic_externs: BTreeMap::new(),
        variadic_defs: BTreeSet::new(),
        boxed_variadic_defs: BTreeSet::new(),
        va_list_boxed: false,
        c_abi_functions: BTreeSet::new(),
        project: project.clone(),
        unsafe_functions: project.unsafe_functions.clone(),
        target_feature_functions: target_feature_functions(cir),
        always_inline_functions: always_inline_functions(cir),
        noinline_functions: c.noinline_functions.clone(),
        must_use_functions: c.must_use_functions.clone(),
        deprecated_functions: c.deprecated_functions.clone(),
        unsupported_attribute_functions: c.unsupported_attribute_functions.clone(),
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
        floating_literals: c.floating_literals.clone(),
        global_floating_literals: c.global_floating_literals.clone(),
        long_double_callback_trampolines: BTreeMap::new(),
        enum_consts: c
            .functions
            .iter()
            .map(|function| (function.name.clone(), function.enum_consts.clone()))
            .collect(),
        local_enum_decls: c
            .functions
            .iter()
            .map(|function| (function.name.clone(), function.local_enum_decls.clone()))
            .collect(),
        asm_gotos: c
            .functions
            .iter()
            .map(|function| (function.name.clone(), function.asm_gotos.clone()))
            .collect(),
        function_return_types: BTreeMap::new(),
        function_param_types: BTreeMap::new(),
        enum_wrapper_fns: std::cell::RefCell::new(BTreeMap::new()),
        needed_enum_from_impls: std::cell::RefCell::new(BTreeSet::new()),
        bitfield_storages: collect_bitfield_storages(cir),
        global_sym_types: BTreeMap::new(),
    };
    lowerer.lower_module(cir, c)
}

fn cir_record_field_types_from_aliases(
    record: &crate::frontend::c_ast::Record,
    aliases: &BTreeMap<String, CirType>,
    va_list_boxed: bool,
) -> Option<Vec<Type>> {
    let members = aliases.values().find_map(|ty| match ty {
        CirType::Struct {
            name: Some(name),
            members: Some(members),
            ..
        }
        | CirType::Union {
            name: Some(name),
            members: Some(members),
            ..
        } if sanitize_ident(name).as_str() == sanitize_ident(&record.name).as_str() => {
            Some(members)
        }
        _ => None,
    })?;
    (members.len() == record.fields.len()).then(|| {
        members
            .iter()
            .map(|member| rust_type_with_aliases(member, aliases, va_list_boxed))
            .collect()
    })
}

pub fn lower_shared_types(
    records: &[crate::frontend::c_ast::Record],
    enums: &[crate::frontend::c_ast::Enum],
    aliases: &BTreeMap<String, CirType>,
) -> Program {
    let mut items = vec![Item::CrateAttrs(vec![CrateAttr::Allow(vec![
        Lint::DeadCode,
        Lint::Unused,
        Lint::NonCamelCaseTypes,
        Lint::NonSnakeCase,
        Lint::NonUpperCaseGlobals,
        Lint::SuspiciousRuntimeSymbolDefinitions,
        Lint::UnpredictableFunctionPointerComparisons,
        Lint::UnusedComparisons,
    ])])];
    if shared_types_use_long_double(records) {
        items.extend(long_double_prelude(Visibility::Pub));
        items.push(Item::ExternBlock {
            abi: "C".into(),
            decls: f80_shim_decls().into_iter().map(ExternDecl::Fn).collect(),
        });
    }
    items.extend(
        enums
            .iter()
            .filter_map(|enm| lower_enum_def(enm, Visibility::Pub).map(Item::Enum)),
    );
    items.extend(records.iter().flat_map(|record| {
        lower_record_def(
            record,
            Visibility::Pub,
            Visibility::Pub,
            true,
            false,
            cir_record_field_types_from_aliases(record, aliases, false).as_deref(),
        )
    }));
    Program { items }
}

pub fn long_double_f80_extern_block() -> Item {
    Item::ExternBlock {
        abi: "C".into(),
        decls: f80_shim_decls().into_iter().map(ExternDecl::Fn).collect(),
    }
}

pub fn shared_types_use_long_double(records: &[crate::frontend::c_ast::Record]) -> bool {
    records
        .iter()
        .flat_map(|record| &record.fields)
        .any(|field| ctype_uses_long_double(&field.ty))
}

fn lower_enum_def(enm: &crate::frontend::c_ast::Enum, vis: Visibility) -> Option<EnumDef> {
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
        Type::Prim(Prim::I16 | Prim::U16 | Prim::F16) => 2,
        Type::Prim(Prim::I32 | Prim::U32 | Prim::F32) => 4,
        Type::Prim(Prim::I64 | Prim::U64 | Prim::Isize | Prim::Usize | Prim::F64)
        | Type::Ptr { .. }
        | Type::FnPtr { .. }
        | Type::Ref { .. }
        | Type::VaList => 8,
        Type::CLib(ty) => ty.alignment(),
        Type::Prim(Prim::I128 | Prim::U128 | Prim::F128) | Type::LongDouble => 16,
        Type::Array { elem, .. } => type_alignment(elem),
        Type::Complex(inner) => type_alignment(inner),
        _ => 1,
    }
}

fn effective_type_alignment(
    ty: &Type,
    records: &BTreeMap<String, crate::frontend::c_ast::Record>,
) -> u32 {
    if let Type::Custom(name) = ty
        && let Some(record) = records.get(name)
        && let Some(align) = record_natural_align(record, records)
    {
        return align as u32;
    }
    type_alignment(ty)
}

fn lower_record_def(
    record: &crate::frontend::c_ast::Record,
    vis: Visibility,
    field_vis: Visibility,
    allow_empty: bool,
    va_list_boxed: bool,
    cir_field_types: Option<&[Type]>,
) -> Vec<Item> {
    if record.fields.is_empty() && !allow_empty {
        return Vec::new();
    }
    let is_union = record.kind == RecordKind::Union;
    let fields: Vec<RecordField> = record
        .fields
        .iter()
        .enumerate()
        .map(|(index, field)| {
            let cir_ty = cir_field_types.and_then(|types| types.get(index));
            let trust_cir = matches!(field.ty, CType::FuncPtr { .. })
                || (field.bit_width.is_none()
                    && matches!(cir_ty, Some(Type::Array { .. }))
                    && !matches!(field.ty, CType::Array(..)));
            RecordField {
                comments: comments(&field.comments),
                name: sanitize_ident(&field.name),
                ty: if trust_cir { cir_ty.cloned() } else { None }
                    .unwrap_or_else(|| c_record_field_type(&field.ty, va_list_boxed)),
            }
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
        field_vis,
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
        trait_: Some(TraitRef::Std(StdTrait::Deref)),
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
        trait_: Some(TraitRef::Std(StdTrait::DerefMut)),
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

fn record_lit_name(record: &crate::frontend::c_ast::Record) -> String {
    let name = sanitize_ident(&record.name).into_string();
    if record.packed.is_some() && record.align.is_some() {
        packed_aligned_inner_name(&name)
    } else {
        name
    }
}

fn wrap_record_lit(record: &crate::frontend::c_ast::Record, lit: Expr) -> Expr {
    if record.packed.is_some() && record.align.is_some() {
        Expr::TupleStructLit {
            name: sanitize_ident(&record.name).into_string(),
            fields: vec![lit],
        }
    } else {
        lit
    }
}

fn comments(lines: &[String]) -> Vec<crate::backend::rust_ast::Comment> {
    if lines.is_empty() {
        Vec::new()
    } else {
        vec![crate::backend::rust_ast::Comment {
            lines: lines.to_vec(),
        }]
    }
}

struct Lowerer<'a> {
    ctx: &'a mut Ctx,
    aliases: BTreeMap<String, CirType>,
    attr_aliases: BTreeMap<String, Attr>,
    call_bindings: HashMap<Loc, CallBinding>,
    known_functions: BTreeMap<String, FunctionIdentity>,
    function_types: BTreeMap<String, String>,
    weak_refs: BTreeMap<String, String>,
    naked_functions: BTreeSet<String>,
    external_weak_targets: BTreeSet<String>,
    weak_aliases: BTreeMap<String, String>,
    records: BTreeMap<String, crate::frontend::c_ast::Record>,
    record_defs: BTreeSet<String>,
    enums: BTreeMap<String, crate::frontend::c_ast::Enum>,
    declaration_comments: BTreeMap<(String, String), Vec<Comment>>,
    global_rust_names: BTreeMap<String, String>,
    globals: BTreeMap<String, GlobalVar>,
    extern_globals: BTreeMap<String, ExternGlobal>,
    strings: BTreeMap<String, Vec<u8>>,
    const_arrays: BTreeMap<String, Vec<Expr>>,
    block_addr_globals: BTreeMap<String, Vec<String>>,
    const_aggregates: BTreeMap<String, Attr>,
    const_zero_globals: BTreeSet<String>,
    used_symbols: BTreeMap<String, Vec<UsedKind>>,
    externs: BTreeMap<String, Vec<Type>>,
    extern_returns: BTreeMap<String, Option<String>>,
    long_double_shims: BTreeMap<String, ExternFnDecl>,
    llvm_intrinsic_shims: BTreeMap<String, ExternFnDecl>,
    uses_long_double: std::cell::Cell<bool>,
    uses_fenv_shims: std::cell::Cell<bool>,
    uses_complex: std::cell::Cell<bool>,
    uses_f128: std::cell::Cell<bool>,
    uses_f16: std::cell::Cell<bool>,
    uses_c_variadic: std::cell::Cell<bool>,
    uses_linkage: std::cell::Cell<bool>,
    uses_thread_local: std::cell::Cell<bool>,
    uses_used_with_arg: std::cell::Cell<bool>,
    uses_asm_goto_outputs: std::cell::Cell<bool>,
    uses_llvm_intrinsics: std::cell::Cell<bool>,
    uses_portable_simd: std::cell::Cell<bool>,
    uses_memchr: std::cell::Cell<bool>,
    synthetic_externs: BTreeMap<String, ExternFnDecl>,
    variadic_defs: BTreeSet<String>,
    boxed_variadic_defs: BTreeSet<String>,
    va_list_boxed: bool,
    c_abi_functions: BTreeSet<String>,
    project: ProjectInfo,
    unsafe_functions: BTreeSet<String>,
    target_feature_functions: BTreeMap<String, Vec<String>>,
    always_inline_functions: BTreeSet<String>,
    noinline_functions: BTreeSet<String>,
    must_use_functions: BTreeSet<String>,
    deprecated_functions: BTreeMap<String, Option<String>>,
    unsupported_attribute_functions: BTreeMap<String, Vec<&'static str>>,
    cross_uses: Vec<Item>,
    ctor_calls: Vec<String>,
    dtor_calls: Vec<String>,
    generated_alloca_frames: Vec<StructDef>,
    layout_queries: BTreeMap<String, Vec<LayoutQuery>>,
    macro_consts: BTreeMap<String, Vec<MacroConst>>,
    floating_literals: HashMap<FloatingLiteralLoc, FloatingLiteralFact>,
    global_floating_literals: HashMap<String, Vec<FloatingLiteralFact>>,
    long_double_callback_trampolines: BTreeMap<String, String>,
    enum_consts: BTreeMap<String, Vec<EnumConstRef>>,
    local_enum_decls: BTreeMap<String, Vec<crate::frontend::c_ast::LocalEnumDecl>>,
    asm_gotos: BTreeMap<String, Vec<crate::frontend::c_ast::AsmGoto>>,
    function_return_types: BTreeMap<String, Type>,
    function_param_types: BTreeMap<String, Vec<Type>>,
    enum_wrapper_fns: std::cell::RefCell<BTreeMap<String, FnDef>>,
    needed_enum_from_impls: std::cell::RefCell<BTreeSet<(String, Type)>>,
    bitfield_storages: BitfieldStorages,
    global_sym_types: BTreeMap<String, CirType>,
}

struct FunctionLowerer<'a, 'b> {
    parent: &'a mut Lowerer<'b>,
    values: BTreeMap<String, Val>,
    value_types: BTreeMap<String, CirType>,
    const_int_values: BTreeMap<String, i128>,
    function_pointer_null_values: BTreeSet<String>,
    slots: BTreeMap<String, String>,
    slot_places: BTreeMap<String, Expr>,
    aligned_slots: BTreeSet<String>,
    slot_types: BTreeMap<String, Type>,
    needs_alloca_layout_preservation: bool,
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
    hoisting_allocas: bool,
    hoisted: BTreeSet<String>,
    resolved_bi_allocas: BTreeSet<String>,
    declared_local_names: BTreeSet<String>,
    forward_allocas: BTreeSet<String>,
    forward_values: BTreeMap<String, Expr>,
    immutable_temps: BTreeSet<String>,
    va_allocas: BTreeSet<String>,
    va_places: BTreeMap<String, Expr>,
    va_args_param: Option<String>,
    layout_queries: VecDeque<LayoutQuery>,
    macro_consts: VecDeque<MacroConst>,
    enum_consts: VecDeque<EnumConstRef>,
    macro_arith_values: BTreeMap<String, i128>,
    asm_outputs: BTreeMap<String, Vec<Expr>>,
    asm_gotos: VecDeque<crate::frontend::c_ast::AsmGoto>,
    asm_output_places: BTreeMap<String, Expr>,
    local_enum_types: BTreeMap<String, String>,
    loaded_field_types: BTreeMap<String, Type>,
    load_ptr_operand: BTreeMap<String, String>,
    member_base_operand: BTreeMap<String, String>,
    coerce_alloca_real_type: BTreeMap<String, (String, String)>,
}

struct DispatchCtx {
    loop_label: Label,
    state_var: String,
    label_to_state: BTreeMap<String, usize>,
    block_to_state: BTreeMap<String, usize>,
    cross_block_names: BTreeMap<String, String>,
    block_args: BTreeMap<usize, Vec<String>>,
    pending_hoists: Vec<IndentStmt>,
}

struct LoopFrame {
    break_label: Option<Label>,
    continue_label: Option<Label>,
    is_loop: bool,
}

struct SwitchCase<'a> {
    patterns: Vec<Pattern>,
    is_default: bool,
    region: &'a inst::Region,
}

struct DuffSwitch<'a> {
    cases: Vec<SwitchCase<'a>>,
    prefix: Vec<&'a Op>,
    condition: &'a inst::Region,
}

#[derive(Debug, Clone)]
struct MemberPtr {
    base: Expr,
    field: String,
    field_ty: Option<Type>,
    unsafe_access: bool,
    bitfield_name: Option<String>,
    field_is_trailing: bool,
}

#[derive(Debug, Clone)]
struct ElementPtr {
    base: Expr,
    index: Expr,
    unsafe_access: bool,
    unbounded: bool,
    out_of_bounds: bool,
    elem_ty: Option<Type>,
}

#[derive(Debug, Clone)]
struct BlockAddrElementPtr {
    labels: Vec<String>,
    index: Expr,
}

#[derive(Debug, Clone)]
struct GlobalVar {
    source_name: String,
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
    source_name: String,
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
    fn rust_global_name(&self, name: &str) -> String {
        self.global_rust_names
            .get(name)
            .cloned()
            .unwrap_or_else(|| sanitize_ident(name).into_string())
    }

    fn floating_literal_at_source_location(
        &self,
        loc: &SourceLocation,
    ) -> Option<FloatingLiteralFact> {
        match loc {
            SourceLocation::File { file, line, column } => {
                let point = SourcePoint {
                    file: file.clone(),
                    line: *line,
                    col: *column,
                };
                self.floating_literals
                    .get(&FloatingLiteralLoc {
                        spelling: point.clone(),
                        expansion: point,
                    })
                    .cloned()
            }
            SourceLocation::Fused(locations) => locations
                .iter()
                .find_map(|loc| self.floating_literal_at_source_location(loc)),
            SourceLocation::Callsite {
                spelling,
                expansion,
            } => {
                let spelling = source_point(spelling)?;
                let expansion = source_point(expansion)?;
                self.floating_literals
                    .get(&FloatingLiteralLoc {
                        spelling,
                        expansion,
                    })
                    .cloned()
            }
            SourceLocation::Loc(_) | SourceLocation::Unknown => None,
        }
    }

    fn declaration_comment_items(&self, kind: &str, name: &str) -> Vec<Item> {
        self.declaration_comments
            .get(&(kind.to_string(), sanitize_ident(name).into_string()))
            .into_iter()
            .flatten()
            .cloned()
            .map(Item::Comment)
            .collect()
    }

    fn cross_module_path(&self, module: &str, name: &str) -> Path {
        let root = self
            .project
            .cross_module_crate
            .as_deref()
            .unwrap_or("crate");
        if module.is_empty() {
            Path::new([Ident::from(root), Ident::from(name)])
        } else {
            Path::new([Ident::from(root), Ident::from(module), Ident::from(name)])
        }
    }

    fn call_binding_at(&self, loc: Option<&SourceLocation>, direct: bool) -> CallBinding {
        if !direct {
            return CallBinding::Indirect;
        }
        loc.and_then(|raw| self.resolve_source_loc(raw))
            .and_then(|loc| self.call_bindings.get(&loc).cloned())
            .unwrap_or_else(|| CallBinding::direct_unknown(None))
    }

    fn resolve_source_loc(&self, raw: &SourceLocation) -> Option<Loc> {
        match raw {
            SourceLocation::File { line, column, .. } => Some(Loc {
                line: *line,
                col: *column,
            }),
            _ => None,
        }
    }

    fn long_double_extern_pointer_shim(&mut self, name: &str, ty: &Type) -> Option<String> {
        if !self.externs.contains_key(name) {
            return None;
        }
        let Type::FnPtr { params, ret, .. } = ty else {
            return None;
        };
        if !params.iter().any(type_mentions_long_double) && !type_mentions_long_double(ret) {
            return None;
        }
        let mut segments = vec![format!("r{}", long_double_shim_type_tag(ret))];
        segments.extend(params.iter().map(long_double_shim_type_tag));
        let shim_name = format!("__slate_{name}__{}", segments.join("_"));
        let ret = ret.as_ref().clone();
        self.long_double_shims
            .entry(shim_name.clone())
            .or_insert_with(|| ExternFnDecl {
                attrs: Vec::new(),
                identity: self
                    .known_functions
                    .get(name)
                    .copied()
                    .unwrap_or(FunctionIdentity::Unknown),
                name: shim_name.clone(),
                declared_type: self.function_types.get(name).cloned(),
                params: params
                    .iter()
                    .enumerate()
                    .map(|(i, ty)| FnParam {
                        name: format!("_{i}"),
                        mutable: false,
                        ty: ty.clone(),
                    })
                    .collect(),
                variadic: false,
                ret: (!ret.is_unit()).then_some(ret),
                safe: false,
            });
        Some(shim_name)
    }

    fn lower_module(&mut self, module: &Module, c: &Unit) -> Program {
        let mut items = vec![Item::CrateAttrs(vec![CrateAttr::Allow(vec![
            Lint::DeadCode,
            Lint::Unused,
            Lint::NonCamelCaseTypes,
            Lint::NonSnakeCase,
            Lint::NonUpperCaseGlobals,
            Lint::ArithmeticOverflow,
            Lint::SuspiciousRuntimeSymbolDefinitions,
            Lint::UnpredictableFunctionPointerComparisons,
            Lint::UnusedComparisons,
        ])])];

        self.c_abi_functions = c_abi_function_targets(module);
        self.c_abi_functions
            .extend(self.project.address_taken_functions.iter().cloned());
        self.va_list_boxed = !module_requires_native_va_list(
            module,
            &self.c_abi_functions,
            self.project.emit_pub,
            &self.aliases,
        );

        items.extend(bitfield_items(&self.bitfield_storages));

        for enm in &c.enums {
            let name = sanitize_ident(&enm.name).into_string();
            if self.project.shared_enums.contains(&name) {
                continue;
            }
            if let Some(item) = self.lower_enum(enm) {
                items.push(item);
            }
        }
        for name in self.record_defs.clone() {
            if self.project.shared_records.contains(&name) {
                continue;
            }
            if let Some(record) = self.records.get(&name).cloned() {
                items.extend(self.lower_record(&record));
            }
        }
        items.extend(self.standard_record_defs());

        self.weak_aliases = module
            .functions
            .iter()
            .filter(|function| function.linkage == GlobalLinkageKind::WeakAny)
            .filter_map(|function| Some((function.name.clone(), function.aliasee.clone()?)))
            .collect();
        self.external_weak_targets = self
            .weak_refs
            .values()
            .filter(|target| {
                !module
                    .functions
                    .iter()
                    .any(|function| function.name == **target)
            })
            .cloned()
            .collect();
        self.function_return_types = module
            .functions
            .iter()
            .map(|function| {
                (
                    function.name.clone(),
                    rust_type_with_aliases(&function.return_ty, &self.aliases, self.va_list_boxed),
                )
            })
            .collect();
        self.function_param_types = module
            .functions
            .iter()
            .map(|function| {
                (
                    function.name.clone(),
                    function
                        .params
                        .iter()
                        .map(|(_, ty)| {
                            rust_type_with_aliases(ty, &self.aliases, self.va_list_boxed)
                        })
                        .collect(),
                )
            })
            .collect();
        let mut assembly_strings = Vec::new();
        collect_assembly_strings(module, &mut assembly_strings);
        let asm_referenced_globals: BTreeSet<String> = module
            .globals
            .iter()
            .map(|global| global.name.as_str())
            .filter(|name| {
                assembly_strings
                    .iter()
                    .any(|assembly| assembly_mentions_symbol(assembly, name))
            })
            .map(|name| sanitize_ident(name).into_string())
            .collect();
        items.extend(lower_module_asm(module, &mut self.ctx.diagnostics));
        items.extend(lower_weak_alias_asm(
            module,
            &self.weak_aliases,
            &mut self.ctx.diagnostics,
        ));
        let has_main = module
            .functions
            .iter()
            .any(|function| function.name == "main" && !function.is_declaration());
        let hooks = collect_lifecycle_hooks(&module.functions, has_main, &mut self.ctx.diagnostics);
        self.ctor_calls = hooks.ctors;
        self.dtor_calls = hooks.dtors;
        self.used_symbols = collect_used_symbols(&module.globals);
        self.global_sym_types = module
            .globals
            .iter()
            .map(|global| (global.name.clone(), global.ty.clone()))
            .collect();
        for global in module
            .globals
            .iter()
            .filter(|global| global.name.starts_with(".str"))
        {
            self.collect_global(global);
        }
        for global in module
            .globals
            .iter()
            .filter(|global| !global.name.starts_with(".str"))
        {
            self.collect_global(global);
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
            if (global.name != global.source_name || !global.name.is_ascii())
                && let Some(no_mangle) = attrs
                    .iter()
                    .position(|attr| matches!(attr, RustAttr::NoMangle))
            {
                attrs[no_mangle] = RustAttr::ExportName(global.source_name.clone());
            }
            if global.thread_local {
                attrs.push(RustAttr::ThreadLocal);
                self.uses_thread_local.set(true);
            }
            items.extend(self.declaration_comment_items("VarDecl", &global.source_name));
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
            if let Some(module) = self.project.cross_module_globals.get(&global.source_name) {
                self.cross_uses.push(Item::Use {
                    path: self.cross_module_path(module, name),
                });
                continue;
            }
            extern_decls.push(ExternDecl::Static {
                attrs: global
                    .thread_local
                    .then_some(RustAttr::ThreadLocal)
                    .into_iter()
                    .chain(
                        (name != &global.source_name)
                            .then(|| RustAttr::LinkName(global.source_name.clone())),
                    )
                    .collect(),
                mutable: true,
                name: name.clone(),
                ty: global.ty.clone(),
            });
        }
        let mut emitted_weak_targets = BTreeSet::new();
        for function in module
            .functions
            .iter()
            .filter(|function| function.is_declaration())
        {
            if function.aliasee.is_some() {
                if function.linkage == GlobalLinkageKind::WeakAny {
                    let name = &function.name;
                    let (decl, params, ret) = self.extern_fn_signature(function);
                    if decl.variadic {
                        self.uses_c_variadic.set(true);
                    }
                    self.externs.insert(name.clone(), params);
                    self.extern_returns.insert(name.clone(), ret);
                    extern_decls.push(ExternDecl::Fn(decl));
                }
                continue;
            }
            let name = &function.name;
            if let Some(target) = self.weak_refs.get(name).cloned() {
                if self.external_weak_targets.contains(&target)
                    && emitted_weak_targets.insert(target.clone())
                {
                    let (decl, params, ret) = self.extern_fn_signature_as(&target, function);
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
                self.cross_uses.push(Item::Use {
                    path: self.cross_module_path(module, name),
                });
                continue;
            }
            let (mut decl, params, ret) = self.extern_fn_signature(function);
            if function.noreturn {
                decl.ret = Some(Type::Never);
            }
            self.externs.insert(name.clone(), params);
            self.extern_returns.insert(name.clone(), ret.clone());
            extern_decls.push(ExternDecl::Fn(decl));
        }
        if !extern_decls.is_empty() {
            items.push(Item::ExternBlock {
                abi: "C".into(),
                decls: extern_decls,
            });
        }

        for function in module.functions.iter().filter(|function| {
            !function.is_declaration() && function.name != "main" && function.varargs
        }) {
            let name = function.name.clone();
            self.variadic_defs.insert(name.clone());
            if self.va_list_boxed {
                self.boxed_variadic_defs.insert(name);
            }
        }
        if !self.boxed_variadic_defs.is_empty() {
            items.push(Item::SupportModule(SupportModule {
                name: "__slate_variadic".into(),
                source: r#"struct __SlateVaArg {
    value: Box<dyn std::any::Any>,
    size: usize,
}

impl __SlateVaArg {
    fn new<T: 'static>(value: T) -> Self {
        Self { value: Box::new(value), size: std::mem::size_of::<T>() }
    }

    fn read<T: Copy + 'static>(&self) -> T {
        if let Some(value) = self.value.downcast_ref::<T>() {
            return *value;
        }
        assert!(self.size >= std::mem::size_of::<T>());
        unsafe {
            std::ptr::read_unaligned(
                (self.value.as_ref() as *const dyn std::any::Any) as *const () as *const T,
            )
        }
    }
}

#[derive(Clone)]
struct __SlateVaArgs {
    args: Option<std::rc::Rc<Vec<__SlateVaArg>>>,
    index: usize,
}

impl __SlateVaArgs {
    fn new(args: Vec<__SlateVaArg>) -> Self {
        Self { args: Some(std::rc::Rc::new(args)), index: 0 }
    }

    const fn empty() -> Self {
        Self { args: None, index: 0 }
    }

    fn next_arg<T: Copy + 'static>(&mut self) -> T {
        if std::mem::size_of::<T>() == 0 {
            return unsafe { std::mem::zeroed() };
        }
        let args = self.args.as_ref().expect("va_arg with no arguments");
        let value = args[self.index].read::<T>();
        self.index += 1;
        value
    }
}"#
                .into(),
                exports: Vec::new(),
            }));
        }
        self.unsafe_functions
            .extend(unsafe_defined_functions(module));
        self.unsafe_functions.extend(
            self.target_feature_functions
                .keys()
                .filter(|name| name.as_str() != "main")
                .cloned(),
        );

        for function in &module.functions {
            let name = &function.name;
            let item = if function.is_declaration() {
                self.lower_func_alias(function, &module.functions)
            } else {
                self.lower_func(function)
            };
            if let Some(item) = item {
                items.extend(self.declaration_comment_items("FunctionDecl", name));
                items.push(item);
            }
        }

        let mut synthetic_externs = Vec::new();
        for (name, decl) in std::mem::take(&mut self.synthetic_externs) {
            if self.externs.contains_key(&name) {
                continue;
            }
            self.externs.insert(
                name.clone(),
                decl.params.iter().map(|param| param.ty.clone()).collect(),
            );
            self.extern_returns
                .insert(name, decl.ret.as_ref().map(Type::render));
            synthetic_externs.push(ExternDecl::Fn(decl));
        }
        if !synthetic_externs.is_empty() {
            items.splice(
                1..1,
                [Item::ExternBlock {
                    abi: "C".into(),
                    decls: synthetic_externs,
                }],
            );
        }

        let storage_items: Vec<Item> = self
            .generated_alloca_frames
            .iter()
            .cloned()
            .map(Item::Struct)
            .collect();
        items.splice(1..1, storage_items);

        for wrapper in self.enum_wrapper_fns.borrow().values() {
            items.push(Item::Fn(wrapper.clone()));
        }
        for (enum_name, from_ty) in self.needed_enum_from_impls.borrow().iter() {
            if let Some(item) = self.enum_from_impl_item(enum_name, from_ty) {
                items.push(item);
            }
        }

        if self.uses_long_double.get() {
            for decl in f80_shim_decls() {
                self.long_double_shims
                    .entry(decl.name.clone())
                    .or_insert(decl);
            }
        }
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
        if self.uses_fenv_shims.get() {
            items.push(Item::ExternBlock {
                abi: "C".into(),
                decls: fenv_shim_decls().into_iter().map(ExternDecl::Fn).collect(),
            });
        }
        if !self.llvm_intrinsic_shims.is_empty() {
            self.uses_llvm_intrinsics.set(true);
            if self
                .llvm_intrinsic_shims
                .values()
                .any(intrinsics::extern_fn_decl_mentions_simd)
            {
                self.uses_portable_simd.set(true);
            }
            items.push(Item::ExternBlock {
                abi: "unadjusted".into(),
                decls: self
                    .llvm_intrinsic_shims
                    .values()
                    .cloned()
                    .map(ExternDecl::Fn)
                    .collect(),
            });
        }
        if self.uses_long_double.get() && !self.project.shared_long_double {
            let vis = if self.project.emit_pub {
                Visibility::Pub
            } else {
                Visibility::Private
            };
            items.splice(1..1, long_double_prelude(vis));
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
            if self.project.shared_long_double {
                wiring.push(Item::Use {
                    path: Path::new([
                        Ident::from(root),
                        Ident::from(module.as_str()),
                        Ident::from(LONG_DOUBLE_TY),
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
        if self.uses_f16.get() {
            insert_crate_feature(&mut items, Feature::F16);
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
        if self.uses_llvm_intrinsics.get() {
            insert_crate_feature(&mut items, Feature::LinkLlvmIntrinsics);
            insert_crate_feature(&mut items, Feature::AbiUnadjusted);
        }
        if self.uses_portable_simd.get() {
            insert_crate_feature(&mut items, Feature::PortableSimd);
            insert_crate_feature(&mut items, Feature::SimdFfi);
        }
        for feature in &self.project.crate_features {
            insert_crate_feature(&mut items, *feature);
        }

        Program { items }
    }

    fn collect_global(&mut self, global: &CirGlobal) {
        let name = global.name.as_str();
        if matches!(name, "llvm.compiler.used" | "llvm.used") {
            return;
        }
        if global.aliasee.is_some() {
            return;
        }
        let rust_name = self.rust_global_name(name);
        let ty = Some(self.rust_type(&global.ty));
        let alignment = ty.as_ref().and_then(|ty| {
            global
                .alignment
                .and_then(|alignment| u32::try_from(alignment).ok())
                .filter(|alignment| *alignment > effective_type_alignment(ty, &self.records))
        });
        let weak = global.linkage == GlobalLinkageKind::WeakAny;
        let thread_local = global.tls_model.is_some();
        if thread_local {
            self.uses_thread_local.set(true);
        }
        if global.linkage == GlobalLinkageKind::Common
            && self.project.cross_module_globals.contains_key(name)
        {
            let Some(ty) = ty else {
                return;
            };
            self.extern_globals.insert(
                rust_name,
                ExternGlobal {
                    source_name: name.to_string(),
                    ty,
                    thread_local,
                },
            );
            return;
        }
        self.warn_protected_visibility(global.visibility, name);
        let section = global.section.clone();
        let used = self
            .used_symbols
            .get(&sanitize_ident(name).into_string())
            .cloned()
            .unwrap_or_default();
        let is_c_global = !name.starts_with("__") && !name.starts_with(".str");
        let Some(init) = global
            .initial_value
            .as_ref()
            .map(|attr| self.resolve_attr(attr))
        else {
            let Some(ty) = ty else {
                return;
            };
            self.extern_globals.insert(
                rust_name,
                ExternGlobal {
                    source_name: name.to_string(),
                    ty,
                    thread_local,
                },
            );
            return;
        };
        if let Attr::ConstArray {
            elts,
            trailing_zeros_num,
            ..
        } = init
            && let Attr::Str(text) = &**elts
        {
            let Some(mut bytes) = cir_string_bytes(text) else {
                return;
            };
            bytes.extend(std::iter::repeat_n(
                0,
                usize::try_from(*trailing_zeros_num).unwrap_or(0),
            ));
            if is_c_global && let Some(ty) = ty {
                let len = type_array_len(&ty)
                    .and_then(|len| usize::try_from(len).ok())
                    .unwrap_or(bytes.len());
                let elems = byte_array_elems(&bytes, &ty);
                self.globals.insert(
                    rust_name.clone(),
                    GlobalVar {
                        source_name: name.to_string(),
                        name: rust_name,
                        ty,
                        init: render_array_literal_expr(
                            &elems,
                            len,
                            Expr::Value(RustValue::I64(0)),
                        ),
                        alignment,
                        thread_local,
                        external: typed_global_is_exported(global)
                            || self.project.cross_referenced_globals.contains(name),
                        weak,
                        section: section.clone(),
                        used: used.clone(),
                    },
                );
            } else {
                self.strings.insert(name.to_string(), bytes);
            }
            return;
        }
        if let Attr::ConstArray { elts, .. } = init
            && let Some(items) = attr_array_values(elts)
        {
            if let Some(labels) = block_addr_labels(items) {
                self.block_addr_globals.insert(rust_name.clone(), labels);
                if is_c_global && let Some(ty) = ty {
                    let init = self.default_value_expr(&ty);
                    self.globals.insert(
                        rust_name.clone(),
                        GlobalVar {
                            source_name: name.to_string(),
                            name: rust_name,
                            ty,
                            init,
                            alignment,
                            thread_local,
                            external: typed_global_is_exported(global)
                                || self.project.cross_referenced_globals.contains(name),
                            weak,
                            section: section.clone(),
                            used: used.clone(),
                        },
                    );
                }
                return;
            }
            let elem_is_byte_sized = matches!(
                &ty,
                Some(Type::Array { elem, .. }) if matches!(**elem, Type::Prim(Prim::I8 | Prim::U8))
            );
            // synthetic (non-`is_c_global`) element arrays of byte-typed
            // elements are narrow string literals in per-element CIR form -
            // render them the same way narrow string literals are.
            if !is_c_global
                && elem_is_byte_sized
                && let Some(bytes) = items
                    .iter()
                    .map(|item| item.as_int().and_then(|value| u8::try_from(value).ok()))
                    .collect::<Option<Vec<u8>>>()
            {
                self.strings.insert(name.to_string(), bytes);
                return;
            }
            // wide-char string literals (`s32i`/`s16i` code-point elements)
            // can't go through the byte-string path above - render them as
            // a plain integer-element const array instead.
            if !is_c_global
                && name.starts_with(".str")
                && !elem_is_byte_sized
                && let Some(values) = items
                    .iter()
                    .map(Attr::as_int)
                    .collect::<Option<Vec<i128>>>()
            {
                let total_len = ty
                    .as_ref()
                    .and_then(type_array_len)
                    .unwrap_or(values.len() as u64) as usize;
                let mut elems: Vec<Expr> = values
                    .into_iter()
                    .map(|value| Expr::Value(RustValue::I64(value as i64)))
                    .collect();
                elems.resize(total_len, Expr::Value(RustValue::I64(0)));
                if let Some(ty) = ty.clone() {
                    self.globals.insert(
                        rust_name.clone(),
                        GlobalVar {
                            source_name: name.to_string(),
                            name: rust_name,
                            ty,
                            init: Expr::ArrayLit(elems),
                            alignment,
                            thread_local,
                            external: false,
                            weak,
                            section: section.clone(),
                            used: used.clone(),
                        },
                    );
                }
                return;
            }
        }
        if let Attr::Zero { .. } = init
            && !ty
                .as_ref()
                .is_some_and(|ty| matches!(ty, Type::VaList) || is_boxed_va_args_type(ty))
            && let Some((elem, len)) = global.ty.as_array()
        {
            if is_c_global && let Some(ty) = ty {
                self.globals.insert(
                    rust_name.clone(),
                    GlobalVar {
                        source_name: name.to_string(),
                        name: rust_name,
                        init: self.default_value_expr(&Type::Array {
                            elem: Box::new(self.rust_type(elem)),
                            len,
                        }),
                        ty,
                        alignment,
                        thread_local,
                        external: typed_global_is_exported(global)
                            || self.project.cross_referenced_globals.contains(name),
                        weak,
                        section: section.clone(),
                        used: used.clone(),
                    },
                );
            } else if matches!(elem, CirType::Named(n) if n == "s8i") && name.starts_with(".str") {
                self.strings.insert(name.to_string(), vec![0; len as usize]);
            } else if name.starts_with(".str")
                && resolved_integer_parts(elem, &self.aliases).is_some()
                && let Some(ty) = ty
            {
                self.globals.insert(
                    rust_name.clone(),
                    GlobalVar {
                        source_name: name.to_string(),
                        name: rust_name,
                        init: self.default_value_expr(&ty),
                        ty,
                        alignment,
                        thread_local,
                        external: false,
                        weak,
                        section: section.clone(),
                        used: used.clone(),
                    },
                );
            } else if resolved_integer_parts(elem, &self.aliases).is_some() {
                self.const_arrays.insert(name.to_string(), Vec::new());
            } else {
                self.const_zero_globals.insert(name.to_string());
            }
            return;
        }
        if let Attr::Zero { .. } = init {
            if is_c_global && let Some(ty) = ty {
                self.globals.insert(
                    rust_name.clone(),
                    GlobalVar {
                        source_name: name.to_string(),
                        name: rust_name,
                        init: self.default_value_expr(&ty),
                        ty,
                        alignment,
                        thread_local,
                        external: typed_global_is_exported(global)
                            || self.project.cross_referenced_globals.contains(name),
                        weak,
                        section: section.clone(),
                        used: used.clone(),
                    },
                );
            } else {
                self.const_zero_globals.insert(name.to_string());
            }
            return;
        }
        if is_c_global && let Some(ty) = ty {
            let mut facts: VecDeque<FloatingLiteralFact> = self
                .global_floating_literals
                .get(name)
                .cloned()
                .unwrap_or_default()
                .into();
            let Some(init) = self.render_const_value_expr(&ty, init, &mut facts) else {
                if type_mentions_long_double(&ty) {
                    self.ctx.diagnostics.error(format!(
                        "lower: global `{name}` has a long double constant without a matching Clang AST value"
                    ));
                }
                return;
            };
            let external = typed_global_is_exported(global)
                || self.project.cross_referenced_globals.contains(name);
            self.globals.insert(
                rust_name.clone(),
                GlobalVar {
                    source_name: name.to_string(),
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
        } else if !is_c_global {
            self.const_aggregates.insert(name.to_string(), init.clone());
        }
    }

    fn lower_enum(&mut self, enm: &crate::frontend::c_ast::Enum) -> Option<Item> {
        lower_enum_def(enm, Visibility::Private).map(Item::Enum)
    }

    fn lower_record(&mut self, record: &crate::frontend::c_ast::Record) -> Vec<Item> {
        if record
            .fields
            .iter()
            .any(|field| ctype_uses_long_double(&field.ty))
        {
            self.uses_long_double.set(true);
        }
        let storage_record;
        let mut synthesized_storage = false;
        let record = if record.kind == RecordKind::Union && record.fields.is_empty() {
            synthesized_storage = true;
            storage_record = crate::frontend::c_ast::Record {
                fields: vec![crate::frontend::c_ast::Decl {
                    name: "__slate_empty".into(),
                    comments: Vec::new(),
                    ty: crate::frontend::c_ast::CType::Array(
                        Box::new(crate::frontend::c_ast::CType::Int {
                            signed: false,
                            bits: 8,
                        }),
                        Some(0),
                    ),
                    bit_width: None,
                }],
                ..record.clone()
            };
            &storage_record
        } else if let Some(fields) = self.bitfield_storage_fields(record) {
            synthesized_storage = true;
            storage_record = crate::frontend::c_ast::Record {
                fields,
                ..record.clone()
            };
            &storage_record
        } else {
            record
        };
        let cir_field_types = if synthesized_storage {
            None
        } else {
            self.cir_record_field_types(record)
        };
        lower_record_def(
            record,
            Visibility::Private,
            Visibility::Private,
            true,
            self.va_list_boxed,
            cir_field_types.as_deref(),
        )
    }

    fn cir_record_field_types(&self, record: &crate::frontend::c_ast::Record) -> Option<Vec<Type>> {
        cir_record_field_types_from_aliases(record, &self.aliases, self.va_list_boxed)
    }

    fn record_field_type_at(
        &self,
        record: &crate::frontend::c_ast::Record,
        index: usize,
    ) -> Option<Type> {
        let field = record.fields.get(index)?;
        if matches!(field.ty, CType::FuncPtr { .. })
            && let Some(field_ty) = self
                .cir_record_field_types(record)
                .and_then(|types| types.get(index).cloned())
        {
            return Some(field_ty);
        }
        Some(self.c_record_field_type(&field.ty))
    }

    fn bitfield_storage_fields(
        &self,
        record: &crate::frontend::c_ast::Record,
    ) -> Option<Vec<crate::frontend::c_ast::Decl>> {
        if record.fields.is_empty() || record.fields.iter().all(|field| field.bit_width.is_none()) {
            return None;
        }
        let expanded = self.aliases.values().find(|ty| {
            slate_record_name(ty).is_some_and(|name| {
                sanitize_ident(name).as_str() == sanitize_ident(&record.name).as_str()
            })
        })?;
        let CirType::Struct {
            members: Some(members),
            ..
        } = expanded
        else {
            return None;
        };
        let preserve_field_names = record
            .fields
            .iter()
            .any(|field| field.name.starts_with("__bitfield_"));
        let record_name = sanitize_ident(&record.name).into_string();
        Some(
            members
                .iter()
                .enumerate()
                .map(|(index, ty)| crate::frontend::c_ast::Decl {
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
                    ty: self
                        .bitfield_storages
                        .get(&(record_name.clone(), index))
                        .map(|storage| {
                            crate::frontend::c_ast::CType::Record(storage.wrapper.clone())
                        })
                        .unwrap_or_else(|| cir_type_to_ctype(ty, &self.aliases)),
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
                .any(|ty| slate_record_name(ty) == Some(name))
            {
                out.push(Item::Record(standard_record_def(name)));
            }
        }
        out
    }

    fn rust_type(&self, cir_ty: &CirType) -> Type {
        let ty = rust_type_with_aliases(cir_ty, &self.aliases, self.va_list_boxed);
        if type_mentions_long_double(&ty) {
            self.uses_long_double.set(true);
        }
        if type_mentions_complex(&ty) {
            self.uses_complex.set(true);
        }
        if type_mentions_f128(&ty) {
            self.uses_f128.set(true);
        }
        if type_mentions_f16(&ty) {
            self.uses_f16.set(true);
        }
        ty
    }

    fn rust_type_has_long_double(&self, ty: &Type) -> bool {
        match ty {
            Type::LongDouble => true,
            Type::Custom(name) => self.records.get(name).is_some_and(|record| {
                record
                    .fields
                    .iter()
                    .any(|field| ctype_uses_long_double(&field.ty))
            }),
            Type::Complex(inner)
            | Type::Ref { inner, .. }
            | Type::Slice(inner)
            | Type::Ptr { inner, .. }
            | Type::Array { elem: inner, .. } => self.rust_type_has_long_double(inner),
            Type::FnPtr { params, ret, .. } => {
                params
                    .iter()
                    .any(|param| self.rust_type_has_long_double(param))
                    || self.rust_type_has_long_double(ret)
            }
            Type::Generic { args, .. } => {
                args.iter().any(|arg| self.rust_type_has_long_double(arg))
            }
            _ => false,
        }
    }

    fn cir_type_is_union(&self, ty: &CirType) -> bool {
        let ty = self.expand_alias(ty);
        if matches!(ty, CirType::Union { .. }) {
            return true;
        }
        slate_record_name(ty)
            .and_then(|name| self.records.get(sanitize_ident(name).as_str()))
            .is_some_and(|record| record.kind == RecordKind::Union)
    }

    fn resolve_attr<'b>(&'b self, attr: &'b Attr) -> &'b Attr {
        let mut current = attr;
        let mut seen = BTreeSet::new();
        while let Attr::Named(name) = current {
            if !seen.insert(name.clone()) {
                break;
            }
            match self.attr_aliases.get(name) {
                Some(next) => current = next,
                None => break,
            }
        }
        current
    }

    fn expand_alias<'b>(&'b self, ty: &'b CirType) -> &'b CirType {
        let mut ty = ty;
        let mut seen = BTreeSet::new();
        while let CirType::Named(name) = ty {
            if !seen.insert(name.clone()) {
                break;
            }
            match self.aliases.get(name) {
                Some(expanded) => ty = expanded,
                None => break,
            }
        }
        ty
    }

    fn record_field_type(&self, ty: &crate::frontend::c_ast::CType) -> Type {
        if ctype_uses_long_double(ty) {
            self.uses_long_double.set(true);
        }
        self.c_record_field_type(ty)
    }

    fn enum_return_mismatch_wrap(&self, fn_name: &str, target_ty: &Type) -> Option<Expr> {
        let Type::FnPtr { ret, params, .. } = target_ty else {
            return None;
        };
        let Type::Custom(enum_name) = ret.as_ref() else {
            return None;
        };
        if !self.enums.contains_key(enum_name) {
            return None;
        }
        let source_ret = self.function_return_types.get(fn_name)?;
        if source_ret == &Type::Custom(enum_name.clone()) {
            return None;
        }
        Some(self.wrap_enum_returning_function(fn_name, source_ret.clone(), enum_name, params))
    }

    fn wrap_enum_returning_function(
        &self,
        fn_name: &str,
        source_ret: Type,
        enum_name: &str,
        params: &[Type],
    ) -> Expr {
        self.needed_enum_from_impls
            .borrow_mut()
            .insert((enum_name.to_string(), source_ret));
        let wrapper_name = format!("__slate_enum_wrap_{fn_name}");
        let fn_params: Vec<FnParam> = params
            .iter()
            .enumerate()
            .map(|(i, ty)| FnParam {
                name: format!("_{i}"),
                mutable: false,
                ty: ty.clone(),
            })
            .collect();
        let args: Vec<Expr> = fn_params
            .iter()
            .map(|p| Expr::Var(p.name.clone().into()))
            .collect();
        self.enum_wrapper_fns
            .borrow_mut()
            .entry(wrapper_name.clone())
            .or_insert_with(|| FnDef {
                attrs: Vec::new(),
                vis: Visibility::Private,
                unsafe_: false,
                abi: Some(Abi::C),
                name: wrapper_name.clone(),
                params: fn_params,
                ret: Some(Type::Custom(enum_name.to_string())),
                body: vec![IndentStmt {
                    depth: 1,
                    stmt: Stmt::Return(Some(Expr::Call {
                        binding: crate::function_identity::CallBinding::Generated,
                        func: Box::new(Expr::Path(Path::new([enum_name, "from"].map(Ident::from)))),
                        args: vec![Expr::Call {
                            binding: crate::function_identity::CallBinding::Generated,
                            func: Box::new(Expr::Var(sanitize_ident(fn_name))),
                            args,
                        }],
                    })),
                }],
            });
        Expr::Call {
            binding: crate::function_identity::CallBinding::Generated,
            func: Box::new(Expr::Var("Some".into())),
            args: vec![Expr::Var(wrapper_name.into())],
        }
    }

    fn enum_from_impl_item(&self, enum_name: &str, from_ty: &Type) -> Option<Item> {
        let enm = self.enums.get(enum_name)?;
        let arms = enm
            .variants
            .iter()
            .map(|variant| ExprMatchArm {
                pattern: int_pattern(variant.value as i128),
                value: Expr::Path(Path::new([
                    Ident::from(enum_name),
                    Ident::from(sanitize_ident(&variant.name).as_str()),
                ])),
            })
            .chain(std::iter::once(ExprMatchArm {
                pattern: Pattern::Wildcard,
                value: Expr::Macro {
                    name: "unreachable".into(),
                    args: Vec::new(),
                },
            }))
            .collect();
        Some(Item::Impl(ImplBlock {
            generics: vec![],
            trait_: Some(TraitRef::From(from_ty.clone())),
            self_ty: Type::Custom(enum_name.to_string()),
            items: vec![ImplItem::Method(Method {
                name: "from".into(),
                self_kind: SelfKind::None,
                params: vec![FnParam {
                    name: "value".into(),
                    mutable: false,
                    ty: from_ty.clone(),
                }],
                ret: Some(Type::Custom(enum_name.to_string())),
                body: Expr::Match {
                    expr: Box::new(Expr::Var("value".into())),
                    arms,
                },
            })],
        }))
    }

    fn default_value_expr(&self, ty: &Type) -> Expr {
        if is_boxed_va_args_type(ty) {
            return empty_va_args_expr();
        }
        match ty {
            Type::Custom(name) => {
                if let Some(storage) = self
                    .bitfield_storages
                    .values()
                    .find(|storage| storage.wrapper == *name)
                {
                    return Expr::Transmute {
                        from: storage.backing.clone(),
                        to: ty.clone(),
                        expr: Box::new(self.default_value_expr(&storage.backing)),
                    };
                }
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
                        if record.kind == RecordKind::Union {
                            let field = fields.first().expect("bitfield record has storage");
                            return Expr::StructLit {
                                name: record_lit_name(record),
                                fields: vec![(
                                    field.name.clone(),
                                    self.default_value_expr(&self.c_record_field_type(&field.ty)),
                                )],
                            };
                        }
                        return Expr::StructLit {
                            name: record_lit_name(record),
                            fields: fields
                                .iter()
                                .map(|field| {
                                    (
                                        field.name.clone(),
                                        self.default_value_expr(
                                            &self.c_record_field_type(&field.ty),
                                        ),
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
                                        self.default_value_expr(
                                            &self.c_record_field_type(&field.ty),
                                        ),
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
                                            self.default_value_expr(
                                                &self.c_record_field_type(&field.ty),
                                            ),
                                        )],
                                    },
                                );
                            }
                            return Expr::StructLit {
                                name: record_lit_name(record),
                                fields: vec![("__slate_empty".into(), Expr::ArrayLit(Vec::new()))],
                            };
                        }
                    }
                }
                standard_record_default_expr(name).unwrap_or_else(|| default_value_for_type(ty))
            }
            Type::LongDouble => long_double_zero_expr(),
            Type::Complex(inner) => {
                let d = default_value_for_type(inner);
                Expr::StructLit {
                    name: COMPLEX_TY.into(),
                    fields: vec![("re".into(), d.clone()), ("im".into(), d)],
                }
            }
            Type::Array { elem, .. } if is_boxed_va_args_type(elem) => Expr::Call {
                binding: crate::function_identity::CallBinding::Generated,
                func: Box::new(Expr::Var("core::array::from_fn".into())),
                args: vec![Expr::Closure {
                    params: vec![Ident::from("_")],
                    body: Box::new(self.default_value_expr(elem)),
                }],
            },
            Type::Array { elem, len } => Expr::ArrayRepeat {
                elem: Box::new(self.default_value_expr(elem)),
                len: *len as usize,
            },
            Type::CLib(ty) => match ty.initializer() {
                CLibInitializer::ScalarZero => Expr::Value(RustValue::I64(0)),
                CLibInitializer::Zeroed => {
                    Expr::Unsafe(Box::new(crate::backend::rust_ast::Block {
                        stmts: Vec::new(),
                        tail: Some(Box::new(Expr::Call {
                            binding: crate::function_identity::CallBinding::Generated,
                            func: Box::new(Expr::Var(
                                format!("std::mem::zeroed::<{}>", ty.path()).into(),
                            )),
                            args: Vec::new(),
                        })),
                    }))
                }
                CLibInitializer::Fields(fields) => Expr::StructLit {
                    name: ty.path().into(),
                    fields: fields
                        .iter()
                        .map(|field| ((*field).into(), Expr::Value(RustValue::I64(0))))
                        .collect(),
                },
            },
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
            LayoutQuery::Size(ty @ crate::frontend::c_ast::CType::Record(_)) => {
                Some(layout_call("size_of", &self.c_type_to_type(ty)))
            }
            LayoutQuery::Align(ty @ crate::frontend::c_ast::CType::Record(_)) => {
                Some(layout_call("align_of", &self.c_type_to_type(ty)))
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

    fn render_const_value_expr(
        &self,
        ty: &Type,
        attr: &Attr,
        facts: &mut VecDeque<FloatingLiteralFact>,
    ) -> Option<Expr> {
        let attr = self.resolve_attr(attr);
        if let Type::Custom(name) = ty
            && let Some(storage) = self
                .bitfield_storages
                .values()
                .find(|storage| storage.wrapper == *name)
        {
            return Some(Expr::Transmute {
                from: storage.backing.clone(),
                to: ty.clone(),
                expr: Box::new(self.render_const_value_expr(&storage.backing, attr, facts)?),
            });
        }
        if let Type::Custom(name) = ty
            && self
                .records
                .get(name)
                .is_some_and(|record| record.kind == RecordKind::Union && record.fields.is_empty())
        {
            return Some(self.default_value_expr(ty));
        }
        if let Type::CLib(clib) = ty
            && clib.initializer() == CLibInitializer::Zeroed
        {
            return Some(self.default_value_expr(ty));
        }
        match attr {
            Attr::ConstComplex { real, imag, .. } => {
                let re = complex_component_from_attr(real)?;
                let im = complex_component_from_attr(imag)?;
                Some(complex_const_expr(Some(ty), re, im))
            }
            Attr::ConstRecord { members, .. } => {
                let elements = attr_array_values(members)?;
                let Type::Custom(name) = ty else {
                    return None;
                };
                let record = self.records.get(name)?;
                match record.kind {
                    RecordKind::Struct => {
                        if let Some(storage_fields) = self.bitfield_storage_fields(record) {
                            let fields = storage_fields
                                .iter()
                                .enumerate()
                                .map(|(i, field)| {
                                    let field_ty = self.c_record_field_type(&field.ty);
                                    let value = elements
                                        .get(i)
                                        .and_then(|e| {
                                            self.render_const_value_expr(&field_ty, e, facts)
                                        })
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
                                let field_ty = self
                                    .record_field_type_at(record, i)
                                    .unwrap_or_else(|| self.c_record_field_type(&field.ty));
                                let value = elements
                                    .get(i)
                                    .and_then(|e| self.render_const_value_expr(&field_ty, e, facts))
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
                        let storage_fields = self.bitfield_storage_fields(record);
                        let field = storage_fields
                            .as_ref()
                            .and_then(|fields| fields.first())
                            .or_else(|| record.fields.first())?;
                        let field_ty = self.c_record_field_type(&field.ty);
                        let value = elements
                            .first()
                            .and_then(|e| self.render_const_value_expr(&field_ty, e, facts))
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
            }
            Attr::ConstArray { elts, .. } => {
                let Type::Array { elem, len } = ty else {
                    return None;
                };
                if let Attr::Str(text) = &**elts {
                    let elems = byte_array_elems(&cir_string_bytes(text)?, ty);
                    return Some(render_array_literal_expr(
                        &elems,
                        *len as usize,
                        self.default_value_expr(elem),
                    ));
                }
                let len = *len as usize;
                let mut out: Vec<Expr> = attr_array_values(elts)?
                    .iter()
                    .take(len)
                    .map(|value| {
                        self.render_const_value_expr(elem, value, facts)
                            .unwrap_or_else(|| self.default_value_expr(elem))
                    })
                    .collect();
                out.resize(len, self.default_value_expr(elem));
                Some(Expr::ArrayLit(out))
            }
            Attr::ConstVector { elts, .. } => Some(Expr::ArrayLit(
                attr_array_values(elts)?
                    .iter()
                    .map(scalar_attr_expr)
                    .collect::<Option<Vec<_>>>()?,
            )),
            _ if matches!(ty, Type::LongDouble) => {
                if let Some(fact) = facts.pop_front() {
                    if fact.bit_width == 80 && !fact.bits.is_empty() {
                        let bits = fact.bits.trim_start_matches("0x").trim_start_matches("0X");
                        f80_literal_bits_expr(bits)
                    } else if !fact.value.is_empty() {
                        f80_literal_expr(&fact.value)
                    } else {
                        None
                    }
                } else {
                    match attr {
                        Attr::Float { text, .. } | Attr::CirFloat { value: text, .. }
                            if cir_positive_zero(text) =>
                        {
                            Some(self.default_value_expr(ty))
                        }
                        _ => None,
                    }
                }
            }
            Attr::Zero { .. } => Some(self.default_value_expr(ty)),
            Attr::ConstPtr { value, .. } if is_null_ptr_value(value) => {
                Some(if matches!(ty, Type::FnPtr { .. }) {
                    Expr::Value(RustValue::None)
                } else {
                    Expr::Value(RustValue::NullPtr)
                })
            }
            Attr::ConstPtr { value, .. } => value.as_int().map(|addr| Expr::Cast {
                expr: Box::new(int_value_expr(addr)),
                ty: ty.clone(),
            }),
            Attr::Int { value, .. } if matches!(ty, Type::Custom(name) if self.enums.contains_key(name)) =>
            {
                let Type::Custom(name) = ty else {
                    unreachable!()
                };
                let enm = self.enums.get(name)?;
                let variant = enm
                    .variants
                    .iter()
                    .find(|variant| i128::from(variant.value) == *value)?;
                Some(Expr::Path(Path::new([
                    Ident::from(name.as_str()),
                    Ident::from(sanitize_ident(&variant.name).as_str()),
                ])))
            }
            Attr::CirInt { value, .. } if matches!(ty, Type::Custom(name) if self.enums.contains_key(name)) => {
                self.render_const_value_expr(
                    ty,
                    &Attr::Int {
                        value: value.parse().ok()?,
                        ty: None,
                    },
                    facts,
                )
            }
            Attr::GlobalView {
                symbol, indices, ..
            } => {
                let indices = match indices.as_deref() {
                    Some(Attr::Array(values)) => values
                        .iter()
                        .map(Attr::as_int)
                        .collect::<Option<Vec<_>>>()?,
                    _ => Vec::new(),
                };
                self.global_view_init_expr(symbol, &indices, ty)
            }
            Attr::Float { text, .. } => {
                Some(fp_literal_expr_for_type(Some(ty), fp_text_value(text)?))
            }
            _ => scalar_attr_expr(attr),
        }
    }

    fn global_view_init_expr(&self, target: &str, indices: &[i128], ty: &Type) -> Option<Expr> {
        if let Some(bytes) = self.strings.get(target) {
            let ptr_expr = Expr::MethodCall {
                recv: Box::new(Expr::ByteStr(bytes.clone())),
                method: "as_ptr".into(),
                args: Vec::new(),
            };
            let ptr_expr = match indices {
                [] => ptr_expr,
                [offset] => Expr::Unsafe(Box::new(crate::backend::rust_ast::Block {
                    stmts: Vec::new(),
                    tail: Some(Box::new(Expr::MethodCall {
                        recv: Box::new(ptr_expr),
                        method: "add".into(),
                        args: vec![int_value_expr(*offset)],
                    })),
                })),
                _ => return None,
            };
            return Some(Expr::Cast {
                expr: Box::new(ptr_expr),
                ty: ty.clone(),
            });
        }
        if matches!(ty, Type::FnPtr { .. }) {
            if let Some(wrapped) = self.enum_return_mismatch_wrap(target, ty) {
                return Some(wrapped);
            }
            let raw_ptr = Type::Ptr {
                mutable: false,
                inner: Box::new(Type::Unit),
            };
            return Some(Expr::Transmute {
                from: raw_ptr.clone(),
                to: ty.clone(),
                expr: Box::new(Expr::Cast {
                    expr: Box::new(Expr::Var(self.rust_global_name(target).into())),
                    ty: raw_ptr,
                }),
            });
        }
        let Type::Ptr { mutable, .. } = ty else {
            return None;
        };
        if self.function_return_types.contains_key(target) {
            return Some(Expr::Cast {
                expr: Box::new(Expr::Cast {
                    expr: Box::new(Expr::Var(self.rust_global_name(target).into())),
                    ty: Type::Ptr {
                        mutable: false,
                        inner: Box::new(Type::Unit),
                    },
                }),
                ty: ty.clone(),
            });
        }
        if indices.is_empty() {
            return Some(Expr::Cast {
                expr: Box::new(Expr::AddrOf {
                    mutable: *mutable,
                    expr: Box::new(Expr::Var(self.rust_global_name(target).into())),
                }),
                ty: ty.clone(),
            });
        }
        let base_ty = self
            .global_sym_types
            .get(target)
            .map(|ty| self.rust_type(ty))?;
        let path = self.global_view_index_path(target, &base_ty, indices)?;
        let addr_expr = Expr::AddrOf {
            mutable: *mutable,
            expr: Box::new(path),
        };
        Some(Expr::Cast {
            expr: Box::new(Expr::Unsafe(Box::new(crate::backend::rust_ast::Block {
                stmts: Vec::new(),
                tail: Some(Box::new(addr_expr)),
            }))),
            ty: ty.clone(),
        })
    }

    fn global_view_index_path(
        &self,
        target: &str,
        base_ty: &Type,
        indices: &[i128],
    ) -> Option<Expr> {
        let raw_ptr_ty = Type::Ptr {
            mutable: true,
            inner: Box::new(base_ty.clone()),
        };
        let mut expr = Expr::Unary {
            op: UnaryOp::Deref,
            expr: Box::new(Expr::Cast {
                expr: Box::new(Expr::AddrOf {
                    mutable: true,
                    expr: Box::new(Expr::Var(self.rust_global_name(target).into())),
                }),
                ty: raw_ptr_ty,
            }),
        };
        let mut ty = base_ty.clone();
        for &index in indices {
            match ty {
                Type::Array { elem, .. } => {
                    expr = Expr::Index {
                        base: Box::new(expr),
                        index: Box::new(int_value_expr(index)),
                    };
                    ty = *elem;
                }
                Type::Custom(name) => {
                    let record = self.records.get(&name)?;
                    let field = record.fields.get(usize::try_from(index).ok()?)?;
                    expr = Expr::Field {
                        base: Box::new(expr),
                        field: sanitize_ident(&field.name).into_string(),
                    };
                    ty = self.record_field_type(&field.ty);
                }
                _ => return None,
            }
        }
        Some(expr)
    }
}

fn builtin_setjmp_ptr_ptr_void() -> Type {
    Type::Ptr {
        mutable: true,
        inner: Box::new(Type::Ptr {
            mutable: true,
            inner: Box::new(Type::CLib(CLibType::VOID)),
        }),
    }
}

fn builtin_setjmp_extern_decl() -> ExternFnDecl {
    ExternFnDecl {
        attrs: Vec::new(),
        name: "setjmp".into(),
        identity: FunctionIdentity::Unknown,
        declared_type: None,
        params: vec![FnParam {
            name: "_0".into(),
            mutable: false,
            ty: builtin_setjmp_ptr_ptr_void(),
        }],
        variadic: false,
        ret: Some(Type::Prim(Prim::I32)),
        safe: false,
    }
}

fn builtin_longjmp_extern_decl() -> ExternFnDecl {
    ExternFnDecl {
        attrs: Vec::new(),
        name: "longjmp".into(),
        identity: FunctionIdentity::Unknown,
        declared_type: None,
        params: vec![
            FnParam {
                name: "_0".into(),
                mutable: false,
                ty: builtin_setjmp_ptr_ptr_void(),
            },
            FnParam {
                name: "_1".into(),
                mutable: false,
                ty: Type::Prim(Prim::I32),
            },
        ],
        variadic: false,
        ret: Some(Type::Never),
        safe: false,
    }
}

fn repair_extern_function_signature(decl: &mut ExternFnDecl, spelling: Option<&str>) {
    let mut params: Vec<Type> = decl.params.iter().map(|param| param.ty.clone()).collect();
    let mut ret = decl.ret.clone();
    if !repair_function_signature(spelling, &mut params, &mut ret) {
        return;
    }
    decl.params = params
        .into_iter()
        .enumerate()
        .map(|(index, ty)| FnParam {
            name: format!("_{index}"),
            mutable: false,
            ty,
        })
        .collect();
    decl.ret = ret;
}

pub fn is_clib_record_type(name: &str) -> bool {
    clib_record_type(name).is_some()
}

fn clib_record_type(name: &str) -> Option<CLibType> {
    CLIB_RECORD_TYPES
        .iter()
        .copied()
        .find(|ty| ty.c_name() == name)
}

fn c_va_list_shaped_type(ty: &crate::frontend::c_ast::CType, boxed: bool) -> Option<Type> {
    use crate::frontend::c_ast::CType;
    let value_ty = || {
        if boxed {
            Type::Custom("__SlateVaArgs".into())
        } else {
            Type::VaList
        }
    };
    let is_va_list_record =
        |ty: &CType| matches!(ty, CType::Record(name) if name == "__va_list_tag");
    if is_va_list_record(ty) {
        return Some(value_ty());
    }
    if let CType::Array(inner, Some(1)) = ty
        && is_va_list_record(inner)
    {
        return Some(value_ty());
    }
    if let CType::Array(inner, Some(len)) = ty
        && let Some(inner_ty) = c_va_list_shaped_type(inner, boxed)
    {
        return Some(Type::Array {
            elem: Box::new(inner_ty),
            len: *len,
        });
    }
    if let CType::Ptr(inner) = ty
        && let Some(inner_ty) = c_va_list_shaped_type(inner, boxed)
    {
        return Some(Type::Ptr {
            mutable: true,
            inner: Box::new(inner_ty),
        });
    }
    None
}

impl<'a> Lowerer<'a> {
    fn c_type_to_type(&self, ty: &crate::frontend::c_ast::CType) -> Type {
        c_type_to_type(ty, self.va_list_boxed)
    }

    fn c_record_field_type(&self, ty: &crate::frontend::c_ast::CType) -> Type {
        c_record_field_type(ty, self.va_list_boxed)
    }
}

fn c_type_to_type(ty: &crate::frontend::c_ast::CType, va_list_boxed: bool) -> Type {
    use crate::frontend::c_ast::CType;
    if let Some(va_list_ty) = c_va_list_shaped_type(ty, va_list_boxed) {
        return va_list_ty;
    }
    let ptr = |inner: &CType| Type::Ptr {
        mutable: true,
        inner: Box::new(c_type_to_type(inner, va_list_boxed)),
    };
    match ty {
        CType::Void => Type::Unit,
        CType::Bool => Type::Prim(Prim::Bool),
        CType::Int { signed, bits } => match (signed, bits) {
            (true, 8) => Type::Prim(Prim::I8),
            (false, 8) => Type::Prim(Prim::U8),
            (true, 16) => Type::Prim(Prim::I16),
            (false, 16) => Type::Prim(Prim::U16),
            (true, 32) => Type::Prim(Prim::I32),
            (false, 32) => Type::Prim(Prim::U32),
            (true, 64) => Type::Prim(Prim::I64),
            (false, 64) => Type::Prim(Prim::U64),
            (true, 128) => Type::Prim(Prim::I128),
            (false, 128) => Type::Prim(Prim::U128),
            (signed, bits) => bitint_type(*signed, *bits),
        },
        CType::Float { bits: 16 } => Type::Prim(Prim::F16),
        CType::Float { bits: 32 } => Type::Prim(Prim::F32),
        CType::Float { bits: 80 } if crate::frontend::toolchain::uses_f64_long_double_abi() => {
            Type::Prim(Prim::F64)
        }
        CType::Float { bits: 80 } => Type::LongDouble,
        CType::Float { bits: 128 } => Type::Prim(Prim::F128),
        CType::Float { .. } => Type::Prim(Prim::F64),
        CType::Ptr(inner) if matches!(&**inner, CType::Void) => Type::Ptr {
            mutable: true,
            inner: Box::new(Type::CLib(CLibType::VOID)),
        },
        CType::Ptr(inner) => ptr(inner),
        CType::FuncPtr { ret, params } => Type::FnPtr {
            abi: Abi::C,
            params: params
                .iter()
                .map(|ty| c_type_to_type(ty, va_list_boxed))
                .collect(),
            ret: Box::new(c_type_to_type(ret, va_list_boxed)),
        },
        CType::Array(inner, Some(len)) => Type::Array {
            elem: Box::new(c_type_to_type(inner, va_list_boxed)),
            len: *len,
        },
        CType::Array(inner, None) => ptr(inner),
        CType::Record(name) if name.starts_with("__slate_bitfields::") => {
            Type::Custom(name.clone())
        }
        CType::Record(name) => clib_record_type(name)
            .map(Type::CLib)
            .unwrap_or_else(|| Type::Custom(rust_record_name(name))),
        CType::Enum(name) => Type::Custom(sanitize_ident(name).into_string()),
    }
}

fn c_record_field_type(ty: &crate::frontend::c_ast::CType, va_list_boxed: bool) -> Type {
    match ty {
        crate::frontend::c_ast::CType::Array(inner, None) => Type::Array {
            elem: Box::new(c_type_to_type(inner, va_list_boxed)),
            len: 0,
        },
        _ => c_type_to_type(ty, va_list_boxed),
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
    ty: &crate::frontend::c_ast::CType,
    records: &BTreeMap<String, crate::frontend::c_ast::Record>,
) -> Option<CLayout> {
    use crate::frontend::c_ast::CType;
    match ty {
        CType::Void => Some(CLayout { size: 0, align: 1 }),
        CType::Bool => Some(CLayout { size: 1, align: 1 }),
        CType::Int { bits, .. } => scalar_layout(*bits),
        CType::Float { bits: 80 } => {
            Some(if crate::frontend::toolchain::uses_f64_long_double_abi() {
                CLayout { size: 8, align: 8 }
            } else {
                CLayout {
                    size: 16,
                    align: 16,
                }
            })
        }
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

fn record_layout(
    name: &str,
    records: &BTreeMap<String, crate::frontend::c_ast::Record>,
) -> Option<CLayout> {
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
    record: &crate::frontend::c_ast::Record,
    records: &BTreeMap<String, crate::frontend::c_ast::Record>,
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
    records: &BTreeMap<String, crate::frontend::c_ast::Record>,
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

fn ctype_uses_long_double(ty: &crate::frontend::c_ast::CType) -> bool {
    use crate::frontend::c_ast::CType;
    match ty {
        CType::Float { bits: 80 } => !crate::frontend::toolchain::uses_f64_long_double_abi(),
        CType::Ptr(inner) | CType::Array(inner, _) => ctype_uses_long_double(inner),
        CType::FuncPtr { ret, params } => {
            ctype_uses_long_double(ret) || params.iter().any(ctype_uses_long_double)
        }
        CType::Enum(_) => false,
        _ => false,
    }
}

fn cir_positive_zero(text: &str) -> bool {
    let text = text.trim();
    !text.starts_with('-') && text.parse::<f64>().is_ok_and(|value| value == 0.0)
}
