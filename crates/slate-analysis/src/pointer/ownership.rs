use super::flow::Flow;
use super::summary::Summaries;
use super::{ESCAPE, FREE, HeapStatement, MACRO, WRITE};
use crate::program::{FunctionId, Program, UnitId};
use foldhash::{HashMap, HashMapExt, HashSet, HashSetExt};
use slate_parser::ast::NodeId;

pub(super) struct Owned {
    pub(super) node: u32,
    pub(super) written: bool,
    pub(super) elided: Vec<NodeId>,
    pub(super) statements: Vec<(NodeId, HeapStatement)>,
}

pub(super) struct OwnershipFacts {
    pub(super) nodes: HashMap<u32, bool>,
    pub(super) elided_releases: HashSet<(UnitId, NodeId)>,
    pub(super) heap_statements: HashMap<(UnitId, NodeId), HeapStatement>,
}

pub(super) fn prove(program: &Program<'_>, index: &Summaries, flow: &Flow) -> OwnershipFacts {
    let Summaries {
        functions: summaries,
        base,
        parameters,
        ..
    } = index;
    let total = flow.node_count();
    let mut class_sizes = vec![0u32; total];
    for class in flow.classes() {
        class_sizes[class as usize] += 1;
    }
    let mut owned: HashMap<u32, bool> = HashMap::new();
    let mut elided_releases = HashSet::new();
    let mut heap_statements = HashMap::new();
    for (function, summary) in summaries.iter().enumerate() {
        let Some(summary) = summary else { continue };
        let Some(entry) = program.function(FunctionId(function as u32)) else {
            continue;
        };
        let offset = base[function];
        for &Owned {
            node,
            written,
            ref elided,
            ref statements,
        } in &summary.owned
        {
            let global = offset + node;
            let class = flow.class_of(global);
            let freed_by_callee = summary.calls.iter().any(|site| {
                site.arguments.iter().any(|&(index, argument)| {
                    argument == node
                        && parameters[site.callee.0 as usize].get(&index).is_some_and(
                            |&parameter| flow.flags(flow.class_of(parameter)) & FREE != 0,
                        )
                })
            });
            if class_sizes[class as usize] == 1
                && flow.flags(class) & (ESCAPE | MACRO) == 0
                && !flow.returned(class)
                && !freed_by_callee
            {
                owned.insert(global, written || flow.flags(class) & WRITE != 0);
                elided_releases.extend(elided.iter().map(|&call| (entry.unit, call)));
                heap_statements.extend(
                    statements
                        .iter()
                        .map(|&(id, fact)| ((entry.unit, id), fact)),
                );
            }
        }
    }

    OwnershipFacts {
        nodes: owned,
        elided_releases,
        heap_statements,
    }
}
