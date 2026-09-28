extern void  abort(void);
typedef long GLint;
void         aglChoosePixelFormat(const GLint *);

void find(const int *alistp) {
  const int *blist;
  int        list[32];
  if (alistp)
    blist = alistp;
  else {
    list[3] = 42;
    blist   = list;
  }
  aglChoosePixelFormat((GLint *)blist);
}

void aglChoosePixelFormat(const GLint *a) {
  int *b = (int *)a;
  if (b[3] != 42)
    abort();
}

int main(void) {
  find(0);
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
// DEFAULT-NEXT:     type @type0 GLint = i64;
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @aglChoosePixelFormat(%7 a: ptr<const i64>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %8 b: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=explicit>(read<ptr<const i64>>(%7));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%8), const<i32>(3)))), const<i32>(42))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @find(%4 alistp: ptr<const i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %5 blist: ptr<const i32> [storage=automatic];
// DEFAULT-NEXT:         let %6 list: array<i32, 32> [storage=automatic] [align=16];
// DEFAULT-NEXT:         if ne<ptr<const i32>>(read<ptr<const i32>>(%4), null<ptr<const i32>>)
// DEFAULT-NEXT:             write<ptr<const i32>>(%5, read<ptr<const i32>>(%4));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(32)>(%6), const<i32>(3))), const<i32>(42));
// DEFAULT-NEXT:                 write<ptr<const i32>>(%5, pointer_cast<ptr<const i32>, reason=assign>(array_decay<ptr<i32>, length=Some(32)>(%6)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i64>) -> void>(%2, pointer_cast<ptr<const i64>, reason=arg>(pointer_cast<ptr<i64>, reason=explicit>(read<ptr<const i32>>(%5))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i32>) -> void>(%3, null<ptr<const i32>>);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
