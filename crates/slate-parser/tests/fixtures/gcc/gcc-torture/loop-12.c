/* Checks that pure functions are not treated as const.  */

char *p;

static int __attribute__((pure)) is_end_of_statement(void) {
  return *p == '\n' || *p == ';' || *p == '!';
}

void foo(void) {
  /* The is_end_of_statement call was moved out of the loop at one stage,
     resulting in an endless loop.  */
  while (!is_end_of_statement())
    p++;
}

int main(void) {
  p = "abc\n";
  foo();
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
// DEFAULT-NEXT:     global %0 p: ptr<i8> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 .str5: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([97, 98, 99, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @is_end_of_statement() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_or<bool>(logical_or<bool>(eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%0)))), const<i32>(10)), eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%0)))), const<i32>(59))), eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%0)))), const<i32>(33))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         while %4 not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%1), const<i32>(0)))
// DEFAULT-NEXT:             let %6: ptr<i8> [synthetic] = read<ptr<i8>>(%0);
// DEFAULT-NEXT:             let %7: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%6), const<i32>(1));
// DEFAULT-NEXT:             write<ptr<i8>>(%0, read<ptr<i8>>(%7));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<ptr<i8>>(%0, array_decay<ptr<i8>, length=Some(5)>(%5));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
