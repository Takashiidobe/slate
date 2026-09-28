/* Copyright (C) 2003  Free Software Foundation.

   Test strcpy optimizations don't evaluate side-effects twice.

   Written by Jakub Jelinek, June 23, 2003.  */

typedef __SIZE_TYPE__ size_t;
extern char          *strcpy(char *, const char *);
extern int            memcmp(const void *, const void *, size_t);
extern void           abort(void);
extern void           exit(int);

size_t test1(char *s, size_t i) {
  strcpy(s, "foobarbaz" + i++);
  return i;
}

size_t check2(void) {
  static size_t r = 5;
  if (r != 5)
    abort();
  return ++r;
}

void test2(char *s) { strcpy(s, "foobarbaz" + check2()); }

int main(void) {
  char buf[10];
  if (test1(buf, 7) != 8 || memcmp(buf, "az", 3))
    abort();
  test2(buf);
  if (memcmp(buf, "baz", 4))
    abort();
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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     global %20 .str20: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([102, 111, 111, 98, 97, 114, 98, 97, 122, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %9 r: u64 [storage=static] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(5))) [linkage=internal];
// DEFAULT-NEXT:     global %21 .str21: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([102, 111, 111, 98, 97, 114, 98, 97, 122, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %22 .str22: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([97, 122, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %23 .str23: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([98, 97, 122, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @strcpy(%14 <unnamed>: ptr<i8>, %15 <unnamed>: ptr<const i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %2 @memcmp(%16 <unnamed>: ptr<const void>, %17 <unnamed>: ptr<const void>, %18 <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %3 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %4 @exit(%19 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %5 @test1(%6 s: ptr<i8>, %7 i: u64) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %24: u64 [synthetic] = read<u64>(%7);
// DEFAULT-NEXT:         let %25: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%24), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         write<u64>(%7, read<u64>(%25));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%1, read<ptr<i8>>(%6), pointer_cast<ptr<const i8>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(10)>(%20), read<u64>(%24))));
// DEFAULT-NEXT:         return read<u64>(%7);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @check2() -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<u64>(read<u64>(%9), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(5))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         let %26: u64 [synthetic] = read<u64>(%9);
// DEFAULT-NEXT:         let %27: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%26), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         write<u64>(%9, read<u64>(%27));
// DEFAULT-NEXT:         return read<u64>(%27);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @test2(%11 s: ptr<i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%1, read<ptr<i8>>(%11), pointer_cast<ptr<const i8>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(10)>(%21), call<u64, signature=fn() -> u64>(%8))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %13 buf: array<i8, 10> [storage=automatic];
// DEFAULT-NEXT:         let %28: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(ptr<i8>, u64) -> u64>(%5, array_decay<ptr<i8>, length=Some(10)>(%13), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(7)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))
// DEFAULT-NEXT:             write<bool>(%28, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%28, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%2, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%13)), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%22)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3)))), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%28)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i8>) -> void>(%10, array_decay<ptr<i8>, length=Some(10)>(%13));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%2, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%13)), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%23)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%4, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
