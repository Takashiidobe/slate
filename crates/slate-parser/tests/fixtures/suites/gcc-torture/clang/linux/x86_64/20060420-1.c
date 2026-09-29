extern void abort(void);

typedef float v4flt __attribute__((vector_size(16)));

void __attribute__((noinline)) foo(float *dst, float **src, int a, int n) {
  int      i, j;
  int      z = sizeof(v4flt) / sizeof(float);
  unsigned m = sizeof(v4flt) - 1;

  for (j = 0; j < n && (((unsigned long)dst + j) & m); ++j) {
    float t = src[0][j];
    for (i = 1; i < a; ++i)
      t += src[i][j];
    dst[j] = t;
  }

  for (; j < (n - (4 * z - 1)); j += 4 * z) {
    v4flt t0 = *(v4flt *)(src[0] + j + 0 * z);
    v4flt t1 = *(v4flt *)(src[0] + j + 1 * z);
    v4flt t2 = *(v4flt *)(src[0] + j + 2 * z);
    v4flt t3 = *(v4flt *)(src[0] + j + 3 * z);
    for (i = 1; i < a; ++i) {
      t0 += *(v4flt *)(src[i] + j + 0 * z);
      t1 += *(v4flt *)(src[i] + j + 1 * z);
      t2 += *(v4flt *)(src[i] + j + 2 * z);
      t3 += *(v4flt *)(src[i] + j + 3 * z);
    }
    *(v4flt *)(dst + j + 0 * z) = t0;
    *(v4flt *)(dst + j + 1 * z) = t1;
    *(v4flt *)(dst + j + 2 * z) = t2;
    *(v4flt *)(dst + j + 3 * z) = t3;
  }
  for (; j < n; ++j) {
    float t = src[0][j];
    for (i = 1; i < a; ++i)
      t += src[i][j];
    dst[j] = t;
  }
}

float buffer[64];

