use crate::compiler_args::{CompilerFlavor, LanguageStandard};
use miette::Severity;
use std::collections::BTreeMap;

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord)]
pub enum Warning {
    LongLong,
    C99Compat,
    ImplicitlyUnsignedLiteral,
    IntegerLiteralTooLarge,
    BitIntExtension,
    C23Extensions,
    PointerSign,
    IncompatiblePointerTypesDiscardsQualifiers,
    IncompatiblePointerTypes,
    PointerTypeMismatch,
    IntConversion,
    PointerIntegerCompare,
    CompareDistinctPointerTypes,
    ConflictingTypes,
    ParameterAlignment,
    IgnoredAttributes,
    UnknownAttributes,
    DeprecatedNonPrototype,
    ImplicitFunctionDeclaration,
    MacroRedefined,
    BuiltinMacroRedefined,
    Deprecated,
}

impl Warning {
    pub const ALL: [Self; 22] = [
        Self::LongLong,
        Self::C99Compat,
        Self::ImplicitlyUnsignedLiteral,
        Self::IntegerLiteralTooLarge,
        Self::BitIntExtension,
        Self::C23Extensions,
        Self::PointerSign,
        Self::IncompatiblePointerTypesDiscardsQualifiers,
        Self::IncompatiblePointerTypes,
        Self::PointerTypeMismatch,
        Self::IntConversion,
        Self::PointerIntegerCompare,
        Self::CompareDistinctPointerTypes,
        Self::ConflictingTypes,
        Self::ParameterAlignment,
        Self::IgnoredAttributes,
        Self::UnknownAttributes,
        Self::DeprecatedNonPrototype,
        Self::ImplicitFunctionDeclaration,
        Self::MacroRedefined,
        Self::BuiltinMacroRedefined,
        Self::Deprecated,
    ];

    pub fn name(self) -> &'static str {
        match self {
            Self::LongLong => "long-long",
            Self::C99Compat => "c99-compat",
            Self::ImplicitlyUnsignedLiteral => "implicitly-unsigned-literal",
            Self::IntegerLiteralTooLarge => "integer-literal-too-large",
            Self::BitIntExtension => "bit-int-extension",
            Self::C23Extensions => "c23-extensions",
            Self::PointerSign => "pointer-sign",
            Self::IncompatiblePointerTypesDiscardsQualifiers => {
                "incompatible-pointer-types-discards-qualifiers"
            }
            Self::IncompatiblePointerTypes => "incompatible-pointer-types",
            Self::PointerTypeMismatch => "pointer-type-mismatch",
            Self::IntConversion => "int-conversion",
            Self::PointerIntegerCompare => "pointer-integer-compare",
            Self::CompareDistinctPointerTypes => "compare-distinct-pointer-types",
            Self::ConflictingTypes => "conflicting-types",
            Self::ParameterAlignment => "parameter-alignment",
            Self::IgnoredAttributes => "ignored-attributes",
            Self::UnknownAttributes => "unknown-attributes",
            Self::DeprecatedNonPrototype => "deprecated-non-prototype",
            Self::ImplicitFunctionDeclaration => "implicit-function-declaration",
            Self::MacroRedefined => "macro-redefined",
            Self::BuiltinMacroRedefined => "builtin-macro-redefined",
            Self::Deprecated => "deprecated",
        }
    }

    pub fn from_name(name: &str) -> Option<Self> {
        Self::ALL.into_iter().find(|warning| warning.name() == name)
    }

    fn is_pedantic(self) -> bool {
        matches!(
            self,
            Self::LongLong
                | Self::BitIntExtension
                | Self::C23Extensions
                | Self::PointerSign
                | Self::IncompatiblePointerTypesDiscardsQualifiers
                | Self::MacroRedefined
                | Self::BuiltinMacroRedefined
                | Self::Deprecated
        )
    }

    fn default_severity(
        self,
        standard: LanguageStandard,
        flavor: CompilerFlavor,
    ) -> DefaultSeverity {
        match self {
            Self::LongLong | Self::BitIntExtension => DefaultSeverity::Ignored,
            Self::DeprecatedNonPrototype if !flavor.is_clang() => DefaultSeverity::Ignored,
            Self::ImplicitFunctionDeclaration if flavor.is_msvc() => DefaultSeverity::Ignored,
            Self::C99Compat if standard.at_least_c99() => DefaultSeverity::Ignored,
            Self::IncompatiblePointerTypes | Self::IntConversion => match flavor {
                CompilerFlavor::Msvc => DefaultSeverity::Warning,
                CompilerFlavor::Gcc if !standard.at_least_c99() => DefaultSeverity::Warning,
                _ => DefaultSeverity::Error,
            },
            _ => DefaultSeverity::Warning,
        }
    }
}

/// what a warning is before any -W flag applies. `Error` is clang's DefaultError
/// and gcc's permerror: still silenced by -Wno-X and demoted by -Wno-error=X.
#[derive(Clone, Copy, PartialEq, Eq)]
enum DefaultSeverity {
    Ignored,
    Warning,
    Error,
}

impl DefaultSeverity {
    fn is_enabled(self) -> bool {
        self != Self::Ignored
    }

    fn is_error(self) -> bool {
        self == Self::Error
    }
}

impl std::fmt::Display for Warning {
    fn fmt(&self, formatter: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        formatter.write_str(self.name())
    }
}

#[derive(Debug, Clone, Copy, Default, PartialEq, Eq)]
struct WarningSetting {
    enabled: Option<bool>,
    error: Option<bool>,
}

#[derive(Debug, Clone, Default, PartialEq, Eq)]
pub struct DiagnosticOptions {
    pub werror: bool,
    pub pedantic: bool,
    pub pedantic_errors: bool,
    pub ignore_warnings: bool,
    settings: BTreeMap<Warning, WarningSetting>,
}

impl DiagnosticOptions {
    pub fn set_enabled(&mut self, warning: Warning, enabled: bool) {
        self.settings.entry(warning).or_default().enabled = Some(enabled);
    }

    pub fn set_error(&mut self, warning: Warning, error: bool) {
        self.settings.entry(warning).or_default().error = Some(error);
    }
}

/// the diagnostic options together with what they are interpreted against: a
/// warning's default severity depends on the standard and the compiler flavor.
#[derive(Clone, Copy)]
pub struct DiagnosticContext<'a> {
    pub options: &'a DiagnosticOptions,
    pub standard: LanguageStandard,
    pub flavor: CompilerFlavor,
}

impl DiagnosticContext<'_> {
    pub fn severity(&self, warning: Warning) -> Option<Severity> {
        self.severity_as(warning, warning.is_pedantic())
    }

    pub fn severity_as(&self, warning: Warning, pedantic: bool) -> Option<Severity> {
        let options = self.options;
        let setting = options.settings.get(&warning).copied().unwrap_or_default();
        let default = warning.default_severity(self.standard, self.flavor);
        let pedantic_group = pedantic && (options.pedantic || options.pedantic_errors);
        if !setting
            .enabled
            .unwrap_or(default.is_enabled() || pedantic_group || setting.error == Some(true))
        {
            return None;
        }
        if options.ignore_warnings {
            return (default.is_error() && setting.error != Some(false)).then_some(Severity::Error);
        }
        let error = setting.error.unwrap_or(
            options.werror || default.is_error() || (options.pedantic_errors && pedantic),
        );
        Some(if error {
            Severity::Error
        } else {
            Severity::Warning
        })
    }
}
