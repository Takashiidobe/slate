long long a;
unsigned  b;
int       c[70];
int       d[70][70];
int       e;

__attribute__((noinline)) void f(long long *g, int p2) { *g = p2; }

__attribute__((noinline)) void fn2() {
  for (int j = 0; j < 70; j++) {
    for (int i = 0; i < 70; i++) {
      if (b)
        c[i] = 0;
      for (int l = 0; l < 70; l++)
        d[i][1] = d[l][i];
    }
    for (int k = 0; k < 70; k++)
      e = c[0];
  }
}

int main() {
  b = 5;
  for (int j = 0; j < 70; ++j)
    c[j] = 2075593088;
  fn2();
  f(&a, e);
  if (a)
    __builtin_abort();
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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: array<i32, 70> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: array<array<i32, 70>, 70> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_g:[0-9]+]] g: ptr<i64>, %[[VALUE_p2:[0-9]+]] p2: i32) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i64>(deref(read<ptr<i64>>(%[[VALUE_g]])), widen<i64, reason=assign>(read<i32>(%[[VALUE_p2]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2:[0-9]+]] @fn2() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j]]), const<i32>(70))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     for %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                         condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(70))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                             let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 if ne<u32>(read<u32>(%[[VALUE_b]]), const<u32>(0))
// DEFAULT-NEXT:                                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(70)>(%[[VALUE_c]]), read<i32>(%[[VALUE_i]]))), const<i32>(0));
// DEFAULT-NEXT:                                 for %[[VALUE6:[0-9]+]]
// DEFAULT-NEXT:                                     init:
// DEFAULT-NEXT:                                         let %[[VALUE_l:[0-9]+]] l: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                                     condition: lt<i32>(read<i32>(%[[VALUE_l]]), const<i32>(70))
// DEFAULT-NEXT:                                     increment: {
// DEFAULT-NEXT:                                         let %[[VALUE7:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_l]]);
// DEFAULT-NEXT:                                         let %[[VALUE8:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE7]]), const<i32>(1));
// DEFAULT-NEXT:                                         write<i32>(%[[VALUE_l]], read<i32>(%[[VALUE8]]));
// DEFAULT-NEXT:                                         yield void;
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                                     body:
// DEFAULT-NEXT:                                         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(70)>(deref(ptr_offset<ptr<array<i32, 70>>, subtract=false, element=array<i32, 70>, overflow=ub>(array_decay<ptr<array<i32, 70>>, length=Some(70)>(%[[VALUE_d]]), read<i32>(%[[VALUE_i]])))), const<i32>(1))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(70)>(deref(ptr_offset<ptr<array<i32, 70>>, subtract=false, element=array<i32, 70>, overflow=ub>(array_decay<ptr<array<i32, 70>>, length=Some(70)>(%[[VALUE_d]]), read<i32>(%[[VALUE_l]])))), read<i32>(%[[VALUE_i]])))));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     for %[[VALUE9:[0-9]+]]
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             let %[[VALUE_k:[0-9]+]] k: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                         condition: lt<i32>(read<i32>(%[[VALUE_k]]), const<i32>(70))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %[[VALUE10:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_k]]);
// DEFAULT-NEXT:                             let %[[VALUE11:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE10]]), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_k]], read<i32>(%[[VALUE11]]));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_e]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(70)>(%[[VALUE_c]]), const<i32>(0)))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<u32>(%[[VALUE_b]], reinterpret<u32, reason=assign, fits=always>(const<i32>(5)));
// DEFAULT-NEXT:         for %[[VALUE12:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_j_2:[0-9]+]] j: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_2]]), const<i32>(70))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE13:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_2]]);
// DEFAULT-NEXT:                 let %[[VALUE14:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE13]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_2]], read<i32>(%[[VALUE14]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(70)>(%[[VALUE_c]]), read<i32>(%[[VALUE_j_2]]))), const<i32>(2075593088));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_fn2]]);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i64>, i32) -> void>(%[[VALUE_f]], addr_of<ptr<i64>>(%[[VALUE_a]]), read<i32>(%[[VALUE_e]]));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%[[VALUE_a]]), const<i64>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
