void abort(void);
void exit(int);

struct rtx_def {
  int code;
};

int main(void) {
  int             tmp[2];
  struct rtx_def *r, s;
  int            *p, *q;

  /* The alias analyzer was creating the same memory tag for r, p and q
     because 'struct rtx_def *' is type-compatible with 'int *'.  However,
     the alias set of 'int[2]' is not the same as 'int *', so variable
     'tmp' was deemed not aliased with anything.  */
  r       = &s;
  r->code = 39;

  /* If 'r' wasn't declared, then q and tmp would have had the same memory
     tag.  */
  p      = tmp;
  q      = p + 1;
  *q     = 0;
  tmp[1] = 39;
  if (*q != 39)
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
// DEFAULT-NEXT:     type @type0 rtx_def = struct {
// DEFAULT-NEXT:         field0 code: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%9 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %4 tmp: array<i32, 2> [storage=automatic];
// DEFAULT-NEXT:         let %5 r: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:         let %6 s: @type0 [storage=automatic];
// DEFAULT-NEXT:         let %7 p: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         let %8 q: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<@type0>>(%5, addr_of<ptr<@type0>>(%6));
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type0>>(%5))), const<i32>(39));
// DEFAULT-NEXT:         write<ptr<i32>>(%7, array_decay<ptr<i32>, length=Some(2)>(%4));
// DEFAULT-NEXT:         write<ptr<i32>>(%8, ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%7), const<i32>(1)));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%8)), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%4), const<i32>(1))), const<i32>(39));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(deref(read<ptr<i32>>(%8))), const<i32>(39))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
