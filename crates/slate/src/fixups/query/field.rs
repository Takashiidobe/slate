use std::ops::RangeBounds;

#[derive(Default)]
pub(in crate::fixups) enum Field<T, Cx = ()> {
    #[default]
    Any,
    Eq(T),
    Predicate(FieldPredicate<T, Cx>),
}

type FieldPredicate<T, Cx> = Box<dyn Fn(&T, &Cx) -> bool>;

impl<T, Cx> Field<T, Cx> {
    pub(in crate::fixups) fn any() -> Self {
        Self::Any
    }

    pub(in crate::fixups) fn predicate(predicate: impl Fn(&T, &Cx) -> bool + 'static) -> Self {
        Self::Predicate(Box::new(predicate))
    }

    pub(in crate::fixups) fn matches(&self, actual: &T, cx: &Cx) -> bool
    where
        T: PartialEq,
    {
        match self {
            Self::Any => true,
            Self::Eq(expected) => actual == expected,
            Self::Predicate(predicate) => predicate(actual, cx),
        }
    }
}

impl<T, Cx> Field<T, Cx>
where
    T: PartialEq,
{
    pub(in crate::fixups) fn eq(expected: T) -> Self {
        Self::Eq(expected)
    }
}

impl<T, Cx> Field<T, Cx>
where
    T: PartialEq + PartialOrd + 'static,
    Cx: 'static,
{
    #[allow(dead_code)]
    pub(in crate::fixups) fn range<R>(range: R) -> Self
    where
        R: RangeBounds<T> + 'static,
    {
        Self::Predicate(Box::new(move |value, _cx| range.contains(value)))
    }
}
