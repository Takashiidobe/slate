// SLATE-FILECHECK-DEFINES DEFAULT

extern double foo(double, double);
extern void bar(float*, int*);

void
baz(int* arg)
{
    float tmp = (float)foo(2.0,1.0);
    unsigned i;
    short junk[64];

    for (i=0; i<10; i++, arg++) {
        bar(&tmp, arg);
    }
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
// DEFAULT-NEXT:     fn %0 @foo(%7 <unnamed>: f64, %8 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %1 @bar(%9 <unnamed>: ptr<f32>, %10 <unnamed>: ptr<i32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @baz(%3 arg: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %4 tmp: f32 [storage=automatic] = float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%0, const<f64>(2.0), const<f64>(1.0)));
// DEFAULT-NEXT:         let %5 i: u32 [storage=automatic];
// DEFAULT-NEXT:         let %6 junk: array<i16, 64> [storage=automatic] [align=16];
// DEFAULT-NEXT:         for %11
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u32>(%5, reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             condition: lt<u32>(read<u32>(%5), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(10)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %12: u32 [synthetic] = read<u32>(%5);
// DEFAULT-NEXT:                 let %13: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%12), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<u32>(%5, read<u32>(%13));
// DEFAULT-NEXT:                 let %14: ptr<i32> [synthetic] = read<ptr<i32>>(%3);
// DEFAULT-NEXT:                 let %15: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%14), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i32>>(%3, read<ptr<i32>>(%15));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn(ptr<f32>, ptr<i32>) -> void>(%1, addr_of<ptr<f32>>(%4), read<ptr<i32>>(%3));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
