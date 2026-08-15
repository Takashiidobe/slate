use crate::parse::ast::{
    BinaryOp, CaseRange, Decl, DeclKind, Expr, ExprKind, GlobalKind, Member, NodeId, Obj, Program,
    Relocation, Stmt, StmtKind, SwitchCase, TagOccurrence, Type, UnaryOp, fold_decls,
    is_compatible, next_node_id,
};
use crate::parse::error::{CompileError, CompileResult, SourceLocation};
use crate::parse::lexer::{Keyword, Punct, Token, TokenKind};
use std::{
    collections::{HashMap, HashSet},
    fmt, mem,
};

pub fn parse(tokens: &[Token]) -> CompileResult<Program> {
    let mut parser = Parser::new(tokens);
    parser.parse_program()
}

fn type_node_id(ty: &Type) -> Option<NodeId> {
    match ty {
        Type::Struct { id, .. } | Type::Union { id, .. } | Type::Enum { id, .. } => Some(*id),
        _ => None,
    }
}

fn x87_to_bytes(value: rustc_apfloat::ieee::X87DoubleExtended) -> [u8; 16] {
    use rustc_apfloat::Float;

    let mut out = [0u8; 16];
    let be = value.to_bits().to_be_bytes();
    out[..10].copy_from_slice(&be[6..]);
    out[..10].reverse();
    out
}

fn x87_to_f64(value: rustc_apfloat::ieee::X87DoubleExtended) -> f64 {
    use rustc_apfloat::{Float, FloatConvert};

    let mut loses_info = false;
    let double: rustc_apfloat::ieee::Double = value.convert(&mut loses_info).value;
    f64::from_bits(double.to_bits() as u64)
}

fn f64_to_x87_bytes(val: f64) -> [u8; 16] {
    format!("{val}")
        .parse::<rustc_apfloat::ieee::X87DoubleExtended>()
        .map(x87_to_bytes)
        .unwrap_or([0u8; 16])
}

pub fn const_expr(tokens: &[Token]) -> CompileResult<i64> {
    let mut parser = Parser::new(tokens);
    let value = parser.const_expr()?;
    if !parser.check_eof() {
        return Err(CompileError::at("extra token", parser.peek().location));
    }
    Ok(value)
}

#[derive(Default)]
struct Parser<'a> {
    tokens: &'a [Token],
    pos: usize,
    locals: Vec<Obj>,
    globals: Vec<Obj>,
    type_registry: HashMap<NodeId, Type>,
    tag_decls: Vec<Decl>,
    last_tag_decl: HashMap<NodeId, NodeId>,
    string_label: usize,
    scopes: Vec<Scope>,
    current_fn_return: Option<Type>,
    current_fn: Option<String>,
    fn_refs: HashMap<String, Vec<String>>,
    root_refs: HashSet<String>,
    fn_labels: Vec<(String, SourceLocation)>,
    fn_gotos: Vec<(String, SourceLocation)>,
    break_depth: usize,
    loop_depth: usize,
    current_switch: Option<SwitchContext>,
    builtin_alloca_idx: Option<usize>, // Index of the builtin alloca function in globals
    in_func_params: bool, // True when parsing function parameter declarations (for VLA bounds)
}

#[derive(Default, Debug, Clone, PartialEq)]
struct VarScope {
    name: String,
    idx: Option<usize>,
    is_local: bool,
    type_def: Option<Type>,
    enum_val: Option<i64>,
    enum_ty: Option<Type>,
}

#[derive(Debug, Default, Clone, PartialEq)]
struct Scope {
    vars: HashMap<String, VarScope>,
    tags: HashMap<String, Type>,
}

#[derive(Debug, Default, Clone, Copy, PartialEq, Eq)]
struct VarAttr {
    is_typedef: bool,
    is_static: bool,
    is_extern: bool,
    is_inline: bool,
    is_tls: bool,
    is_constexpr: bool,
    is_const_attr: bool,
    is_noreturn: bool,
    is_deprecated: bool,
    is_nodiscard: bool,
    is_maybe_unused: bool,
    align: i32,
}

#[derive(Debug, Clone, PartialEq, Eq)]
struct SwitchContext {
    cases: Vec<SwitchCase>,
    default_label: Option<String>,
    break_label: String,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
enum RecordKind {
    Struct,
    Union,
}

#[derive(Debug, Default, Clone, Copy, PartialEq, Eq)]
struct RecordAttr {
    is_packed: bool,
    align: Option<i64>,
}

enum SwitchItem<'a> {
    CaseLabel,
    Stmt(&'a Stmt),
}

/// Represents a variable initializer. Since initializers can be nested
/// (e.g. `int x[2][2] = {{1, 2}, {3, 4}}`), this is a tree structure.
#[derive(Debug, Clone, PartialEq)]
struct Initializer {
    ty: Type,
    /// If it's not an aggregate type and has an initializer,
    /// `expr` has an initialization expression.
    expr: Option<Expr>,
    /// If it's an initializer for an aggregate type (e.g. array),
    /// `children` has initializers for its children.
    children: Vec<Initializer>,
    /// True if this is a flexible array (length not specified, e.g., `int x[] = {1,2,3}`)
    is_flexible: bool,
    /// For unions, which member is initialized.
    mem: Option<Member>,
}

/// Represents a designator for local variable initialization.
/// This is used to build expressions like `x[0][1]` or `x.member`.
#[derive(Debug, Clone, PartialEq)]
struct InitDesg {
    /// Index for array element
    idx: i32,
    /// Member for struct element
    member: Option<Member>,
    /// Variable being initialized (only for the root)
    var_idx: Option<usize>,
    var_is_local: bool,
}

impl<'a> Parser<'a> {
    fn new(tokens: &'a [Token]) -> Self {
        Self {
            tokens,
            scopes: vec![Scope::default()],
            ..Default::default()
        }
    }

    fn push_builtin_fn(
        &mut self,
        name: &str,
        params: Vec<Type>,
        return_ty: Type,
        is_variadic: bool,
    ) {
        let ty = Type::Func {
            return_ty: Box::new(return_ty),
            params: params
                .into_iter()
                .enumerate()
                .map(|(i, ty)| (format!("arg{i}"), ty))
                .collect(),
            is_variadic,
        };
        self.globals.push(Obj {
            id: next_node_id(),
            name: name.to_string(),
            ty,
            location: self.last_location(),
            is_function: true,
            is_definition: false, // Declaration only
            ..Default::default()
        });
    }

    fn declare_builtin_functions(&mut self) {
        // Declare alloca as a builtin function: void *alloca(int)
        let alloca_ty = Type::Func {
            return_ty: Box::new(Type::Ptr(Box::new(Type::Void))),
            params: vec![("size".to_string(), Type::Int)],
            is_variadic: false,
        };
        let alloca_idx = self.globals.len();
        self.globals.push(Obj {
            id: next_node_id(),
            name: "alloca".to_string(),
            ty: alloca_ty,
            location: self.last_location(),
            is_function: true,
            is_definition: false, // Declaration only
            ..Default::default()
        });
        self.builtin_alloca_idx = Some(alloca_idx);

        let void_ptr = || Type::Ptr(Box::new(Type::Void));

        for name in [
            "__builtin_clz",
            "__builtin_ctz",
            "__builtin_ffs",
            "__builtin_popcount",
            "__builtin_parity",
            "__builtin_clrsb",
        ] {
            self.push_builtin_fn(name, vec![Type::UInt], Type::Int, false);
        }
        self.push_builtin_fn("__builtin_bswap32", vec![Type::UInt], Type::UInt, false);
        self.push_builtin_fn(
            "__builtin_bitreverse32",
            vec![Type::UInt],
            Type::UInt,
            false,
        );
        for name in ["__builtin_rotateleft32", "__builtin_rotateright32"] {
            self.push_builtin_fn(name, vec![Type::UInt, Type::UInt], Type::UInt, false);
        }

        self.push_builtin_fn(
            "__builtin_expect",
            vec![Type::Long, Type::Long],
            Type::Long,
            false,
        );
        self.push_builtin_fn("__builtin_assume", vec![Type::Int], Type::Void, false);
        self.push_builtin_fn("__builtin_prefetch", vec![void_ptr()], Type::Void, true);
        self.push_builtin_fn("__builtin_trap", vec![], Type::Void, false);
        self.push_builtin_fn("__builtin_unreachable", vec![], Type::Void, false);
        self.push_builtin_fn("__builtin_debugtrap", vec![], Type::Void, false);
        self.push_builtin_fn(
            "__builtin_return_address",
            vec![Type::UInt],
            void_ptr(),
            false,
        );
        self.push_builtin_fn(
            "__builtin_frame_address",
            vec![Type::UInt],
            void_ptr(),
            false,
        );
        self.push_builtin_fn(
            "__builtin___clear_cache",
            vec![void_ptr(), void_ptr()],
            Type::Void,
            false,
        );

        self.push_builtin_fn(
            "__builtin_bcopy",
            vec![void_ptr(), void_ptr(), Type::ULong],
            Type::Void,
            false,
        );
        self.push_builtin_fn(
            "__builtin_bzero",
            vec![void_ptr(), Type::ULong],
            Type::Void,
            false,
        );
        self.push_builtin_fn(
            "__builtin_memchr",
            vec![void_ptr(), Type::Int, Type::ULong],
            void_ptr(),
            false,
        );

        for name in [
            "__builtin_sin",
            "__builtin_cos",
            "__builtin_tan",
            "__builtin_exp",
            "__builtin_exp2",
            "__builtin_log",
            "__builtin_log10",
            "__builtin_log2",
            "__builtin_sqrt",
        ] {
            self.push_builtin_fn(name, vec![Type::Double], Type::Double, false);
        }
        for name in ["__builtin_pow", "__builtin_fmod"] {
            self.push_builtin_fn(name, vec![Type::Double, Type::Double], Type::Double, false);
        }
        self.push_builtin_fn("__builtin_lround", vec![Type::Double], Type::Long, false);
        self.push_builtin_fn("__builtin_llround", vec![Type::Double], Type::Long, false);

        self.push_builtin_fn("__builtin_FILE", vec![], void_ptr(), false);
        self.push_builtin_fn("__builtin_FUNCTION", vec![], void_ptr(), false);
        self.push_builtin_fn("__builtin_LINE", vec![], Type::Int, false);
        self.push_builtin_fn("__builtin_COLUMN", vec![], Type::Int, false);

        self.push_builtin_fn("__builtin_constant_p", vec![Type::Long], Type::Int, false);
        self.push_builtin_fn(
            "__builtin_object_size",
            vec![void_ptr(), Type::Int],
            Type::ULong,
            false,
        );

        self.push_builtin_fn(
            "__atomic_test_and_set",
            vec![void_ptr(), Type::Int],
            Type::Bool,
            false,
        );
        self.push_builtin_fn(
            "__atomic_clear",
            vec![void_ptr(), Type::Int],
            Type::Void,
            false,
        );

        for name in [
            "__builtin_ia32_pause",
            "__builtin_ia32_lfence",
            "__builtin_ia32_mfence",
            "__builtin_ia32_sfence",
        ] {
            self.push_builtin_fn(name, vec![], Type::Void, false);
        }
        self.push_builtin_fn("__builtin_ia32_rdtsc", vec![], Type::ULong, false);
        self.push_builtin_fn(
            "__builtin_ia32_rdtscp",
            vec![Type::Ptr(Box::new(Type::UInt))],
            Type::ULong,
            false,
        );
        self.push_builtin_fn(
            "_mm_crc32_u8",
            vec![Type::UInt, Type::UChar],
            Type::UInt,
            false,
        );
        self.push_builtin_fn(
            "_mm_crc32_u16",
            vec![Type::UInt, Type::UShort],
            Type::UInt,
            false,
        );
        self.push_builtin_fn(
            "_mm_crc32_u32",
            vec![Type::UInt, Type::UInt],
            Type::UInt,
            false,
        );
        self.push_builtin_fn(
            "_mm_crc32_u64",
            vec![Type::ULong, Type::ULong],
            Type::ULong,
            false,
        );

        for name in [
            "__builtin_isnan",
            "__builtin_isinf",
            "__builtin_isfinite",
            "__builtin_isnormal",
            "__builtin_signbit",
        ] {
            self.push_builtin_fn(name, vec![Type::Double], Type::Int, false);
        }
        for name in ["__builtin_islessgreater", "__builtin_isunordered"] {
            self.push_builtin_fn(name, vec![Type::Double, Type::Double], Type::Int, false);
        }
        self.push_builtin_fn("__builtin_huge_val", vec![], Type::Double, false);
        self.push_builtin_fn("__builtin_huge_valf", vec![], Type::Float, false);
        self.push_builtin_fn("__builtin_huge_vall", vec![], Type::LDouble, false);
    }

    fn declare_builtin_typedefs(&mut self) {
        // C11 char16_t and char32_t types (normally defined in <uchar.h>)
        self.push_scope_typedef("char16_t".to_string(), Type::UShort);
        self.push_scope_typedef("char32_t".to_string(), Type::UInt);
        self.push_scope_typedef(
            "__builtin_va_list".to_string(),
            Type::Ptr(Box::new(Type::Void)),
        );
    }

    fn parse_program(&mut self) -> CompileResult<Program> {
        self.declare_builtin_functions();
        self.declare_builtin_typedefs();

        while !self.check_eof() {
            // Handle _Static_assert at file scope
            if self.consume_keyword(Keyword::StaticAssert) {
                self.parse_static_assert()?;
                continue;
            }

            if self.consume_keyword(Keyword::Asm) {
                self.parse_asm_stmt()?;
                self.consume_punct(Punct::Semicolon);
                continue;
            }

            let mut attr = VarAttr::default();
            let basety = self.parse_declspec(Some(&mut attr))?;

            if attr.is_typedef {
                if attr.is_constexpr {
                    self.bail_here("constexpr is not allowed with typedef")?;
                }
                self.parse_typedef(basety)?;
                continue;
            }

            if self.is_function()? {
                self.parse_function_with_basety(basety, &attr)?;
                continue;
            }

            // Global variable
            self.parse_global_variable(basety, &attr)?;
        }
        self.apply_inline_liveness();
        self.scan_globals();
        let decls = fold_decls(&self.globals, &self.tag_decls);
        Ok(Program {
            globals: mem::take(&mut self.globals),
            types: mem::take(&mut self.type_registry),
            decls,
        })
    }

    fn parse_declspec(&mut self, mut attr: Option<&mut VarAttr>) -> CompileResult<Type> {
        const VOID: i32 = 1 << 0;
        const BOOL: i32 = 1 << 2;
        const CHAR: i32 = 1 << 4;
        const SHORT: i32 = 1 << 6;
        const SHORT_INT: i32 = SHORT + INT;
        const INT: i32 = 1 << 8;
        const LONG: i32 = 1 << 10;
        const LONG_INT: i32 = LONG + INT;
        const LONG_LONG: i32 = LONG + LONG;
        const LONG_LONG_INT: i32 = LONG + LONG + INT;
        const OTHER: i32 = 1 << 12;
        const FLOAT: i32 = 1 << 16;
        const DOUBLE: i32 = 1 << 18;
        const LONG_DOUBLE: i32 = LONG + DOUBLE;
        const COMPLEX: i32 = 1 << 20;
        const FLOAT_COMPLEX: i32 = FLOAT + COMPLEX;
        const DOUBLE_COMPLEX: i32 = DOUBLE + COMPLEX;
        const LONG_DOUBLE_COMPLEX: i32 = LONG_DOUBLE + COMPLEX;
        const SIGNED: i32 = 1 << 13;
        const UNSIGNED: i32 = 1 << 14;
        const SIGNED_CHAR: i32 = SIGNED + CHAR;
        const SIGNED_SHORT: i32 = SIGNED + SHORT;
        const SIGNED_SHORT_INT: i32 = SIGNED + SHORT_INT;
        const SIGNED_INT: i32 = SIGNED + INT;
        const SIGNED_LONG: i32 = SIGNED + LONG;
        const SIGNED_LONG_INT: i32 = SIGNED + LONG_INT;
        const SIGNED_LONG_LONG: i32 = SIGNED + LONG_LONG;
        const SIGNED_LONG_LONG_INT: i32 = SIGNED + LONG_LONG_INT;
        const UNSIGNED_CHAR: i32 = UNSIGNED + CHAR;
        const UNSIGNED_SHORT: i32 = UNSIGNED + SHORT;
        const UNSIGNED_SHORT_INT: i32 = UNSIGNED + SHORT_INT;
        const UNSIGNED_INT: i32 = UNSIGNED + INT;
        const UNSIGNED_LONG: i32 = UNSIGNED + LONG;
        const UNSIGNED_LONG_INT: i32 = UNSIGNED + LONG_INT;
        const UNSIGNED_LONG_LONG: i32 = UNSIGNED + LONG_LONG;
        const UNSIGNED_LONG_LONG_INT: i32 = UNSIGNED + LONG_LONG_INT;

        let mut ty = Type::Int;
        let mut counter = 0i32;
        let mut saw_typename = false;
        let mut is_atomic_qualifier = false;
        let mut record_attr = RecordAttr::default();

        while self.is_typename() || self.peek_is_attribute() {
            if self.peek_is_attribute() {
                self.parse_attributes(attr.as_deref_mut())?;
                continue;
            }
            if matches!(self.peek().kind, TokenKind::Keyword(Keyword::Attribute)) {
                self.parse_gnu_attributes(attr.as_deref_mut(), Some(&mut record_attr))?;
                continue;
            }
            saw_typename = true;

            // Handle storage class specifiers
            let token = self.peek();
            let is_typedef_kw = matches!(token.kind, TokenKind::Keyword(Keyword::Typedef));
            let is_static_kw = matches!(token.kind, TokenKind::Keyword(Keyword::Static));
            let is_extern_kw = matches!(token.kind, TokenKind::Keyword(Keyword::Extern));
            let is_inline_kw = matches!(token.kind, TokenKind::Keyword(Keyword::Inline));
            let is_tls_kw = matches!(token.kind, TokenKind::Keyword(Keyword::ThreadLocal));

            if is_typedef_kw || is_static_kw || is_extern_kw || is_inline_kw || is_tls_kw {
                if let Some(attr) = attr.as_deref_mut() {
                    if is_typedef_kw {
                        self.consume_keyword(Keyword::Typedef);
                        attr.is_typedef = true;
                    } else if is_static_kw {
                        self.consume_keyword(Keyword::Static);
                        attr.is_static = true;
                    } else if is_extern_kw {
                        self.consume_keyword(Keyword::Extern);
                        attr.is_extern = true;
                    } else if is_inline_kw {
                        self.consume_keyword(Keyword::Inline);
                        attr.is_inline = true;
                    } else {
                        self.consume_keyword(Keyword::ThreadLocal);
                        attr.is_tls = true;
                    }

                    if attr.is_typedef
                        && (attr.is_static || attr.is_extern || attr.is_inline || attr.is_tls)
                    {
                        self.bail_here(
                            "typedef may not be used together with static, extern, inline, __thread or _Thread_local",
                        )?;
                    }
                } else {
                    self.bail_here("storage class specifier is not allowed in this context")?;
                }
                continue;
            }

            if self.consume_keyword(Keyword::Constexpr) {
                if let Some(attr) = attr.as_deref_mut() {
                    attr.is_constexpr = true;
                } else {
                    self.bail_here("constexpr is not allowed in this context")?;
                }
                continue;
            }

            // Ignore qualifiers and non-semantic specifiers.
            if self.consume_type_qualifier() {
                continue;
            }

            // Handle _Alignas
            if self.consume_keyword(Keyword::Alignas) {
                if let Some(attr) = attr.as_deref_mut() {
                    self.expect_punct(Punct::LParen)?;

                    // _Alignas can take either a typename or a constant expression
                    if self.is_typename() {
                        let ty = self.parse_typename()?;
                        attr.align = ty.align() as i32;
                    } else {
                        attr.align = self.const_expr()? as i32;
                    }

                    self.expect_punct(Punct::RParen)?;
                } else {
                    self.bail_here("_Alignas is not allowed in this context")?;
                }
                continue;
            }

            // Handle _Atomic
            if self.consume_keyword(Keyword::Atomic) {
                if self.consume_punct(Punct::LParen) {
                    // _Atomic(type) form
                    ty = self.parse_typename()?;
                    ty = Type::Atomic(Box::new(ty));
                    self.expect_punct(Punct::RParen)?;
                    counter += OTHER;
                } else {
                    // _Atomic as a type qualifier (will be handled below at the end)
                    is_atomic_qualifier = true;
                }
                continue;
            }

            let token = self.peek().clone();
            let typedef_ty = self.find_typedef(&token);
            let is_struct_union_enum = matches!(
                token.kind,
                TokenKind::Keyword(Keyword::Struct | Keyword::Union | Keyword::Enum)
            );
            let is_typeof = matches!(
                token.kind,
                TokenKind::Keyword(Keyword::Typeof | Keyword::TypeofUnqual)
            );
            let is_bitint = matches!(token.kind, TokenKind::Keyword(Keyword::BitInt));
            let is_int128 = matches!(token.kind, TokenKind::Ident(ref name) if name == "__int128");
            let is_float128 =
                matches!(token.kind, TokenKind::Ident(ref name) if name == "__float128");
            if is_struct_union_enum
                || is_typeof
                || is_bitint
                || is_int128
                || is_float128
                || typedef_ty.is_some()
            {
                // Allow _BitInt to follow signed/unsigned, but nothing else
                let only_signedness = counter == SIGNED || counter == UNSIGNED || counter == 0;
                if (is_bitint || is_int128) && only_signedness {
                    // Allow _BitInt after signed/unsigned
                } else if counter != 0 {
                    break;
                }

                if self.consume_keyword(Keyword::Struct) {
                    let pending = mem::take(&mut record_attr);
                    ty = self.parse_struct_decl_with_attr(pending)?;
                } else if self.consume_keyword(Keyword::Union) {
                    let pending = mem::take(&mut record_attr);
                    ty = self.parse_union_decl_with_attr(pending)?;
                } else if self.consume_keyword(Keyword::Enum) {
                    ty = self.parse_enum_specifier()?;
                } else if self.consume_keyword(Keyword::Typeof)
                    || self.consume_keyword(Keyword::TypeofUnqual)
                {
                    ty = self.parse_typeof_specifier()?;
                } else if self.consume_keyword(Keyword::BitInt) {
                    self.expect_punct(Punct::LParen)?;
                    let width = self.const_expr()? as i32;
                    if width < 1 {
                        self.bail_here("_BitInt width must be at least 1")?;
                    }
                    // BITINT_MAXWIDTH - support up to 512 bits for now
                    if width > 512 {
                        self.bail_here("_BitInt width exceeds maximum supported (512)")?;
                    }
                    self.expect_punct(Punct::RParen)?;
                    // Check if 'unsigned' was specified before _BitInt
                    let is_signed = (counter & UNSIGNED) == 0;
                    ty = Type::BitInt { width, is_signed };
                } else if is_int128 {
                    self.pos += 1;
                    let is_signed = (counter & UNSIGNED) == 0;
                    ty = Type::BitInt {
                        width: 128,
                        is_signed,
                    };
                } else if is_float128 {
                    self.pos += 1;
                    ty = Type::F128;
                } else if let Some(ty2) = typedef_ty {
                    self.pos += 1;
                    ty = ty2;
                }
                counter += OTHER;
                continue;
            }

            let location = self.peek().location;
            if self.consume_keyword(Keyword::Void) {
                counter += VOID;
            } else if self.consume_keyword(Keyword::Bool) {
                counter += BOOL;
            } else if self.consume_keyword(Keyword::Char) {
                counter += CHAR;
            } else if self.consume_keyword(Keyword::Short) {
                counter += SHORT;
            } else if self.consume_keyword(Keyword::Int) {
                counter += INT;
            } else if self.consume_keyword(Keyword::Long) {
                counter += LONG;
            } else if self.consume_keyword(Keyword::Float) {
                counter += FLOAT;
            } else if self.consume_keyword(Keyword::Double) {
                counter += DOUBLE;
            } else if self.consume_keyword(Keyword::Signed) {
                counter |= SIGNED;
            } else if self.consume_keyword(Keyword::Unsigned) {
                counter |= UNSIGNED;
            } else if self.consume_keyword(Keyword::Complex) {
                counter |= COMPLEX;
            } else {
                self.bail_at(location, "invalid type")?;
            }

            ty = match counter {
                VOID => Type::Void,
                BOOL => Type::Bool,
                CHAR | SIGNED_CHAR => Type::Char,
                UNSIGNED_CHAR => Type::UChar,
                SHORT | SHORT_INT | SIGNED_SHORT | SIGNED_SHORT_INT => Type::Short,
                UNSIGNED_SHORT | UNSIGNED_SHORT_INT => Type::UShort,
                INT | SIGNED | SIGNED_INT => Type::Int,
                UNSIGNED | UNSIGNED_INT => Type::UInt,
                LONG | LONG_INT | LONG_LONG | LONG_LONG_INT | SIGNED_LONG | SIGNED_LONG_INT
                | SIGNED_LONG_LONG | SIGNED_LONG_LONG_INT => Type::Long,
                UNSIGNED_LONG | UNSIGNED_LONG_INT | UNSIGNED_LONG_LONG | UNSIGNED_LONG_LONG_INT => {
                    Type::ULong
                }
                FLOAT => Type::Float,
                DOUBLE => Type::Double,
                LONG_DOUBLE => Type::LDouble,
                FLOAT_COMPLEX => Type::FloatComplex,
                DOUBLE_COMPLEX | COMPLEX => Type::DoubleComplex,
                LONG_DOUBLE_COMPLEX => Type::LDoubleComplex,
                // GNU _Complex/__complex__ applied to an integer type (e.g. `_Complex int`):
                // resolve the integer type from the remaining bits and wrap it.
                _ if counter & COMPLEX != 0 => match counter & !COMPLEX {
                    BOOL => Type::IntComplex(Box::new(Type::Bool)),
                    CHAR | SIGNED_CHAR => Type::IntComplex(Box::new(Type::Char)),
                    UNSIGNED_CHAR => Type::IntComplex(Box::new(Type::UChar)),
                    SHORT | SHORT_INT | SIGNED_SHORT | SIGNED_SHORT_INT => {
                        Type::IntComplex(Box::new(Type::Short))
                    }
                    UNSIGNED_SHORT | UNSIGNED_SHORT_INT => Type::IntComplex(Box::new(Type::UShort)),
                    INT | SIGNED | SIGNED_INT => Type::IntComplex(Box::new(Type::Int)),
                    UNSIGNED | UNSIGNED_INT => Type::IntComplex(Box::new(Type::UInt)),
                    LONG | LONG_INT | LONG_LONG | LONG_LONG_INT | SIGNED_LONG | SIGNED_LONG_INT
                    | SIGNED_LONG_LONG | SIGNED_LONG_LONG_INT => {
                        Type::IntComplex(Box::new(Type::Long))
                    }
                    UNSIGNED_LONG
                    | UNSIGNED_LONG_INT
                    | UNSIGNED_LONG_LONG
                    | UNSIGNED_LONG_LONG_INT => Type::IntComplex(Box::new(Type::ULong)),
                    _ => self.bail_at(location, "invalid type")?,
                },
                _ => self.bail_at(location, "invalid type")?,
            };
        }

        if !saw_typename {
            self.bail_here("typename expected")?;
        }

        if is_atomic_qualifier {
            ty = Type::Atomic(Box::new(ty));
        }

        Ok(ty)
    }

    fn is_typename(&self) -> bool {
        self.is_typename_token(self.peek())
    }

