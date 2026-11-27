# Facts and Lowering

Analysis runs over a `Program` for one target. `analyze` assembles the results
into a read-only `Facts` store. Lowering receives the matching `UnitFacts` view
for each translation unit.

A binding ID is local to its unit, so the program-wide key is the unit and
binding together. For example, `UnitFacts::binding` retrieves a binding's
pointer and mutability facts in that unit. A node ID identifies an individual IR
node. For example, format and heap statement queries use the call or statement
node ID.

When a proof is absent, lowering keeps the ordinary C representation for that
use. To understand an output change, first check which fact is queried in the
lowerer, then follow the query to `crates/slate-analysis/src/facts.rs` and the
analysis that supplies it.
