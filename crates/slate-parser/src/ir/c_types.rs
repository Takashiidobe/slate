use super::{BindingId, Place, PlaceKind, TypeDefinition, TypeId, Value};
use crate::ast::NodeId;
use crate::sema::ctype::{CTypeKind, CTypes, QualType};
use foldhash::HashMap;

#[derive(Debug, Clone, Default)]
pub struct CTypeTable {
    pub types: CTypes,
    pub definitions: Vec<TypeDefinition>,
    pub nodes: HashMap<NodeId, QualType>,
    pub bindings: HashMap<BindingId, QualType>,
    pub record_fields: HashMap<TypeId, Vec<QualType>>,
    pub macro_type_arguments:
        HashMap<(crate::ast::MacroExpansionId, usize), (crate::ir::Type, QualType)>,
}

impl CTypeTable {
    pub fn value(&self, value: &Value) -> Option<QualType> {
        self.nodes.get(&value.node.id).copied()
    }

    pub fn place(&self, place: &Place) -> Option<QualType> {
        match &place.kind {
            PlaceKind::Binding(id)
            | PlaceKind::CompoundLiteral { object: id, .. }
            | PlaceKind::Temporary { object: id, .. } => self.bindings.get(id).copied(),
            PlaceKind::Deref(pointer) | PlaceKind::Index { base: pointer, .. } => {
                match self.types.kind(self.types.desugar(self.value(pointer)?).ty) {
                    CTypeKind::Pointer(pointee, _) => Some(*pointee),
                    _ => None,
                }
            }
            PlaceKind::Field { base, index, .. } => {
                match self.types.canonical_kind(self.place(base)?) {
                    CTypeKind::Record { id, .. } => {
                        self.record_fields.get(id)?.get(*index).copied()
                    }
                    _ => None,
                }
            }
            PlaceKind::ComplexPart { base, .. } => {
                match self.types.canonical_kind(self.place(base)?) {
                    CTypeKind::Complex(component) => Some(QualType::new(*component)),
                    _ => None,
                }
            }
            PlaceKind::Lane { base, .. } => match self.types.canonical_kind(self.place(base)?) {
                CTypeKind::Vector { element, .. } => Some(*element),
                _ => None,
            },
            PlaceKind::Swizzle { .. } => None,
            PlaceKind::MacroArg { place, .. } => self.place(place),
        }
    }

    pub fn written_spelling(&self, c: QualType) -> Option<String> {
        let rendered = self.types.render(c, &self.definitions);
        (rendered.spelling != rendered.canonical).then_some(rendered.spelling)
    }
}
