/* { dg-add-options stack_size } */

/* Test for a reload bug:
   if you have a memory reference using the indexed addressing
   mode, and the base address is a pseudo containing an address in the frame
   and this pseudo fails to get a hard register, we end up with a double PLUS,
   so the frame address gets reloaded.  Now, when the index got a hard register,
   and it dies in this insn, push_reload will consider that hard register as
   a reload register, and disregrad overlaps with rld[n_reloads].in .  That is
   fine as long as the add can be done with a single insn, but when the
   constant is so large that it has to be reloaded into a register first,
   that clobbers the index.  */

#include <stdarg.h>

void abort(void);
void exit(int);

#ifdef STACK_SIZE
/* We need to be careful that we don't blow our stack.  Function f, in the
   worst case, needs to fit on the stack:

   * 40 int[CHUNK] arrays;
   * ~40 ints;
   * ~40 pointers for stdarg passing.

   Subtract the last two off STACK_SIZE and figure out what the maximum
   chunk size can be.  We make the last bit conservative to account for
   register saves and other processor-dependent saving.  Limit the
   chunk size to some sane values.  */

#define MIN(X, Y) ((X) < (Y) ? (X) : (Y))
#define MAX(X, Y) ((X) > (Y) ? (X) : (Y))

#define CHUNK                                                                  \
  MIN(500,                                                                     \
      (MAX(1, (signed)(STACK_SIZE - 40 * sizeof(int) - 256 * sizeof(void *)) / \
                  (signed)(40 * sizeof(int)))))
#else
#define CHUNK 500
#endif

void s(int, ...);
void z(int, ...);
void c(int, ...);

typedef int l[CHUNK];

void f(int n) {
  int i;
  l   a0, a1, a2, a3, a4, a5, a6, a7, a8, a9;
  l   a10, a11, a12, a13, a14, a15, a16, a17, a18, a19;
  l   a20, a21, a22, a23, a24, a25, a26, a27, a28, a29;
  l   a30, a31, a32, a33, a34, a35, a36, a37, a38, a39;
  int i0, i1, i2, i3, i4, i5, i6, i7, i8, i9;
  int i10, i11, i12, i13, i14, i15, i16, i17, i18, i19;
  int i20, i21, i22, i23, i24, i25, i26, i27, i28, i29;
  int i30, i31, i32, i33, i34, i35, i36, i37, i38, i39;

  for (i = 0; i < n; i++) {
    s(40, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15,
      a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30,
      a31, a32, a33, a34, a35, a36, a37, a38, a39);
    i0  = a0[0];
    i1  = a1[0];
    i2  = a2[0];
    i3  = a3[0];
    i4  = a4[0];
    i5  = a5[0];
    i6  = a6[0];
    i7  = a7[0];
    i8  = a8[0];
    i9  = a9[0];
    i10 = a10[0];
    i11 = a11[0];
    i12 = a12[0];
    i13 = a13[0];
    i14 = a14[0];
    i15 = a15[0];
    i16 = a16[0];
    i17 = a17[0];
    i18 = a18[0];
    i19 = a19[0];
    i20 = a20[0];
    i21 = a21[0];
    i22 = a22[0];
    i23 = a23[0];
    i24 = a24[0];
    i25 = a25[0];
    i26 = a26[0];
    i27 = a27[0];
    i28 = a28[0];
    i29 = a29[0];
    i30 = a30[0];
    i31 = a31[0];
    i32 = a32[0];
    i33 = a33[0];
    i34 = a34[0];
    i35 = a35[0];
    i36 = a36[0];
    i37 = a37[0];
    i38 = a38[0];
    i39 = a39[0];
    z(40, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15,
      a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30,
      a31, a32, a33, a34, a35, a36, a37, a38, a39);
    a0[i0]   = i0;
    a1[i1]   = i1;
    a2[i2]   = i2;
    a3[i3]   = i3;
    a4[i4]   = i4;
    a5[i5]   = i5;
    a6[i6]   = i6;
    a7[i7]   = i7;
    a8[i8]   = i8;
    a9[i9]   = i9;
    a10[i10] = i10;
    a11[i11] = i11;
    a12[i12] = i12;
    a13[i13] = i13;
    a14[i14] = i14;
    a15[i15] = i15;
    a16[i16] = i16;
    a17[i17] = i17;
    a18[i18] = i18;
    a19[i19] = i19;
    a20[i20] = i20;
    a21[i21] = i21;
    a22[i22] = i22;
    a23[i23] = i23;
    a24[i24] = i24;
    a25[i25] = i25;
    a26[i26] = i26;
    a27[i27] = i27;
    a28[i28] = i28;
    a29[i29] = i29;
    a30[i30] = i30;
    a31[i31] = i31;
    a32[i32] = i32;
    a33[i33] = i33;
    a34[i34] = i34;
    a35[i35] = i35;
    a36[i36] = i36;
    a37[i37] = i37;
    a38[i38] = i38;
    a39[i39] = i39;
    c(40, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15,
      a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30,
      a31, a32, a33, a34, a35, a36, a37, a38, a39);
  }
}

