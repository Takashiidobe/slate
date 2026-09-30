/* Two of these types will, on current gcc targets, have the same
   mode but have different alias sets.  DOIT tries to get gcse to
   invalidly hoist one of the values out of the loop.  */

void abort(void);
void exit(int);

typedef int       T0;
typedef long      T1;
typedef long long T2;

int doit(int sel, int n, void *p) {
  T0 *const p0 = p;
  T1 *const p1 = p;
  T2 *const p2 = p;

  switch (sel) {
  case 0:
    do
      *p0 += *p0;
    while (--n);
    return *p0 == 0;

  case 1:
    do
      *p1 += *p1;
    while (--n);
    return *p1 == 0;

  case 2:
    do
      *p2 += *p2;
    while (--n);
    return *p2 == 0;

  default:
    abort();
  }
}

int main() {
  T0 v0;
  T1 v1;
  T2 v2;

  v0 = 1;
  doit(0, 5, &v0);
  v1 = 1;
  doit(1, 5, &v1);
  v2 = 1;
  doit(2, 5, &v2);

  if (v0 != 32)
    abort();
  if (v1 != 32)
    abort();
  if (v2 != 32)
    abort();

  exit(0);
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
// DEFAULT-NEXT:     type @type[[TYPE_T0:[0-9]+]] T0 = i32;
// DEFAULT-NEXT:     type @type[[TYPE_T1:[0-9]+]] T1 = i64;
// DEFAULT-NEXT:     type @type[[TYPE_T2:[0-9]+]] T2 = i64;
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_doit:[0-9]+]] @doit(%[[VALUE_sel:[0-9]+]] sel: i32, %[[VALUE_n:[0-9]+]] n: i32, %[[VALUE_p:[0-9]+]] p: ptr<void>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_p0:[0-9]+]] p0: ptr<i32> [storage=automatic] [const] = pointer_cast<ptr<i32>, reason=assign>(read<ptr<void>>(%[[VALUE_p]]));
// DEFAULT-NEXT:         let %[[VALUE_p1:[0-9]+]] p1: ptr<i64> [storage=automatic] [const] = pointer_cast<ptr<i64>, reason=assign>(read<ptr<void>>(%[[VALUE_p]]));
// DEFAULT-NEXT:         let %[[VALUE_p2:[0-9]+]] p2: ptr<i64> [storage=automatic] [const] = pointer_cast<ptr<i64>, reason=assign>(read<ptr<void>>(%[[VALUE_p]]));
// DEFAULT-NEXT:         switch %[[VALUE1:[0-9]+]] read<i32>(%[[VALUE_sel]])
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(0):
// DEFAULT-NEXT:                     do %[[VALUE2:[0-9]+]]
// DEFAULT-NEXT:                         let %[[VALUE3:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(%[[VALUE_p0]]);
// DEFAULT-NEXT:                         let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%[[VALUE3]])));
// DEFAULT-NEXT:                         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), read<i32>(deref(read<ptr<i32>>(%[[VALUE_p0]]))));
// DEFAULT-NEXT:                         write<i32>(deref(read<ptr<i32>>(%[[VALUE3]])), read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                     while {
// DEFAULT-NEXT:                         let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_n]]);
// DEFAULT-NEXT:                         let %[[VALUE7:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE6]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_n]], read<i32>(%[[VALUE7]]));
// DEFAULT-NEXT:                         yield ne<i32>(read<i32>(%[[VALUE7]]), const<i32>(0));
// DEFAULT-NEXT:                     };
// DEFAULT-NEXT:                 return from_bool<i32, reason=return>(eq<i32>(read<i32>(deref(read<ptr<i32>>(%[[VALUE_p0]]))), const<i32>(0)));
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(1):
// DEFAULT-NEXT:                     do %[[VALUE8:[0-9]+]]
// DEFAULT-NEXT:                         let %[[VALUE9:[0-9]+]]: ptr<i64> [synthetic] = read<ptr<i64>>(%[[VALUE_p1]]);
// DEFAULT-NEXT:                         let %[[VALUE10:[0-9]+]]: i64 [synthetic] = read<i64>(deref(read<ptr<i64>>(%[[VALUE9]])));
// DEFAULT-NEXT:                         let %[[VALUE11:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE10]]), read<i64>(deref(read<ptr<i64>>(%[[VALUE_p1]]))));
// DEFAULT-NEXT:                         write<i64>(deref(read<ptr<i64>>(%[[VALUE9]])), read<i64>(%[[VALUE11]]));
// DEFAULT-NEXT:                     while {
// DEFAULT-NEXT:                         let %[[VALUE12:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_n]]);
// DEFAULT-NEXT:                         let %[[VALUE13:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE12]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_n]], read<i32>(%[[VALUE13]]));
// DEFAULT-NEXT:                         yield ne<i32>(read<i32>(%[[VALUE13]]), const<i32>(0));
// DEFAULT-NEXT:                     };
// DEFAULT-NEXT:                 return from_bool<i32, reason=return>(eq<i64>(read<i64>(deref(read<ptr<i64>>(%[[VALUE_p1]]))), widen<i64, reason=usual_arith>(const<i32>(0))));
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(2):
// DEFAULT-NEXT:                     do %[[VALUE14:[0-9]+]]
// DEFAULT-NEXT:                         let %[[VALUE15:[0-9]+]]: ptr<i64> [synthetic] = read<ptr<i64>>(%[[VALUE_p2]]);
// DEFAULT-NEXT:                         let %[[VALUE16:[0-9]+]]: i64 [synthetic] = read<i64>(deref(read<ptr<i64>>(%[[VALUE15]])));
// DEFAULT-NEXT:                         let %[[VALUE17:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE16]]), read<i64>(deref(read<ptr<i64>>(%[[VALUE_p2]]))));
// DEFAULT-NEXT:                         write<i64>(deref(read<ptr<i64>>(%[[VALUE15]])), read<i64>(%[[VALUE17]]));
// DEFAULT-NEXT:                     while {
// DEFAULT-NEXT:                         let %[[VALUE18:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_n]]);
// DEFAULT-NEXT:                         let %[[VALUE19:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE18]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_n]], read<i32>(%[[VALUE19]]));
// DEFAULT-NEXT:                         yield ne<i32>(read<i32>(%[[VALUE19]]), const<i32>(0));
// DEFAULT-NEXT:                     };
// DEFAULT-NEXT:                 return from_bool<i32, reason=return>(eq<i64>(read<i64>(deref(read<ptr<i64>>(%[[VALUE_p2]]))), widen<i64, reason=usual_arith>(const<i32>(0))));
// DEFAULT-NEXT:                 default %[[VALUE1]]:
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_v0:[0-9]+]] v0: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_v1:[0-9]+]] v1: i64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_v2:[0-9]+]] v2: i64 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%[[VALUE_v0]], const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, i32, ptr<void>) -> i32>(%[[VALUE_doit]], const<i32>(0), const<i32>(5), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i32>>(%[[VALUE_v0]])));
// DEFAULT-NEXT:         write<i64>(%[[VALUE_v1]], widen<i64, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, i32, ptr<void>) -> i32>(%[[VALUE_doit]], const<i32>(1), const<i32>(5), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i64>>(%[[VALUE_v1]])));
// DEFAULT-NEXT:         write<i64>(%[[VALUE_v2]], widen<i64, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, i32, ptr<void>) -> i32>(%[[VALUE_doit]], const<i32>(2), const<i32>(5), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i64>>(%[[VALUE_v2]])));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_v0]]), const<i32>(32))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%[[VALUE_v1]]), widen<i64, reason=usual_arith>(const<i32>(32)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%[[VALUE_v2]]), widen<i64, reason=usual_arith>(const<i32>(32)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
