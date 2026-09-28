extern void abort(void);
extern void exit(int);

int sub1(int val) { return val; }

int testcond(int val) {
  int flag1;

  {
    int t1 = val;
    {
      int t2 = t1;
      {
        flag1 = sub1(t2) == 0;
        goto lab1;
      };
    }
  lab1:;
  }

  if (flag1 != 0)
    return 0x4d0000;
  else
    return 0;
}

int main(void) {
  if (testcond(1))
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
// DEFAULT-NEXT:     fn %1 @exit(%11 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @sub1(%3 val: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%3);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @testcond(%6 val: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %7 flag1: i32 [storage=automatic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %8 t1: i32 [storage=automatic] = read<i32>(%6);
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %9 t2: i32 [storage=automatic] = read<i32>(%8);
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(%7, from_bool<i32, reason=assign>(eq<i32>(call<i32, signature=fn(i32) -> i32>(%2, read<i32>(%9)), const<i32>(0))));
// DEFAULT-NEXT:                     from_bool<i32, reason=assign>(eq<i32>(call<i32, signature=fn(i32) -> i32>(%2, read<i32>(%9)), const<i32>(0)));
// DEFAULT-NEXT:                     goto %5;
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             label %5 lab1:
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%7), const<i32>(0))
// DEFAULT-NEXT:             return const<i32>(5046272);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%4, const<i32>(1)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
