#include <stdlib.h>

int main(int argc, char **argv) {
  const int len = argc == 2 ? atoi(argv[1]) : 4;

  int count;
  int data[64];
  int M1[len][len];
  int M2[len][len];

  for (int i = 0; i < len; i++)
    for (int j = 0; j < len; j++)
      M1[i][j] = M2[i][j] = i * len + j;

  M2[1][0] = M2[0][1];

  /* This writes successively 0 and 1 into data[M2[0][1]].  */
  for (int i = 0; i < len - 1; i++)
    for (int j = 0; j < len; j++)
      if (M1[i + 1][j] > M1[i][j])
        data[M2[i][j]] = i;

  if (data[M2[0][1]] != 1)
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
// DEFAULT-NEXT:     fn %0 @atoi(%14 __nptr: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @main(%3 argc: i32, %4 argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %5 len: i32 [storage=automatic] [const];
// DEFAULT-NEXT:         let %23: i32 [synthetic];
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%3), const<i32>(2))
// DEFAULT-NEXT:             write<i32>(%23, call<i32, signature=fn(ptr<const i8>) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(%4), const<i32>(1)))))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<i32>(%23, const<i32>(4));
// DEFAULT-NEXT:         write<i32>(%5, read<i32>(%23));
// DEFAULT-NEXT:         let %6 count: i32 [storage=automatic];
// DEFAULT-NEXT:         let %7 data: array<i32, 64> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %15: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%5)));
// DEFAULT-NEXT:         let %16: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%5)));
// DEFAULT-NEXT:         let %8 M1: vla<vla<i32, %16>, %15> [storage=automatic];
// DEFAULT-NEXT:         let %17: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%5)));
// DEFAULT-NEXT:         let %18: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%5)));
// DEFAULT-NEXT:         let %9 M2: vla<vla<i32, %18>, %17> [storage=automatic];
// DEFAULT-NEXT:         for %19
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %10 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%10), read<i32>(%5))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %24: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                 let %25: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%24), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%10, read<i32>(%25));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 for %20
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         let %11 j: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%11), read<i32>(%5))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %26: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:                         let %27: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%26), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%11, read<i32>(%27));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(deref(ptr_offset<ptr<vla<i32, %18>>, subtract=false, element=vla<i32, %18>, overflow=ub>(array_decay<ptr<vla<i32, %18>>, length=None>(%9), read<i32>(%10)))), read<i32>(%11))), add<i32, overflow=ub>(mul<i32, overflow=ub>(read<i32>(%10), read<i32>(%5)), read<i32>(%11)));
// DEFAULT-NEXT:                         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(deref(ptr_offset<ptr<vla<i32, %16>>, subtract=false, element=vla<i32, %16>, overflow=ub>(array_decay<ptr<vla<i32, %16>>, length=None>(%8), read<i32>(%10)))), read<i32>(%11))), add<i32, overflow=ub>(mul<i32, overflow=ub>(read<i32>(%10), read<i32>(%5)), read<i32>(%11)));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(deref(ptr_offset<ptr<vla<i32, %18>>, subtract=false, element=vla<i32, %18>, overflow=ub>(array_decay<ptr<vla<i32, %18>>, length=None>(%9), const<i32>(1)))), const<i32>(0))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(deref(ptr_offset<ptr<vla<i32, %18>>, subtract=false, element=vla<i32, %18>, overflow=ub>(array_decay<ptr<vla<i32, %18>>, length=None>(%9), const<i32>(0)))), const<i32>(1)))));
// DEFAULT-NEXT:         for %21
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %12 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%12), sub<i32, overflow=ub>(read<i32>(%5), const<i32>(1)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %28: i32 [synthetic] = read<i32>(%12);
// DEFAULT-NEXT:                 let %29: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%28), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%12, read<i32>(%29));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 for %22
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         let %13 j: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%13), read<i32>(%5))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %30: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:                         let %31: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%30), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%13, read<i32>(%31));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         if gt<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(deref(ptr_offset<ptr<vla<i32, %16>>, subtract=false, element=vla<i32, %16>, overflow=ub>(array_decay<ptr<vla<i32, %16>>, length=None>(%8), add<i32, overflow=ub>(read<i32>(%12), const<i32>(1))))), read<i32>(%13)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(deref(ptr_offset<ptr<vla<i32, %16>>, subtract=false, element=vla<i32, %16>, overflow=ub>(array_decay<ptr<vla<i32, %16>>, length=None>(%8), read<i32>(%12)))), read<i32>(%13)))))
// DEFAULT-NEXT:                             write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(64)>(%7), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(deref(ptr_offset<ptr<vla<i32, %18>>, subtract=false, element=vla<i32, %18>, overflow=ub>(array_decay<ptr<vla<i32, %18>>, length=None>(%9), read<i32>(%12)))), read<i32>(%13)))))), read<i32>(%12));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(64)>(%7), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(deref(ptr_offset<ptr<vla<i32, %18>>, subtract=false, element=vla<i32, %18>, overflow=ub>(array_decay<ptr<vla<i32, %18>>, length=None>(%9), const<i32>(0)))), const<i32>(1))))))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
