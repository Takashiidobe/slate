
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
// DEFAULT-NEXT:     fn %2 @use_foo(%30 f: ptr<@type0>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @test_sign_ext(%4 f: ptr<@type0>, %5 i: ptr<i32, ptr32_sptr>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<ptr<i32>>(field1(deref(read<ptr<@type0>>(%4))), address_space_cast<ptr<i32>, reason=assign>(read<ptr<i32, ptr32_sptr>>(%5)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>) -> void>(%2, read<ptr<@type0>>(%4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @test_zero_ext(%7 f: ptr<@type0>, %8 i: ptr<i32, ptr32_uptr>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<ptr<i32>>(field1(deref(read<ptr<@type0>>(%7))), address_space_cast<ptr<i32>, reason=assign>(read<ptr<i32, ptr32_uptr>>(%8)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>) -> void>(%2, read<ptr<@type0>>(%7));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @test_trunc(%10 f: ptr<@type0>, %11 i: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<ptr<i32, ptr32_sptr>>(field0(deref(read<ptr<@type0>>(%10))), address_space_cast<ptr<i32, ptr32_sptr>, reason=assign>(read<ptr<i32>>(%11)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>) -> void>(%2, read<ptr<@type0>>(%10));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @test_noop(%13 f: ptr<@type0>, %14 i: ptr<i32, ptr32_sptr>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<ptr<i32, ptr32_sptr>>(field0(deref(read<ptr<@type0>>(%13))), read<ptr<i32, ptr32_sptr>>(%14));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>) -> void>(%2, read<ptr<@type0>>(%13));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @test_other(%16 f: ptr<@type0>, %17 i: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<ptr<i32, ptr32_sptr>>(field0(deref(read<ptr<@type0>>(%16))), address_space_cast<ptr<i32, ptr32_sptr>, reason=explicit>(read<ptr<i32>>(%17)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>) -> void>(%2, read<ptr<@type0>>(%16));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @test_compare1(%19 i: ptr<i32, ptr32_uptr>, %20 j: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<ptr<i32, ptr32_uptr>>(read<ptr<i32, ptr32_uptr>>(%19), address_space_cast<ptr<i32, ptr32_uptr>, reason=usual_arith>(read<ptr<i32>>(%20))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @test_compare2(%22 i: ptr<i32, ptr32_sptr>, %23 j: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<ptr<i32, ptr32_sptr>>(read<ptr<i32, ptr32_sptr>>(%22), address_space_cast<ptr<i32, ptr32_sptr>, reason=usual_arith>(read<ptr<i32>>(%23))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %24 @test_compare3(%25 i: ptr<i32, ptr32_uptr>, %26 j: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<ptr<i32>>(read<ptr<i32>>(%26), address_space_cast<ptr<i32>, reason=usual_arith>(read<ptr<i32, ptr32_uptr>>(%25))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %27 @test_compare4(%28 i: ptr<i32, ptr32_sptr>, %29 j: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<ptr<i32>>(read<ptr<i32>>(%29), address_space_cast<ptr<i32>, reason=usual_arith>(read<ptr<i32, ptr32_sptr>>(%28))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
