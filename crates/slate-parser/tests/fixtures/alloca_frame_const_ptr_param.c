typedef struct {
  int windowLog;
} Params;

static void use_params(const Params *params, unsigned *out, const char *lo,
                       const char *hi) {
  long diff = hi - lo;
  *out      = (unsigned)(params->windowLog + (int)diff);
}

int main(void) {
  Params p;
  p.windowLog = 5;
  char     buf[8];
  unsigned out = 0;
  use_params(&p, &out, buf, buf + 4);
  return (int)out;
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
// DEFAULT-NEXT:     type @type0 = struct {
// DEFAULT-NEXT:         field0 windowLog: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type1 Params = @type0;
// DEFAULT-NEXT:     fn %2 @use_params(%3 params: ptr<const @type0>, %4 out: ptr<u32>, %5 lo: ptr<const i8>, %6 hi: ptr<const i8>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %7 diff: i64 [storage=automatic] = ptr_diff<i64, element=i8, same_array=required, overflow=ub>(read<ptr<const i8>>(%6), read<ptr<const i8>>(%5));
// DEFAULT-NEXT:         write<u32>(deref(read<ptr<u32>>(%4)), reinterpret<u32, reason=explicit, fits=unknown>(add<i32, overflow=ub>(read<i32>(field0(deref(read<ptr<const @type0>>(%3)))), truncate<i32, reason=explicit, fits=unknown>(read<i64>(%7)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %9 p: @type0 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(field0(%9), const<i32>(5));
// DEFAULT-NEXT:         let %10 buf: array<i8, 8> [storage=automatic];
// DEFAULT-NEXT:         let %11 out: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const @type0>, ptr<u32>, ptr<const i8>, ptr<const i8>) -> void>(%2, pointer_cast<ptr<const @type0>, reason=arg>(addr_of<ptr<@type0>>(%9)), addr_of<ptr<u32>>(%11), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%10)), pointer_cast<ptr<const i8>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(8)>(%10), const<i32>(4))));
// DEFAULT-NEXT:         return reinterpret<i32, reason=explicit, fits=unknown>(read<u32>(%11));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
