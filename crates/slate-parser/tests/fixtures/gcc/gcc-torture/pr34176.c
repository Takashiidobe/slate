
typedef __SIZE_TYPE__ size_t;
typedef unsigned int  index_ty;
typedef index_ty     *index_list_ty;

struct mult_index {
  index_ty     index;
  unsigned int count;
};

struct mult_index_list {
  struct mult_index *item;
  size_t             nitems;
  size_t             nitems_max;

  struct mult_index *item2;
  size_t             nitems2_max;
};

int __attribute__((noinline)) hash_find_entry(size_t *result) {
  *result = 2;
  return 0;
}

extern void                                  abort(void);
struct mult_index *__attribute__((noinline)) foo(size_t n) {
  static int count = 0;
  if (count++ > 0)
    abort();
  return 0;
}

int main(void) {
  size_t nitems = 0;

  for (;;) {
    size_t list;

    hash_find_entry(&list);
    {
      size_t             len2 = list;
      struct mult_index *destptr;
      struct mult_index *dest;
      size_t             new_max = nitems + len2;

      if (new_max != len2)
        break;
      dest = foo(new_max);

      destptr = dest;
      while (len2--)
        destptr++;

      nitems = destptr - dest;
    }
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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     type @type1 index_ty = u32;
// DEFAULT-NEXT:     type @type2 index_list_ty = ptr<u32>;
// DEFAULT-NEXT:     type @type3 mult_index = struct {
// DEFAULT-NEXT:         field0 index: u32;
// DEFAULT-NEXT:         field1 count: u32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type4 mult_index_list = struct {
// DEFAULT-NEXT:         field0 item: ptr<@type3>;
// DEFAULT-NEXT:         field1 nitems: u64;
// DEFAULT-NEXT:         field2 nitems_max: u64;
// DEFAULT-NEXT:         field3 item2: ptr<@type3>;
// DEFAULT-NEXT:         field4 nitems2_max: u64;
// DEFAULT-NEXT:     } [size=40, align=8, offsets=[0, 8, 16, 24, 32]];
// DEFAULT-NEXT:     global %10 count: i32 [storage=static] = const<i32>(0) [linkage=internal];
// DEFAULT-NEXT:     fn %5 @hash_find_entry(%6 result: ptr<u64>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<u64>(deref(read<ptr<u64>>(%6)), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(2))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %8 @foo(%9 n: u64) -> ptr<@type3> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %20: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:         let %21: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%20), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%10, read<i32>(%21));
// DEFAULT-NEXT:         if gt<i32>(read<i32>(%20), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%7);
// DEFAULT-NEXT:         return null<ptr<@type3>>;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %12 nitems: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         for %18
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: omitted
// DEFAULT-NEXT:             increment: omitted
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %13 list: u64 [storage=automatic];
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<u64>) -> i32>(%5, addr_of<ptr<u64>>(%13));
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %14 len2: u64 [storage=automatic] = read<u64>(%13);
// DEFAULT-NEXT:                         let %15 destptr: ptr<@type3> [storage=automatic];
// DEFAULT-NEXT:                         let %16 dest: ptr<@type3> [storage=automatic];
// DEFAULT-NEXT:                         let %17 new_max: u64 [storage=automatic] = add<u64, overflow=wrap>(read<u64>(%12), read<u64>(%14));
// DEFAULT-NEXT:                         if ne<u64>(read<u64>(%17), read<u64>(%14))
// DEFAULT-NEXT:                             break %18;
// DEFAULT-NEXT:                         write<ptr<@type3>>(%16, call<ptr<@type3>, signature=fn(u64) -> ptr<@type3>>(%8, read<u64>(%17)));
// DEFAULT-NEXT:                         call<ptr<@type3>, signature=fn(u64) -> ptr<@type3>>(%8, read<u64>(%17));
// DEFAULT-NEXT:                         write<ptr<@type3>>(%15, read<ptr<@type3>>(%16));
// DEFAULT-NEXT:                         while %19 {
// DEFAULT-NEXT:                             let %22: u64 [synthetic] = read<u64>(%14);
// DEFAULT-NEXT:                             let %23: u64 [synthetic] = sub<u64, overflow=wrap>(read<u64>(%22), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                             write<u64>(%14, read<u64>(%23));
// DEFAULT-NEXT:                             yield ne<u64>(read<u64>(%22), const<u64>(0));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                             let %24: ptr<@type3> [synthetic] = read<ptr<@type3>>(%15);
// DEFAULT-NEXT:                             let %25: ptr<@type3> [synthetic] = ptr_offset<ptr<@type3>, subtract=false, element=@type3, overflow=ub>(read<ptr<@type3>>(%24), const<i32>(1));
// DEFAULT-NEXT:                             write<ptr<@type3>>(%15, read<ptr<@type3>>(%25));
// DEFAULT-NEXT:                         write<u64>(%12, reinterpret<u64, reason=assign, fits=unknown>(ptr_diff<i64, element=@type3, same_array=required, overflow=ub>(read<ptr<@type3>>(%15), read<ptr<@type3>>(%16))));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
