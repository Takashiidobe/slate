/* Test array initializion by store_by_pieces.  */
/* { dg-do run } */
/* { dg-options "-O2" } */

struct A { char c[10]; };
extern void abort (void);

void
__attribute__((noinline))
check (struct A * a, int b)
{
  const char *p;
  switch (b)
    {
    case 0:
      p = "abcdefghi";
      break;
    case 1:
      p = "j\0\0\0\0\0\0\0\0";
      break;
    case 2:
      p = "kl\0\0\0\0\0\0\0";
      break;
    case 3:
      p = "mnop\0\0\0\0\0";
      break;
    case 4:
      p = "qrstuvwx\0";
      break;
    default:
      abort ();
    }
  if (__builtin_memcmp (a->c, p, 10) != 0)
    abort ();
}

int
main (void)
{
  struct A a = { "abcdefghi" };
  check (&a, 0);
  struct A b = { "j" };
  check (&b, 1);
  struct A c = { "kl" };
  check (&c, 2);
  struct A d = { "mnop" };
  check (&d, 3);
  struct A e = { "qrstuvwx" };
  check (&e, 4);
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
// DEFAULT-NEXT:     type @type0 A = struct {
// DEFAULT-NEXT:         field0 c: array<i8, 10>;
// DEFAULT-NEXT:     } [size=10, align=1, offsets=[0]];
// DEFAULT-NEXT:     global %13 .str13: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([97, 98, 99, 100, 101, 102, 103, 104, 105, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %14 .str14: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([106, 0, 0, 0, 0, 0, 0, 0, 0, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %15 .str15: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([107, 108, 0, 0, 0, 0, 0, 0, 0, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %16 .str16: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([109, 110, 111, 112, 0, 0, 0, 0, 0, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %17 .str17: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([113, 114, 115, 116, 117, 118, 119, 120, 0, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %21 @__builtin_memcmp(%18 <unnamed>: ptr<const void>, %19 <unnamed>: ptr<const void>, %20 <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @check(%3 a: ptr<@type0>, %4 b: i32) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %5 p: ptr<const i8> [storage=automatic];
// DEFAULT-NEXT:         switch %12 read<i32>(%4)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %12 const<i32>(0):
// DEFAULT-NEXT:                     write<ptr<const i8>>(%5, pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(10)>(%13)));
// DEFAULT-NEXT:                 break %12;
// DEFAULT-NEXT:                 case %12 const<i32>(1):
// DEFAULT-NEXT:                     write<ptr<const i8>>(%5, pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(10)>(%14)));
// DEFAULT-NEXT:                 break %12;
// DEFAULT-NEXT:                 case %12 const<i32>(2):
// DEFAULT-NEXT:                     write<ptr<const i8>>(%5, pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(10)>(%15)));
// DEFAULT-NEXT:                 break %12;
// DEFAULT-NEXT:                 case %12 const<i32>(3):
// DEFAULT-NEXT:                     write<ptr<const i8>>(%5, pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(10)>(%16)));
// DEFAULT-NEXT:                 break %12;
// DEFAULT-NEXT:                 case %12 const<i32>(4):
// DEFAULT-NEXT:                     write<ptr<const i8>>(%5, pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(10)>(%17)));
// DEFAULT-NEXT:                 break %12;
// DEFAULT-NEXT:                 default %12:
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%21, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(field0(deref(read<ptr<@type0>>(%3))))), pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i8>>(%5)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(10)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %7 a: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = code_units<array<i8, 10>>([97, 98, 99, 100, 101, 102, 103, 104, 105, 0]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>, i32) -> void>(%2, addr_of<ptr<@type0>>(%7), const<i32>(0));
// DEFAULT-NEXT:         let %8 b: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = code_units<array<i8, 10>>([106, 0, 0, 0, 0, 0, 0, 0, 0, 0]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>, i32) -> void>(%2, addr_of<ptr<@type0>>(%8), const<i32>(1));
// DEFAULT-NEXT:         let %9 c: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = code_units<array<i8, 10>>([107, 108, 0, 0, 0, 0, 0, 0, 0, 0]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>, i32) -> void>(%2, addr_of<ptr<@type0>>(%9), const<i32>(2));
// DEFAULT-NEXT:         let %10 d: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = code_units<array<i8, 10>>([109, 110, 111, 112, 0, 0, 0, 0, 0, 0]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>, i32) -> void>(%2, addr_of<ptr<@type0>>(%10), const<i32>(3));
// DEFAULT-NEXT:         let %11 e: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = code_units<array<i8, 10>>([113, 114, 115, 116, 117, 118, 119, 120, 0, 0]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>, i32) -> void>(%2, addr_of<ptr<@type0>>(%11), const<i32>(4));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
