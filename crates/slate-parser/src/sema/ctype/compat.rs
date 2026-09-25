use super::{CTypeKind, CTypes, Extent, QualType};

impl CTypes {
    pub fn compatible(&self, a: QualType, b: QualType) -> bool {
        let a = self.canonical(a);
        let b = self.canonical(b);
        if a.quals != b.quals {
            return false;
        }
        self.compatible_unqualified(a.local_unqualified(), b.local_unqualified())
    }

    pub fn compatible_unqualified(&self, a: QualType, b: QualType) -> bool {
        let a = self.canonical(a);
        let b = self.canonical(b);
        if a == b {
            return true;
        }
        if self.enum_matches(a, b) || self.enum_matches(b, a) {
            return true;
        }
        match (self.kind(a.ty), self.kind(b.ty)) {
            (CTypeKind::Pointer(a), CTypeKind::Pointer(b)) => self.compatible(*a, *b),
            (
                CTypeKind::Array {
                    element: a,
                    extent: ae,
                },
                CTypeKind::Array {
                    element: b,
                    extent: be,
                },
            ) => self.compatible(*a, *b) && compatible_extents(*ae, *be),
            (CTypeKind::Function { .. }, CTypeKind::Function { .. }) => {
                self.compatible_functions(a, b)
            }
            (
                CTypeKind::Record {
                    id: a,
                    union: a_union,
                },
                CTypeKind::Record {
                    id: b,
                    union: b_union,
                },
            ) => a_union == b_union && self.tag_class(*a) == self.tag_class(*b),
            (CTypeKind::Enum(a), CTypeKind::Enum(b)) => self.tag_class(*a) == self.tag_class(*b),
            _ => false,
        }
    }

    fn compatible_functions(&self, a: QualType, b: QualType) -> bool {
        let Some((ar, ap, av, aproto)) = self.function_parts(a) else {
            return false;
        };
        let Some((br, bp, bv, bproto)) = self.function_parts(b) else {
            return false;
        };
        if !self.compatible(ar, br) {
            return false;
        }
        match (aproto, bproto) {
            (true, true) => {
                av == bv
                    && ap.len() == bp.len()
                    && ap.iter().zip(bp).all(|(a, b)| self.same(*a, *b))
            }
            (false, false) => true,
            _ => {
                let (params, variadic) = if aproto { (ap, av) } else { (bp, bv) };
                !variadic && params.iter().all(|param| self.promotes_to_itself(*param))
            }
        }
    }

    fn promotes_to_itself(&self, param: QualType) -> bool {
        !matches!(
            self.canonical_kind(param),
            CTypeKind::Bool
                | CTypeKind::Char
                | CTypeKind::SChar
                | CTypeKind::UChar
                | CTypeKind::Int {
                    rank: super::IntRank::Short,
                    ..
                }
                | CTypeKind::Float(super::FloatKind::Float)
        )
    }

    fn enum_matches(&self, tag: QualType, other: QualType) -> bool {
        self.enum_underlying(tag)
            .is_some_and(|underlying| self.same(underlying, other))
    }

    pub fn composite(&mut self, a: QualType, b: QualType) -> Option<QualType> {
        if !self.compatible_unqualified(a, b) {
            return None;
        }
        let quals = self.quals(a).union(self.quals(b));
        let canonical_a = self.canonical(a).local_unqualified();
        let canonical_b = self.canonical(b).local_unqualified();
        if canonical_a == canonical_b {
            return Some(a.local_unqualified().with(quals));
        }
        let composite = match (self.kind(canonical_a.ty), self.kind(canonical_b.ty)) {
            (CTypeKind::Pointer(a), CTypeKind::Pointer(b)) => {
                let (a, b) = (*a, *b);
                let pointee = self.composite(a, b)?;
                self.pointer(pointee)
            }
            (
                CTypeKind::Array {
                    element: a,
                    extent: ae,
                },
                CTypeKind::Array {
                    element: b,
                    extent: be,
                },
            ) => {
                let (a, b, ae, be) = (*a, *b, *ae, *be);
                let element = self.composite(a, b)?;
                self.qual(CTypeKind::Array {
                    element,
                    extent: composite_extent(ae, be),
                })
            }
            (CTypeKind::Function { .. }, CTypeKind::Function { .. }) => {
                self.composite_function(canonical_a, canonical_b)?
            }
            _ => canonical_a,
        };
        Some(composite.with(quals))
    }

    fn composite_function(&mut self, a: QualType, b: QualType) -> Option<QualType> {
        let (ar, ap, av, aproto) = self.function_parts(a)?;
        let (br, bp, bv, bproto) = self.function_parts(b)?;
        let (ap, bp) = (ap.to_vec(), bp.to_vec());
        let ret = self.composite(ar, br)?;
        let (params, variadic, prototyped) = match (aproto, bproto) {
            (true, true) => {
                let params = ap
                    .iter()
                    .zip(&bp)
                    .map(|(a, b)| self.composite(*a, *b))
                    .collect::<Option<Vec<_>>>()?;
                (params, av, true)
            }
            (true, false) => (ap, av, true),
            (false, true) => (bp, bv, true),
            (false, false) => (Vec::new(), false, false),
        };
        Some(self.qual(CTypeKind::Function {
            ret,
            params,
            variadic,
            prototyped,
        }))
    }

    pub fn merge_pointer(&mut self, a: QualType, b: QualType) -> Option<QualType> {
        let a_pointee = self.pointee(a)?;
        let b_pointee = self.pointee(b)?;
        let quals = self.quals(a_pointee).union(self.quals(b_pointee));
        let pointee = if self.is_void(a_pointee) || self.is_void(b_pointee) {
            let void = self.qual(CTypeKind::Void);
            void.with(quals)
        } else {
            let unqualified_a = self.unqualified(a_pointee);
            let unqualified_b = self.unqualified(b_pointee);
            self.composite(unqualified_a, unqualified_b)?.with(quals)
        };
        Some(self.pointer(pointee))
    }
}

fn compatible_extents(a: Extent, b: Extent) -> bool {
    match (a, b) {
        (Extent::Fixed(a), Extent::Fixed(b)) => a == b,
        _ => true,
    }
}

fn composite_extent(a: Extent, b: Extent) -> Extent {
    match (a, b) {
        (Extent::Fixed(_), _) => a,
        (_, Extent::Fixed(_)) => b,
        (Extent::Variable(_), _) => a,
        _ => b,
    }
}
