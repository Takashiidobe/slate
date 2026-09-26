#include <limits.h>

void abort(void);
void exit(int);

#if INT_MAX <= 32767
int main() { exit(0); }
#else
void get_addrs(const char **x, int *y) {
  x[0] = "a1111" + (y[0] - 0x10000) * 2;
  x[1] = "a1112" + (y[1] - 0x20000) * 2;
  x[2] = "a1113" + (y[2] - 0x30000) * 2;
  x[3] = "a1114" + (y[3] - 0x40000) * 2;
  x[4] = "a1115" + (y[4] - 0x50000) * 2;
  x[5] = "a1116" + (y[5] - 0x60000) * 2;
  x[6] = "a1117" + (y[6] - 0x70000) * 2;
  x[7] = "a1118" + (y[7] - 0x80000) * 2;
}

int main() {
  const char *x[8];
  int         y[8];
  int         i;

  for (i = 0; i < 8; i++)
    y[i] = 0x10000 * (i + 1);
  get_addrs(x, y);
  for (i = 0; i < 8; i++)
    if (*x[i] != 'a')
      abort();
  exit(0);
}
#endif



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
// DEFAULT-NEXT:     global %10 .str10: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([97, 49, 49, 49, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %11 .str11: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([97, 49, 49, 49, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %12 .str12: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([97, 49, 49, 49, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %13 .str13: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([97, 49, 49, 49, 52, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %14 .str14: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([97, 49, 49, 49, 53, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %15 .str15: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([97, 49, 49, 49, 54, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %16 .str16: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([97, 49, 49, 49, 55, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %17 .str17: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([97, 49, 49, 49, 56, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%9 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @get_addrs(%3 x: ptr<ptr<const i8>>, %4 y: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<ptr<const i8>>(deref(ptr_offset<ptr<ptr<const i8>>, subtract=false, element=ptr<const i8>, overflow=ub>(read<ptr<ptr<const i8>>>(%3), const<i32>(0))), pointer_cast<ptr<const i8>, reason=assign>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(6)>(%10), mul<i32, overflow=ub>(sub<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%4), const<i32>(0)))), const<i32>(65536)), const<i32>(2)))));
// DEFAULT-NEXT:         write<ptr<const i8>>(deref(ptr_offset<ptr<ptr<const i8>>, subtract=false, element=ptr<const i8>, overflow=ub>(read<ptr<ptr<const i8>>>(%3), const<i32>(1))), pointer_cast<ptr<const i8>, reason=assign>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(6)>(%11), mul<i32, overflow=ub>(sub<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%4), const<i32>(1)))), const<i32>(131072)), const<i32>(2)))));
// DEFAULT-NEXT:         write<ptr<const i8>>(deref(ptr_offset<ptr<ptr<const i8>>, subtract=false, element=ptr<const i8>, overflow=ub>(read<ptr<ptr<const i8>>>(%3), const<i32>(2))), pointer_cast<ptr<const i8>, reason=assign>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(6)>(%12), mul<i32, overflow=ub>(sub<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%4), const<i32>(2)))), const<i32>(196608)), const<i32>(2)))));
// DEFAULT-NEXT:         write<ptr<const i8>>(deref(ptr_offset<ptr<ptr<const i8>>, subtract=false, element=ptr<const i8>, overflow=ub>(read<ptr<ptr<const i8>>>(%3), const<i32>(3))), pointer_cast<ptr<const i8>, reason=assign>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(6)>(%13), mul<i32, overflow=ub>(sub<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%4), const<i32>(3)))), const<i32>(262144)), const<i32>(2)))));
// DEFAULT-NEXT:         write<ptr<const i8>>(deref(ptr_offset<ptr<ptr<const i8>>, subtract=false, element=ptr<const i8>, overflow=ub>(read<ptr<ptr<const i8>>>(%3), const<i32>(4))), pointer_cast<ptr<const i8>, reason=assign>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(6)>(%14), mul<i32, overflow=ub>(sub<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%4), const<i32>(4)))), const<i32>(327680)), const<i32>(2)))));
// DEFAULT-NEXT:         write<ptr<const i8>>(deref(ptr_offset<ptr<ptr<const i8>>, subtract=false, element=ptr<const i8>, overflow=ub>(read<ptr<ptr<const i8>>>(%3), const<i32>(5))), pointer_cast<ptr<const i8>, reason=assign>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(6)>(%15), mul<i32, overflow=ub>(sub<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%4), const<i32>(5)))), const<i32>(393216)), const<i32>(2)))));
// DEFAULT-NEXT:         write<ptr<const i8>>(deref(ptr_offset<ptr<ptr<const i8>>, subtract=false, element=ptr<const i8>, overflow=ub>(read<ptr<ptr<const i8>>>(%3), const<i32>(6))), pointer_cast<ptr<const i8>, reason=assign>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(6)>(%16), mul<i32, overflow=ub>(sub<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%4), const<i32>(6)))), const<i32>(458752)), const<i32>(2)))));
// DEFAULT-NEXT:         write<ptr<const i8>>(deref(ptr_offset<ptr<ptr<const i8>>, subtract=false, element=ptr<const i8>, overflow=ub>(read<ptr<ptr<const i8>>>(%3), const<i32>(7))), pointer_cast<ptr<const i8>, reason=assign>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(6)>(%17), mul<i32, overflow=ub>(sub<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%4), const<i32>(7)))), const<i32>(524288)), const<i32>(2)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %6 x: array<ptr<const i8>, 8> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %7 y: array<i32, 8> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %8 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %18
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%8, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%8), const<i32>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %20: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:                 let %21: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%20), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%8, read<i32>(%21));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(8)>(%7), read<i32>(%8))), mul<i32, overflow=ub>(const<i32>(65536), add<i32, overflow=ub>(read<i32>(%8), const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<ptr<const i8>>, ptr<i32>) -> void>(%2, array_decay<ptr<ptr<const i8>>, length=Some(8)>(%6), array_decay<ptr<i32>, length=Some(8)>(%7));
// DEFAULT-NEXT:         for %19
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%8, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%8), const<i32>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %22: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:                 let %23: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%22), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%8, read<i32>(%23));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<const i8>>(deref(ptr_offset<ptr<ptr<const i8>>, subtract=false, element=ptr<const i8>, overflow=ub>(array_decay<ptr<ptr<const i8>>, length=Some(8)>(%6), read<i32>(%8))))))), const<i32>(97))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
