# IR Reference

Slate IR represents a checked C translation unit for one configuration. It is
the input to IR transforms, analysis, and Rust lowering. Sema resolves types,
implicit conversions, storage layout, and calling conventions before emitting
the IR. You can inspect it with `slate emit-slate-ir input.c`.

- [IR Module](ir-module.md) explains the target header, module settings, item
  lists, and source information.
- [IR Operations and Policies](ir-operations.md) explains the choices on
  arithmetic, conversions, memory accesses, and atomics.
- [Printed IR Grammar](ir-grammar.md) gives the exact syntax of the dump.

## Types and layout

`void` has no value and `bool` carries a boolean. `iN` and `uN` are signed and
unsigned integers of `N` bits. The `b` suffix names a bit-precise C integer, so
`i128b` identifies a different type from `i128` even at the same width. `bf16`,
`f16`, `f32`, `f64`, `f80`, and `f128` name binary floating formats. `d32`,
`d64`, and `d128` name decimal formats.

`complex<T>` contains a real and an imaginary component. `imaginary<T>` contains
only the imaginary component. `fixed<T, N>` uses an integer storage word `T`
with `N` fractional bits, and `sat_fixed<T, N>` selects its saturating variant.
`vector<T, N>` has `N` scalar lanes. An optional third argument names its
underlying SVE type. `sizeless<T>` has no compile-time byte size.

- `ptr<T>` gives the pointee type and can carry its `const`, `volatile`, and
  `atomic` qualifications. Its optional suffix chooses a representation.
  - No suffix uses the target default.
  - `ptr32_sptr` is a 32-bit pointer with signed extension.
  - `ptr32_uptr` is a 32-bit pointer with unsigned extension.
  - `ptr64` is an explicit 64-bit pointer.
- `array<T, …>` gives the element type and extent.
  - `N` gives a constant extent.
  - `incomplete` means the extent is unavailable.
- `vla<T, …>` gives the element type and variable extent.
  - `%N` names the binding holding the captured runtime extent.
  - `*` means no extent is captured in this context.

`fn(...) -> T` gives the parameter and return types and can specify a calling
convention. `unprototyped` records a declaration with no prototype, and `...`
marks a variadic signature. `va_list` is the target's variadic argument cursor.

### Type definitions

`@typeN` identifies an alias, record, or enum definition. An alias names another
type. A record is a `struct` with separate members or a `union` with overlapping
members. Each `fieldN` gives the declaration index, optional name, type, and
optional bit width after `:`. A record or enum can be `incomplete` when its
definition is unavailable. A complete enum carries its resolved underlying
integer type and enumerator values.

A record's layout can contain these fields.

- `size` is total storage in bytes.
- `align` is required alignment in bytes.
- `offsets` gives one byte offset per field.
- `bit_offsets` gives the bit offset from the start of the record for each
  bit-field.
- `bit_units` lists the `(byte offset, byte size)` of each storage unit.
- `field_units` gives the storage unit index for each bit-field.
- `underaligned` lists indices of fields placed below their usual type
  alignment.

An ordinary field has `None` in the applicable per-field bit-field lists.

## Objects, functions, and ABI

`global` declares an object definition, which is zero-initialized when it has no
initializer. `extern` declares an object without defining it. `let` declares a
local or introduces a `[synthetic]` binding for an intermediate result, captured
extent, or other value created during IR construction.

### Object fields

- `storage` gives the object's lifetime.
  - `automatic` lasts until execution leaves the object's block.
  - `static` lasts for the entire program.
  - `thread` provides separate storage for each thread's lifetime.
- `linkage` determines where declarations can name the same entity.
  - `internal` is limited to the translation unit.
  - `external` allows declarations across translation units to name the same
    entity.
- `restrict` records C's restricted-access qualification.
- `const` qualifies the object as read-only through its declared type.
- `constexpr` marks a C constant-expression declaration.
- `byref` marks a `__block` object whose storage is shared with captures.
- `align=N` gives the object's recorded alignment in bytes.
- `cleanup=%N` names the function called when the automatic object leaves scope.
- `register="…"` gives an explicit register binding.
- `auto_init` gives the compiler's initialization policy for an automatic
  object.
  - `zero` fills the object with zeroes.
  - `pattern(typed, integer=…, floating=…)` uses separate fill bytes for integer
    and floating representations.
  - `pattern(value_bits=…)` fills value bits from a byte pattern and zeros the
    remaining bits.
