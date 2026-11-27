# IR Operations and Policies

An operation gives its type, its operands, and the policies that affect its
result or effects. For example, `add<i32, overflow=ub>` computes a signed 32-bit
sum. If it overflows, the source program has undefined behavior. This field
describes the consequence of overflow. It supplies no range proof for the
operands.

## Integer and fixed-point arithmetic

- `overflow` gives the behavior when the result is out of range.
  - `ub` gives undefined behavior. This is the default for signed addition,
    subtraction, multiplication, and negation.
  - `wrap` keeps the low `N` bits of an `N`-bit result. It applies to unsigned
    arithmetic and signed arithmetic covered by `-fwrapv`.
  - `trap` requires a trap for operations covered by `-ftrapv`.
  - `saturate` clamps a saturating fixed-point result to the nearest endpoint.
- `by_zero` gives the division or remainder policy for a zero divisor.
  - `ub` gives undefined behavior.
- `min_by_neg_one` gives the policy for dividing the minimum signed value by
  `-1`.
  - `ub` gives undefined behavior for the quotient or remainder operation.

The signed wrapping option leaves both division policies in effect.

For `int add(int a, int b) { return a + b; }`, compare the output of
`slate emit-slate-ir input.c`, `slate emit-slate-ir -fwrapv input.c`, and
`slate emit-slate-ir -ftrapv input.c`. The same operands and `i32` computation
type appear in each dump, with `overflow=ub`, `wrap`, and `trap`, respectively.

- `amount_out_of_range` gives the policy for an invalid shift count.
  - `ub` gives undefined behavior for a negative count or one at least as large
    as the width of the promoted left operand.
- `negative_left` gives the signed left-shift policy for a negative operand.
  - `ub` gives undefined behavior.
- `fill` selects the bits introduced by a right shift.
  - `sign_extend` copies the sign bit.
  - `zero_extend` shifts in zeroes.

The shift count keeps its own promoted type.

`add`, `sub`, `mul`, `div`, and `rem` are the usual binary arithmetic nodes.
`neg` is unary negation. `and`, `or`, `xor`, and `not` apply the bitwise
operation, with `not<bool>` used for boolean negation.

- `elementwise=true` applies the operation lane by lane to vectors.
- `complex=true` selects arithmetic over the real and imaginary components.

`overflow_add`, `overflow_sub`, and `overflow_mul` write the converted
arithmetic result to an output place and return whether it overflowed. The
normal `overflow=ub` contract does not describe these checked operations.

## Floating-point arithmetic

- `rounding` selects how to represent an inexact result.
  - `nearest_even` rounds to nearest with ties to even.
  - `environment` uses the rounding mode active when the operation executes.
  - `toward_zero` discards fractional precision in fixed-point operations.
- `exceptions` gives the floating-point exception policy.
  - `ignore` allows the operation to disregard effects on the exception
    environment.
  - `observable` preserves observable exception effects, even with fixed
    rounding.
- `contract` gives permission to fuse eligible floating-point operations.
  - `off` disallows fusion.
  - `on` permits fusion within a source expression.
  - `fast` also permits fusion across statements.
- `range` gives the permitted complex multiplication or division behavior.
  - `full` handles infinities and NaNs.
  - `basic` permits the direct formulas.
  - `improved` permits scaled division and basic multiplication.

`minimum` and `maximum` propagate a NaN. `minnum`, `maxnum`, `minimum_num`, and
`maximum_num` prefer the numeric operand when only one is a NaN. `minimum`,
`maximum`, `minimum_num`, and `maximum_num` order negative zero before positive
zero. `minnum` and `maxnum` do not specify that ordering.

`eq`, `ne`, `lt`, `le`, `gt`, and `ge` print the comparison's operand type. A
scalar comparison returns `bool`. A vector comparison can print
`result=vector<…>` for the lane-mask type. `logical_and` and `logical_or`
evaluate the right operand only when needed.

- `test` selects the boolean predicate of `float_class`, which raises no
  exception.
  - `nan` checks for a NaN.
  - `infinite` checks for either infinity.
  - `finite` checks for a value other than an infinity or NaN.
  - `normal` checks for a normal finite nonzero value.
  - `subnormal` checks for a subnormal value.
  - `zero` checks for either signed zero.
  - `signaling` checks for a signaling NaN.
  - `sign_bit` reads the sign bit.

See [Floating-point environment](../floating-point-environment.md) for an
example where one C pragma changes the policies on only one division.

## Conversions

