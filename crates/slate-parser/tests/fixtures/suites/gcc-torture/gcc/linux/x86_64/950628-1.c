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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 hours: i8;
// DEFAULT-NEXT:         field1 day: i8;
// DEFAULT-NEXT:         field2 month: i8;
// DEFAULT-NEXT:         field3 year: i16;
// DEFAULT-NEXT:     } [size=6, align=2, offsets=[0, 1, 2, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_T:[0-9]+]] T = @type[[TYPE0]];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_g:[0-9]+]] @g() -> @type[[TYPE0]] [linkage=external] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_now:[0-9]+]] now: @type[[TYPE0]] [storage=automatic];
// DEFAULT-NEXT:         write<i8>(field0(%[[VALUE_now]]), truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(field1(%[[VALUE_now]]), truncate<i8, reason=assign, fits=always>(const<i32>(2)));
// DEFAULT-NEXT:         write<i8>(field2(%[[VALUE_now]]), truncate<i8, reason=assign, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         write<i16>(field3(%[[VALUE_now]]), truncate<i16, reason=assign, fits=always>(const<i32>(4)));
// DEFAULT-NEXT:         return copy<@type[[TYPE0]], reason=return>(read<@type[[TYPE0]]>(%[[VALUE_now]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f() -> @type[[TYPE0]] [linkage=external] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_virk:[0-9]+]] virk: @type[[TYPE0]] [storage=automatic];
// DEFAULT-NEXT:         write<@type[[TYPE0]]>(%[[VALUE_virk]], copy<@type[[TYPE0]], reason=assign>(call<@type[[TYPE0]], signature=fn() -> @type[[TYPE0]], abi=sysv64() -> native_c>(%[[VALUE_g]])));
// DEFAULT-NEXT:         return copy<@type[[TYPE0]], reason=return>(read<@type[[TYPE0]]>(%[[VALUE_virk]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(field0(temporary %[[VALUE2:[0-9]+]] = call<@type[[TYPE0]], signature=fn() -> @type[[TYPE0]], abi=sysv64() -> native_c>(%[[VALUE_f]])))), const<i32>(1))
// DEFAULT-NEXT:             write<bool>(%[[VALUE1]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE1]], ne<i32>(widen<i32, reason=promotion>(read<i8>(field1(temporary %[[VALUE3:[0-9]+]] = call<@type[[TYPE0]], signature=fn() -> @type[[TYPE0]], abi=sysv64() -> native_c>(%[[VALUE_f]])))), const<i32>(2)));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE1]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE4]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE4]], ne<i32>(widen<i32, reason=promotion>(read<i8>(field2(temporary %[[VALUE5:[0-9]+]] = call<@type[[TYPE0]], signature=fn() -> @type[[TYPE0]], abi=sysv64() -> native_c>(%[[VALUE_f]])))), const<i32>(3)));
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE4]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE6]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE6]], ne<i32>(widen<i32, reason=promotion>(read<i16>(field3(temporary %[[VALUE7:[0-9]+]] = call<@type[[TYPE0]], signature=fn() -> @type[[TYPE0]], abi=sysv64() -> native_c>(%[[VALUE_f]])))), const<i32>(4)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE6]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
