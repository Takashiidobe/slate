struct barstruct {
  char const *some_string;
} x;
extern void                    abort(void);
void __attribute__((noinline)) foo(void) {
  if (!x.some_string)
    abort();
}
void baz(int b) {
  struct barstruct  bar;
  struct barstruct *barptr;
  if (b)
    barptr = &bar;
  else {
    barptr = &x + 1;
    barptr = barptr - 1;
  }
  barptr->some_string = "Everything OK";
  foo();
  barptr->some_string = "Everything OK";
}
int main() {
  x.some_string = (void *)0;
  baz(0);
  if (!x.some_string)
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
// DEFAULT-NEXT:     type @type0 barstruct = struct {
// DEFAULT-NEXT:         field0 some_string: ptr<const i8>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     global %1 x: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %9 .str9: array<i8, 14> [storage=static] = code_units<array<i8, 14>>([69, 118, 101, 114, 121, 116, 104, 105, 110, 103, 32, 79, 75, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %10 .str10: array<i8, 14> [storage=static] = code_units<array<i8, 14>>([69, 118, 101, 114, 121, 116, 104, 105, 110, 103, 32, 79, 75, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %2 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @foo() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if not<bool>(ne<ptr<const i8>>(read<ptr<const i8>>(field0(%1)), null<ptr<const i8>>))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @baz(%5 b: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %6 bar: @type0 [storage=automatic];
// DEFAULT-NEXT:         let %7 barptr: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%5), const<i32>(0))
// DEFAULT-NEXT:             write<ptr<@type0>>(%7, addr_of<ptr<@type0>>(%6));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<ptr<@type0>>(%7, ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(addr_of<ptr<@type0>>(%1), const<i32>(1)));
// DEFAULT-NEXT:                 write<ptr<@type0>>(%7, ptr_offset<ptr<@type0>, subtract=true, element=@type0, overflow=ub>(read<ptr<@type0>>(%7), const<i32>(1)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<ptr<const i8>>(field0(deref(read<ptr<@type0>>(%7))), pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(14)>(%9)));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         write<ptr<const i8>>(field0(deref(read<ptr<@type0>>(%7))), pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(14)>(%10)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<ptr<const i8>>(field0(%1), null<ptr<const i8>>);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%4, const<i32>(0));
// DEFAULT-NEXT:         if not<bool>(ne<ptr<const i8>>(read<ptr<const i8>>(field0(%1)), null<ptr<const i8>>))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
