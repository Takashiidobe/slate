void abort(void);
void exit(int);

struct vc_data {
  unsigned long space;
  unsigned char vc_palette[16 * 3];
};

struct vc {
  struct vc_data *d;
};

struct vc_data a_con;
struct vc      vc_cons[63] = {&a_con};
int            default_red[16];
int            default_grn[16];
int            default_blu[16];

extern void bar(int);

void reset_palette(int currcons) {
  int j, k;
  for (j = k = 0; j < 16; j++) {
    (vc_cons[currcons].d->vc_palette)[k++] = default_red[j];
    (vc_cons[currcons].d->vc_palette)[k++] = default_grn[j];
    (vc_cons[currcons].d->vc_palette)[k++] = default_blu[j];
  }
  bar(k);
}

void bar(int k) {
  if (k != 16 * 3)
    abort();
}

int main() {
  reset_palette(0);
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
// DEFAULT-NEXT:     type @type0 vc_data = struct {
// DEFAULT-NEXT:         field0 space: u64;
// DEFAULT-NEXT:         field1 vc_palette: array<u8, 48>;
// DEFAULT-NEXT:     } [size=56, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type1 vc = struct {
// DEFAULT-NEXT:         field0 d: ptr<@type0>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     global %4 a_con: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 vc_cons: array<@type1, 63> [storage=static] [align=16] = aggregate<array<@type1, 63>, zero_fill=true>(index0 = aggregate<@type1, zero_fill=false>(field0 = addr_of<ptr<@type0>>(%4))) [linkage=external];
// DEFAULT-NEXT:     global %6 default_red: array<i32, 16> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %7 default_grn: array<i32, 16> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %8 default_blu: array<i32, 16> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%16 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %9 @bar(%14 k: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%14), mul<i32, overflow=ub>(const<i32>(16), const<i32>(3)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @reset_palette(%11 currcons: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %12 j: i32 [storage=automatic];
// DEFAULT-NEXT:         let %13 k: i32 [storage=automatic];
// DEFAULT-NEXT:         for %18
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%13, const<i32>(0));
// DEFAULT-NEXT:                 write<i32>(%12, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%12), const<i32>(16))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %19: i32 [synthetic] = read<i32>(%12);
// DEFAULT-NEXT:                 let %20: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%19), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%12, read<i32>(%20));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %21: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:                     let %22: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%21), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%13, read<i32>(%22));
// DEFAULT-NEXT:                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(48)>(field1(deref(read<ptr<@type0>>(field0(deref(ptr_offset<ptr<@type1>, subtract=false, element=@type1, overflow=ub>(array_decay<ptr<@type1>, length=Some(63)>(%5), read<i32>(%11)))))))), read<i32>(%21))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(16)>(%6), read<i32>(%12)))))));
// DEFAULT-NEXT:                     let %23: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:                     let %24: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%23), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%13, read<i32>(%24));
// DEFAULT-NEXT:                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(48)>(field1(deref(read<ptr<@type0>>(field0(deref(ptr_offset<ptr<@type1>, subtract=false, element=@type1, overflow=ub>(array_decay<ptr<@type1>, length=Some(63)>(%5), read<i32>(%11)))))))), read<i32>(%23))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(16)>(%7), read<i32>(%12)))))));
// DEFAULT-NEXT:                     let %25: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:                     let %26: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%25), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%13, read<i32>(%26));
// DEFAULT-NEXT:                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(48)>(field1(deref(read<ptr<@type0>>(field0(deref(ptr_offset<ptr<@type1>, subtract=false, element=@type1, overflow=ub>(array_decay<ptr<@type1>, length=Some(63)>(%5), read<i32>(%11)))))))), read<i32>(%25))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(16)>(%8), read<i32>(%12)))))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%9, read<i32>(%13));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%10, const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
