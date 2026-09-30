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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: array<i32, 1> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_i:[0-9]+]] i: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_j:[0-9]+]] j: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_k:[0-9]+]] k: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_m:[0-9]+]] m: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_q:[0-9]+]] q: array<i32, 2> [storage=static] = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_t:[0-9]+]] t: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: volatile i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_r:[0-9]+]] r: ptr<ptr<i32>> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_s:[0-9]+]] s: volatile ptr<ptr<ptr<i32>>> [storage=static] = addr_of<ptr<ptr<ptr<i32>>>>(%[[VALUE_r]]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g:[0-9]+]] g: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_o:[0-9]+]] o: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_x:[0-9]+]] x: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_h:[0-9]+]] h: volatile ptr<i32> [storage=static] = addr_of<ptr<i32>>(%[[VALUE_f]]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_p:[0-9]+]] p: ptr<i32> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_n:[0-9]+]] n: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_fn1:[0-9]+]] @fn1() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(%[[VALUE_b]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_a]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_a]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_a]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_a]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_a]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_a]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_a]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]]))))))))))))))))))))))))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_b]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_a]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_a]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_a]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_a]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_a]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_a]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_a]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]]))))))))))))))))))))))))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_b]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_a]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]]))))))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_b]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_a]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_a]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_a]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_a]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_a]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_a]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_a]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]]))))))))))))))))))))))))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_b]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_a]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_a]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_a]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_a]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_a]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_a]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_a]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]]))))))))))))))))))))))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2:[0-9]+]] @fn2() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i8>(%[[VALUE_n]], truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: ne<i32>(read<i32>(%[[VALUE_g]]), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_t]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_t]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     for %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                         condition: omitted
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_m]]);
// DEFAULT-NEXT:                             let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_m]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 read<i32, volatile>(%[[VALUE_d]]);
// DEFAULT-NEXT:                                 let %[[VALUE_u:[0-9]+]] u: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:                                 let %[[VALUE_v:[0-9]+]] v: array<ptr<ptr<i32>>, 175> [storage=automatic] [align=16] = aggregate<array<ptr<ptr<i32>>, 175>, zero_fill=false>(index0 = null<ptr<ptr<i32>>>, index1 = null<ptr<ptr<i32>>>, index2 = null<ptr<ptr<i32>>>, index3 = null<ptr<ptr<i32>>>, index4 = null<ptr<ptr<i32>>>, index5 = null<ptr<ptr<i32>>>, index6 = null<ptr<ptr<i32>>>, index7 = null<ptr<ptr<i32>>>, index8 = null<ptr<ptr<i32>>>, index9 = null<ptr<ptr<i32>>>, index10 = null<ptr<ptr<i32>>>, index11 = null<ptr<ptr<i32>>>, index12 = null<ptr<ptr<i32>>>, index13 = null<ptr<ptr<i32>>>, index14 = null<ptr<ptr<i32>>>, index15 = null<ptr<ptr<i32>>>, index16 = null<ptr<ptr<i32>>>, index17 = null<ptr<ptr<i32>>>, index18 = null<ptr<ptr<i32>>>, index19 = null<ptr<ptr<i32>>>, index20 = null<ptr<ptr<i32>>>, index21 = null<ptr<ptr<i32>>>, index22 = null<ptr<ptr<i32>>>, index23 = null<ptr<ptr<i32>>>, index24 = null<ptr<ptr<i32>>>, index25 = null<ptr<ptr<i32>>>, index26 = null<ptr<ptr<i32>>>, index27 = null<ptr<ptr<i32>>>, index28 = null<ptr<ptr<i32>>>, index29 = null<ptr<ptr<i32>>>, index30 = null<ptr<ptr<i32>>>, index31 = null<ptr<ptr<i32>>>, index32 = null<ptr<ptr<i32>>>, index33 = null<ptr<ptr<i32>>>, index34 = null<ptr<ptr<i32>>>, index35 = null<ptr<ptr<i32>>>, index36 = null<ptr<ptr<i32>>>, index37 = null<ptr<ptr<i32>>>, index38 = null<ptr<ptr<i32>>>, index39 = null<ptr<ptr<i32>>>, index40 = null<ptr<ptr<i32>>>, index41 = null<ptr<ptr<i32>>>, index42 = null<ptr<ptr<i32>>>, index43 = null<ptr<ptr<i32>>>, index44 = null<ptr<ptr<i32>>>, index45 = null<ptr<ptr<i32>>>, index46 = null<ptr<ptr<i32>>>, index47 = null<ptr<ptr<i32>>>, index48 = null<ptr<ptr<i32>>>, index49 = null<ptr<ptr<i32>>>, index50 = null<ptr<ptr<i32>>>, index51 = null<ptr<ptr<i32>>>, index52 = null<ptr<ptr<i32>>>, index53 = null<ptr<ptr<i32>>>, index54 = null<ptr<ptr<i32>>>, index55 = null<ptr<ptr<i32>>>, index56 = null<ptr<ptr<i32>>>, index57 = null<ptr<ptr<i32>>>, index58 = null<ptr<ptr<i32>>>, index59 = null<ptr<ptr<i32>>>, index60 = null<ptr<ptr<i32>>>, index61 = null<ptr<ptr<i32>>>, index62 = null<ptr<ptr<i32>>>, index63 = null<ptr<ptr<i32>>>, index64 = null<ptr<ptr<i32>>>, index65 = null<ptr<ptr<i32>>>, index66 = null<ptr<ptr<i32>>>, index67 = null<ptr<ptr<i32>>>, index68 = null<ptr<ptr<i32>>>, index69 = null<ptr<ptr<i32>>>, index70 = null<ptr<ptr<i32>>>, index71 = null<ptr<ptr<i32>>>, index72 = null<ptr<ptr<i32>>>, index73 = null<ptr<ptr<i32>>>, index74 = null<ptr<ptr<i32>>>, index75 = null<ptr<ptr<i32>>>, index76 = null<ptr<ptr<i32>>>, index77 = null<ptr<ptr<i32>>>, index78 = null<ptr<ptr<i32>>>, index79 = null<ptr<ptr<i32>>>, index80 = null<ptr<ptr<i32>>>, index81 = null<ptr<ptr<i32>>>, index82 = null<ptr<ptr<i32>>>, index83 = null<ptr<ptr<i32>>>, index84 = null<ptr<ptr<i32>>>, index85 = null<ptr<ptr<i32>>>, index86 = null<ptr<ptr<i32>>>, index87 = null<ptr<ptr<i32>>>, index88 = null<ptr<ptr<i32>>>, index89 = null<ptr<ptr<i32>>>, index90 = null<ptr<ptr<i32>>>, index91 = null<ptr<ptr<i32>>>, index92 = null<ptr<ptr<i32>>>, index93 = null<ptr<ptr<i32>>>, index94 = null<ptr<ptr<i32>>>, index95 = null<ptr<ptr<i32>>>, index96 = null<ptr<ptr<i32>>>, index97 = null<ptr<ptr<i32>>>, index98 = null<ptr<ptr<i32>>>, index99 = null<ptr<ptr<i32>>>, index100 = null<ptr<ptr<i32>>>, index101 = null<ptr<ptr<i32>>>, index102 = null<ptr<ptr<i32>>>, index103 = null<ptr<ptr<i32>>>, index104 = null<ptr<ptr<i32>>>, index105 = null<ptr<ptr<i32>>>, index106 = null<ptr<ptr<i32>>>, index107 = null<ptr<ptr<i32>>>, index108 = null<ptr<ptr<i32>>>, index109 = null<ptr<ptr<i32>>>, index110 = null<ptr<ptr<i32>>>, index111 = addr_of<ptr<ptr<i32>>>(%[[VALUE_u]]), index112 = null<ptr<ptr<i32>>>, index113 = null<ptr<ptr<i32>>>, index114 = null<ptr<ptr<i32>>>, index115 = null<ptr<ptr<i32>>>, index116 = addr_of<ptr<ptr<i32>>>(%[[VALUE_u]]), index117 = addr_of<ptr<ptr<i32>>>(%[[VALUE_u]]), index118 = addr_of<ptr<ptr<i32>>>(%[[VALUE_u]]), index119 = addr_of<ptr<ptr<i32>>>(%[[VALUE_u]]), index120 = addr_of<ptr<ptr<i32>>>(%[[VALUE_u]]), index121 = addr_of<ptr<ptr<i32>>>(%[[VALUE_u]]), index122 = addr_of<ptr<ptr<i32>>>(%[[VALUE_u]]), index123 = null<ptr<ptr<i32>>>, index124 = addr_of<ptr<ptr<i32>>>(%[[VALUE_u]]), index125 = null<ptr<ptr<i32>>>, index126 = addr_of<ptr<ptr<i32>>>(%[[VALUE_u]]), index127 = addr_of<ptr<ptr<i32>>>(%[[VALUE_u]]), index128 = addr_of<ptr<ptr<i32>>>(%[[VALUE_u]]), index129 = null<ptr<ptr<i32>>>, index130 = addr_of<ptr<ptr<i32>>>(%[[VALUE_u]]), index131 = addr_of<ptr<ptr<i32>>>(%[[VALUE_u]]), index132 = null<ptr<ptr<i32>>>, index133 = addr_of<ptr<ptr<i32>>>(%[[VALUE_u]]), index134 = addr_of<ptr<ptr<i32>>>(%[[VALUE_u]]), index135 = addr_of<ptr<ptr<i32>>>(%[[VALUE_u]]), index136 = addr_of<ptr<ptr<i32>>>(%[[VALUE_u]]), index137 = null<ptr<ptr<i32>>>, index138 = addr_of<ptr<ptr<i32>>>(%[[VALUE_u]]), index139 = addr_of<ptr<ptr<i32>>>(%[[VALUE_u]]), index140 = addr_of<ptr<ptr<i32>>>(%[[VALUE_u]]), index141 = addr_of<ptr<ptr<i32>>>(%[[VALUE_u]]), index142 = addr_of<ptr<ptr<i32>>>(%[[VALUE_u]]), index143 = null<ptr<ptr<i32>>>, index144 = addr_of<ptr<ptr<i32>>>(%[[VALUE_u]]), index145 = addr_of<ptr<ptr<i32>>>(%[[VALUE_u]]), index146 = null<ptr<ptr<i32>>>, index147 = addr_of<ptr<ptr<i32>>>(%[[VALUE_u]]), index148 = null<ptr<ptr<i32>>>, index149 = addr_of<ptr<ptr<i32>>>(%[[VALUE_u]]), index150 = addr_of<ptr<ptr<i32>>>(%[[VALUE_u]]), index151 = null<ptr<ptr<i32>>>, index152 = addr_of<ptr<ptr<i32>>>(%[[VALUE_u]]), index153 = addr_of<ptr<ptr<i32>>>(%[[VALUE_u]]), index154 = addr_of<ptr<ptr<i32>>>(%[[VALUE_u]]), index155 = addr_of<ptr<ptr<i32>>>(%[[VALUE_u]]), index156 = addr_of<ptr<ptr<i32>>>(%[[VALUE_u]]), index157 = null<ptr<ptr<i32>>>, index158 = addr_of<ptr<ptr<i32>>>(%[[VALUE_u]]), index159 = null<ptr<ptr<i32>>>, index160 = null<ptr<ptr<i32>>>, index161 = null<ptr<ptr<i32>>>, index162 = addr_of<ptr<ptr<i32>>>(%[[VALUE_u]]), index163 = addr_of<ptr<ptr<i32>>>(%[[VALUE_u]]), index164 = addr_of<ptr<ptr<i32>>>(%[[VALUE_u]]), index165 = null<ptr<ptr<i32>>>, index166 = null<ptr<ptr<i32>>>, index167 = addr_of<ptr<ptr<i32>>>(%[[VALUE_u]]), index168 = addr_of<ptr<ptr<i32>>>(%[[VALUE_u]]), index169 = addr_of<ptr<ptr<i32>>>(%[[VALUE_u]]), index170 = null<ptr<ptr<i32>>>, index171 = addr_of<ptr<ptr<i32>>>(%[[VALUE_u]]), index172 = null<ptr<ptr<i32>>>, index173 = addr_of<ptr<ptr<i32>>>(%[[VALUE_u]]), index174 = addr_of<ptr<ptr<i32>>>(%[[VALUE_u]]));
// DEFAULT-NEXT:                                 let %[[VALUE_w:[0-9]+]] w: array<ptr<ptr<ptr<i32>>>, 1> [storage=automatic] = aggregate<array<ptr<ptr<ptr<i32>>>, 1>, zero_fill=false>(index0 = addr_of<ptr<ptr<ptr<i32>>>>(deref(ptr_offset<ptr<ptr<ptr<i32>>>, subtract=false, element=ptr<ptr<i32>>, overflow=ub>(array_decay<ptr<ptr<ptr<i32>>>, length=Some(175)>(%[[VALUE_v]]), const<i32>(0)))));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(deref(read<ptr<i32>>(%[[VALUE_p]]))), const<i32>(0))
// DEFAULT-NEXT:                                     break %[[VALUE3]];
// DEFAULT-NEXT:                                 return const<i32>(0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     write<i32>(deref(read<ptr<i32>, volatile>(%[[VALUE_h]])), const<i32>(0));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3:[0-9]+]] @fn3() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_y:[0-9]+]] y: array<ptr<i32>, 8> [storage=automatic] [align=16] = aggregate<array<ptr<i32>, 8>, zero_fill=false>(index0 = null<ptr<i32>>, index1 = null<ptr<i32>>, index2 = null<ptr<i32>>, index3 = null<ptr<i32>>, index4 = null<ptr<i32>>, index5 = null<ptr<i32>>, index6 = null<ptr<i32>>, index7 = null<ptr<i32>>);
// DEFAULT-NEXT:         for %[[VALUE6:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: ne<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE8:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE7]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE8]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x]], const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_fn2]]), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_z:[0-9]+]] z: array<ptr<i32>, 6> [storage=automatic] [align=16] = aggregate<array<ptr<i32>, 6>, zero_fill=true>();
// DEFAULT-NEXT:                 for %[[VALUE9:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                     condition: lt<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_n]])), const<i32>(1))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE10:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_n]]);
// DEFAULT-NEXT:                         let %[[VALUE11:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE10]])), const<i32>(1)));
// DEFAULT-NEXT:                         write<i8>(%[[VALUE_n]], read<i8>(%[[VALUE11]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         write<i32>(deref(read<ptr<i32>, volatile>(%[[VALUE_h]])), const<i32>(0));
// DEFAULT-NEXT:                 let %[[VALUE_t1:[0-9]+]] t1: array<i32, 7> [storage=automatic] [align=16];
// DEFAULT-NEXT:                 for %[[VALUE12:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                     condition: ne<i32>(read<i32>(%[[VALUE_c]]), const<i32>(0))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE13:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_c]]);
// DEFAULT-NEXT:                         let %[[VALUE14:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE13]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_c]], read<i32>(%[[VALUE14]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_o]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(7)>(%[[VALUE_t1]]), const<i32>(0)))));
// DEFAULT-NEXT:                 for %[[VALUE15:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                     condition: ne<i32>(read<i32>(%[[VALUE_e]]), const<i32>(0))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE16:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_e]]);
// DEFAULT-NEXT:                         let %[[VALUE17:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE16]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_e]], read<i32>(%[[VALUE17]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             let %[[VALUE_t2:[0-9]+]] t2: ptr<ptr<i32>> [storage=automatic] = addr_of<ptr<ptr<i32>>>(deref(ptr_offset<ptr<ptr<i32>>, subtract=false, element=ptr<i32>, overflow=ub>(array_decay<ptr<ptr<i32>>, length=Some(8)>(%[[VALUE_y]]), const<i32>(0))));
// DEFAULT-NEXT:                             let %[[VALUE_t3:[0-9]+]] t3: ptr<ptr<ptr<i32>>> [storage=automatic] = addr_of<ptr<ptr<ptr<i32>>>>(%[[VALUE_t2]]);
// DEFAULT-NEXT:                             write<ptr<ptr<i32>>>(deref(read<ptr<ptr<ptr<i32>>>>(%[[VALUE_t3]])), addr_of<ptr<ptr<i32>>>(deref(ptr_offset<ptr<ptr<i32>>, subtract=false, element=ptr<i32>, overflow=ub>(array_decay<ptr<ptr<i32>>, length=Some(6)>(%[[VALUE_z]]), const<i32>(0)))));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<ptr<ptr<i32>>>(deref(read<ptr<ptr<ptr<i32>>>, volatile>(%[[VALUE_s]])), null<ptr<ptr<i32>>>);
// DEFAULT-NEXT:         for %[[VALUE18:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i8>(%[[VALUE_n]], truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             condition: omitted
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 write<i8>(%[[VALUE_n]], truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_t4:[0-9]+]] t4: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%[[VALUE_q]]), widen<i32, reason=promotion>(read<i8>(%[[VALUE_n]]))))), const<i32>(0))
// DEFAULT-NEXT:                         break %[[VALUE18]];
// DEFAULT-NEXT:                     write<ptr<i32>>(deref(read<ptr<ptr<i32>>>(%[[VALUE_r]])), addr_of<ptr<i32>>(%[[VALUE_t4]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         for %[[VALUE19:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: ne<i32>(read<i32>(%[[VALUE_j]]), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE20:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j]]);
// DEFAULT-NEXT:                 let %[[VALUE21:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE20]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j]], read<i32>(%[[VALUE21]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_a]]), const<i32>(0))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_fn3]]);
// DEFAULT-NEXT:         for %[[VALUE22:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: ne<i32>(read<i32>(%[[VALUE_k]]), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE23:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_k]]);
// DEFAULT-NEXT:                 let %[[VALUE24:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE23]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_k]], read<i32>(%[[VALUE24]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_fn1]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_fn1]]);
// DEFAULT-NEXT:         if ne<i8>(read<i8>(%[[VALUE_n]]), const<i8>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
