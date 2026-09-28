/* This used to fail as we would convert f into just return (unsigned
   int)usVlanID which is wrong. */

int f(unsigned short usVlanID) __attribute__((noinline, noclone));
int f(unsigned short usVlanID) {
  unsigned int uiVlanID = 0xffffffff;
  int          i;
  if ((unsigned short)0xffff != usVlanID)
    uiVlanID = (unsigned int)usVlanID;
  return uiVlanID;
}

int main(void) {
  if (f(1) != 1)
    __builtin_abort();
  if (f(0xffff) != -1)
    __builtin_abort();
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
// DEFAULT-NEXT:     fn %1 @f(%2 usVlanID: u16) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 uiVlanID: u32 [storage=automatic] = const<u32>(4294967295);
// DEFAULT-NEXT:         let %4 i: i32 [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(65535))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%2))))
// DEFAULT-NEXT:             write<u32>(%3, widen<u32, reason=explicit>(read<u16>(%2)));
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(read<u32>(%3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u16) -> i32>(%1, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(1)))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%7);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u16) -> i32>(%1, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535)))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%7);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
