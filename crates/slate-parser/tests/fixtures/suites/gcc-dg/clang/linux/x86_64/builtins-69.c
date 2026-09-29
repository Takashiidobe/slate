/* PR middle-end/86308 - ICE in verify_gimple calling index() with
   an invalid declaration
   { dg-do compile }
   { dg-options "-O2 -Wall" }  */

int index (int, int);   /* { dg-warning "conflicting types for built-in function .index.; expected .char \\\*\\\(const char \\\*, int\\\)." } */

int test_index (void)
{
  return index (0, 0);
}


/* PR middle-end/86202 - ICE in get_range_info calling an invalid memcpy()
   declaration */

void *memcpy (void *, void *, __SIZE_TYPE__ *);   /* { dg-warning "conflicting types for built-in function .memcpy.; expected .void \\\*\\\(void \\\*, const void \\\*, \(long \)*unsigned int\\\)." } */

void test_memcpy (void *p, void *q, __SIZE_TYPE__ *r)
{
  memcpy (p, q, r);
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
// DEFAULT-NEXT:     fn %[[VALUE_index:[0-9]+]] @index(%[[VALUE0:[0-9]+]] <unnamed>: i32, %[[VALUE1:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_index:[0-9]+]] @test_index() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_index]], const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_memcpy:[0-9]+]] @memcpy(%[[VALUE2:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE3:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE4:[0-9]+]] <unnamed>: ptr<u64>) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_memcpy:[0-9]+]] @test_memcpy(%[[VALUE_p:[0-9]+]] p: ptr<void>, %[[VALUE_q:[0-9]+]] q: ptr<void>, %[[VALUE_r:[0-9]+]] r: ptr<u64>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<void>, ptr<u64>) -> ptr<void>>(%[[VALUE_memcpy]], read<ptr<void>>(%[[VALUE_p]]), read<ptr<void>>(%[[VALUE_q]]), read<ptr<u64>>(%[[VALUE_r]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
