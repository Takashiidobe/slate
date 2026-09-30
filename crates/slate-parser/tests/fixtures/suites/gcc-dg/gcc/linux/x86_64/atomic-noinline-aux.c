/* Supply a set of generic atomic functions to test the compiler make the
   calls properly.  */
/* { dg-do compile } */
/* { dg-options "-w" } */

/* Test that the generic builtins make calls as expected.  This file provides
   the exact entry points the test file will require.  All these routines
   simply set the first parameter to 1, and the caller will test for that.  */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


char 
__atomic_exchange_1 (char *p, char t, int i)
{
  *p = 1;
}

short
__atomic_load_2 (short *p, int i)
{ 
  *p = 1;
}

void
__atomic_store_1 (char *p, char v, int i)
{
  *p = 1;
}

int __atomic_compare_exchange_2 (short *p, short *a, short b, int y, int z)
{
  /* Fail if the memory models aren't correct as that will indicate the external
     call has failed to remove the weak/strong parameter as required by the
     library.  */
  if (y != __ATOMIC_SEQ_CST || z != __ATOMIC_ACQUIRE)
    *p = 0;
  else
    *p = 1;
}

char __atomic_fetch_add_1 (char *p, char v, int i)
{
  *p = 1;
}

short __atomic_fetch_add_2 (short *p, short v, int i)
{
  *p = 1;
}

/* Really perform a NAND.  PR51040 showed incorrect calculation of a 
   non-inlined fetch_nand.  */
unsigned char 
__atomic_fetch_nand_1 (unsigned char *p, unsigned char v, int i)
{
  unsigned char ret;

  ret = *p;
  *p = ~(*p & v);

  return ret;
}

bool __atomic_is_lock_free (size_t i, void *p)
{
  *(short *)p = 1;
  return true;
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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     fn %[[VALUE___atomic_exchange_1:[0-9]+]] @__atomic_exchange_1(%[[VALUE_p:[0-9]+]] p: ptr<i8>, %[[VALUE_t:[0-9]+]] t: i8, %[[VALUE_i:[0-9]+]] i: i32) -> i8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i8>(deref(read<ptr<i8>>(%[[VALUE_p]])), truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___atomic_load_2:[0-9]+]] @__atomic_load_2(%[[VALUE_p_2:[0-9]+]] p: ptr<i16>, %[[VALUE_i_2:[0-9]+]] i: i32) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i16>(deref(read<ptr<i16>>(%[[VALUE_p_2]])), truncate<i16, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___atomic_store_1:[0-9]+]] @__atomic_store_1(%[[VALUE_p_3:[0-9]+]] p: ptr<i8>, %[[VALUE_v:[0-9]+]] v: i8, %[[VALUE_i_3:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8>(deref(read<ptr<i8>>(%[[VALUE_p_3]])), truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___atomic_compare_exchange_2:[0-9]+]] @__atomic_compare_exchange_2(%[[VALUE_p_4:[0-9]+]] p: ptr<i16>, %[[VALUE_a:[0-9]+]] a: ptr<i16>, %[[VALUE_b:[0-9]+]] b: i16, %[[VALUE_y:[0-9]+]] y: i32, %[[VALUE_z:[0-9]+]] z: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(%[[VALUE_y]]), const<i32>(5)), ne<i32>(read<i32>(%[[VALUE_z]]), const<i32>(2)))
// DEFAULT-NEXT:             write<i16>(deref(read<ptr<i16>>(%[[VALUE_p_4]])), truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<i16>(deref(read<ptr<i16>>(%[[VALUE_p_4]])), truncate<i16, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___atomic_fetch_add_1:[0-9]+]] @__atomic_fetch_add_1(%[[VALUE_p_5:[0-9]+]] p: ptr<i8>, %[[VALUE_v_2:[0-9]+]] v: i8, %[[VALUE_i_4:[0-9]+]] i: i32) -> i8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i8>(deref(read<ptr<i8>>(%[[VALUE_p_5]])), truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___atomic_fetch_add_2:[0-9]+]] @__atomic_fetch_add_2(%[[VALUE_p_6:[0-9]+]] p: ptr<i16>, %[[VALUE_v_3:[0-9]+]] v: i16, %[[VALUE_i_5:[0-9]+]] i: i32) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i16>(deref(read<ptr<i16>>(%[[VALUE_p_6]])), truncate<i16, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___atomic_fetch_nand_1:[0-9]+]] @__atomic_fetch_nand_1(%[[VALUE_p_7:[0-9]+]] p: ptr<u8>, %[[VALUE_v_4:[0-9]+]] v: u8, %[[VALUE_i_6:[0-9]+]] i: i32) -> u8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_ret:[0-9]+]] ret: u8 [storage=automatic];
// DEFAULT-NEXT:         write<u8>(%[[VALUE_ret]], read<u8>(deref(read<ptr<u8>>(%[[VALUE_p_7]]))));
// DEFAULT-NEXT:         write<u8>(deref(read<ptr<u8>>(%[[VALUE_p_7]])), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(not<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(read<ptr<u8>>(%[[VALUE_p_7]]))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_v_4]]))))))));
// DEFAULT-NEXT:         return read<u8>(%[[VALUE_ret]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___atomic_is_lock_free:[0-9]+]] @__atomic_is_lock_free(%[[VALUE_i_7:[0-9]+]] i: u64, %[[VALUE_p_8:[0-9]+]] p: ptr<void>) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i16>(deref(pointer_cast<ptr<i16>, reason=explicit>(read<ptr<void>>(%[[VALUE_p_8]]))), truncate<i16, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         return const<bool>(true);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