    fn is_typename_token(&self, token: &Token) -> bool {
        match token.kind {
            TokenKind::Keyword(
                Keyword::Void
                | Keyword::Bool
                | Keyword::Char
                | Keyword::Short
                | Keyword::Int
                | Keyword::Long
                | Keyword::Float
                | Keyword::Double
                | Keyword::Struct
                | Keyword::Union
                | Keyword::Enum
                | Keyword::Typedef
                | Keyword::Static
                | Keyword::Extern
                | Keyword::Alignas
                | Keyword::Signed
                | Keyword::Unsigned
                | Keyword::Const
                | Keyword::Volatile
                | Keyword::Auto
                | Keyword::Register
                | Keyword::Restrict
                | Keyword::Noreturn
                | Keyword::Inline
                | Keyword::ThreadLocal
                | Keyword::Attribute
                | Keyword::Constexpr
                | Keyword::Atomic
                | Keyword::Complex
                | Keyword::Typeof
                | Keyword::TypeofUnqual
                | Keyword::BitInt,
            ) => true,
            TokenKind::Ident(ref name) if name == "__int128" || name == "__float128" => true,
            TokenKind::Ident(_) => self.find_typedef(token).is_some(),
            _ => false,
        }
    }

    fn peek_is_attribute(&self) -> bool {
        matches!(self.peek().kind, TokenKind::Punct(Punct::LBracket))
            && matches!(self.peek_n(1).kind, TokenKind::Punct(Punct::LBracket))
    }

    fn parse_attributes(&mut self, mut attr: Option<&mut VarAttr>) -> CompileResult<()> {
        while self.peek_is_attribute() {
            self.expect_punct(Punct::LBracket)?;
            self.expect_punct(Punct::LBracket)?;

            let mut first = true;
            while !(matches!(self.peek().kind, TokenKind::Punct(Punct::RBracket))
                && matches!(self.peek_n(1).kind, TokenKind::Punct(Punct::RBracket)))
            {
                if !first {
                    self.expect_punct(Punct::Comma)?;
                }
                first = false;

                let token = self.peek().clone();
                let name = match token.kind {
                    TokenKind::Ident(name) => {
                        self.pos += 1;
                        name
                    }
                    TokenKind::Keyword(keyword) => {
                        self.pos += 1;
                        keyword.to_string()
                    }
                    _ => self.bail_here("invalid attribute")?,
                };

                if name == "noreturn"
                    && let Some(attr) = attr.as_deref_mut()
                {
                    attr.is_noreturn = true;
                }
                if name == "deprecated"
                    && let Some(attr) = attr.as_deref_mut()
                {
                    attr.is_deprecated = true;
                }
                if name == "nodiscard"
                    && let Some(attr) = attr.as_deref_mut()
                {
                    attr.is_nodiscard = true;
                }
                if name == "maybe_unused"
                    && let Some(attr) = attr.as_deref_mut()
                {
                    attr.is_maybe_unused = true;
                }
                if name == "fallthrough" {
                    // Statement attribute; ignore in declaration context.
                }

                if self.consume_punct(Punct::LParen) {
                    let mut depth = 1i32;
                    while depth > 0 {
                        if matches!(self.peek().kind, TokenKind::Eof) {
                            self.bail_here("unterminated attribute")?;
                        }
                        if self.consume_punct(Punct::LParen) {
                            depth += 1;
                            continue;
                        }
                        if self.consume_punct(Punct::RParen) {
                            depth -= 1;
                            continue;
                        }
                        self.pos += 1;
                    }
                }
            }

            self.expect_punct(Punct::RBracket)?;
            self.expect_punct(Punct::RBracket)?;
        }
        Ok(())
    }

    fn parse_stmt_attributes(&mut self) -> CompileResult<bool> {
        let mut saw_fallthrough = false;
        while self.peek_is_attribute() {
            self.expect_punct(Punct::LBracket)?;
            self.expect_punct(Punct::LBracket)?;

            let mut first = true;
            while !(matches!(self.peek().kind, TokenKind::Punct(Punct::RBracket))
                && matches!(self.peek_n(1).kind, TokenKind::Punct(Punct::RBracket)))
            {
                if !first {
                    self.expect_punct(Punct::Comma)?;
                }
                first = false;

                let token = self.peek().clone();
                let name = match token.kind {
                    TokenKind::Ident(name) => {
                        self.pos += 1;
                        name
                    }
                    TokenKind::Keyword(keyword) => {
                        self.pos += 1;
                        keyword.to_string()
                    }
                    _ => self.bail_here("invalid attribute")?,
                };

                if name == "fallthrough" {
                    saw_fallthrough = true;
                }

                if self.consume_punct(Punct::LParen) {
                    let mut depth = 1i32;
                    while depth > 0 {
                        if matches!(self.peek().kind, TokenKind::Eof) {
                            self.bail_here("unterminated attribute")?;
                        }
                        if self.consume_punct(Punct::LParen) {
                            depth += 1;
                            continue;
                        }
                        if self.consume_punct(Punct::RParen) {
                            depth -= 1;
                            continue;
                        }
                        self.pos += 1;
                    }
                }
            }

            self.expect_punct(Punct::RBracket)?;
            self.expect_punct(Punct::RBracket)?;
        }
        Ok(saw_fallthrough)
    }

    fn consume_type_qualifier(&mut self) -> bool {
        self.consume_keyword(Keyword::Const)
            || self.consume_keyword(Keyword::Volatile)
            || self.consume_keyword(Keyword::Auto)
            || self.consume_keyword(Keyword::Register)
            || self.consume_keyword(Keyword::Restrict)
            || self.consume_keyword(Keyword::Noreturn)
    }

    fn parse_function_with_basety(&mut self, basety: Type, attr: &VarAttr) -> CompileResult<()> {
        // Parse declarator (function name + parameters)
        let (ty, name_token) = self.parse_declarator(basety.clone())?;
        let name = match name_token.kind {
            TokenKind::Ident(name) => name,
            _ => self.bail_at(name_token.location, "function name omitted")?,
        };
        if attr.is_constexpr {
            self.bail_at(name_token.location, "constexpr is only allowed on objects")?;
        }

        // Extract is_variadic and params from the function type
        let (is_variadic, type_params) = match &ty {
            Type::Func {
                is_variadic,
                params,
                ..
            } => (*is_variadic, params.clone()),
            _ => unreachable!("is_function() ensures this is a function type"),
        };

        self.parse_gnu_attributes(None, None)?;

        // Check if this is a declaration (;) or definition ({...})
        let is_definition = !self.consume_punct(Punct::Semicolon);

        // Check if function already exists
        let existing_fn_idx = self.find_global_by_name(&name);

        if let Some(idx) = existing_fn_idx {
            let existing = &self.globals[idx];

            // Check if it was redeclared as a different kind of symbol
            if !existing.is_function {
                self.bail_at(
                    name_token.location,
                    "redeclared as a different kind of symbol",
                )?;
            }

            // Check for redefinition
            if existing.is_definition && is_definition {
                self.bail_at(name_token.location, format!("redefinition of {}", name))?;
            }

            // Check for static after non-static
            let is_static = attr.is_static || (attr.is_inline && !attr.is_extern);
            if !existing.is_static && is_static {
                self.bail_at(
                    name_token.location,
                    "static declaration follows a non-static declaration",
                )?;
            }

            // Update is_definition if this is a definition
            if is_definition {
                self.globals[idx].is_definition = true;
                if attr.is_const_attr {
                    self.globals[idx].is_const_attr = true;
                }
                if attr.is_deprecated {
                    self.globals[idx].is_deprecated = true;
                }
                if attr.is_nodiscard {
                    self.globals[idx].is_nodiscard = true;
                }
                if attr.is_maybe_unused {
                    self.globals[idx].is_maybe_unused = true;
                }
            }
        }

        if !is_definition {
            // Function declaration - no body
            let is_static = attr.is_static || (attr.is_inline && !attr.is_extern);
            if let Some(fn_idx) = existing_fn_idx {
                if attr.is_const_attr {
                    self.globals[fn_idx].is_const_attr = true;
                }
                if attr.is_deprecated {
                    self.globals[fn_idx].is_deprecated = true;
                } else if attr.is_nodiscard {
                    self.globals[fn_idx].is_nodiscard = true;
                } else if attr.is_maybe_unused {
                    self.globals[fn_idx].is_maybe_unused = true;
                }
            } else {
                self.new_function_decl(
                    name,
                    ty,
                    is_static,
                    attr.is_inline,
                    attr.is_const_attr,
                    attr.is_deprecated,
                    attr.is_nodiscard,
                    attr.is_maybe_unused,
                );
            }
            return Ok(());
        }

        // Extract return type from function type
        let return_ty = match &ty {
            Type::Func { return_ty, .. } => return_ty.as_ref().clone(),
            _ => unreachable!(),
        };

        // For function definitions, we need to create local variables from the params
        self.locals.clear();
        self.enter_scope();

        // Create local variables for each parameter
        let mut param_indices = Vec::new();
        if matches!(return_ty, Type::Struct { .. } | Type::Union { .. }) && return_ty.size() > 16 {
            let name = self.new_unique_name();
            let idx = self.new_lvar(name, Type::Ptr(Box::new(return_ty.clone())));
            param_indices.push(idx);
        }
        for (param_name, param_ty) in type_params {
            let name = if param_name.is_empty() {
                self.new_unique_name()
            } else {
                param_name
            };
            let idx = self.new_lvar(name, param_ty);
            param_indices.push(idx);
        }

        self.fn_labels.clear();
        self.fn_gotos.clear();

        // Ensure the function is visible for recursive calls.
        let is_static = attr.is_static || (attr.is_inline && !attr.is_extern);
        if existing_fn_idx.is_none() {
            self.new_function_decl(
                name.clone(),
                ty.clone(),
                is_static,
                attr.is_inline,
                attr.is_const_attr,
                attr.is_deprecated,
                attr.is_nodiscard,
                attr.is_maybe_unused,
            );
        }
        self.fn_refs.entry(name.clone()).or_default();
        let prev_fn = self.current_fn.take();
        self.current_fn = Some(name.clone());

        let prev_return = self.current_fn_return.clone();
        self.current_fn_return = Some(return_ty.clone());

        // Create __va_area__ for variadic functions
        let va_area_idx = if is_variadic {
            Some(self.new_lvar("__va_area__".to_string(), Type::array(Type::Char, 136)))
        } else {
            None
        };

        // Create __alloca_bottom__ for tracking alloca allocations
        let alloca_bottom_idx = Some(self.new_lvar(
            "__alloca_size__".to_string(),
            Type::Ptr(Box::new(Type::Char)),
        ));

        self.expect_punct(Punct::LBrace)?;

        let mut func_bytes = name.as_bytes().to_vec();
        func_bytes.push(0);
        let func_ty = Type::array(Type::Char, func_bytes.len() as i32);
        let func_idx = self.new_string_literal(func_bytes.clone(), func_ty.clone());
        self.push_scope_var_named("__func__".to_string(), func_idx, false);
        let function_idx = self.new_string_literal(func_bytes, func_ty);
        self.push_scope_var_named("__FUNCTION__".to_string(), function_idx, false);

        let mut body = self.parse_block_items()?;
        self.leave_scope();
        self.current_fn_return = prev_return;

        self.validate_goto_labels()?;
        self.fn_labels.clear();
        self.fn_gotos.clear();
        self.current_fn = prev_fn;

        for stmt in &mut body {
            self.add_type_stmt(stmt)?;
        }

        let mut locals = mem::take(&mut self.locals);
        let stack_size = assign_lvar_offsets(&mut locals, &param_indices);

        // Create function object
        let params = param_indices
            .iter()
            .map(|idx| locals[*idx].clone())
            .collect();

        self.new_function_def(
            name,
            ty,
            params,
            body,
            locals,
            va_area_idx,
            alloca_bottom_idx,
            stack_size,
            is_static,
            attr.is_inline,
            attr.is_const_attr,
            attr.is_deprecated,
            attr.is_nodiscard,
            attr.is_maybe_unused,
        );
        Ok(())
    }

    fn is_function(&mut self) -> CompileResult<bool> {
        // Like chibicc: parse the declarator and check if it's a function type
        if self.check_punct(Punct::Semicolon) {
            return Ok(false);
        }

        let saved_pos = self.pos;
        let dummy_type = Type::Int; // dummy base type
        let result = self.parse_declarator(dummy_type);
        self.pos = saved_pos; // restore position

        match result {
            Ok((ty, _)) => Ok(matches!(ty, Type::Func { .. })),
            Err(_) => Ok(false),
        }
    }

    fn parse_global_variable(&mut self, basety: Type, attr: &VarAttr) -> CompileResult<()> {
        let mut first = true;

        while !self.check_punct(Punct::Semicolon) {
            if !first {
                self.expect_punct(Punct::Comma)?;
            }
            first = false;

            let (ty, name_token) = self.parse_declarator(basety.clone())?;
            let name = match name_token.kind {
                TokenKind::Ident(name) => name,
                _ => self.bail_at(name_token.location, "variable name expected")?,
            };
            let var_idx = self.new_gvar(name, ty);
            self.globals[var_idx].is_definition = !attr.is_extern;
            self.globals[var_idx].is_static = attr.is_static;
            self.globals[var_idx].is_tls = attr.is_tls;
            self.globals[var_idx].is_constexpr = attr.is_constexpr;
            self.globals[var_idx].is_const_attr = attr.is_const_attr;
            if attr.is_deprecated {
                self.globals[var_idx].is_deprecated = true;
            }
            if attr.is_nodiscard {
                self.globals[var_idx].is_nodiscard = true;
            }
            if attr.is_maybe_unused {
                self.globals[var_idx].is_maybe_unused = true;
            }
            if attr.align != 0 {
                self.globals[var_idx].align = attr.align;
            }

            // Check for initializer
            if self.consume_punct(Punct::Assign) {
                let assign_location = self.last_location();
                self.parse_gvar_initializer(var_idx, assign_location)?;
            } else if attr.is_constexpr && !attr.is_extern {
                self.bail_at(
                    name_token.location,
                    "constexpr variable requires an initializer",
                )?;
            } else if !attr.is_extern && !attr.is_tls {
                self.globals[var_idx].is_tentative = true;
            }
        }

        self.expect_punct(Punct::Semicolon)?;
        Ok(())
    }

    fn parse_stmt(&mut self) -> CompileResult<Stmt> {
        if self.consume_keyword(Keyword::Return) {
            if self.consume_punct(Punct::Semicolon) {
                return Ok(self.stmt_last(StmtKind::Return(None)));
            }

            let expr = self.parse_expr()?;
            self.expect_punct(Punct::Semicolon)?;
            let expr = if let Some(ret_ty) = self.current_fn_return.clone() {
                if matches!(ret_ty, Type::Struct { .. } | Type::Union { .. }) {
                    expr
                } else {
                    self.cast_expr(expr, ret_ty)
                }
            } else {
                expr
            };
            return Ok(self.stmt_last(StmtKind::Return(Some(expr))));
        }

        if self.peek_is_attribute() {
            let fallthrough = self.parse_stmt_attributes()?;
            self.expect_punct(Punct::Semicolon)?;
            return if fallthrough {
                Ok(self.stmt_last(StmtKind::Fallthrough))
            } else {
                let expr = self.expr_at(ExprKind::Null, self.last_location());
                Ok(self.stmt_last(StmtKind::Expr(expr)))
            };
        }

        if self.consume_keyword(Keyword::If) {
            self.expect_punct(Punct::LParen)?;
            let cond = self.parse_expr()?;
            self.expect_punct(Punct::RParen)?;
            let then = self.parse_stmt()?;
            let els = if self.consume_keyword(Keyword::Else) {
                Some(Box::new(self.parse_stmt()?))
            } else {
                None
            };
            return Ok(self.stmt_last(StmtKind::If {
                cond,
                then: Box::new(then),
                els,
            }));
        }

        if self.consume_keyword(Keyword::Switch) {
            let location = self.last_location();
            self.expect_punct(Punct::LParen)?;
            let cond = self.parse_expr()?;
            self.expect_punct(Punct::RParen)?;

            let prev_switch = self.current_switch.take();
            let break_label = self.new_unique_name();
            self.current_switch = Some(SwitchContext {
                cases: Vec::new(),
                default_label: None,
                break_label: break_label.clone(),
            });

            let prev_break = self.break_depth;
            self.break_depth += 1;
            let body = self.parse_stmt()?;
            self.break_depth = prev_break;

            let ctx = self.current_switch.take().unwrap();
            self.current_switch = prev_switch;

            return Ok(self.stmt_at(
                StmtKind::Switch {
                    cond,
                    body: Box::new(body),
                    cases: ctx.cases,
                    default_label: ctx.default_label,
                    break_label: ctx.break_label,
                },
                location,
            ));
        }

        if self.consume_keyword(Keyword::Case) {
            let location = self.last_location();
            let begin = self.const_expr()?;
            let end = if self.consume_punct(Punct::Ellipsis) {
                let end = self.const_expr()?;
                if end < begin {
                    self.bail_at(self.last_location(), "empty case range specified")?;
                }
                end
            } else {
                begin
            };
            self.expect_punct(Punct::Colon)?;

            let label = self.new_unique_name();
            if let Some(ctx) = self.current_switch.as_mut() {
                ctx.cases.push(SwitchCase {
                    begin,
                    end,
                    label: label.clone(),
                });
            } else {
                self.bail_at(location, "stray case")?;
            }

            let stmt = self.parse_stmt()?;
            return Ok(self.stmt_at(
                StmtKind::Case {
                    range: Some(CaseRange { begin, end }),
                    label,
                    stmt: Box::new(stmt),
                },
                location,
            ));
        }

        if self.consume_keyword(Keyword::Default) {
            let location = self.last_location();
            self.expect_punct(Punct::Colon)?;

            let label = self.new_unique_name();
            if let Some(ctx) = self.current_switch.as_mut() {
                ctx.default_label = Some(label.clone());
            } else {
                self.bail_at(location, "stray default")?;
            }

            let stmt = self.parse_stmt()?;
            return Ok(self.stmt_at(
                StmtKind::Case {
                    range: None,
                    label,
                    stmt: Box::new(stmt),
                },
                location,
            ));
        }

        if self.consume_keyword(Keyword::For) {
            self.expect_punct(Punct::LParen)?;

            self.enter_scope();

            let init = if self.is_typename() {
                let basety = self.parse_declspec(None)?;
                self.parse_declaration(basety, &VarAttr::default())?
            } else {
                self.parse_expr_stmt()?
            };

            let cond = if self.consume_punct(Punct::Semicolon) {
                None
            } else {
                let expr = self.parse_expr()?;
                self.expect_punct(Punct::Semicolon)?;
                Some(expr)
            };
            let inc = if self.consume_punct(Punct::RParen) {
                None
            } else {
                let expr = self.parse_expr()?;
                self.expect_punct(Punct::RParen)?;
                Some(expr)
            };
            self.break_depth += 1;
            self.loop_depth += 1;
            let body = self.parse_stmt()?;
            self.loop_depth -= 1;
            self.break_depth -= 1;

            self.leave_scope();

            return Ok(self.stmt_last(StmtKind::For {
                init: Some(Box::new(init)),
                cond,
                inc,
                body: Box::new(body),
            }));
        }

        if self.consume_keyword(Keyword::While) {
            self.expect_punct(Punct::LParen)?;
            let cond = self.parse_expr()?;
            self.expect_punct(Punct::RParen)?;
            self.break_depth += 1;
            self.loop_depth += 1;
            let body = self.parse_stmt()?;
            self.loop_depth -= 1;
            self.break_depth -= 1;
            return Ok(self.stmt_last(StmtKind::For {
                init: None,
                cond: Some(cond),
                inc: None,
                body: Box::new(body),
            }));
        }

        if self.consume_keyword(Keyword::Do) {
            self.break_depth += 1;
            self.loop_depth += 1;
            let body = self.parse_stmt()?;
            self.loop_depth -= 1;
            self.break_depth -= 1;
            if !self.consume_keyword(Keyword::While) {
                self.bail_here("expected 'while'")?;
            }
            self.expect_punct(Punct::LParen)?;
            let cond = self.parse_expr()?;
            self.expect_punct(Punct::RParen)?;
            self.expect_punct(Punct::Semicolon)?;
            return Ok(self.stmt_last(StmtKind::DoWhile {
                body: Box::new(body),
                cond,
            }));
        }

        if self.consume_keyword(Keyword::Break) {
            if self.break_depth == 0 {
                self.bail_here("stray break")?;
            }
            self.expect_punct(Punct::Semicolon)?;
            return Ok(self.stmt_last(StmtKind::Break));
        }

        if self.consume_keyword(Keyword::Continue) {
            if self.loop_depth == 0 {
                self.bail_here("stray continue")?;
            }
            self.expect_punct(Punct::Semicolon)?;
            return Ok(self.stmt_last(StmtKind::Continue));
        }

        if self.consume_keyword(Keyword::Asm) {
            return self.parse_asm_stmt();
        }

        if self.consume_keyword(Keyword::Goto) {
            let location = self.last_location();
            if self.consume_punct(Punct::Star) {
                let target = self.parse_expr()?;
                self.expect_punct(Punct::Semicolon)?;
                return Ok(self.stmt_at(StmtKind::GotoExpr { target }, location));
            }

            let name_token = self.expect_ident_token()?;
            let label = match name_token.kind {
                TokenKind::Ident(name) => name,
                _ => unreachable!(),
            };
            self.fn_gotos.push((label.clone(), name_token.location));
            self.expect_punct(Punct::Semicolon)?;
            return Ok(self.stmt_at(StmtKind::Goto { label }, name_token.location));
        }

        if matches!(self.peek().kind, TokenKind::Ident(_))
            && matches!(self.peek_n(1).kind, TokenKind::Punct(Punct::Colon))
        {
            let name_token = self.expect_ident_token()?;
            self.expect_punct(Punct::Colon)?;
            let label = match name_token.kind {
                TokenKind::Ident(name) => name,
                _ => unreachable!(),
            };
            self.fn_labels.push((label.clone(), name_token.location));
            let stmt = if self.check_punct(Punct::RBrace) {
                let expr = self.expr_at(ExprKind::Null, self.peek().location);
                self.stmt_at(StmtKind::Expr(expr), self.peek().location)
            } else {
                self.parse_stmt()?
            };
            return Ok(self.stmt_at(
                StmtKind::Label {
                    label,
                    stmt: Box::new(stmt),
                },
                name_token.location,
            ));
        }

        if self.consume_punct(Punct::LBrace) {
            return self.parse_block_stmt();
        }

        self.parse_expr_stmt()
    }

    fn parse_asm_string_literal(&mut self) -> CompileResult<String> {
        let TokenKind::Str { bytes, ty } = self.peek().kind.clone() else {
            return self.bail_here("expected string literal");
        };
        if !matches!(
            ty,
            Type::Array { base, .. } if matches!(*base, Type::Char)
        ) {
            return self.bail_here("expected string literal");
        }
        self.pos += 1;
        Ok(String::from_utf8_lossy(&bytes)
            .trim_end_matches('\0')
            .to_string())
    }

    fn parse_asm_operand_list(&mut self) -> CompileResult<()> {
        if matches!(
            self.peek().kind,
            TokenKind::Punct(Punct::Colon | Punct::RParen)
        ) {
            return Ok(());
        }
        loop {
            if self.consume_punct(Punct::LBracket) {
                self.expect_ident_token()?;
                self.expect_punct(Punct::RBracket)?;
            }
            self.parse_asm_string_literal()?;
            self.expect_punct(Punct::LParen)?;
            self.parse_assign()?;
            self.expect_punct(Punct::RParen)?;
            if !self.consume_punct(Punct::Comma) {
                break;
            }
        }
        Ok(())
    }

    fn parse_asm_clobber_list(&mut self) -> CompileResult<()> {
        if matches!(
            self.peek().kind,
            TokenKind::Punct(Punct::Colon | Punct::RParen)
        ) {
            return Ok(());
        }
        loop {
            self.parse_asm_string_literal()?;
            if !self.consume_punct(Punct::Comma) {
                break;
            }
        }
        Ok(())
    }

    fn parse_asm_goto_label_list(&mut self) -> CompileResult<()> {
        if matches!(self.peek().kind, TokenKind::Punct(Punct::RParen)) {
            return Ok(());
        }
        loop {
            let name_token = self.expect_ident_token()?;
            let label = match name_token.kind {
                TokenKind::Ident(name) => name,
                _ => unreachable!(),
            };
            self.fn_gotos.push((label, name_token.location));
            if !self.consume_punct(Punct::Comma) {
                break;
            }
        }
        Ok(())
    }

    fn parse_asm_stmt(&mut self) -> CompileResult<Stmt> {
        let location = self.last_location();
        loop {
            if self.consume_keyword(Keyword::Volatile) {
                continue;
            }
            if self.consume_keyword(Keyword::Inline) {
                continue;
            }
            break;
        }
        let is_goto = self.consume_keyword(Keyword::Goto);
        self.expect_punct(Punct::LParen)?;
        let asm_str = self.parse_asm_string_literal()?;

        if self.check_punct(Punct::RParen) {
            self.expect_punct(Punct::RParen)?;
            return Ok(self.stmt_at(StmtKind::Asm(asm_str), location));
        }

        self.expect_punct(Punct::Colon)?;
        self.parse_asm_operand_list()?;
        if self.consume_punct(Punct::Colon) {
            self.parse_asm_operand_list()?;
            if self.consume_punct(Punct::Colon) {
                self.parse_asm_clobber_list()?;
                if is_goto && self.consume_punct(Punct::Colon) {
                    self.parse_asm_goto_label_list()?;
                }
            }
        }
        self.expect_punct(Punct::RParen)?;
        Ok(self.stmt_at(StmtKind::Asm(asm_str), location))
    }

    /// Generate code for computing a VLA size.
    /// This returns an expression that computes and stores the VLA size in a local variable.
    fn compute_vla_size(&mut self, ty: &mut Type, location: SourceLocation) -> CompileResult<Expr> {
        // Start with a null expression
        let mut node = self.expr_at(ExprKind::Null, location);

        // Recursively compute VLA sizes for base types first
        // This ensures inner dimensions have their size_var set before outer dimensions need them
        match ty {
            Type::Vla { base, .. } => {
                let base_expr = self.compute_vla_size(base, location)?;
                node = self.expr_at(
                    ExprKind::Comma {
                        lhs: Box::new(node),
                        rhs: Box::new(base_expr),
                    },
                    location,
                );
            }
            Type::Array { base, .. } => {
                let base_expr = self.compute_vla_size(base, location)?;
                node = self.expr_at(
                    ExprKind::Comma {
                        lhs: Box::new(node),
                        rhs: Box::new(base_expr),
                    },
                    location,
                );
            }
            Type::Ptr(base) => {
                let base_expr = self.compute_vla_size(base, location)?;
                node = self.expr_at(
                    ExprKind::Comma {
                        lhs: Box::new(node),
                        rhs: Box::new(base_expr),
                    },
                    location,
                );
            }
            _ => {}
        }

        // Now compute this VLA's size if it is a VLA
        if let Type::Vla {
            base,
            len,
            size_var,
        } = ty
        {
            // Determine base size - at this point, if base is VLA, it has its size_var set
            let base_sz = if let Type::Vla {
                size_var: Some(idx),
                ..
            } = **base
            {
                self.expr_at(
                    ExprKind::Var {
                        idx,
                        is_local: true,
                    },
                    location,
                )
            } else {
                // Otherwise, use compile-time size
                self.expr_at(
                    ExprKind::Num {
                        value: base.size(),
                        fval: 0.0,
                    },
                    location,
                )
            };

            // Create a local variable to hold the VLA size
            let idx = self.new_lvar("".to_string(), Type::ULong);
            *size_var = Some(idx);

            // Create expression: size_var = vla_len * base_sz
            let mul_expr = self.expr_at(
                ExprKind::Binary {
                    op: BinaryOp::Mul,
                    lhs: Box::new(*len.clone()),
                    rhs: Box::new(base_sz),
                },
                location,
            );

            let assign_expr = self.expr_at(
                ExprKind::Assign {
                    lhs: Box::new(self.expr_at(
                        ExprKind::Var {
                            idx,
                            is_local: true,
                        },
                        location,
                    )),
                    rhs: Box::new(mul_expr),
                },
                location,
            );

            node = self.expr_at(
                ExprKind::Comma {
                    lhs: Box::new(node),
                    rhs: Box::new(assign_expr),
                },
                location,
            );
        }

        Ok(node)
    }

