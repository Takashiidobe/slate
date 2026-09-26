/* PR tree-optimization/58277 */

extern void abort(void);
static int  a[1], b, c, e, i, j, k, m, q[] = {1, 1}, t;
int volatile d;
int **r;
static int ***volatile s = &r;
int f, g, o, x;
static int *volatile h = &f, *p;
char n;

static void fn1() {
  b = a[a[a[a[a[a[a[a[b]]]]]]]];
  b = a[a[a[a[a[a[a[a[b]]]]]]]];
  b = a[a[b]];
  b = a[a[a[a[a[a[a[a[b]]]]]]]];
  b = a[a[a[a[a[a[a[a[b]]]]]]]];
}

static int fn2() {
  n = 0;
  for (; g; t++) {
    for (;; m++) {
      d;
      int  *u;
      int **v[] = {
          0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
          0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
          0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
          0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
          0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
          0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
          0,  0,  0,  &u, 0,  0,  0,  0,  &u, &u, &u, &u, &u, &u, &u, 0,  &u, 0,
          &u, &u, &u, 0,  &u, &u, 0,  &u, &u, &u, &u, 0,  &u, &u, &u, &u, &u, 0,
          &u, &u, 0,  &u, 0,  &u, &u, 0,  &u, &u, &u, &u, &u, 0,  &u, 0,  0,  0,
          &u, &u, &u, 0,  0,  &u, &u, &u, 0,  &u, 0,  &u, &u};
      int ***w[] = {&v[0]};
      if (*p)
        break;
      return 0;
    }
    *h = 0;
  }
  return 1;
}

static void fn3() {
  int *y[] = {0, 0, 0, 0, 0, 0, 0, 0};
  for (; i; i++)
    x = 0;
  if (fn2()) {
    int *z[6] = {};
    for (; n < 1; n++)
      *h = 0;
    int t1[7];
    for (; c; c++)
      o = t1[0];
    for (; e; e--) {
      int  **t2 = &y[0];
      int ***t3 = &t2;
      *t3       = &z[0];
    }
  }
  *s = 0;
  for (n = 0;; n = 0) {
    int t4 = 0;
    if (q[n])
      break;
    *r = &t4;
  }
}

