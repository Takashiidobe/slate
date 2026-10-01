# Parser and Slate architectural audit

<!-- toc -->
- [Scope and evidence](#scope-and-evidence)
- [Priorities](#priorities)
- [Checked semantic facts](#checked-semantic-facts)
- [Source ownership](#source-ownership)
- [Project translation](#project-translation)
- [Reachability](#reachability)
- [Representation and lexer costs](#representation-and-lexer-costs)
- [One explicit translation
  pipeline](#one-explicit-translation-pipeline)
- [Measurement and validation](#measurement-and-validation)
<!-- /toc -->

## Scope and evidence

- Audit date: 2026-10-01. Static review of preprocessing, parsing, reachability, semantic facts, IR construction, Rust lowering, formatting, and project orchestration.
- Rust navigation: rust-analyzer document symbols, definitions, references, and hover. Design context: concept pages, work logs, and beads.
- No Rust implementation changes, new benchmarks, or test runs. Performance opportunities below are hypotheses unless explicitly attributed to historical measurements.
- Preserve the current boundary: C semantics belong to slate-parser; Slate mechanically translates typed IR. Do not recreate C typing in the Rust backend.
- The checker-facts and preprocessor rewrites already landed: `slate-parser-ygrj`, `slate-parser-ryfo`. Recommending those rewrites again would duplicate completed work.

## Priorities

| Order | Change | Main payoff | Tracking |
| --- | --- | --- | --- |
| 1 | Require checked semantic facts at the public lowering boundary | Enforce invariants and reduce accidental rule duplication | `slate-5ezs` |
| 2 | Centralize exact source snapshots and diagnostics | Correct diagnostics, less copying and repeated I/O | `slate-bd8f` |
| 3 | Complete the existing migration with explicit pass options | One implementation and consistent entry points | `slate-p58o.6`, `slate-p58o.1.4` |
| 4 | Consume pruned AST nodes instead of cloning them | Remove an identifiable whole-tree copy | `slate-lrdn` |
| 5 | Bound project representation lifetimes | Lower peak memory and improve worker scaling | `slate-db4s` |
| 6 | Measure phases before changing representation or the scanner | Select optimizations supported by real workloads | `slate-wepr`; existing `slate-parser-xgj9` |

## Checked semantic facts

| Evidence | Architectural consequence |
| --- | --- |
| [Sema](../../crates/slate-parser/src/sema/mod.rs:36), [analyze](../../crates/slate-parser/src/sema/validate.rs:56), [lower](../../crates/slate-parser/src/sema/module.rs:24) | The same public type permits lowering before checking; the documented ordering is a caller convention. |
| [TypeResolver](../../crates/slate-parser/src/sema/types.rs:44) | Semantic facts, diagnostic accumulation, entity state, type interning, and extent binding share one mutable object. |
| [operand_steps and computation](../../crates/slate-parser/src/sema/expression.rs:697) | Lowering clones conversion lists; missing or unbound facts can invoke typing. VLA rebinding is a documented exception. |
| [Slate integration](../../crates/slate/src/slate_parser_frontend.rs:188) | Successful analysis and lowering warning results are discarded; parser/sema failures are converted to strings. |

- Return a `CheckedUnit` from successful checking; require it for primary IR lowering. Keep the existing AST and side tables rather than allocating a second typed tree.
- Separate immutable semantic facts from lowering state: captured extents, temporary bindings, emitted IR, and diagnostics.
- Preserve the VLA exception first; subsequently represent a checked type shape with extent slots and instantiate those slots during lowering.
- Expose warnings and structured failures in a compilation result. Formatting belongs at the CLI boundary.
- Success criterion: non-VM lowering cannot silently repair missing facts; ordinary lowering consumes facts, while dynamic extent instantiation is explicit.

## Source ownership

| Evidence | Architectural consequence |
| --- | --- |
| [Preprocessor source setup](../../crates/slate-parser/src/pp/mod.rs:645) | Stores source text and duplicates each file's line table in two structures. |
| [Parser handoff](../../crates/slate-parser/src/parser/mod.rs:421), [IR lowering setup](../../crates/slate-parser/src/sema/module.rs:33) | Clone `Files` and line tables across stage boundaries. |
| [Files](../../crates/slate-parser/src/files.rs:50) | Stores paths and line tables, but not the input snapshots; path interning scans the entries. |
| [Sema diagnostics](../../crates/slate-parser/src/sema/validate.rs:229), [Slate diagnostics](../../crates/slate/src/slate_parser_frontend.rs:75) | Reopen source files; in-memory sanitized input or later filesystem changes can differ from the text that produced the spans. Slate also uses a different invalid-byte decoding path. |

- Use one source manager with indexed paths, immutable decoded snapshots, and shared line tables. Borrow or share it across stages.
- Render diagnostics from the exact input snapshot, including `parse_file_with_source` and virtual files.
- Later consider a project-level raw-source cache. Cache expanded output only with complete configuration and macro-state dependencies; path alone is insufficient.
- Token sharing needs separate evaluation: current `TokenText` uses `Rc`, so cross-worker token caching requires a deliberate ownership design. Do not change every token to `Arc` without measuring the cost.

## Project translation

- [parse_slate_units](../../crates/slate/src/main.rs:1479) materializes all modules and file tables; [translate_slate_project](../../crates/slate/src/main.rs:1591) borrows every unit while collecting all Rust programs, then collects formatting jobs.
- The complete IR set stays alive while the complete Rust program set is being built. This creates overlap between two large representations.
- [Worker count](../../crates/slate/src/main.rs:956) defaults to half the CPUs; separate pools are constructed for parsing, lowering, and formatting. Neither policy accounts for per-TU memory.
- [imported_commons](../../crates/slate/src/main.rs:1552) needs global symbol ownership; `main` discovery also needs a project summary. These requirements prevent simply streaming independent TUs without coordination.
- First extract the small summaries, consume owned units during lowering, and reuse a single pool. Then measure whether bounded batches or a more substantial summary/body split are necessary.
- Move project orchestration into the library. The CLI should parse options and report results; project linking, support assets, and feature aggregation should have reusable APIs.
- Preserve deterministic errors and output, common-symbol ownership, and multi-TU linking. Avoid parallelizing individual functions before project worker scaling is measured.

## Reachability

- [filter_translation_unit](../../crates/slate-parser/src/reachability.rs:6) borrows the full AST and deep-clones retained declarations and tags. [parse_input](../../crates/slate-parser/src/parser/decl.rs:290) creates that temporary full AST before calling it.
- First compute the retained sets, then filter owned vectors by moving nodes. This removes copying without changing the reachability rules.
- Current edges are spelling-based; local shadowing keeps unrelated global declarations. The [IR pipeline spec](ir/pipeline.md#reachability-pruning) already calls for resolved dependencies.
- Consider a lightweight scope-aware graph before checking reachable bodies, so downstream stages reuse binding identities.
- Do not move pruning after eager full-header semantic checking: that would sacrifice its current performance benefit and expose previously unvisited unsupported declarations.
- Preserve roots for pragmas, retention attributes, redeclarations, inline emission, and first tag introductions.

## Representation and lexer costs

- [NodeId allocation](../../crates/slate-parser/src/ast.rs:415) uses a process-wide atomic counter. Parallel TUs share that counter, and raw IDs depend on process history and scheduling. Contention is a performance hypothesis, not a measured bottleneck.
- If profiling supports it, use compilation-owned ID allocation, with module identity explicit when IDs cross TU boundaries. Local IDs can enable compact indexes and reproducible dumps.
- [Semantic fact storage](../../crates/slate-parser/src/sema/types.rs:44) uses many independently allocated hash maps. Consider a compact per-expression index and grouped fact access after recording memory and lookup costs.
- Do not allocate a vector indexed directly by today's global `NodeId`: tokens, synthesized nodes, pruning, and other TUs make that space sparse.
- [Lexer setup](../../crates/slate-parser/src/lexer.rs:578) builds `Vec<char>` and `Vec<usize>` for each source. [Identifier scanning](../../crates/slate-parser/src/lexer.rs:819) collects `String` before creating shared token text.
- A byte-oriented scanner with a Unicode/escape path could avoid those intermediate buffers. Preserve line splicing, universal character names, invalid source bytes, and original byte offsets.
- Historical evidence: [2026-09-28 token sharing](../log/2026-09-28-07-41.md) recorded 653 ms to 602 ms on `windows.h` and a libc allocation/copy share reduction from 50% to 35%. `slate-parser-xgj9` was closed with that result accepted. These are not current-tree measurements.
- [Formatting](../../crates/slate/src/backend/format.rs:36) builds source, reparses it into a syn AST, and prints it again; parse failure launches rustfmt. Measure this stage before choosing direct formatted emission or a different Rust AST representation.

## One explicit translation pipeline

- [translate_with_args](../../crates/slate/src/api.rs:134) and the CLI translation paths still default to CIR. `slate-p58o.6` already tracks removing that frontend and dependency.
- The revised Phase 6 bead explicitly retains the Rust rewrite engine. Treat it as a supported backend component, rather than proposing its deletion with CIR ingestion.
- [Normal Slate translation](../../crates/slate/src/api.rs:176) and [directive translation](../../crates/slate/src/frontend/directive_translate.rs:834) invoke backend rewrites; [raw translation](../../crates/slate/src/main.rs:1806) and [project translation](../../crates/slate/src/main.rs:1629) bypass them.
- [backend::apply](../../crates/slate/src/backend/mod.rs:20) additionally selects behavior from environment variables. The current raw differential gate does not establish rewrite correctness on Slate output.
- Use explicit compilation options for configuration, rewrite selection, diagnostics, and concurrency; share stage runners across single-file, directive, multi-target, and project APIs.
- Add differential coverage for enabled rewrites before treating them as validated. Raw output may remain a deliberate mode.

## Measurement and validation

- [bench_macro_expansion.py](../../crates/slate-parser/tools/bench_macro_expansion.py) measures aggregate `parse` wall time on synthetic macro or literal inputs; it does not isolate stages or project peak memory.
- Record preprocess, parse/prune, names/check, IR/effects, Rust lowering, rewrites, and formatting time; include token/node counts and output size.
- Use real header-heavy TUs, macro-heavy TUs, large functions, and multi-TU projects. Record flags, input revision, worker count, wall time, and peak RSS.
- Compare worker scaling before adding concurrency; measure cold and warm source-cache behavior separately.
- Validate implementation with the release nextest profiles for each affected crate. Representation changes also need stable IR/pp output or an explained semantic change; project changes need existing differential multi-TU fixtures.
- No general source optimizer: retain the policy of folding only required constants and preserving C structure for Rust translation.