    /// Create an expression that calls alloca(sz).
    fn new_alloca(&self, sz: Expr, location: SourceLocation) -> CompileResult<Expr> {
        let alloca_idx = self
            .builtin_alloca_idx
            .ok_or_else(|| CompileError::at("alloca not declared", location))?;

        let alloca_ty = self.globals[alloca_idx].ty.clone();
        if !matches!(&alloca_ty, Type::Func { .. }) {
            return Err(CompileError::at("alloca is not a function", location));
        }

        let callee = self.expr_at(
            ExprKind::Var {
                idx: alloca_idx,
                is_local: false,
            },
            location,
        );

        Ok(self.expr_at(
            ExprKind::Call {
                callee: Box::new(callee),
                args: vec![sz],
                ret_buffer: None,
            },
            location,
        ))
    }

    fn parse_declaration(&mut self, basety: Type, attr: &VarAttr) -> CompileResult<Stmt> {
        let location = self.peek().location;

        let mut stmts = Vec::new();
        let mut first = true;

        while !self.check_punct(Punct::Semicolon) {
            if !first {
                self.expect_punct(Punct::Comma)?;
            }
            first = false;

            let (mut ty, name_token) = self.parse_declarator(basety.clone())?;
            let name = match name_token.kind {
                TokenKind::Ident(name) => name,
                _ => self.bail_at(name_token.location, "variable name omitted")?,
            };
            if ty == Type::Void {
                self.bail_at(name_token.location, "variable declared void")?;
            }

            // Generate code for computing a VLA size. We need to do this
            // even if ty is not VLA because ty may be a pointer to VLA
            // (e.g. int (*foo)[n][m] where n and m are variables.)
            let vla_size_expr = self.compute_vla_size(&mut ty, name_token.location)?;
            stmts.push(self.stmt_at(StmtKind::Expr(vla_size_expr), name_token.location));

            // Handle VLA declarations
            if matches!(ty, Type::Vla { .. }) {
                if self.check_punct(Punct::Assign) {
                    return self.bail_here("variable-sized object may not be initialized");
                }

                // Variable length arrays (VLAs) are translated to alloca() calls.
                // For example, `int x[n+2]` is translated to `tmp = n + 2,
                // x = alloca(tmp)`.
                let Type::Vla { size_var, .. } = &ty else {
                    unreachable!();
                };
                let Some(size_idx) = size_var else {
                    return self.bail_at(name_token.location, "VLA has no size variable");
                };

                let idx = self.new_lvar(name, ty.clone());
                let alloca_call = self.new_alloca(
                    self.expr_at(
                        ExprKind::Var {
                            idx: *size_idx,
                            is_local: true,
                        },
                        name_token.location,
                    ),
                    name_token.location,
                )?;

                let assign_expr = self.expr_at(
                    ExprKind::Assign {
                        lhs: Box::new(self.new_vla_ptr(idx, true, name_token.location)),
                        rhs: Box::new(alloca_call),
                    },
                    name_token.location,
                );

                stmts.push(self.stmt_at(StmtKind::Expr(assign_expr), name_token.location));
                continue;
            }

            // Handle static local variables
            if attr.is_static {
                // Static local variables are stored as globals with anonymous names,
                // but are scoped locally with the user-visible name.
                let idx = self.new_anon_gvar(ty.clone());
                self.globals[idx].is_static = true;
                self.globals[idx].global_kind = GlobalKind::StaticLocal;
                self.globals[idx].is_constexpr = attr.is_constexpr;
                self.globals[idx].is_const_attr = attr.is_const_attr;
                if attr.is_deprecated {
                    self.globals[idx].is_deprecated = true;
                }
                if attr.is_nodiscard {
                    self.globals[idx].is_nodiscard = true;
                }
                if attr.is_maybe_unused {
                    self.globals[idx].is_maybe_unused = true;
                }
                self.push_scope_var_named(name.clone(), idx, false);

                if self.consume_punct(Punct::Assign) {
                    let assign_location = self.last_location();
                    self.parse_gvar_initializer(idx, assign_location)?;
                } else if attr.is_constexpr {
                    self.bail_at(
                        name_token.location,
                        "constexpr variable requires an initializer",
                    )?;
                }
                continue;
            }

            let idx = self.new_lvar(name, ty.clone());
            if attr.is_constexpr {
                self.locals[idx].is_constexpr = true;
            }
            if attr.is_const_attr {
                self.locals[idx].is_const_attr = true;
            }
            if attr.is_deprecated {
                self.locals[idx].is_deprecated = true;
            }
            if attr.is_nodiscard {
                self.locals[idx].is_nodiscard = true;
            }
            if attr.is_maybe_unused {
                self.locals[idx].is_maybe_unused = true;
            }
            if attr.align != 0 {
                self.locals[idx].align = attr.align;
            }
            stmts.push(self.stmt_at(StmtKind::Decl(idx), name_token.location));

            if self.consume_punct(Punct::Assign) {
                let assign_location = self.last_location();
                let mut constexpr_value = None;
                let expr = self.parse_lvar_initializer(
                    idx,
                    true,
                    ty,
                    assign_location,
                    if attr.is_constexpr {
                        Some(&mut constexpr_value)
                    } else {
                        None
                    },
                )?;
                if let Some(value) = constexpr_value {
                    self.locals[idx].const_value = Some(value);
                }
                stmts.push(self.stmt_at(StmtKind::Expr(expr), assign_location));
            } else if attr.is_constexpr {
                self.bail_at(
                    name_token.location,
                    "constexpr variable requires an initializer",
                )?;
            }

            // Check for incomplete type after initialization (which may determine array length)
            let var = &self.locals[idx];
            if var.ty.size() < 0 {
                self.bail_at(name_token.location, "variable has incomplete type")?;
            }
            if var.ty == Type::Void {
                self.bail_at(name_token.location, "variable declared void")?;
            }
        }

        self.expect_punct(Punct::Semicolon)?;
        Ok(self.stmt_at(StmtKind::Block(stmts), location))
    }

    fn parse_declarator(&mut self, mut ty: Type) -> CompileResult<(Type, Token)> {
        ty = self.parse_pointers(ty);

        // Handle nested declarators: "(" declarator ")"
        if self.check_punct(Punct::LParen) {
            let start_pos = self.pos;
            self.pos += 1; // consume '('

            // First pass: parse declarator with dummy type to find where it ends
            let dummy = Type::Char;
            let _ = self.parse_declarator(dummy)?;

            // Skip ')'
            self.expect_punct(Punct::RParen)?;

            // Parse the suffix (arrays, functions, etc.) and apply to current type
            // Save position after suffix - this is where the caller should continue
            ty = self.parse_type_suffix(ty)?;
            let final_pos = self.pos;

            // Second pass: re-parse the declarator with the suffix-applied type
            self.pos = start_pos + 1; // reset to after '('
            let (final_ty, ident_token) = self.parse_declarator(ty)?;

            // Restore position to after the suffix (where the caller should continue)
            self.pos = final_pos;

            return Ok((final_ty, ident_token));
        }

        // Try to consume an identifier, but allow it to be omitted
        let token = self.peek().clone();
        if matches!(token.kind, TokenKind::Ident(_)) {
            self.pos += 1;
        }

        // Parse type suffixes (arrays, functions) recursively
        ty = self.parse_type_suffix(ty)?;

        Ok((ty, token))
    }

    fn parse_abstract_declarator(&mut self, mut ty: Type) -> CompileResult<Type> {
        ty = self.parse_pointers(ty);

        if self.check_punct(Punct::LParen) {
            let start_pos = self.pos;
            self.pos += 1;

            let dummy = Type::Char;
            let _ = self.parse_abstract_declarator(dummy)?;
            self.expect_punct(Punct::RParen)?;

            ty = self.parse_type_suffix(ty)?;
            let final_pos = self.pos;

            self.pos = start_pos + 1;
            let ty = self.parse_abstract_declarator(ty)?;
            self.pos = final_pos;
            return Ok(ty);
        }

        self.parse_type_suffix(ty)
    }

    fn parse_pointers(&mut self, mut ty: Type) -> Type {
        while self.consume_punct(Punct::Star) {
            ty = Type::Ptr(Box::new(ty));
            while self.consume_keyword(Keyword::Const)
                || self.consume_keyword(Keyword::Volatile)
                || self.consume_keyword(Keyword::Restrict)
            {}
        }
        ty
    }

    fn parse_typename(&mut self) -> CompileResult<Type> {
        let basety = self.parse_declspec(None)?;
        self.parse_abstract_declarator(basety)
    }

    fn parse_enum_specifier(&mut self) -> CompileResult<Type> {
        let mut tag: Option<Token> = None;
        if matches!(self.peek().kind, TokenKind::Ident(_)) {
            tag = Some(self.peek().clone());
            self.pos += 1;
        }

        if let Some(tag_token) = &tag
            && !self.check_punct(Punct::LBrace)
            && !self.check_punct(Punct::Colon)
        {
            let name = match &tag_token.kind {
                TokenKind::Ident(name) => name,
                _ => unreachable!(),
            };
            let ty = self
                .find_tag(name)
                .ok_or_else(|| self.err_at(tag_token.location, "unknown enum type"))?;
            if !matches!(ty, Type::Enum { .. }) {
                self.bail_at(tag_token.location, "not an enum tag")?;
            }
            return Ok(ty);
        }

        let mut base_ty = Type::Int;
        if self.consume_punct(Punct::Colon) {
            let ty = self.parse_declspec(None)?;
            if !ty.is_integer() || matches!(ty, Type::Enum { .. }) {
                self.bail_here("enum underlying type must be an integer type")?;
            }
            base_ty = ty;
        }

        self.expect_punct(Punct::LBrace)?;

        let enum_ty = Type::enum_type(base_ty);

        let mut first = true;
        let mut val: i64 = 0;
        while !self.consume_end() {
            if !first {
                self.expect_punct(Punct::Comma)?;
            }
            first = false;

            let name_token = self.expect_ident_token()?;
            let name = match &name_token.kind {
                TokenKind::Ident(name) => name.clone(),
                _ => unreachable!(),
            };

            if self.consume_punct(Punct::Assign) {
                val = self.const_expr()?;
            }

            self.push_scope_enum(name, val, enum_ty.clone());
            val += 1;
        }

        if let Some(tag_token) = tag {
            let name = match tag_token.kind {
                TokenKind::Ident(name) => name,
                _ => unreachable!(),
            };
            if let Some(type_id) = type_node_id(&enum_ty) {
                self.log_tag_decl(
                    name.clone(),
                    tag_token.location,
                    type_id,
                    true,
                    DeclKind::Enum,
                );
            }
            self.push_tag_scope(name, enum_ty.clone());
        }

        Ok(enum_ty)
    }

    fn parse_typeof_specifier(&mut self) -> CompileResult<Type> {
        self.expect_punct(Punct::LParen)?;
        let ty = if self.is_typename() {
            self.parse_typename()?
        } else {
            let mut expr = self.parse_expr()?;
            self.add_type_expr(&mut expr)?;
            expr.ty
                .clone()
                .ok_or_else(|| self.err_at(expr.location, "invalid typeof expression"))?
        };
        self.expect_punct(Punct::RParen)?;
        Ok(ty)
    }

    fn parse_gnu_attributes(
        &mut self,
        mut attr: Option<&mut VarAttr>,
        mut record_attr: Option<&mut RecordAttr>,
    ) -> CompileResult<()> {
        while self.consume_keyword(Keyword::Attribute) {
            self.expect_punct(Punct::LParen)?;
            self.expect_punct(Punct::LParen)?;

            let mut first = true;
            while !self.consume_punct(Punct::RParen) {
                if !first {
                    self.expect_punct(Punct::Comma)?;
                }
                first = false;

                let token = self.peek().clone();
                let name = match token.kind {
                    TokenKind::Ident(name) => {
                        self.pos += 1;
                        name
                    }
                    TokenKind::Keyword(keyword) => {
                        self.pos += 1;
                        keyword.to_string()
                    }
                    _ => self.bail_here("invalid attribute")?,
                };

                match name.as_str() {
                    "const" => {
                        if let Some(attr) = attr.as_deref_mut() {
                            attr.is_const_attr = true;
                        }
                    }
                    "packed" => {
                        if let Some(record_attr) = record_attr.as_deref_mut() {
                            record_attr.is_packed = true;
                        }
                    }
                    "aligned" => {
                        self.expect_punct(Punct::LParen)?;
                        let align = self.const_expr()?;
                        self.expect_punct(Punct::RParen)?;
                        if let Some(record_attr) = record_attr.as_deref_mut() {
                            record_attr.align = Some(align);
                        }
                    }
                    _ => {
                        if self.consume_punct(Punct::LParen) {
                            let mut depth = 1i32;
                            while depth > 0 {
                                if matches!(self.peek().kind, TokenKind::Eof) {
                                    self.bail_here("unterminated attribute")?;
                                }
                                if self.consume_punct(Punct::LParen) {
                                    depth += 1;
                                    continue;
                                }
                                if self.consume_punct(Punct::RParen) {
                                    depth -= 1;
                                    continue;
                                }
                                self.pos += 1;
                            }
                        }
                    }
                }
            }

            self.expect_punct(Punct::RParen)?;
        }

        Ok(())
    }

    /// Parse __attribute__((...)) list for struct/union declarations.
    fn parse_record_attributes(&mut self) -> CompileResult<RecordAttr> {
        let mut attr = RecordAttr::default();
        while self.consume_keyword(Keyword::Attribute) {
            self.expect_punct(Punct::LParen)?;
            self.expect_punct(Punct::LParen)?;

            let mut first = true;
            while !self.consume_punct(Punct::RParen) {
                if !first {
                    self.expect_punct(Punct::Comma)?;
                }
                first = false;

                if matches!(self.peek().kind, TokenKind::Ident(ref name) if name == "packed") {
                    self.pos += 1;
                    attr.is_packed = true;
                    continue;
                }

                if matches!(self.peek().kind, TokenKind::Ident(ref name) if name == "aligned") {
                    self.pos += 1;
                    self.expect_punct(Punct::LParen)?;
                    let align = self.const_expr()?;
                    self.expect_punct(Punct::RParen)?;
                    attr.align = Some(align);
                    continue;
                }

                return self.bail_here("unknown attribute");
            }

            self.expect_punct(Punct::RParen)?;
        }

        Ok(attr)
    }

    fn apply_record_attr(&self, ty: &mut Type, attr: RecordAttr) {
        match ty {
            Type::Struct {
                is_packed,
                align_override,
                ..
            }
            | Type::Union {
                is_packed,
                align_override,
                ..
            } => {
                *is_packed |= attr.is_packed;
                if let Some(align) = attr.align {
                    *align_override = (*align_override).max(align);
                }
            }
            _ => {}
        }
    }

    fn parse_struct_union_decl(
        &mut self,
        kind: RecordKind,
        mut attr_before: RecordAttr,
    ) -> CompileResult<Type> {
        // Parse leading __attribute__((...)) if present
        let parsed_attr = self.parse_record_attributes()?;
        attr_before.is_packed |= parsed_attr.is_packed;
        if let Some(align) = parsed_attr.align {
            attr_before.align = Some(align);
        }

        // Read a tag (optional identifier)
        let tag = if matches!(self.peek().kind, TokenKind::Ident(_)) {
            let tag_token = self.peek().clone();
            self.pos += 1;
            Some(tag_token)
        } else {
            None
        };

        let tag_name = tag.as_ref().map(|tag_token| match &tag_token.kind {
            TokenKind::Ident(name) => name.clone(),
            _ => unreachable!(),
        });
        let tag_location = tag.as_ref().map(|tag_token| tag_token.location);

        // If we have a tag but no opening brace, look up the existing type or
        // create an incomplete type.
        if let Some(name) = tag_name.clone()
            && !self.check_punct(Punct::LBrace)
        {
            if let Some(mut ty) = self.find_tag(&name) {
                self.apply_record_attr(&mut ty, attr_before);
                return Ok(ty);
            }

            let mut ty = match kind {
                RecordKind::Struct => Type::incomplete_struct(Some(name.clone())),
                RecordKind::Union => Type::incomplete_union(Some(name.clone())),
            };
            self.apply_record_attr(&mut ty, attr_before);
            if let Some(type_id) = type_node_id(&ty) {
                self.log_tag_decl(
                    name.clone(),
                    tag_location.unwrap(),
                    type_id,
                    false,
                    DeclKind::Record,
                );
            }
            self.push_tag_scope(name, ty.clone());
            return Ok(ty);
        }

        // If we have a tag, make sure it exists in the current scope before
        // parsing members so self-references work.
        if let Some(name) = tag_name.as_ref()
            && self.find_tag_in_current_scope(name).is_none()
        {
            let mut ty = match kind {
                RecordKind::Struct => Type::incomplete_struct(Some(name.clone())),
                RecordKind::Union => Type::incomplete_union(Some(name.clone())),
            };
            self.apply_record_attr(&mut ty, attr_before);
            if let Some(type_id) = type_node_id(&ty) {
                self.log_tag_decl(
                    name.clone(),
                    tag_location.unwrap(),
                    type_id,
                    false,
                    DeclKind::Record,
                );
            }
            self.push_tag_scope(name.clone(), ty);
        }

        let stub_id = tag_name
            .as_ref()
            .and_then(|name| self.find_tag_in_current_scope(name))
            .and_then(|ty| type_node_id(&ty));

        // Parse members
        let brace_location = self.peek().location;
        self.expect_punct(Punct::LBrace)?;
        let (members, is_flexible) = self.parse_struct_members()?;

        let mut ty = match kind {
            RecordKind::Struct => Type::complete_struct(members, tag_name.clone(), is_flexible),
            RecordKind::Union => Type::complete_union(members, tag_name.clone(), is_flexible),
        };
        if let Some(id) = stub_id {
            match &mut ty {
                Type::Struct { id: ty_id, .. } | Type::Union { id: ty_id, .. } => *ty_id = id,
                _ => {}
            }
        }

        // Parse trailing __attribute__((...)) after closing brace
        let attr_after = self.parse_record_attributes()?;
        let mut attr = attr_before;
        attr.is_packed |= attr_after.is_packed;
        if let Some(align) = attr_after.align {
            attr.align = Some(align);
        }
        self.apply_record_attr(&mut ty, attr);

        if let Some(name) = tag_name {
            if let Some(type_id) = type_node_id(&ty) {
                self.log_tag_decl(
                    name.clone(),
                    tag_location.unwrap(),
                    type_id,
                    true,
                    DeclKind::Record,
                );
            }
            if self.update_tag_in_current_scope(&name, ty.clone()) {
                return Ok(ty);
            }
            self.push_tag_scope(name, ty.clone());
        } else {
            self.register_type(&ty);
            if let Some(type_id) = type_node_id(&ty) {
                self.log_tag_decl(
                    String::new(),
                    brace_location,
                    type_id,
                    true,
                    DeclKind::Record,
                );
            }
        }

        Ok(ty)
    }

    fn parse_struct_decl_with_attr(&mut self, attr: RecordAttr) -> CompileResult<Type> {
        let mut ty = self.parse_struct_union_decl(RecordKind::Struct, attr)?;

        if let Type::Struct {
            members,
            is_incomplete,
            is_packed,
            ..
        } = &mut ty
        {
            if *is_incomplete {
                return Ok(ty);
            }

            let is_packed = *is_packed;

            // Assign offsets within the struct to members
            let mut bits = 0i32;
            for member in members.iter_mut() {
                if member.is_bitfield && member.bit_width == 0 {
                    // Zero-width anonymous bitfield has a special meaning.
                    // It affects only alignment.
                    bits = align_to(bits, bytes_to_bits(member.ty.size() as i32));
                } else if member.is_bitfield {
                    let sz = member.ty.size() as i32;
                    // Check if the bitfield fits in the current storage unit
                    if crosses_storage_unit(bits, member.bit_width, sz) {
                        // Start a new storage unit
                        bits = align_bits_to(bits, sz);
                    }
                    member.offset = align_down(bits_to_bytes(bits), sz);
                    member.bit_offset = bits % bytes_to_bits(sz);
                    bits += member.bit_width;
                } else {
                    // Regular member
                    if !is_packed {
                        bits = align_bits_to(bits, member.align);
                    }
                    member.offset = bits_to_bytes(bits);
                    bits += bytes_to_bits(member.ty.size() as i32);
                }
            }
        }

        if let Type::Struct { tag: Some(tag), .. } = &ty {
            self.update_tag_in_current_scope(tag, ty.clone());
        }

        Ok(ty)
    }

    fn parse_union_decl_with_attr(&mut self, attr: RecordAttr) -> CompileResult<Type> {
        let ty = self.parse_struct_union_decl(RecordKind::Union, attr)?;

        if let Type::Union { is_incomplete, .. } = &ty
            && *is_incomplete
        {
            return Ok(ty);
        }

        // All union members start at offset 0 (already initialized to 0)
        Ok(ty)
    }

    fn parse_struct_members(&mut self) -> CompileResult<(Vec<Member>, bool)> {
        let mut members = Vec::new();
        let mut idx = 0;
        while !self.check_punct(Punct::RBrace) {
            let mut attr = VarAttr::default();
            let basety = self.parse_declspec(Some(&mut attr))?;

            // Anonymous struct/union member
            if matches!(basety, Type::Struct { .. } | Type::Union { .. })
                && self.consume_punct(Punct::Semicolon)
            {
                let align = if attr.align != 0 {
                    attr.align
                } else {
                    basety.align() as i32
                };
                members.push(Member {
                    id: next_node_id(),
                    name: String::new(), // Anonymous - no name
                    ty: basety,
                    location: self.last_location(),
                    idx,
                    align,
                    offset: 0,
                    is_bitfield: false,
                    bit_offset: 0,
                    bit_width: 0,
                });
                idx += 1;
                continue;
            }

            let mut first = true;

            while !self.check_punct(Punct::Semicolon) {
                if !first {
                    self.expect_punct(Punct::Comma)?;
                }
                first = false;
                let (ty, name_token) = self.parse_declarator(basety.clone())?;

                let align = if attr.align != 0 {
                    attr.align
                } else {
                    ty.align() as i32
                };

                let (is_bitfield, bit_width) = if self.consume_punct(Punct::Colon) {
                    let width = self.const_expr()?;
                    (true, width as i32)
                } else {
                    (false, 0)
                };

                // Extract member name, allowing anonymous bitfields
                let name = match name_token.kind {
                    TokenKind::Ident(name) => name,
                    _ if is_bitfield => String::new(), // Anonymous bitfield
                    _ => self.bail_at(name_token.location, "member name expected")?,
                };

                members.push(Member {
                    id: next_node_id(),
                    name,
                    ty,
                    location: name_token.location,
                    idx,
                    align,
                    offset: 0,
                    is_bitfield,
                    bit_offset: 0,
                    bit_width,
                });
                idx += 1;
            }

            self.expect_punct(Punct::Semicolon)?;
        }

        // If the last element is an array of incomplete type, it's
        // called a "flexible array member". It should behave as if
        // it were a zero-sized array.
        let mut is_flexible = false;
        if let Some(last_member) = members.last_mut()
            && let Type::Array { base, len } = &last_member.ty
            && *len < 0
        {
            last_member.ty = Type::array(*base.clone(), 0);
            is_flexible = true;
        }

        self.expect_punct(Punct::RBrace)?;
        Ok((members, is_flexible))
    }

    fn parse_type_suffix(&mut self, ty: Type) -> CompileResult<Type> {
        // Handle function parameters: (...)
        if self.check_punct(Punct::LParen) {
            return self.parse_func_params(ty);
        }

        // Handle array dimensions: [...]
        if self.check_punct(Punct::LBracket) {
            return self.parse_array_dimensions(ty);
        }

        // No suffix
        Ok(ty)
    }

    fn parse_func_params(&mut self, return_ty: Type) -> CompileResult<Type> {
        self.expect_punct(Punct::LParen)?;

        // Handle void parameter list: foo(void)
        if matches!(self.peek().kind, TokenKind::Keyword(Keyword::Void))
            && self
                .tokens
                .get(self.pos + 1)
                .is_some_and(|t| matches!(t.kind, TokenKind::Punct(Punct::RParen)))
        {
            self.consume_keyword(Keyword::Void);
            self.expect_punct(Punct::RParen)?;
            return Ok(Type::func(return_ty, Vec::new(), false));
        }

        let mut params = Vec::new();
        let mut is_variadic = false;

        // Empty parameter list foo() is treated as variadic (old K&R style)
        if self.check_punct(Punct::RParen) {
            self.expect_punct(Punct::RParen)?;
            return Ok(Type::func(return_ty, Vec::new(), true));
        }

        // Set flag to allow VLA bounds to reference undefined identifiers
        // (e.g., function parameters that haven't been added to scope yet)
        let was_in_func_params = self.in_func_params;
        self.in_func_params = true;

        // Parse parameter list
        loop {
            // Check for "..."
            if self.consume_punct(Punct::Ellipsis) {
                is_variadic = true;
                break;
            }

            let param_basety = self.parse_declspec(None)?;
            let (param_ty, param_token) = self.parse_declarator(param_basety)?;

            // Decay array parameters to pointers (also decay VLAs)
            let param_ty = match param_ty {
                Type::Array { base, .. } | Type::Vla { base, .. } => Type::Ptr(base),
                Type::Func { .. } => Type::Ptr(Box::new(param_ty)),
                other => other,
            };

            let param_name = match param_token.kind {
                TokenKind::Ident(name) => name,
                _ => String::new(), // Allow anonymous parameters in declarations
            };

            params.push((param_name, param_ty));

            if !self.consume_punct(Punct::Comma) {
                break;
            }
        }

        self.in_func_params = was_in_func_params;
        self.expect_punct(Punct::RParen)?;
        Ok(Type::func(return_ty, params, is_variadic))
    }

    fn parse_array_dimensions(&mut self, ty: Type) -> CompileResult<Type> {
        self.expect_punct(Punct::LBracket)?;

        while self.consume_keyword(Keyword::Static)
            || self.consume_keyword(Keyword::Restrict)
            || self.consume_keyword(Keyword::Const)
        {}

        if self.consume_punct(Punct::RBracket) {
            // Empty brackets: flexible array
            let ty = self.parse_type_suffix(ty)?;
            return Ok(Type::array(ty, -1));
        }

        // Parse the array size expression
        let mut expr = self.parse_conditional()?;
        self.expect_punct(Punct::RBracket)?;

        // Recursively parse remaining suffixes (could be more arrays or functions)
        let ty = self.parse_type_suffix(ty)?;

        // Check if this should be a VLA
        if matches!(ty, Type::Vla { .. }) || !self.is_const_expr(&mut expr)? {
            return Ok(Type::vla(ty, expr));
        }

        // Build array type with the constant result
        Ok(Type::array(ty, self.eval(&mut expr)? as i32))
    }

    fn parse_expr_stmt(&mut self) -> CompileResult<Stmt> {
        if self.consume_punct(Punct::Semicolon) {
            return Ok(self.stmt_last(StmtKind::Block(Vec::new())));
        }

        let location = self.peek().location;
        let expr = self.parse_expr()?;
        self.expect_punct(Punct::Semicolon)?;
        Ok(self.stmt_at(StmtKind::Expr(expr), location))
    }

