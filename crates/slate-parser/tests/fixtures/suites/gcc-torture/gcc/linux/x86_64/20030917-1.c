// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT gnu89

/* { dg-additional-options "-std=gnu89" } */

typedef struct string STR;
typedef struct atbl ARRAY;
struct string {
    unsigned char str_pok;
};
struct atbl {
    int ary_fill;
};
blah(size,strp)
register int size;
register STR **strp;
{
    register ARRAY *ar;
    ar->ary_fill = size - 1;
    while (size--)
     (*strp)->str_pok &= ~128;
}

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
// DEFAULT-NEXT:     type @type0 string = struct {
// DEFAULT-NEXT:         field0 str_pok: u8;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type1 STR = @type0;
// DEFAULT-NEXT:     type @type2 atbl = struct {
// DEFAULT-NEXT:         field0 ary_fill: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type3 ARRAY = @type2;
// DEFAULT-NEXT:     fn %4 @blah(%5 size: i32, %6 strp: ptr<ptr<@type0>>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %7 ar: ptr<@type2> [storage=automatic];
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type2>>(%7))), sub<i32, overflow=ub>(read<i32>(%5), const<i32>(1)));
// DEFAULT-NEXT:         while %8 {
// DEFAULT-NEXT:             let %9: i32 [synthetic] = read<i32>(%5);
// DEFAULT-NEXT:             let %10: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%9), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%5, read<i32>(%10));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%9), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             let %11: ptr<@type0> [synthetic] = read<ptr<@type0>>(deref(read<ptr<ptr<@type0>>>(%6)));
// DEFAULT-NEXT:             let %12: u8 [synthetic] = read<u8>(field0(deref(read<ptr<@type0>>(%11))));
// DEFAULT-NEXT:             let %13: u8 [synthetic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%12))), not<i32>(const<i32>(128)))));
// DEFAULT-NEXT:             write<u8>(field0(deref(read<ptr<@type0>>(%11))), read<u8>(%13));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
