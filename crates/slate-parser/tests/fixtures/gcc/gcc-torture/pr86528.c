/* PR middle-end/86528 */

void __attribute__((noinline, noclone)) test(char *data, __SIZE_TYPE__ len) {
  static char const appended[] = "/./";
  char             *buf        = __builtin_alloca(len + sizeof appended);
  __builtin_memcpy(buf, data, len);
  __builtin_strcpy(buf + len, &appended[data[len - 1] == '/']);
  if (__builtin_strcmp(buf, "test1234/./"))
    __builtin_abort();
}

int main() {
  char *arg = "test1234/";
  test(arg, __builtin_strlen(arg));
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
// DEFAULT-NEXT:     global %3 appended: array<i8, 4> [storage=static] [const] = code_units<array<i8, 4>>([47, 46, 47, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %7 .str7: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([116, 101, 115, 116, 49, 50, 51, 52, 47, 46, 47, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %8 .str8: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([116, 101, 115, 116, 49, 50, 51, 52, 47, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @test(%1 data: ptr<i8>, %2 len: u64) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %4 buf: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(__builtin_alloca, add<u64, overflow=wrap>(read<u64>(%2), const<u64>(4))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(__builtin_memcpy, pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%4)), pointer_cast<ptr<const void>, reason=arg>(read<ptr<i8>>(%1)), read<u64>(%2));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(__builtin_strcpy, ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%4), read<u64>(%2)), addr_of<ptr<const i8>>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(4)>(%3), from_bool<i32, reason=promotion>(eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%1), sub<u64, overflow=wrap>(read<u64>(%2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))))), const<i32>(47)))))));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(__builtin_strcmp, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%4)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(12)>(%7))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %6 arg: ptr<i8> [storage=automatic] = array_decay<ptr<i8>, length=Some(10)>(%8);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i8>, u64) -> void>(%0, read<ptr<i8>>(%6), call<u64, signature=fn(ptr<const i8>) -> u64>(__builtin_strlen, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%6))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
