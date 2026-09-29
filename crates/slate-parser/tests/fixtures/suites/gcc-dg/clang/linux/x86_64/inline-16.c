/* { dg-do link } */
/* { dg-options "-std=c99" } */

static inline __SIZE_TYPE__
func1(const volatile void * base, __SIZE_TYPE__ byteOffset)
{
  volatile __SIZE_TYPE__ *addr
    = (volatile __SIZE_TYPE__ *)((__SIZE_TYPE__)base + byteOffset);
  return *addr;
}

static inline __SIZE_TYPE__
func2(__SIZE_TYPE__ data)
{
    return func1(&data, 0);
}

int main(int argc, char *argv[]) {
  __SIZE_TYPE__ b = func2(argc);

  return 0;
}

// SLATE-FILECHECK-STD DEFAULT c99
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
// DEFAULT-NEXT:     fn %[[VALUE_func1:[0-9]+]] @func1(%[[VALUE_base:[0-9]+]] base: ptr<const volatile void>, %[[VALUE_byteOffset:[0-9]+]] byteOffset: u64) -> u64 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_addr:[0-9]+]] addr: ptr<volatile u64> [storage=automatic] = int_to_ptr<ptr<volatile u64>, reason=explicit>(add<u64, overflow=wrap>(ptr_to_int<u64, reason=explicit>(read<ptr<const volatile void>>(%[[VALUE_base]])), read<u64>(%[[VALUE_byteOffset]])));
// DEFAULT-NEXT:         return read<u64, volatile>(deref(read<ptr<volatile u64>>(%[[VALUE_addr]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_func2:[0-9]+]] @func2(%[[VALUE_data:[0-9]+]] data: u64) -> u64 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<u64, signature=fn(ptr<const volatile void>, u64) -> u64>(%[[VALUE_func1]], pointer_cast<ptr<const volatile void>, reason=arg>(addr_of<ptr<u64>>(%[[VALUE_data]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(%[[VALUE_argc:[0-9]+]] argc: i32, %[[VALUE_argv:[0-9]+]] argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: u64 [storage=automatic] = call<u64, signature=fn(u64) -> u64>(%[[VALUE_func2]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_argc]]))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
