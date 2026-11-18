use std::fmt;

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum Type {
    Named(&'static str),
    Struct(String),
    Pointer(Box<Type>),
    FnPtr {
        ret: Box<Type>,
        params: Vec<Type>,
        variadic: bool,
    },
}

impl Type {
    pub fn pointer(self) -> Self {
        Self::Pointer(Box::new(self))
    }

    pub fn is_void(&self) -> bool {
        matches!(self, Self::Named("void"))
    }

    fn declare(&self, f: &mut fmt::Formatter<'_>, declarator: &str) -> fmt::Result {
        match self {
            Self::Named(name) => write!(f, "{name}")?,
            Self::Struct(tag) => write!(f, "struct {tag}")?,
            Self::Pointer(inner) => return inner.declare(f, &format!("*{declarator}")),
            Self::FnPtr {
                ret,
                params,
                variadic,
            } => {
                return ret.declare(
                    f,
                    &format!("(*{declarator})({})", TypeList(params, *variadic)),
                );
            }
        }
        if declarator.is_empty() {
            Ok(())
        } else {
            write!(f, " {declarator}")
        }
    }
}

impl fmt::Display for Type {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        self.declare(f, "")
    }
}

struct TypeList<'a>(&'a [Type], bool);

impl fmt::Display for TypeList<'_> {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        let TypeList(types, variadic) = *self;
        if types.is_empty() && !variadic {
            return f.write_str("void");
        }
        for (index, ty) in types.iter().enumerate() {
            if index > 0 {
                f.write_str(", ")?;
            }
            write!(f, "{ty}")?;
        }
        if variadic {
            f.write_str(if types.is_empty() { "..." } else { ", ..." })?;
        }
        Ok(())
    }
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Param {
    pub ty: Type,
    pub name: String,
}

impl Param {
    pub fn new(ty: Type, name: impl Into<String>) -> Self {
        Self {
            ty,
            name: name.into(),
        }
    }
}

struct ParamList<'a>(&'a [Param], bool);

impl fmt::Display for ParamList<'_> {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        let ParamList(params, variadic) = *self;
        if params.is_empty() && !variadic {
            return f.write_str("void");
        }
        for (index, param) in params.iter().enumerate() {
            if index > 0 {
                f.write_str(", ")?;
            }
            param.ty.declare(f, &param.name)?;
        }
        if variadic {
            f.write_str(if params.is_empty() { "..." } else { ", ..." })?;
        }
        Ok(())
    }
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum Expr {
    Name(String),
    Call { callee: Box<Expr>, args: Vec<Expr> },
    Cast { ty: Type, expr: Box<Expr> },
    Deref(Box<Expr>),
}

impl Expr {
    pub fn name(name: impl Into<String>) -> Self {
        Self::Name(name.into())
    }

    pub fn call(callee: impl Into<String>, args: Vec<Expr>) -> Self {
        Self::Call {
            callee: Box::new(Self::name(callee)),
            args,
        }
    }

    pub fn call_expr(callee: Expr, args: Vec<Expr>) -> Self {
        Self::Call {
            callee: Box::new(callee),
            args,
        }
    }

    pub fn cast(ty: Type, expr: Expr) -> Self {
        Self::Cast {
            ty,
            expr: Box::new(expr),
        }
    }

    pub fn deref(expr: Expr) -> Self {
        Self::Deref(Box::new(expr))
    }
}

impl fmt::Display for Expr {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Self::Name(name) => f.write_str(name),
            Self::Call { callee, args } => {
                match **callee {
                    Self::Name(_) | Self::Call { .. } => write!(f, "{callee}")?,
                    _ => write!(f, "({callee})")?,
                }
                f.write_str("(")?;
                for (index, arg) in args.iter().enumerate() {
                    if index > 0 {
                        f.write_str(", ")?;
                    }
                    write!(f, "{arg}")?;
                }
                f.write_str(")")
            }
            Self::Cast { ty, expr } => write!(f, "({ty}){expr}"),
            Self::Deref(expr) => write!(f, "*{expr}"),
        }
    }
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum Stmt {
    Expr(Expr),
    Return(Expr),
    Decl {
        ty: Type,
        name: String,
        init: Option<Expr>,
    },
}

