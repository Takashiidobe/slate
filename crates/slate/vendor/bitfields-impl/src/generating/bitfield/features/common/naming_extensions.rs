use proc_macro2::TokenStream;
use quote::{ToTokens, format_ident, quote};

use crate::parsing::bitfields::bitfield::{Bitfield, Field};

impl Bitfield {
    /// Generates the identifier of the bitfield internal value.
    pub fn bitfield_internal_value_ident_tokens(&self, builder_caller: bool) -> TokenStream {
        let self_prefix = builder_caller.then(|| quote! { self. });
        if self.has_ignored_fields() {
            quote! {
                #self_prefix this.val
            }
        } else {
            quote! {
                #self_prefix this.0
            }
        }
    }
}

impl Field {
    /// Generates the setter identifier token stream for the field.
    pub fn setter_ident_tokens(&self) -> TokenStream {
        self.c_name_ident("set")
            .unwrap_or_else(|| {
                format_ident!("set_{}", self.name(), span = self.name_ident().span())
            })
            .to_token_stream()
    }

    pub fn checked_setter_ident_tokens(&self) -> TokenStream {
        self.c_name_ident("checked_set")
            .unwrap_or_else(|| {
                format_ident!("checked_set_{}", self.name(), span = self.name_ident().span())
            })
            .to_token_stream()
    }

    pub fn bits_constant_ident_tokens(&self) -> TokenStream {
        self.c_name_ident("bits")
            .unwrap_or_else(|| {
                format_ident!(
                    "{}_BITS",
                    self.name().to_uppercase(),
                    span = self.name_ident().span()
                )
            })
            .to_token_stream()
    }

    pub fn offset_constant_ident_tokens(&self) -> TokenStream {
        self.c_name_ident("offset")
            .unwrap_or_else(|| {
                format_ident!(
                    "{}_OFFSET",
                    self.name().to_uppercase(),
                    span = self.name_ident().span()
                )
            })
            .to_token_stream()
    }
}
