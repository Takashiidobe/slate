#include <stdio.h>
#include <stdlib.h>

static int values[] = {11, 22, 33, 44, 55};
static const int *middle = &values[(8 - 2) / 3];
static const int *end = values + (2 * 3 - 1);
static int twice(int x) { return x * 2; }
struct Entry {
    const int *value;
    int (*callback)(int);
    void *opaque;
    const char *name;
};
static struct Entry entries[] = {
    {values + (7 - 6), &twice, (void *)&twice, "twice"},
    {&values[1 + 2], abs, (void *)abs, "abs"}
};
static struct {
    struct Entry nested[2];
    const int *tail;
} group = {{{values + (9 / 3), twice, (void *)twice, "group"}}, values + (1 << 2)};

int main(void) {
    static int flag = 7;
    static void *addresses[] = {(void *)&flag, (void *)&twice};
    int (*callback)(int) = (int (*)(int))entries[0].opaque;
    int (*external)(int) = (int (*)(int))entries[1].opaque;
    int (*local)(int) = (int (*)(int))addresses[1];
    printf("%d %ld %d %d %s %d %d %d %d %d %d\n",
           *middle, (long)(end - values), *entries[0].value,
           *entries[1].value, entries[0].name, callback(6), external(-9),
           *group.nested[0].value, *group.tail, *(int *)addresses[0], local(4));
    return group.nested[1].value != 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn abs(__x: i32) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: static mut values: __SlateAlign16<[i32; 5]> = __SlateAlign16([
// CHECK-NEXT:     11i32, 22i32, 33i32, 44i32, 55i32,
// CHECK-NEXT: ]);
// CHECK-NEXT: static mut middle: *const i32 = (unsafe {
// CHECK-NEXT:     unsafe { std::ptr::addr_of_mut!(values.0) as *mut i32 }.offset(2i32 as isize)
// CHECK-NEXT: }) as *const i32;
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut end: *const i32 = (unsafe {
// CHECK-NEXT:     unsafe { std::ptr::addr_of_mut!(values.0) as *mut i32 }.offset(5i32 as isize)
// CHECK-NEXT: }) as *const i32;
// CHECK-NEXT: extern "C-unwind" fn twice(x: i32) -> i32 {
// CHECK-NEXT:     x * 2
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Entry {
// CHECK-NEXT:     value: *const i32,
// CHECK-NEXT:     callback: Option<unsafe extern "C-unwind" fn(i32) -> i32>,
// CHECK-NEXT:     opaque: *mut (),
// CHECK-NEXT:     name: *const i8,
// CHECK-NEXT: }
// CHECK-NEXT: static mut entries: __SlateAlign16<[Entry; 2]> = __SlateAlign16([
// CHECK-NEXT:     Entry {
// CHECK-NEXT:         value: (unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(values.0) as *mut i32 }.offset(1i32 as isize)
// CHECK-NEXT:         }) as *const i32,
// CHECK-NEXT:         callback: Some(twice),
// CHECK-NEXT:         opaque: unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn(i32) -> i32>,
// CHECK-NEXT:                 *mut (),
// CHECK-NEXT:             >(Some(twice))
// CHECK-NEXT:         },
// CHECK-NEXT:         name: (b"twice\0".as_ptr() as *mut i8) as *const i8,
// CHECK-NEXT:     },
// CHECK-NEXT:     Entry {
// CHECK-NEXT:         value: (unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(values.0) as *mut i32 }.offset(3i32 as isize)
// CHECK-NEXT:         }) as *const i32,
// CHECK-NEXT:         callback: unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 *const (),
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn(i32) -> i32>,
// CHECK-NEXT:             >(abs as *const ())
// CHECK-NEXT:         },
// CHECK-NEXT:         opaque: unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn(i32) -> i32>,
// CHECK-NEXT:                 *mut (),
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     *const (),
// CHECK-NEXT:                     Option<unsafe extern "C-unwind" fn(i32) -> i32>,
// CHECK-NEXT:                 >(abs as *const ())
// CHECK-NEXT:             })
// CHECK-NEXT:         },
// CHECK-NEXT:         name: (b"abs\0".as_ptr() as *mut i8) as *const i8,
// CHECK-NEXT:     },
// CHECK-NEXT: ]);
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     nested: [Entry; 2],
// CHECK-NEXT:     tail: *const i32,
// CHECK-NEXT: }
// CHECK-NEXT: static mut group: [[SLATERECORD_0]] = [[SLATERECORD_0]] {
// CHECK-NEXT:     nested: {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: [Entry; 2] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]][0] = Entry {
// CHECK-NEXT:             value: (unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(values.0) as *mut i32 }
// CHECK-NEXT:                     .offset(3i32 as isize)
// CHECK-NEXT:             }) as *const i32,
// CHECK-NEXT:             callback: Some(twice),
// CHECK-NEXT:             opaque: unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     Option<unsafe extern "C-unwind" fn(i32) -> i32>,
// CHECK-NEXT:                     *mut (),
// CHECK-NEXT:                 >(Some(twice))
// CHECK-NEXT:             },
// CHECK-NEXT:             name: (b"group\0".as_ptr() as *mut i8) as *const i8,
// CHECK-NEXT:         };
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     },
// CHECK-NEXT:     tail: (unsafe {
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(values.0) as *mut i32 }.offset(4i32 as isize)
// CHECK-NEXT:     }) as *const i32,
// CHECK-NEXT: };
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let callback: Option<unsafe extern "C-unwind" fn(i32) -> i32> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             *mut (),
// CHECK-NEXT:             Option<unsafe extern "C-unwind" fn(i32) -> i32>,
// CHECK-NEXT:         >(unsafe { entries.0[0].opaque })
// CHECK-NEXT:     };
// CHECK-NEXT:     let external: Option<unsafe extern "C-unwind" fn(i32) -> i32> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             *mut (),
// CHECK-NEXT:             Option<unsafe extern "C-unwind" fn(i32) -> i32>,
// CHECK-NEXT:         >(unsafe { entries.0[1].opaque })
// CHECK-NEXT:     };
// CHECK-NEXT:     let local: Option<unsafe extern "C-unwind" fn(i32) -> i32> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             *mut (),
// CHECK-NEXT:             Option<unsafe extern "C-unwind" fn(i32) -> i32>,
// CHECK-NEXT:         >(unsafe { addresses.0[1] })
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %ld %d %d %s %d %d %d %d %d %d\n".as_ptr(),
// CHECK-NEXT:             *middle,
// CHECK-NEXT:             unsafe { end }
// CHECK-NEXT:                 .offset_from(
// CHECK-NEXT:                     (std::ptr::addr_of_mut!(values.0) as *mut i32) as *const i32,
// CHECK-NEXT:                 ) as i64,
// CHECK-NEXT:             *entries.0[0].value,
// CHECK-NEXT:             *entries.0[1].value,
// CHECK-NEXT:             entries.0[0].name,
// CHECK-NEXT:             callback.unwrap()(6i32),
// CHECK-NEXT:             external.unwrap()(-9i32),
// CHECK-NEXT:             *group.nested[0].value,
// CHECK-NEXT:             *group.tail,
// CHECK-NEXT:             *(addresses.0[0] as *mut i32),
// CHECK-NEXT:             local.unwrap()(4i32),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     !unsafe { group.nested[1].value }.is_null() as i32
// CHECK-NEXT: }
// CHECK-NEXT: static mut flag: i32 = 7;
// CHECK-NEXT: static mut addresses: __SlateAlign16<[*mut (); 2]> = __SlateAlign16([
// CHECK-NEXT:     (unsafe { std::ptr::addr_of_mut!(flag) }) as *mut (),
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             Option<unsafe extern "C-unwind" fn(i32) -> i32>,
// CHECK-NEXT:             *mut (),
// CHECK-NEXT:         >(Some(twice))
// CHECK-NEXT:     },
// CHECK-NEXT: ]);
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
