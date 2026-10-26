#include <stdio.h>

struct Record {
  int  head;
  char name[10];
  int  tail;
};

struct Trailing {
  int count;
  int items[2];
};

struct Nested {
  struct Record record;
  int           extra[3];
};

static struct Record global_record;

#define SIZES(...)                                                             \
  printf("%-22s %lu %lu %lu %lu\n", #__VA_ARGS__,                              \
         (unsigned long)__builtin_object_size(__VA_ARGS__, 0),                 \
         (unsigned long)__builtin_object_size(__VA_ARGS__, 1),                 \
         (unsigned long)__builtin_object_size(__VA_ARGS__, 2),                 \
         (unsigned long)__builtin_object_size(__VA_ARGS__, 3))

static unsigned long member_sizes(struct Record *record, struct Trailing *trailing) {
  return __builtin_object_size(&record->name, 1) * 1000 +
         __builtin_object_size(record->name + 4, 3) * 100 +
         __builtin_object_size(trailing->items, 3);
}

int main(void) {
  struct Record   record;
  struct Trailing trailing;
  struct Nested   nested;
  int             numbers[5];
  int             index = 2;

  SIZES(&record);
  SIZES(&record.name);
  SIZES(&record.name[3]);
  SIZES(record.name + 12);
  SIZES(&record.tail);
  SIZES((char *)&record.head);
  SIZES((char *)&record.head + 1);
  SIZES((char *)&record.head + 2);
  SIZES((void *)&record.head);
  SIZES(trailing.items);
  SIZES(&trailing.items[1]);
  SIZES(&nested.record.name[1]);
  SIZES(nested.extra + 1);
  SIZES(&nested.record);
  SIZES(numbers);
  SIZES(&numbers[5]);
  SIZES(&numbers[7]);
  SIZES(numbers - 1);
  SIZES(&numbers);
  SIZES(&numbers + 1);
  SIZES("hello");
  SIZES(&"hello"[2]);
  SIZES(((int[]){1, 2, 3}));
  SIZES(&global_record.name[2]);

  printf("members %lu\n", member_sizes(&record, &trailing));
  printf("effects %lu %lu %lu %d\n",
         (unsigned long)__builtin_object_size(&numbers[index++], 0),
         (unsigned long)__builtin_object_size(&numbers[index++], 2),
         (unsigned long)__builtin_dynamic_object_size(&numbers[index++], 1),
         index);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Record {
// CHECK-NEXT:     head: i32,
// CHECK-NEXT:     name: [i8; 10],
// CHECK-NEXT:     tail: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Trailing {
// CHECK-NEXT:     count: i32,
// CHECK-NEXT:     items: [i32; 2],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Nested {
// CHECK-NEXT:     record: Record,
// CHECK-NEXT:     extra: [i32; 3],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut global_record: Record = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn member_sizes(record: &Record, trailing: &Trailing) -> u64 {
// CHECK-NEXT:     10608u64
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut record: Record = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut trailing: Trailing = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     __SlateAlign16([0i32; 5]);
// CHECK-NEXT:     let index: i32 = 2;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%-22s %lu %lu %lu %lu\n".as_ptr(),
// CHECK-NEXT:             c"&record".as_ptr() as *mut i8,
// CHECK-NEXT:             20u64,
// CHECK-NEXT:             20u64,
// CHECK-NEXT:             20u64,
// CHECK-NEXT:             20u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%-22s %lu %lu %lu %lu\n".as_ptr(),
// CHECK-NEXT:             c"&record.name".as_ptr() as *mut i8,
// CHECK-NEXT:             16u64,
// CHECK-NEXT:             10u64,
// CHECK-NEXT:             16u64,
// CHECK-NEXT:             10u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%-22s %lu %lu %lu %lu\n".as_ptr(),
// CHECK-NEXT:             c"&record.name[3]".as_ptr() as *mut i8,
// CHECK-NEXT:             13u64,
// CHECK-NEXT:             7u64,
// CHECK-NEXT:             13u64,
// CHECK-NEXT:             7u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%-22s %lu %lu %lu %lu\n".as_ptr(),
// CHECK-NEXT:             c"record.name + 12".as_ptr() as *mut i8,
// CHECK-NEXT:             4u64,
// CHECK-NEXT:             4u64,
// CHECK-NEXT:             4u64,
// CHECK-NEXT:             0u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%-22s %lu %lu %lu %lu\n".as_ptr(),
// CHECK-NEXT:             c"&record.tail".as_ptr() as *mut i8,
// CHECK-NEXT:             4u64,
// CHECK-NEXT:             4u64,
// CHECK-NEXT:             4u64,
// CHECK-NEXT:             4u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%-22s %lu %lu %lu %lu\n".as_ptr(),
// CHECK-NEXT:             c"(char *)&record.head".as_ptr() as *mut i8,
// CHECK-NEXT:             20u64,
// CHECK-NEXT:             4u64,
// CHECK-NEXT:             20u64,
// CHECK-NEXT:             4u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%-22s %lu %lu %lu %lu\n".as_ptr(),
// CHECK-NEXT:             c"(char *)&record.head + 1".as_ptr() as *mut i8,
// CHECK-NEXT:             19u64,
// CHECK-NEXT:             19u64,
// CHECK-NEXT:             19u64,
// CHECK-NEXT:             0u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%-22s %lu %lu %lu %lu\n".as_ptr(),
// CHECK-NEXT:             c"(char *)&record.head + 2".as_ptr() as *mut i8,
// CHECK-NEXT:             18u64,
// CHECK-NEXT:             18u64,
// CHECK-NEXT:             18u64,
// CHECK-NEXT:             0u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%-22s %lu %lu %lu %lu\n".as_ptr(),
// CHECK-NEXT:             c"(void *)&record.head".as_ptr() as *mut i8,
// CHECK-NEXT:             20u64,
// CHECK-NEXT:             4u64,
// CHECK-NEXT:             20u64,
// CHECK-NEXT:             4u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%-22s %lu %lu %lu %lu\n".as_ptr(),
// CHECK-NEXT:             c"trailing.items".as_ptr() as *mut i8,
// CHECK-NEXT:             8u64,
// CHECK-NEXT:             8u64,
// CHECK-NEXT:             8u64,
// CHECK-NEXT:             8u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%-22s %lu %lu %lu %lu\n".as_ptr(),
// CHECK-NEXT:             c"&trailing.items[1]".as_ptr() as *mut i8,
// CHECK-NEXT:             4u64,
// CHECK-NEXT:             4u64,
// CHECK-NEXT:             4u64,
// CHECK-NEXT:             4u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%-22s %lu %lu %lu %lu\n".as_ptr(),
// CHECK-NEXT:             c"&nested.record.name[1]".as_ptr() as *mut i8,
// CHECK-NEXT:             27u64,
// CHECK-NEXT:             9u64,
// CHECK-NEXT:             27u64,
// CHECK-NEXT:             9u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%-22s %lu %lu %lu %lu\n".as_ptr(),
// CHECK-NEXT:             c"nested.extra + 1".as_ptr() as *mut i8,
// CHECK-NEXT:             8u64,
// CHECK-NEXT:             8u64,
// CHECK-NEXT:             8u64,
// CHECK-NEXT:             8u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%-22s %lu %lu %lu %lu\n".as_ptr(),
// CHECK-NEXT:             c"&nested.record".as_ptr() as *mut i8,
// CHECK-NEXT:             32u64,
// CHECK-NEXT:             20u64,
// CHECK-NEXT:             32u64,
// CHECK-NEXT:             20u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%-22s %lu %lu %lu %lu\n".as_ptr(),
// CHECK-NEXT:             c"numbers".as_ptr() as *mut i8,
// CHECK-NEXT:             20u64,
// CHECK-NEXT:             20u64,
// CHECK-NEXT:             20u64,
// CHECK-NEXT:             20u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%-22s %lu %lu %lu %lu\n".as_ptr(),
// CHECK-NEXT:             c"&numbers[5]".as_ptr() as *mut i8,
// CHECK-NEXT:             0u64,
// CHECK-NEXT:             0u64,
// CHECK-NEXT:             0u64,
// CHECK-NEXT:             0u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%-22s %lu %lu %lu %lu\n".as_ptr(),
// CHECK-NEXT:             c"&numbers[7]".as_ptr() as *mut i8,
// CHECK-NEXT:             0u64,
// CHECK-NEXT:             0u64,
// CHECK-NEXT:             0u64,
// CHECK-NEXT:             0u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%-22s %lu %lu %lu %lu\n".as_ptr(),
// CHECK-NEXT:             c"numbers - 1".as_ptr() as *mut i8,
// CHECK-NEXT:             0u64,
// CHECK-NEXT:             0u64,
// CHECK-NEXT:             0u64,
// CHECK-NEXT:             0u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%-22s %lu %lu %lu %lu\n".as_ptr(),
// CHECK-NEXT:             c"&numbers".as_ptr() as *mut i8,
// CHECK-NEXT:             20u64,
// CHECK-NEXT:             20u64,
// CHECK-NEXT:             20u64,
// CHECK-NEXT:             20u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%-22s %lu %lu %lu %lu\n".as_ptr(),
// CHECK-NEXT:             c"&numbers + 1".as_ptr() as *mut i8,
// CHECK-NEXT:             0u64,
// CHECK-NEXT:             0u64,
// CHECK-NEXT:             0u64,
// CHECK-NEXT:             0u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%-22s %lu %lu %lu %lu\n".as_ptr(),
// CHECK-NEXT:             c"\"hello\"".as_ptr() as *mut i8,
// CHECK-NEXT:             6u64,
// CHECK-NEXT:             6u64,
// CHECK-NEXT:             6u64,
// CHECK-NEXT:             6u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%-22s %lu %lu %lu %lu\n".as_ptr(),
// CHECK-NEXT:             c"&\"hello\"[2]".as_ptr() as *mut i8,
// CHECK-NEXT:             4u64,
// CHECK-NEXT:             4u64,
// CHECK-NEXT:             4u64,
// CHECK-NEXT:             4u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%-22s %lu %lu %lu %lu\n".as_ptr(),
// CHECK-NEXT:             c"((int[]){1, 2, 3})".as_ptr() as *mut i8,
// CHECK-NEXT:             12u64,
// CHECK-NEXT:             12u64,
// CHECK-NEXT:             12u64,
// CHECK-NEXT:             12u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%-22s %lu %lu %lu %lu\n".as_ptr(),
// CHECK-NEXT:             c"&global_record.name[2]".as_ptr() as *mut i8,
// CHECK-NEXT:             14u64,
// CHECK-NEXT:             8u64,
// CHECK-NEXT:             14u64,
// CHECK-NEXT:             8u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"members %lu\n".as_ptr(),
// CHECK-NEXT:             member_sizes(
// CHECK-NEXT:                 &(*std::ptr::addr_of_mut!(record)),
// CHECK-NEXT:                 &(*std::ptr::addr_of_mut!(trailing)),
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"effects %lu %lu %lu %d\n".as_ptr(),
// CHECK-NEXT:             18446744073709551615u64,
// CHECK-NEXT:             0u64,
// CHECK-NEXT:             18446744073709551615u64,
// CHECK-NEXT:             index,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
