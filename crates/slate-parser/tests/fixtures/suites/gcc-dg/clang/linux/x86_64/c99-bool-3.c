/* Test for _Bool bit-fields.  They have the semantics of _Bool, at
   least for now (DR#335 Spring 2007 discussion).  */
/* Origin: Joseph Myers <joseph@codesourcery.com> */
/* { dg-do run } */
/* { dg-options "-std=iso9899:1999 -pedantic-errors" } */
struct foo
{
  _Bool a : 1;
} sf;

extern void abort (void);
extern void exit (int);

int
main (void)
{
  int i;
  for (i = 0; i < sizeof (struct foo); i++)
    *((unsigned char *)&sf + i) = (unsigned char) -1;
  sf.a = 2;
  if (sf.a != 1)
    abort ();
  sf.a = 0;
  if (sf.a != 0)
    abort ();
  sf.a = 0.2;
  if (sf.a != 1)
    abort ();
  sf.a = &sf;
  if (sf.a != 1)
    abort ();
  exit (0);
}

// SLATE-FILECHECK-STD DEFAULT iso9899:1999
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
// DEFAULT-NEXT:     type @type[[TYPE_foo:[0-9]+]] foo = struct {
// DEFAULT-NEXT:         field0 a: bool : 1;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0], bit_offsets=[Some(0)], bit_units=[(0, 1)], field_units=[Some(0)]];
// DEFAULT-NEXT:     global %[[VALUE_sf:[0-9]+]] sf: @type[[TYPE_foo]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i]]))), const<u64>(1))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(pointer_cast<ptr<u8>, reason=explicit>(addr_of<ptr<@type[[TYPE_foo]]>>(%[[VALUE_sf]])), read<i32>(%[[VALUE_i]]))), reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         write<bool>(bitfield0<unit=0, bytes=0..1, bits=0..1>(%[[VALUE_sf]]), ne<i32, reason=assign>(const<i32>(2), const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(bitfield0<unit=0, bytes=0..1, bits=0..1>(%[[VALUE_sf]]))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<bool>(bitfield0<unit=0, bytes=0..1, bits=0..1>(%[[VALUE_sf]]), ne<i32, reason=assign>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(bitfield0<unit=0, bytes=0..1, bits=0..1>(%[[VALUE_sf]]))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<bool>(bitfield0<unit=0, bytes=0..1, bits=0..1>(%[[VALUE_sf]]), ne<f64, reason=assign, exceptions=ignore>(const<f64>(0.2), const<f64>(0.0)));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(bitfield0<unit=0, bytes=0..1, bits=0..1>(%[[VALUE_sf]]))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<bool>(bitfield0<unit=0, bytes=0..1, bits=0..1>(%[[VALUE_sf]]), ne<ptr<@type[[TYPE_foo]]>, reason=assign>(addr_of<ptr<@type[[TYPE_foo]]>>(%[[VALUE_sf]]), null<ptr<@type[[TYPE_foo]]>>));
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(read<bool>(bitfield0<unit=0, bytes=0..1, bits=0..1>(%[[VALUE_sf]]))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