- `= value` holds the initializer written in the source.
- `common` marks an eligible external tentative definition as a common symbol.

### Symbol fields

- `asm_name="…"` gives the symbol's linker spelling.
- `visibility` controls symbol visibility at link time, independently of C
  linkage.
  - `default` uses ordinary target visibility.
  - `hidden` limits visibility to the defining linkage component.
  - `protected` remains externally visible and binds internal references
    locally.
  - `internal` requests the target's stronger internal visibility.
- `weak` gives the symbol weak linkage.
- `alias="…"` names the symbol that this symbol aliases.
- `weakref="…"` redirects uses to a weak target without defining an alias
  symbol.
- `ifunc="…"` names the resolver for an indirect function.
- `section="…"` gives the output section.
- `used` asks the compiler to emit the item.
- `retain` asks the linker to keep the item.
- `tls_model` selects how to address thread-local storage.
  - `global-dynamic` resolves the variable dynamically.
  - `local-dynamic` resolves the current module's TLS block dynamically.
  - `initial-exec` assumes a static TLS allocation at startup.
  - `local-exec` uses a link-time offset for locally resolved TLS.
- `dllimport` requests import through a Windows DLL.
- `dllexport` requests export from a Windows DLL.
- `selectany` lets the linker select among duplicate definitions.
- `fallback="…"` gives the linker an alternate for a missing primary symbol.
- `include` asks the linker to keep the symbol.

### Function fields

`fn` without a body is a declaration. Parameters carry their binding identity,
type, and qualifiers. The fields below describe parameters and functions.

- `array` retains the extent of an array parameter adjusted to a pointer.
  - `N` gives a constant extent.
  - `%N` names a binding holding the runtime extent.
  - `*` marks an unspecified extent.
  - `static` records C's minimum extent contract, with the bound when available.
- `fallthrough` gives the behavior of reaching the end of the body.
  - `ret_zero` returns zero, as in a C99 or later `main`.
  - `ret_void` returns without a value.
  - `ret(value)` evaluates and returns the supplied value.
  - `ub_if_used` gives undefined behavior if the caller uses the missing result.
  - `ub` gives undefined behavior on reaching the end.
- `inline` gives the inlining preference.
  - `hint` suggests inlining.
  - `always` requests inlining whenever possible.
  - `never` requests that calls remain out of line.
- `definition` gives the emission policy for the function body.
  - `emitted` supplies a linkable definition.
  - `inline_only` supplies an inline body without a linkable definition.
  - `discardable` can be kept private or coalesced with duplicate definitions.
  - `weak_odr` can be coalesced and supplies a linkable symbol.
- `noreturn` marks a function that does not return normally.
- `naked` suppresses the ordinary function prologue and epilogue.
- `interrupt` requests an interrupt calling convention.
- `constructor=N` gives the startup registration priority.
- `destructor=N` gives the teardown registration priority.
- `overloadable` lets distinct bindings share a spelled function name.
- `target` records per-function machine settings.
  - `+feature` enables a feature.
  - `-feature` disables a feature.
  - `arch=…` selects an architecture or CPU baseline.
  - `tune=…` selects a CPU for tuning.
  - `branch-protection=…` sets branch protection.
- `sme` records AArch64 streaming mode or use of the `za` and `zt0` states.
  - `streaming` uses streaming mode.
  - `streaming_compatible` can run in either mode.
  - `locally_streaming` switches to streaming mode in the function body.
  - `new` gives the function its own state.
  - `in` takes state from the caller.
  - `out` supplies state to the caller.
  - `inout` takes and supplies state.
  - `preserves` leaves the named state intact.
- `memory` records a declared memory-effect contract.
  - `none` records the GNU `const` contract.
  - `read` records the GNU `pure` contract and allows reads without writes.
  - Absent means there is no special memory-effect promise.
- `deallocator=%N` names a function that releases the returned pointer.
- `argument=K` gives the deallocator's zero-based parameter for that pointer.

### ABI fields

- `abi` gives the convention and argument and result passing classes.
  - A signature gives the resolved convention and passing classes.
  - `incomplete` means a record in the signature is still incomplete.
  - Absent can mean that the signature uses the default scalar rules.
