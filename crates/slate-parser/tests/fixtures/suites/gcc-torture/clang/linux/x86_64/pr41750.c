/* PR 41750 - IPA-SRA used to pass hash->sgot by value rather than by
   reference.  */

struct bfd_link_hash_table {
  int hash;
};

struct foo_link_hash_table {
  struct bfd_link_hash_table root;
  int                       *dynobj;
  int                       *sgot;
};

struct foo_link_info {
  struct foo_link_hash_table *hash;
};

extern void abort(void);

int __attribute__((noinline))
foo_create_got_section(int *abfd, struct foo_link_info *info) {
  info->hash->sgot = abfd;
  return 1;
}

static int *get_got(int *abfd, struct foo_link_info *info,
                    struct foo_link_hash_table *hash) {
  int *got;
  int *dynobj;

  got = hash->sgot;
  if (!got) {
    dynobj = hash->dynobj;
    if (!dynobj)
      hash->dynobj = dynobj = abfd;
    if (!foo_create_got_section(dynobj, info))
      return 0;
    got = hash->sgot;
  }
  return got;
}

int *__attribute__((noinline, noclone))
elf64_ia64_check_relocs(int *abfd, struct foo_link_info *info) {
  return get_got(abfd, info, info->hash);
}

struct foo_link_info       link_info;
struct foo_link_hash_table hash;
int                        abfd;

int main() {
  link_info.hash = &hash;
  if (elf64_ia64_check_relocs(&abfd, &link_info) != &abfd)
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
// DEFAULT-NEXT:     type @type[[TYPE_bfd_link_hash_table:[0-9]+]] bfd_link_hash_table = struct {
// DEFAULT-NEXT:         field0 hash: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_foo_link_hash_table:[0-9]+]] foo_link_hash_table = struct {
// DEFAULT-NEXT:         field0 root: @type[[TYPE_bfd_link_hash_table]];
// DEFAULT-NEXT:         field1 dynobj: ptr<i32>;
// DEFAULT-NEXT:         field2 sgot: ptr<i32>;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     type @type[[TYPE_foo_link_info:[0-9]+]] foo_link_info = struct {
// DEFAULT-NEXT:         field0 hash: ptr<@type[[TYPE_foo_link_hash_table]]>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_link_info:[0-9]+]] link_info: @type[[TYPE_foo_link_info]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_hash:[0-9]+]] hash: @type[[TYPE_foo_link_hash_table]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_abfd:[0-9]+]] abfd: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo_create_got_section:[0-9]+]] @foo_create_got_section(%[[VALUE_abfd_2:[0-9]+]] abfd: ptr<i32>, %[[VALUE_info:[0-9]+]] info: ptr<@type[[TYPE_foo_link_info]]>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<ptr<i32>>(field2(deref(read<ptr<@type[[TYPE_foo_link_hash_table]]>>(field0(deref(read<ptr<@type[[TYPE_foo_link_info]]>>(%[[VALUE_info]])))))), read<ptr<i32>>(%[[VALUE_abfd_2]]));
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_get_got:[0-9]+]] @get_got(%[[VALUE_abfd_3:[0-9]+]] abfd: ptr<i32>, %[[VALUE_info_2:[0-9]+]] info: ptr<@type[[TYPE_foo_link_info]]>, %[[VALUE_hash_2:[0-9]+]] hash: ptr<@type[[TYPE_foo_link_hash_table]]>) -> ptr<i32> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_got:[0-9]+]] got: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_dynobj:[0-9]+]] dynobj: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_got]], read<ptr<i32>>(field2(deref(read<ptr<@type[[TYPE_foo_link_hash_table]]>>(%[[VALUE_hash_2]])))));
// DEFAULT-NEXT:         if not<bool>(ne<ptr<i32>>(read<ptr<i32>>(%[[VALUE_got]]), null<ptr<i32>>))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<ptr<i32>>(%[[VALUE_dynobj]], read<ptr<i32>>(field1(deref(read<ptr<@type[[TYPE_foo_link_hash_table]]>>(%[[VALUE_hash_2]])))));
// DEFAULT-NEXT:                 if not<bool>(ne<ptr<i32>>(read<ptr<i32>>(%[[VALUE_dynobj]]), null<ptr<i32>>))
// DEFAULT-NEXT:                     write<ptr<i32>>(%[[VALUE_dynobj]], read<ptr<i32>>(%[[VALUE_abfd_3]]));
// DEFAULT-NEXT:                     write<ptr<i32>>(field1(deref(read<ptr<@type[[TYPE_foo_link_hash_table]]>>(%[[VALUE_hash_2]]))), read<ptr<i32>>(%[[VALUE_abfd_3]]));
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(call<i32, signature=fn(ptr<i32>, ptr<@type[[TYPE_foo_link_info]]>) -> i32>(%[[VALUE_foo_create_got_section]], read<ptr<i32>>(%[[VALUE_dynobj]]), read<ptr<@type[[TYPE_foo_link_info]]>>(%[[VALUE_info_2]])), const<i32>(0)))
// DEFAULT-NEXT:                     return null<ptr<i32>>;
// DEFAULT-NEXT:                 write<ptr<i32>>(%[[VALUE_got]], read<ptr<i32>>(field2(deref(read<ptr<@type[[TYPE_foo_link_hash_table]]>>(%[[VALUE_hash_2]])))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<ptr<i32>>(%[[VALUE_got]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_elf64_ia64_check_relocs:[0-9]+]] @elf64_ia64_check_relocs(%[[VALUE_abfd_4:[0-9]+]] abfd: ptr<i32>, %[[VALUE_info_3:[0-9]+]] info: ptr<@type[[TYPE_foo_link_info]]>) -> ptr<i32> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<ptr<i32>, signature=fn(ptr<i32>, ptr<@type[[TYPE_foo_link_info]]>, ptr<@type[[TYPE_foo_link_hash_table]]>) -> ptr<i32>>(%[[VALUE_get_got]], read<ptr<i32>>(%[[VALUE_abfd_4]]), read<ptr<@type[[TYPE_foo_link_info]]>>(%[[VALUE_info_3]]), read<ptr<@type[[TYPE_foo_link_hash_table]]>>(field0(deref(read<ptr<@type[[TYPE_foo_link_info]]>>(%[[VALUE_info_3]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_foo_link_hash_table]]>>(field0(%[[VALUE_link_info]]), addr_of<ptr<@type[[TYPE_foo_link_hash_table]]>>(%[[VALUE_hash]]));
// DEFAULT-NEXT:         if ne<ptr<i32>>(call<ptr<i32>, signature=fn(ptr<i32>, ptr<@type[[TYPE_foo_link_info]]>) -> ptr<i32>>(%[[VALUE_elf64_ia64_check_relocs]], addr_of<ptr<i32>>(%[[VALUE_abfd]]), addr_of<ptr<@type[[TYPE_foo_link_info]]>>(%[[VALUE_link_info]])), addr_of<ptr<i32>>(%[[VALUE_abfd]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
