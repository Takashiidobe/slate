extern void abort(void);

int          foo = 0;
void        *bar = 0;
unsigned int baz = 100;

void *pure_alloc() {
  void *res;

  while (1) {
    res  = (void *)((((unsigned int)(foo + bar))) & ~1);
    foo += 2;
    if (foo < baz)
      return res;
    foo = 0;
  }
}

int main() {
  pure_alloc();
  if (!foo)
    abort();
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
// DEFAULT-NEXT:     global %[[VALUE_foo:[0-9]+]] foo: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_bar:[0-9]+]] bar: ptr<void> [storage=static] = null<ptr<void>> [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_baz:[0-9]+]] baz: u32 [storage=static] = reinterpret<u32, reason=assign, fits=always>(const<i32>(100)) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_pure_alloc:[0-9]+]] @pure_alloc() -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_res:[0-9]+]] res: ptr<void> [storage=automatic];
// DEFAULT-NEXT:         while %[[VALUE0:[0-9]+]] ne<i32>(const<i32>(1), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<ptr<void>>(%[[VALUE_res]], int_to_ptr<ptr<void>, reason=explicit>(and<u32>(ptr_to_int<u32, reason=explicit>(ptr_offset<ptr<void>, subtract=false, element=void, overflow=ub>(read<ptr<void>>(%[[VALUE_bar]]), read<i32>(%[[VALUE_foo]]))), reinterpret<u32, reason=usual_arith, fits=unknown>(not<i32>(const<i32>(1))))));
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_foo]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(2));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_foo]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 if lt<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%[[VALUE_foo]])), read<u32>(%[[VALUE_baz]]))
// DEFAULT-NEXT:                     return read<ptr<void>>(%[[VALUE_res]]);
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_foo]], const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn() -> ptr<void>>(%[[VALUE_pure_alloc]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32>(%[[VALUE_foo]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
