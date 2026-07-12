//! lower: combine the CIR Op-tree with the C AST oracle into Rust output.

use crate::c_ast::{RecordKind, Unit};
use crate::cir::ir::{Attr, Block, Module, Op, Region};
use crate::ctx::Ctx;
use crate::rust_ast::{
    AtomicOrdering, AtomicRmwOp, AtomicType, Attr as RustAttr, BinOp, CLibType, CrateAttr, Derive,
    EnumConst, Expr, ExprMatchArm, ExternDecl, ExternFnDecl, Feature, FnDef, FnParam, GenericParam,
    Ident, ImplBlock, ImplItem, IndentStmt, Item, Label, Lint, MatchArm, Method, Path, Pattern,
    Prim, Program, RecordDef, Repr, RustValue, StdTrait, Stmt, StructDef, StructFields, TraitBound,
    Type, UnaryOp, Visibility,
};
use std::collections::{BTreeMap, BTreeSet};

/// How a translation unit fits into a multi-file project: which symbols other
/// units define, which sibling modules the crate root must declare, and whether
/// definitions are emitted `pub` so siblings can import them.
#[derive(Default, Clone)]
pub struct ProjectInfo {
    /// function symbol → module (rust file stem) that defines it, for functions
    /// defined anywhere in the project. A body-less decl whose symbol is in here
    /// becomes `use crate::<module>::<sym>;` instead of an `extern "C"` decl.
    pub cross_module: BTreeMap<String, String>,
    /// global symbol → module that defines it. An `extern` global whose symbol is
    /// in here becomes `use crate::<module>::<sym>;` instead of an extern static.
    pub cross_module_globals: BTreeMap<String, String>,
    /// modules the crate root declares with `mod <name>;` (empty for non-root).
    pub child_modules: Vec<String>,
    /// emit function and global definitions as `pub` so other modules can import them.
    pub emit_pub: bool,
}

/// CIR encodes external linkage as `linkage = 0`; a C `static` is nonzero.
fn linkage_is_external(op: &Op) -> bool {
    attr_int(op, "linkage").unwrap_or(0) == 0
}

pub fn defined_functions(module: &Module) -> Vec<String> {
    let Some(module_op) = module.ops.iter().find(|op| op.name == "builtin.module") else {
        return Vec::new();
    };
    region_ops(module_op)
        .iter()
        .filter(|op| op.name == "cir.func" && !region_ops(op).is_empty() && linkage_is_external(op))
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
            op.name == "cir.global"
                && attr_str(op, "initial_value").is_some()
                && linkage_is_external(op)
        })
        .filter_map(|op| attr_str(op, "sym_name").map(|name| sanitize_ident(name).into_string()))
        .collect()
}

/// Lower a parsed CIR module (with the C AST as an oracle) to a Rust program.
pub fn lower(cir: &Module, c: &Unit, ctx: &mut Ctx) -> Program {
    lower_with_project(cir, c, ctx, &ProjectInfo::default())
}

/// Lower a translation unit that is part of a multi-file project.
pub fn lower_with_project(cir: &Module, c: &Unit, ctx: &mut Ctx, project: &ProjectInfo) -> Program {
    let mut lowerer = Lowerer {
        ctx,
        aliases: cir.aliases.clone(),
        records: c
            .records
            .iter()
            .map(|record| (sanitize_ident(&record.name).into_string(), record.clone()))
            .collect(),
        globals: BTreeMap::new(),
        extern_globals: BTreeMap::new(),
        strings: BTreeMap::new(),
        const_arrays: BTreeMap::new(),
        const_aggregates: BTreeMap::new(),
        const_zero_globals: BTreeSet::new(),
        externs: BTreeMap::new(),
        extern_returns: BTreeMap::new(),
        uses_long_double: std::cell::Cell::new(false),
        uses_complex: std::cell::Cell::new(false),
        uses_c_variadic: std::cell::Cell::new(false),
        variadic_defs: BTreeSet::new(),
        project: project.clone(),
        cross_uses: Vec::new(),
    };
    lowerer.lower_module(cir, c)
}

struct Lowerer<'a> {
    ctx: &'a mut Ctx,
    aliases: BTreeMap<String, String>,
    records: BTreeMap<String, crate::c_ast::Record>,
    globals: BTreeMap<String, GlobalVar>,
    extern_globals: BTreeMap<String, Type>,
    strings: BTreeMap<String, Vec<u8>>,
    /// numeric aggregate const globals (e.g. `int a[5]={..}`) → element literals,
    /// keyed by raw sym_name; consumed when a `cir.copy` initializes a local.
    const_arrays: BTreeMap<String, Vec<Expr>>,
    /// nested aggregate const globals (structs, unions, arrays of aggregates) →
    /// their raw `#cir.const_record`/`#cir.const_array` initializer, keyed by raw
    /// sym_name; rendered recursively against the destination type on `cir.copy`.
    const_aggregates: BTreeMap<String, String>,
    const_zero_globals: BTreeSet<String>,
    /// external (body-less) functions → rust types of their fixed params; the
    /// call site uses this to `as`-cast args and wrap the call in `unsafe`.
    externs: BTreeMap<String, Vec<Type>>,
    extern_returns: BTreeMap<String, Option<String>>,
    uses_long_double: std::cell::Cell<bool>,
    uses_complex: std::cell::Cell<bool>,
    uses_c_variadic: std::cell::Cell<bool>,
    variadic_defs: BTreeSet<String>,
    project: ProjectInfo,
    /// `use crate::<mod>::<sym>;` items for body-less decls resolved to a sibling.
    cross_uses: Vec<Item>,
}

struct FunctionLowerer<'a, 'b> {
    parent: &'a mut Lowerer<'b>,
    values: BTreeMap<String, Val>,
    slots: BTreeMap<String, String>,
    slot_types: BTreeMap<String, String>,
    member_ptrs: BTreeMap<String, MemberPtr>,
    element_ptrs: BTreeMap<String, ElementPtr>,
    temp_counter: usize,
    indent: usize,
    body: Vec<IndentStmt>,
    is_main: bool,
    loop_stack: Vec<LoopFrame>,
    label_counter: usize,
    /// Set for functions with unstructured control flow (goto/multi-block); drives
    /// the state-machine dispatch loop. `None` for structured functions.
    dispatch: Option<DispatchCtx>,
    /// Alloca results hoisted above the dispatch loop so locals outlive block arms.
    hoisted: BTreeSet<String>,
    /// `va_list` SSA values (the `ap` alloca and its array-decay casts) → slot name.
    va_places: BTreeMap<String, String>,
    va_args_param: Option<String>,
}

/// Maps CIR jump targets to dispatch-loop states for a `goto`-bearing function.
struct DispatchCtx {
    loop_label: Label,
    state_var: String,
    /// C label name (`cir.label`/`cir.goto`) -> block index.
    label_to_state: BTreeMap<String, usize>,
    /// Block label (`^bbN`, entry as `bb0`) -> block index, for `cir.br`.
    block_to_state: BTreeMap<String, usize>,
}

// How `cir.break`/`cir.continue` lower for the enclosing loop. A C `for` loop's
// step must still run on `continue`, but Rust's `continue` skips the loop tail,
// so the body is wrapped in a labeled block and `continue` becomes
// `break 'continue`. Rust then forbids an unlabeled `break` from diverging out
// through that block, so the loop is labeled too and `break` targets it.
struct LoopFrame {
    break_label: Option<Label>,
    continue_label: Option<Label>,
    is_loop: bool,
}

struct SwitchCase<'a> {
    values: Vec<i64>,
    is_default: bool,
    region: &'a Region,
}

#[derive(Debug, Clone)]
struct MemberPtr {
    base: Expr,
    field: String,
    unsafe_access: bool,
}

#[derive(Debug, Clone)]
struct ElementPtr {
    base: Expr,
    index: Expr,
    unsafe_access: bool,
}

