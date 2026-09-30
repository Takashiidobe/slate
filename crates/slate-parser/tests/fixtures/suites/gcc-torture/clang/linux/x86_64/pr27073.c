void abort(void);
void exit(int);

void __attribute__((noinline)) foo(int *p, int d1, int d2, int d3, short count,
                                   int s1, int s2, int s3, int s4, int s5) {
  int n = count;
  while (n--) {
    *p++ = s1;
    *p++ = s2;
    *p++ = s3;
    *p++ = s4;
    *p++ = s5;
  }
}

int main() {
  int x[10], i;

  foo(x, 0, 0, 0, 2, 100, 200, 300, 400, 500);
  for (i = 0; i < 10; i++)
    if (x[i] != (i % 5 + 1) * 100)
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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_p:[0-9]+]] p: ptr<i32>, %[[VALUE_d1:[0-9]+]] d1: i32, %[[VALUE_d2:[0-9]+]] d2: i32, %[[VALUE_d3:[0-9]+]] d3: i32, %[[VALUE_count:[0-9]+]] count: i16, %[[VALUE_s1:[0-9]+]] s1: i32, %[[VALUE_s2:[0-9]+]] s2: i32, %[[VALUE_s3:[0-9]+]] s3: i32, %[[VALUE_s4:[0-9]+]] s4: i32, %[[VALUE_s5:[0-9]+]] s5: i32) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_n:[0-9]+]] n: i32 [storage=automatic] = widen<i32, reason=assign>(read<i16>(%[[VALUE_count]]));
// DEFAULT-NEXT:         while %[[VALUE1:[0-9]+]] {
// DEFAULT-NEXT:             let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_n]]);
// DEFAULT-NEXT:             let %[[VALUE3:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_n]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%[[VALUE2]]), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(%[[VALUE_p]]);
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i32>>(%[[VALUE_p]], read<ptr<i32>>(%[[VALUE5]]));
// DEFAULT-NEXT:                 write<i32>(deref(read<ptr<i32>>(%[[VALUE4]])), read<i32>(%[[VALUE_s1]]));
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(%[[VALUE_p]]);
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE6]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i32>>(%[[VALUE_p]], read<ptr<i32>>(%[[VALUE7]]));
// DEFAULT-NEXT:                 write<i32>(deref(read<ptr<i32>>(%[[VALUE6]])), read<i32>(%[[VALUE_s2]]));
// DEFAULT-NEXT:                 let %[[VALUE8:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(%[[VALUE_p]]);
// DEFAULT-NEXT:                 let %[[VALUE9:[0-9]+]]: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE8]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i32>>(%[[VALUE_p]], read<ptr<i32>>(%[[VALUE9]]));
// DEFAULT-NEXT:                 write<i32>(deref(read<ptr<i32>>(%[[VALUE8]])), read<i32>(%[[VALUE_s3]]));
// DEFAULT-NEXT:                 let %[[VALUE10:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(%[[VALUE_p]]);
// DEFAULT-NEXT:                 let %[[VALUE11:[0-9]+]]: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE10]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i32>>(%[[VALUE_p]], read<ptr<i32>>(%[[VALUE11]]));
// DEFAULT-NEXT:                 write<i32>(deref(read<ptr<i32>>(%[[VALUE10]])), read<i32>(%[[VALUE_s4]]));
// DEFAULT-NEXT:                 let %[[VALUE12:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(%[[VALUE_p]]);
// DEFAULT-NEXT:                 let %[[VALUE13:[0-9]+]]: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE12]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i32>>(%[[VALUE_p]], read<ptr<i32>>(%[[VALUE13]]));
// DEFAULT-NEXT:                 write<i32>(deref(read<ptr<i32>>(%[[VALUE12]])), read<i32>(%[[VALUE_s5]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: array<i32, 10> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>, i32, i32, i32, i16, i32, i32, i32, i32, i32) -> void>(%[[VALUE_foo]], array_decay<ptr<i32>, length=Some(10)>(%[[VALUE_x]]), const<i32>(0), const<i32>(0), const<i32>(0), truncate<i16, reason=arg, fits=always>(const<i32>(2)), const<i32>(100), const<i32>(200), const<i32>(300), const<i32>(400), const<i32>(500));
// DEFAULT-NEXT:         for %[[VALUE14:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE15:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE16:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE15]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE16]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(10)>(%[[VALUE_x]]), read<i32>(%[[VALUE_i]])))), mul<i32, overflow=ub>(add<i32, overflow=ub>(rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_i]]), const<i32>(5)), const<i32>(1)), const<i32>(100)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
