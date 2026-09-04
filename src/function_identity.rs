use std::collections::BTreeSet;

macro_rules! known_function_catalog {
    ($entry:ident) => {
        $entry! {
            Malloc, "malloc", "stdlib.h"; Calloc, "calloc", "stdlib.h";
            Realloc, "realloc", "stdlib.h"; Free, "free", "stdlib.h";
            MemCpy, "memcpy", "string.h"; MemMove, "memmove", "string.h";
            MemSet, "memset", "string.h"; MemChr, "memchr", "string.h";
            StrLen, "strlen", "string.h"; StrNLen, "strnlen", "string.h";
            StrCpy, "strcpy", "string.h"; StrCat, "strcat", "string.h";
            StrNCpy, "strncpy", "string.h"; StrNCat, "strncat", "string.h";
            StrCmp, "strcmp", "string.h"; StrNCmp, "strncmp", "string.h";
            MemCmp, "memcmp", "string.h"; StrChr, "strchr", "string.h";
            StrRChr, "strrchr", "string.h"; StrStr, "strstr", "string.h";
            StrPBrk, "strpbrk", "string.h"; StrSpn, "strspn", "string.h";
            StrCSpn, "strcspn", "string.h"; Atoi, "atoi", "stdlib.h";
            Atol, "atol", "stdlib.h"; Atoll, "atoll", "stdlib.h";
            Atof, "atof", "stdlib.h"; StrTol, "strtol", "stdlib.h";
            StrToul, "strtoul", "stdlib.h"; StrTod, "strtod", "stdlib.h";
            StrTold, "strtold", "stdlib.h"; StrFromL, "strfroml", "stdlib.h";
            Printf, "printf", "stdio.h"; FPrintf, "fprintf", "stdio.h";
            SPrintf, "sprintf", "stdio.h"; SNPrintf, "snprintf", "stdio.h";
            Sscanf, "sscanf", "stdio.h";
            Exit, "exit", "stdlib.h"; Abort, "abort", "stdlib.h";
            Getenv, "getenv", "stdlib.h";
            Puts, "puts", "stdio.h"; FOpen, "fopen", "stdio.h";
            FPuts, "fputs", "stdio.h"; FGets, "fgets", "stdio.h";
            FRead, "fread", "stdio.h"; FWrite, "fwrite", "stdio.h";
            FClose, "fclose", "stdio.h"; FFlush, "fflush", "stdio.h";
            Remove, "remove", "stdio.h"; Perror, "perror", "stdio.h";
            ToUpper, "toupper", "ctype.h"; ToLower, "tolower", "ctype.h";
            IsAlpha, "isalpha", "ctype.h"; IsDigit, "isdigit", "ctype.h";
            IsUpper, "isupper", "ctype.h"; IsLower, "islower", "ctype.h";
            IsAlnum, "isalnum", "ctype.h"; IsXDigit, "isxdigit", "ctype.h";
            IsPunct, "ispunct", "ctype.h"; IsCntrl, "iscntrl", "ctype.h";
            IsGraph, "isgraph", "ctype.h"; IsPrint, "isprint", "ctype.h";
            IsSpace, "isspace", "ctype.h"; Sin, "sin", "math.h";
            Cos, "cos", "math.h"; Tan, "tan", "math.h"; Log, "log", "math.h";
            Log10, "log10", "math.h"; Log2, "log2", "math.h"; Pow, "pow", "math.h";
            Sqrt, "sqrt", "math.h"; Exp, "exp", "math.h"; Exp2, "exp2", "math.h";
            Fmod, "fmod", "math.h"; Lround, "lround", "math.h";
            Llround, "llround", "math.h"; Nan, "nan", "math.h";
            Nanf, "nanf", "math.h"; Nanl, "nanl", "math.h";
            PthreadCreate, "pthread_create", "pthread.h";
            PthreadJoin, "pthread_join", "pthread.h"; Qsort, "qsort", "stdlib.h";
            Bsearch, "bsearch", "stdlib.h";
        }
    };
}

macro_rules! define_known {
    ($($variant:ident, $symbol:literal, $header:literal;)*) => {
        #[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
        pub enum Known {
            $($variant,)*
        }

        impl Known {
            pub fn symbol(self) -> &'static str {
                match self {
                    $(Self::$variant => $symbol,)*
                }
            }

            pub fn header(self) -> &'static str {
                match self {
                    $(Self::$variant => $header,)*
                }
            }

            pub fn from_symbol(symbol: &str) -> Option<Self> {
                match symbol {
                    $($symbol => Some(Self::$variant),)*
                    _ => None,
                }
            }

            fn from_builtin_symbol(symbol: &str) -> Option<Self> {
                symbol
                    .strip_prefix("__builtin_")
                    .and_then(Self::from_symbol)
                    .or(match symbol {
                        "__builtin_bit_cast" => Some(Self::MemCpy),
                        "__builtin_bcopy" => Some(Self::MemMove),
                        "__builtin_bzero" => Some(Self::MemSet),
                        _ => None,
                    })
            }

            fn classify(name: &str, headers: &[&str]) -> Option<Self> {
                Self::from_symbol(name).filter(|known| headers.contains(&known.header()))
            }
        }
    };
}

known_function_catalog!(define_known);

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
        trusted_declaration: Option<String>,
        trusted_headers: BTreeSet<String>,
    },
    Indirect,
    #[default]
    Generated,
}

impl CallBinding {
    pub fn unknown() -> Self {
        Self::Direct {
            identity: FunctionIdentity::Unknown,
            canonical_type: None,
            trusted_declaration: None,
            trusted_headers: BTreeSet::new(),
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

    pub fn trusted_declaration(&self) -> Option<&str> {
        match self {
            Self::Direct {
                trusted_declaration,
                ..
            } => trusted_declaration.as_deref(),
            Self::Indirect | Self::Generated => None,
        }
    }

    pub fn trusted_headers(&self) -> Box<dyn Iterator<Item = &str> + '_> {
        match self {
            Self::Direct {
                trusted_headers, ..
            } => Box::new(trusted_headers.iter().map(String::as_str)),
            Self::Indirect | Self::Generated => Box::new([].into_iter()),
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
    if let Some(known) = Known::from_builtin_symbol(name) {
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
