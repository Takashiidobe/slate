extern void abort(void);
struct T {
  int t;
  int r[8];
};
struct S {
  int      a;
  int      b;
  int      c[6];
  struct T d;
};

__attribute__((noinline)) void foo(struct S *s) {
  *s = (struct S){s->b, s->a, {0, 0, 0, 0, 0, 0}, s->d};
}

int main(void) {
  struct S s = {6, 12, {1, 2, 3, 4, 5, 6}, {7, {8, 9, 10, 11, 12, 13, 14, 15}}};
  foo(&s);
  if (s.a != 12 || s.b != 6 || s.c[0] || s.c[1] || s.c[2] || s.c[3] || s.c[4] ||
      s.c[5])
    abort();
  if (s.d.t != 7 || s.d.r[0] != 8 || s.d.r[1] != 9 || s.d.r[2] != 10 ||
      s.d.r[3] != 11 || s.d.r[4] != 12 || s.d.r[5] != 13 || s.d.r[6] != 14 ||
      s.d.r[7] != 15)
    abort();
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
// DEFAULT-NEXT:     type @type0 T = struct {
// DEFAULT-NEXT:         field0 t: i32;
// DEFAULT-NEXT:         field1 r: array<i32, 8>;
// DEFAULT-NEXT:     } [size=36, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type1 S = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:         field2 c: array<i32, 6>;
// DEFAULT-NEXT:         field3 d: @type0;
// DEFAULT-NEXT:     } [size=68, align=4, offsets=[0, 4, 8, 32]];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @foo(%4 s: ptr<@type1>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<@type1>(deref(read<ptr<@type1>>(%4)), copy<@type1, reason=assign>(read<@type1>(compound_literal %7 [storage=automatic] = aggregate<@type1, zero_fill=false>(field0 = read<i32>(field1(deref(read<ptr<@type1>>(%4)))), field1 = read<i32>(field0(deref(read<ptr<@type1>>(%4)))), field2 = aggregate<array<i32, 6>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(0), index2 = const<i32>(0), index3 = const<i32>(0), index4 = const<i32>(0), index5 = const<i32>(0)), field3 = copy<@type0, reason=assign>(read<@type0>(field3(deref(read<ptr<@type1>>(%4)))))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %6 s: @type1 [storage=automatic] = aggregate<@type1, zero_fill=false>(field0 = const<i32>(6), field1 = const<i32>(12), field2 = aggregate<array<i32, 6>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2), index2 = const<i32>(3), index3 = const<i32>(4), index4 = const<i32>(5), index5 = const<i32>(6)), field3 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(7), field1 = aggregate<array<i32, 8>, zero_fill=false>(index0 = const<i32>(8), index1 = const<i32>(9), index2 = const<i32>(10), index3 = const<i32>(11), index4 = const<i32>(12), index5 = const<i32>(13), index6 = const<i32>(14), index7 = const<i32>(15))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type1>) -> void>(%3, addr_of<ptr<@type1>>(%6));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(field0(%6)), const<i32>(12)), ne<i32>(read<i32>(field1(%6)), const<i32>(6))), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(6)>(field2(%6)), const<i32>(0)))), const<i32>(0))), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(6)>(field2(%6)), const<i32>(1)))), const<i32>(0))), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(6)>(field2(%6)), const<i32>(2)))), const<i32>(0))), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(6)>(field2(%6)), const<i32>(3)))), const<i32>(0))), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(6)>(field2(%6)), const<i32>(4)))), const<i32>(0))), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(6)>(field2(%6)), const<i32>(5)))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(field0(field3(%6))), const<i32>(7)), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(8)>(field1(field3(%6))), const<i32>(0)))), const<i32>(8))), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(8)>(field1(field3(%6))), const<i32>(1)))), const<i32>(9))), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(8)>(field1(field3(%6))), const<i32>(2)))), const<i32>(10))), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(8)>(field1(field3(%6))), const<i32>(3)))), const<i32>(11))), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(8)>(field1(field3(%6))), const<i32>(4)))), const<i32>(12))), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(8)>(field1(field3(%6))), const<i32>(5)))), const<i32>(13))), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(8)>(field1(field3(%6))), const<i32>(6)))), const<i32>(14))), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(8)>(field1(field3(%6))), const<i32>(7)))), const<i32>(15)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
