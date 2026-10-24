use super::Preprocessor;
use super::error::PPFailure;
use super::syntax::{Directive, identifier};
use crate::ast::HeaderKind;
use crate::compiler_args::{CompilerFlavor, math_switch_argument};
use crate::compiler_options::{MathOptions, MathSwitch, Optimization};
use crate::lexer::Token;
use crate::target_info::TargetFamily;
use std::path::PathBuf;

type MathMacro = (&'static str, Option<&'static str>);

pub(super) struct PragmaMath {
    errno_default: bool,
    command_line: MathOptions,
    requested: MathOptions,
    current: MathOptions,
    pushed: Vec<(MathOptions, Optimization)>,
    command_line_optimization: Optimization,
    optimization: Optimization,
    before_ofast: Option<MathOptions>,
    sources: usize,
}

impl PragmaMath {
    pub(super) fn new(math: MathOptions, optimization: Optimization) -> Self {
        Self {
            errno_default: true,
            command_line: math,
            requested: MathOptions {
                associative_math: math.requested_associative_math,
                ..math
            },
            current: math,
            pushed: Vec::new(),
            command_line_optimization: optimization,
            optimization,
            before_ofast: None,
            sources: 0,
        }
    }

    fn restart(&mut self, math: MathOptions) {
        self.current = math;
        self.optimization = self.command_line_optimization;
        self.before_ofast = None;
    }

    fn apply_option(&mut self, flavor: CompilerFlavor, option: &str) {
        let name = option.trim_start_matches('-');
        if name == "Ofast" {
            self.before_ofast.get_or_insert(self.current);
            self.current.apply(flavor, MathSwitch::FastMath, true);
            self.optimization = Optimization::Speed;
        } else if !self.apply_level(name.strip_prefix('O').unwrap_or(name)) {
            let argument = if option.starts_with('-') {
                option.to_owned()
            } else {
                format!("-f{option}")
            };
            if let Some((switch, value)) = math_switch_argument(&argument) {
                self.current.apply(flavor, switch, value);
            }
        }
    }

    fn apply_level(&mut self, level: &str) -> bool {
        let Some(optimization) = Optimization::from_level(level) else {
            return false;
        };
        if let Some(saved) = self.before_ofast.take() {
            self.current = saved;
        }
        self.optimization = optimization;
        true
    }
}

fn optimization_macros(optimization: Optimization) -> [MathMacro; 2] {
    let flag = |name, on: bool| (name, on.then_some("1"));
    [
        flag("__OPTIMIZE__", optimization != Optimization::None),
        flag("__OPTIMIZE_SIZE__", optimization == Optimization::Size),
    ]
}

