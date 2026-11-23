#include <stdio.h>
#include <stdlib.h>

typedef int v2 __attribute__((ext_vector_type(2)));
typedef int v3 __attribute__((ext_vector_type(3)));
typedef int v4 __attribute__((ext_vector_type(4)));

struct aligned {
  int tag;
  v4 value;
};

struct __attribute__((packed)) packed {
  char tag;
  v4 value;
};

static int bases;
static int operands;

static struct packed *pick_base(struct packed *p) {
  ++bases;
  return p;
}

static v2 pick_operand(void) {
  ++operands;
  return (v2){31, 32};
}

// @check-fn-begin
static void update_aligned(struct aligned *p) {
  p->value.zyx.yz = p->value.xy;
  p->value.zyx += (v3){4, 5, 6};
  p->value.zyx[0] += 10;
}
// @check-fn-end

// @check-fn-begin
static void update_packed(struct packed *p) {
  pick_base(p)->value.yx = pick_operand();
  pick_base(p)->value.wz = p->value.xy + (v2){10, 20};
  pick_base(p)->value.wz += pick_operand();
}
// @check-fn-end

int main(void) {
  struct aligned a = {7, {1, 2, 3, 4}};
  update_aligned(&a);
  v3 part = a.value.zyx;
  printf("%d %d %d %d %d\n", a.tag, part[0], part[1], part[2], a.value[3]);

  struct packed *p = malloc(sizeof(*p));
  if (!p)
    return 1;
  p->tag = 8;
  update_packed(p);
  v2 first = pick_base(p)->value.xy;
  v2 last = p->value.zw;
  int index = 1;
  int selected = p->value.wz[index++];
  printf("%d %d %d %d %d %d %d %d %d\n", p->tag, first[0], first[1], last[0],
         last[1], bases, operands, selected, index);
  free(p);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn update_aligned(p: &mut aligned) {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: __slate_vector::__SlateVector<i32, 3, __slate_vector::Align16> = unsafe {
// CHECK-NEXT:         core::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[T_0:__t[0-9]+]] = (core::ptr::addr_of!((*p).value) as *const core::simd::Simd<i32, 4>)
// CHECK-NEXT:         as *const i32;
// CHECK-NEXT:     let [[T_1:__t[0-9]+]]: core::simd::Simd<i32, 2> = core::simd::Simd::from_array(
// CHECK-NEXT:         unsafe { [[[T_0]].add(0).read_unaligned(), [[T_0]].add(1).read_unaligned()] },
// CHECK-NEXT:     );
// CHECK-NEXT:     let [[T_2:__t[0-9]+]] = (core::ptr::addr_of_mut!((*p).value) as *mut core::simd::Simd<i32, 4>)
// CHECK-NEXT:         as *mut i32;
// CHECK-NEXT:     unsafe { [[T_2]].add(1).write_unaligned([[T_1]][0]) };
// CHECK-NEXT:     unsafe { [[T_2]].add(0).write_unaligned([[T_1]][1]) };
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: *mut aligned = p as *mut aligned;
// CHECK-NEXT:     let [[T_3:__t[0-9]+]] = ({
// CHECK-NEXT:         let [[T_4:__t[0-9]+]] = unsafe {
// CHECK-NEXT:             (core::ptr::addr_of!((*[[V_0]]).value) as *const core::simd::Simd<i32, 4>)
// CHECK-NEXT:                 as *const i32
// CHECK-NEXT:         };
// CHECK-NEXT:         let [[T_5:__t[0-9]+]]: __slate_vector::__SlateVector<i32, 3, __slate_vector::Align16> = __slate_vector::__SlateVector::from_array(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 [
// CHECK-NEXT:                     [[T_4]].add(2).read_unaligned(),
// CHECK-NEXT:                     [[T_4]].add(1).read_unaligned(),
// CHECK-NEXT:                     [[T_4]].add(0).read_unaligned(),
// CHECK-NEXT:                 ]
// CHECK-NEXT:             },
// CHECK-NEXT:         );
// CHECK-NEXT:         [[T_5]]
// CHECK-NEXT:     })
// CHECK-NEXT:         + unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_0]] = __slate_vector::__SlateVector::from_array(
// CHECK-NEXT:                     [4i32, 5i32, 6i32],
// CHECK-NEXT:                 );
// CHECK-NEXT:                 core::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     let [[T_6:__t[0-9]+]] = unsafe {
// CHECK-NEXT:         (core::ptr::addr_of_mut!((*[[V_0]]).value) as *mut core::simd::Simd<i32, 4>)
// CHECK-NEXT:             as *mut i32
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { [[T_6]].add(2).write_unaligned([[T_3]][0]) };
// CHECK-NEXT:     unsafe { [[T_6]].add(1).write_unaligned([[T_3]][1]) };
// CHECK-NEXT:     unsafe { [[T_6]].add(0).write_unaligned([[T_3]][2]) };
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: *mut aligned = p as *mut aligned;
// CHECK-NEXT:     let [[V_2:__v[0-9]+]]: i32 = 0;
// CHECK-NEXT:     let [[T_7:__t[0-9]+]] = ({
// CHECK-NEXT:         let [[T_8:__t[0-9]+]] = unsafe {
// CHECK-NEXT:             (core::ptr::addr_of!((*[[V_1]]).value) as *const core::simd::Simd<i32, 4>)
// CHECK-NEXT:                 as *const i32
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { [[T_8]].add([2, 1, 0][[[V_2]] as usize]).read_unaligned() }
// CHECK-NEXT:     }) + 10;
// CHECK-NEXT:     let [[T_9:__t[0-9]+]] = unsafe {
// CHECK-NEXT:         (core::ptr::addr_of_mut!((*[[V_1]]).value) as *mut core::simd::Simd<i32, 4>)
// CHECK-NEXT:             as *mut i32
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { [[T_9]].add([2, 1, 0][[[V_2]] as usize]).write_unaligned([[T_7]]) };
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn update_packed(p: *mut packed) {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: core::simd::Simd<i32, 2> = unsafe {
// CHECK-NEXT:         core::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[T_0:__t[0-9]+]] = pick_operand();
// CHECK-NEXT:     let [[T_1:__t[0-9]+]] = unsafe {
// CHECK-NEXT:         (core::ptr::addr_of_mut!((*pick_base(p)).value) as *mut core::simd::Simd<i32, 4>)
// CHECK-NEXT:             as *mut i32
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { [[T_1]].add(1).write_unaligned([[T_0]][0]) };
// CHECK-NEXT:     unsafe { [[T_1]].add(0).write_unaligned([[T_0]][1]) };
// CHECK-NEXT:     let [[T_2:__t[0-9]+]] = ({
// CHECK-NEXT:         let [[T_3:__t[0-9]+]] = unsafe {
// CHECK-NEXT:             (core::ptr::addr_of!((*p).value) as *const core::simd::Simd<i32, 4>)
// CHECK-NEXT:                 as *const i32
// CHECK-NEXT:         };
// CHECK-NEXT:         let [[T_4:__t[0-9]+]]: core::simd::Simd<i32, 2> = core::simd::Simd::from_array(
// CHECK-NEXT:             unsafe { [[[T_3]].add(0).read_unaligned(), [[T_3]].add(1).read_unaligned()] },
// CHECK-NEXT:         );
// CHECK-NEXT:         [[T_4]]
// CHECK-NEXT:     })
// CHECK-NEXT:         + unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_0]] = core::simd::Simd::from_array([10i32, 20i32]);
// CHECK-NEXT:                 core::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     let [[T_5:__t[0-9]+]] = unsafe {
// CHECK-NEXT:         (core::ptr::addr_of_mut!((*pick_base(p)).value) as *mut core::simd::Simd<i32, 4>)
// CHECK-NEXT:             as *mut i32
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { [[T_5]].add(3).write_unaligned([[T_2]][0]) };
// CHECK-NEXT:     unsafe { [[T_5]].add(2).write_unaligned([[T_2]][1]) };
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: core::simd::Simd<i32, 2> = pick_operand();
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: *mut packed = pick_base(p);
// CHECK-NEXT:     let [[T_6:__t[0-9]+]] = ({
// CHECK-NEXT:         let [[T_7:__t[0-9]+]] = unsafe {
// CHECK-NEXT:             (core::ptr::addr_of!((*[[V_1]]).value) as *const core::simd::Simd<i32, 4>)
// CHECK-NEXT:                 as *const i32
// CHECK-NEXT:         };
// CHECK-NEXT:         let [[T_8:__t[0-9]+]]: core::simd::Simd<i32, 2> = core::simd::Simd::from_array(
// CHECK-NEXT:             unsafe { [[[T_7]].add(3).read_unaligned(), [[T_7]].add(2).read_unaligned()] },
// CHECK-NEXT:         );
// CHECK-NEXT:         [[T_8]]
// CHECK-NEXT:     }) + [[V_0]];
// CHECK-NEXT:     let [[T_9:__t[0-9]+]] = unsafe {
// CHECK-NEXT:         (core::ptr::addr_of_mut!((*[[V_1]]).value) as *mut core::simd::Simd<i32, 4>)
// CHECK-NEXT:             as *mut i32
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { [[T_9]].add(3).write_unaligned([[T_6]][0]) };
// CHECK-NEXT:     unsafe { [[T_9]].add(2).write_unaligned([[T_6]][1]) };
// CHECK: {{^}}}
// SLATE-FILECHECK-END
