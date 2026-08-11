#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum Known {
    Malloc,
    Calloc,
    Realloc,
    Free,
    MemCpy,
    MemMove,
    MemSet,
    MemChr,
    StrLen,
    StrCpy,
    StrCat,
    StrNCpy,
    StrNCat,
    StrCmp,
    StrNCmp,
    MemCmp,
    StrChr,
    StrRChr,
    StrStr,
    StrPBrk,
    StrSpn,
    StrCSpn,
    Atoi,
    Atol,
    StrTol,
    StrToul,
    StrTod,
    Printf,
    FPrintf,
    SPrintf,
    SNPrintf,
    Exit,
    Puts,
    FOpen,
    FPuts,
    FGets,
    FRead,
    FWrite,
    FClose,
    FFlush,
    Remove,
    Perror,
    ToUpper,
    ToLower,
    IsAlpha,
    IsDigit,
    IsUpper,
    IsLower,
    IsAlnum,
    IsXDigit,
    IsPunct,
    IsCntrl,
    IsGraph,
    IsPrint,
    IsSpace,
    Sin,
    Cos,
    Tan,
    Log,
    Log10,
    Log2,
    Pow,
    Sqrt,
    Exp,
    Exp2,
    Fmod,
    Lround,
    Llround,
    PthreadCreate,
    PthreadJoin,
    Qsort,
    Bsearch,
}

