// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -std=c23

int next(void);

unsigned long size(int n) { return sizeof(int[n]); }

unsigned long once(int n) { return sizeof(int[n++][next()]); }

unsigned long branch(int c, int n) { return c ? sizeof(int[n]) : 0; }

unsigned long pointer(int n) { return sizeof(int (*)[n]) + _Alignof(int[n][2]); }

int cast(int n, void *p) {
  int (*q)[n] = (int (*)[n])p;
  (void)(int (*)[next()])p;
  return q[1][2] + ((int (*)[n])p)[1][2];
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
// IR-NEXT:     fn %[[VALUE_next:[0-9]+]] @next() -> i32 [linkage=external];
// IR-NEXT:     fn %[[VALUE_size:[0-9]+]] @size(%[[VALUE_n:[0-9]+]] n: i32) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE0:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n]])));
// IR-NEXT:         return mul<u64, overflow=wrap>(read<u64>(%[[VALUE0]]), const<u64>(4));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_once:[0-9]+]] @once(%[[VALUE_n_2:[0-9]+]] n: i32) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_n_2]]);
// IR-NEXT:         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// IR-NEXT:         write<i32>(%[[VALUE_n_2]], read<i32>(%[[VALUE2]]));
// IR-NEXT:         let %[[VALUE3:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE1]])));
// IR-NEXT:         let %[[VALUE4:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(call<i32, signature=fn() -> i32>(%[[VALUE_next]])));
// IR-NEXT:         return mul<u64, overflow=wrap>(read<u64>(%[[VALUE3]]), mul<u64, overflow=wrap>(read<u64>(%[[VALUE4]]), const<u64>(4)));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_branch:[0-9]+]] @branch(%[[VALUE_c:[0-9]+]] c: i32, %[[VALUE_n_3:[0-9]+]] n: i32) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE5:[0-9]+]]: u64 [synthetic];
// IR-NEXT:         if ne<i32>(read<i32>(%[[VALUE_c]]), const<i32>(0))
// IR-NEXT:             let %[[VALUE6:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n_3]])));
// IR-NEXT:             write<u64>(%[[VALUE5]], mul<u64, overflow=wrap>(read<u64>(%[[VALUE6]]), const<u64>(4)));
// IR-NEXT:         else
// IR-NEXT:             write<u64>(%[[VALUE5]], reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))));
// IR-NEXT:         return read<u64>(%[[VALUE5]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_pointer:[0-9]+]] @pointer(%[[VALUE_n_4:[0-9]+]] n: i32) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<u64, overflow=wrap>(const<u64>(8), const<u64>(4));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_cast:[0-9]+]] @cast(%[[VALUE_n_5:[0-9]+]] n: i32, %[[VALUE_p:[0-9]+]] p: ptr<void>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE7:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n_5]])));
// IR-NEXT:         let %[[VALUE_q:[0-9]+]] q: ptr<vla<i32, %[[VALUE7]]>> [storage=automatic];
// IR-NEXT:         let %[[VALUE8:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n_5]])));
// IR-NEXT:         write<ptr<vla<i32, %[[VALUE7]]>>>(%[[VALUE_q]], pointer_cast<ptr<vla<i32, %[[VALUE7]]>>, reason=assign>(pointer_cast<ptr<vla<i32, %[[VALUE8]]>>, reason=explicit>(read<ptr<void>>(%[[VALUE_p]]))));
// IR-NEXT:         let %[[VALUE9:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(call<i32, signature=fn() -> i32>(%[[VALUE_next]])));
// IR-NEXT:         let %[[VALUE10:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n_5]])));
// IR-NEXT:         return add<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(deref(ptr_offset<ptr<vla<i32, %[[VALUE7]]>>, subtract=false, element=vla<i32, %[[VALUE7]]>, overflow=ub>(read<ptr<vla<i32, %[[VALUE7]]>>>(%[[VALUE_q]]), const<i32>(1)))), const<i32>(2)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(deref(ptr_offset<ptr<vla<i32, %[[VALUE10]]>>, subtract=false, element=vla<i32, %[[VALUE10]]>, overflow=ub>(pointer_cast<ptr<vla<i32, %[[VALUE10]]>>, reason=explicit>(read<ptr<void>>(%[[VALUE_p]])), const<i32>(1)))), const<i32>(2)))));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
