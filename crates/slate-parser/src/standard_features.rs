use crate::compiler_args::LanguageStandard;

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
    pub keyword_bool_true_false: Availability,
    pub keyword_nullptr: Availability,
    pub keyword_static_assert: Availability,
    pub keyword_thread_local: Availability,
    pub keyword_restrict: Availability,
    pub keyword_inline: Availability,
    pub keyword_typeof: Availability,
    pub keyword_typeof_unqual: Availability,
    pub keyword_constexpr: Availability,
    pub long_long_type: Availability,
    pub decimal_floating_point: Availability,
    pub implicit_int: Availability,
    pub unicode_literal_prefixes: Availability,
    pub u8_character_constant: Availability,
    pub u8_literals_are_unsigned: bool,
    pub control_statement_scopes: bool,
    pub auto_type_inference: bool,
    pub empty_parens_are_prototype: bool,
    pub main_implicit_return_zero: bool,
}

impl StandardFeatures {
    pub fn new(standard: LanguageStandard) -> Self {
        use Availability::{Extension, Rejected, Standard};
        let c89 = matches!(standard, LanguageStandard::C89 | LanguageStandard::Gnu89);
        let c11 = standard.stdc_version() >= Some(201112);
        let c23 = matches!(standard, LanguageStandard::C23 | LanguageStandard::Gnu23);
        let c23_keyword = if c23 { Standard } else { Rejected };
        Self {
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
            long_long_type: if c89 { Extension } else { Standard },
            decimal_floating_point: if c23 { Standard } else { Extension },
            implicit_int: if c89 { Standard } else { Rejected },
            unicode_literal_prefixes: if c11 { Standard } else { Rejected },
            u8_character_constant: c23_keyword,
            u8_literals_are_unsigned: c23,
            control_statement_scopes: !c89,
            auto_type_inference: c23,
            empty_parens_are_prototype: c23,
            main_implicit_return_zero: standard.stdc_version().is_some(),
        }
    }
}

impl Default for StandardFeatures {
    fn default() -> Self {
        Self::new(LanguageStandard::default())
    }
}