The type on a conversion is the destination.

- `reason` records the source context that required the conversion.
  - `promotion` is an integer promotion.
  - `usual_arith` reconciles arithmetic operands.
  - `excess_precision` selects or leaves a wider evaluation format.
  - `assign` converts to an assignment destination's type.
  - `arg` converts to a fixed parameter's type.
  - `vararg` applies a default argument promotion.
  - `return` converts to the function's result type.
  - `explicit` denotes a written cast.
- `fits` records whether a truncate or reinterpret preserves the numeric value.
  - `always` means the input is known to fit.
  - `unknown` means there is no recorded fit proof.
- `exact` records whether `int_to_float` can require rounding.
  - `true` means the result is known to be exact.
  - `false` means rounding can be needed.
- `out_of_range` gives the policy for `float_to_int` outside the destination
  range.
  - `ub` gives undefined behavior if the truncated value cannot be represented.

`widen` extends using the source signedness. `truncate` keeps the low bits.
`reinterpret` changes the interpretation at the same width, commonly to change
signedness. A conversion can have well-defined low bits even when the original
numeric value does not fit.

`from_bool` produces integer zero or one. `bit_cast` copies representation bits
between equal-size types, while `enum_to_int` and `int_to_enum` convert between
an enum and its integer representation.

`float_widen` widens a floating format, `float_narrow` narrows it, and
`float_convert` crosses between formats such as decimal and binary. Conversions
that can round carry the applicable `rounding` and `exceptions` fields.
`float_to_int` truncates toward zero.

`pointer_cast` changes pointer type. `address_space_cast` changes its
representation class. `ptr_to_int` and `int_to_ptr` cross the pointer/integer
boundary. Pointer arithmetic is separate from these conversions.

`real_to_complex`, `complex_to_real`, `complex_to_imag`, and `complex_convert`
construct a complex value, extract the indicated component, or convert both
components. `real_to_imaginary`, `imaginary_to_real`, `imaginary_to_complex`,
`complex_to_imaginary`, and `imaginary_convert` express the corresponding
changes involving imaginary types. `vector_splat` repeats a scalar in each lane
and `vector_bit_cast` reinterprets the vector's representation. The
`int_to_fixed`, `fixed_to_int`, `float_to_fixed`, `fixed_to_float`, and
`fixed_convert` forms cross into, out of, or between fixed-point formats.

## Access and atomic fields

- `volatile` marks an observable volatile access.
- `align=N` gives the access alignment in bytes.
- `atomic` gives the memory ordering. An absent field denotes an ordinary
  access.
  - `relaxed` requires atomicity without acquire or release ordering.
  - `consume` orders dependent operations after a load.
  - `acquire` orders later operations after the acquiring access.
  - `release` orders earlier operations before the releasing access.
  - `acq_rel` combines acquire and release.
  - `seq_cst` also participates in the order of sequentially consistent
    operations.
  - `dynamic(value)` supplies an ordering at runtime.
- `sync_scope` gives the synchronization scope of a scoped atomic operation.
  - Absent means `system`, the default scope.
  - `device` scopes synchronization to the device.
  - `workgroup` scopes it to the workgroup.
  - `wavefront` scopes it to the wavefront.
  - `single` scopes it to a single thread or work item.
  - `cluster` scopes it to the target's cluster.
  - `dynamic(value)` supplies a scope at runtime.
- `scope` gives the fence kind.
  - `thread` is a thread fence.
  - `signal` is a compiler fence for signal handling.
- `order` gives the fence ordering using the same choices as `atomic`.

The IR retains requested scopes and orderings, including combinations that would
be undefined under C rules.

### Updates

- `result` selects the value returned by `update`.
  - `old` returns the value before the write.
  - `new` returns the updated value.

The computation can use `old<T>` for the original value without doing another
load. An atomic `update` keeps the read and write in one operation.

### Compare-exchange

`compare_exchange` takes a place, an expected operand, and a desired value.

- `success` gives the memory ordering when the exchange succeeds.
- `failure` gives the memory ordering when the exchange fails.
- `weak` controls whether a matching comparison can still fail.
  - `true` allows spurious failure.
  - `false` selects a strong exchange.
  - `dynamic(value)` selects the mode at runtime.
- `form` determines what happens to the expected operand and what is returned.
  - `write_back` returns success and writes the observed value through the
    expected pointer on failure.
  - `success` takes the expected value directly and returns success.
  - `old` takes the expected value directly and returns the previous value.
