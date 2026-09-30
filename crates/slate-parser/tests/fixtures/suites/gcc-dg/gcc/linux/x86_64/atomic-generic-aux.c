/* Supply a set of generic atomic functions to test the compiler make the
   calls properly.  */
/* { dg-do compile } */
/* { dg-options "-w" } */

/* Test that the generic builtins make calls as expected.  */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

void
__atomic_exchange (size_t size, void *obj, void *val, void *ret, int model)
{
  /* Copy old value into *ret.  */
  memcpy (ret, obj, size);
  /* Copy val into object.  */
  memcpy (obj, val, size);
}


/* Note that the external version of this routine has the boolean weak/strong
   parameter removed.  This is required by the external library.  */
bool
__atomic_compare_exchange (size_t size, void *obj, void *expected,
			   void *desired, int model1, int model2)
{
  bool ret;
  if (!memcmp (obj, expected, size))
    {
      memcpy (obj, desired, size);
      ret = true;
    }
  else
    {
      memcpy (expected, obj, size);
      ret = false;
    }

  /* Make sure the parameters have been properly adjusted for the external
     function call (no weak/strong parameter.  */
  if (model1 != __ATOMIC_SEQ_CST || model2 != __ATOMIC_ACQUIRE)
    ret = !ret;

  return ret;
}


void __atomic_load (size_t size, void *obj, void *ret, int model)
{
  memcpy (ret, obj, size);
}


void __atomic_store (size_t size, void *obj, void *val, int model)
{
  memcpy (obj, val, size);
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
// DEFAULT-NEXT:     fn %[[VALUE_memcpy:[0-9]+]] @memcpy(%[[VALUE___dest:[0-9]+]] __dest: ptr<void> [restrict], %[[VALUE___src:[0-9]+]] __src: ptr<const void> [restrict], %[[VALUE___n:[0-9]+]] __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_memcmp:[0-9]+]] @memcmp(%[[VALUE___s1:[0-9]+]] __s1: ptr<const void>, %[[VALUE___s2:[0-9]+]] __s2: ptr<const void>, %[[VALUE___n_2:[0-9]+]] __n: u64) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE___atomic_exchange:[0-9]+]] @__atomic_exchange(%[[VALUE_size:[0-9]+]] size: u64, %[[VALUE_obj:[0-9]+]] obj: ptr<void>, %[[VALUE_val:[0-9]+]] val: ptr<void>, %[[VALUE_ret:[0-9]+]] ret: ptr<void>, %[[VALUE_model:[0-9]+]] model: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], read<ptr<void>>(%[[VALUE_ret]]), pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%[[VALUE_obj]])), read<u64>(%[[VALUE_size]]));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], read<ptr<void>>(%[[VALUE_obj]]), pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%[[VALUE_val]])), read<u64>(%[[VALUE_size]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___atomic_compare_exchange:[0-9]+]] @__atomic_compare_exchange(%[[VALUE_size_2:[0-9]+]] size: u64, %[[VALUE_obj_2:[0-9]+]] obj: ptr<void>, %[[VALUE_expected:[0-9]+]] expected: ptr<void>, %[[VALUE_desired:[0-9]+]] desired: ptr<void>, %[[VALUE_model1:[0-9]+]] model1: i32, %[[VALUE_model2:[0-9]+]] model2: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_ret_2:[0-9]+]] ret: bool [storage=automatic];
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%[[VALUE_obj_2]])), pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%[[VALUE_expected]])), read<u64>(%[[VALUE_size_2]])), const<i32>(0)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], read<ptr<void>>(%[[VALUE_obj_2]]), pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%[[VALUE_desired]])), read<u64>(%[[VALUE_size_2]]));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE_ret_2]], const<bool>(true));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], read<ptr<void>>(%[[VALUE_expected]]), pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%[[VALUE_obj_2]])), read<u64>(%[[VALUE_size_2]]));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE_ret_2]], const<bool>(false));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(%[[VALUE_model1]]), const<i32>(5)), ne<i32>(read<i32>(%[[VALUE_model2]]), const<i32>(2)))
// DEFAULT-NEXT:             write<bool>(%[[VALUE_ret_2]], not<bool>(read<bool>(%[[VALUE_ret_2]])));
// DEFAULT-NEXT:         return read<bool>(%[[VALUE_ret_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___atomic_load:[0-9]+]] @__atomic_load(%[[VALUE_size_3:[0-9]+]] size: u64, %[[VALUE_obj_3:[0-9]+]] obj: ptr<void>, %[[VALUE_ret_3:[0-9]+]] ret: ptr<void>, %[[VALUE_model_2:[0-9]+]] model: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], read<ptr<void>>(%[[VALUE_ret_3]]), pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%[[VALUE_obj_3]])), read<u64>(%[[VALUE_size_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___atomic_store:[0-9]+]] @__atomic_store(%[[VALUE_size_4:[0-9]+]] size: u64, %[[VALUE_obj_4:[0-9]+]] obj: ptr<void>, %[[VALUE_val_2:[0-9]+]] val: ptr<void>, %[[VALUE_model_3:[0-9]+]] model: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], read<ptr<void>>(%[[VALUE_obj_4]]), pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%[[VALUE_val_2]])), read<u64>(%[[VALUE_size_4]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
