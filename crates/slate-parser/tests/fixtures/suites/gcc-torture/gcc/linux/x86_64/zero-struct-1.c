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
// DEFAULT-NEXT:     type @type[[TYPE_g:[0-9]+]] g = struct {
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[]];
// DEFAULT-NEXT:     global %[[VALUE_y:[0-9]+]] y: array<i8, 3> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: ptr<i8> [storage=static] = addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_y]]), const<i32>(0)))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ff:[0-9]+]] ff: ptr<i8> [storage=static] = addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_y]]), const<i32>(0)))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_h:[0-9]+]] @h() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_t:[0-9]+]] t: @type[[TYPE_g]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_ff]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE0]]), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_ff]], read<ptr<i8>>(%[[VALUE1]]));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_f]]);
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_f]], read<ptr<i8>>(%[[VALUE3]]));
// DEFAULT-NEXT:         write<@type[[TYPE_g]]>(deref(pointer_cast<ptr<@type[[TYPE_g]]>, reason=explicit>(read<ptr<i8>>(%[[VALUE2]]))), copy<@type[[TYPE_g]], reason=assign>(read<@type[[TYPE_g]]>(deref(pointer_cast<ptr<@type[[TYPE_g]]>, reason=explicit>(read<ptr<i8>>(%[[VALUE0]]))))));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_f]]);
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_f]], read<ptr<i8>>(%[[VALUE5]]));
// DEFAULT-NEXT:         write<@type[[TYPE_g]]>(deref(pointer_cast<ptr<@type[[TYPE_g]]>, reason=explicit>(read<ptr<i8>>(%[[VALUE4]]))), copy<@type[[TYPE_g]], reason=assign>(read<@type[[TYPE_g]]>(compound_literal %[[VALUE6:[0-9]+]] [storage=automatic] = aggregate<@type[[TYPE_g]], zero_fill=false>())));
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_ff]]);
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE7]]), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_ff]], read<ptr<i8>>(%[[VALUE8]]));
// DEFAULT-NEXT:         write<@type[[TYPE_g]]>(%[[VALUE_t]], copy<@type[[TYPE_g]], reason=assign>(read<@type[[TYPE_g]]>(deref(pointer_cast<ptr<@type[[TYPE_g]]>, reason=explicit>(read<ptr<i8>>(%[[VALUE7]]))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_h]]);
// DEFAULT-NEXT:         if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_f]]), addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_y]]), const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_ff]]), addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_y]]), const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
