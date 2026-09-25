/* Problem originally visible on ia64.

   There is a partial redundancy of "in + 1" that makes GCSE want to
   transform the final while loop to

     p = in + 1;
     tmp = p;
     ...
     goto start;
   top:
     tmp = tmp + 1;
   start:
     in = tmp;
     if (in < p) goto top;

   We miscalculate the number of loop iterations as (p - tmp) = 0
   instead of (p - in) = 1, which results in overflow in the doloop
   optimization.  */

static const char *test(const char *in, char *out) {
  while (1) {
    if (*in == 'a') {
      const char *p = in + 1;
      while (*p == 'x')
        ++p;
      if (*p == 'b')
        return p;
      while (in < p)
        *out++ = *in++;
    }
  }
}

int main() {
  char out[4];
  test("aab", out);
  return 0;
}


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
// DEFAULT-NEXT:     global %9 .str9: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([97, 97, 98, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @test(%1 in: ptr<const i8>, %2 out: ptr<i8>) -> ptr<const i8> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         while %6 ne<i32>(const<i32>(1), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<const i8>>(%1)))), const<i32>(97))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %3 p: ptr<const i8> [storage=automatic] = ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%1), const<i32>(1));
// DEFAULT-NEXT:                         while %7 eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<const i8>>(%3)))), const<i32>(120))
// DEFAULT-NEXT:                             let %10: ptr<const i8> [synthetic] = read<ptr<const i8>>(%3);
// DEFAULT-NEXT:                             let %11: ptr<const i8> [synthetic] = ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%10), const<i32>(1));
// DEFAULT-NEXT:                             write<ptr<const i8>>(%3, read<ptr<const i8>>(%11));
// DEFAULT-NEXT:                         if eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<const i8>>(%3)))), const<i32>(98))
// DEFAULT-NEXT:                             return read<ptr<const i8>>(%3);
// DEFAULT-NEXT:                         while %8 lt<ptr<const i8>>(read<ptr<const i8>>(%1), read<ptr<const i8>>(%3))
// DEFAULT-NEXT:                             let %12: ptr<const i8> [synthetic] = read<ptr<const i8>>(%1);
// DEFAULT-NEXT:                             let %13: ptr<const i8> [synthetic] = ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%12), const<i32>(1));
// DEFAULT-NEXT:                             write<ptr<const i8>>(%1, read<ptr<const i8>>(%13));
// DEFAULT-NEXT:                             let %14: ptr<i8> [synthetic] = read<ptr<i8>>(%2);
// DEFAULT-NEXT:                             let %15: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%14), const<i32>(1));
// DEFAULT-NEXT:                             write<ptr<i8>>(%2, read<ptr<i8>>(%15));
// DEFAULT-NEXT:                             write<i8>(deref(read<ptr<i8>>(%14)), read<i8>(deref(read<ptr<const i8>>(%12))));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %5 out: array<i8, 4> [storage=automatic];
// DEFAULT-NEXT:         call<ptr<const i8>, signature=fn(ptr<const i8>, ptr<i8>) -> ptr<const i8>>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%9)), array_decay<ptr<i8>, length=Some(4)>(%5));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
