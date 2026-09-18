use crate::compiler_args::LanguageStandard;
use miette::Severity;
use std::collections::BTreeMap;

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord)]
pub enum Warning {
    LongLong,
    C99Compat,
    ImplicitlyUnsignedLiteral,
}

impl Warning {
    pub const ALL: [Self; 3] = [
        Self::LongLong,
        Self::C99Compat,
        Self::ImplicitlyUnsignedLiteral,
    ];

    pub fn name(self) -> &'static str {
        match self {
            Self::LongLong => "long-long",
            Self::C99Compat => "c99-compat",
            Self::ImplicitlyUnsignedLiteral => "implicitly-unsigned-literal",
        }
    }

    pub fn from_name(name: &str) -> Option<Self> {
        Self::ALL.into_iter().find(|warning| warning.name() == name)
    }

    fn is_pedantic(self) -> bool {
        matches!(self, Self::LongLong)
    }

    fn enabled_by_default(self, standard: LanguageStandard) -> bool {
        match self {
            Self::LongLong => false,
            Self::C99Compat => standard.stdc_version().is_none(),
            Self::ImplicitlyUnsignedLiteral => true,
        }
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
    settings: BTreeMap<Warning, WarningSetting>,
}

impl DiagnosticOptions {
    pub fn set_enabled(&mut self, warning: Warning, enabled: bool) {
        self.settings.entry(warning).or_default().enabled = Some(enabled);
    }

    pub fn set_error(&mut self, warning: Warning, error: bool) {
        self.settings.entry(warning).or_default().error = Some(error);
    }

    pub fn severity(&self, warning: Warning, standard: LanguageStandard) -> Option<Severity> {
        let setting = self.settings.get(&warning).copied().unwrap_or_default();
        let pedantic_group = warning.is_pedantic() && (self.pedantic || self.pedantic_errors);
        if !setting.enabled.unwrap_or(
            warning.enabled_by_default(standard) || pedantic_group || setting.error == Some(true),
        ) {
            return None;
        }
        let error = setting
            .error
            .unwrap_or(self.werror || (self.pedantic_errors && warning.is_pedantic()));
        Some(if error {
            Severity::Error
        } else {
            Severity::Warning
        })
    }
}
