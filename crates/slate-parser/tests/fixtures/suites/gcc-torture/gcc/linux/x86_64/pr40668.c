#if (__SIZEOF_INT__ == 2)
#define TESTVALUE 0x1234
#else
#define TESTVALUE 0x12345678
#endif
static void foo(unsigned int x, void *p) { __builtin_memcpy(p, &x, sizeof x); }

void bar(int type, void *number) {
  switch (type) {
  case 1:
    foo(TESTVALUE, number);
    break;
  case 7:
    foo(0, number);
    break;
  case 8:
    foo(0, number);
    break;
  case 9:
    foo(0, number);
    break;
  }
}

int main(void) {
  unsigned int x;
  bar(1, &x);
  if (x != TESTVALUE)
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
// DEFAULT-NEXT:     fn %[[VALUE___builtin_memcpy:[0-9]+]] @__builtin_memcpy(%[[VALUE0:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE2:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: u32, %[[VALUE_p:[0-9]+]] p: ptr<void>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE___builtin_memcpy]], read<ptr<void>>(%[[VALUE_p]]), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<u32>>(%[[VALUE_x]])), const<u64>(4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_type:[0-9]+]] type: i32, %[[VALUE_number:[0-9]+]] number: ptr<void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         switch %[[VALUE3:[0-9]+]] read<i32>(%[[VALUE_type]])
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %[[VALUE3]] const<i32>(1):
// DEFAULT-NEXT:                     call<void, signature=fn(u32, ptr<void>) -> void>(%[[VALUE_foo]], reinterpret<u32, reason=arg, fits=always>(const<i32>(305419896)), read<ptr<void>>(%[[VALUE_number]]));
// DEFAULT-NEXT:                 break %[[VALUE3]];
// DEFAULT-NEXT:                 case %[[VALUE3]] const<i32>(7):
// DEFAULT-NEXT:                     call<void, signature=fn(u32, ptr<void>) -> void>(%[[VALUE_foo]], reinterpret<u32, reason=arg, fits=always>(const<i32>(0)), read<ptr<void>>(%[[VALUE_number]]));
// DEFAULT-NEXT:                 break %[[VALUE3]];
// DEFAULT-NEXT:                 case %[[VALUE3]] const<i32>(8):
// DEFAULT-NEXT:                     call<void, signature=fn(u32, ptr<void>) -> void>(%[[VALUE_foo]], reinterpret<u32, reason=arg, fits=always>(const<i32>(0)), read<ptr<void>>(%[[VALUE_number]]));
// DEFAULT-NEXT:                 break %[[VALUE3]];
// DEFAULT-NEXT:                 case %[[VALUE3]] const<i32>(9):
// DEFAULT-NEXT:                     call<void, signature=fn(u32, ptr<void>) -> void>(%[[VALUE_foo]], reinterpret<u32, reason=arg, fits=always>(const<i32>(0)), read<ptr<void>>(%[[VALUE_number]]));
// DEFAULT-NEXT:                 break %[[VALUE3]];
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_x_2:[0-9]+]] x: u32 [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<void>) -> void>(%[[VALUE_bar]], const<i32>(1), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<u32>>(%[[VALUE_x_2]])));
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%[[VALUE_x_2]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(305419896)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
