/* PR optimization/13394 */
/* Origin: Carlo Wood <carlo@gcc.gnu.org> */

/* Verify that a bogus "function does return" warning is not issued
   in presence of tail recursion within a noreturn function.  */

/* { dg-do compile } */
/* { dg-options "-O2 -Wreturn-type -Wmissing-noreturn -fno-ipa-icf" } */


void f(void) __attribute__ ((__noreturn__));
void _exit(int status) __attribute__ ((__noreturn__));

int z = 0;

void g() /* { dg-warning "might be candidate" } */
{
  if (++z > 10)
    _exit(0);
  g();
}

void f()
{
  if (++z > 10)
    _exit(0);
  f();
}             /* { dg-bogus "does return" } */

int h() /* { dg-warning "might be candidate" } */
{
  if (++z > 10)
    _exit(0);
  return h();
}             /* { dg-bogus "end of non-void function" } */

int k() /* { dg-warning "might be candidate" } */
{
  if (++z > 10)
    _exit(0);
  k();
}             /* { dg-warning "control reaches" } */

// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     global %3 z: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     fn %0 @f() -> void [linkage=external] [noreturn] [fallthrough=ub] {
// DEFAULT-NEXT:         let %8: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:         let %9: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%8), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%3, read<i32>(%9));
// DEFAULT-NEXT:         if gt<i32>(read<i32>(%9), const<i32>(10))
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%2, const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @_exit(%7 status: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %4 @g() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %10: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:         let %11: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%10), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%3, read<i32>(%11));
// DEFAULT-NEXT:         if gt<i32>(read<i32>(%11), const<i32>(10))
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%2, const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @h() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %12: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:         let %13: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%12), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%3, read<i32>(%13));
// DEFAULT-NEXT:         if gt<i32>(read<i32>(%13), const<i32>(10))
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%2, const<i32>(0));
// DEFAULT-NEXT:         return call<i32, signature=fn() -> i32>(%5);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @k() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %14: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:         let %15: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%14), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%3, read<i32>(%15));
// DEFAULT-NEXT:         if gt<i32>(read<i32>(%15), const<i32>(10))
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%2, const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%6);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
