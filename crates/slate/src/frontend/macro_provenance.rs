use foldhash::{HashMap, HashSet};
use slate_parser::ast::{MacroExpansionId, NodeId};
use slate_parser::ir::{Module, Place, ReadVisitor, Statement, Value, walk_module_ref};

#[derive(Default)]
struct Sites {
    nodes: HashSet<NodeId>,
    regions: HashSet<MacroExpansionId>,
}

impl ReadVisitor for Sites {
    fn statement(&mut self, statement: &Statement) {
        if let Statement::MacroExpansion { expansion, .. } = statement {
            self.regions.insert(*expansion);
        }
    }

    fn value(&mut self, value: &Value) {
        self.nodes.insert(value.node.id);
    }

    fn place(&mut self, place: &Place) {
        self.nodes.insert(place.id);
    }
}

fn sites(module: &Module) -> Sites {
    let mut sites = Sites::default();
    walk_module_ref(module, &mut sites);
    sites
}

#[derive(Clone, Copy, PartialEq, Eq, Hash)]
enum Site {
    Node(NodeId),
    Region,
}

pub(super) struct ProvenanceCheck {
    pending: HashMap<MacroExpansionId, HashSet<Site>>,
    lost: HashMap<MacroExpansionId, (&'static str, Site)>,
}

impl ProvenanceCheck {
    pub(super) fn new(module: &Module) -> Self {
        let present = sites(module);
        let mut pending: HashMap<MacroExpansionId, HashSet<Site>> = HashMap::default();
        for (id, wraps) in &module.macro_values {
            if present.nodes.contains(id) {
                for wrap in wraps {
                    pending
                        .entry(wrap.expansion())
                        .or_default()
                        .insert(Site::Node(*id));
                }
            }
        }
        for expansion in present.regions {
            pending.entry(expansion).or_default().insert(Site::Region);
        }
        Self {
            pending,
            lost: HashMap::default(),
        }
    }

    pub(super) fn after(&mut self, step: &'static str, module: &Module) {
        let present = sites(module);
        for (expansion, sites) in &mut self.pending {
            sites.retain(|site| {
                let kept = match site {
                    Site::Node(id) => present.nodes.contains(id),
                    Site::Region => present.regions.contains(expansion),
                };
                if !kept {
                    self.lost.entry(*expansion).or_insert((step, *site));
                }
                kept
            });
        }
    }

    pub(super) fn report(self, module: &Module) -> Option<String> {
        let mut lost: Vec<_> = self
            .lost
            .into_iter()
            .filter(|(expansion, _)| !module.macro_rejections.contains_key(expansion))
            .collect();
        if lost.is_empty() {
            return None;
        }
        lost.sort_by_key(|(expansion, _)| expansion.0);
        Some(
            lost.iter()
                .map(|(expansion, (step, site))| {
                    let what = match site {
                        Site::Node(id) => format!("node {}", id.0),
                        Site::Region => "statement region".to_string(),
                    };
                    format!(
                        "  {step}: #{} {} lost its {what} without a macro_rejections reason",
                        expansion.0,
                        module.macros.definition(*expansion).name
                    )
                })
                .collect::<Vec<_>>()
                .join("\n"),
        )
    }
}
