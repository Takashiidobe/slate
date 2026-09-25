void abort(void);

char  q;
void *a[2];

void foo(char *p) {
  int i;
  for (i = 0; i < 2; i++)
    a[i] += p - &q;
}

int main(void) {
  int i;
  foo(&q);
  for (i = 0; i < 2; i++)
    if (a[i])
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
// DEFAULT-NEXT:     global %1 q: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 a: array<ptr<void>, 2> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @foo(%4 p: ptr<i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %5 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %8
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%5, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%5), const<i32>(2))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %10: i32 [synthetic] = read<i32>(%5);
// DEFAULT-NEXT:                 let %11: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%10), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%5, read<i32>(%11));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %12: ptr<ptr<void>> [synthetic] = ptr_offset<ptr<ptr<void>>, subtract=false, element=ptr<void>, overflow=ub>(array_decay<ptr<ptr<void>>, length=Some(2)>(%2), read<i32>(%5));
// DEFAULT-NEXT:                 let %13: ptr<void> [synthetic] = read<ptr<void>>(deref(read<ptr<ptr<void>>>(%12)));
// DEFAULT-NEXT:                 let %14: ptr<void> [synthetic] = ptr_offset<ptr<void>, subtract=false, element=void, overflow=ub>(read<ptr<void>>(%13), ptr_diff<i64, element=i8, same_array=required, overflow=ub>(read<ptr<i8>>(%4), addr_of<ptr<i8>>(%1)));
// DEFAULT-NEXT:                 write<ptr<void>>(deref(read<ptr<ptr<void>>>(%12)), read<ptr<void>>(%14));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %7 i: i32 [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i8>) -> void>(%3, addr_of<ptr<i8>>(%1));
// DEFAULT-NEXT:         for %9
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%7, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%7), const<i32>(2))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %15: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                 let %16: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%15), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%7, read<i32>(%16));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<ptr<void>>(read<ptr<void>>(deref(ptr_offset<ptr<ptr<void>>, subtract=false, element=ptr<void>, overflow=ub>(array_decay<ptr<ptr<void>>, length=Some(2)>(%2), read<i32>(%7)))), null<ptr<void>>)
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
