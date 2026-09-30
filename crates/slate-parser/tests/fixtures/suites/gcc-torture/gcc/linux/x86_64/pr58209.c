/* PR tree-optimization/58209 */

extern void             abort(void);
typedef __INTPTR_TYPE__ T;
T                       buf[1024];

T *foo(T n) {
  if (n == 0)
    return (T *)buf;
  T s = (T)foo(n - 1);
  return (T *)(s + sizeof(T));
}

T *bar(T n) {
  if (n == 0)
    return buf;
  return foo(n - 1) + 1;
}

int main() {
  int i;
  for (i = 0; i < 27; i++)
    if (foo(i) != buf + i || bar(i) != buf + i)
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
// DEFAULT-NEXT:     type @type[[TYPE_T:[0-9]+]] T = i64;
// DEFAULT-NEXT:     global %[[VALUE_buf:[0-9]+]] buf: array<i64, 1024> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_n:[0-9]+]] n: i64) -> ptr<i64> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if eq<i64>(read<i64>(%[[VALUE_n]]), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             return array_decay<ptr<i64>, length=Some(1024)>(%[[VALUE_buf]]);
// DEFAULT-NEXT:         let %[[VALUE_s:[0-9]+]] s: i64 [storage=automatic] = ptr_to_int<i64, reason=explicit>(call<ptr<i64>, signature=fn(i64) -> ptr<i64>>(%[[VALUE_foo]], sub<i64, overflow=ub>(read<i64>(%[[VALUE_n]]), widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         return int_to_ptr<ptr<i64>, reason=explicit>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE_s]])), const<u64>(8)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_n_2:[0-9]+]] n: i64) -> ptr<i64> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if eq<i64>(read<i64>(%[[VALUE_n_2]]), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             return array_decay<ptr<i64>, length=Some(1024)>(%[[VALUE_buf]]);
// DEFAULT-NEXT:         return ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(call<ptr<i64>, signature=fn(i64) -> ptr<i64>>(%[[VALUE_foo]], sub<i64, overflow=ub>(read<i64>(%[[VALUE_n_2]]), widen<i64, reason=usual_arith>(const<i32>(1)))), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(27))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<ptr<i64>>(call<ptr<i64>, signature=fn(i64) -> ptr<i64>>(%[[VALUE_foo]], widen<i64, reason=arg>(read<i32>(%[[VALUE_i]]))), ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(1024)>(%[[VALUE_buf]]), read<i32>(%[[VALUE_i]])))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE3]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE3]], ne<ptr<i64>>(call<ptr<i64>, signature=fn(i64) -> ptr<i64>>(%[[VALUE_bar]], widen<i64, reason=arg>(read<i32>(%[[VALUE_i]]))), ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(1024)>(%[[VALUE_buf]]), read<i32>(%[[VALUE_i]]))));
// DEFAULT-NEXT:                 if read<bool>(%[[VALUE3]])
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
