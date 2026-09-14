use crate::compiler_args::CompilerFlavor;
use crate::ir::{Exceptions, FloatingSemantics, Overflow, Rounding};
use crate::target_info::{LongDoubleFormat, TargetInfo};

#[derive(Debug, thiserror::Error)]
pub enum OptionError {
    #[error("unsupported argument for msvc flavor: {0}")]
    UnsupportedMsvc(String),
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct CompilerOptions {
    pub operations: OperationOptions,
    pub layout: LayoutOptions,
    pub arguments: Vec<String>,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct OperationOptions {
    pub signed_overflow: Overflow,
    pub pointer_wrap: bool,
    pub floating: FloatingSemantics,
}

#[derive(Debug, Default, Clone, Copy, PartialEq, Eq)]
pub struct LayoutOptions {
    pub long_double: Option<LongDoubleFormat>,
}

impl Default for CompilerOptions {
    fn default() -> Self {
        Self {
            operations: OperationOptions {
                signed_overflow: Overflow::Undefined,
                pointer_wrap: false,
                floating: FloatingSemantics {
                    rounding: Rounding::NearestEven,
                    exceptions: Exceptions::Ignore,
                },
            },
            layout: LayoutOptions::default(),
            arguments: Vec::new(),
        }
    }
}

impl CompilerOptions {
    pub fn for_flavor(flavor: CompilerFlavor) -> Self {
        let mut options = Self::default();
        if flavor == CompilerFlavor::Gcc {
            options.operations.floating.exceptions = Exceptions::Observable;
        }
        options
    }

    pub fn recognizes(arg: &str) -> bool {
        matches!(
            arg,
            "-fwrapv"
                | "-fno-wrapv"
                | "-ftrapv"
                | "-fno-trapv"
                | "-fstrict-overflow"
                | "-fno-strict-overflow"
                | "-frounding-math"
                | "-fno-rounding-math"
                | "-ftrapping-math"
                | "-fno-trapping-math"
                | "-mlong-double-64"
                | "-mlong-double-80"
                | "-mlong-double-128"
        )
    }

    pub fn resolve(arguments: Vec<String>, flavor: CompilerFlavor) -> Result<Self, OptionError> {
        let mut options = Self::for_flavor(flavor);
        let mut wrap = None;
        let mut trap = None;
        for (index, arg) in arguments.iter().enumerate() {
            if Self::recognizes(arg) && flavor == CompilerFlavor::Msvc {
                return Err(OptionError::UnsupportedMsvc(arg.clone()));
            }
            match arg.as_str() {
                "-fwrapv" => wrap = Some(index),
                "-fno-wrapv" => wrap = None,
                "-ftrapv" => trap = Some(index),
                "-fno-trapv" => trap = None,
                "-fstrict-overflow" => {
                    wrap = None;
                    options.operations.pointer_wrap = false;
                }
                "-fno-strict-overflow" => {
                    wrap = Some(index);
                    options.operations.pointer_wrap = true;
                }
                "-frounding-math" => options.operations.floating.rounding = Rounding::Environment,
                "-fno-rounding-math" => {
                    options.operations.floating.rounding = Rounding::NearestEven
                }
                "-ftrapping-math" => {
                    options.operations.floating.exceptions = Exceptions::Observable
                }
                "-fno-trapping-math" => options.operations.floating.exceptions = Exceptions::Ignore,
                "-mlong-double-64" => options.layout.long_double = Some(LongDoubleFormat::Binary64),
                "-mlong-double-80" => options.layout.long_double = Some(LongDoubleFormat::X87),
                "-mlong-double-128" => {
                    options.layout.long_double = Some(LongDoubleFormat::Binary128)
                }
                _ => {}
            }
        }
        options.operations.signed_overflow = match (wrap, trap) {
            (_, Some(_)) if flavor == CompilerFlavor::Clang => Overflow::Trap,
            (Some(w), Some(t)) if t > w => Overflow::Trap,
            (Some(_), _) => Overflow::Wrap,
            (_, Some(_)) => Overflow::Trap,
            _ => Overflow::Undefined,
        };
        options.arguments = arguments;
        Ok(options)
    }

    pub fn effective_target(&self, mut target: TargetInfo) -> TargetInfo {
        if let Some(format) = self.layout.long_double {
            target.long_double = format;
        }
        target
    }
}
