// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-STD IR c23

extern double narrow(float x);
unsigned long sizes(int array[4]) {
    int local[3];
    return sizeof(local) + sizeof(array) + sizeof(narrow(1.0));
}
int nulls(int *p) {
    int *q = 2 - 2;
    int *r = nullptr;
    q = (int *)0;
    return p == (1 - 1) ? !q : p != r;
}
int shadow(int x) {
    { short x = 1; x += 2; }
    return x;
}
int address(int x) { int *p = &x; *p = 'a'; return +*p; }
float casts(double d, unsigned long u) { return (short)d + (float)u; }

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type0 = fn(f32) -> f64;
// IR-NEXT:     type @type1 = array<i32, 4>;
// IR-NEXT:     type @type2 = ptr<i32>;
// IR-NEXT:     type @type3 = fn(@type2) -> u64;
// IR-NEXT:     type @type4 = array<i32, 4>;
// IR-NEXT:     type @type5 = array<i32, 3>;
// IR-NEXT:     type @type6 = ptr<i32>;
// IR-NEXT:     type @type7 = fn(@type6) -> i32;
// IR-NEXT:     type @type8 = ptr<i32>;
// IR-NEXT:     type @type9 = ptr<i32>;
// IR-NEXT:     type @type10 = ptr<i32>;
// IR-NEXT:     type @type11 = ptr<void>;
// IR-NEXT:     type @type12 = ptr<i32>;
// IR-NEXT:     type @type13 = fn(i32) -> i32;
// IR-NEXT:     type @type14 = fn(i32) -> i32;
// IR-NEXT:     type @type15 = ptr<i32>;
// IR-NEXT:     type @type16 = fn(f64, u64) -> f32;
// IR-NEXT:     fn %0 @narrow(%17 x: f32) -> f64 [linkage=external];
// IR-NEXT:     fn %1 @sizes(%2 array: @type2) -> u64 [linkage=external] {
// IR-NEXT:         let %3 local: @type5 [storage=automatic];
// IR-NEXT:         return add<u64, overflow=wrap>(add<u64, overflow=wrap>(const<u64>(12), const<u64>(8)), const<u64>(8));
// IR-NEXT:     }
// IR-NEXT:     fn %4 @nulls(%5 p: @type8) -> i32 [linkage=external] {
// IR-NEXT:         let %6 q: @type9 [storage=automatic] = null<@type9>;
// IR-NEXT:         let %7 r: @type10 [storage=automatic] = null<@type10>;
// IR-NEXT:         store<@type9>(%6, null<@type9>);
// IR-NEXT:         return conditional<i32>(eq<@type8>(read<@type8>(%5), null<@type8>), from_bool<i32, reason=promotion>(not<bool>(ne<@type9>(read<@type9>(%6), null<@type9>))), from_bool<i32, reason=promotion>(ne<@type8>(read<@type8>(%5), pointer_cast<@type8, reason=usual_arith>(read<@type10>(%7)))));
// IR-NEXT:     }
// IR-NEXT:     fn %8 @shadow(%9 x: i32) -> i32 [linkage=external] {
// IR-NEXT:         {
// IR-NEXT:             let %10 x: i16 [storage=automatic] = truncate<i16, reason=assign, fits=always>(const<i32>(1));
// IR-NEXT:             update<i16, result=new>(%10, truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=undefined>(widen<i32, reason=promotion>(old<i16>), const<i32>(2))));
// IR-NEXT:         }
// IR-NEXT:         return read<i32>(%9);
// IR-NEXT:     }
// IR-NEXT:     fn %11 @address(%12 x: i32) -> i32 [linkage=external] {
// IR-NEXT:         let %13 p: @type15 [storage=automatic] = pointer_cast<@type15, reason=assign>(addr_of<@type2>(%12));
// IR-NEXT:         store<i32>(deref(read<@type15>(%13)), const<i32>(97));
// IR-NEXT:         return read<i32>(deref(read<@type15>(%13)));
// IR-NEXT:     }
// IR-NEXT:     fn %14 @casts(%15 d: f64, %16 u: u64) -> f32 [linkage=external] {
// IR-NEXT:         return add<f32, rounding=nearest_even, exceptions=ignore>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(widen<i32, reason=promotion>(float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f64>(%15)))), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(read<u64>(%16)));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
