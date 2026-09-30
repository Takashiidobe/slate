use super::Value;
use std::fmt;

#[derive(Debug, Clone)]
pub enum MemoryOrder {
    Relaxed,
    Consume,
    Acquire,
    Release,
    AcqRel,
    SeqCst,
    Dynamic(Box<Value>),
}

#[derive(Debug, Clone)]
pub enum SyncScope {
    System,
    Device,
    Workgroup,
    Wavefront,
    Single,
    Cluster,
    Dynamic(Box<Value>),
}

#[derive(Debug, Clone)]
pub struct Atomicity {
    pub order: MemoryOrder,
    pub scope: SyncScope,
}

impl From<MemoryOrder> for Atomicity {
    fn from(order: MemoryOrder) -> Self {
        Self {
            order,
            scope: SyncScope::System,
        }
    }
}

#[derive(Debug, Clone)]
pub enum Weakness {
    Strong,
    Weak,
    Dynamic(Box<Value>),
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum CompareExchangeForm {
    WriteBack,
    Success,
    Old,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum FenceScope {
    Thread,
    Signal,
}

impl MemoryOrder {
    pub fn from_c(value: u64) -> Option<Self> {
        Some(match value {
            0 => Self::Relaxed,
            1 => Self::Consume,
            2 => Self::Acquire,
            3 => Self::Release,
            4 => Self::AcqRel,
            5 => Self::SeqCst,
            _ => return None,
        })
    }

    pub(super) fn display_mode(&self, compact: bool) -> impl fmt::Display + '_ {
        struct DisplayOrder<'a>(&'a MemoryOrder, bool);

        impl fmt::Display for DisplayOrder<'_> {
            fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
                f.write_str(match self.0 {
                    MemoryOrder::Relaxed => "relaxed",
                    MemoryOrder::Consume => "consume",
                    MemoryOrder::Acquire => "acquire",
                    MemoryOrder::Release => "release",
                    MemoryOrder::AcqRel => "acq_rel",
                    MemoryOrder::SeqCst => "seq_cst",
                    MemoryOrder::Dynamic(value) => {
                        return write!(
                            f,
                            "dynamic({})",
                            value.display_metadata(false, None).with_compact(self.1)
                        );
                    }
                })
            }
        }

        DisplayOrder(self, compact)
    }
}

impl SyncScope {
    pub fn from_c(value: u64) -> Option<Self> {
        Some(match value {
            0 => Self::System,
            1 => Self::Device,
            2 => Self::Workgroup,
            3 => Self::Wavefront,
            4 => Self::Single,
            5 => Self::Cluster,
            _ => return None,
        })
    }

    pub(super) fn display_mode(&self, compact: bool) -> impl fmt::Display + '_ {
        struct DisplayScope<'a>(&'a SyncScope, bool);

        impl fmt::Display for DisplayScope<'_> {
            fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
                f.write_str(match self.0 {
                    SyncScope::System => "system",
                    SyncScope::Device => "device",
                    SyncScope::Workgroup => "workgroup",
                    SyncScope::Wavefront => "wavefront",
                    SyncScope::Single => "single",
                    SyncScope::Cluster => "cluster",
                    SyncScope::Dynamic(value) => {
                        return write!(
                            f,
                            "dynamic({})",
                            value.display_metadata(false, None).with_compact(self.1)
                        );
                    }
                })
            }
        }

        DisplayScope(self, compact)
    }
}

pub(super) struct SyncScopeAttribute<'a>(pub &'a SyncScope, pub bool);

impl fmt::Display for SyncScopeAttribute<'_> {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self.0 {
            SyncScope::System => Ok(()),
            scope => write!(f, ", sync_scope={}", scope.display_mode(self.1)),
        }
    }
}

pub(super) fn format_ordering(
    f: &mut fmt::Formatter<'_>,
    ordering: Option<&Atomicity>,
    compact: bool,
) -> fmt::Result {
    match ordering {
        Some(ordering) => write!(
            f,
            ", atomic={}{}",
            ordering.order.display_mode(compact),
            SyncScopeAttribute(&ordering.scope, compact)
        ),
        None => Ok(()),
    }
}

impl Weakness {
    pub(super) fn display_mode(&self, compact: bool) -> impl fmt::Display + '_ {
        struct DisplayWeakness<'a>(&'a Weakness, bool);

        impl fmt::Display for DisplayWeakness<'_> {
            fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
                match self.0 {
                    Weakness::Strong => f.write_str("false"),
                    Weakness::Weak => f.write_str("true"),
                    Weakness::Dynamic(value) => write!(
                        f,
                        "dynamic({})",
                        value.display_metadata(false, None).with_compact(self.1)
                    ),
                }
            }
        }

        DisplayWeakness(self, compact)
    }
}

impl fmt::Display for CompareExchangeForm {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        f.write_str(match self {
            Self::WriteBack => "write_back",
            Self::Success => "success",
            Self::Old => "old",
        })
    }
}

impl fmt::Display for FenceScope {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        f.write_str(match self {
            Self::Thread => "thread",
            Self::Signal => "signal",
        })
    }
}
