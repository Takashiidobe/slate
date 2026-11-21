mod const_fold;
mod dead_temporaries;
mod inline_temporaries;
mod truth_canonicalize;

use slate_parser::ir::Module;
pub use truth_canonicalize::negation;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Slot {
    BeforeMacroKeep,
    AfterMacroStrip,
}

pub struct Pass {
    pub name: &'static str,
    pub slots: &'static [Slot],
    run: fn(&mut Module),
}

pub static PASSES: &[Pass] = &[
    Pass {
        name: "truth-canonicalize",
        slots: &[Slot::AfterMacroStrip],
        run: truth_canonicalize::transform,
    },
    Pass {
        name: "inline-temps",
        slots: &[Slot::BeforeMacroKeep, Slot::AfterMacroStrip],
        run: inline_temporaries::transform,
    },
    Pass {
        name: "const-fold",
        slots: &[Slot::AfterMacroStrip],
        run: const_fold::transform,
    },
    Pass {
        name: "dead-temporaries",
        slots: &[Slot::AfterMacroStrip],
        run: dead_temporaries::transform,
    },
];

#[derive(Clone)]
pub struct Passes(Vec<&'static Pass>);

impl Default for Passes {
    fn default() -> Self {
        Self(PASSES.iter().collect())
    }
}

impl Passes {
    pub fn none() -> Self {
        Self(Vec::new())
    }

    pub fn named(names: &str) -> Result<Self, String> {
        if names == "all" {
            return Ok(Self::default());
        }
        names
            .split(',')
            .filter(|name| !name.is_empty())
            .map(|name| {
                PASSES.iter().find(|pass| pass.name == name).ok_or_else(|| {
                    let known: Vec<_> = PASSES.iter().map(|pass| pass.name).collect();
                    format!("unknown pass `{name}` (known: all, {})", known.join(", "))
                })
            })
            .collect::<Result<_, _>>()
            .map(Self)
    }

    pub fn run(&self, module: &mut Module, slot: Slot) {
        for pass in PASSES {
            if pass.slots.contains(&slot)
                && self.0.iter().any(|selected| std::ptr::eq(*selected, pass))
            {
                (pass.run)(module);
            }
        }
    }
}
