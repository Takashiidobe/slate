/* { dg-do run } */
/* { dg-options "-O2" } */
#include <stddef.h>
void *a;
int *b;
struct c {void * a;} c;
struct d {short * a;} d;

int *ip= (int *)(size_t)2;
int **ipp = &ip;

int
main()
{
  float **ptr;
  void **uptr;
  int* const* cipp = (int* const*)ipp;
  /* as an extension we consider void * universal. Writes to it should alias.  */
  asm ("":"=r"(ptr):"0"(&a));
  a=NULL;
  *ptr=(float*)(size_t)1;
  if (!a)
    __builtin_abort ();
  a=NULL;
  if (*ptr)
    __builtin_abort ();

  asm ("":"=r"(uptr):"0"(&b));
  b=NULL;
  *uptr=(void*)(size_t)1;
  if (!b)
    __builtin_abort ();
  b=NULL;
  if (*uptr)
    __builtin_abort ();

  /* Check that we disambiguate int * and char *.  */
  asm ("":"=r"(ptr):"0"(&b));
  b=NULL;
  *ptr=(float*)(size_t)1;
  if (b)
    __builtin_abort ();

  /* Again we should make void * in the structure conflict with any pointer.  */
  asm ("":"=r"(ptr):"0"(&c));
  c.a=NULL;
  *ptr=(float*)(size_t)1;
  if (!c.a)
    __builtin_abort ();
  c.a=NULL;
  if (*ptr)
    __builtin_abort ();

  asm ("":"=r"(uptr):"0"(&d));
  d.a=NULL;
  *uptr=(void*)(size_t)1;
  if (!d.a)
    __builtin_abort ();
  d.a=NULL;
  if (*uptr)
    __builtin_abort ();

  if ((void *)*cipp != (void*)(size_t)2)
    __builtin_abort ();
  *ipp = NULL;
  if (*cipp)
    __builtin_abort ();

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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_c:[0-9]+]] c = struct {
// DEFAULT-NEXT:         field0 a: ptr<void>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_d:[0-9]+]] d = struct {
// DEFAULT-NEXT:         field0 a: ptr<i16>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: ptr<void> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: ptr<i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: @type[[TYPE_c]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: @type[[TYPE_d]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ip:[0-9]+]] ip: ptr<i32> [storage=static] = int_to_ptr<ptr<i32>, reason=explicit>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ipp:[0-9]+]] ipp: ptr<ptr<i32>> [storage=static] = addr_of<ptr<ptr<i32>>>(%[[VALUE_ip]]) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_ptr:[0-9]+]] ptr: ptr<ptr<f32>> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_uptr:[0-9]+]] uptr: ptr<ptr<void>> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_cipp:[0-9]+]] cipp: ptr<const ptr<i32>> [storage=automatic] = pointer_cast<ptr<const ptr<i32>>, reason=explicit>(read<ptr<ptr<i32>>>(%[[VALUE_ipp]]));
// DEFAULT-NEXT:         asm "" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             inlateout 0 "r" [reg] width 64 place<ptr<ptr<f32>>>(%[[VALUE_ptr]]) from addr_of<ptr<ptr<void>>>(%[[VALUE_a]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<ptr<void>>(%[[VALUE_a]], null<ptr<void>>);
// DEFAULT-NEXT:         write<ptr<f32>>(deref(read<ptr<ptr<f32>>>(%[[VALUE_ptr]])), int_to_ptr<ptr<f32>, reason=explicit>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))));
// DEFAULT-NEXT:         if not<bool>(ne<ptr<void>>(read<ptr<void>>(%[[VALUE_a]]), null<ptr<void>>))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         write<ptr<void>>(%[[VALUE_a]], null<ptr<void>>);
// DEFAULT-NEXT:         if ne<ptr<f32>>(read<ptr<f32>>(deref(read<ptr<ptr<f32>>>(%[[VALUE_ptr]]))), null<ptr<f32>>)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         asm "" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             inlateout 0 "r" [reg] width 64 place<ptr<ptr<void>>>(%[[VALUE_uptr]]) from addr_of<ptr<ptr<i32>>>(%[[VALUE_b]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_b]], null<ptr<i32>>);
// DEFAULT-NEXT:         write<ptr<void>>(deref(read<ptr<ptr<void>>>(%[[VALUE_uptr]])), int_to_ptr<ptr<void>, reason=explicit>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))));
// DEFAULT-NEXT:         if not<bool>(ne<ptr<i32>>(read<ptr<i32>>(%[[VALUE_b]]), null<ptr<i32>>))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_b]], null<ptr<i32>>);
// DEFAULT-NEXT:         if ne<ptr<void>>(read<ptr<void>>(deref(read<ptr<ptr<void>>>(%[[VALUE_uptr]]))), null<ptr<void>>)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         asm "" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             inlateout 0 "r" [reg] width 64 place<ptr<ptr<f32>>>(%[[VALUE_ptr]]) from addr_of<ptr<ptr<i32>>>(%[[VALUE_b]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_b]], null<ptr<i32>>);
// DEFAULT-NEXT:         write<ptr<f32>>(deref(read<ptr<ptr<f32>>>(%[[VALUE_ptr]])), int_to_ptr<ptr<f32>, reason=explicit>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))));
// DEFAULT-NEXT:         if ne<ptr<i32>>(read<ptr<i32>>(%[[VALUE_b]]), null<ptr<i32>>)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         asm "" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             inlateout 0 "r" [reg] width 64 place<ptr<ptr<f32>>>(%[[VALUE_ptr]]) from addr_of<ptr<@type[[TYPE_c]]>>(%[[VALUE_c]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<ptr<void>>(field0(%[[VALUE_c]]), null<ptr<void>>);
// DEFAULT-NEXT:         write<ptr<f32>>(deref(read<ptr<ptr<f32>>>(%[[VALUE_ptr]])), int_to_ptr<ptr<f32>, reason=explicit>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))));
// DEFAULT-NEXT:         if not<bool>(ne<ptr<void>>(read<ptr<void>>(field0(%[[VALUE_c]])), null<ptr<void>>))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         write<ptr<void>>(field0(%[[VALUE_c]]), null<ptr<void>>);
// DEFAULT-NEXT:         if ne<ptr<f32>>(read<ptr<f32>>(deref(read<ptr<ptr<f32>>>(%[[VALUE_ptr]]))), null<ptr<f32>>)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         asm "" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             inlateout 0 "r" [reg] width 64 place<ptr<ptr<void>>>(%[[VALUE_uptr]]) from addr_of<ptr<@type[[TYPE_d]]>>(%[[VALUE_d]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<ptr<i16>>(field0(%[[VALUE_d]]), null<ptr<i16>>);
// DEFAULT-NEXT:         write<ptr<void>>(deref(read<ptr<ptr<void>>>(%[[VALUE_uptr]])), int_to_ptr<ptr<void>, reason=explicit>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))));
// DEFAULT-NEXT:         if not<bool>(ne<ptr<i16>>(read<ptr<i16>>(field0(%[[VALUE_d]])), null<ptr<i16>>))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         write<ptr<i16>>(field0(%[[VALUE_d]]), null<ptr<i16>>);
// DEFAULT-NEXT:         if ne<ptr<void>>(read<ptr<void>>(deref(read<ptr<ptr<void>>>(%[[VALUE_uptr]]))), null<ptr<void>>)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<ptr<void>>(pointer_cast<ptr<void>, reason=explicit>(read<ptr<i32>>(deref(read<ptr<const ptr<i32>>>(%[[VALUE_cipp]])))), int_to_ptr<ptr<void>, reason=explicit>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         write<ptr<i32>>(deref(read<ptr<ptr<i32>>>(%[[VALUE_ipp]])), null<ptr<i32>>);
// DEFAULT-NEXT:         if ne<ptr<i32>>(read<ptr<i32>>(deref(read<ptr<const ptr<i32>>>(%[[VALUE_cipp]]))), null<ptr<i32>>)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