    fn parse_expr(&mut self) -> CompileResult<Expr> {
        let expr = self.parse_assign()?;
        if self.consume_punct(Punct::Comma) {
            let location = self.last_location();
            let rhs = self.parse_expr()?;
            return Ok(self.expr_at(
                ExprKind::Comma {
                    lhs: Box::new(expr),
                    rhs: Box::new(rhs),
                },
                location,
            ));
        }
        Ok(expr)
    }

    fn parse_generic_selection(&mut self) -> CompileResult<Expr> {
        let start = self.peek().clone();
        self.expect_punct(Punct::LParen)?;

        let mut ctrl = self.parse_assign()?;
        self.add_type_expr(&mut ctrl)?;
        let mut ctrl_ty = ctrl.ty.clone().unwrap_or(Type::Int);
        ctrl_ty = match ctrl_ty {
            Type::Func { .. } => Type::Ptr(Box::new(ctrl_ty)),
            Type::Array { base, .. } => Type::Ptr(base),
            other => other,
        };

        let mut ret = None::<Expr>;
        while !self.check_punct(Punct::RParen) {
            self.expect_punct(Punct::Comma)?;

            if self.consume_keyword(Keyword::Default) {
                self.expect_punct(Punct::Colon)?;
                let expr = self.parse_assign()?;
                if ret.is_none() {
                    ret = Some(expr);
                }
                continue;
            }

            let assoc_ty = self.parse_typename()?;
            self.expect_punct(Punct::Colon)?;
            let expr = self.parse_assign()?;
            if is_compatible(&ctrl_ty, &assoc_ty) {
                ret = Some(expr);
            }
        }

        self.expect_punct(Punct::RParen)?;
        ret.ok_or_else(|| {
            self.err_at(
                start.location,
                "controlling expression type not compatible with any generic association type",
            )
        })
    }

    fn parse_assign(&mut self) -> CompileResult<Expr> {
        let expr = self.parse_conditional()?;

        if self.consume_punct(Punct::Assign) {
            if !self.is_lvalue(&expr) {
                self.bail_here("invalid assignment target")?;
            }
            let location = self.last_location();
            let rhs = self.parse_assign()?;
            return Ok(self.expr_at(
                ExprKind::Assign {
                    lhs: Box::new(expr),
                    rhs: Box::new(rhs),
                },
                location,
            ));
        }

        let compound_ops = [
            (Punct::AddAssign, BinaryOp::Add),
            (Punct::SubAssign, BinaryOp::Sub),
            (Punct::MulAssign, BinaryOp::Mul),
            (Punct::DivAssign, BinaryOp::Div),
            (Punct::ModAssign, BinaryOp::Mod),
            (Punct::AndAssign, BinaryOp::BitAnd),
            (Punct::OrAssign, BinaryOp::BitOr),
            (Punct::XorAssign, BinaryOp::BitXor),
            (Punct::ShlAssign, BinaryOp::Shl),
            (Punct::ShrAssign, BinaryOp::Shr),
        ];

        for (punct, op) in compound_ops {
            if self.consume_punct(punct) {
                if !self.is_lvalue(&expr) {
                    self.bail_here("invalid assignment target")?;
                }
                let location = self.last_location();
                let rhs = self.parse_assign()?;
                return self.to_assign(expr, rhs, op, location);
            }
        }

        Ok(expr)
    }

