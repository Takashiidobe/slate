/* Copyright (C) 2001 Free Software Foundation, Inc.  */

/* { dg-do compile } */
/* { dg-options "-Wtraditional -std=c89" } */

/* This tests various diagnostics with -Wtraditioanl about escape
   sequences, for both the preprocessor and the compiler.

   Neil Booth, 22 May 2001.  */

#if '\a'		/* { dg-warning "traditional" "traditional bell" } */
#endif
#if '\x1a' != 26	/* { dg-warning "traditional" "traditional hex" } */
 #error bad hex		/* { dg-bogus "bad" "bad hexadecimal evaluation" } */
#endif
#if L'\u00a1'		/* { dg-warning "only valid" "\u is unknown in C89" } */
#endif

void foo ()
{
  int c = '\a';		/* { dg-warning "traditional" "traditional bell" } */

  c = '\xa1';		/* { dg-warning "traditional" "traditional hex" } */
  c = L'\u00a1';	/* { dg-warning "only valid" "\u is unknown in C89" } */
}

// SLATE-FILECHECK-STD DEFAULT c89
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
// DEFAULT-NEXT:     fn %0 @foo(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %1 c: i32 [storage=automatic] = const<i32>(7);
// DEFAULT-NEXT:         write<i32>(%1, const<i32>(-95));
// DEFAULT-NEXT:         write<i32>(%1, const<i32>(161));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
