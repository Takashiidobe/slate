/* Origin: PR target/11535 from H. J. Lu <hjl@lucon.org> */
/* { dg-require-effective-target return_address } */

void vararg(int i, ...) { (void)i; }

int i0[0], i1;

void test1(void) {
  int a = (int)(long long)__builtin_return_address(0);
  vararg(0, a);
}

void test2(void) { i0[0] = (int)(long long)__builtin_return_address(0); }

void test3(void) { i1 = (int)(long long)__builtin_return_address(0); }

void test4(void) {
  volatile long long a = (long long)__builtin_return_address(0);
  i0[0]                = (int)a;
}

int main(void) { return 0; }


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
// DEFAULT-NEXT:     global %2 i0: array<i32, 0> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 i1: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @vararg(%1 i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         read<i32>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @test1() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %5 a: i32 [storage=automatic] = truncate<i32, reason=explicit, fits=unknown>(ptr_to_int<i64, reason=explicit>(call<ptr<void>, signature=fn(u32) -> ptr<void>>(__builtin_return_address, reinterpret<u32, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%0, const<i32>(0), read<i32>(%5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @test2() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(0)>(%2), const<i32>(0))), truncate<i32, reason=explicit, fits=unknown>(ptr_to_int<i64, reason=explicit>(call<ptr<void>, signature=fn(u32) -> ptr<void>>(__builtin_return_address, reinterpret<u32, reason=arg, fits=always>(const<i32>(0))))));
// DEFAULT-NEXT:         truncate<i32, reason=explicit, fits=unknown>(ptr_to_int<i64, reason=explicit>(call<ptr<void>, signature=fn(u32) -> ptr<void>>(__builtin_return_address, reinterpret<u32, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @test3() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(%3, truncate<i32, reason=explicit, fits=unknown>(ptr_to_int<i64, reason=explicit>(call<ptr<void>, signature=fn(u32) -> ptr<void>>(__builtin_return_address, reinterpret<u32, reason=arg, fits=always>(const<i32>(0))))));
// DEFAULT-NEXT:         truncate<i32, reason=explicit, fits=unknown>(ptr_to_int<i64, reason=explicit>(call<ptr<void>, signature=fn(u32) -> ptr<void>>(__builtin_return_address, reinterpret<u32, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @test4() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %9 a: volatile i64 [storage=automatic] = ptr_to_int<i64, reason=explicit>(call<ptr<void>, signature=fn(u32) -> ptr<void>>(__builtin_return_address, reinterpret<u32, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(0)>(%2), const<i32>(0))), truncate<i32, reason=explicit, fits=unknown>(read<i64, volatile>(%9)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
