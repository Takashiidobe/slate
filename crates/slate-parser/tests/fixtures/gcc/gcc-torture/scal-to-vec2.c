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
// DEFAULT-NEXT:     fn %0 @vlng() -> i64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return widen<i64, reason=explicit>(const<i32>(42));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1 @vint() -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(43);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @vsrt() -> i16 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=explicit, fits=always>(const<i32>(42));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @vchr() -> i8 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i8, reason=explicit, fits=always>(const<i32>(42));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %27 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %4 @main(%5 argc: i32, %6 argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %7 c0: vector<i8, 16> [storage=automatic] = aggregate<vector<i8, 16>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=unknown>(read<i32>(%5)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), index2 = truncate<i8, reason=assign, fits=always>(const<i32>(2)), index3 = truncate<i8, reason=assign, fits=always>(const<i32>(3)), index4 = truncate<i8, reason=assign, fits=always>(const<i32>(4)), index5 = truncate<i8, reason=assign, fits=always>(const<i32>(5)), index6 = truncate<i8, reason=assign, fits=always>(const<i32>(6)), index7 = truncate<i8, reason=assign, fits=always>(const<i32>(7)), index8 = truncate<i8, reason=assign, fits=unknown>(read<i32>(%5)), index9 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), index10 = truncate<i8, reason=assign, fits=always>(const<i32>(2)), index11 = truncate<i8, reason=assign, fits=always>(const<i32>(3)), index12 = truncate<i8, reason=assign, fits=always>(const<i32>(4)), index13 = truncate<i8, reason=assign, fits=always>(const<i32>(5)), index14 = truncate<i8, reason=assign, fits=always>(const<i32>(6)), index15 = truncate<i8, reason=assign, fits=always>(const<i32>(7)));
// DEFAULT-NEXT:         let %8 c1: vector<i8, 16> [storage=automatic];
// DEFAULT-NEXT:         let %9 s0: vector<i16, 8> [storage=automatic] = aggregate<vector<i16, 8>, zero_fill=false>(index0 = truncate<i16, reason=assign, fits=unknown>(read<i32>(%5)), index1 = truncate<i16, reason=assign, fits=always>(const<i32>(1)), index2 = truncate<i16, reason=assign, fits=always>(const<i32>(2)), index3 = truncate<i16, reason=assign, fits=always>(const<i32>(3)), index4 = truncate<i16, reason=assign, fits=always>(const<i32>(4)), index5 = truncate<i16, reason=assign, fits=always>(const<i32>(5)), index6 = truncate<i16, reason=assign, fits=always>(const<i32>(6)), index7 = truncate<i16, reason=assign, fits=always>(const<i32>(7)));
// DEFAULT-NEXT:         let %10 s1: vector<i16, 8> [storage=automatic];
// DEFAULT-NEXT:         let %11 i0: vector<i32, 4> [storage=automatic] = aggregate<vector<i32, 4>, zero_fill=false>(index0 = read<i32>(%5), index1 = const<i32>(1), index2 = const<i32>(2), index3 = const<i32>(3));
// DEFAULT-NEXT:         let %12 i1: vector<i32, 4> [storage=automatic];
// DEFAULT-NEXT:         let %13 l0: vector<i64, 2> [storage=automatic] = aggregate<vector<i64, 2>, zero_fill=false>(index0 = widen<i64, reason=assign>(read<i32>(%5)), index1 = widen<i64, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:         let %14 l1: vector<i64, 2> [storage=automatic];
// DEFAULT-NEXT:         write<vector<i8, 16>>(%8, add<vector<i8, 16>, elementwise=true, overflow=wrap>(vector_splat<vector<i8, 16>, reason=usual_arith>(truncate<i8, reason=usual_arith, fits=unknown>(widen<i32, reason=promotion>(call<i8, signature=fn() -> i8>(%3)))), read<vector<i8, 16>>(%7)));
// DEFAULT-NEXT:         add<vector<i8, 16>, elementwise=true, overflow=wrap>(vector_splat<vector<i8, 16>, reason=usual_arith>(truncate<i8, reason=usual_arith, fits=unknown>(widen<i32, reason=promotion>(call<i8, signature=fn() -> i8>(%3)))), read<vector<i8, 16>>(%7));
// DEFAULT-NEXT:         do %25
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %15 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %26
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%15, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%15), const<i32>(16))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %46: i32 [synthetic] = read<i32>(%15);
// DEFAULT-NEXT:                         let %47: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%46), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%15, read<i32>(%47));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<vector<i8, 16>>>(%8)), read<i32>(%15))))), add<i32, overflow=ub>(widen<i32, reason=promotion>(call<i8, signature=fn() -> i8>(%3)), widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<vector<i8, 16>>>(%7)), read<i32>(%15)))))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%27);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<i16, 8>>(%10, add<vector<i16, 8>, elementwise=true, overflow=wrap>(vector_splat<vector<i16, 8>, reason=usual_arith>(truncate<i16, reason=usual_arith, fits=unknown>(widen<i32, reason=promotion>(call<i16, signature=fn() -> i16>(%2)))), read<vector<i16, 8>>(%9)));
// DEFAULT-NEXT:         add<vector<i16, 8>, elementwise=true, overflow=wrap>(vector_splat<vector<i16, 8>, reason=usual_arith>(truncate<i16, reason=usual_arith, fits=unknown>(widen<i32, reason=promotion>(call<i16, signature=fn() -> i16>(%2)))), read<vector<i16, 8>>(%9));
// DEFAULT-NEXT:         do %28
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %16 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %29
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%16, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%16), const<i32>(8))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %48: i32 [synthetic] = read<i32>(%16);
// DEFAULT-NEXT:                         let %49: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%48), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%16, read<i32>(%49));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%10)), read<i32>(%16))))), add<i32, overflow=ub>(widen<i32, reason=promotion>(call<i16, signature=fn() -> i16>(%2)), widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%9)), read<i32>(%16)))))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%27);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<i16, 8>>(%10, add<vector<i16, 8>, elementwise=true, overflow=wrap>(vector_splat<vector<i16, 8>, reason=usual_arith>(truncate<i16, reason=usual_arith, fits=unknown>(widen<i32, reason=promotion>(call<i8, signature=fn() -> i8>(%3)))), read<vector<i16, 8>>(%9)));
// DEFAULT-NEXT:         add<vector<i16, 8>, elementwise=true, overflow=wrap>(vector_splat<vector<i16, 8>, reason=usual_arith>(truncate<i16, reason=usual_arith, fits=unknown>(widen<i32, reason=promotion>(call<i8, signature=fn() -> i8>(%3)))), read<vector<i16, 8>>(%9));
// DEFAULT-NEXT:         do %30
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %17 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %31
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%17, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%17), const<i32>(8))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %50: i32 [synthetic] = read<i32>(%17);
// DEFAULT-NEXT:                         let %51: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%50), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%17, read<i32>(%51));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%10)), read<i32>(%17))))), add<i32, overflow=ub>(widen<i32, reason=promotion>(call<i8, signature=fn() -> i8>(%3)), widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(pointer_cast<ptr<i16>, reason=explicit>(addr_of<ptr<vector<i16, 8>>>(%9)), read<i32>(%17)))))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%27);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<i32, 4>>(%12, mul<vector<i32, 4>, elementwise=true, overflow=wrap>(vector_splat<vector<i32, 4>, reason=usual_arith>(call<i32, signature=fn() -> i32>(%1)), read<vector<i32, 4>>(%11)));
// DEFAULT-NEXT:         mul<vector<i32, 4>, elementwise=true, overflow=wrap>(vector_splat<vector<i32, 4>, reason=usual_arith>(call<i32, signature=fn() -> i32>(%1)), read<vector<i32, 4>>(%11));
// DEFAULT-NEXT:         do %32
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %18 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %33
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%18, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%18), const<i32>(4))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %52: i32 [synthetic] = read<i32>(%18);
// DEFAULT-NEXT:                         let %53: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%52), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%18, read<i32>(%53));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(pointer_cast<ptr<i32>, reason=explicit>(addr_of<ptr<vector<i32, 4>>>(%12)), read<i32>(%18)))), mul<i32, overflow=ub>(call<i32, signature=fn() -> i32>(%1), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(pointer_cast<ptr<i32>, reason=explicit>(addr_of<ptr<vector<i32, 4>>>(%11)), read<i32>(%18))))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%27);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<i32, 4>>(%12, mul<vector<i32, 4>, elementwise=true, overflow=wrap>(vector_splat<vector<i32, 4>, reason=usual_arith>(widen<i32, reason=promotion>(call<i16, signature=fn() -> i16>(%2))), read<vector<i32, 4>>(%11)));
// DEFAULT-NEXT:         mul<vector<i32, 4>, elementwise=true, overflow=wrap>(vector_splat<vector<i32, 4>, reason=usual_arith>(widen<i32, reason=promotion>(call<i16, signature=fn() -> i16>(%2))), read<vector<i32, 4>>(%11));
// DEFAULT-NEXT:         do %34
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %19 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %35
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%19, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%19), const<i32>(4))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %54: i32 [synthetic] = read<i32>(%19);
// DEFAULT-NEXT:                         let %55: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%54), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%19, read<i32>(%55));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(pointer_cast<ptr<i32>, reason=explicit>(addr_of<ptr<vector<i32, 4>>>(%12)), read<i32>(%19)))), mul<i32, overflow=ub>(widen<i32, reason=promotion>(call<i16, signature=fn() -> i16>(%2)), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(pointer_cast<ptr<i32>, reason=explicit>(addr_of<ptr<vector<i32, 4>>>(%11)), read<i32>(%19))))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%27);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<i32, 4>>(%12, mul<vector<i32, 4>, elementwise=true, overflow=wrap>(vector_splat<vector<i32, 4>, reason=usual_arith>(widen<i32, reason=promotion>(call<i8, signature=fn() -> i8>(%3))), read<vector<i32, 4>>(%11)));
// DEFAULT-NEXT:         mul<vector<i32, 4>, elementwise=true, overflow=wrap>(vector_splat<vector<i32, 4>, reason=usual_arith>(widen<i32, reason=promotion>(call<i8, signature=fn() -> i8>(%3))), read<vector<i32, 4>>(%11));
// DEFAULT-NEXT:         do %36
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %20 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %37
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%20, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%20), const<i32>(4))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %56: i32 [synthetic] = read<i32>(%20);
// DEFAULT-NEXT:                         let %57: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%56), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%20, read<i32>(%57));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(pointer_cast<ptr<i32>, reason=explicit>(addr_of<ptr<vector<i32, 4>>>(%12)), read<i32>(%20)))), mul<i32, overflow=ub>(widen<i32, reason=promotion>(call<i8, signature=fn() -> i8>(%3)), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(pointer_cast<ptr<i32>, reason=explicit>(addr_of<ptr<vector<i32, 4>>>(%11)), read<i32>(%20))))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%27);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<i64, 2>>(%14, mul<vector<i64, 2>, elementwise=true, overflow=wrap>(vector_splat<vector<i64, 2>, reason=usual_arith>(call<i64, signature=fn() -> i64>(%0)), read<vector<i64, 2>>(%13)));
// DEFAULT-NEXT:         mul<vector<i64, 2>, elementwise=true, overflow=wrap>(vector_splat<vector<i64, 2>, reason=usual_arith>(call<i64, signature=fn() -> i64>(%0)), read<vector<i64, 2>>(%13));
// DEFAULT-NEXT:         do %38
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %21 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %39
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%21, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%21), const<i32>(2))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %58: i32 [synthetic] = read<i32>(%21);
// DEFAULT-NEXT:                         let %59: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%58), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%21, read<i32>(%59));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i64>(read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(pointer_cast<ptr<i64>, reason=explicit>(addr_of<ptr<vector<i64, 2>>>(%14)), read<i32>(%21)))), mul<i64, overflow=ub>(call<i64, signature=fn() -> i64>(%0), read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(pointer_cast<ptr<i64>, reason=explicit>(addr_of<ptr<vector<i64, 2>>>(%13)), read<i32>(%21))))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%27);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<i64, 2>>(%14, mul<vector<i64, 2>, elementwise=true, overflow=wrap>(vector_splat<vector<i64, 2>, reason=usual_arith>(widen<i64, reason=usual_arith>(call<i32, signature=fn() -> i32>(%1))), read<vector<i64, 2>>(%13)));
// DEFAULT-NEXT:         mul<vector<i64, 2>, elementwise=true, overflow=wrap>(vector_splat<vector<i64, 2>, reason=usual_arith>(widen<i64, reason=usual_arith>(call<i32, signature=fn() -> i32>(%1))), read<vector<i64, 2>>(%13));
// DEFAULT-NEXT:         do %40
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %22 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %41
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%22, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%22), const<i32>(2))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %60: i32 [synthetic] = read<i32>(%22);
// DEFAULT-NEXT:                         let %61: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%60), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%22, read<i32>(%61));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i64>(read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(pointer_cast<ptr<i64>, reason=explicit>(addr_of<ptr<vector<i64, 2>>>(%14)), read<i32>(%22)))), mul<i64, overflow=ub>(widen<i64, reason=usual_arith>(call<i32, signature=fn() -> i32>(%1)), read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(pointer_cast<ptr<i64>, reason=explicit>(addr_of<ptr<vector<i64, 2>>>(%13)), read<i32>(%22))))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%27);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<i64, 2>>(%14, mul<vector<i64, 2>, elementwise=true, overflow=wrap>(vector_splat<vector<i64, 2>, reason=usual_arith>(widen<i64, reason=usual_arith>(widen<i32, reason=promotion>(call<i16, signature=fn() -> i16>(%2)))), read<vector<i64, 2>>(%13)));
// DEFAULT-NEXT:         mul<vector<i64, 2>, elementwise=true, overflow=wrap>(vector_splat<vector<i64, 2>, reason=usual_arith>(widen<i64, reason=usual_arith>(widen<i32, reason=promotion>(call<i16, signature=fn() -> i16>(%2)))), read<vector<i64, 2>>(%13));
// DEFAULT-NEXT:         do %42
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %23 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %43
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%23, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%23), const<i32>(2))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %62: i32 [synthetic] = read<i32>(%23);
// DEFAULT-NEXT:                         let %63: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%62), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%23, read<i32>(%63));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i64>(read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(pointer_cast<ptr<i64>, reason=explicit>(addr_of<ptr<vector<i64, 2>>>(%14)), read<i32>(%23)))), mul<i64, overflow=ub>(widen<i64, reason=usual_arith>(widen<i32, reason=promotion>(call<i16, signature=fn() -> i16>(%2))), read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(pointer_cast<ptr<i64>, reason=explicit>(addr_of<ptr<vector<i64, 2>>>(%13)), read<i32>(%23))))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%27);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<vector<i64, 2>>(%14, mul<vector<i64, 2>, elementwise=true, overflow=wrap>(vector_splat<vector<i64, 2>, reason=usual_arith>(widen<i64, reason=usual_arith>(widen<i32, reason=promotion>(call<i8, signature=fn() -> i8>(%3)))), read<vector<i64, 2>>(%13)));
// DEFAULT-NEXT:         mul<vector<i64, 2>, elementwise=true, overflow=wrap>(vector_splat<vector<i64, 2>, reason=usual_arith>(widen<i64, reason=usual_arith>(widen<i32, reason=promotion>(call<i8, signature=fn() -> i8>(%3)))), read<vector<i64, 2>>(%13));
// DEFAULT-NEXT:         do %44
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %24 __i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %45
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%24, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%24), const<i32>(2))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %64: i32 [synthetic] = read<i32>(%24);
// DEFAULT-NEXT:                         let %65: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%64), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%24, read<i32>(%65));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i64>(read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(pointer_cast<ptr<i64>, reason=explicit>(addr_of<ptr<vector<i64, 2>>>(%14)), read<i32>(%24)))), mul<i64, overflow=ub>(widen<i64, reason=usual_arith>(widen<i32, reason=promotion>(call<i8, signature=fn() -> i8>(%3))), read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(pointer_cast<ptr<i64>, reason=explicit>(addr_of<ptr<vector<i64, 2>>>(%13)), read<i32>(%24))))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%27);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
