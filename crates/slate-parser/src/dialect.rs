use crate::charset::Charset;
use crate::compiler_args::{CompilerFlavor, LanguageStandard};
use crate::compiler_options::{CompilerOptions, InlineSemantics};
use crate::standard_features::{Availability, StandardFeatures};
use crate::target_info::TargetInfo;

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Dialect {
    flavor: CompilerFlavor,
    standard: LanguageStandard,
    features: StandardFeatures,
    target: TargetInfo,
    options: CompilerOptions,
}

impl Dialect {
    pub fn new(
        flavor: CompilerFlavor,
        standard: LanguageStandard,
        target: TargetInfo,
        options: CompilerOptions,
    ) -> Self {
        let mut target = options.effective_target(target);
        if flavor.is_msvc() {
            target.execution_charset = options
                .microsoft
                .execution_charset
                .unwrap_or(Charset::Windows1252);
        }
        let mut features = StandardFeatures::for_compiler(
            standard,
            flavor,
            &target,
            options.microsoft,
            options.graphs,
        );
        if flavor.is_gcc() && options.diagnostics.permissive {
            features.valueless_return_in_nonvoid = true;
            features.value_return_in_void = true;
            if !features.implicit_int.is_accepted() {
                features.implicit_int = Availability::Extension;
            }
        }
        Self {
            flavor,
            standard,
            features,
            target,
            options,
        }
    }

    pub fn for_flavor(flavor: CompilerFlavor, target: TargetInfo) -> Self {
        Self::new(
            flavor,
            LanguageStandard::default_for(flavor),
            target,
            CompilerOptions::for_flavor(flavor),
        )
    }

    pub fn source_charset(&self) -> Charset {
        match self.flavor.is_msvc() {
            true => self
                .options
                .microsoft
                .source_charset
                .unwrap_or(Charset::Windows1252),
            false => Charset::Utf8,
        }
    }

    pub fn flavor(&self) -> CompilerFlavor {
        self.flavor
    }

    pub fn standard(&self) -> LanguageStandard {
        self.standard
    }

    pub fn features(&self) -> StandardFeatures {
        self.features
    }

    pub fn target(&self) -> &TargetInfo {
        &self.target
    }

    pub fn options(&self) -> &CompilerOptions {
        &self.options
    }

    pub fn inline_semantics(&self) -> InlineSemantics {
        self.options
            .inline_semantics
            .unwrap_or(self.features.inline_semantics)
    }
}
