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
// DEFAULT-NEXT:     fn %[[VALUE_extern_const:[0-9]+]] @extern_const(%[[VALUE_a:[0-9]+]] a: i32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_foo1:[0-9]+]] @foo1(%[[VALUE_a_2:[0-9]+]] a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(i32) -> i32>(%[[VALUE_extern_const]], read<i32>(%[[VALUE_a_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo2:[0-9]+]] @foo2(%[[VALUE_n:[0-9]+]] n: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_ret:[0-9]+]] ret: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_n]]))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_ret]]);
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE3]]), call<i32, signature=fn(i32) -> i32>(%[[VALUE_extern_const]], read<i32>(%[[VALUE_i]])));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_ret]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_ret]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo2b:[0-9]+]] @foo2b(%[[VALUE_n_2:[0-9]+]] n: i32) -> i32 [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_ret_2:[0-9]+]] ret: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_i_2:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE5:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), read<i32>(%[[VALUE_n_2]]))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE6]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE7]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE8:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_ret_2]]);
// DEFAULT-NEXT:                 let %[[VALUE9:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE8]]), call<i32, signature=fn(i32) -> i32>(%[[VALUE_extern_const]], read<i32>(%[[VALUE_i_2]])));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_ret_2]], read<i32>(%[[VALUE9]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_ret_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo3:[0-9]+]] @foo3(%[[VALUE_n_3:[0-9]+]] n: u32) -> i32 [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_ret_3:[0-9]+]] ret: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_i_3:[0-9]+]] i: u32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE10:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u32>(%[[VALUE_i_3]], reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             condition: ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_extern_const]], reinterpret<i32, reason=arg, fits=unknown>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_i_3]]), read<u32>(%[[VALUE_n_3]])))), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE11:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_n_3]]);
// DEFAULT-NEXT:                 let %[[VALUE12:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE11]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<u32>(%[[VALUE_n_3]], read<u32>(%[[VALUE12]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE13:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_ret_3]]);
// DEFAULT-NEXT:                 let %[[VALUE14:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE13]]), call<i32, signature=fn(i32) -> i32>(%[[VALUE_extern_const]], reinterpret<i32, reason=arg, fits=unknown>(read<u32>(%[[VALUE_i_3]]))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_ret_3]], read<i32>(%[[VALUE14]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_ret_3]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo4:[0-9]+]] @foo4(%[[VALUE_n_4:[0-9]+]] n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(call<i32, signature=fn(u32) -> i32>(%[[VALUE_foo3]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_n_4]]))), call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo2b]], read<i32>(%[[VALUE_n_4]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo5:[0-9]+]] @foo5(%[[VALUE_n_5:[0-9]+]] n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo2]], read<i32>(%[[VALUE_n_5]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
