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

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR 41750 - IPA-SRA used to pass hash->sgot by value rather than by
// CHECK-NEXT: //! reference.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct bfd_link_hash_table {
// CHECK-NEXT:     hash: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct foo_link_hash_table {
// CHECK-NEXT:     root: bfd_link_hash_table,
// CHECK-NEXT:     dynobj: *mut i32,
// CHECK-NEXT:     sgot: *mut i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct foo_link_info {
// CHECK-NEXT:     hash: *mut foo_link_hash_table,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo_create_got_section(mut [[ABFD_0:abfd_[0-9]+]]: *mut i32, mut info: *mut foo_link_info) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe { (*info).hash }).sgot = [[ABFD_0]];
// CHECK-NEXT:     }
// CHECK-NEXT:     1 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn get_got(
// CHECK-NEXT:     mut [[ABFD_1:abfd_[0-9]+]]: *mut i32,
// CHECK-NEXT:     mut info: *mut foo_link_info,
// CHECK-NEXT:     mut [[HASH_0:hash_[0-9]+]]: *mut foo_link_hash_table,
// CHECK-NEXT: ) -> *mut i32 {
// CHECK-NEXT:     let mut got: *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut dynobj: *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     got = unsafe { (*[[HASH_0]]).sgot };
// CHECK-NEXT:     if !(got != std::ptr::null_mut::<i32>()) {
// CHECK-NEXT:         dynobj = unsafe { (*[[HASH_0]]).dynobj };
// CHECK-NEXT:         if !(dynobj != std::ptr::null_mut::<i32>()) {
// CHECK-NEXT:             let mut [[V_0:__v[0-9]+]]: *mut i32 = [[ABFD_1]];
// CHECK-NEXT:             dynobj = [[V_0]];
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*[[HASH_0]]).dynobj = [[V_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         if !(foo_create_got_section(dynobj, info) != (0 as i32)) {
// CHECK-NEXT:             return std::ptr::null_mut::<i32>();
// CHECK-NEXT:         }
// CHECK-NEXT:         got = unsafe { (*[[HASH_0]]).sgot };
// CHECK-NEXT:     }
// CHECK-NEXT:     got
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn elf64_ia64_check_relocs(mut [[ABFD_2:abfd_[0-9]+]]: *mut i32, mut info: *mut foo_link_info) -> *mut i32 {
// CHECK-NEXT:     get_got([[ABFD_2]], info, unsafe { (*info).hash })
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut link_info: foo_link_info = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut hash: foo_link_hash_table = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut abfd: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         link_info.hash = unsafe { std::ptr::addr_of_mut!(hash) };
// CHECK-NEXT:     }
// CHECK-NEXT:     if elf64_ia64_check_relocs(unsafe { std::ptr::addr_of_mut!(abfd) }, unsafe {
// CHECK-NEXT:         std::ptr::addr_of_mut!(link_info)
// CHECK-NEXT:     }) != unsafe { std::ptr::addr_of_mut!(abfd) }
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
