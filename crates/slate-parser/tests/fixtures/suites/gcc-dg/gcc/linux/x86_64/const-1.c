/* { dg-do compile { target { nonpic || pie_enabled } } } */
/* { dg-options "-O2 -Wsuggest-attribute=const -fno-finite-loops" } */

extern int extern_const(int a) __attribute__ ((const));

/* Trivial.  */
int
foo1(int a)  /* { dg-bogus "normally" "detect const candidate" } */
{ /* { dg-warning "const" "detect const candidate" { target *-*-* } "8" } */ 
  return extern_const (a);
}

/* Loops known to be normally and extern const calls should be safe.  */

int __attribute__ ((noinline))
foo2(int n)  /* { dg-bogus "normally" "detect const candidate" } */
{ /* { dg-warning "const" "detect const candidate" { target *-*-* } "16" } */
  int ret = 0;
  int i;
  for (i=0; i<n; i++)
    ret+=extern_const (i);
  return ret;
}

/* No warning here; we can work it by ourselves.  */
static int __attribute__ ((noinline))
foo2b(int n)
{
  int ret = 0;
  int i;
  for (i=0; i<n; i++)
    ret+=extern_const (i);
  return ret;
}

/* Unbounded loops are not safe.  */
static int __attribute__ ((noinline))
foo3(unsigned int n)  /* { dg-warning "const\[^\n\]* normally" "detect const candidate" } */
{
  int ret = 0;
  unsigned int i;
  for (i=0; extern_const (i+n); n++)
    ret+=extern_const (i);
  return ret;
}

int
foo4(int n) /* { dg-warning "const\[^\n\]* normally" "detect const candidate" } */
{
  return foo3(n) + foo2b(n);
} 

int
foo5(int n)  /* { dg-bogus "normally" "detect const candidate" } */
{ /* { dg-warning "const" "detect const candidate" { target *-*-* } "54" } */
  return foo2(n);
} 

// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     fn %1 @extern_const(%20 a: i32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %2 @foo1(%3 a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(i32) -> i32>(%1, read<i32>(%3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @foo2(%5 n: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %6 ret: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %7 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %21
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%7, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%7), read<i32>(%5))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %24: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                 let %25: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%24), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%7, read<i32>(%25));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %26: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:                 let %27: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%26), call<i32, signature=fn(i32) -> i32>(%1, read<i32>(%7)));
// DEFAULT-NEXT:                 write<i32>(%6, read<i32>(%27));
// DEFAULT-NEXT:         return read<i32>(%6);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @foo2b(%9 n: i32) -> i32 [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %10 ret: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %11 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %22
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%11, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%11), read<i32>(%9))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %28: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:                 let %29: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%28), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%11, read<i32>(%29));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %30: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                 let %31: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%30), call<i32, signature=fn(i32) -> i32>(%1, read<i32>(%11)));
// DEFAULT-NEXT:                 write<i32>(%10, read<i32>(%31));
// DEFAULT-NEXT:         return read<i32>(%10);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @foo3(%13 n: u32) -> i32 [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %14 ret: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %15 i: u32 [storage=automatic];
// DEFAULT-NEXT:         for %23
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u32>(%15, reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             condition: ne<i32>(call<i32, signature=fn(i32) -> i32>(%1, reinterpret<i32, reason=arg, fits=unknown>(add<u32, overflow=wrap>(read<u32>(%15), read<u32>(%13)))), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %32: u32 [synthetic] = read<u32>(%13);
// DEFAULT-NEXT:                 let %33: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%32), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<u32>(%13, read<u32>(%33));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %34: i32 [synthetic] = read<i32>(%14);
// DEFAULT-NEXT:                 let %35: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%34), call<i32, signature=fn(i32) -> i32>(%1, reinterpret<i32, reason=arg, fits=unknown>(read<u32>(%15))));
// DEFAULT-NEXT:                 write<i32>(%14, read<i32>(%35));
// DEFAULT-NEXT:         return read<i32>(%14);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @foo4(%17 n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(call<i32, signature=fn(u32) -> i32>(%12, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%17))), call<i32, signature=fn(i32) -> i32>(%8, read<i32>(%17)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @foo5(%19 n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(i32) -> i32>(%4, read<i32>(%19));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
