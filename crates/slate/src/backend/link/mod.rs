mod calls;
mod types;

pub(in crate::backend) use calls::{bind_calls, rust_bound};
pub(in crate::backend) use types::{SHARED_MODULE, SharedTypes};

use crate::backend::rust_ast::{CommentAttach, Item};

fn insert_imports(items: &mut Vec<Item>, imports: impl IntoIterator<Item = Item>) {
    let position = items
        .iter()
        .position(|item| match item {
            Item::CrateAttrs(_) => false,
            Item::Comment(comment) => comment.attach != CommentAttach::Prologue,
            _ => true,
        })
        .unwrap_or(items.len());
    items.splice(position..position, imports);
}
