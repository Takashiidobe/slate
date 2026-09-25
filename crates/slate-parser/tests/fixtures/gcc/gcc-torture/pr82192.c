/* PR rtl-optimization/82192 */

unsigned long long int a = 0x95dd3d896f7422e2ULL;
struct S {
  unsigned int m : 13;
} b;

__attribute__((noinline, noclone)) void foo(void) {
  b.m = ((unsigned)a) >>
        (0x644eee9667723bf7LL | a & ~0xdee27af8U) - 0x644eee9667763bd8LL;
}

int main() {
  if (__INT_MAX__ != 0x7fffffffULL)
    return 0;
  foo();
  if (b.m != 0)
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
// DEFAULT-NEXT:         field0 m: u32 : 13;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0], bit_offsets=[Some(0)], bit_units=[(0, 2)], field_units=[Some(0)]];
// DEFAULT-NEXT:     global %0 a: u64 [storage=static] = const<u64>(10798855141994013410) [linkage=external];
// DEFAULT-NEXT:     global %2 b: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %3 @foo() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..2, bits=0..13>(%2), shr<u32, amount_out_of_range=ub, fill=zero_extend>(truncate<u32, reason=explicit, fits=unknown>(read<u64>(%0)), sub<u64, overflow=wrap>(or<u64>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(7227976781724269559)), and<u64>(read<u64>(%0), widen<u64, reason=usual_arith>(not<u32>(const<u32>(3739384568))))), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(7227976781724531672)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2147483647))), const<u64>(2147483647))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..2, bits=0..13>(%2))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
