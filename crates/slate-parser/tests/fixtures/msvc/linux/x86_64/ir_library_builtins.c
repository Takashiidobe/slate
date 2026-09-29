
void exit(int);
int toupper(int);
void quit(int);

int plain(int x) {
  if (x)
    exit(1);
  return toupper(x) + toupper(x);
}

int before_declspec(int x) {
  if (x)
    quit(2);
  return x + 1;
}

__declspec(noreturn) void quit(int);

void undeclared(void) { abort(); }

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
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_toupper:[0-9]+]] @toupper(%[[VALUE1:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_quit:[0-9]+]] @quit(%[[VALUE2:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_plain:[0-9]+]] @plain(%[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(1));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_toupper]], read<i32>(%[[VALUE_x]])), call<i32, signature=fn(i32) -> i32>(%[[VALUE_toupper]], read<i32>(%[[VALUE_x]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_before_declspec:[0-9]+]] @before_declspec(%[[VALUE_x_2:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_x_2]]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%[[VALUE_quit]], const<i32>(2));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%[[VALUE_x_2]]), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_undeclared:[0-9]+]] @undeclared() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
