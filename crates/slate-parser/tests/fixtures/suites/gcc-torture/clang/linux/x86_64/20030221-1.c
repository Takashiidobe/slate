/* PR optimization/8613 */
/* Contributed by Glen Nakamura */

extern void abort(void);

int main(void) {
  char  buf[16] = "1234567890";
  char *p       = buf;

  *p++ = (char)__builtin_strlen(buf);

  if ((buf[0] != 10) || (p - buf != 1))
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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_strlen:[0-9]+]] @__builtin_strlen(%[[VALUE0:[0-9]+]] <unnamed>: ptr<const i8>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_buf:[0-9]+]] buf: array<i8, 16> [storage=automatic] [align=16] = code_units<array<i8, 16>>([49, 50, 51, 52, 53, 54, 55, 56, 57, 48, 0, 0, 0, 0, 0, 0]);
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<i8> [storage=automatic] = array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_buf]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_p]]);
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_p]], read<ptr<i8>>(%[[VALUE2]]));
// DEFAULT-NEXT:         write<i8>(deref(read<ptr<i8>>(%[[VALUE1]])), reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE___builtin_strlen]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_buf]]))))));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_buf]]), const<i32>(0))))), const<i32>(10)), ne<i64>(ptr_diff<i64, element=i8, same_array=required, overflow=ub>(read<ptr<i8>>(%[[VALUE_p]]), array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_buf]])), widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
