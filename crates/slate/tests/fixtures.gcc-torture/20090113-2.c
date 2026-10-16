struct obstack {};
struct bitmap_head_def;
typedef struct bitmap_head_def       *bitmap;
typedef const struct bitmap_head_def *const_bitmap;
typedef unsigned long                 BITMAP_WORD;
typedef struct bitmap_obstack {
  struct bitmap_element_def *elements;
  struct bitmap_head_def    *heads;
  struct obstack             obstack;
} bitmap_obstack;
typedef struct bitmap_element_def {
  struct bitmap_element_def *next;
  struct bitmap_element_def *prev;
  unsigned int               indx;
  BITMAP_WORD                bits[((128 + (8 * 8 * 1u) - 1) / (8 * 8 * 1u))];
} bitmap_element;

struct bitmap_descriptor;

typedef struct bitmap_head_def {
  bitmap_element *first;
  bitmap_element *current;
  unsigned int    indx;
  bitmap_obstack *obstack;
} bitmap_head;

bitmap_element bitmap_zero_bits;

typedef struct {
  bitmap_element *elt1;
  bitmap_element *elt2;
  unsigned        word_no;
  BITMAP_WORD     bits;
} bitmap_iterator;

static void __attribute__((noinline)) bmp_iter_set_init(bitmap_iterator *bi,
                                                        const_bitmap     map,
                                                        unsigned  start_bit,
                                                        unsigned *bit_no) {
  bi->elt1 = map->first;
  bi->elt2 = ((void *)0);

  while (1) {
    if (!bi->elt1) {
      bi->elt1 = &bitmap_zero_bits;
      break;
    }

    if (bi->elt1->indx >=
        start_bit / (((128 + (8 * 8 * 1u) - 1) / (8 * 8 * 1u)) * (8 * 8 * 1u)))
      break;
    bi->elt1 = bi->elt1->next;
  }

  if (bi->elt1->indx !=
      start_bit / (((128 + (8 * 8 * 1u) - 1) / (8 * 8 * 1u)) * (8 * 8 * 1u)))
    start_bit = bi->elt1->indx *
                (((128 + (8 * 8 * 1u) - 1) / (8 * 8 * 1u)) * (8 * 8 * 1u));

  bi->word_no =
      start_bit / (8 * 8 * 1u) % ((128 + (8 * 8 * 1u) - 1) / (8 * 8 * 1u));
  bi->bits   = bi->elt1->bits[bi->word_no];
  bi->bits >>= start_bit % (8 * 8 * 1u);

  start_bit += !bi->bits;

  *bit_no = start_bit;
}

static void __attribute__((noinline)) bmp_iter_next(bitmap_iterator *bi,
                                                    unsigned        *bit_no) {
  bi->bits >>= 1;
  *bit_no   += 1;
}

static unsigned char __attribute__((noinline))
bmp_iter_set_tail(bitmap_iterator *bi, unsigned *bit_no) {
  while (!(bi->bits & 1)) {
    bi->bits >>= 1;
    *bit_no   += 1;
  }
  return 1;
}

static __inline__ unsigned char bmp_iter_set(bitmap_iterator *bi,
                                             unsigned        *bit_no) {
  unsigned        bno  = *bit_no;
  BITMAP_WORD     bits = bi->bits;
  bitmap_element *elt1;

  if (bits) {
    while (!(bits & 1)) {
      bits >>= 1;
      bno   += 1;
    }
    *bit_no = bno;
    return 1;
  }

  *bit_no = ((bno + 64 - 1) / 64 * 64);
  bi->word_no++;

  elt1 = bi->elt1;
  while (1) {
    while (bi->word_no != 2) {
      bi->bits = elt1->bits[bi->word_no];
      if (bi->bits) {
        bi->elt1 = elt1;
        return bmp_iter_set_tail(bi, bit_no);
      }
      *bit_no += 64;
      bi->word_no++;
    }

    elt1 = elt1->next;
    if (!elt1) {
      bi->elt1 = elt1;
      return 0;
    }
    *bit_no     = elt1->indx * (2 * 64);
    bi->word_no = 0;
  }
}

