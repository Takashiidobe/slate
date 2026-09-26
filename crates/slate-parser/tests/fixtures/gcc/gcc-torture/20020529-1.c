/* PR target/6838 from cato@df.lth.se.
   cris-elf got an ICE with -O2: the insn matching
      (insn 49 48 52 (parallel[
                  (set (mem/s:HI (plus:SI (reg/v/f:SI 0 r0 [24])
                              (const_int 8 [0x8])) [5 <variable>.c+0 S2 A8])
                      (reg:HI 2 r2 [27]))
                  (set (reg/f:SI 2 r2 [31])
                      (plus:SI (reg/v/f:SI 0 r0 [24])
                          (const_int 8 [0x8])))
              ] ) 24 {*mov_sidehi_mem} (nil)
          (nil))
   forced a splitter through the output pattern "#", but there was no
   matching splitter.  */

void abort(void);
void exit(int);

struct xx {
  int        a;
  struct xx *b;
  short      c;
};

int  f1(struct xx *);
void f2(void);

int foo(struct xx *p, int b, int c, int d) {
  int a;

  for (;;) {
    a = f1(p);
    if (a)
      return (0);
    if (b)
      continue;
    p->c = d;
    if (p->a)
      f2();
    if (c)
      f2();
    d = p->c;
    switch (a) {
    case 1:
      if (p->b)
        f2();
      if (c)
        f2();
    default:
      break;
    }
  }
  return d;
}

int main(void) {
  struct xx s = {0, &s, 23};
  if (foo(&s, 0, 0, 0) != 0 || s.a != 0 || s.b != &s || s.c != 0)
    abort();
  exit(0);
}

int f1(struct xx *p) {
  static int beenhere = 0;
  if (beenhere++ > 1)
    abort();
  return beenhere > 1;
}

void f2(void) { abort(); }


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
// DEFAULT-NEXT:     type @type0 xx = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: ptr<@type0>;
// DEFAULT-NEXT:         field2 c: i16;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     global %14 beenhere: i32 [storage=static] = const<i32>(0) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%15 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @f1(%13 p: ptr<@type0>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %19: i32 [synthetic] = read<i32>(%14);
// DEFAULT-NEXT:         let %20: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%19), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%14, read<i32>(%20));
// DEFAULT-NEXT:         if gt<i32>(read<i32>(%19), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(gt<i32>(read<i32>(%14), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @f2() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @foo(%6 p: ptr<@type0>, %7 b: i32, %8 c: i32, %9 d: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %10 a: i32 [storage=automatic];
// DEFAULT-NEXT:         for %17
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: omitted
// DEFAULT-NEXT:             increment: omitted
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(%10, call<i32, signature=fn(ptr<@type0>) -> i32>(%3, read<ptr<@type0>>(%6)));
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<@type0>) -> i32>(%3, read<ptr<@type0>>(%6));
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%10), const<i32>(0))
// DEFAULT-NEXT:                         return const<i32>(0);
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%7), const<i32>(0))
// DEFAULT-NEXT:                         continue %17;
// DEFAULT-NEXT:                     write<i16>(field2(deref(read<ptr<@type0>>(%6))), truncate<i16, reason=assign, fits=unknown>(read<i32>(%9)));
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(field0(deref(read<ptr<@type0>>(%6)))), const<i32>(0))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%8), const<i32>(0))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:                     write<i32>(%9, widen<i32, reason=assign>(read<i16>(field2(deref(read<ptr<@type0>>(%6))))));
// DEFAULT-NEXT:                     switch %18 read<i32>(%10)
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             case %18 const<i32>(1):
// DEFAULT-NEXT:                                 if ne<ptr<@type0>>(read<ptr<@type0>>(field1(deref(read<ptr<@type0>>(%6)))), null<ptr<@type0>>)
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:                             if ne<i32>(read<i32>(%8), const<i32>(0))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:                             default %18:
// DEFAULT-NEXT:                                 break %18;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<i32>(%9);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %12 s: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = const<i32>(0), field1 = addr_of<ptr<@type0>>(%12), field2 = truncate<i16, reason=assign, fits=always>(const<i32>(23)));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(call<i32, signature=fn(ptr<@type0>, i32, i32, i32) -> i32>(%5, addr_of<ptr<@type0>>(%12), const<i32>(0), const<i32>(0), const<i32>(0)), const<i32>(0)), ne<i32>(read<i32>(field0(%12)), const<i32>(0))), ne<ptr<@type0>>(read<ptr<@type0>>(field1(%12)), addr_of<ptr<@type0>>(%12))), ne<i32>(widen<i32, reason=promotion>(read<i16>(field2(%12))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
