use crate::compiler_args::CompilerFlavor;
use crate::diagnostics::DiagnosticOptions;
use crate::ir::{AsmDialect, Exceptions, FloatingSemantics, Overflow, Rounding};
use crate::target_info::{LongDoubleFormat, TargetInfo};

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct CompilerOptions {
    pub inline_semantics: Option<InlineSemantics>,
    pub operations: OperationOptions,
    pub math: MathOptions,
    pub iso_flt_eval_method: bool,
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
    pub macro_prefix_map: Vec<(String, String)>,
    pub fixed_registers: Vec<String>,
    pub graphs: SourceGraphs,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct CodegenOptions {
    pub pic: Option<Pic>,
    pub stack_protector: Option<StackProtector>,
    pub cf_protection: Option<u8>,
    pub three_dnow: u8,
    pub delete_null_pointer_checks: bool,
    pub trivial_auto_var_init: TrivialAutoVarInit,
    pub optimization: Optimization,
}

#[derive(Debug, Default, Clone, Copy, PartialEq, Eq)]
pub enum Optimization {
    #[default]
    None,
    Speed,
    Size,
}

impl Optimization {
    pub fn from_level(level: &str) -> Option<Self> {
        match level {
            "0" => Some(Self::None),
            "" | "1" | "2" | "3" | "g" | "fast" => Some(Self::Speed),
            "s" | "z" => Some(Self::Size),
            _ => None,
        }
    }
}

impl Default for CodegenOptions {
    fn default() -> Self {
        Self {
            pic: None,
            stack_protector: None,
            cf_protection: None,
            three_dnow: 0,
            delete_null_pointer_checks: true,
            trivial_auto_var_init: TrivialAutoVarInit::Uninitialized,
            optimization: Optimization::None,
        }
    }
}

#[derive(Debug, Default, Clone, Copy, PartialEq, Eq)]
pub enum TrivialAutoVarInit {
    #[default]
    Uninitialized,
    Zero,
    Pattern,
}

impl std::str::FromStr for TrivialAutoVarInit {
    type Err = String;

    fn from_str(value: &str) -> Result<Self, Self::Err> {
        match value {
            "uninitialized" => Ok(Self::Uninitialized),
            "zero" => Ok(Self::Zero),
            "pattern" => Ok(Self::Pattern),
            _ => Err("expected uninitialized, zero, or pattern".into()),
        }
    }
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
pub struct SourceGraphs {
    pub trigraphs: Option<bool>,
    pub digraphs: Option<bool>,
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

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct OperationValues {
    pub signed_overflow: Overflow,
    pub strict_overflow: Option<bool>,
    pub math: MathOptions,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum MathSwitch {
    FastMath,
    UnsafeMath,
    AssociativeMath,
    ReciprocalMath,
    SignedZeros,
    TrappingMath,
    MathErrno,
    FiniteMathOnly,
    RoundingMath,
    SignalingNans,
    CxLimitedRange,
    FastExcessPrecision,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct MathOptions {
    pub unsafe_math: bool,
    pub finite_math_only: bool,
    pub math_errno: Option<bool>,
    pub trapping_math: bool,
    pub requested_trapping_math: bool,
    pub signed_zeros: bool,
    pub associative_math: bool,
    pub reciprocal_math: bool,
    pub rounding_math: bool,
    pub signaling_nans: bool,
    pub cx_limited_range: bool,
    pub fast_excess_precision: Option<bool>,
}

impl MathOptions {
    pub fn for_flavor(flavor: CompilerFlavor) -> Self {
        Self {
            unsafe_math: false,
            finite_math_only: false,
            math_errno: None,
            trapping_math: flavor.is_gcc(),
            requested_trapping_math: flavor.is_gcc(),
            signed_zeros: true,
            associative_math: false,
            reciprocal_math: false,
            rounding_math: false,
            signaling_nans: false,
            cx_limited_range: false,
            fast_excess_precision: None,
        }
    }

    pub fn resolve(flavor: CompilerFlavor, switches: &[(MathSwitch, bool)]) -> Self {
        let mut math = Self::for_flavor(flavor);
        for &(switch, value) in switches {
            math.apply(flavor, switch, value);
        }
        if flavor.is_gcc() {
            math.trapping_math |= math.signaling_nans;
            math.associative_math &= !math.trapping_math && !math.signed_zeros;
        }
        math
    }

    pub fn apply(&mut self, flavor: CompilerFlavor, switch: MathSwitch, value: bool) {
        let gcc = flavor.is_gcc();
        match switch {
            MathSwitch::FastMath if !value && !gcc => *self = Self::for_flavor(flavor),
            MathSwitch::FastMath => {
                self.apply(flavor, MathSwitch::UnsafeMath, value);
                self.finite_math_only = value;
                self.math_errno = Some(!value);
                if value {
                    self.fast_excess_precision = Some(true);
                    self.signaling_nans = false;
                    self.rounding_math = false;
                }
            }
            MathSwitch::UnsafeMath => {
                self.unsafe_math = value;
                self.associative_math = value;
                self.reciprocal_math = value;
                self.signed_zeros = !value;
                if gcc || value {
                    self.apply(flavor, MathSwitch::TrappingMath, !value);
                }
            }
            MathSwitch::AssociativeMath => self.associative_math = value,
            MathSwitch::ReciprocalMath => self.reciprocal_math = value,
            MathSwitch::SignedZeros => self.signed_zeros = value,
            MathSwitch::TrappingMath => {
                self.trapping_math = value;
                self.requested_trapping_math = value;
            }
            MathSwitch::MathErrno => self.math_errno = Some(value),
            MathSwitch::FiniteMathOnly => self.finite_math_only = value,
            MathSwitch::RoundingMath => self.rounding_math = value,
            MathSwitch::SignalingNans => self.signaling_nans = value,
            MathSwitch::CxLimitedRange => self.cx_limited_range = value,
            MathSwitch::FastExcessPrecision => self.fast_excess_precision = Some(value),
        }
    }

    pub fn math_errno_or(&self, default: bool) -> bool {
        self.math_errno.unwrap_or(default)
    }

    pub fn gcc_fast_math(&self, errno_default: bool) -> bool {
        !self.trapping_math && self.gcc_requested_fast_math(errno_default)
    }

    pub fn gcc_requested_fast_math(&self, errno_default: bool) -> bool {
        !self.requested_trapping_math
            && self.unsafe_math
            && self.finite_math_only
            && !self.signed_zeros
            && !self.math_errno_or(errno_default)
    }

    pub fn clang_unsafe_fp_math(&self) -> bool {
        self.unsafe_math
            && self.associative_math
            && self.reciprocal_math
            && !self.signed_zeros
            && !self.trapping_math
    }

    pub fn clang_fast_math(&self, errno_default: bool) -> bool {
        self.clang_unsafe_fp_math()
            && self.finite_math_only
            && !self.math_errno_or(errno_default)
            && !self.rounding_math
    }

    pub fn breaks_iec_559(&self) -> bool {
        self.unsafe_math
            || self.associative_math
            || self.reciprocal_math
            || self.finite_math_only
            || !self.signed_zeros
    }
}

#[derive(Debug, Default, Clone, Copy, PartialEq, Eq)]
pub struct LayoutOptions {
    pub long_double: Option<LongDoubleFormat>,
    pub preferred_stack_alignment: Option<u32>,
    pub char_signed: Option<bool>,
    pub short_wchar: Option<bool>,
    pub short_enums: bool,
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
            math: MathOptions::for_flavor(CompilerFlavor::Clang),
            iso_flt_eval_method: false,
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
            macro_prefix_map: Vec::new(),
            fixed_registers: Vec::new(),
            graphs: SourceGraphs::default(),
        }
    }
}

impl CompilerOptions {
    pub fn for_flavor(flavor: CompilerFlavor) -> Self {
        let mut options = Self::default();
        if flavor.is_gcc() {
            options.operations.floating.exceptions = Exceptions::Observable;
        }
        options.math = MathOptions::for_flavor(flavor);
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
        options.operations.floating.rounding = if values.math.rounding_math {
            Rounding::Environment
        } else {
            Rounding::NearestEven
        };
        options.operations.floating.exceptions = if values.math.trapping_math {
            Exceptions::Observable
        } else {
            Exceptions::Ignore
        };
        options.math = values.math;
        options.operations.signed_overflow = values.signed_overflow;
        options
    }

    pub fn remap_macro_path(&self, path: &str) -> String {
        self.macro_prefix_map
            .iter()
            .find_map(|(old, new)| {
                path.strip_prefix(old.as_str())
                    .map(|rest| format!("{new}{rest}"))
            })
            .unwrap_or_else(|| path.to_owned())
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
