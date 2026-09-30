/* Test for VLA size evaluation; see PR 35198.  */
/* Origin: Joseph Myers <joseph@codesourcery.com> */
/* { dg-do run } */
/* { dg-options "-std=c99" } */

extern void exit (int);
extern void abort (void);

int i;
void *p;

void
f1 (void *x, int j)
{
  p = (int (*)[++i])x;
  if (i != j)
    abort ();
}

void
f1c (void *x, int j)
{
  p = (int (*)[++i]){x};
  if (i != j)
    abort ();
}

void
f2 (void *x, int j)
{
  x = (void *)(int (*)[++i])p;
  if (i != j)
    abort ();
}

void
f2c (void *x, int j)
{
  x = (void *)(int (*)[++i]){p};
  if (i != j)
    abort ();
}

void
f3 (void *x, int j)
{
  (void)(int (*)[++i])p;
  if (i != j)
    abort ();
}

void
f3c (void *x, int j)
{
  (void)(int (*)[++i]){p};
  if (i != j)
    abort ();
}

void
f4 (void *x, int j)
{
  (int (*)[++i])p;
  (int (*)[++i])p;
  if (i != j)
    abort ();
}

void
f4c (void *x, int j)
{
  (int (*)[++i]){p};
  (int (*)[++i]){p};
  if (i != j)
    abort ();
}

void
f5c (void *x, int j, int k)
{
  (++i, f3c (x, j), (int (*)[++i]){p});
  if (i != k)
    abort ();
}

int
main (void)
{
  f1 (p, 1);
  f2 (p, 2);
  f3 (p, 3);
  f4 (p, 5);
  f1c (p, 6);
  f2c (p, 7);
  f3c (p, 8);
  f4c (p, 10);
  f5c (p, 12, 13);
  exit (0);
}

