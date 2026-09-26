void abort(void);
void exit(int);

struct F {
  int i;
};

void f1(struct F *x, struct F *y) {
  int timeout = 0;
  for (; ((const struct F *)x)->i < y->i; x->i++)
    if (++timeout > 5)
      abort();
}

int main(void) {
  struct F x, y;
  x.i = 0;
  y.i = 1;
  f1(&x, &y);
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
// DEFAULT-NEXT:     type @type0 F = struct {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%10 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @f1(%4 x: ptr<@type0>, %5 y: ptr<@type0>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %6 timeout: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %11
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(field0(deref(pointer_cast<ptr<const @type0>, reason=explicit>(read<ptr<@type0>>(%4))))), read<i32>(field0(deref(read<ptr<@type0>>(%5)))))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %12: ptr<@type0> [synthetic] = read<ptr<@type0>>(%4);
// DEFAULT-NEXT:                 let %13: i32 [synthetic] = read<i32>(field0(deref(read<ptr<@type0>>(%12))));
// DEFAULT-NEXT:                 let %14: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%13), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(field0(deref(read<ptr<@type0>>(%12))), read<i32>(%14));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %15: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:                 let %16: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%15), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%6, read<i32>(%16));
// DEFAULT-NEXT:                 if gt<i32>(read<i32>(%16), const<i32>(5))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %8 x: @type0 [storage=automatic];
// DEFAULT-NEXT:         let %9 y: @type0 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(field0(%8), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(field0(%9), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>, ptr<@type0>) -> void>(%3, addr_of<ptr<@type0>>(%8), addr_of<ptr<@type0>>(%9));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
