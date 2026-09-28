void abort(void);
void exit(int);

void f(int *x) { *x = 0; }

int main(void) {
  int  s, c, x;
  char a[] = "c";

  f(&s);
  a[c = 0] = s == 0 ? (x = 1, 'a') : (x = 2, 'b');
  if (a[c] != 'a')
    abort();
  exit(0);
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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%9 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @f(%3 x: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%3)), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %5 s: i32 [storage=automatic];
// DEFAULT-NEXT:         let %6 c: i32 [storage=automatic];
// DEFAULT-NEXT:         let %7 x: i32 [storage=automatic];
// DEFAULT-NEXT:         let %8 a: array<i8, 2> [storage=automatic] = code_units<array<i8, 2>>([99, 0]);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%2, addr_of<ptr<i32>>(%5));
// DEFAULT-NEXT:         let %10: i32 [synthetic];
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%5), const<i32>(0))
// DEFAULT-NEXT:             write<i32>(%7, const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%10, const<i32>(97));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<i32>(%7, const<i32>(2));
// DEFAULT-NEXT:             write<i32>(%10, const<i32>(98));
// DEFAULT-NEXT:         write<i32>(%6, const<i32>(0));
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(%8), const<i32>(0))), truncate<i8, reason=assign, fits=unknown>(read<i32>(%10)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(%8), read<i32>(%6))))), const<i32>(97))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
