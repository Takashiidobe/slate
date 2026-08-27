mod singleton_scopes;

use super::NodeRule;

pub(super) fn registry() -> Vec<Box<dyn NodeRule>> {
    vec![
        Box::new(singleton_scopes::WhileLoopUnwrap),
        Box::new(singleton_scopes::DoWhileLoopUnwrap),
        Box::new(singleton_scopes::SingletonUnwrap),
    ]
}
