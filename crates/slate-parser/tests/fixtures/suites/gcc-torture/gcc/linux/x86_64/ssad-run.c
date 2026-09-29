extern void abort();
extern int  abs(int __x) __attribute__((__nothrow__, __leaf__))
__attribute__((__const__));

static int foo(signed char *w, int i, signed char *x, int j) {
  int tot = 0;
  for (int a = 0; a < 16; a++) {
    for (int b = 0; b < 16; b++)
      tot += abs(w[b] - x[b]);
    w += i;
    x += j;
  }
  return tot;
}

void bar(signed char *w, signed char *x, int i, int *result) {
  *result = foo(w, 16, x, i);
}

int main(void) {
  signed char m[256];
  signed char n[256];
  int         sum, i;

  for (i = 0; i < 256; ++i)
    if (i % 2 == 0) {
      m[i] = (i % 8) * 2 + 1;
      n[i] = -(i % 8);
    } else {
      m[i] = -((i % 8) * 2 + 2);
      n[i] = -((i % 8) >> 1);
    }

  bar(m, n, 16, &sum);

  if (sum != 2368)
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
// DEFAULT-NEXT:     fn %[[VALUE_abs:[0-9]+]] @abs(%[[VALUE___x:[0-9]+]] __x: i32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_w:[0-9]+]] w: ptr<i8>, %[[VALUE_i:[0-9]+]] i: i32, %[[VALUE_x:[0-9]+]] x: ptr<i8>, %[[VALUE_j:[0-9]+]] j: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_tot:[0-9]+]] tot: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_a:[0-9]+]] a: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_a]]), const<i32>(16))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     for %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             let %[[VALUE_b:[0-9]+]] b: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                         condition: lt<i32>(read<i32>(%[[VALUE_b]]), const<i32>(16))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_b]]);
// DEFAULT-NEXT:                             let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_b]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_tot]]);
// DEFAULT-NEXT:                             let %[[VALUE7:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE6]]), call<i32, signature=fn(i32) -> i32>(%[[VALUE_abs]], sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_w]]), read<i32>(%[[VALUE_b]]))))), widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_x]]), read<i32>(%[[VALUE_b]]))))))));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_tot]], read<i32>(%[[VALUE7]]));
// DEFAULT-NEXT:                     let %[[VALUE8:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_w]]);
// DEFAULT-NEXT:                     let %[[VALUE9:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE8]]), read<i32>(%[[VALUE_i]]));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_w]], read<ptr<i8>>(%[[VALUE9]]));
// DEFAULT-NEXT:                     let %[[VALUE10:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_x]]);
// DEFAULT-NEXT:                     let %[[VALUE11:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE10]]), read<i32>(%[[VALUE_j]]));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_x]], read<ptr<i8>>(%[[VALUE11]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_tot]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_w_2:[0-9]+]] w: ptr<i8>, %[[VALUE_x_2:[0-9]+]] x: ptr<i8>, %[[VALUE_i_2:[0-9]+]] i: i32, %[[VALUE_result:[0-9]+]] result: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE_result]])), call<i32, signature=fn(ptr<i8>, i32, ptr<i8>, i32) -> i32>(%[[VALUE_foo]], read<ptr<i8>>(%[[VALUE_w_2]]), const<i32>(16), read<ptr<i8>>(%[[VALUE_x_2]]), read<i32>(%[[VALUE_i_2]])));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<i8>, i32, ptr<i8>, i32) -> i32>(%[[VALUE_foo]], read<ptr<i8>>(%[[VALUE_w_2]]), const<i32>(16), read<ptr<i8>>(%[[VALUE_x_2]]), read<i32>(%[[VALUE_i_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_m:[0-9]+]] m: array<i8, 256> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_n:[0-9]+]] n: array<i8, 256> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_sum:[0-9]+]] sum: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i_3:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE12:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_3]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_3]]), const<i32>(256))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE13:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_3]]);
// DEFAULT-NEXT:                 let %[[VALUE14:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE13]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_3]], read<i32>(%[[VALUE14]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if eq<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_i_3]]), const<i32>(2)), const<i32>(0))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(256)>(%[[VALUE_m]]), read<i32>(%[[VALUE_i_3]]))), truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(mul<i32, overflow=ub>(rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_i_3]]), const<i32>(8)), const<i32>(2)), const<i32>(1))));
// DEFAULT-NEXT:                         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(256)>(%[[VALUE_n]]), read<i32>(%[[VALUE_i_3]]))), truncate<i8, reason=assign, fits=unknown>(neg<i32, overflow=ub>(rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_i_3]]), const<i32>(8)))));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(256)>(%[[VALUE_m]]), read<i32>(%[[VALUE_i_3]]))), truncate<i8, reason=assign, fits=unknown>(neg<i32, overflow=ub>(add<i32, overflow=ub>(mul<i32, overflow=ub>(rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_i_3]]), const<i32>(8)), const<i32>(2)), const<i32>(2)))));
// DEFAULT-NEXT:                         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(256)>(%[[VALUE_n]]), read<i32>(%[[VALUE_i_3]]))), truncate<i8, reason=assign, fits=unknown>(neg<i32, overflow=ub>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_i_3]]), const<i32>(8)), const<i32>(1)))));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i8>, ptr<i8>, i32, ptr<i32>) -> void>(%[[VALUE_bar]], array_decay<ptr<i8>, length=Some(256)>(%[[VALUE_m]]), array_decay<ptr<i8>, length=Some(256)>(%[[VALUE_n]]), const<i32>(16), addr_of<ptr<i32>>(%[[VALUE_sum]]));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_sum]]), const<i32>(2368))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
