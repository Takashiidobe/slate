/* PR optimization/8634 */
/* Contributed by Glen Nakamura <glen at imodulo dot com> */

extern void abort(void);

struct foo {
  char a, b, c, d, e, f, g, h, i, j;
};

int test1() {
  const char X[8] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
  char       buffer[8];
  __builtin_memcpy(buffer, X, 8);
  if (buffer[0] != 'A' || buffer[1] != 'B' || buffer[2] != 'C' ||
      buffer[3] != 'D' || buffer[4] != 'E' || buffer[5] != 'F' ||
      buffer[6] != 'G' || buffer[7] != 'H')
    abort();
  return 0;
}

int test2() {
  const char X[10] = {'A', 'B', 'C', 'D', 'E'};
  char       buffer[10];
  __builtin_memcpy(buffer, X, 10);
  if (buffer[0] != 'A' || buffer[1] != 'B' || buffer[2] != 'C' ||
      buffer[3] != 'D' || buffer[4] != 'E' || buffer[5] != '\0' ||
      buffer[6] != '\0' || buffer[7] != '\0' || buffer[8] != '\0' ||
      buffer[9] != '\0')
    abort();
  return 0;
}

int test3() {
  const struct foo X = {a : 'A', c : 'C', e : 'E', g : 'G', i : 'I'};
  char             buffer[10];
  __builtin_memcpy(buffer, &X, 10);
  if (buffer[0] != 'A' || buffer[1] != '\0' || buffer[2] != 'C' ||
      buffer[3] != '\0' || buffer[4] != 'E' || buffer[5] != '\0' ||
      buffer[6] != 'G' || buffer[7] != '\0' || buffer[8] != 'I' ||
      buffer[9] != '\0')
    abort();
  return 0;
}

int test4() {
  const struct foo X = {.b = 'B', .d = 'D', .f = 'F', .h = 'H', .j = 'J'};
  char             buffer[10];
  __builtin_memcpy(buffer, &X, 10);
  if (buffer[0] != '\0' || buffer[1] != 'B' || buffer[2] != '\0' ||
      buffer[3] != 'D' || buffer[4] != '\0' || buffer[5] != 'F' ||
      buffer[6] != '\0' || buffer[7] != 'H' || buffer[8] != '\0' ||
      buffer[9] != 'J')
    abort();
  return 0;
}

