extern void abort(void);
extern void exit(int);

struct baz {
  int a, b, c, d, e;
};

void bar(struct baz *x, int f, int g, int h, int i, int j) {
  if (x->a != 1 || x->b != 2 || x->c != 3 || x->d != 4 || x->e != 5 || f != 6 ||
      g != 7 || h != 8 || i != 9 || j != 10)
    abort();
}

void foo(struct baz x, char **y) { bar(&x, 6, 7, 8, 9, 10); }

int main() {
  struct baz x;

  x.a = 1;
  x.b = 2;
  x.c = 3;
  x.d = 4;
  x.e = 5;
  foo(x, (char **)0);
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
// DEFAULT-NEXT:         field3 d: i32;
// DEFAULT-NEXT:         field4 e: i32;
// DEFAULT-NEXT:     } [size=20, align=4, offsets=[0, 4, 8, 12, 16]];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_x:[0-9]+]] x: ptr<@type[[TYPE_baz]]>, %[[VALUE_f:[0-9]+]] f: i32, %[[VALUE_g:[0-9]+]] g: i32, %[[VALUE_h:[0-9]+]] h: i32, %[[VALUE_i:[0-9]+]] i: i32, %[[VALUE_j:[0-9]+]] j: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(field0(deref(read<ptr<@type[[TYPE_baz]]>>(%[[VALUE_x]])))), const<i32>(1)), ne<i32>(read<i32>(field1(deref(read<ptr<@type[[TYPE_baz]]>>(%[[VALUE_x]])))), const<i32>(2))), ne<i32>(read<i32>(field2(deref(read<ptr<@type[[TYPE_baz]]>>(%[[VALUE_x]])))), const<i32>(3))), ne<i32>(read<i32>(field3(deref(read<ptr<@type[[TYPE_baz]]>>(%[[VALUE_x]])))), const<i32>(4))), ne<i32>(read<i32>(field4(deref(read<ptr<@type[[TYPE_baz]]>>(%[[VALUE_x]])))), const<i32>(5))), ne<i32>(read<i32>(%[[VALUE_f]]), const<i32>(6))), ne<i32>(read<i32>(%[[VALUE_g]]), const<i32>(7))), ne<i32>(read<i32>(%[[VALUE_h]]), const<i32>(8))), ne<i32>(read<i32>(%[[VALUE_i]]), const<i32>(9))), ne<i32>(read<i32>(%[[VALUE_j]]), const<i32>(10)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x_2:[0-9]+]] x: @type[[TYPE_baz]], %[[VALUE_y:[0-9]+]] y: ptr<ptr<i8>>) -> void [linkage=external] [abi=sysv64(native_c, scalar) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_baz]]>, i32, i32, i32, i32, i32) -> void>(%[[VALUE_bar]], addr_of<ptr<@type[[TYPE_baz]]>>(%[[VALUE_x_2]]), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_x_3:[0-9]+]] x: @type[[TYPE_baz]] [storage=automatic];
// DEFAULT-NEXT:         write<i32>(field0(%[[VALUE_x_3]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(field1(%[[VALUE_x_3]]), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(field2(%[[VALUE_x_3]]), const<i32>(3));
// DEFAULT-NEXT:         write<i32>(field3(%[[VALUE_x_3]]), const<i32>(4));
// DEFAULT-NEXT:         write<i32>(field4(%[[VALUE_x_3]]), const<i32>(5));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_baz]], ptr<ptr<i8>>) -> void, abi=sysv64(native_c, scalar) -> void>(%[[VALUE_foo]], copy<@type[[TYPE_baz]], reason=arg>(read<@type[[TYPE_baz]]>(%[[VALUE_x_3]])), null<ptr<ptr<i8>>>);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