int main() {
  for (; j; j--)
    a[0] = 0;
  fn3();
  for (; k; k++)
    fn1();
  fn1();

  if (n)
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
// DEFAULT-NEXT:     global %1 a: array<i32, 1> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %2 b: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %3 c: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %4 e: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %5 i: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %6 j: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %7 k: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %8 m: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %9 q: array<i32, 2> [storage=static] = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %10 t: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %11 d: volatile i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %12 r: ptr<ptr<i32>> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %13 s: volatile ptr<ptr<ptr<i32>>> [storage=static] = addr_of<ptr<ptr<ptr<i32>>>>(%12) [linkage=internal];
// DEFAULT-NEXT:     global %14 f: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %15 g: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %16 o: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %17 x: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %18 h: volatile ptr<i32> [storage=static] = addr_of<ptr<i32>>(%14) [linkage=internal];
// DEFAULT-NEXT:     global %19 p: ptr<i32> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %20 n: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %21 @fn1() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%1), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%1), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%1), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%1), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%1), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%1), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%1), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%1), read<i32>(%2))))))))))))))))))))))))));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%1), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%1), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%1), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%1), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%1), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%1), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%1), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%1), read<i32>(%2))))))))))))))))))))))))));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%1), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%1), read<i32>(%2))))))));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%1), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%1), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%1), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%1), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%1), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%1), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%1), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%1), read<i32>(%2))))))))))))))))))))))))));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%1), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%1), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%1), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%1), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%1), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%1), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%1), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%1), read<i32>(%2))))))))))))))))))))))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @fn2() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i8>(%20, truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         for %34
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: ne<i32>(read<i32>(%15), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %43: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                 let %44: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%43), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%10, read<i32>(%44));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     for %35
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                         condition: omitted
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %45: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:                             let %46: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%45), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%8, read<i32>(%46));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 read<i32, volatile>(%11);
// DEFAULT-NEXT:                                 let %23 u: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:                                 let %24 v: array<ptr<ptr<i32>>, 175> [storage=automatic] [align=16] = aggregate<array<ptr<ptr<i32>>, 175>, zero_fill=false>(index0 = null<ptr<ptr<i32>>>, index1 = null<ptr<ptr<i32>>>, index2 = null<ptr<ptr<i32>>>, index3 = null<ptr<ptr<i32>>>, index4 = null<ptr<ptr<i32>>>, index5 = null<ptr<ptr<i32>>>, index6 = null<ptr<ptr<i32>>>, index7 = null<ptr<ptr<i32>>>, index8 = null<ptr<ptr<i32>>>, index9 = null<ptr<ptr<i32>>>, index10 = null<ptr<ptr<i32>>>, index11 = null<ptr<ptr<i32>>>, index12 = null<ptr<ptr<i32>>>, index13 = null<ptr<ptr<i32>>>, index14 = null<ptr<ptr<i32>>>, index15 = null<ptr<ptr<i32>>>, index16 = null<ptr<ptr<i32>>>, index17 = null<ptr<ptr<i32>>>, index18 = null<ptr<ptr<i32>>>, index19 = null<ptr<ptr<i32>>>, index20 = null<ptr<ptr<i32>>>, index21 = null<ptr<ptr<i32>>>, index22 = null<ptr<ptr<i32>>>, index23 = null<ptr<ptr<i32>>>, index24 = null<ptr<ptr<i32>>>, index25 = null<ptr<ptr<i32>>>, index26 = null<ptr<ptr<i32>>>, index27 = null<ptr<ptr<i32>>>, index28 = null<ptr<ptr<i32>>>, index29 = null<ptr<ptr<i32>>>, index30 = null<ptr<ptr<i32>>>, index31 = null<ptr<ptr<i32>>>, index32 = null<ptr<ptr<i32>>>, index33 = null<ptr<ptr<i32>>>, index34 = null<ptr<ptr<i32>>>, index35 = null<ptr<ptr<i32>>>, index36 = null<ptr<ptr<i32>>>, index37 = null<ptr<ptr<i32>>>, index38 = null<ptr<ptr<i32>>>, index39 = null<ptr<ptr<i32>>>, index40 = null<ptr<ptr<i32>>>, index41 = null<ptr<ptr<i32>>>, index42 = null<ptr<ptr<i32>>>, index43 = null<ptr<ptr<i32>>>, index44 = null<ptr<ptr<i32>>>, index45 = null<ptr<ptr<i32>>>, index46 = null<ptr<ptr<i32>>>, index47 = null<ptr<ptr<i32>>>, index48 = null<ptr<ptr<i32>>>, index49 = null<ptr<ptr<i32>>>, index50 = null<ptr<ptr<i32>>>, index51 = null<ptr<ptr<i32>>>, index52 = null<ptr<ptr<i32>>>, index53 = null<ptr<ptr<i32>>>, index54 = null<ptr<ptr<i32>>>, index55 = null<ptr<ptr<i32>>>, index56 = null<ptr<ptr<i32>>>, index57 = null<ptr<ptr<i32>>>, index58 = null<ptr<ptr<i32>>>, index59 = null<ptr<ptr<i32>>>, index60 = null<ptr<ptr<i32>>>, index61 = null<ptr<ptr<i32>>>, index62 = null<ptr<ptr<i32>>>, index63 = null<ptr<ptr<i32>>>, index64 = null<ptr<ptr<i32>>>, index65 = null<ptr<ptr<i32>>>, index66 = null<ptr<ptr<i32>>>, index67 = null<ptr<ptr<i32>>>, index68 = null<ptr<ptr<i32>>>, index69 = null<ptr<ptr<i32>>>, index70 = null<ptr<ptr<i32>>>, index71 = null<ptr<ptr<i32>>>, index72 = null<ptr<ptr<i32>>>, index73 = null<ptr<ptr<i32>>>, index74 = null<ptr<ptr<i32>>>, index75 = null<ptr<ptr<i32>>>, index76 = null<ptr<ptr<i32>>>, index77 = null<ptr<ptr<i32>>>, index78 = null<ptr<ptr<i32>>>, index79 = null<ptr<ptr<i32>>>, index80 = null<ptr<ptr<i32>>>, index81 = null<ptr<ptr<i32>>>, index82 = null<ptr<ptr<i32>>>, index83 = null<ptr<ptr<i32>>>, index84 = null<ptr<ptr<i32>>>, index85 = null<ptr<ptr<i32>>>, index86 = null<ptr<ptr<i32>>>, index87 = null<ptr<ptr<i32>>>, index88 = null<ptr<ptr<i32>>>, index89 = null<ptr<ptr<i32>>>, index90 = null<ptr<ptr<i32>>>, index91 = null<ptr<ptr<i32>>>, index92 = null<ptr<ptr<i32>>>, index93 = null<ptr<ptr<i32>>>, index94 = null<ptr<ptr<i32>>>, index95 = null<ptr<ptr<i32>>>, index96 = null<ptr<ptr<i32>>>, index97 = null<ptr<ptr<i32>>>, index98 = null<ptr<ptr<i32>>>, index99 = null<ptr<ptr<i32>>>, index100 = null<ptr<ptr<i32>>>, index101 = null<ptr<ptr<i32>>>, index102 = null<ptr<ptr<i32>>>, index103 = null<ptr<ptr<i32>>>, index104 = null<ptr<ptr<i32>>>, index105 = null<ptr<ptr<i32>>>, index106 = null<ptr<ptr<i32>>>, index107 = null<ptr<ptr<i32>>>, index108 = null<ptr<ptr<i32>>>, index109 = null<ptr<ptr<i32>>>, index110 = null<ptr<ptr<i32>>>, index111 = addr_of<ptr<ptr<i32>>>(%23), index112 = null<ptr<ptr<i32>>>, index113 = null<ptr<ptr<i32>>>, index114 = null<ptr<ptr<i32>>>, index115 = null<ptr<ptr<i32>>>, index116 = addr_of<ptr<ptr<i32>>>(%23), index117 = addr_of<ptr<ptr<i32>>>(%23), index118 = addr_of<ptr<ptr<i32>>>(%23), index119 = addr_of<ptr<ptr<i32>>>(%23), index120 = addr_of<ptr<ptr<i32>>>(%23), index121 = addr_of<ptr<ptr<i32>>>(%23), index122 = addr_of<ptr<ptr<i32>>>(%23), index123 = null<ptr<ptr<i32>>>, index124 = addr_of<ptr<ptr<i32>>>(%23), index125 = null<ptr<ptr<i32>>>, index126 = addr_of<ptr<ptr<i32>>>(%23), index127 = addr_of<ptr<ptr<i32>>>(%23), index128 = addr_of<ptr<ptr<i32>>>(%23), index129 = null<ptr<ptr<i32>>>, index130 = addr_of<ptr<ptr<i32>>>(%23), index131 = addr_of<ptr<ptr<i32>>>(%23), index132 = null<ptr<ptr<i32>>>, index133 = addr_of<ptr<ptr<i32>>>(%23), index134 = addr_of<ptr<ptr<i32>>>(%23), index135 = addr_of<ptr<ptr<i32>>>(%23), index136 = addr_of<ptr<ptr<i32>>>(%23), index137 = null<ptr<ptr<i32>>>, index138 = addr_of<ptr<ptr<i32>>>(%23), index139 = addr_of<ptr<ptr<i32>>>(%23), index140 = addr_of<ptr<ptr<i32>>>(%23), index141 = addr_of<ptr<ptr<i32>>>(%23), index142 = addr_of<ptr<ptr<i32>>>(%23), index143 = null<ptr<ptr<i32>>>, index144 = addr_of<ptr<ptr<i32>>>(%23), index145 = addr_of<ptr<ptr<i32>>>(%23), index146 = null<ptr<ptr<i32>>>, index147 = addr_of<ptr<ptr<i32>>>(%23), index148 = null<ptr<ptr<i32>>>, index149 = addr_of<ptr<ptr<i32>>>(%23), index150 = addr_of<ptr<ptr<i32>>>(%23), index151 = null<ptr<ptr<i32>>>, index152 = addr_of<ptr<ptr<i32>>>(%23), index153 = addr_of<ptr<ptr<i32>>>(%23), index154 = addr_of<ptr<ptr<i32>>>(%23), index155 = addr_of<ptr<ptr<i32>>>(%23), index156 = addr_of<ptr<ptr<i32>>>(%23), index157 = null<ptr<ptr<i32>>>, index158 = addr_of<ptr<ptr<i32>>>(%23), index159 = null<ptr<ptr<i32>>>, index160 = null<ptr<ptr<i32>>>, index161 = null<ptr<ptr<i32>>>, index162 = addr_of<ptr<ptr<i32>>>(%23), index163 = addr_of<ptr<ptr<i32>>>(%23), index164 = addr_of<ptr<ptr<i32>>>(%23), index165 = null<ptr<ptr<i32>>>, index166 = null<ptr<ptr<i32>>>, index167 = addr_of<ptr<ptr<i32>>>(%23), index168 = addr_of<ptr<ptr<i32>>>(%23), index169 = addr_of<ptr<ptr<i32>>>(%23), index170 = null<ptr<ptr<i32>>>, index171 = addr_of<ptr<ptr<i32>>>(%23), index172 = null<ptr<ptr<i32>>>, index173 = addr_of<ptr<ptr<i32>>>(%23), index174 = addr_of<ptr<ptr<i32>>>(%23));
// DEFAULT-NEXT:                                 let %25 w: array<ptr<ptr<ptr<i32>>>, 1> [storage=automatic] = aggregate<array<ptr<ptr<ptr<i32>>>, 1>, zero_fill=false>(index0 = addr_of<ptr<ptr<ptr<i32>>>>(deref(ptr_offset<ptr<ptr<ptr<i32>>>, subtract=false, element=ptr<ptr<i32>>, overflow=ub>(array_decay<ptr<ptr<ptr<i32>>>, length=Some(175)>(%24), const<i32>(0)))));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(deref(read<ptr<i32>>(%19))), const<i32>(0))
// DEFAULT-NEXT:                                     break %35;
// DEFAULT-NEXT:                                 return const<i32>(0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     write<i32>(deref(read<ptr<i32>, volatile>(%18)), const<i32>(0));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %26 @fn3() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %27 y: array<ptr<i32>, 8> [storage=automatic] [align=16] = aggregate<array<ptr<i32>, 8>, zero_fill=false>(index0 = null<ptr<i32>>, index1 = null<ptr<i32>>, index2 = null<ptr<i32>>, index3 = null<ptr<i32>>, index4 = null<ptr<i32>>, index5 = null<ptr<i32>>, index6 = null<ptr<i32>>, index7 = null<ptr<i32>>);
// DEFAULT-NEXT:         for %36
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: ne<i32>(read<i32>(%5), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %47: i32 [synthetic] = read<i32>(%5);
// DEFAULT-NEXT:                 let %48: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%47), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%5, read<i32>(%48));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(%17, const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn() -> i32>(%22), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %28 z: array<ptr<i32>, 6> [storage=automatic] [align=16] = aggregate<array<ptr<i32>, 6>, zero_fill=true>();
// DEFAULT-NEXT:                 for %37
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                     condition: lt<i32>(widen<i32, reason=promotion>(read<i8>(%20)), const<i32>(1))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %49: i8 [synthetic] = read<i8>(%20);
// DEFAULT-NEXT:                         let %50: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%49)), const<i32>(1)));
// DEFAULT-NEXT:                         write<i8>(%20, read<i8>(%50));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         write<i32>(deref(read<ptr<i32>, volatile>(%18)), const<i32>(0));
// DEFAULT-NEXT:                 let %29 t1: array<i32, 7> [storage=automatic] [align=16];
// DEFAULT-NEXT:                 for %38
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                     condition: ne<i32>(read<i32>(%3), const<i32>(0))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %51: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:                         let %52: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%51), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%3, read<i32>(%52));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         write<i32>(%16, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(7)>(%29), const<i32>(0)))));
// DEFAULT-NEXT:                 for %39
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                     condition: ne<i32>(read<i32>(%4), const<i32>(0))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %53: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:                         let %54: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%53), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%4, read<i32>(%54));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             let %30 t2: ptr<ptr<i32>> [storage=automatic] = addr_of<ptr<ptr<i32>>>(deref(ptr_offset<ptr<ptr<i32>>, subtract=false, element=ptr<i32>, overflow=ub>(array_decay<ptr<ptr<i32>>, length=Some(8)>(%27), const<i32>(0))));
// DEFAULT-NEXT:                             let %31 t3: ptr<ptr<ptr<i32>>> [storage=automatic] = addr_of<ptr<ptr<ptr<i32>>>>(%30);
// DEFAULT-NEXT:                             write<ptr<ptr<i32>>>(deref(read<ptr<ptr<ptr<i32>>>>(%31)), addr_of<ptr<ptr<i32>>>(deref(ptr_offset<ptr<ptr<i32>>, subtract=false, element=ptr<i32>, overflow=ub>(array_decay<ptr<ptr<i32>>, length=Some(6)>(%28), const<i32>(0)))));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<ptr<ptr<i32>>>(deref(read<ptr<ptr<ptr<i32>>>, volatile>(%13)), null<ptr<ptr<i32>>>);
// DEFAULT-NEXT:         for %40
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i8>(%20, truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             condition: omitted
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 write<i8>(%20, truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %32 t4: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%9), widen<i32, reason=promotion>(read<i8>(%20))))), const<i32>(0))
// DEFAULT-NEXT:                         break %40;
// DEFAULT-NEXT:                     write<ptr<i32>>(deref(read<ptr<ptr<i32>>>(%12)), addr_of<ptr<i32>>(%32));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %33 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         for %41
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: ne<i32>(read<i32>(%6), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %55: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:                 let %56: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%55), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%6, read<i32>(%56));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%1), const<i32>(0))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%26);
// DEFAULT-NEXT:         for %42
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: ne<i32>(read<i32>(%7), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %57: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                 let %58: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%57), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%7, read<i32>(%58));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%21);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%21);
// DEFAULT-NEXT:         if ne<i8>(read<i8>(%20), const<i8>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
