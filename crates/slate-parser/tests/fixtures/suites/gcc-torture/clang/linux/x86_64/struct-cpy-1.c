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
// DEFAULT-NEXT:     type @type[[TYPE_termios:[0-9]+]] termios = struct {
// DEFAULT-NEXT:         field0 a: u32;
// DEFAULT-NEXT:         field1 b: u32;
// DEFAULT-NEXT:         field2 c: u32;
// DEFAULT-NEXT:         field3 d: u32;
// DEFAULT-NEXT:         field4 pad: array<u8, 28>;
// DEFAULT-NEXT:     } [size=44, align=4, offsets=[0, 4, 8, 12, 16]];
// DEFAULT-NEXT:     type @type[[TYPE_tty_driver:[0-9]+]] tty_driver = struct {
// DEFAULT-NEXT:         field0 pad1: array<u8, 38>;
// DEFAULT-NEXT:         field1 t: @type[[TYPE_termios]];
// DEFAULT-NEXT:     } [size=88, align=8, offsets=[0, 40]];
// DEFAULT-NEXT:     global %[[VALUE_zero_t:[0-9]+]] zero_t: @type[[TYPE_termios]] [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_pty:[0-9]+]] pty: @type[[TYPE_tty_driver]] [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_ini:[0-9]+]] @ini() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<@type[[TYPE_termios]]>(field1(%[[VALUE_pty]]), copy<@type[[TYPE_termios]], reason=assign>(read<@type[[TYPE_termios]]>(%[[VALUE_zero_t]])));
// DEFAULT-NEXT:         write<u32>(field0(field1(%[[VALUE_pty]])), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(field1(field1(%[[VALUE_pty]])), reinterpret<u32, reason=assign, fits=always>(const<i32>(2)));
// DEFAULT-NEXT:         write<u32>(field2(field1(%[[VALUE_pty]])), reinterpret<u32, reason=assign, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         write<u32>(field3(field1(%[[VALUE_pty]])), reinterpret<u32, reason=assign, fits=always>(const<i32>(4)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_ini]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(read<u32>(field0(field1(%[[VALUE_pty]]))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), ne<u32>(read<u32>(field1(field1(%[[VALUE_pty]]))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2)))), ne<u32>(read<u32>(field2(field1(%[[VALUE_pty]]))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(3)))), ne<u32>(read<u32>(field3(field1(%[[VALUE_pty]]))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(4))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
