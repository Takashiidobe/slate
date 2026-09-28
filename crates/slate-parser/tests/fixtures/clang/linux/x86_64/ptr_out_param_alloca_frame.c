#include <stdio.h>

static int check_target(const char *ptr, const char *end, int *tokPtr) {
  int upper = 0;
  *tokPtr   = 7;
  if (end - ptr != 3)
    return 1;
  switch (ptr[0]) {
  case 'x':
    break;
  case 'X':
    upper = 1;
    break;
  default:
    return 1;
  }
  if (upper)
    *tokPtr = 9;
  return 0;
}

int main(void) {
  int         tok = 0;
  const char *s   = "xyz";
  int         r   = check_target(s, s + 3, &tok);
  printf("%d %d\n", r, tok);
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
// DEFAULT-NEXT:     global %12 .str12: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([120, 121, 122, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %13 .str13: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%10 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @check_target(%2 ptr: ptr<const i8>, %3 end: ptr<const i8>, %4 tokPtr: ptr<i32>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %5 upper: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%4)), const<i32>(7));
// DEFAULT-NEXT:         if ne<i64>(ptr_diff<i64, element=i8, same_array=required, overflow=ub>(read<ptr<const i8>>(%3), read<ptr<const i8>>(%2)), widen<i64, reason=usual_arith>(const<i32>(3)))
// DEFAULT-NEXT:             return const<i32>(1);
// DEFAULT-NEXT:         switch %11 widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%2), const<i32>(0)))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %11 const<i32>(120):
// DEFAULT-NEXT:                     break %11;
// DEFAULT-NEXT:                 case %11 const<i32>(88):
// DEFAULT-NEXT:                     write<i32>(%5, const<i32>(1));
// DEFAULT-NEXT:                 break %11;
// DEFAULT-NEXT:                 default %11:
// DEFAULT-NEXT:                     return const<i32>(1);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%5), const<i32>(0))
// DEFAULT-NEXT:             write<i32>(deref(read<ptr<i32>>(%4)), const<i32>(9));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %7 tok: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %8 s: ptr<const i8> [storage=automatic] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(4)>(%12));
// DEFAULT-NEXT:         let %9 r: i32 [storage=automatic] = call<i32, signature=fn(ptr<const i8>, ptr<const i8>, ptr<i32>) -> i32>(%1, read<ptr<const i8>>(%8), ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%8), const<i32>(3)), addr_of<ptr<i32>>(%7));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%13)), read<i32>(%9), read<i32>(%7));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