// SLATE-FILECHECK-STD DEFAULT c99
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
// DEFAULT-NEXT:     global %[[VALUE_i:[0-9]+]] i: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_p:[0-9]+]] p: ptr<void> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_f1:[0-9]+]] @f1(%[[VALUE_x:[0-9]+]] x: ptr<void>, %[[VALUE_j:[0-9]+]] j: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE2]])));
// DEFAULT-NEXT:         write<ptr<void>>(%[[VALUE_p]], pointer_cast<ptr<void>, reason=assign>(pointer_cast<ptr<vla<i32, %[[VALUE3]]>>, reason=explicit>(read<ptr<void>>(%[[VALUE_x]]))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_j]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f1c:[0-9]+]] @f1c(%[[VALUE_x_2:[0-9]+]] x: ptr<void>, %[[VALUE_j_2:[0-9]+]] j: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE5]])));
// DEFAULT-NEXT:         write<ptr<void>>(%[[VALUE_p]], pointer_cast<ptr<void>, reason=assign>(read<ptr<vla<i32, %[[VALUE6]]>>>(compound_literal %[[VALUE7:[0-9]+]] [storage=automatic] = pointer_cast<ptr<vla<i32, %[[VALUE6]]>>, reason=assign>(read<ptr<void>>(%[[VALUE_x_2]])))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_j_2]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f2:[0-9]+]] @f2(%[[VALUE_x_3:[0-9]+]] x: ptr<void>, %[[VALUE_j_3:[0-9]+]] j: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE8]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE9]]));
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE9]])));
// DEFAULT-NEXT:         write<ptr<void>>(%[[VALUE_x_3]], pointer_cast<ptr<void>, reason=explicit>(pointer_cast<ptr<vla<i32, %[[VALUE10]]>>, reason=explicit>(read<ptr<void>>(%[[VALUE_p]]))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_j_3]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f2c:[0-9]+]] @f2c(%[[VALUE_x_4:[0-9]+]] x: ptr<void>, %[[VALUE_j_4:[0-9]+]] j: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE11]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE12]]));
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE12]])));
// DEFAULT-NEXT:         write<ptr<void>>(%[[VALUE_x_4]], pointer_cast<ptr<void>, reason=explicit>(read<ptr<vla<i32, %[[VALUE13]]>>>(compound_literal %[[VALUE14:[0-9]+]] [storage=automatic] = pointer_cast<ptr<vla<i32, %[[VALUE13]]>>, reason=assign>(read<ptr<void>>(%[[VALUE_p]])))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_j_4]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f3:[0-9]+]] @f3(%[[VALUE_x_5:[0-9]+]] x: ptr<void>, %[[VALUE_j_5:[0-9]+]] j: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE15:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:         let %[[VALUE16:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE15]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE16]]));
// DEFAULT-NEXT:         let %[[VALUE17:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE16]])));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_j_5]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f3c:[0-9]+]] @f3c(%[[VALUE_x_6:[0-9]+]] x: ptr<void>, %[[VALUE_j_6:[0-9]+]] j: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE18:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:         let %[[VALUE19:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE18]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE19]]));
// DEFAULT-NEXT:         let %[[VALUE20:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE19]])));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_j_6]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f4:[0-9]+]] @f4(%[[VALUE_x_7:[0-9]+]] x: ptr<void>, %[[VALUE_j_7:[0-9]+]] j: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE21:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:         let %[[VALUE22:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE21]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE22]]));
// DEFAULT-NEXT:         let %[[VALUE23:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE22]])));
// DEFAULT-NEXT:         let %[[VALUE24:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:         let %[[VALUE25:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE24]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE25]]));
// DEFAULT-NEXT:         let %[[VALUE26:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE25]])));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_j_7]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f4c:[0-9]+]] @f4c(%[[VALUE_x_8:[0-9]+]] x: ptr<void>, %[[VALUE_j_8:[0-9]+]] j: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE27:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:         let %[[VALUE28:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE27]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE28]]));
// DEFAULT-NEXT:         let %[[VALUE29:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE28]])));
// DEFAULT-NEXT:         let %[[VALUE30:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:         let %[[VALUE31:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE30]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE31]]));
// DEFAULT-NEXT:         let %[[VALUE32:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE31]])));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_j_8]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f5c:[0-9]+]] @f5c(%[[VALUE_x_9:[0-9]+]] x: ptr<void>, %[[VALUE_j_9:[0-9]+]] j: i32, %[[VALUE_k:[0-9]+]] k: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE33:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:         let %[[VALUE34:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE33]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE34]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, i32) -> void>(%[[VALUE_f3c]], read<ptr<void>>(%[[VALUE_x_9]]), read<i32>(%[[VALUE_j_9]]));
// DEFAULT-NEXT:         let %[[VALUE35:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:         let %[[VALUE36:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE35]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE36]]));
// DEFAULT-NEXT:         let %[[VALUE37:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE36]])));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_k]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, i32) -> void>(%[[VALUE_f1]], read<ptr<void>>(%[[VALUE_p]]), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, i32) -> void>(%[[VALUE_f2]], read<ptr<void>>(%[[VALUE_p]]), const<i32>(2));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, i32) -> void>(%[[VALUE_f3]], read<ptr<void>>(%[[VALUE_p]]), const<i32>(3));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, i32) -> void>(%[[VALUE_f4]], read<ptr<void>>(%[[VALUE_p]]), const<i32>(5));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, i32) -> void>(%[[VALUE_f1c]], read<ptr<void>>(%[[VALUE_p]]), const<i32>(6));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, i32) -> void>(%[[VALUE_f2c]], read<ptr<void>>(%[[VALUE_p]]), const<i32>(7));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, i32) -> void>(%[[VALUE_f3c]], read<ptr<void>>(%[[VALUE_p]]), const<i32>(8));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, i32) -> void>(%[[VALUE_f4c]], read<ptr<void>>(%[[VALUE_p]]), const<i32>(10));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, i32, i32) -> void>(%[[VALUE_f5c]], read<ptr<void>>(%[[VALUE_p]]), const<i32>(12), const<i32>(13));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
