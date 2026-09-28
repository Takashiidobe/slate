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
// DEFAULT-NEXT:     type @type0 A = struct {
// DEFAULT-NEXT:         field0 a1: i32;
// DEFAULT-NEXT:         field1 a2: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     extern %1 tcc1: ptr<const u8> [storage=thread] [linkage=external];
// DEFAULT-NEXT:     extern %2 tcc2: ptr<ptr<const u8>> [storage=thread] [linkage=external];
// DEFAULT-NEXT:     fn %3 @foo() -> ptr<ptr<const u8>> [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %4 a: ptr<ptr<const u8>> [storage=automatic] = addr_of<ptr<ptr<const u8>>>(%1);
// DEFAULT-NEXT:         if eq<ptr<const u8>>(read<ptr<const u8>>(deref(read<ptr<ptr<const u8>>>(%4))), null<ptr<const u8>>)
// DEFAULT-NEXT:             write<ptr<const u8>>(deref(read<ptr<ptr<const u8>>>(%4)), ptr_offset<ptr<const u8>, subtract=false, element=u8, overflow=ub>(read<ptr<const u8>>(deref(read<ptr<ptr<const u8>>>(%2))), const<i32>(128)));
// DEFAULT-NEXT:         return read<ptr<ptr<const u8>>>(%4);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @bar(%6 x: ptr<const @type0>) -> i32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %7 a: i32 [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(and<i32>(read<i32>(field1(deref(read<ptr<const @type0>>(%6)))), const<i32>(8)), const<i32>(0))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         write<i32>(%7, read<i32>(field0(deref(read<ptr<const @type0>>(%6)))));
// DEFAULT-NEXT:         let %13: bool [synthetic];
// DEFAULT-NEXT:         if gt<i32>(read<i32>(%7), const<i32>(0))
// DEFAULT-NEXT:             write<bool>(%13, ne<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<const u8>, subtract=false, element=u8, overflow=ub>(read<ptr<const u8>>(deref(call<ptr<ptr<const u8>>, signature=fn() -> ptr<ptr<const u8>>>(%3))), read<i32>(%7)))))), const<i32>(64)), const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%13, const<bool>(false));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(read<bool>(%13));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @baz(%9 x: ptr<const @type0>, %10 y: ptr<i8>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %11 a: ptr<const @type0> [storage=automatic];
// DEFAULT-NEXT:         for %12
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<ptr<const @type0>>(%11, read<ptr<const @type0>>(%9));
// DEFAULT-NEXT:             condition: not<bool>(not<bool>(ne<i32>(read<i32>(field0(deref(read<ptr<const @type0>>(%11)))), const<i32>(0))))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %14: ptr<const @type0> [synthetic] = read<ptr<const @type0>>(%11);
// DEFAULT-NEXT:                 let %15: ptr<const @type0> [synthetic] = ptr_offset<ptr<const @type0>, subtract=false, element=@type0, overflow=ub>(read<ptr<const @type0>>(%14), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<const @type0>>(%11, read<ptr<const @type0>>(%15));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(and<i32>(read<i32>(field1(deref(read<ptr<const @type0>>(%9)))), const<i32>(8)), const<i32>(0)))
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(ptr<const @type0>) -> i32>(%5, read<ptr<const @type0>>(%11)), const<i32>(0))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             let %16: ptr<i8> [synthetic] = read<ptr<i8>>(%10);
// DEFAULT-NEXT:                             let %17: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%16), const<i32>(1));
// DEFAULT-NEXT:                             write<ptr<i8>>(%10, read<ptr<i8>>(%17));
// DEFAULT-NEXT:                             write<i8>(deref(read<ptr<i8>>(%16)), truncate<i8, reason=assign, fits=unknown>(read<i32>(field0(deref(read<ptr<const @type0>>(%11))))));
// DEFAULT-NEXT:                             if ne<i32>(read<i32>(field0(deref(read<ptr<const @type0>>(%9)))), const<i32>(0))
// DEFAULT-NEXT:                                 let %18: ptr<i8> [synthetic] = read<ptr<i8>>(%10);
// DEFAULT-NEXT:                                 let %19: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%18), const<i32>(1));
// DEFAULT-NEXT:                                 write<ptr<i8>>(%10, read<ptr<i8>>(%19));
// DEFAULT-NEXT:                                 write<i8>(deref(read<ptr<i8>>(%18)), truncate<i8, reason=assign, fits=always>(const<i32>(58)));
// DEFAULT-NEXT:                             write<i8>(deref(read<ptr<i8>>(%10)), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
