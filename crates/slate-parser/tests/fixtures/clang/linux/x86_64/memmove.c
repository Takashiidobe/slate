#include <stdio.h>
#include <string.h>

static int get_count(void) { return 4; }

int main(void) {
  unsigned char forward_buf[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  memmove(forward_buf + 1, forward_buf, 5);

  unsigned char backward_buf[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  memmove(backward_buf, backward_buf + 1, 5);

  unsigned char dyn_buf[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  int           n          = get_count();
  memmove(dyn_buf, dyn_buf + 1, n);

  for (int i = 0; i < 8; i++)
    printf("%d ", forward_buf[i]);
  for (int i = 0; i < 8; i++)
    printf("%d ", backward_buf[i]);
  for (int i = 0; i < 8; i++)
    printf("%d ", dyn_buf[i]);
  printf("\n");
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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     global %21 .str21: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 32, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %23 .str23: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 32, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %25 .str25: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 32, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %26 .str26: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %2 @printf(%16 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %6 @memmove(%17 __dest: ptr<void>, %18 __src: ptr<const void>, %19 __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %7 @get_count() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(4);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %9 forward_buf: array<u8, 8> [storage=automatic] = aggregate<array<u8, 8>, zero_fill=false>(index0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))), index1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(2))), index2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(3))), index3 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(4))), index4 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(5))), index5 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(6))), index6 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(7))), index7 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(8))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%6, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(8)>(%9), const<i32>(1))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<u8>, length=Some(8)>(%9)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5))));
// DEFAULT-NEXT:         let %10 backward_buf: array<u8, 8> [storage=automatic] = aggregate<array<u8, 8>, zero_fill=false>(index0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))), index1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(2))), index2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(3))), index3 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(4))), index4 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(5))), index5 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(6))), index6 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(7))), index7 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(8))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%6, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<u8>, length=Some(8)>(%10)), pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(8)>(%10), const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5))));
// DEFAULT-NEXT:         let %11 dyn_buf: array<u8, 8> [storage=automatic] = aggregate<array<u8, 8>, zero_fill=false>(index0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))), index1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(2))), index2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(3))), index3 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(4))), index4 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(5))), index5 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(6))), index6 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(7))), index7 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(8))));
// DEFAULT-NEXT:         let %12 n: i32 [storage=automatic] = call<i32, signature=fn() -> i32>(%7);
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%6, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<u8>, length=Some(8)>(%11)), pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(8)>(%11), const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%12))));
// DEFAULT-NEXT:         for %20
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %13 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%13), const<i32>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %27: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:                 let %28: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%27), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%13, read<i32>(%28));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%2, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%21)), reinterpret<i32, reason=vararg, fits=unknown>(widen<u32, reason=vararg>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(8)>(%9), read<i32>(%13)))))));
// DEFAULT-NEXT:         for %22
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %14 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%14), const<i32>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %29: i32 [synthetic] = read<i32>(%14);
// DEFAULT-NEXT:                 let %30: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%29), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%14, read<i32>(%30));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%2, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%23)), reinterpret<i32, reason=vararg, fits=unknown>(widen<u32, reason=vararg>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(8)>(%10), read<i32>(%14)))))));
// DEFAULT-NEXT:         for %24
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %15 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%15), const<i32>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %31: i32 [synthetic] = read<i32>(%15);
// DEFAULT-NEXT:                 let %32: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%31), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%15, read<i32>(%32));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%2, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%25)), reinterpret<i32, reason=vararg, fits=unknown>(widen<u32, reason=vararg>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(8)>(%11), read<i32>(%15)))))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%2, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%26)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
