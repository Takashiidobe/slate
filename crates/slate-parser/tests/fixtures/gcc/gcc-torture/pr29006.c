struct __attribute__((__packed__)) s {
  char               c;
  unsigned long long x;
};
void __attribute__((__noinline__)) foo(struct s *s) { s->x = 0; }
int                                main(void) {
  struct s s = {1, ~0ULL};
  foo(&s);
  return s.x != 0;
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
// DEFAULT-NEXT:     type @type0 s = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 x: u64;
// DEFAULT-NEXT:     } [size=9, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     fn %1 @foo(%2 s: ptr<@type0>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<u64>(field1(deref(read<ptr<@type0>>(%2))), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %4 s: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), field1 = not<u64>(const<u64>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>) -> void>(%1, addr_of<ptr<@type0>>(%4));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<u64>(read<u64>(field1(%4)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
