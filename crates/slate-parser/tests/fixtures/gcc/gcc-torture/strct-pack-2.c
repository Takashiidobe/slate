void abort(void);
void exit(int);

typedef struct {
  short  a __attribute__((aligned(2), packed));
  short *ap[2] __attribute__((aligned(2), packed));
} A;

int main(void) {
  short i, j   = 1;
  A     a, *ap = &a;
  ap->ap[j] = &i;
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
// DEFAULT-NEXT:         field0 a: i16;
// DEFAULT-NEXT:         field1 ap: array<ptr<i16>, 2>;
// DEFAULT-NEXT:     } [size=18, align=2, offsets=[0, 2]];
// DEFAULT-NEXT:     type @type1 A = @type0;
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%9 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %5 i: i16 [storage=automatic];
// DEFAULT-NEXT:         let %6 j: i16 [storage=automatic] = truncate<i16, reason=assign, fits=always>(const<i32>(1));
// DEFAULT-NEXT:         let %7 a: @type0 [storage=automatic];
// DEFAULT-NEXT:         let %8 ap: ptr<@type0> [storage=automatic] = addr_of<ptr<@type0>>(%7);
// DEFAULT-NEXT:         write<ptr<i16>>(deref(ptr_offset<ptr<ptr<i16>>, subtract=false, element=ptr<i16>, overflow=ub>(array_decay<ptr<ptr<i16>>, length=Some(2)>(field1(deref(read<ptr<@type0>>(%8)))), widen<i32, reason=promotion>(read<i16>(%6)))), addr_of<ptr<i16>>(%5));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
