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
// DEFAULT-NEXT:     type @type[[TYPE_vc_data:[0-9]+]] vc_data = struct {
// DEFAULT-NEXT:         field0 space: u64;
// DEFAULT-NEXT:         field1 vc_palette: array<u8, 48>;
// DEFAULT-NEXT:     } [size=56, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_vc:[0-9]+]] vc = struct {
// DEFAULT-NEXT:         field0 d: ptr<@type[[TYPE_vc_data]]>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_a_con:[0-9]+]] a_con: @type[[TYPE_vc_data]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_vc_cons:[0-9]+]] vc_cons: array<@type[[TYPE_vc]], 63> [storage=static] [align=16] = aggregate<array<@type[[TYPE_vc]], 63>, zero_fill=true>(index0 = aggregate<@type[[TYPE_vc]], zero_fill=false>(field0 = addr_of<ptr<@type[[TYPE_vc_data]]>>(%[[VALUE_a_con]]))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_default_red:[0-9]+]] default_red: array<i32, 16> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_default_grn:[0-9]+]] default_grn: array<i32, 16> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_default_blu:[0-9]+]] default_blu: array<i32, 16> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_k:[0-9]+]] k: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_k]]), mul<i32, overflow=ub>(const<i32>(16), const<i32>(3)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_reset_palette:[0-9]+]] @reset_palette(%[[VALUE_currcons:[0-9]+]] currcons: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_k_2:[0-9]+]] k: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_k_2]], const<i32>(0));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j]]), const<i32>(16))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j]]);
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_k_2]]);
// DEFAULT-NEXT:                     let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_k_2]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(48)>(field1(deref(read<ptr<@type[[TYPE_vc_data]]>>(field0(deref(ptr_offset<ptr<@type[[TYPE_vc]]>, subtract=false, element=@type[[TYPE_vc]], overflow=ub>(array_decay<ptr<@type[[TYPE_vc]]>, length=Some(63)>(%[[VALUE_vc_cons]]), read<i32>(%[[VALUE_currcons]])))))))), read<i32>(%[[VALUE4]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(16)>(%[[VALUE_default_red]]), read<i32>(%[[VALUE_j]])))))));
// DEFAULT-NEXT:                     let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_k_2]]);
// DEFAULT-NEXT:                     let %[[VALUE7:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE6]]), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_k_2]], read<i32>(%[[VALUE7]]));
// DEFAULT-NEXT:                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(48)>(field1(deref(read<ptr<@type[[TYPE_vc_data]]>>(field0(deref(ptr_offset<ptr<@type[[TYPE_vc]]>, subtract=false, element=@type[[TYPE_vc]], overflow=ub>(array_decay<ptr<@type[[TYPE_vc]]>, length=Some(63)>(%[[VALUE_vc_cons]]), read<i32>(%[[VALUE_currcons]])))))))), read<i32>(%[[VALUE6]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(16)>(%[[VALUE_default_grn]]), read<i32>(%[[VALUE_j]])))))));
// DEFAULT-NEXT:                     let %[[VALUE8:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_k_2]]);
// DEFAULT-NEXT:                     let %[[VALUE9:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE8]]), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_k_2]], read<i32>(%[[VALUE9]]));
// DEFAULT-NEXT:                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(48)>(field1(deref(read<ptr<@type[[TYPE_vc_data]]>>(field0(deref(ptr_offset<ptr<@type[[TYPE_vc]]>, subtract=false, element=@type[[TYPE_vc]], overflow=ub>(array_decay<ptr<@type[[TYPE_vc]]>, length=Some(63)>(%[[VALUE_vc_cons]]), read<i32>(%[[VALUE_currcons]])))))))), read<i32>(%[[VALUE8]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(16)>(%[[VALUE_default_blu]]), read<i32>(%[[VALUE_j]])))))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_bar]], read<i32>(%[[VALUE_k_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_reset_palette]], const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
