use slate_parser::ast::Span;
use slate_parser::files::Files;
use slate_parser::ir;

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
        /// This enum is a list of function calls we know come from libc.
        #[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
        pub enum Known {
            $(#[doc = concat!("The libc function [`", $symbol, "`](https://pubs.opengroup.org/onlinepubs/9699919799/functions/", $symbol, ".html).")]
            $variant,)*
        }

        impl Known {
            /// Returns the C symbol name for this function.
            pub fn symbol(self) -> &'static str {
                match self {
                    $(Self::$variant => $symbol,)*
                }
            }

            /// Returns the standard header associated with this function.
            pub fn header(self) -> &'static str {
                match self {
                    $(Self::$variant => $header,)*
                }
            }

            /// Finds a known function by its C symbol name.
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
/// The recognized identity of a called function.
pub enum FunctionIdentity {
    /// A function recognized from the known libc catalog.
    Known(Known),
    HeaderInline,
    #[default]
    /// A function whose identity could not be established.
    Unknown,
}

pub(crate) fn identify(
    function: &Span<ir::Function>,
    files: &Files,
    hosted: bool,
) -> FunctionIdentity {
    let provenance = &function.provenance;
    if function.body.is_some() {
        return if provenance.system_header.is_some() {
            FunctionIdentity::HeaderInline
        } else {
            FunctionIdentity::Unknown
        };
    }
    let system_headers: Vec<&str> = provenance
        .system_header
        .into_iter()
        .flat_map(|outermost| [outermost, provenance.file])
        .filter_map(|header| files.get_path(header))
        .filter_map(|path| path.file_name())
        .filter_map(|name| name.to_str())
        .collect();
    classify_function(&function.name, &system_headers, hosted)
}

fn classify_function(name: &str, system_headers: &[&str], hosted: bool) -> FunctionIdentity {
    if let Some(known) = Known::from_builtin_symbol(name) {
        return FunctionIdentity::Known(known);
    }
    if !hosted {
        return FunctionIdentity::Unknown;
    }
    Known::classify(name, system_headers)
        .map(FunctionIdentity::Known)
        .unwrap_or(FunctionIdentity::Unknown)
}
