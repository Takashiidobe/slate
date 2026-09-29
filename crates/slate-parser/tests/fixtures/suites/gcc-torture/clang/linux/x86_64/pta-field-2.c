struct Foo {
  int *p;
  int *q;
};

void __attribute__((noinline)) bar(int **x) {
  struct Foo *f = (struct Foo *)(x - 1);
  *(f->p)       = 0;
}

int foo(void) {
  struct Foo f;
  int        i = 1, j = 2;
  f.p = &i;
  f.q = &j;
  bar(&f.q);
  return i;
}

extern void abort(void);
int         main() {
  if (foo() != 0)
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
// DEFAULT-NEXT:     type @type[[TYPE_Foo:[0-9]+]] Foo = struct {
// DEFAULT-NEXT:         field0 p: ptr<i32>;
// DEFAULT-NEXT:         field1 q: ptr<i32>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_x:[0-9]+]] x: ptr<ptr<i32>>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_f:[0-9]+]] f: ptr<@type[[TYPE_Foo]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE_Foo]]>, reason=explicit>(ptr_offset<ptr<ptr<i32>>, subtract=true, element=ptr<i32>, overflow=ub>(read<ptr<ptr<i32>>>(%[[VALUE_x]]), const<i32>(1)));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(field0(deref(read<ptr<@type[[TYPE_Foo]]>>(%[[VALUE_f]]))))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_f_2:[0-9]+]] f: @type[[TYPE_Foo]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:         write<ptr<i32>>(field0(%[[VALUE_f_2]]), addr_of<ptr<i32>>(%[[VALUE_i]]));
// DEFAULT-NEXT:         write<ptr<i32>>(field1(%[[VALUE_f_2]]), addr_of<ptr<i32>>(%[[VALUE_j]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<ptr<i32>>) -> void>(%[[VALUE_bar]], addr_of<ptr<ptr<i32>>>(field1(%[[VALUE_f_2]])));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_foo]]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
