// use crate::fixups::facts::{BindingFact, FixupFacts, FunctionFact, FunctionId};
// use crate::rust_ast::{Item, Program};
// use salsa;
//
// // Inputs ---------------------------------------------------------------
//
// #[salsa::input(singleton)]
// pub struct ProgramInput {
//     #[returns(ref)]
//     pub program: Program,
// }
//
// #[salsa::input(singleton)]
// pub struct PrecomputedFactsInput {
//     #[returns(ref)]
//     pub facts: FixupFacts,
// }
//
// // Database trait used by tracked functions
// pub trait FixupDb: salsa::Database {}
//
// // Tracked queries -----------------------------------------------------
//
// #[salsa::tracked(returns(clone))]
// pub fn function_facts(db: &dyn FixupDb) -> Vec<FunctionFact> {
//     ProgramInput::get(db)
//         .program(db)
//         .items
//         .iter()
//         .enumerate()
//         .filter_map(|(i, item)| match item {
//             Item::Fn(function) => Some(FunctionFact {
//                 id: FunctionId(i),
//                 name: function.name.clone(),
//                 item_index: i,
//             }),
//             _ => None,
//         })
//         .collect()
// }
//
// #[salsa::tracked(returns(clone))]
// pub fn bindings_by_function(db: &dyn FixupDb, function: FunctionId) -> Vec<BindingFact> {
//     PrecomputedFactsInput::get(db)
//         .facts(db)
//         .bindings
//         .iter()
//         .filter(|b| b.function == function)
//         .cloned()
//         .collect()
// }
//
// // Database implementation ------------------------------------------------
//
// #[salsa::database(ProgramInput, PrecomputedFactsInput)]
// #[derive(Default)]
// pub struct Database {
//     storage: salsa::Storage<Self>,
// }
//
// impl salsa::Database for Database {}
// impl FixupDb for Database {}
