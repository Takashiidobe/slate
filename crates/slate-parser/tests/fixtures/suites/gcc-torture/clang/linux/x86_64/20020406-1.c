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
// DEFAULT-NEXT:     type @type0 FFelem = u32;
// DEFAULT-NEXT:     type @type1 DUPFFstruct = struct {
// DEFAULT-NEXT:         field0 maxdeg: i32;
// DEFAULT-NEXT:         field1 deg: i32;
// DEFAULT-NEXT:         field2 coeffs: ptr<u32>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type2 DUPFF = ptr<@type1>;
// DEFAULT-NEXT:     global %56 .str56: array<i8, 40> [storage=static] = code_units<array<i8, 40>>([68, 85, 80, 70, 70, 101, 120, 103, 99, 100, 32, 99, 97, 108, 108, 101, 100, 32, 111, 110, 32, 100, 101, 103, 114, 101, 101, 115, 32, 37, 100, 32, 97, 110, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %59 .str59: array<i8, 41> [storage=static] = code_units<array<i8, 41>>([99, 97, 108, 108, 105, 110, 103, 32, 68, 85, 80, 70, 70, 101, 120, 103, 99, 100, 32, 111, 110, 32, 100, 101, 103, 114, 101, 101, 115, 32, 37, 100, 32, 97, 110, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @malloc(%51 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %2 @calloc(%52 <unnamed>: u64, %53 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %4 @FFmul(%5 x: u32 [const], %6 y: u32 [const]) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<u32>(%5);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @DUPFFdeg(%10 f: ptr<@type1> [const]) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(field1(deref(read<ptr<@type1>>(%10))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @DUPFFnew(%12 maxdeg: i32 [const]) -> ptr<@type1> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %13 ans: ptr<@type1> [storage=automatic] = pointer_cast<ptr<@type1>, reason=explicit>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%1, const<u64>(16)));
// DEFAULT-NEXT:         write<ptr<u32>>(field2(deref(read<ptr<@type1>>(%13))), null<ptr<u32>>);
// DEFAULT-NEXT:         if ge<i32>(read<i32>(%12), const<i32>(0))
// DEFAULT-NEXT:             write<ptr<u32>>(field2(deref(read<ptr<@type1>>(%13))), pointer_cast<ptr<u32>, reason=explicit>(call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%2, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(read<i32>(%12), const<i32>(1)))), const<u64>(4))));
// DEFAULT-NEXT:             pointer_cast<ptr<u32>, reason=explicit>(call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%2, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(read<i32>(%12), const<i32>(1)))), const<u64>(4)));
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type1>>(%13))), read<i32>(%12));
// DEFAULT-NEXT:         write<i32>(field1(deref(read<ptr<@type1>>(%13))), neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:         return read<ptr<@type1>>(%13);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @DUPFFfree(%15 x: ptr<@type1>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @DUPFFswap(%17 x: ptr<@type1>, %18 y: ptr<@type1>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @DUPFFcopy(%20 x: ptr<@type1> [const]) -> ptr<@type1> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<ptr<@type1>>(%20);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @DUPFFshift_add(%22 f: ptr<@type1>, %23 g: ptr<@type1> [const], %24 deg: i32, %25 coeff: u32 [const]) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %55 @__builtin_printf(%54 <unnamed>: ptr<const i8>, ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %26 @DUPFFexgcd(%27 fcofac: ptr<ptr<@type1>>, %28 gcofac: ptr<ptr<@type1>>, %29 f: ptr<@type1> [const], %30 g: ptr<@type1> [const]) -> ptr<@type1> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %31 u: ptr<@type1> [storage=automatic];
// DEFAULT-NEXT:         let %32 v: ptr<@type1> [storage=automatic];
// DEFAULT-NEXT:         let %33 uf: ptr<@type1> [storage=automatic];
// DEFAULT-NEXT:         let %34 ug: ptr<@type1> [storage=automatic];
// DEFAULT-NEXT:         let %35 vf: ptr<@type1> [storage=automatic];
// DEFAULT-NEXT:         let %36 vg: ptr<@type1> [storage=automatic];
// DEFAULT-NEXT:         let %37 q: u32 [storage=automatic];
// DEFAULT-NEXT:         let %38 lcu: u32 [storage=automatic];
// DEFAULT-NEXT:         let %39 lcvrecip: u32 [storage=automatic];
// DEFAULT-NEXT:         let %40 p: u32 [storage=automatic];
// DEFAULT-NEXT:         let %41 df: i32 [storage=automatic];
// DEFAULT-NEXT:         let %42 dg: i32 [storage=automatic];
// DEFAULT-NEXT:         let %43 du: i32 [storage=automatic];
// DEFAULT-NEXT:         let %44 dv: i32 [storage=automatic];
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%55, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(40)>(%56)), call<i32, signature=fn(ptr<@type1>) -> i32>(%9, read<ptr<@type1>>(%29)), call<i32, signature=fn(ptr<@type1>) -> i32>(%9, read<ptr<@type1>>(%30)));
// DEFAULT-NEXT:         if lt<i32>(call<i32, signature=fn(ptr<@type1>) -> i32>(%9, read<ptr<@type1>>(%29)), call<i32, signature=fn(ptr<@type1>) -> i32>(%9, read<ptr<@type1>>(%30)))
// DEFAULT-NEXT:             return call<ptr<@type1>, signature=fn(ptr<ptr<@type1>>, ptr<ptr<@type1>>, ptr<@type1>, ptr<@type1>) -> ptr<@type1>>(%26, read<ptr<ptr<@type1>>>(%28), read<ptr<ptr<@type1>>>(%27), read<ptr<@type1>>(%30), read<ptr<@type1>>(%29));
// DEFAULT-NEXT:         let %60: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type1>) -> i32>(%9, read<ptr<@type1>>(%29)), const<i32>(2))
// DEFAULT-NEXT:             write<bool>(%60, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%60, ne<i32>(call<i32, signature=fn(ptr<@type1>) -> i32>(%9, read<ptr<@type1>>(%30)), const<i32>(1)));
// DEFAULT-NEXT:         if read<bool>(%60)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if eq<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(field2(deref(read<ptr<@type1>>(%29)))), const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:             return read<ptr<@type1>>(%29);
// DEFAULT-NEXT:         write<u32>(%40, reinterpret<u32, reason=assign, fits=always>(const<i32>(2)));
// DEFAULT-NEXT:         write<i32>(%41, call<i32, signature=fn(ptr<@type1>) -> i32>(%9, read<ptr<@type1>>(%29)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type1>) -> i32>(%9, read<ptr<@type1>>(%29));
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%41), const<i32>(0))
// DEFAULT-NEXT:             write<i32>(%41, const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%42, call<i32, signature=fn(ptr<@type1>) -> i32>(%9, read<ptr<@type1>>(%30)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type1>) -> i32>(%9, read<ptr<@type1>>(%30));
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%42), const<i32>(0))
// DEFAULT-NEXT:             write<i32>(%42, const<i32>(0));
// DEFAULT-NEXT:         write<ptr<@type1>>(%31, call<ptr<@type1>, signature=fn(ptr<@type1>) -> ptr<@type1>>(%19, read<ptr<@type1>>(%29)));
// DEFAULT-NEXT:         call<ptr<@type1>, signature=fn(ptr<@type1>) -> ptr<@type1>>(%19, read<ptr<@type1>>(%29));
// DEFAULT-NEXT:         write<ptr<@type1>>(%32, call<ptr<@type1>, signature=fn(ptr<@type1>) -> ptr<@type1>>(%19, read<ptr<@type1>>(%30)));
// DEFAULT-NEXT:         call<ptr<@type1>, signature=fn(ptr<@type1>) -> ptr<@type1>>(%19, read<ptr<@type1>>(%30));
// DEFAULT-NEXT:         write<ptr<@type1>>(%33, call<ptr<@type1>, signature=fn(i32) -> ptr<@type1>>(%11, read<i32>(%42)));
// DEFAULT-NEXT:         call<ptr<@type1>, signature=fn(i32) -> ptr<@type1>>(%11, read<i32>(%42));
// DEFAULT-NEXT:         write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(field2(deref(read<ptr<@type1>>(%33)))), const<i32>(0))), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<i32>(field1(deref(read<ptr<@type1>>(%33))), const<i32>(0));
// DEFAULT-NEXT:         write<ptr<@type1>>(%34, call<ptr<@type1>, signature=fn(i32) -> ptr<@type1>>(%11, read<i32>(%41)));
// DEFAULT-NEXT:         call<ptr<@type1>, signature=fn(i32) -> ptr<@type1>>(%11, read<i32>(%41));
// DEFAULT-NEXT:         write<ptr<@type1>>(%35, call<ptr<@type1>, signature=fn(i32) -> ptr<@type1>>(%11, read<i32>(%42)));
// DEFAULT-NEXT:         call<ptr<@type1>, signature=fn(i32) -> ptr<@type1>>(%11, read<i32>(%42));
// DEFAULT-NEXT:         write<ptr<@type1>>(%36, call<ptr<@type1>, signature=fn(i32) -> ptr<@type1>>(%11, read<i32>(%41)));
// DEFAULT-NEXT:         call<ptr<@type1>, signature=fn(i32) -> ptr<@type1>>(%11, read<i32>(%41));
// DEFAULT-NEXT:         write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(field2(deref(read<ptr<@type1>>(%36)))), const<i32>(0))), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<i32>(field1(deref(read<ptr<@type1>>(%36))), const<i32>(0));
// DEFAULT-NEXT:         while %57 gt<i32>(call<i32, signature=fn(ptr<@type1>) -> i32>(%9, read<ptr<@type1>>(%32)), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i32>(%44, call<i32, signature=fn(ptr<@type1>) -> i32>(%9, read<ptr<@type1>>(%32)));
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<@type1>) -> i32>(%9, read<ptr<@type1>>(%32));
// DEFAULT-NEXT:                 write<u32>(%39, call<u32, signature=fn(u32, u32) -> u32>(%4, reinterpret<u32, reason=arg, fits=always>(const<i32>(1)), read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(field2(deref(read<ptr<@type1>>(%32)))), read<i32>(%44))))));
// DEFAULT-NEXT:                 call<u32, signature=fn(u32, u32) -> u32>(%4, reinterpret<u32, reason=arg, fits=always>(const<i32>(1)), read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(field2(deref(read<ptr<@type1>>(%32)))), read<i32>(%44)))));
// DEFAULT-NEXT:                 while %58 ge<i32>(call<i32, signature=fn(ptr<@type1>) -> i32>(%9, read<ptr<@type1>>(%31)), read<i32>(%44))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         write<i32>(%43, call<i32, signature=fn(ptr<@type1>) -> i32>(%9, read<ptr<@type1>>(%31)));
// DEFAULT-NEXT:                         call<i32, signature=fn(ptr<@type1>) -> i32>(%9, read<ptr<@type1>>(%31));
// DEFAULT-NEXT:                         write<u32>(%38, read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(field2(deref(read<ptr<@type1>>(%31)))), read<i32>(%43)))));
// DEFAULT-NEXT:                         write<u32>(%37, call<u32, signature=fn(u32, u32) -> u32>(%4, read<u32>(%38), read<u32>(%39)));
// DEFAULT-NEXT:                         call<u32, signature=fn(u32, u32) -> u32>(%4, read<u32>(%38), read<u32>(%39));
// DEFAULT-NEXT:                         call<void, signature=fn(ptr<@type1>, ptr<@type1>, i32, u32) -> void>(%21, read<ptr<@type1>>(%31), read<ptr<@type1>>(%32), sub<i32, overflow=ub>(read<i32>(%43), read<i32>(%44)), sub<u32, overflow=wrap>(read<u32>(%40), read<u32>(%37)));
// DEFAULT-NEXT:                         call<void, signature=fn(ptr<@type1>, ptr<@type1>, i32, u32) -> void>(%21, read<ptr<@type1>>(%33), read<ptr<@type1>>(%35), sub<i32, overflow=ub>(read<i32>(%43), read<i32>(%44)), sub<u32, overflow=wrap>(read<u32>(%40), read<u32>(%37)));
// DEFAULT-NEXT:                         call<void, signature=fn(ptr<@type1>, ptr<@type1>, i32, u32) -> void>(%21, read<ptr<@type1>>(%34), read<ptr<@type1>>(%36), sub<i32, overflow=ub>(read<i32>(%43), read<i32>(%44)), sub<u32, overflow=wrap>(read<u32>(%40), read<u32>(%37)));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<@type1>, ptr<@type1>) -> void>(%16, read<ptr<@type1>>(%31), read<ptr<@type1>>(%32));
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<@type1>, ptr<@type1>) -> void>(%16, read<ptr<@type1>>(%33), read<ptr<@type1>>(%35));
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<@type1>, ptr<@type1>) -> void>(%16, read<ptr<@type1>>(%34), read<ptr<@type1>>(%36));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if eq<i32>(call<i32, signature=fn(ptr<@type1>) -> i32>(%9, read<ptr<@type1>>(%32)), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<@type1>, ptr<@type1>) -> void>(%16, read<ptr<@type1>>(%31), read<ptr<@type1>>(%32));
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<@type1>, ptr<@type1>) -> void>(%16, read<ptr<@type1>>(%33), read<ptr<@type1>>(%35));
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<@type1>, ptr<@type1>) -> void>(%16, read<ptr<@type1>>(%34), read<ptr<@type1>>(%36));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type1>) -> void>(%14, read<ptr<@type1>>(%35));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type1>) -> void>(%14, read<ptr<@type1>>(%36));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type1>) -> void>(%14, read<ptr<@type1>>(%32));
// DEFAULT-NEXT:         write<ptr<@type1>>(deref(read<ptr<ptr<@type1>>>(%27)), read<ptr<@type1>>(%33));
// DEFAULT-NEXT:         write<ptr<@type1>>(deref(read<ptr<ptr<@type1>>>(%28)), read<ptr<@type1>>(%34));
// DEFAULT-NEXT:         return read<ptr<@type1>>(%31);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %45 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %46 f: ptr<@type1> [storage=automatic];
// DEFAULT-NEXT:         let %47 g: ptr<@type1> [storage=automatic];
// DEFAULT-NEXT:         let %48 cf: ptr<@type1> [storage=automatic];
// DEFAULT-NEXT:         let %49 cg: ptr<@type1> [storage=automatic];
// DEFAULT-NEXT:         let %50 h: ptr<@type1> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<@type1>>(%46, call<ptr<@type1>, signature=fn(i32) -> ptr<@type1>>(%11, const<i32>(1)));
// DEFAULT-NEXT:         call<ptr<@type1>, signature=fn(i32) -> ptr<@type1>>(%11, const<i32>(1));
// DEFAULT-NEXT:         write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(field2(deref(read<ptr<@type1>>(%46)))), const<i32>(1))), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<i32>(field1(deref(read<ptr<@type1>>(%46))), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<@type1>>(%47, call<ptr<@type1>, signature=fn(i32) -> ptr<@type1>>(%11, const<i32>(2)));
// DEFAULT-NEXT:         call<ptr<@type1>, signature=fn(i32) -> ptr<@type1>>(%11, const<i32>(2));
// DEFAULT-NEXT:         write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(field2(deref(read<ptr<@type1>>(%47)))), const<i32>(2))), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<i32>(field1(deref(read<ptr<@type1>>(%47))), const<i32>(2));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%55, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(41)>(%59)), call<i32, signature=fn(ptr<@type1>) -> i32>(%9, read<ptr<@type1>>(%46)), call<i32, signature=fn(ptr<@type1>) -> i32>(%9, read<ptr<@type1>>(%47)));
// DEFAULT-NEXT:         write<ptr<@type1>>(%50, call<ptr<@type1>, signature=fn(ptr<ptr<@type1>>, ptr<ptr<@type1>>, ptr<@type1>, ptr<@type1>) -> ptr<@type1>>(%26, addr_of<ptr<ptr<@type1>>>(%48), addr_of<ptr<ptr<@type1>>>(%49), read<ptr<@type1>>(%46), read<ptr<@type1>>(%47)));
// DEFAULT-NEXT:         call<ptr<@type1>, signature=fn(ptr<ptr<@type1>>, ptr<ptr<@type1>>, ptr<@type1>, ptr<@type1>) -> ptr<@type1>>(%26, addr_of<ptr<ptr<@type1>>>(%48), addr_of<ptr<ptr<@type1>>>(%49), read<ptr<@type1>>(%46), read<ptr<@type1>>(%47));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
