use crate::backend::rust_ast::Cfg;
use slate_parser::target_info::{Endian, TargetEnvironment, TargetFamily, TargetInfo, TargetOs};
use std::collections::{BTreeMap, BTreeSet, HashMap};
use std::sync::Mutex;

pub const MAX_TARGETS: usize = u64::BITS as usize;
const SEPARATOR: usize = ", ".len();

pub(crate) fn target_cfg(target: &TargetInfo) -> Cfg {
    let opt = |key: &str, value: &str| Cfg::Opt {
        key: key.into(),
        value: value.into(),
    };
    let arch = match target.family {
        TargetFamily::X86_64 => "x86_64",
        TargetFamily::X86 => "x86",
        TargetFamily::AArch64 => "aarch64",
        TargetFamily::Arm32 => "arm",
    };
    let os = match target.os {
        TargetOs::Windows => Cfg::Flag("windows".into()),
        TargetOs::Linux => opt("target_os", "linux"),
        TargetOs::Darwin => opt("target_os", "macos"),
        TargetOs::Android => opt("target_os", "android"),
        TargetOs::FreeBsd => opt("target_os", "freebsd"),
    };
    let env = match target.environment {
        _ if target.triple.contains("musl") => Some("musl"),
        TargetEnvironment::Gnu | TargetEnvironment::GnuEabi | TargetEnvironment::GnuEabiHf => {
            Some("gnu")
        }
        TargetEnvironment::Msvc => Some("msvc"),
        TargetEnvironment::Darwin | TargetEnvironment::Android | TargetEnvironment::FreeBsd => None,
    };
    let mut atoms = vec![opt("target_arch", arch), os];
    atoms.extend(env.map(|env| opt("target_env", env)));
    Cfg::All(atoms)
}

fn cfg_atom_key(cfg: &Cfg) -> Option<String> {
    match cfg {
        Cfg::Flag(name) => Some(format!("flag:{name}")),
        Cfg::Opt { key, .. } => Some(key.clone()),
        Cfg::Not(_) | Cfg::Any(_) | Cfg::All(_) => None,
    }
}

fn cfg_atoms(cfg: &Cfg) -> Option<Vec<Cfg>> {
    match cfg {
        Cfg::All(atoms) => atoms
            .iter()
            .map(|atom| cfg_atom_key(atom).map(|_| atom.clone()))
            .collect(),
        atom if cfg_atom_key(atom).is_some() => Some(vec![atom.clone()]),
        _ => None,
    }
}

pub(crate) fn factor_cfgs(cfgs: Vec<Cfg>) -> Cfg {
    if cfgs.len() == 1 {
        return cfgs.into_iter().next().unwrap();
    }
    let fallback = || Cfg::Any(cfgs.clone());

    let Some(atom_lists): Option<Vec<Vec<Cfg>>> = cfgs.iter().map(cfg_atoms).collect() else {
        return fallback();
    };
    let template_keys: Vec<String> = atom_lists[0]
        .iter()
        .map(|atom| cfg_atom_key(atom).expect("cfg_atoms only returns keyed atoms"))
        .collect();
    let same_shape = atom_lists.iter().all(|atoms| {
        atoms.len() == template_keys.len()
            && atoms
                .iter()
                .map(|atom| cfg_atom_key(atom).expect("cfg_atoms only returns keyed atoms"))
                .eq(template_keys.iter().cloned())
    });
    if !same_shape {
        return fallback();
    }

    let mut varying_positions = Vec::new();
    for (i, _) in template_keys.iter().enumerate() {
        let mut distinct: Vec<&Cfg> = Vec::new();
        for atoms in &atom_lists {
            if !distinct.contains(&&atoms[i]) {
                distinct.push(&atoms[i]);
            }
        }
        if distinct.len() > 1 {
            varying_positions.push(i);
        }
    }
    let [varying] = varying_positions[..] else {
        return fallback();
    };

    let mut result_atoms = Vec::with_capacity(template_keys.len());
    for i in 0..template_keys.len() {
        if i == varying {
            let mut values: Vec<Cfg> = Vec::new();
            for atoms in &atom_lists {
                if !values.contains(&atoms[i]) {
                    values.push(atoms[i].clone());
                }
            }
            result_atoms.push(Cfg::Any(values));
        } else {
            result_atoms.push(atom_lists[0][i].clone());
        }
    }
    Cfg::All(result_atoms)
}

pub fn target_properties(target: &TargetInfo) -> BTreeSet<Cfg> {
    let opt = |key: &str, value: String| Cfg::Opt {
        key: key.into(),
        value,
    };
    let mut atoms = match target_cfg(target) {
        Cfg::All(atoms) => BTreeSet::from_iter(atoms),
        atom => BTreeSet::from([atom]),
    };
    atoms.insert(opt(
        "target_pointer_width",
        target.pointer_width.to_string(),
    ));
    let endian = match target.endian {
        Endian::Little => "little",
        Endian::Big => "big",
    };
    atoms.insert(opt("target_endian", endian.to_string()));
    if !matches!(target.os, TargetOs::Windows) {
        atoms.insert(Cfg::Flag("unix".into()));
    }
    atoms
}

pub(crate) fn holds(cfg: &Cfg, atoms: &BTreeSet<Cfg>) -> bool {
    match cfg {
        Cfg::Flag(_) | Cfg::Opt { .. } => atoms.contains(cfg),
        Cfg::Not(inner) => !holds(inner, atoms),
        Cfg::Any(items) => items.iter().any(|item| holds(item, atoms)),
        Cfg::All(items) => items.iter().all(|item| holds(item, atoms)),
    }
}