#[derive(Debug, Clone)]
struct GlobalVar {
    name: String,
    ty: Type,
    init: Expr,
    external: bool,
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
            Val::Global(_) => Expr::Raw(self.render(strings)),
        }
    }

    fn render(&self, strings: &BTreeMap<String, Vec<u8>>) -> String {
        match self {
            Val::Expr(e) => e.render(),
            Val::Global(name) => match strings.get(name) {
                Some(bytes) => {
                    // *mut so it fits *mut char slots; weakens to *const for printf/libc.
                    format!("{}.as_ptr() as *mut libc::c_char", rust_byte_string(bytes))
                }
                None => name.clone(),
            },
        }
    }
}

impl<'a> Lowerer<'a> {
    fn lower_module(&mut self, module: &Module, c: &Unit) -> Program {
        let mut items = vec![Item::CrateAttrs(vec![CrateAttr::Allow(vec![
            Lint::DeadCode,
            Lint::Unused,
            Lint::NonSnakeCase,
            Lint::NonUpperCaseGlobals,
            Lint::ArithmeticOverflow,
        ])])];

        for enm in &c.enums {
            if let Some(item) = self.lower_enum(enm) {
                items.push(item);
            }
        }
        for record in &c.records {
            if let Some(item) = self.lower_record(record) {
                items.push(item);
            }
        }
        items.extend(self.standard_record_defs());

        let Some(module_op) = module.ops.iter().find(|op| op.name == "builtin.module") else {
            self.ctx
                .diagnostics
                .error("lower: no builtin.module op", None);
            return Program { items };
        };

        let ops = region_ops(module_op);
        for op in &ops {
            if op.name == "cir.global" {
                self.collect_global(op);
            }
        }
        for global in self.globals.values() {
            let global_vis = if self.project.emit_pub && global.external {
                Visibility::Pub
            } else {
                Visibility::Private
            };
            items.push(Item::Static {
                vis: global_vis,
                mutable: true,
                name: global.name.clone(),
                ty: global.ty.clone(),
                init: global.init.clone(),
            });
        }

        let module_uses_long_double = ops.iter().any(|op| op_mentions_long_double(op));
        let mut extern_decls = Vec::new();
        for (name, ty) in &self.extern_globals {
            // an extern global defined in a sibling TU becomes a module import.
            if let Some(module) = self.project.cross_module_globals.get(name) {
                self.cross_uses.push(Item::Use {
                    path: Path::new([
                        Ident::from("crate"),
                        Ident::from(module.as_str()),
                        Ident::from(name.as_str()),
                    ]),
                });
                continue;
            }
            extern_decls.push(ExternDecl::Static {
                mutable: true,
                name: name.clone(),
                ty: ty.clone(),
            });
        }
        for op in &ops {
            if op.name != "cir.func" || !region_ops(op).is_empty() {
                continue;
            }
            let Some(name) = attr_str(op, "sym_name") else {
                continue;
            };
            // complex runtime routines are declared in the prelude.
            if is_complex_runtime_call(name) {
                continue;
            }
            // a prototype whose definition lives in a sibling TU becomes a module
            // import; the call then flows through the normal (non-extern) path.
            if let Some(module) = self.project.cross_module.get(name) {
                self.cross_uses.push(Item::Use {
                    path: Path::new([
                        Ident::from("crate"),
                        Ident::from(module.as_str()),
                        Ident::from(name),
                    ]),
                });
                continue;
            }
            let function_type = attr_str(op, "function_type").unwrap_or("");
            let (decl, params, ret) = self.extern_fn_signature(name, function_type);
            self.externs.insert(name.to_string(), params);
            self.extern_returns.insert(name.to_string(), ret.clone());
            if name == "strtold" && ret.as_deref() == Some(LONG_DOUBLE_TY) {
                extern_decls.push(ExternDecl::Fn(ExternFnDecl {
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
                                inner: Box::new(Type::LongDouble),
                            },
                        },
                    ],
                    variadic: false,
                    ret: None,
                }));
            } else {
                extern_decls.push(ExternDecl::Fn(decl));
            }
            if name == "printf" && module_uses_long_double {
                extern_decls.push(ExternDecl::Fn(ExternFnDecl {
                    name: "__slate_printf_ld_i32".into(),
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
                                mutable: false,
                                inner: Box::new(Type::LongDouble),
                            },
                        },
                        FnParam {
                            name: "_2".into(),
                            mutable: false,
                            ty: Type::Prim(Prim::I32),
                        },
                    ],
                    variadic: false,
                    ret: Some(Type::Prim(Prim::I32)),
                }));
            }
        }
        if !extern_decls.is_empty() {
            items.push(Item::ExternBlock {
                abi: "C".into(),
                decls: extern_decls,
            });
        }

        // collected before lowering so call sites wrap in `unsafe` regardless of order.
        for op in &ops {
            if op.name == "cir.func"
                && !region_ops(op).is_empty()
                && attr_str(op, "sym_name").is_some_and(|name| name != "main")
                && function_type_is_variadic(attr_str(op, "function_type").unwrap_or(""))
            {
                self.variadic_defs
                    .insert(attr_str(op, "sym_name").unwrap().to_string());
            }
        }

        for op in ops {
            if op.name != "cir.func" || region_ops(op).is_empty() {
                continue;
            }
            match self.lower_func(op) {
                Some(item) => items.push(item),
                None => self.ctx.diagnostics.warn(
                    format!("lower: skipped function {:?}", attr_str(op, "sym_name")),
                    op.loc.clone(),
                ),
            }
        }

        if self.uses_long_double.get() {
            items.splice(1..1, long_double_prelude());
        }
        if self.uses_complex.get() {
            items.splice(1..1, complex_prelude());
        }

        // module wiring goes right after the crate-level `#![allow(..)]` attr.
        let mut wiring: Vec<Item> = self
            .project
            .child_modules
            .iter()
            .map(|name| Item::Mod {
                name: Ident::from(name.as_str()),
            })
            .collect();
        wiring.append(&mut self.cross_uses);
        for (offset, item) in wiring.into_iter().enumerate() {
            items.insert(1 + offset, item);
        }

        // grouped with the crate-level `#![allow(..)]` so both stay at the top.
        if self.uses_c_variadic.get()
            && let Some(Item::CrateAttrs(attrs)) = items.first_mut()
        {
            attrs.insert(0, CrateAttr::Feature(Feature::CVariadic));
        }

        Program { items }
    }

    fn collect_global(&mut self, op: &Op) {
        let Some(name) = attr_str(op, "sym_name") else {
            return;
        };
        let rust_name = sanitize_ident(name).into_string();
        let ty = attr_str(op, "sym_type").map(|ty| self.rust_type(ty));
        let is_c_global = !name.starts_with("__") && !name.starts_with(".str");
        let Some(raw) = attr_str(op, "initial_value") else {
            let Some(ty) = ty else {
                return;
            };
            self.extern_globals.insert(rust_name, ty);
            return;
        };
        if let Some(mut bytes) = parse_cir_const_array(raw) {
            if is_c_global && let Some(ty) = ty {
                let elems: Vec<Expr> = bytes
                    .iter()
                    .map(|b| Expr::Value(RustValue::I64(i64::from(*b))))
                    .collect();
                self.globals.insert(
                    rust_name.clone(),
                    GlobalVar {
                        name: rust_name,
                        ty,
                        init: render_array_literal_expr(
                            &elems,
                            bytes.len(),
                            Expr::Value(RustValue::I64(0)),
                        ),
                        external: linkage_is_external(op),
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
                            external: linkage_is_external(op),
                        },
                    );
                }
            } else {
                self.const_arrays.insert(name.to_string(), elems);
            }
        } else if is_cir_aggregate_init(raw) {
            if is_c_global && let Some(ty) = ty {
                if let Some(init) = self.render_const_value_expr(&ty.render(), raw) {
                    self.globals.insert(
                        rust_name.clone(),
                        GlobalVar {
                            name: rust_name,
                            ty,
                            init,
                            external: linkage_is_external(op),
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
                            elem: Box::new(
                                self.default_value_expr(&self.rust_type(&elem).render()),
                            ),
                            len: len as usize,
                        },
                        external: linkage_is_external(op),
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
                        init: self.default_value_expr(&ty.render()),
                        ty,
                        external: linkage_is_external(op),
                    },
                );
            } else {
                self.const_zero_globals.insert(name.to_string());
            }
        } else if let Some(init) = parse_cir_scalar_expr(raw) {
            let ty = ty.unwrap_or_else(|| self.rust_type("!s32i"));
            let external = linkage_is_external(op);
            self.globals.insert(
                rust_name.clone(),
                GlobalVar {
                    name: rust_name,
                    ty,
                    init,
                    external,
                },
            );
        }
    }

    fn lower_enum(&mut self, enm: &crate::c_ast::Enum) -> Option<Item> {
        if enm.variants.is_empty() {
            return None;
        }
        let consts = enm
            .variants
            .iter()
            .map(|variant| EnumConst {
                name: sanitize_ident(&variant.name).into_string(),
                value: variant.value,
            })
            .collect();
        Some(Item::Enum(consts))
    }

    fn lower_record(&mut self, record: &crate::c_ast::Record) -> Option<Item> {
        if record.fields.is_empty() {
            return None;
        }
        let fields = record
            .fields
            .iter()
            .map(|field| {
                (
                    sanitize_ident(&field.name),
                    self.record_field_type(&field.ty),
                )
            })
            .collect();
        Some(Item::Record(RecordDef {
            is_union: record.kind == RecordKind::Union,
            allow_non_camel_case: false,
            name: sanitize_ident(&record.name).into_string(),
            fields,
        }))
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

        let (vis, unsafe_extern_c, ret, prelude) = if is_main {
            params.clear();
            (
                Visibility::Private,
                false,
                None,
                self.main_arg_bindings(entry),
            )
        } else {
            let vis = if self.project.emit_pub && linkage_is_external(op) {
                Visibility::Pub
            } else {
                Visibility::Private
            };
            let unsafe_extern_c = if is_variadic {
                self.uses_c_variadic.set(true);
                self.variadic_defs.insert(name.to_string());
                true
            } else {
                false
            };
            let ret = Some(self.rust_type(ret_ty.as_deref().unwrap_or("()")));
            (vis, unsafe_extern_c, ret, Vec::<Stmt>::new())
        };

        let mut f = FunctionLowerer {
            parent: self,
            values: BTreeMap::new(),
            slots: BTreeMap::new(),
            slot_types: BTreeMap::new(),
            member_ptrs: BTreeMap::new(),
            element_ptrs: BTreeMap::new(),
            temp_counter: 0,
            indent: 1,
            body: Vec::new(),
            is_main,
            loop_stack: Vec::new(),
            label_counter: 0,
            dispatch: None,
            hoisted: BTreeSet::new(),
            va_places: BTreeMap::new(),
            va_args_param,
        };

        for stmt in prelude {
            f.push_stmt(stmt);
        }
        for (arg, _) in &entry.args {
            f.values
                .insert(arg.clone(), Val::Expr(Expr::Var(arg.clone().into())));
        }
        let body = op.regions.first().unwrap();
        if body.blocks.len() > 1 {
            f.lower_dispatch(body);
        } else {
            f.lower_block(entry);
        }
        Some(Item::Fn(FnDef {
            vis,
            unsafe_extern_c,
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
        c_type_to_type(ty)
    }

    fn default_value_expr(&self, ty: &str) -> Expr {
        if let Some(record) = self.records.get(ty) {
            match record.kind {
                RecordKind::Struct => {
                    let fields = record
                        .fields
                        .iter()
                        .map(|field| {
                            (
                                sanitize_ident(&field.name).into_string(),
                                self.default_value_expr(&c_type_to_rust(&field.ty)),
                            )
                        })
                        .collect();
                    return Expr::StructLit {
                        name: sanitize_ident(&record.name).into_string(),
                        fields,
                    };
                }
                RecordKind::Union => {
                    if let Some(field) = record.fields.first() {
                        return Expr::StructLit {
                            name: sanitize_ident(&record.name).into_string(),
                            fields: vec![(
                                sanitize_ident(&field.name).into_string(),
                                self.default_value_expr(&c_type_to_rust(&field.ty)),
                            )],
                        };
                    }
                }
            }
        }
        if is_long_double(ty) || ty == LONG_DOUBLE_TY {
            return Expr::Call {
                func: Box::new(Expr::Var(LONG_DOUBLE_TY.into())),
                args: vec![Expr::Value(RustValue::Float(0.0))],
            };
        }
        if let Some(value) = standard_record_default_expr(ty) {
            return value;
        }
        if let Some(inner) = ty
            .strip_prefix(COMPLEX_TY)
            .and_then(|s| s.strip_suffix('>'))
        {
            let d = default_value_expr(inner);
            return Expr::StructLit {
                name: "Complex".into(),
                fields: vec![("re".into(), d.clone()), ("im".into(), d)],
            };
        }
        if let Some((inner, len)) = parse_cir_array_type(ty) {
            return Expr::ArrayRepeat {
                elem: Box::new(self.default_value_expr(&inner)),
                len: len as usize,
            };
        }
        if let Some((inner, len)) = parse_rust_array_type(ty) {
            return Expr::ArrayRepeat {
                elem: Box::new(self.default_value_expr(inner)),
                len: len as usize,
            };
        }
        default_value_expr(ty)
    }

    fn render_const_value_expr(&self, rust_ty: &str, raw: &str) -> Option<Expr> {
        let raw = raw.trim();
        if let Some((re, im)) = parse_cir_const_complex(raw) {
            Some(Expr::StructLit {
                name: "Complex".into(),
                fields: vec![
                    ("re".into(), fp_literal_expr(re)),
                    ("im".into(), fp_literal_expr(im)),
                ],
            })
        } else if raw.starts_with("#cir.const_record<") {
            let record = self.records.get(rust_ty)?;
            let open = raw.find('{')?;
            let close = raw.rfind('}')?;
            let elems = split_top_level(&raw[open + 1..close], ',');
            match record.kind {
                RecordKind::Struct => {
                    let fields = record
                        .fields
                        .iter()
                        .enumerate()
                        .map(|(i, field)| {
                            let field_ty = c_type_to_rust(&field.ty);
                            let value = elems
                                .get(i)
                                .and_then(|e| self.render_const_value_expr(&field_ty, e.trim()))
                                .unwrap_or_else(|| self.default_value_expr(&field_ty));
                            (sanitize_ident(&field.name).into_string(), value)
                        })
                        .collect();
                    Some(Expr::StructLit {
                        name: sanitize_ident(&record.name).into_string(),
                        fields,
                    })
                }
                RecordKind::Union => {
                    let field = record.fields.first()?;
                    let field_ty = c_type_to_rust(&field.ty);
                    let value = elems
                        .first()
                        .and_then(|e| self.render_const_value_expr(&field_ty, e.trim()))
                        .unwrap_or_else(|| self.default_value_expr(&field_ty));
                    Some(Expr::StructLit {
                        name: sanitize_ident(&record.name).into_string(),
                        fields: vec![(sanitize_ident(&field.name).into_string(), value)],
                    })
                }
            }
        } else if raw.starts_with("#cir.const_array<[") {
            let (elem_ty, len) = parse_rust_array_type(rust_ty)?;
            let open = raw.find('[')?;
            let close = raw.rfind(']')?;
            let mut out: Vec<Expr> = split_top_level(&raw[open + 1..close], ',')
                .into_iter()
                .map(|e| e.trim().to_string())
                .filter(|e| !e.is_empty())
                .take(len as usize)
                .map(|e| {
                    self.render_const_value_expr(elem_ty, &e)
                        .unwrap_or_else(|| self.default_value_expr(elem_ty))
                })
                .collect();
            out.resize(len as usize, self.default_value_expr(elem_ty));
            Some(Expr::ArrayLit(out))
        } else if raw.starts_with("#cir.zero") {
            Some(self.default_value_expr(rust_ty))
        } else {
            parse_cir_scalar_expr(raw)
        }
    }
}

fn c_type_to_rust(ty: &crate::c_ast::CType) -> String {
    match ty {
        crate::c_ast::CType::Void => "()".into(),
        crate::c_ast::CType::Bool => "bool".into(),
        crate::c_ast::CType::Int {
            signed: true,
            bits: 8,
        } => "i8".into(),
        crate::c_ast::CType::Int {
            signed: false,
            bits: 8,
        } => "u8".into(),
        crate::c_ast::CType::Int {
            signed: true,
            bits: 16,
        } => "i16".into(),
        crate::c_ast::CType::Int {
            signed: false,
            bits: 16,
        } => "u16".into(),
        crate::c_ast::CType::Int {
            signed: true,
            bits: 32,
        } => "i32".into(),
        crate::c_ast::CType::Int {
            signed: false,
            bits: 32,
        } => "u32".into(),
        crate::c_ast::CType::Int {
            signed: true,
            bits: 64,
        } => "i64".into(),
        crate::c_ast::CType::Int {
            signed: false,
            bits: 64,
        } => "u64".into(),
        crate::c_ast::CType::Int { .. } => "i32".into(),
        crate::c_ast::CType::Float { bits: 32 } => "f32".into(),
        crate::c_ast::CType::Float { bits: 64 } => "f64".into(),
        crate::c_ast::CType::Float { bits: 80 } => LONG_DOUBLE_TY.into(),
        crate::c_ast::CType::Float { .. } => "f64".into(),
        crate::c_ast::CType::Ptr(inner) => format!("*mut {}", c_type_to_rust(inner)),
        crate::c_ast::CType::FuncPtr { ret, params } => {
            let params = params
                .iter()
                .map(c_type_to_rust)
                .collect::<Vec<_>>()
                .join(", ");
            format!("Option<fn({params}) -> {}>", c_type_to_rust(ret))
        }
        crate::c_ast::CType::Array(inner, Some(len)) => {
            format!("[{}; {len}]", c_type_to_rust(inner))
        }
        crate::c_ast::CType::Array(inner, None) => format!("*mut {}", c_type_to_rust(inner)),
        crate::c_ast::CType::Record(name) => sanitize_ident(name).into_string(),
    }
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
            _ => Prim::I32,
        }),
        CType::Float { bits: 32 } => Type::Prim(Prim::F32),
        CType::Float { bits: 80 } => Type::LongDouble,
        CType::Float { .. } => Type::Prim(Prim::F64),
        CType::Ptr(inner) => ptr(inner),
        CType::FuncPtr { ret, params } => Type::FnPtr {
            params: params.iter().map(c_type_to_type).collect(),
            ret: Box::new(c_type_to_type(ret)),
        },
        CType::Array(inner, Some(len)) => Type::Array {
            elem: Box::new(c_type_to_type(inner)),
            len: *len,
        },
        CType::Array(inner, None) => ptr(inner),
        CType::Record(name) => Type::Custom(sanitize_ident(name).into_string()),
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
        _ => false,
    }
}

