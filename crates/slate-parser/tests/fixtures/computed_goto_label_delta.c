#include <stdio.h>

static int interpret(const unsigned char *code, int length) {
  static const int offsets[] = {&&add - &&dispatch, &&double_it - &&dispatch,
                                &&subtract - &&dispatch};
  int              index     = 0;
  int              value     = 1;

dispatch:
  if (index == length)
    return value;
  goto *(&&dispatch + offsets[code[index++]]);

add:
  value += 3;
  goto dispatch;
double_it:
  value *= 2;
  goto dispatch;
subtract:
  value -= 5;
  goto dispatch;
}

int main(void) {
  const unsigned char code[] = {0, 1, 2, 1};
  printf("%d\n", interpret(code, 4));
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
// DEFAULT-NEXT:     global %8 offsets: array<i32, 3> [storage=static] [const] = aggregate<array<i32, 3>, zero_fill=false>(index0 = truncate<i32, reason=assign, fits=unknown>(ptr_diff<i64, element=void, same_array=required, overflow=ub>(label_addr<ptr<void>>(%3), label_addr<ptr<void>>(%2))), index1 = truncate<i32, reason=assign, fits=unknown>(ptr_diff<i64, element=void, same_array=required, overflow=ub>(label_addr<ptr<void>>(%4), label_addr<ptr<void>>(%2))), index2 = truncate<i32, reason=assign, fits=unknown>(ptr_diff<i64, element=void, same_array=required, overflow=ub>(label_addr<ptr<void>>(%5), label_addr<ptr<void>>(%2)))) [linkage=internal];
// DEFAULT-NEXT:     global %14 .str14: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%13 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @interpret(%6 code: ptr<const u8>, %7 length: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %9 index: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %10 value: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         label %2 dispatch:
// DEFAULT-NEXT:             if eq<i32>(read<i32>(%9), read<i32>(%7))
// DEFAULT-NEXT:                 return read<i32>(%10);
// DEFAULT-NEXT:         let %15: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:         let %16: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%15), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%9, read<i32>(%16));
// DEFAULT-NEXT:         goto *ptr_offset<ptr<void>, subtract=false, element=void, overflow=ub>(label_addr<ptr<void>>(%2), read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<const i32>, length=Some(3)>(%8), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<const u8>, subtract=false, element=u8, overflow=ub>(read<ptr<const u8>>(%6), read<i32>(%15))))))))));
// DEFAULT-NEXT:         label %3 add:
// DEFAULT-NEXT:             let %17: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:             let %18: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%17), const<i32>(3));
// DEFAULT-NEXT:             write<i32>(%10, read<i32>(%18));
// DEFAULT-NEXT:         goto %2;
// DEFAULT-NEXT:         label %4 double_it:
// DEFAULT-NEXT:             let %19: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:             let %20: i32 [synthetic] = mul<i32, overflow=ub>(read<i32>(%19), const<i32>(2));
// DEFAULT-NEXT:             write<i32>(%10, read<i32>(%20));
// DEFAULT-NEXT:         goto %2;
// DEFAULT-NEXT:         label %5 subtract:
// DEFAULT-NEXT:             let %21: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:             let %22: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%21), const<i32>(5));
// DEFAULT-NEXT:             write<i32>(%10, read<i32>(%22));
// DEFAULT-NEXT:         goto %2;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %12 code: array<u8, 4> [storage=automatic] [const] = aggregate<array<u8, 4>, zero_fill=false>(index0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))), index1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))), index2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(2))), index3 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%14)), call<i32, signature=fn(ptr<const u8>, i32) -> i32>(%1, array_decay<ptr<const u8>, length=Some(4)>(%12), const<i32>(4)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
