extern void abort(void);
extern void exit(int);

struct baz {
  int a, b, c;
};

struct baz *c;

void bar(int b) {
  if (c->a != 1 || c->b != 2 || c->c != 3 || b != 4)
    abort();
}

void foo(struct baz a, int b) {
  c = &a;
  bar(b);
}

int main() {
  struct baz a;
  a.a = 1;
  a.b = 2;
  a.c = 3;
  foo(a, 4);
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
// DEFAULT-NEXT:     type @type[[TYPE_baz:[0-9]+]] baz = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:         field2 c: i32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: ptr<@type[[TYPE_baz]]> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_b:[0-9]+]] b: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(field0(deref(read<ptr<@type[[TYPE_baz]]>>(%[[VALUE_c]])))), const<i32>(1)), ne<i32>(read<i32>(field1(deref(read<ptr<@type[[TYPE_baz]]>>(%[[VALUE_c]])))), const<i32>(2))), ne<i32>(read<i32>(field2(deref(read<ptr<@type[[TYPE_baz]]>>(%[[VALUE_c]])))), const<i32>(3))), ne<i32>(read<i32>(%[[VALUE_b]]), const<i32>(4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_a:[0-9]+]] a: @type[[TYPE_baz]], %[[VALUE_b_2:[0-9]+]] b: i32) -> void [linkage=external] [abi=sysv64(native_c, scalar) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_baz]]>>(%[[VALUE_c]], addr_of<ptr<@type[[TYPE_baz]]>>(%[[VALUE_a]]));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_bar]], read<i32>(%[[VALUE_b_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a_2:[0-9]+]] a: @type[[TYPE_baz]] [storage=automatic];
// DEFAULT-NEXT:         write<i32>(field0(%[[VALUE_a_2]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(field1(%[[VALUE_a_2]]), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(field2(%[[VALUE_a_2]]), const<i32>(3));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_baz]], i32) -> void, abi=sysv64(native_c, scalar) -> void>(%[[VALUE_foo]], copy<@type[[TYPE_baz]], reason=arg>(read<@type[[TYPE_baz]]>(%[[VALUE_a_2]])), const<i32>(4));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
