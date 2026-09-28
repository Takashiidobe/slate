/* PR middle-end/83945 */
/* { dg-do compile { target tls } } */
/* { dg-options "-O2" } */

struct S { int a[1]; };
__thread struct T { int c; } e;
int f;
void bar (int);

void
foo (int f, int x)
{
  struct S *h = (struct S *) &e.c;
  for (;;)
    {
      int *a = h->a, i;
      for (i = x; i; i--)
	bar (a[f]);
      bar (a[f]);
    }
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
// DEFAULT-NEXT:     type @type0 S = struct {
// DEFAULT-NEXT:         field0 a: array<i32, 1>;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type1 T = struct {
// DEFAULT-NEXT:         field0 c: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     global %2 e: @type1 [storage=thread] [linkage=external];
// DEFAULT-NEXT:     global %3 f: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %4 @bar(%11 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %5 @foo(%6 f: i32, %7 x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %8 h: ptr<@type0> [storage=automatic] = pointer_cast<ptr<@type0>, reason=explicit>(addr_of<ptr<i32>>(field0(%2)));
// DEFAULT-NEXT:         for %12
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: omitted
// DEFAULT-NEXT:             increment: omitted
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %9 a: ptr<i32> [storage=automatic] = array_decay<ptr<i32>, length=Some(1)>(field0(deref(read<ptr<@type0>>(%8))));
// DEFAULT-NEXT:                     let %10 i: i32 [storage=automatic];
// DEFAULT-NEXT:                     for %13
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             write<i32>(%10, read<i32>(%7));
// DEFAULT-NEXT:                         condition: ne<i32>(read<i32>(%10), const<i32>(0))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %14: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                             let %15: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%14), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%10, read<i32>(%15));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             call<void, signature=fn(i32) -> void>(%4, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%9), read<i32>(%6)))));
// DEFAULT-NEXT:                     call<void, signature=fn(i32) -> void>(%4, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%9), read<i32>(%6)))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
