/* PR middle-end/90348 */

void __attribute__((noipa)) set_one(unsigned char *ptr) { *ptr = 1; }

void __attribute__((noipa)) check_zero(unsigned char const *in,
                                       unsigned int         len) {
  for (unsigned int i = 0; i < len; ++i)
    if (in[i] != 0)
      __builtin_abort();
}

static void set_one_on_stack(void) {
  unsigned char buf[1];
  set_one(buf);
}

int main() {
  for (int i = 0; i <= 4; ++i) {
    unsigned char in[4];
    for (int j = 0; j < i; ++j) {
      in[j] = 0;
      set_one_on_stack();
    }
    check_zero(in, i);
  }
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
// DEFAULT-NEXT:     fn %[[VALUE_set_one:[0-9]+]] @set_one(%[[VALUE_ptr:[0-9]+]] ptr: ptr<u8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<u8>(deref(read<ptr<u8>>(%[[VALUE_ptr]])), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_check_zero:[0-9]+]] @check_zero(%[[VALUE_in:[0-9]+]] in: ptr<const u8>, %[[VALUE_len:[0-9]+]] len: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i:[0-9]+]] i: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u32>(read<u32>(%[[VALUE_i]]), read<u32>(%[[VALUE_len]]))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE1]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<u32>(%[[VALUE_i]], read<u32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<const u8>, subtract=false, element=u8, overflow=ub>(read<ptr<const u8>>(%[[VALUE_in]]), read<u32>(%[[VALUE_i]])))))), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_set_one_on_stack:[0-9]+]] @set_one_on_stack() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_buf:[0-9]+]] buf: array<u8, 1> [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<u8>) -> void>(%[[VALUE_set_one]], array_decay<ptr<u8>, length=Some(1)>(%[[VALUE_buf]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         for %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i_2:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_in_2:[0-9]+]] in: array<u8, 4> [storage=automatic];
// DEFAULT-NEXT:                     for %[[VALUE6:[0-9]+]]
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                         condition: lt<i32>(read<i32>(%[[VALUE_j]]), read<i32>(%[[VALUE_i_2]]))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %[[VALUE7:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j]]);
// DEFAULT-NEXT:                             let %[[VALUE8:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE7]]), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_j]], read<i32>(%[[VALUE8]]));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(4)>(%[[VALUE_in_2]]), read<i32>(%[[VALUE_j]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE_set_one_on_stack]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     call<void, signature=fn(ptr<const u8>, u32) -> void>(%[[VALUE_check_zero]], pointer_cast<ptr<const u8>, reason=arg>(array_decay<ptr<u8>, length=Some(4)>(%[[VALUE_in_2]])), reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_i_2]])));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
