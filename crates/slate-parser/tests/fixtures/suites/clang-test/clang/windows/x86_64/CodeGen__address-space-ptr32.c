
_Static_assert(sizeof(void *) == 8, "sizeof(void *) has unexpected value.  Expected 8.");

int foo(void) {
  int (*__ptr32 a)(int);
  return sizeof(a);
}

int bar(void) {
  int *__ptr32 p;
  return sizeof(p);
}


int baz(void) {
  typedef int *__ptr32 IP32_PTR;

  IP32_PTR p;
  return sizeof(p);
}

int fugu(void) {
  typedef int *int_star;

  int_star __ptr32 p;
  return sizeof(p);
}

typedef __SIZE_TYPE__ size_t;
size_t strlen(const char *);

size_t test_calling_strlen_with_32_bit_pointer ( char *__ptr32 s ) {
   return strlen ( s );
}


size_t test_calling_strlen_with_64_bit_pointer ( char *s ) {
  return strlen ( s );
}

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT gnu17

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-pc-windows-msvc" {
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
// DEFAULT-NEXT:     type @type[[TYPE_IP32_PTR:[0-9]+]] IP32_PTR = ptr<i32, ptr32_sptr>;
// DEFAULT-NEXT:     type @type[[TYPE_int_star:[0-9]+]] int_star = ptr<i32>;
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: ptr<fn(i32) -> i32, ptr32_sptr> [storage=automatic];
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=always>(const<u64>(4)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<i32, ptr32_sptr> [storage=automatic];
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=always>(const<u64>(4)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_baz:[0-9]+]] @baz() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_p_2:[0-9]+]] p: ptr<i32, ptr32_sptr> [storage=automatic];
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=always>(const<u64>(4)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fugu:[0-9]+]] @fugu() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_p_3:[0-9]+]] p: ptr<i32, ptr32_sptr> [storage=automatic];
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=always>(const<u64>(4)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_strlen:[0-9]+]] @strlen(%[[VALUE0:[0-9]+]] <unnamed>: ptr<const i8>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_calling_strlen_with_32_bit_pointer:[0-9]+]] @test_calling_strlen_with_32_bit_pointer(%[[VALUE_s:[0-9]+]] s: ptr<i8, ptr32_sptr>) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], address_space_cast<ptr<const i8>, reason=arg>(read<ptr<i8, ptr32_sptr>>(%[[VALUE_s]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_calling_strlen_with_64_bit_pointer:[0-9]+]] @test_calling_strlen_with_64_bit_pointer(%[[VALUE_s_2:[0-9]+]] s: ptr<i8>) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_s_2]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
