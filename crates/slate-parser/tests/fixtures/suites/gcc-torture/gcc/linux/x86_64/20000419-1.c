void abort(void);
void exit(int);

struct foo {
  int a, b, c;
};

void brother(int a, int b, int c) {
  if (a)
    abort();
}

void sister(struct foo f, int b, int c) { brother((f.b == b), b, c); }

int main() {
  struct foo f = {7, 8, 9};
  sister(f, 1, 2);
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
// DEFAULT-NEXT:     type @type[[TYPE_foo:[0-9]+]] foo = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:         field2 c: i32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_brother:[0-9]+]] @brother(%[[VALUE_a:[0-9]+]] a: i32, %[[VALUE_b:[0-9]+]] b: i32, %[[VALUE_c:[0-9]+]] c: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_a]]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_sister:[0-9]+]] @sister(%[[VALUE_f:[0-9]+]] f: @type[[TYPE_foo]], %[[VALUE_b_2:[0-9]+]] b: i32, %[[VALUE_c_2:[0-9]+]] c: i32) -> void [linkage=external] [abi=sysv64(native_c, scalar, scalar) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_brother]], from_bool<i32, reason=arg>(eq<i32>(read<i32>(field1(%[[VALUE_f]])), read<i32>(%[[VALUE_b_2]]))), read<i32>(%[[VALUE_b_2]]), read<i32>(%[[VALUE_c_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_f_2:[0-9]+]] f: @type[[TYPE_foo]] [storage=automatic] = aggregate<@type[[TYPE_foo]], zero_fill=false>(field0 = const<i32>(7), field1 = const<i32>(8), field2 = const<i32>(9));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_foo]], i32, i32) -> void, abi=sysv64(native_c, scalar, scalar) -> void>(%[[VALUE_sister]], copy<@type[[TYPE_foo]], reason=arg>(read<@type[[TYPE_foo]]>(%[[VALUE_f_2]])), const<i32>(1), const<i32>(2));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
