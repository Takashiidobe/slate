use crate::compiler_args::{CompilerFlavor, LanguageStandard};
use crate::compiler_options::{CompilerOptions, InlineSemantics};
use crate::standard_features::StandardFeatures;
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
        let target = options.effective_target(target);
        let features = StandardFeatures::for_compiler(standard, flavor, &target, options.microsoft);
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
            LanguageStandard::default(),
            target,
            CompilerOptions::for_flavor(flavor),
        )
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
