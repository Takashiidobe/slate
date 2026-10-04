use crate::compiler_args::{CompilerFlavor, LanguageStandard};
use crate::compiler_options::{InlineSemantics, MicrosoftFlags};

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Availability {
    Standard,
    Extension,
    Rejected,
}

impl Availability {
    pub fn is_accepted(self) -> bool {
        !matches!(self, Self::Rejected)
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct StandardFeatures {
    pub keyword_alignof: Availability,
    pub keyword_bool_true_false: Availability,
    pub keyword_nullptr: Availability,
    pub keyword_static_assert: Availability,
    pub keyword_thread_local: Availability,
    pub keyword_restrict: Availability,
    pub keyword_inline: Availability,
    pub keyword_typeof: Availability,
    pub keyword_typeof_unqual: Availability,
    pub keyword_constexpr: Availability,
    pub keyword_float16: Availability,
    pub long_long_type: Availability,
    pub decimal_floating_point: Availability,
    pub bit_int_type: Availability,
    pub implicit_int: Availability,
    pub identifier_list_definitions: Availability,
    pub unnamed_definition_parameters: Availability,
    pub unicode_literal_prefixes: Availability,
    pub u8_character_constant: Availability,
    pub u8_literals_are_unsigned: bool,
    pub digit_separators: bool,
    pub octal_prefix: bool,
    pub keyword_countof: bool,
    pub enumerators_have_enum_type: bool,
    pub conditional_pointers: crate::sema::PointerMerge,
    pub compatible_tag_redefinitions: bool,
    pub control_statement_scopes: bool,
    pub auto_type_inference: bool,
    pub empty_parens_are_prototype: bool,
    pub main_implicit_return_zero: bool,
    pub valueless_return_in_nonvoid: bool,
    pub inline_semantics: InlineSemantics,
    pub microsoft_extensions: bool,
    pub microsoft_compatibility: bool,
    pub gnu_floating_keywords: bool,
    pub widest_integer_literal_fallback: bool,
    pub keyword_float80: bool,
    pub fixed_point_keywords: bool,
    pub x86_segment_keywords: bool,
}

impl StandardFeatures {
    fn new(standard: LanguageStandard) -> Self {
        use Availability::{Extension, Rejected, Standard};
        let c89 = matches!(
            standard,
            LanguageStandard::C89 | LanguageStandard::C94 | LanguageStandard::Gnu89
        );
        let c11 = standard.at_least_c11();
        let c23 = standard.at_least_c23();
        let c23_keyword = if c23 { Standard } else { Rejected };
        Self {
            keyword_alignof: c23_keyword,
            keyword_bool_true_false: c23_keyword,
            keyword_nullptr: c23_keyword,
            keyword_static_assert: c23_keyword,
            keyword_thread_local: c23_keyword,
            keyword_restrict: if c89 { Rejected } else { Standard },
            keyword_inline: match (c89, standard.is_gnu()) {
                (false, _) => Standard,
                (true, true) => Extension,
                (true, false) => Rejected,
            },
            keyword_typeof: match (c23, standard.is_gnu()) {
                (true, _) => Standard,
                (false, true) => Extension,
                (false, false) => Rejected,
            },
            keyword_typeof_unqual: c23_keyword,
            keyword_constexpr: c23_keyword,
            keyword_float16: if c23 { Standard } else { Extension },
            long_long_type: if c89 { Extension } else { Standard },
            decimal_floating_point: if c23 { Standard } else { Extension },
            bit_int_type: if c23 { Standard } else { Extension },
            implicit_int: if c89 { Standard } else { Rejected },
            identifier_list_definitions: if c23 { Rejected } else { Standard },
            unnamed_definition_parameters: if c23 { Standard } else { Extension },
            unicode_literal_prefixes: if c11 { Standard } else { Rejected },
            u8_character_constant: c23_keyword,
            u8_literals_are_unsigned: c23,
            digit_separators: c23,
            octal_prefix: true,
            keyword_countof: true,
            enumerators_have_enum_type: c23,
            conditional_pointers: crate::sema::PointerMerge::EXACT,
            compatible_tag_redefinitions: c23,
            control_statement_scopes: !c89,
            auto_type_inference: c23,
            empty_parens_are_prototype: c23,
            main_implicit_return_zero: standard.at_least_c99(),
            valueless_return_in_nonvoid: c89,
            inline_semantics: if c89 {
                InlineSemantics::SupressDef
            } else {
                InlineSemantics::ProvideDef
            },
            microsoft_extensions: false,
            microsoft_compatibility: false,
            gnu_floating_keywords: false,
            widest_integer_literal_fallback: false,
            keyword_float80: false,
            fixed_point_keywords: true,
            x86_segment_keywords: false,
        }
    }

    pub fn for_compiler(
        standard: LanguageStandard,
        flavor: CompilerFlavor,
        target: &crate::target_info::TargetInfo,
        microsoft: MicrosoftFlags,
    ) -> Self {
        let mut features = Self::new(standard);
        (
            features.microsoft_extensions,
            features.microsoft_compatibility,
        ) = match flavor {
            CompilerFlavor::Msvc => (true, true),
            CompilerFlavor::Clang => {
                let windows_msvc =
                    target.environment == crate::target_info::TargetEnvironment::Msvc;
                let compatibility = microsoft
                    .compatibility
                    .unwrap_or(windows_msvc && microsoft.extensions != Some(false));
                (
                    compatibility || microsoft.extensions.unwrap_or(windows_msvc),
                    compatibility,
                )
            }
            CompilerFlavor::Gcc => (false, false),
        };
        features.octal_prefix = !flavor.is_msvc();
        features.keyword_countof = !flavor.is_msvc();
        if flavor.is_gcc() {
            features.widest_integer_literal_fallback = true;
            // gcc drops _Atomic, and before c23 qualifiers of an array's elements, from `?:` pointers
            features.conditional_pointers = crate::sema::PointerMerge {
                array_element_quals: features.enumerators_have_enum_type,
                atomic: false,
            };
            features = features.with_gcc_keywords(
                standard,
                matches!(
                    target.family,
                    crate::target_info::TargetFamily::X86_64
                        | crate::target_info::TargetFamily::X86
                ),
            );
        }
        features
    }

    fn with_gcc_keywords(mut self, standard: LanguageStandard, x86: bool) -> Self {
        self.gnu_floating_keywords = true;
        self.keyword_float80 = x86;
        // gcc's -std=cNN implies -fno-asm, which unreserves the fixed-point keywords
        self.fixed_point_keywords = standard.is_gnu();
        self.x86_segment_keywords = x86 && standard.is_gnu();
        self
    }
}
