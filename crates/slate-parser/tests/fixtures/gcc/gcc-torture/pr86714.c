/* PR tree-optimization/86714 - tree-ssa-forwprop.c confused by too
   long initializer

   The excessively long initializer for a[0] is undefined but this
   test verifies that the excess elements are not considered a part
   of the value of the array as a matter of QoI.  */

const char a[2][3] = {"1234", "xyz"};
char       b[6];

void *pb = b;

int main() {
  __builtin_memcpy(b, a, 4);
  __builtin_memset(b + 4, 'a', 2);

  if (b[0] != '1' || b[1] != '2' || b[2] != '3' || b[3] != 'x' || b[4] != 'a' ||
      b[5] != 'a')
    __builtin_abort();

  if (__builtin_memcmp(pb, "123xaa", 6))
    __builtin_abort();

  return 0;
}


// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     global %0 a: array<array<i8, 3>, 2> [storage=static] [const] = aggregate<array<array<i8, 3>, 2>, zero_fill=false>(index0 = code_units<array<i8, 3>>([49, 50, 51]), index1 = code_units<array<i8, 3>>([120, 121, 122])) [linkage=external];
// DEFAULT-NEXT:     global %1 b: array<i8, 6> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 pb: ptr<void> [storage=static] = pointer_cast<ptr<void>, reason=assign>(array_decay<ptr<i8>, length=Some(6)>(%1)) [linkage=external];
// DEFAULT-NEXT:     global %4 .str4: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([49, 50, 51, 120, 97, 97, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(__builtin_memcpy, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%1)), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<const array<i8, 3>>, length=Some(2)>(%0)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(__builtin_memset, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(6)>(%1), const<i32>(4))), const<i32>(97), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(6)>(%1), const<i32>(0))))), const<i32>(49)), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(6)>(%1), const<i32>(1))))), const<i32>(50))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(6)>(%1), const<i32>(2))))), const<i32>(51))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(6)>(%1), const<i32>(3))))), const<i32>(120))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(6)>(%1), const<i32>(4))))), const<i32>(97))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(6)>(%1), const<i32>(5))))), const<i32>(97)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(__builtin_memcmp, pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%2)), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%4)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
