// Origin: abbott@dima.unige.it
// PR c/5120

extern void  abort(void);
extern void *malloc(__SIZE_TYPE__);
extern void *calloc(__SIZE_TYPE__, __SIZE_TYPE__);

typedef unsigned int FFelem;

FFelem FFmul(const FFelem x, const FFelem y) { return x; }

struct DUPFFstruct {
  int     maxdeg;
  int     deg;
  FFelem *coeffs;
};

typedef struct DUPFFstruct *DUPFF;

int DUPFFdeg(const DUPFF f) { return f->deg; }

DUPFF DUPFFnew(const int maxdeg) {
  DUPFF ans   = (DUPFF)malloc(sizeof(struct DUPFFstruct));
  ans->coeffs = 0;
  if (maxdeg >= 0)
    ans->coeffs = (FFelem *)calloc(maxdeg + 1, sizeof(FFelem));
  ans->maxdeg = maxdeg;
  ans->deg    = -1;
  return ans;
}

void DUPFFfree(DUPFF x) {}

void DUPFFswap(DUPFF x, DUPFF y) {}

DUPFF DUPFFcopy(const DUPFF x) { return x; }

void DUPFFshift_add(DUPFF f, const DUPFF g, int deg, const FFelem coeff) {}

DUPFF DUPFFexgcd(DUPFF *fcofac, DUPFF *gcofac, const DUPFF f, const DUPFF g) {
  DUPFF  u, v, uf, ug, vf, vg;
  FFelem q, lcu, lcvrecip, p;
  int    df, dg, du, dv;

  __builtin_printf("DUPFFexgcd called on degrees %d and %d\n", DUPFFdeg(f),
                   DUPFFdeg(g));
  if (DUPFFdeg(f) < DUPFFdeg(g))
    return DUPFFexgcd(gcofac, fcofac, g, f); /*** BUG IN THIS LINE ***/
  if (DUPFFdeg(f) != 2 || DUPFFdeg(g) != 1)
    abort();
  if (f->coeffs[0] == 0)
    return f;
  /****** NEVER REACH HERE IN THE EXAMPLE ******/
  p = 2;

  df = DUPFFdeg(f);
  if (df < 0)
    df = 0; /* both inputs are zero */
  dg = DUPFFdeg(g);
  if (dg < 0)
    dg = 0; /* one input is zero */
  u = DUPFFcopy(f);
  v = DUPFFcopy(g);

  uf            = DUPFFnew(dg);
  uf->coeffs[0] = 1;
  uf->deg       = 0;
  ug            = DUPFFnew(df);
  vf            = DUPFFnew(dg);
  vg            = DUPFFnew(df);
  vg->coeffs[0] = 1;
  vg->deg       = 0;

  while (DUPFFdeg(v) > 0) {
    dv       = DUPFFdeg(v);
    lcvrecip = FFmul(1, v->coeffs[dv]);
    while (DUPFFdeg(u) >= dv) {
      du  = DUPFFdeg(u);
      lcu = u->coeffs[du];
      q   = FFmul(lcu, lcvrecip);
      DUPFFshift_add(u, v, du - dv, p - q);
      DUPFFshift_add(uf, vf, du - dv, p - q);
      DUPFFshift_add(ug, vg, du - dv, p - q);
    }
    DUPFFswap(u, v);
    DUPFFswap(uf, vf);
    DUPFFswap(ug, vg);
  }
  if (DUPFFdeg(v) == 0) {
    DUPFFswap(u, v);
    DUPFFswap(uf, vf);
    DUPFFswap(ug, vg);
  }
  DUPFFfree(vf);
  DUPFFfree(vg);
  DUPFFfree(v);
  *fcofac = uf;
  *gcofac = ug;
  return u;
}

