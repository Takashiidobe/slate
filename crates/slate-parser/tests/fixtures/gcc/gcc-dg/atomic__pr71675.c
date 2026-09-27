/* PR c++/71675 - __atomic_compare_exchange_n returns wrong type for typed
   enum  */
/* { dg-do compile } */
/* { dg-options "-std=c11" } */


#define Test(T)								\
  do {									\
    static T x;								\
    int r [_Generic (__atomic_compare_exchange_n (&x, &x, x, 0, 0, 0),	\
		     _Bool: 1, default: -1)];				\
    (void)&r;								\
  } while (0)

void f (void)
{
  /* __atomic_compare_exchange_n would fail to return _Bool when
     its arguments were one of the three character types.  */
  Test (char);
  Test (signed char);
  Test (unsigned char);

  Test (int);
  Test (unsigned int);

  Test (long);
  Test (unsigned long);

  Test (long long);
  Test (unsigned long long);

  typedef enum E { e } E;
  Test (E);
}

// SLATE-FILECHECK-FLAVOR gcc
// SLATE-FILECHECK-STD DEFAULT c11
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
// DEFAULT-NEXT:     type @type0 E = enum : u32 {
// DEFAULT-NEXT:         %0 e = const<i32>(0);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type1 E = @type0;
// DEFAULT-NEXT:     global %1 x: i8 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %3 x: i8 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %5 x: u8 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %7 x: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %9 x: u32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %11 x: i64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %13 x: u64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %15 x: i64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %17 x: u64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %22 x: @type0 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %0 @f() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         do %24
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %25: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:                 let %2 r: vla<i32, %25> [storage=automatic];
// DEFAULT-NEXT:                 addr_of<ptr<vla<i32, %25>>>(%2);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %26
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %27: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:                 let %4 r: vla<i32, %27> [storage=automatic];
// DEFAULT-NEXT:                 addr_of<ptr<vla<i32, %27>>>(%4);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %28
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %29: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:                 let %6 r: vla<i32, %29> [storage=automatic];
// DEFAULT-NEXT:                 addr_of<ptr<vla<i32, %29>>>(%6);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %30
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %31: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:                 let %8 r: vla<i32, %31> [storage=automatic];
// DEFAULT-NEXT:                 addr_of<ptr<vla<i32, %31>>>(%8);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %32
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %33: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:                 let %10 r: vla<i32, %33> [storage=automatic];
// DEFAULT-NEXT:                 addr_of<ptr<vla<i32, %33>>>(%10);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %34
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %35: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:                 let %12 r: vla<i32, %35> [storage=automatic];
// DEFAULT-NEXT:                 addr_of<ptr<vla<i32, %35>>>(%12);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %36
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %37: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:                 let %14 r: vla<i32, %37> [storage=automatic];
// DEFAULT-NEXT:                 addr_of<ptr<vla<i32, %37>>>(%14);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %38
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %39: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:                 let %16 r: vla<i32, %39> [storage=automatic];
// DEFAULT-NEXT:                 addr_of<ptr<vla<i32, %39>>>(%16);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %40
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %41: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:                 let %18 r: vla<i32, %41> [storage=automatic];
// DEFAULT-NEXT:                 addr_of<ptr<vla<i32, %41>>>(%18);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %42
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %43: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:                 let %23 r: vla<i32, %43> [storage=automatic];
// DEFAULT-NEXT:                 addr_of<ptr<vla<i32, %43>>>(%23);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
