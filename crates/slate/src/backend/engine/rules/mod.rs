mod singleton_scopes;
mod zero_init;

use super::NodeRule;

pub(super) fn registry() -> Vec<Box<dyn NodeRule>> {
    vec![
        Box::new(zero_init::ZeroInitFold),
        Box::new(singleton_scopes::WhileLoopUnwrap),
        Box::new(singleton_scopes::DoWhileLoopUnwrap),
        Box::new(singleton_scopes::SingletonUnwrap),
    ]
}
