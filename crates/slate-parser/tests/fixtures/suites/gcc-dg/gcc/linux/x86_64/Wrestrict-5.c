/* PR tree-optimization/83655 - ICE on an invalid call to memcpy declared
   with no prototype
   Test to verify that valid calls to common restrict-qualified built-in
   functions declared with no prototype are checked for overlap, and that
   invalid calls are ignored.
  { dg-do compile }
  { dg-options "-std=gnu17 -O2 -fpermissive -Wrestrict" }  */

void* memcpy ();
char* strncpy ();

#if __cplusplus
}   /* extern "C" */
#endif

void test_memcpy_warn (char *d)
{
  memcpy (d, d + 2, 3);       /* { dg-warning "accessing 3 bytes at offsets 0 and 2 overlaps 1 byte at offset 2" } */
}

void test_memcpy_nowarn (char *d)
{
  memcpy (d, d + 2, "");
}


void test_strncpy_warn (char *d)
{
  strncpy (d + 1, d + 3, 5);  /* { dg-warning "accessing 5 bytes at offsets 1 and 3 overlaps between 1 and 2 bytes at offset 3" } */
}

void test_strncpy_nowarn (char *d)
{
  strncpy (d + 1, d + 3, "");
}

/* { dg-prune-output "\\\[-Wbuiltin-declaration-mismatch]" }
   { dg-prune-output "\\\[-Wint-conversion]" } */

// SLATE-FILECHECK-ARGS -fpermissive
// SLATE-FILECHECK-STD DEFAULT gnu17
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
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_memcpy:[0-9]+]] @memcpy(unprototyped) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strncpy:[0-9]+]] @strncpy(unprototyped) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_memcpy_warn:[0-9]+]] @test_memcpy_warn(%[[VALUE_d:[0-9]+]] d: ptr<i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%[[VALUE_d]])), pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_d]]), const<i32>(2))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_memcpy_nowarn:[0-9]+]] @test_memcpy_nowarn(%[[VALUE_d_2:[0-9]+]] d: ptr<i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%[[VALUE_d_2]])), pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_d_2]]), const<i32>(2))), ptr_to_int<u64, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_strncpy_warn:[0-9]+]] @test_strncpy_warn(%[[VALUE_d_3:[0-9]+]] d: ptr<i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_d_3]]), const<i32>(1)), pointer_cast<ptr<const i8>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_d_3]]), const<i32>(3))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_strncpy_nowarn:[0-9]+]] @test_strncpy_nowarn(%[[VALUE_d_4:[0-9]+]] d: ptr<i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_d_4]]), const<i32>(1)), pointer_cast<ptr<const i8>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_d_4]]), const<i32>(3))), ptr_to_int<u64, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_2]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
