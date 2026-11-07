/* { dg-do compile } */
/* { dg-options "-std=gnu99" } */

/* These are crash tests related to PR middle-end/6994; see also
   g++.dg/ext/vla1.C.  Note that at present A and C cannot be inlined.  */

static inline void A (int i)
{
  struct S { int ar[1][i]; } s;

  s.ar[0][0] = 0;
}

void B(void)
{
  A(23);
}

static inline void C (int i)
{
  union U { int ar[1][i]; } u;

  u.ar[0][0] = 0;
}

void D(void)
{
  C(23);
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
// DEFAULT-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// DEFAULT-NEXT:         field0 ar: array<vla<i32, %[[VALUE0:[0-9]+]]>, 1>;
// DEFAULT-NEXT:     } [size=0, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_U:[0-9]+]] U = union {
// DEFAULT-NEXT:         field0 ar: array<vla<i32, %[[VALUE1:[0-9]+]]>, 1>;
// DEFAULT-NEXT:     } [size=0, align=4, offsets=[0]];
// DEFAULT-NEXT:     fn %[[VALUE_A:[0-9]+]] @A(%[[VALUE_i:[0-9]+]] i: i32) -> void [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE0]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_i]]))) [c="__size_t"];
// DEFAULT-NEXT:         let %[[VALUE_s:[0-9]+]] s: @type[[TYPE_S]] [storage=automatic];
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(deref(ptr_offset<ptr<vla<i32, %[[VALUE0]]>>, subtract=false, element=vla<i32, %[[VALUE0]]>, overflow=ub>(array_decay<ptr<vla<i32, %[[VALUE0]]>>, length=Some(1)>(field0(%[[VALUE_s]])), const<i32>(0)))), const<i32>(0))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_B:[0-9]+]] @B() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_A]], const<i32>(23));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_C:[0-9]+]] @C(%[[VALUE_i_2:[0-9]+]] i: i32) -> void [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE1]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_i_2]]))) [c="__size_t"];
// DEFAULT-NEXT:         let %[[VALUE_u:[0-9]+]] u: @type[[TYPE_U]] [storage=automatic];
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(deref(ptr_offset<ptr<vla<i32, %[[VALUE1]]>>, subtract=false, element=vla<i32, %[[VALUE1]]>, overflow=ub>(array_decay<ptr<vla<i32, %[[VALUE1]]>>, length=Some(1)>(field0(%[[VALUE_u]])), const<i32>(0)))), const<i32>(0))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_D:[0-9]+]] @D() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_C]], const<i32>(23));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
