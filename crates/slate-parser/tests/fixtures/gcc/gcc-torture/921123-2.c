void abort(void);
void exit(int);

typedef struct {
  unsigned short b0, b1, b2, b3;
} four_quarters;

four_quarters x;
int           a, b;

void f(four_quarters j) {
  b = j.b2;
  a = j.b3;
}

int main(void) {
  four_quarters x;
  x.b0 = x.b1 = x.b2 = 0;
  x.b3               = 38;
  f(x);
  if (a != 38)
    abort();
  exit(0);
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
// DEFAULT-NEXT:     type @type0 = struct {
// DEFAULT-NEXT:         field0 b0: u16;
// DEFAULT-NEXT:         field1 b1: u16;
// DEFAULT-NEXT:         field2 b2: u16;
// DEFAULT-NEXT:         field3 b3: u16;
// DEFAULT-NEXT:     } [size=8, align=2, offsets=[0, 2, 4, 6]];
// DEFAULT-NEXT:     type @type1 four_quarters = @type0;
// DEFAULT-NEXT:     global %4 x: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 a: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%11 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %7 @f(%8 j: @type0) -> void [linkage=external] [abi=sysv64(coerce<i64>) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(%6, reinterpret<i32, reason=assign, fits=unknown>(widen<u32, reason=assign>(read<u16>(field2(%8)))));
// DEFAULT-NEXT:         write<i32>(%5, reinterpret<i32, reason=assign, fits=unknown>(widen<u32, reason=assign>(read<u16>(field3(%8)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %10 x: @type0 [storage=automatic];
// DEFAULT-NEXT:         write<u16>(field2(%10), reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         write<u16>(field1(%10), reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         write<u16>(field0(%10), reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         write<u16>(field3(%10), reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(38))));
// DEFAULT-NEXT:         call<void, signature=fn(@type0) -> void, abi=sysv64(coerce<i64>) -> void>(%7, copy<@type0, reason=arg>(read<@type0>(%10)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%5), const<i32>(38))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
