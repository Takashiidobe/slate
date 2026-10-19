#include <stdio.h>
#include "shapes.h"

int main(void) {
    struct shape shapes[3] = {
        {0, {1, 2}, {1, 0, 10}},
        {0, {3, 4}, {0, 2, 11}},
        {0, {5, 6}, {1, 3, 12}},
    };
    struct registry *registry = registry_new();
    for (int i = 0; i < 3; i++) {
        shape_move(&shapes[i], i, 1);
        registry_add(registry, &shapes[i]);
    }
    tag_t tag = {"heavy", 21};
    printf("%d %d\n", registry_count(registry), shape_area_sum(shapes, 3));
    printf("%u %u %d\n", shapes[2].flags.ready, shapes[2].flags.level, shapes[2].flags.id);
    printf("%d %d\n", tag_score(&tag), count_vowels("translation unit"));
    registry_free(registry);
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: mod __slate_types;
// CHECK-NEXT: #[path = "shapes.rs"]
// CHECK-NEXT: mod __slate_unit_shapes;
// CHECK-EMPTY:
// CHECK-NEXT: use crate::__slate_types::[[SLATERECORD_0:__SlateRecord[0-9]+]] as [[SLATERECORD_1:__SlateRecord[0-9]+]];
// CHECK-NEXT: use crate::__slate_types::[[SLATERECORD_2:__SlateRecord[0-9]+]] as [[SLATERECORD_3:__SlateRecord[0-9]+]];
// CHECK-NEXT: use crate::__slate_types::flags;
// CHECK-NEXT: use crate::__slate_types::registry;
// CHECK-NEXT: use crate::__slate_types::shape;
// CHECK-NEXT: use crate::__slate_unit_shapes::count_vowels;
// CHECK-NEXT: use crate::__slate_unit_shapes::registry_add;
// CHECK-NEXT: use crate::__slate_unit_shapes::registry_count;
// CHECK-NEXT: use crate::__slate_unit_shapes::registry_free;
// CHECK-NEXT: use crate::__slate_unit_shapes::registry_new;
// CHECK-NEXT: use crate::__slate_unit_shapes::shape_area_sum;
// CHECK-NEXT: use crate::__slate_unit_shapes::shape_move;
// CHECK-NEXT: use crate::__slate_unit_shapes::tag_score;
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut shapes: __SlateAlign16<[shape; 3]> = __SlateAlign16([
// CHECK-NEXT:         shape {
// CHECK-NEXT:             next: std::ptr::null_mut::<shape>(),
// CHECK-NEXT:             origin: [[SLATERECORD_1]] { x: 1, y: 2 },
// CHECK-NEXT:             flags: {
// CHECK-NEXT:                 let mut [[T_0:__t[0-9]+]]: flags = unsafe { std::mem::zeroed() };
// CHECK-NEXT:                 [[T_0]].__slate_bits_0.__set_ready(1u32);
// CHECK-NEXT:                 [[T_0]].__slate_bits_0.__set_level(0u32);
// CHECK-NEXT:                 [[T_0]].id = 10i32;
// CHECK-NEXT:                 [[T_0]]
// CHECK-NEXT:             },
// CHECK-NEXT:         },
// CHECK-NEXT:         shape {
// CHECK-NEXT:             next: std::ptr::null_mut::<shape>(),
// CHECK-NEXT:             origin: [[SLATERECORD_1]] { x: 3, y: 4 },
// CHECK-NEXT:             flags: {
// CHECK-NEXT:                 let mut [[T_1:__t[0-9]+]]: flags = unsafe { std::mem::zeroed() };
// CHECK-NEXT:                 [[T_1]].__slate_bits_0.__set_ready(0u32);
// CHECK-NEXT:                 [[T_1]].__slate_bits_0.__set_level(2u32);
// CHECK-NEXT:                 [[T_1]].id = 11i32;
// CHECK-NEXT:                 [[T_1]]
// CHECK-NEXT:             },
// CHECK-NEXT:         },
// CHECK-NEXT:         shape {
// CHECK-NEXT:             next: std::ptr::null_mut::<shape>(),
// CHECK-NEXT:             origin: [[SLATERECORD_1]] { x: 5, y: 6 },
// CHECK-NEXT:             flags: {
// CHECK-NEXT:                 let mut [[T_2:__t[0-9]+]]: flags = unsafe { std::mem::zeroed() };
// CHECK-NEXT:                 [[T_2]].__slate_bits_0.__set_ready(1u32);
// CHECK-NEXT:                 [[T_2]].__slate_bits_0.__set_level(3u32);
// CHECK-NEXT:                 [[T_2]].id = 12i32;
// CHECK-NEXT:                 [[T_2]]
// CHECK-NEXT:             },
// CHECK-NEXT:         },
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let registry: *mut registry = unsafe { registry_new() };
// CHECK-NEXT:     for i in 0i32..3i32 {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             shape_move(
// CHECK-NEXT:                 (shapes.0.as_mut_ptr() as *mut shape).offset(i as isize),
// CHECK-NEXT:                 i,
// CHECK-NEXT:                 1i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             registry_add(
// CHECK-NEXT:                 registry,
// CHECK-NEXT:                 (shapes.0.as_mut_ptr() as *mut shape).offset(i as isize),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut tag: [[SLATERECORD_3]] = {
// CHECK-NEXT:         let mut [[T_3:__t[0-9]+]]: [[SLATERECORD_3]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_3]].label = c"heavy".as_ptr();
// CHECK-NEXT:         [[T_3]].weight = 21i32;
// CHECK-NEXT:         [[T_3]]
// CHECK-NEXT:     };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {}",
// CHECK-NEXT:         unsafe { registry_count(registry as *const registry) },
// CHECK-NEXT:         unsafe { shape_area_sum(&shapes.0) }
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%u %u %d\n".as_ptr(),
// CHECK-NEXT:             shapes.0[2].flags.__slate_bits_0.__get_ready() as i32,
// CHECK-NEXT:             shapes.0[2].flags.__slate_bits_0.__get_level() as i32,
// CHECK-NEXT:             shapes.0[2].flags.id,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {}",
// CHECK-NEXT:         unsafe { tag_score(&(*(std::ptr::addr_of_mut!(tag) as *const [[SLATERECORD_3]]))) },
// CHECK-NEXT:         unsafe { count_vowels(c"translation unit".as_ptr()) }
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe { registry_free(registry) };
// CHECK-NEXT:     0
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
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
