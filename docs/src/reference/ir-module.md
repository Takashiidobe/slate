# IR Module

One `ir::Module` holds one resolved translation unit. The textual dump starts
with target facts, then prints module options, assembly, linker options, types,
globals, and functions. It can also print source annotations. The sections below
name the in-memory fields where they differ from the printed form.

## Target

The `target` field is the resolved `TargetInfo`. The printed `target "…"` header
identifies the target after command-line options have been applied.

- `endian` gives the target byte order.
  - `little` stores the least significant byte first.
  - `big` stores the most significant byte first.
- `pointer` describes an ordinary target pointer.
  - `size=N` gives its storage size in bytes.
  - `align=A` gives its alignment in bytes.
- `stack_alignment` gives the ABI's preferred stack alignment in bytes.
- `code16` marks 16-bit instruction encoding with i686 C layout and semantics.
- `long_double` names the target's C `long double` format.
  - `f64` is IEEE binary64.
  - `f80` is x87 extended precision.
  - `f128` is IEEE binary128.
- `storage TYPE` gives layout for a supported scalar type, including padding.
  - `size=N` gives its storage size in bytes.
  - `align=A` gives its alignment in bytes.

## Translation options

- `hosted` selects the C execution environment.
  - `true` treats the unit as hosted.
  - `false` prints `freestanding;` and treats `main` as an ordinary symbol.
- `nostdinc` records whether standard system include directories were disabled.
  - `true` prints `nostdinc;` and skips those directories.
  - `false` leaves normal header lookup available.
- `null_pointer_is_valid` controls assumptions about data pointers at address
  zero.
  - `true` prints `null_pointer_is_valid;` and allows accesses at address zero.
  - `false` uses the ordinary C null-pointer access rules.
- `zero_init_padding_bits` gives the aggregate-padding policy. The header omits
  it.
  - `Standard` uses the standard initialization rules.
  - `Unions` also requests zeroed union padding where the option applies.
  - `All` requests zeroed struct and union padding where the option applies.
- `alloca` holds layout settings for ordinary dynamic stack allocations. The
  header omits them. See [Dynamic Stack Allocation](../lowering/alloca.md).
  - `padding` is the number of extra bytes to reserve per allocation.
  - `alignment` is the minimum allocation alignment in bytes.

### Machine options

`machine` holds the effective machine code generation settings carried forward
for Rust output. The optional `machine […]` line prints the recorded facts in a
fixed order. An unset option can be omitted even when the target has a default
for it.

- `code_model` gives the target's address and placement assumptions.
  - `tiny` selects the target's most constrained placement model.
  - `small` assumes code and data fit the target's small-address model.
  - `kernel` selects the address model for kernel code.
  - `medium` relaxes placement restrictions for some objects.
  - `large` relaxes placement restrictions for both code and data.
- `pic` or `pie` selects position independence. `N` is the compiler's level.
  - `pic=none` explicitly disables position-independent code.
  - `pic=N` selects position-independent code.
  - `pie=N` selects position-independent executable code.
- `red_zone` controls use of the ABI space below the stack pointer.
  - `on` allows its use.
  - `off` disables its use.
- `cf_protection` selects branch and return protection.
  - `none` disables protection.
  - `branch` selects indirect branch protection.
  - `return` selects return protection.
  - `full` selects both branch and return protection.
  - `check` requests GCC's link-time consistency check, alone or as `+check`.
- `indirect_branch_cs_prefix` requests the x86 CS prefix on affected thunks.
- `retpoline` requests retpoline lowering for indirect branches.
- `function_return` specifies how to emit returns when a policy is recorded.
  - `thunk_extern` requests an external return thunk.
- `stack_protector` selects when to insert stack canaries.
  - `off` disables insertion.
  - `on` uses the ordinary heuristic.
  - `strong` uses a broader heuristic.
  - `all` covers all eligible functions.
  - `explicit` uses explicit function annotations.
