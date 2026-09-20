use super::ctype::QualType;
use super::numeric::ResolveError;
use crate::ir::BindingId;
use std::collections::HashMap;

#[derive(Debug, Default, Clone, Copy)]
pub(super) struct ObjectRequest {
    pub alignment: Option<u64>,
    pub common: Option<bool>,
}

impl ObjectRequest {
    fn merge(&mut self, later: Self) {
        self.alignment = self.alignment.max(later.alignment);
        self.common = self.common.max(later.common);
    }
}

#[derive(Debug, Clone, Copy)]
pub(super) struct Entity {
    pub ty: QualType,
    pub request: ObjectRequest,
    pub is_register: bool,
}

#[derive(Debug, Default)]
pub(super) struct Entities {
    entities: HashMap<BindingId, Entity>,
}

impl Entities {
    pub(super) fn ty(&self, id: &BindingId) -> Option<QualType> {
        self.entities.get(id).map(|entity| entity.ty)
    }

    pub(super) fn declare(
        &mut self,
        id: BindingId,
        ty: QualType,
        is_register: bool,
    ) -> Option<QualType> {
        match self.entities.get_mut(&id) {
            Some(entity) => {
                entity.is_register |= is_register;
                Some(std::mem::replace(&mut entity.ty, ty))
            }
            None => {
                self.entities.insert(
                    id,
                    Entity {
                        ty,
                        request: ObjectRequest::default(),
                        is_register,
                    },
                );
                None
            }
        }
    }

    pub(super) fn is_register(&self, id: &BindingId) -> bool {
        self.entities
            .get(id)
            .is_some_and(|entity| entity.is_register)
    }

    pub(super) fn request(&self, id: &BindingId) -> ObjectRequest {
        self.entities
            .get(id)
            .map(|entity| entity.request)
            .unwrap_or_default()
    }

    pub(super) fn merge_request(
        &mut self,
        id: BindingId,
        later: ObjectRequest,
    ) -> Result<(), ResolveError> {
        let entity = self.entities.get_mut(&id).ok_or(ResolveError::Unsupported(
            "attribute on an undeclared object",
        ))?;
        entity.request.merge(later);
        Ok(())
    }

    pub(super) fn types(&self) -> impl Iterator<Item = (BindingId, QualType)> + '_ {
        self.entities.iter().map(|(id, entity)| (*id, entity.ty))
    }

    pub(super) fn discard_after(&mut self, next_id: u32) {
        self.entities.retain(|id, _| id.0 < next_id);
    }
}
