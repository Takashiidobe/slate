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
    pub implicit_int: Availability,
    pub control_statement_scopes: bool,
    pub auto_type_inference: bool,
    pub empty_parens_are_prototype: bool,
    pub main_implicit_return_zero: bool,
}

impl StandardFeatures {
    pub fn new(standard: LanguageStandard) -> Self {
        use Availability::{Rejected, Standard};
        let c89 = matches!(standard, LanguageStandard::C89 | LanguageStandard::Gnu89);
        let c23 = matches!(standard, LanguageStandard::C23 | LanguageStandard::Gnu23);
        let c23_keyword = if c23 { Standard } else { Rejected };
        Self {
            keyword_bool_true_false: c23_keyword,
            keyword_nullptr: c23_keyword,
            keyword_static_assert: c23_keyword,
            keyword_thread_local: c23_keyword,
            implicit_int: if c89 { Standard } else { Rejected },
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
