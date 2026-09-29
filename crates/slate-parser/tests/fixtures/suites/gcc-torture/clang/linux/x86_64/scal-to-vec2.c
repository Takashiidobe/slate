#define vector(elcount, type)                                                  \
  __attribute__((vector_size((elcount) * sizeof(type)))) type

#define vidx(type, vec, idx) (*((type *)&(vec) + idx))

#define operl(a, b, op) (a op b)
#define operr(a, b, op) (b op a)

#define check(type, count, vec0, vec1, num, op, lr)                            \
  do {                                                                         \
    int __i;                                                                   \
    for (__i = 0; __i < count; __i++) {                                        \
      if (vidx(type, vec1, __i) != oper##lr(num, vidx(type, vec0, __i), op))   \
        __builtin_abort();                                                     \
    }                                                                          \
  } while (0)

#define veccompare(type, count, v0, v1)                                        \
  do {                                                                         \
    int __i;                                                                   \
    for (__i = 0; __i < count; __i++) {                                        \
      if (vidx(type, v0, __i) != vidx(type, v1, __i))                          \
        __builtin_abort();                                                     \
    }                                                                          \
  } while (0)

long __attribute__((noinline))  vlng() { return (long)42; }
int __attribute__((noinline))   vint() { return (int)43; }
short __attribute__((noinline)) vsrt() { return (short)42; }
char __attribute__((noinline))  vchr() { return (char)42; }

int main(int argc, char *argv[]) {
  vector(16, char) c0 = {argc, 1, 2, 3, 4, 5, 6, 7, argc, 1, 2, 3, 4, 5, 6, 7};
  vector(16, char) c1;

  vector(8, short) s0 = {argc, 1, 2, 3, 4, 5, 6, 7};
  vector(8, short) s1;

  vector(4, int) i0 = {argc, 1, 2, 3};
  vector(4, int) i1;

  vector(2, long) l0 = {argc, 1};
  vector(2, long) l1;

  c1 = vchr() + c0;
  check(char, 16, c0, c1, vchr(), +, l);

  s1 = vsrt() + s0;
  check(short, 8, s0, s1, vsrt(), +, l);
  s1 = vchr() + s0;
  check(short, 8, s0, s1, vchr(), +, l);

  i1 = vint() * i0;
  check(int, 4, i0, i1, vint(), *, l);
  i1 = vsrt() * i0;
  check(int, 4, i0, i1, vsrt(), *, l);
  i1 = vchr() * i0;
  check(int, 4, i0, i1, vchr(), *, l);

  l1 = vlng() * l0;
  check(long, 2, l0, l1, vlng(), *, l);
  l1 = vint() * l0;
  check(long, 2, l0, l1, vint(), *, l);
  l1 = vsrt() * l0;
  check(long, 2, l0, l1, vsrt(), *, l);
  l1 = vchr() * l0;
  check(long, 2, l0, l1, vchr(), *, l);

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
// DEFAULT-NEXT:     fn %[[VALUE_vlng:[0-9]+]] @vlng() -> i64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return widen<i64, reason=explicit>(const<i32>(42));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_vint:[0-9]+]] @vint() -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(43);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_vsrt:[0-9]+]] @vsrt() -> i16 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=explicit, fits=always>(const<i32>(42));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_vchr:[0-9]+]] @vchr() -> i8 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i8, reason=explicit, fits=always>(const<i32>(42));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(%[[VALUE_argc:[0-9]+]] argc: i32, %[[VALUE_argv:[0-9]+]] argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_c0:[0-9]+]] c0: vector<i8, 16> [storage=automatic] = aggregate<vector<i8, 16>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=unknown>(read<i32>(%[[VALUE_argc]])), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), index2 = truncate<i8, reason=assign, fits=always>(const<i32>(2)), index3 = truncate<i8, reason=assign, fits=always>(const<i32>(3)), index4 = truncate<i8, reason=assign, fits=always>(const<i32>(4)), index5 = truncate<i8, reason=assign, fits=always>(const<i32>(5)), index6 = truncate<i8, reason=assign, fits=always>(const<i32>(6)), index7 = truncate<i8, reason=assign, fits=always>(const<i32>(7)), index8 = truncate<i8, reason=assign, fits=unknown>(read<i32>(%[[VALUE_argc]])), index9 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), index10 = truncate<i8, reason=assign, fits=always>(const<i32>(2)), index11 = truncate<i8, reason=assign, fits=always>(const<i32>(3)), index12 = truncate<i8, reason=assign, fits=always>(const<i32>(4)), index13 = truncate<i8, reason=assign, fits=always>(const<i32>(5)), index14 = truncate<i8, reason=assign, fits=always>(const<i32>(6)), index15 = truncate<i8, reason=assign, fits=always>(const<i32>(7)));
// DEFAULT-NEXT:         let %[[VALUE_c1:[0-9]+]] c1: vector<i8, 16> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_s0:[0-9]+]] s0: vector<i16, 8> [storage=automatic] = aggregate<vector<i16, 8>, zero_fill=false>(index0 = truncate<i16, reason=assign, fits=unknown>(read<i32>(%[[VALUE_argc]])), index1 = truncate<i16, reason=assign, fits=always>(const<i32>(1)), index2 = truncate<i16, reason=assign, fits=always>(const<i32>(2)), index3 = truncate<i16, reason=assign, fits=always>(const<i32>(3)), index4 = truncate<i16, reason=assign, fits=always>(const<i32>(4)), index5 = truncate<i16, reason=assign, fits=always>(const<i32>(5)), index6 = truncate<i16, reason=assign, fits=always>(const<i32>(6)), index7 = truncate<i16, reason=assign, fits=always>(const<i32>(7)));
// DEFAULT-NEXT:         let %[[VALUE_s1:[0-9]+]] s1: vector<i16, 8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i0:[0-9]+]] i0: vector<i32, 4> [storage=automatic] = aggregate<vector<i32, 4>, zero_fill=false>(index0 = read<i32>(%[[VALUE_argc]]), index1 = const<i32>(1), index2 = const<i32>(2), index3 = const<i32>(3));
// DEFAULT-NEXT:         let %[[VALUE_i1:[0-9]+]] i1: vector<i32, 4> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_l0:[0-9]+]] l0: vector<i64, 2> [storage=automatic] = aggregate<vector<i64, 2>, zero_fill=false>(index0 = widen<i64, reason=assign>(read<i32>(%[[VALUE_argc]])), index1 = widen<i64, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE_l1:[0-9]+]] l1: vector<i64, 2> [storage=automatic];
// DEFAULT-NEXT:         write<vector<i8, 16>>(%[[VALUE_c1]], add<vector<i8, 16>, elementwise=true, overflow=wrap>(vector_splat<vector<i8, 16>, reason=usual_arith>(truncate<i8, reason=usual_arith, fits=unknown>(widen<i32, reason=promotion>(call<i8, signature=fn() -> i8>(%[[VALUE_vchr]])))), read<vector<i8, 16>>(%[[VALUE_c0]])));
// DEFAULT-NEXT:         add<vector<i8, 16>, elementwise=true, overflow=wrap>(vector_splat<vector<i8, 16>, reason=usual_arith>(truncate<i8, reason=usual_arith, fits=unknown>(widen<i32, reason=promotion>(call<i8, signature=fn() -> i8>(%[[VALUE_vchr]])))), read<vector<i8, 16>>(%[[VALUE_c0]]));
// DEFAULT-NEXT:         do %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i]]), const<i32>(16))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i]]);
// DEFAULT-NEXT:                         let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<vector<i8, 16>>>(%[[VALUE_c1]])), read<i32>(%[[VALUE___i]]))))), add<i32, overflow=ub>(widen<i32, reason=promotion>(call<i8, signature=fn() -> i8>(%[[VALUE_vchr]])), widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<vector<i8, 16>>>(%[[VALUE_c0]])), read<i32>(%[[VALUE___i]])))))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<i16, 8>>(%[[VALUE_s1]], add<vector<i16, 8>, elementwise=true, overflow=wrap>(vector_splat<vector<i16, 8>, reason=usual_arith>(truncate<i16, reason=usual_arith, fits=unknown>(widen<i32, reason=promotion>(call<i16, signature=fn() -> i16>(%[[VALUE_vsrt]])))), read<vector<i16, 8>>(%[[VALUE_s0]])));
// DEFAULT-NEXT:         add<vector<i16, 8>, elementwise=true, overflow=wrap>(vector_splat<vector<i16, 8>, reason=usual_arith>(truncate<i16, reason=usual_arith, fits=unknown>(widen<i32, reason=promotion>(call<i16, signature=fn() -> i16>(%[[VALUE_vsrt]])))), read<vector<i16, 8>>(%[[VALUE_s0]]));
// DEFAULT-NEXT:         do %[[VALUE4:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_2:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE5:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_2]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_2]]), const<i32>(8))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_2]]);
// DEFAULT-NEXT:                         let %[[VALUE7:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE6]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_2]], read<i32>(%[[VALUE7]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%[[VALUE_s1]])), read<i32>(%[[VALUE___i_2]]))))), add<i32, overflow=ub>(widen<i32, reason=promotion>(call<i16, signature=fn() -> i16>(%[[VALUE_vsrt]])), widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%[[VALUE_s0]])), read<i32>(%[[VALUE___i_2]])))))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<i16, 8>>(%[[VALUE_s1]], add<vector<i16, 8>, elementwise=true, overflow=wrap>(vector_splat<vector<i16, 8>, reason=usual_arith>(truncate<i16, reason=usual_arith, fits=unknown>(widen<i32, reason=promotion>(call<i8, signature=fn() -> i8>(%[[VALUE_vchr]])))), read<vector<i16, 8>>(%[[VALUE_s0]])));
// DEFAULT-NEXT:         add<vector<i16, 8>, elementwise=true, overflow=wrap>(vector_splat<vector<i16, 8>, reason=usual_arith>(truncate<i16, reason=usual_arith, fits=unknown>(widen<i32, reason=promotion>(call<i8, signature=fn() -> i8>(%[[VALUE_vchr]])))), read<vector<i16, 8>>(%[[VALUE_s0]]));
// DEFAULT-NEXT:         do %[[VALUE8:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_3:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE9:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_3]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_3]]), const<i32>(8))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE10:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_3]]);
// DEFAULT-NEXT:                         let %[[VALUE11:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE10]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_3]], read<i32>(%[[VALUE11]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%[[VALUE_s1]])), read<i32>(%[[VALUE___i_3]]))))), add<i32, overflow=ub>(widen<i32, reason=promotion>(call<i8, signature=fn() -> i8>(%[[VALUE_vchr]])), widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%[[VALUE_s0]])), read<i32>(%[[VALUE___i_3]])))))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<i32, 4>>(%[[VALUE_i1]], mul<vector<i32, 4>, elementwise=true, overflow=wrap>(vector_splat<vector<i32, 4>, reason=usual_arith>(call<i32, signature=fn() -> i32>(%[[VALUE_vint]])), read<vector<i32, 4>>(%[[VALUE_i0]])));
// DEFAULT-NEXT:         mul<vector<i32, 4>, elementwise=true, overflow=wrap>(vector_splat<vector<i32, 4>, reason=usual_arith>(call<i32, signature=fn() -> i32>(%[[VALUE_vint]])), read<vector<i32, 4>>(%[[VALUE_i0]]));
// DEFAULT-NEXT:         do %[[VALUE12:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_4:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE13:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_4]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_4]]), const<i32>(4))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE14:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_4]]);
// DEFAULT-NEXT:                         let %[[VALUE15:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE14]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_4]], read<i32>(%[[VALUE15]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(pointer_cast<ptr<i32>, reason=explicit>(addr_of<ptr<vector<i32, 4>>>(%[[VALUE_i1]])), read<i32>(%[[VALUE___i_4]])))), mul<i32, overflow=ub>(call<i32, signature=fn() -> i32>(%[[VALUE_vint]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(pointer_cast<ptr<i32>, reason=explicit>(addr_of<ptr<vector<i32, 4>>>(%[[VALUE_i0]])), read<i32>(%[[VALUE___i_4]]))))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<i32, 4>>(%[[VALUE_i1]], mul<vector<i32, 4>, elementwise=true, overflow=wrap>(vector_splat<vector<i32, 4>, reason=usual_arith>(widen<i32, reason=promotion>(call<i16, signature=fn() -> i16>(%[[VALUE_vsrt]]))), read<vector<i32, 4>>(%[[VALUE_i0]])));
// DEFAULT-NEXT:         mul<vector<i32, 4>, elementwise=true, overflow=wrap>(vector_splat<vector<i32, 4>, reason=usual_arith>(widen<i32, reason=promotion>(call<i16, signature=fn() -> i16>(%[[VALUE_vsrt]]))), read<vector<i32, 4>>(%[[VALUE_i0]]));
// DEFAULT-NEXT:         do %[[VALUE16:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_5:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE17:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_5]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_5]]), const<i32>(4))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE18:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_5]]);
// DEFAULT-NEXT:                         let %[[VALUE19:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE18]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_5]], read<i32>(%[[VALUE19]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(pointer_cast<ptr<i32>, reason=explicit>(addr_of<ptr<vector<i32, 4>>>(%[[VALUE_i1]])), read<i32>(%[[VALUE___i_5]])))), mul<i32, overflow=ub>(widen<i32, reason=promotion>(call<i16, signature=fn() -> i16>(%[[VALUE_vsrt]])), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(pointer_cast<ptr<i32>, reason=explicit>(addr_of<ptr<vector<i32, 4>>>(%[[VALUE_i0]])), read<i32>(%[[VALUE___i_5]]))))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<i32, 4>>(%[[VALUE_i1]], mul<vector<i32, 4>, elementwise=true, overflow=wrap>(vector_splat<vector<i32, 4>, reason=usual_arith>(widen<i32, reason=promotion>(call<i8, signature=fn() -> i8>(%[[VALUE_vchr]]))), read<vector<i32, 4>>(%[[VALUE_i0]])));
// DEFAULT-NEXT:         mul<vector<i32, 4>, elementwise=true, overflow=wrap>(vector_splat<vector<i32, 4>, reason=usual_arith>(widen<i32, reason=promotion>(call<i8, signature=fn() -> i8>(%[[VALUE_vchr]]))), read<vector<i32, 4>>(%[[VALUE_i0]]));
// DEFAULT-NEXT:         do %[[VALUE20:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_6:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE21:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_6]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_6]]), const<i32>(4))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE22:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_6]]);
// DEFAULT-NEXT:                         let %[[VALUE23:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE22]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_6]], read<i32>(%[[VALUE23]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(pointer_cast<ptr<i32>, reason=explicit>(addr_of<ptr<vector<i32, 4>>>(%[[VALUE_i1]])), read<i32>(%[[VALUE___i_6]])))), mul<i32, overflow=ub>(widen<i32, reason=promotion>(call<i8, signature=fn() -> i8>(%[[VALUE_vchr]])), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(pointer_cast<ptr<i32>, reason=explicit>(addr_of<ptr<vector<i32, 4>>>(%[[VALUE_i0]])), read<i32>(%[[VALUE___i_6]]))))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<i64, 2>>(%[[VALUE_l1]], mul<vector<i64, 2>, elementwise=true, overflow=wrap>(vector_splat<vector<i64, 2>, reason=usual_arith>(call<i64, signature=fn() -> i64>(%[[VALUE_vlng]])), read<vector<i64, 2>>(%[[VALUE_l0]])));
// DEFAULT-NEXT:         mul<vector<i64, 2>, elementwise=true, overflow=wrap>(vector_splat<vector<i64, 2>, reason=usual_arith>(call<i64, signature=fn() -> i64>(%[[VALUE_vlng]])), read<vector<i64, 2>>(%[[VALUE_l0]]));
// DEFAULT-NEXT:         do %[[VALUE24:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_7:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE25:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_7]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_7]]), const<i32>(2))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE26:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_7]]);
// DEFAULT-NEXT:                         let %[[VALUE27:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE26]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_7]], read<i32>(%[[VALUE27]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i64>(read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(pointer_cast<ptr<i64>, reason=explicit>(addr_of<ptr<vector<i64, 2>>>(%[[VALUE_l1]])), read<i32>(%[[VALUE___i_7]])))), mul<i64, overflow=ub>(call<i64, signature=fn() -> i64>(%[[VALUE_vlng]]), read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(pointer_cast<ptr<i64>, reason=explicit>(addr_of<ptr<vector<i64, 2>>>(%[[VALUE_l0]])), read<i32>(%[[VALUE___i_7]]))))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<i64, 2>>(%[[VALUE_l1]], mul<vector<i64, 2>, elementwise=true, overflow=wrap>(vector_splat<vector<i64, 2>, reason=usual_arith>(widen<i64, reason=usual_arith>(call<i32, signature=fn() -> i32>(%[[VALUE_vint]]))), read<vector<i64, 2>>(%[[VALUE_l0]])));
// DEFAULT-NEXT:         mul<vector<i64, 2>, elementwise=true, overflow=wrap>(vector_splat<vector<i64, 2>, reason=usual_arith>(widen<i64, reason=usual_arith>(call<i32, signature=fn() -> i32>(%[[VALUE_vint]]))), read<vector<i64, 2>>(%[[VALUE_l0]]));
// DEFAULT-NEXT:         do %[[VALUE28:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_8:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE29:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_8]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_8]]), const<i32>(2))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE30:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_8]]);
// DEFAULT-NEXT:                         let %[[VALUE31:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE30]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_8]], read<i32>(%[[VALUE31]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i64>(read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(pointer_cast<ptr<i64>, reason=explicit>(addr_of<ptr<vector<i64, 2>>>(%[[VALUE_l1]])), read<i32>(%[[VALUE___i_8]])))), mul<i64, overflow=ub>(widen<i64, reason=usual_arith>(call<i32, signature=fn() -> i32>(%[[VALUE_vint]])), read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(pointer_cast<ptr<i64>, reason=explicit>(addr_of<ptr<vector<i64, 2>>>(%[[VALUE_l0]])), read<i32>(%[[VALUE___i_8]]))))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<i64, 2>>(%[[VALUE_l1]], mul<vector<i64, 2>, elementwise=true, overflow=wrap>(vector_splat<vector<i64, 2>, reason=usual_arith>(widen<i64, reason=usual_arith>(widen<i32, reason=promotion>(call<i16, signature=fn() -> i16>(%[[VALUE_vsrt]])))), read<vector<i64, 2>>(%[[VALUE_l0]])));
// DEFAULT-NEXT:         mul<vector<i64, 2>, elementwise=true, overflow=wrap>(vector_splat<vector<i64, 2>, reason=usual_arith>(widen<i64, reason=usual_arith>(widen<i32, reason=promotion>(call<i16, signature=fn() -> i16>(%[[VALUE_vsrt]])))), read<vector<i64, 2>>(%[[VALUE_l0]]));
// DEFAULT-NEXT:         do %[[VALUE32:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_9:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE33:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_9]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_9]]), const<i32>(2))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE34:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_9]]);
// DEFAULT-NEXT:                         let %[[VALUE35:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE34]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_9]], read<i32>(%[[VALUE35]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i64>(read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(pointer_cast<ptr<i64>, reason=explicit>(addr_of<ptr<vector<i64, 2>>>(%[[VALUE_l1]])), read<i32>(%[[VALUE___i_9]])))), mul<i64, overflow=ub>(widen<i64, reason=usual_arith>(widen<i32, reason=promotion>(call<i16, signature=fn() -> i16>(%[[VALUE_vsrt]]))), read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(pointer_cast<ptr<i64>, reason=explicit>(addr_of<ptr<vector<i64, 2>>>(%[[VALUE_l0]])), read<i32>(%[[VALUE___i_9]]))))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<i64, 2>>(%[[VALUE_l1]], mul<vector<i64, 2>, elementwise=true, overflow=wrap>(vector_splat<vector<i64, 2>, reason=usual_arith>(widen<i64, reason=usual_arith>(widen<i32, reason=promotion>(call<i8, signature=fn() -> i8>(%[[VALUE_vchr]])))), read<vector<i64, 2>>(%[[VALUE_l0]])));
// DEFAULT-NEXT:         mul<vector<i64, 2>, elementwise=true, overflow=wrap>(vector_splat<vector<i64, 2>, reason=usual_arith>(widen<i64, reason=usual_arith>(widen<i32, reason=promotion>(call<i8, signature=fn() -> i8>(%[[VALUE_vchr]])))), read<vector<i64, 2>>(%[[VALUE_l0]]));
// DEFAULT-NEXT:         do %[[VALUE36:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___i_10:[0-9]+]] __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE37:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_10]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE___i_10]]), const<i32>(2))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE38:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___i_10]]);
// DEFAULT-NEXT:                         let %[[VALUE39:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE38]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE___i_10]], read<i32>(%[[VALUE39]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i64>(read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(pointer_cast<ptr<i64>, reason=explicit>(addr_of<ptr<vector<i64, 2>>>(%[[VALUE_l1]])), read<i32>(%[[VALUE___i_10]])))), mul<i64, overflow=ub>(widen<i64, reason=usual_arith>(widen<i32, reason=promotion>(call<i8, signature=fn() -> i8>(%[[VALUE_vchr]]))), read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(pointer_cast<ptr<i64>, reason=explicit>(addr_of<ptr<vector<i64, 2>>>(%[[VALUE_l0]])), read<i32>(%[[VALUE___i_10]]))))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
