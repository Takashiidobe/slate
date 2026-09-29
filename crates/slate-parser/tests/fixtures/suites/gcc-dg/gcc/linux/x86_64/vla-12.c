/* Test for typeof evaluation: should be at the appropriate point in
   the containing expression rather than just adding a statement.  */
/* Origin: Joseph Myers <joseph@codesourcery.com> */
/* { dg-do run } */
/* { dg-options "-std=gnu99" } */

extern void exit (int);
extern void abort (void);

void *p;

void
f1 (void)
{
  int i = 0, j = -1, k = -1;
  /* typeof applied to expression with cast.  */
  (j = ++i), (void)(typeof ((int (*)[(k = ++i)])p))p;
  if (j != 1 || k != 2 || i != 2)
    abort ();
}

void
f2 (void)
{
  int i = 0, j = -1, k = -1;
  /* typeof applied to type.  */
  (j = ++i), (void)(typeof (int (*)[(k = ++i)]))p;
  if (j != 1 || k != 2 || i != 2)
    abort ();
}

void
f3 (void)
{
  int i = 0, j = -1, k = -1;
  void *q;
  /* typeof applied to expression with cast that is used.  */
  (j = ++i), (void)((typeof (1 + (int (*)[(k = ++i)])p))p);
  if (j != 1 || k != 2 || i != 2)
    abort ();
}

int
main (void)
{
  f1 ();
  f2 ();
  f3 ();
  exit (0);
}

// SLATE-FILECHECK-STD DEFAULT gnu99
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
// DEFAULT-NEXT:     global %[[VALUE_p:[0-9]+]] p: ptr<void> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_f1:[0-9]+]] @f1() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE_k:[0-9]+]] k: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_j]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE3]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_k]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE4]])));
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: ptr<vla<i32, %[[VALUE5]]>> [synthetic] = pointer_cast<ptr<vla<i32, %[[VALUE5]]>>, reason=explicit>(read<ptr<void>>(%[[VALUE_p]]));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(%[[VALUE_j]]), const<i32>(1)), ne<i32>(read<i32>(%[[VALUE_k]]), const<i32>(2))), ne<i32>(read<i32>(%[[VALUE_i]]), const<i32>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f2:[0-9]+]] @f2() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i_2:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_j_2:[0-9]+]] j: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE_k_2:[0-9]+]] k: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE7]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE8]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_j_2]], read<i32>(%[[VALUE8]]));
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE9]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE10]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_k_2]], read<i32>(%[[VALUE10]]));
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE10]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(%[[VALUE_j_2]]), const<i32>(1)), ne<i32>(read<i32>(%[[VALUE_k_2]]), const<i32>(2))), ne<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f3:[0-9]+]] @f3() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i_3:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_j_3:[0-9]+]] j: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE_k_3:[0-9]+]] k: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE_q:[0-9]+]] q: ptr<void> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_3]]);
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE12]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i_3]], read<i32>(%[[VALUE13]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_j_3]], read<i32>(%[[VALUE13]]));
// DEFAULT-NEXT:         let %[[VALUE14:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_3]]);
// DEFAULT-NEXT:         let %[[VALUE15:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE14]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i_3]], read<i32>(%[[VALUE15]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_k_3]], read<i32>(%[[VALUE15]]));
// DEFAULT-NEXT:         let %[[VALUE16:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE15]])));
// DEFAULT-NEXT:         let %[[VALUE17:[0-9]+]]: ptr<vla<i32, %[[VALUE16]]>> [synthetic] = ptr_offset<ptr<vla<i32, %[[VALUE16]]>>, subtract=false, element=vla<i32, %[[VALUE16]]>, overflow=ub>(pointer_cast<ptr<vla<i32, %[[VALUE16]]>>, reason=explicit>(read<ptr<void>>(%[[VALUE_p]])), const<i32>(1));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(%[[VALUE_j_3]]), const<i32>(1)), ne<i32>(read<i32>(%[[VALUE_k_3]]), const<i32>(2))), ne<i32>(read<i32>(%[[VALUE_i_3]]), const<i32>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_f1]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_f2]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_f3]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
