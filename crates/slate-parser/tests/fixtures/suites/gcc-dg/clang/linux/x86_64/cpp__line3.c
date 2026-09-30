/* { dg-do compile } */
/* { dg-options "-Wwrite-strings" } */ 
/* The purpose of this test is to ensure that line numbers in diagnostics
   are accurate after macros whose arguments contain newlines and are
   substituted multiple times.  The semicolons are on separate lines because
   #line can only correct numbering on line boundaries.  */
#define one(x) x
#define two(x) x x
#define four(x) two(x) two(x)

int
main(void)
{
  char *A;

  A = "text";		/* { dg-warning "discards 'const' qualifier" "case zero" } */
  A = one("text"	/* { dg-warning "discards 'const' qualifier" "case one" } */
	  "text")
	;
  A = two("text"	/* { dg-warning "discards 'const' qualifier" "case two" } */
	  "text")
	;
  A = four("text"	/* { dg-warning "discards 'const' qualifier" "case four" } */
	   "text")
	;

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
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([116, 101, 120, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([116, 101, 120, 116, 116, 101, 120, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 17> [storage=static] = code_units<array<i8, 17>>([116, 101, 120, 116, 116, 101, 120, 116, 116, 101, 120, 116, 116, 101, 120, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 33> [storage=static] = code_units<array<i8, 33>>([116, 101, 120, 116, 116, 101, 120, 116, 116, 101, 120, 116, 116, 101, 120, 116, 116, 101, 120, 116, 116, 101, 120, 116, 116, 101, 120, 116, 116, 101, 120, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_A:[0-9]+]] A: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_A]], array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str]]));
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_A]], array_decay<ptr<i8>, length=Some(9)>(%[[VALUE_str_2]]));
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_A]], array_decay<ptr<i8>, length=Some(17)>(%[[VALUE_str_3]]));
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_A]], array_decay<ptr<i8>, length=Some(33)>(%[[VALUE_str_4]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
