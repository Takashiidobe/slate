use std::ops::RangeBounds;

#[derive(Default)]
pub(in crate::backend) enum Field<T, Cx = ()> {
    #[default]
    Any,
    Eq(T),
    Predicate(FieldPredicate<T, Cx>),
}

type FieldPredicate<T, Cx> = Box<dyn Fn(&T, &Cx) -> bool>;

impl<T, Cx> Field<T, Cx> {
    pub(in crate::backend) fn any() -> Self {
        Self::Any
    }

    pub(in crate::backend) fn predicate(predicate: impl Fn(&T, &Cx) -> bool + 'static) -> Self {
        Self::Predicate(Box::new(predicate))
    }

    pub(in crate::backend) fn matches(&self, actual: &T, cx: &Cx) -> bool
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
    pub(in crate::backend) fn eq(expected: T) -> Self {
        Self::Eq(expected)
    }
}

impl<T, Cx> Field<T, Cx>
where
    T: PartialEq + PartialOrd + 'static,
    Cx: 'static,
{
    #[expect(
        dead_code,
        reason = "query API surface not yet wired into a fixup rule"
    )]
    pub(in crate::backend) fn range<R>(range: R) -> Self
    where
        R: RangeBounds<T> + 'static,
    {
        Self::Predicate(Box::new(move |value, _cx| range.contains(value)))
    }
}
