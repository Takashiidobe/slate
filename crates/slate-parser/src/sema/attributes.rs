use crate::ast::Attribute;

/// What an attribute written on an object or typedef declaration means to IR lowering.
pub(super) enum Use {
    /// Consumed by `symbol_attributes` into the entity's `SymbolAttributes`.
    Symbol,
    /// Consumed during type resolution or by the object layout request.
    Layout,
    /// Carries no meaning the IR needs to represent.
    Ignored,
    /// Changes semantics in a way lowering cannot yet express.
    Unsupported(&'static str),
}

pub(super) fn declaration_use(attribute: &Attribute) -> Use {
    match attribute {
        Attribute::Visibility(_)
        | Attribute::TlsModel(_)
        | Attribute::Weak
        | Attribute::Alias(_)
        | Attribute::WeakRef(_)
        | Attribute::Section(_)
        | Attribute::Used
        | Attribute::Retain
        | Attribute::DllImport
        | Attribute::DllExport
        | Attribute::SelectAny => Use::Symbol,

        Attribute::Aligned(_)
        | Attribute::AlignAs(_)
        | Attribute::VectorSize(_)
        | Attribute::ExtVectorType(_)
        | Attribute::Packed
        | Attribute::ThreadLocal
        | Attribute::Common
        | Attribute::NoCommon => Use::Layout,

        Attribute::Mode(_) => Use::Unsupported("machine mode attribute"),
        Attribute::AddressSpace(_) => Use::Unsupported("address space attribute"),
        Attribute::Cleanup(_) => Use::Unsupported("cleanup attribute"),
        Attribute::ScalarStorageOrder(_) => Use::Unsupported("scalar storage order attribute"),
        Attribute::TransparentUnion => Use::Unsupported("transparent union attribute"),
        Attribute::MsStruct | Attribute::GccStruct => Use::Unsupported("record layout attribute"),
        Attribute::Ifunc(_) => Use::Unsupported("ifunc attribute"),
        Attribute::CodeSeg(_) => Use::Unsupported("code segment attribute"),
        Attribute::Invalid { .. } => Use::Unsupported("invalid attribute"),

        Attribute::PassObjectSize { .. }
        | Attribute::LifetimeBound
        | Attribute::Overloadable
        | Attribute::GnuInline
        | Attribute::NoThrow
        | Attribute::NoAlias
        | Attribute::RestrictReturn
        | Attribute::OptimizeNone
        | Attribute::NoInline
        | Attribute::AlwaysInline
        | Attribute::NoReturn
        | Attribute::Constructor(_)
        | Attribute::Destructor(_)
        | Attribute::NonNull(_)
        | Attribute::Annotate(_)
        | Attribute::Target(_)
        | Attribute::Malloc
        | Attribute::AssumeAligned(_)
        | Attribute::AllocSize(_)
        | Attribute::AllocAlign(_)
        | Attribute::ReturnsNonNull
        | Attribute::WarnUnusedResult
        | Attribute::Sentinel(_)
        | Attribute::Cold
        | Attribute::Flatten
        | Attribute::Hot
        | Attribute::Leaf
        | Attribute::NoIpa
        | Attribute::NoClone
        | Attribute::Optimize(_)
        | Attribute::Naked
        | Attribute::Interrupt
        | Attribute::NoSplitStack
        | Attribute::ReturnsTwice
        | Attribute::CpuDispatch(_)
        | Attribute::CpuSpecific(_)
        | Attribute::TargetClones(_)
        | Attribute::WeakImport
        | Attribute::CallingConvention(_)
        | Attribute::NoMips16
        | Attribute::Availability(_)
        | Attribute::Format(_)
        | Attribute::FormatArg(_)
        | Attribute::Pure
        | Attribute::Const
        | Attribute::MayAlias
        | Attribute::Deprecated(_)
        | Attribute::NoDiscard(_)
        | Attribute::MaybeUnused
        | Attribute::Fallthrough
        | Attribute::Unknown { .. } => Use::Ignored,
    }
}

pub(super) fn unsupported(attribute: &Attribute) -> Option<&'static str> {
    match declaration_use(attribute) {
        Use::Unsupported(reason) => Some(reason),
        _ => None,
    }
}

/// Alignment written on a typedef belongs to the aliased type, which the
/// structural IR type cannot carry, so it is refused rather than dropped.
pub(super) fn typedef_unsupported(attribute: &Attribute) -> Option<&'static str> {
    match attribute {
        Attribute::Aligned(_) | Attribute::AlignAs(_) => Some("typedef alignment attribute"),
        other => unsupported(other),
    }
}

/// A parameter has no linkage and no storage of its own, so alignment is the
/// only attribute lowering can represent on one; anything else would be
/// dropped silently rather than applied.
pub(super) fn parameter_unsupported(attribute: &Attribute) -> Option<&'static str> {
    match attribute {
        Attribute::Aligned(_) | Attribute::AlignAs(_) => None,
        other => match declaration_use(other) {
            Use::Ignored => None,
            Use::Unsupported(reason) => Some(reason),
            Use::Symbol => Some("symbol attribute on a parameter"),
            Use::Layout => Some("layout attribute on a parameter"),
        },
    }
}
