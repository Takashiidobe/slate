// SLATE-FILECHECK-DEFINES DEFAULT

/* Copyright 2000 Free Software Foundation

   by Alexandre Oliva  <aoliva@redhat.com>

   Based on zlib/gzio.c.

   This used to generate duplicate labels when compiled with
   sh-elf-gcc -O2 -m3 -fPIC.

   Bug reported by NIIBE Yutaka <gniibe@m17n.org>.  */

void foo (void);

void
bar ()
{
    unsigned len;

    for (len = 0; len < 2; len++)
	foo ();
}

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
// DEFAULT-NEXT:     fn %0 @foo() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @bar() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %2 len: u32 [storage=automatic];
// DEFAULT-NEXT:         for %3
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u32>(%2, reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             condition: lt<u32>(read<u32>(%2), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %4: u32 [synthetic] = read<u32>(%2);
// DEFAULT-NEXT:                 let %5: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%4), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<u32>(%2, read<u32>(%5));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
