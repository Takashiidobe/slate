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
// DEFAULT-NEXT:     type @type[[TYPE___gnuc_va_list:[0-9]+]] __gnuc_va_list = va_list;
// DEFAULT-NEXT:     type @type[[TYPE_va_list:[0-9]+]] va_list = va_list;
// DEFAULT-NEXT:     type @type[[TYPE_l:[0-9]+]] l = array<i32, 500>;
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_s:[0-9]+]] @s(%[[VALUE_n:[0-9]+]] n: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_list:[0-9]+]] list: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_list]]);
// DEFAULT-NEXT:         while %[[VALUE1:[0-9]+]] {
// DEFAULT-NEXT:             let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_n]]);
// DEFAULT-NEXT:             let %[[VALUE3:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_n]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%[[VALUE2]]), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a:[0-9]+]] a: ptr<i32> [storage=automatic] = va_arg<ptr<i32>>(%[[VALUE_list]]);
// DEFAULT-NEXT:                 write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_a]]), const<i32>(0))), read<i32>(%[[VALUE_n]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         va_end(%[[VALUE_list]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_z:[0-9]+]] @z(%[[VALUE_n_2:[0-9]+]] n: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_list_2:[0-9]+]] list: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_list_2]]);
// DEFAULT-NEXT:         while %[[VALUE4:[0-9]+]] {
// DEFAULT-NEXT:             let %[[VALUE5:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_n_2]]);
// DEFAULT-NEXT:             let %[[VALUE6:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE5]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_n_2]], read<i32>(%[[VALUE6]]));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%[[VALUE5]]), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_2:[0-9]+]] a: ptr<i32> [storage=automatic] = va_arg<ptr<i32>>(%[[VALUE_list_2]]);
// DEFAULT-NEXT:                 call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE___builtin_memset:[0-9]+]], pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%[[VALUE_a_2]])), const<i32>(0), const<u64>(2000));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         va_end(%[[VALUE_list_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_c:[0-9]+]] @c(%[[VALUE_n_3:[0-9]+]] n: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_list_3:[0-9]+]] list: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_list_3]]);
// DEFAULT-NEXT:         while %[[VALUE7:[0-9]+]] {
// DEFAULT-NEXT:             let %[[VALUE8:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_n_3]]);
// DEFAULT-NEXT:             let %[[VALUE9:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE8]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_n_3]], read<i32>(%[[VALUE9]]));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%[[VALUE8]]), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_3:[0-9]+]] a: ptr<i32> [storage=automatic] = va_arg<ptr<i32>>(%[[VALUE_list_3]]);
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_a_3]]), read<i32>(%[[VALUE_n_3]])))), read<i32>(%[[VALUE_n_3]]))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         va_end(%[[VALUE_list_3]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_n_4:[0-9]+]] n: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a0:[0-9]+]] a0: array<i32, 500> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_a1:[0-9]+]] a1: array<i32, 500> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_a2:[0-9]+]] a2: array<i32, 500> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_a3:[0-9]+]] a3: array<i32, 500> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_a4:[0-9]+]] a4: array<i32, 500> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_a5:[0-9]+]] a5: array<i32, 500> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_a6:[0-9]+]] a6: array<i32, 500> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_a7:[0-9]+]] a7: array<i32, 500> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_a8:[0-9]+]] a8: array<i32, 500> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_a9:[0-9]+]] a9: array<i32, 500> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_a10:[0-9]+]] a10: array<i32, 500> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_a11:[0-9]+]] a11: array<i32, 500> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_a12:[0-9]+]] a12: array<i32, 500> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_a13:[0-9]+]] a13: array<i32, 500> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_a14:[0-9]+]] a14: array<i32, 500> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_a15:[0-9]+]] a15: array<i32, 500> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_a16:[0-9]+]] a16: array<i32, 500> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_a17:[0-9]+]] a17: array<i32, 500> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_a18:[0-9]+]] a18: array<i32, 500> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_a19:[0-9]+]] a19: array<i32, 500> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_a20:[0-9]+]] a20: array<i32, 500> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_a21:[0-9]+]] a21: array<i32, 500> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_a22:[0-9]+]] a22: array<i32, 500> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_a23:[0-9]+]] a23: array<i32, 500> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_a24:[0-9]+]] a24: array<i32, 500> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_a25:[0-9]+]] a25: array<i32, 500> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_a26:[0-9]+]] a26: array<i32, 500> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_a27:[0-9]+]] a27: array<i32, 500> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_a28:[0-9]+]] a28: array<i32, 500> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_a29:[0-9]+]] a29: array<i32, 500> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_a30:[0-9]+]] a30: array<i32, 500> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_a31:[0-9]+]] a31: array<i32, 500> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_a32:[0-9]+]] a32: array<i32, 500> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_a33:[0-9]+]] a33: array<i32, 500> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_a34:[0-9]+]] a34: array<i32, 500> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_a35:[0-9]+]] a35: array<i32, 500> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_a36:[0-9]+]] a36: array<i32, 500> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_a37:[0-9]+]] a37: array<i32, 500> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_a38:[0-9]+]] a38: array<i32, 500> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_a39:[0-9]+]] a39: array<i32, 500> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_i0:[0-9]+]] i0: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i1:[0-9]+]] i1: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i2:[0-9]+]] i2: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i3:[0-9]+]] i3: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i4:[0-9]+]] i4: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i5:[0-9]+]] i5: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i6:[0-9]+]] i6: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i7:[0-9]+]] i7: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i8:[0-9]+]] i8: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i9:[0-9]+]] i9: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i10:[0-9]+]] i10: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i11:[0-9]+]] i11: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i12:[0-9]+]] i12: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i13:[0-9]+]] i13: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i14:[0-9]+]] i14: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i15:[0-9]+]] i15: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i16:[0-9]+]] i16: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i17:[0-9]+]] i17: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i18:[0-9]+]] i18: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i19:[0-9]+]] i19: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i20:[0-9]+]] i20: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i21:[0-9]+]] i21: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i22:[0-9]+]] i22: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i23:[0-9]+]] i23: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i24:[0-9]+]] i24: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i25:[0-9]+]] i25: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i26:[0-9]+]] i26: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i27:[0-9]+]] i27: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i28:[0-9]+]] i28: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i29:[0-9]+]] i29: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i30:[0-9]+]] i30: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i31:[0-9]+]] i31: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i32:[0-9]+]] i32: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i33:[0-9]+]] i33: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i34:[0-9]+]] i34: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i35:[0-9]+]] i35: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i36:[0-9]+]] i36: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i37:[0-9]+]] i37: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i38:[0-9]+]] i38: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i39:[0-9]+]] i39: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE10:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_n_4]]))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE11:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE12:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE11]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE12]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn(i32, ...) -> void>(%[[VALUE_s]], const<i32>(40), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a0]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a1]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a2]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a3]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a4]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a5]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a6]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a7]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a8]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a9]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a10]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a11]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a12]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a13]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a14]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a15]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a16]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a17]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a18]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a19]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a20]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a21]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a22]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a23]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a24]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a25]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a26]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a27]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a28]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a29]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a30]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a31]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a32]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a33]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a34]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a35]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a36]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a37]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a38]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a39]]));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i0]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a0]]), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i1]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a1]]), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i2]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a2]]), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i3]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a3]]), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i4]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a4]]), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i5]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a5]]), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i6]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a6]]), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i7]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a7]]), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i8]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a8]]), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i9]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a9]]), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i10]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a10]]), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i11]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a11]]), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i12]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a12]]), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i13]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a13]]), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i14]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a14]]), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i15]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a15]]), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i16]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a16]]), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i17]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a17]]), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i18]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a18]]), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i19]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a19]]), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i20]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a20]]), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i21]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a21]]), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i22]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a22]]), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i23]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a23]]), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i24]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a24]]), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i25]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a25]]), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i26]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a26]]), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i27]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a27]]), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i28]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a28]]), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i29]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a29]]), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i30]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a30]]), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i31]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a31]]), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i32]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a32]]), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i33]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a33]]), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i34]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a34]]), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i35]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a35]]), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i36]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a36]]), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i37]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a37]]), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i38]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a38]]), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i39]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a39]]), const<i32>(0)))));
// DEFAULT-NEXT:                     call<void, signature=fn(i32, ...) -> void>(%[[VALUE_z]], const<i32>(40), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a0]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a1]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a2]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a3]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a4]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a5]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a6]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a7]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a8]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a9]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a10]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a11]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a12]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a13]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a14]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a15]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a16]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a17]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a18]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a19]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a20]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a21]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a22]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a23]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a24]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a25]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a26]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a27]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a28]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a29]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a30]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a31]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a32]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a33]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a34]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a35]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a36]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a37]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a38]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a39]]));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a0]]), read<i32>(%[[VALUE_i0]]))), read<i32>(%[[VALUE_i0]]));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a1]]), read<i32>(%[[VALUE_i1]]))), read<i32>(%[[VALUE_i1]]));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a2]]), read<i32>(%[[VALUE_i2]]))), read<i32>(%[[VALUE_i2]]));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a3]]), read<i32>(%[[VALUE_i3]]))), read<i32>(%[[VALUE_i3]]));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a4]]), read<i32>(%[[VALUE_i4]]))), read<i32>(%[[VALUE_i4]]));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a5]]), read<i32>(%[[VALUE_i5]]))), read<i32>(%[[VALUE_i5]]));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a6]]), read<i32>(%[[VALUE_i6]]))), read<i32>(%[[VALUE_i6]]));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a7]]), read<i32>(%[[VALUE_i7]]))), read<i32>(%[[VALUE_i7]]));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a8]]), read<i32>(%[[VALUE_i8]]))), read<i32>(%[[VALUE_i8]]));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a9]]), read<i32>(%[[VALUE_i9]]))), read<i32>(%[[VALUE_i9]]));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a10]]), read<i32>(%[[VALUE_i10]]))), read<i32>(%[[VALUE_i10]]));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a11]]), read<i32>(%[[VALUE_i11]]))), read<i32>(%[[VALUE_i11]]));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a12]]), read<i32>(%[[VALUE_i12]]))), read<i32>(%[[VALUE_i12]]));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a13]]), read<i32>(%[[VALUE_i13]]))), read<i32>(%[[VALUE_i13]]));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a14]]), read<i32>(%[[VALUE_i14]]))), read<i32>(%[[VALUE_i14]]));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a15]]), read<i32>(%[[VALUE_i15]]))), read<i32>(%[[VALUE_i15]]));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a16]]), read<i32>(%[[VALUE_i16]]))), read<i32>(%[[VALUE_i16]]));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a17]]), read<i32>(%[[VALUE_i17]]))), read<i32>(%[[VALUE_i17]]));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a18]]), read<i32>(%[[VALUE_i18]]))), read<i32>(%[[VALUE_i18]]));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a19]]), read<i32>(%[[VALUE_i19]]))), read<i32>(%[[VALUE_i19]]));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a20]]), read<i32>(%[[VALUE_i20]]))), read<i32>(%[[VALUE_i20]]));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a21]]), read<i32>(%[[VALUE_i21]]))), read<i32>(%[[VALUE_i21]]));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a22]]), read<i32>(%[[VALUE_i22]]))), read<i32>(%[[VALUE_i22]]));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a23]]), read<i32>(%[[VALUE_i23]]))), read<i32>(%[[VALUE_i23]]));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a24]]), read<i32>(%[[VALUE_i24]]))), read<i32>(%[[VALUE_i24]]));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a25]]), read<i32>(%[[VALUE_i25]]))), read<i32>(%[[VALUE_i25]]));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a26]]), read<i32>(%[[VALUE_i26]]))), read<i32>(%[[VALUE_i26]]));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a27]]), read<i32>(%[[VALUE_i27]]))), read<i32>(%[[VALUE_i27]]));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a28]]), read<i32>(%[[VALUE_i28]]))), read<i32>(%[[VALUE_i28]]));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a29]]), read<i32>(%[[VALUE_i29]]))), read<i32>(%[[VALUE_i29]]));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a30]]), read<i32>(%[[VALUE_i30]]))), read<i32>(%[[VALUE_i30]]));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a31]]), read<i32>(%[[VALUE_i31]]))), read<i32>(%[[VALUE_i31]]));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a32]]), read<i32>(%[[VALUE_i32]]))), read<i32>(%[[VALUE_i32]]));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a33]]), read<i32>(%[[VALUE_i33]]))), read<i32>(%[[VALUE_i33]]));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a34]]), read<i32>(%[[VALUE_i34]]))), read<i32>(%[[VALUE_i34]]));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a35]]), read<i32>(%[[VALUE_i35]]))), read<i32>(%[[VALUE_i35]]));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a36]]), read<i32>(%[[VALUE_i36]]))), read<i32>(%[[VALUE_i36]]));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a37]]), read<i32>(%[[VALUE_i37]]))), read<i32>(%[[VALUE_i37]]));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a38]]), read<i32>(%[[VALUE_i38]]))), read<i32>(%[[VALUE_i38]]));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a39]]), read<i32>(%[[VALUE_i39]]))), read<i32>(%[[VALUE_i39]]));
// DEFAULT-NEXT:                     call<void, signature=fn(i32, ...) -> void>(%[[VALUE_c]], const<i32>(40), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a0]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a1]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a2]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a3]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a4]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a5]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a6]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a7]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a8]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a9]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a10]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a11]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a12]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a13]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a14]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a15]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a16]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a17]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a18]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a19]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a20]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a21]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a22]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a23]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a24]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a25]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a26]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a27]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a28]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a29]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a30]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a31]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a32]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a33]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a34]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a35]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a36]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a37]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a38]]), array_decay<ptr<i32>, length=Some(500)>(%[[VALUE_a39]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if lt<i32>(const<i32>(500), const<i32>(40))
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_f]], const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_memset]] @__builtin_memset(%[[VALUE13:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE14:[0-9]+]] <unnamed>: i32, %[[VALUE15:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
