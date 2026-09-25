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
// DEFAULT-NEXT:     type @type0 bfd_link_hash_table = struct {
// DEFAULT-NEXT:         field0 hash: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type1 foo_link_hash_table = struct {
// DEFAULT-NEXT:         field0 root: @type0;
// DEFAULT-NEXT:         field1 dynobj: ptr<i32>;
// DEFAULT-NEXT:         field2 sgot: ptr<i32>;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     type @type2 foo_link_info = struct {
// DEFAULT-NEXT:         field0 hash: ptr<@type1>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     global %16 link_info: @type2 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %17 hash: @type1 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %18 abfd: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %3 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %4 @foo_create_got_section(%5 abfd: ptr<i32>, %6 info: ptr<@type2>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<ptr<i32>>(field2(deref(read<ptr<@type1>>(field0(deref(read<ptr<@type2>>(%6)))))), read<ptr<i32>>(%5));
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @get_got(%8 abfd: ptr<i32>, %9 info: ptr<@type2>, %10 hash: ptr<@type1>) -> ptr<i32> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %11 got: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         let %12 dynobj: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<i32>>(%11, read<ptr<i32>>(field2(deref(read<ptr<@type1>>(%10)))));
// DEFAULT-NEXT:         if not<bool>(ne<ptr<i32>>(read<ptr<i32>>(%11), null<ptr<i32>>))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<ptr<i32>>(%12, read<ptr<i32>>(field1(deref(read<ptr<@type1>>(%10)))));
// DEFAULT-NEXT:                 if not<bool>(ne<ptr<i32>>(read<ptr<i32>>(%12), null<ptr<i32>>))
// DEFAULT-NEXT:                     write<ptr<i32>>(%12, read<ptr<i32>>(%8));
// DEFAULT-NEXT:                     write<ptr<i32>>(field1(deref(read<ptr<@type1>>(%10))), read<ptr<i32>>(%8));
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(call<i32, signature=fn(ptr<i32>, ptr<@type2>) -> i32>(%4, read<ptr<i32>>(%12), read<ptr<@type2>>(%9)), const<i32>(0)))
// DEFAULT-NEXT:                     return null<ptr<i32>>;
// DEFAULT-NEXT:                 write<ptr<i32>>(%11, read<ptr<i32>>(field2(deref(read<ptr<@type1>>(%10)))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<ptr<i32>>(%11);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @elf64_ia64_check_relocs(%14 abfd: ptr<i32>, %15 info: ptr<@type2>) -> ptr<i32> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<ptr<i32>, signature=fn(ptr<i32>, ptr<@type2>, ptr<@type1>) -> ptr<i32>>(%7, read<ptr<i32>>(%14), read<ptr<@type2>>(%15), read<ptr<@type1>>(field0(deref(read<ptr<@type2>>(%15)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<ptr<@type1>>(field0(%16), addr_of<ptr<@type1>>(%17));
// DEFAULT-NEXT:         if ne<ptr<i32>>(call<ptr<i32>, signature=fn(ptr<i32>, ptr<@type2>) -> ptr<i32>>(%13, addr_of<ptr<i32>>(%18), addr_of<ptr<@type2>>(%16)), addr_of<ptr<i32>>(%18))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
