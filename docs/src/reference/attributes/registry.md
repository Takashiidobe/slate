# Compiler Attribute Registry

The lists below reproduce Slate's captured compiler spellings. Search by the
attribute name and check the heading for its compiler and syntax. Slate applies
its own flavor and target gates to individually handled names, so a name in a
compiler snapshot can still be rejected for a particular target.

When a spelling is recognized only for compatibility, Slate accepts it where the
syntax is valid and emits no corresponding Rust behavior. For example:

```c
__attribute__((no_stack_protector)) int inc(int x) { return x + 1; }
```

```rust
fn inc(x: i32) -> i32 {
    x + 1
}
```

Slate emits no per-function stack protector setting for this attribute.

The MSVC flavor accepts arbitrary names in `__declspec(...)` and
`[[msvc::...]]`. See [Microsoft spellings](microsoft.md) for the names Slate
handles individually. In GCC mode, Slate looks up `__declspec` names using the
GNU registry.

For compiler meanings and applicability, see the
[Clang attribute reference](https://clang.llvm.org/docs/AttributeReference.html),
[GCC attribute syntax](https://gcc.gnu.org/onlinedocs/gcc/Attribute-Syntax.html),
and
[Microsoft's `__declspec` reference](https://learn.microsoft.com/en-us/cpp/cpp/declspec).

## GNU spelling rules for individually handled names

These groups show the flavor and target rules for `__attribute__((name))`. The
Microsoft flavor has no GNU attribute syntax. Scoped spellings also have to
appear in the corresponding snapshot below.

### Clang any target, GCC any target

```text
alias, aligned, alloc_align, alloc_size, always_inline, assume_aligned,
cleanup, cold, common, const, constructor, counted_by, deprecated, destructor,
fallthrough, flatten, format, format_arg, gnu_inline, hot, leaf, malloc,
may_alias, mode, no_split_stack, nocommon, noinline, nonnull, noreturn,
nothrow, packed, pure, retain, returns_nonnull, returns_twice, section,
sentinel, target, target_clones, tls_model, transparent_union, unused, used,
vector_size, visibility, warn_unused_result, weak, weakref
```

### Clang any target, GCC unavailable

```text
address_space, annotate, availability, cpu_dispatch, cpu_specific,
ext_vector_type, lifetimebound, neon_polyvector_type, neon_vector_type,
optnone, overloadable, pass_dynamic_object_size, pass_object_size,
preserve_all, preserve_most, vectorcall, weak_import
```

### Clang any target, GCC Windows

```text
selectany
```

### Clang unavailable, GCC unavailable

```text
code_seg, maybe_unused, noalias, nodiscard, nomips16, restrict, thread
```

### Clang outside Arm32, GCC any target

```text
preserve_none
```

### Clang unavailable, GCC any target

```text
noclone, noipa, optimize, scalar_storage_order
```

### Clang any target, GCC x86 and Arm32

```text
naked
```

### Clang outside AArch64, GCC x86 and Arm32

```text
interrupt
```

### Clang outside Windows, GCC any target

```text
ifunc
```

### Clang Windows, GCC Windows

```text
dllexport, dllimport
```

### Clang any target, GCC x86

```text
cdecl, fastcall, gcc_struct, ms_abi, ms_struct, regparm, stdcall, sysv_abi,
thiscall
```

### Clang any target, GCC Arm32

```text
pcs
```

## Captured compiler spellings

The snapshots below are unions across the targets queried by Slate. Their source
is the
[checked-in compiler registry](https://github.com/Takashiidobe/slate/blob/main/crates/slate-parser/src/attribute_support/registered.rs).

### Clang GNU syntax

```text
NSObject, __clang_arm_builtin_alias, __clang_arm_mve_strict_polymorphism,
__const, aarch64_sve_pcs, aarch64_vector_pcs, abi_tag, acquire_capability,
acquire_handle, acquire_shared_capability, acquired_after, acquired_before,
address_space, alias, align_value, aligned, alloc_align, alloc_size,
allocating, always_destroy, always_inline, amdgpu_flat_work_group_size,
amdgpu_kernel, amdgpu_max_num_work_groups, amdgpu_num_sgpr, amdgpu_num_vgpr,
amdgpu_waves_per_eu, analyzer_noreturn, annotate, argument_with_type_tag,
arm_sve_vector_bits, artificial, assert_capability, assert_exclusive_lock,
assert_shared_capability, assert_shared_lock, assume, assume_aligned, atomic,
availability, available_only_in_default_eval_method, blocking, blocks,
bounded, btf_decl_tag, btf_type_tag, callable_when, callback, called_once,
capability, carries_dependency, cdecl, cf_audited_transfer, cf_consumed,
cf_returns_not_retained, cf_returns_retained, cf_unknown_transfer,
cfi_canonical_jump_table, cfi_salt, cfi_unchecked_callee, clang_builtin_alias,
cleanup, clspv_libclc_builtin, cluster_dims, cmse_nonsecure_call,
cmse_nonsecure_entry, code_align, cold, common, const, constant, constructor,
consumable, consumable_auto_cast_state, consumable_set_state_on_read,
convergent, coro_await_elidable, coro_await_elidable_argument,
coro_disable_lifetimebound, coro_lifetimebound,
coro_only_destroy_when_complete, coro_return_type, coro_wrapper, counted_by,
counted_by_or_null, cpu_dispatch, cpu_specific, cudart_builtin, deprecated,
destructor, device, device_builtin, device_builtin_surface_type,
device_builtin_texture_type, device_kernel, diagnose_as_builtin, diagnose_if,
disable_sanitizer_instrumentation, disable_tail_calls, dllexport, dllimport,
enable_if, enforce_read_only_placement, enforce_tcb, enforce_tcb_leaf,
enum_extensibility, error, exclude_from_explicit_instantiation,
exclusive_lock_function, exclusive_locks_required, exclusive_trylock_function,
ext_vector_type, external_source_symbol, fallthrough, fastcall, flag_enum,
flatten, force_align_arg_pointer, format, format_arg, format_matches,
function_return, gcc_struct, global, gnu_inline, grid_constant, guard,
guarded_by, guarded_var, host, hot, ibaction, iboutlet, iboutletcollection,
ifunc, init_priority, intel_ocl_bicc, intel_reqd_sub_group_size,
internal_linkage, interrupt, interrupt_save_fp, launch_bounds, leaf,
lifetime_capture_by, lifetime_capture_by_global, lifetime_capture_by_this,
lifetime_capture_by_unknown, lifetimebound, loader_uninitialized,
lock_returned, lockable, locks_excluded, lto_visibility_public, m68k_rtd,
malloc, malloc_span, managed, matrix_type, may_alias, maybe_undef,
mig_server_routine, min_vector_width, minsize, mode, model, modular_format,
ms_abi, ms_struct, musttail, naked, neon_polyvector_type, neon_vector_type,
no_address_safety_analysis, no_builtin, no_caller_saved_registers, no_cluster,
no_destroy, no_field_protection, no_instrument_function, no_outline,
no_profile_instrument_function, no_randomize_layout, no_sanitize,
no_sanitize_address, no_sanitize_memory, no_sanitize_thread,
no_specializations, no_speculative_load_hardening, no_split_stack,
no_stack_protector, no_thread_safety_analysis, nocf_check, nocommon,
noconvergent, nodebug, noderef, noduplicate, noescape, noinline, nomerge,
nonallocating, nonblocking, nonnull, nonstring, noreturn, nosvm,
not_tail_called, nothrow, nouwtable, ns_consumed, ns_consumes_self,
ns_error_domain, ns_returns_autoreleased, ns_returns_not_retained,
ns_returns_retained, nv_weak, nvptx_kernel,
objc_arc_weak_reference_unavailable, objc_boxable, objc_bridge,
objc_bridge_mutable, objc_bridge_related, objc_class_stub,
objc_designated_initializer, objc_direct, objc_direct_members, objc_exception,
objc_externally_retained, objc_gc, objc_independent_class, objc_method_family,
objc_non_runtime_protocol, objc_nonlazy_class, objc_ownership,
objc_precise_lifetime, objc_requires_property_definitions,
objc_requires_super, objc_returns_inner_pointer, objc_root_class,
objc_runtime_name, objc_runtime_visible, objc_subclassing_restricted,
opencl_constant, opencl_generic, opencl_global, opencl_global_device,
opencl_global_host, opencl_local, opencl_private, opencl_unroll_hint, optnone,
os_consumed, os_consumes_this, os_returns_not_retained, os_returns_retained,
os_returns_retained_on_non_zero, os_returns_retained_on_zero,
overflow_behavior, overloadable, ownership_holds, ownership_returns,
ownership_takes, packed, param_typestate, pascal, pass_dynamic_object_size,
pass_object_size, patchable_function_entry, pcs, personality,
pointer_field_protection, pointer_with_type_tag, preferred_name,
preferred_type, preserve_all, preserve_most, preserve_none, pt_guarded_by,
pt_guarded_var, ptrauth_vtable_pointer, pure, randomize_layout,
reentrant_capability, regcall, regparm, reinitializes, release_capability,
release_generic_capability, release_handle, release_shared_capability,
reqd_work_group_size, require_constant_initialization,
require_explicit_initialization, requires_capability,
requires_shared_capability, retain, return_typestate, returns_nonnull,
returns_twice, riscv_rvv_vector_bits, scoped_lockable, section, selectany,
sentinel, set_typestate, shared, shared_capability, shared_lock_function,
shared_locks_required, shared_trylock_function, sized_by, sized_by_or_null,
speculative_load_hardening, stack_protector_ignore, standalone_debug, stdcall,
suppress, swift_async, swift_async_context, swift_async_error,
swift_async_name, swift_attr, swift_bridge, swift_bridged_typedef,
swift_context, swift_error, swift_error_result, swift_indirect_result,
swift_name, swift_newtype, swift_objc_members, swift_private, swift_wrapper,
swiftasynccall, swiftcall, sycl_kernel, sycl_special_class, sysv_abi, target,
target_clones, target_version, test_typestate, thiscall, tls_model,
transparent_union, trivial_abi, try_acquire_capability,
try_acquire_shared_capability, type_tag_for_datatype, type_visibility,
unavailable, uninitialized, unlock_function, unsafe_buffer_usage, unused,
use_handle, used, using_if_exists, vec_type_hint, vecreturn, vector_size,
vectorcall, visibility, warn_unused, warn_unused_result, warning, weak,
weak_import, weakref, work_group_size_hint, xray_always_instrument,
xray_log_args, xray_never_instrument, zero_call_used_regs
```

### GCC GNU syntax

```text
NSObject, _Noreturn, aarch64_vector_pcs, access, alias, aligned, alloc_align,
alloc_size, always_inline, arm_sve_vector_bits, artificial, assume,
assume_aligned, btf_decl_tag, btf_type_tag, callee_pop_aggregate_return,
cdecl, cf_check, cleanup, cold, common, const, constructor, copy, counted_by,
deprecated, designated_init, destructor, error, expected_throw,
externally_visible, fallthrough, fastcall, fd_arg, fd_arg_read, fd_arg_write,
fentry_name, fentry_section, flag_enum, flatten, force_align_arg_pointer,
format, format_arg, function_return, gcc_struct, gnu_inline, hardbool, hot,
ifunc, indirect_branch, indirect_return, interrupt, leaf, malloc, may_alias,
maybe_unused, mode, ms_abi, ms_hook_prologue, ms_struct, musttail, naked,
no_address_safety_analysis, no_callee_saved_registers,
no_caller_saved_registers, no_icf, no_instrument_function,
no_profile_instrument_function, no_reorder, no_sanitize, no_sanitize_address,
no_sanitize_coverage, no_sanitize_thread, no_sanitize_undefined,
no_split_stack, no_stack_limit, no_stack_protector, nocf_check, noclone,
nocommon, nodirect_extern_access, nodiscard, noinit, noinline, noipa, nonnull,
nonnull_if_nonzero, nonstring, noplt, noreturn, nothrow,
null_terminated_string_arg, objc_nullability, objc_root_class, optimize,
packed, patchable_function_entry, persistent, preserve_none, pure, regparm,
reproducible, retain, returns_nonnull, returns_twice, scalar_storage_order,
section, sentinel, signed_bool_precision, simd, sseregparm, stack_protect,
stdcall, strict_flex_array, strub, symver, sysv_abi, tainted_args, target,
target_clones, target_version, thiscall, tls_model, transaction_callable,
transaction_may_cancel_outer, transaction_pure, transaction_safe,
transaction_safe_dynamic, transaction_unsafe, transaction_wrap,
transparent_union, unavailable, uninitialized, unsequenced, unused, used,
vector_mask, vector_size, visibility, volatile, warn_if_not_aligned,
warn_unused, warn_unused_result, warning, weak, weakref, zero_call_used_regs
```

### Clang gnu scope

```text
__const, alias, aligned, alloc_align, alloc_size, always_inline, artificial,
assume_aligned, cdecl, cleanup, cold, common, const, constructor, deprecated,
destructor, dllexport, dllimport, error, fallthrough, fastcall, flatten,
force_align_arg_pointer, format, format_arg, format_matches, function_return,
gcc_struct, gnu_inline, hot, ifunc, interrupt, leaf, malloc, may_alias, mode,
model, ms_abi, ms_struct, naked, no_address_safety_analysis,
no_caller_saved_registers, no_instrument_function,
no_profile_instrument_function, no_randomize_layout, no_sanitize,
no_sanitize_address, no_sanitize_thread, no_split_stack, no_stack_protector,
nocf_check, nocommon, nodebug, noinline, nonnull, nonstring, noreturn,
nothrow, packed, patchable_function_entry, pcs, pure, randomize_layout,
regcall, regparm, retain, returns_nonnull, returns_twice, section, selectany,
sentinel, stdcall, sysv_abi, target, target_clones, target_version, thiscall,
tls_model, transparent_union, unused, used, vector_size, visibility,
warn_unused, warn_unused_result, warning, weak, weakref, zero_call_used_regs
```

### Clang clang scope

```text
NSObject, __clang_arm_builtin_alias, __clang_arm_mve_strict_polymorphism,
aarch64_sve_pcs, aarch64_vector_pcs, acquire_handle, address_space,
allocating, always_inline, amdgpu_kernel, annotate, annotate_type,
argument_with_type_tag, assume, atomic, availability,
available_only_in_default_eval_method, blocking, blocks, btf_decl_tag,
btf_type_tag, builtin_alias, callback, called_once, cf_audited_transfer,
cf_consumed, cf_returns_not_retained, cf_returns_retained,
cf_unknown_transfer, cfi_canonical_jump_table, cfi_salt, cfi_unchecked_callee,
clspv_libclc_builtin, code_align, convergent, coro_await_elidable,
coro_await_elidable_argument, coro_disable_lifetimebound, coro_lifetimebound,
coro_only_destroy_when_complete, coro_return_type, coro_wrapper, counted_by,
counted_by_or_null, cpu_dispatch, cpu_specific, device_kernel,
diagnose_as_builtin, disable_sanitizer_instrumentation, disable_tail_calls,
enforce_read_only_placement, enforce_tcb, enforce_tcb_leaf,
enum_extensibility, exclude_from_explicit_instantiation, ext_vector_type,
external_source_symbol, flag_enum, guard, ibaction, iboutlet,
iboutletcollection, internal_linkage, lifetime_capture_by,
lifetime_capture_by_global, lifetime_capture_by_this,
lifetime_capture_by_unknown, lifetimebound, likely, loader_uninitialized,
lto_visibility_public, m68k_rtd, malloc_span, matrix_type, maybe_undef,
mig_server_routine, min_vector_width, minsize, modular_format, musttail,
neon_polyvector_type, neon_vector_type, no_builtin, no_field_protection,
no_outline, no_sanitize, no_sanitize_memory, no_speculative_load_hardening,
no_stack_protector, no_thread_safety_analysis, noconvergent, noderef,
noduplicate, noescape, noinline, nomerge, nonallocating, nonblocking,
not_tail_called, nouwtable, ns_consumed, ns_consumes_self,
ns_returns_autoreleased, ns_returns_not_retained, ns_returns_retained,
nvptx_kernel, objc_arc_weak_reference_unavailable, objc_boxable, objc_bridge,
objc_bridge_mutable, objc_bridge_related, objc_class_stub,
objc_designated_initializer, objc_direct, objc_direct_members, objc_exception,
objc_externally_retained, objc_gc, objc_independent_class, objc_method_family,
objc_non_runtime_protocol, objc_nonlazy_class, objc_ownership,
objc_precise_lifetime, objc_requires_property_definitions,
objc_requires_super, objc_returns_inner_pointer, objc_root_class,
objc_runtime_name, objc_runtime_visible, objc_subclassing_restricted,
opencl_constant, opencl_generic, opencl_global, opencl_global_device,
opencl_global_host, opencl_local, opencl_private, optnone, os_consumed,
os_consumes_this, os_returns_not_retained, os_returns_retained,
os_returns_retained_on_non_zero, os_returns_retained_on_zero,
overflow_behavior, overloadable, ownership_holds, ownership_returns,
ownership_takes, pascal, pass_dynamic_object_size, pass_object_size,
personality, pointer_field_protection, pointer_with_type_tag, preferred_type,
preserve_all, preserve_most, preserve_none, ptrauth_vtable_pointer,
reentrant_capability, release_handle, require_explicit_initialization,
sized_by, sized_by_or_null, speculative_load_hardening,
stack_protector_ignore, suppress, swift_async, swift_async_context,
swift_async_error, swift_context, swift_error_result, swift_indirect_result,
swiftasynccall, swiftcall, sycl_kernel, sycl_special_class,
type_tag_for_datatype, type_visibility, unavailable, unlikely,
unsafe_buffer_usage, use_handle, vectorcall, weak_import,
xray_always_instrument, xray_log_args, xray_never_instrument
```

### Clang msvc scope

```text
forceinline, forceinline_calls, noinline
```

### GCC gnu scope

```text
NSObject, aarch64_vector_pcs, access, alias, aligned, alloc_align, alloc_size,
always_inline, arm_sve_vector_bits, artificial, assume, assume_aligned,
btf_decl_tag, btf_type_tag, callee_pop_aggregate_return, cdecl, cf_check,
cleanup, cold, common, const, constructor, copy, counted_by, deprecated,
designated_init, destructor, error, expected_throw, externally_visible,
fallthrough, fastcall, fd_arg, fd_arg_read, fd_arg_write, fentry_name,
fentry_section, flag_enum, flatten, force_align_arg_pointer, format,
format_arg, function_return, gcc_struct, gnu_inline, hardbool, hot, ifunc,
indirect_branch, indirect_return, interrupt, leaf, malloc, may_alias, mode,
ms_abi, ms_hook_prologue, ms_struct, musttail, naked,
no_address_safety_analysis, no_callee_saved_registers,
no_caller_saved_registers, no_icf, no_instrument_function,
no_profile_instrument_function, no_reorder, no_sanitize, no_sanitize_address,
no_sanitize_coverage, no_sanitize_thread, no_sanitize_undefined,
no_split_stack, no_stack_limit, no_stack_protector, nocf_check, noclone,
nocommon, nodirect_extern_access, noinit, noinline, noipa, nonnull,
nonnull_if_nonzero, nonstring, noplt, noreturn, nothrow,
null_terminated_string_arg, objc_nullability, objc_root_class, optimize,
packed, patchable_function_entry, persistent, preserve_none, pure, regparm,
reproducible, retain, returns_nonnull, returns_twice, scalar_storage_order,
section, sentinel, signed_bool_precision, simd, sseregparm, stack_protect,
stdcall, strict_flex_array, strub, symver, sysv_abi, tainted_args, target,
target_clones, target_version, thiscall, tls_model, transaction_callable,
transaction_may_cancel_outer, transaction_pure, transaction_safe,
transaction_safe_dynamic, transaction_unsafe, transaction_wrap,
transparent_union, unavailable, uninitialized, unsequenced, unused, used,
vector_mask, vector_size, visibility, volatile, warn_if_not_aligned,
warn_unused, warn_unused_result, warning, weak, weakref, zero_call_used_regs
```
