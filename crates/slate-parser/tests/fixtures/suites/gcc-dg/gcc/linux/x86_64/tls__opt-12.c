/* PR target/29198 */
/* { dg-do run } */
/* { dg-options "-O2 -fpic" } */
/* { dg-require-effective-target tls_runtime } */
/* { dg-add-options tls } */
/* { dg-require-effective-target fpic } */

extern void abort (void);

int f2 (int, int, int, int);
struct s { char b[4]; };
__thread struct s thra[2];

void
__attribute__((noinline))
f1 (int a1, int a2)
{
  int i, j;
  for (i = 0; i < 4; i++)
    {
      int tot = 0;
      for (j = 0; j < 4; j++)
	tot += f2 (a1, a2, i, j);
      *(&thra[0].b[0] + i) = tot;
    }
}

int
__attribute__((noinline))
f2 (int a, int b, int c, int d)
{
  return a + b + c + d;
}

int
main (void)
{
  f1 (0, 0);
  if (thra[0].b[0] != 6
      || thra[0].b[1] != 10
      || thra[0].b[2] != 14
      || thra[0].b[3] != 18)
    abort ();
  f1 (2, 3);
  if (thra[0].b[0] != 26
      || thra[0].b[1] != 30
      || thra[0].b[2] != 34
      || thra[0].b[3] != 38)
    abort ();
  return 0;
}

// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     type @type[[TYPE_s:[0-9]+]] s = struct {
// DEFAULT-NEXT:         field0 b: array<i8, 4>;
// DEFAULT-NEXT:     } [size=4, align=1, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_thra:[0-9]+]] thra: array<@type[[TYPE_s]], 2> [storage=thread] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_f2:[0-9]+]] @f2(%[[VALUE_a:[0-9]+]] a: i32, %[[VALUE_b:[0-9]+]] b: i32, %[[VALUE_c:[0-9]+]] c: i32, %[[VALUE_d:[0-9]+]] d: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]])), read<i32>(%[[VALUE_c]])), read<i32>(%[[VALUE_d]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f1:[0-9]+]] @f1(%[[VALUE_a1:[0-9]+]] a1: i32, %[[VALUE_a2:[0-9]+]] a2: i32) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_tot:[0-9]+]] tot: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                     for %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_j]], const<i32>(0));
// DEFAULT-NEXT:                         condition: lt<i32>(read<i32>(%[[VALUE_j]]), const<i32>(4))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j]]);
// DEFAULT-NEXT:                             let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_j]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_tot]]);
// DEFAULT-NEXT:                             let %[[VALUE7:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE6]]), call<i32, signature=fn(i32, i32, i32, i32) -> i32>(%[[VALUE_f2]], read<i32>(%[[VALUE_a1]]), read<i32>(%[[VALUE_a2]]), read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_j]])));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_tot]], read<i32>(%[[VALUE7]]));
// DEFAULT-NEXT:                     write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(field0(deref(ptr_offset<ptr<@type[[TYPE_s]]>, subtract=false, element=@type[[TYPE_s]], overflow=ub>(array_decay<ptr<@type[[TYPE_s]]>, length=Some(2)>(%[[VALUE_thra]]), const<i32>(0))))), const<i32>(0)))), read<i32>(%[[VALUE_i]]))), truncate<i8, reason=assign, fits=unknown>(read<i32>(%[[VALUE_tot]])));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_f1]], const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>,
// DEFAULT-SAME: length=Some(4)>(field0(deref(ptr_offset<ptr<@type[[TYPE_s]]>, subtract=false,
// DEFAULT-SAME: element=@type[[TYPE_s]],
// DEFAULT-SAME: overflow=ub>(array_decay<ptr<@type[[TYPE_s]]>,
// DEFAULT-SAME: length=Some(2)>(%[[VALUE_thra]]), const<i32>(0))))), const<i32>(0))))), const<i32>(6)), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>,
// DEFAULT-SAME: subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(field0(deref(ptr_offset<ptr<@type[[TYPE_s]]>, subtract=false,
// DEFAULT-SAME: element=@type[[TYPE_s]],
// DEFAULT-SAME: overflow=ub>(array_decay<ptr<@type[[TYPE_s]]>,
// DEFAULT-SAME: length=Some(2)>(%[[VALUE_thra]]), const<i32>(0))))), const<i32>(1))))), const<i32>(10))), ne<i32>(widen<i32,
// DEFAULT-SAME: reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>,
// DEFAULT-SAME: length=Some(4)>(field0(deref(ptr_offset<ptr<@type[[TYPE_s]]>, subtract=false,
// DEFAULT-SAME: element=@type[[TYPE_s]],
// DEFAULT-SAME: overflow=ub>(array_decay<ptr<@type[[TYPE_s]]>,
// DEFAULT-SAME: length=Some(2)>(%[[VALUE_thra]]), const<i32>(0))))), const<i32>(2))))), const<i32>(14))), ne<i32>(widen<i32,
// DEFAULT-SAME: reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>,
// DEFAULT-SAME: length=Some(4)>(field0(deref(ptr_offset<ptr<@type[[TYPE_s]]>, subtract=false,
// DEFAULT-SAME: element=@type[[TYPE_s]],
// DEFAULT-SAME: overflow=ub>(array_decay<ptr<@type[[TYPE_s]]>,
// DEFAULT-SAME: length=Some(2)>(%[[VALUE_thra]]), const<i32>(0))))), const<i32>(3))))), const<i32>(18)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_f1]], const<i32>(2), const<i32>(3));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>,
// DEFAULT-SAME: length=Some(4)>(field0(deref(ptr_offset<ptr<@type[[TYPE_s]]>, subtract=false,
// DEFAULT-SAME: element=@type[[TYPE_s]],
// DEFAULT-SAME: overflow=ub>(array_decay<ptr<@type[[TYPE_s]]>,
// DEFAULT-SAME: length=Some(2)>(%[[VALUE_thra]]), const<i32>(0))))), const<i32>(0))))), const<i32>(26)), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>,
// DEFAULT-SAME: subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(field0(deref(ptr_offset<ptr<@type[[TYPE_s]]>, subtract=false,
// DEFAULT-SAME: element=@type[[TYPE_s]],
// DEFAULT-SAME: overflow=ub>(array_decay<ptr<@type[[TYPE_s]]>,
// DEFAULT-SAME: length=Some(2)>(%[[VALUE_thra]]), const<i32>(0))))), const<i32>(1))))), const<i32>(30))), ne<i32>(widen<i32,
// DEFAULT-SAME: reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>,
// DEFAULT-SAME: length=Some(4)>(field0(deref(ptr_offset<ptr<@type[[TYPE_s]]>, subtract=false,
// DEFAULT-SAME: element=@type[[TYPE_s]],
// DEFAULT-SAME: overflow=ub>(array_decay<ptr<@type[[TYPE_s]]>,
// DEFAULT-SAME: length=Some(2)>(%[[VALUE_thra]]), const<i32>(0))))), const<i32>(2))))), const<i32>(34))), ne<i32>(widen<i32,
// DEFAULT-SAME: reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>,
// DEFAULT-SAME: length=Some(4)>(field0(deref(ptr_offset<ptr<@type[[TYPE_s]]>, subtract=false,
// DEFAULT-SAME: element=@type[[TYPE_s]],
// DEFAULT-SAME: overflow=ub>(array_decay<ptr<@type[[TYPE_s]]>,
// DEFAULT-SAME: length=Some(2)>(%[[VALUE_thra]]), const<i32>(0))))), const<i32>(3))))), const<i32>(38)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