- The **ABI family** identifies the platform's calling rules.
  - `sysv64` is x86-64 System V.
  - `win64` is Windows x64.
  - `x86_cdecl` and `x86_win32` are the 32-bit x86 System V and Windows
    families.
  - `aapcs64` and `win_arm64` are the AArch64 and Windows Arm64 families.
  - `aapcs32` and `aapcs32_hard_float` are 32-bit Arm with base and hardware
    floating-point argument passing, respectively.
- An explicit **calling convention** refines the family.
  - `stdcall` uses the x86 callee-cleanup convention.
  - `fastcall` passes eligible early arguments in registers.
  - `vectorcall` adds vector register argument passing.
  - `regcall` uses the target's register-heavy convention.
  - `thiscall` uses the target's C++ member-function convention.
  - `ms_abi` requests Microsoft's ABI.
  - `sysv_abi` requests the System V ABI.
- `regparm=N` selects how many eligible integer arguments can use x86 registers.
- `sseregparm` selects register passing for eligible x86 floating arguments.
- The **passing class** describes how one argument or result is conveyed.
  - `scalar` uses the convention's ordinary scalar rules.
  - `direct` passes the value as its own type.
  - `native_c` records agreement with Rust's C ABI for the representation.
  - `coerce<…>` gives the pieces used in registers.
  - `scalable` uses the scalable-vector calling convention.
  - `byval<align=N>` passes a copy in memory.
  - `byref<align=N>` passes an indirect argument.
  - `sret<align=N>` uses caller-provided result storage.
  - `void` denotes an absent argument or result.

## Statements and evaluation

`write<T>(place, value)` stores to a place. A value used as a statement is
evaluated for its effects. A bare `;` represents an empty statement. `return`
leaves the function and can supply a result. A normal `{ … }` forms a lexical
scope. `block %N { … }` can be introduced by a transform so `break %N` can leave
that block.

`if` evaluates its condition and runs one branch. `switch` chooses a case or
default, with `case` and `default` carrying the ID of the switch they belong to.
A case can cover an inclusive range. Statement order and the absence of a
transfer express fallthrough.

- `folded` records a known condition or selector.
  - `true` and `false` give a known `if` condition.
  - An integer gives a known `switch` selector.

`while` tests before an iteration and `do` tests after it. `for` has an
initializer, condition, increment, and body, with `omitted` recording an absent
condition or increment. Conditions and increments can contain
`{ … yield value; }` to keep side effects at the point where that expression is
evaluated. `break` names the loop, switch, or labeled block it leaves, and
`continue` names the loop whose continuation point it resumes. `label` defines
an entry for `goto`. `goto %N` transfers to a named label and `goto *value`
transfers to a computed label address. `[unsequenced]` identifies a statement
position that commits to an order C leaves unspecified.

`asm` carries a template and optional structured operands, clobbers, and jump
labels. A resolved `template:` refers to operand and label indices in the
printed statement.

- `dialect` identifies x86 assembler syntax.
  - `att` uses AT&T syntax.
  - `intel` uses Intel syntax.
- `alternative` records constraint selection.
  - `N` selects the alternative at that index.
  - `none` means no alternative fits.
- `options` records properties derived from operands, clobbers, and semantics.
  - `pure` marks an assembly expression with no effects beyond its outputs.
  - `nomem` means it accesses no externally visible memory.
  - `readonly` means it can read externally visible memory without writing it.
  - `nostack` means it uses no stack space for its own operations.
  - `preserves_flags` means it preserves the relevant processor flags.
  - `may_unwind` means it can unwind.

`nested_fn` defines a nested C function with its captures. `block_fn` gives the
body associated with a `block_literal`. `macro_body` identifies a region of
statements supplied by an expansion in macro-aware output. `seh_try` records
MSVC structured exception handling with an exception filter and handler or a
finalizer. `seh_leave` exits the named protected region.

### OpenMP fields

- `num_threads` requests the thread count for `omp_parallel`.
- `if` supplies the condition for running the region in parallel.
- `shared` identifies a binding shared by the region's threads.
- `private` creates a separate binding for each thread.
- `firstprivate` initializes each private copy from the original value.
- `schedule` gives how `omp_for` distributes iterations.
  - `static` assigns chunks up front.
  - `dynamic` assigns chunks as threads request more work.
  - `guided` assigns chunks that shrink as work runs out.
  - `auto` delegates scheduling to the implementation.
  - `runtime` reads the runtime schedule setting.
- A **schedule modifier** qualifies the order in which threads execute chunks.
  - `monotonic` keeps each thread's assigned chunks in increasing order.
  - `nonmonotonic` allows a different order.
