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
// DEFAULT-NEXT:     type @type[[TYPE_xx:[0-9]+]] xx = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: ptr<@type[[TYPE_xx]]>;
// DEFAULT-NEXT:         field2 c: i16;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     global %[[VALUE_beenhere:[0-9]+]] beenhere: i32 [storage=static] = const<i32>(0) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_f1:[0-9]+]] @f1(%[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_xx]]>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_beenhere]]);
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_beenhere]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:         if gt<i32>(read<i32>(%[[VALUE1]]), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(gt<i32>(read<i32>(%[[VALUE_beenhere]]), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f2:[0-9]+]] @f2() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_p_2:[0-9]+]] p: ptr<@type[[TYPE_xx]]>, %[[VALUE_b:[0-9]+]] b: i32, %[[VALUE_c:[0-9]+]] c: i32, %[[VALUE_d:[0-9]+]] d: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: omitted
// DEFAULT-NEXT:             increment: omitted
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_a]], call<i32, signature=fn(ptr<@type[[TYPE_xx]]>) -> i32>(%[[VALUE_f1]], read<ptr<@type[[TYPE_xx]]>>(%[[VALUE_p_2]])));
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%[[VALUE_a]]), const<i32>(0))
// DEFAULT-NEXT:                         return const<i32>(0);
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%[[VALUE_b]]), const<i32>(0))
// DEFAULT-NEXT:                         continue %[[VALUE3]];
// DEFAULT-NEXT:                     write<i16>(field2(deref(read<ptr<@type[[TYPE_xx]]>>(%[[VALUE_p_2]]))), truncate<i16, reason=assign, fits=unknown>(read<i32>(%[[VALUE_d]])));
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(field0(deref(read<ptr<@type[[TYPE_xx]]>>(%[[VALUE_p_2]])))), const<i32>(0))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_f2]]);
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%[[VALUE_c]]), const<i32>(0))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_f2]]);
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_d]], widen<i32, reason=assign>(read<i16>(field2(deref(read<ptr<@type[[TYPE_xx]]>>(%[[VALUE_p_2]]))))));
// DEFAULT-NEXT:                     switch %[[VALUE4:[0-9]+]] read<i32>(%[[VALUE_a]])
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             case %[[VALUE4]] const<i32>(1):
// DEFAULT-NEXT:                                 if ne<ptr<@type[[TYPE_xx]]>>(read<ptr<@type[[TYPE_xx]]>>(field1(deref(read<ptr<@type[[TYPE_xx]]>>(%[[VALUE_p_2]])))), null<ptr<@type[[TYPE_xx]]>>)
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_f2]]);
// DEFAULT-NEXT:                             if ne<i32>(read<i32>(%[[VALUE_c]]), const<i32>(0))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE_f2]]);
// DEFAULT-NEXT:                             default %[[VALUE4]]:
// DEFAULT-NEXT:                                 break %[[VALUE4]];
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_d]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_s:[0-9]+]] s: @type[[TYPE_xx]] [storage=automatic] = aggregate<@type[[TYPE_xx]], zero_fill=false>(field0 = const<i32>(0), field1 = addr_of<ptr<@type[[TYPE_xx]]>>(%[[VALUE_s]]), field2 = truncate<i16, reason=assign, fits=always>(const<i32>(23)));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(call<i32, signature=fn(ptr<@type[[TYPE_xx]]>, i32, i32, i32) -> i32>(%[[VALUE_foo]], addr_of<ptr<@type[[TYPE_xx]]>>(%[[VALUE_s]]), const<i32>(0), const<i32>(0), const<i32>(0)), const<i32>(0)), ne<i32>(read<i32>(field0(%[[VALUE_s]])), const<i32>(0))), ne<ptr<@type[[TYPE_xx]]>>(read<ptr<@type[[TYPE_xx]]>>(field1(%[[VALUE_s]])), addr_of<ptr<@type[[TYPE_xx]]>>(%[[VALUE_s]]))), ne<i32>(widen<i32, reason=promotion>(read<i16>(field2(%[[VALUE_s]]))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