impl<'a, 'b> FunctionLowerer<'a, 'b> {
    fn lower_block(&mut self, block: &Block) {
        for op in &block.ops {
            self.lower_op(op);
        }
    }

    fn lower_region_ops(&mut self, region: &Region) {
        for block in &region.blocks {
            self.lower_block(block);
        }
    }

    fn lower_op(&mut self, op: &Op) {
        match op.name.as_str() {
            "cir.alloca" => self.lower_alloca(op),
            "cir.store" => self.lower_store(op),
            "cir.copy" => self.lower_copy(op),
            "cir.load" => self.lower_load(op),
            "cir.const" => self.lower_const(op),
            "cir.add.overflow" => self.lower_overflow_arith(op, "overflowing_add"),
            "cir.sub.overflow" => self.lower_overflow_arith(op, "overflowing_sub"),
            "cir.mul.overflow" => self.lower_overflow_arith(op, "overflowing_mul"),
            "cir.div.overflow" => self.lower_overflow_arith(op, "overflowing_div"),
            "cir.rem.overflow" => self.lower_overflow_arith(op, "overflowing_rem"),
            "cir.add" => self.lower_int_arith(op, BinOp::Add),
            "cir.sub" => self.lower_int_arith(op, BinOp::Sub),
            "cir.mul" => self.lower_int_arith(op, BinOp::Mul),
            "cir.div" => self.lower_int_arith(op, BinOp::Div),
            "cir.rem" => self.lower_int_arith(op, BinOp::Rem),
            "cir.and" => self.lower_int_arith(op, BinOp::BitAnd),
            "cir.or" => self.lower_int_arith(op, BinOp::BitOr),
            "cir.xor" => self.lower_int_arith(op, BinOp::BitXor),
            "cir.shift" => self.lower_shift(op),
            "cir.not" => self.lower_not(op),
            "cir.minus" | "cir.fneg" => self.lower_neg(op),
            "cir.abs" => self.lower_abs(op),
            "cir.ceil" => self.lower_unary_method(op, "ceil"),
            "cir.copysign" => self.lower_binary_method(op, "copysign"),
            "cir.fabs" => self.lower_unary_method(op, "abs"),
            "cir.floor" => self.lower_unary_method(op, "floor"),
            "cir.fmaxnum" => self.lower_binary_method(op, "max"),
            "cir.fminnum" => self.lower_binary_method(op, "min"),
            "cir.is_fp_class" => self.lower_is_fp_class(op),
            "cir.modf" => self.lower_modf(op),
            "cir.nearbyint" => self.lower_unary_method(op, "round_ties_even"),
            "cir.rint" => self.lower_unary_method(op, "round_ties_even"),
            "cir.round" => self.lower_unary_method(op, "round"),
            "cir.signbit" => self.lower_signbit(op),
            "cir.trunc" => self.lower_unary_method(op, "trunc"),
            "cir.fadd" => self.lower_binary(op, BinOp::Add),
            "cir.fsub" => self.lower_binary(op, BinOp::Sub),
            "cir.fmul" => self.lower_binary(op, BinOp::Mul),
            "cir.fdiv" => self.lower_binary(op, BinOp::Div),
            "cir.complex.add" => self.lower_binary(op, BinOp::Add),
            "cir.complex.sub" => self.lower_binary(op, BinOp::Sub),
            "cir.complex.create" => self.lower_complex_create(op),
            "cir.complex.real" => self.lower_complex_part(op, "re"),
            "cir.complex.imag" => self.lower_complex_part(op, "im"),
            "cir.inc" => self.lower_step(op, BinOp::Add),
            "cir.dec" => self.lower_step(op, BinOp::Sub),
            "cir.cmp" => self.lower_cmp(op),
            "cir.select" => self.lower_select(op),
            "cir.ternary" => self.lower_ternary(op),
            "cir.get_global" => self.lower_get_global(op),
            "cir.get_member" => self.lower_get_member(op),
            "cir.get_bitfield" => self.lower_get_bitfield(op),
            "cir.set_bitfield" => self.lower_set_bitfield(op),
            "cir.get_element" => self.lower_get_element(op),
            "cir.cast" => self.lower_cast(op),
            "cir.ptr_stride" => self.lower_ptr_stride(op),
            "cir.ptr_diff" => self.lower_ptr_diff(op),
            "cir.call" => self.lower_call(op),
            "cir.va_start" => self.lower_va_start(op),
            "cir.va_arg" => self.lower_va_arg(op),
            // `VaList` drops on scope exit.
            "cir.va_end" => {}
            "cir.atomic.fetch" => self.lower_atomic_fetch(op),
            "cir.atomic.xchg" => self.lower_atomic_xchg(op),
            "cir.atomic.cmpxchg" => self.lower_atomic_cmpxchg(op),
            "cir.atomic.fence" => self.lower_atomic_fence(op),
            "cir.return" => self.lower_return(op),
            "cir.scope" => self.lower_scope(op),
            "cir.if" => self.lower_if(op),
            "cir.switch" => self.lower_switch(op),
            "cir.for" => self.lower_for(op),
            "cir.while" => self.lower_while(op),
            "cir.do" => self.lower_do(op),
            "cir.break" => self.lower_break(),
            "cir.continue" => self.lower_continue(),
            "cir.goto" => self.lower_goto(op),
            "cir.br" => self.lower_br(op),
            "cir.label" => {}
            "cir.yield" | "cir.condition" => {}
            other => {
                self.parent
                    .ctx
                    .diagnostics
                    .warn(format!("lower: unsupported CIR op {other}"), op.loc.clone());
                self.emit_todo(other);
            }
        }
    }

