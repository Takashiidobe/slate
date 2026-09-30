/* PR45070 */
extern void abort(void);

struct packed_ushort {
  unsigned short ucs;
} __attribute__((packed));

struct source {
  int pos, length;
};

static int flag;

static void __attribute__((noinline)) fetch(struct source *p) {
  p->length = 128;
}

static struct packed_ushort __attribute__((noinline)) next(struct source *p) {
  struct packed_ushort rv;

  if (p->pos >= p->length) {
    if (flag) {
      flag = 0;
      fetch(p);
      return next(p);
    }
    flag   = 1;
    rv.ucs = 0xffff;
    return rv;
  }
  rv.ucs = 0;
  return rv;
}

int main(void) {
  struct source s;
  int           i;

  s.pos    = 0;
  s.length = 0;
  flag     = 0;

  for (i = 0; i < 16; i++) {
    struct packed_ushort rv = next(&s);
    if ((i == 0 && rv.ucs != 0xffff) || (i > 0 && rv.ucs != 0))
      abort();
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
// DEFAULT-NEXT:     type @type[[TYPE_packed_ushort:[0-9]+]] packed_ushort = struct {
// DEFAULT-NEXT:         field0 ucs: u16;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_source:[0-9]+]] source = struct {
// DEFAULT-NEXT:         field0 pos: i32;
// DEFAULT-NEXT:         field1 length: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     global %[[VALUE_flag:[0-9]+]] flag: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_fetch:[0-9]+]] @fetch(%[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_source]]>) -> void [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(field1(deref(read<ptr<@type[[TYPE_source]]>>(%[[VALUE_p]]))), const<i32>(128));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_next:[0-9]+]] @next(%[[VALUE_p_2:[0-9]+]] p: ptr<@type[[TYPE_source]]>) -> @type[[TYPE_packed_ushort]] [linkage=internal] [inline=never] [definition=emitted] [abi=sysv64(scalar) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_rv:[0-9]+]] rv: @type[[TYPE_packed_ushort]] [storage=automatic];
// DEFAULT-NEXT:         if ge<i32>(read<i32>(field0(deref(read<ptr<@type[[TYPE_source]]>>(%[[VALUE_p_2]])))), read<i32>(field1(deref(read<ptr<@type[[TYPE_source]]>>(%[[VALUE_p_2]])))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%[[VALUE_flag]]), const<i32>(0))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_flag]], const<i32>(0));
// DEFAULT-NEXT:                         call<void, signature=fn(ptr<@type[[TYPE_source]]>) -> void>(%[[VALUE_fetch]], read<ptr<@type[[TYPE_source]]>>(%[[VALUE_p_2]]));
// DEFAULT-NEXT:                         return copy<@type[[TYPE_packed_ushort]], reason=return>(call<@type[[TYPE_packed_ushort]], signature=fn(ptr<@type[[TYPE_source]]>) -> @type[[TYPE_packed_ushort]], abi=sysv64(scalar) -> native_c>(%[[VALUE_next]], read<ptr<@type[[TYPE_source]]>>(%[[VALUE_p_2]])));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_flag]], const<i32>(1));
// DEFAULT-NEXT:                 write<u16>(field0(%[[VALUE_rv]]), reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(const<i32>(65535))));
// DEFAULT-NEXT:                 return copy<@type[[TYPE_packed_ushort]], reason=return>(read<@type[[TYPE_packed_ushort]]>(%[[VALUE_rv]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<u16>(field0(%[[VALUE_rv]]), reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         return copy<@type[[TYPE_packed_ushort]], reason=return>(read<@type[[TYPE_packed_ushort]]>(%[[VALUE_rv]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_s:[0-9]+]] s: @type[[TYPE_source]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(field0(%[[VALUE_s]]), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(field1(%[[VALUE_s]]), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_flag]], const<i32>(0));
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(16))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_rv_2:[0-9]+]] rv: @type[[TYPE_packed_ushort]] [storage=automatic] = copy<@type[[TYPE_packed_ushort]], reason=assign>(call<@type[[TYPE_packed_ushort]], signature=fn(ptr<@type[[TYPE_source]]>) -> @type[[TYPE_packed_ushort]], abi=sysv64(scalar) -> native_c>(%[[VALUE_next]], addr_of<ptr<@type[[TYPE_source]]>>(%[[VALUE_s]])));
// DEFAULT-NEXT:                     if logical_or<bool>(logical_and<bool>(eq<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0)), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field0(%[[VALUE_rv_2]])))), const<i32>(65535))), logical_and<bool>(gt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0)), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field0(%[[VALUE_rv_2]])))), const<i32>(0))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
