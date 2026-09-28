
struct Foo {
  int * __ptr32 p32;
  int * __ptr64 p64;
};
void use_foo(struct Foo *f);
void test_sign_ext(struct Foo *f, int * __ptr32 __sptr i) {
  f->p64 = i;
  use_foo(f);
}
void test_zero_ext(struct Foo *f, int * __ptr32 __uptr i) {
  f->p64 = i;
  use_foo(f);
}
void test_trunc(struct Foo *f, int * __ptr64 i) {
  f->p32 = i;
  use_foo(f);
}
void test_noop(struct Foo *f, int * __ptr32 i) {
  f->p32 = i;
  use_foo(f);
}

void test_other(struct Foo *f, __attribute__((address_space(10))) int *i) {
  f->p32 = (int * __ptr32)i;
  use_foo(f);
}

int test_compare1(int *__ptr32 __uptr i, int *__ptr64 j) {
  return (i == j);
}

int test_compare2(int *__ptr32 __sptr i, int *__ptr64 j) {
  return (i == j);
}

int test_compare3(int *__ptr32 __uptr i, int *__ptr64 j) {
  return (j == i);
}

int test_compare4(int *__ptr32 __sptr i, int *__ptr64 j) {
  return (j == i);
}

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT gnu17

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "aarch64-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f64;
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
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     type @type0 Foo = struct {
// DEFAULT-NEXT:         field0 p32: ptr<i32, ptr32_sptr>;
// DEFAULT-NEXT:         field1 p64: ptr<i32>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     fn %1 @use_foo(%29 f: ptr<@type0>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @test_sign_ext(%3 f: ptr<@type0>, %4 i: ptr<i32, ptr32_sptr>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<ptr<i32>>(field1(deref(read<ptr<@type0>>(%3))), address_space_cast<ptr<i32>, reason=assign>(read<ptr<i32, ptr32_sptr>>(%4)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>) -> void>(%1, read<ptr<@type0>>(%3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @test_zero_ext(%6 f: ptr<@type0>, %7 i: ptr<i32, ptr32_uptr>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<ptr<i32>>(field1(deref(read<ptr<@type0>>(%6))), address_space_cast<ptr<i32>, reason=assign>(read<ptr<i32, ptr32_uptr>>(%7)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>) -> void>(%1, read<ptr<@type0>>(%6));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @test_trunc(%9 f: ptr<@type0>, %10 i: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<ptr<i32, ptr32_sptr>>(field0(deref(read<ptr<@type0>>(%9))), address_space_cast<ptr<i32, ptr32_sptr>, reason=assign>(read<ptr<i32>>(%10)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>) -> void>(%1, read<ptr<@type0>>(%9));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @test_noop(%12 f: ptr<@type0>, %13 i: ptr<i32, ptr32_sptr>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<ptr<i32, ptr32_sptr>>(field0(deref(read<ptr<@type0>>(%12))), read<ptr<i32, ptr32_sptr>>(%13));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>) -> void>(%1, read<ptr<@type0>>(%12));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @test_other(%15 f: ptr<@type0>, %16 i: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<ptr<i32, ptr32_sptr>>(field0(deref(read<ptr<@type0>>(%15))), address_space_cast<ptr<i32, ptr32_sptr>, reason=explicit>(read<ptr<i32>>(%16)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>) -> void>(%1, read<ptr<@type0>>(%15));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @test_compare1(%18 i: ptr<i32, ptr32_uptr>, %19 j: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<ptr<i32, ptr32_uptr>>(read<ptr<i32, ptr32_uptr>>(%18), address_space_cast<ptr<i32, ptr32_uptr>, reason=usual_arith>(read<ptr<i32>>(%19))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @test_compare2(%21 i: ptr<i32, ptr32_sptr>, %22 j: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<ptr<i32, ptr32_sptr>>(read<ptr<i32, ptr32_sptr>>(%21), address_space_cast<ptr<i32, ptr32_sptr>, reason=usual_arith>(read<ptr<i32>>(%22))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @test_compare3(%24 i: ptr<i32, ptr32_uptr>, %25 j: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<ptr<i32>>(read<ptr<i32>>(%25), address_space_cast<ptr<i32>, reason=usual_arith>(read<ptr<i32, ptr32_uptr>>(%24))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %26 @test_compare4(%27 i: ptr<i32, ptr32_sptr>, %28 j: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<ptr<i32>>(read<ptr<i32>>(%28), address_space_cast<ptr<i32>, reason=usual_arith>(read<ptr<i32, ptr32_sptr>>(%27))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
