# Hybrid C++ migration with residual fallback

> Design sketch, not implemented. This extends
> [cxx-stdlib-shim-lowering.md](cxx-stdlib-shim-lowering.md): unsupported C++
> does not have to abort translation. Slate can retain it as residual C++ and
> progressively replace C++-owned values and calls with Rust rewrites.

## The baseline may be a C++ launcher

The broadest correctness-first starting point is a hybrid program:

```text
original C++
    ├── translated Rust islands
    ├── residual C++ functions and types
    └── generated C-ABI adapters in both directions
```

At the limit, the first successful output may be a Rust launcher around the
original C++ program. That is still useful if Slate reports precisely which
functions and type dependencies remain residual. As lowering and rewrite
coverage grows, the same program can migrate without an all-or-nothing
translation boundary.

Fallback should begin at a coarse boundary. A whole function can safely remain
in its original translation-unit context, preserving overload resolution,
private access, templates, RAII, and implicit temporaries. Outlining an
arbitrary unsupported expression is later work because its cleanup scopes,
control flow, and source-only names may not form an independent ABI operation.

## Native Rust and C++ representations may coexist

A supported Rust island may prefer `Vec<MyType>` while a residual C++ consumer
still requires `std::vector<MyType>`. A generated bridge can recursively
convert the container and its elements in either direction. This permits a
larger Rust island at the cost of an O(n) allocation and conversion at each
representation transition.

The bridge is a conversion, not a cast. It may change allocation, address,
capacity, iterator identity, and constructor/destructor activity. Rules must
therefore classify their semantic strength and prerequisites. A
`std::vector<int>` bridge is much less constrained than a bridge for a vector
of move-only, self-referential, polymorphic, allocator-aware, or
side-effectful objects.

## Use a lazy shadow with one authoritative side

Unconditionally constructing both representations is not a semantics-neutral
default. Constructing an otherwise-unused C++ shadow can invoke observable
copy/move constructors, destructors, allocators, registration hooks, or
exceptions. The general representation should create and synchronize the
shadow lazily:

```rust
struct HybridVector<T> {
    rust: Option<Vec<T>>,
    cpp: Option<CxxVectorHandle>,
    authoritative: Authority,
    rust_epoch: u64,
    cpp_epoch: u64,
}
```

Only one side is authoritative after a mutation:

```text
Rust current, C++ absent/stale
    ── residual C++ access ──> synchronize, then C++ current
    ── translated Rust access <── synchronize, then Rust current
```

Several adjacent calls on one side share one synchronization. Eager mirroring
remains available for bridge rules proven to be value-like and non-observable,
or as an explicit validation mode. A dual-cache design must never permit
independent writes to two current copies.

Mutable-reference adapters need copy-in/call/copy-out behavior on both normal
and exceptional return. Multiple arguments aliasing the same source object
must receive the same shadow handle. A live pointer, reference, iterator,
`span`, or `string_view` into a representation prevents switching until it is
dead; an escaped interior alias may pin the value permanently to its current
side.

Reentrant callbacks are synchronization barriers. Residual C++ operating on a
C++-current value cannot call translated Rust that observes a stale Rust copy.
The callback must receive a view of the active representation, synchronize
first, or keep the callback-connected component on one side.

## Rewrite and deletion are fixed-point operations

Generated boundary actions should be explicit operations rather than
incidental adapter code:

```rust
slate_bridge::ensure_cpp(&mut value);
slate_bridge::ensure_rust(&mut value);
slate_bridge::mark_cpp_mutated(&mut value);
```

A whole-program Rust cleanup pass can replace `HybridVector<T>` with `Vec<T>`
once it proves that no reachable caller, callback, export, global, or residual
C++ operation requests the C++ representation. Removing one residual callee
may make all its callers eligible, so deletion should iterate to a fixed point.
The analysis must include the residual C++ call graph and exported ABI; rustc
alone cannot prove that an external C++ caller does not exist.

The same rule applies recursively to the type graph. A container cannot become
Rust-only while a residual consumer still expects the C++ layout of its
element, base, member, allocator, comparator, or callback state unless an
explicit bridge handles that edge.

## Evidence levels for increasingly interpretive rewrites

Each rewrite and bridge should state what justifies it:

| Evidence | Meaning                                                          |
| -------- | ---------------------------------------------------------------- |
| proven   | Static whole-program analysis establishes the preconditions.     |
| guarded  | A runtime check detects misspeculation and falls back or panics. |
| assumed  | The user explicitly accepts a property Slate cannot prove.       |
| observed | Profiling saw the property, but untested executions may differ.  |
| residual | Keep the C++ implementation or representation.                   |

This supports multiple migration policies without conflating them:

- **Exact**: only proven rewrites; otherwise retain C++.
- **Guarded**: speculative rewrites may dynamically fall back to retained C++.
- **Value-semantic**: permit representation changes that preserve abstract
  values but not allocation, address, capacity, or iterator identity.
- **Assisted idiomatic**: permit user-approved changes such as exceptions to
  `Result`, byte strings to UTF-8 `String`, or inheritance to composition.

Profiles are evidence for prioritization and speculation, not proofs of
absence. A profiled property can enable a guarded rewrite while the residual
C++ path still exists. Deleting the fallback requires a static proof, a user
assumption, or acceptance that a missed case will panic instead of preserving
the original behavior.

## Shadow execution can validate rewrite rules

For pure or isolated operations, Slate can execute both the residual C++ and
candidate Rust implementations and compare their results or post-state. The
C++ result remains an oracle until the rule is trusted. This does not apply
blindly to I/O, locks, allocation hooks, globals, or other external effects,
because executing both implementations would duplicate those effects.

Useful output modes are:

- **Residual**: C++ is authoritative and Rust calls adapters.
- **Hybrid**: lazy representations synchronize across the boundary.
- **Shadow**: eligible Rust and C++ operations run together and are compared.
- **Rust-only check**: every remaining residual dependency is a targeted
  diagnostic rather than an unresolved linker symbol.

## Migration diagnostics are part of the product

Comments may identify local residual operations, but they become stale and the
Rust compiler cannot diagnose a semantically unnecessary C++ shadow. Slate
should also emit a structured report recording:

- translated, rewritten, shimmed, and residual functions;
- the first blocking call or type-graph edge for each residual item;
- synchronization sites and their estimated conversion cost;
- the evidence and assumptions used by every non-exact rewrite;
- the deletion frontier: the remaining changes needed to remove each C++
  representation or the C++ artifact entirely.

The useful measure of migration is the number and weight of remaining boundary
edges, not the percentage of translated statements. One conversion inside a
hot loop can dominate a large otherwise-Rust function.

## Suggested implementation sequence

1. Retain unsupported whole functions in their original C++ translation-unit
   context and generate C-ABI calls and reverse callback adapters.
2. Add opaque handles with C++-owned lifetime and exception-status adapters.
3. Introduce explicit bridge operations and lazy single-authority hybrid
   storage for a narrow value-like type such as `std::vector<int>`.
4. Add whole-program residual-call and type-usage facts, then delete unused
   shadows to a fixed point.
5. Add guarded and profile-informed policies without weakening exact mode.
6. Add shadow execution for effect-isolated rewrite rules and a Rust-only
   diagnostic mode.