    // Convert op= operators to expressions containing an assignment.
    //
    // In general, `A op= B` is converted to `tmp = &A, *tmp = *tmp op B`.
    // If the lhs is a member access, convert `A.x op= B` to
    // `tmp = &A, (*tmp).x = (*tmp).x op B` to avoid taking the address of a bitfield.
    #[expect(
        clippy::wrong_self_convention,
        reason = "to_assign names the `A op= B` -> assignment-expression conversion it performs, not a `to_*` self-consuming conversion"
    )]
    fn to_assign(
        &mut self,
        mut lhs: Expr,
        mut rhs: Expr,
        op: BinaryOp,
        location: SourceLocation,
    ) -> CompileResult<Expr> {
        self.add_type_expr(&mut lhs)?;
        self.add_type_expr(&mut rhs)?;

        // Handle atomic op= by converting to CAS loop
        if let Some(ref lhs_ty) = lhs.ty
            && lhs_ty.is_atomic()
        {
            // Get the base type (unwrap the Atomic wrapper)
            let base_ty = lhs_ty
                .base()
                .ok_or_else(|| self.err_at(location, "atomic type has no base"))?;
            let base_ty = base_ty.clone();
            let ptr_ty = Type::Ptr(Box::new(lhs_ty.clone()));
            // The addend's type may legitimately differ from the atomic's own
            // type -- e.g. `_Atomic(T*) += n` takes an integer offset `n`, not
            // a `T*` -- so size `val` from the actual rhs, not from base_ty.
            let val_ty = rhs.ty.clone().unwrap_or_else(|| base_ty.clone());

            // Create local variables
            let addr_idx = self.new_lvar(String::new(), ptr_ty.clone());
            let val_idx = self.new_lvar(String::new(), val_ty);
            let old_idx = self.new_lvar(String::new(), base_ty.clone());
            let new_idx = self.new_lvar(String::new(), base_ty.clone());

            let mut stmts = vec![
                self.stmt_at(StmtKind::Decl(addr_idx), location),
                self.stmt_at(StmtKind::Decl(val_idx), location),
                self.stmt_at(StmtKind::Decl(old_idx), location),
                self.stmt_at(StmtKind::Decl(new_idx), location),
            ];

            // addr = &lhs
            let addr_var = self.expr_at(
                ExprKind::Var {
                    idx: addr_idx,
                    is_local: true,
                },
                location,
            );
            let addr_init = self.expr_at(
                ExprKind::Assign {
                    lhs: Box::new(addr_var.clone()),
                    rhs: Box::new(self.expr_at(ExprKind::Addr(Box::new(lhs.clone())), location)),
                },
                location,
            );
            stmts.push(self.stmt_at(StmtKind::Expr(addr_init), location));

            // val = rhs
            let val_var = self.expr_at(
                ExprKind::Var {
                    idx: val_idx,
                    is_local: true,
                },
                location,
            );
            let val_init = self.expr_at(
                ExprKind::Assign {
                    lhs: Box::new(val_var.clone()),
                    rhs: Box::new(rhs),
                },
                location,
            );
            stmts.push(self.stmt_at(StmtKind::Expr(val_init), location));

            // old = *addr
            let old_var = self.expr_at(
                ExprKind::Var {
                    idx: old_idx,
                    is_local: true,
                },
                location,
            );
            let old_init = self.expr_at(
                ExprKind::Assign {
                    lhs: Box::new(old_var.clone()),
                    rhs: Box::new(
                        self.expr_at(ExprKind::Deref(Box::new(addr_var.clone())), location),
                    ),
                },
                location,
            );
            stmts.push(self.stmt_at(StmtKind::Expr(old_init), location));

            // Create loop body: new = old op val
            let new_var = self.expr_at(
                ExprKind::Var {
                    idx: new_idx,
                    is_local: true,
                },
                location,
            );
            let bin_expr = match op {
                BinaryOp::Add => self.new_add(old_var.clone(), val_var.clone(), location)?,
                BinaryOp::Sub => self.new_sub(old_var.clone(), val_var.clone(), location)?,
                _ => self.expr_at(
                    ExprKind::Binary {
                        op,
                        lhs: Box::new(old_var.clone()),
                        rhs: Box::new(val_var.clone()),
                    },
                    location,
                ),
            };
            let new_assign = self.expr_at(
                ExprKind::Assign {
                    lhs: Box::new(new_var.clone()),
                    rhs: Box::new(bin_expr),
                },
                location,
            );

            // Create CAS: !atomic_compare_exchange_strong(&old, old, new)
            let old_addr = self.expr_at(ExprKind::Addr(Box::new(old_var.clone())), location);
            let cas = self.expr_at(
                ExprKind::Cas {
                    addr: Box::new(addr_var),
                    old: Box::new(old_addr),
                    new: Box::new(new_var.clone()),
                },
                location,
            );
            let not_cas = self.expr_at(
                ExprKind::Unary {
                    op: UnaryOp::Not,
                    expr: Box::new(cas),
                },
                location,
            );

            // Create do-while loop
            let loop_stmt = self.stmt_at(
                StmtKind::DoWhile {
                    body: Box::new(self.stmt_at(
                        StmtKind::Block(vec![self.stmt_at(StmtKind::Expr(new_assign), location)]),
                        location,
                    )),
                    cond: not_cas,
                },
                location,
            );
            stmts.push(loop_stmt);

            // Final expression is new
            stmts.push(self.stmt_at(StmtKind::Expr(new_var), location));

            return Ok(self.expr_at(ExprKind::StmtExpr(stmts), location));
        }

        if let ExprKind::Var { .. } = lhs.kind {
            let lhs_ty = lhs
                .ty
                .clone()
                .ok_or_else(|| self.err_at(location, "lhs has no type"))?;

            if matches!(lhs_ty, Type::Ptr(_) | Type::Array { .. } | Type::Vla { .. }) {
                let bin_expr = match op {
                    BinaryOp::Add => self.new_add(lhs.clone(), rhs, location)?,
                    BinaryOp::Sub => self.new_sub(lhs.clone(), rhs, location)?,
                    _ => self.expr_at(
                        ExprKind::Binary {
                            op,
                            lhs: Box::new(lhs.clone()),
                            rhs: Box::new(rhs),
                        },
                        location,
                    ),
                };
                return Ok(self.expr_at(
                    ExprKind::Assign {
                        lhs: Box::new(lhs),
                        rhs: Box::new(bin_expr),
                    },
                    location,
                ));
            }

            self.cast_expr_in_place(&mut rhs, lhs_ty);
            return Ok(self.expr_at(
                ExprKind::CompoundAssign {
                    op,
                    lhs: Box::new(lhs),
                    rhs: Box::new(rhs),
                },
                location,
            ));
        }

        if let ExprKind::Member {
            lhs: member_lhs,
            member,
        } = &lhs.kind
        {
            let mut base = member_lhs.as_ref().clone();
            self.add_type_expr(&mut base)?;
            let base_ty = base
                .ty
                .clone()
                .ok_or_else(|| self.err_at(location, "lhs has no type"))?;
            let ptr_ty = Type::Ptr(Box::new(base_ty.clone()));

            let var_idx = self.new_lvar(String::new(), ptr_ty.clone());
            let mut tmp_var = self.expr_at(
                ExprKind::Var {
                    idx: var_idx,
                    is_local: true,
                },
                location,
            );
            tmp_var.ty = Some(ptr_ty);

            let expr1 = self.expr_at(
                ExprKind::Assign {
                    lhs: Box::new(tmp_var.clone()),
                    rhs: Box::new(self.expr_at(ExprKind::Addr(Box::new(base)), location)),
                },
                location,
            );

            let mut deref_tmp = self.expr_at(ExprKind::Deref(Box::new(tmp_var.clone())), location);
            deref_tmp.ty = Some(base_ty);
            let member_lhs_expr = self.expr_at(
                ExprKind::Member {
                    lhs: Box::new(deref_tmp.clone()),
                    member: member.clone(),
                },
                location,
            );
            let member_rhs_expr = self.expr_at(
                ExprKind::Member {
                    lhs: Box::new(deref_tmp),
                    member: member.clone(),
                },
                location,
            );

            let bin_expr = match op {
                BinaryOp::Add => self.new_add(member_rhs_expr, rhs, location)?,
                BinaryOp::Sub => self.new_sub(member_rhs_expr, rhs, location)?,
                _ => self.expr_at(
                    ExprKind::Binary {
                        op,
                        lhs: Box::new(member_rhs_expr),
                        rhs: Box::new(rhs),
                    },
                    location,
                ),
            };

            let expr2 = self.expr_at(
                ExprKind::Assign {
                    lhs: Box::new(member_lhs_expr),
                    rhs: Box::new(bin_expr),
                },
                location,
            );

            return Ok(self.expr_at(
                ExprKind::StmtExpr(vec![
                    self.stmt_at(StmtKind::Decl(var_idx), location),
                    self.stmt_at(StmtKind::Expr(expr1), location),
                    self.stmt_at(StmtKind::Expr(expr2), location),
                ]),
                location,
            ));
        }

        let lhs_ty = lhs
            .ty
            .clone()
            .ok_or_else(|| self.err_at(location, "lhs has no type"))?;
        let ptr_ty = Type::Ptr(Box::new(lhs_ty));

        // Create temporary pointer variable
        let var_idx = self.new_lvar(String::new(), ptr_ty);

        // expr1: tmp = &A
        let expr1 = self.expr_at(
            ExprKind::Assign {
                lhs: Box::new(self.expr_at(
                    ExprKind::Var {
                        idx: var_idx,
                        is_local: true,
                    },
                    location,
                )),
                rhs: Box::new(self.expr_at(ExprKind::Addr(Box::new(lhs.clone())), location)),
            },
            location,
        );

        // *tmp
        let deref_tmp = self.expr_at(
            ExprKind::Deref(Box::new(self.expr_at(
                ExprKind::Var {
                    idx: var_idx,
                    is_local: true,
                },
                location,
            ))),
            location,
        );

        // *tmp op B
        let bin_expr = match op {
            BinaryOp::Add => self.new_add(deref_tmp.clone(), rhs, location)?,
            BinaryOp::Sub => self.new_sub(deref_tmp.clone(), rhs, location)?,
            _ => self.expr_at(
                ExprKind::Binary {
                    op,
                    lhs: Box::new(deref_tmp.clone()),
                    rhs: Box::new(rhs),
                },
                location,
            ),
        };

        // expr2: *tmp = *tmp op B
        let expr2 = self.expr_at(
            ExprKind::Assign {
                lhs: Box::new(deref_tmp),
                rhs: Box::new(bin_expr),
            },
            location,
        );

        Ok(self.expr_at(
            ExprKind::StmtExpr(vec![
                self.stmt_at(StmtKind::Decl(var_idx), location),
                self.stmt_at(StmtKind::Expr(expr1), location),
                self.stmt_at(StmtKind::Expr(expr2), location),
            ]),
            location,
        ))
    }

    fn is_lvalue(&self, expr: &Expr) -> bool {
        match &expr.kind {
            ExprKind::Var { .. } | ExprKind::Deref(_) | ExprKind::Member { .. } => true,
            ExprKind::Comma { rhs, .. } => self.is_lvalue(rhs),
            ExprKind::Unary {
                op: UnaryOp::Real | UnaryOp::Imag,
                ..
            } => true,
            _ => false,
        }
    }

    fn parse_binary_ops<F>(
        &mut self,
        mut parse_next: F,
        operators: &[(Punct, BinaryOp, bool)],
    ) -> CompileResult<Expr>
    where
        F: FnMut(&mut Self) -> CompileResult<Expr>,
    {
        let mut expr = parse_next(self)?;

        loop {
            let mut matched = false;
            for &(punct, op, swap) in operators {
                if self.consume_punct(punct) {
                    let location = self.last_location();
                    let rhs = parse_next(self)?;
                    expr = if swap {
                        self.expr_at(
                            ExprKind::Binary {
                                op,
                                lhs: Box::new(rhs),
                                rhs: Box::new(expr),
                            },
                            location,
                        )
                    } else {
                        self.expr_at(
                            ExprKind::Binary {
                                op,
                                lhs: Box::new(expr),
                                rhs: Box::new(rhs),
                            },
                            location,
                        )
                    };
                    matched = true;
                    break;
                }
            }
            if !matched {
                break;
            }
        }

        Ok(expr)
    }

    fn parse_logor(&mut self) -> CompileResult<Expr> {
        self.parse_binary_ops(
            Self::parse_logand,
            &[(Punct::LogOr, BinaryOp::LogOr, false)],
        )
    }

    fn parse_conditional(&mut self) -> CompileResult<Expr> {
        let mut cond = self.parse_logor()?;

        if !self.consume_punct(Punct::Question) {
            return Ok(cond);
        }

        let location = self.last_location();
        if self.check_punct(Punct::Colon) {
            // GNU extension: a ?: b -> tmp = a, tmp ? tmp : b
            self.add_type_expr(&mut cond)?;
            let cond_ty = cond
                .ty
                .clone()
                .ok_or_else(|| self.err_at(location, "conditional has no type"))?;
            let var_idx = self.new_lvar(String::new(), cond_ty);
            let tmp_var = self.expr_at(
                ExprKind::Var {
                    idx: var_idx,
                    is_local: true,
                },
                location,
            );
            let assign = self.expr_at(
                ExprKind::Assign {
                    lhs: Box::new(tmp_var.clone()),
                    rhs: Box::new(cond),
                },
                location,
            );
            self.expect_punct(Punct::Colon)?;
            let els = self.parse_conditional()?;
            let rhs = self.expr_at(
                ExprKind::Cond {
                    cond: Box::new(tmp_var.clone()),
                    then: Box::new(tmp_var),
                    els: Box::new(els),
                },
                location,
            );
            return Ok(self.expr_at(
                ExprKind::Comma {
                    lhs: Box::new(assign),
                    rhs: Box::new(rhs),
                },
                location,
            ));
        }
        let then = self.parse_expr()?;
        self.expect_punct(Punct::Colon)?;
        let els = self.parse_conditional()?;

        Ok(self.expr_at(
            ExprKind::Cond {
                cond: Box::new(cond),
                then: Box::new(then),
                els: Box::new(els),
            },
            location,
        ))
    }

    fn parse_logand(&mut self) -> CompileResult<Expr> {
        self.parse_binary_ops(
            Self::parse_bitor,
            &[(Punct::LogAnd, BinaryOp::LogAnd, false)],
        )
    }

    fn parse_bitor(&mut self) -> CompileResult<Expr> {
        self.parse_binary_ops(Self::parse_bitxor, &[(Punct::Pipe, BinaryOp::BitOr, false)])
    }

    fn parse_bitxor(&mut self) -> CompileResult<Expr> {
        self.parse_binary_ops(
            Self::parse_bitand,
            &[(Punct::Caret, BinaryOp::BitXor, false)],
        )
    }

    fn parse_bitand(&mut self) -> CompileResult<Expr> {
        self.parse_binary_ops(
            Self::parse_equality,
            &[(Punct::Amp, BinaryOp::BitAnd, false)],
        )
    }

    fn parse_equality(&mut self) -> CompileResult<Expr> {
        self.parse_binary_ops(
            Self::parse_relational,
            &[
                (Punct::EqEq, BinaryOp::Eq, false),
                (Punct::NotEq, BinaryOp::Ne, false),
            ],
        )
    }

    fn parse_relational(&mut self) -> CompileResult<Expr> {
        self.parse_binary_ops(
            Self::parse_shift,
            &[
                (Punct::Less, BinaryOp::Lt, false),
                (Punct::LessEq, BinaryOp::Le, false),
                (Punct::Greater, BinaryOp::Lt, true),
                (Punct::GreaterEq, BinaryOp::Le, true),
            ],
        )
    }

    fn parse_shift(&mut self) -> CompileResult<Expr> {
        self.parse_binary_ops(
            Self::parse_add,
            &[
                (Punct::Shl, BinaryOp::Shl, false),
                (Punct::Shr, BinaryOp::Shr, false),
            ],
        )
    }

    fn parse_add(&mut self) -> CompileResult<Expr> {
        let mut expr = self.parse_mul()?;

        loop {
            let op = if self.consume_punct(Punct::Plus) {
                Some(BinaryOp::Add)
            } else if self.consume_punct(Punct::Minus) {
                Some(BinaryOp::Sub)
            } else {
                None
            };

            let Some(op) = op else { break };
            let location = self.last_location();
            let rhs = self.parse_mul()?;
            expr = match op {
                BinaryOp::Add => self.new_add(expr, rhs, location)?,
                BinaryOp::Sub => self.new_sub(expr, rhs, location)?,
                _ => unreachable!("parse_add only handles +/-"),
            };
        }

        Ok(expr)
    }

    fn parse_mul(&mut self) -> CompileResult<Expr> {
        self.parse_binary_ops(
            Self::parse_cast,
            &[
                (Punct::Star, BinaryOp::Mul, false),
                (Punct::Slash, BinaryOp::Div, false),
                (Punct::Mod, BinaryOp::Mod, false),
            ],
        )
    }

    fn parse_cast(&mut self) -> CompileResult<Expr> {
        if self.check_punct(Punct::LParen) && self.is_typename_token(self.peek_n(1)) {
            let start_pos = self.pos;
            self.expect_punct(Punct::LParen)?;
            let location = self.last_location();
            let ty = self.parse_typename()?;
            self.expect_punct(Punct::RParen)?;

            // compound literal
            if self.check_punct(Punct::LBrace) {
                self.pos = start_pos;
                return self.parse_unary();
            }

            // type cast
            let expr = self.parse_cast()?;
            return Ok(self.expr_at(
                ExprKind::Cast {
                    expr: Box::new(expr),
                    ty,
                },
                location,
            ));
        }

        self.parse_unary()
    }

    fn parse_unary(&mut self) -> CompileResult<Expr> {
        if self.consume_punct(Punct::Plus) {
            return self.parse_cast();
        }
        if self.consume_punct(Punct::Minus) {
            let location = self.last_location();
            let expr = self.parse_cast()?;
            return Ok(self.expr_at(
                ExprKind::Unary {
                    op: UnaryOp::Neg,
                    expr: Box::new(expr),
                },
                location,
            ));
        }

        if self.consume_punct(Punct::Amp) {
            let location = self.last_location();
            let mut expr = self.parse_cast()?;
            self.add_type_expr(&mut expr)?;
            if let ExprKind::Member { member, .. } = &expr.kind
                && member.is_bitfield
            {
                self.bail_at(location, "cannot take address of bitfield")?;
            }
            return Ok(self.expr_at(ExprKind::Addr(Box::new(expr)), location));
        }

        if self.consume_punct(Punct::Star) {
            let location = self.last_location();
            let mut expr = self.parse_cast()?;
            self.add_type_expr(&mut expr)?;
            if matches!(expr.ty, Some(Type::Func { .. })) {
                return Ok(expr);
            }
            return Ok(self.expr_at(ExprKind::Deref(Box::new(expr)), location));
        }

        if self.consume_punct(Punct::Not) {
            let location = self.last_location();
            let expr = self.parse_cast()?;
            return Ok(self.expr_at(
                ExprKind::Unary {
                    op: UnaryOp::Not,
                    expr: Box::new(expr),
                },
                location,
            ));
        }

        if self.consume_punct(Punct::BitNot) {
            let location = self.last_location();
            let expr = self.parse_cast()?;
            return Ok(self.expr_at(
                ExprKind::Unary {
                    op: UnaryOp::BitNot,
                    expr: Box::new(expr),
                },
                location,
            ));
        }

        // __real__ and __imag__ for complex numbers
        if self.consume_keyword(Keyword::Real) {
            let location = self.last_location();
            let expr = self.parse_cast()?;
            return Ok(self.expr_at(
                ExprKind::Unary {
                    op: UnaryOp::Real,
                    expr: Box::new(expr),
                },
                location,
            ));
        }

        if self.consume_keyword(Keyword::Imag) {
            let location = self.last_location();
            let expr = self.parse_cast()?;
            return Ok(self.expr_at(
                ExprKind::Unary {
                    op: UnaryOp::Imag,
                    expr: Box::new(expr),
                },
                location,
            ));
        }

        // ++i => i+=1
        if self.consume_punct(Punct::Inc) {
            let location = self.last_location();
            let expr = self.parse_unary()?;
            let one = self.expr_at(
                ExprKind::Num {
                    value: 1,
                    fval: 0.0,
                },
                location,
            );
            return self.to_assign(expr, one, BinaryOp::Add, location);
        }

        // --i => i-=1
        if self.consume_punct(Punct::Dec) {
            let location = self.last_location();
            let expr = self.parse_unary()?;
            let one = self.expr_at(
                ExprKind::Num {
                    value: 1,
                    fval: 0.0,
                },
                location,
            );
            return self.to_assign(expr, one, BinaryOp::Sub, location);
        }

        if self.consume_punct(Punct::LogAnd) {
            let label_token = self.expect_ident_token()?;
            let label = match label_token.kind {
                TokenKind::Ident(name) => name,
                _ => unreachable!(),
            };
            self.fn_gotos.push((label.clone(), label_token.location));
            return Ok(self.expr_at(ExprKind::LabelVal { label }, label_token.location));
        }

        self.parse_postfix()
    }

    // Convert A++ to `(typeof A)((A += 1) - 1)`
    // Convert A-- to `(typeof A)((A -= 1) + 1)`
    fn new_inc_dec(
        &mut self,
        mut node: Expr,
        addend: i64,
        location: SourceLocation,
    ) -> CompileResult<Expr> {
        self.add_type_expr(&mut node)?;
        let node_ty = node
            .ty
            .clone()
            .ok_or_else(|| self.err_at(location, "node has no type"))?;

        // Convert to assignment using the binary add expression
        // This transforms: (i + 1) or (i + (-1))
        // Into: (tmp = &i, *tmp = *tmp + addend)
        let assign_result = self.to_assign(
            node,
            self.expr_at(
                ExprKind::Num {
                    value: addend,
                    fval: 0.0,
                },
                location,
            ),
            BinaryOp::Add,
            location,
        )?;

        // Subtract addend from assignment result to get original value
        // For i++: (i + 1) - 1 = i
        // For i--: (i - 1) - (-1) = i
        let neg_addend = self.expr_at(
            ExprKind::Num {
                value: -addend,
                fval: 0.0,
            },
            location,
        );
        let result = self.new_add(assign_result, neg_addend, location)?;

        // Cast back to original type
        Ok(self.cast_expr(result, node_ty))
    }

    fn parse_postfix(&mut self) -> CompileResult<Expr> {
        // Compound literal: (typename) { initializer }
        if self.check_punct(Punct::LParen) && self.is_typename_token(self.peek_n(1)) {
            let location = self.peek().location;
            self.expect_punct(Punct::LParen)?;
            let ty = self.parse_typename()?;
            self.expect_punct(Punct::RParen)?;

            if self.scopes.len() == 1 {
                // Global scope
                let var_idx = self.new_anon_gvar(ty);
                self.globals[var_idx].global_kind = GlobalKind::CompoundLiteral;
                self.parse_gvar_initializer(var_idx, location)?;
                return Ok(self.expr_at(
                    ExprKind::Var {
                        idx: var_idx,
                        is_local: false,
                    },
                    location,
                ));
            }

            // Local scope
            let var_idx = self.new_lvar(String::new(), ty.clone());
            let lhs = self.parse_lvar_initializer(var_idx, true, ty, location, None)?;
            let rhs = self.expr_at(
                ExprKind::Var {
                    idx: var_idx,
                    is_local: true,
                },
                location,
            );
            return Ok(self.expr_at(
                ExprKind::Comma {
                    lhs: Box::new(lhs),
                    rhs: Box::new(rhs),
                },
                location,
            ));
        }

        let mut expr = self.parse_primary()?;

        // Handle array subscript: x[y] is equivalent to *(x+y)
        loop {
            if self.consume_punct(Punct::LParen) {
                let location = self.last_location();
                expr = self.parse_funcall(expr, location)?;
                continue;
            }

            if self.consume_punct(Punct::LBracket) {
                let location = self.last_location();
                let index = self.parse_expr()?;
                self.expect_punct(Punct::RBracket)?;

                // Transform x[y] to *(x+y)
                let add_expr = self.new_add(expr, index, location)?;
                expr = self.expr_at(ExprKind::Deref(Box::new(add_expr)), location);
                continue;
            }

            if self.consume_punct(Punct::Dot) {
                let name_token = self.expect_ident_token()?;
                expr = self.struct_ref(expr, name_token)?;
                continue;
            }

            if self.consume_punct(Punct::Arrow) {
                let location = self.last_location();
                let name_token = self.expect_ident_token()?;
                // x->y is short for (*x).y
                expr = self.expr_at(ExprKind::Deref(Box::new(expr)), location);
                expr = self.struct_ref(expr, name_token)?;
                continue;
            }

            if self.consume_punct(Punct::Inc) {
                let location = self.last_location();
                expr = self.new_inc_dec(expr, 1, location)?;
                continue;
            }

            if self.consume_punct(Punct::Dec) {
                let location = self.last_location();
                expr = self.new_inc_dec(expr, -1, location)?;
                continue;
            }

            break;
        }

        Ok(expr)
    }

    /// Find a struct member by name, recursively searching anonymous struct/union members.
    /// Returns Some(member) if found, None otherwise.
    fn get_struct_member(members: &[Member], name: &str) -> Option<Member> {
        for member in members {
            // Check if this is the member we're looking for
            if member.name == name {
                return Some(member.clone());
            }

            // Recursively search anonymous struct/union members
            if member.name.is_empty()
                && matches!(member.ty, Type::Struct { .. } | Type::Union { .. })
            {
                let inner_members = match &member.ty {
                    Type::Struct { members, .. } | Type::Union { members, .. } => members,
                    _ => continue,
                };
                if Self::get_struct_member(inner_members, name).is_some() {
                    return Some(member.clone());
                }
            }
        }
        None
    }

    fn struct_ref(&self, mut lhs: Expr, name_token: Token) -> CompileResult<Expr> {
        self.add_type_expr(&mut lhs)?;
        let members = match self.canonicalize_type(lhs.ty.clone().unwrap_or(Type::Int)) {
            Type::Struct { members, .. } | Type::Union { members, .. } => members,
            _ => self.bail_at(lhs.location, "not a struct nor a union")?,
        };
        let name = match name_token.kind {
            TokenKind::Ident(name) => name,
            _ => self.bail_at(name_token.location, "member name expected")?,
        };

        // Create a chain of member accesses for anonymous struct/union members.
        // For example, if we have: struct { struct { int a; }; } x;
        // And we access x.a, we need to create: x.<anonymous>. a
        let mut node = lhs;
        let mut current_members = members;

        loop {
            let member = Self::get_struct_member(&current_members, &name)
                .ok_or_else(|| self.err_at(name_token.location, "no such member"))?;

            node = self.expr_at(
                ExprKind::Member {
                    lhs: Box::new(node),
                    member: member.clone(),
                },
                name_token.location,
            );

            // If this member has a name, we're done (we found the target member)
            if !member.name.is_empty() {
                break;
            }

            // Otherwise, this is an anonymous member, continue searching within it
            current_members = match &member.ty {
                Type::Struct { members, .. } | Type::Union { members, .. } => members.clone(),
                _ => break,
            };
        }

        Ok(node)
    }

    fn parse_primary(&mut self) -> CompileResult<Expr> {
        let token = self.peek().clone();
        match token.kind {
            TokenKind::Punct(Punct::LParen)
                if matches!(self.peek_n(1).kind, TokenKind::Punct(Punct::LBrace)) =>
            {
                let location = token.location;
                self.pos += 2;
                let stmts = self.parse_block_items()?;
                self.expect_punct(Punct::RParen)?;
                Ok(self.expr_at(ExprKind::StmtExpr(stmts), location))
            }
            TokenKind::Punct(Punct::LParen) => {
                self.pos += 1;
                let expr = self.parse_expr()?;
                self.expect_punct(Punct::RParen)?;
                Ok(expr)
            }
            TokenKind::Keyword(Keyword::Sizeof) => {
                let location = token.location;
                if matches!(self.peek_n(1).kind, TokenKind::Punct(Punct::LParen))
                    && self.is_typename_token(self.peek_n(2))
                {
                    self.pos += 1;
                    self.expect_punct(Punct::LParen)?;
                    let mut ty = self.parse_typename()?;
                    self.expect_punct(Punct::RParen)?;

                    // Check if VLA
                    if let Type::Vla { size_var, .. } = &ty {
                        if let Some(idx) = size_var {
                            return Ok(self.expr_at(
                                ExprKind::Var {
                                    idx: *idx,
                                    is_local: true,
                                },
                                location,
                            ));
                        }

                        // Need to compute VLA size
                        let lhs = self.compute_vla_size(&mut ty, location)?;
                        let Type::Vla {
                            size_var: Some(idx),
                            ..
                        } = ty
                        else {
                            unreachable!()
                        };
                        let rhs = self.expr_at(
                            ExprKind::Var {
                                idx,
                                is_local: true,
                            },
                            location,
                        );
                        return Ok(self.expr_at(
                            ExprKind::Comma {
                                lhs: Box::new(lhs),
                                rhs: Box::new(rhs),
                            },
                            location,
                        ));
                    }
                    return Ok(self.new_ulong_expr(ty.size(), location));
                }

                self.pos += 1;
                let mut expr = self.parse_unary()?;
                self.add_type_expr(&mut expr)?;
                let ty = expr
                    .ty
                    .as_ref()
                    .ok_or_else(|| CompileError::at("expression has no type", location))?;
                // Check if VLA
                if let Type::Vla {
                    size_var: Some(idx),
                    ..
                } = ty
                {
                    return Ok(self.expr_at(
                        ExprKind::Var {
                            idx: *idx,
                            is_local: true,
                        },
                        location,
                    ));
                }
                let size = ty.size();
                Ok(self.new_ulong_expr(size, location))
            }
            TokenKind::Keyword(Keyword::Alignof) => {
                let location = token.location;
                self.pos += 1;

                // Check if it's _Alignof(typename)
                if matches!(self.peek().kind, TokenKind::Punct(Punct::LParen))
                    && self.is_typename_token(self.peek_n(1))
                {
                    self.expect_punct(Punct::LParen)?;
                    let ty = self.parse_typename()?;
                    self.expect_punct(Punct::RParen)?;
                    return Ok(self.new_ulong_expr(ty.align(), location));
                }

                // Otherwise, parse as _Alignof unary (GNU extension)
                let mut expr = self.parse_unary()?;
                self.add_type_expr(&mut expr)?;
                let align = expr.ty.as_ref().map(|ty| ty.align()).unwrap_or(1);
                Ok(self.new_ulong_expr(align, location))
            }
            TokenKind::Keyword(Keyword::Generic) => {
                self.pos += 1;
                self.parse_generic_selection()
            }
            TokenKind::Keyword(Keyword::True) | TokenKind::Keyword(Keyword::False) => {
                let value = if matches!(token.kind, TokenKind::Keyword(Keyword::True)) {
                    1
                } else {
                    0
                };
                let mut expr = self.expr_at(ExprKind::Num { value, fval: 0.0 }, token.location);
                expr.ty = Some(Type::Bool);
                self.pos += 1;
                Ok(expr)
            }
            TokenKind::Keyword(Keyword::NullPtr) => {
                let mut expr = self.expr_at(
                    ExprKind::Num {
                        value: 0,
                        fval: 0.0,
                    },
                    token.location,
                );
                expr.ty = Some(Type::NullPtr);
                self.pos += 1;
                Ok(expr)
            }
            TokenKind::Ident(ref name) if name == "__builtin_reg_class" => {
                let location = token.location;
                self.pos += 1;
                self.expect_punct(Punct::LParen)?;
                let ty = self.parse_typename()?;
                self.expect_punct(Punct::RParen)?;
                let value = if ty.is_integer() || matches!(ty, Type::Ptr(_)) {
                    0
                } else if matches!(ty, Type::Float | Type::Double) {
                    1
                } else {
                    2
                };
                let mut expr = self.expr_at(ExprKind::Num { value, fval: 0.0 }, location);
                expr.ty = Some(Type::Int);
                Ok(expr)
            }
            TokenKind::Ident(ref name) if name == "__builtin_types_compatible_p" => {
                let location = token.location;
                self.pos += 1;
                self.expect_punct(Punct::LParen)?;
                let ty1 = self.parse_typename()?;
                self.expect_punct(Punct::Comma)?;
                let ty2 = self.parse_typename()?;
                self.expect_punct(Punct::RParen)?;
                let value = if is_compatible(&ty1, &ty2) { 1 } else { 0 };
                let mut expr = self.expr_at(ExprKind::Num { value, fval: 0.0 }, location);
                expr.ty = Some(Type::Int);
                Ok(expr)
            }
            TokenKind::Ident(ref name) if name == "__builtin_compare_and_swap" => {
                let location = token.location;
                self.pos += 1;
                self.expect_punct(Punct::LParen)?;
                let addr = Box::new(self.parse_assign()?);
                self.expect_punct(Punct::Comma)?;
                let old = Box::new(self.parse_assign()?);
                self.expect_punct(Punct::Comma)?;
                let new = Box::new(self.parse_assign()?);
                self.expect_punct(Punct::RParen)?;
                Ok(self.expr_at(ExprKind::Cas { addr, old, new }, location))
            }
            TokenKind::Ident(ref name)
                if matches!(
                    name.as_str(),
                    "__builtin_add_overflow" | "__builtin_sub_overflow" | "__builtin_mul_overflow"
                ) =>
            {
                let op = match name.as_str() {
                    "__builtin_add_overflow" => BinaryOp::Add,
                    "__builtin_sub_overflow" => BinaryOp::Sub,
                    _ => BinaryOp::Mul,
                };
                let location = token.location;
                self.pos += 1;
                self.expect_punct(Punct::LParen)?;
                let lhs = Box::new(self.parse_assign()?);
                self.expect_punct(Punct::Comma)?;
                let rhs = Box::new(self.parse_assign()?);
                self.expect_punct(Punct::Comma)?;
                let result_ptr = self.parse_assign()?;
                self.expect_punct(Punct::RParen)?;
                let target =
                    Box::new(self.expr_at(ExprKind::Deref(Box::new(result_ptr)), location));
                let value = self.expr_at(ExprKind::Binary { op, lhs, rhs }, location);
                let assign = self.expr_at(
                    ExprKind::Assign {
                        lhs: target,
                        rhs: Box::new(value),
                    },
                    location,
                );
                let zero = self.expr_at(
                    ExprKind::Num {
                        value: 0,
                        fval: 0.0,
                    },
                    location,
                );
                let mut expr = self.expr_at(
                    ExprKind::Comma {
                        lhs: Box::new(assign),
                        rhs: Box::new(zero),
                    },
                    location,
                );
                expr.ty = Some(Type::Int);
                Ok(expr)
            }
            TokenKind::Ident(ref name) if name == "__builtin_complex" => {
                let location = token.location;
                self.pos += 1;
                self.expect_punct(Punct::LParen)?;
                let re = self.parse_assign()?;
                self.expect_punct(Punct::Comma)?;
                let im = self.parse_assign()?;
                self.expect_punct(Punct::RParen)?;
                let complex_ty = match re.ty {
                    Some(Type::Float) => Type::FloatComplex,
                    Some(Type::LDouble) => Type::LDoubleComplex,
                    _ => Type::DoubleComplex,
                };
                let discard = self.expr_at(
                    ExprKind::Cast {
                        expr: Box::new(re),
                        ty: complex_ty.clone(),
                    },
                    location,
                );
                let mut expr = self.expr_at(
                    ExprKind::Comma {
                        lhs: Box::new(im),
                        rhs: Box::new(discard),
                    },
                    location,
                );
                expr.ty = Some(complex_ty);
                Ok(expr)
            }
            TokenKind::Ident(ref name) if name == "__builtin_bit_cast" => {
                let location = token.location;
                self.pos += 1;
                self.expect_punct(Punct::LParen)?;
                let ty = self.parse_typename()?;
                self.expect_punct(Punct::Comma)?;
                let inner = self.parse_assign()?;
                self.expect_punct(Punct::RParen)?;
                let mut expr = self.expr_at(
                    ExprKind::Cast {
                        expr: Box::new(inner),
                        ty: ty.clone(),
                    },
                    location,
                );
                expr.ty = Some(ty);
                Ok(expr)
            }
            TokenKind::Ident(ref name) if name == "__builtin_addressof" => {
                let location = token.location;
                self.pos += 1;
                self.expect_punct(Punct::LParen)?;
                let inner = self.parse_assign()?;
                self.expect_punct(Punct::RParen)?;
                Ok(self.expr_at(ExprKind::Addr(Box::new(inner)), location))
            }
            TokenKind::Ident(ref name) if name == "__builtin_atomic_exchange" => {
                let location = token.location;
                self.pos += 1;
                self.expect_punct(Punct::LParen)?;
                let addr = Box::new(self.parse_assign()?);
                self.expect_punct(Punct::Comma)?;
                let val = Box::new(self.parse_assign()?);
                self.expect_punct(Punct::RParen)?;
                Ok(self.expr_at(ExprKind::Exch { addr, val }, location))
            }
            TokenKind::Ident(ref name) if name == "__builtin_shufflevector" => {
                // (vec1, vec2, idx...) -> a vector of vec1's type; the shuffle
                // indices don't affect parsing/type-checking, only real codegen
                // (which doesn't exist yet for this frontend), so every operand
                // is chained through a Comma so it's still parsed and type-checked.
                let location = token.location;
                self.pos += 1;
                self.expect_punct(Punct::LParen)?;
                let first = self.parse_assign()?;
                let result_ty = first.ty.clone();
                let mut chain = first;
                while self.consume_punct(Punct::Comma) {
                    let next = self.parse_assign()?;
                    chain = self.expr_at(
                        ExprKind::Comma {
                            lhs: Box::new(chain),
                            rhs: Box::new(next),
                        },
                        location,
                    );
                }
                self.expect_punct(Punct::RParen)?;
                chain.ty = result_ty;
                Ok(chain)
            }
            TokenKind::Ident(ref name) => {
                let name = name.clone();
                let expr = if let Some(scope) = self.find_var_scope(&name) {
                    if let Some(idx) = scope.idx {
                        let obj = if scope.is_local {
                            &self.locals[idx]
                        } else {
                            &self.globals[idx]
                        };
                        if obj.is_deprecated {
                            self.warn_at(token.location, format!("'{}' is deprecated", obj.name));
                        }
                        self.expr_at(
                            ExprKind::Var {
                                idx,
                                is_local: scope.is_local,
                            },
                            token.location,
                        )
                    } else if let Some(val) = scope.enum_val {
                        let mut expr = self.expr_at(
                            ExprKind::Num {
                                value: val,
                                fval: 0.0,
                            },
                            token.location,
                        );
                        let enum_ty = scope.enum_ty.clone().unwrap_or(Type::Int);
                        expr.ty = Some(enum_ty);
                        expr
                    } else {
                        self.bail_at(token.location, "undefined variable")?
                    }
                } else if let Some((idx, obj)) = self.find_global_idx(&name) {
                    let obj_name = obj.name.clone();
                    let is_function = obj.is_function;
                    let is_deprecated = obj.is_deprecated;
                    if is_function {
                        self.record_function_ref(&obj_name);
                    }
                    if is_deprecated {
                        self.warn_at(token.location, format!("'{}' is deprecated", obj_name));
                    }
                    self.expr_at(
                        ExprKind::Var {
                            idx,
                            is_local: false,
                        },
                        token.location,
                    )
                } else if self.in_func_params {
                    // In function parameter declarations, undefined identifiers in array
                    // bounds (like VLA sizes referencing other parameters) are allowed.
                    // The array will decay to a pointer anyway, so use a placeholder.
                    let mut expr = self.expr_at(
                        ExprKind::Num {
                            value: 1,
                            fval: 0.0,
                        },
                        token.location,
                    );
                    expr.ty = Some(Type::Int);
                    expr
                } else {
                    if matches!(self.peek_n(1).kind, TokenKind::Punct(Punct::LParen)) {
                        self.bail_at(token.location, "implicit declaration of a function")?;
                    }
                    self.bail_at(token.location, "undefined variable")?
                };
                self.pos += 1;
                Ok(expr)
            }
            TokenKind::Str { bytes, ty } => {
                let idx = self.new_string_literal(bytes, ty);
                self.pos += 1;
                Ok(self.expr_at(
                    ExprKind::Var {
                        idx,
                        is_local: false,
                    },
                    token.location,
                ))
            }
            TokenKind::Num {
                value,
                fval,
                ref ty,
                ldouble,
            } => {
                let raw_text = matches!(ty, Type::BitInt { .. }).then(|| token.text());
                self.pos += 1;
                let expr_kind = if let Some(raw) = raw_text {
                    ExprKind::BigIntLiteral { raw }
                } else if let Some(value) = ldouble {
                    ExprKind::LDoubleLiteral { value }
                } else if ty.is_flonum() || ty.is_complex() {
                    ExprKind::Num { value: 0, fval }
                } else {
                    ExprKind::Num { value, fval: 0.0 }
                };
                let mut expr = self.expr_at(expr_kind, token.location);
                expr.ty = Some(ty.clone());
                Ok(expr)
            }
            _ => self.bail_expected("a primary expression"),
        }
    }

    fn parse_typedef(&mut self, basety: Type) -> CompileResult<()> {
        let mut first = true;
        while !self.consume_punct(Punct::Semicolon) {
            if !first {
                self.expect_punct(Punct::Comma)?;
            }
            first = false;
            let (ty, name_token) = self.parse_declarator(basety.clone())?;
            let ty = self.parse_type_attributes(ty)?;
            let name = match name_token.kind {
                TokenKind::Ident(name) => name,
                _ => self.bail_at(name_token.location, "typedef name omitted")?,
            };
            self.push_scope_typedef(name, ty);
        }
        Ok(())
    }

    /// Parse `__attribute__((...))` after a declarator, applying type-affecting
    /// attributes (currently just GNU `vector_size`) to `ty`; anything else is
    /// consumed and discarded, same as parse_gnu_attributes.
    fn parse_type_attributes(&mut self, mut ty: Type) -> CompileResult<Type> {
        while self.consume_keyword(Keyword::Attribute) {
            self.expect_punct(Punct::LParen)?;
            self.expect_punct(Punct::LParen)?;

            let mut first = true;
            while !self.consume_punct(Punct::RParen) {
                if !first {
                    self.expect_punct(Punct::Comma)?;
                }
                first = false;

                let token = self.peek().clone();
                let name = match token.kind {
                    TokenKind::Ident(name) => {
                        self.pos += 1;
                        name
                    }
                    TokenKind::Keyword(keyword) => {
                        self.pos += 1;
                        keyword.to_string()
                    }
                    _ => self.bail_here("invalid attribute")?,
                };

                if name == "vector_size" {
                    self.expect_punct(Punct::LParen)?;
                    let bytes = self.const_expr()?;
                    self.expect_punct(Punct::RParen)?;
                    let elem_size = ty.size().max(1);
                    if bytes < 1 || bytes % elem_size != 0 {
                        self.bail_here(
                            "vector_size must be a positive multiple of the base type's size",
                        )?;
                    }
                    ty = Type::Vector {
                        base: Box::new(ty),
                        len: (bytes / elem_size) as i32,
                    };
                    continue;
                }

                if self.consume_punct(Punct::LParen) {
                    let mut depth = 1i32;
                    while depth > 0 {
                        if matches!(self.peek().kind, TokenKind::Eof) {
                            self.bail_here("unterminated attribute")?;
                        }
                        if self.consume_punct(Punct::LParen) {
                            depth += 1;
                            continue;
                        }
                        if self.consume_punct(Punct::RParen) {
                            depth -= 1;
                            continue;
                        }
                        self.pos += 1;
                    }
                }
            }

            self.expect_punct(Punct::RParen)?;
        }

        Ok(ty)
    }

    /// Parse _Static_assert(constant-expression, string-literal);
    /// C23 also allows _Static_assert(constant-expression); without the message
    fn parse_static_assert(&mut self) -> CompileResult<()> {
        let location = self.last_location();
        self.expect_punct(Punct::LParen)?;

        // Parse constant expression
        let value = self.const_expr()?;

        // Check for optional message (C11 requires it, C23 makes it optional)
        let message = if self.consume_punct(Punct::Comma) {
            // Parse string literal
            match &self.peek().kind {
                TokenKind::Str { bytes, .. } => {
                    let msg = String::from_utf8_lossy(bytes).to_string();
                    self.pos += 1;
                    Some(msg)
                }
                _ => {
                    return self.bail_here("expected string literal in _Static_assert");
                }
            }
        } else {
            None
        };

        self.expect_punct(Punct::RParen)?;
        self.expect_punct(Punct::Semicolon)?;

        // Check assertion at compile time
        if value == 0 {
            let msg = message.unwrap_or_else(|| "static assertion failed".to_string());
            return Err(CompileError::at(msg, location));
        }

        Ok(())
    }

    fn get_common_type(&self, ty1: &Type, ty2: &Type) -> Type {
        if matches!(ty1, Type::Vector { .. }) {
            return ty1.clone();
        }
        if matches!(ty2, Type::Vector { .. }) {
            return ty2.clone();
        }

        if let Some(base) = ty1.base() {
            return Type::Ptr(Box::new(base.clone()));
        }

        if matches!(ty1, Type::Func { .. }) {
            return Type::Ptr(Box::new(ty1.clone()));
        }
        if matches!(ty2, Type::Func { .. }) {
            return Type::Ptr(Box::new(ty2.clone()));
        }

        if matches!(ty1, Type::NullPtr) && ty2.base().is_some() {
            return ty2.clone();
        }
        if matches!(ty2, Type::NullPtr) && ty1.base().is_some() {
            return ty1.clone();
        }

        // Handle complex types
        if matches!(ty1, Type::LDoubleComplex) || matches!(ty2, Type::LDoubleComplex) {
            return Type::LDoubleComplex;
        }
        if matches!(ty1, Type::DoubleComplex) || matches!(ty2, Type::DoubleComplex) {
            return Type::DoubleComplex;
        }
        if matches!(ty1, Type::FloatComplex) || matches!(ty2, Type::FloatComplex) {
            return Type::FloatComplex;
        }

        if matches!(ty1, Type::LDouble) || matches!(ty2, Type::LDouble) {
            return Type::LDouble;
        }
        if matches!(ty1, Type::Double) || matches!(ty2, Type::Double) {
            return Type::Double;
        }
        if matches!(ty1, Type::Float) || matches!(ty2, Type::Float) {
            return Type::Float;
        }

        let mut ty1 = ty1.clone();
        let mut ty2 = ty2.clone();

        if ty1.size() < 4 {
            ty1 = Type::Int;
        }
        if ty2.size() < 4 {
            ty2 = Type::Int;
        }

        if ty1.size() != ty2.size() {
            return if ty1.size() < ty2.size() { ty2 } else { ty1 };
        }

        if ty2.is_unsigned() { ty2 } else { ty1 }
    }

    fn dummy_expr(&self, location: SourceLocation) -> Expr {
        self.expr_at(
            ExprKind::Num {
                value: 0,
                fval: 0.0,
            },
            location,
        )
    }

    fn cast_expr(&self, expr: Expr, ty: Type) -> Expr {
        let location = expr.location;
        let mut cast = self.expr_at(
            ExprKind::Cast {
                expr: Box::new(expr),
                ty: ty.clone(),
            },
            location,
        );
        cast.ty = Some(ty);
        cast
    }

    fn cast_expr_in_place(&self, expr: &mut Expr, ty: Type) {
        let location = expr.location;
        let inner = mem::replace(expr, self.dummy_expr(location));
        *expr = self.cast_expr(inner, ty);
    }

    fn usual_arith_conv(&self, lhs: &mut Expr, rhs: &mut Expr) -> CompileResult<Type> {
        self.add_type_expr(lhs)?;
        self.add_type_expr(rhs)?;
        let ty = self.get_common_type(
            lhs.ty.as_ref().unwrap_or(&Type::Int),
            rhs.ty.as_ref().unwrap_or(&Type::Int),
        );
        self.cast_expr_in_place(lhs, ty.clone());
        self.cast_expr_in_place(rhs, ty.clone());
        Ok(ty)
    }

    fn consume_keyword(&mut self, kw: Keyword) -> bool {
        let token = self.peek().clone();
        if matches!(token.kind, TokenKind::Keyword(found) if found == kw) {
            self.pos += 1;
            true
        } else {
            false
        }
    }

    fn expect_ident_token(&mut self) -> CompileResult<Token> {
        let token = self.peek().clone();
        match token.kind {
            TokenKind::Ident(_) => {
                self.pos += 1;
                Ok(token)
            }
            _ => self.bail_expected("a variable name"),
        }
    }

    fn expect_punct(&mut self, punct: Punct) -> CompileResult<()> {
        let token = self.peek().clone();
        match token.kind {
            TokenKind::Punct(found) if found == punct => {
                self.pos += 1;
                Ok(())
            }
            _ => self.bail_expected(format!("'{punct}'")),
        }
    }

    fn consume_punct(&mut self, punct: Punct) -> bool {
        let token = self.peek().clone();
        if matches!(token.kind, TokenKind::Punct(found) if found == punct) {
            self.pos += 1;
            true
        } else {
            false
        }
    }

    fn check_punct(&self, punct: Punct) -> bool {
        matches!(self.peek().kind, TokenKind::Punct(found) if found == punct)
    }

    fn check_eof(&self) -> bool {
        matches!(self.peek().kind, TokenKind::Eof)
    }

    /// Check if we're at the end of an enum or initializer list.
    /// Returns true if current token is `}` OR if it's `,` followed by `}`.
    fn is_end(&self) -> bool {
        self.check_punct(Punct::RBrace)
            || (self.check_punct(Punct::Comma)
                && matches!(self.peek_n(1).kind, TokenKind::Punct(Punct::RBrace)))
    }

    /// Consume the end of an enum or initializer list.
    /// Consumes either `}` or `,}` and returns true if successful.
    fn consume_end(&mut self) -> bool {
        if self.check_punct(Punct::RBrace) {
            self.pos += 1;
            return true;
        }

        if self.check_punct(Punct::Comma)
            && matches!(self.peek_n(1).kind, TokenKind::Punct(Punct::RBrace))
        {
            self.pos += 2;
            return true;
        }

        false
    }

    fn peek(&self) -> &Token {
        self.tokens
            .get(self.pos)
            .unwrap_or_else(|| &self.tokens[self.tokens.len().saturating_sub(1)])
    }

    fn peek_n(&self, n: usize) -> &Token {
        self.tokens
            .get(self.pos + n)
            .unwrap_or_else(|| &self.tokens[self.tokens.len().saturating_sub(1)])
    }

    fn last_location(&self) -> SourceLocation {
        self.tokens
            .get(self.pos.saturating_sub(1))
            .map(|token| token.location)
            .unwrap_or_else(|| self.peek().location)
    }

    fn stmt_last(&self, kind: StmtKind) -> Stmt {
        self.stmt_at(kind, self.last_location())
    }

    fn stmt_at(&self, kind: StmtKind, location: SourceLocation) -> Stmt {
        Stmt {
            id: next_node_id(),
            kind,
            location,
        }
    }

    fn expr_at(&self, kind: ExprKind, location: SourceLocation) -> Expr {
        Expr {
            id: next_node_id(),
            kind,
            location,
            ty: None,
        }
    }

    /// Create a VlaPtr expression (used for VLA designators in assignments)
    fn new_vla_ptr(&self, idx: usize, is_local: bool, location: SourceLocation) -> Expr {
        self.expr_at(ExprKind::VlaPtr { idx, is_local }, location)
    }

    fn new_ulong_expr(&self, value: i64, location: SourceLocation) -> Expr {
        let mut expr = self.expr_at(ExprKind::Num { value, fval: 0.0 }, location);
        expr.ty = Some(Type::ULong);
        expr
    }

    fn err_at(&self, location: SourceLocation, message: impl Into<String>) -> CompileError {
        CompileError::at(message, location)
    }

    fn warn_at(&self, location: SourceLocation, message: impl Into<String>) {
        let message = message.into();
        eprintln!("warning: {message}");

        let Some(file) = crate::parse::lexer::get_input_file(location.file_no) else {
            return;
        };
        let line_text = Self::line_at_byte(&file.contents, location.byte);
        let width = location.line.to_string().len().max(3);

        eprintln!(
            "  --> {}:{}:{}",
            file.display_name, location.line, location.column
        );
        eprintln!("{:>width$} |", "", width = width);
        if let Some(text) = line_text {
            eprintln!("{:>width$} | {}", location.line, text, width = width);
            let caret_width =
                crate::parse::lexer::display_width(text, location.column.saturating_sub(1));
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

    fn err_expected(&self, expected: impl Into<String> + fmt::Display) -> CompileError {
        let found = self.token_desc(self.peek());
        self.err_at(
            self.peek().location,
            format!("expected {expected}, found {found}"),
        )
    }

    fn bail_at<T>(&self, location: SourceLocation, message: impl Into<String>) -> CompileResult<T> {
        Err(self.err_at(location, message))
    }

    fn bail_here<T>(&self, message: impl Into<String>) -> CompileResult<T> {
        self.bail_at(self.peek().location, message)
    }

    fn bail_expected<T>(&self, expected: impl Into<String> + fmt::Display) -> CompileResult<T> {
        Err(self.err_expected(expected))
    }

    fn token_desc(&self, token: &Token) -> String {
        match &token.kind {
            TokenKind::Keyword(kw) => format!("keyword '{kw:?}'"),
            TokenKind::Ident(name) => format!("identifier '{name}'"),
            TokenKind::Num { value, .. } => format!("number {value}"),
            TokenKind::PPNum => unreachable!("PPNum should be converted during preprocessing"),
            TokenKind::Str { .. } => "string literal".to_string(),
            TokenKind::Punct(punct) => format!("'{punct}'"),
            TokenKind::Eof => "end of file".to_string(),
        }
    }

    fn find_var_scope(&self, name: &str) -> Option<&VarScope> {
        for scope in self.scopes.iter().rev() {
            if let Some(var) = scope.vars.get(name) {
                return Some(var);
            }
        }
        None
    }

    fn find_global_idx(&self, name: &str) -> Option<(usize, &Obj)> {
        self.globals
            .iter()
            .enumerate()
            .rfind(|(_, obj)| obj.name == name)
    }

    fn record_function_ref(&mut self, name: &str) {
        if let Some(current_fn) = self.current_fn.clone() {
            self.fn_refs
                .entry(current_fn)
                .or_default()
                .push(name.to_string());
        } else {
            self.root_refs.insert(name.to_string());
        }
    }

    fn apply_inline_liveness(&mut self) {
        for obj in &mut self.globals {
            if obj.is_function && obj.is_definition {
                if let Some(refs) = self.fn_refs.get(&obj.name) {
                    obj.refs = refs.clone();
                }
                obj.is_root =
                    !(obj.is_static && obj.is_inline) || self.root_refs.contains(&obj.name);
            }
        }

        for idx in 0..self.globals.len() {
            if self.globals[idx].is_function
                && self.globals[idx].is_definition
                && self.globals[idx].is_root
            {
                self.mark_live(idx);
            }
        }
    }

    fn mark_live(&mut self, idx: usize) {
        if !self.globals[idx].is_function || self.globals[idx].is_live {
            return;
        }
        self.globals[idx].is_live = true;
        let refs = self.globals[idx].refs.clone();
        for name in refs {
            if let Some(next_idx) = self.find_function_idx(&name) {
                self.mark_live(next_idx);
            }
        }
    }

    /// Remove redundant tentative definitions.
    fn scan_globals(&mut self) {
        for i in 0..self.globals.len() {
            if !self.globals[i].is_tentative {
                continue;
            }

            // Find another definition of the same identifier.
            let mut found_real_def = false;
            for j in 0..self.globals.len() {
                if i != j
                    && self.globals[j].is_definition
                    && self.globals[i].name == self.globals[j].name
                {
                    found_real_def = true;
                    break;
                }
            }

            // If there's another definition, the tentative definition is redundant
            // Mark it as not a definition so it won't be emitted
            if found_real_def {
                self.globals[i].is_definition = false;
            }
        }
    }

    fn find_function_idx(&self, name: &str) -> Option<usize> {
        self.globals
            .iter()
            .enumerate()
            .rfind(|(_, obj)| obj.is_function && obj.is_definition && obj.name == name)
            .or_else(|| {
                self.globals
                    .iter()
                    .enumerate()
                    .rfind(|(_, obj)| obj.is_function && obj.name == name)
            })
            .map(|(idx, _)| idx)
    }

    fn find_global_by_name(&self, name: &str) -> Option<usize> {
        self.globals
            .iter()
            .enumerate()
            .rfind(|(_, obj)| obj.name == name)
            .map(|(idx, _)| idx)
    }

    fn find_typedef(&self, tok: &Token) -> Option<Type> {
        if let TokenKind::Ident(name) = &tok.kind {
            for scope in self.scopes.iter().rev() {
                if let Some(var) = scope.vars.get(name) {
                    if let Some(ty) = var.type_def.clone() {
                        return Some(self.canonicalize_type(ty));
                    }
                    return None;
                }
            }
        }
        None
    }

    fn canonicalize_type(&self, ty: Type) -> Type {
        match type_node_id(&ty).and_then(|id| self.type_registry.get(&id)) {
            Some(canonical) => canonical.clone(),
            None => ty,
        }
    }

    fn find_tag(&self, name: &str) -> Option<Type> {
        for scope in self.scopes.iter().rev() {
            if let Some(ty) = scope.tags.get(name) {
                return Some(ty.clone());
            }
        }
        None
    }

    fn find_tag_in_current_scope(&self, name: &str) -> Option<Type> {
        self.scopes
            .last()
            .and_then(|scope| scope.tags.get(name))
            .cloned()
    }

    fn update_tag_in_current_scope(&mut self, name: &str, ty: Type) -> bool {
        self.register_type(&ty);
        if let Some(scope) = self.scopes.last_mut()
            && let Some(existing) = scope.tags.get_mut(name)
        {
            *existing = ty;
            return true;
        }
        false
    }

    fn push_tag_scope(&mut self, name: String, ty: Type) {
        self.register_type(&ty);
        if let Some(scope) = self.scopes.last_mut() {
            scope.tags.insert(name, ty);
        }
    }

    fn register_type(&mut self, ty: &Type) {
        if let Some(id) = type_node_id(ty) {
            self.type_registry.insert(id, ty.clone());
        }
    }

    fn log_tag_decl(
        &mut self,
        name: String,
        location: SourceLocation,
        type_id: NodeId,
        is_definition: bool,
        kind: impl FnOnce(TagOccurrence) -> DeclKind,
    ) {
        let previous_decl = self.last_tag_decl.get(&type_id).copied();
        let id = next_node_id();
        self.tag_decls.push(Decl {
            id,
            name,
            location,
            kind: kind(TagOccurrence {
                type_id,
                is_definition,
                previous_decl,
            }),
        });
        self.last_tag_decl.insert(type_id, id);
    }

    fn new_lvar(&mut self, name: String, ty: Type) -> usize {
        let idx = self.locals.len();
        let align = ty.align() as i32;
        self.locals.push(Obj {
            id: next_node_id(),
            name,
            ty,
            location: self.last_location(),
            is_local: true,
            align,
            ..Default::default()
        });
        self.push_scope_var(idx, true);
        idx
    }

    fn new_gvar(&mut self, name: String, ty: Type) -> usize {
        let idx = self.globals.len();
        let align = ty.align() as i32;
        self.globals.push(Obj {
            id: next_node_id(),
            name,
            ty,
            location: self.last_location(),
            align,
            is_static: true,
            is_definition: true,
            ..Default::default()
        });
        self.push_scope_var(idx, false);
        idx
    }

    #[expect(
        clippy::too_many_arguments,
        reason = "mirrors the Obj/Type fields a C function declaration carries; grouping them into a struct would just move the same arity into a builder"
    )]
    fn new_function_decl(
        &mut self,
        name: String,
        ty: Type,
        is_static: bool,
        is_inline: bool,
        is_const_attr: bool,
        is_deprecated: bool,
        is_nodiscard: bool,
        is_maybe_unused: bool,
    ) {
        let location = self.last_location();

        // Extract params from function type
        let params = if let Type::Func { params, .. } = &ty {
            params
                .iter()
                .map(|(name, param_ty)| Obj {
                    id: next_node_id(),
                    name: name.clone(),
                    ty: param_ty.clone(),
                    location,
                    is_local: true,
                    ..Default::default()
                })
                .collect()
        } else {
            Vec::new()
        };

        self.globals.push(Obj {
            id: next_node_id(),
            name,
            ty,
            location,
            is_function: true,
            is_static,
            is_inline,
            is_root: !(is_static && is_inline),
            is_const_attr,
            is_deprecated,
            is_nodiscard,
            is_maybe_unused,
            params,
            ..Default::default()
        });
    }

    #[expect(
        clippy::too_many_arguments,
        reason = "mirrors the Obj fields a C function definition carries (body, locals, va_area, stack layout); grouping them into a struct would just move the same arity into a builder"
    )]
    fn new_function_def(
        &mut self,
        name: String,
        ty: Type,
        params: Vec<Obj>,
        body: Vec<Stmt>,
        locals: Vec<Obj>,
        va_area: Option<usize>,
        alloca_bottom: Option<usize>,
        stack_size: i32,
        is_static: bool,
        is_inline: bool,
        is_const_attr: bool,
        is_deprecated: bool,
        is_nodiscard: bool,
        is_maybe_unused: bool,
    ) {
        // Check if function already exists (as a declaration)
        if let Some(idx) = self.find_global_by_name(&name) {
            // Update existing function declaration with definition
            let existing = &mut self.globals[idx];
            existing.ty = ty;
            existing.is_definition = true;
            existing.is_static = is_static;
            existing.is_inline = is_inline;
            if is_const_attr {
                existing.is_const_attr = true;
            }
            if is_deprecated {
                existing.is_deprecated = true;
            }
            if is_nodiscard {
                existing.is_nodiscard = true;
            }
            if is_maybe_unused {
                existing.is_maybe_unused = true;
            }
            existing.params = params;
            existing.body = body;
            existing.locals = locals;
            existing.va_area = va_area;
            existing.alloca_bottom = alloca_bottom;
            existing.stack_size = stack_size;
            existing.is_root = !(is_static && is_inline);
        } else {
            // Create new function definition
            self.globals.push(Obj {
                id: next_node_id(),
                name,
                ty,
                location: self.last_location(),
                is_function: true,
                is_definition: true,
                is_static,
                is_inline,
                is_root: !(is_static && is_inline),
                is_const_attr,
                is_deprecated,
                is_nodiscard,
                is_maybe_unused,
                params,
                body,
                locals,
                va_area,
                alloca_bottom,
                stack_size,
                ..Default::default()
            });
        }
    }

    fn new_unique_name(&mut self) -> String {
        let name = format!("__anon{}", self.string_label);
        self.string_label += 1;
        name
    }

    fn validate_goto_labels(&self) -> CompileResult<()> {
        let mut labels = std::collections::HashMap::new();
        for (label, location) in &self.fn_labels {
            if labels.insert(label.clone(), *location).is_some() {
                self.bail_at(*location, "duplicate label")?;
            }
        }
        for (label, location) in &self.fn_gotos {
            if !labels.contains_key(label) {
                self.bail_at(*location, "use of undeclared label")?;
            }
        }
        Ok(())
    }

    fn new_anon_gvar(&mut self, ty: Type) -> usize {
        let name = self.new_unique_name();
        self.new_gvar(name, ty)
    }

    fn new_string_literal(&mut self, bytes: Vec<u8>, ty: Type) -> usize {
        let idx = self.new_anon_gvar(ty);
        if let Some(obj) = self.globals.get_mut(idx) {
            obj.init_data = Some(bytes);
            obj.is_readonly = true;
            obj.global_kind = GlobalKind::Literal;
        }
        idx
    }

    fn enter_scope(&mut self) {
        self.scopes.push(Scope::default());
    }

    fn leave_scope(&mut self) {
        self.scopes.pop();
    }

    fn push_scope_var(&mut self, idx: usize, is_local: bool) {
        let name = if is_local {
            self.locals[idx].name.clone()
        } else {
            self.globals[idx].name.clone()
        };
        self.push_scope_var_named(name, idx, is_local);
    }

    fn push_scope_var_named(&mut self, name: String, idx: usize, is_local: bool) {
        if let Some(scope) = self.scopes.last_mut() {
            scope.vars.insert(
                name.clone(),
                VarScope {
                    name,
                    idx: Some(idx),
                    is_local,
                    ..Default::default()
                },
            );
        }
    }

    fn push_scope_typedef(&mut self, name: String, ty: Type) {
        if let Some(scope) = self.scopes.last_mut() {
            scope.vars.insert(
                name.clone(),
                VarScope {
                    name,
                    type_def: Some(ty),
                    ..Default::default()
                },
            );
        }
    }

    fn push_scope_enum(&mut self, name: String, val: i64, enum_ty: Type) {
        if let Some(scope) = self.scopes.last_mut() {
            scope.vars.insert(
                name.clone(),
                VarScope {
                    name,
                    enum_val: Some(val),
                    enum_ty: Some(enum_ty),
                    ..Default::default()
                },
            );
        }
    }

    fn parse_block_stmt(&mut self) -> CompileResult<Stmt> {
        let stmts = self.parse_block_items()?;
        Ok(self.stmt_last(StmtKind::Block(stmts)))
    }

    fn parse_block_items(&mut self) -> CompileResult<Vec<Stmt>> {
        self.enter_scope();
        let mut stmts = Vec::new();
        while !self.check_punct(Punct::RBrace) {
            // Handle _Static_assert in block scope
            if self.consume_keyword(Keyword::StaticAssert) {
                self.parse_static_assert()?;
                continue;
            }

            // Skip labels (e.g., "label:") when checking for typenames
            if (self.is_typename()
                || (self.peek_is_attribute() && self.peek_after_attributes_is_typename()))
                && !matches!(self.peek_n(1).kind, TokenKind::Punct(Punct::Colon))
            {
                let mut attr = VarAttr::default();
                let basety = self.parse_declspec(Some(&mut attr))?;
                if attr.is_typedef {
                    self.parse_typedef(basety)?;
                    continue;
                }

                if self.is_function()? {
                    self.parse_function_with_basety(basety, &attr)?;
                    continue;
                }

                if attr.is_extern {
                    self.parse_global_variable(basety, &attr)?;
                    continue;
                }

                stmts.push(self.parse_declaration(basety, &attr)?);
                continue;
            }

            stmts.push(self.parse_stmt()?);
        }
        self.expect_punct(Punct::RBrace)?;
        self.leave_scope();
        Ok(stmts)
    }

    fn peek_after_attributes_is_typename(&self) -> bool {
        let mut idx = self.pos;
        let tokens = self.tokens;
        while idx + 1 < tokens.len()
            && matches!(tokens[idx].kind, TokenKind::Punct(Punct::LBracket))
            && matches!(tokens[idx + 1].kind, TokenKind::Punct(Punct::LBracket))
        {
            idx += 2;
            while idx + 1 < tokens.len() {
                if matches!(tokens[idx].kind, TokenKind::Punct(Punct::RBracket))
                    && matches!(tokens[idx + 1].kind, TokenKind::Punct(Punct::RBracket))
                {
                    idx += 2;
                    break;
                }
                idx += 1;
            }
        }
        if idx >= tokens.len() {
            return false;
        }
        if matches!(
            tokens.get(idx + 1).map(|tok| &tok.kind),
            Some(TokenKind::Punct(Punct::Colon))
        ) {
            return false;
        }
        self.is_typename_token(&tokens[idx])
    }

    fn parse_funcall(&mut self, mut callee: Expr, location: SourceLocation) -> CompileResult<Expr> {
        self.add_type_expr(&mut callee)?;

        let func_ty = match callee.ty.clone() {
            Some(Type::Func { .. }) => callee.ty.clone().unwrap(),
            Some(Type::Ptr(base)) if matches!(*base, Type::Func { .. }) => *base,
            _ => self.bail_at(callee.location, "not a function")?,
        };

        // Extract param types and is_variadic from function type
        let (param_types, is_variadic, return_ty) = match &func_ty {
            Type::Func {
                params,
                is_variadic,
                return_ty,
            } => (params.clone(), *is_variadic, return_ty.as_ref().clone()),
            _ => unreachable!("function must have function type"),
        };

        let mut args = Vec::new();
        let mut param_iter = param_types.iter();
        if !self.check_punct(Punct::RParen) {
            loop {
                let mut arg = self.parse_assign()?;
                self.add_type_expr(&mut arg)?;

                let param_opt = param_iter.next();
                // Check for too many arguments
                if param_opt.is_none() && !is_variadic {
                    return self.bail_at(arg.location, "too many arguments");
                }

                if let Some((_name, param_ty)) = param_opt {
                    if !matches!(param_ty, Type::Struct { .. } | Type::Union { .. }) {
                        arg = self.cast_expr(arg, param_ty.clone());
                    }
                } else if matches!(arg.ty.as_ref(), Some(Type::Float)) {
                    // Default argument promotion: float -> double for omitted parameter types.
                    arg = self.cast_expr(arg, Type::Double);
                }
                args.push(arg);
                if self.consume_punct(Punct::Comma) {
                    continue;
                }
                break;
            }
        }

        // Check for too few arguments
        if param_iter.next().is_some() {
            return self.bail_at(location, "too few arguments");
        }

        self.expect_punct(Punct::RParen)?;
        let mut ret_buffer = None;
        if matches!(return_ty, Type::Struct { .. } | Type::Union { .. }) {
            let name = self.new_unique_name();
            ret_buffer = Some(self.new_lvar(name, return_ty.clone()));
        }
        let mut expr = self.expr_at(
            ExprKind::Call {
                callee: Box::new(callee),
                args,
                ret_buffer,
            },
            location,
        );
        expr.ty = Some(return_ty);
        Ok(expr)
    }

    fn new_add(
        &self,
        mut lhs: Expr,
        mut rhs: Expr,
        location: SourceLocation,
    ) -> CompileResult<Expr> {
        self.add_type_expr(&mut lhs)?;
        self.add_type_expr(&mut rhs)?;

        let mut lhs_ty = lhs.ty.clone().unwrap_or(Type::Int).decay_atomic();
        let mut rhs_ty = rhs.ty.clone().unwrap_or(Type::Int).decay_atomic();

        if lhs_ty.is_numeric() && rhs_ty.is_numeric()
            || matches!(lhs_ty, Type::Vector { .. }) && matches!(rhs_ty, Type::Vector { .. })
        {
            return Ok(self.expr_at(
                ExprKind::Binary {
                    op: BinaryOp::Add,
                    lhs: Box::new(lhs),
                    rhs: Box::new(rhs),
                },
                location,
            ));
        }

        // Check if types have base (pointers or arrays)
        let lhs_has_base = lhs_ty.base().is_some();
        let rhs_has_base = rhs_ty.base().is_some();

        if lhs_has_base && rhs_has_base {
            self.bail_at(location, "invalid operands")?;
        }

        if !lhs_has_base && rhs_has_base {
            mem::swap(&mut lhs, &mut rhs);
            mem::swap(&mut lhs_ty, &mut rhs_ty);
        }

        // Check if the base is a VLA
        if let Some(base) = lhs_ty.base()
            && let Type::Vla {
                size_var: Some(size_idx),
                ..
            } = base
        {
            // VLA + num: multiply by the runtime size variable
            let scale = self.expr_at(
                ExprKind::Var {
                    idx: *size_idx,
                    is_local: true,
                },
                location,
            );
            let rhs = self.expr_at(
                ExprKind::Binary {
                    op: BinaryOp::Mul,
                    lhs: Box::new(rhs),
                    rhs: Box::new(scale),
                },
                location,
            );
            return Ok(self.expr_at(
                ExprKind::Binary {
                    op: BinaryOp::Add,
                    lhs: Box::new(lhs),
                    rhs: Box::new(rhs),
                },
                location,
            ));
        }

        let base_size = lhs_ty.base().map(|base| base.size()).unwrap_or(8);
        let mut scale = self.expr_at(
            ExprKind::Num {
                value: base_size,
                fval: 0.0,
            },
            location,
        );
        scale.ty = Some(Type::Long);
        let rhs = self.expr_at(
            ExprKind::Binary {
                op: BinaryOp::Mul,
                lhs: Box::new(rhs),
                rhs: Box::new(scale),
            },
            location,
        );
        let mut expr = self.expr_at(
            ExprKind::Binary {
                op: BinaryOp::Add,
                lhs: Box::new(lhs),
                rhs: Box::new(rhs),
            },
            location,
        );
        self.add_type_expr(&mut expr)?;
        Ok(expr)
    }

    fn new_sub(
        &self,
        mut lhs: Expr,
        mut rhs: Expr,
        location: SourceLocation,
    ) -> CompileResult<Expr> {
        self.add_type_expr(&mut lhs)?;
        self.add_type_expr(&mut rhs)?;

        let lhs_ty = lhs.ty.clone().unwrap_or(Type::Int).decay_atomic();
        let rhs_ty = rhs.ty.clone().unwrap_or(Type::Int).decay_atomic();

        if lhs_ty.is_numeric() && rhs_ty.is_numeric()
            || matches!(lhs_ty, Type::Vector { .. }) && matches!(rhs_ty, Type::Vector { .. })
        {
            return Ok(self.expr_at(
                ExprKind::Binary {
                    op: BinaryOp::Sub,
                    lhs: Box::new(lhs),
                    rhs: Box::new(rhs),
                },
                location,
            ));
        }

        if lhs_ty.base().is_some() && rhs_ty.is_integer() {
            // Check if the base is a VLA
            if let Some(base) = lhs_ty.base()
                && let Type::Vla {
                    size_var: Some(size_idx),
                    ..
                } = base
            {
                // VLA - num: multiply by the runtime size variable
                let scale = self.expr_at(
                    ExprKind::Var {
                        idx: *size_idx,
                        is_local: true,
                    },
                    location,
                );
                let rhs = self.expr_at(
                    ExprKind::Binary {
                        op: BinaryOp::Mul,
                        lhs: Box::new(rhs),
                        rhs: Box::new(scale),
                    },
                    location,
                );
                let mut expr = self.expr_at(
                    ExprKind::Binary {
                        op: BinaryOp::Sub,
                        lhs: Box::new(lhs),
                        rhs: Box::new(rhs),
                    },
                    location,
                );
                self.add_type_expr(&mut expr)?;
                return Ok(expr);
            }

            let base_size = lhs_ty.base().map(|base| base.size()).unwrap_or(8);
            let mut scale = self.expr_at(
                ExprKind::Num {
                    value: base_size,
                    fval: 0.0,
                },
                location,
            );
            scale.ty = Some(Type::Long);
            let rhs = self.expr_at(
                ExprKind::Binary {
                    op: BinaryOp::Mul,
                    lhs: Box::new(rhs),
                    rhs: Box::new(scale),
                },
                location,
            );
            let mut expr = self.expr_at(
                ExprKind::Binary {
                    op: BinaryOp::Sub,
                    lhs: Box::new(lhs),
                    rhs: Box::new(rhs),
                },
                location,
            );
            self.add_type_expr(&mut expr)?;
            return Ok(expr);
        }

        if lhs_ty.base().is_some() && rhs_ty.base().is_some() {
            let base_size = lhs_ty.base().map(|base| base.size()).unwrap_or(8);
            let mut scale = self.expr_at(
                ExprKind::Num {
                    value: base_size,
                    fval: 0.0,
                },
                location,
            );
            scale.ty = Some(Type::Long);
            let mut sub_expr = self.expr_at(
                ExprKind::Binary {
                    op: BinaryOp::Sub,
                    lhs: Box::new(lhs),
                    rhs: Box::new(rhs),
                },
                location,
            );
            sub_expr.ty = Some(Type::Long);
            let mut expr = self.expr_at(
                ExprKind::Binary {
                    op: BinaryOp::Div,
                    lhs: Box::new(sub_expr),
                    rhs: Box::new(scale),
                },
                location,
            );
            self.add_type_expr(&mut expr)?;
            return Ok(expr);
        }

        self.bail_at(location, "invalid operands")
    }

    /// Create a new initializer tree for the given type.
    fn new_initializer(&self, ty: Type, is_flexible: bool) -> Initializer {
        // For flexible arrays with unspecified length, don't create children yet
        // Note: len can be -1 (regular flexible array) or 0 (flexible array member in struct)
        if is_flexible
            && let Type::Array { len, .. } = &ty
            && *len <= 0
        {
            return Initializer {
                ty,
                expr: None,
                children: Vec::new(),
                is_flexible: true,
                mem: None,
            };
        }

        let children = if let Type::Array { base, len } | Type::Vector { base, len } = &ty {
            (0..(*len as usize))
                .map(|_| self.new_initializer(*base.clone(), false))
                .collect()
        } else if let Type::Struct {
            members,
            is_flexible: struct_is_flexible,
            ..
        }
        | Type::Union {
            members,
            is_flexible: struct_is_flexible,
            ..
        } = &ty
        {
            // Create children for each struct/union member
            let num_members = members.len();
            members
                .iter()
                .enumerate()
                .map(|(idx, member)| {
                    // If this is a flexible struct and we're on the last member, make it flexible
                    if is_flexible && *struct_is_flexible && idx == num_members - 1 {
                        self.new_initializer(member.ty.clone(), true)
                    } else {
                        self.new_initializer(member.ty.clone(), false)
                    }
                })
                .collect()
        } else {
            Vec::new()
        };

        Initializer {
            ty,
            expr: None,
            children,
            is_flexible: false,
            mem: None,
        }
    }

    /// Initialize a char array with a string literal.
    /// For example: char x[4] = "abc";
    fn string_initializer(&mut self, init: &mut Initializer, str_bytes: &[u8]) {
        let base = match &init.ty {
            Type::Array { base, .. } => base.clone(),
            _ => return,
        };
        let base_size = base.size() as usize;
        if base_size == 0 {
            return;
        }

        let string_len = match base_size {
            1 => str_bytes.len(),
            2 => str_bytes.len() / 2,
            4 => str_bytes.len() / 4,
            _ => return,
        };

        // If this is a flexible array (e.g., char x[] = "abc"), determine the length from the string
        if init.is_flexible {
            let new_ty = Type::array(*base.clone(), string_len as i32);
            *init = self.new_initializer(new_ty, false);
        }

        // Initialize each array element with the corresponding character from the string
        // Use the minimum of array length and string length to handle both:
        // - char x[10] = "abc";  // Initialize first 3, rest are 0
        // - char x[2] = "abc";   // Initialize first 2, truncate rest
        let array_len = if let Type::Array { len, .. } = &init.ty {
            *len as usize
        } else {
            return;
        };

        let len = array_len.min(string_len);
        let file_no = self.last_location().file_no;

        match base_size {
            1 => {
                (0..len).for_each(|i| {
                    init.children[i].expr = Some(self.expr_at(
                        ExprKind::Num {
                            value: str_bytes[i] as i64,
                            fval: 0.0,
                        },
                        SourceLocation {
                            line: 0,
                            column: 0,
                            byte: 0,
                            file_no,
                        },
                    ));
                });
            }
            2 => {
                (0..len).for_each(|i| {
                    let offset = i * 2;
                    let value = u16::from_le_bytes(
                        str_bytes[offset..offset + 2]
                            .try_into()
                            .expect("utf-16 string initializer"),
                    );
                    init.children[i].expr = Some(self.expr_at(
                        ExprKind::Num {
                            value: value as i64,
                            fval: 0.0,
                        },
                        SourceLocation {
                            line: 0,
                            column: 0,
                            byte: 0,
                            file_no,
                        },
                    ));
                });
            }
            4 => {
                (0..len).for_each(|i| {
                    let offset = i * 4;
                    let value = u32::from_le_bytes(
                        str_bytes[offset..offset + 4]
                            .try_into()
                            .expect("utf-32 string initializer"),
                    );
                    init.children[i].expr = Some(self.expr_at(
                        ExprKind::Num {
                            value: value as i64,
                            fval: 0.0,
                        },
                        SourceLocation {
                            line: 0,
                            column: 0,
                            byte: 0,
                            file_no,
                        },
                    ));
                });
            }
            _ => {}
        }
    }

    /// Skip an excess initializer element (one that exceeds the array size).
    /// This handles both braced initializers and simple expressions.
    fn skip_excess_element(&mut self) -> CompileResult<()> {
        if self.consume_punct(Punct::LBrace) {
            // Recursively skip the contents of braced initializer
            self.skip_excess_element()?;
            self.expect_punct(Punct::RBrace)?;
            return Ok(());
        }

        // Parse and discard a simple expression
        self.parse_assign()?;
        Ok(())
    }

    /// Count the number of elements in an array initializer.
    /// This is used for flexible arrays to determine their length.
    fn count_array_init_elements(&mut self, base_ty: &Type) -> CompileResult<usize> {
        let dummy = self.new_initializer(base_ty.clone(), true);
        let mut first = true;
        let mut idx = 0i64;
        let mut max = 0i64;

        while !self.is_end() {
            if !first {
                self.expect_punct(Punct::Comma)?;
            }
            first = false;

            if self.check_punct(Punct::LBracket) {
                self.expect_punct(Punct::LBracket)?;
                idx = self.const_expr()?;
                if self.consume_punct(Punct::Ellipsis) {
                    idx = self.const_expr()?;
                }
                self.expect_punct(Punct::RBracket)?;

                let mut dummy_copy = dummy.clone();
                self.designation(&mut dummy_copy)?;
            } else {
                // Parse (and discard) the initializer to advance position
                let mut dummy_copy = dummy.clone();
                self.parse_initializer2(&mut dummy_copy)?;
            }

            idx += 1;
            if idx > max {
                max = idx;
            }
        }

        if max < 0 { Ok(0) } else { Ok(max as usize) }
    }

    /// Parse an array designator: "[" const-expr ("..." const-expr)? "]"
    fn array_designator(&mut self, ty: &Type) -> CompileResult<(usize, usize)> {
        let location = self.peek().location;
        self.expect_punct(Punct::LBracket)?;
        let begin = self.const_expr()?;
        let end = if self.consume_punct(Punct::Ellipsis) {
            let end = self.const_expr()?;
            if end < begin {
                self.bail_at(
                    location,
                    format!("array designator range [{begin}, {end}] is empty"),
                )?;
            }
            end
        } else {
            begin
        };
        self.expect_punct(Punct::RBracket)?;

        let len = match ty {
            Type::Array { len, .. } => *len,
            _ => 0,
        };

        if begin < 0 || begin >= len as i64 {
            self.bail_at(location, "array designator index exceeds array bounds")?;
        }
        if end < 0 || end >= len as i64 {
            self.bail_at(location, "array designator index exceeds array bounds")?;
        }

        Ok((begin as usize, end as usize))
    }

    /// Parse a struct designator: "." ident
    fn struct_designator(&mut self, ty: &Type) -> CompileResult<Member> {
        let start_pos = self.pos;
        let location = self.peek().location;
        self.expect_punct(Punct::Dot)?;
        let ident = match &self.peek().kind {
            TokenKind::Ident(name) => name.clone(),
            _ => {
                self.bail_at(location, "expected a field designator")?;
                unreachable!()
            }
        };
        self.pos += 1;

        let members = match ty {
            Type::Struct { members, .. } | Type::Union { members, .. } => members,
            _ => {
                self.bail_at(location, "field name not in struct or union initializer")?;
                unreachable!()
            }
        };

        for member in members {
            if member.name.is_empty()
                && matches!(member.ty, Type::Struct { .. })
                && let Type::Struct { members: inner, .. } = &member.ty
                && Self::get_struct_member(inner, &ident).is_some()
            {
                self.pos = start_pos;
                return Ok(member.clone());
            }

            if member.name == ident {
                return Ok(member.clone());
            }
        }

        self.bail_at(location, "struct has no such member")?;
        unreachable!()
    }

    /// Parse a designation: ("[" const-expr "]" | "." ident)* "="? initializer
    fn designation(&mut self, init: &mut Initializer) -> CompileResult<()> {
        if self.check_punct(Punct::LBracket) {
            if !matches!(init.ty, Type::Array { .. }) {
                self.bail_at(self.peek().location, "array index in non-array initializer")?;
            }

            let (begin, end) = self.array_designator(&init.ty)?;
            let start_pos = self.pos;
            let mut end_pos = start_pos;
            for idx in begin..=end {
                self.pos = start_pos;
                self.designation(&mut init.children[idx])?;
                end_pos = self.pos;
            }
            self.pos = end_pos;
            self.array_initializer2(init, begin + 1)?;
            return Ok(());
        }

        if self.check_punct(Punct::Dot) && matches!(init.ty, Type::Struct { .. }) {
            let member = self.struct_designator(&init.ty)?;
            self.designation(&mut init.children[member.idx])?;
            init.expr = None;
            self.struct_initializer2(init, Some(member.idx + 1))?;
            return Ok(());
        }

        if self.check_punct(Punct::Dot) && matches!(init.ty, Type::Union { .. }) {
            let member = self.struct_designator(&init.ty)?;
            init.mem = Some(member.clone());
            self.designation(&mut init.children[member.idx])?;
            return Ok(());
        }

        if self.check_punct(Punct::Dot) {
            self.bail_at(
                self.peek().location,
                "field name not in struct or union initializer",
            )?;
        }

        self.consume_punct(Punct::Assign);
        self.parse_initializer2(init)?;
        Ok(())
    }

    /// Parse an array initializer with braces.
    /// array-initializer = "{" initializer ("," initializer)* "}"
    /// Parse array initializer with braces: "{" initializer ("," initializer)* "}"
    fn array_initializer1(&mut self, init: &mut Initializer) -> CompileResult<()> {
        self.expect_punct(Punct::LBrace)?;
        let mut first = true;

        // If this is a flexible array (e.g., int x[] = {1,2,3}), count elements to determine length
        if init.is_flexible
            && let Type::Array { base, .. } = &init.ty
        {
            // Save position to count elements
            let saved_pos = self.pos;
            let count = self.count_array_init_elements(base)?;
            // Restore position to parse elements
            self.pos = saved_pos;

            // Create a new initializer with the determined length
            let new_ty = Type::array(*base.clone(), count as i32);
            *init = self.new_initializer(new_ty, false);
        }

        let len = if let Type::Array { len, .. } | Type::Vector { len, .. } = &init.ty {
            *len
        } else {
            return Ok(());
        };

        let mut i = 0;
        // Parse all initializers until closing brace
        while !self.consume_end() {
            if !first {
                self.expect_punct(Punct::Comma)?;
            }
            first = false;

            if self.check_punct(Punct::LBracket) {
                let (begin, end) = self.array_designator(&init.ty)?;
                let start_pos = self.pos;
                let mut end_pos = start_pos;
                for idx in begin..=end {
                    self.pos = start_pos;
                    self.designation(&mut init.children[idx])?;
                    end_pos = self.pos;
                }
                self.pos = end_pos;
                i = end + 1;
                continue;
            }

            if i < (len as usize) {
                // Normal case: parse initializer for array element
                self.parse_initializer2(&mut init.children[i])?;
            } else {
                // Excess element: skip it
                self.skip_excess_element()?;
            }
            i += 1;
        }

        Ok(())
    }

    /// Parse array initializer without braces: initializer ("," initializer)*
    fn array_initializer2(&mut self, init: &mut Initializer, start: usize) -> CompileResult<()> {
        // If this is a flexible array, count elements to determine length
        if init.is_flexible
            && let Type::Array { base, .. } = &init.ty
        {
            let saved_pos = self.pos;
            let count = self.count_array_init_elements(base)?;
            self.pos = saved_pos;

            let new_ty = Type::array(*base.clone(), count as i32);
            *init = self.new_initializer(new_ty, false);
        }

        let len = if let Type::Array { len, .. } = &init.ty {
            *len
        } else {
            return Ok(());
        };

        // Parse initializers without braces, stopping at '}' or after all elements
        for i in start..(len as usize) {
            if self.is_end() {
                break;
            }

            let start_pos = self.pos;

            if i > 0 {
                self.expect_punct(Punct::Comma)?;
            }

            if self.check_punct(Punct::LBracket) || self.check_punct(Punct::Dot) {
                self.pos = start_pos;
                return Ok(());
            }

            self.parse_initializer2(&mut init.children[i])?;
        }

        Ok(())
    }

    /// Parse struct initializer with braces: "{" initializer ("," initializer)* "}"
    fn struct_initializer1(&mut self, init: &mut Initializer) -> CompileResult<()> {
        self.expect_punct(Punct::LBrace)?;

        let members = if let Type::Struct { members, .. } = &init.ty {
            members.clone()
        } else {
            return Ok(());
        };

        let mut member_idx = 0;
        let mut first = true;
        while !self.consume_end() {
            if !first {
                self.expect_punct(Punct::Comma)?;
            }
            first = false;

            if self.check_punct(Punct::Dot) {
                let member = self.struct_designator(&init.ty)?;
                self.designation(&mut init.children[member.idx])?;
                member_idx = member.idx + 1;
                continue;
            }

            if member_idx < members.len() {
                // Normal case: parse initializer for struct member
                let member = &members[member_idx];

                // Parse the member initializer normally
                self.parse_initializer2(&mut init.children[member.idx])?;
            } else {
                // Excess element: skip it
                self.skip_excess_element()?;
            }
            member_idx += 1;
        }

        Ok(())
    }

    /// Parse struct initializer without braces: initializer ("," initializer)*
    fn struct_initializer2(
        &mut self,
        init: &mut Initializer,
        start_idx: Option<usize>,
    ) -> CompileResult<()> {
        let members = if let Type::Struct { members, .. } = &init.ty {
            members.clone()
        } else {
            return Ok(());
        };

        let start_idx = start_idx.unwrap_or(0);
        let mut first = true;
        for member in members.into_iter().skip(start_idx) {
            if self.is_end() {
                break;
            }

            let start_pos = self.pos;

            if !first {
                self.expect_punct(Punct::Comma)?;
            }
            first = false;

            if self.check_punct(Punct::LBracket) || self.check_punct(Punct::Dot) {
                self.pos = start_pos;
                return Ok(());
            }

            self.parse_initializer2(&mut init.children[member.idx])?;
        }

        Ok(())
    }

    /// Parse a union initializer with or without braces.
    /// union-initializer = "{" initializer "}" | initializer
    /// Unlike structs, union initializers only initialize the first member.
    fn union_initializer(&mut self, init: &mut Initializer) -> CompileResult<()> {
        if self.check_punct(Punct::LBrace)
            && matches!(self.peek_n(1).kind, TokenKind::Punct(Punct::Dot))
        {
            self.expect_punct(Punct::LBrace)?;
            self.designation(init)?;
            self.expect_punct(Punct::RBrace)?;
            return Ok(());
        }

        if let Type::Union { members, .. } = &init.ty {
            init.mem = members.first().cloned();
        }

        if self.consume_punct(Punct::LBrace) {
            // With braces: { initializer }
            self.parse_initializer2(&mut init.children[0])?;
            self.consume_punct(Punct::Comma); // Optional trailing comma
            self.expect_punct(Punct::RBrace)?;
        } else {
            // Without braces: just the initializer
            self.parse_initializer2(&mut init.children[0])?;
        }
        Ok(())
    }

    /// Parse an initializer recursively.
    /// initializer = string-initializer | array-initializer | struct-initializer | assign
    fn parse_initializer2(&mut self, init: &mut Initializer) -> CompileResult<()> {
        // Check for string literal initializer for char arrays
        if let Type::Array { .. } = &init.ty
            && let TokenKind::Str { bytes, .. } = &self.peek().kind
        {
            let bytes_clone = bytes.clone();
            self.string_initializer(init, &bytes_clone);
            self.pos += 1; // Consume the string token
            return Ok(());
        }

        // Check for array initializer
        if let Type::Array { .. } = &init.ty {
            if self.check_punct(Punct::LBrace) {
                return self.array_initializer1(init);
            } else {
                return self.array_initializer2(init, 0);
            }
        }

        // A GNU vector (`__attribute__((vector_size(N)))`) can be brace-initialized
        // like an array, e.g. `v4si a = {1, 2, 3, 4};`; without braces it's just a
        // plain expression (e.g. `v4si c = a + b;`), handled by the scalar case below.
        if let Type::Vector { .. } = &init.ty
            && self.check_punct(Punct::LBrace)
        {
            return self.array_initializer1(init);
        }

        // Check for struct initializer
        if let Type::Struct { .. } = &init.ty {
            if self.check_punct(Punct::LBrace) {
                return self.struct_initializer1(init);
            }

            // A struct can be initialized with another struct. E.g.
            // `struct T x = y;` where y is a variable of type `struct T`.
            // Try to parse as an expression first
            let saved_pos = self.pos;
            let mut expr = self.parse_assign()?;
            self.add_type_expr(&mut expr)?;
            if matches!(&expr.ty, Some(Type::Struct { .. })) {
                init.expr = Some(expr);
                return Ok(());
            }

            // Not a struct expression, restore position and parse as struct initializer without braces
            self.pos = saved_pos;
            return self.struct_initializer2(init, None);
        }

        // Check for union initializer
        if let Type::Union { .. } = &init.ty {
            return self.union_initializer(init);
        }

        // Check for scalar initializer with braces
        if self.consume_punct(Punct::LBrace) {
            // An initializer for a scalar variable can be surrounded by
            // braces. E.g. `int x = {3};`. Handle that case.
            self.parse_initializer2(init)?;
            self.expect_punct(Punct::RBrace)?;
            return Ok(());
        }

        // Scalar initializer
        init.expr = Some(self.parse_assign()?);
        Ok(())
    }

    /// Deep copy a struct or union type, including its members.
    fn copy_struct_type(&self, ty: Type) -> Type {
        match ty {
            Type::Struct {
                members,
                tag,
                is_incomplete,
                is_flexible,
                is_packed,
                align_override,
                id,
            } => Type::Struct {
                members: members.to_vec(),
                tag,
                is_incomplete,
                is_flexible,
                is_packed,
                align_override,
                id,
            },
            Type::Union {
                members,
                tag,
                is_incomplete,
                is_flexible,
                is_packed,
                align_override,
                id,
            } => Type::Union {
                members: members.to_vec(),
                tag,
                is_incomplete,
                is_flexible,
                is_packed,
                align_override,
                id,
            },
            other => other,
        }
    }

    /// Parse an initializer for the given type.
    fn parse_initializer(&mut self, ty: Type) -> CompileResult<(Initializer, Type)> {
        let mut init = self.new_initializer(ty.clone(), true);
        self.parse_initializer2(&mut init)?;

        // Handle flexible array members in structs/unions
        if let Type::Struct {
            is_flexible: true, ..
        }
        | Type::Union {
            is_flexible: true, ..
        } = &ty
        {
            let mut ty = self.copy_struct_type(ty);

            // Find the last member
            let members = match &mut ty {
                Type::Struct { members, .. } | Type::Union { members, .. } => members,
                _ => unreachable!(),
            };

            let num_members = members.len();
            if num_members > 0 {
                let last_idx = num_members - 1;
                // Update the last member's type to match the initialized array size
                members[last_idx].ty = init.children[last_idx].ty.clone();

                // The struct size will be recalculated when needed based on the updated member types
                return Ok((init, ty));
            }
        }

        let updated_ty = init.ty.clone();
        Ok((init, updated_ty))
    }

    /// Build an expression for the designated element.
    /// For example, `x[0][1]` for a 2D array.
    fn init_desg_expr(
        &self,
        desg_stack: &[InitDesg],
        location: SourceLocation,
    ) -> CompileResult<Expr> {
        // Start with the variable reference
        let root = &desg_stack[0];
        let mut expr = self.expr_at(
            ExprKind::Var {
                idx: root.var_idx.unwrap(),
                is_local: root.var_is_local,
            },
            location,
        );

        // Apply indices/members from the stack (skip the first one which is the variable)
        for desg in &desg_stack[1..] {
            if let Some(member) = &desg.member {
                // Member access: x.member
                expr = self.expr_at(
                    ExprKind::Member {
                        lhs: Box::new(expr),
                        member: member.clone(),
                    },
                    location,
                );
            } else {
                // Array index: x[idx]
                let idx_expr = self.expr_at(
                    ExprKind::Num {
                        value: desg.idx as i64,
                        fval: 0.0,
                    },
                    location,
                );
                // Use new_add to properly handle pointer arithmetic
                expr = self.new_add(expr, idx_expr, location)?;
                expr = self.expr_at(ExprKind::Deref(Box::new(expr)), location);
            }
        }

        Ok(expr)
    }

    /// Convert an initializer tree into assignment expressions.
    fn create_lvar_init(
        &self,
        init: &Initializer,
        desg_stack: &mut Vec<InitDesg>,
        location: SourceLocation,
    ) -> CompileResult<Expr> {
        if let Type::Array { base: _, len } = &init.ty {
            let mut node = self.expr_at(ExprKind::Null, location);

            for i in 0..(*len as usize) {
                desg_stack.push(InitDesg {
                    idx: i as i32,
                    member: None,
                    var_idx: None,
                    var_is_local: false,
                });
                let rhs = self.create_lvar_init(&init.children[i], desg_stack, location)?;
                desg_stack.pop();

                node = self.expr_at(
                    ExprKind::Comma {
                        lhs: Box::new(node),
                        rhs: Box::new(rhs),
                    },
                    location,
                );
            }

            return Ok(node);
        }

        if let Type::Struct { members, .. } = &init.ty {
            // If we have a direct struct expression (e.g., `T y = x;`), don't do member-by-member init
            if init.expr.is_none() {
                let mut node = self.expr_at(ExprKind::Null, location);

                for member in members {
                    desg_stack.push(InitDesg {
                        idx: 0,
                        member: Some(member.clone()),
                        var_idx: None,
                        var_is_local: false,
                    });
                    let rhs =
                        self.create_lvar_init(&init.children[member.idx], desg_stack, location)?;
                    desg_stack.pop();

                    node = self.expr_at(
                        ExprKind::Comma {
                            lhs: Box::new(node),
                            rhs: Box::new(rhs),
                        },
                        location,
                    );
                }

                return Ok(node);
            }
        }

        if let Type::Union { members, .. } = &init.ty {
            // For unions, only initialize the first member
            let first_member = init.mem.as_ref().unwrap_or(&members[0]);
            desg_stack.push(InitDesg {
                idx: 0,
                member: Some(first_member.clone()),
                var_idx: None,
                var_is_local: false,
            });
            let result =
                self.create_lvar_init(&init.children[first_member.idx], desg_stack, location)?;
            desg_stack.pop();
            return Ok(result);
        }

        // If no initializer expression, return null (element will be zero-initialized)
        if init.expr.is_none() {
            return Ok(self.expr_at(ExprKind::Null, location));
        }

        let lhs = self.init_desg_expr(desg_stack, location)?;
        let rhs = init.expr.clone().unwrap();

        Ok(self.expr_at(
            ExprKind::Assign {
                lhs: Box::new(lhs),
                rhs: Box::new(rhs),
            },
            location,
        ))
    }

    /// Parse a local variable initializer and convert it to assignment expressions.
    /// A variable definition with an initializer is shorthand for a variable definition
    /// followed by assignments. For example:
    /// `int x[2][2] = {{6, 7}, {8, 9}}` is converted to:
    ///   x[0][0] = 6; x[0][1] = 7; x[1][0] = 8; x[1][1] = 9;
    ///
    /// If a partial initializer list is given, the C standard requires that
    /// unspecified elements are set to 0. We implement this by zero-initializing
    /// the entire memory region before applying user-supplied values.
    fn parse_lvar_initializer(
        &mut self,
        var_idx: usize,
        var_is_local: bool,
        ty: Type,
        location: SourceLocation,
        constexpr_value: Option<&mut Option<i64>>,
    ) -> CompileResult<Expr> {
        let (init, updated_ty) = self.parse_initializer(ty)?;

        // Update the variable's type (important for flexible arrays)
        if var_is_local {
            self.locals[var_idx].ty = updated_ty;
        } else {
            self.globals[var_idx].ty = updated_ty;
        }
        if let Some(constexpr_value) = constexpr_value {
            let value = self.eval_constexpr_initializer(&init, location)?;
            *constexpr_value = Some(value);
        }

        let mut desg_stack = vec![InitDesg {
            idx: 0,
            member: None,
            var_idx: Some(var_idx),
            var_is_local,
        }];

        // First, zero-initialize the entire variable
        let memzero = self.expr_at(
            ExprKind::Memzero {
                idx: var_idx,
                is_local: var_is_local,
            },
            location,
        );

        // Then apply user-supplied initializers
        let assignments = self.create_lvar_init(&init, &mut desg_stack, location)?;

        // Combine them with a comma operator: memzero, assignments
        Ok(self.expr_at(
            ExprKind::Comma {
                lhs: Box::new(memzero),
                rhs: Box::new(assignments),
            },
            location,
        ))
    }

    fn eval_constexpr_initializer(
        &mut self,
        init: &Initializer,
        location: SourceLocation,
    ) -> CompileResult<i64> {
        if !init.children.is_empty() || init.expr.is_none() {
            self.bail_at(
                location,
                "constexpr initializer must be an integer constant expression",
            )?;
        }

        let mut expr = init.expr.clone().unwrap();
        self.add_type_expr(&mut expr)?;
        if !expr.ty.as_ref().is_some_and(|ty| ty.is_integer()) {
            self.bail_at(
                location,
                "constexpr initializer must be an integer constant expression",
            )?;
        }
        self.eval(&mut expr)
    }

    /// Read a value from a buffer based on size (1, 2, 4, or 8 bytes).
    fn read_buf(buf: &[u8], sz: usize, offset: usize) -> u64 {
        let n = sz.min(8);
        let mut bytes = [0u8; 8];
        bytes[..n].copy_from_slice(&buf[offset..offset + n]);
        u64::from_le_bytes(bytes)
    }

    /// Write a value to a buffer based on size (1, 2, 4, or 8 bytes).
    fn write_buf(buf: &mut [u8], val: u64, sz: usize, offset: usize) {
        let n = sz.min(8);
        let bytes = val.to_le_bytes();
        buf[offset..offset + n].copy_from_slice(&bytes[..n]);
        for b in &mut buf[offset + n..offset + sz] {
            *b = 0;
        }
    }

    /// Recursively serialize initializer data to a byte array for global variables.
    fn write_gvar_data(
        &self,
        init: &Initializer,
        buf: &mut [u8],
        offset: usize,
    ) -> CompileResult<Vec<Relocation>> {
        if let Type::Array { base, len } = &init.ty {
            let sz = base.size() as usize;
            let mut relocations = Vec::new();
            for i in 0..(*len as usize) {
                let mut rels = self.write_gvar_data(&init.children[i], buf, offset + sz * i)?;
                relocations.append(&mut rels);
            }
            return Ok(relocations);
        }

        if let Type::Union { members, .. } = &init.ty {
            // For unions, only initialize the first member
            if let Some(member) = init.mem.as_ref().or_else(|| members.first()) {
                return self.write_gvar_data(&init.children[member.idx], buf, offset);
            }
            return Ok(Vec::new());
        }

        if let Type::Struct { members, .. } = &init.ty {
            let mut relocations = Vec::new();
            for member in members {
                if member.is_bitfield {
                    // Handle bitfield members
                    if let Some(expr) = &init.children[member.idx].expr {
                        let loc_offset = offset + member.offset as usize;
                        let oldval = Self::read_buf(buf, member.ty.size() as usize, loc_offset);
                        let mut expr_clone = expr.clone();
                        let newval = self.eval(&mut expr_clone)? as u64;
                        let mask = (1u64 << member.bit_width) - 1;
                        let combined = oldval | ((newval & mask) << member.bit_offset);
                        Self::write_buf(buf, combined, member.ty.size() as usize, loc_offset);
                    }
                } else {
                    let mut rels = self.write_gvar_data(
                        &init.children[member.idx],
                        buf,
                        offset + member.offset as usize,
                    )?;
                    relocations.append(&mut rels);
                }
            }
            return Ok(relocations);
        }

        if let Type::Float = &init.ty
            && let Some(expr) = &init.expr
        {
            let mut expr_clone = expr.clone();
            let val = self.eval_double(&mut expr_clone)?;
            let bytes = (val as f32).to_le_bytes();
            let end = offset + bytes.len();
            buf[offset..end].copy_from_slice(&bytes);
            return Ok(Vec::new());
        }

        if let Type::Double = &init.ty
            && let Some(expr) = &init.expr
        {
            let mut expr_clone = expr.clone();
            let val = self.eval_double(&mut expr_clone)?;
            let bytes = val.to_le_bytes();
            let end = offset + bytes.len();
            buf[offset..end].copy_from_slice(&bytes);
            return Ok(Vec::new());
        }

        if let Type::LDouble = &init.ty
            && let Some(expr) = &init.expr
        {
            let bytes = if let ExprKind::LDoubleLiteral { value } = &expr.kind {
                x87_to_bytes(*value)
            } else {
                let mut expr_clone = expr.clone();
                let val = self.eval_double(&mut expr_clone)?;
                f64_to_x87_bytes(val)
            };
            let end = offset + bytes.len();
            buf[offset..end].copy_from_slice(&bytes);
            return Ok(Vec::new());
        }

        if let Some(expr) = &init.expr {
            let mut expr_clone = expr.clone();
            let mut label = None;
            let val = self.eval2(&mut expr_clone, Some(&mut label))?;

            if let Some(label_str) = label {
                // This is a pointer to a global variable
                return Ok(vec![Relocation {
                    offset,
                    label: label_str,
                    addend: val,
                }]);
            } else {
                // This is a regular constant value
                Self::write_buf(buf, val as u64, init.ty.size() as usize, offset);
            }
        }

        Ok(Vec::new())
    }

    /// Parse and evaluate global variable initializers at compile-time.
    /// Initializers for global variables are embedded in the .data section.
    fn parse_gvar_initializer(
        &mut self,
        var_idx: usize,
        location: SourceLocation,
    ) -> CompileResult<()> {
        let ty = self.globals[var_idx].ty.clone();
        let (init, updated_ty) = self.parse_initializer(ty)?;

        // Update the variable's type (important for flexible arrays)
        self.globals[var_idx].ty = updated_ty.clone();

        // Allocate buffer and serialize initializer data
        let size = updated_ty.size() as usize;
        let mut buf = vec![0u8; size];
        let relocations = self.write_gvar_data(&init, &mut buf, 0)?;
        self.globals[var_idx].init_data = Some(buf);
        self.globals[var_idx].relocations = relocations;
        if self.globals[var_idx].is_constexpr {
            let value = self.eval_constexpr_initializer(&init, location)?;
            self.globals[var_idx].const_value = Some(value);
        }

        Ok(())
    }

    fn add_type_stmt(&self, stmt: &mut Stmt) -> CompileResult<()> {
        match &mut stmt.kind {
            StmtKind::Return(expr) => {
                if let Some(expr) = expr {
                    self.add_type_expr(expr)?;
                }
            }
            StmtKind::Block(stmts) => {
                for stmt in stmts {
                    self.add_type_stmt(stmt)?;
                }
            }
            StmtKind::If { cond, then, els } => {
                self.add_type_expr(cond)?;
                self.add_type_stmt(then)?;
                if let Some(els) = els {
                    self.add_type_stmt(els)?;
                }
            }
            StmtKind::For {
                init,
                cond,
                inc,
                body,
            } => {
                if let Some(init) = init {
                    self.add_type_stmt(init)?;
                }
                if let Some(cond) = cond {
                    self.add_type_expr(cond)?;
                }
                if let Some(inc) = inc {
                    self.add_type_expr(inc)?;
                }
                self.add_type_stmt(body)?;
            }
            StmtKind::DoWhile { body, cond } => {
                self.add_type_stmt(body)?;
                self.add_type_expr(cond)?;
            }
            StmtKind::Switch { cond, body, .. } => {
                self.add_type_expr(cond)?;
                self.add_type_stmt(body)?;
                self.warn_fallthrough_in_switch(body);
            }
            StmtKind::Case { stmt, .. } => {
                self.add_type_stmt(stmt)?;
            }
            StmtKind::GotoExpr { target } => {
                self.add_type_expr(target)?;
            }
            StmtKind::Goto { .. } => {}
            StmtKind::Break => {}
            StmtKind::Continue => {}
            StmtKind::Label { stmt, .. } => {
                self.add_type_stmt(stmt)?;
            }
            StmtKind::Fallthrough => {}
            StmtKind::Expr(expr) => {
                self.add_type_expr(expr)?;
                self.warn_nodiscard_expr(expr);
            }
            StmtKind::Decl(_) => {}
            StmtKind::Asm(_) => {}
        }
        Ok(())
    }

    fn warn_nodiscard_expr(&self, expr: &Expr) {
        let ExprKind::Call { callee, .. } = &expr.kind else {
            return;
        };
        let ExprKind::Var { idx, is_local } = &callee.kind else {
            return;
        };
        let obj = if *is_local {
            &self.locals[*idx]
        } else {
            &self.globals[*idx]
        };
        if obj.is_function && obj.is_nodiscard {
            self.warn_at(
                expr.location,
                format!("'{}' discards return value", obj.name),
            );
        }
    }

    fn warn_fallthrough_in_switch(&self, body: &Stmt) {
        let mut items = Vec::new();
        self.flatten_switch_items(body, &mut items);

        let mut in_case = false;
        let mut has_stmt = false;
        let mut last_stmt: Option<&Stmt> = None;
        let mut fallthrough_attr = false;

        for item in items {
            match item {
                SwitchItem::CaseLabel => {
                    if in_case
                        && has_stmt
                        && !fallthrough_attr
                        && let Some(stmt) = last_stmt
                        && !Self::stmt_terminates(stmt)
                    {
                        self.warn_at(stmt.location, "implicit fallthrough");
                    }
                    in_case = true;
                    has_stmt = false;
                    last_stmt = None;
                    fallthrough_attr = false;
                }
                SwitchItem::Stmt(stmt) => {
                    if !in_case {
                        continue;
                    }
                    match stmt.kind {
                        StmtKind::Fallthrough => {
                            fallthrough_attr = true;
                            last_stmt = Some(stmt);
                        }
                        StmtKind::Case { .. } => {}
                        _ => {
                            has_stmt = true;
                            fallthrough_attr = false;
                            last_stmt = Some(stmt);
                        }
                    }
                }
            }
        }
        if in_case
            && has_stmt
            && !fallthrough_attr
            && let Some(stmt) = last_stmt
            && !Self::stmt_terminates(stmt)
        {
            self.warn_at(stmt.location, "implicit fallthrough");
        }
    }

    fn flatten_switch_items<'b>(&self, stmt: &'b Stmt, out: &mut Vec<SwitchItem<'b>>) {
        match &stmt.kind {
            StmtKind::Block(stmts) => {
                for stmt in stmts {
                    self.flatten_switch_items(stmt, out);
                }
            }
            StmtKind::Case { stmt, .. } => {
                out.push(SwitchItem::CaseLabel);
                self.flatten_switch_items(stmt, out);
            }
            _ => out.push(SwitchItem::Stmt(stmt)),
        }
    }

    fn stmt_terminates(stmt: &Stmt) -> bool {
        matches!(
            &stmt.kind,
            StmtKind::Return(_)
                | StmtKind::Break
                | StmtKind::Continue
                | StmtKind::Goto { .. }
                | StmtKind::GotoExpr { .. }
        )
    }

    fn add_type_expr(&self, expr: &mut Expr) -> CompileResult<()> {
        if expr.ty.is_some() && !matches!(expr.kind, ExprKind::Cast { .. }) {
            return Ok(());
        }

        let ty = match &mut expr.kind {
            ExprKind::Null => Type::Void,
            ExprKind::Memzero { .. } => Type::Void,
            ExprKind::Num { .. } => Type::Int,
            ExprKind::BigIntLiteral { .. } => unreachable!(
                "BigIntLiteral is only constructed with expr.ty already set, in the \
                 TokenKind::Num primary-expression case above"
            ),
            ExprKind::LDoubleLiteral { .. } => unreachable!(
                "LDoubleLiteral is only constructed with expr.ty already set, in the \
                 TokenKind::Num primary-expression case above"
            ),
            ExprKind::Var { idx, is_local } | ExprKind::VlaPtr { idx, is_local } => {
                let map = if *is_local {
                    &self.locals
                } else {
                    &self.globals
                };
                map.get(*idx).map(|obj| obj.ty.clone()).unwrap_or(Type::Int)
            }
            ExprKind::Call { callee, args, .. } => {
                self.add_type_expr(callee)?;
                for arg in args {
                    self.add_type_expr(arg)?;
                }
                match callee.ty.as_ref() {
                    Some(Type::Func { return_ty, .. }) => return_ty.as_ref().clone(),
                    Some(Type::Ptr(base)) if matches!(**base, Type::Func { .. }) => match &**base {
                        Type::Func { return_ty, .. } => return_ty.as_ref().clone(),
                        _ => Type::Int,
                    },
                    _ => Type::Int,
                }
            }
            ExprKind::Unary { op, expr } => {
                self.add_type_expr(expr)?;
                match op {
                    UnaryOp::Not => Type::Int,
                    UnaryOp::BitNot => expr.ty.clone().unwrap_or(Type::Int),
                    UnaryOp::Neg => {
                        let ty = self
                            .get_common_type(&Type::Int, expr.ty.as_ref().unwrap_or(&Type::Int));
                        self.cast_expr_in_place(expr, ty.clone());
                        ty
                    }
                    UnaryOp::Real | UnaryOp::Imag => {
                        // __real__ and __imag__ return the base type of the complex
                        let expr_ty = expr.ty.as_ref().unwrap_or(&Type::DoubleComplex);
                        match expr_ty {
                            Type::FloatComplex => Type::Float,
                            Type::DoubleComplex => Type::Double,
                            Type::LDoubleComplex => Type::LDouble,
                            Type::IntComplex(base) => (**base).clone(),
                            // For non-complex types, return the same type
                            _ => expr_ty.clone(),
                        }
                    }
                }
            }
            ExprKind::Addr(expr) => {
                self.add_type_expr(expr)?;
                let expr_ty = expr.ty.clone().unwrap_or(Type::Int);
                // If taking address of array, return pointer to base type
                // since &array[10] should give int*, not int(*)[10]
                if let Type::Array { base, .. } = expr_ty {
                    Type::Ptr(base)
                } else {
                    Type::Ptr(Box::new(expr_ty))
                }
            }
            ExprKind::Deref(expr) => {
                self.add_type_expr(expr)?;
                let expr_ty = expr.ty.clone().unwrap_or(Type::Int);
                // Dereference works on pointers and arrays
                match expr_ty.base() {
                    Some(base) => {
                        if *base == Type::Void {
                            self.bail_at(expr.location, "dereferencing a void pointer")?;
                        }
                        base.clone()
                    }
                    None => self.bail_at(expr.location, "invalid pointer dereference")?,
                }
            }
            ExprKind::StmtExpr(stmts) => {
                if stmts.is_empty() {
                    self.bail_at(
                        expr.location,
                        "statement expression returning void is not supported",
                    )?
                }
                for stmt in stmts.iter_mut() {
                    self.add_type_stmt(stmt)?;
                }
                let last = stmts.last_mut().unwrap();
                match &mut last.kind {
                    StmtKind::Expr(expr) => {
                        self.add_type_expr(expr)?;
                        expr.ty.clone().unwrap_or(Type::Int)
                    }
                    _ => self.bail_at(
                        expr.location,
                        "statement expression returning void is not supported",
                    )?,
                }
            }
            ExprKind::LabelVal { .. } => Type::Ptr(Box::new(Type::Void)),
            ExprKind::Assign { lhs, rhs } => {
                self.add_type_expr(lhs)?;
                self.add_type_expr(rhs)?;
                let lhs_ty = lhs.ty.clone().unwrap_or(Type::Int);
                // Arrays are not assignable (but VLAs are, since they're pointers)
                if matches!(lhs_ty, Type::Array { .. }) {
                    self.bail_at(lhs.location, "not an lvalue")?
                }
                if !matches!(lhs_ty, Type::Struct { .. }) {
                    self.cast_expr_in_place(rhs, lhs_ty.clone());
                }
                lhs_ty
            }
            ExprKind::CompoundAssign { lhs, rhs, .. } => {
                self.add_type_expr(lhs)?;
                self.add_type_expr(rhs)?;
                lhs.ty.clone().unwrap_or(Type::Int)
            }
            ExprKind::Cond { cond, then, els } => {
                self.add_type_expr(cond)?;
                self.add_type_expr(then)?;
                self.add_type_expr(els)?;
                if matches!(then.ty, Some(Type::Void)) || matches!(els.ty, Some(Type::Void)) {
                    Type::Void
                } else {
                    self.usual_arith_conv(then, els)?
                }
            }
            ExprKind::Member { member, .. } => member.ty.clone(),
            ExprKind::Cast { expr, ty } => {
                self.add_type_expr(expr)?;
                ty.clone()
            }
            ExprKind::Comma { lhs, rhs } => {
                self.add_type_expr(lhs)?;
                self.add_type_expr(rhs)?;
                rhs.ty.clone().unwrap_or(Type::Int)
            }
            ExprKind::Binary { op, lhs, rhs } => match op {
                BinaryOp::Eq | BinaryOp::Ne | BinaryOp::Lt | BinaryOp::Le => {
                    self.usual_arith_conv(lhs, rhs)?;
                    Type::Int
                }
                BinaryOp::LogAnd | BinaryOp::LogOr => {
                    self.add_type_expr(lhs)?;
                    self.add_type_expr(rhs)?;
                    Type::Int
                }
                BinaryOp::Add
                | BinaryOp::Sub
                | BinaryOp::Mul
                | BinaryOp::Div
                | BinaryOp::Mod
                | BinaryOp::BitAnd
                | BinaryOp::BitOr
                | BinaryOp::BitXor
                | BinaryOp::Shl
                | BinaryOp::Shr => self.usual_arith_conv(lhs, rhs)?,
            },
            ExprKind::Cas { addr, old, new } => {
                self.add_type_expr(addr)?;
                self.add_type_expr(old)?;
                self.add_type_expr(new)?;

                // Check that addr is a pointer
                if !matches!(addr.ty, Some(Type::Ptr(_))) {
                    self.bail_at(addr.location, "pointer expected")?;
                }

                // Check that old is a pointer
                if !matches!(old.ty, Some(Type::Ptr(_))) {
                    self.bail_at(old.location, "pointer expected")?;
                }

                Type::Bool
            }
            ExprKind::Exch { addr, val } => {
                self.add_type_expr(addr)?;
                self.add_type_expr(val)?;

                // Check that addr is a pointer

                match &addr.ty {
                    Some(Type::Ptr(base)) => base.as_ref().clone(),
                    _ => self.bail_at(addr.location, "pointer expected")?,
                }
            }
        };

        expr.ty = Some(ty);
        Ok(())
    }

    /// Evaluate a given expression as a constant expression at compile time.
    fn eval(&self, expr: &mut Expr) -> CompileResult<i64> {
        self.eval2(expr, None)
    }

    /// Evaluate a given expression as a constant expression.
    ///
    /// A constant expression is either just a number or ptr+n where ptr
    /// is a pointer to a global variable and n is a positive/negative
    /// number. The latter form is accepted only as an initialization
    /// expression for a global variable.
    fn eval2(&self, expr: &mut Expr, label: Option<&mut Option<String>>) -> CompileResult<i64> {
        self.add_type_expr(expr)?;

        if expr.ty.as_ref().is_some_and(|ty| ty.is_flonum()) {
            return Ok(self.eval_double(expr)? as i64);
        }

        match &mut expr.kind {
            ExprKind::Num { value: val, .. } => Ok(*val),
            ExprKind::BigIntLiteral { raw } => {
                let digits = raw.trim_end_matches(['w', 'W', 'b', 'B', 'u', 'U']);
                let mut value: i64 = 0;
                for byte in digits.bytes() {
                    if let Some(digit) = (byte as char).to_digit(10) {
                        value = value.wrapping_mul(10).wrapping_add(digit as i64);
                    }
                }
                Ok(value)
            }
            ExprKind::LabelVal { label: label_name } => {
                let label_ref = match label {
                    Some(label_ref) => label_ref,
                    None => self.bail_at(expr.location, "not a compile-time constant")?,
                };
                let func_name = self
                    .current_fn
                    .as_ref()
                    .ok_or_else(|| self.err_at(expr.location, "not a compile-time constant"))?;
                *label_ref = Some(label_symbol(func_name, label_name));
                Ok(0)
            }
            ExprKind::Unary { op, expr } => {
                let val = self.eval2(expr, label)?;
                match op {
                    UnaryOp::Neg => Ok(-val),
                    UnaryOp::Not => Ok(if val == 0 { 1 } else { 0 }),
                    UnaryOp::BitNot => Ok(!val),
                    UnaryOp::Real | UnaryOp::Imag => {
                        // Complex constants not fully supported, return the value
                        Ok(val)
                    }
                }
            }
            ExprKind::Binary { op, lhs, rhs } => match op {
                BinaryOp::Add => {
                    let lval = self.eval2(lhs, label)?;
                    let rval = self.eval(rhs)?;
                    Ok(lval + rval)
                }
                BinaryOp::Sub => {
                    let lval = self.eval2(lhs, label)?;
                    let rval = self.eval(rhs)?;
                    Ok(lval - rval)
                }
                _ => {
                    let lval = self.eval(lhs)?;
                    let rval = self.eval(rhs)?;
                    let expr_ty = expr.ty.as_ref().unwrap_or(&Type::Int);
                    let lhs_is_unsigned = lhs
                        .ty
                        .as_ref()
                        .is_some_and(|ty| ty.is_unsigned() || matches!(ty, Type::Ptr(_)));
                    match op {
                        BinaryOp::Mul => Ok(lval * rval),
                        BinaryOp::Div => {
                            if expr_ty.is_unsigned() {
                                Ok(((lval as u64) / (rval as u64)) as i64)
                            } else {
                                Ok(lval / rval)
                            }
                        }
                        BinaryOp::Mod => {
                            if expr_ty.is_unsigned() {
                                Ok(((lval as u64) % (rval as u64)) as i64)
                            } else {
                                Ok(lval % rval)
                            }
                        }
                        BinaryOp::BitAnd => Ok(lval & rval),
                        BinaryOp::BitOr => Ok(lval | rval),
                        BinaryOp::BitXor => Ok(lval ^ rval),
                        BinaryOp::Shl => Ok(lval << rval),
                        BinaryOp::Shr => {
                            if expr_ty.is_unsigned() && expr_ty.size() == 8 {
                                Ok(((lval as u64) >> (rval as u64)) as i64)
                            } else {
                                Ok(lval >> rval)
                            }
                        }
                        BinaryOp::Eq => Ok(if lval == rval { 1 } else { 0 }),
                        BinaryOp::Ne => Ok(if lval != rval { 1 } else { 0 }),
                        BinaryOp::Lt => {
                            let result = if lhs_is_unsigned {
                                (lval as u64) < (rval as u64)
                            } else {
                                lval < rval
                            };
                            Ok(if result { 1 } else { 0 })
                        }
                        BinaryOp::Le => {
                            let result = if lhs_is_unsigned {
                                (lval as u64) <= (rval as u64)
                            } else {
                                lval <= rval
                            };
                            Ok(if result { 1 } else { 0 })
                        }
                        BinaryOp::LogAnd => Ok(if lval != 0 && rval != 0 { 1 } else { 0 }),
                        BinaryOp::LogOr => Ok(if lval != 0 || rval != 0 { 1 } else { 0 }),
                        _ => unreachable!(),
                    }
                }
            },
            ExprKind::Cond { cond, then, els } => {
                let cond_val = self.eval(cond)?;
                if cond_val != 0 {
                    self.eval2(then, label)
                } else {
                    self.eval2(els, label)
                }
            }
            ExprKind::Comma { lhs: _, rhs } => self.eval2(rhs, label),
            ExprKind::Cast { expr, ty } => {
                let val = self.eval2(expr, label)?;
                if ty.is_integer() {
                    match ty.size() {
                        1 => Ok(if ty.is_unsigned() {
                            val as u8 as i64
                        } else {
                            val as i8 as i64
                        }),
                        2 => Ok(if ty.is_unsigned() {
                            val as u16 as i64
                        } else {
                            val as i16 as i64
                        }),
                        4 => Ok(if ty.is_unsigned() {
                            val as u32 as i64
                        } else {
                            val as i32 as i64
                        }),
                        _ => Ok(val),
                    }
                } else {
                    Ok(val)
                }
            }
            ExprKind::Addr(inner) => self.eval_rval(inner, label),
            ExprKind::Member { lhs, member } => {
                if label.is_none() {
                    self.bail_at(expr.location, "not a compile-time constant")?
                }
                if !member.ty.is_array() {
                    self.bail_at(expr.location, "invalid initializer")?
                }
                let offset = self.eval_rval(lhs, label)?;
                Ok(offset + member.offset as i64)
            }
            ExprKind::Var { idx, is_local } => {
                let var = if *is_local {
                    &self.locals[*idx]
                } else {
                    &self.globals[*idx]
                };
                if var.is_constexpr {
                    if let Some(value) = var.const_value {
                        return Ok(value);
                    }
                    if label.is_none() {
                        self.bail_at(expr.location, "not a compile-time constant")?
                    }
                }
                if label.is_none() {
                    self.bail_at(expr.location, "not a compile-time constant")?
                }
                if !var.ty.is_array() && !matches!(var.ty, Type::Func { .. }) {
                    self.bail_at(expr.location, "invalid initializer")?
                }
                if let Some(label_ref) = label {
                    *label_ref = Some(var.name.clone());
                }
                Ok(0)
            }
            _ => self.bail_at(expr.location, "not a compile-time constant"),
        }
    }

    fn eval_double(&self, expr: &mut Expr) -> CompileResult<f64> {
        self.add_type_expr(expr)?;

        if expr.ty.as_ref().is_some_and(|ty| ty.is_integer()) {
            let val = self.eval(expr)?;
            if expr.ty.as_ref().is_some_and(|ty| ty.is_unsigned()) {
                return Ok(val as u64 as f64);
            }
            return Ok(val as f64);
        }

        match &mut expr.kind {
            ExprKind::Binary { op, lhs, rhs } => match op {
                BinaryOp::Add => Ok(self.eval_double(lhs)? + self.eval_double(rhs)?),
                BinaryOp::Sub => Ok(self.eval_double(lhs)? - self.eval_double(rhs)?),
                BinaryOp::Mul => Ok(self.eval_double(lhs)? * self.eval_double(rhs)?),
                BinaryOp::Div => Ok(self.eval_double(lhs)? / self.eval_double(rhs)?),
                _ => self.bail_at(expr.location, "not a compile-time constant"),
            },
            ExprKind::Unary { op, expr } => match op {
                UnaryOp::Neg => Ok(-self.eval_double(expr)?),
                _ => self.bail_at(expr.location, "not a compile-time constant"),
            },
            ExprKind::Cond { cond, then, els } => {
                if self.eval_double(cond)? != 0.0 {
                    self.eval_double(then)
                } else {
                    self.eval_double(els)
                }
            }
            ExprKind::Comma { lhs: _, rhs } => self.eval_double(rhs),
            ExprKind::Cast { expr, .. } => {
                if expr.ty.as_ref().is_some_and(|ty| ty.is_flonum()) {
                    self.eval_double(expr)
                } else {
                    Ok(self.eval(expr)? as f64)
                }
            }
            ExprKind::Num { fval, .. } => Ok(*fval),
            ExprKind::LDoubleLiteral { value } => Ok(x87_to_f64(*value)),
            _ => self.bail_at(expr.location, "not a compile-time constant"),
        }
    }

    fn eval_rval(&self, expr: &mut Expr, label: Option<&mut Option<String>>) -> CompileResult<i64> {
        self.add_type_expr(expr)?;

        match &mut expr.kind {
            ExprKind::Var { idx, is_local } => {
                if *is_local {
                    self.bail_at(expr.location, "not a compile-time constant")?
                }
                let var = &self.globals[*idx];
                if let Some(label_ref) = label {
                    *label_ref = Some(var.name.clone());
                }
                Ok(0)
            }
            ExprKind::Deref(inner) => self.eval2(inner, label),
            ExprKind::Member { lhs, member } => {
                let offset = self.eval_rval(lhs, label)?;
                Ok(offset + member.offset as i64)
            }
            _ => self.bail_at(expr.location, "invalid initializer"),
        }
    }

    /// Check if an expression can be evaluated at compile-time.
    fn is_const_expr(&mut self, expr: &mut Expr) -> CompileResult<bool> {
        self.add_type_expr(expr)?;

        Ok(match &expr.kind {
            ExprKind::Binary { op, lhs, rhs } => {
                matches!(
                    op,
                    BinaryOp::Add
                        | BinaryOp::Sub
                        | BinaryOp::Mul
                        | BinaryOp::Div
                        | BinaryOp::BitAnd
                        | BinaryOp::BitOr
                        | BinaryOp::BitXor
                        | BinaryOp::Shl
                        | BinaryOp::Shr
                        | BinaryOp::Eq
                        | BinaryOp::Ne
                        | BinaryOp::Lt
                        | BinaryOp::Le
                        | BinaryOp::LogAnd
                        | BinaryOp::LogOr
                ) && self.is_const_expr(&mut lhs.clone())?
                    && self.is_const_expr(&mut rhs.clone())?
            }
            ExprKind::Cond { cond, then, els } => {
                let mut cond_clone = *cond.clone();
                if !self.is_const_expr(&mut cond_clone)? {
                    return Ok(false);
                }
                let cond_val = self.eval(&mut cond_clone)?;
                let chosen = if cond_val != 0 { then } else { els };
                self.is_const_expr(&mut chosen.clone())?
            }
            ExprKind::Comma { rhs, .. } => self.is_const_expr(&mut rhs.clone())?,
            ExprKind::Unary { op, expr } => {
                matches!(
                    op,
                    UnaryOp::Neg | UnaryOp::Not | UnaryOp::BitNot | UnaryOp::Real | UnaryOp::Imag
                ) && self.is_const_expr(&mut expr.clone())?
            }
            ExprKind::Cast { expr, .. } => self.is_const_expr(&mut expr.clone())?,
            ExprKind::Num { .. } | ExprKind::LDoubleLiteral { .. } => true,
            ExprKind::Var { idx, is_local } => {
                let var = if *is_local {
                    &self.locals[*idx]
                } else {
                    &self.globals[*idx]
                };
                var.is_constexpr && var.const_value.is_some()
            }
            _ => false,
        })
    }

    /// Parse and evaluate a constant expression.
    fn const_expr(&mut self) -> CompileResult<i64> {
        let mut expr = self.parse_conditional()?;
        self.eval(&mut expr)
    }
}

