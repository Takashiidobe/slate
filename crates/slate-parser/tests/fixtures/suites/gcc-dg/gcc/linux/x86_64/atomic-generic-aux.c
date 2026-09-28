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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     fn %4 @memcpy(%33 __dest: ptr<void> [restrict], %34 __src: ptr<const void> [restrict], %35 __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %8 @memcmp(%36 __s1: ptr<const void>, %37 __s2: ptr<const void>, %38 __n: u64) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %9 @__atomic_exchange(%10 size: u64, %11 obj: ptr<void>, %12 val: ptr<void>, %13 ret: ptr<void>, %14 model: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%4, read<ptr<void>>(%13), pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%11)), read<u64>(%10));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%4, read<ptr<void>>(%11), pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%12)), read<u64>(%10));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @__atomic_compare_exchange(%16 size: u64, %17 obj: ptr<void>, %18 expected: ptr<void>, %19 desired: ptr<void>, %20 model1: i32, %21 model2: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %22 ret: bool [storage=automatic];
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%8, pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%17)), pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%18)), read<u64>(%16)), const<i32>(0)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%4, read<ptr<void>>(%17), pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%19)), read<u64>(%16));
// DEFAULT-NEXT:                 write<bool>(%22, const<bool>(true));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%4, read<ptr<void>>(%18), pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%17)), read<u64>(%16));
// DEFAULT-NEXT:                 write<bool>(%22, const<bool>(false));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(%20), const<i32>(5)), ne<i32>(read<i32>(%21), const<i32>(2)))
// DEFAULT-NEXT:             write<bool>(%22, not<bool>(read<bool>(%22)));
// DEFAULT-NEXT:         return read<bool>(%22);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @__atomic_load(%24 size: u64, %25 obj: ptr<void>, %26 ret: ptr<void>, %27 model: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%4, read<ptr<void>>(%26), pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%25)), read<u64>(%24));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %28 @__atomic_store(%29 size: u64, %30 obj: ptr<void>, %31 val: ptr<void>, %32 model: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%4, read<ptr<void>>(%30), pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%31)), read<u64>(%29));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