impl Known {
    pub fn symbol(self) -> &'static str {
        match self {
            Self::Malloc => "malloc",
            Self::Calloc => "calloc",
            Self::Realloc => "realloc",
            Self::Free => "free",
            Self::MemCpy => "memcpy",
            Self::MemMove => "memmove",
            Self::MemSet => "memset",
            Self::MemChr => "memchr",
            Self::StrLen => "strlen",
            Self::StrCpy => "strcpy",
            Self::StrCat => "strcat",
            Self::StrNCpy => "strncpy",
            Self::StrNCat => "strncat",
            Self::StrCmp => "strcmp",
            Self::StrNCmp => "strncmp",
            Self::MemCmp => "memcmp",
            Self::StrChr => "strchr",
            Self::StrRChr => "strrchr",
            Self::StrStr => "strstr",
            Self::StrPBrk => "strpbrk",
            Self::StrSpn => "strspn",
            Self::StrCSpn => "strcspn",
            Self::Atoi => "atoi",
            Self::Atol => "atol",
            Self::StrTol => "strtol",
            Self::StrToul => "strtoul",
            Self::StrTod => "strtod",
            Self::Printf => "printf",
            Self::FPrintf => "fprintf",
            Self::SPrintf => "sprintf",
            Self::SNPrintf => "snprintf",
            Self::Exit => "exit",
            Self::Puts => "puts",
            Self::FOpen => "fopen",
            Self::FPuts => "fputs",
            Self::FGets => "fgets",
            Self::FRead => "fread",
            Self::FWrite => "fwrite",
            Self::FClose => "fclose",
            Self::FFlush => "fflush",
            Self::Remove => "remove",
            Self::Perror => "perror",
            Self::ToUpper => "toupper",
            Self::ToLower => "tolower",
            Self::IsAlpha => "isalpha",
            Self::IsDigit => "isdigit",
            Self::IsUpper => "isupper",
            Self::IsLower => "islower",
            Self::IsAlnum => "isalnum",
            Self::IsXDigit => "isxdigit",
            Self::IsPunct => "ispunct",
            Self::IsCntrl => "iscntrl",
            Self::IsGraph => "isgraph",
            Self::IsPrint => "isprint",
            Self::IsSpace => "isspace",
            Self::Sin => "sin",
            Self::Cos => "cos",
            Self::Tan => "tan",
            Self::Log => "log",
            Self::Log10 => "log10",
            Self::Log2 => "log2",
            Self::Pow => "pow",
            Self::Sqrt => "sqrt",
            Self::Exp => "exp",
            Self::Exp2 => "exp2",
            Self::Fmod => "fmod",
            Self::Lround => "lround",
            Self::Llround => "llround",
            Self::PthreadCreate => "pthread_create",
            Self::PthreadJoin => "pthread_join",
            Self::Qsort => "qsort",
            Self::Bsearch => "bsearch",
        }
    }

    pub fn header(self) -> &'static str {
        match self {
            Self::Malloc
            | Self::Calloc
            | Self::Realloc
            | Self::Free
            | Self::Atoi
            | Self::Atol
            | Self::StrTol
            | Self::StrToul
            | Self::StrTod
            | Self::Exit
            | Self::Qsort
            | Self::Bsearch => "stdlib.h",
            Self::MemCpy
            | Self::MemMove
            | Self::MemSet
            | Self::MemChr
            | Self::StrLen
            | Self::StrCpy
            | Self::StrCat
            | Self::StrNCpy
            | Self::StrNCat
            | Self::StrCmp
            | Self::StrNCmp
            | Self::MemCmp
            | Self::StrChr
            | Self::StrRChr
            | Self::StrStr
            | Self::StrPBrk
            | Self::StrSpn
            | Self::StrCSpn => "string.h",
            Self::Printf
            | Self::FPrintf
            | Self::SPrintf
            | Self::SNPrintf
            | Self::Puts
            | Self::FOpen
            | Self::FPuts
            | Self::FGets
            | Self::FRead
            | Self::FWrite
            | Self::FClose
            | Self::FFlush
            | Self::Remove
            | Self::Perror => "stdio.h",
            Self::ToUpper
            | Self::ToLower
            | Self::IsAlpha
            | Self::IsDigit
            | Self::IsUpper
            | Self::IsLower
            | Self::IsAlnum
            | Self::IsXDigit
            | Self::IsPunct
            | Self::IsCntrl
            | Self::IsGraph
            | Self::IsPrint
            | Self::IsSpace => "ctype.h",
            Self::Sin
            | Self::Cos
            | Self::Tan
            | Self::Log
            | Self::Log10
            | Self::Log2
            | Self::Pow
            | Self::Sqrt
            | Self::Exp
            | Self::Exp2
            | Self::Fmod
            | Self::Lround
            | Self::Llround => "math.h",
            Self::PthreadCreate | Self::PthreadJoin => "pthread.h",
        }
    }

    fn classify(name: &str, headers: &[&str]) -> Option<Self> {
        let (known, header) = match name {
            "malloc" => (Self::Malloc, "stdlib.h"),
            "calloc" => (Self::Calloc, "stdlib.h"),
            "realloc" => (Self::Realloc, "stdlib.h"),
            "free" => (Self::Free, "stdlib.h"),
            "memcpy" => (Self::MemCpy, "string.h"),
            "memmove" => (Self::MemMove, "string.h"),
            "memset" => (Self::MemSet, "string.h"),
            "memchr" => (Self::MemChr, "string.h"),
            "strlen" => (Self::StrLen, "string.h"),
            "strcpy" => (Self::StrCpy, "string.h"),
            "strcat" => (Self::StrCat, "string.h"),
            "strncpy" => (Self::StrNCpy, "string.h"),
            "strncat" => (Self::StrNCat, "string.h"),
            "strcmp" => (Self::StrCmp, "string.h"),
            "strncmp" => (Self::StrNCmp, "string.h"),
            "memcmp" => (Self::MemCmp, "string.h"),
            "strchr" => (Self::StrChr, "string.h"),
            "strrchr" => (Self::StrRChr, "string.h"),
            "strstr" => (Self::StrStr, "string.h"),
            "strpbrk" => (Self::StrPBrk, "string.h"),
            "strspn" => (Self::StrSpn, "string.h"),
            "strcspn" => (Self::StrCSpn, "string.h"),
            "atoi" => (Self::Atoi, "stdlib.h"),
            "atol" => (Self::Atol, "stdlib.h"),
            "strtol" => (Self::StrTol, "stdlib.h"),
            "strtoul" => (Self::StrToul, "stdlib.h"),
            "strtod" => (Self::StrTod, "stdlib.h"),
            "printf" => (Self::Printf, "stdio.h"),
            "fprintf" => (Self::FPrintf, "stdio.h"),
            "sprintf" => (Self::SPrintf, "stdio.h"),
            "snprintf" => (Self::SNPrintf, "stdio.h"),
            "exit" => (Self::Exit, "stdlib.h"),
            "puts" => (Self::Puts, "stdio.h"),
            "fopen" => (Self::FOpen, "stdio.h"),
            "fputs" => (Self::FPuts, "stdio.h"),
            "fgets" => (Self::FGets, "stdio.h"),
            "fread" => (Self::FRead, "stdio.h"),
            "fwrite" => (Self::FWrite, "stdio.h"),
            "fclose" => (Self::FClose, "stdio.h"),
            "fflush" => (Self::FFlush, "stdio.h"),
            "remove" => (Self::Remove, "stdio.h"),
            "perror" => (Self::Perror, "stdio.h"),
            "toupper" => (Self::ToUpper, "ctype.h"),
            "tolower" => (Self::ToLower, "ctype.h"),
            "isalpha" => (Self::IsAlpha, "ctype.h"),
            "isdigit" => (Self::IsDigit, "ctype.h"),
            "isupper" => (Self::IsUpper, "ctype.h"),
            "islower" => (Self::IsLower, "ctype.h"),
            "isalnum" => (Self::IsAlnum, "ctype.h"),
            "isxdigit" => (Self::IsXDigit, "ctype.h"),
            "ispunct" => (Self::IsPunct, "ctype.h"),
            "iscntrl" => (Self::IsCntrl, "ctype.h"),
            "isgraph" => (Self::IsGraph, "ctype.h"),
            "isprint" => (Self::IsPrint, "ctype.h"),
            "isspace" => (Self::IsSpace, "ctype.h"),
            "sin" => (Self::Sin, "math.h"),
            "cos" => (Self::Cos, "math.h"),
            "tan" => (Self::Tan, "math.h"),
            "log" => (Self::Log, "math.h"),
            "log10" => (Self::Log10, "math.h"),
            "log2" => (Self::Log2, "math.h"),
            "pow" => (Self::Pow, "math.h"),
            "sqrt" => (Self::Sqrt, "math.h"),
            "exp" => (Self::Exp, "math.h"),
            "exp2" => (Self::Exp2, "math.h"),
            "fmod" => (Self::Fmod, "math.h"),
            "lround" => (Self::Lround, "math.h"),
            "llround" => (Self::Llround, "math.h"),
            "pthread_create" => (Self::PthreadCreate, "pthread.h"),
            "pthread_join" => (Self::PthreadJoin, "pthread.h"),
            "qsort" => (Self::Qsort, "stdlib.h"),
            "bsearch" => (Self::Bsearch, "stdlib.h"),
            _ => return None,
        };
        headers.contains(&header).then_some(known)
    }
}

