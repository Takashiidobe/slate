/* PR48985 */
/* { dg-do run } */
/* { dg-options "-std=gnu89" } */
/* { dg-skip-if "packed attribute missing for struct s" { "epiphany-*-*" } } */

extern void abort (void);

struct s {
    int i;
    char c[];
} s = { 1, "01234" };

__SIZE_TYPE__ f (void) { return __builtin_object_size (&s.c, 0); }

int
main()
{
  if (f() != sizeof ("01234"))
    abort ();

  return 0;
}

// SLATE-FILECHECK-STD DEFAULT gnu89
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
// DEFAULT-NEXT:     type @type0 s = struct {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:         field1 c: array<i8, incomplete>;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     global %2 s: @type0 [storage=static] = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1), field1 = code_units<array<i8, 6>>([48, 49, 50, 51, 52, 0])) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %7 @__builtin_object_size(%5 <unnamed>: ptr<const void>, %6 <unnamed>: i32) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %3 @f() -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<u64, signature=fn(ptr<const void>, i32) -> u64>(%7, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<array<i8, incomplete>>>(field1(%2))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @main(unprototyped) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn() -> u64>(%3), const<u64>(6))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
