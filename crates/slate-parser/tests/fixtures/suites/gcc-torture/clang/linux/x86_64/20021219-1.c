/* PR optimization/8988 */
/* Contributed by Kevin Easton */

void foo(char *p1, char **p2) {}

int main(void) {
  char  str[] = "foo { xx }";
  char *ptr   = str + 5;

  foo(ptr, &ptr);

  while (*ptr && (*ptr == 13 || *ptr == 32))
    ptr++;

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
// DEFAULT-NEXT:     fn %0 @foo(%1 p1: ptr<i8>, %2 p2: ptr<ptr<i8>>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %4 str: array<i8, 11> [storage=automatic] = code_units<array<i8, 11>>([102, 111, 111, 32, 123, 32, 120, 120, 32, 125, 0]);
// DEFAULT-NEXT:         let %5 ptr: ptr<i8> [storage=automatic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%4), const<i32>(5));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i8>, ptr<ptr<i8>>) -> void>(%0, read<ptr<i8>>(%5), addr_of<ptr<ptr<i8>>>(%5));
// DEFAULT-NEXT:         while %6 logical_and<bool>(ne<i8>(read<i8>(deref(read<ptr<i8>>(%5))), const<i8>(0)), logical_or<bool>(eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%5)))), const<i32>(13)), eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%5)))), const<i32>(32))))
// DEFAULT-NEXT:             let %7: ptr<i8> [synthetic] = read<ptr<i8>>(%5);
// DEFAULT-NEXT:             let %8: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%7), const<i32>(1));
// DEFAULT-NEXT:             write<ptr<i8>>(%5, read<ptr<i8>>(%8));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
