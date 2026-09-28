struct g {};
char  y[3];
char *f  = &y[0];
char *ff = &y[0];
void  h(void) {
  struct g t;
  *((struct g *)(f++)) = *((struct g *)(ff++));
  *((struct g *)(f++)) = (struct g){};
  t                    = *((struct g *)(ff++));
}

void abort(void);

int main(void) {
  h();
  if (f != &y[2])
    abort();
  if (ff != &y[2])
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
// DEFAULT-NEXT:     type @type0 g = struct {
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[]];
// DEFAULT-NEXT:     global %1 y: array<i8, 3> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 f: ptr<i8> [storage=static] = addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(3)>(%1), const<i32>(0)))) [linkage=external];
// DEFAULT-NEXT:     global %3 ff: ptr<i8> [storage=static] = addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(3)>(%1), const<i32>(0)))) [linkage=external];
// DEFAULT-NEXT:     fn %4 @h() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %5 t: @type0 [storage=automatic];
// DEFAULT-NEXT:         let %9: ptr<i8> [synthetic] = read<ptr<i8>>(%3);
// DEFAULT-NEXT:         let %10: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%9), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<i8>>(%3, read<ptr<i8>>(%10));
// DEFAULT-NEXT:         let %11: ptr<i8> [synthetic] = read<ptr<i8>>(%2);
// DEFAULT-NEXT:         let %12: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%11), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<i8>>(%2, read<ptr<i8>>(%12));
// DEFAULT-NEXT:         write<@type0>(deref(pointer_cast<ptr<@type0>, reason=explicit>(read<ptr<i8>>(%11))), copy<@type0, reason=assign>(read<@type0>(deref(pointer_cast<ptr<@type0>, reason=explicit>(read<ptr<i8>>(%9))))));
// DEFAULT-NEXT:         let %13: ptr<i8> [synthetic] = read<ptr<i8>>(%2);
// DEFAULT-NEXT:         let %14: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%13), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<i8>>(%2, read<ptr<i8>>(%14));
// DEFAULT-NEXT:         write<@type0>(deref(pointer_cast<ptr<@type0>, reason=explicit>(read<ptr<i8>>(%13))), copy<@type0, reason=assign>(read<@type0>(compound_literal %8 [storage=automatic] = aggregate<@type0, zero_fill=false>())));
// DEFAULT-NEXT:         let %15: ptr<i8> [synthetic] = read<ptr<i8>>(%3);
// DEFAULT-NEXT:         let %16: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%15), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<i8>>(%3, read<ptr<i8>>(%16));
// DEFAULT-NEXT:         write<@type0>(%5, copy<@type0, reason=assign>(read<@type0>(deref(pointer_cast<ptr<@type0>, reason=explicit>(read<ptr<i8>>(%15))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if ne<ptr<i8>>(read<ptr<i8>>(%2), addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(3)>(%1), const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<ptr<i8>>(read<ptr<i8>>(%3), addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(3)>(%1), const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
