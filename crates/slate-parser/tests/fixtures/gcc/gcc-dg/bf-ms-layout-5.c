/* PR target/52991 */
/* { dg-do run { target i?86-*-* x86_64-*-* } } */

struct S {
  int                              a : 2;
  __attribute__((aligned(8))) int  b : 2;
  int                              c : 28;
  __attribute__((aligned(16))) int d : 2;
  int                              e : 30;
} __attribute__((ms_struct));

struct S s;

int
main() {
  int i;
  if (sizeof(s) != 32)
    __builtin_abort();
  s.a = -1;
  for (i = 0; i < 32; ++i)
    if (((char *)&s)[i] != (i ? 0 : 3))
      __builtin_abort();
  s.a = 0;
  s.b = -1;
  for (i = 0; i < 32; ++i)
    if (((char *)&s)[i] != (i ? 0 : 12))
      __builtin_abort();
  s.b = 0;
  s.c = -1;
  for (i = 0; i < 32; ++i)
    if (((signed char *)&s)[i] != (i > 3 ? 0 : (i ? -1 : -16)))
      __builtin_abort();
  s.c = 0;
  s.d = -1;
  for (i = 0; i < 32; ++i)
    if (((signed char *)&s)[i] != (i == 16 ? 3 : 0))
      __builtin_abort();
  s.d = 0;
  s.e = -1;
  for (i = 0; i < 32; ++i)
    if (((signed char *)&s)[i] !=
        ((i < 16 || i > 19) ? 0 : (i == 16 ? -4 : -1)))
      __builtin_abort();
  return 0;
}



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
// DEFAULT-NEXT:     type @type0 S = struct {
// DEFAULT-NEXT:         field0 a: i32 : 2;
// DEFAULT-NEXT:         field1 b: i32 : 2;
// DEFAULT-NEXT:         field2 c: i32 : 28;
// DEFAULT-NEXT:         field3 d: i32 : 2;
// DEFAULT-NEXT:         field4 e: i32 : 30;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 0, 0, 16, 16], bit_offsets=[Some(0), Some(2), Some(4), Some(128), Some(130)], bit_units=[(0, 4), (16, 4)], field_units=[Some(0), Some(0), Some(0), Some(1), Some(1)]];
// DEFAULT-NEXT:     global %1 s: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %4 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %3 i: i32 [storage=automatic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(32), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(32))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         write<i32>(bitfield0<unit=0, bytes=0..4, bits=0..2>(%1), neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:         for %5
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%3, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%3), const<i32>(32))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %10: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:                 let %11: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%10), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%3, read<i32>(%11));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type0>>(%1)), read<i32>(%3))))), conditional<i32>(ne<i32>(read<i32>(%3), const<i32>(0)), const<i32>(0), const<i32>(3)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         write<i32>(bitfield0<unit=0, bytes=0..4, bits=0..2>(%1), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(bitfield1<unit=0, bytes=0..4, bits=2..4>(%1), neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:         for %6
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%3, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%3), const<i32>(32))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %12: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:                 let %13: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%12), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%3, read<i32>(%13));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type0>>(%1)), read<i32>(%3))))), conditional<i32>(ne<i32>(read<i32>(%3), const<i32>(0)), const<i32>(0), const<i32>(12)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         write<i32>(bitfield1<unit=0, bytes=0..4, bits=2..4>(%1), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(bitfield2<unit=0, bytes=0..4, bits=4..32>(%1), neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:         for %7
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%3, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%3), const<i32>(32))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %14: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:                 let %15: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%14), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%3, read<i32>(%15));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type0>>(%1)), read<i32>(%3))))), conditional<i32>(gt<i32>(read<i32>(%3), const<i32>(3)), const<i32>(0), conditional<i32>(ne<i32>(read<i32>(%3), const<i32>(0)), neg<i32, overflow=ub>(const<i32>(1)), neg<i32, overflow=ub>(const<i32>(16)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         write<i32>(bitfield2<unit=0, bytes=0..4, bits=4..32>(%1), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(bitfield3<unit=1, bytes=16..20, bits=0..2>(%1), neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:         for %8
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%3, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%3), const<i32>(32))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %16: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:                 let %17: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%16), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%3, read<i32>(%17));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type0>>(%1)), read<i32>(%3))))), conditional<i32>(eq<i32>(read<i32>(%3), const<i32>(16)), const<i32>(3), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         write<i32>(bitfield3<unit=1, bytes=16..20, bits=0..2>(%1), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(bitfield4<unit=1, bytes=16..20, bits=2..32>(%1), neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:         for %9
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%3, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%3), const<i32>(32))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %18: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:                 let %19: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%18), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%3, read<i32>(%19));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type0>>(%1)), read<i32>(%3))))), conditional<i32>(logical_or<bool>(lt<i32>(read<i32>(%3), const<i32>(16)), gt<i32>(read<i32>(%3), const<i32>(19))), const<i32>(0), conditional<i32>(eq<i32>(read<i32>(%3), const<i32>(16)), neg<i32, overflow=ub>(const<i32>(4)), neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