int main() {
  test1();
  test2();
  test3();
  test4();
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
// DEFAULT-NEXT:     type @type0 foo = struct {
// DEFAULT-NEXT:         field0 a: i8;
// DEFAULT-NEXT:         field1 b: i8;
// DEFAULT-NEXT:         field2 c: i8;
// DEFAULT-NEXT:         field3 d: i8;
// DEFAULT-NEXT:         field4 e: i8;
// DEFAULT-NEXT:         field5 f: i8;
// DEFAULT-NEXT:         field6 g: i8;
// DEFAULT-NEXT:         field7 h: i8;
// DEFAULT-NEXT:         field8 i: i8;
// DEFAULT-NEXT:         field9 j: i8;
// DEFAULT-NEXT:     } [size=10, align=1, offsets=[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @test1() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 X: array<i8, 8> [storage=automatic] [const] = aggregate<array<i8, 8>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(65)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(66)), index2 = truncate<i8, reason=assign, fits=always>(const<i32>(67)), index3 = truncate<i8, reason=assign, fits=always>(const<i32>(68)), index4 = truncate<i8, reason=assign, fits=always>(const<i32>(69)), index5 = truncate<i8, reason=assign, fits=always>(const<i32>(70)), index6 = truncate<i8, reason=assign, fits=always>(const<i32>(71)), index7 = truncate<i8, reason=assign, fits=always>(const<i32>(72)));
// DEFAULT-NEXT:         let %4 buffer: array<i8, 8> [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(__builtin_memcpy, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%4)), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<const i8>, length=Some(8)>(%3)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8))));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(8)>(%4), const<i32>(0))))), const<i32>(65)), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(8)>(%4), const<i32>(1))))), const<i32>(66))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(8)>(%4), const<i32>(2))))), const<i32>(67))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(8)>(%4), const<i32>(3))))), const<i32>(68))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(8)>(%4), const<i32>(4))))), const<i32>(69))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(8)>(%4), const<i32>(5))))), const<i32>(70))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(8)>(%4), const<i32>(6))))), const<i32>(71))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(8)>(%4), const<i32>(7))))), const<i32>(72)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @test2() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %6 X: array<i8, 10> [storage=automatic] [const] = aggregate<array<i8, 10>, zero_fill=true>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(65)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(66)), index2 = truncate<i8, reason=assign, fits=always>(const<i32>(67)), index3 = truncate<i8, reason=assign, fits=always>(const<i32>(68)), index4 = truncate<i8, reason=assign, fits=always>(const<i32>(69)));
// DEFAULT-NEXT:         let %7 buffer: array<i8, 10> [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(__builtin_memcpy, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%7)), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<const i8>, length=Some(10)>(%6)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(10))));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(10)>(%7), const<i32>(0))))), const<i32>(65)), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(10)>(%7), const<i32>(1))))), const<i32>(66))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(10)>(%7), const<i32>(2))))), const<i32>(67))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(10)>(%7), const<i32>(3))))), const<i32>(68))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(10)>(%7), const<i32>(4))))), const<i32>(69))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(10)>(%7), const<i32>(5))))), const<i32>(0))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(10)>(%7), const<i32>(6))))), const<i32>(0))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(10)>(%7), const<i32>(7))))), const<i32>(0))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(10)>(%7), const<i32>(8))))), const<i32>(0))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(10)>(%7), const<i32>(9))))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @test3() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %9 X: @type0 [storage=automatic] [const] = aggregate<@type0, zero_fill=true>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(65)), field2 = truncate<i8, reason=assign, fits=always>(const<i32>(67)), field4 = truncate<i8, reason=assign, fits=always>(const<i32>(69)), field6 = truncate<i8, reason=assign, fits=always>(const<i32>(71)), field8 = truncate<i8, reason=assign, fits=always>(const<i32>(73)));
// DEFAULT-NEXT:         let %10 buffer: array<i8, 10> [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(__builtin_memcpy, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%10)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<const @type0>>(%9)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(10))));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(10)>(%10), const<i32>(0))))), const<i32>(65)), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(10)>(%10), const<i32>(1))))), const<i32>(0))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(10)>(%10), const<i32>(2))))), const<i32>(67))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(10)>(%10), const<i32>(3))))), const<i32>(0))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(10)>(%10), const<i32>(4))))), const<i32>(69))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(10)>(%10), const<i32>(5))))), const<i32>(0))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(10)>(%10), const<i32>(6))))), const<i32>(71))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(10)>(%10), const<i32>(7))))), const<i32>(0))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(10)>(%10), const<i32>(8))))), const<i32>(73))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(10)>(%10), const<i32>(9))))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @test4() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %12 X: @type0 [storage=automatic] [const] = aggregate<@type0, zero_fill=true>(field1 = truncate<i8, reason=assign, fits=always>(const<i32>(66)), field3 = truncate<i8, reason=assign, fits=always>(const<i32>(68)), field5 = truncate<i8, reason=assign, fits=always>(const<i32>(70)), field7 = truncate<i8, reason=assign, fits=always>(const<i32>(72)), field9 = truncate<i8, reason=assign, fits=always>(const<i32>(74)));
// DEFAULT-NEXT:         let %13 buffer: array<i8, 10> [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(__builtin_memcpy, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<const @type0>>(%12)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(10))));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(10)>(%13), const<i32>(0))))), const<i32>(0)), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(10)>(%13), const<i32>(1))))), const<i32>(66))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(10)>(%13), const<i32>(2))))), const<i32>(0))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(10)>(%13), const<i32>(3))))), const<i32>(68))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(10)>(%13), const<i32>(4))))), const<i32>(0))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(10)>(%13), const<i32>(5))))), const<i32>(70))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(10)>(%13), const<i32>(6))))), const<i32>(0))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(10)>(%13), const<i32>(7))))), const<i32>(72))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(10)>(%13), const<i32>(8))))), const<i32>(0))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(10)>(%13), const<i32>(9))))), const<i32>(74)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%2);
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%5);
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%8);
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%11);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
