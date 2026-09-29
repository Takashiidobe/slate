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
  BITMAP_WORD                bits[(2)];
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

static __attribute__((noinline)) void bmp_iter_set_init(bitmap_iterator *bi,
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

    if (bi->elt1->indx >= start_bit / (128u))
      break;
    bi->elt1 = bi->elt1->next;
  }

  if (bi->elt1->indx != start_bit / (128u))
    start_bit = bi->elt1->indx * (128u);

  bi->word_no   = start_bit / 64u % (2);
  bi->bits      = bi->elt1->bits[bi->word_no];
  bi->bits    >>= start_bit % 64u;

  start_bit += !bi->bits;

  *bit_no = start_bit;
}

static __inline__ __attribute__((always_inline)) void
bmp_iter_next(bitmap_iterator *bi, unsigned *bit_no) {
  bi->bits >>= 1;
  *bit_no   += 1;
}

static __inline__ __attribute__((always_inline)) unsigned char
bmp_iter_set(bitmap_iterator *bi, unsigned *bit_no) {
  if (bi->bits) {
    while (!(bi->bits & 1)) {
      bi->bits >>= 1;
      *bit_no   += 1;
    }
    return 1;
  }

  *bit_no = ((*bit_no + 64u - 1) / 64u * 64u);
  bi->word_no++;

  while (1) {
    while (bi->word_no != (2)) {
      bi->bits = bi->elt1->bits[bi->word_no];
      if (bi->bits) {
        while (!(bi->bits & 1)) {
          bi->bits >>= 1;
          *bit_no   += 1;
        }
        return 1;
      }
      *bit_no += 64u;
      bi->word_no++;
    }

    bi->elt1 = bi->elt1->next;
    if (!bi->elt1)
      return 0;
    *bit_no     = bi->elt1->indx * (128u);
    bi->word_no = 0;
  }
}

