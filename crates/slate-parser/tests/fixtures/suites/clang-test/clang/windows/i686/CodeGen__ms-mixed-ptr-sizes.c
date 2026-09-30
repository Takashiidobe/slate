
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
// DEFAULT-NEXT:     target "i686-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=4, align=4];
// DEFAULT-NEXT:         stack_alignment = 4;
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
// DEFAULT-NEXT:     type @type[[TYPE_Foo:[0-9]+]] Foo = struct {
// DEFAULT-NEXT:         field0 p32: ptr<i32>;
// DEFAULT-NEXT:         field1 p64: ptr<i32, ptr64>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     fn %[[VALUE_use_foo:[0-9]+]] @use_foo(%[[VALUE_f:[0-9]+]] f: ptr<@type[[TYPE_Foo]]>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_sign_ext:[0-9]+]] @test_sign_ext(%[[VALUE_f_2:[0-9]+]] f: ptr<@type[[TYPE_Foo]]>, %[[VALUE_i:[0-9]+]] i: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<ptr<i32, ptr64>>(field1(deref(read<ptr<@type[[TYPE_Foo]]>>(%[[VALUE_f_2]]))), address_space_cast<ptr<i32, ptr64>, reason=assign>(read<ptr<i32>>(%[[VALUE_i]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_Foo]]>) -> void>(%[[VALUE_use_foo]], read<ptr<@type[[TYPE_Foo]]>>(%[[VALUE_f_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_zero_ext:[0-9]+]] @test_zero_ext(%[[VALUE_f_3:[0-9]+]] f: ptr<@type[[TYPE_Foo]]>, %[[VALUE_i_2:[0-9]+]] i: ptr<i32, ptr32_uptr>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<ptr<i32, ptr64>>(field1(deref(read<ptr<@type[[TYPE_Foo]]>>(%[[VALUE_f_3]]))), address_space_cast<ptr<i32, ptr64>, reason=assign>(read<ptr<i32, ptr32_uptr>>(%[[VALUE_i_2]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_Foo]]>) -> void>(%[[VALUE_use_foo]], read<ptr<@type[[TYPE_Foo]]>>(%[[VALUE_f_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_trunc:[0-9]+]] @test_trunc(%[[VALUE_f_4:[0-9]+]] f: ptr<@type[[TYPE_Foo]]>, %[[VALUE_i_3:[0-9]+]] i: ptr<i32, ptr64>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<ptr<i32>>(field0(deref(read<ptr<@type[[TYPE_Foo]]>>(%[[VALUE_f_4]]))), address_space_cast<ptr<i32>, reason=assign>(read<ptr<i32, ptr64>>(%[[VALUE_i_3]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_Foo]]>) -> void>(%[[VALUE_use_foo]], read<ptr<@type[[TYPE_Foo]]>>(%[[VALUE_f_4]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_noop:[0-9]+]] @test_noop(%[[VALUE_f_5:[0-9]+]] f: ptr<@type[[TYPE_Foo]]>, %[[VALUE_i_4:[0-9]+]] i: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<ptr<i32>>(field0(deref(read<ptr<@type[[TYPE_Foo]]>>(%[[VALUE_f_5]]))), read<ptr<i32>>(%[[VALUE_i_4]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_Foo]]>) -> void>(%[[VALUE_use_foo]], read<ptr<@type[[TYPE_Foo]]>>(%[[VALUE_f_5]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_other:[0-9]+]] @test_other(%[[VALUE_f_6:[0-9]+]] f: ptr<@type[[TYPE_Foo]]>, %[[VALUE_i_5:[0-9]+]] i: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<ptr<i32>>(field0(deref(read<ptr<@type[[TYPE_Foo]]>>(%[[VALUE_f_6]]))), read<ptr<i32>>(%[[VALUE_i_5]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_Foo]]>) -> void>(%[[VALUE_use_foo]], read<ptr<@type[[TYPE_Foo]]>>(%[[VALUE_f_6]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_compare1:[0-9]+]] @test_compare1(%[[VALUE_i_6:[0-9]+]] i: ptr<i32, ptr32_uptr>, %[[VALUE_j:[0-9]+]] j: ptr<i32, ptr64>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<ptr<i32, ptr32_uptr>>(read<ptr<i32, ptr32_uptr>>(%[[VALUE_i_6]]), address_space_cast<ptr<i32, ptr32_uptr>, reason=usual_arith>(read<ptr<i32, ptr64>>(%[[VALUE_j]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_compare2:[0-9]+]] @test_compare2(%[[VALUE_i_7:[0-9]+]] i: ptr<i32>, %[[VALUE_j_2:[0-9]+]] j: ptr<i32, ptr64>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<ptr<i32>>(read<ptr<i32>>(%[[VALUE_i_7]]), address_space_cast<ptr<i32>, reason=usual_arith>(read<ptr<i32, ptr64>>(%[[VALUE_j_2]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_compare3:[0-9]+]] @test_compare3(%[[VALUE_i_8:[0-9]+]] i: ptr<i32, ptr32_uptr>, %[[VALUE_j_3:[0-9]+]] j: ptr<i32, ptr64>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<ptr<i32, ptr64>>(read<ptr<i32, ptr64>>(%[[VALUE_j_3]]), address_space_cast<ptr<i32, ptr64>, reason=usual_arith>(read<ptr<i32, ptr32_uptr>>(%[[VALUE_i_8]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_compare4:[0-9]+]] @test_compare4(%[[VALUE_i_9:[0-9]+]] i: ptr<i32>, %[[VALUE_j_4:[0-9]+]] j: ptr<i32, ptr64>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<ptr<i32, ptr64>>(read<ptr<i32, ptr64>>(%[[VALUE_j_4]]), address_space_cast<ptr<i32, ptr64>, reason=usual_arith>(read<ptr<i32>>(%[[VALUE_i_9]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
