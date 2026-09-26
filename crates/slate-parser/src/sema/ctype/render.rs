use super::{
    CTypeKind, CTypes, Extent, FixedKind, FixedRank, FixedType, FloatKind, IntRank, QualType,
    Qualifiers,
};
use crate::ir::TypeDefinition;

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct CTypeMetadata {
    pub spelling: String,
    pub canonical: String,
    pub typedef_chain: Vec<String>,
    pub qualifiers: Qualifiers,
}

impl CTypeMetadata {
    pub fn entries(&self) -> Vec<(String, String)> {
        let mut entries = vec![("c".into(), self.spelling.clone())];
        if self.canonical != self.spelling {
            entries.push(("c_canon".into(), self.canonical.clone()));
        }
        if !self.typedef_chain.is_empty() {
            entries.push(("typedef_chain".into(), self.typedef_chain.join(" -> ")));
        }
        for (present, key) in [
            (self.qualifiers.is_const, "c_const"),
            (self.qualifiers.is_volatile, "c_volatile"),
            (self.qualifiers.is_restrict, "c_restrict"),
            (self.qualifiers.is_atomic, "c_atomic"),
        ] {
            if present {
                entries.push((key.into(), "true".into()));
            }
        }
        entries
    }
}

struct Declarator {
    text: String,
    suffix: bool,
}

impl Declarator {
    fn empty() -> Self {
        Self {
            text: String::new(),
            suffix: false,
        }
    }

    fn grouped(self) -> String {
        if self.text.is_empty() || self.suffix {
            self.text
        } else {
            format!("({})", self.text)
        }
    }
}

struct Printer<'a> {
    types: &'a CTypes,
    definitions: &'a [TypeDefinition],
    desugar: bool,
}

impl CTypes {
    pub fn render(&self, q: QualType, definitions: &[TypeDefinition]) -> CTypeMetadata {
        let print = |desugar| {
            Printer {
                types: self,
                definitions,
                desugar,
            }
            .print(q, Declarator::empty())
        };
        CTypeMetadata {
            spelling: print(false),
            canonical: print(true),
            typedef_chain: self.typedef_chain(q),
            qualifiers: match self.canonical_kind(q) {
                CTypeKind::Function { .. } => Qualifiers::NONE,
                _ => self.quals(q),
            },
        }
    }

    /// The type printed as a declaration of `name`, the way Clang spells a
    /// function in `__PRETTY_FUNCTION__`.
    pub fn declaration_spelling(
        &self,
        q: QualType,
        name: &str,
        definitions: &[TypeDefinition],
    ) -> String {
        Printer {
            types: self,
            definitions,
            desugar: false,
        }
        .print(
            q,
            Declarator {
                text: name.to_owned(),
                suffix: true,
            },
        )
    }

    pub fn spelling(&self, q: QualType, definitions: &[TypeDefinition]) -> String {
        Printer {
            types: self,
            definitions,
            desugar: false,
        }
        .print(q, Declarator::empty())
    }

    fn typedef_chain(&self, mut q: QualType) -> Vec<String> {
        let mut chain = Vec::new();
        loop {
            q = match self.kind(q.ty) {
                CTypeKind::Typedef {
                    name, underlying, ..
                } => {
                    chain.push(name.clone());
                    *underlying
                }
                CTypeKind::TypeOf { underlying, .. } => *underlying,
                CTypeKind::AtomicSpecifier(inner) | CTypeKind::Pointer(inner) => *inner,
                CTypeKind::Array { element, .. } => *element,
                CTypeKind::Function { ret, .. } => *ret,
                _ => return chain,
            };
        }
    }
}

