use crate::ast::Attribute;

#[derive(Clone, Copy, PartialEq, Eq)]
pub(super) enum Subject {
    Function,
    Object { automatic: bool },
    Parameter,
    Typedef,
    Field,
    Record { union: bool },
}

pub(super) enum Use {
    Symbol,
    Layout,
    Ignored,
    Unknown,
    UnsupportedDeclspec,
    Inapplicable {
        spelling: &'static str,
        applies_to: Option<&'static str>,
    },
    Rejected(&'static str),
    Unimplemented(&'static str),
}

pub(super) fn declaration_use(attribute: &Attribute, subject: Subject) -> Use {
    if let Some((spelling, applies_to)) = inapplicable(attribute, subject) {
        return Use::Inapplicable {
            spelling,
            applies_to,
        };
    }
    let alignment = matches!(attribute, Attribute::Aligned(_) | Attribute::AlignAs(_));
    match (subject, general_use(attribute)) {
        (Subject::Typedef, _) if matches!(attribute, Attribute::AlignAs(_)) => {
            Use::Rejected("'_Alignas' applied to a typedef")
        }
        (Subject::Parameter, _) if alignment || attribute.is_type_attribute() => Use::Layout,
        (Subject::Parameter, Use::Symbol | Use::Layout) => parameter_use(attribute),
        (Subject::Field | Subject::Record { .. }, general) => {
            member_use(attribute, subject, general)
        }
        (_, general) => general,
    }
}

fn parameter_use(attribute: &Attribute) -> Use {
    let spelling = match attribute {
        Attribute::Section(_) => {
            return Use::Rejected(
                "'section' attribute only applies to functions and global variables",
            );
        }
        Attribute::Alias(_) => {
            return Use::Rejected(
                "'alias' attribute only applies to functions and global variables",
            );
        }
        Attribute::TlsModel(_) => {
            return Use::Rejected("'tls_model' attribute only applies to thread-local variables");
        }
        Attribute::ThreadLocal => {
            return Use::Rejected("'__declspec(thread)' variables must have global storage");
        }
        Attribute::Visibility(_) => "visibility",
        Attribute::Weak => "weak",
        Attribute::WeakRef(_) => "weakref",
        Attribute::DllImport => "dllimport",
        Attribute::DllExport => "dllexport",
        Attribute::SelectAny => "selectany",
        Attribute::Common => "common",
        Attribute::NoCommon => "nocommon",
        _ => return Use::Unimplemented("attribute on a parameter"),
    };
    Use::Inapplicable {
        spelling,
        applies_to: None,
    }
}

fn member_use(attribute: &Attribute, subject: Subject, general: Use) -> Use {
    match attribute {
        Attribute::Section(_) => {
            Use::Rejected("'section' attribute only applies to functions and global variables")
        }
        Attribute::Alias(_) => {
            Use::Rejected("'alias' attribute only applies to functions and global variables")
        }
        Attribute::TlsModel(_) => {
            Use::Rejected("'tls_model' attribute only applies to thread-local variables")
        }
        Attribute::Mode(_) if subject != Subject::Field => Use::Rejected(
            "'mode' attribute only applies to variables, enums, typedefs, and non-static data members",
        ),
        _ => general,
    }
}

/// What the attribute means to lowering once it is known to apply.
fn general_use(attribute: &Attribute) -> Use {
    match attribute {
        Attribute::Visibility(_)
        | Attribute::TlsModel(_)
        | Attribute::Weak
        | Attribute::Alias(_)
        | Attribute::WeakRef(_)
        | Attribute::Ifunc(_)
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
        | Attribute::MsStruct
        | Attribute::GccStruct
        | Attribute::ThreadLocal
        | Attribute::Common
        | Attribute::NoCommon => Use::Layout,

        Attribute::Mode(_) => Use::Ignored,
        Attribute::AddressSpace(_) => Use::Ignored,
        Attribute::Cleanup(_) => Use::Ignored,
        Attribute::ScalarStorageOrder(_) => Use::Ignored,
        Attribute::TransparentUnion => Use::Ignored,
        Attribute::CodeSeg(_) => Use::Unimplemented("code segment attribute"),
        Attribute::Invalid { .. } => Use::Rejected("invalid attribute"),

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
        | Attribute::Fallthrough => Use::Ignored,

        Attribute::Unknown { .. } => Use::Unknown,
        Attribute::IgnoredDeclspec { .. } => Use::UnsupportedDeclspec,
    }
}

fn inapplicable(
    attribute: &Attribute,
    subject: Subject,
) -> Option<(&'static str, Option<&'static str>)> {
    let function_only =
        |spelling| (subject != Subject::Function).then_some((spelling, Some("functions")));
    let record = matches!(subject, Subject::Record { .. });
    match attribute {
        Attribute::Packed => (!record && subject != Subject::Field).then_some(("packed", None)),
        Attribute::TransparentUnion => {
            (!matches!(subject, Subject::Record { union: true } | Subject::Typedef))
                .then_some(("transparent_union", Some("unions")))
        }
        Attribute::MsStruct => {
            (!record).then_some(("ms_struct", Some("structs, unions, and classes")))
        }
        Attribute::GccStruct => {
            (!record).then_some(("gcc_struct", Some("structs, unions, and classes")))
        }
        Attribute::Ifunc(_) => function_only("ifunc"),
        Attribute::Malloc => function_only("malloc"),
        Attribute::Cold => function_only("cold"),
        Attribute::Hot => function_only("hot"),
        Attribute::Flatten => function_only("flatten"),
        Attribute::AlwaysInline => (subject != Subject::Function)
            .then_some(("always_inline", Some("functions and statements"))),
        Attribute::NoInline => {
            (subject != Subject::Function).then_some(("noinline", Some("functions and statements")))
        }
        Attribute::AllocSize(_) => (subject != Subject::Function)
            .then_some(("alloc_size", Some("non-K&R-style functions"))),
        Attribute::AllocAlign(_) => function_only("alloc_align"),
        Attribute::ReturnsNonNull => function_only("returns_nonnull"),
        Attribute::ReturnsTwice => function_only("returns_twice"),
        Attribute::NoReturn => {
            (subject != Subject::Function).then_some(("noreturn", Some("function types")))
        }
        Attribute::NonNull(_) => (subject != Subject::Function && subject != Subject::Parameter)
            .then_some(("nonnull", Some("functions, methods, and parameters"))),
        Attribute::Constructor(_) => function_only("constructor"),
        Attribute::Destructor(_) => function_only("destructor"),
        Attribute::Naked => function_only("naked"),
        Attribute::Interrupt => function_only("interrupt"),
        Attribute::Leaf => function_only("leaf"),
        Attribute::NoIpa => function_only("noipa"),
        Attribute::NoClone => function_only("noclone"),
        Attribute::NoSplitStack => function_only("no_split_stack"),
        Attribute::Target(_) => function_only("target"),
        Attribute::TargetClones(_) => function_only("target_clones"),
        Attribute::CpuDispatch(_) => function_only("cpu_dispatch"),
        Attribute::CpuSpecific(_) => function_only("cpu_specific"),
        Attribute::OptimizeNone => function_only("optnone"),
        Attribute::GnuInline => function_only("gnu_inline"),
        Attribute::Format(_) => function_only("format"),
        Attribute::Sentinel(_) => function_only("sentinel"),
        Attribute::Pure => function_only("pure"),
        Attribute::Const => function_only("const"),

        Attribute::Common | Attribute::NoCommon => {
            let spelling = if matches!(attribute, Attribute::Common) {
                "common"
            } else {
                "nocommon"
            };
            (!matches!(subject, Subject::Object { .. } | Subject::Parameter))
                .then_some((spelling, Some("variables")))
        }
        Attribute::Used | Attribute::Retain => {
            let spelling = if matches!(attribute, Attribute::Used) {
                "used"
            } else {
                "retain"
            };
            matches!(
                subject,
                Subject::Object { automatic: true }
                    | Subject::Parameter
                    | Subject::Typedef
                    | Subject::Field
                    | Subject::Record { .. }
            )
            .then_some((
                spelling,
                Some("variables with non-local storage and functions"),
            ))
        }
        Attribute::Cleanup(_) => (!matches!(subject, Subject::Object { automatic: true }))
            .then_some(("cleanup", Some("local variables"))),
        Attribute::Visibility(_) => (subject == Subject::Typedef).then_some(("visibility", None)),
        Attribute::Weak => matches!(
            subject,
            Subject::Typedef | Subject::Field | Subject::Record { .. }
        )
        .then_some(("weak", None)),

        _ => None,
    }
}