#[derive(Clone)]
struct Term {
    targets: u64,
    literals: Vec<Cfg>,
    size: usize,
}

impl Term {
    fn cfg(&self) -> Cfg {
        match self.literals.as_slice() {
            [literal] => literal.clone(),
            literals => Cfg::All(literals.to_vec()),
        }
    }
}

fn literals(properties: &[BTreeSet<Cfg>]) -> BTreeMap<u64, Cfg> {
    let everything = u64::MAX >> (u64::BITS as usize - properties.len());
    let atoms: BTreeSet<&Cfg> = properties.iter().flatten().collect();
    let mut preferred: BTreeMap<u64, Cfg> = BTreeMap::new();
    for atom in atoms {
        let members = properties
            .iter()
            .enumerate()
            .filter(|(_, target)| target.contains(atom))
            .fold(0u64, |mask, (index, _)| mask | 1 << index);
        for (mask, literal) in [
            (members, atom.clone()),
            (everything & !members, Cfg::Not(Box::new(atom.clone()))),
        ] {
            if mask == 0 || mask == everything {
                continue;
            }
            let rank = |cfg: &Cfg| (matches!(cfg, Cfg::Not(_)), cfg.render().len());
            let known = preferred.get(&mask);
            if known.is_none_or(|known| rank(&literal) < rank(known)) {
                preferred.insert(mask, literal);
            }
        }
    }
    preferred
}

fn reachable_terms(properties: &[BTreeSet<Cfg>]) -> Vec<Term> {
    let everything = u64::MAX >> (u64::BITS as usize - properties.len());
    let literals = literals(properties);
    let mut best = BTreeMap::from([(
        everything,
        Term {
            targets: everything,
            literals: Vec::new(),
            size: 0,
        },
    )]);
    let mut frontier = BTreeSet::from([everything]);
    while !frontier.is_empty() {
        let mut next = BTreeSet::new();
        for mask in frontier {
            let term = best[&mask].clone();
            for (&members, literal) in &literals {
                let narrowed = mask & members;
                if narrowed == 0 || narrowed == mask {
                    continue;
                }
                let size = term.size + literal.render().len();
                let cost = (term.literals.len() + 1, size);
                let known = best.get(&narrowed);
                if known.is_none_or(|known| cost < (known.literals.len(), known.size)) {
                    let mut chosen = term.literals.clone();
                    chosen.push(literal.clone());
                    chosen.sort_by_key(Cfg::render);
                    best.insert(
                        narrowed,
                        Term {
                            targets: narrowed,
                            literals: chosen,
                            size,
                        },
                    );
                    next.insert(narrowed);
                }
            }
        }
        frontier = next;
    }
    best.into_values()
        .map(|term| Term {
            size: term.cfg().render().len(),
            ..term
        })
        .collect()
}

fn shortest_cover(
    uncovered: u64,
    usable: &[&Term],
    memo: &mut HashMap<u64, (usize, usize)>,
) -> usize {
    if uncovered == 0 {
        return 0;
    }
    if let Some((cost, _)) = memo.get(&uncovered) {
        return *cost;
    }
    let lowest = uncovered.isolate_lowest_one();
    let (cost, term) = usable
        .iter()
        .enumerate()
        .filter(|(_, term)| term.targets & lowest != 0)
        .map(|(index, term)| {
            let rest = shortest_cover(uncovered & !term.targets, usable, memo);
            (rest + term.size + SEPARATOR, index)
        })
        .min()
        .expect("distinct targets are separated by some term");
    memo.insert(uncovered, (cost, term));
    cost
}

pub struct TargetConditions {
    properties: Vec<BTreeSet<Cfg>>,
    terms: Vec<Term>,
    selected: Mutex<HashMap<(u64, u64), Cfg>>,
}

impl TargetConditions {
    pub fn new(properties: Vec<BTreeSet<Cfg>>) -> Self {
        let terms = reachable_terms(&properties);
        Self {
            properties,
            terms,
            selected: Mutex::new(HashMap::new()),
        }
    }

    fn cover(&self, on: u64, care: u64) -> Vec<&Term> {
        let usable: Vec<&Term> = self
            .terms
            .iter()
            .filter(|term| term.targets & care & !on == 0 && term.targets & on != 0)
            .collect();
        let mut memo = HashMap::new();
        shortest_cover(on, &usable, &mut memo);
        let mut chosen = Vec::new();
        let mut uncovered = on;
        while uncovered != 0 {
            let (_, term) = memo[&uncovered];
            chosen.push(usable[term]);
            uncovered &= !usable[term].targets;
        }
        chosen
    }

    fn disjunction(&self, on: u64, care: u64) -> Cfg {
        let terms = self.cover(on, care);
        match terms.as_slice() {
            [term] => term.cfg(),
            terms => Cfg::Any(terms.iter().map(|term| term.cfg()).collect()),
        }
    }

    pub fn select(&self, on: u64, care: u64) -> Cfg {
        let mut selected = self.selected.lock().expect("no selection panics");
        selected
            .entry((on, care))
            .or_insert_with(|| {
                let direct = self.disjunction(on, care);
                let off = care & !on;
                let cfg = if off == 0 {
                    direct
                } else {
                    let inverted = Cfg::Not(Box::new(self.disjunction(off, care)));
                    if inverted.render().len() < direct.render().len() {
                        inverted
                    } else {
                        direct
                    }
                };
                for (index, target) in self.properties.iter().enumerate() {
                    if care >> index & 1 == 1 {
                        assert_eq!(
                            holds(&cfg, target),
                            on >> index & 1 == 1,
                            "{} is wrong for matrix target {index}",
                            cfg.render()
                        );
                    }
                }
                cfg
            })
            .clone()
    }
}
