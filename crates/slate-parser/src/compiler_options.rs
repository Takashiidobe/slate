use crate::compiler_args::{CompilerFlavor, LanguageStandard};
use crate::diagnostics::DiagnosticOptions;
use crate::ir::{Exceptions, FloatingSemantics, Overflow, Rounding};
use crate::target_info::{LongDoubleFormat, TargetInfo};

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct CompilerOptions {
    pub inline_semantics: Option<InlineSemantics>,
    pub operations: OperationOptions,
    pub layout: LayoutOptions,
    pub diagnostics: DiagnosticOptions,
    pub arguments: Vec<String>,
    pub common: bool,
    pub explicit_standard: bool,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum InlineSemantics {
    SupressDef,
    ProvideDef,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct OperationOptions {
    pub signed_overflow: Overflow,
    pub pointer_wrap: bool,
    pub floating: FloatingSemantics,
}

#[derive(Debug, Default, Clone, Copy, PartialEq, Eq)]
pub struct OperationValues {
    pub signed_overflow: Overflow,
    pub strict_overflow: Option<bool>,
    pub rounding_math: Option<bool>,
    pub trapping_math: Option<bool>,
}

#[derive(Debug, Default, Clone, Copy, PartialEq, Eq)]
pub struct LayoutOptions {
    pub long_double: Option<LongDoubleFormat>,
    pub preferred_stack_alignment: Option<u32>,
}

impl Default for CompilerOptions {
    fn default() -> Self {
        Self {
            inline_semantics: None,
            operations: OperationOptions {
                signed_overflow: Overflow::Undefined,
                pointer_wrap: false,
                floating: FloatingSemantics {
                    rounding: Rounding::NearestEven,
                    exceptions: Exceptions::Ignore,
                },
            },
            layout: LayoutOptions::default(),
            diagnostics: DiagnosticOptions::default(),
            arguments: Vec::new(),
            common: false,
            explicit_standard: false,
        }
    }
}

impl CompilerOptions {
    pub fn effective_inline_semantics(&self, standard: LanguageStandard) -> InlineSemantics {
        self.inline_semantics.unwrap_or_else(|| {
            crate::standard_features::StandardFeatures::new(standard).inline_semantics
        })
    }

    pub fn for_flavor(flavor: CompilerFlavor) -> Self {
        let mut options = Self::default();
        if flavor == CompilerFlavor::Gcc {
            options.operations.floating.exceptions = Exceptions::Observable;
        }
        options
    }

    pub fn from_values(
        flavor: CompilerFlavor,
        layout: LayoutOptions,
        diagnostics: DiagnosticOptions,
        arguments: Vec<String>,
        values: OperationValues,
    ) -> Self {
        let mut options = Self::for_flavor(flavor);
        options.layout = layout;
        options.diagnostics = diagnostics;
        if let Some(value) = values.strict_overflow {
            options.operations.pointer_wrap = !value;
        }
        if let Some(value) = values.rounding_math {
            options.operations.floating.rounding = if value {
                Rounding::Environment
            } else {
                Rounding::NearestEven
            };
        }
        if let Some(value) = values.trapping_math {
            options.operations.floating.exceptions = if value {
                Exceptions::Observable
            } else {
                Exceptions::Ignore
            };
        }
        options.operations.signed_overflow = values.signed_overflow;
        options.arguments = arguments;
        options
    }

    pub fn effective_target(&self, mut target: TargetInfo) -> TargetInfo {
        if let Some(format) = self.layout.long_double {
            target = target.with_long_double(format);
        }
        if let Some(alignment) = self.layout.preferred_stack_alignment {
            target = target.with_preferred_stack_alignment(alignment);
        }
        target
    }
}