extern void abort(void);

static void __attribute__((noinline)) catchme(int i) {
  if (i != 0 && i != 64)
    abort();
}
static void __attribute__((noinline)) foobar(bitmap_head *chain) {
  bitmap_iterator rsi;
  unsigned int    regno;
  for (bmp_iter_set_init(&(rsi), (chain), (0), &(regno));
       bmp_iter_set(&(rsi), &(regno)); bmp_iter_next(&(rsi), &(regno)))
    catchme(regno);
}

int main() {
  bitmap_element elem            = {(void *)0, (void *)0, 0, {1, 1}};
  bitmap_head    live_throughout = {&elem, &elem, 0, (void *)0};
  foobar(&live_throughout);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct obstack {}
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct bitmap_obstack {
// CHECK-NEXT:     elements: *mut bitmap_element_def,
// CHECK-NEXT:     heads: *mut bitmap_head_def,
// CHECK-NEXT:     obstack: obstack,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct bitmap_element_def {
// CHECK-NEXT:     next: *mut bitmap_element_def,
// CHECK-NEXT:     prev: *mut bitmap_element_def,
// CHECK-NEXT:     indx: u32,
// CHECK-NEXT:     bits: [u64; 2],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct bitmap_head_def {
// CHECK-NEXT:     first: *mut bitmap_element_def,
// CHECK-NEXT:     current: *mut bitmap_element_def,
// CHECK-NEXT:     indx: u32,
// CHECK-NEXT:     obstack: *mut bitmap_obstack,
// CHECK-NEXT: }
// CHECK-NEXT: static mut bitmap_zero_bits: bitmap_element_def = unsafe { std::mem::zeroed() };
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     elt1: *mut bitmap_element_def,
// CHECK-NEXT:     elt2: *mut bitmap_element_def,
// CHECK-NEXT:     word_no: u32,
// CHECK-NEXT:     bits: u64,
// CHECK-NEXT: }
// CHECK-NEXT: fn bmp_iter_set_init(
// CHECK-NEXT:     mut bi: &mut [[SLATERECORD_0]],
// CHECK-NEXT:     mut map: *const bitmap_head_def,
// CHECK-NEXT:     mut start_bit: u32,
// CHECK-NEXT:     mut bit_no: &mut u32,
// CHECK-NEXT: ) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(bi as *mut [[SLATERECORD_0]])).elt1 = unsafe { (*map).first };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(bi as *mut [[SLATERECORD_0]])).elt2 = std::ptr::null_mut::<
// CHECK-NEXT:             bitmap_element_def,
// CHECK-NEXT:         >();
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (1 as i32) != (0 as i32) {
// CHECK-NEXT:         if !((unsafe { (*(bi as *mut [[SLATERECORD_0]])).elt1 })
// CHECK-NEXT:             != std::ptr::null_mut::<bitmap_element_def>())
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*(bi as *mut [[SLATERECORD_0]])).elt1 = unsafe {
// CHECK-NEXT:                     std::ptr::addr_of_mut!(bitmap_zero_bits)
// CHECK-NEXT:                 };
// CHECK-NEXT:             }
// CHECK-NEXT:             break '[[SLATE_BREAK_0]];
// CHECK-NEXT:         }
// CHECK-NEXT:         if (unsafe { (*unsafe { (*(bi as *mut [[SLATERECORD_0]])).elt1 }).indx })
// CHECK-NEXT:             >= start_bit
// CHECK-NEXT:                 / (((128 as i32) as u32)
// CHECK-NEXT:                     .wrapping_add(
// CHECK-NEXT:                         (((8 as i32) * (8 as i32)) as u32).wrapping_mul(1 as u32),
// CHECK-NEXT:                     )
// CHECK-NEXT:                     .wrapping_sub((1 as i32) as u32)
// CHECK-NEXT:                     / (((8 as i32) * (8 as i32)) as u32).wrapping_mul(1 as u32))
// CHECK-NEXT:                     .wrapping_mul(
// CHECK-NEXT:                         (((8 as i32) * (8 as i32)) as u32).wrapping_mul(1 as u32),
// CHECK-NEXT:                     )
// CHECK-NEXT:         {
// CHECK-NEXT:             break '[[SLATE_BREAK_0]];
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*(bi as *mut [[SLATERECORD_0]])).elt1 = unsafe {
// CHECK-NEXT:                 (*unsafe { (*(bi as *mut [[SLATERECORD_0]])).elt1 }).next
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { (*unsafe { (*(bi as *mut [[SLATERECORD_0]])).elt1 }).indx })
// CHECK-NEXT:         != start_bit
// CHECK-NEXT:             / (((128 as i32) as u32)
// CHECK-NEXT:                 .wrapping_add((((8 as i32) * (8 as i32)) as u32).wrapping_mul(1 as u32))
// CHECK-NEXT:                 .wrapping_sub((1 as i32) as u32)
// CHECK-NEXT:                 / (((8 as i32) * (8 as i32)) as u32).wrapping_mul(1 as u32))
// CHECK-NEXT:                 .wrapping_mul((((8 as i32) * (8 as i32)) as u32).wrapping_mul(1 as u32))
// CHECK-NEXT:     {
// CHECK-NEXT:         start_bit = unsafe { (*unsafe { (*(bi as *mut [[SLATERECORD_0]])).elt1 }).indx }
// CHECK-NEXT:             .wrapping_mul(
// CHECK-NEXT:                 (((128 as i32) as u32)
// CHECK-NEXT:                     .wrapping_add(
// CHECK-NEXT:                         (((8 as i32) * (8 as i32)) as u32).wrapping_mul(1 as u32),
// CHECK-NEXT:                     )
// CHECK-NEXT:                     .wrapping_sub((1 as i32) as u32)
// CHECK-NEXT:                     / (((8 as i32) * (8 as i32)) as u32).wrapping_mul(1 as u32))
// CHECK-NEXT:                     .wrapping_mul(
// CHECK-NEXT:                         (((8 as i32) * (8 as i32)) as u32).wrapping_mul(1 as u32),
// CHECK-NEXT:                     ),
// CHECK-NEXT:             );
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(bi as *mut [[SLATERECORD_0]])).word_no = start_bit
// CHECK-NEXT:             / (((8 as i32) * (8 as i32)) as u32).wrapping_mul(1 as u32)
// CHECK-NEXT:             % (((128 as i32) as u32)
// CHECK-NEXT:                 .wrapping_add((((8 as i32) * (8 as i32)) as u32).wrapping_mul(1 as u32))
// CHECK-NEXT:                 .wrapping_sub((1 as i32) as u32)
// CHECK-NEXT:                 / (((8 as i32) * (8 as i32)) as u32).wrapping_mul(1 as u32));
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(bi as *mut [[SLATERECORD_0]])).bits = unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe { (*(bi as *mut [[SLATERECORD_0]])).elt1 }).bits.as_mut_ptr()
// CHECK-NEXT:                         as *mut u64
// CHECK-NEXT:                 }
// CHECK-NEXT:                     .offset(
// CHECK-NEXT:                         (unsafe { (*(bi as *mut [[SLATERECORD_0]])).word_no }) as isize,
// CHECK-NEXT:                     )
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: u64 = unsafe { (*bi).bits };
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: u64 = [[V_0]]
// CHECK-NEXT:         >> start_bit % (((8 as i32) * (8 as i32)) as u32).wrapping_mul(1 as u32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*bi).bits = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: u32 = start_bit;
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: u32 = [[V_2]]
// CHECK-NEXT:         .wrapping_add(
// CHECK-NEXT:             (!((unsafe { (*(bi as *mut [[SLATERECORD_0]])).bits }) != (0 as u64)) as i32)
// CHECK-NEXT:                 as u32,
// CHECK-NEXT:         );
// CHECK-NEXT:     start_bit = [[V_3]];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(bit_no as *mut u32) = start_bit;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn bmp_iter_next(mut bi: &mut [[SLATERECORD_0]], mut bit_no: &mut u32) {
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: u64 = unsafe { (*bi).bits };
// CHECK-NEXT:     let mut [[V_5:__v[0-9]+]]: u64 = [[V_4]] >> (1 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*bi).bits = [[V_5]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_6:__v[0-9]+]]: u32 = unsafe { *bit_no };
// CHECK-NEXT:     let mut [[V_7:__v[0-9]+]]: u32 = [[V_6]].wrapping_add((1 as i32) as u32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *bit_no = [[V_7]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn bmp_iter_set_tail(mut bi: &mut [[SLATERECORD_0]], mut bit_no: &mut u32) -> u8 {
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while !((unsafe { (*(bi as *mut [[SLATERECORD_0]])).bits })
// CHECK-NEXT:         & (((1 as i32) as i64) as u64) != (0 as u64))
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut [[V_8:__v[0-9]+]]: u64 = unsafe { (*bi).bits };
// CHECK-NEXT:         let mut [[V_9:__v[0-9]+]]: u64 = [[V_8]] >> (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*bi).bits = [[V_9]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_10:__v[0-9]+]]: u32 = unsafe { *bit_no };
// CHECK-NEXT:         let mut [[V_11:__v[0-9]+]]: u32 = [[V_10]].wrapping_add((1 as i32) as u32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *bit_no = [[V_11]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     ((1 as i32) as i8) as u8
// CHECK-NEXT: }
// CHECK-NEXT: fn bmp_iter_set(mut bi: &mut [[SLATERECORD_0]], mut bit_no: &mut u32) -> u8 {
// CHECK-NEXT:     let mut bno: u32 = unsafe { *(bit_no as *mut u32) };
// CHECK-NEXT:     let mut bits: u64 = unsafe { (*(bi as *mut [[SLATERECORD_0]])).bits };
// CHECK-NEXT:     let mut elt1: *mut bitmap_element_def = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     if bits != (0 as u64) {
// CHECK-NEXT:         '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while !(bits & (((1 as i32) as i64) as u64) != (0 as u64)) {
// CHECK-NEXT:             let mut [[V_12:__v[0-9]+]]: u64 = bits;
// CHECK-NEXT:             let mut [[V_13:__v[0-9]+]]: u64 = [[V_12]] >> (1 as i32);
// CHECK-NEXT:             bits = [[V_13]];
// CHECK-NEXT:             let mut [[V_14:__v[0-9]+]]: u32 = bno;
// CHECK-NEXT:             let mut [[V_15:__v[0-9]+]]: u32 = [[V_14]].wrapping_add((1 as i32) as u32);
// CHECK-NEXT:             bno = [[V_15]];
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *(bit_no as *mut u32) = bno;
// CHECK-NEXT:         }
// CHECK-NEXT:         return ((1 as i32) as i8) as u8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(bit_no as *mut u32) = (bno
// CHECK-NEXT:             .wrapping_add((64 as i32) as u32)
// CHECK-NEXT:             .wrapping_sub((1 as i32) as u32) / ((64 as i32) as u32))
// CHECK-NEXT:             .wrapping_mul((64 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_16:__v[0-9]+]]: u32 = unsafe { (*bi).word_no };
// CHECK-NEXT:     let mut [[V_17:__v[0-9]+]]: u32 = [[V_16]].wrapping_add((1 as i32) as u32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*bi).word_no = [[V_17]];
// CHECK-NEXT:     }
// CHECK-NEXT:     elt1 = unsafe { (*(bi as *mut [[SLATERECORD_0]])).elt1 };
// CHECK-NEXT:     '[[SLATE_BREAK_3:__slate_break_[0-9]+]]: while (1 as i32) != (0 as i32) {
// CHECK-NEXT:         '[[SLATE_BREAK_4:__slate_break_[0-9]+]]: while (unsafe { (*(bi as *mut [[SLATERECORD_0]])).word_no })
// CHECK-NEXT:             != ((2 as i32) as u32)
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*(bi as *mut [[SLATERECORD_0]])).bits = unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         unsafe { (*elt1).bits.as_mut_ptr() as *mut u64 }
// CHECK-NEXT:                             .offset(
// CHECK-NEXT:                                 (unsafe { (*(bi as *mut [[SLATERECORD_0]])).word_no })
// CHECK-NEXT:                                     as isize,
// CHECK-NEXT:                             )
// CHECK-NEXT:                     }
// CHECK-NEXT:                 };
// CHECK-NEXT:             }
// CHECK-NEXT:             if (unsafe { (*(bi as *mut [[SLATERECORD_0]])).bits }) != (0 as u64) {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*(bi as *mut [[SLATERECORD_0]])).elt1 = elt1;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 return bmp_iter_set_tail(bi, bit_no);
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_18:__v[0-9]+]]: u32 = unsafe { *bit_no };
// CHECK-NEXT:             let mut [[V_19:__v[0-9]+]]: u32 = [[V_18]].wrapping_add((64 as i32) as u32);
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *bit_no = [[V_19]];
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_20:__v[0-9]+]]: u32 = unsafe { (*bi).word_no };
// CHECK-NEXT:             let mut [[V_21:__v[0-9]+]]: u32 = [[V_20]].wrapping_add((1 as i32) as u32);
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*bi).word_no = [[V_21]];
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         elt1 = unsafe { (*elt1).next };
// CHECK-NEXT:         if !(elt1 != std::ptr::null_mut::<bitmap_element_def>()) {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*(bi as *mut [[SLATERECORD_0]])).elt1 = elt1;
// CHECK-NEXT:             }
// CHECK-NEXT:             return ((0 as i32) as i8) as u8;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *(bit_no as *mut u32) = unsafe { (*elt1).indx }
// CHECK-NEXT:                 .wrapping_mul(((2 as i32) * (64 as i32)) as u32);
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*(bi as *mut [[SLATERECORD_0]])).word_no = (0 as i32) as u32;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-NEXT: fn catchme(mut i: i32) {
// CHECK-NEXT:     if i != (0 as i32) && i != (64 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn foobar(mut chain: &bitmap_head_def) {
// CHECK-NEXT:     let mut rsi: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut regno: u32 = 0 as u32;
// CHECK-NEXT:     bmp_iter_set_init(
// CHECK-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(rsi)) },
// CHECK-NEXT:         chain as *const bitmap_head_def,
// CHECK-NEXT:         (0 as i32) as u32,
// CHECK-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(regno)) },
// CHECK-NEXT:     );
// CHECK-NEXT:     '[[SLATE_BREAK_5:__slate_break_[0-9]+]]: while bmp_iter_set(
// CHECK-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(rsi)) },
// CHECK-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(regno)) },
// CHECK-NEXT:     ) != (0 as u8)
// CHECK-NEXT:     {
// CHECK-NEXT:         catchme(regno as i32);
// CHECK-NEXT:         bmp_iter_next(
// CHECK-NEXT:             unsafe { &mut (*std::ptr::addr_of_mut!(rsi)) },
// CHECK-NEXT:             unsafe { &mut (*std::ptr::addr_of_mut!(regno)) },
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut elem: bitmap_element_def = bitmap_element_def {
// CHECK-NEXT:         next: std::ptr::null_mut::<bitmap_element_def>(),
// CHECK-NEXT:         prev: std::ptr::null_mut::<bitmap_element_def>(),
// CHECK-NEXT:         indx: (0 as i32) as u32,
// CHECK-NEXT:         bits: [((1 as i32) as i64) as u64, ((1 as i32) as i64) as u64],
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut live_throughout: bitmap_head_def = bitmap_head_def {
// CHECK-NEXT:         first: std::ptr::addr_of_mut!(elem),
// CHECK-NEXT:         current: std::ptr::addr_of_mut!(elem),
// CHECK-NEXT:         indx: (0 as i32) as u32,
// CHECK-NEXT:         obstack: std::ptr::null_mut::<bitmap_obstack>(),
// CHECK-NEXT:     };
// CHECK-NEXT:     foobar(unsafe { &(*std::ptr::addr_of_mut!(live_throughout)) });
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
