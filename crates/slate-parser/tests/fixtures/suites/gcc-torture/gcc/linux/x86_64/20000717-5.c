void abort(void);
void exit(int);

typedef struct trio {
  int a, b, c;
} trio;

int bar(int i, int j, int k, trio t) {
  if (t.a != 1 || t.b != 2 || t.c != 3 || i != 4 || j != 5 || k != 6)
    abort();
}

int foo(trio t, int i, int j, int k) { return bar(i, j, k, t); }

int main(void) {
  trio t = {1, 2, 3};

  foo(t, 4, 5, 6);
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
// DEFAULT-NEXT:     type @type[[TYPE_trio:[0-9]+]] trio = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:         field2 c: i32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_trio_2:[0-9]+]] trio = @type[[TYPE_trio]];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_i:[0-9]+]] i: i32, %[[VALUE_j:[0-9]+]] j: i32, %[[VALUE_k:[0-9]+]] k: i32, %[[VALUE_t:[0-9]+]] t: @type[[TYPE_trio]]) -> i32 [linkage=external] [abi=sysv64(scalar, scalar, scalar, native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(field0(%[[VALUE_t]])), const<i32>(1)), ne<i32>(read<i32>(field1(%[[VALUE_t]])), const<i32>(2))), ne<i32>(read<i32>(field2(%[[VALUE_t]])), const<i32>(3))), ne<i32>(read<i32>(%[[VALUE_i]]), const<i32>(4))), ne<i32>(read<i32>(%[[VALUE_j]]), const<i32>(5))), ne<i32>(read<i32>(%[[VALUE_k]]), const<i32>(6)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_t_2:[0-9]+]] t: @type[[TYPE_trio]], %[[VALUE_i_2:[0-9]+]] i: i32, %[[VALUE_j_2:[0-9]+]] j: i32, %[[VALUE_k_2:[0-9]+]] k: i32) -> i32 [linkage=external] [abi=sysv64(native_c, scalar, scalar, scalar) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(i32, i32, i32, @type[[TYPE_trio]]) -> i32, abi=sysv64(scalar, scalar, scalar, native_c) -> scalar>(%[[VALUE_bar]], read<i32>(%[[VALUE_i_2]]), read<i32>(%[[VALUE_j_2]]), read<i32>(%[[VALUE_k_2]]), copy<@type[[TYPE_trio]], reason=arg>(read<@type[[TYPE_trio]]>(%[[VALUE_t_2]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_t_3:[0-9]+]] t: @type[[TYPE_trio]] [storage=automatic] = aggregate<@type[[TYPE_trio]], zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(2), field2 = const<i32>(3));
// DEFAULT-NEXT:         call<i32, signature=fn(@type[[TYPE_trio]], i32, i32, i32) -> i32, abi=sysv64(native_c, scalar, scalar, scalar) -> scalar>(%[[VALUE_foo]], copy<@type[[TYPE_trio]], reason=arg>(read<@type[[TYPE_trio]]>(%[[VALUE_t_3]])), const<i32>(4), const<i32>(5), const<i32>(6));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
