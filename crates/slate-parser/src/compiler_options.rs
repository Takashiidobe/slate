use crate::compiler_args::CompilerFlavor;
use crate::diagnostics::DiagnosticOptions;
use crate::ir::{AsmDialect, Exceptions, FloatingSemantics, Overflow, Rounding};
use crate::target_info::{LongDoubleFormat, TargetInfo};

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct CompilerOptions {
    pub inline_semantics: Option<InlineSemantics>,
    pub operations: OperationOptions,
    pub layout: LayoutOptions,
    pub diagnostics: DiagnosticOptions,
    pub common: bool,
    pub explicit_standard: bool,
    pub asm_dialect: AsmDialect,
    pub microsoft: MicrosoftFlags,
    pub hosted: bool,
    pub library_builtins: LibraryBuiltins,
    pub implicit_stdc_predef: bool,
    pub asynchronous_unwind_tables: bool,
    pub late_parsed_attributes: bool,
    pub strict_flex_arrays: u8,
    pub codegen: CodegenOptions,
}

#[derive(Debug, Default, Clone, Copy, PartialEq, Eq)]
pub struct CodegenOptions {
    pub pic: Option<Pic>,
    pub stack_protector: Option<StackProtector>,
    pub cf_protection: Option<u8>,
    pub three_dnow: u8,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct Pic {
    pub level: u8,
    pub executable: bool,
}

impl Pic {
    pub const OFF: Self = Self {
        level: 0,
        executable: false,
    };
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord)]
pub enum StackProtector {
    Off,
    On,
    Strong,
    All,
    Explicit,
}

#[derive(Debug, Default, Clone, Copy, PartialEq, Eq)]
pub struct MicrosoftFlags {
    pub extensions: Option<bool>,
    pub compatibility: Option<bool>,
    pub asm_blocks: bool,
    pub anonymous_structs: Option<bool>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct LibraryBuiltins {
    pub enabled: bool,
    pub disabled: Vec<String>,
}

impl Default for LibraryBuiltins {
    fn default() -> Self {
        Self {
            enabled: true,
            disabled: Vec::new(),
        }
    }
}

impl LibraryBuiltins {
    pub fn recognizes(&self, name: &str) -> bool {
        self.enabled && !self.disabled.iter().any(|disabled| disabled == name)
    }
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
    pub char_signed: Option<bool>,
    pub short_wchar: Option<bool>,
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
            common: false,
            explicit_standard: false,
            asm_dialect: AsmDialect::Att,
            microsoft: MicrosoftFlags::default(),
            hosted: true,
            library_builtins: LibraryBuiltins::default(),
            implicit_stdc_predef: true,
            asynchronous_unwind_tables: true,
            late_parsed_attributes: false,
            strict_flex_arrays: 0,
            codegen: CodegenOptions::default(),
        }
    }
}

impl CompilerOptions {
    pub fn for_flavor(flavor: CompilerFlavor) -> Self {
        let mut options = Self::default();
        if flavor.is_gcc() {
            options.operations.floating.exceptions = Exceptions::Observable;
        }
        options
    }

    pub fn from_values(
        flavor: CompilerFlavor,
        layout: LayoutOptions,
        diagnostics: DiagnosticOptions,
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
        options
    }

    pub fn effective_target(&self, mut target: TargetInfo) -> TargetInfo {
        if let Some(format) = self.layout.long_double {
            target = target.with_long_double(format);
        }
        if let Some(alignment) = self.layout.preferred_stack_alignment {
            target = target.with_preferred_stack_alignment(alignment);
        }
        if let Some(signed) = self.layout.char_signed {
            target = target.with_char_signed(signed);
        }
        if let Some(short) = self.layout.short_wchar {
            target = target.with_short_wchar(short);
        }
        target
    }
}