fn label_symbol(function: &str, label: &str) -> String {
    format!(".L.{}.{}", asm_symbol(function), label)
}

fn asm_symbol(name: &str) -> String {
    if name
        .as_bytes()
        .iter()
        .all(|b| b.is_ascii_alphanumeric() || matches!(b, b'_' | b'.' | b'$'))
    {
        return name.to_string();
    }

    let mut out = String::from("__cc3_u8_");
    for b in name.as_bytes() {
        use std::fmt::Write;
        write!(&mut out, "{:02x}", b).expect("utf-8 symbol encode");
    }
    out
}

fn align_to(n: i32, align: i32) -> i32 {
    (n + align - 1) / align * align
}

fn align_down(n: i32, align: i32) -> i32 {
    align_to(n - align + 1, align)
}

// Bit/byte conversion helpers
fn bits_to_bytes(bits: i32) -> i32 {
    bits / 8
}

fn bytes_to_bits(bytes: i32) -> i32 {
    bytes * 8
}

// Align bits to byte boundary
fn align_bits_to(bits: i32, bytes: i32) -> i32 {
    align_to(bits, bytes_to_bits(bytes))
}

// Check if bitfield crosses storage unit boundary
fn crosses_storage_unit(bit_offset: i32, bit_width: i32, storage_size: i32) -> bool {
    let storage_bits = bytes_to_bits(storage_size);
    bit_offset / storage_bits != (bit_offset + bit_width - 1) / storage_bits
}