    fn lower_alloca(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        // hoisted allocas were already declared above the dispatch loop.
        if self.hoisted.contains(result) && self.dispatch.is_some() {
            return;
        }
        let name = sanitize_ident(attr_str(op, "name").unwrap_or(result)).into_string();
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
        let ty_str = ty.render();
        self.slots.insert(result.clone(), name.clone());
        self.slot_types.insert(result.clone(), ty_str.clone());
        self.push_stmt(Stmt::Let {
            name,
            mutable: true,
            ty: Some(ty),
            init: Some(self.default_value_expr(&ty_str)),
        });
    }

    fn lower_store(&mut self, op: &Op) {
        if op.operands.len() < 2 {
            return;
        }
        let operand_types = op_operand_types(op.ty.as_deref().unwrap_or(""));
        let value_ty = operand_types.first().copied();
        let value = if value_ty.is_some_and(is_cir_function_pointer_type) {
            self.function_pointer_operand_expr(&op.operands[0])
        } else if value_ty.is_some_and(|ty| ty.starts_with("!cir.ptr<")) {
            self.pointer_operand_expr(&op.operands[0])
        } else {
            self.operand_expr(&op.operands[0])
        };
        let ptr = &op.operands[1];
        if !attr_bool(op, "is_volatile") && self.try_atomic_store(op, ptr, value_ty, value.clone())
        {
            return;
        }
        if attr_bool(op, "is_volatile") {
            self.push_stmt(Stmt::Expr(Self::unsafe_expr(Expr::Call {
                func: Box::new(Self::raw_expr("std::ptr::write_volatile")),
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
            .and_then(|ty| parse_rust_array_type(ty))
            .map(|(_, len)| len as usize);
        match self.values.get(src) {
            Some(Val::Global(name)) => {
                if let Some(bytes) = self.parent.strings.get(name) {
                    let elems: Vec<Expr> = bytes
                        .iter()
                        .map(|b| Expr::Value(RustValue::I64(i64::from(*b))))
                        .collect();
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
            _ => self.slots.contains_key(src).then(|| self.operand_expr(src)),
        }
    }

    fn render_const_value_expr(&self, rust_ty: &str, raw: &str) -> Option<Expr> {
        self.parent.render_const_value_expr(rust_ty, raw)
    }

    fn lower_load(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        let Some(ptr) = op.operands.first() else {
            return;
        };
        let value = if attr_bool(op, "is_volatile") {
            Self::unsafe_expr(Expr::Call {
                func: Box::new(Self::raw_expr("std::ptr::read_volatile")),
                args: vec![self.load_address_expr(ptr)],
            })
        } else if let Some(atomic) = self.atomic_load_expr(op, ptr) {
            atomic
        } else if let Some(global) = self.global_name(ptr) {
            Self::unsafe_expr(Expr::Var(global.into()))
        } else if let Some(member) = self.member_ptrs.get(ptr) {
            Self::unsafe_expr(Expr::Field {
                base: Box::new(member.base.clone()),
                field: member.field.clone(),
            })
        } else if let Some(element) = self.element_ptrs.get(ptr) {
            let place = self.element_place_expr(element);
            if element.unsafe_access {
                Self::unsafe_expr(place)
            } else {
                place
            }
        } else if let Some(slot) = self.slots.get(ptr) {
            Expr::Var(slot.clone().into())
        } else {
            Self::unsafe_expr(Expr::Unary {
                op: UnaryOp::Deref,
                expr: Box::new(self.operand_expr(ptr)),
            })
        };
        self.materialize_expr(result, value, op_result_type(op));
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
            addr_of(Expr::Field {
                base: Box::new(member.base.clone()),
                field: member.field.clone(),
            })
        } else if let Some(element) = self.element_ptrs.get(ptr) {
            addr_of(self.element_place_expr(element))
        } else if let Some(slot) = self.slots.get(ptr) {
            addr_of(Expr::Var(slot.clone().into()))
        } else if let Some(global) = self.global_name(ptr) {
            addr_of(Expr::Var(global.into()))
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

    fn lower_const(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        let raw = attr_str(op, "value").unwrap_or("");
        // MLIR may print a const as an attribute alias (e.g. `#false`); expand it.
        let raw = self.parent.aliases.get(raw).map_or(raw, String::as_str);
        let result_ty = op_result_type(op);
        if let Some((re, im)) = parse_cir_const_complex(raw) {
            self.materialize_expr(
                result,
                Expr::StructLit {
                    name: "Complex".into(),
                    fields: vec![
                        ("re".into(), fp_literal_expr(re)),
                        ("im".into(), fp_literal_expr(im)),
                    ],
                },
                result_ty,
            );
            return;
        }
        if let Some(b) = parse_cir_bool(raw) {
            self.materialize_expr(result, Expr::Value(RustValue::Bool(b)), result_ty);
            return;
        }
        if raw.starts_with("#cir.ptr<null>") {
            self.materialize_expr(result, Expr::Raw("std::ptr::null_mut()".into()), result_ty);
            return;
        }
        let value = if result_ty.is_some_and(is_long_double) {
            let value = parse_cir_fp_expr(raw)
                .or_else(|| parse_cir_int(raw).map(int_value_expr))
                .unwrap_or_else(|| Expr::Value(RustValue::Float(0.0)));
            Expr::Call {
                func: Box::new(Expr::Var(LONG_DOUBLE_TY.into())),
                args: vec![value],
            }
        } else {
            parse_cir_scalar_expr(raw).unwrap_or_else(|| Expr::Value(RustValue::I64(0)))
        };
        self.materialize_expr(result, value, result_ty);
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
                    if op.name == "cir.yield" {
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
        let lhs = self.operand_expr(&op.operands[0]);
        let rhs = self.operand_expr(&op.operands[1]);
        let ty = op_result_type(op);
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
                rhs: Box::new(Self::raw_expr("f64::NEG_INFINITY")),
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
                rhs: Box::new(Self::raw_expr("f64::INFINITY")),
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
        let lhs = self.operand_expr(&op.operands[0]);
        let rhs = self.operand_expr(&op.operands[1]);
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

    fn lower_get_global(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        let name = attr_str(op, "name")
            .unwrap_or("")
            .trim_start_matches('@')
            .trim_matches('"')
            .to_string();
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
        } else if let Some(slot) = self.slots.get(ptr) {
            Some(Expr::Var(slot.clone().into()))
        } else {
            self.global_name(ptr).map(|name| Expr::Var(name.into()))
        }
    }

    fn place_or_deref_expr(&self, ptr: &str) -> Expr {
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
        let base = self.place_or_deref_expr(base_ptr);
        let field = sanitize_ident(attr_str(op, "name").unwrap_or(result)).into_string();
        let unsafe_access = self.ptr_requires_unsafe(base_ptr) || self.op_base_is_union(op);
        self.member_ptrs.insert(
            result.clone(),
            MemberPtr {
                base,
                field,
                unsafe_access,
            },
        );
    }

    fn lower_set_bitfield(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        if op.operands.len() < 2 {
            return;
        }
        let value = self.operand_expr(&op.operands[1]);
        let ty = op_result_type(op);
        let trunc = self.truncate_bitfield_expr(op, value, ty);
        self.materialize_expr(result, trunc, ty);
        let stored = self.operand_expr(result);
        let (place, needs_unsafe) = self.bitfield_place(&op.operands[0]);
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
        let read = if needs_unsafe {
            Self::unsafe_expr(place)
        } else {
            place
        };
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
        let raw = attr_str(op, "bitfield_info")?;
        let resolved = self.parent.aliases.get(raw).map_or(raw, String::as_str);
        let rest = resolved.split("size = ").nth(1)?;
        let end = rest
            .find(|c: char| !c.is_ascii_digit())
            .unwrap_or(rest.len());
        rest[..end].parse().ok()
    }

    fn lower_get_element(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        if op.operands.len() < 2 {
            return;
        }
        let base_ptr = &op.operands[0];
        let base = self.place_or_deref_expr(base_ptr);
        let index = self.operand_expr(&op.operands[1]);
        let unsafe_access = self.ptr_requires_unsafe(base_ptr);
        self.element_ptrs.insert(
            result.clone(),
            ElementPtr {
                base,
                index,
                unsafe_access,
            },
        );
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
            Some(Val::Global(name)) => Val::Global(name),
            _ if self
                .slot_types
                .get(src)
                .is_some_and(|ty| parse_rust_array_type(ty).is_some()) =>
            {
                Val::Expr(Expr::ArrayPtr {
                    array: Box::new(self.operand_expr(src)),
                    mutable: true,
                })
            }
            _ if is_long_double(result_ty) && !is_long_double(operand_ty) => {
                Val::Expr(Expr::Call {
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
            // integer sentinel (SIG_IGN/SIG_DFL/SIG_ERR = (void(*)(int))N) cast to a
            // fn pointer: `as` cannot target Option<fn(..)>, so reinterpret the bits.
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
        let lhs = self.operand_expr(&op.operands[0]);
        let rhs = self.operand_expr(&op.operands[1]);
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
        let base = self.operand_expr(&op.operands[0]);
        let index = self.operand_expr(&op.operands[1]);
        self.values.insert(
            result.clone(),
            Val::Expr(Self::unsafe_expr(Expr::MethodCall {
                recv: Box::new(base),
                method: "offset".into(),
                args: vec![Expr::Cast {
                    expr: Box::new(index),
                    ty: crate::rust_ast::Type::Prim(Prim::Isize),
                }],
            })),
        );
    }

    fn lower_call(&mut self, op: &Op) {
        let operand_types = op_operand_types(op.ty.as_deref().unwrap_or(""));
        let direct_callee =
            attr_str(op, "callee").map(|callee| callee.trim_start_matches('@').to_string());
        let (callee_name, callee_expr, arg_operands, arg_types) =
            if let Some(callee) = direct_callee {
                (
                    callee.clone(),
                    Expr::Var(callee.into()),
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
                    ty: Some(Type::LongDouble),
                    init: Some(self.default_value_expr(LONG_DOUBLE_TY)),
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
                self.values
                    .insert(result.to_string(), Val::Expr(Expr::Var(name.into())));
            }
            return;
        }
        if callee_name == "printf"
            && arg_types.iter().any(|ty| is_long_double(ty))
            && args.len() == 3
            && arg_types.get(1).is_some_and(|ty| is_long_double(ty))
            && arg_types.get(2).is_some_and(|ty| *ty == "!s32i")
        {
            let i8_ptr = Type::Ptr {
                mutable: true,
                inner: Box::new(Type::Prim(Prim::I8)),
            };
            let long_double_ptr = Type::Ptr {
                mutable: false,
                inner: Box::new(Type::LongDouble),
            };
            let expr = Self::unsafe_expr(Expr::Call {
                func: Box::new(Expr::Var("__slate_printf_ld_i32".into())),
                args: vec![
                    Expr::Cast {
                        expr: Box::new(args[0].clone()),
                        ty: i8_ptr,
                    },
                    Expr::Cast {
                        expr: Box::new(Expr::AddrOf {
                            mutable: false,
                            expr: Box::new(args[1].clone()),
                        }),
                        ty: long_double_ptr,
                    },
                    Expr::Cast {
                        expr: Box::new(args[2].clone()),
                        ty: Type::Prim(Prim::I32),
                    },
                ],
            });
            if let Some(result) = op.results.first() {
                self.materialize_expr(result, expr, op_result_type(op));
            } else {
                self.push_stmt(Stmt::Expr(expr));
            }
            return;
        }
        let call = Expr::Call {
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
        let fetched = Expr::AtomicFetch {
            ty: atomic_ty,
            op: atomic_rmw_op(binop),
            ptr: Box::new(self.store_address_expr(&op.operands[0])),
            value: Box::new(val.clone()),
            ordering: rust_ordering(attr_int(op, "mem_order").unwrap_or(5)),
        };
        // std atomics always return the pre-op value; `fetch_first` wants that,
        // otherwise recompute the post-op value the op just stored.
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
            let expr = Expr::AtomicSwap {
                ty: atomic_ty,
                ptr: Box::new(self.store_address_expr(&op.operands[0])),
                value: Box::new(val),
                ordering: rust_ordering(attr_int(op, "mem_order").unwrap_or(5)),
            };
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
        self.values.insert(result, Val::Expr(Expr::Var(old.into())));
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
            // Always strong: `compare_exchange_weak` may spuriously fail and
            // diverge from the C reference under differential testing.
            let res = self.next_temp();
            self.push_stmt(Stmt::Let {
                name: res.clone(),
                mutable: false,
                ty: Some(Type::Generic {
                    name: "Result".into(),
                    args: vec![ty.clone(), ty.clone()],
                }),
                init: Some(Expr::AtomicCompareExchange {
                    ty: atomic_ty,
                    ptr: Box::new(self.store_address_expr(&op.operands[0])),
                    expected: Box::new(expected),
                    desired: Box::new(desired),
                    success: rust_ordering(attr_int(op, "succ_order").unwrap_or(5)),
                    failure: load_ordering(attr_int(op, "fail_order").unwrap_or(5)),
                }),
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
        self.values
            .insert(op.results[0].clone(), Val::Expr(Expr::Var(old.into())));
        self.values
            .insert(op.results[1].clone(), Val::Expr(Expr::Var(ok.into())));
    }

    fn lower_atomic_fence(&mut self, op: &Op) {
        // a relaxed thread fence is a no-op, and `fence(Relaxed)` panics in Rust.
        let ordering = attr_int(op, "ordering").unwrap_or(5);
        if ordering == 0 {
            return;
        }
        self.push_stmt(Stmt::Expr(Expr::AtomicFence {
            ordering: rust_ordering(ordering),
        }));
    }

    // atomic `cir.load`/`cir.store` (they carry `mem_order`) — real atomic
    // access so a shared `_Atomic` object is never touched non-atomically.
    fn atomic_load_expr(&self, op: &Op, ptr: &str) -> Option<Expr> {
        let mem_order = attr_int(op, "mem_order")?;
        let ty = op_result_type(op).map(|ty| self.parent.rust_type(ty))?;
        let atomic_ty = atomic_type(&ty)?;
        Some(Expr::AtomicLoad {
            ty: atomic_ty,
            ptr: Box::new(self.store_address_expr(ptr)),
            ordering: load_ordering(mem_order),
        })
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
        self.push_stmt(Stmt::Expr(Expr::AtomicStore {
            ty: wrapper,
            ptr: Box::new(self.store_address_expr(ptr)),
            value: Box::new(value),
            ordering: store_ordering(mem_order),
        }));
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
            Self::raw_expr(slot),
            Expr::MethodCall {
                recv: Box::new(Self::raw_expr(args)),
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
            self.push_stmt(Stmt::Expr(Expr::Call {
                func: Box::new(Self::raw_expr("std::process::exit")),
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
                if op.name == "cir.condition" {
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
            .filter(|op| op.name == "cir.case")
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
            for value in &case.values {
                selector_arms.push(ExprMatchArm {
                    pattern: int_pattern(*value as i128),
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
                        Self::raw_expr(case_name.clone()),
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
                    expr: Box::new(Self::raw_expr(selector_name)),
                    arms: selector_arms,
                }),
            }),
            Self::indent_stmt(Stmt::Loop {
                label: Some(label),
                body: vec![Self::indent_stmt(Stmt::Match {
                    expr: Self::raw_expr(case_name),
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

    fn lower_dispatch(&mut self, body: &Region) {
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
                if op.name == "cir.label"
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
                if op.name == "cir.alloca" {
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
                    Self::raw_expr(state_var.clone()),
                    Expr::Value(RustValue::I64((i + 1) as i64)),
                )));
                body.push(Self::indent_stmt(Stmt::Continue(Some(loop_label.clone()))));
            }
            arms.push(MatchArm {
                pattern: int_pattern(i as i128),
                body,
            });
        }
        arms.push(MatchArm {
            pattern: Pattern::Wildcard,
            body: vec![Self::indent_stmt(Stmt::Break(Some(loop_label.clone())))],
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
                expr: Self::raw_expr(state_var),
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
                    Self::raw_expr(state_var),
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
                    Self::raw_expr(state_var),
                    Expr::Value(RustValue::I64(state as i64)),
                );
                self.push_stmt(Stmt::Continue(Some(loop_label)));
            }
            None => self.emit_todo("cir.br: unknown successor"),
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
        self.values
            .insert(result.to_string(), Val::Expr(Expr::Var(name.into())));
    }

    fn operand_expr(&self, operand: &str) -> Expr {
        if let Some(val) = self.values.get(operand) {
            return val.to_expr(&self.parent.strings);
        }
        if let Some(slot) = self.slots.get(operand) {
            return Expr::Var(slot.clone().into());
        }
        Expr::Var(sanitize_ident(operand))
    }

    fn element_place_expr(&self, element: &ElementPtr) -> Expr {
        Expr::Index {
            base: Box::new(element.base.clone()),
            index: Box::new(Expr::Cast {
                expr: Box::new(element.index.clone()),
                ty: Type::Prim(Prim::Usize),
            }),
        }
    }

    fn pointer_operand_expr(&self, operand: &str) -> Expr {
        if self.member_ptrs.contains_key(operand) || self.element_ptrs.contains_key(operand) {
            return self.store_address_expr(operand);
        }
        if let Some(value) = self.values.get(operand) {
            return value.to_expr(&self.parent.strings);
        }
        if let Some(slot) = self.slots.get(operand) {
            return if self
                .slot_types
                .get(operand)
                .is_some_and(|ty| parse_rust_array_type(ty).is_some())
            {
                Expr::MethodCall {
                    recv: Box::new(Expr::Var(slot.clone().into())),
                    method: "as_mut_ptr".into(),
                    args: vec![],
                }
            } else {
                Expr::AddrOf {
                    mutable: true,
                    expr: Box::new(Expr::Var(slot.clone().into())),
                }
            };
        }
        Expr::Var(sanitize_ident(operand))
    }

    fn function_pointer_operand_expr(&self, operand: &str) -> Expr {
        match self.values.get(operand) {
            Some(Val::Global(name)) if !self.parent.strings.contains_key(name) => Expr::Call {
                func: Box::new(Expr::Var("Some".into())),
                args: vec![Expr::Var(sanitize_ident(name))],
            },
            Some(value) => value.to_expr(&self.parent.strings),
            None => self.operand_expr(operand),
        }
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

    /// Raw escape hatch for CIR constructs slate cannot lower yet: emits a
    /// `todo!("<note>")` placeholder so the generated Rust still compiles-shaped.
    fn emit_todo(&mut self, note: &str) {
        self.push_stmt(Stmt::Expr(Expr::Todo(note.to_string())));
    }

    /// Raw escape hatch for pre-rendered statement text (variadic prelude lines,
    /// opaque aggregate memcpy, the `strtold` shim) with no structured node yet.
    fn emit_line(&mut self, line: &str) {
        self.push_stmt(Stmt::Raw(line.to_string()));
    }

    fn push_stmt(&mut self, stmt: Stmt) {
        self.body.push(IndentStmt {
            depth: self.indent,
            stmt,
        });
    }

    fn raw_expr(expr: impl Into<String>) -> Expr {
        let expr = expr.into();
        if is_rust_ident(&expr) {
            Expr::Var(expr.into())
        } else {
            Expr::Raw(expr)
        }
    }

    fn unsafe_expr(value: Expr) -> Expr {
        Expr::Unsafe(Box::new(crate::rust_ast::Block {
            stmts: Vec::new(),
            tail: Some(Box::new(value)),
        }))
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

    fn default_value_expr(&self, ty: &str) -> Expr {
        self.parent.default_value_expr(ty)
    }
}

fn region_ops(op: &Op) -> Vec<&Op> {
    op.regions
        .iter()
        .flat_map(|region| region.blocks.iter())
        .flat_map(|block| block.ops.iter())
        .collect()
}

fn op_mentions_long_double(op: &Op) -> bool {
    op.ty
        .as_deref()
        .is_some_and(|ty| ty.contains("!cir.long_double"))
        || op
            .attrs
            .values()
            .filter_map(Attr::as_str)
            .any(|value| value.contains("!cir.long_double"))
        || op.regions.iter().any(|region| {
            region.blocks.iter().any(|block| {
                block
                    .args
                    .iter()
                    .any(|(_, ty)| ty.contains("!cir.long_double"))
                    || block.ops.iter().any(op_mentions_long_double)
            })
        })
}

fn attr_str<'a>(op: &'a Op, key: &str) -> Option<&'a str> {
    op.attrs.get(key).and_then(Attr::as_str)
}

fn attr_int(op: &Op, key: &str) -> Option<i64> {
    op.attrs.get(key).and_then(Attr::as_int)
}

fn attr_bool(op: &Op, key: &str) -> bool {
    op.attrs.contains_key(key)
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
    let is_default = attr_int(op, "kind") == Some(0);
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
    let region = op.regions.first()?;
    Some(SwitchCase {
        values,
        is_default,
        region,
    })
}

/// Whether a dispatch block ends in its own control transfer (so the dispatch
/// loop must not append a fall-through to the next state).
fn block_diverges(block: &Block) -> bool {
    block
        .ops
        .last()
        .is_some_and(|op| matches!(op.name.as_str(), "cir.return" | "cir.br" | "cir.goto"))
}

fn region_ends_control_flow(region: &Region) -> bool {
    region
        .blocks
        .iter()
        .rev()
        .flat_map(|block| block.ops.iter().rev())
        .find(|op| op.name != "cir.yield")
        .is_some_and(|op| {
            matches!(
                op.name.as_str(),
                "cir.break" | "cir.continue" | "cir.return"
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
        takes_self: true,
        params,
        ret: Some(long_double_ty()),
        body: Expr::Call {
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
                RustAttr::Derive(vec![Derive::Clone, Derive::Copy]),
            ],
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

const COMPLEX_TY: &str = "Complex<";

// clang lowers complex `*`/`/` to the libgcc runtime (__mul?c3/__div?c3), reached
// directly for `/` and via a NaN-recovery branch for `*`. We call the same symbols
// so results are bit-identical; #[repr(C)] {re, im} matches the return ABI.
fn is_complex_runtime_call(name: &str) -> bool {
    matches!(name, "__muldc3" | "__divdc3" | "__mulsc3" | "__divsc3")
}

fn complex_ty(inner: Type) -> Type {
    Type::Complex(Box::new(inner))
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
        takes_self: true,
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
                RustAttr::Derive(vec![Derive::Clone, Derive::Copy]),
            ],
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

fn rust_type(cir_ty: &str) -> Type {
    rust_type_with_aliases(cir_ty, &BTreeMap::new())
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
    ops.iter().any(|op| match op.name.as_str() {
        "cir.continue" => true,
        "cir.for" | "cir.while" | "cir.do" => false,
        _ => op.regions.iter().any(region_has_direct_continue),
    })
}

fn rust_type_with_aliases(cir_ty: &str, aliases: &BTreeMap<String, String>) -> Type {
    let ty = cir_ty.trim();
    if let Some(expanded) = aliases.get(ty) {
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
    } else if ty == "!cir.float" {
        Type::Prim(Prim::F32)
    } else if ty == "!cir.double" {
        Type::Prim(Prim::F64)
    } else if is_long_double(ty) {
        Type::LongDouble
    } else if let Some(inner) = ty
        .strip_prefix("!cir.complex<")
        .and_then(|s| s.strip_suffix('>'))
    {
        Type::Complex(Box::new(rust_type_with_aliases(inner, aliases)))
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
        if name == "_IO_FILE" {
            Type::CLib(CLibType::File)
        } else {
            Type::Custom(sanitize_ident(name).into_string())
        }
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
        params,
        ret: Box::new(ret),
    })
}

fn type_mentions_long_double(ty: &Type) -> bool {
    match ty {
        Type::LongDouble => true,
        Type::Complex(inner) => type_mentions_long_double(inner),
        Type::Generic { args, .. } => args.iter().any(type_mentions_long_double),
        Type::Ptr { inner, .. } => type_mentions_long_double(inner),
        Type::Array { elem, .. } => type_mentions_long_double(elem),
        Type::FnPtr { params, ret } => {
            params.iter().any(type_mentions_long_double) || type_mentions_long_double(ret)
        }
        Type::Prim(_)
        | Type::Custom(_)
        | Type::TyVar(_)
        | Type::CLib(_)
        | Type::VaList
        | Type::Unit
        | Type::Variadic => false,
    }
}

fn type_mentions_complex(ty: &Type) -> bool {
    match ty {
        Type::Complex(_) => true,
        Type::Ptr { inner, .. } => type_mentions_complex(inner),
        Type::Array { elem, .. } => type_mentions_complex(elem),
        Type::FnPtr { params, ret } => {
            params.iter().any(type_mentions_complex) || type_mentions_complex(ret)
        }
        Type::Generic { args, .. } => args.iter().any(type_mentions_complex),
        Type::Prim(_)
        | Type::Custom(_)
        | Type::LongDouble
        | Type::TyVar(_)
        | Type::CLib(_)
        | Type::VaList
        | Type::Unit
        | Type::Variadic => false,
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

fn default_value_expr(ty: &str) -> Expr {
    match ty {
        "bool" => Expr::Value(RustValue::Bool(false)),
        "f32" | "f64" => Expr::Value(RustValue::Float(0.0)),
        ty if ty.starts_with("*mut ") => Expr::Value(RustValue::NullPtr),
        ty if ty.starts_with("Option<fn(") => Expr::Value(RustValue::None),
        _ => Expr::Value(RustValue::I64(0)),
    }
}

fn standard_record_def(name: &str) -> RecordDef {
    let i8_ty = || Type::Prim(Prim::I8);
    let i32_ty = || Type::Prim(Prim::I32);
    let i64_ty = || Type::Prim(Prim::I64);
    let i8_ptr_ty = || Type::Ptr {
        mutable: true,
        inner: Box::new(Type::Prim(Prim::I8)),
    };
    let fields: Vec<(Ident, Type)> = match name {
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
    };
    RecordDef {
        is_union: false,
        allow_non_camel_case: true,
        name: name.to_string(),
        fields,
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
        Type::Ptr { .. } => Expr::Value(RustValue::NullPtr),
        Type::FnPtr { .. } => Expr::Value(RustValue::None),
        _ => Expr::Value(RustValue::I64(0)),
    }
}

fn render_array_literal_expr(elems: &[Expr], len: usize, default: Expr) -> Expr {
    let mut out: Vec<Expr> = elems.iter().take(len).cloned().collect();
    out.resize(len, default);
    Expr::ArrayLit(out)
}

fn parse_cir_scalar_expr(s: &str) -> Option<Expr> {
    parse_cir_int(s)
        .map(int_value_expr)
        .or_else(|| parse_cir_fp_expr(s))
        .or_else(|| parse_cir_bool(s).map(|b| Expr::Value(RustValue::Bool(b))))
        .or_else(|| {
            s.trim_start()
                .starts_with("#cir.ptr<null>")
                .then(|| default_value_expr("*mut i32"))
        })
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

fn fp_literal_expr(fp: String) -> Expr {
    fp.parse::<f64>()
        .map(|n| Expr::Value(RustValue::Float(n)))
        .unwrap_or_else(|_| Expr::Raw(fp))
}

// i128 so a full-range `!u64i` value (e.g. SIG_ERR = (void(*)(int))-1, which CIR
// prints as the unsigned bit pattern 18446744073709551615) survives as a valid
// unsigned literal rather than overflowing i64 and collapsing to 0.
fn parse_cir_int(s: &str) -> Option<i128> {
    let start = s.find("#cir.int<")? + "#cir.int<".len();
    let rest = &s[start..];
    let end = rest.find('>')?;
    rest[..end].parse().ok()
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
    let start = s.find("#cir.fp<")? + "#cir.fp<".len();
    let rest = &s[start..];
    let end = rest.find('>')?;
    let text = rest[..end].trim();
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

// `#cir.const_complex<#cir.fp<re> : ty, #cir.fp<im> : ty>` -> (re, im) literals.
fn parse_cir_const_complex(s: &str) -> Option<(String, String)> {
    let start = s.find("#cir.const_complex<")? + "#cir.const_complex<".len();
    let inner = &s[start..];
    let re = parse_cir_fp(inner)?;
    let comma = inner.find(',')?;
    let im = parse_cir_fp(&inner[comma..])?;
    Some((re, im))
}

fn parse_cir_const_array(s: &str) -> Option<Vec<u8>> {
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

fn rust_byte_string(bytes: &[u8]) -> String {
    let mut out = String::from("b\"");
    for b in bytes {
        match *b {
            b'\n' => out.push_str("\\n"),
            b'\r' => out.push_str("\\r"),
            b'\t' => out.push_str("\\t"),
            b'\\' => out.push_str("\\\\"),
            b'"' => out.push_str("\\\""),
            0 => out.push_str("\\0"),
            0x20..=0x7e => out.push(*b as char),
            _ => out.push_str(&format!("\\x{b:02x}")),
        }
    }
    out.push('"');
    out
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

fn is_rust_ident(s: &str) -> bool {
    let s = s.strip_prefix("r#").unwrap_or(s);
    let bytes = s.as_bytes();
    !bytes.is_empty()
        && (bytes[0] == b'_' || bytes[0].is_ascii_alphabetic())
        && bytes
            .iter()
            .all(|b| *b == b'_' || b.is_ascii_alphanumeric())
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
