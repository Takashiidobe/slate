use crate::program::{FunctionId, ProgramBinding};
use foldhash::{HashMap, HashMapExt, HashSet, HashSetExt};

pub(crate) struct FunctionEffects {
    pub names: Vec<ProgramBinding>,
    pub opaque: bool,
    pub callees: Vec<FunctionId>,
}

pub(crate) struct Effects {
    opaque: Vec<bool>,
    callers: Vec<Vec<u32>>,
    namers: HashMap<ProgramBinding, Vec<u32>>,
}

impl Effects {
    pub fn new(functions: &[Option<FunctionEffects>]) -> Self {
        let mut callers = vec![Vec::new(); functions.len()];
        let mut namers: HashMap<ProgramBinding, Vec<u32>> = HashMap::new();
        let mut opaque = vec![false; functions.len()];
        let mut pending = Vec::new();
        for (function, effects) in functions.iter().enumerate() {
            let Some(effects) = effects else {
                continue;
            };
            for callee in &effects.callees {
                callers[callee.0 as usize].push(function as u32);
            }
            for &name in &effects.names {
                namers.entry(name).or_default().push(function as u32);
            }
            if effects.opaque {
                opaque[function] = true;
                pending.push(function as u32);
            }
        }
        while let Some(function) = pending.pop() {
            for &caller in &callers[function as usize] {
                if !opaque[caller as usize] {
                    opaque[caller as usize] = true;
                    pending.push(caller);
                }
            }
        }
        Self {
            opaque,
            callers,
            namers,
        }
    }

    pub fn opaque(&self, function: FunctionId) -> bool {
        self.opaque[function.0 as usize]
    }

    pub fn reaching(&self, object: ProgramBinding) -> HashSet<FunctionId> {
        let mut reached = HashSet::new();
        let mut pending: Vec<u32> = self.namers.get(&object).cloned().unwrap_or_default();
        while let Some(function) = pending.pop() {
            if reached.insert(FunctionId(function)) {
                pending.extend(self.callers[function as usize].iter().copied());
            }
        }
        reached
    }
}
