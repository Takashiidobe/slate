void abort(void);
void exit(int);

struct s {
  _Complex unsigned short x;
};
struct s                           gs = {100 + 200i};
struct s __attribute__((noinline)) foo(void) { return gs; }

int main() {
  if (foo().x != gs.x)
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
// DEFAULT-NEXT:     type @type[[TYPE_s:[0-9]+]] s = struct {
// DEFAULT-NEXT:         field0 x: complex<u16>;
// DEFAULT-NEXT:     } [size=4, align=2, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_gs:[0-9]+]] gs: @type[[TYPE_s]] [storage=static] = aggregate<@type[[TYPE_s]], zero_fill=false>(field0 = complex_convert<complex<u16>, reason=assign, fits=unknown>(add<complex<i32>, complex=true, overflow=ub>(const<i32>(100), aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(200))))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> @type[[TYPE_s]] [linkage=external] [inline=never] [definition=emitted] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type[[TYPE_s]], reason=return>(read<@type[[TYPE_s]]>(%[[VALUE_gs]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<complex<i32>>(complex_convert<complex<i32>, reason=usual_arith, fits=unknown>(read<complex<u16>>(field0(temporary %[[VALUE1:[0-9]+]] = call<@type[[TYPE_s]], signature=fn() -> @type[[TYPE_s]], abi=sysv64() -> native_c>(%[[VALUE_foo]])))), complex_convert<complex<i32>, reason=usual_arith, fits=unknown>(read<complex<u16>>(field0(%[[VALUE_gs]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
