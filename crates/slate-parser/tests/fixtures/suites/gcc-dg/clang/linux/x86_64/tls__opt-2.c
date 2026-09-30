/* This testcase generated invalid assembly on IA-32,
   since %gs:0 memory load was not exposed to the compiler
   as memory load and mem to mem moves are not possible
   on IA-32.  */
/* { dg-do link } */
/* { dg-options "-O2 -ftls-model=initial-exec" } */
/* { dg-options "-O2 -ftls-model=initial-exec -march=i686" { target { { i?86-*-* x86_64-*-* } && ia32 } } } */
/* { dg-require-effective-target tls } */
/* { dg-require-effective-target tls_runtime  } */

__thread int thr;

struct A
{
  unsigned int a, b, c, d, e;
};

int bar (int x, unsigned long y, void *z)
{
  return 0;
}

int
foo (int x, int y, const struct A *z)
{
  struct A b;
  int d;

  b = *z;
  d = bar (x, y, &b);
  if (d == 0 && y == 0x5402)
    {
      int e = thr;
      d = bar (x, 0x5401, &b);
      if (d)
	{
	  thr = e;
	  d = 0;
	}
      else if ((z->c & 0600) != (b.c & 0600)
	       || ((z->c & 060) && ((z->c & 060) != (b.c & 060))))
	{
	  thr = 22;
	  d = -1;
	}
    }

  return d;
}

int main (void)
{
  foo (1, 2, 0);
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
// DEFAULT-NEXT:     type @type[[TYPE_A:[0-9]+]] A = struct {
// DEFAULT-NEXT:         field0 a: u32;
// DEFAULT-NEXT:         field1 b: u32;
// DEFAULT-NEXT:         field2 c: u32;
// DEFAULT-NEXT:         field3 d: u32;
// DEFAULT-NEXT:         field4 e: u32;
// DEFAULT-NEXT:     } [size=20, align=4, offsets=[0, 4, 8, 12, 16]];
// DEFAULT-NEXT:     global %[[VALUE_thr:[0-9]+]] thr: i32 [storage=thread] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_x:[0-9]+]] x: i32, %[[VALUE_y:[0-9]+]] y: u64, %[[VALUE_z:[0-9]+]] z: ptr<void>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x_2:[0-9]+]] x: i32, %[[VALUE_y_2:[0-9]+]] y: i32, %[[VALUE_z_2:[0-9]+]] z: ptr<const @type[[TYPE_A]]>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: @type[[TYPE_A]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_d:[0-9]+]] d: i32 [storage=automatic];
// DEFAULT-NEXT:         write<@type[[TYPE_A]]>(%[[VALUE_b]], copy<@type[[TYPE_A]], reason=assign>(read<@type[[TYPE_A]]>(deref(read<ptr<const @type[[TYPE_A]]>>(%[[VALUE_z_2]])))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_d]], call<i32, signature=fn(i32, u64, ptr<void>) -> i32>(%[[VALUE_bar]], read<i32>(%[[VALUE_x_2]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_y_2]]))), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type[[TYPE_A]]>>(%[[VALUE_b]]))));
// DEFAULT-NEXT:         if logical_and<bool>(eq<i32>(read<i32>(%[[VALUE_d]]), const<i32>(0)), eq<i32>(read<i32>(%[[VALUE_y_2]]), const<i32>(21506)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_e:[0-9]+]] e: i32 [storage=automatic] = read<i32>(%[[VALUE_thr]]);
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_d]], call<i32, signature=fn(i32, u64, ptr<void>) -> i32>(%[[VALUE_bar]], read<i32>(%[[VALUE_x_2]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(21505))), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type[[TYPE_A]]>>(%[[VALUE_b]]))));
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%[[VALUE_d]]), const<i32>(0))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_thr]], read<i32>(%[[VALUE_e]]));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_d]], const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     if logical_or<bool>(ne<u32>(and<u32>(read<u32>(field2(deref(read<ptr<const @type[[TYPE_A]]>>(%[[VALUE_z_2]])))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(384))), and<u32>(read<u32>(field2(%[[VALUE_b]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(384)))), logical_and<bool>(ne<u32>(and<u32>(read<u32>(field2(deref(read<ptr<const @type[[TYPE_A]]>>(%[[VALUE_z_2]])))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(48))), const<u32>(0)), ne<u32>(and<u32>(read<u32>(field2(deref(read<ptr<const @type[[TYPE_A]]>>(%[[VALUE_z_2]])))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(48))), and<u32>(read<u32>(field2(%[[VALUE_b]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(48))))))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_thr]], const<i32>(22));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_d]], neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_d]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(i32, i32, ptr<const @type[[TYPE_A]]>) -> i32>(%[[VALUE_foo]], const<i32>(1), const<i32>(2), null<ptr<const @type[[TYPE_A]]>>);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
