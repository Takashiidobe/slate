use crate::backend::engine::NodeRule;
use crate::backend::engine::arena::{Arena, NodeId, NodeKindTag};

pub(in crate::backend::engine) struct StructureGoto;

impl NodeRule for StructureGoto {
    fn name(&self) -> &'static str {
        "structure_goto::structure"
    }

    fn priority(&self) -> u32 {
        1
    }

    fn kinds(&self) -> &'static [NodeKindTag] {
        &[NodeKindTag::Loop]
    }

    fn matches(&self, _arena: &Arena, _id: NodeId) -> bool {
        false
    }

    fn apply(&self, _arena: &mut Arena, _id: NodeId) -> bool {
        false
    }
}
