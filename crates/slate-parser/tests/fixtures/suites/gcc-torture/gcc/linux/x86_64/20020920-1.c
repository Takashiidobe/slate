extern void abort(void);
extern void exit(int);

struct B {
  int x;
  int y;
};

struct A {
  int      z;
  struct B b;
};

struct A f() {
  struct B b = {0, 1};
  struct A a = {2, b};
  return a;
}

int main(void) {
  struct A a = f();
  if (a.z != 2 || a.b.x != 0 || a.b.y != 1)
    abort();
  exit(0);
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
// DEFAULT-NEXT:     type @type[[TYPE_B:[0-9]+]] B = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 y: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_A:[0-9]+]] A = struct {
// DEFAULT-NEXT:         field0 z: i32;
// DEFAULT-NEXT:         field1 b: @type[[TYPE_B]];
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f() -> @type[[TYPE_A]] [linkage=external] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: @type[[TYPE_B]] [storage=automatic] = aggregate<@type[[TYPE_B]], zero_fill=false>(field0 = const<i32>(0), field1 = const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: @type[[TYPE_A]] [storage=automatic] = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = const<i32>(2), field1 = copy<@type[[TYPE_B]], reason=assign>(read<@type[[TYPE_B]]>(%[[VALUE_b]])));
// DEFAULT-NEXT:         return copy<@type[[TYPE_A]], reason=return>(read<@type[[TYPE_A]]>(%[[VALUE_a]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a_2:[0-9]+]] a: @type[[TYPE_A]] [storage=automatic] = copy<@type[[TYPE_A]], reason=assign>(call<@type[[TYPE_A]], signature=fn() -> @type[[TYPE_A]], abi=sysv64() -> native_c>(%[[VALUE_f]]));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(field0(%[[VALUE_a_2]])), const<i32>(2)), ne<i32>(read<i32>(field0(field1(%[[VALUE_a_2]]))), const<i32>(0))), ne<i32>(read<i32>(field1(field1(%[[VALUE_a_2]]))), const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
