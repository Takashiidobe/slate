use crate::lexer::TokenText;
use foldhash::HashMap;
use std::rc::Rc;

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash, Default)]
pub(super) struct HideSet(u32);

impl HideSet {
    pub(super) const EMPTY: Self = Self(0);
}

pub(super) struct HideSets {
    sets: Vec<Rc<[TokenText]>>,
    interned: HashMap<Rc<[TokenText]>, HideSet>,
    with: HashMap<(HideSet, TokenText), HideSet>,
    unions: HashMap<(HideSet, HideSet), HideSet>,
    intersections: HashMap<(HideSet, HideSet), HideSet>,
}

impl HideSets {
    pub(super) fn new() -> Self {
        let empty: Rc<[TokenText]> = Rc::from([]);
        Self {
            sets: vec![empty.clone()],
            interned: HashMap::from_iter([(empty, HideSet::EMPTY)]),
            with: HashMap::default(),
            unions: HashMap::default(),
            intersections: HashMap::default(),
        }
    }

    pub(super) fn contains(&self, set: HideSet, name: &str) -> bool {
        set != HideSet::EMPTY
            && self.sets[set.0 as usize]
                .binary_search_by(|member| (**member).cmp(name))
                .is_ok()
    }

    pub(super) fn with(&mut self, set: HideSet, name: &TokenText) -> HideSet {
        if let Some(&result) = self.with.get(&(set, name.clone())) {
            return result;
        }
        let mut members = self.sets[set.0 as usize].to_vec();
        if let Err(index) = members.binary_search_by(|member| (**member).cmp(&**name)) {
            members.insert(index, name.clone());
        }
        let result = self.intern(members);
        self.with.insert((set, name.clone()), result);
        result
    }

    pub(super) fn union(&mut self, left: HideSet, right: HideSet) -> HideSet {
        if left == right || right == HideSet::EMPTY {
            return left;
        }
        if left == HideSet::EMPTY {
            return right;
        }
        let key = (left.min(right), left.max(right));
        if let Some(&result) = self.unions.get(&key) {
            return result;
        }
        let mut members = self.sets[left.0 as usize].to_vec();
        members.extend(self.sets[right.0 as usize].iter().cloned());
        members.sort_unstable();
        members.dedup();
        let result = self.intern(members);
        self.unions.insert(key, result);
        result
    }

    pub(super) fn intersection(&mut self, left: HideSet, right: HideSet) -> HideSet {
        if left == right {
            return left;
        }
        if left == HideSet::EMPTY || right == HideSet::EMPTY {
            return HideSet::EMPTY;
        }
        let key = (left.min(right), left.max(right));
        if let Some(&result) = self.intersections.get(&key) {
            return result;
        }
        let other = &self.sets[right.0 as usize];
        let members = self.sets[left.0 as usize]
            .iter()
            .filter(|member| {
                other
                    .binary_search_by(|candidate| (**candidate).cmp(&***member))
                    .is_ok()
            })
            .cloned()
            .collect();
        let result = self.intern(members);
        self.intersections.insert(key, result);
        result
    }

    fn intern(&mut self, members: Vec<TokenText>) -> HideSet {
        let members: Rc<[TokenText]> = members.into();
        if let Some(&set) = self.interned.get(&members) {
            return set;
        }
        let set = HideSet(self.sets.len() as u32);
        self.sets.push(members.clone());
        self.interned.insert(members, set);
        set
    }
}