int main() {
  DUPFF f, g, cf, cg, h;
  f            = DUPFFnew(1);
  f->coeffs[1] = 1;
  f->deg       = 1;
  g            = DUPFFnew(2);
  g->coeffs[2] = 1;
  g->deg       = 2;

  __builtin_printf("calling DUPFFexgcd on degrees %d and %d\n", DUPFFdeg(f),
                   DUPFFdeg(g));
  h = DUPFFexgcd(&cf, &cg, f, g);
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
// DEFAULT-NEXT:     type @type[[TYPE_FFelem:[0-9]+]] FFelem = u32;
// DEFAULT-NEXT:     type @type[[TYPE_DUPFFstruct:[0-9]+]] DUPFFstruct = struct {
// DEFAULT-NEXT:         field0 maxdeg: i32;
// DEFAULT-NEXT:         field1 deg: i32;
// DEFAULT-NEXT:         field2 coeffs: ptr<u32>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_DUPFF:[0-9]+]] DUPFF = ptr<@type[[TYPE_DUPFFstruct]]>;
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 40> [storage=static] = code_units<array<i8, 40>>([68, 85, 80, 70, 70, 101, 120, 103, 99, 100, 32, 99, 97, 108, 108, 101, 100, 32, 111, 110, 32, 100, 101, 103, 114, 101, 101, 115, 32, 37, 100, 32, 97, 110, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 41> [storage=static] = code_units<array<i8, 41>>([99, 97, 108, 108, 105, 110, 103, 32, 68, 85, 80, 70, 70, 101, 120, 103, 99, 100, 32, 111, 110, 32, 100, 101, 103, 114, 101, 101, 115, 32, 37, 100, 32, 97, 110, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_malloc:[0-9]+]] @malloc(%[[VALUE0:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_calloc:[0-9]+]] @calloc(%[[VALUE1:[0-9]+]] <unnamed>: u64, %[[VALUE2:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_FFmul:[0-9]+]] @FFmul(%[[VALUE_x:[0-9]+]] x: u32 [const], %[[VALUE_y:[0-9]+]] y: u32 [const]) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<u32>(%[[VALUE_x]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_DUPFFdeg:[0-9]+]] @DUPFFdeg(%[[VALUE_f:[0-9]+]] f: ptr<@type[[TYPE_DUPFFstruct]]> [const]) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(field1(deref(read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_f]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_DUPFFnew:[0-9]+]] @DUPFFnew(%[[VALUE_maxdeg:[0-9]+]] maxdeg: i32 [const]) -> ptr<@type[[TYPE_DUPFFstruct]]> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_ans:[0-9]+]] ans: ptr<@type[[TYPE_DUPFFstruct]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE_DUPFFstruct]]>, reason=explicit>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_malloc]], const<u64>(16)));
// DEFAULT-NEXT:         write<ptr<u32>>(field2(deref(read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_ans]]))), null<ptr<u32>>);
// DEFAULT-NEXT:         if ge<i32>(read<i32>(%[[VALUE_maxdeg]]), const<i32>(0))
// DEFAULT-NEXT:             write<ptr<u32>>(field2(deref(read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_ans]]))), pointer_cast<ptr<u32>, reason=explicit>(call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_calloc]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(read<i32>(%[[VALUE_maxdeg]]), const<i32>(1)))), const<u64>(4))));
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_ans]]))), read<i32>(%[[VALUE_maxdeg]]));
// DEFAULT-NEXT:         write<i32>(field1(deref(read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_ans]]))), neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:         return read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_ans]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_DUPFFfree:[0-9]+]] @DUPFFfree(%[[VALUE_x_2:[0-9]+]] x: ptr<@type[[TYPE_DUPFFstruct]]>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_DUPFFswap:[0-9]+]] @DUPFFswap(%[[VALUE_x_3:[0-9]+]] x: ptr<@type[[TYPE_DUPFFstruct]]>, %[[VALUE_y_2:[0-9]+]] y: ptr<@type[[TYPE_DUPFFstruct]]>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_DUPFFcopy:[0-9]+]] @DUPFFcopy(%[[VALUE_x_4:[0-9]+]] x: ptr<@type[[TYPE_DUPFFstruct]]> [const]) -> ptr<@type[[TYPE_DUPFFstruct]]> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_x_4]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_DUPFFshift_add:[0-9]+]] @DUPFFshift_add(%[[VALUE_f_2:[0-9]+]] f: ptr<@type[[TYPE_DUPFFstruct]]>, %[[VALUE_g:[0-9]+]] g: ptr<@type[[TYPE_DUPFFstruct]]> [const], %[[VALUE_deg:[0-9]+]] deg: i32, %[[VALUE_coeff:[0-9]+]] coeff: u32 [const]) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_printf:[0-9]+]] @__builtin_printf(%[[VALUE3:[0-9]+]] <unnamed>: ptr<const i8>, ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_DUPFFexgcd:[0-9]+]] @DUPFFexgcd(%[[VALUE_fcofac:[0-9]+]] fcofac: ptr<ptr<@type[[TYPE_DUPFFstruct]]>>, %[[VALUE_gcofac:[0-9]+]] gcofac: ptr<ptr<@type[[TYPE_DUPFFstruct]]>>, %[[VALUE_f_3:[0-9]+]] f: ptr<@type[[TYPE_DUPFFstruct]]> [const], %[[VALUE_g_2:[0-9]+]] g: ptr<@type[[TYPE_DUPFFstruct]]> [const]) -> ptr<@type[[TYPE_DUPFFstruct]]> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_u:[0-9]+]] u: ptr<@type[[TYPE_DUPFFstruct]]> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_v:[0-9]+]] v: ptr<@type[[TYPE_DUPFFstruct]]> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_uf:[0-9]+]] uf: ptr<@type[[TYPE_DUPFFstruct]]> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ug:[0-9]+]] ug: ptr<@type[[TYPE_DUPFFstruct]]> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_vf:[0-9]+]] vf: ptr<@type[[TYPE_DUPFFstruct]]> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_vg:[0-9]+]] vg: ptr<@type[[TYPE_DUPFFstruct]]> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_q:[0-9]+]] q: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_lcu:[0-9]+]] lcu: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_lcvrecip:[0-9]+]] lcvrecip: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_df:[0-9]+]] df: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_dg:[0-9]+]] dg: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_du:[0-9]+]] du: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_dv:[0-9]+]] dv: i32 [storage=automatic];
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(40)>(%[[VALUE_str]])), call<i32, signature=fn(ptr<@type[[TYPE_DUPFFstruct]]>) -> i32>(%[[VALUE_DUPFFdeg]], read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_f_3]])), call<i32, signature=fn(ptr<@type[[TYPE_DUPFFstruct]]>) -> i32>(%[[VALUE_DUPFFdeg]], read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_g_2]])));
// DEFAULT-NEXT:         if lt<i32>(call<i32, signature=fn(ptr<@type[[TYPE_DUPFFstruct]]>) -> i32>(%[[VALUE_DUPFFdeg]], read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_f_3]])), call<i32, signature=fn(ptr<@type[[TYPE_DUPFFstruct]]>) -> i32>(%[[VALUE_DUPFFdeg]], read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_g_2]])))
// DEFAULT-NEXT:             return call<ptr<@type[[TYPE_DUPFFstruct]]>, signature=fn(ptr<ptr<@type[[TYPE_DUPFFstruct]]>>, ptr<ptr<@type[[TYPE_DUPFFstruct]]>>, ptr<@type[[TYPE_DUPFFstruct]]>, ptr<@type[[TYPE_DUPFFstruct]]>) -> ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_DUPFFexgcd]], read<ptr<ptr<@type[[TYPE_DUPFFstruct]]>>>(%[[VALUE_gcofac]]), read<ptr<ptr<@type[[TYPE_DUPFFstruct]]>>>(%[[VALUE_fcofac]]), read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_g_2]]), read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_f_3]]));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type[[TYPE_DUPFFstruct]]>) -> i32>(%[[VALUE_DUPFFdeg]], read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_f_3]])), const<i32>(2))
// DEFAULT-NEXT:             write<bool>(%[[VALUE4]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE4]], ne<i32>(call<i32, signature=fn(ptr<@type[[TYPE_DUPFFstruct]]>) -> i32>(%[[VALUE_DUPFFdeg]], read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_g_2]])), const<i32>(1)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE4]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if eq<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(field2(deref(read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_f_3]])))), const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:             return read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_f_3]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_p]], reinterpret<u32, reason=assign, fits=always>(const<i32>(2)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_df]], call<i32, signature=fn(ptr<@type[[TYPE_DUPFFstruct]]>) -> i32>(%[[VALUE_DUPFFdeg]], read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_f_3]])));
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%[[VALUE_df]]), const<i32>(0))
// DEFAULT-NEXT:             write<i32>(%[[VALUE_df]], const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_dg]], call<i32, signature=fn(ptr<@type[[TYPE_DUPFFstruct]]>) -> i32>(%[[VALUE_DUPFFdeg]], read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_g_2]])));
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%[[VALUE_dg]]), const<i32>(0))
// DEFAULT-NEXT:             write<i32>(%[[VALUE_dg]], const<i32>(0));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_u]], call<ptr<@type[[TYPE_DUPFFstruct]]>, signature=fn(ptr<@type[[TYPE_DUPFFstruct]]>) -> ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_DUPFFcopy]], read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_f_3]])));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_v]], call<ptr<@type[[TYPE_DUPFFstruct]]>, signature=fn(ptr<@type[[TYPE_DUPFFstruct]]>) -> ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_DUPFFcopy]], read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_g_2]])));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_uf]], call<ptr<@type[[TYPE_DUPFFstruct]]>, signature=fn(i32) -> ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_DUPFFnew]], read<i32>(%[[VALUE_dg]])));
// DEFAULT-NEXT:         write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(field2(deref(read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_uf]])))), const<i32>(0))), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<i32>(field1(deref(read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_uf]]))), const<i32>(0));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_ug]], call<ptr<@type[[TYPE_DUPFFstruct]]>, signature=fn(i32) -> ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_DUPFFnew]], read<i32>(%[[VALUE_df]])));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_vf]], call<ptr<@type[[TYPE_DUPFFstruct]]>, signature=fn(i32) -> ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_DUPFFnew]], read<i32>(%[[VALUE_dg]])));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_vg]], call<ptr<@type[[TYPE_DUPFFstruct]]>, signature=fn(i32) -> ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_DUPFFnew]], read<i32>(%[[VALUE_df]])));
// DEFAULT-NEXT:         write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(field2(deref(read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_vg]])))), const<i32>(0))), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<i32>(field1(deref(read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_vg]]))), const<i32>(0));
// DEFAULT-NEXT:         while %[[VALUE5:[0-9]+]] gt<i32>(call<i32, signature=fn(ptr<@type[[TYPE_DUPFFstruct]]>) -> i32>(%[[VALUE_DUPFFdeg]], read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_v]])), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_dv]], call<i32, signature=fn(ptr<@type[[TYPE_DUPFFstruct]]>) -> i32>(%[[VALUE_DUPFFdeg]], read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_v]])));
// DEFAULT-NEXT:                 write<u32>(%[[VALUE_lcvrecip]], call<u32, signature=fn(u32, u32) -> u32>(%[[VALUE_FFmul]], reinterpret<u32, reason=arg, fits=always>(const<i32>(1)), read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(field2(deref(read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_v]])))), read<i32>(%[[VALUE_dv]]))))));
// DEFAULT-NEXT:                 while %[[VALUE6:[0-9]+]] ge<i32>(call<i32, signature=fn(ptr<@type[[TYPE_DUPFFstruct]]>) -> i32>(%[[VALUE_DUPFFdeg]], read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_u]])), read<i32>(%[[VALUE_dv]]))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_du]], call<i32, signature=fn(ptr<@type[[TYPE_DUPFFstruct]]>) -> i32>(%[[VALUE_DUPFFdeg]], read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_u]])));
// DEFAULT-NEXT:                         write<u32>(%[[VALUE_lcu]], read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(field2(deref(read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_u]])))), read<i32>(%[[VALUE_du]])))));
// DEFAULT-NEXT:                         write<u32>(%[[VALUE_q]], call<u32, signature=fn(u32, u32) -> u32>(%[[VALUE_FFmul]], read<u32>(%[[VALUE_lcu]]), read<u32>(%[[VALUE_lcvrecip]])));
// DEFAULT-NEXT:                         call<void, signature=fn(ptr<@type[[TYPE_DUPFFstruct]]>, ptr<@type[[TYPE_DUPFFstruct]]>, i32, u32) -> void>(%[[VALUE_DUPFFshift_add]], read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_u]]), read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_v]]), sub<i32, overflow=ub>(read<i32>(%[[VALUE_du]]), read<i32>(%[[VALUE_dv]])), sub<u32, overflow=wrap>(read<u32>(%[[VALUE_p]]), read<u32>(%[[VALUE_q]])));
// DEFAULT-NEXT:                         call<void, signature=fn(ptr<@type[[TYPE_DUPFFstruct]]>, ptr<@type[[TYPE_DUPFFstruct]]>, i32, u32) -> void>(%[[VALUE_DUPFFshift_add]], read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_uf]]), read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_vf]]), sub<i32, overflow=ub>(read<i32>(%[[VALUE_du]]), read<i32>(%[[VALUE_dv]])), sub<u32, overflow=wrap>(read<u32>(%[[VALUE_p]]), read<u32>(%[[VALUE_q]])));
// DEFAULT-NEXT:                         call<void, signature=fn(ptr<@type[[TYPE_DUPFFstruct]]>, ptr<@type[[TYPE_DUPFFstruct]]>, i32, u32) -> void>(%[[VALUE_DUPFFshift_add]], read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_ug]]), read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_vg]]), sub<i32, overflow=ub>(read<i32>(%[[VALUE_du]]), read<i32>(%[[VALUE_dv]])), sub<u32, overflow=wrap>(read<u32>(%[[VALUE_p]]), read<u32>(%[[VALUE_q]])));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<@type[[TYPE_DUPFFstruct]]>, ptr<@type[[TYPE_DUPFFstruct]]>) -> void>(%[[VALUE_DUPFFswap]], read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_u]]), read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_v]]));
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<@type[[TYPE_DUPFFstruct]]>, ptr<@type[[TYPE_DUPFFstruct]]>) -> void>(%[[VALUE_DUPFFswap]], read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_uf]]), read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_vf]]));
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<@type[[TYPE_DUPFFstruct]]>, ptr<@type[[TYPE_DUPFFstruct]]>) -> void>(%[[VALUE_DUPFFswap]], read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_ug]]), read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_vg]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if eq<i32>(call<i32, signature=fn(ptr<@type[[TYPE_DUPFFstruct]]>) -> i32>(%[[VALUE_DUPFFdeg]], read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_v]])), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<@type[[TYPE_DUPFFstruct]]>, ptr<@type[[TYPE_DUPFFstruct]]>) -> void>(%[[VALUE_DUPFFswap]], read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_u]]), read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_v]]));
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<@type[[TYPE_DUPFFstruct]]>, ptr<@type[[TYPE_DUPFFstruct]]>) -> void>(%[[VALUE_DUPFFswap]], read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_uf]]), read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_vf]]));
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<@type[[TYPE_DUPFFstruct]]>, ptr<@type[[TYPE_DUPFFstruct]]>) -> void>(%[[VALUE_DUPFFswap]], read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_ug]]), read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_vg]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_DUPFFstruct]]>) -> void>(%[[VALUE_DUPFFfree]], read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_vf]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_DUPFFstruct]]>) -> void>(%[[VALUE_DUPFFfree]], read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_vg]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_DUPFFstruct]]>) -> void>(%[[VALUE_DUPFFfree]], read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_v]]));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_DUPFFstruct]]>>(deref(read<ptr<ptr<@type[[TYPE_DUPFFstruct]]>>>(%[[VALUE_fcofac]])), read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_uf]]));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_DUPFFstruct]]>>(deref(read<ptr<ptr<@type[[TYPE_DUPFFstruct]]>>>(%[[VALUE_gcofac]])), read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_ug]]));
// DEFAULT-NEXT:         return read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_u]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_f_4:[0-9]+]] f: ptr<@type[[TYPE_DUPFFstruct]]> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_g_3:[0-9]+]] g: ptr<@type[[TYPE_DUPFFstruct]]> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_cf:[0-9]+]] cf: ptr<@type[[TYPE_DUPFFstruct]]> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_cg:[0-9]+]] cg: ptr<@type[[TYPE_DUPFFstruct]]> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_h:[0-9]+]] h: ptr<@type[[TYPE_DUPFFstruct]]> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_f_4]], call<ptr<@type[[TYPE_DUPFFstruct]]>, signature=fn(i32) -> ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_DUPFFnew]], const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(field2(deref(read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_f_4]])))), const<i32>(1))), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<i32>(field1(deref(read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_f_4]]))), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_g_3]], call<ptr<@type[[TYPE_DUPFFstruct]]>, signature=fn(i32) -> ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_DUPFFnew]], const<i32>(2)));
// DEFAULT-NEXT:         write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(field2(deref(read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_g_3]])))), const<i32>(2))), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<i32>(field1(deref(read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_g_3]]))), const<i32>(2));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(41)>(%[[VALUE_str_2]])), call<i32, signature=fn(ptr<@type[[TYPE_DUPFFstruct]]>) -> i32>(%[[VALUE_DUPFFdeg]], read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_f_4]])), call<i32, signature=fn(ptr<@type[[TYPE_DUPFFstruct]]>) -> i32>(%[[VALUE_DUPFFdeg]], read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_g_3]])));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_h]], call<ptr<@type[[TYPE_DUPFFstruct]]>, signature=fn(ptr<ptr<@type[[TYPE_DUPFFstruct]]>>, ptr<ptr<@type[[TYPE_DUPFFstruct]]>>, ptr<@type[[TYPE_DUPFFstruct]]>, ptr<@type[[TYPE_DUPFFstruct]]>) -> ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_DUPFFexgcd]], addr_of<ptr<ptr<@type[[TYPE_DUPFFstruct]]>>>(%[[VALUE_cf]]), addr_of<ptr<ptr<@type[[TYPE_DUPFFstruct]]>>>(%[[VALUE_cg]]), read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_f_4]]), read<ptr<@type[[TYPE_DUPFFstruct]]>>(%[[VALUE_g_3]])));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