impl Preprocessor<'_> {
    fn fast_excess_precision(&self, math: &MathOptions) -> bool {
        math.fast_excess_precision
            .unwrap_or(self.dialect.standard().is_gnu())
    }

    fn gcc_fast_math(&self, math: &MathOptions, pragma: bool) -> bool {
        (pragma || self.fast_excess_precision(math)) && math.gcc_fast_math(self.math.errno_default)
    }

    fn math_macros(&self, math: &MathOptions, pragma: bool) -> Vec<MathMacro> {
        let gcc = self.dialect.flavor().is_gcc();
        let errno_default = self.math.errno_default;
        let flag = |name, on: bool| (name, on.then_some("1"));
        let finite = (
            "__FINITE_MATH_ONLY__",
            Some(if math.finite_math_only { "1" } else { "0" }),
        );
        let no_errno = flag("__NO_MATH_ERRNO__", !math.math_errno_or(errno_default));
        let mut macros = if gcc {
            vec![
                flag("__FAST_MATH__", self.gcc_fast_math(math, pragma)),
                finite,
                no_errno,
                flag("__RECIPROCAL_MATH__", math.reciprocal_math),
                flag("__NO_SIGNED_ZEROS__", !math.signed_zeros),
                flag("__NO_TRAPPING_MATH__", !math.trapping_math),
                flag("__ASSOCIATIVE_MATH__", math.associative_math),
                flag("__ROUNDING_MATH__", math.rounding_math),
                flag("__SUPPORT_SNAN__", math.signaling_nans),
            ]
        } else {
            vec![
                flag("__FAST_MATH__", math.clang_fast_math(errno_default)),
                finite,
                no_errno,
            ]
        };
        let arm_fp_fast = match self.dialect.target().family {
            TargetFamily::AArch64 => true,
            TargetFamily::Arm32 => !gcc,
            TargetFamily::X86 | TargetFamily::X86_64 => false,
        };
        if arm_fp_fast && !pragma {
            let unsafe_math = if gcc {
                math.unsafe_math
            } else {
                math.clang_unsafe_fp_math()
            };
            macros.push(flag("__ARM_FP_FAST", unsafe_math));
        }
        macros
    }

    pub(super) fn math_predefines(&mut self, defines: &mut Vec<String>) {
        self.math.errno_default = !self.macros.contains_key("__NO_MATH_ERRNO__");
        let math = self.dialect.options().math;
        let mut macros = self.math_macros(&math, false);
        if self.dialect.flavor().is_gcc() {
            let eval_method = defines
                .iter()
                .rev()
                .find_map(|define| define.strip_prefix("__FLT_EVAL_METHOD__="))
                .map(str::to_owned)
                .or_else(|| self.scalar_macro_spelling("__FLT_EVAL_METHOD__"));
            let x87 = eval_method.as_deref() == Some("2");
            let iso_excess_precision = !self.dialect.standard().is_gnu()
                && x87
                && math.fast_excess_precision == Some(true);
            let iec_559 = !math.breaks_iec_559() && !iso_excess_precision;
            if !iec_559 {
                macros.extend([
                    ("__GCC_IEC_559", Some("0")),
                    ("__STDC_IEC_559__", None),
                    ("__STDC_IEC_60559_BFP__", None),
                ]);
            }
            if !iec_559 || math.cx_limited_range {
                macros.extend([
                    ("__GCC_IEC_559_COMPLEX", Some("0")),
                    ("__STDC_IEC_559_COMPLEX__", None),
                    ("__STDC_IEC_60559_COMPLEX__", None),
                ]);
            }
            if x87
                && self.fast_excess_precision(&math)
                && math.gcc_requested_fast_math(self.math.errno_default)
            {
                macros.extend([
                    ("__FLT_EVAL_METHOD__", Some("0")),
                    ("__FLT_EVAL_METHOD_TS_18661_3__", Some("0")),
                ]);
            }
        }
        for (name, value) in macros {
            match value {
                Some(value) => defines.push(format!("{name}={value}")),
                None => {
                    self.macros.remove(name);
                    defines.retain(|define| {
                        define
                            .split_once('=')
                            .is_none_or(|(defined, _)| defined != name)
                    });
                }
            }
        }
    }

    pub(super) fn gcc_options_pragma(&mut self, directive: &Directive) -> Result<(), PPFailure> {
        if !self.dialect.flavor().is_gcc() {
            return Ok(());
        }
        let source = self.source(directive.loc.file);
        let Some(action) = directive
            .arguments
            .get(1)
            .and_then(|token| identifier(source, token))
        else {
            return Ok(());
        };
        let flavor = self.dialect.flavor();
        let previous = self.math.current;
        let previous_optimization = self.math.optimization;
        match action.as_str() {
            "optimize" => {
                self.math.restart(self.math.requested);
                for token in &directive.arguments[2..] {
                    match &token.value {
                        Token::StringLit(text) => {
                            for option in text.split(',').map(str::trim) {
                                self.math.apply_option(flavor, option);
                            }
                        }
                        Token::IntLit(level) => {
                            self.math.apply_level(level);
                        }
                        _ => {}
                    }
                }
            }
            "push_options" => self.math.pushed.push((previous, previous_optimization)),
            "pop_options" => {
                if let Some((saved, optimization)) = self.math.pushed.pop() {
                    self.math.current = saved;
                    self.math.optimization = optimization;
                }
            }
            "reset_options" => self.math.restart(self.math.command_line),
            _ => {}
        }
        let changed: Vec<MathMacro> = self
            .math_macros(&previous, true)
            .into_iter()
            .chain(optimization_macros(previous_optimization))
            .zip(
                self.math_macros(&self.math.current, true)
                    .into_iter()
                    .chain(optimization_macros(self.math.optimization)),
            )
            .filter(|(before, after)| before != after)
            .map(|(_, after)| after)
            .collect();
        let mut definitions = String::new();
        for (name, value) in changed {
            self.macros.remove(name);
            if let Some(value) = value {
                definitions.push_str(&format!("#define {name} {value}\n"));
            }
        }
        if definitions.is_empty() {
            return Ok(());
        }
        self.math.sources += 1;
        let path = PathBuf::from(format!("<pragma GCC optimize {}>", self.math.sources));
        let file = self.files.intern(path, HeaderKind::User);
        self.parse_source(&definitions, file).map(drop)
    }
}
