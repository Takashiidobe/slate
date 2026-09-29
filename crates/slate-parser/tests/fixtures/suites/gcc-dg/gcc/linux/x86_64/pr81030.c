/* { dg-do compile } */
/* { dg-options "-O1" } */

void __assert_fail (const char *, const char *, unsigned int, const char *);

int a, b, c, d, e, f, h;
unsigned char g;

int main ()
{
  int i, *j = &b;
  if (a)
    {
      if (h)
	{
	  int **k = &j;
	  d = 0;
	  *k = &e;
	}
      else
	for (b = 0; b > -28; b = g)
	  ;
      c || !j ? : __assert_fail ("c || !j", "small.c", 20, "main");
      if (f)
	for (i = 0; i < 1; i++)
	  ;
    }
  return 0;
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
// DEFAULT-NEXT:     global %1 a: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 c: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 d: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 e: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 f: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %7 h: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %8 g: u8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %19 .str19: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([99, 32, 124, 124, 32, 33, 106, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %20 .str20: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([115, 109, 97, 108, 108, 46, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %21 .str21: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([109, 97, 105, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @__assert_fail(%13 <unnamed>: ptr<const i8>, %14 <unnamed>: ptr<const i8>, %15 <unnamed>: u32, %16 <unnamed>: ptr<const i8>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %9 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %10 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %11 j: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(%2);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%7), const<i32>(0))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %12 k: ptr<ptr<i32>> [storage=automatic] = addr_of<ptr<ptr<i32>>>(%11);
// DEFAULT-NEXT:                         write<i32>(%4, const<i32>(0));
// DEFAULT-NEXT:                         write<ptr<i32>>(deref(read<ptr<ptr<i32>>>(%12)), addr_of<ptr<i32>>(%5));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     for %17
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             write<i32>(%2, const<i32>(0));
// DEFAULT-NEXT:                         condition: gt<i32>(read<i32>(%2), neg<i32, overflow=ub>(const<i32>(28)))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             write<i32>(%2, reinterpret<i32, reason=assign, fits=unknown>(widen<u32, reason=assign>(read<u8>(%8))));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             ;
// DEFAULT-NEXT:                 let %18: bool [synthetic] = logical_or<bool>(ne<i32>(read<i32>(%3), const<i32>(0)), not<bool>(ne<ptr<i32>>(read<ptr<i32>>(%11), null<ptr<i32>>)));
// DEFAULT-NEXT:                 if ne<i32>(read<bool>(%18), const<i32>(0))
// DEFAULT-NEXT:                     read<bool>(%18);
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<void, signature=fn(ptr<const i8>, ptr<const i8>, u32, ptr<const i8>) -> void>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%19)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%20)), reinterpret<u32, reason=arg, fits=always>(const<i32>(20)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%21)));
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%6), const<i32>(0))
// DEFAULT-NEXT:                     for %22
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             write<i32>(%10, const<i32>(0));
// DEFAULT-NEXT:                         condition: lt<i32>(read<i32>(%10), const<i32>(1))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %23: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                             let %24: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%23), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%10, read<i32>(%24));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             ;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