int main(void) {
  int    i;
  float *dst, *src[2];
  char  *cptr;

  cptr    = (char *)buffer;
  cptr   += (-(long int)buffer & (16 * sizeof(float) - 1));
  dst     = (float *)cptr;
  src[0]  = dst + 16;
  src[1]  = dst + 32;
  for (i = 0; i < 16; ++i) {
    src[0][i] = (float)i + 11 * (float)i;
    src[1][i] = (float)i + 12 * (float)i;
  }
  foo(dst, src, 2, 16);
  for (i = 0; i < 16; ++i) {
    float e = (float)i + 11 * (float)i + (float)i + 12 * (float)i;
    if (dst[i] != e)
      abort();
  }
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
// DEFAULT-NEXT:     type @type[[TYPE_v4flt:[0-9]+]] v4flt = vector<f32, 4>;
// DEFAULT-NEXT:     global %[[VALUE_buffer:[0-9]+]] buffer: array<f32, 64> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_dst:[0-9]+]] dst: ptr<f32>, %[[VALUE_src:[0-9]+]] src: ptr<ptr<f32>>, %[[VALUE_a:[0-9]+]] a: i32, %[[VALUE_n:[0-9]+]] n: i32) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_z:[0-9]+]] z: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(div<u64, by_zero=ub>(const<u64>(16), const<u64>(4))));
// DEFAULT-NEXT:         let %[[VALUE_m:[0-9]+]] m: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(sub<u64, overflow=wrap>(const<u64>(16), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j]], const<i32>(0));
// DEFAULT-NEXT:             condition: logical_and<bool>(lt<i32>(read<i32>(%[[VALUE_j]]), read<i32>(%[[VALUE_n]])), ne<u64>(and<u64>(add<u64, overflow=wrap>(ptr_to_int<u64, reason=explicit>(read<ptr<f32>>(%[[VALUE_dst]])), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_j]])))), widen<u64, reason=usual_arith>(read<u32>(%[[VALUE_m]]))), const<u64>(0)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_t:[0-9]+]] t: f32 [storage=automatic] = read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(read<ptr<ptr<f32>>>(%[[VALUE_src]]), const<i32>(0)))), read<i32>(%[[VALUE_j]]))));
// DEFAULT-NEXT:                     for %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_i]], const<i32>(1));
// DEFAULT-NEXT:                         condition: lt<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_a]]))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                             let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             let %[[VALUE6:[0-9]+]]: f32 [synthetic] = read<f32>(%[[VALUE_t]]);
// DEFAULT-NEXT:                             let %[[VALUE7:[0-9]+]]: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%[[VALUE6]]), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(read<ptr<ptr<f32>>>(%[[VALUE_src]]), read<i32>(%[[VALUE_i]])))), read<i32>(%[[VALUE_j]])))));
// DEFAULT-NEXT:                             write<f32>(%[[VALUE_t]], read<f32>(%[[VALUE7]]));
// DEFAULT-NEXT:                     write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%[[VALUE_dst]]), read<i32>(%[[VALUE_j]]))), read<f32>(%[[VALUE_t]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %[[VALUE8:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j]]), sub<i32, overflow=ub>(read<i32>(%[[VALUE_n]]), sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(4), read<i32>(%[[VALUE_z]])), const<i32>(1))))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE9:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j]]);
// DEFAULT-NEXT:                 let %[[VALUE10:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE9]]), mul<i32, overflow=ub>(const<i32>(4), read<i32>(%[[VALUE_z]])));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j]], read<i32>(%[[VALUE10]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_t0:[0-9]+]] t0: vector<f32, 4> [storage=automatic] = read<vector<f32, 4>>(deref(pointer_cast<ptr<vector<f32, 4>>, reason=explicit>(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(read<ptr<ptr<f32>>>(%[[VALUE_src]]), const<i32>(0)))), read<i32>(%[[VALUE_j]])), mul<i32, overflow=ub>(const<i32>(0), read<i32>(%[[VALUE_z]]))))));
// DEFAULT-NEXT:                     let %[[VALUE_t1:[0-9]+]] t1: vector<f32, 4> [storage=automatic] = read<vector<f32, 4>>(deref(pointer_cast<ptr<vector<f32, 4>>, reason=explicit>(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(read<ptr<ptr<f32>>>(%[[VALUE_src]]), const<i32>(0)))), read<i32>(%[[VALUE_j]])), mul<i32, overflow=ub>(const<i32>(1), read<i32>(%[[VALUE_z]]))))));
// DEFAULT-NEXT:                     let %[[VALUE_t2:[0-9]+]] t2: vector<f32, 4> [storage=automatic] = read<vector<f32, 4>>(deref(pointer_cast<ptr<vector<f32, 4>>, reason=explicit>(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(read<ptr<ptr<f32>>>(%[[VALUE_src]]), const<i32>(0)))), read<i32>(%[[VALUE_j]])), mul<i32, overflow=ub>(const<i32>(2), read<i32>(%[[VALUE_z]]))))));
// DEFAULT-NEXT:                     let %[[VALUE_t3:[0-9]+]] t3: vector<f32, 4> [storage=automatic] = read<vector<f32, 4>>(deref(pointer_cast<ptr<vector<f32, 4>>, reason=explicit>(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(read<ptr<ptr<f32>>>(%[[VALUE_src]]), const<i32>(0)))), read<i32>(%[[VALUE_j]])), mul<i32, overflow=ub>(const<i32>(3), read<i32>(%[[VALUE_z]]))))));
// DEFAULT-NEXT:                     for %[[VALUE11:[0-9]+]]
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_i]], const<i32>(1));
// DEFAULT-NEXT:                         condition: lt<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_a]]))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %[[VALUE12:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                             let %[[VALUE13:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE12]]), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE13]]));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE14:[0-9]+]]: vector<f32, 4> [synthetic] = read<vector<f32, 4>>(%[[VALUE_t0]]);
// DEFAULT-NEXT:                                 let %[[VALUE15:[0-9]+]]: vector<f32, 4> [synthetic] = add<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f32, 4>>(%[[VALUE14]]), read<vector<f32, 4>>(deref(pointer_cast<ptr<vector<f32, 4>>, reason=explicit>(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(read<ptr<ptr<f32>>>(%[[VALUE_src]]), read<i32>(%[[VALUE_i]])))), read<i32>(%[[VALUE_j]])), mul<i32, overflow=ub>(const<i32>(0), read<i32>(%[[VALUE_z]])))))));
// DEFAULT-NEXT:                                 write<vector<f32, 4>>(%[[VALUE_t0]], read<vector<f32, 4>>(%[[VALUE15]]));
// DEFAULT-NEXT:                                 let %[[VALUE16:[0-9]+]]: vector<f32, 4> [synthetic] = read<vector<f32, 4>>(%[[VALUE_t1]]);
// DEFAULT-NEXT:                                 let %[[VALUE17:[0-9]+]]: vector<f32, 4> [synthetic] = add<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f32, 4>>(%[[VALUE16]]), read<vector<f32, 4>>(deref(pointer_cast<ptr<vector<f32, 4>>, reason=explicit>(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(read<ptr<ptr<f32>>>(%[[VALUE_src]]), read<i32>(%[[VALUE_i]])))), read<i32>(%[[VALUE_j]])), mul<i32, overflow=ub>(const<i32>(1), read<i32>(%[[VALUE_z]])))))));
// DEFAULT-NEXT:                                 write<vector<f32, 4>>(%[[VALUE_t1]], read<vector<f32, 4>>(%[[VALUE17]]));
// DEFAULT-NEXT:                                 let %[[VALUE18:[0-9]+]]: vector<f32, 4> [synthetic] = read<vector<f32, 4>>(%[[VALUE_t2]]);
// DEFAULT-NEXT:                                 let %[[VALUE19:[0-9]+]]: vector<f32, 4> [synthetic] = add<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f32, 4>>(%[[VALUE18]]), read<vector<f32, 4>>(deref(pointer_cast<ptr<vector<f32, 4>>, reason=explicit>(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(read<ptr<ptr<f32>>>(%[[VALUE_src]]), read<i32>(%[[VALUE_i]])))), read<i32>(%[[VALUE_j]])), mul<i32, overflow=ub>(const<i32>(2), read<i32>(%[[VALUE_z]])))))));
// DEFAULT-NEXT:                                 write<vector<f32, 4>>(%[[VALUE_t2]], read<vector<f32, 4>>(%[[VALUE19]]));
// DEFAULT-NEXT:                                 let %[[VALUE20:[0-9]+]]: vector<f32, 4> [synthetic] = read<vector<f32, 4>>(%[[VALUE_t3]]);
// DEFAULT-NEXT:                                 let %[[VALUE21:[0-9]+]]: vector<f32, 4> [synthetic] = add<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f32, 4>>(%[[VALUE20]]), read<vector<f32, 4>>(deref(pointer_cast<ptr<vector<f32, 4>>, reason=explicit>(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(read<ptr<ptr<f32>>>(%[[VALUE_src]]), read<i32>(%[[VALUE_i]])))), read<i32>(%[[VALUE_j]])), mul<i32, overflow=ub>(const<i32>(3), read<i32>(%[[VALUE_z]])))))));
// DEFAULT-NEXT:                                 write<vector<f32, 4>>(%[[VALUE_t3]], read<vector<f32, 4>>(%[[VALUE21]]));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     write<vector<f32, 4>>(deref(pointer_cast<ptr<vector<f32, 4>>, reason=explicit>(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%[[VALUE_dst]]), read<i32>(%[[VALUE_j]])), mul<i32, overflow=ub>(const<i32>(0), read<i32>(%[[VALUE_z]]))))), read<vector<f32, 4>>(%[[VALUE_t0]]));
// DEFAULT-NEXT:                     write<vector<f32, 4>>(deref(pointer_cast<ptr<vector<f32, 4>>, reason=explicit>(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%[[VALUE_dst]]), read<i32>(%[[VALUE_j]])), mul<i32, overflow=ub>(const<i32>(1), read<i32>(%[[VALUE_z]]))))), read<vector<f32, 4>>(%[[VALUE_t1]]));
// DEFAULT-NEXT:                     write<vector<f32, 4>>(deref(pointer_cast<ptr<vector<f32, 4>>, reason=explicit>(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%[[VALUE_dst]]), read<i32>(%[[VALUE_j]])), mul<i32, overflow=ub>(const<i32>(2), read<i32>(%[[VALUE_z]]))))), read<vector<f32, 4>>(%[[VALUE_t2]]));
// DEFAULT-NEXT:                     write<vector<f32, 4>>(deref(pointer_cast<ptr<vector<f32, 4>>, reason=explicit>(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%[[VALUE_dst]]), read<i32>(%[[VALUE_j]])), mul<i32, overflow=ub>(const<i32>(3), read<i32>(%[[VALUE_z]]))))), read<vector<f32, 4>>(%[[VALUE_t3]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %[[VALUE22:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j]]), read<i32>(%[[VALUE_n]]))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE23:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j]]);
// DEFAULT-NEXT:                 let %[[VALUE24:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE23]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j]], read<i32>(%[[VALUE24]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_t_2:[0-9]+]] t: f32 [storage=automatic] = read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(read<ptr<ptr<f32>>>(%[[VALUE_src]]), const<i32>(0)))), read<i32>(%[[VALUE_j]]))));
// DEFAULT-NEXT:                     for %[[VALUE25:[0-9]+]]
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_i]], const<i32>(1));
// DEFAULT-NEXT:                         condition: lt<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_a]]))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %[[VALUE26:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                             let %[[VALUE27:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE26]]), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE27]]));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             let %[[VALUE28:[0-9]+]]: f32 [synthetic] = read<f32>(%[[VALUE_t_2]]);
// DEFAULT-NEXT:                             let %[[VALUE29:[0-9]+]]: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%[[VALUE28]]), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(read<ptr<ptr<f32>>>(%[[VALUE_src]]), read<i32>(%[[VALUE_i]])))), read<i32>(%[[VALUE_j]])))));
// DEFAULT-NEXT:                             write<f32>(%[[VALUE_t_2]], read<f32>(%[[VALUE29]]));
// DEFAULT-NEXT:                     write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%[[VALUE_dst]]), read<i32>(%[[VALUE_j]]))), read<f32>(%[[VALUE_t_2]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_i_2:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_dst_2:[0-9]+]] dst: ptr<f32> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_src_2:[0-9]+]] src: array<ptr<f32>, 2> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_cptr:[0-9]+]] cptr: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_cptr]], pointer_cast<ptr<i8>, reason=explicit>(array_decay<ptr<f32>, length=Some(64)>(%[[VALUE_buffer]])));
// DEFAULT-NEXT:         let %[[VALUE30:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_cptr]]);
// DEFAULT-NEXT:         let %[[VALUE31:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE30]]), and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(neg<i64, overflow=ub>(ptr_to_int<i64, reason=explicit>(array_decay<ptr<f32>, length=Some(64)>(%[[VALUE_buffer]])))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(16))), const<u64>(4)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))));
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_cptr]], read<ptr<i8>>(%[[VALUE31]]));
// DEFAULT-NEXT:         write<ptr<f32>>(%[[VALUE_dst_2]], pointer_cast<ptr<f32>, reason=explicit>(read<ptr<i8>>(%[[VALUE_cptr]])));
// DEFAULT-NEXT:         write<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(2)>(%[[VALUE_src_2]]), const<i32>(0))), ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%[[VALUE_dst_2]]), const<i32>(16)));
// DEFAULT-NEXT:         write<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(2)>(%[[VALUE_src_2]]), const<i32>(1))), ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%[[VALUE_dst_2]]), const<i32>(32)));
// DEFAULT-NEXT:         for %[[VALUE32:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(16))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE33:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE34:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE33]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE34]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(2)>(%[[VALUE_src_2]]), const<i32>(0)))), read<i32>(%[[VALUE_i_2]]))), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%[[VALUE_i_2]])), mul<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(11)), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%[[VALUE_i_2]])))));
// DEFAULT-NEXT:                     write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(deref(ptr_offset<ptr<ptr<f32>>, subtract=false, element=ptr<f32>, overflow=ub>(array_decay<ptr<ptr<f32>>, length=Some(2)>(%[[VALUE_src_2]]), const<i32>(1)))), read<i32>(%[[VALUE_i_2]]))), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%[[VALUE_i_2]])), mul<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(12)), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%[[VALUE_i_2]])))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn(ptr<f32>, ptr<ptr<f32>>, i32, i32) -> void>(%[[VALUE_foo]], read<ptr<f32>>(%[[VALUE_dst_2]]), array_decay<ptr<ptr<f32>>, length=Some(2)>(%[[VALUE_src_2]]), const<i32>(2), const<i32>(16));
// DEFAULT-NEXT:         for %[[VALUE35:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(16))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE36:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE37:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE36]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE37]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_e:[0-9]+]] e: f32 [storage=automatic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(add<f32, rounding=nearest_even,
// DEFAULT-SAME: exceptions=ignore, contract=on>(add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even,
// DEFAULT-SAME: exceptions=ignore>(read<i32>(%[[VALUE_i_2]])), mul<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=usual_arith,
// DEFAULT-SAME: exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(11)), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even,
// DEFAULT-SAME: exceptions=ignore>(read<i32>(%[[VALUE_i_2]])))), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even,
// DEFAULT-SAME: exceptions=ignore>(read<i32>(%[[VALUE_i_2]]))), mul<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=usual_arith,
// DEFAULT-SAME: exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(12)), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even,
// DEFAULT-SAME: exceptions=ignore>(read<i32>(%[[VALUE_i_2]]))));
// DEFAULT-NEXT:                     if ne<f32, exceptions=ignore>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%[[VALUE_dst_2]]), read<i32>(%[[VALUE_i_2]])))), read<f32>(%[[VALUE_e]]))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
