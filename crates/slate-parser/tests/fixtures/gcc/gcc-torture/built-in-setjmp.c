/* { dg-require-effective-target indirect_jumps } */

extern int   strcmp(const char *, const char *);
extern char *strcpy(char *, const char *);
extern void  abort(void);
extern void  exit(int);

void *buf[20];

void __attribute__((noinline)) sub2(void) { __builtin_longjmp(buf, 1); }

int main() {
  char *p = (char *)__builtin_alloca(20);

  strcpy(p, "test");

  if (__builtin_setjmp(buf)) {
    if (strcmp(p, "test") != 0)
      abort();

    exit(0);
  }

  {
    int *q = (int *)__builtin_alloca(p[2] * sizeof(int));
    int  i;

    for (i = 0; i < p[2]; i++)
      q[i] = 0;

    while (1)
      sub2();
  }
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
// DEFAULT-NEXT:     global %4 buf: array<ptr<void>, 20> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %15 .str15: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([116, 101, 115, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %16 .str16: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([116, 101, 115, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @strcmp(%10 <unnamed>: ptr<const i8>, %11 <unnamed>: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @strcpy(%12 <unnamed>: ptr<i8>, %13 <unnamed>: ptr<const i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %2 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @exit(%14 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %5 @sub2() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<ptr<void>>, i32) -> void>(__builtin_longjmp, array_decay<ptr<ptr<void>>, length=Some(20)>(%4), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %7 p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(__builtin_alloca, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(20)))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%1, read<ptr<i8>>(%7), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%15)));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<ptr<void>>) -> i32>(__builtin_setjmp, array_decay<ptr<ptr<void>>, length=Some(20)>(%4)), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%7)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%16))), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:                 call<void, signature=fn(i32) -> void>(%3, const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %8 q: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=explicit>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(__builtin_alloca, mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%7), const<i32>(2))))))), const<u64>(4))));
// DEFAULT-NEXT:             let %9 i: i32 [storage=automatic];
// DEFAULT-NEXT:             for %17
// DEFAULT-NEXT:                 init:
// DEFAULT-NEXT:                     write<i32>(%9, const<i32>(0));
// DEFAULT-NEXT:                 condition: lt<i32>(read<i32>(%9), widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%7), const<i32>(2))))))
// DEFAULT-NEXT:                 increment: {
// DEFAULT-NEXT:                     let %19: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:                     let %20: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%19), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%9, read<i32>(%20));
// DEFAULT-NEXT:                     yield void;
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:                 body:
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%8), read<i32>(%9))), const<i32>(0));
// DEFAULT-NEXT:             while %18 ne<i32>(const<i32>(1), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
