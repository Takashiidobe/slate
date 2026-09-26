/* PR target/17377
   Bug in code emitted by "return" pattern on CRIS: missing pop of
   forced return address on stack.  */
/* { dg-require-effective-target return_address } */
void abort(void);
void exit(int);

int calls = 0;

void *f(int) __attribute__((__noinline__));
void *f(int i) {
  /* The code does a little brittle song and dance to trig the "return"
     pattern instead of the function epilogue.  This must still be a
     leaf function for the bug to be exposed.  */

  if (calls++ == 0)
    return __builtin_return_address(0);

  switch (i) {
  case 1:
    return f;
  case 0:
    return __builtin_return_address(0);
  }
  return 0;
}

volatile int x;

void *y(int i) __attribute__((__noinline__, __noclone__));
void *y(int i) {
  x = 0;

  /* This must not be a sibling call: the return address must appear
     constant for different calls to this function.  Postincrementing x
     catches otherwise unidentified multiple returns (e.g. through the
     return-address register and then this epilogue popping the address
     stored on stack in "f").  */
  return (char *)f(i) + x++;
}

int main(void) {
  void *v = y(4);
  if (y(1) != f
      /* Can't reasonably check the validity of the return address
         above, but it's not that important: the test-case will probably
         crash on the first call to f with the bug present, or it will
         run wild including returning early (in y or here), so we also
         try and check the number of calls.  */
      || y(0) != v || y(3) != 0 || y(-1) != 0 || calls != 5)
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
// DEFAULT-NEXT:     global %2 calls: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %5 x: volatile i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%10 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @f(%4 i: i32) -> ptr<void> [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %14: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %15: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%14), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%15));
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%14), const<i32>(0))
// DEFAULT-NEXT:             return call<ptr<void>, signature=fn(u32) -> ptr<void>>(__builtin_return_address, reinterpret<u32, reason=arg, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         switch %12 read<i32>(%4)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %12 const<i32>(1):
// DEFAULT-NEXT:                     return pointer_cast<ptr<void>, reason=return>(function_decay<ptr<fn(i32) -> ptr<void>>>(%3));
// DEFAULT-NEXT:                 case %12 const<i32>(0):
// DEFAULT-NEXT:                     return call<ptr<void>, signature=fn(u32) -> ptr<void>>(__builtin_return_address, reinterpret<u32, reason=arg, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return null<ptr<void>>;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @y(%7 i: i32) -> ptr<void> [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32, volatile>(%5, const<i32>(0));
// DEFAULT-NEXT:         let %16: i32 [synthetic] = read<i32, volatile>(%5);
// DEFAULT-NEXT:         let %17: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%16), const<i32>(1));
// DEFAULT-NEXT:         write<i32, volatile>(%5, read<i32>(%17));
// DEFAULT-NEXT:         return pointer_cast<ptr<void>, reason=return>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(pointer_cast<ptr<i8>, reason=explicit>(call<ptr<void>, signature=fn(i32) -> ptr<void>>(%3, read<i32>(%7))), read<i32>(%16)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %9 v: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(i32) -> ptr<void>>(%6, const<i32>(4));
// DEFAULT-NEXT:         let %18: bool [synthetic];
// DEFAULT-NEXT:         if ne<ptr<void>>(call<ptr<void>, signature=fn(i32) -> ptr<void>>(%6, const<i32>(1)), pointer_cast<ptr<void>, reason=usual_arith>(function_decay<ptr<fn(i32) -> ptr<void>>>(%3)))
// DEFAULT-NEXT:             write<bool>(%18, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%18, ne<ptr<void>>(call<ptr<void>, signature=fn(i32) -> ptr<void>>(%6, const<i32>(0)), read<ptr<void>>(%9)));
// DEFAULT-NEXT:         let %19: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%18)
// DEFAULT-NEXT:             write<bool>(%19, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%19, ne<ptr<void>>(call<ptr<void>, signature=fn(i32) -> ptr<void>>(%6, const<i32>(3)), null<ptr<void>>));
// DEFAULT-NEXT:         let %20: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%19)
// DEFAULT-NEXT:             write<bool>(%20, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%20, ne<ptr<void>>(call<ptr<void>, signature=fn(i32) -> ptr<void>>(%6, neg<i32, overflow=ub>(const<i32>(1))), null<ptr<void>>));
// DEFAULT-NEXT:         if logical_or<bool>(read<bool>(%20), ne<i32>(read<i32>(%2), const<i32>(5)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
