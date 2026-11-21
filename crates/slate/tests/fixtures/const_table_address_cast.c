#include <stdio.h>
#include <string.h>

struct entry {
  const char *name;
  int value;
  struct entry *next;
};

static const struct entry entries[] = {
    {"zero", 0, 0},
    {"one", 1, 0},
    {"two", 2, 0},
};

static const unsigned int crc_table[4] = {0x00000000u, 0x77073096u, 0xee0e612cu, 0x990951bau};

const int exported_table[3] = {10, 20, 30};

static const char *const names[] = {"alpha", "beta", "gamma"};

static const struct entry *const first = &entries[0];

static const unsigned char small[2] = {3, 4};

static int sum_values(const struct entry *table, int count) {
  int total = 0;
  for (int i = 0; i < count; i++)
    total += table[i].value;
  return total;
}

static unsigned int fold_crc(const unsigned int *table, int count) {
  unsigned int crc = 0;
  for (int i = 0; i < count; i++)
    crc ^= table[i];
  return crc;
}

static int read_mutable(int *table, int index) { return table[index]; }

int main(void) {
  struct entry *loose = (struct entry *)&entries[1];
  unsigned int *crc = (unsigned int *)crc_table;
  const struct entry *walk = entries;
  printf("%d %s %d\n", sum_values(entries, 3), loose->name, loose->value);
  printf("%08x %08x\n", fold_crc(crc_table, 4), crc[3]);
  printf("%d %d\n", read_mutable((int *)exported_table, 2), exported_table[0]);
  printf("%s %zu %s\n", names[2], strlen(names[0]), walk[2].name);
  printf("%d\n", (int)(sizeof entries / sizeof entries[0]));
  printf("%s %d %d\n", first->name, small[1], *(unsigned char *)&small[0]);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct entry {
// CHECK-NEXT:     name: *const i8,
// CHECK-NEXT:     value: i32,
// CHECK-NEXT:     next: *mut entry,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe impl<T> Sync for __SlateAlign16<T> {}
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(transparent)]
// CHECK-NEXT: struct __SlateConst<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: unsafe impl<T> Sync for __SlateConst<T> {}
// CHECK-EMPTY:
// CHECK-NEXT: static entries: __SlateAlign16<[entry; 3]> = __SlateAlign16(
// CHECK-NEXT:     [
// CHECK-NEXT:         {
// CHECK-NEXT:             let mut [[T_0:__t[0-9]+]]: entry = unsafe { core::mem::zeroed() };
// CHECK-NEXT:             [[T_0]].name = (b"zero\0".as_ptr() as *mut i8) as *const i8;
// CHECK-NEXT:             [[T_0]].value = 0i32;
// CHECK-NEXT:             [[T_0]].next = core::ptr::null_mut::<entry>();
// CHECK-NEXT:             [[T_0]]
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:             let mut [[T_1:__t[0-9]+]]: entry = unsafe { core::mem::zeroed() };
// CHECK-NEXT:             [[T_1]].name = (b"one\0".as_ptr() as *mut i8) as *const i8;
// CHECK-NEXT:             [[T_1]].value = 1i32;
// CHECK-NEXT:             [[T_1]].next = core::ptr::null_mut::<entry>();
// CHECK-NEXT:             [[T_1]]
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:             let mut [[T_2:__t[0-9]+]]: entry = unsafe { core::mem::zeroed() };
// CHECK-NEXT:             [[T_2]].name = (b"two\0".as_ptr() as *mut i8) as *const i8;
// CHECK-NEXT:             [[T_2]].value = 2i32;
// CHECK-NEXT:             [[T_2]].next = core::ptr::null_mut::<entry>();
// CHECK-NEXT:             [[T_2]]
// CHECK-NEXT:         },
// CHECK-NEXT:     ],
// CHECK-NEXT: );
// CHECK-EMPTY:
// CHECK-NEXT: static crc_table: __SlateAlign16<[u32; 4]> = __SlateAlign16(
// CHECK-NEXT:     [0u32, 1996959894u32, 3993919788u32, 2567524794u32],
// CHECK-NEXT: );
// CHECK-EMPTY:
// CHECK-NEXT: static exported_table: [i32; 3] = [10, 20, 30];
// CHECK-EMPTY:
// CHECK-NEXT: static names: __SlateAlign16<[*const i8; 3]> = __SlateAlign16(
// CHECK-NEXT:     [
// CHECK-NEXT:         (b"alpha\0".as_ptr() as *mut i8) as *const i8,
// CHECK-NEXT:         (b"beta\0".as_ptr() as *mut i8) as *const i8,
// CHECK-NEXT:         (b"gamma\0".as_ptr() as *mut i8) as *const i8,
// CHECK-NEXT:     ],
// CHECK-NEXT: );
// CHECK-EMPTY:
// CHECK-NEXT: static first: __SlateConst<*const entry> = __SlateConst(
// CHECK-NEXT:     unsafe { core::ptr::addr_of!(entries.0) as *const entry }
// CHECK-NEXT:         .wrapping_offset(0i32 as isize),
// CHECK-NEXT: );
// CHECK-EMPTY:
// CHECK-NEXT: static small: [u8; 2] = [3, 4];
// CHECK-EMPTY:
// CHECK-NEXT: use slate_support::strlen;
// CHECK-EMPTY:
// CHECK-NEXT: fn sum_values(table: &[entry]) -> i32 {
// CHECK-NEXT:     let count: i32 = table.len() as i32;
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     for i in 0..count {
// CHECK-NEXT:         total += table[i as usize].value;
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fold_crc(table: &[u32]) -> u32 {
// CHECK-NEXT:     let count: i32 = table.len() as i32;
// CHECK-NEXT:     let mut crc: u32 = 0;
// CHECK-NEXT:     for i in 0..count {
// CHECK-NEXT:         crc ^= table[i as usize];
// CHECK-NEXT:     }
// CHECK-NEXT:     crc
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn read_mutable(table: *mut i32, index: i32) -> i32 {
// CHECK-NEXT:     unsafe { *table.offset(index as isize) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let loose: *mut entry = (unsafe {
// CHECK-NEXT:         (core::ptr::addr_of!(entries.0) as *const entry).add(1)
// CHECK-NEXT:     }) as *mut entry;
// CHECK-NEXT:     let crc: *mut u32 = (unsafe { core::ptr::addr_of!(crc_table.0) as *const u32 })
// CHECK-NEXT:         as *mut u32;
// CHECK-NEXT:     let walk: *const entry = unsafe { core::ptr::addr_of!(entries.0) as *const entry };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%d %s %d\n".as_ptr(),
// CHECK-NEXT:             sum_values(
// CHECK-NEXT:                 core::slice::from_raw_parts(
// CHECK-NEXT:                     core::ptr::addr_of!(entries.0) as *const entry,
// CHECK-NEXT:                     3 as usize,
// CHECK-NEXT:                 ),
// CHECK-NEXT:             ),
// CHECK-NEXT:             (*loose).name,
// CHECK-NEXT:             (*loose).value,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%08x %08x\n".as_ptr(),
// CHECK-NEXT:             fold_crc(
// CHECK-NEXT:                 core::slice::from_raw_parts(
// CHECK-NEXT:                     core::ptr::addr_of!(crc_table.0) as *const u32,
// CHECK-NEXT:                     4 as usize,
// CHECK-NEXT:                 ),
// CHECK-NEXT:             ),
// CHECK-NEXT:             *crc.add(3),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {}",
// CHECK-NEXT:         read_mutable(
// CHECK-NEXT:             (unsafe { core::ptr::addr_of!(exported_table) as *const i32 }) as *mut i32,
// CHECK-NEXT:             2i32,
// CHECK-NEXT:         ),
// CHECK-NEXT:         unsafe { exported_table[0] }
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%s %zu %s\n".as_ptr(),
// CHECK-NEXT:             names.0[2],
// CHECK-NEXT:             strlen(names.0[0] as *const core::ffi::c_char) as u64,
// CHECK-NEXT:             (*walk.add(2)).name,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}",
// CHECK-NEXT:         ((core::mem::size_of::<[entry; 3]>() as u64)
// CHECK-NEXT:             / (core::mem::size_of::<entry>() as u64)) as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%s %d %d\n".as_ptr(),
// CHECK-NEXT:             (*first.0).name,
// CHECK-NEXT:             small[1] as i32,
// CHECK-NEXT:             *((core::ptr::addr_of!(small) as *const u8).add(0) as *mut u8) as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
