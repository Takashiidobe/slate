#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum KnownLibc {
    StrLen,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum FunctionIdentity {
    KnownLibc(KnownLibc),
    Unknown,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Provenance {
    TrustedHeader,
    Unknown,
}

#[derive(Debug, Clone, Default, PartialEq, Eq)]
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
}

pub fn classify_function<'a>(
    name: &str,
    headers: impl IntoIterator<Item = &'a str>,
    canonical_type: &str,
    provenance: Provenance,
) -> FunctionIdentity {
    if provenance != Provenance::TrustedHeader {
        return FunctionIdentity::Unknown;
    }
    let headers = headers.into_iter().collect::<Vec<_>>();
    match name {
        "strlen" if headers.contains(&"string.h") && is_strlen_signature(canonical_type) => {
            FunctionIdentity::KnownLibc(KnownLibc::StrLen)
        }
        _ => FunctionIdentity::Unknown,
    }
}

fn is_strlen_signature(canonical_type: &str) -> bool {
    let Some((ret, params)) = canonical_type.split_once('(') else {
        return false;
    };
    let ret = ret.trim();
    let params = params.strip_suffix(')').unwrap_or(params).trim();
    matches!(
        ret,
        "size_t"
            | "__size_t"
            | "unsigned int"
            | "unsigned long"
            | "unsigned long long"
            | "unsigned __int64"
    ) && params == "const char *"
}
