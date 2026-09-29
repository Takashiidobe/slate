typedef struct __attribute__((__may_alias__)) {
  short x;
} test;

test *p;

int g(int *a) { p = (test *)a; }

int f() {
  int a;
  g(&a);
  a      = 10;
  test s = {1};
  *p     = s;
  return a;
}

int main() {
  if (f() == 10)
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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 x: i16;
// DEFAULT-NEXT:     } [size=2, align=2, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_test:[0-9]+]] test = @type[[TYPE0]];
// DEFAULT-NEXT:     global %[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE0]]> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_g:[0-9]+]] @g(%[[VALUE_a:[0-9]+]] a: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<ptr<@type[[TYPE0]]>>(%[[VALUE_p]], pointer_cast<ptr<@type[[TYPE0]]>, reason=explicit>(read<ptr<i32>>(%[[VALUE_a]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_a_2:[0-9]+]] a: i32 [storage=automatic];
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<i32>) -> i32>(%[[VALUE_g]], addr_of<ptr<i32>>(%[[VALUE_a_2]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_a_2]], const<i32>(10));
// DEFAULT-NEXT:         let %[[VALUE_s:[0-9]+]] s: @type[[TYPE0]] [storage=automatic] = aggregate<@type[[TYPE0]], zero_fill=false>(field0 = truncate<i16, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<@type[[TYPE0]]>(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_p]])), copy<@type[[TYPE0]], reason=assign>(read<@type[[TYPE0]]>(%[[VALUE_s]])));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_a_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if eq<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_f]]), const<i32>(10))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
