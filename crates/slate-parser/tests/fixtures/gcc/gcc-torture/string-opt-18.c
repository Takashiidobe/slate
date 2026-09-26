/* Copyright (C) 2003  Free Software Foundation.

   Test equal pointer optimizations don't break anything.

   Written by Roger Sayle, July 14, 2003.  */

extern void           abort();
typedef __SIZE_TYPE__ size_t;

extern void *memcpy(void *, const void *, size_t);
extern void *mempcpy(void *, const void *, size_t);
extern void *memmove(void *, const void *, size_t);
extern char *strcpy(char *, const char *);
extern int   memcmp(const void *, const void *, size_t);
extern int   strcmp(const char *, const char *);
extern int   strncmp(const char *, const char *, size_t);

void test1(void *ptr) {
  if (memcpy(ptr, ptr, 8) != ptr)
    abort();
}

void test2(char *ptr) {
  if (mempcpy(ptr, ptr, 8) != ptr + 8)
    abort();
}

void test3(void *ptr) {
  if (memmove(ptr, ptr, 8) != ptr)
    abort();
}

void test4(char *ptr) {
  if (strcpy(ptr, ptr) != ptr)
    abort();
}

void test5(void *ptr) {
  if (memcmp(ptr, ptr, 8) != 0)
    abort();
}

void test6(const char *ptr) {
  if (strcmp(ptr, ptr) != 0)
    abort();
}

void test7(const char *ptr) {
  if (strncmp(ptr, ptr, 8) != 0)
    abort();
}

int main() {
  char buf[10];

  test1(buf);
  test2(buf);
  test3(buf);
  test4(buf);
  test5(buf);
  test6(buf);
  test7(buf);

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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @memcpy(%25 <unnamed>: ptr<void>, %26 <unnamed>: ptr<const void>, %27 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %3 @mempcpy(%28 <unnamed>: ptr<void>, %29 <unnamed>: ptr<const void>, %30 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %4 @memmove(%31 <unnamed>: ptr<void>, %32 <unnamed>: ptr<const void>, %33 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %5 @strcpy(%34 <unnamed>: ptr<i8>, %35 <unnamed>: ptr<const i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %6 @memcmp(%36 <unnamed>: ptr<const void>, %37 <unnamed>: ptr<const void>, %38 <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %7 @strcmp(%39 <unnamed>: ptr<const i8>, %40 <unnamed>: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %8 @strncmp(%41 <unnamed>: ptr<const i8>, %42 <unnamed>: ptr<const i8>, %43 <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %9 @test1(%10 ptr: ptr<void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%2, read<ptr<void>>(%10), pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%10)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8)))), read<ptr<void>>(%10))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @test2(%12 ptr: ptr<i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%3, pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%12)), pointer_cast<ptr<const void>, reason=arg>(read<ptr<i8>>(%12)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8)))), pointer_cast<ptr<void>, reason=usual_arith>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%12), const<i32>(8))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @test3(%14 ptr: ptr<void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%4, read<ptr<void>>(%14), pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%14)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8)))), read<ptr<void>>(%14))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @test4(%16 ptr: ptr<i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%5, read<ptr<i8>>(%16), pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%16))), read<ptr<i8>>(%16))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @test5(%18 ptr: ptr<void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%6, pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%18)), pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%18)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @test6(%20 ptr: ptr<const i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%7, read<ptr<const i8>>(%20), read<ptr<const i8>>(%20)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @test7(%22 ptr: ptr<const i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>, u64) -> i32>(%8, read<ptr<const i8>>(%22), read<ptr<const i8>>(%22), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %24 buf: array<i8, 10> [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%9, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%24)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i8>) -> void>(%11, array_decay<ptr<i8>, length=Some(10)>(%24));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%13, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%24)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i8>) -> void>(%15, array_decay<ptr<i8>, length=Some(10)>(%24));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%17, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%24)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>) -> void>(%19, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%24)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>) -> void>(%21, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%24)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