- `chunk` supplies an optional chunk size.
- `nowait` removes the implicit barrier at the end of the loop.

A standalone `fence` records an atomic ordering for thread or signal
synchronization. See
[IR Operations and Policies](ir-operations.md#access-and-atomic-fields).

## Places

A place describes storage. Forming `deref(value)` identifies the location
designated by a pointer. It takes an explicit `read` or `write` to access that
location. `%N` identifies a bound place. `index(base, index)` is the indexed
place form, though C array and pointer subscripts normally print as
`deref(ptr_offset(...))`.

`fieldN(base)` projects a field by declaration index. The bit-field form adds
`unit`, `bytes`, and `bits`. `unit` selects a storage unit from the record
layout, `bytes` gives its half-open byte range in the record, and `bits` gives
the half-open range measured from that unit's least significant bit. The
bit-field can be read or written without having an independent address.

`real` and `imag` project complex components. `lane` and `swizzle` project a
vector lane or distinct selected lanes. `compound_literal` makes a distinct
object with the given storage duration, alignment, and initializer. `temporary`
materializes an aggregate rvalue for a read-only projection, such as selecting a
member of a returned struct.

## Values

`const<T>` represents a constant of type `T`, including `bits=0x…` to preserve a
floating NaN encoding. `null<T>` gives a null pointer and `void` gives no
result. `size_of`, `align_of`, and `offset_of` retain the queried type or member
along with the resolved number. `code_units` supplies the literal units of a
string. `aggregate` initializes selected fields, elements, or element ranges.

- `zero_fill` controls initialization of omitted aggregate subobjects.
  - `true` supplies implicit zero values.
  - `false` requests no implicit fill.

`copy<T>` carries a value through without changing its representation and
records the conversion reason.

`read` loads a place. `store` writes a value and yields the stored value.
Effects normalization can turn a store or non-atomic update into `write`
statements. `update`, `old`, and `compare_exchange` represent operations with an
old or new result or a conditional write. Their result, ordering, and scope
fields are described in [IR Operations and Policies](ir-operations.md).

`addr_of` forms a pointer to a place. `array_decay` forms a pointer to the
array's first element. `function_decay` forms a function pointer. `label_addr`
forms a label address for a computed jump. `ptr_offset` moves a pointer and
`ptr_diff` computes the distance between two pointers in elements.

- `length` records the known extent on `array_decay`.
  - `Some(N)` gives the number of elements.
  - `None` means no extent is recorded.
- `element` gives the type used for a pointer stride or difference.
- `subtract` selects the direction of a pointer offset.
  - `true` subtracts the amount.
  - `false` adds the amount.
- `same_array` gives the pointer difference's relationship requirement.
  - `required` means both pointers belong to the same array or are one past it.
- `overflow=ub` on a pointer difference requires a representable result.

`conditional` evaluates its test and exactly one result operand. `sequence`
evaluates its left operand before yielding the right. `call` names a binding for
a direct callee or a pointer value for an indirect callee. `signature` is the
type visible at the call site. `abi` gives the resolved passing classification
when needed.

`va_start` initializes a cursor, `va_copy` duplicates its position, `va_arg`
retrieves and advances past the requested value, and `va_end` ends use of the
cursor.

- `win64` selects the Windows variadic convention.
- The **passing class** gives the location or representation of an aggregate
  `va_arg`.
  - `memory` places the argument in the stack overflow area.
  - `regs=[…]` lists register pieces in order.
    - `int` selects an integer register piece.
    - `sse` selects an SSE register piece.
    - `sse_wide` selects a wider SSE register piece.
  - `hfa=NxM` describes a homogeneous floating-point aggregate.
  - `indirect` retrieves the argument through a pointer.

`lane<T>` extracts one element from a vector value. `shuffle` selects lanes from
one vector or the concatenation of two.

- `mask` says which input lanes `shuffle` selects.
  - A constant list gives a lane number or `undef` for each output lane.
  - `dynamic(value)` gives a runtime mask.

`intrinsic<T>` names an LLVM intrinsic using its base name and the types of the
operands and result. `block_literal` names an invoke binding and capture list. A
capture can be by value or `byref` for shared storage. The `seh_exception_code`,
`seh_exception_info`, and `seh_abnormal_termination` values query the active
exception or how the finalizer was entered.
