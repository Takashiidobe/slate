extern void abort(void);

typedef signed short   int16_t;
typedef unsigned short uint16_t;

int16_t logadd(int16_t *a, int16_t *b);
void    ba_compute_psd(int16_t start);

int16_t masktab[6] = {1, 2, 3, 4, 5};
int16_t psd[6]     = {50, 40, 30, 20, 10};
int16_t bndpsd[6]  = {1, 2, 3, 4, 5};

void ba_compute_psd(int16_t start) {
  int     i, j, k;
  int16_t lastbin = 4;

  j = start;
  k = masktab[start];

  bndpsd[k] = psd[j];
  j++;

  for (i = j; i < lastbin; i++) {
    bndpsd[k] = logadd(&bndpsd[k], &psd[j]);
    j++;
  }
}

int16_t logadd(int16_t *a, int16_t *b) { return *a + *b; }

int main(void) {
  int i;

  ba_compute_psd(0);

  if (bndpsd[1] != 140)
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
// DEFAULT-NEXT:     type @type0 int16_t = i16;
// DEFAULT-NEXT:     type @type1 uint16_t = u16;
// DEFAULT-NEXT:     global %8 masktab: array<i16, 6> [storage=static] = aggregate<array<i16, 6>, zero_fill=true>(index0 = truncate<i16, reason=assign, fits=always>(const<i32>(1)), index1 = truncate<i16, reason=assign, fits=always>(const<i32>(2)), index2 = truncate<i16, reason=assign, fits=always>(const<i32>(3)), index3 = truncate<i16, reason=assign, fits=always>(const<i32>(4)), index4 = truncate<i16, reason=assign, fits=always>(const<i32>(5))) [linkage=external];
// DEFAULT-NEXT:     global %9 psd: array<i16, 6> [storage=static] = aggregate<array<i16, 6>, zero_fill=true>(index0 = truncate<i16, reason=assign, fits=always>(const<i32>(50)), index1 = truncate<i16, reason=assign, fits=always>(const<i32>(40)), index2 = truncate<i16, reason=assign, fits=always>(const<i32>(30)), index3 = truncate<i16, reason=assign, fits=always>(const<i32>(20)), index4 = truncate<i16, reason=assign, fits=always>(const<i32>(10))) [linkage=external];
// DEFAULT-NEXT:     global %10 bndpsd: array<i16, 6> [storage=static] = aggregate<array<i16, 6>, zero_fill=true>(index0 = truncate<i16, reason=assign, fits=always>(const<i32>(1)), index1 = truncate<i16, reason=assign, fits=always>(const<i32>(2)), index2 = truncate<i16, reason=assign, fits=always>(const<i32>(3)), index3 = truncate<i16, reason=assign, fits=always>(const<i32>(4)), index4 = truncate<i16, reason=assign, fits=always>(const<i32>(5))) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %5 @logadd(%16 a: ptr<i16>, %17 b: ptr<i16>) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(deref(read<ptr<i16>>(%16)))), widen<i32, reason=promotion>(read<i16>(deref(read<ptr<i16>>(%17))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @ba_compute_psd(%11 start: i16) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %12 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %13 j: i32 [storage=automatic];
// DEFAULT-NEXT:         let %14 k: i32 [storage=automatic];
// DEFAULT-NEXT:         let %15 lastbin: i16 [storage=automatic] = truncate<i16, reason=assign, fits=always>(const<i32>(4));
// DEFAULT-NEXT:         write<i32>(%13, widen<i32, reason=assign>(read<i16>(%11)));
// DEFAULT-NEXT:         write<i32>(%14, widen<i32, reason=assign>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(6)>(%8), widen<i32, reason=promotion>(read<i16>(%11)))))));
// DEFAULT-NEXT:         write<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(6)>(%10), read<i32>(%14))), read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(6)>(%9), read<i32>(%13)))));
// DEFAULT-NEXT:         let %24: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:         let %25: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%24), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%25));
// DEFAULT-NEXT:         for %23
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%12, read<i32>(%13));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%12), widen<i32, reason=promotion>(read<i16>(%15)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %26: i32 [synthetic] = read<i32>(%12);
// DEFAULT-NEXT:                 let %27: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%26), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%12, read<i32>(%27));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(6)>(%10), read<i32>(%14))), call<i16, signature=fn(ptr<i16>, ptr<i16>) -> i16>(%5, addr_of<ptr<i16>>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(6)>(%10), read<i32>(%14)))), addr_of<ptr<i16>>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(6)>(%9), read<i32>(%13))))));
// DEFAULT-NEXT:                     call<i16, signature=fn(ptr<i16>, ptr<i16>) -> i16>(%5, addr_of<ptr<i16>>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(6)>(%10), read<i32>(%14)))), addr_of<ptr<i16>>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(6)>(%9), read<i32>(%13)))));
// DEFAULT-NEXT:                     let %28: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:                     let %29: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%28), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%13, read<i32>(%29));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %19 i: i32 [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(i16) -> void>(%7, truncate<i16, reason=arg, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(6)>(%10), const<i32>(1))))), const<i32>(140))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