impl Stmt {
    pub fn decl(ty: Type, name: impl Into<String>, init: Option<Expr>) -> Self {
        Self::Decl {
            ty,
            name: name.into(),
            init,
        }
    }
}

struct Declared<'a>(&'a Type, &'a str);

impl fmt::Display for Declared<'_> {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        self.0.declare(f, self.1)
    }
}

impl fmt::Display for Stmt {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Self::Expr(expr) => write!(f, "{expr};"),
            Self::Return(expr) => write!(f, "return {expr};"),
            Self::Decl { ty, name, init } => {
                write!(f, "{}", Declared(ty, name))?;
                if let Some(init) = init {
                    write!(f, " = {init}")?;
                }
                f.write_str(";")
            }
        }
    }
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Field {
    pub align: Option<u64>,
    pub ty: Type,
    pub name: String,
    pub len: Option<u64>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Prototype {
    pub ret: Type,
    pub name: String,
    pub params: Vec<Type>,
    pub variadic: bool,
    pub asm_label: Option<String>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Function {
    pub ret: Type,
    pub name: String,
    pub params: Vec<Param>,
    pub variadic: bool,
    pub body: Vec<Stmt>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum Item {
    Define(&'static str),
    Include(&'static str),
    Raw(&'static str),
    Struct { tag: String, fields: Vec<Field> },
    Prototype(Prototype),
    Function(Function),
}

impl Item {
    fn is_directive(&self) -> bool {
        matches!(self, Self::Define(_) | Self::Include(_))
    }

    fn joins_next(&self, next: &Self) -> bool {
        self.is_directive() && next.is_directive()
            || matches!((self, next), (Self::Prototype(_), Self::Function(_)))
    }
}

impl fmt::Display for Item {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Self::Define(name) => writeln!(f, "#define {name}"),
            Self::Include(header) => writeln!(f, "#include <{header}>"),
            Self::Raw(source) => f.write_str(source),
            Self::Struct { tag, fields } => {
                writeln!(f, "struct {tag} {{")?;
                for field in fields {
                    f.write_str("    ")?;
                    if let Some(align) = field.align {
                        write!(f, "_Alignas({align}) ")?;
                    }
                    write!(f, "{}", Declared(&field.ty, &field.name))?;
                    if let Some(len) = field.len {
                        write!(f, "[{len}]")?;
                    }
                    writeln!(f, ";")?;
                }
                writeln!(f, "}};")
            }
            Self::Prototype(prototype) => {
                if prototype.asm_label.is_some() {
                    f.write_str("extern ")?;
                }
                let declarator = format!(
                    "{}({})",
                    prototype.name,
                    TypeList(&prototype.params, prototype.variadic)
                );
                write!(f, "{}", Declared(&prototype.ret, &declarator))?;
                if let Some(label) = &prototype.asm_label {
                    write!(f, " __asm__({label:?})")?;
                }
                writeln!(f, ";")
            }
            Self::Function(function) => {
                let declarator = format!(
                    "{}({})",
                    function.name,
                    ParamList(&function.params, function.variadic)
                );
                writeln!(f, "{} {{", Declared(&function.ret, &declarator))?;
                for stmt in &function.body {
                    writeln!(f, "    {stmt}")?;
                }
                writeln!(f, "}}")
            }
        }
    }
}

#[derive(Debug, Clone, Default, PartialEq, Eq)]
pub struct Unit {
    pub items: Vec<Item>,
}

impl Unit {
    pub fn push(&mut self, item: Item) {
        self.items.push(item);
    }
}

impl Extend<Item> for Unit {
    fn extend<I: IntoIterator<Item = Item>>(&mut self, items: I) {
        self.items.extend(items);
    }
}

impl fmt::Display for Unit {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        for (index, item) in self.items.iter().enumerate() {
            if index > 0 && !self.items[index - 1].joins_next(item) {
                f.write_str("\n")?;
            }
            write!(f, "{item}")?;
        }
        Ok(())
    }
}
