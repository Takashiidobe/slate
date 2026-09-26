/* powerpc64-linux gcc miscompiled this due to rs6000.c:expand_block_move
   not setting mem aliasing info correctly for the code implementing the
   structure assignment.  */

struct termios {
  unsigned int  a;
  unsigned int  b;
  unsigned int  c;
  unsigned int  d;
  unsigned char pad[28];
};

struct tty_driver {
  unsigned char  pad1[38];
  struct termios t __attribute__((aligned(8)));
};

static struct termios    zero_t;
static struct tty_driver pty;

void ini(void) {
  pty.t   = zero_t;
  pty.t.a = 1;
  pty.t.b = 2;
  pty.t.c = 3;
  pty.t.d = 4;
}

int main(void) {
  extern void abort(void);

  ini();
  if (pty.t.a != 1 || pty.t.b != 2 || pty.t.c != 3 || pty.t.d != 4)
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
// DEFAULT-NEXT:     type @type0 termios = struct {
// DEFAULT-NEXT:         field0 a: u32;
// DEFAULT-NEXT:         field1 b: u32;
// DEFAULT-NEXT:         field2 c: u32;
// DEFAULT-NEXT:         field3 d: u32;
// DEFAULT-NEXT:         field4 pad: array<u8, 28>;
// DEFAULT-NEXT:     } [size=44, align=4, offsets=[0, 4, 8, 12, 16]];
// DEFAULT-NEXT:     type @type1 tty_driver = struct {
// DEFAULT-NEXT:         field0 pad1: array<u8, 38>;
// DEFAULT-NEXT:         field1 t: @type0;
// DEFAULT-NEXT:     } [size=88, align=8, offsets=[0, 40]];
// DEFAULT-NEXT:     global %2 zero_t: @type0 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %3 pty: @type1 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %4 @ini() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<@type0>(field1(%3), copy<@type0, reason=assign>(read<@type0>(%2)));
// DEFAULT-NEXT:         write<u32>(field0(field1(%3)), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(field1(field1(%3)), reinterpret<u32, reason=assign, fits=always>(const<i32>(2)));
// DEFAULT-NEXT:         write<u32>(field2(field1(%3)), reinterpret<u32, reason=assign, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         write<u32>(field3(field1(%3)), reinterpret<u32, reason=assign, fits=always>(const<i32>(4)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(read<u32>(field0(field1(%3))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), ne<u32>(read<u32>(field1(field1(%3))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2)))), ne<u32>(read<u32>(field2(field1(%3))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(3)))), ne<u32>(read<u32>(field3(field1(%3))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(4))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