#[derive(Debug, Clone, Copy, Default, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum FunctionIdentity {
    Known(Known),
    #[default]
    Unknown,
}

#[derive(Debug, Clone, Copy, Default, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum Provenance {
    TrustedHeader,
    #[default]
    Unknown,
}

#[derive(Debug, Clone, Default, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum CallBinding {
    Direct {
        identity: FunctionIdentity,
        canonical_type: Option<String>,
    },
    Indirect,
    #[default]
    Generated,
}

impl CallBinding {
    pub fn direct_unknown(canonical_type: Option<String>) -> Self {
        Self::Direct {
            identity: FunctionIdentity::Unknown,
            canonical_type,
        }
    }

    pub fn known(&self) -> Option<Known> {
        match self {
            Self::Direct {
                identity: FunctionIdentity::Known(known),
                ..
            } => Some(*known),
            Self::Direct { .. } | Self::Indirect | Self::Generated => None,
        }
    }
}

pub fn classify_function<'a>(
    name: &str,
    headers: impl IntoIterator<Item = &'a str>,
    canonical_type: &str,
    provenance: Provenance,
) -> FunctionIdentity {
    if !valid_function_type(canonical_type) {
        return FunctionIdentity::Unknown;
    }
    let builtin = match name {
        "__builtin_memcpy" | "__builtin_bit_cast" => Some(Known::MemCpy),
        "__builtin_memmove" | "__builtin_bcopy" => Some(Known::MemMove),
        "__builtin_memset" | "__builtin_bzero" => Some(Known::MemSet),
        "__builtin_memchr" => Some(Known::MemChr),
        "__builtin_sin" => Some(Known::Sin),
        "__builtin_cos" => Some(Known::Cos),
        "__builtin_tan" => Some(Known::Tan),
        "__builtin_log" => Some(Known::Log),
        "__builtin_log10" => Some(Known::Log10),
        "__builtin_log2" => Some(Known::Log2),
        "__builtin_pow" => Some(Known::Pow),
        "__builtin_sqrt" => Some(Known::Sqrt),
        "__builtin_exp" => Some(Known::Exp),
        "__builtin_exp2" => Some(Known::Exp2),
        "__builtin_fmod" => Some(Known::Fmod),
        "__builtin_lround" => Some(Known::Lround),
        "__builtin_llround" => Some(Known::Llround),
        _ => None,
    };
    if let Some(known) = builtin {
        return FunctionIdentity::Known(known);
    }
    if provenance != Provenance::TrustedHeader {
        return FunctionIdentity::Unknown;
    }
    let headers = headers.into_iter().collect::<Vec<_>>();
    Known::classify(name, &headers)
        .map(FunctionIdentity::Known)
        .unwrap_or(FunctionIdentity::Unknown)
}

fn valid_function_type(canonical_type: &str) -> bool {
    let Some((ret, params)) = canonical_type.split_once('(') else {
        return false;
    };
    if ret.trim().is_empty() {
        return false;
    }
    let mut depth = 1;
    for character in params.chars() {
        match character {
            '(' => depth += 1,
            ')' => {
                depth -= 1;
                if depth == 0 {
                    return true;
                }
            }
            _ => {}
        }
    }
    false
}

pub fn known_call(expr: &crate::backend::rust_ast::Expr) -> Option<Known> {
    let crate::backend::rust_ast::Expr::Call { binding, .. } = expr else {
        return None;
    };
    binding.known()
}
