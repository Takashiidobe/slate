/* { dg-do compile } */
/* { dg-options "-O2" } */
/* { dg-require-effective-target tls_native } */

struct A
{
  int a1;
  int a2;
};

extern __thread const unsigned char *tcc1, **tcc2;

extern inline const unsigned char ** __attribute__ ((const))
foo (void)
{
  const unsigned char **a = &tcc1;
  if (*a == 0)
    *a = *tcc2 + 128;
  return a;
}

extern inline int
bar (const struct A *x)
{
  int a;

  if (x->a2 & 8)
    return 0;
  a = x->a1;
  return a > 0 && ((*foo ())[a] & 64);
}

int
baz (const struct A *x, char *y)
{
  const struct A *a;

  for (a = x; !!a->a1; a++)
    if (! (x->a2 & 8))
      if (bar (a))
	{
	  *y++ = a->a1;
	  if (x->a1)
	    *y++ = ':';
	  *y = '\0';
	}
  return 0;
}

/* Verify tcc1 and tcc2 variables show up only in the TLS access sequences.  */
/* { dg-final { scan-assembler "tcc1@" { target i?86-*-* x86_64-*-* } } } */
/* { dg-final { scan-assembler "tcc2@" { target i?86-*-* x86_64-*-* } } } */
/* { dg-final { scan-assembler-not "tcc1\[^@\]" { target i?86-*-* x86_64-*-* } } } */
/* { dg-final { scan-assembler-not "tcc2\[^@\]" { target i?86-*-* x86_64-*-* } } } */

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
// DEFAULT-NEXT:         field0 a1: i32;
// DEFAULT-NEXT:         field1 a2: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     extern %[[VALUE_tcc1:[0-9]+]] tcc1: ptr<const u8> [storage=thread] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_tcc2:[0-9]+]] tcc2: ptr<ptr<const u8>> [storage=thread] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> ptr<ptr<const u8>> [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: ptr<ptr<const u8>> [storage=automatic] = addr_of<ptr<ptr<const u8>>>(%[[VALUE_tcc1]]);
// DEFAULT-NEXT:         if eq<ptr<const u8>>(read<ptr<const u8>>(deref(read<ptr<ptr<const u8>>>(%[[VALUE_a]]))), null<ptr<const u8>>)
// DEFAULT-NEXT:             write<ptr<const u8>>(deref(read<ptr<ptr<const u8>>>(%[[VALUE_a]])), ptr_offset<ptr<const u8>, subtract=false, element=u8, overflow=ub>(read<ptr<const u8>>(deref(read<ptr<ptr<const u8>>>(%[[VALUE_tcc2]]))), const<i32>(128)));
// DEFAULT-NEXT:         return read<ptr<ptr<const u8>>>(%[[VALUE_a]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_x:[0-9]+]] x: ptr<const @type[[TYPE_A]]>) -> i32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_a_2:[0-9]+]] a: i32 [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(and<i32>(read<i32>(field1(deref(read<ptr<const @type[[TYPE_A]]>>(%[[VALUE_x]])))), const<i32>(8)), const<i32>(0))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_a_2]], read<i32>(field0(deref(read<ptr<const @type[[TYPE_A]]>>(%[[VALUE_x]])))));
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if gt<i32>(read<i32>(%[[VALUE_a_2]]), const<i32>(0))
// DEFAULT-NEXT:             write<bool>(%[[VALUE0]], ne<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<const u8>, subtract=false, element=u8, overflow=ub>(read<ptr<const u8>>(deref(call<ptr<ptr<const u8>>, signature=fn() -> ptr<ptr<const u8>>>(%[[VALUE_foo]]))), read<i32>(%[[VALUE_a_2]])))))), const<i32>(64)), const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE0]], const<bool>(false));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(read<bool>(%[[VALUE0]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_baz:[0-9]+]] @baz(%[[VALUE_x_2:[0-9]+]] x: ptr<const @type[[TYPE_A]]>, %[[VALUE_y:[0-9]+]] y: ptr<i8>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_a_3:[0-9]+]] a: ptr<const @type[[TYPE_A]]> [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<ptr<const @type[[TYPE_A]]>>(%[[VALUE_a_3]], read<ptr<const @type[[TYPE_A]]>>(%[[VALUE_x_2]]));
// DEFAULT-NEXT:             condition: not<bool>(not<bool>(ne<i32>(read<i32>(field0(deref(read<ptr<const @type[[TYPE_A]]>>(%[[VALUE_a_3]])))), const<i32>(0))))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: ptr<const @type[[TYPE_A]]> [synthetic] = read<ptr<const @type[[TYPE_A]]>>(%[[VALUE_a_3]]);
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: ptr<const @type[[TYPE_A]]> [synthetic] = ptr_offset<ptr<const @type[[TYPE_A]]>, subtract=false, element=@type[[TYPE_A]], overflow=ub>(read<ptr<const @type[[TYPE_A]]>>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<const @type[[TYPE_A]]>>(%[[VALUE_a_3]], read<ptr<const @type[[TYPE_A]]>>(%[[VALUE3]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(and<i32>(read<i32>(field1(deref(read<ptr<const @type[[TYPE_A]]>>(%[[VALUE_x_2]])))), const<i32>(8)), const<i32>(0)))
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(ptr<const @type[[TYPE_A]]>) -> i32>(%[[VALUE_bar]], read<ptr<const @type[[TYPE_A]]>>(%[[VALUE_a_3]])), const<i32>(0))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             let %[[VALUE4:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_y]]);
// DEFAULT-NEXT:                             let %[[VALUE5:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                             write<ptr<i8>>(%[[VALUE_y]], read<ptr<i8>>(%[[VALUE5]]));
// DEFAULT-NEXT:                             write<i8>(deref(read<ptr<i8>>(%[[VALUE4]])), truncate<i8, reason=assign, fits=unknown>(read<i32>(field0(deref(read<ptr<const @type[[TYPE_A]]>>(%[[VALUE_a_3]]))))));
// DEFAULT-NEXT:                             if ne<i32>(read<i32>(field0(deref(read<ptr<const @type[[TYPE_A]]>>(%[[VALUE_x_2]])))), const<i32>(0))
// DEFAULT-NEXT:                                 let %[[VALUE6:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_y]]);
// DEFAULT-NEXT:                                 let %[[VALUE7:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE6]]), const<i32>(1));
// DEFAULT-NEXT:                                 write<ptr<i8>>(%[[VALUE_y]], read<ptr<i8>>(%[[VALUE7]]));
// DEFAULT-NEXT:                                 write<i8>(deref(read<ptr<i8>>(%[[VALUE6]])), truncate<i8, reason=assign, fits=always>(const<i32>(58)));
// DEFAULT-NEXT:                             write<i8>(deref(read<ptr<i8>>(%[[VALUE_y]])), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