- `stack_protector_guard` records that a machine option changed the guard setup.
- `sanitize` gives the selected sanitizer instrumentation.
  - `address` instruments userspace memory accesses.
  - `kernel-address` selects the kernel form of address instrumentation.
  - `thread` instruments operations for race detection.
- `reg_struct_return` requests register returns for eligible i686 structs.
- `regparm=N` requests up to `N` integer argument registers on i686. Zero is
  omitted.

## Program items

- `asm` is the ordered list of file-scope assembly items. Each can carry a
  template and, where available, resolved operand sections.
- `linker_options` contains strings from linker pragmas that are carried through
  as options. Each prints as `linker "…";`. MSVC `/alternatename` and `/include`
  can instead be represented by symbol attributes.
- `types` contains named or anonymous definitions indexed by `@typeN`.
  Definitions are aliases, records, or enums. References carry the type identity
  without repeating its definition.
- `globals` contains object definitions and declaration-only objects. It
  includes file-scope objects, block-scope statics, and materialized string
  literals. The dump spells definitions `global` and declarations `extern`.
- `functions` contains resolved function declarations and definitions. A
  declaration prints without a body. Calls refer to the function by binding
  identity even when the source spelling differs from the linker symbol.
- `next_binding` is the allocator cursor for fresh binding IDs. It has no
  printed form. A printed `%N` is an identity, even when a name is reused in
  another scope. Node IDs separately identify syntax and operation nodes for
  source annotations.

## Source information and recovery

- `metadata` maps node IDs to key/value annotations. `--show-metadata` prints
  entries such as `[c_canon="…"]` and `[typedef_chain="…"]` beside their owners.
  The `c` field records C type spelling, which can also appear in a default dump
  when needed to show a distinct spelling.
- `comments` records comment ownership and placement for `--show-comments`.
  - `leading` attaches a comment before its owner.
  - `trailing` attaches a comment after its owner.
  - `detached` keeps a comment without a direct owner.
  - `doc` marks a documentation comment.
- `c_types` is the side table of C type spellings and names.
- `macros` holds definitions and invocation records for `--show-macros`.
  - `definition=#N` refers to a `macro_definition`.
  - `site=…` gives the invocation's source range.
  - `arguments=…` gives the argument ranges.
  - `parent=#N` refers to the enclosing invocation.
  - `empty` marks a substitution that produced no tokens.
  - `covers` describes the syntax covered by the expansion. `argN` describes
    what the corresponding argument covers. Both take the same values.
    - `expr` covers one expression.
    - `callee` covers the callee expression of a call.
    - `call_arguments` covers a call's parenthesized argument list.
    - `statements` covers a statement or a run of sibling statements.
    - `decl` covers a declaration.
    - `type` covers a type name or type specifiers.
    - `name` covers a single identifier.
    - `split` crosses syntax boundaries that prevent recovering one region.
- `macro_rejections` maps expansions to the reason they must stay expanded.
- `macro_hidden_arguments` maps expansions to bindings threaded through implicit
  macro arguments.
- `macro_written_casts` keeps the destination type, operand node, and expansion
  for a written integer cast on a macro argument. This lets a recovered macro
  retain the cast even if the particular invocation did not need a conversion.
- `macro_values` maps a node to its nested value and argument wrappers.
  `--show-macros` can print these as `[macro_value="#E"]` and
  `[macro_arg="#E.K"]`. A token or statement associated with an expansion can
  also print `[macro=NAME#E body]` or `[macro=NAME#E argK]`.

`--show-spans` exposes location annotations in the form
`[spelling=FILE:OFFSET+LENGTH, expansion=FILE:OFFSET+LENGTH]`. Spelling points
to the original characters. Expansion points to the corresponding macro use site
when there is one.

`Module` and `MachineCodegen` are declared in
`crates/slate-parser/src/ir/module.rs`. The dump is implemented by
`DisplayModule` in `crates/slate-parser/src/ir/module_print.rs`.
