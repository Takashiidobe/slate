// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -std=gnu11

typedef unsigned long size_t;

struct plain {
  int n;
  char name[];
};

struct counted {
  short pad;
  unsigned char n;
  long name[] __attribute__((counted_by(n)));
};

struct nested_counter {
  struct {
    int n;
  };
  int name[] __attribute__((__counted_by__(n)));
};

struct counted_pointer {
  int n;
  int *items __attribute__((counted_by(n)));
};

#define flex_counter(fam) __builtin_counted_by_ref(fam)
#define set_flex_counter(fam, count)                                           \
  ({                                                                           \
    *_Generic(flex_counter(fam), void *: &(size_t){0},                         \
              default: flex_counter(fam)) = (count);                           \
  })

_Static_assert(__builtin_types_compatible_p(typeof(flex_counter(((struct plain *)0)->name)), void *), "plain");
_Static_assert(__builtin_types_compatible_p(typeof(flex_counter(((struct counted *)0)->name)), unsigned char *), "counted");

void store_counted(struct counted *p, size_t count) {
  *__builtin_counted_by_ref(p->name) = count;
}

void store_nested(struct nested_counter *p) {
  *__builtin_counted_by_ref((p->name)) = 4;
}

void store_pointer(struct counted_pointer *p) {
  *__builtin_counted_by_ref(p->items) += 1;
}

void store_dot(struct counted *p) {
  *__builtin_counted_by_ref(p[1].name) = 2;
}

void set_plain(struct plain *p, size_t count) {
  set_flex_counter(p->name, count);
}

void set_counted(struct counted *p, size_t count) {
  set_flex_counter(p->name, count);
}

int has_counter(struct plain *p) {
  return flex_counter(p->name) ? 1 : 0;
}

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// IR-NEXT:     type @type[[TYPE_plain:[0-9]+]] plain = struct {
// IR-NEXT:         field0 n: i32;
// IR-NEXT:         field1 name: array<i8, incomplete>;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type[[TYPE_counted:[0-9]+]] counted = struct {
// IR-NEXT:         field0 pad: i16;
// IR-NEXT:         field1 n: u8;
// IR-NEXT:         field2 name: array<i64, incomplete>;
// IR-NEXT:     } [size=8, align=8, offsets=[0, 2, 8]];
// IR-NEXT:     type @type[[TYPE_nested_counter:[0-9]+]] nested_counter = struct {
// IR-NEXT:         field0 <anonymous>: @type[[TYPE0:[0-9]+]];
// IR-NEXT:         field1 name: array<i32, incomplete>;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type[[TYPE0]] = struct {
// IR-NEXT:         field0 n: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_counted_pointer:[0-9]+]] counted_pointer = struct {
// IR-NEXT:         field0 n: i32;
// IR-NEXT:         field1 items: ptr<i32>;
// IR-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// IR-NEXT:     fn %[[VALUE_store_counted:[0-9]+]] @store_counted(%[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_counted]]>, %[[VALUE_count:[0-9]+]] count: u64) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         write<u8>(deref(addr_of<ptr<u8>>(field1(deref(read<ptr<@type[[TYPE_counted]]>>(%[[VALUE_p]]))))), truncate<u8, reason=assign, fits=unknown>(read<u64>(%[[VALUE_count]])));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_store_nested:[0-9]+]] @store_nested(%[[VALUE_p_2:[0-9]+]] p: ptr<@type[[TYPE_nested_counter]]>) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         write<i32>(deref(addr_of<ptr<i32>>(field0(field0(deref(read<ptr<@type[[TYPE_nested_counter]]>>(%[[VALUE_p_2]])))))), const<i32>(4));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_store_pointer:[0-9]+]] @store_pointer(%[[VALUE_p_3:[0-9]+]] p: ptr<@type[[TYPE_counted_pointer]]>) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         let %[[VALUE0:[0-9]+]]: ptr<i32> [synthetic] = addr_of<ptr<i32>>(field0(deref(read<ptr<@type[[TYPE_counted_pointer]]>>(%[[VALUE_p_3]]))));
// IR-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%[[VALUE0]])));
// IR-NEXT:         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// IR-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE0]])), read<i32>(%[[VALUE2]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_store_dot:[0-9]+]] @store_dot(%[[VALUE_p_4:[0-9]+]] p: ptr<@type[[TYPE_counted]]>) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         write<u8>(deref(addr_of<ptr<u8>>(field1(deref(ptr_offset<ptr<@type[[TYPE_counted]]>, subtract=false, element=@type[[TYPE_counted]], overflow=ub>(read<ptr<@type[[TYPE_counted]]>>(%[[VALUE_p_4]]), const<i32>(1)))))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(2))));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_set_plain:[0-9]+]] @set_plain(%[[VALUE_p_5:[0-9]+]] p: ptr<@type[[TYPE_plain]]>, %[[VALUE_count_2:[0-9]+]] count: u64) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         let %[[VALUE3:[0-9]+]]: u64 [synthetic];
// IR-NEXT:         {
// IR-NEXT:             let %[[VALUE4:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_count_2]]);
// IR-NEXT:             write<u64>(deref(addr_of<ptr<u64>>(compound_literal %[[VALUE5:[0-9]+]] [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))))), read<u64>(%[[VALUE4]]));
// IR-NEXT:             write<u64>(%[[VALUE3]], read<u64>(%[[VALUE4]]));
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_set_counted:[0-9]+]] @set_counted(%[[VALUE_p_6:[0-9]+]] p: ptr<@type[[TYPE_counted]]>, %[[VALUE_count_3:[0-9]+]] count: u64) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         let %[[VALUE6:[0-9]+]]: u8 [synthetic];
// IR-NEXT:         {
// IR-NEXT:             let %[[VALUE7:[0-9]+]]: u8 [synthetic] = truncate<u8, reason=assign, fits=unknown>(read<u64>(%[[VALUE_count_3]]));
// IR-NEXT:             write<u8>(deref(addr_of<ptr<u8>>(field1(deref(read<ptr<@type[[TYPE_counted]]>>(%[[VALUE_p_6]]))))), read<u8>(%[[VALUE7]]));
// IR-NEXT:             write<u8>(%[[VALUE6]], read<u8>(%[[VALUE7]]));
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_has_counter:[0-9]+]] @has_counter(%[[VALUE_p_7:[0-9]+]] p: ptr<@type[[TYPE_plain]]>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return conditional<i32>(ne<ptr<void>>(null<ptr<void>>, null<ptr<void>>), const<i32>(1), const<i32>(0));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
