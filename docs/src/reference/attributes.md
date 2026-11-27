# Attributes

Slate checks attributes using the selected compiler flavor, target, and
declaration. This page lists the attributes Slate handles individually, from
ones that change generated Rust to ones it records or accepts for compatibility.

```c
struct __attribute__((packed)) Header {
    unsigned char kind;
    unsigned int length;
};
```

GNU names below go inside `__attribute__((...))`. GCC and Clang also accept
registered scoped spellings such as `[[gnu::packed]]`. Clang-specific attributes
can have a `clang::` or `gnu::` spelling depending on the name. In MSVC mode,
use the listed `__declspec(...)` or `[[msvc::...]]` spellings.

For conditional headers, see [Preprocessor Reference](preprocessor.md). A
positive feature query can also apply to attributes whose semantics Slate does
not implement. See the
[compiler attribute registry](https://github.com/Takashiidobe/slate/blob/main/crates/slate-parser/src/attribute_support/registered.rs).

## Standard C attributes

- `[[deprecated]]` records a deprecation marker and an optional message. GCC and
  Clang also accept `deprecated` in GNU syntax.
- `[[fallthrough]]` marks an intentional fallthrough within a switch. Slate
  checks its placement. GCC and Clang also accept GNU `fallthrough`.
- `[[maybe_unused]]` records that a declaration can go unused. GNU `unused` is
  the GCC and Clang spelling.
- `[[nodiscard]]` records that a result should be used and accepts an optional
  message. GNU `warn_unused_result` is covered under function contracts.
- `[[noreturn]]` and `[[_Noreturn]]` mark a function that never returns. Slate
  also accepts the `_Noreturn` function specifier and the GNU `noreturn`
  attribute.
- `[[reproducible]]` is recognized by GCC-mode feature queries. It carries no
  function semantics in Slate.
- `[[unsequenced]]` is recognized by GCC-mode feature queries. It carries no
  function semantics in Slate.

## Size, alignment, and layout

- `_Alignas(N)` and `alignas(N)` request alignment in bytes. With a type
  argument, they request that type's alignment.
- `aligned(N)` requests alignment for an object, type declaration, or field.
  Bare `aligned` requests the target's biggest alignment. GCC mode also models
  it in a type name such as a `sizeof` operand.
- `packed` reduces alignment requirements on a record or field. See
  [Packed Records and Bit-fields](../lowering/packed-records.md).
- `ms_struct` selects Microsoft record layout.
- `gcc_struct` selects GNU record layout. Both layout override spellings are
  gated to x86 in GCC mode.
- `mode(MODE)` changes the declared type to a supported machine mode.
- `vector_size(BYTES)` constructs a vector with the specified total size.
- `ext_vector_type(LANES)` constructs a Clang vector with the specified number
  of elements.
- `neon_vector_type(LANES)` constructs a Clang NEON vector type.
- `neon_polyvector_type(LANES)` accepts the corresponding Clang polynomial
  vector spelling.
- `arm_sve_vector_bits(BITS)` constructs a fixed-length SVE vector from an SVE
  type in GCC mode on AArch64.
- `transparent_union` lets a call supply a compatible member type directly.
  Slate classifies the argument using the first member for supported unions.
- `address_space(N)` accepts a Clang address space annotation. This attribute
  does not change Slate's type layout.
- `scalar_storage_order("ORDER")` accepts the GCC annotation. Slate does not
  change the byte order of fields based on it.
- `may_alias` is accepted in GCC and Clang modes. It does not change record
  layout or produce a Rust aliasing attribute.

## Objects and symbols

- `alias("SYMBOL")` aliases a function or object to an assembler symbol.
- `common` requests common linkage for an eligible tentative definition.
- `nocommon` overrides common symbol emission for an object.
- `dllimport` marks an imported Windows symbol.
- `dllexport` marks a Windows symbol for export.
- `ifunc("RESOLVER")` names the resolver for an indirect function. Slate lowers
  this on ELF targets.
- `retain` asks the linker to keep a symbol.
- `section("NAME")` assigns a function or object to a named section.
- `selectany` marks an external definition as discardable when another
  definition exists. GCC's GNU spelling is Windows-only.
- `tls_model("MODEL")` records the addressing model for a thread-local object.
- `used` keeps an otherwise unreferenced symbol in the generated output.
- `visibility("KIND")` records symbol visibility. Clang mode warns and ignores
  unknown kinds. GCC mode rejects them.
- `weak` marks a symbol as weak. Backend support depends on the target and
  whether the item is a definition or an external reference.
- `weak_import` accepts the Clang weak-import annotation. Slate does not lower
  it as a weak linker reference.
- `weakref("SYMBOL")` redirects an internal declaration's uses to a weak
  reference. Bare `weakref` can take its target from `alias("SYMBOL")`.

## Function behavior and targeting

- `always_inline` records an always-inline preference.
- `noinline` records a never-inline preference. When both preferences occur on a
  function, Slate keeps the first one.
- `gnu_inline` uses GNU inline definition emission rules.
- `const` records that a function does not access memory.
- `pure` records that a function can read memory without writing it.
- `constructor` registers a startup function. An optional integer sets its
  priority.
- `destructor` registers a shutdown function. An optional integer sets its
  priority.
- `naked` requests a function without a compiler-generated prologue or epilogue.
  Slate lowers supported assembly bodies. GCC mode registers it on x86 and
  Arm32.
- `interrupt` marks a handler. Slate emits the Rust x86 interrupt ABI for
  supported x86 handlers. Parser recognition also covers some non-x86 targets.
- `overloadable` enables Clang-style overload resolution. Slate resolves calls
  to supported overloads. Externally linked overload sets can require name
  mangling that Slate does not implement.
- `target("FEATURES")` selects per-function target features. Slate records them
  in IR and maps supported features to Rust.
- `target_clones(...)` accepts a function multiversioning request. Slate does
  not generate the clones or runtime dispatch.
- `cpu_dispatch(...)` accepts Clang's CPU dispatch declaration. Slate does not
  emit CPU dispatch.
- `cpu_specific(...)` accepts Clang's CPU-specific variant declaration. Slate
  does not emit CPU-specific multiversioning.

## Allocation, bounds, and cleanup

- `alloc_size(N)` and `alloc_size(N, M)` record which arguments specify the
  allocation size. Slate checks the attribute's applicability and retains it as
  metadata.
- `alloc_align(N)` records which argument specifies allocation alignment.
- `assume_aligned(N)` and `assume_aligned(N, OFFSET)` retain a return alignment
  annotation. Slate does not turn it into a Rust assumption.
- `malloc` accepts the allocation annotation on a function. `malloc(FREE)` and
  `malloc(FREE, N)` record the deallocator and which of its parameters receives
  the pointer.
- `cleanup(FUNCTION)` runs a cleanup function for an automatic object at scope
  exit. The function receives the object's address.
- `uninitialized` suppresses initialization of an automatic object when
  `-ftrivial-auto-var-init` would otherwise initialize it. GCC also accepts
  `[[gnu::uninitialized]]`.
- `counted_by(MEMBER)` associates a field with its element count. Slate resolves
  the member for `__builtin_counted_by_ref`.
- `counted_by_or_null(MEMBER)` accepts Clang's nullable element-count annotation
  and validates the member reference.
- `sized_by(MEMBER)` accepts Clang's byte-size annotation and validates the
  member reference.
- `sized_by_or_null(MEMBER)` accepts Clang's nullable byte-size annotation and
  validates the member reference.
- `pass_object_size(N)` distinguishes Clang overloads and can break an overload
  tie. Slate does not pass the hidden size argument.
- `pass_dynamic_object_size(N)` uses the same overload handling. Slate does not
  pass the hidden dynamic size argument.

## Function contracts and compiler hints

Slate accepts these attributes as annotations. They do not by themselves
generate Rust diagnostics, checks, or optimization directives.

- `annotate("TEXT")` records a Clang annotation string.
- `availability(...)` accepts a Clang platform availability annotation.
- `format(...)` records the format family, format argument, and first argument
  to check. Slate checks that the attribute applies to the type.
- `format_arg(...)` records an argument that supplies a format string.
- `lifetimebound` accepts a Clang lifetime annotation.
- `nonnull` or `nonnull(N, ...)` records non-null expectations. Slate checks
  whether the attribute applies to the declaration's type.
- `returns_nonnull` records a non-null return expectation.
- `sentinel` or `sentinel(N)` records a variadic sentinel expectation.
- `warn_unused_result` accepts the GNU unused-result annotation.
- `nothrow` records the no-unwind annotation.
- `returns_twice` accepts the returns-twice annotation.
- `cold` accepts a cold-function hint.
- `hot` accepts a hot-function hint.
- `leaf` accepts a leaf-function hint.
- `flatten` accepts the request to inline calls within a function.
- `no_split_stack` accepts a request to disable split stacks for a function.
- `noipa` accepts GCC's request to disable interprocedural optimization.
- `noclone` accepts GCC's request to prevent function cloning.
- `optimize(...)` accepts GCC's per-function optimization request.
- `optnone` accepts Clang's request to disable optimization.

## Calling convention attributes

- `cdecl`, `stdcall`, `fastcall`, `thiscall`, `vectorcall`, `regcall`, `ms_abi`,
  and `sysv_abi` are covered individually in
  [Calling Conventions](calling-conventions.md), including their keyword
  spellings.
- `regparm(N)` parses an integer register-count request. Slate does not apply it
  to an individual function. See `-mregparm=N` in
  [Calling Conventions](calling-conventions.md#flags-that-change-the-default).
- `pcs("aapcs")` parses an Arm base calling convention request. It does not
  override the target ABI in IR.
- `pcs("aapcs-vfp")` parses an Arm hard-float convention request. It does not
  override the target ABI in IR.
- `preserve_all` and `preserve_none` parse when registered for the flavor and
  target. Slate does not apply them to the function ABI.
- `preserve_most` and `swiftcall` produce an unsupported convention diagnostic
  when registered.
- `nomips16` has a parser representation. The registry disables the GNU spelling
  because Slate has no MIPS target.

## Microsoft spellings

- `__declspec(align(N))` requests alignment like GNU `aligned(N)`.
- `__declspec(allocate("NAME"))` assigns a section like GNU `section`.
- `__declspec(deprecated)` accepts deprecation and an optional message.
- `__declspec(dllexport)` marks a symbol for export.
- `__declspec(dllimport)` marks an imported symbol.
- `__declspec(naked)` marks a function as naked.
- `__declspec(noinline)` records a never-inline preference.
- `__declspec(noreturn)` records that a function never returns.
- `__declspec(selectany)` marks an eligible external definition as discardable.
- `__declspec(thread)` gives an object thread-local storage.
- `[[msvc::noinline]]` records a never-inline preference.
- `[[msvc::forceinline]]` records an always-inline preference.
- `__declspec(noalias)` and `__declspec(restrict)` accept memory and return
  aliasing annotations. Slate does not emit Rust aliasing guarantees for them.
- `__declspec(nothrow)` accepts the no-unwind annotation.
- `__declspec(cpu_dispatch(...))` and `__declspec(cpu_specific(...))` accept
  declarations without emitting runtime dispatch.
- `__declspec(code_seg("NAME"))` parses the requested code section. Slate
  reports it as unimplemented.

Slate registers these additional `__declspec` names in Clang mode for
compatibility. They have no translation effect.

- `allocator`, `no_init_all`, `property`, `safebuffers`, and `uuid`.
- `empty_bases`, `guard`, `layout_version`, and `novtable` on Windows.

## AArch64 SME attributes

Slate records these on AArch64 function declarations and definitions. It also
accepts corresponding `[[arm::...]]` spellings such as `[[arm::streaming]]`.
State arguments are `"za"` or `"zt0"`. Translation to Rust stops when it
requires an SME function state.

- `__arm_streaming` requests streaming mode at the function boundary.
- `__arm_streaming_compatible` marks a function callable in either mode.
- `__arm_locally_streaming` requests streaming mode for the function body.
- `__arm_new(STATE)` requests a new state local to the function.
- `__arm_in(STATE)` marks state as input.
- `__arm_out(STATE)` marks state as output.
- `__arm_inout(STATE)` marks state as input and output.
- `__arm_preserves(STATE)` marks state that the function preserves.