impl Printer<'_> {
    fn print(&self, q: QualType, declarator: Declarator) -> String {
        match self.types.kind(q.ty) {
            CTypeKind::Typedef { underlying, .. } | CTypeKind::TypeOf { underlying, .. }
                if self.desugar =>
            {
                self.print(underlying.with(q.quals), declarator)
            }
            CTypeKind::Typedef { name, .. } => self.base(q.quals, name, declarator),
            CTypeKind::TypeOf { spelling, .. } => self.base(q.quals, spelling, declarator),
            CTypeKind::AtomicSpecifier(inner) => {
                let inner = self.print(*inner, Declarator::empty());
                self.base(
                    q.quals.without(Qualifiers::ATOMIC),
                    &format!("_Atomic({inner})"),
                    declarator,
                )
            }
            CTypeKind::Pointer(pointee) => {
                let mut text = format!("*{}", words(q.quals).join(" "));
                if !declarator.text.is_empty() {
                    if !q.quals.is_empty() && !declarator.suffix {
                        text.push(' ');
                    }
                    text.push_str(&declarator.text);
                }
                self.print(
                    *pointee,
                    Declarator {
                        text,
                        suffix: false,
                    },
                )
            }
            CTypeKind::Array { element, extent } => {
                let bound = match extent {
                    Extent::Incomplete => "[]".to_owned(),
                    Extent::Fixed(length) => format!("[{length}]"),
                    Extent::Variable(_) => "[*]".to_owned(),
                };
                self.print(element.with(q.quals), suffixed(declarator, &bound))
            }
            CTypeKind::Function { .. } if self.desugar && self.types.canonical(q).ty != q.ty => {
                self.print(self.types.canonical(q), declarator)
            }
            CTypeKind::Function {
                ret,
                params,
                variadic,
                prototyped,
            } => {
                let mut parts = params
                    .iter()
                    .map(|param| self.print(*param, Declarator::empty()))
                    .collect::<Vec<_>>();
                let list = if !prototyped {
                    "()".to_owned()
                } else if parts.is_empty() && !variadic {
                    "(void)".to_owned()
                } else {
                    if *variadic {
                        parts.push("...".into());
                    }
                    format!("({})", parts.join(", "))
                };
                self.print(*ret, suffixed(declarator, &list))
            }
            kind => self.base(q.quals, &self.name(kind), declarator),
        }
    }

    fn name(&self, kind: &CTypeKind) -> String {
        match kind {
            CTypeKind::Void => "void".into(),
            CTypeKind::Bool => "_Bool".into(),
            CTypeKind::Char => "char".into(),
            CTypeKind::SChar => "signed char".into(),
            CTypeKind::UChar => "unsigned char".into(),
            CTypeKind::Int { rank, signed } => {
                let name = match rank {
                    IntRank::Short => "short",
                    IntRank::Int => "int",
                    IntRank::Long => "long",
                    IntRank::LongLong => "long long",
                    IntRank::Int128 => "__int128",
                };
                if *signed {
                    name.into()
                } else {
                    format!("unsigned {name}")
                }
            }
            CTypeKind::BitInt { width, signed } => {
                format!("{}_BitInt({width})", if *signed { "" } else { "unsigned " })
            }
            CTypeKind::Float(kind) => float_name(*kind).into(),
            CTypeKind::Complex(component) => {
                format!("_Complex {}", self.name(self.types.kind(*component)))
            }
            CTypeKind::Imaginary(kind) => format!("_Imaginary {}", float_name(*kind)),
            CTypeKind::FixedPoint(fixed) => fixed_point_name(*fixed),
            CTypeKind::Vector { element, bytes, .. } => format!(
                "{} __attribute__((vector_size({bytes})))",
                self.print(*element, Declarator::empty())
            ),
            CTypeKind::VaList => "__builtin_va_list".into(),
            CTypeKind::Record { id, union } => {
                tag_name(if *union { "union" } else { "struct" }, self.tag(*id))
            }
            CTypeKind::Enum(id) => tag_name("enum", self.tag(*id)),
            CTypeKind::Pointer(_)
            | CTypeKind::Array { .. }
            | CTypeKind::Function { .. }
            | CTypeKind::Typedef { .. }
            | CTypeKind::TypeOf { .. }
            | CTypeKind::AtomicSpecifier(_) => String::new(),
        }
    }

    fn tag(&self, id: crate::ir::TypeId) -> Option<&str> {
        self.definitions
            .get(id.0 as usize)
            .and_then(|definition| definition.name.as_deref())
    }

    fn base(&self, quals: Qualifiers, name: &str, declarator: Declarator) -> String {
        let mut text = words(quals);
        text.push(name);
        let mut text = text.join(" ");
        if !declarator.text.is_empty() {
            let named = declarator
                .text
                .starts_with(|c: char| c.is_alphanumeric() || c == '_');
            if !declarator.suffix || named {
                text.push(' ');
            }
            text.push_str(&declarator.text);
        }
        text
    }
}

fn suffixed(declarator: Declarator, suffix: &str) -> Declarator {
    let grouped = !declarator.text.is_empty() && !declarator.suffix;
    let mut text = declarator.grouped();
    text.push_str(suffix);
    Declarator {
        text,
        suffix: !grouped,
    }
}

fn words(quals: Qualifiers) -> Vec<&'static str> {
    [
        (quals.is_const, "const"),
        (quals.is_volatile, "volatile"),
        (quals.is_restrict, "restrict"),
        (quals.is_atomic, "_Atomic"),
    ]
    .into_iter()
    .filter_map(|(present, word)| present.then_some(word))
    .collect()
}

fn float_name(kind: FloatKind) -> &'static str {
    match kind {
        FloatKind::BFloat16 => "__bf16",
        FloatKind::Float16 => "_Float16",
        FloatKind::Fp16 => "__fp16",
        FloatKind::Float => "float",
        FloatKind::Double => "double",
        FloatKind::LongDouble => "long double",
        FloatKind::Float128 => "__float128",
        FloatKind::Decimal32 => "_Decimal32",
        FloatKind::Decimal64 => "_Decimal64",
        FloatKind::Decimal128 => "_Decimal128",
    }
}

fn fixed_point_name(fixed: FixedType) -> String {
    let mut words = Vec::new();
    if fixed.saturating {
        words.push("_Sat");
    }
    if !fixed.signed {
        words.push("unsigned");
    }
    words.extend(match fixed.rank {
        FixedRank::Short => Some("short"),
        FixedRank::Default => None,
        FixedRank::Long => Some("long"),
        FixedRank::LongLong => Some("long long"),
    });
    words.push(match fixed.kind {
        FixedKind::Fract => "_Fract",
        FixedKind::Accum => "_Accum",
    });
    words.join(" ")
}

fn tag_name(prefix: &str, name: Option<&str>) -> String {
    name.map_or_else(|| prefix.to_owned(), |name| format!("{prefix} {name}"))
}
