void abort(void);
void exit(int);

typedef struct {
  char  hours, day, month;
  short year;
} T;

T g(void) {
  T now;

  now.hours = 1;
  now.day   = 2;
  now.month = 3;
  now.year  = 4;
  return now;
}

T f(void) {
  T virk;

  virk = g();
  return virk;
}

int main(void) {
  if (f().hours != 1 || f().day != 2 || f().month != 3 || f().year != 4)
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
// DEFAULT-NEXT:     type @type0 = struct {
// DEFAULT-NEXT:         field0 hours: i8;
// DEFAULT-NEXT:         field1 day: i8;
// DEFAULT-NEXT:         field2 month: i8;
// DEFAULT-NEXT:         field3 year: i16;
// DEFAULT-NEXT:     } [size=6, align=2, offsets=[0, 1, 2, 4]];
// DEFAULT-NEXT:     type @type1 T = @type0;
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%9 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %4 @g() -> @type0 [linkage=external] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %5 now: @type0 [storage=automatic];
// DEFAULT-NEXT:         write<i8>(field0(%5), truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(field1(%5), truncate<i8, reason=assign, fits=always>(const<i32>(2)));
// DEFAULT-NEXT:         write<i8>(field2(%5), truncate<i8, reason=assign, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         write<i16>(field3(%5), truncate<i16, reason=assign, fits=always>(const<i32>(4)));
// DEFAULT-NEXT:         return copy<@type0, reason=return>(read<@type0>(%5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @f() -> @type0 [linkage=external] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %7 virk: @type0 [storage=automatic];
// DEFAULT-NEXT:         write<@type0>(%7, copy<@type0, reason=assign>(call<@type0, signature=fn() -> @type0, abi=sysv64() -> native_c>(%4)));
// DEFAULT-NEXT:         copy<@type0, reason=assign>(call<@type0, signature=fn() -> @type0, abi=sysv64() -> native_c>(%4));
// DEFAULT-NEXT:         return copy<@type0, reason=return>(read<@type0>(%7));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %14: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(field0(temporary %10 = call<@type0, signature=fn() -> @type0, abi=sysv64() -> native_c>(%6)))), const<i32>(1))
// DEFAULT-NEXT:             write<bool>(%14, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%14, ne<i32>(widen<i32, reason=promotion>(read<i8>(field1(temporary %11 = call<@type0, signature=fn() -> @type0, abi=sysv64() -> native_c>(%6)))), const<i32>(2)));
// DEFAULT-NEXT:         let %15: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%14)
// DEFAULT-NEXT:             write<bool>(%15, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%15, ne<i32>(widen<i32, reason=promotion>(read<i8>(field2(temporary %12 = call<@type0, signature=fn() -> @type0, abi=sysv64() -> native_c>(%6)))), const<i32>(3)));
// DEFAULT-NEXT:         let %16: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%15)
// DEFAULT-NEXT:             write<bool>(%16, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%16, ne<i32>(widen<i32, reason=promotion>(read<i16>(field3(temporary %13 = call<@type0, signature=fn() -> @type0, abi=sysv64() -> native_c>(%6)))), const<i32>(4)));
// DEFAULT-NEXT:         if read<bool>(%16)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
