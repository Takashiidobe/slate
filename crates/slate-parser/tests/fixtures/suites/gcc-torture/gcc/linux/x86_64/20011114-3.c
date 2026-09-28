// SLATE-FILECHECK-DEFINES DEFAULT

typedef struct { int s, t; } C;
C x;
int foo (void);
void bar (int);

int baz (void)
{
  int a = 0, c, d = 0;
  C *b = &x;

  while ((c = foo ()))
    switch(c)
      {
      case 23:
	bar (1);
	break;
      default:
	break;
      }

  if (a == 0 || (a & 1))
    {
      if (b->s)
	{
	  if (a)
	    bar (1);
	  else
	    a = 16;
	}
      else if (b->t)
	{
	  if (a)
	    bar (1);
	  else
	    a = 32;
	}
    }

  if (d && (a & ~127))
    bar (2);
  return 0;
}

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
// DEFAULT-NEXT:     type @type0 = struct {
// DEFAULT-NEXT:         field0 s: i32;
// DEFAULT-NEXT:         field1 t: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type1 C = @type0;
// DEFAULT-NEXT:     global %2 x: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %3 @foo() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %4 @bar(%10 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %5 @baz() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %6 a: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %7 c: i32 [storage=automatic];
// DEFAULT-NEXT:         let %8 d: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %9 b: ptr<@type0> [storage=automatic] = addr_of<ptr<@type0>>(%2);
// DEFAULT-NEXT:         while %11 {
// DEFAULT-NEXT:             write<i32>(%7, call<i32, signature=fn() -> i32>(%3));
// DEFAULT-NEXT:             yield ne<i32>(call<i32, signature=fn() -> i32>(%3), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             switch %12 read<i32>(%7)
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     case %12 const<i32>(23):
// DEFAULT-NEXT:                         call<void, signature=fn(i32) -> void>(%4, const<i32>(1));
// DEFAULT-NEXT:                     break %12;
// DEFAULT-NEXT:                     default %12:
// DEFAULT-NEXT:                         break %12;
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         if logical_or<bool>(eq<i32>(read<i32>(%6), const<i32>(0)), ne<i32>(and<i32>(read<i32>(%6), const<i32>(1)), const<i32>(0)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(field0(deref(read<ptr<@type0>>(%9)))), const<i32>(0))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(read<i32>(%6), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn(i32) -> void>(%4, const<i32>(1));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<i32>(%6, const<i32>(16));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(field1(deref(read<ptr<@type0>>(%9)))), const<i32>(0))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(read<i32>(%6), const<i32>(0))
// DEFAULT-NEXT:                                 call<void, signature=fn(i32) -> void>(%4, const<i32>(1));
// DEFAULT-NEXT:                             else
// DEFAULT-NEXT:                                 write<i32>(%6, const<i32>(32));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if logical_and<bool>(ne<i32>(read<i32>(%8), const<i32>(0)), ne<i32>(and<i32>(read<i32>(%6), not<i32>(const<i32>(127))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(2));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