int main() {
  /* CHUNK needs to be at least 40 to avoid stack corruption,
     since index variable i0 in "a[i0] = i0" equals 39.  */
  if (CHUNK < 40)
    exit(0);

  f(1);
  exit(0);
}

void s(int n, ...) {
  va_list list;

  va_start(list, n);
  while (n--) {
    int *a = va_arg(list, int *);
    a[0]   = n;
  }
  va_end(list);
}

void z(int n, ...) {
  va_list list;

  va_start(list, n);
  while (n--) {
    int *a = va_arg(list, int *);
    __builtin_memset(a, 0, sizeof(l));
  }
  va_end(list);
}

void c(int n, ...) {
  va_list list;

  va_start(list, n);
  while (n--) {
    int *a = va_arg(list, int *);
    if (a[n] != n)
      abort();
  }
  va_end(list);
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
// DEFAULT-NEXT:     type @type0 va_list = va_list;
// DEFAULT-NEXT:     type @type1 l = array<i32, 500>;
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @exit(%100 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @s(%91 n: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %92 list: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%92);
// DEFAULT-NEXT:         while %105 {
// DEFAULT-NEXT:             let %108: i32 [synthetic] = read<i32>(%91);
// DEFAULT-NEXT:             let %109: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%108), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%91, read<i32>(%109));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%108), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %93 a: ptr<i32> [storage=automatic] = va_arg<ptr<i32>>(%92);
// DEFAULT-NEXT:                 write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%93), const<i32>(0))), read<i32>(%91));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         va_end(%92);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @z(%94 n: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %95 list: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%95);
// DEFAULT-NEXT:         while %106 {
// DEFAULT-NEXT:             let %110: i32 [synthetic] = read<i32>(%94);
// DEFAULT-NEXT:             let %111: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%110), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%94, read<i32>(%111));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%110), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %96 a: ptr<i32> [storage=automatic] = va_arg<ptr<i32>>(%95);
// DEFAULT-NEXT:                 call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(__builtin_memset, pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%96)), const<i32>(0), const<u64>(2000));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         va_end(%95);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @c(%97 n: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %98 list: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%98);
// DEFAULT-NEXT:         while %107 {
// DEFAULT-NEXT:             let %112: i32 [synthetic] = read<i32>(%97);
// DEFAULT-NEXT:             let %113: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%112), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%97, read<i32>(%113));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%112), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %99 a: ptr<i32> [storage=automatic] = va_arg<ptr<i32>>(%98);
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%99), read<i32>(%97)))), read<i32>(%97))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         va_end(%98);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @f(%8 n: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %9 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %10 a0: array<i32, 500> [storage=automatic];
// DEFAULT-NEXT:         let %11 a1: array<i32, 500> [storage=automatic];
// DEFAULT-NEXT:         let %12 a2: array<i32, 500> [storage=automatic];
// DEFAULT-NEXT:         let %13 a3: array<i32, 500> [storage=automatic];
// DEFAULT-NEXT:         let %14 a4: array<i32, 500> [storage=automatic];
// DEFAULT-NEXT:         let %15 a5: array<i32, 500> [storage=automatic];
// DEFAULT-NEXT:         let %16 a6: array<i32, 500> [storage=automatic];
// DEFAULT-NEXT:         let %17 a7: array<i32, 500> [storage=automatic];
// DEFAULT-NEXT:         let %18 a8: array<i32, 500> [storage=automatic];
// DEFAULT-NEXT:         let %19 a9: array<i32, 500> [storage=automatic];
// DEFAULT-NEXT:         let %20 a10: array<i32, 500> [storage=automatic];
// DEFAULT-NEXT:         let %21 a11: array<i32, 500> [storage=automatic];
// DEFAULT-NEXT:         let %22 a12: array<i32, 500> [storage=automatic];
// DEFAULT-NEXT:         let %23 a13: array<i32, 500> [storage=automatic];
// DEFAULT-NEXT:         let %24 a14: array<i32, 500> [storage=automatic];
// DEFAULT-NEXT:         let %25 a15: array<i32, 500> [storage=automatic];
// DEFAULT-NEXT:         let %26 a16: array<i32, 500> [storage=automatic];
// DEFAULT-NEXT:         let %27 a17: array<i32, 500> [storage=automatic];
// DEFAULT-NEXT:         let %28 a18: array<i32, 500> [storage=automatic];
// DEFAULT-NEXT:         let %29 a19: array<i32, 500> [storage=automatic];
// DEFAULT-NEXT:         let %30 a20: array<i32, 500> [storage=automatic];
// DEFAULT-NEXT:         let %31 a21: array<i32, 500> [storage=automatic];
// DEFAULT-NEXT:         let %32 a22: array<i32, 500> [storage=automatic];
// DEFAULT-NEXT:         let %33 a23: array<i32, 500> [storage=automatic];
// DEFAULT-NEXT:         let %34 a24: array<i32, 500> [storage=automatic];
// DEFAULT-NEXT:         let %35 a25: array<i32, 500> [storage=automatic];
// DEFAULT-NEXT:         let %36 a26: array<i32, 500> [storage=automatic];
// DEFAULT-NEXT:         let %37 a27: array<i32, 500> [storage=automatic];
// DEFAULT-NEXT:         let %38 a28: array<i32, 500> [storage=automatic];
// DEFAULT-NEXT:         let %39 a29: array<i32, 500> [storage=automatic];
// DEFAULT-NEXT:         let %40 a30: array<i32, 500> [storage=automatic];
// DEFAULT-NEXT:         let %41 a31: array<i32, 500> [storage=automatic];
// DEFAULT-NEXT:         let %42 a32: array<i32, 500> [storage=automatic];
// DEFAULT-NEXT:         let %43 a33: array<i32, 500> [storage=automatic];
// DEFAULT-NEXT:         let %44 a34: array<i32, 500> [storage=automatic];
// DEFAULT-NEXT:         let %45 a35: array<i32, 500> [storage=automatic];
// DEFAULT-NEXT:         let %46 a36: array<i32, 500> [storage=automatic];
// DEFAULT-NEXT:         let %47 a37: array<i32, 500> [storage=automatic];
// DEFAULT-NEXT:         let %48 a38: array<i32, 500> [storage=automatic];
// DEFAULT-NEXT:         let %49 a39: array<i32, 500> [storage=automatic];
// DEFAULT-NEXT:         let %50 i0: i32 [storage=automatic];
// DEFAULT-NEXT:         let %51 i1: i32 [storage=automatic];
// DEFAULT-NEXT:         let %52 i2: i32 [storage=automatic];
// DEFAULT-NEXT:         let %53 i3: i32 [storage=automatic];
// DEFAULT-NEXT:         let %54 i4: i32 [storage=automatic];
// DEFAULT-NEXT:         let %55 i5: i32 [storage=automatic];
// DEFAULT-NEXT:         let %56 i6: i32 [storage=automatic];
// DEFAULT-NEXT:         let %57 i7: i32 [storage=automatic];
// DEFAULT-NEXT:         let %58 i8: i32 [storage=automatic];
// DEFAULT-NEXT:         let %59 i9: i32 [storage=automatic];
// DEFAULT-NEXT:         let %60 i10: i32 [storage=automatic];
// DEFAULT-NEXT:         let %61 i11: i32 [storage=automatic];
// DEFAULT-NEXT:         let %62 i12: i32 [storage=automatic];
// DEFAULT-NEXT:         let %63 i13: i32 [storage=automatic];
// DEFAULT-NEXT:         let %64 i14: i32 [storage=automatic];
// DEFAULT-NEXT:         let %65 i15: i32 [storage=automatic];
// DEFAULT-NEXT:         let %66 i16: i32 [storage=automatic];
// DEFAULT-NEXT:         let %67 i17: i32 [storage=automatic];
// DEFAULT-NEXT:         let %68 i18: i32 [storage=automatic];
// DEFAULT-NEXT:         let %69 i19: i32 [storage=automatic];
// DEFAULT-NEXT:         let %70 i20: i32 [storage=automatic];
// DEFAULT-NEXT:         let %71 i21: i32 [storage=automatic];
// DEFAULT-NEXT:         let %72 i22: i32 [storage=automatic];
// DEFAULT-NEXT:         let %73 i23: i32 [storage=automatic];
// DEFAULT-NEXT:         let %74 i24: i32 [storage=automatic];
// DEFAULT-NEXT:         let %75 i25: i32 [storage=automatic];
// DEFAULT-NEXT:         let %76 i26: i32 [storage=automatic];
// DEFAULT-NEXT:         let %77 i27: i32 [storage=automatic];
// DEFAULT-NEXT:         let %78 i28: i32 [storage=automatic];
// DEFAULT-NEXT:         let %79 i29: i32 [storage=automatic];
// DEFAULT-NEXT:         let %80 i30: i32 [storage=automatic];
// DEFAULT-NEXT:         let %81 i31: i32 [storage=automatic];
// DEFAULT-NEXT:         let %82 i32: i32 [storage=automatic];
// DEFAULT-NEXT:         let %83 i33: i32 [storage=automatic];
// DEFAULT-NEXT:         let %84 i34: i32 [storage=automatic];
// DEFAULT-NEXT:         let %85 i35: i32 [storage=automatic];
// DEFAULT-NEXT:         let %86 i36: i32 [storage=automatic];
// DEFAULT-NEXT:         let %87 i37: i32 [storage=automatic];
// DEFAULT-NEXT:         let %88 i38: i32 [storage=automatic];
// DEFAULT-NEXT:         let %89 i39: i32 [storage=automatic];
// DEFAULT-NEXT:         for %104
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%9, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%9), read<i32>(%8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %114: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:                 let %115: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%114), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%9, read<i32>(%115));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn(i32, ...) -> void>(%3, const<i32>(40), array_decay<ptr<i32>, length=Some(500)>(%10), array_decay<ptr<i32>, length=Some(500)>(%11), array_decay<ptr<i32>, length=Some(500)>(%12), array_decay<ptr<i32>, length=Some(500)>(%13), array_decay<ptr<i32>, length=Some(500)>(%14), array_decay<ptr<i32>, length=Some(500)>(%15), array_decay<ptr<i32>, length=Some(500)>(%16), array_decay<ptr<i32>, length=Some(500)>(%17), array_decay<ptr<i32>, length=Some(500)>(%18), array_decay<ptr<i32>, length=Some(500)>(%19), array_decay<ptr<i32>, length=Some(500)>(%20), array_decay<ptr<i32>, length=Some(500)>(%21), array_decay<ptr<i32>, length=Some(500)>(%22), array_decay<ptr<i32>, length=Some(500)>(%23), array_decay<ptr<i32>, length=Some(500)>(%24), array_decay<ptr<i32>, length=Some(500)>(%25), array_decay<ptr<i32>, length=Some(500)>(%26), array_decay<ptr<i32>, length=Some(500)>(%27), array_decay<ptr<i32>, length=Some(500)>(%28), array_decay<ptr<i32>, length=Some(500)>(%29), array_decay<ptr<i32>, length=Some(500)>(%30), array_decay<ptr<i32>, length=Some(500)>(%31), array_decay<ptr<i32>, length=Some(500)>(%32), array_decay<ptr<i32>, length=Some(500)>(%33), array_decay<ptr<i32>, length=Some(500)>(%34), array_decay<ptr<i32>, length=Some(500)>(%35), array_decay<ptr<i32>, length=Some(500)>(%36), array_decay<ptr<i32>, length=Some(500)>(%37), array_decay<ptr<i32>, length=Some(500)>(%38), array_decay<ptr<i32>, length=Some(500)>(%39), array_decay<ptr<i32>, length=Some(500)>(%40), array_decay<ptr<i32>, length=Some(500)>(%41), array_decay<ptr<i32>, length=Some(500)>(%42), array_decay<ptr<i32>, length=Some(500)>(%43), array_decay<ptr<i32>, length=Some(500)>(%44), array_decay<ptr<i32>, length=Some(500)>(%45), array_decay<ptr<i32>, length=Some(500)>(%46), array_decay<ptr<i32>, length=Some(500)>(%47), array_decay<ptr<i32>, length=Some(500)>(%48), array_decay<ptr<i32>, length=Some(500)>(%49));
// DEFAULT-NEXT:                     write<i32>(%50, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%10), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%51, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%11), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%52, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%12), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%53, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%13), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%54, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%14), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%55, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%15), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%56, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%16), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%57, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%17), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%58, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%18), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%59, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%19), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%60, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%20), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%61, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%21), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%62, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%22), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%63, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%23), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%64, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%24), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%65, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%25), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%66, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%26), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%67, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%27), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%68, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%28), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%69, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%29), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%70, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%30), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%71, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%31), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%72, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%32), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%73, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%33), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%74, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%34), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%75, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%35), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%76, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%36), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%77, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%37), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%78, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%38), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%79, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%39), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%80, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%40), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%81, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%41), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%82, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%42), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%83, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%43), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%84, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%44), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%85, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%45), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%86, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%46), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%87, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%47), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%88, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%48), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%89, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%49), const<i32>(0)))));
// DEFAULT-NEXT:                     call<void, signature=fn(i32, ...) -> void>(%4, const<i32>(40), array_decay<ptr<i32>, length=Some(500)>(%10), array_decay<ptr<i32>, length=Some(500)>(%11), array_decay<ptr<i32>, length=Some(500)>(%12), array_decay<ptr<i32>, length=Some(500)>(%13), array_decay<ptr<i32>, length=Some(500)>(%14), array_decay<ptr<i32>, length=Some(500)>(%15), array_decay<ptr<i32>, length=Some(500)>(%16), array_decay<ptr<i32>, length=Some(500)>(%17), array_decay<ptr<i32>, length=Some(500)>(%18), array_decay<ptr<i32>, length=Some(500)>(%19), array_decay<ptr<i32>, length=Some(500)>(%20), array_decay<ptr<i32>, length=Some(500)>(%21), array_decay<ptr<i32>, length=Some(500)>(%22), array_decay<ptr<i32>, length=Some(500)>(%23), array_decay<ptr<i32>, length=Some(500)>(%24), array_decay<ptr<i32>, length=Some(500)>(%25), array_decay<ptr<i32>, length=Some(500)>(%26), array_decay<ptr<i32>, length=Some(500)>(%27), array_decay<ptr<i32>, length=Some(500)>(%28), array_decay<ptr<i32>, length=Some(500)>(%29), array_decay<ptr<i32>, length=Some(500)>(%30), array_decay<ptr<i32>, length=Some(500)>(%31), array_decay<ptr<i32>, length=Some(500)>(%32), array_decay<ptr<i32>, length=Some(500)>(%33), array_decay<ptr<i32>, length=Some(500)>(%34), array_decay<ptr<i32>, length=Some(500)>(%35), array_decay<ptr<i32>, length=Some(500)>(%36), array_decay<ptr<i32>, length=Some(500)>(%37), array_decay<ptr<i32>, length=Some(500)>(%38), array_decay<ptr<i32>, length=Some(500)>(%39), array_decay<ptr<i32>, length=Some(500)>(%40), array_decay<ptr<i32>, length=Some(500)>(%41), array_decay<ptr<i32>, length=Some(500)>(%42), array_decay<ptr<i32>, length=Some(500)>(%43), array_decay<ptr<i32>, length=Some(500)>(%44), array_decay<ptr<i32>, length=Some(500)>(%45), array_decay<ptr<i32>, length=Some(500)>(%46), array_decay<ptr<i32>, length=Some(500)>(%47), array_decay<ptr<i32>, length=Some(500)>(%48), array_decay<ptr<i32>, length=Some(500)>(%49));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%10), read<i32>(%50))), read<i32>(%50));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%11), read<i32>(%51))), read<i32>(%51));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%12), read<i32>(%52))), read<i32>(%52));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%13), read<i32>(%53))), read<i32>(%53));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%14), read<i32>(%54))), read<i32>(%54));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%15), read<i32>(%55))), read<i32>(%55));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%16), read<i32>(%56))), read<i32>(%56));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%17), read<i32>(%57))), read<i32>(%57));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%18), read<i32>(%58))), read<i32>(%58));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%19), read<i32>(%59))), read<i32>(%59));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%20), read<i32>(%60))), read<i32>(%60));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%21), read<i32>(%61))), read<i32>(%61));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%22), read<i32>(%62))), read<i32>(%62));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%23), read<i32>(%63))), read<i32>(%63));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%24), read<i32>(%64))), read<i32>(%64));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%25), read<i32>(%65))), read<i32>(%65));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%26), read<i32>(%66))), read<i32>(%66));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%27), read<i32>(%67))), read<i32>(%67));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%28), read<i32>(%68))), read<i32>(%68));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%29), read<i32>(%69))), read<i32>(%69));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%30), read<i32>(%70))), read<i32>(%70));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%31), read<i32>(%71))), read<i32>(%71));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%32), read<i32>(%72))), read<i32>(%72));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%33), read<i32>(%73))), read<i32>(%73));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%34), read<i32>(%74))), read<i32>(%74));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%35), read<i32>(%75))), read<i32>(%75));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%36), read<i32>(%76))), read<i32>(%76));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%37), read<i32>(%77))), read<i32>(%77));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%38), read<i32>(%78))), read<i32>(%78));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%39), read<i32>(%79))), read<i32>(%79));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%40), read<i32>(%80))), read<i32>(%80));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%41), read<i32>(%81))), read<i32>(%81));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%42), read<i32>(%82))), read<i32>(%82));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%43), read<i32>(%83))), read<i32>(%83));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%44), read<i32>(%84))), read<i32>(%84));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%45), read<i32>(%85))), read<i32>(%85));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%46), read<i32>(%86))), read<i32>(%86));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%47), read<i32>(%87))), read<i32>(%87));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%48), read<i32>(%88))), read<i32>(%88));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%49), read<i32>(%89))), read<i32>(%89));
// DEFAULT-NEXT:                     call<void, signature=fn(i32, ...) -> void>(%5, const<i32>(40), array_decay<ptr<i32>, length=Some(500)>(%10), array_decay<ptr<i32>, length=Some(500)>(%11), array_decay<ptr<i32>, length=Some(500)>(%12), array_decay<ptr<i32>, length=Some(500)>(%13), array_decay<ptr<i32>, length=Some(500)>(%14), array_decay<ptr<i32>, length=Some(500)>(%15), array_decay<ptr<i32>, length=Some(500)>(%16), array_decay<ptr<i32>, length=Some(500)>(%17), array_decay<ptr<i32>, length=Some(500)>(%18), array_decay<ptr<i32>, length=Some(500)>(%19), array_decay<ptr<i32>, length=Some(500)>(%20), array_decay<ptr<i32>, length=Some(500)>(%21), array_decay<ptr<i32>, length=Some(500)>(%22), array_decay<ptr<i32>, length=Some(500)>(%23), array_decay<ptr<i32>, length=Some(500)>(%24), array_decay<ptr<i32>, length=Some(500)>(%25), array_decay<ptr<i32>, length=Some(500)>(%26), array_decay<ptr<i32>, length=Some(500)>(%27), array_decay<ptr<i32>, length=Some(500)>(%28), array_decay<ptr<i32>, length=Some(500)>(%29), array_decay<ptr<i32>, length=Some(500)>(%30), array_decay<ptr<i32>, length=Some(500)>(%31), array_decay<ptr<i32>, length=Some(500)>(%32), array_decay<ptr<i32>, length=Some(500)>(%33), array_decay<ptr<i32>, length=Some(500)>(%34), array_decay<ptr<i32>, length=Some(500)>(%35), array_decay<ptr<i32>, length=Some(500)>(%36), array_decay<ptr<i32>, length=Some(500)>(%37), array_decay<ptr<i32>, length=Some(500)>(%38), array_decay<ptr<i32>, length=Some(500)>(%39), array_decay<ptr<i32>, length=Some(500)>(%40), array_decay<ptr<i32>, length=Some(500)>(%41), array_decay<ptr<i32>, length=Some(500)>(%42), array_decay<ptr<i32>, length=Some(500)>(%43), array_decay<ptr<i32>, length=Some(500)>(%44), array_decay<ptr<i32>, length=Some(500)>(%45), array_decay<ptr<i32>, length=Some(500)>(%46), array_decay<ptr<i32>, length=Some(500)>(%47), array_decay<ptr<i32>, length=Some(500)>(%48), array_decay<ptr<i32>, length=Some(500)>(%49));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %90 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if lt<i32>(const<i32>(500), const<i32>(40))
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%2, const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%7, const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%2, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