fn has_flonum(ty: &Type, lo: i64, hi: i64, offset: i64) -> bool {
    match ty {
        Type::Struct { members, .. } | Type::Union { members, .. } => members
            .iter()
            .all(|member| has_flonum(&member.ty, lo, hi, offset + member.offset as i64)),
        Type::Array { base, len } => {
            (0..*len).all(|idx| has_flonum(base, lo, hi, offset + base.size() * idx as i64))
        }
        _ => {
            let is_flonum = matches!(ty, Type::Float | Type::Double);
            offset < lo || hi <= offset || is_flonum
        }
    }
}

fn assign_lvar_offsets(locals: &mut [Obj], param_indices: &[usize]) -> i32 {
    const GP_MAX: i32 = 6;
    const FP_MAX: i32 = 8;

    let mut top = 16;
    let mut bottom = 0;
    let mut gp = 0;
    let mut fp = 0;

    for &idx in param_indices {
        let var = &mut locals[idx];
        match var.ty {
            Type::Struct { .. } | Type::Union { .. } => {
                if var.ty.size() <= 16 {
                    let fp1 = has_flonum(&var.ty, 0, 8, 0);
                    let fp2 = has_flonum(&var.ty, 8, 16, 8);
                    let fp_needed = i32::from(fp1) + i32::from(fp2);
                    let gp_needed = i32::from(!fp1) + i32::from(!fp2);
                    if fp + fp_needed < FP_MAX && gp + gp_needed < GP_MAX {
                        fp += fp_needed;
                        gp += gp_needed;
                        continue;
                    }
                }
            }
            Type::Float | Type::Double => {
                if fp < FP_MAX {
                    fp += 1;
                    continue;
                }
            }
            Type::LDouble => {}
            _ => {
                if gp < GP_MAX {
                    gp += 1;
                    continue;
                }
            }
        }

        top = align_to(top, 8);
        var.offset = top;
        top += var.ty.size() as i32;
    }

    for var in locals.iter_mut().rev() {
        if var.offset != 0 {
            continue;
        }

        // AMD64 System V ABI has a special alignment rule for an array of
        // length at least 16 bytes. We need to align such array to at least
        // 16-byte boundaries. See p.14 of
        // https://github.com/hjl-tools/x86-psABI/wiki/x86-64-psABI-draft.pdf.
        let align = if matches!(var.ty, Type::Array { .. }) && var.ty.size() >= 16 {
            var.align.max(16)
        } else {
            var.align
        };

        // VLA variables are pointers, so they take 8 bytes on the stack
        let size = if matches!(var.ty, Type::Vla { .. }) {
            8
        } else {
            var.ty.size() as i32
        };

        bottom += size;
        bottom = align_to(bottom, align);
        var.offset = -bottom;
    }

    align_to(bottom, 16)
}
