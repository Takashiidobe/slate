/* Test whether store motion recognizes pure functions as potentially reading
   any memory.  */

typedef __SIZE_TYPE__ size_t;
extern void           abort(void);
extern void          *memcpy(void *dest, const void *src, size_t n);
extern size_t         strlen(const char *s);
extern int strcmp(const char *s1, const char *s2) __attribute__((pure));

char buf[50];

static void foo(void) {
  if (memcpy(buf, "abc", 4) != buf)
    abort();
  if (strcmp(buf, "abc"))
    abort();
  memcpy(buf, "abcdefgh", strlen("abcdefgh") + 1);
}

int main(void) {
  foo();
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
// DEFAULT-NEXT:     global %5 buf: array<i8, 50> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %14 .str14: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([97, 98, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %15 .str15: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([97, 98, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %16 .str16: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([97, 98, 99, 100, 101, 102, 103, 104, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %17 .str17: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([97, 98, 99, 100, 101, 102, 103, 104, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @memcpy(%8 dest: ptr<void>, %9 src: ptr<const void>, %10 n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %3 @strlen(%11 s: ptr<const i8>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %4 @strcmp(%12 s1: ptr<const i8>, %13 s2: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %6 @foo() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%2, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(50)>(%5)), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%14)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4)))), pointer_cast<ptr<void>, reason=usual_arith>(array_decay<ptr<i8>, length=Some(50)>(%5)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%4, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(50)>(%5)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%15))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%2, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(50)>(%5)), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%16)), add<u64, overflow=wrap>(call<u64, signature=fn(ptr<const i8>) -> u64>(%3, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%17))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