static void __attribute__((noinline)) foobar(bitmap_head *live_throughout) {
  bitmap_iterator rsi;
  unsigned int    regno;
  for (bmp_iter_set_init(&(rsi), (live_throughout), (0), &(regno));
       bmp_iter_set(&(rsi), &(regno)); bmp_iter_next(&(rsi), &(regno)))
    ;
}
int main() {
  bitmap_element elem            = {(void *)0, (void *)0, 0, {1, 1}};
  bitmap_head    live_throughout = {&elem, &elem, 0, (void *)0};
  foobar(&live_throughout);
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
// DEFAULT-NEXT:     type @type[[TYPE_obstack:[0-9]+]] obstack = struct {
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[]];
// DEFAULT-NEXT:     type @type[[TYPE_bitmap_head_def:[0-9]+]] bitmap_head_def = struct {
// DEFAULT-NEXT:         field0 first: ptr<@type[[TYPE_bitmap_element_def:[0-9]+]]>;
// DEFAULT-NEXT:         field1 current: ptr<@type[[TYPE_bitmap_element_def]]>;
// DEFAULT-NEXT:         field2 indx: u32;
// DEFAULT-NEXT:         field3 obstack: ptr<@type[[TYPE_bitmap_obstack:[0-9]+]]>;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 16, 24]];
// DEFAULT-NEXT:     type @type[[TYPE_bitmap:[0-9]+]] bitmap = ptr<@type[[TYPE_bitmap_head_def]]>;
// DEFAULT-NEXT:     type @type[[TYPE_const_bitmap:[0-9]+]] const_bitmap = ptr<const @type[[TYPE_bitmap_head_def]]>;
// DEFAULT-NEXT:     type @type[[TYPE_BITMAP_WORD:[0-9]+]] BITMAP_WORD = u64;
// DEFAULT-NEXT:     type @type[[TYPE_bitmap_obstack]] bitmap_obstack = struct {
// DEFAULT-NEXT:         field0 elements: ptr<@type[[TYPE_bitmap_element_def]]>;
// DEFAULT-NEXT:         field1 heads: ptr<@type[[TYPE_bitmap_head_def]]>;
// DEFAULT-NEXT:         field2 obstack: @type[[TYPE_obstack]];
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     type @type[[TYPE_bitmap_element_def]] bitmap_element_def = struct {
// DEFAULT-NEXT:         field0 next: ptr<@type[[TYPE_bitmap_element_def]]>;
// DEFAULT-NEXT:         field1 prev: ptr<@type[[TYPE_bitmap_element_def]]>;
// DEFAULT-NEXT:         field2 indx: u32;
// DEFAULT-NEXT:         field3 bits: array<u64, 2>;
// DEFAULT-NEXT:     } [size=40, align=8, offsets=[0, 8, 16, 24]];
// DEFAULT-NEXT:     type @type[[TYPE_bitmap_obstack_2:[0-9]+]] bitmap_obstack = @type[[TYPE_bitmap_obstack]];
// DEFAULT-NEXT:     type @type[[TYPE_bitmap_element:[0-9]+]] bitmap_element = @type[[TYPE_bitmap_element_def]];
// DEFAULT-NEXT:     type @type[[TYPE_bitmap_descriptor:[0-9]+]] bitmap_descriptor = struct incomplete;
// DEFAULT-NEXT:     type @type[[TYPE_bitmap_head:[0-9]+]] bitmap_head = @type[[TYPE_bitmap_head_def]];
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 elt1: ptr<@type[[TYPE_bitmap_element_def]]>;
// DEFAULT-NEXT:         field1 elt2: ptr<@type[[TYPE_bitmap_element_def]]>;
// DEFAULT-NEXT:         field2 word_no: u32;
// DEFAULT-NEXT:         field3 bits: u64;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 16, 24]];
// DEFAULT-NEXT:     type @type[[TYPE_bitmap_iterator:[0-9]+]] bitmap_iterator = @type[[TYPE0]];
// DEFAULT-NEXT:     global %[[VALUE_bitmap_zero_bits:[0-9]+]] bitmap_zero_bits: @type[[TYPE_bitmap_element_def]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_bmp_iter_set_init:[0-9]+]] @bmp_iter_set_init(%[[VALUE_bi:[0-9]+]] bi: ptr<@type[[TYPE0]]>, %[[VALUE_map:[0-9]+]] map: ptr<const @type[[TYPE_bitmap_head_def]]>, %[[VALUE_start_bit:[0-9]+]] start_bit: u32, %[[VALUE_bit_no:[0-9]+]] bit_no: ptr<u32>) -> void [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_bitmap_element_def]]>>(field0(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_bi]]))), read<ptr<@type[[TYPE_bitmap_element_def]]>>(field0(deref(read<ptr<const @type[[TYPE_bitmap_head_def]]>>(%[[VALUE_map]])))));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_bitmap_element_def]]>>(field1(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_bi]]))), null<ptr<@type[[TYPE_bitmap_element_def]]>>);
// DEFAULT-NEXT:         while %[[VALUE0:[0-9]+]] ne<i32>(const<i32>(1), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<ptr<@type[[TYPE_bitmap_element_def]]>>(read<ptr<@type[[TYPE_bitmap_element_def]]>>(field0(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_bi]])))), null<ptr<@type[[TYPE_bitmap_element_def]]>>))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         write<ptr<@type[[TYPE_bitmap_element_def]]>>(field0(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_bi]]))), addr_of<ptr<@type[[TYPE_bitmap_element_def]]>>(%[[VALUE_bitmap_zero_bits]]));
// DEFAULT-NEXT:                         break %[[VALUE0]];
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 if ge<u32>(read<u32>(field2(deref(read<ptr<@type[[TYPE_bitmap_element_def]]>>(field0(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_bi]]))))))), div<u32, by_zero=ub>(read<u32>(%[[VALUE_start_bit]]), const<u32>(128)))
// DEFAULT-NEXT:                     break %[[VALUE0]];
// DEFAULT-NEXT:                 write<ptr<@type[[TYPE_bitmap_element_def]]>>(field0(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_bi]]))), read<ptr<@type[[TYPE_bitmap_element_def]]>>(field0(deref(read<ptr<@type[[TYPE_bitmap_element_def]]>>(field0(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_bi]]))))))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if ne<u32>(read<u32>(field2(deref(read<ptr<@type[[TYPE_bitmap_element_def]]>>(field0(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_bi]]))))))), div<u32, by_zero=ub>(read<u32>(%[[VALUE_start_bit]]), const<u32>(128)))
// DEFAULT-NEXT:             write<u32>(%[[VALUE_start_bit]], mul<u32, overflow=wrap>(read<u32>(field2(deref(read<ptr<@type[[TYPE_bitmap_element_def]]>>(field0(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_bi]]))))))), const<u32>(128)));
// DEFAULT-NEXT:         write<u32>(field2(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_bi]]))), rem<u32, by_zero=ub>(div<u32, by_zero=ub>(read<u32>(%[[VALUE_start_bit]]), const<u32>(64)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:         write<u64>(field3(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_bi]]))), read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(2)>(field3(deref(read<ptr<@type[[TYPE_bitmap_element_def]]>>(field0(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_bi]]))))))), read<u32>(field2(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_bi]]))))))));
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: ptr<@type[[TYPE0]]> [synthetic] = read<ptr<@type[[TYPE0]]>>(%[[VALUE_bi]]);
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: u64 [synthetic] = read<u64>(field3(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE1]]))));
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: u64 [synthetic] = shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%[[VALUE2]]), rem<u32, by_zero=ub>(read<u32>(%[[VALUE_start_bit]]), const<u32>(64)));
// DEFAULT-NEXT:         write<u64>(field3(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE1]]))), read<u64>(%[[VALUE3]]));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_start_bit]]);
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE4]]), reinterpret<u32, reason=usual_arith, fits=always>(from_bool<i32, reason=promotion>(not<bool>(ne<u64>(read<u64>(field3(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_bi]])))), const<u64>(0))))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_start_bit]], read<u32>(%[[VALUE5]]));
// DEFAULT-NEXT:         write<u32>(deref(read<ptr<u32>>(%[[VALUE_bit_no]])), read<u32>(%[[VALUE_start_bit]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bmp_iter_next:[0-9]+]] @bmp_iter_next(%[[VALUE_bi_2:[0-9]+]] bi: ptr<@type[[TYPE0]]>, %[[VALUE_bit_no_2:[0-9]+]] bit_no: ptr<u32>) -> void [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: ptr<@type[[TYPE0]]> [synthetic] = read<ptr<@type[[TYPE0]]>>(%[[VALUE_bi_2]]);
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: u64 [synthetic] = read<u64>(field3(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE6]]))));
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: u64 [synthetic] = shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%[[VALUE7]]), const<i32>(1));
// DEFAULT-NEXT:         write<u64>(field3(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE6]]))), read<u64>(%[[VALUE8]]));
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: ptr<u32> [synthetic] = read<ptr<u32>>(%[[VALUE_bit_no_2]]);
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: u32 [synthetic] = read<u32>(deref(read<ptr<u32>>(%[[VALUE9]])));
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE10]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(deref(read<ptr<u32>>(%[[VALUE9]])), read<u32>(%[[VALUE11]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bmp_iter_set:[0-9]+]] @bmp_iter_set(%[[VALUE_bi_3:[0-9]+]] bi: ptr<@type[[TYPE0]]>, %[[VALUE_bit_no_3:[0-9]+]] bit_no: ptr<u32>) -> u8 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<u64>(read<u64>(field3(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_bi_3]])))), const<u64>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 while %[[VALUE12:[0-9]+]] not<bool>(ne<u64>(and<u64>(read<u64>(field3(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_bi_3]])))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), const<u64>(0)))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE13:[0-9]+]]: ptr<@type[[TYPE0]]> [synthetic] = read<ptr<@type[[TYPE0]]>>(%[[VALUE_bi_3]]);
// DEFAULT-NEXT:                         let %[[VALUE14:[0-9]+]]: u64 [synthetic] = read<u64>(field3(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE13]]))));
// DEFAULT-NEXT:                         let %[[VALUE15:[0-9]+]]: u64 [synthetic] = shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%[[VALUE14]]), const<i32>(1));
// DEFAULT-NEXT:                         write<u64>(field3(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE13]]))), read<u64>(%[[VALUE15]]));
// DEFAULT-NEXT:                         let %[[VALUE16:[0-9]+]]: ptr<u32> [synthetic] = read<ptr<u32>>(%[[VALUE_bit_no_3]]);
// DEFAULT-NEXT:                         let %[[VALUE17:[0-9]+]]: u32 [synthetic] = read<u32>(deref(read<ptr<u32>>(%[[VALUE16]])));
// DEFAULT-NEXT:                         let %[[VALUE18:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE17]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                         write<u32>(deref(read<ptr<u32>>(%[[VALUE16]])), read<u32>(%[[VALUE18]]));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 return reinterpret<u8, reason=return, fits=unknown>(truncate<i8, reason=return, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<u32>(deref(read<ptr<u32>>(%[[VALUE_bit_no_3]])), mul<u32, overflow=wrap>(div<u32, by_zero=ub>(sub<u32, overflow=wrap>(add<u32, overflow=wrap>(read<u32>(deref(read<ptr<u32>>(%[[VALUE_bit_no_3]]))), const<u32>(64)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), const<u32>(64)), const<u32>(64)));
// DEFAULT-NEXT:         let %[[VALUE19:[0-9]+]]: ptr<@type[[TYPE0]]> [synthetic] = read<ptr<@type[[TYPE0]]>>(%[[VALUE_bi_3]]);
// DEFAULT-NEXT:         let %[[VALUE20:[0-9]+]]: u32 [synthetic] = read<u32>(field2(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE19]]))));
// DEFAULT-NEXT:         let %[[VALUE21:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE20]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(field2(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE19]]))), read<u32>(%[[VALUE21]]));
// DEFAULT-NEXT:         while %[[VALUE22:[0-9]+]] ne<i32>(const<i32>(1), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 while %[[VALUE23:[0-9]+]] ne<u32>(read<u32>(field2(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_bi_3]])))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2)))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         write<u64>(field3(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_bi_3]]))), read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(2)>(field3(deref(read<ptr<@type[[TYPE_bitmap_element_def]]>>(field0(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_bi_3]]))))))), read<u32>(field2(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_bi_3]]))))))));
// DEFAULT-NEXT:                         if ne<u64>(read<u64>(field3(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_bi_3]])))), const<u64>(0))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 while %[[VALUE24:[0-9]+]] not<bool>(ne<u64>(and<u64>(read<u64>(field3(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_bi_3]])))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), const<u64>(0)))
// DEFAULT-NEXT:                                     {
// DEFAULT-NEXT:                                         let %[[VALUE25:[0-9]+]]: ptr<@type[[TYPE0]]> [synthetic] = read<ptr<@type[[TYPE0]]>>(%[[VALUE_bi_3]]);
// DEFAULT-NEXT:                                         let %[[VALUE26:[0-9]+]]: u64 [synthetic] = read<u64>(field3(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE25]]))));
// DEFAULT-NEXT:                                         let %[[VALUE27:[0-9]+]]: u64 [synthetic] = shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%[[VALUE26]]), const<i32>(1));
// DEFAULT-NEXT:                                         write<u64>(field3(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE25]]))), read<u64>(%[[VALUE27]]));
// DEFAULT-NEXT:                                         let %[[VALUE28:[0-9]+]]: ptr<u32> [synthetic] = read<ptr<u32>>(%[[VALUE_bit_no_3]]);
// DEFAULT-NEXT:                                         let %[[VALUE29:[0-9]+]]: u32 [synthetic] = read<u32>(deref(read<ptr<u32>>(%[[VALUE28]])));
// DEFAULT-NEXT:                                         let %[[VALUE30:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE29]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                                         write<u32>(deref(read<ptr<u32>>(%[[VALUE28]])), read<u32>(%[[VALUE30]]));
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                                 return reinterpret<u8, reason=return, fits=unknown>(truncate<i8, reason=return, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         let %[[VALUE31:[0-9]+]]: ptr<u32> [synthetic] = read<ptr<u32>>(%[[VALUE_bit_no_3]]);
// DEFAULT-NEXT:                         let %[[VALUE32:[0-9]+]]: u32 [synthetic] = read<u32>(deref(read<ptr<u32>>(%[[VALUE31]])));
// DEFAULT-NEXT:                         let %[[VALUE33:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE32]]), const<u32>(64));
// DEFAULT-NEXT:                         write<u32>(deref(read<ptr<u32>>(%[[VALUE31]])), read<u32>(%[[VALUE33]]));
// DEFAULT-NEXT:                         let %[[VALUE34:[0-9]+]]: ptr<@type[[TYPE0]]> [synthetic] = read<ptr<@type[[TYPE0]]>>(%[[VALUE_bi_3]]);
// DEFAULT-NEXT:                         let %[[VALUE35:[0-9]+]]: u32 [synthetic] = read<u32>(field2(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE34]]))));
// DEFAULT-NEXT:                         let %[[VALUE36:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE35]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                         write<u32>(field2(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE34]]))), read<u32>(%[[VALUE36]]));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 write<ptr<@type[[TYPE_bitmap_element_def]]>>(field0(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_bi_3]]))), read<ptr<@type[[TYPE_bitmap_element_def]]>>(field0(deref(read<ptr<@type[[TYPE_bitmap_element_def]]>>(field0(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_bi_3]]))))))));
// DEFAULT-NEXT:                 if not<bool>(ne<ptr<@type[[TYPE_bitmap_element_def]]>>(read<ptr<@type[[TYPE_bitmap_element_def]]>>(field0(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_bi_3]])))), null<ptr<@type[[TYPE_bitmap_element_def]]>>))
// DEFAULT-NEXT:                     return reinterpret<u8, reason=return, fits=unknown>(truncate<i8, reason=return, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 write<u32>(deref(read<ptr<u32>>(%[[VALUE_bit_no_3]])), mul<u32, overflow=wrap>(read<u32>(field2(deref(read<ptr<@type[[TYPE_bitmap_element_def]]>>(field0(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_bi_3]]))))))), const<u32>(128)));
// DEFAULT-NEXT:                 write<u32>(field2(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_bi_3]]))), reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foobar:[0-9]+]] @foobar(%[[VALUE_live_throughout:[0-9]+]] live_throughout: ptr<@type[[TYPE_bitmap_head_def]]>) -> void [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_rsi:[0-9]+]] rsi: @type[[TYPE0]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_regno:[0-9]+]] regno: u32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE37:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<@type[[TYPE0]]>, ptr<const @type[[TYPE_bitmap_head_def]]>, u32, ptr<u32>) -> void>(%[[VALUE_bmp_iter_set_init]], addr_of<ptr<@type[[TYPE0]]>>(%[[VALUE_rsi]]), pointer_cast<ptr<const @type[[TYPE_bitmap_head_def]]>, reason=arg>(read<ptr<@type[[TYPE_bitmap_head_def]]>>(%[[VALUE_live_throughout]])), reinterpret<u32, reason=arg, fits=always>(const<i32>(0)), addr_of<ptr<u32>>(%[[VALUE_regno]]));
// DEFAULT-NEXT:             condition: ne<u8>(call<u8, signature=fn(ptr<@type[[TYPE0]]>, ptr<u32>) -> u8>(%[[VALUE_bmp_iter_set]], addr_of<ptr<@type[[TYPE0]]>>(%[[VALUE_rsi]]), addr_of<ptr<u32>>(%[[VALUE_regno]])), const<u8>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<@type[[TYPE0]]>, ptr<u32>) -> void>(%[[VALUE_bmp_iter_next]], addr_of<ptr<@type[[TYPE0]]>>(%[[VALUE_rsi]]), addr_of<ptr<u32>>(%[[VALUE_regno]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_elem:[0-9]+]] elem: @type[[TYPE_bitmap_element_def]] [storage=automatic] = aggregate<@type[[TYPE_bitmap_element_def]], zero_fill=false>(field0 = null<ptr<@type[[TYPE_bitmap_element_def]]>>, field1 = null<ptr<@type[[TYPE_bitmap_element_def]]>>, field2 = reinterpret<u32, reason=assign, fits=always>(const<i32>(0)), field3 = aggregate<array<u64, 2>, zero_fill=false>(index0 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(1))), index1 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(1)))));
// DEFAULT-NEXT:         let %[[VALUE_live_throughout_2:[0-9]+]] live_throughout: @type[[TYPE_bitmap_head_def]] [storage=automatic] = aggregate<@type[[TYPE_bitmap_head_def]], zero_fill=false>(field0 = addr_of<ptr<@type[[TYPE_bitmap_element_def]]>>(%[[VALUE_elem]]), field1 = addr_of<ptr<@type[[TYPE_bitmap_element_def]]>>(%[[VALUE_elem]]), field2 = reinterpret<u32, reason=assign, fits=always>(const<i32>(0)), field3 = null<ptr<@type[[TYPE_bitmap_obstack]]>>);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_bitmap_head_def]]>) -> void>(%[[VALUE_foobar]], addr_of<ptr<@type[[TYPE_bitmap_head_def]]>>(%[[VALUE_live_throughout_2]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
