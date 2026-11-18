use anyhow::Result;
use proc_macro2::TokenStream;

pub fn file(tokens: TokenStream) -> Result<String> {
    Ok(prettyplease::unparse(&syn::parse2(tokens)?))
}

pub fn parse<T: syn::parse::Parse>(source: &str) -> Option<T> {
    syn::parse_str(source).ok()
}
