/* PR middle-end/36137 */
extern void abort(void);

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

int main() {
  unsigned int u;
  int          i = -1;

  u = MAX((unsigned int)MAX(i, 0), 1);
  if (u != 1)
    abort();

  u = MIN((unsigned int)MAX(i, 0), (unsigned int)i);
  if (u != 0)
    abort();
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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %2 u: u32 [storage=automatic];
// DEFAULT-NEXT:         let %3 i: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:         write<u32>(%2, conditional<u32>(gt<u32>(reinterpret<u32, reason=explicit, fits=unknown>(conditional<i32>(gt<i32>(read<i32>(%3), const<i32>(0)), read<i32>(%3), const<i32>(0))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=explicit, fits=unknown>(conditional<i32>(gt<i32>(read<i32>(%3), const<i32>(0)), read<i32>(%3), const<i32>(0))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%2), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%2, conditional<u32>(lt<u32>(reinterpret<u32, reason=explicit, fits=unknown>(conditional<i32>(gt<i32>(read<i32>(%3), const<i32>(0)), read<i32>(%3), const<i32>(0))), reinterpret<u32, reason=explicit, fits=unknown>(read<i32>(%3))), reinterpret<u32, reason=explicit, fits=unknown>(conditional<i32>(gt<i32>(read<i32>(%3), const<i32>(0)), read<i32>(%3), const<i32>(0))), reinterpret<u32, reason=explicit, fits=unknown>(read<i32>(%3))));
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%2), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
