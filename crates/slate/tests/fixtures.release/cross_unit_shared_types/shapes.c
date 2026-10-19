#include <stdlib.h>
#include "shapes.h"

struct registry_stats {
    int adds;
};

struct registry {
    struct shape *head;
    int count;
    struct registry_stats stats;
};

struct registry *registry_new(void) {
    struct registry *registry = malloc(sizeof *registry);
    registry->head = 0;
    registry->count = 0;
    registry->stats.adds = 0;
    return registry;
}

void registry_add(struct registry *registry, struct shape *shape) {
    shape->next = registry->head;
    registry->head = shape;
    registry->count++;
    registry->stats.adds++;
}

int registry_count(const struct registry *registry) { return registry->count + registry->stats.adds; }

void registry_free(struct registry *registry) { free(registry); }

void shape_move(struct shape *shape, int dx, int dy) {
    shape->origin.x += dx;
    shape->origin.y += dy;
    shape->flags.level = (shape->flags.level + 1) & 15;
}

int shape_area_sum(const struct shape *shapes, int count) {
    int sum = 0;
    for (int i = 0; i < count; i++) {
        sum += shapes[i].origin.x * shapes[i].origin.y;
    }
    return sum;
}

int tag_score(const tag_t *tag) { return tag->weight * 2; }

int count_vowels(const char *text) {
    int count = 0;
    for (int i = 0; text[i]; i++) {
        char c = text[i];
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            count++;
        }
    }
    return count;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}use crate::__slate_types::[[SLATERECORD_0:__SlateRecord[0-9]+]] as [[SLATERECORD_1:__SlateRecord[0-9]+]];
// CHECK-NEXT: use crate::__slate_types::[[SLATERECORD_2:__SlateRecord[0-9]+]] as [[SLATERECORD_3:__SlateRecord[0-9]+]];
// CHECK-NEXT: use crate::__slate_types::flags;
// CHECK-NEXT: use crate::__slate_types::registry;
// CHECK-NEXT: use crate::__slate_types::registry_stats;
// CHECK-NEXT: use crate::__slate_types::shape;
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn malloc(__size: u64) -> *mut ();
// CHECK-NEXT:     fn free(__ptr: *mut ());
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: pub fn registry_new() -> *mut registry {
// CHECK-NEXT:     let registry: *mut registry = (unsafe { malloc(16 as u64) }) as *mut registry;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*registry).head = std::ptr::null_mut::<shape>();
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*registry).count = 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*registry).stats.adds = 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     registry
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: pub fn registry_add(registry: *mut registry, shape: *mut shape) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*shape).next = (*registry).head;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*registry).head = shape;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*registry).count = (*registry).count + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*registry).stats.adds = (*registry).stats.adds + 1;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: pub fn registry_count(registry: *const registry) -> i32 {
// CHECK-NEXT:     (unsafe { (*registry).count }) + unsafe { (*registry).stats.adds }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: pub fn registry_free(registry: *mut registry) {
// CHECK-NEXT:     unsafe { free(registry as *mut ()) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: pub fn shape_move(shape: *mut shape, dx: i32, dy: i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*shape).origin.x = (*shape).origin.x + dx;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*shape).origin.y = (*shape).origin.y + dy;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]] = (((*shape).flags.__slate_bits_0.__get_level() as i32) + 1 & 15) as u32;
// CHECK-NEXT:         (*shape).flags.__slate_bits_0.__set_level([[T_0]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: pub fn shape_area_sum(shapes: &[shape]) -> i32 {
// CHECK-NEXT:     let mut sum: i32 = 0;
// CHECK-NEXT:     let mut i: i32 = 0;
// CHECK-NEXT:     while i < (shapes.len() as i32) {
// CHECK-NEXT:         sum += (unsafe { shapes[i as usize].origin.x }) * unsafe { shapes[i as usize].origin.y };
// CHECK-NEXT:         i += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     sum
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: pub fn tag_score(tag: &[[SLATERECORD_3]]) -> i32 {
// CHECK-NEXT:     (unsafe { (*(tag as *const [[SLATERECORD_3]])).weight }) * 2
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: pub fn count_vowels(text: *const i8) -> i32 {
// CHECK-NEXT:     let mut count: i32 = 0;
// CHECK-NEXT:     let mut i: i32 = 0;
// CHECK-NEXT:     while (unsafe { *text.offset(i as isize) }) != 0 {
// CHECK-NEXT:         let c: i8 = unsafe { *text.offset(i as isize) };
// CHECK-NEXT:         if (c as i32) == 97
// CHECK-NEXT:             || (c as i32) == 101
// CHECK-NEXT:             || (c as i32) == 105
// CHECK-NEXT:             || (c as i32) == 111
// CHECK-NEXT:             || (c as i32) == 117
// CHECK-NEXT:         {
// CHECK-NEXT:             count += 1;
// CHECK-NEXT:         }
// CHECK-NEXT:         i += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     count
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
