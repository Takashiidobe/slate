// SLATE-FILECHECK-DEFINES DEFAULT

/* When compiled with -pedantic, this program will cause an ICE when the
   constant propagator tries to set the value of *str to UNDEFINED.
   
   This happens because *str is erroneously considered as a store alias.
   The aliasing code is then making *str an alias leader for its alias set
   and when the PHI node at the end of the while() is visited the first
   time, CCP will try to assign it a value of UNDEFINED, but the default
   value for *str is a constant.  */
typedef	__SIZE_TYPE__ size_t;
size_t strlength (const char * const);
char foo();

static const char * const str = "mingo";

int
bar(void)
{
  size_t c;
  char *x;

  c = strlength (str);
  while (c < 10)
    {
      if (c > 5)
	*x = foo ();
      if (*x < 'a')
	break;
    }

  return *x == '3';
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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     global %8 .str8: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([109, 105, 110, 103, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %3 str: ptr<const i8> [storage=static] [const] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(6)>(%8)) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @strlength(%7 <unnamed>: ptr<const i8> [const]) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %2 @foo() -> i8 [linkage=external];
// DEFAULT-NEXT:     fn %4 @bar() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %5 c: u64 [storage=automatic];
// DEFAULT-NEXT:         let %6 x: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         write<u64>(%5, call<u64, signature=fn(ptr<const i8>) -> u64>(%1, read<ptr<const i8>>(%3)));
// DEFAULT-NEXT:         call<u64, signature=fn(ptr<const i8>) -> u64>(%1, read<ptr<const i8>>(%3));
// DEFAULT-NEXT:         while %9 lt<u64>(read<u64>(%5), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(10))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if gt<u64>(read<u64>(%5), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(5))))
// DEFAULT-NEXT:                     write<i8>(deref(read<ptr<i8>>(%6)), call<i8, signature=fn() -> i8>(%2));
// DEFAULT-NEXT:                     call<i8, signature=fn() -> i8>(%2);
// DEFAULT-NEXT:                 if lt<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%6)))), const<i32>(97))
// DEFAULT-NEXT:                     break %9;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%6)))), const<i32>(51)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
