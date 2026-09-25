void bodies(int x) {
  if (x) { x++; } else x--;
  if (x) x++; else { x--; }
  while (x) { x--; }
  while (x) x--;
  do { x++; } while (x);
  do x++; while (x);
  for (; x; x--) { x++; }
  for (; x; x--) x++;
  switch (x) { case 0: x++; break; }
  switch (x) default: x++;
  if (x) {} else ;
}
typedef int T;
int condition_typedef(void) {
  if (sizeof(enum { T = 1 })) (void)sizeof(T);
  return sizeof(T);
}
int body_typedef(void) {
  if (1) (void)sizeof(enum { T = 1 });
  return sizeof(T);
}
int do_typedef(void) {
  do (void)sizeof(enum { T = 1 }); while (sizeof(T));
  return sizeof(T);
}
int nested_typedef(void) {
  if (sizeof(enum { T = 1 })) {
    typedef int T;
    T x;
  }
  return sizeof(T);
}

// SLATE-FILECHECK-DEFINES C89
// SLATE-FILECHECK-STD C89 c89
// SLATE-FILECHECK-DEFINES C99
// SLATE-FILECHECK-STD C99 c99

// SLATE-FILECHECK-IR-ERROR C99

// SLATE-FILECHECK-BEGIN C99
// C99: Error:   × unsupported in numeric IR lowering: unknown typedef
// SLATE-FILECHECK-END C99
// SLATE-FILECHECK-BEGIN C89
// C89: module {
// C89-NEXT:     target "x86_64-unknown-linux-gnu" {
// C89-NEXT:         endian = little;
// C89-NEXT:         pointer [size=8, align=8];
// C89-NEXT:         stack_alignment = 16;
// C89-NEXT:         long_double = f80;
// C89-NEXT:         storage bool [size=1, align=1];
// C89-NEXT:         storage i8, u8 [size=1, align=1];
// C89-NEXT:         storage i16, u16 [size=2, align=2];
// C89-NEXT:         storage i32, u32 [size=4, align=4];
// C89-NEXT:         storage i64, u64 [size=8, align=8];
// C89-NEXT:         storage i128, u128 [size=16, align=16];
// C89-NEXT:         storage bf16 [size=2, align=2];
// C89-NEXT:         storage f16 [size=2, align=2];
// C89-NEXT:         storage f32 [size=4, align=4];
// C89-NEXT:         storage f64 [size=8, align=8];
// C89-NEXT:         storage f80 [size=16, align=16];
// C89-NEXT:         storage f128 [size=16, align=16];
// C89-NEXT:         storage d32 [size=4, align=4];
// C89-NEXT:         storage d64 [size=8, align=8];
// C89-NEXT:         storage d128 [size=16, align=16];
// C89-NEXT:     }
// C89-NEXT:     type @type0 T = i32;
// C89-NEXT:     type @type1 = enum : u32 {
// C89-NEXT:         %0 T = const<i32>(1);
// C89-NEXT:     } [size=4, align=4];
// C89-NEXT:     type @type2 = enum : u32 {
// C89-NEXT:         %0 T = const<i32>(1);
// C89-NEXT:     } [size=4, align=4];
// C89-NEXT:     type @type3 = enum : u32 {
// C89-NEXT:         %0 T = const<i32>(1);
// C89-NEXT:     } [size=4, align=4];
// C89-NEXT:     type @type4 = enum : u32 {
// C89-NEXT:         %0 T = const<i32>(1);
// C89-NEXT:     } [size=4, align=4];
// C89-NEXT:     type @type5 T = i32;
// C89-NEXT:     fn %0 @bodies(%1 x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// C89-NEXT:         if ne<i32>(read<i32>(%1), const<i32>(0))
// C89-NEXT:             {
// C89-NEXT:                 let %26: i32 [synthetic] = read<i32>(%1);
// C89-NEXT:                 let %27: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%26), const<i32>(1));
// C89-NEXT:                 write<i32>(%1, read<i32>(%27));
// C89-NEXT:             }
// C89-NEXT:         else
// C89-NEXT:             let %28: i32 [synthetic] = read<i32>(%1);
// C89-NEXT:             let %29: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%28), const<i32>(1));
// C89-NEXT:             write<i32>(%1, read<i32>(%29));
// C89-NEXT:         if ne<i32>(read<i32>(%1), const<i32>(0))
// C89-NEXT:             let %30: i32 [synthetic] = read<i32>(%1);
// C89-NEXT:             let %31: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%30), const<i32>(1));
// C89-NEXT:             write<i32>(%1, read<i32>(%31));
// C89-NEXT:         else
// C89-NEXT:             {
// C89-NEXT:                 let %32: i32 [synthetic] = read<i32>(%1);
// C89-NEXT:                 let %33: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%32), const<i32>(1));
// C89-NEXT:                 write<i32>(%1, read<i32>(%33));
// C89-NEXT:             }
// C89-NEXT:         while %17 ne<i32>(read<i32>(%1), const<i32>(0))
// C89-NEXT:             {
// C89-NEXT:                 let %34: i32 [synthetic] = read<i32>(%1);
// C89-NEXT:                 let %35: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%34), const<i32>(1));
// C89-NEXT:                 write<i32>(%1, read<i32>(%35));
// C89-NEXT:             }
// C89-NEXT:         while %18 ne<i32>(read<i32>(%1), const<i32>(0))
// C89-NEXT:             let %36: i32 [synthetic] = read<i32>(%1);
// C89-NEXT:             let %37: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%36), const<i32>(1));
// C89-NEXT:             write<i32>(%1, read<i32>(%37));
// C89-NEXT:         do %19
// C89-NEXT:             {
// C89-NEXT:                 let %38: i32 [synthetic] = read<i32>(%1);
// C89-NEXT:                 let %39: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%38), const<i32>(1));
// C89-NEXT:                 write<i32>(%1, read<i32>(%39));
// C89-NEXT:             }
// C89-NEXT:         while ne<i32>(read<i32>(%1), const<i32>(0));
// C89-NEXT:         do %20
// C89-NEXT:             let %40: i32 [synthetic] = read<i32>(%1);
// C89-NEXT:             let %41: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%40), const<i32>(1));
// C89-NEXT:             write<i32>(%1, read<i32>(%41));
// C89-NEXT:         while ne<i32>(read<i32>(%1), const<i32>(0));
// C89-NEXT:         for %21
// C89-NEXT:             init:
// C89-NEXT:             condition: ne<i32>(read<i32>(%1), const<i32>(0))
// C89-NEXT:             increment: {
// C89-NEXT:                 let %42: i32 [synthetic] = read<i32>(%1);
// C89-NEXT:                 let %43: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%42), const<i32>(1));
// C89-NEXT:                 write<i32>(%1, read<i32>(%43));
// C89-NEXT:                 yield void;
// C89-NEXT:             }
// C89-NEXT:             body:
// C89-NEXT:                 {
// C89-NEXT:                     let %44: i32 [synthetic] = read<i32>(%1);
// C89-NEXT:                     let %45: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%44), const<i32>(1));
// C89-NEXT:                     write<i32>(%1, read<i32>(%45));
// C89-NEXT:                 }
// C89-NEXT:         for %22
// C89-NEXT:             init:
// C89-NEXT:             condition: ne<i32>(read<i32>(%1), const<i32>(0))
// C89-NEXT:             increment: {
// C89-NEXT:                 let %46: i32 [synthetic] = read<i32>(%1);
// C89-NEXT:                 let %47: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%46), const<i32>(1));
// C89-NEXT:                 write<i32>(%1, read<i32>(%47));
// C89-NEXT:                 yield void;
// C89-NEXT:             }
// C89-NEXT:             body:
// C89-NEXT:                 let %48: i32 [synthetic] = read<i32>(%1);
// C89-NEXT:                 let %49: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%48), const<i32>(1));
// C89-NEXT:                 write<i32>(%1, read<i32>(%49));
// C89-NEXT:         switch %23 read<i32>(%1)
// C89-NEXT:             {
// C89-NEXT:                 case %23 const<i32>(0):
// C89-NEXT:                     let %50: i32 [synthetic] = read<i32>(%1);
// C89-NEXT:                     let %51: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%50), const<i32>(1));
// C89-NEXT:                     write<i32>(%1, read<i32>(%51));
// C89-NEXT:                 break %23;
// C89-NEXT:             }
// C89-NEXT:         switch %24 read<i32>(%1)
// C89-NEXT:             default %24:
// C89-NEXT:                 let %52: i32 [synthetic] = read<i32>(%1);
// C89-NEXT:                 let %53: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%52), const<i32>(1));
// C89-NEXT:                 write<i32>(%1, read<i32>(%53));
// C89-NEXT:         if ne<i32>(read<i32>(%1), const<i32>(0))
// C89-NEXT:             {
// C89-NEXT:             }
// C89-NEXT:         else
// C89-NEXT:             ;
// C89-NEXT:     }
// C89-NEXT:     fn %3 @condition_typedef() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// C89-NEXT:         if ne<u64>(const<u64>(4), const<u64>(0))
// C89-NEXT:             const<u64>(4);
// C89-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=always>(const<u64>(4)));
// C89-NEXT:     }
// C89-NEXT:     fn %6 @body_typedef() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// C89-NEXT:         if ne<i32>(const<i32>(1), const<i32>(0))
// C89-NEXT:             const<u64>(4);
// C89-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=always>(const<u64>(4)));
// C89-NEXT:     }
// C89-NEXT:     fn %9 @do_typedef() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// C89-NEXT:         do %25
// C89-NEXT:             const<u64>(4);
// C89-NEXT:         while ne<u64>(const<u64>(4), const<u64>(0));
// C89-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=always>(const<u64>(4)));
// C89-NEXT:     }
// C89-NEXT:     fn %12 @nested_typedef() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// C89-NEXT:         if ne<u64>(const<u64>(4), const<u64>(0))
// C89-NEXT:             {
// C89-NEXT:                 let %16 x: i32 [storage=automatic];
// C89-NEXT:             }
// C89-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=always>(const<u64>(4)));
// C89-NEXT:     }
// C89-NEXT: }
// SLATE-FILECHECK-END C89
