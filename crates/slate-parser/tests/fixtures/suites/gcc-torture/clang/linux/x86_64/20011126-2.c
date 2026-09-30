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
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([97, 97, 98, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_test:[0-9]+]] @test(%[[VALUE_in:[0-9]+]] in: ptr<const i8>, %[[VALUE_out:[0-9]+]] out: ptr<i8>) -> ptr<const i8> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         while %[[VALUE0:[0-9]+]] ne<i32>(const<i32>(1), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<const i8>>(%[[VALUE_in]])))), const<i32>(97))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_p:[0-9]+]] p: ptr<const i8> [storage=automatic] = ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE_in]]), const<i32>(1));
// DEFAULT-NEXT:                         while %[[VALUE1:[0-9]+]] eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<const i8>>(%[[VALUE_p]])))), const<i32>(120))
// DEFAULT-NEXT:                             let %[[VALUE2:[0-9]+]]: ptr<const i8> [synthetic] = read<ptr<const i8>>(%[[VALUE_p]]);
// DEFAULT-NEXT:                             let %[[VALUE3:[0-9]+]]: ptr<const i8> [synthetic] = ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:                             write<ptr<const i8>>(%[[VALUE_p]], read<ptr<const i8>>(%[[VALUE3]]));
// DEFAULT-NEXT:                         if eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<const i8>>(%[[VALUE_p]])))), const<i32>(98))
// DEFAULT-NEXT:                             return read<ptr<const i8>>(%[[VALUE_p]]);
// DEFAULT-NEXT:                         while %[[VALUE4:[0-9]+]] lt<ptr<const i8>>(read<ptr<const i8>>(%[[VALUE_in]]), read<ptr<const i8>>(%[[VALUE_p]]))
// DEFAULT-NEXT:                             let %[[VALUE5:[0-9]+]]: ptr<const i8> [synthetic] = read<ptr<const i8>>(%[[VALUE_in]]);
// DEFAULT-NEXT:                             let %[[VALUE6:[0-9]+]]: ptr<const i8> [synthetic] = ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE5]]), const<i32>(1));
// DEFAULT-NEXT:                             write<ptr<const i8>>(%[[VALUE_in]], read<ptr<const i8>>(%[[VALUE6]]));
// DEFAULT-NEXT:                             let %[[VALUE7:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_out]]);
// DEFAULT-NEXT:                             let %[[VALUE8:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE7]]), const<i32>(1));
// DEFAULT-NEXT:                             write<ptr<i8>>(%[[VALUE_out]], read<ptr<i8>>(%[[VALUE8]]));
// DEFAULT-NEXT:                             write<i8>(deref(read<ptr<i8>>(%[[VALUE7]])), read<i8>(deref(read<ptr<const i8>>(%[[VALUE5]]))));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_out_2:[0-9]+]] out: array<i8, 4> [storage=automatic];
// DEFAULT-NEXT:         call<ptr<const i8>, signature=fn(ptr<const i8>, ptr<i8>) -> ptr<const i8>>(%[[VALUE_test]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str]])), array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_out_2]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
