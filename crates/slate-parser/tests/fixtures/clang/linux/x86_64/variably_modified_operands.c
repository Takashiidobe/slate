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
// IR-NEXT:     global %[[VALUE_a:[0-9]+]] a: array<i32, 1> [storage=static] [linkage=external];
// IR-NEXT:     fn %[[VALUE_next:[0-9]+]] @next() -> i32 [linkage=external];
// IR-NEXT:     fn %[[VALUE_typeof_declaration:[0-9]+]] @typeof_declaration() -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// IR-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// IR-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE0]]), const<i32>(1));
// IR-NEXT:         write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE1]]));
// IR-NEXT:         let %[[VALUE2:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_i]])));
// IR-NEXT:         let %[[VALUE3:[0-9]+]]: ptr<vla<i32, %[[VALUE2]]>> [synthetic] = pointer_cast<ptr<vla<i32, %[[VALUE2]]>>, reason=explicit>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_a]]));
// IR-NEXT:         let %[[VALUE_q:[0-9]+]] q: ptr<vla<i32, %[[VALUE2]]>> [storage=automatic] = null<ptr<vla<i32, %[[VALUE2]]>>>;
// IR-NEXT:         return add<u64, overflow=wrap>(mul<u64, overflow=wrap>(read<u64>(%[[VALUE2]]), const<u64>(4)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i]]))));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_typeof_type_name:[0-9]+]] @typeof_type_name(%[[VALUE_n:[0-9]+]] n: i32) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_n]]);
// IR-NEXT:         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// IR-NEXT:         write<i32>(%[[VALUE_n]], read<i32>(%[[VALUE5]]));
// IR-NEXT:         let %[[VALUE6:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE4]])));
// IR-NEXT:         let %[[VALUE_x:[0-9]+]] x: vla<i32, %[[VALUE6]]> [storage=automatic];
// IR-NEXT:         return mul<u64, overflow=wrap>(read<u64>(%[[VALUE6]]), const<u64>(4));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_typeof_without_effects:[0-9]+]] @typeof_without_effects(%[[VALUE_n_2:[0-9]+]] n: i32) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE7:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n_2]])));
// IR-NEXT:         let %[[VALUE_original:[0-9]+]] original: vla<i32, %[[VALUE7]]> [storage=automatic];
// IR-NEXT:         let %[[VALUE8:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n_2]])));
// IR-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<vla<i32, %[[VALUE8]]>> [storage=automatic] = null<ptr<vla<i32, %[[VALUE8]]>>>;
// IR-NEXT:         let %[[VALUE_copy:[0-9]+]] copy: vla<i32, %[[VALUE7]]> [storage=automatic];
// IR-NEXT:         return add<u64, overflow=wrap>(mul<u64, overflow=wrap>(read<u64>(%[[VALUE7]]), const<u64>(4)), mul<u64, overflow=wrap>(read<u64>(%[[VALUE8]]), const<u64>(4)));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_sizeof_typeof:[0-9]+]] @sizeof_typeof() -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_i_2:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// IR-NEXT:         let %[[VALUE9:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// IR-NEXT:         let %[[VALUE10:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE9]]), const<i32>(1));
// IR-NEXT:         write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE10]]));
// IR-NEXT:         let %[[VALUE11:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_i_2]])));
// IR-NEXT:         let %[[VALUE12:[0-9]+]]: ptr<i8> [synthetic] = array_decay<ptr<i8>, length=None>(deref(pointer_cast<ptr<vla<i8, %[[VALUE11]]>>, reason=explicit>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_a]]))));
// IR-NEXT:         return mul<u64, overflow=wrap>(read<u64>(%[[VALUE11]]), const<u64>(1));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_auto_type:[0-9]+]] @auto_type(%[[VALUE_p_2:[0-9]+]] p: ptr<i32>) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_q_2:[0-9]+]] q: ptr<vla<i32, %[[VALUE13:[0-9]+]]>> [storage=automatic];
// IR-NEXT:         let %[[VALUE13]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(call<i32, signature=fn() -> i32>(%[[VALUE_next]])));
// IR-NEXT:         write<ptr<vla<i32, %[[VALUE13]]>>>(%[[VALUE_q_2]], pointer_cast<ptr<vla<i32, %[[VALUE13]]>>, reason=explicit>(read<ptr<i32>>(%[[VALUE_p_2]])));
// IR-NEXT:         return mul<u64, overflow=wrap>(read<u64>(%[[VALUE13]]), const<u64>(4));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_statement_expression:[0-9]+]] @statement_expression(%[[VALUE_n_3:[0-9]+]] n: i32, %[[VALUE_p_3:[0-9]+]] p: ptr<i32>) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE14:[0-9]+]]: ptr<vla<i32, %[[VALUE15:[0-9]+]]>> [synthetic];
// IR-NEXT:         {
// IR-NEXT:             write<i32>(%[[VALUE_n_3]], const<i32>(20));
// IR-NEXT:             let %[[VALUE15]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n_3]])));
// IR-NEXT:             let %[[VALUE_r:[0-9]+]] r: ptr<vla<i32, %[[VALUE15]]>> [storage=automatic];
// IR-NEXT:             let %[[VALUE16:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n_3]])));
// IR-NEXT:             write<ptr<vla<i32, %[[VALUE15]]>>>(%[[VALUE_r]], pointer_cast<ptr<vla<i32, %[[VALUE15]]>>, reason=assign>(pointer_cast<ptr<vla<i32, %[[VALUE16]]>>, reason=explicit>(read<ptr<i32>>(%[[VALUE_p_3]]))));
// IR-NEXT:             write<ptr<vla<i32, %[[VALUE15]]>>>(%[[VALUE14]], read<ptr<vla<i32, %[[VALUE15]]>>>(%[[VALUE_r]]));
// IR-NEXT:         }
// IR-NEXT:         return mul<u64, overflow=wrap>(read<u64>(%[[VALUE15]]), const<u64>(4));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_evaluated_row:[0-9]+]] @evaluated_row(%[[VALUE_n_4:[0-9]+]] n: i32, %[[VALUE_i_3:[0-9]+]] i: i32) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE17:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n_4]])));
// IR-NEXT:         let %[[VALUE18:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n_4]])));
// IR-NEXT:         let %[[VALUE_m:[0-9]+]] m: vla<vla<i32, %[[VALUE18]]>, %[[VALUE17]]> [storage=automatic];
// IR-NEXT:         let %[[VALUE19:[0-9]+]]: i32 [synthetic, unsequenced] = read<i32>(%[[VALUE_i_3]]);
// IR-NEXT:         let %[[VALUE20:[0-9]+]]: i32 [synthetic, unsequenced] = add<i32, overflow=ub>(read<i32>(%[[VALUE19]]), const<i32>(1));
// IR-NEXT:         write<i32, unsequenced>(%[[VALUE_i_3]], read<i32>(%[[VALUE20]]));
// IR-NEXT:         return add<u64, overflow=wrap>(mul<u64, overflow=wrap>(read<u64>(%[[VALUE18]]), const<u64>(4)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i_3]]))));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
