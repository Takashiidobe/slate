#include <stdio.h>

static void pairwise_swap(int *items, int len) {
  for (int i = 0; i < len; i++) {
    for (int j = 0; j < len; j++) {
      if (items[i] > items[j]) {
        int tmp  = items[i];
        items[i] = items[j];
        items[j] = tmp;
      }
    }
  }
}

static void pairwise_swap_tmp_reused(int *items, int len) {
  for (int i = 0; i < len; i++) {
    for (int j = 0; j < len; j++) {
      if (items[i] > items[j]) {
        int tmp  = items[i];
        items[i] = items[j];
        items[j] = tmp;
        printf("tmp=%d\n", tmp);
      }
    }
  }
}

static void nested_self_swap(int *items, int len) {
  for (int i = 0; i < len; i++) {
    for (int j = 0; j < len; j++) {
      if (items[i] > 0) {
        int tmp  = items[j];
        items[j] = items[j];
        items[j] = tmp;
      }
    }
  }
}

int main(void) {
  int a[5] = {5, 3, 4, 1, 2};
  pairwise_swap(a, 5);
  printf("%d %d %d %d %d\n", a[0], a[1], a[2], a[3], a[4]);

  int b[3] = {3, 1, 2};
  pairwise_swap_tmp_reused(b, 3);
  printf("%d %d %d\n", b[0], b[1], b[2]);

  int c[3] = {7, 8, 9};
  nested_self_swap(c, 3);
  printf("%d %d %d\n", c[0], c[1], c[2]);
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
// DEFAULT-NEXT:     global %29 .str29: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([116, 109, 112, 61, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %32 .str32: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %33 .str33: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %34 .str34: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%24 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @pairwise_swap(%3 items: ptr<i32>, %4 len: i32) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         for %25
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %5 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%5), read<i32>(%4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %35: i32 [synthetic] = read<i32>(%5);
// DEFAULT-NEXT:                 let %36: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%35), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%5, read<i32>(%36));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     for %26
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             let %6 j: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                         condition: lt<i32>(read<i32>(%6), read<i32>(%4))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %37: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:                             let %38: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%37), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%6, read<i32>(%38));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 if gt<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%3), read<i32>(%5)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%3), read<i32>(%6)))))
// DEFAULT-NEXT:                                     {
// DEFAULT-NEXT:                                         let %7 tmp: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%3), read<i32>(%5))));
// DEFAULT-NEXT:                                         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%3), read<i32>(%5))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%3), read<i32>(%6)))));
// DEFAULT-NEXT:                                         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%3), read<i32>(%6))), read<i32>(%7));
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @pairwise_swap_tmp_reused(%9 items: ptr<i32>, %10 len: i32) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         for %27
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %11 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%11), read<i32>(%10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %39: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:                 let %40: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%39), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%11, read<i32>(%40));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     for %28
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             let %12 j: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                         condition: lt<i32>(read<i32>(%12), read<i32>(%10))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %41: i32 [synthetic] = read<i32>(%12);
// DEFAULT-NEXT:                             let %42: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%41), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%12, read<i32>(%42));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 if gt<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%9), read<i32>(%11)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%9), read<i32>(%12)))))
// DEFAULT-NEXT:                                     {
// DEFAULT-NEXT:                                         let %13 tmp: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%9), read<i32>(%11))));
// DEFAULT-NEXT:                                         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%9), read<i32>(%11))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%9), read<i32>(%12)))));
// DEFAULT-NEXT:                                         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%9), read<i32>(%12))), read<i32>(%13));
// DEFAULT-NEXT:                                         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%29)), read<i32>(%13));
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @nested_self_swap(%15 items: ptr<i32>, %16 len: i32) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         for %30
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %17 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%17), read<i32>(%16))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %43: i32 [synthetic] = read<i32>(%17);
// DEFAULT-NEXT:                 let %44: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%43), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%17, read<i32>(%44));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     for %31
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             let %18 j: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                         condition: lt<i32>(read<i32>(%18), read<i32>(%16))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %45: i32 [synthetic] = read<i32>(%18);
// DEFAULT-NEXT:                             let %46: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%45), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%18, read<i32>(%46));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 if gt<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%15), read<i32>(%17)))), const<i32>(0))
// DEFAULT-NEXT:                                     {
// DEFAULT-NEXT:                                         let %19 tmp: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%15), read<i32>(%18))));
// DEFAULT-NEXT:                                         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%15), read<i32>(%18))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%15), read<i32>(%18)))));
// DEFAULT-NEXT:                                         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%15), read<i32>(%18))), read<i32>(%19));
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %21 a: array<i32, 5> [storage=automatic] [align=16] = aggregate<array<i32, 5>, zero_fill=false>(index0 = const<i32>(5), index1 = const<i32>(3), index2 = const<i32>(4), index3 = const<i32>(1), index4 = const<i32>(2));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>, i32) -> void>(%2, array_decay<ptr<i32>, length=Some(5)>(%21), const<i32>(5));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%32)), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(5)>(%21), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(5)>(%21), const<i32>(1)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(5)>(%21), const<i32>(2)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(5)>(%21), const<i32>(3)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(5)>(%21), const<i32>(4)))));
// DEFAULT-NEXT:         let %22 b: array<i32, 3> [storage=automatic] = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(3), index1 = const<i32>(1), index2 = const<i32>(2));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>, i32) -> void>(%8, array_decay<ptr<i32>, length=Some(3)>(%22), const<i32>(3));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%33)), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(%22), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(%22), const<i32>(1)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(%22), const<i32>(2)))));
// DEFAULT-NEXT:         let %23 c: array<i32, 3> [storage=automatic] = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(7), index1 = const<i32>(8), index2 = const<i32>(9));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>, i32) -> void>(%14, array_decay<ptr<i32>, length=Some(3)>(%23), const<i32>(3));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%34)), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(%23), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(%23), const<i32>(1)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(%23), const<i32>(2)))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
