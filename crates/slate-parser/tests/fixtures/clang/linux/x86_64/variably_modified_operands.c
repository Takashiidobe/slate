// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -std=gnu11

int next(void);

int a[1];

unsigned long typeof_declaration(void) {
  int i = 0;
  typeof(++i, (int (*)[i])a) q = 0;
  return sizeof(*q) + i;
}

unsigned long typeof_type_name(int n) {
  typeof(int[n++]) x;
  return sizeof(x);
}

unsigned long typeof_without_effects(int n) {
  int original[n];
  int (*p)[n] = (typeof(p))0;
  typeof(original) copy;
  return sizeof(copy) + sizeof(*p);
}

unsigned long sizeof_typeof(void) {
  int i = 0;
  return sizeof(typeof(*(++i, (char (*)[i])a)));
}

unsigned long auto_type(int *p) {
  __auto_type q = (int (*)[next()])p;
  return sizeof(*q);
}

unsigned long statement_expression(int n, int *p) {
  return sizeof(*({ n = 20; int (*r)[n] = (int (*)[n])p; r; }));
}

unsigned long evaluated_row(int n, int i) {
  int m[n][n];
  return sizeof(m[i++]) + i;
}

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     global %1 a: array<i32, 1> [storage=static] [linkage=external];
// IR-NEXT:     fn %0 @next() -> i32 [linkage=external];
// IR-NEXT:     fn %2 @typeof_declaration() -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %3 i: i32 [storage=automatic] = const<i32>(0);
// IR-NEXT:         let %38: i32 [synthetic] = read<i32>(%3);
// IR-NEXT:         let %39: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%38), const<i32>(1));
// IR-NEXT:         write<i32>(%3, read<i32>(%39));
// IR-NEXT:         let %26: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%3)));
// IR-NEXT:         let %27: ptr<vla<i32, %26>> [synthetic] = pointer_cast<ptr<vla<i32, %26>>, reason=explicit>(array_decay<ptr<i32>, length=Some(1)>(%1));
// IR-NEXT:         let %4 q: ptr<vla<i32, %26>> [storage=automatic] = null<ptr<vla<i32, %26>>>;
// IR-NEXT:         return add<u64, overflow=wrap>(mul<u64, overflow=wrap>(read<u64>(%26), const<u64>(4)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%3))));
// IR-NEXT:     }
// IR-NEXT:     fn %5 @typeof_type_name(%6 n: i32) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %40: i32 [synthetic] = read<i32>(%6);
// IR-NEXT:         let %41: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%40), const<i32>(1));
// IR-NEXT:         write<i32>(%6, read<i32>(%41));
// IR-NEXT:         let %28: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%40)));
// IR-NEXT:         let %7 x: vla<i32, %28> [storage=automatic];
// IR-NEXT:         return mul<u64, overflow=wrap>(read<u64>(%28), const<u64>(4));
// IR-NEXT:     }
// IR-NEXT:     fn %8 @typeof_without_effects(%9 n: i32) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %29: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%9)));
// IR-NEXT:         let %10 original: vla<i32, %29> [storage=automatic];
// IR-NEXT:         let %30: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%9)));
// IR-NEXT:         let %11 p: ptr<vla<i32, %30>> [storage=automatic] = null<ptr<vla<i32, %30>>>;
// IR-NEXT:         let %12 copy: vla<i32, %29> [storage=automatic];
// IR-NEXT:         return add<u64, overflow=wrap>(mul<u64, overflow=wrap>(read<u64>(%29), const<u64>(4)), mul<u64, overflow=wrap>(read<u64>(%30), const<u64>(4)));
// IR-NEXT:     }
// IR-NEXT:     fn %13 @sizeof_typeof() -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %14 i: i32 [storage=automatic] = const<i32>(0);
// IR-NEXT:         let %42: i32 [synthetic] = read<i32>(%14);
// IR-NEXT:         let %43: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%42), const<i32>(1));
// IR-NEXT:         write<i32>(%14, read<i32>(%43));
// IR-NEXT:         let %31: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%14)));
// IR-NEXT:         let %32: ptr<i8> [synthetic] = array_decay<ptr<i8>, length=None>(deref(pointer_cast<ptr<vla<i8, %31>>, reason=explicit>(array_decay<ptr<i32>, length=Some(1)>(%1))));
// IR-NEXT:         return mul<u64, overflow=wrap>(read<u64>(%31), const<u64>(1));
// IR-NEXT:     }
// IR-NEXT:     fn %15 @auto_type(%16 p: ptr<i32>) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %17 q: ptr<vla<i32, %33>> [storage=automatic];
// IR-NEXT:         let %33: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(call<i32, signature=fn() -> i32>(%0)));
// IR-NEXT:         write<ptr<vla<i32, %33>>>(%17, pointer_cast<ptr<vla<i32, %33>>, reason=explicit>(read<ptr<i32>>(%16)));
// IR-NEXT:         return mul<u64, overflow=wrap>(read<u64>(%33), const<u64>(4));
// IR-NEXT:     }
// IR-NEXT:     fn %18 @statement_expression(%19 n: i32, %20 p: ptr<i32>) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %44: ptr<vla<i32, %34>> [synthetic];
// IR-NEXT:         {
// IR-NEXT:             write<i32>(%19, const<i32>(20));
// IR-NEXT:             let %34: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%19)));
// IR-NEXT:             let %21 r: ptr<vla<i32, %34>> [storage=automatic];
// IR-NEXT:             let %35: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%19)));
// IR-NEXT:             write<ptr<vla<i32, %34>>>(%21, pointer_cast<ptr<vla<i32, %34>>, reason=assign>(pointer_cast<ptr<vla<i32, %35>>, reason=explicit>(read<ptr<i32>>(%20))));
// IR-NEXT:             write<ptr<vla<i32, %34>>>(%44, read<ptr<vla<i32, %34>>>(%21));
// IR-NEXT:         }
// IR-NEXT:         return mul<u64, overflow=wrap>(read<u64>(%34), const<u64>(4));
// IR-NEXT:     }
// IR-NEXT:     fn %22 @evaluated_row(%23 n: i32, %24 i: i32) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %36: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%23)));
// IR-NEXT:         let %37: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%23)));
// IR-NEXT:         let %25 m: vla<vla<i32, %37>, %36> [storage=automatic];
// IR-NEXT:         let %45: i32 [synthetic, unsequenced] = read<i32>(%24);
// IR-NEXT:         let %46: i32 [synthetic, unsequenced] = add<i32, overflow=ub>(read<i32>(%45), const<i32>(1));
// IR-NEXT:         write<i32, unsequenced>(%24, read<i32>(%46));
// IR-NEXT:         return add<u64, overflow=wrap>(mul<u64, overflow=wrap>(read<u64>(%37), const<u64>(4)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%24))));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
