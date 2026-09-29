/* { dg-do compile } */
/* { dg-require-weak "" } */
/* { dg-options "-fno-common" } */
/* { dg-skip-if "" { x86_64-*-mingw* } } */
/* NVPTX's weak is applied to the definition,  not declaration.  */
/* { dg-skip-if "" { nvptx-*-* } } */
/* { dg-skip-if PR119369 { amdgcn-*-* } } */

/* { dg-final { scan-weak "a" } } */
/* { dg-final { scan-not-weak "b" } } */
/* { dg-final { scan-weak "c" } } */
/* { dg-final { scan-weak "d" } } */

#pragma weak a
extern char a[];

char *user_a(void)
{
  return a+1;
}

int x;
int extern inline b(int y)
{
  return x+y;
}

extern int b(int y);

int user_b(int z)
{
  return b(z);
}

#pragma weak c
extern int c;

int *user_c = &c;

#pragma weak d
extern char d[];

char *user_d = &d[1];

// SLATE-FILECHECK-STD DEFAULT gnu23
// SLATE-FILECHECK-ARGS -fno-common
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
// DEFAULT-NEXT:     extern %[[VALUE_a:[0-9]+]] a: array<i8, incomplete> [storage=static] [linkage=external] [weak];
// DEFAULT-NEXT:     global %[[VALUE_x:[0-9]+]] x: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_c:[0-9]+]] c: i32 [storage=static] [linkage=external] [weak];
// DEFAULT-NEXT:     global %[[VALUE_user_c:[0-9]+]] user_c: ptr<i32> [storage=static] = addr_of<ptr<i32>>(%[[VALUE_c]]) [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_d:[0-9]+]] d: array<i8, incomplete> [storage=static] [linkage=external] [weak];
// DEFAULT-NEXT:     global %[[VALUE_user_d:[0-9]+]] user_d: ptr<i8> [storage=static] = addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=None>(%[[VALUE_d]]), const<i32>(1)))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_user_a:[0-9]+]] @user_a() -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=None>(%[[VALUE_a]]), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_b:[0-9]+]] @b(%[[VALUE_y:[0-9]+]] y: i32) -> i32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%[[VALUE_x]]), read<i32>(%[[VALUE_y]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_user_b:[0-9]+]] @user_b(%[[VALUE_z:[0-9]+]] z: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(i32) -> i32>(%[[VALUE_b]], read<i32>(%[[VALUE_z]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
