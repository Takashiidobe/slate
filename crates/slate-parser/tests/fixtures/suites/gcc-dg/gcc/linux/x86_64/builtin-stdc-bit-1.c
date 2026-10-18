/* { dg-do run } */
/* { dg-options "-std=c11" } */

unsigned int
leading_zeros (unsigned char a, unsigned short b, unsigned int c,
	       unsigned long d, unsigned long long e)
{
  return (__builtin_stdc_leading_zeros (a)
	  + __builtin_stdc_leading_zeros (b)
	  + __builtin_stdc_leading_zeros (c)
	  + __builtin_stdc_leading_zeros (d)
	  + __builtin_stdc_leading_zeros (e));
}

unsigned int
leading_ones (unsigned char a, unsigned short b, unsigned int c,
	      unsigned long d, unsigned long long e)
{
  return (__builtin_stdc_leading_ones (a)
	  + __builtin_stdc_leading_ones (b)
	  + __builtin_stdc_leading_ones (c)
	  + __builtin_stdc_leading_ones (d)
	  + __builtin_stdc_leading_ones (e));
}

unsigned int
trailing_zeros (unsigned char a, unsigned short b, unsigned int c,
		unsigned long d, unsigned long long e)
{
  return (__builtin_stdc_trailing_zeros (a)
	  + __builtin_stdc_trailing_zeros (b)
	  + __builtin_stdc_trailing_zeros (c)
	  + __builtin_stdc_trailing_zeros (d)
	  + __builtin_stdc_trailing_zeros (e));
}

unsigned int
trailing_ones (unsigned char a, unsigned short b, unsigned int c,
	       unsigned long d, unsigned long long e)
{
  return (__builtin_stdc_trailing_ones (a)
	  + __builtin_stdc_trailing_ones (b)
	  + __builtin_stdc_trailing_ones (c)
	  + __builtin_stdc_trailing_ones (d)
	  + __builtin_stdc_trailing_ones (e));
}

unsigned int
first_leading_zero (unsigned char a, unsigned short b, unsigned int c,
		    unsigned long d, unsigned long long e)
{
  return (__builtin_stdc_first_leading_zero (a)
	  + __builtin_stdc_first_leading_zero (b)
	  + __builtin_stdc_first_leading_zero (c)
	  + __builtin_stdc_first_leading_zero (d)
	  + __builtin_stdc_first_leading_zero (e));
}

unsigned int
first_leading_one (unsigned char a, unsigned short b, unsigned int c,
		   unsigned long d, unsigned long long e)
{
  return (__builtin_stdc_first_leading_one (a)
	  + __builtin_stdc_first_leading_one (b)
	  + __builtin_stdc_first_leading_one (c)
	  + __builtin_stdc_first_leading_one (d)
	  + __builtin_stdc_first_leading_one (e));
}

unsigned int
first_trailing_zero (unsigned char a, unsigned short b, unsigned int c,
		     unsigned long d, unsigned long long e)
{
  return (__builtin_stdc_first_trailing_zero (a)
	  + __builtin_stdc_first_trailing_zero (b)
	  + __builtin_stdc_first_trailing_zero (c)
	  + __builtin_stdc_first_trailing_zero (d)
	  + __builtin_stdc_first_trailing_zero (e));
}

unsigned int
first_trailing_one (unsigned char a, unsigned short b, unsigned int c,
		    unsigned long d, unsigned long long e)
{
  return (__builtin_stdc_first_trailing_one (a)
	  + __builtin_stdc_first_trailing_one (b)
	  + __builtin_stdc_first_trailing_one (c)
	  + __builtin_stdc_first_trailing_one (d)
	  + __builtin_stdc_first_trailing_one (e));
}

unsigned int
count_zeros (unsigned char a, unsigned short b, unsigned int c,
	     unsigned long d, unsigned long long e)
{
  return (__builtin_stdc_count_zeros (a)
	  + __builtin_stdc_count_zeros (b)
	  + __builtin_stdc_count_zeros (c)
	  + __builtin_stdc_count_zeros (d)
	  + __builtin_stdc_count_zeros (e));
}

unsigned int
count_ones (unsigned char a, unsigned short b, unsigned int c,
	    unsigned long d, unsigned long long e)
{
  return (__builtin_stdc_count_ones (a)
	  + __builtin_stdc_count_ones (b)
	  + __builtin_stdc_count_ones (c)
	  + __builtin_stdc_count_ones (d)
	  + __builtin_stdc_count_ones (e));
}

unsigned int
has_single_bit (unsigned char a, unsigned short b, unsigned int c,
		unsigned long d, unsigned long long e)
{
  return (__builtin_stdc_has_single_bit (a)
	  || __builtin_stdc_has_single_bit (b)
	  || __builtin_stdc_has_single_bit (c)
	  || __builtin_stdc_has_single_bit (d)
	  || __builtin_stdc_has_single_bit (e));
}

unsigned int
bit_width (unsigned char a, unsigned short b, unsigned int c,
	   unsigned long d, unsigned long long e)
{
  return (__builtin_stdc_bit_width (a)
	  + __builtin_stdc_bit_width (b)
	  + __builtin_stdc_bit_width (c)
	  + __builtin_stdc_bit_width (d)
	  + __builtin_stdc_bit_width (e));
}

unsigned long long
bit_floor (unsigned char a, unsigned short b, unsigned int c,
	   unsigned long d, unsigned long long e)
{
  return (__builtin_stdc_bit_floor (a)
	  + __builtin_stdc_bit_floor (b)
	  + __builtin_stdc_bit_floor (c)
	  + __builtin_stdc_bit_floor (d)
	  + __builtin_stdc_bit_floor (e));
}

unsigned long long
bit_ceil (unsigned char a, unsigned short b, unsigned int c,
	  unsigned long d, unsigned long long e)
{
  return (__builtin_stdc_bit_ceil (a)
	  + __builtin_stdc_bit_ceil (b)
	  + __builtin_stdc_bit_ceil (c)
	  + __builtin_stdc_bit_ceil (d)
	  + __builtin_stdc_bit_ceil (e));
}

#define expr_has_type(e, t) _Generic (e, default : 0, t : 1)

int
main ()
{
  if (__builtin_stdc_leading_zeros ((unsigned char) 0) != __CHAR_BIT__
      || !expr_has_type (__builtin_stdc_leading_zeros ((unsigned char) 0), unsigned int)
      || __builtin_stdc_leading_zeros ((unsigned short) 0) != __SIZEOF_SHORT__ * __CHAR_BIT__
      || !expr_has_type (__builtin_stdc_leading_zeros ((unsigned short) 0), unsigned int)
      || __builtin_stdc_leading_zeros (0U) != __SIZEOF_INT__ * __CHAR_BIT__
      || !expr_has_type (__builtin_stdc_leading_zeros (0U), unsigned int)
      || __builtin_stdc_leading_zeros (0UL) != __SIZEOF_LONG__ * __CHAR_BIT__
      || !expr_has_type (__builtin_stdc_leading_zeros (0UL), unsigned int)
      || __builtin_stdc_leading_zeros (0ULL) != __SIZEOF_LONG_LONG__ * __CHAR_BIT__
      || !expr_has_type (__builtin_stdc_leading_zeros (0ULL), unsigned int))
    __builtin_abort ();
  if (__builtin_stdc_leading_zeros ((unsigned char) ~0U) != 0
      || __builtin_stdc_leading_zeros ((unsigned short) ~0U) != 0
      || __builtin_stdc_leading_zeros (~0U) != 0
      || __builtin_stdc_leading_zeros (~0UL) != 0
      || __builtin_stdc_leading_zeros (~0ULL) != 0)
    __builtin_abort ();
  if (__builtin_stdc_leading_zeros ((unsigned char) 3) != __CHAR_BIT__ - 2
      || __builtin_stdc_leading_zeros ((unsigned short) 9) != __SIZEOF_SHORT__ * __CHAR_BIT__ - 4
      || __builtin_stdc_leading_zeros (34U) != __SIZEOF_INT__ * __CHAR_BIT__ - 6
      || __builtin_stdc_leading_zeros (130UL) != __SIZEOF_LONG__ * __CHAR_BIT__ - 8
      || __builtin_stdc_leading_zeros (512ULL) != __SIZEOF_LONG_LONG__ * __CHAR_BIT__ - 10)
    __builtin_abort ();
  if (__builtin_stdc_leading_ones ((unsigned char) 0) != 0
      || !expr_has_type (__builtin_stdc_leading_ones ((unsigned char) 0), unsigned int)
      || __builtin_stdc_leading_ones ((unsigned short) 0) != 0
      || !expr_has_type (__builtin_stdc_leading_ones ((unsigned short) 0), unsigned int)
      || __builtin_stdc_leading_ones (0U) != 0
      || !expr_has_type (__builtin_stdc_leading_ones (0U), unsigned int)
      || __builtin_stdc_leading_ones (0UL) != 0
      || !expr_has_type (__builtin_stdc_leading_ones (0UL), unsigned int)
      || __builtin_stdc_leading_ones (0ULL) != 0
      || !expr_has_type (__builtin_stdc_leading_ones (0ULL), unsigned int))
    __builtin_abort ();
  if (__builtin_stdc_leading_ones ((unsigned char) ~0U) != __CHAR_BIT__
      || __builtin_stdc_leading_ones ((unsigned short) ~0U) != __SIZEOF_SHORT__ * __CHAR_BIT__
      || __builtin_stdc_leading_ones (~0U) != __SIZEOF_INT__ * __CHAR_BIT__
      || __builtin_stdc_leading_ones (~0UL) != __SIZEOF_LONG__ * __CHAR_BIT__
      || __builtin_stdc_leading_ones (~0ULL) != __SIZEOF_LONG_LONG__ * __CHAR_BIT__)
    __builtin_abort ();
  if (__builtin_stdc_leading_ones ((unsigned char) ~3) != __CHAR_BIT__ - 2
      || __builtin_stdc_leading_ones ((unsigned short) ~9) != __SIZEOF_SHORT__ * __CHAR_BIT__ - 4
      || __builtin_stdc_leading_ones (~34U) != __SIZEOF_INT__ * __CHAR_BIT__ - 6
      || __builtin_stdc_leading_ones (~130UL) != __SIZEOF_LONG__ * __CHAR_BIT__ - 8
      || __builtin_stdc_leading_ones (~512ULL) != __SIZEOF_LONG_LONG__ * __CHAR_BIT__ - 10)
    __builtin_abort ();
  if (__builtin_stdc_trailing_zeros ((unsigned char) 0) != __CHAR_BIT__
      || !expr_has_type (__builtin_stdc_trailing_zeros ((unsigned char) 0), unsigned int)
      || __builtin_stdc_trailing_zeros ((unsigned short) 0) != __SIZEOF_SHORT__ * __CHAR_BIT__
      || !expr_has_type (__builtin_stdc_trailing_zeros ((unsigned short) 0), unsigned int)
      || __builtin_stdc_trailing_zeros (0U) != __SIZEOF_INT__ * __CHAR_BIT__
      || !expr_has_type (__builtin_stdc_trailing_zeros (0U), unsigned int)
      || __builtin_stdc_trailing_zeros (0UL) != __SIZEOF_LONG__ * __CHAR_BIT__
      || !expr_has_type (__builtin_stdc_trailing_zeros (0UL), unsigned int)
      || __builtin_stdc_trailing_zeros (0ULL) != __SIZEOF_LONG_LONG__ * __CHAR_BIT__
      || !expr_has_type (__builtin_stdc_trailing_zeros (0ULL), unsigned int))
    __builtin_abort ();
  if (__builtin_stdc_trailing_zeros ((unsigned char) ~0U) != 0
      || __builtin_stdc_trailing_zeros ((unsigned short) ~0U) != 0
      || __builtin_stdc_trailing_zeros (~0U) != 0
      || __builtin_stdc_trailing_zeros (~0UL) != 0
      || __builtin_stdc_trailing_zeros (~0ULL) != 0)
    __builtin_abort ();
  if (__builtin_stdc_trailing_zeros ((unsigned char) 2) != 1
      || __builtin_stdc_trailing_zeros ((unsigned short) 24) != 3
      || __builtin_stdc_trailing_zeros (32U) != 5
      || __builtin_stdc_trailing_zeros (128UL) != 7
      || __builtin_stdc_trailing_zeros (512ULL) != 9)
    __builtin_abort ();
  if (__builtin_stdc_trailing_ones ((unsigned char) 0) != 0
      || !expr_has_type (__builtin_stdc_trailing_ones ((unsigned char) 0), unsigned int)
      || __builtin_stdc_trailing_ones ((unsigned short) 0) != 0
      || !expr_has_type (__builtin_stdc_trailing_ones ((unsigned short) 0), unsigned int)
      || __builtin_stdc_trailing_ones (0U) != 0
      || !expr_has_type (__builtin_stdc_trailing_ones (0U), unsigned int)
      || __builtin_stdc_trailing_ones (0UL) != 0
      || !expr_has_type (__builtin_stdc_trailing_ones (0UL), unsigned int)
      || __builtin_stdc_trailing_ones (0ULL) != 0
      || !expr_has_type (__builtin_stdc_trailing_ones (0ULL), unsigned int))
    __builtin_abort ();
  if (__builtin_stdc_trailing_ones ((unsigned char) ~0U) != __CHAR_BIT__
      || __builtin_stdc_trailing_ones ((unsigned short) ~0U) != __SIZEOF_SHORT__ * __CHAR_BIT__
      || __builtin_stdc_trailing_ones (~0U) != __SIZEOF_INT__ * __CHAR_BIT__
      || __builtin_stdc_trailing_ones (~0UL) != __SIZEOF_LONG__ * __CHAR_BIT__
      || __builtin_stdc_trailing_ones (~0ULL) != __SIZEOF_LONG_LONG__ * __CHAR_BIT__)
    __builtin_abort ();
  if (__builtin_stdc_trailing_ones ((unsigned char) 5) != 1
      || __builtin_stdc_trailing_ones ((unsigned short) 15) != 4
      || __builtin_stdc_trailing_ones (127U) != 7
      || __builtin_stdc_trailing_ones (511UL) != 9
      || __builtin_stdc_trailing_ones (~0ULL >> 2) != __SIZEOF_LONG_LONG__ * __CHAR_BIT__ - 2)
    __builtin_abort ();
  if (__builtin_stdc_first_leading_zero ((unsigned char) 0) != 1
      || !expr_has_type (__builtin_stdc_first_leading_zero ((unsigned char) 0), unsigned int)
      || __builtin_stdc_first_leading_zero ((unsigned short) 0) != 1
      || !expr_has_type (__builtin_stdc_first_leading_zero ((unsigned short) 0), unsigned int)
      || __builtin_stdc_first_leading_zero (0U) != 1
      || !expr_has_type (__builtin_stdc_first_leading_zero (0U), unsigned int)
      || __builtin_stdc_first_leading_zero (0UL) != 1
      || !expr_has_type (__builtin_stdc_first_leading_zero (0UL), unsigned int)
      || __builtin_stdc_first_leading_zero (0ULL) != 1
      || !expr_has_type (__builtin_stdc_first_leading_zero (0ULL), unsigned int))
    __builtin_abort ();
  if (__builtin_stdc_first_leading_zero ((unsigned char) ~0U) != 0
      || __builtin_stdc_first_leading_zero ((unsigned short) ~0U) != 0
      || __builtin_stdc_first_leading_zero (~0U) != 0
      || __builtin_stdc_first_leading_zero (~0UL) != 0
      || __builtin_stdc_first_leading_zero (~0ULL) != 0)
    __builtin_abort ();
  if (__builtin_stdc_first_leading_zero ((unsigned char) ~3U) != __CHAR_BIT__ - 1
      || __builtin_stdc_first_leading_zero ((unsigned short) ~15U) != __SIZEOF_SHORT__ * __CHAR_BIT__ - 3
      || __builtin_stdc_first_leading_zero (~63U) != __SIZEOF_INT__ * __CHAR_BIT__ - 5
      || __builtin_stdc_first_leading_zero (~255UL) != __SIZEOF_LONG__ * __CHAR_BIT__ - 7
      || __builtin_stdc_first_leading_zero (~1023ULL) != __SIZEOF_LONG_LONG__ * __CHAR_BIT__ - 9)
    __builtin_abort ();
  if (__builtin_stdc_first_leading_one ((unsigned char) 0) != 0
      || !expr_has_type (__builtin_stdc_first_leading_one ((unsigned char) 0), unsigned int)
      || __builtin_stdc_first_leading_one ((unsigned short) 0) != 0
      || !expr_has_type (__builtin_stdc_first_leading_one ((unsigned short) 0), unsigned int)
      || __builtin_stdc_first_leading_one (0U) != 0
      || !expr_has_type (__builtin_stdc_first_leading_one (0U), unsigned int)
      || __builtin_stdc_first_leading_one (0UL) != 0
      || !expr_has_type (__builtin_stdc_first_leading_one (0UL), unsigned int)
      || __builtin_stdc_first_leading_one (0ULL) != 0
      || !expr_has_type (__builtin_stdc_first_leading_one (0ULL), unsigned int))
    __builtin_abort ();
  if (__builtin_stdc_first_leading_one ((unsigned char) ~0U) != 1
      || __builtin_stdc_first_leading_one ((unsigned short) ~0U) != 1
      || __builtin_stdc_first_leading_one (~0U) != 1
      || __builtin_stdc_first_leading_one (~0UL) != 1
      || __builtin_stdc_first_leading_one (~0ULL) != 1)
    __builtin_abort ();
  if (__builtin_stdc_first_leading_one ((unsigned char) 3) != __CHAR_BIT__ - 1
      || __builtin_stdc_first_leading_one ((unsigned short) 9) != __SIZEOF_SHORT__ * __CHAR_BIT__ - 3
      || __builtin_stdc_first_leading_one (34U) != __SIZEOF_INT__ * __CHAR_BIT__ - 5
      || __builtin_stdc_first_leading_one (130UL) != __SIZEOF_LONG__ * __CHAR_BIT__ - 7
      || __builtin_stdc_first_leading_one (512ULL) != __SIZEOF_LONG_LONG__ * __CHAR_BIT__ - 9)
    __builtin_abort ();
  if (__builtin_stdc_first_trailing_zero ((unsigned char) 0) != 1
      || !expr_has_type (__builtin_stdc_first_trailing_zero ((unsigned char) 0), unsigned int)
      || __builtin_stdc_first_trailing_zero ((unsigned short) 0) != 1
      || !expr_has_type (__builtin_stdc_first_trailing_zero ((unsigned short) 0), unsigned int)
      || __builtin_stdc_first_trailing_zero (0U) != 1
      || !expr_has_type (__builtin_stdc_first_trailing_zero (0U), unsigned int)
      || __builtin_stdc_first_trailing_zero (0UL) != 1
      || !expr_has_type (__builtin_stdc_first_trailing_zero (0UL), unsigned int)
      || __builtin_stdc_first_trailing_zero (0ULL) != 1
      || !expr_has_type (__builtin_stdc_first_trailing_zero (0ULL), unsigned int))
    __builtin_abort ();
  if (__builtin_stdc_first_trailing_zero ((unsigned char) ~0U) != 0
      || __builtin_stdc_first_trailing_zero ((unsigned short) ~0U) != 0
      || __builtin_stdc_first_trailing_zero (~0U) != 0
      || __builtin_stdc_first_trailing_zero (~0UL) != 0
      || __builtin_stdc_first_trailing_zero (~0ULL) != 0)
    __builtin_abort ();
  if (__builtin_stdc_first_trailing_zero ((unsigned char) 2) != 1
      || __builtin_stdc_first_trailing_zero ((unsigned short) 15) != 5
      || __builtin_stdc_first_trailing_zero (63U) != 7
      || __builtin_stdc_first_trailing_zero (128UL) != 1
      || __builtin_stdc_first_trailing_zero (511ULL) != 10)
    __builtin_abort ();
  if (__builtin_stdc_first_trailing_one ((unsigned char) 0) != 0
      || !expr_has_type (__builtin_stdc_first_trailing_one ((unsigned char) 0), unsigned int)
      || __builtin_stdc_first_trailing_one ((unsigned short) 0) != 0
      || !expr_has_type (__builtin_stdc_first_trailing_one ((unsigned short) 0), unsigned int)
      || __builtin_stdc_first_trailing_one (0U) != 0
      || !expr_has_type (__builtin_stdc_first_trailing_one (0U), unsigned int)
      || __builtin_stdc_first_trailing_one (0UL) != 0
      || !expr_has_type (__builtin_stdc_first_trailing_one (0UL), unsigned int)
      || __builtin_stdc_first_trailing_one (0ULL) != 0
      || !expr_has_type (__builtin_stdc_first_trailing_one (0ULL), unsigned int))
    __builtin_abort ();
  if (__builtin_stdc_first_trailing_one ((unsigned char) ~0U) != 1
      || __builtin_stdc_first_trailing_one ((unsigned short) ~0U) != 1
      || __builtin_stdc_first_trailing_one (~0U) != 1
      || __builtin_stdc_first_trailing_one (~0UL) != 1
      || __builtin_stdc_first_trailing_one (~0ULL) != 1)
    __builtin_abort ();
  if (__builtin_stdc_first_trailing_one ((unsigned char) 4) != 3
      || __builtin_stdc_first_trailing_one ((unsigned short) 96) != 6
      || __builtin_stdc_first_trailing_one (127U) != 1
      || __builtin_stdc_first_trailing_one (511UL) != 1
      || __builtin_stdc_first_trailing_one (~0ULL << 12) != 13)
    __builtin_abort ();
  if (__builtin_stdc_count_zeros ((unsigned char) 0) != __CHAR_BIT__
      || !expr_has_type (__builtin_stdc_count_zeros ((unsigned char) 0), unsigned int)
      || __builtin_stdc_count_zeros ((unsigned short) 0) != __SIZEOF_SHORT__ * __CHAR_BIT__
      || !expr_has_type (__builtin_stdc_count_zeros ((unsigned short) 0), unsigned int)
      || __builtin_stdc_count_zeros (0U) != __SIZEOF_INT__ * __CHAR_BIT__
      || !expr_has_type (__builtin_stdc_count_zeros (0U), unsigned int)
      || __builtin_stdc_count_zeros (0UL) != __SIZEOF_LONG__ * __CHAR_BIT__
      || !expr_has_type (__builtin_stdc_count_zeros (0UL), unsigned int)
      || __builtin_stdc_count_zeros (0ULL) != __SIZEOF_LONG_LONG__ * __CHAR_BIT__
      || !expr_has_type (__builtin_stdc_count_zeros (0ULL), unsigned int))
    __builtin_abort ();
  if (__builtin_stdc_count_zeros ((unsigned char) ~0U) != 0
      || __builtin_stdc_count_zeros ((unsigned short) ~0U) != 0
      || __builtin_stdc_count_zeros (~0U) != 0
      || __builtin_stdc_count_zeros (~0UL) != 0
      || __builtin_stdc_count_zeros (~0ULL) != 0)
    __builtin_abort ();
  if (__builtin_stdc_count_zeros ((unsigned char) 1U) != __CHAR_BIT__ - 1
      || __builtin_stdc_count_zeros ((unsigned short) 42) != __SIZEOF_SHORT__ * __CHAR_BIT__ - 3
      || __builtin_stdc_count_zeros (291U) != __SIZEOF_INT__ * __CHAR_BIT__ - 4
      || __builtin_stdc_count_zeros (~1315UL) != 5
      || __builtin_stdc_count_zeros (3363ULL) != __SIZEOF_LONG_LONG__ * __CHAR_BIT__ - 6)
    __builtin_abort ();
  if (__builtin_stdc_count_ones ((unsigned char) 0) != 0
      || !expr_has_type (__builtin_stdc_count_ones ((unsigned char) 0), unsigned int)
      || __builtin_stdc_count_ones ((unsigned short) 0) != 0
      || !expr_has_type (__builtin_stdc_count_ones ((unsigned short) 0), unsigned int)
      || __builtin_stdc_count_ones (0U) != 0
      || !expr_has_type (__builtin_stdc_count_ones (0U), unsigned int)
      || __builtin_stdc_count_ones (0UL) != 0
      || !expr_has_type (__builtin_stdc_count_ones (0UL), unsigned int)
      || __builtin_stdc_count_ones (0ULL) != 0
      || !expr_has_type (__builtin_stdc_count_ones (0ULL), unsigned int))
    __builtin_abort ();
  if (__builtin_stdc_count_ones ((unsigned char) ~0U) != __CHAR_BIT__
      || __builtin_stdc_count_ones ((unsigned short) ~0U) != __SIZEOF_SHORT__ * __CHAR_BIT__
      || __builtin_stdc_count_ones (~0U) != __SIZEOF_INT__ * __CHAR_BIT__
      || __builtin_stdc_count_ones (~0UL) != __SIZEOF_LONG__ * __CHAR_BIT__
      || __builtin_stdc_count_ones (~0ULL) != __SIZEOF_LONG_LONG__ * __CHAR_BIT__)
    __builtin_abort ();
  if (__builtin_stdc_count_ones ((unsigned char) ~1U) != __CHAR_BIT__ - 1
      || __builtin_stdc_count_ones ((unsigned short) ~42) != __SIZEOF_SHORT__ * __CHAR_BIT__ - 3
      || __builtin_stdc_count_ones (~291U) != __SIZEOF_INT__ * __CHAR_BIT__ - 4
      || __builtin_stdc_count_ones (1315UL) != 5
      || __builtin_stdc_count_ones (~3363ULL) != __SIZEOF_LONG_LONG__ * __CHAR_BIT__ - 6)
    __builtin_abort ();
  if (__builtin_stdc_has_single_bit ((unsigned char) 0)
      || !expr_has_type (__builtin_stdc_has_single_bit ((unsigned char) 0), _Bool)
      || __builtin_stdc_has_single_bit ((unsigned short) 0)
      || !expr_has_type (__builtin_stdc_has_single_bit ((unsigned short) 0), _Bool)
      || __builtin_stdc_has_single_bit (0U)
      || !expr_has_type (__builtin_stdc_has_single_bit (0U), _Bool)
      || __builtin_stdc_has_single_bit (0UL)
      || !expr_has_type (__builtin_stdc_has_single_bit (0UL), _Bool)
      || __builtin_stdc_has_single_bit (0ULL)
      || !expr_has_type (__builtin_stdc_has_single_bit (0ULL), _Bool))
    __builtin_abort ();
  if (!__builtin_stdc_has_single_bit ((unsigned char) 2)
      || !__builtin_stdc_has_single_bit ((unsigned short) 8)
      || !__builtin_stdc_has_single_bit (32U)
      || !__builtin_stdc_has_single_bit (128UL)
      || !__builtin_stdc_has_single_bit (512ULL))
    __builtin_abort ();
  if (__builtin_stdc_has_single_bit ((unsigned char) 7)
      || __builtin_stdc_has_single_bit ((unsigned short) 96)
      || __builtin_stdc_has_single_bit (513U)
      || __builtin_stdc_has_single_bit (1022UL)
      || __builtin_stdc_has_single_bit (12ULL))
    __builtin_abort ();
  if (__builtin_stdc_bit_width ((unsigned char) 0) != 0
      || !expr_has_type (__builtin_stdc_bit_width ((unsigned char) 0), unsigned int)
      || __builtin_stdc_bit_width ((unsigned short) 0) != 0
      || !expr_has_type (__builtin_stdc_bit_width ((unsigned short) 0), unsigned int)
      || __builtin_stdc_bit_width (0U) != 0
      || !expr_has_type (__builtin_stdc_bit_width (0U), unsigned int)
      || __builtin_stdc_bit_width (0UL) != 0
      || !expr_has_type (__builtin_stdc_bit_width (0UL), unsigned int)
      || __builtin_stdc_bit_width (0ULL) != 0
      || !expr_has_type (__builtin_stdc_bit_width (0ULL), unsigned int))
    __builtin_abort ();
  if (__builtin_stdc_bit_width ((unsigned char) ~0U) != __CHAR_BIT__
      || __builtin_stdc_bit_width ((unsigned short) ~0U) != __SIZEOF_SHORT__ * __CHAR_BIT__
      || __builtin_stdc_bit_width (~0U) != __SIZEOF_INT__ * __CHAR_BIT__
      || __builtin_stdc_bit_width (~0UL) != __SIZEOF_LONG__ * __CHAR_BIT__
      || __builtin_stdc_bit_width (~0ULL) != __SIZEOF_LONG_LONG__ * __CHAR_BIT__)
    __builtin_abort ();
  if (__builtin_stdc_bit_width ((unsigned char) ((unsigned char) ~0U >> 1)) != __CHAR_BIT__ - 1
      || __builtin_stdc_bit_width ((unsigned char) 6) != 3
      || __builtin_stdc_bit_width ((unsigned short) 12U) != 4
      || __builtin_stdc_bit_width ((unsigned short) ((unsigned short) ~0U >> 5)) != __SIZEOF_SHORT__ * __CHAR_BIT__ - 5
      || __builtin_stdc_bit_width (137U) != 8
      || __builtin_stdc_bit_width (269U) != 9
      || __builtin_stdc_bit_width (39UL) != 6
      || __builtin_stdc_bit_width (~0UL >> 2) != __SIZEOF_LONG__ * __CHAR_BIT__ - 2
      || __builtin_stdc_bit_width (1023ULL) != 10
      || __builtin_stdc_bit_width (1024ULL) != 11)
    __builtin_abort ();
  if (__builtin_stdc_bit_floor ((unsigned char) 0) != 0
      || !expr_has_type (__builtin_stdc_bit_floor ((unsigned char) 0), unsigned char)
      || __builtin_stdc_bit_floor ((unsigned short) 0) != 0
      || !expr_has_type (__builtin_stdc_bit_floor ((unsigned short) 0), unsigned short)
      || __builtin_stdc_bit_floor (0U) != 0U
      || !expr_has_type (__builtin_stdc_bit_floor (0U), unsigned int)
      || __builtin_stdc_bit_floor (0UL) != 0UL
      || !expr_has_type (__builtin_stdc_bit_floor (0UL), unsigned long)
      || __builtin_stdc_bit_floor (0ULL) != 0ULL
      || !expr_has_type (__builtin_stdc_bit_floor (0ULL), unsigned long long))
    __builtin_abort ();
  if (__builtin_stdc_bit_floor ((unsigned char) ~0U) != (1U << (__CHAR_BIT__ - 1))
      || __builtin_stdc_bit_floor ((unsigned short) ~0U) != (1U << (__SIZEOF_SHORT__ * __CHAR_BIT__ - 1))
      || __builtin_stdc_bit_floor (~0U) != (1U << (__SIZEOF_INT__ * __CHAR_BIT__ - 1))
      || __builtin_stdc_bit_floor (~0UL) != (1UL << (__SIZEOF_LONG__ * __CHAR_BIT__ - 1))
      || __builtin_stdc_bit_floor (~0ULL) != (1ULL << (__SIZEOF_LONG_LONG__ * __CHAR_BIT__ - 1)))
    __builtin_abort ();
  if (__builtin_stdc_bit_floor ((unsigned char) 4) != 4
      || __builtin_stdc_bit_floor ((unsigned char) 7) != 4
      || __builtin_stdc_bit_floor ((unsigned short) 8U) != 8
      || __builtin_stdc_bit_floor ((unsigned short) 31U) != 16
      || __builtin_stdc_bit_floor (137U) != 128U
      || __builtin_stdc_bit_floor (269U) != 256U
      || __builtin_stdc_bit_floor (511UL) != 256UL
      || __builtin_stdc_bit_floor (512UL) != 512UL
      || __builtin_stdc_bit_floor (513UL) != 512ULL
      || __builtin_stdc_bit_floor (1024ULL) != 1024ULL)
    __builtin_abort ();
  if (__builtin_stdc_bit_ceil ((unsigned char) 0) != 1
      || !expr_has_type (__builtin_stdc_bit_ceil ((unsigned char) 0), unsigned char)
      || __builtin_stdc_bit_ceil ((unsigned short) 0) != 1
      || !expr_has_type (__builtin_stdc_bit_ceil ((unsigned short) 0), unsigned short)
      || __builtin_stdc_bit_ceil (0U) != 1U
      || !expr_has_type (__builtin_stdc_bit_ceil (0U), unsigned int)
      || __builtin_stdc_bit_ceil (0UL) != 1UL
      || !expr_has_type (__builtin_stdc_bit_ceil (0UL), unsigned long)
      || __builtin_stdc_bit_ceil (0ULL) != 1ULL
      || !expr_has_type (__builtin_stdc_bit_ceil (0ULL), unsigned long long))
    __builtin_abort ();
  if (__builtin_stdc_bit_ceil ((unsigned char) ~0U) != 0
      || __builtin_stdc_bit_ceil ((unsigned short) ~0U) != 0
      || __builtin_stdc_bit_ceil (~0U) != 0U
      || __builtin_stdc_bit_ceil (~0UL) != 0UL
      || __builtin_stdc_bit_ceil (~0ULL) != 0ULL)
    __builtin_abort ();
  if (__builtin_stdc_bit_ceil ((unsigned char) ((unsigned char) ~0U >> 1)) != (1U << (__CHAR_BIT__ - 1))
      || __builtin_stdc_bit_ceil ((unsigned char) ((unsigned char) ~0U >> 1)) != (1U << (__CHAR_BIT__ - 1))
      || __builtin_stdc_bit_ceil ((unsigned short) ((unsigned short) ~0U >> 1)) != (1U << (__SIZEOF_SHORT__ * __CHAR_BIT__ - 1))
      || __builtin_stdc_bit_ceil ((unsigned short) ((unsigned short) ~0U >> 1)) != (1U << (__SIZEOF_SHORT__ * __CHAR_BIT__ - 1))
      || __builtin_stdc_bit_ceil (~0U >> 1) != (1U << (__SIZEOF_INT__ * __CHAR_BIT__ - 1))
      || __builtin_stdc_bit_ceil (1U << (__SIZEOF_INT__ * __CHAR_BIT__ - 1)) != (1U << (__SIZEOF_INT__ * __CHAR_BIT__ - 1))
      || __builtin_stdc_bit_ceil (~0UL >> 1) != (1UL << (__SIZEOF_LONG__ * __CHAR_BIT__ - 1))
      || __builtin_stdc_bit_ceil (~0UL >> 1) != (1UL << (__SIZEOF_LONG__ * __CHAR_BIT__ - 1))
      || __builtin_stdc_bit_ceil (1ULL << (__SIZEOF_LONG_LONG__ * __CHAR_BIT__ - 1)) != (1ULL << (__SIZEOF_LONG_LONG__ * __CHAR_BIT__ - 1))
      || __builtin_stdc_bit_ceil (~0ULL >> 1) != (1ULL << (__SIZEOF_LONG_LONG__ * __CHAR_BIT__ - 1)))
    __builtin_abort ();
  if (__builtin_stdc_bit_ceil ((unsigned char) 1) != 1
      || __builtin_stdc_bit_ceil ((unsigned char) 2) != 2
      || __builtin_stdc_bit_ceil ((unsigned short) 3U) != 4
      || __builtin_stdc_bit_ceil ((unsigned short) 4U) != 4
      || __builtin_stdc_bit_ceil (5U) != 8U
      || __builtin_stdc_bit_ceil (269U) != 512U
      || __builtin_stdc_bit_ceil (511UL) != 512UL
      || __builtin_stdc_bit_ceil (512UL) != 512UL
      || __builtin_stdc_bit_ceil (513ULL) != 1024ULL
      || __builtin_stdc_bit_ceil (1025ULL) != 2048ULL)
    __builtin_abort ();
#ifdef __SIZEOF_INT128__
  if (__builtin_stdc_leading_zeros ((unsigned __int128) 0) != __SIZEOF_INT128__ * __CHAR_BIT__
      || !expr_has_type (__builtin_stdc_leading_zeros ((unsigned __int128) 0), unsigned int)
      || __builtin_stdc_leading_zeros (~(unsigned __int128) 0) != 0)
    __builtin_abort ();
  if (__builtin_stdc_leading_ones ((unsigned __int128) 0) != 0
      || !expr_has_type (__builtin_stdc_leading_ones ((unsigned __int128) 0), unsigned int)
      || __builtin_stdc_leading_ones (~(unsigned __int128) 0) != __SIZEOF_INT128__ * __CHAR_BIT__)
    __builtin_abort ();
  if (__builtin_stdc_trailing_zeros ((unsigned __int128) 0) != __SIZEOF_INT128__ * __CHAR_BIT__
      || !expr_has_type (__builtin_stdc_trailing_zeros ((unsigned __int128) 0), unsigned int)
      || __builtin_stdc_trailing_zeros (~(unsigned __int128) 0) != 0)
    __builtin_abort ();
  if (__builtin_stdc_trailing_ones ((unsigned __int128) 0) != 0
      || !expr_has_type (__builtin_stdc_trailing_ones ((unsigned __int128) 0), unsigned int)
      || __builtin_stdc_trailing_ones (~(unsigned __int128) 0) != __SIZEOF_INT128__ * __CHAR_BIT__)
    __builtin_abort ();
  if (__builtin_stdc_first_leading_zero ((unsigned __int128) 0) != 1
      || !expr_has_type (__builtin_stdc_first_leading_zero ((unsigned __int128) 0), unsigned int)
      || __builtin_stdc_first_leading_zero (~(unsigned __int128) 0) != 0)
    __builtin_abort ();
  if (__builtin_stdc_first_leading_one ((unsigned __int128) 0) != 0
      || !expr_has_type (__builtin_stdc_first_leading_one ((unsigned __int128) 0), unsigned int)
      || __builtin_stdc_first_leading_one (~(unsigned __int128) 0) != 1)
    __builtin_abort ();
  if (__builtin_stdc_first_trailing_zero ((unsigned __int128) 0) != 1
      || !expr_has_type (__builtin_stdc_first_trailing_zero ((unsigned __int128) 0), unsigned int)
      || __builtin_stdc_first_trailing_zero (~(unsigned __int128) 0) != 0)
    __builtin_abort ();
  if (__builtin_stdc_first_trailing_one ((unsigned __int128) 0) != 0
      || !expr_has_type (__builtin_stdc_first_trailing_one ((unsigned __int128) 0), unsigned int)
      || __builtin_stdc_first_trailing_one (~(unsigned __int128) 0) != 1)
    __builtin_abort ();
  if (__builtin_stdc_count_zeros ((unsigned __int128) 0) != __SIZEOF_INT128__ * __CHAR_BIT__
      || !expr_has_type (__builtin_stdc_count_zeros ((unsigned __int128) 0), unsigned int)
      || __builtin_stdc_count_zeros (~(unsigned __int128) 0) != 0)
    __builtin_abort ();
  if (__builtin_stdc_count_ones ((unsigned __int128) 0) != 0
      || !expr_has_type (__builtin_stdc_count_ones ((unsigned __int128) 0), unsigned int)
      || __builtin_stdc_count_ones (~(unsigned __int128) 0) != __SIZEOF_INT128__ * __CHAR_BIT__)
    __builtin_abort ();
  if (__builtin_stdc_has_single_bit ((unsigned __int128) 0)
      || !expr_has_type (__builtin_stdc_has_single_bit ((unsigned __int128) 0), _Bool)
      || __builtin_stdc_has_single_bit (~(unsigned __int128) 0))
    __builtin_abort ();
  if (__builtin_stdc_bit_width ((unsigned __int128) 0) != 0
      || !expr_has_type (__builtin_stdc_bit_width ((unsigned __int128) 0), unsigned int)
      || __builtin_stdc_bit_width (~(unsigned __int128) 0) != __SIZEOF_INT128__ * __CHAR_BIT__)
    __builtin_abort ();
  if (__builtin_stdc_bit_floor ((unsigned __int128) 0) != 0
      || !expr_has_type (__builtin_stdc_bit_floor ((unsigned __int128) 0), unsigned __int128)
      || __builtin_stdc_bit_floor (~(unsigned __int128) 0) != ((unsigned __int128) 1) << (__SIZEOF_INT128__ * __CHAR_BIT__ - 1))
    __builtin_abort ();
  if (__builtin_stdc_bit_ceil ((unsigned __int128) 0) != 1
      || !expr_has_type (__builtin_stdc_bit_ceil ((unsigned __int128) 0), unsigned __int128)
      || __builtin_stdc_bit_ceil ((unsigned __int128) 1) != 1
      || __builtin_stdc_bit_ceil ((~(unsigned __int128) 0) >> 1) != ((unsigned __int128) 1) << (__SIZEOF_INT128__ * __CHAR_BIT__ - 1)
      || __builtin_stdc_bit_ceil (~(unsigned __int128) 0) != 0)
    __builtin_abort ();
#endif
#if __has_builtin (__builtin_stdc_leading_zeros) != 1
#error __builtin_stdc_leading_zeros not implemented
#endif
#if __has_builtin (__builtin_stdc_leading_ones) != 1
#error __builtin_stdc_leading_ones not implemented
#endif
#if __has_builtin (__builtin_stdc_trailing_zeros) != 1
#error __builtin_stdc_trailing_zeros not implemented
#endif
#if __has_builtin (__builtin_stdc_trailing_ones) != 1
#error __builtin_stdc_trailing_ones not implemented
#endif
#if __has_builtin (__builtin_stdc_first_leading_zero) != 1
#error __builtin_stdc_first_leading_zero not implemented
#endif
#if __has_builtin (__builtin_stdc_first_leading_one) != 1
#error __builtin_stdc_first_leading_one not implemented
#endif
#if __has_builtin (__builtin_stdc_first_trailing_zero) != 1
#error __builtin_stdc_first_trailing_zero not implemented
#endif
#if __has_builtin (__builtin_stdc_first_trailing_one) != 1
#error __builtin_stdc_first_trailing_one not implemented
#endif
#if __has_builtin (__builtin_stdc_count_zeros) != 1
#error __builtin_stdc_count_zeros not implemented
#endif
#if __has_builtin (__builtin_stdc_count_ones) != 1
#error __builtin_stdc_count_ones not implemented
#endif
#if __has_builtin (__builtin_stdc_has_single_bit) != 1
#error __builtin_stdc_single_bit not implemented
#endif
#if __has_builtin (__builtin_stdc_bit_width) != 1
#error __builtin_stdc_bit_width not implemented
#endif
#if __has_builtin (__builtin_stdc_bit_floor) != 1
#error __builtin_stdc_bit_floor not implemented
#endif
#if __has_builtin (__builtin_stdc_bit_ceil) != 1
#error __builtin_stdc_bit_ceil not implemented
#endif
  unsigned char a = 0;
  if (__builtin_stdc_bit_width (a++) != 0 || a != 1)
    __builtin_abort ();
  unsigned long long b = 0;
  if (__builtin_stdc_bit_width (b++) != 0 || b != 1)
    __builtin_abort ();
  if (__builtin_stdc_bit_floor (a++) != 1 || a != 2)
    __builtin_abort ();
  if (__builtin_stdc_bit_floor (b++) != 1 || b != 2)
    __builtin_abort ();
  if (__builtin_stdc_bit_ceil (a++) != 2 || a != 3)
    __builtin_abort ();
  if (__builtin_stdc_bit_ceil (b++) != 2 || b != 3)
    __builtin_abort ();
  if (__builtin_stdc_leading_zeros (a++) != __CHAR_BIT__ - 2 || a != 4)
    __builtin_abort ();
  if (__builtin_stdc_leading_zeros (b++) != __SIZEOF_LONG_LONG__ * __CHAR_BIT__ - 2 || b != 4)
    __builtin_abort ();
  if (__builtin_stdc_leading_ones (a++) != 0 || a != 5)
    __builtin_abort ();
  if (__builtin_stdc_leading_ones (b++) != 0 || b != 5)
    __builtin_abort ();
  if (__builtin_stdc_trailing_zeros (a++) != 0 || a != 6)
    __builtin_abort ();
  if (__builtin_stdc_trailing_zeros (b++) != 0 || b != 6)
    __builtin_abort ();
  if (__builtin_stdc_trailing_ones (a++) != 0 || a != 7)
    __builtin_abort ();
  if (__builtin_stdc_trailing_ones (b++) != 0 || b != 7)
    __builtin_abort ();
  if (__builtin_stdc_first_leading_zero (a++) != 1 || a != 8)
    __builtin_abort ();
  if (__builtin_stdc_first_leading_zero (b++) != 1 || b != 8)
    __builtin_abort ();
  if (__builtin_stdc_first_leading_one (a++) != __CHAR_BIT__ - 3 || a != 9)
    __builtin_abort ();
  if (__builtin_stdc_first_leading_one (b++) != __SIZEOF_LONG_LONG__ * __CHAR_BIT__ - 3 || b != 9)
    __builtin_abort ();
  if (__builtin_stdc_first_trailing_zero (a++) != 2 || a != 10)
    __builtin_abort ();
  if (__builtin_stdc_first_trailing_zero (b++) != 2 || b != 10)
    __builtin_abort ();
  if (__builtin_stdc_first_trailing_one (a++) != 2 || a != 11)
    __builtin_abort ();
  if (__builtin_stdc_first_trailing_one (b++) != 2 || b != 11)
    __builtin_abort ();
  if (__builtin_stdc_count_zeros (a++) != __CHAR_BIT__ - 3 || a != 12)
    __builtin_abort ();
  if (__builtin_stdc_count_zeros (b++) != __SIZEOF_LONG_LONG__ * __CHAR_BIT__ - 3 || b != 12)
    __builtin_abort ();
  if (__builtin_stdc_count_ones (a++) != 2 || a != 13)
    __builtin_abort ();
  if (__builtin_stdc_count_ones (b++) != 2 || b != 13)
    __builtin_abort ();
  if (__builtin_stdc_has_single_bit (a++) || a != 14)
    __builtin_abort ();
  if (__builtin_stdc_has_single_bit (b++) || b != 14)
    __builtin_abort ();
#if __BITINT_MAXWIDTH__ >= 64
  if (__builtin_stdc_leading_zeros (0uwb) != 1
      || !expr_has_type (__builtin_stdc_leading_zeros (0uwb), unsigned int)
      || __builtin_stdc_leading_zeros (1uwb) != 0
      || !expr_has_type (__builtin_stdc_leading_zeros (1uwb), unsigned int))
    __builtin_abort ();
  if (__builtin_stdc_leading_ones (0uwb) != 0
      || !expr_has_type (__builtin_stdc_leading_ones (0uwb), unsigned int)
      || __builtin_stdc_leading_ones (1uwb) != 1
      || !expr_has_type (__builtin_stdc_leading_ones (1uwb), unsigned int))
    __builtin_abort ();
  if (__builtin_stdc_trailing_zeros (0uwb) != 1
      || !expr_has_type (__builtin_stdc_trailing_zeros (0uwb), unsigned int)
      || __builtin_stdc_trailing_zeros (1uwb) != 0
      || !expr_has_type (__builtin_stdc_trailing_zeros (1uwb), unsigned int))
    __builtin_abort ();
  if (__builtin_stdc_trailing_ones (0uwb) != 0
      || !expr_has_type (__builtin_stdc_trailing_ones (0uwb), unsigned int)
      || __builtin_stdc_trailing_ones (1uwb) != 1
      || !expr_has_type (__builtin_stdc_trailing_ones (1uwb), unsigned int))
    __builtin_abort ();
  if (__builtin_stdc_first_leading_zero (0uwb) != 1
      || !expr_has_type (__builtin_stdc_first_leading_zero (0uwb), unsigned int)
      || __builtin_stdc_first_leading_zero (1uwb) != 0
      || !expr_has_type (__builtin_stdc_first_leading_zero (1uwb), unsigned int))
    __builtin_abort ();
  if (__builtin_stdc_first_leading_one (0uwb) != 0
      || !expr_has_type (__builtin_stdc_first_leading_one (0uwb), unsigned int)
      || __builtin_stdc_first_leading_one (1uwb) != 1
      || !expr_has_type (__builtin_stdc_first_leading_one (1uwb), unsigned int))
    __builtin_abort ();
  if (__builtin_stdc_first_trailing_zero (0uwb) != 1
      || !expr_has_type (__builtin_stdc_first_trailing_zero (0uwb), unsigned int)
      || __builtin_stdc_first_trailing_zero (1uwb) != 0
      || !expr_has_type (__builtin_stdc_first_trailing_zero (1uwb), unsigned int))
    __builtin_abort ();
  if (__builtin_stdc_first_trailing_one (0uwb) != 0
      || !expr_has_type (__builtin_stdc_first_trailing_one (0uwb), unsigned int)
      || __builtin_stdc_first_trailing_one (1uwb) != 1
      || !expr_has_type (__builtin_stdc_first_trailing_one (1uwb), unsigned int))
    __builtin_abort ();
  if (__builtin_stdc_count_zeros (0uwb) != 1
      || !expr_has_type (__builtin_stdc_count_zeros (0uwb), unsigned int)
      || __builtin_stdc_count_zeros (1uwb) != 0
      || !expr_has_type (__builtin_stdc_count_zeros (1uwb), unsigned int))
    __builtin_abort ();
  if (__builtin_stdc_count_ones (0uwb) != 0
      || !expr_has_type (__builtin_stdc_count_ones (0uwb), unsigned int)
      || __builtin_stdc_count_ones (1uwb) != 1
      || !expr_has_type (__builtin_stdc_count_ones (1uwb), unsigned int))
    __builtin_abort ();
  if (__builtin_stdc_has_single_bit (0uwb)
      || !expr_has_type (__builtin_stdc_has_single_bit (0uwb), _Bool)
      || !__builtin_stdc_has_single_bit (1uwb)
      || !expr_has_type (__builtin_stdc_has_single_bit (1uwb), _Bool))
    __builtin_abort ();
  if (__builtin_stdc_bit_width (0uwb) != 0
      || !expr_has_type (__builtin_stdc_bit_width (0uwb), unsigned int)
      || __builtin_stdc_bit_width (1uwb) != 1
      || !expr_has_type (__builtin_stdc_bit_width (1uwb), unsigned int))
    __builtin_abort ();
  if (__builtin_stdc_bit_floor (0uwb) != 0
      || !expr_has_type (__builtin_stdc_bit_floor (0uwb), unsigned _BitInt(1))
      || __builtin_stdc_bit_floor (1uwb) != 1
      || !expr_has_type (__builtin_stdc_bit_floor (1uwb), unsigned _BitInt(1)))
    __builtin_abort ();
  if (__builtin_stdc_bit_ceil (0uwb) != 1
      || !expr_has_type (__builtin_stdc_bit_ceil (0uwb), unsigned _BitInt(1))
      || __builtin_stdc_bit_ceil (1uwb) != 1
      || !expr_has_type (__builtin_stdc_bit_ceil (1uwb), unsigned _BitInt(1)))
    __builtin_abort ();
  unsigned _BitInt(1) c = 0;
  if (__builtin_stdc_bit_floor (c++) != 0 || c != 1)
    __builtin_abort ();
  if (__builtin_stdc_bit_floor (c++) != 1 || c != 0)
    __builtin_abort ();
  if (__builtin_stdc_bit_ceil (c++) != 1 || c != 1)
    __builtin_abort ();
  if (__builtin_stdc_bit_ceil (c++) != 1 || c != 0)
    __builtin_abort ();
#endif
#if __BITINT_MAXWIDTH__ >= 512
  if (__builtin_stdc_leading_zeros ((unsigned _BitInt(512)) 0) != 512
      || !expr_has_type (__builtin_stdc_leading_zeros ((unsigned _BitInt(512)) 0), unsigned int)
      || __builtin_stdc_leading_zeros ((unsigned _BitInt(373)) 0) != 373
      || !expr_has_type (__builtin_stdc_leading_zeros ((unsigned _BitInt(373)) 0), unsigned int))
    __builtin_abort ();
  if (__builtin_stdc_leading_zeros (~(unsigned _BitInt(512)) 0) != 0
      || __builtin_stdc_leading_zeros (~(unsigned _BitInt(373)) 0) != 0)
    __builtin_abort ();
  if (__builtin_stdc_leading_zeros ((unsigned _BitInt(512)) 275) != 512 - 9
      || __builtin_stdc_leading_zeros ((unsigned _BitInt(373)) 512) != 373 - 10)
    __builtin_abort ();
  if (__builtin_stdc_leading_ones ((unsigned _BitInt(512)) 0) != 0
      || !expr_has_type (__builtin_stdc_leading_ones ((unsigned _BitInt(512)) 0), unsigned int)
      || __builtin_stdc_leading_ones ((unsigned _BitInt(373)) 0) != 0
      || !expr_has_type (__builtin_stdc_leading_ones ((unsigned _BitInt(373)) 0), unsigned int))
    __builtin_abort ();
  if (__builtin_stdc_leading_ones (~(unsigned _BitInt(512)) 0) != 512
      || __builtin_stdc_leading_ones (~(unsigned _BitInt(373)) 0) != 373)
    __builtin_abort ();
  if (__builtin_stdc_leading_ones (~(unsigned _BitInt(512)) 275) != 512 - 9
      || __builtin_stdc_leading_ones (~(unsigned _BitInt(373)) 512) != 373 - 10)
    __builtin_abort ();
  if (__builtin_stdc_trailing_zeros ((unsigned _BitInt(512)) 0) != 512
      || !expr_has_type (__builtin_stdc_trailing_zeros ((unsigned _BitInt(512)) 0), unsigned int)
      || __builtin_stdc_trailing_zeros ((unsigned _BitInt(373)) 0) != 373
      || !expr_has_type (__builtin_stdc_trailing_zeros ((unsigned _BitInt(373)) 0), unsigned int))
    __builtin_abort ();
  if (__builtin_stdc_trailing_zeros (~(unsigned _BitInt(512)) 0) != 0
      || __builtin_stdc_trailing_zeros (~(unsigned _BitInt(373)) 0) != 0)
    __builtin_abort ();
  if (__builtin_stdc_trailing_zeros ((unsigned _BitInt(512)) 256) != 8
      || __builtin_stdc_trailing_zeros ((unsigned _BitInt(373)) 512) != 9)
    __builtin_abort ();
  if (__builtin_stdc_trailing_ones ((unsigned _BitInt(512)) 0) != 0
      || !expr_has_type (__builtin_stdc_trailing_ones ((unsigned _BitInt(512)) 0), unsigned int)
      || __builtin_stdc_trailing_ones ((unsigned _BitInt(373)) 0) != 0
      || !expr_has_type (__builtin_stdc_trailing_ones ((unsigned _BitInt(373)) 0), unsigned int))
    __builtin_abort ();
  if (__builtin_stdc_trailing_ones (~(unsigned _BitInt(512)) 0) != 512
      || __builtin_stdc_trailing_ones (~(unsigned _BitInt(373)) 0) != 373)
    __builtin_abort ();
  if (__builtin_stdc_trailing_ones ((unsigned _BitInt(512)) 255) != 8
      || __builtin_stdc_trailing_ones ((~(unsigned _BitInt(373)) 0) >> 2) != 373 - 2)
    __builtin_abort ();
  if (__builtin_stdc_first_leading_zero ((unsigned _BitInt(512)) 0) != 1
      || !expr_has_type (__builtin_stdc_first_leading_zero ((unsigned _BitInt(512)) 0), unsigned int)
      || __builtin_stdc_first_leading_zero ((unsigned _BitInt(373)) 0) != 1
      || !expr_has_type (__builtin_stdc_first_leading_zero ((unsigned _BitInt(373)) 0), unsigned int))
    __builtin_abort ();
  if (__builtin_stdc_first_leading_zero (~(unsigned _BitInt(512)) 0) != 0
      || __builtin_stdc_first_leading_zero (~(unsigned _BitInt(373)) 0) != 0)
    __builtin_abort ();
  if (__builtin_stdc_first_leading_zero (~(unsigned _BitInt(512)) 511) != 512 - 8
      || __builtin_stdc_first_leading_zero (~(unsigned _BitInt(373)) 1023) != 373 - 9)
    __builtin_abort ();
  if (__builtin_stdc_first_leading_one ((unsigned _BitInt(512)) 0) != 0
      || !expr_has_type (__builtin_stdc_first_leading_one ((unsigned _BitInt(512)) 0), unsigned int)
      || __builtin_stdc_first_leading_one ((unsigned _BitInt(373)) 0) != 0
      || !expr_has_type (__builtin_stdc_first_leading_one ((unsigned _BitInt(373)) 0), unsigned int))
    __builtin_abort ();
  if (__builtin_stdc_first_leading_one (~(unsigned _BitInt(512)) 0) != 1
      || __builtin_stdc_first_leading_one (~(unsigned _BitInt(373)) 0) != 1)
    __builtin_abort ();
  if (__builtin_stdc_first_leading_one ((unsigned _BitInt(512)) 275) != 512 - 8
      || __builtin_stdc_first_leading_one ((unsigned _BitInt(373)) 512) != 373 - 9)
    __builtin_abort ();
  if (__builtin_stdc_first_trailing_zero ((unsigned _BitInt(512)) 0) != 1
      || !expr_has_type (__builtin_stdc_first_trailing_zero ((unsigned _BitInt(512)) 0), unsigned int)
      || __builtin_stdc_first_trailing_zero ((unsigned _BitInt(373)) 0) != 1
      || !expr_has_type (__builtin_stdc_first_trailing_zero ((unsigned _BitInt(373)) 0), unsigned int))
    __builtin_abort ();
  if (__builtin_stdc_first_trailing_zero (~(unsigned _BitInt(512)) 0) != 0
      || __builtin_stdc_first_trailing_zero (~(unsigned _BitInt(373)) 0) != 0)
    __builtin_abort ();
  if (__builtin_stdc_first_trailing_zero ((unsigned _BitInt(512)) 255) != 9
      || __builtin_stdc_first_trailing_zero ((unsigned _BitInt(373)) 511) != 10)
    __builtin_abort ();
  if (__builtin_stdc_first_trailing_one ((unsigned _BitInt(512)) 0) != 0
      || !expr_has_type (__builtin_stdc_first_trailing_one ((unsigned _BitInt(512)) 0), unsigned int)
      || __builtin_stdc_first_trailing_one ((unsigned _BitInt(373)) 0) != 0
      || !expr_has_type (__builtin_stdc_first_trailing_one ((unsigned _BitInt(373)) 0), unsigned int))
    __builtin_abort ();
  if (__builtin_stdc_first_trailing_one (~(unsigned _BitInt(512)) 0) != 1
      || __builtin_stdc_first_trailing_one (~(unsigned _BitInt(373)) 0) != 1)
    __builtin_abort ();
  if (__builtin_stdc_first_trailing_one (((unsigned _BitInt(512)) 255) << 175) != 176
      || __builtin_stdc_first_trailing_one ((~(unsigned _BitInt(373)) 0) << 311) != 312)
    __builtin_abort ();
  if (__builtin_stdc_count_zeros ((unsigned _BitInt(512)) 0) != 512
      || !expr_has_type (__builtin_stdc_count_zeros ((unsigned _BitInt(512)) 0), unsigned int)
      || __builtin_stdc_count_zeros ((unsigned _BitInt(373)) 0) != 373
      || !expr_has_type (__builtin_stdc_count_zeros ((unsigned _BitInt(373)) 0), unsigned int))
    __builtin_abort ();
  if (__builtin_stdc_count_zeros (~(unsigned _BitInt(512)) 0) != 0
      || __builtin_stdc_count_zeros (~(unsigned _BitInt(373)) 0) != 0)
    __builtin_abort ();
  if (__builtin_stdc_count_zeros ((unsigned _BitInt(512)) 1315) != 512 - 5
      || __builtin_stdc_count_zeros ((unsigned _BitInt(373)) 3363) != 373 - 6)
    __builtin_abort ();
  if (__builtin_stdc_count_ones ((unsigned _BitInt(512)) 0) != 0
      || !expr_has_type (__builtin_stdc_count_ones ((unsigned _BitInt(512)) 0), unsigned int)
      || __builtin_stdc_count_ones ((unsigned _BitInt(373)) 0) != 0
      || !expr_has_type (__builtin_stdc_count_ones ((unsigned _BitInt(373)) 0), unsigned int))
    __builtin_abort ();
  if (__builtin_stdc_count_ones (~(unsigned _BitInt(512)) 0) != 512
      || __builtin_stdc_count_ones (~(unsigned _BitInt(373)) 0) != 373)
    __builtin_abort ();
  if (__builtin_stdc_count_ones (~(unsigned _BitInt(512)) 1315) != 512 - 5
      || __builtin_stdc_count_ones (~(unsigned _BitInt(373)) 3363) != 373 - 6)
    __builtin_abort ();
  if (__builtin_stdc_has_single_bit ((unsigned _BitInt(512)) 0)
      || !expr_has_type (__builtin_stdc_has_single_bit ((unsigned _BitInt(512)) 0), _Bool)
      || __builtin_stdc_has_single_bit ((unsigned _BitInt(373)) 0)
      || !expr_has_type (__builtin_stdc_has_single_bit ((unsigned _BitInt(373)) 0), _Bool))
    __builtin_abort ();
  if (__builtin_stdc_has_single_bit (~(unsigned _BitInt(512)) 0)
      || __builtin_stdc_has_single_bit (~(unsigned _BitInt(373)) 0))
    __builtin_abort ();
  if (__builtin_stdc_has_single_bit (((unsigned _BitInt(512)) 1022) << 279)
      || __builtin_stdc_has_single_bit (((unsigned _BitInt(373)) 12) << 305))
    __builtin_abort ();
  if (__builtin_stdc_bit_width ((unsigned _BitInt(512)) 0) != 0
      || !expr_has_type (__builtin_stdc_bit_width ((unsigned _BitInt(512)) 0), unsigned int)
      || __builtin_stdc_bit_width ((unsigned _BitInt(373)) 0) != 0
      || !expr_has_type (__builtin_stdc_bit_width ((unsigned _BitInt(373)) 0), unsigned int))
    __builtin_abort ();
  if (__builtin_stdc_bit_width (~(unsigned _BitInt(512)) 0) != 512
      || __builtin_stdc_bit_width (~(unsigned _BitInt(373)) 0) != 373)
    __builtin_abort ();
  if (__builtin_stdc_bit_width (((unsigned _BitInt(512)) 1023) << 405) != 405 + 10
      || __builtin_stdc_bit_width (((unsigned _BitInt(373)) 1024) << 242) != 242 + 11)
    __builtin_abort ();
  if (__builtin_stdc_bit_floor ((unsigned _BitInt(512)) 0) != 0
      || !expr_has_type (__builtin_stdc_bit_floor ((unsigned _BitInt(512)) 0), unsigned _BitInt(512))
      || __builtin_stdc_bit_floor ((unsigned _BitInt(373)) 0) != 0
      || !expr_has_type (__builtin_stdc_bit_floor ((unsigned _BitInt(373)) 0), unsigned _BitInt(373)))
    __builtin_abort ();
  if (__builtin_stdc_bit_floor (~(unsigned _BitInt(512)) 0) != ((unsigned _BitInt(512)) 1) << (512 - 1)
      || __builtin_stdc_bit_floor (~(unsigned _BitInt(373)) 0) != ((unsigned _BitInt(373)) 1) << (373 - 1))
    __builtin_abort ();
  if (__builtin_stdc_bit_floor (((unsigned _BitInt(512)) 511) << 405) != (((unsigned _BitInt(512)) 256) << 405)
      || __builtin_stdc_bit_floor (((unsigned _BitInt(373)) 512) << 242) != (((unsigned _BitInt(373)) 512) << 242))
    __builtin_abort ();
  if (__builtin_stdc_bit_ceil ((unsigned _BitInt(512)) 0) != 1
      || !expr_has_type (__builtin_stdc_bit_ceil ((unsigned _BitInt(512)) 0), unsigned _BitInt(512))
      || __builtin_stdc_bit_ceil ((unsigned _BitInt(373)) 0) != 1
      || !expr_has_type (__builtin_stdc_bit_ceil ((unsigned _BitInt(373)) 0), unsigned _BitInt(373)))
    __builtin_abort ();
  if (__builtin_stdc_bit_ceil (~(unsigned _BitInt(512)) 0) != 0
      || __builtin_stdc_bit_ceil (~(unsigned _BitInt(373)) 0) != 0)
    __builtin_abort ();
  if (__builtin_stdc_bit_ceil (((unsigned _BitInt(512)) 1) << (512 - 1)) != ((unsigned _BitInt(512)) 1) << (512 - 1)
      || __builtin_stdc_bit_ceil ((~(unsigned _BitInt(373)) 0) >> 1) != ((unsigned _BitInt(373)) 1) << (373 - 1))
    __builtin_abort ();
  if (__builtin_stdc_bit_ceil (((unsigned _BitInt(512)) 512) << 405) != (((unsigned _BitInt(512)) 512) << 405)
      || __builtin_stdc_bit_ceil (((unsigned _BitInt(373)) 513) << 242) != (((unsigned _BitInt(373)) 1024) << 242))
    __builtin_abort ();
  if (__builtin_stdc_bit_floor ((unsigned _BitInt(__BITINT_MAXWIDTH__)) 0) != 0)
    __builtin_abort ();
  if (__builtin_stdc_bit_floor (~(unsigned _BitInt(__BITINT_MAXWIDTH__)) 0) != ((unsigned _BitInt(__BITINT_MAXWIDTH__)) 1) << (__BITINT_MAXWIDTH__ - 1))
    __builtin_abort ();
  if (__builtin_stdc_bit_floor (((unsigned _BitInt(__BITINT_MAXWIDTH__)) 511) << 405) != (((unsigned _BitInt(__BITINT_MAXWIDTH__)) 256) << 405)
      || __builtin_stdc_bit_floor (((unsigned _BitInt(__BITINT_MAXWIDTH__)) 512) << 405) != (((unsigned _BitInt(__BITINT_MAXWIDTH__)) 512) << 405))
    __builtin_abort ();
  if (__builtin_stdc_bit_ceil ((unsigned _BitInt(__BITINT_MAXWIDTH__)) 0) != 1)
    __builtin_abort ();
  if (__builtin_stdc_bit_ceil (~(unsigned _BitInt(__BITINT_MAXWIDTH__)) 0) != 0)
    __builtin_abort ();
  if (__builtin_stdc_bit_ceil (((unsigned _BitInt(__BITINT_MAXWIDTH__)) 1) << (__BITINT_MAXWIDTH__ - 1)) != ((unsigned _BitInt(__BITINT_MAXWIDTH__)) 1) << (__BITINT_MAXWIDTH__ - 1))
    __builtin_abort ();
  if (__builtin_stdc_bit_ceil (((unsigned _BitInt(__BITINT_MAXWIDTH__)) 512) << 405) != (((unsigned _BitInt(__BITINT_MAXWIDTH__)) 512) << 405)
      || __builtin_stdc_bit_ceil (((unsigned _BitInt(__BITINT_MAXWIDTH__)) 513) << 405) != (((unsigned _BitInt(__BITINT_MAXWIDTH__)) 1024) << 405))
    __builtin_abort ();
#endif
}

// SLATE-FILECHECK-STD DEFAULT c11
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
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_leading_zeros:[0-9]+]] @__builtin_stdc_leading_zeros(%[[VALUE0:[0-9]+]] <unnamed>: u8) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_leading_zeros_2:[0-9]+]] @__builtin_stdc_leading_zeros(%[[VALUE1:[0-9]+]] <unnamed>: u16) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_leading_zeros_3:[0-9]+]] @__builtin_stdc_leading_zeros(%[[VALUE2:[0-9]+]] <unnamed>: u32) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_leading_zeros_4:[0-9]+]] @__builtin_stdc_leading_zeros(%[[VALUE3:[0-9]+]] <unnamed>: u64) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_leading_zeros_5:[0-9]+]] @__builtin_stdc_leading_zeros(%[[VALUE4:[0-9]+]] <unnamed>: u64) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_leading_zeros:[0-9]+]] @leading_zeros(%[[VALUE_a:[0-9]+]] a: u8, %[[VALUE_b:[0-9]+]] b: u16, %[[VALUE_c:[0-9]+]] c: u32, %[[VALUE_d:[0-9]+]] d: u64, %[[VALUE_e:[0-9]+]] e: u64) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(call<u32, signature=fn(u8) -> u32>(%[[VALUE___builtin_stdc_leading_zeros]], read<u8>(%[[VALUE_a]])), call<u32, signature=fn(u16) -> u32>(%[[VALUE___builtin_stdc_leading_zeros_2]], read<u16>(%[[VALUE_b]]))), call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_leading_zeros_3]], read<u32>(%[[VALUE_c]]))), call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_leading_zeros_4]], read<u64>(%[[VALUE_d]]))), call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_leading_zeros_5]], read<u64>(%[[VALUE_e]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_leading_ones:[0-9]+]] @__builtin_stdc_leading_ones(%[[VALUE5:[0-9]+]] <unnamed>: u8) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_leading_ones_2:[0-9]+]] @__builtin_stdc_leading_ones(%[[VALUE6:[0-9]+]] <unnamed>: u16) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_leading_ones_3:[0-9]+]] @__builtin_stdc_leading_ones(%[[VALUE7:[0-9]+]] <unnamed>: u32) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_leading_ones_4:[0-9]+]] @__builtin_stdc_leading_ones(%[[VALUE8:[0-9]+]] <unnamed>: u64) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_leading_ones_5:[0-9]+]] @__builtin_stdc_leading_ones(%[[VALUE9:[0-9]+]] <unnamed>: u64) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_leading_ones:[0-9]+]] @leading_ones(%[[VALUE_a_2:[0-9]+]] a: u8, %[[VALUE_b_2:[0-9]+]] b: u16, %[[VALUE_c_2:[0-9]+]] c: u32, %[[VALUE_d_2:[0-9]+]] d: u64, %[[VALUE_e_2:[0-9]+]] e: u64) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(call<u32, signature=fn(u8) -> u32>(%[[VALUE___builtin_stdc_leading_ones]], read<u8>(%[[VALUE_a_2]])), call<u32, signature=fn(u16) -> u32>(%[[VALUE___builtin_stdc_leading_ones_2]], read<u16>(%[[VALUE_b_2]]))), call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_leading_ones_3]], read<u32>(%[[VALUE_c_2]]))), call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_leading_ones_4]], read<u64>(%[[VALUE_d_2]]))), call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_leading_ones_5]], read<u64>(%[[VALUE_e_2]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_trailing_zeros:[0-9]+]] @__builtin_stdc_trailing_zeros(%[[VALUE10:[0-9]+]] <unnamed>: u8) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_trailing_zeros_2:[0-9]+]] @__builtin_stdc_trailing_zeros(%[[VALUE11:[0-9]+]] <unnamed>: u16) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_trailing_zeros_3:[0-9]+]] @__builtin_stdc_trailing_zeros(%[[VALUE12:[0-9]+]] <unnamed>: u32) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_trailing_zeros_4:[0-9]+]] @__builtin_stdc_trailing_zeros(%[[VALUE13:[0-9]+]] <unnamed>: u64) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_trailing_zeros_5:[0-9]+]] @__builtin_stdc_trailing_zeros(%[[VALUE14:[0-9]+]] <unnamed>: u64) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_trailing_zeros:[0-9]+]] @trailing_zeros(%[[VALUE_a_3:[0-9]+]] a: u8, %[[VALUE_b_3:[0-9]+]] b: u16, %[[VALUE_c_3:[0-9]+]] c: u32, %[[VALUE_d_3:[0-9]+]] d: u64, %[[VALUE_e_3:[0-9]+]] e: u64) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(call<u32, signature=fn(u8) -> u32>(%[[VALUE___builtin_stdc_trailing_zeros]], read<u8>(%[[VALUE_a_3]])), call<u32, signature=fn(u16) -> u32>(%[[VALUE___builtin_stdc_trailing_zeros_2]], read<u16>(%[[VALUE_b_3]]))), call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_trailing_zeros_3]], read<u32>(%[[VALUE_c_3]]))), call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_trailing_zeros_4]], read<u64>(%[[VALUE_d_3]]))), call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_trailing_zeros_5]], read<u64>(%[[VALUE_e_3]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_trailing_ones:[0-9]+]] @__builtin_stdc_trailing_ones(%[[VALUE15:[0-9]+]] <unnamed>: u8) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_trailing_ones_2:[0-9]+]] @__builtin_stdc_trailing_ones(%[[VALUE16:[0-9]+]] <unnamed>: u16) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_trailing_ones_3:[0-9]+]] @__builtin_stdc_trailing_ones(%[[VALUE17:[0-9]+]] <unnamed>: u32) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_trailing_ones_4:[0-9]+]] @__builtin_stdc_trailing_ones(%[[VALUE18:[0-9]+]] <unnamed>: u64) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_trailing_ones_5:[0-9]+]] @__builtin_stdc_trailing_ones(%[[VALUE19:[0-9]+]] <unnamed>: u64) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_trailing_ones:[0-9]+]] @trailing_ones(%[[VALUE_a_4:[0-9]+]] a: u8, %[[VALUE_b_4:[0-9]+]] b: u16, %[[VALUE_c_4:[0-9]+]] c: u32, %[[VALUE_d_4:[0-9]+]] d: u64, %[[VALUE_e_4:[0-9]+]] e: u64) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(call<u32, signature=fn(u8) -> u32>(%[[VALUE___builtin_stdc_trailing_ones]], read<u8>(%[[VALUE_a_4]])), call<u32, signature=fn(u16) -> u32>(%[[VALUE___builtin_stdc_trailing_ones_2]], read<u16>(%[[VALUE_b_4]]))), call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_trailing_ones_3]], read<u32>(%[[VALUE_c_4]]))), call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_trailing_ones_4]], read<u64>(%[[VALUE_d_4]]))), call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_trailing_ones_5]], read<u64>(%[[VALUE_e_4]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_first_leading_zero:[0-9]+]] @__builtin_stdc_first_leading_zero(%[[VALUE20:[0-9]+]] <unnamed>: u8) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_first_leading_zero_2:[0-9]+]] @__builtin_stdc_first_leading_zero(%[[VALUE21:[0-9]+]] <unnamed>: u16) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_first_leading_zero_3:[0-9]+]] @__builtin_stdc_first_leading_zero(%[[VALUE22:[0-9]+]] <unnamed>: u32) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_first_leading_zero_4:[0-9]+]] @__builtin_stdc_first_leading_zero(%[[VALUE23:[0-9]+]] <unnamed>: u64) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_first_leading_zero_5:[0-9]+]] @__builtin_stdc_first_leading_zero(%[[VALUE24:[0-9]+]] <unnamed>: u64) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_first_leading_zero:[0-9]+]] @first_leading_zero(%[[VALUE_a_5:[0-9]+]] a: u8, %[[VALUE_b_5:[0-9]+]] b: u16, %[[VALUE_c_5:[0-9]+]] c: u32, %[[VALUE_d_5:[0-9]+]] d: u64, %[[VALUE_e_5:[0-9]+]] e: u64) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(call<u32, signature=fn(u8) -> u32>(%[[VALUE___builtin_stdc_first_leading_zero]], read<u8>(%[[VALUE_a_5]])), call<u32, signature=fn(u16) -> u32>(%[[VALUE___builtin_stdc_first_leading_zero_2]], read<u16>(%[[VALUE_b_5]]))), call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_first_leading_zero_3]], read<u32>(%[[VALUE_c_5]]))), call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_first_leading_zero_4]], read<u64>(%[[VALUE_d_5]]))), call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_first_leading_zero_5]], read<u64>(%[[VALUE_e_5]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_first_leading_one:[0-9]+]] @__builtin_stdc_first_leading_one(%[[VALUE25:[0-9]+]] <unnamed>: u8) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_first_leading_one_2:[0-9]+]] @__builtin_stdc_first_leading_one(%[[VALUE26:[0-9]+]] <unnamed>: u16) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_first_leading_one_3:[0-9]+]] @__builtin_stdc_first_leading_one(%[[VALUE27:[0-9]+]] <unnamed>: u32) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_first_leading_one_4:[0-9]+]] @__builtin_stdc_first_leading_one(%[[VALUE28:[0-9]+]] <unnamed>: u64) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_first_leading_one_5:[0-9]+]] @__builtin_stdc_first_leading_one(%[[VALUE29:[0-9]+]] <unnamed>: u64) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_first_leading_one:[0-9]+]] @first_leading_one(%[[VALUE_a_6:[0-9]+]] a: u8, %[[VALUE_b_6:[0-9]+]] b: u16, %[[VALUE_c_6:[0-9]+]] c: u32, %[[VALUE_d_6:[0-9]+]] d: u64, %[[VALUE_e_6:[0-9]+]] e: u64) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(call<u32, signature=fn(u8) -> u32>(%[[VALUE___builtin_stdc_first_leading_one]], read<u8>(%[[VALUE_a_6]])), call<u32, signature=fn(u16) -> u32>(%[[VALUE___builtin_stdc_first_leading_one_2]], read<u16>(%[[VALUE_b_6]]))), call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_first_leading_one_3]], read<u32>(%[[VALUE_c_6]]))), call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_first_leading_one_4]], read<u64>(%[[VALUE_d_6]]))), call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_first_leading_one_5]], read<u64>(%[[VALUE_e_6]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_first_trailing_zero:[0-9]+]] @__builtin_stdc_first_trailing_zero(%[[VALUE30:[0-9]+]] <unnamed>: u8) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_first_trailing_zero_2:[0-9]+]] @__builtin_stdc_first_trailing_zero(%[[VALUE31:[0-9]+]] <unnamed>: u16) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_first_trailing_zero_3:[0-9]+]] @__builtin_stdc_first_trailing_zero(%[[VALUE32:[0-9]+]] <unnamed>: u32) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_first_trailing_zero_4:[0-9]+]] @__builtin_stdc_first_trailing_zero(%[[VALUE33:[0-9]+]] <unnamed>: u64) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_first_trailing_zero_5:[0-9]+]] @__builtin_stdc_first_trailing_zero(%[[VALUE34:[0-9]+]] <unnamed>: u64) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_first_trailing_zero:[0-9]+]] @first_trailing_zero(%[[VALUE_a_7:[0-9]+]] a: u8, %[[VALUE_b_7:[0-9]+]] b: u16, %[[VALUE_c_7:[0-9]+]] c: u32, %[[VALUE_d_7:[0-9]+]] d: u64, %[[VALUE_e_7:[0-9]+]] e: u64) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(call<u32, signature=fn(u8) -> u32>(%[[VALUE___builtin_stdc_first_trailing_zero]], read<u8>(%[[VALUE_a_7]])), call<u32, signature=fn(u16) -> u32>(%[[VALUE___builtin_stdc_first_trailing_zero_2]], read<u16>(%[[VALUE_b_7]]))), call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_first_trailing_zero_3]], read<u32>(%[[VALUE_c_7]]))), call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_first_trailing_zero_4]], read<u64>(%[[VALUE_d_7]]))), call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_first_trailing_zero_5]], read<u64>(%[[VALUE_e_7]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_first_trailing_one:[0-9]+]] @__builtin_stdc_first_trailing_one(%[[VALUE35:[0-9]+]] <unnamed>: u8) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_first_trailing_one_2:[0-9]+]] @__builtin_stdc_first_trailing_one(%[[VALUE36:[0-9]+]] <unnamed>: u16) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_first_trailing_one_3:[0-9]+]] @__builtin_stdc_first_trailing_one(%[[VALUE37:[0-9]+]] <unnamed>: u32) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_first_trailing_one_4:[0-9]+]] @__builtin_stdc_first_trailing_one(%[[VALUE38:[0-9]+]] <unnamed>: u64) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_first_trailing_one_5:[0-9]+]] @__builtin_stdc_first_trailing_one(%[[VALUE39:[0-9]+]] <unnamed>: u64) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_first_trailing_one:[0-9]+]] @first_trailing_one(%[[VALUE_a_8:[0-9]+]] a: u8, %[[VALUE_b_8:[0-9]+]] b: u16, %[[VALUE_c_8:[0-9]+]] c: u32, %[[VALUE_d_8:[0-9]+]] d: u64, %[[VALUE_e_8:[0-9]+]] e: u64) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(call<u32, signature=fn(u8) -> u32>(%[[VALUE___builtin_stdc_first_trailing_one]], read<u8>(%[[VALUE_a_8]])), call<u32, signature=fn(u16) -> u32>(%[[VALUE___builtin_stdc_first_trailing_one_2]], read<u16>(%[[VALUE_b_8]]))), call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_first_trailing_one_3]], read<u32>(%[[VALUE_c_8]]))), call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_first_trailing_one_4]], read<u64>(%[[VALUE_d_8]]))), call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_first_trailing_one_5]], read<u64>(%[[VALUE_e_8]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_count_zeros:[0-9]+]] @__builtin_stdc_count_zeros(%[[VALUE40:[0-9]+]] <unnamed>: u8) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_count_zeros_2:[0-9]+]] @__builtin_stdc_count_zeros(%[[VALUE41:[0-9]+]] <unnamed>: u16) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_count_zeros_3:[0-9]+]] @__builtin_stdc_count_zeros(%[[VALUE42:[0-9]+]] <unnamed>: u32) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_count_zeros_4:[0-9]+]] @__builtin_stdc_count_zeros(%[[VALUE43:[0-9]+]] <unnamed>: u64) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_count_zeros_5:[0-9]+]] @__builtin_stdc_count_zeros(%[[VALUE44:[0-9]+]] <unnamed>: u64) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_count_zeros:[0-9]+]] @count_zeros(%[[VALUE_a_9:[0-9]+]] a: u8, %[[VALUE_b_9:[0-9]+]] b: u16, %[[VALUE_c_9:[0-9]+]] c: u32, %[[VALUE_d_9:[0-9]+]] d: u64, %[[VALUE_e_9:[0-9]+]] e: u64) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(call<u32, signature=fn(u8) -> u32>(%[[VALUE___builtin_stdc_count_zeros]], read<u8>(%[[VALUE_a_9]])), call<u32, signature=fn(u16) -> u32>(%[[VALUE___builtin_stdc_count_zeros_2]], read<u16>(%[[VALUE_b_9]]))), call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_count_zeros_3]], read<u32>(%[[VALUE_c_9]]))), call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_count_zeros_4]], read<u64>(%[[VALUE_d_9]]))), call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_count_zeros_5]], read<u64>(%[[VALUE_e_9]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_count_ones:[0-9]+]] @__builtin_stdc_count_ones(%[[VALUE45:[0-9]+]] <unnamed>: u8) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_count_ones_2:[0-9]+]] @__builtin_stdc_count_ones(%[[VALUE46:[0-9]+]] <unnamed>: u16) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_count_ones_3:[0-9]+]] @__builtin_stdc_count_ones(%[[VALUE47:[0-9]+]] <unnamed>: u32) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_count_ones_4:[0-9]+]] @__builtin_stdc_count_ones(%[[VALUE48:[0-9]+]] <unnamed>: u64) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_count_ones_5:[0-9]+]] @__builtin_stdc_count_ones(%[[VALUE49:[0-9]+]] <unnamed>: u64) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_count_ones:[0-9]+]] @count_ones(%[[VALUE_a_10:[0-9]+]] a: u8, %[[VALUE_b_10:[0-9]+]] b: u16, %[[VALUE_c_10:[0-9]+]] c: u32, %[[VALUE_d_10:[0-9]+]] d: u64, %[[VALUE_e_10:[0-9]+]] e: u64) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(call<u32, signature=fn(u8) -> u32>(%[[VALUE___builtin_stdc_count_ones]], read<u8>(%[[VALUE_a_10]])), call<u32, signature=fn(u16) -> u32>(%[[VALUE___builtin_stdc_count_ones_2]], read<u16>(%[[VALUE_b_10]]))), call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_count_ones_3]], read<u32>(%[[VALUE_c_10]]))), call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_count_ones_4]], read<u64>(%[[VALUE_d_10]]))), call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_count_ones_5]], read<u64>(%[[VALUE_e_10]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_has_single_bit:[0-9]+]] @__builtin_stdc_has_single_bit(%[[VALUE50:[0-9]+]] <unnamed>: u8) -> bool [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_has_single_bit_2:[0-9]+]] @__builtin_stdc_has_single_bit(%[[VALUE51:[0-9]+]] <unnamed>: u16) -> bool [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_has_single_bit_3:[0-9]+]] @__builtin_stdc_has_single_bit(%[[VALUE52:[0-9]+]] <unnamed>: u32) -> bool [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_has_single_bit_4:[0-9]+]] @__builtin_stdc_has_single_bit(%[[VALUE53:[0-9]+]] <unnamed>: u64) -> bool [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_has_single_bit_5:[0-9]+]] @__builtin_stdc_has_single_bit(%[[VALUE54:[0-9]+]] <unnamed>: u64) -> bool [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_has_single_bit:[0-9]+]] @has_single_bit(%[[VALUE_a_11:[0-9]+]] a: u8, %[[VALUE_b_11:[0-9]+]] b: u16, %[[VALUE_c_11:[0-9]+]] c: u32, %[[VALUE_d_11:[0-9]+]] d: u64, %[[VALUE_e_11:[0-9]+]] e: u64) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<u32, reason=return>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(call<bool, signature=fn(u8) -> bool>(%[[VALUE___builtin_stdc_has_single_bit]], read<u8>(%[[VALUE_a_11]])), call<bool, signature=fn(u16) -> bool>(%[[VALUE___builtin_stdc_has_single_bit_2]], read<u16>(%[[VALUE_b_11]]))), call<bool, signature=fn(u32) -> bool>(%[[VALUE___builtin_stdc_has_single_bit_3]], read<u32>(%[[VALUE_c_11]]))), call<bool, signature=fn(u64) -> bool>(%[[VALUE___builtin_stdc_has_single_bit_4]], read<u64>(%[[VALUE_d_11]]))), call<bool, signature=fn(u64) -> bool>(%[[VALUE___builtin_stdc_has_single_bit_5]], read<u64>(%[[VALUE_e_11]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_bit_width:[0-9]+]] @__builtin_stdc_bit_width(%[[VALUE55:[0-9]+]] <unnamed>: u8) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_bit_width_2:[0-9]+]] @__builtin_stdc_bit_width(%[[VALUE56:[0-9]+]] <unnamed>: u16) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_bit_width_3:[0-9]+]] @__builtin_stdc_bit_width(%[[VALUE57:[0-9]+]] <unnamed>: u32) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_bit_width_4:[0-9]+]] @__builtin_stdc_bit_width(%[[VALUE58:[0-9]+]] <unnamed>: u64) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_bit_width_5:[0-9]+]] @__builtin_stdc_bit_width(%[[VALUE59:[0-9]+]] <unnamed>: u64) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_bit_width:[0-9]+]] @bit_width(%[[VALUE_a_12:[0-9]+]] a: u8, %[[VALUE_b_12:[0-9]+]] b: u16, %[[VALUE_c_12:[0-9]+]] c: u32, %[[VALUE_d_12:[0-9]+]] d: u64, %[[VALUE_e_12:[0-9]+]] e: u64) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(call<u32, signature=fn(u8) -> u32>(%[[VALUE___builtin_stdc_bit_width]], read<u8>(%[[VALUE_a_12]])), call<u32, signature=fn(u16) -> u32>(%[[VALUE___builtin_stdc_bit_width_2]], read<u16>(%[[VALUE_b_12]]))), call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_bit_width_3]], read<u32>(%[[VALUE_c_12]]))), call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_bit_width_4]], read<u64>(%[[VALUE_d_12]]))), call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_bit_width_5]], read<u64>(%[[VALUE_e_12]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_bit_floor:[0-9]+]] @__builtin_stdc_bit_floor(%[[VALUE60:[0-9]+]] <unnamed>: u8) -> u8 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_bit_floor_2:[0-9]+]] @__builtin_stdc_bit_floor(%[[VALUE61:[0-9]+]] <unnamed>: u16) -> u16 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_bit_floor_3:[0-9]+]] @__builtin_stdc_bit_floor(%[[VALUE62:[0-9]+]] <unnamed>: u32) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_bit_floor_4:[0-9]+]] @__builtin_stdc_bit_floor(%[[VALUE63:[0-9]+]] <unnamed>: u64) -> u64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_bit_floor_5:[0-9]+]] @__builtin_stdc_bit_floor(%[[VALUE64:[0-9]+]] <unnamed>: u64) -> u64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_bit_floor:[0-9]+]] @bit_floor(%[[VALUE_a_13:[0-9]+]] a: u8, %[[VALUE_b_13:[0-9]+]] b: u16, %[[VALUE_c_13:[0-9]+]] c: u32, %[[VALUE_d_13:[0-9]+]] d: u64, %[[VALUE_e_13:[0-9]+]] e: u64) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<u64, overflow=wrap>(add<u64, overflow=wrap>(widen<u64, reason=usual_arith>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8) -> u8>(%[[VALUE___builtin_stdc_bit_floor]], read<u8>(%[[VALUE_a_13]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_stdc_bit_floor_2]], read<u16>(%[[VALUE_b_13]])))))), call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_bit_floor_3]], read<u32>(%[[VALUE_c_13]])))), call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_stdc_bit_floor_4]], read<u64>(%[[VALUE_d_13]]))), call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_stdc_bit_floor_5]], read<u64>(%[[VALUE_e_13]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_bit_ceil:[0-9]+]] @__builtin_stdc_bit_ceil(%[[VALUE65:[0-9]+]] <unnamed>: u8) -> u8 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_bit_ceil_2:[0-9]+]] @__builtin_stdc_bit_ceil(%[[VALUE66:[0-9]+]] <unnamed>: u16) -> u16 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_bit_ceil_3:[0-9]+]] @__builtin_stdc_bit_ceil(%[[VALUE67:[0-9]+]] <unnamed>: u32) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_bit_ceil_4:[0-9]+]] @__builtin_stdc_bit_ceil(%[[VALUE68:[0-9]+]] <unnamed>: u64) -> u64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_bit_ceil_5:[0-9]+]] @__builtin_stdc_bit_ceil(%[[VALUE69:[0-9]+]] <unnamed>: u64) -> u64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_bit_ceil:[0-9]+]] @bit_ceil(%[[VALUE_a_14:[0-9]+]] a: u8, %[[VALUE_b_14:[0-9]+]] b: u16, %[[VALUE_c_14:[0-9]+]] c: u32, %[[VALUE_d_14:[0-9]+]] d: u64, %[[VALUE_e_14:[0-9]+]] e: u64) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<u64, overflow=wrap>(add<u64, overflow=wrap>(widen<u64, reason=usual_arith>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8) -> u8>(%[[VALUE___builtin_stdc_bit_ceil]], read<u8>(%[[VALUE_a_14]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_stdc_bit_ceil_2]], read<u16>(%[[VALUE_b_14]])))))), call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_bit_ceil_3]], read<u32>(%[[VALUE_c_14]])))), call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_stdc_bit_ceil_4]], read<u64>(%[[VALUE_d_14]]))), call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_stdc_bit_ceil_5]], read<u64>(%[[VALUE_e_14]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_leading_zeros_6:[0-9]+]] @__builtin_stdc_leading_zeros(%[[VALUE70:[0-9]+]] <unnamed>: u128) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_leading_ones_6:[0-9]+]] @__builtin_stdc_leading_ones(%[[VALUE71:[0-9]+]] <unnamed>: u128) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_trailing_zeros_6:[0-9]+]] @__builtin_stdc_trailing_zeros(%[[VALUE72:[0-9]+]] <unnamed>: u128) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_trailing_ones_6:[0-9]+]] @__builtin_stdc_trailing_ones(%[[VALUE73:[0-9]+]] <unnamed>: u128) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_first_leading_zero_6:[0-9]+]] @__builtin_stdc_first_leading_zero(%[[VALUE74:[0-9]+]] <unnamed>: u128) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_first_leading_one_6:[0-9]+]] @__builtin_stdc_first_leading_one(%[[VALUE75:[0-9]+]] <unnamed>: u128) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_first_trailing_zero_6:[0-9]+]] @__builtin_stdc_first_trailing_zero(%[[VALUE76:[0-9]+]] <unnamed>: u128) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_first_trailing_one_6:[0-9]+]] @__builtin_stdc_first_trailing_one(%[[VALUE77:[0-9]+]] <unnamed>: u128) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_count_zeros_6:[0-9]+]] @__builtin_stdc_count_zeros(%[[VALUE78:[0-9]+]] <unnamed>: u128) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_count_ones_6:[0-9]+]] @__builtin_stdc_count_ones(%[[VALUE79:[0-9]+]] <unnamed>: u128) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_has_single_bit_6:[0-9]+]] @__builtin_stdc_has_single_bit(%[[VALUE80:[0-9]+]] <unnamed>: u128) -> bool [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_bit_width_6:[0-9]+]] @__builtin_stdc_bit_width(%[[VALUE81:[0-9]+]] <unnamed>: u128) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_bit_floor_6:[0-9]+]] @__builtin_stdc_bit_floor(%[[VALUE82:[0-9]+]] <unnamed>: u128) -> u128 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_bit_ceil_6:[0-9]+]] @__builtin_stdc_bit_ceil(%[[VALUE83:[0-9]+]] <unnamed>: u128) -> u128 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_leading_zeros_7:[0-9]+]] @__builtin_stdc_leading_zeros(%[[VALUE84:[0-9]+]] <unnamed>: u1b) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_leading_ones_7:[0-9]+]] @__builtin_stdc_leading_ones(%[[VALUE85:[0-9]+]] <unnamed>: u1b) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_trailing_zeros_7:[0-9]+]] @__builtin_stdc_trailing_zeros(%[[VALUE86:[0-9]+]] <unnamed>: u1b) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_trailing_ones_7:[0-9]+]] @__builtin_stdc_trailing_ones(%[[VALUE87:[0-9]+]] <unnamed>: u1b) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_first_leading_zero_7:[0-9]+]] @__builtin_stdc_first_leading_zero(%[[VALUE88:[0-9]+]] <unnamed>: u1b) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_first_leading_one_7:[0-9]+]] @__builtin_stdc_first_leading_one(%[[VALUE89:[0-9]+]] <unnamed>: u1b) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_first_trailing_zero_7:[0-9]+]] @__builtin_stdc_first_trailing_zero(%[[VALUE90:[0-9]+]] <unnamed>: u1b) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_first_trailing_one_7:[0-9]+]] @__builtin_stdc_first_trailing_one(%[[VALUE91:[0-9]+]] <unnamed>: u1b) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_count_zeros_7:[0-9]+]] @__builtin_stdc_count_zeros(%[[VALUE92:[0-9]+]] <unnamed>: u1b) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_count_ones_7:[0-9]+]] @__builtin_stdc_count_ones(%[[VALUE93:[0-9]+]] <unnamed>: u1b) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_has_single_bit_7:[0-9]+]] @__builtin_stdc_has_single_bit(%[[VALUE94:[0-9]+]] <unnamed>: u1b) -> bool [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_bit_width_7:[0-9]+]] @__builtin_stdc_bit_width(%[[VALUE95:[0-9]+]] <unnamed>: u1b) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_bit_floor_7:[0-9]+]] @__builtin_stdc_bit_floor(%[[VALUE96:[0-9]+]] <unnamed>: u1b) -> u1b [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_bit_ceil_7:[0-9]+]] @__builtin_stdc_bit_ceil(%[[VALUE97:[0-9]+]] <unnamed>: u1b) -> u1b [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_leading_zeros_8:[0-9]+]] @__builtin_stdc_leading_zeros(%[[VALUE98:[0-9]+]] <unnamed>: u512b) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_leading_zeros_9:[0-9]+]] @__builtin_stdc_leading_zeros(%[[VALUE99:[0-9]+]] <unnamed>: u373b) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_leading_ones_8:[0-9]+]] @__builtin_stdc_leading_ones(%[[VALUE100:[0-9]+]] <unnamed>: u512b) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_leading_ones_9:[0-9]+]] @__builtin_stdc_leading_ones(%[[VALUE101:[0-9]+]] <unnamed>: u373b) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_trailing_zeros_8:[0-9]+]] @__builtin_stdc_trailing_zeros(%[[VALUE102:[0-9]+]] <unnamed>: u512b) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_trailing_zeros_9:[0-9]+]] @__builtin_stdc_trailing_zeros(%[[VALUE103:[0-9]+]] <unnamed>: u373b) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_trailing_ones_8:[0-9]+]] @__builtin_stdc_trailing_ones(%[[VALUE104:[0-9]+]] <unnamed>: u512b) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_trailing_ones_9:[0-9]+]] @__builtin_stdc_trailing_ones(%[[VALUE105:[0-9]+]] <unnamed>: u373b) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_first_leading_zero_8:[0-9]+]] @__builtin_stdc_first_leading_zero(%[[VALUE106:[0-9]+]] <unnamed>: u512b) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_first_leading_zero_9:[0-9]+]] @__builtin_stdc_first_leading_zero(%[[VALUE107:[0-9]+]] <unnamed>: u373b) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_first_leading_one_8:[0-9]+]] @__builtin_stdc_first_leading_one(%[[VALUE108:[0-9]+]] <unnamed>: u512b) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_first_leading_one_9:[0-9]+]] @__builtin_stdc_first_leading_one(%[[VALUE109:[0-9]+]] <unnamed>: u373b) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_first_trailing_zero_8:[0-9]+]] @__builtin_stdc_first_trailing_zero(%[[VALUE110:[0-9]+]] <unnamed>: u512b) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_first_trailing_zero_9:[0-9]+]] @__builtin_stdc_first_trailing_zero(%[[VALUE111:[0-9]+]] <unnamed>: u373b) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_first_trailing_one_8:[0-9]+]] @__builtin_stdc_first_trailing_one(%[[VALUE112:[0-9]+]] <unnamed>: u512b) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_first_trailing_one_9:[0-9]+]] @__builtin_stdc_first_trailing_one(%[[VALUE113:[0-9]+]] <unnamed>: u373b) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_count_zeros_8:[0-9]+]] @__builtin_stdc_count_zeros(%[[VALUE114:[0-9]+]] <unnamed>: u512b) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_count_zeros_9:[0-9]+]] @__builtin_stdc_count_zeros(%[[VALUE115:[0-9]+]] <unnamed>: u373b) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_count_ones_8:[0-9]+]] @__builtin_stdc_count_ones(%[[VALUE116:[0-9]+]] <unnamed>: u512b) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_count_ones_9:[0-9]+]] @__builtin_stdc_count_ones(%[[VALUE117:[0-9]+]] <unnamed>: u373b) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_has_single_bit_8:[0-9]+]] @__builtin_stdc_has_single_bit(%[[VALUE118:[0-9]+]] <unnamed>: u512b) -> bool [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_has_single_bit_9:[0-9]+]] @__builtin_stdc_has_single_bit(%[[VALUE119:[0-9]+]] <unnamed>: u373b) -> bool [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_bit_width_8:[0-9]+]] @__builtin_stdc_bit_width(%[[VALUE120:[0-9]+]] <unnamed>: u512b) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_bit_width_9:[0-9]+]] @__builtin_stdc_bit_width(%[[VALUE121:[0-9]+]] <unnamed>: u373b) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_bit_floor_8:[0-9]+]] @__builtin_stdc_bit_floor(%[[VALUE122:[0-9]+]] <unnamed>: u512b) -> u512b [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_bit_floor_9:[0-9]+]] @__builtin_stdc_bit_floor(%[[VALUE123:[0-9]+]] <unnamed>: u373b) -> u373b [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_bit_ceil_8:[0-9]+]] @__builtin_stdc_bit_ceil(%[[VALUE124:[0-9]+]] <unnamed>: u512b) -> u512b [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_bit_ceil_9:[0-9]+]] @__builtin_stdc_bit_ceil(%[[VALUE125:[0-9]+]] <unnamed>: u373b) -> u373b [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_bit_floor_10:[0-9]+]] @__builtin_stdc_bit_floor(%[[VALUE126:[0-9]+]] <unnamed>: u65535b) -> u65535b [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stdc_bit_ceil_10:[0-9]+]] @__builtin_stdc_bit_ceil(%[[VALUE127:[0-9]+]] <unnamed>: u65535b) -> u65535b [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(unprototyped) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u8) -> u32>(%[[VALUE___builtin_stdc_leading_zeros]], reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(8))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u16) -> u32>(%[[VALUE___builtin_stdc_leading_zeros_2]], reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=unknown>(mul<i32, overflow=ub>(const<i32>(2), const<i32>(8))))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_leading_zeros_3]], const<u32>(0)), reinterpret<u32, reason=usual_arith, fits=unknown>(mul<i32, overflow=ub>(const<i32>(4), const<i32>(8))))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_leading_zeros_4]], const<u64>(0)), reinterpret<u32, reason=usual_arith, fits=unknown>(mul<i32, overflow=ub>(const<i32>(8), const<i32>(8))))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_leading_zeros_5]], const<u64>(0)), reinterpret<u32, reason=usual_arith, fits=unknown>(mul<i32, overflow=ub>(const<i32>(8), const<i32>(8))))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u8) -> u32>(%[[VALUE___builtin_stdc_leading_zeros]], truncate<u8, reason=explicit, fits=unknown>(not<u32>(const<u32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), ne<u32>(call<u32, signature=fn(u16) -> u32>(%[[VALUE___builtin_stdc_leading_zeros_2]], truncate<u16, reason=explicit, fits=unknown>(not<u32>(const<u32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_leading_zeros_3]], not<u32>(const<u32>(0))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_leading_zeros_4]], not<u64>(const<u64>(0))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_leading_zeros_5]], not<u64>(const<u64>(0))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u8) -> u32>(%[[VALUE___builtin_stdc_leading_zeros]], reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(3)))), reinterpret<u32, reason=usual_arith, fits=unknown>(sub<i32, overflow=ub>(const<i32>(8), const<i32>(2)))), ne<u32>(call<u32, signature=fn(u16) -> u32>(%[[VALUE___builtin_stdc_leading_zeros_2]], reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(9)))), reinterpret<u32, reason=usual_arith, fits=unknown>(sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(2), const<i32>(8)), const<i32>(4))))), ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_leading_zeros_3]], const<u32>(34)), reinterpret<u32, reason=usual_arith, fits=unknown>(sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(4), const<i32>(8)), const<i32>(6))))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_leading_zeros_4]], const<u64>(130)), reinterpret<u32, reason=usual_arith, fits=unknown>(sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(8), const<i32>(8)), const<i32>(8))))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_leading_zeros_5]], const<u64>(512)), reinterpret<u32, reason=usual_arith, fits=unknown>(sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(8), const<i32>(8)), const<i32>(10)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u8) -> u32>(%[[VALUE___builtin_stdc_leading_ones]], reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u16) -> u32>(%[[VALUE___builtin_stdc_leading_ones_2]], reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_leading_ones_3]], const<u32>(0)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_leading_ones_4]], const<u64>(0)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_leading_ones_5]], const<u64>(0)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u8) -> u32>(%[[VALUE___builtin_stdc_leading_ones]], truncate<u8, reason=explicit, fits=unknown>(not<u32>(const<u32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(8))), ne<u32>(call<u32, signature=fn(u16) -> u32>(%[[VALUE___builtin_stdc_leading_ones_2]], truncate<u16, reason=explicit, fits=unknown>(not<u32>(const<u32>(0)))), reinterpret<u32, reason=usual_arith, fits=unknown>(mul<i32, overflow=ub>(const<i32>(2), const<i32>(8))))), ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_leading_ones_3]], not<u32>(const<u32>(0))), reinterpret<u32, reason=usual_arith, fits=unknown>(mul<i32, overflow=ub>(const<i32>(4), const<i32>(8))))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_leading_ones_4]], not<u64>(const<u64>(0))), reinterpret<u32, reason=usual_arith, fits=unknown>(mul<i32, overflow=ub>(const<i32>(8), const<i32>(8))))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_leading_ones_5]], not<u64>(const<u64>(0))), reinterpret<u32, reason=usual_arith, fits=unknown>(mul<i32, overflow=ub>(const<i32>(8), const<i32>(8)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u8) -> u32>(%[[VALUE___builtin_stdc_leading_ones]], reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(not<i32>(const<i32>(3))))), reinterpret<u32, reason=usual_arith, fits=unknown>(sub<i32, overflow=ub>(const<i32>(8), const<i32>(2)))), ne<u32>(call<u32, signature=fn(u16) -> u32>(%[[VALUE___builtin_stdc_leading_ones_2]], reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(not<i32>(const<i32>(9))))), reinterpret<u32, reason=usual_arith, fits=unknown>(sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(2), const<i32>(8)), const<i32>(4))))), ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_leading_ones_3]], not<u32>(const<u32>(34))), reinterpret<u32, reason=usual_arith, fits=unknown>(sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(4), const<i32>(8)), const<i32>(6))))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_leading_ones_4]], not<u64>(const<u64>(130))), reinterpret<u32, reason=usual_arith, fits=unknown>(sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(8), const<i32>(8)), const<i32>(8))))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_leading_ones_5]], not<u64>(const<u64>(512))), reinterpret<u32, reason=usual_arith, fits=unknown>(sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(8), const<i32>(8)), const<i32>(10)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u8) -> u32>(%[[VALUE___builtin_stdc_trailing_zeros]], reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(8))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u16) -> u32>(%[[VALUE___builtin_stdc_trailing_zeros_2]], reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=unknown>(mul<i32, overflow=ub>(const<i32>(2), const<i32>(8))))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_trailing_zeros_3]], const<u32>(0)), reinterpret<u32, reason=usual_arith, fits=unknown>(mul<i32, overflow=ub>(const<i32>(4), const<i32>(8))))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_trailing_zeros_4]], const<u64>(0)), reinterpret<u32, reason=usual_arith, fits=unknown>(mul<i32, overflow=ub>(const<i32>(8), const<i32>(8))))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_trailing_zeros_5]], const<u64>(0)), reinterpret<u32, reason=usual_arith, fits=unknown>(mul<i32, overflow=ub>(const<i32>(8), const<i32>(8))))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u8) -> u32>(%[[VALUE___builtin_stdc_trailing_zeros]], truncate<u8, reason=explicit, fits=unknown>(not<u32>(const<u32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), ne<u32>(call<u32, signature=fn(u16) -> u32>(%[[VALUE___builtin_stdc_trailing_zeros_2]], truncate<u16, reason=explicit, fits=unknown>(not<u32>(const<u32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_trailing_zeros_3]], not<u32>(const<u32>(0))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_trailing_zeros_4]], not<u64>(const<u64>(0))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_trailing_zeros_5]], not<u64>(const<u64>(0))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u8) -> u32>(%[[VALUE___builtin_stdc_trailing_zeros]], reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(2)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), ne<u32>(call<u32, signature=fn(u16) -> u32>(%[[VALUE___builtin_stdc_trailing_zeros_2]], reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(24)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(3)))), ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_trailing_zeros_3]], const<u32>(32)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(5)))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_trailing_zeros_4]], const<u64>(128)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(7)))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_trailing_zeros_5]], const<u64>(512)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(9))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u8) -> u32>(%[[VALUE___builtin_stdc_trailing_ones]], reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u16) -> u32>(%[[VALUE___builtin_stdc_trailing_ones_2]], reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_trailing_ones_3]], const<u32>(0)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_trailing_ones_4]], const<u64>(0)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_trailing_ones_5]], const<u64>(0)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u8) -> u32>(%[[VALUE___builtin_stdc_trailing_ones]], truncate<u8, reason=explicit, fits=unknown>(not<u32>(const<u32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(8))), ne<u32>(call<u32, signature=fn(u16) -> u32>(%[[VALUE___builtin_stdc_trailing_ones_2]], truncate<u16, reason=explicit, fits=unknown>(not<u32>(const<u32>(0)))), reinterpret<u32, reason=usual_arith, fits=unknown>(mul<i32, overflow=ub>(const<i32>(2), const<i32>(8))))), ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_trailing_ones_3]], not<u32>(const<u32>(0))), reinterpret<u32, reason=usual_arith, fits=unknown>(mul<i32, overflow=ub>(const<i32>(4), const<i32>(8))))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_trailing_ones_4]], not<u64>(const<u64>(0))), reinterpret<u32, reason=usual_arith, fits=unknown>(mul<i32, overflow=ub>(const<i32>(8), const<i32>(8))))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_trailing_ones_5]], not<u64>(const<u64>(0))), reinterpret<u32, reason=usual_arith, fits=unknown>(mul<i32, overflow=ub>(const<i32>(8), const<i32>(8)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u8) -> u32>(%[[VALUE___builtin_stdc_trailing_ones]], reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(5)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), ne<u32>(call<u32, signature=fn(u16) -> u32>(%[[VALUE___builtin_stdc_trailing_ones_2]], reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(15)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(4)))), ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_trailing_ones_3]], const<u32>(127)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(7)))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_trailing_ones_4]], const<u64>(511)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(9)))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_trailing_ones_5]], shr<u64, amount_out_of_range=ub, fill=zero_extend>(not<u64>(const<u64>(0)), const<i32>(2))), reinterpret<u32, reason=usual_arith, fits=unknown>(sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(8), const<i32>(8)), const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u8) -> u32>(%[[VALUE___builtin_stdc_first_leading_zero]], reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u16) -> u32>(%[[VALUE___builtin_stdc_first_leading_zero_2]], reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_first_leading_zero_3]], const<u32>(0)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_first_leading_zero_4]], const<u64>(0)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_first_leading_zero_5]], const<u64>(0)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u8) -> u32>(%[[VALUE___builtin_stdc_first_leading_zero]], truncate<u8, reason=explicit, fits=unknown>(not<u32>(const<u32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), ne<u32>(call<u32, signature=fn(u16) -> u32>(%[[VALUE___builtin_stdc_first_leading_zero_2]], truncate<u16, reason=explicit, fits=unknown>(not<u32>(const<u32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_first_leading_zero_3]], not<u32>(const<u32>(0))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_first_leading_zero_4]], not<u64>(const<u64>(0))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_first_leading_zero_5]], not<u64>(const<u64>(0))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u8) -> u32>(%[[VALUE___builtin_stdc_first_leading_zero]], truncate<u8, reason=explicit, fits=unknown>(not<u32>(const<u32>(3)))), reinterpret<u32, reason=usual_arith, fits=unknown>(sub<i32, overflow=ub>(const<i32>(8), const<i32>(1)))), ne<u32>(call<u32, signature=fn(u16) -> u32>(%[[VALUE___builtin_stdc_first_leading_zero_2]], truncate<u16, reason=explicit, fits=unknown>(not<u32>(const<u32>(15)))), reinterpret<u32, reason=usual_arith, fits=unknown>(sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(2), const<i32>(8)), const<i32>(3))))), ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_first_leading_zero_3]], not<u32>(const<u32>(63))), reinterpret<u32, reason=usual_arith, fits=unknown>(sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(4), const<i32>(8)), const<i32>(5))))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_first_leading_zero_4]], not<u64>(const<u64>(255))), reinterpret<u32, reason=usual_arith, fits=unknown>(sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(8), const<i32>(8)), const<i32>(7))))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_first_leading_zero_5]], not<u64>(const<u64>(1023))), reinterpret<u32, reason=usual_arith, fits=unknown>(sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(8), const<i32>(8)), const<i32>(9)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u8) -> u32>(%[[VALUE___builtin_stdc_first_leading_one]], reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u16) -> u32>(%[[VALUE___builtin_stdc_first_leading_one_2]], reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_first_leading_one_3]], const<u32>(0)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_first_leading_one_4]], const<u64>(0)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_first_leading_one_5]], const<u64>(0)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u8) -> u32>(%[[VALUE___builtin_stdc_first_leading_one]], truncate<u8, reason=explicit, fits=unknown>(not<u32>(const<u32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), ne<u32>(call<u32, signature=fn(u16) -> u32>(%[[VALUE___builtin_stdc_first_leading_one_2]], truncate<u16, reason=explicit, fits=unknown>(not<u32>(const<u32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))), ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_first_leading_one_3]], not<u32>(const<u32>(0))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_first_leading_one_4]], not<u64>(const<u64>(0))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_first_leading_one_5]], not<u64>(const<u64>(0))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u8) -> u32>(%[[VALUE___builtin_stdc_first_leading_one]], reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(3)))), reinterpret<u32, reason=usual_arith, fits=unknown>(sub<i32, overflow=ub>(const<i32>(8), const<i32>(1)))), ne<u32>(call<u32, signature=fn(u16) -> u32>(%[[VALUE___builtin_stdc_first_leading_one_2]], reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(9)))), reinterpret<u32, reason=usual_arith, fits=unknown>(sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(2), const<i32>(8)), const<i32>(3))))), ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_first_leading_one_3]], const<u32>(34)), reinterpret<u32, reason=usual_arith, fits=unknown>(sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(4), const<i32>(8)), const<i32>(5))))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_first_leading_one_4]], const<u64>(130)), reinterpret<u32, reason=usual_arith, fits=unknown>(sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(8), const<i32>(8)), const<i32>(7))))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_first_leading_one_5]], const<u64>(512)), reinterpret<u32, reason=usual_arith, fits=unknown>(sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(8), const<i32>(8)), const<i32>(9)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u8) -> u32>(%[[VALUE___builtin_stdc_first_trailing_zero]], reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u16) -> u32>(%[[VALUE___builtin_stdc_first_trailing_zero_2]], reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_first_trailing_zero_3]], const<u32>(0)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_first_trailing_zero_4]], const<u64>(0)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_first_trailing_zero_5]], const<u64>(0)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u8) -> u32>(%[[VALUE___builtin_stdc_first_trailing_zero]], truncate<u8, reason=explicit, fits=unknown>(not<u32>(const<u32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), ne<u32>(call<u32, signature=fn(u16) -> u32>(%[[VALUE___builtin_stdc_first_trailing_zero_2]], truncate<u16, reason=explicit, fits=unknown>(not<u32>(const<u32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_first_trailing_zero_3]], not<u32>(const<u32>(0))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_first_trailing_zero_4]], not<u64>(const<u64>(0))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_first_trailing_zero_5]], not<u64>(const<u64>(0))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u8) -> u32>(%[[VALUE___builtin_stdc_first_trailing_zero]], reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(2)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), ne<u32>(call<u32, signature=fn(u16) -> u32>(%[[VALUE___builtin_stdc_first_trailing_zero_2]], reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(15)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(5)))), ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_first_trailing_zero_3]], const<u32>(63)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(7)))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_first_trailing_zero_4]], const<u64>(128)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_first_trailing_zero_5]], const<u64>(511)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(10))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u8) -> u32>(%[[VALUE___builtin_stdc_first_trailing_one]], reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u16) -> u32>(%[[VALUE___builtin_stdc_first_trailing_one_2]], reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_first_trailing_one_3]], const<u32>(0)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_first_trailing_one_4]], const<u64>(0)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_first_trailing_one_5]], const<u64>(0)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u8) -> u32>(%[[VALUE___builtin_stdc_first_trailing_one]], truncate<u8, reason=explicit, fits=unknown>(not<u32>(const<u32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), ne<u32>(call<u32, signature=fn(u16) -> u32>(%[[VALUE___builtin_stdc_first_trailing_one_2]], truncate<u16, reason=explicit, fits=unknown>(not<u32>(const<u32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))), ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_first_trailing_one_3]], not<u32>(const<u32>(0))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_first_trailing_one_4]], not<u64>(const<u64>(0))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_first_trailing_one_5]], not<u64>(const<u64>(0))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u8) -> u32>(%[[VALUE___builtin_stdc_first_trailing_one]], reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(4)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(3))), ne<u32>(call<u32, signature=fn(u16) -> u32>(%[[VALUE___builtin_stdc_first_trailing_one_2]], reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(96)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(6)))), ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_first_trailing_one_3]], const<u32>(127)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_first_trailing_one_4]], const<u64>(511)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_first_trailing_one_5]], shl<u64, overflow=wrap, amount_out_of_range=ub>(not<u64>(const<u64>(0)), const<i32>(12))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(13))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u8) -> u32>(%[[VALUE___builtin_stdc_count_zeros]], reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(8))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u16) -> u32>(%[[VALUE___builtin_stdc_count_zeros_2]], reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=unknown>(mul<i32, overflow=ub>(const<i32>(2), const<i32>(8))))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_count_zeros_3]], const<u32>(0)), reinterpret<u32, reason=usual_arith, fits=unknown>(mul<i32, overflow=ub>(const<i32>(4), const<i32>(8))))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_count_zeros_4]], const<u64>(0)), reinterpret<u32, reason=usual_arith, fits=unknown>(mul<i32, overflow=ub>(const<i32>(8), const<i32>(8))))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_count_zeros_5]], const<u64>(0)), reinterpret<u32, reason=usual_arith, fits=unknown>(mul<i32, overflow=ub>(const<i32>(8), const<i32>(8))))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u8) -> u32>(%[[VALUE___builtin_stdc_count_zeros]], truncate<u8, reason=explicit, fits=unknown>(not<u32>(const<u32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), ne<u32>(call<u32, signature=fn(u16) -> u32>(%[[VALUE___builtin_stdc_count_zeros_2]], truncate<u16, reason=explicit, fits=unknown>(not<u32>(const<u32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_count_zeros_3]], not<u32>(const<u32>(0))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_count_zeros_4]], not<u64>(const<u64>(0))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_count_zeros_5]], not<u64>(const<u64>(0))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u8) -> u32>(%[[VALUE___builtin_stdc_count_zeros]], truncate<u8, reason=explicit, fits=always>(const<u32>(1))), reinterpret<u32, reason=usual_arith, fits=unknown>(sub<i32, overflow=ub>(const<i32>(8), const<i32>(1)))), ne<u32>(call<u32, signature=fn(u16) -> u32>(%[[VALUE___builtin_stdc_count_zeros_2]], reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(42)))), reinterpret<u32, reason=usual_arith, fits=unknown>(sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(2), const<i32>(8)), const<i32>(3))))), ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_count_zeros_3]], const<u32>(291)), reinterpret<u32, reason=usual_arith, fits=unknown>(sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(4), const<i32>(8)), const<i32>(4))))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_count_zeros_4]], not<u64>(const<u64>(1315))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(5)))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_count_zeros_5]], const<u64>(3363)), reinterpret<u32, reason=usual_arith, fits=unknown>(sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(8), const<i32>(8)), const<i32>(6)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u8) -> u32>(%[[VALUE___builtin_stdc_count_ones]], reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u16) -> u32>(%[[VALUE___builtin_stdc_count_ones_2]], reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_count_ones_3]], const<u32>(0)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_count_ones_4]], const<u64>(0)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_count_ones_5]], const<u64>(0)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u8) -> u32>(%[[VALUE___builtin_stdc_count_ones]], truncate<u8, reason=explicit, fits=unknown>(not<u32>(const<u32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(8))), ne<u32>(call<u32, signature=fn(u16) -> u32>(%[[VALUE___builtin_stdc_count_ones_2]], truncate<u16, reason=explicit, fits=unknown>(not<u32>(const<u32>(0)))), reinterpret<u32, reason=usual_arith, fits=unknown>(mul<i32, overflow=ub>(const<i32>(2), const<i32>(8))))), ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_count_ones_3]], not<u32>(const<u32>(0))), reinterpret<u32, reason=usual_arith, fits=unknown>(mul<i32, overflow=ub>(const<i32>(4), const<i32>(8))))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_count_ones_4]], not<u64>(const<u64>(0))), reinterpret<u32, reason=usual_arith, fits=unknown>(mul<i32, overflow=ub>(const<i32>(8), const<i32>(8))))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_count_ones_5]], not<u64>(const<u64>(0))), reinterpret<u32, reason=usual_arith, fits=unknown>(mul<i32, overflow=ub>(const<i32>(8), const<i32>(8)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u8) -> u32>(%[[VALUE___builtin_stdc_count_ones]], truncate<u8, reason=explicit, fits=unknown>(not<u32>(const<u32>(1)))), reinterpret<u32, reason=usual_arith, fits=unknown>(sub<i32, overflow=ub>(const<i32>(8), const<i32>(1)))), ne<u32>(call<u32, signature=fn(u16) -> u32>(%[[VALUE___builtin_stdc_count_ones_2]], reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(not<i32>(const<i32>(42))))), reinterpret<u32, reason=usual_arith, fits=unknown>(sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(2), const<i32>(8)), const<i32>(3))))), ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_count_ones_3]], not<u32>(const<u32>(291))), reinterpret<u32, reason=usual_arith, fits=unknown>(sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(4), const<i32>(8)), const<i32>(4))))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_count_ones_4]], const<u64>(1315)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(5)))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_count_ones_5]], not<u64>(const<u64>(3363))), reinterpret<u32, reason=usual_arith, fits=unknown>(sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(8), const<i32>(8)), const<i32>(6)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(call<bool, signature=fn(u8) -> bool>(%[[VALUE___builtin_stdc_has_single_bit]], reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), call<bool, signature=fn(u16) -> bool>(%[[VALUE___builtin_stdc_has_single_bit_2]], reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0))))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), call<bool, signature=fn(u32) -> bool>(%[[VALUE___builtin_stdc_has_single_bit_3]], const<u32>(0))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), call<bool, signature=fn(u64) -> bool>(%[[VALUE___builtin_stdc_has_single_bit_4]], const<u64>(0))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), call<bool, signature=fn(u64) -> bool>(%[[VALUE___builtin_stdc_has_single_bit_5]], const<u64>(0))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(not<bool>(call<bool, signature=fn(u8) -> bool>(%[[VALUE___builtin_stdc_has_single_bit]], reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))))), not<bool>(call<bool, signature=fn(u16) -> bool>(%[[VALUE___builtin_stdc_has_single_bit_2]], reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(8)))))), not<bool>(call<bool, signature=fn(u32) -> bool>(%[[VALUE___builtin_stdc_has_single_bit_3]], const<u32>(32)))), not<bool>(call<bool, signature=fn(u64) -> bool>(%[[VALUE___builtin_stdc_has_single_bit_4]], const<u64>(128)))), not<bool>(call<bool, signature=fn(u64) -> bool>(%[[VALUE___builtin_stdc_has_single_bit_5]], const<u64>(512))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(call<bool, signature=fn(u8) -> bool>(%[[VALUE___builtin_stdc_has_single_bit]], reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(7)))), call<bool, signature=fn(u16) -> bool>(%[[VALUE___builtin_stdc_has_single_bit_2]], reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(96))))), call<bool, signature=fn(u32) -> bool>(%[[VALUE___builtin_stdc_has_single_bit_3]], const<u32>(513))), call<bool, signature=fn(u64) -> bool>(%[[VALUE___builtin_stdc_has_single_bit_4]], const<u64>(1022))), call<bool, signature=fn(u64) -> bool>(%[[VALUE___builtin_stdc_has_single_bit_5]], const<u64>(12)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u8) -> u32>(%[[VALUE___builtin_stdc_bit_width]], reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u16) -> u32>(%[[VALUE___builtin_stdc_bit_width_2]], reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_bit_width_3]], const<u32>(0)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_bit_width_4]], const<u64>(0)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_bit_width_5]], const<u64>(0)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u8) -> u32>(%[[VALUE___builtin_stdc_bit_width]], truncate<u8, reason=explicit, fits=unknown>(not<u32>(const<u32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(8))), ne<u32>(call<u32, signature=fn(u16) -> u32>(%[[VALUE___builtin_stdc_bit_width_2]], truncate<u16, reason=explicit, fits=unknown>(not<u32>(const<u32>(0)))), reinterpret<u32, reason=usual_arith, fits=unknown>(mul<i32, overflow=ub>(const<i32>(2), const<i32>(8))))), ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_bit_width_3]], not<u32>(const<u32>(0))), reinterpret<u32, reason=usual_arith, fits=unknown>(mul<i32, overflow=ub>(const<i32>(4), const<i32>(8))))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_bit_width_4]], not<u64>(const<u64>(0))), reinterpret<u32, reason=usual_arith, fits=unknown>(mul<i32, overflow=ub>(const<i32>(8), const<i32>(8))))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_bit_width_5]], not<u64>(const<u64>(0))), reinterpret<u32, reason=usual_arith, fits=unknown>(mul<i32, overflow=ub>(const<i32>(8), const<i32>(8)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u8) -> u32>(%[[VALUE___builtin_stdc_bit_width]], reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(not<u32>(const<u32>(0))))), const<i32>(1))))), reinterpret<u32, reason=usual_arith, fits=unknown>(sub<i32, overflow=ub>(const<i32>(8), const<i32>(1)))), ne<u32>(call<u32, signature=fn(u8) -> u32>(%[[VALUE___builtin_stdc_bit_width]], reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(6)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(3)))), ne<u32>(call<u32, signature=fn(u16) -> u32>(%[[VALUE___builtin_stdc_bit_width_2]], truncate<u16, reason=explicit, fits=always>(const<u32>(12))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(4)))), ne<u32>(call<u32, signature=fn(u16) -> u32>(%[[VALUE___builtin_stdc_bit_width_2]], reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u16, reason=explicit, fits=unknown>(not<u32>(const<u32>(0))))), const<i32>(5))))), reinterpret<u32, reason=usual_arith, fits=unknown>(sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(2), const<i32>(8)), const<i32>(5))))), ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_bit_width_3]], const<u32>(137)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(8)))), ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_bit_width_3]], const<u32>(269)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(9)))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_bit_width_4]], const<u64>(39)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(6)))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_bit_width_4]], shr<u64, amount_out_of_range=ub, fill=zero_extend>(not<u64>(const<u64>(0)), const<i32>(2))), reinterpret<u32, reason=usual_arith, fits=unknown>(sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(8), const<i32>(8)), const<i32>(2))))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_bit_width_5]], const<u64>(1023)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(10)))), ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_bit_width_5]], const<u64>(1024)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(11))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8) -> u8>(%[[VALUE___builtin_stdc_bit_floor]], reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0)))))), const<i32>(0)), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_stdc_bit_floor_2]], reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0)))))), const<i32>(0))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_bit_floor_3]], const<u32>(0)), const<u32>(0))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u64>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_stdc_bit_floor_4]], const<u64>(0)), const<u64>(0))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u64>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_stdc_bit_floor_5]], const<u64>(0)), const<u64>(0))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8) -> u8>(%[[VALUE___builtin_stdc_bit_floor]], truncate<u8, reason=explicit, fits=unknown>(not<u32>(const<u32>(0))))))), shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(1), sub<i32, overflow=ub>(const<i32>(8), const<i32>(1)))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_stdc_bit_floor_2]], truncate<u16, reason=explicit, fits=unknown>(not<u32>(const<u32>(0))))))), shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(1), sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(2), const<i32>(8)), const<i32>(1))))), ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_bit_floor_3]], not<u32>(const<u32>(0))), shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(1), sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(4), const<i32>(8)), const<i32>(1))))), ne<u64>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_stdc_bit_floor_4]], not<u64>(const<u64>(0))), shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(8), const<i32>(8)), const<i32>(1))))), ne<u64>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_stdc_bit_floor_5]], not<u64>(const<u64>(0))), shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(8), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8) -> u8>(%[[VALUE___builtin_stdc_bit_floor]], reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(4)))))), const<i32>(4)), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8) -> u8>(%[[VALUE___builtin_stdc_bit_floor]], reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(7)))))), const<i32>(4))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_stdc_bit_floor_2]], truncate<u16, reason=explicit, fits=always>(const<u32>(8))))), const<i32>(8))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_stdc_bit_floor_2]], truncate<u16, reason=explicit, fits=always>(const<u32>(31))))), const<i32>(16))), ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_bit_floor_3]], const<u32>(137)), const<u32>(128))), ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_bit_floor_3]], const<u32>(269)), const<u32>(256))), ne<u64>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_stdc_bit_floor_4]], const<u64>(511)), const<u64>(256))), ne<u64>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_stdc_bit_floor_4]], const<u64>(512)), const<u64>(512))), ne<u64>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_stdc_bit_floor_4]], const<u64>(513)), const<u64>(512))), ne<u64>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_stdc_bit_floor_5]], const<u64>(1024)), const<u64>(1024)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8) -> u8>(%[[VALUE___builtin_stdc_bit_ceil]], reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0)))))), const<i32>(1)), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_stdc_bit_ceil_2]], reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(0)))))), const<i32>(1))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_bit_ceil_3]], const<u32>(0)), const<u32>(1))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u64>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_stdc_bit_ceil_4]], const<u64>(0)), const<u64>(1))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u64>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_stdc_bit_ceil_5]], const<u64>(0)), const<u64>(1))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8) -> u8>(%[[VALUE___builtin_stdc_bit_ceil]], truncate<u8, reason=explicit, fits=unknown>(not<u32>(const<u32>(0)))))), const<i32>(0)), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_stdc_bit_ceil_2]], truncate<u16, reason=explicit, fits=unknown>(not<u32>(const<u32>(0)))))), const<i32>(0))), ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_bit_ceil_3]], not<u32>(const<u32>(0))), const<u32>(0))), ne<u64>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_stdc_bit_ceil_4]], not<u64>(const<u64>(0))), const<u64>(0))), ne<u64>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_stdc_bit_ceil_5]], not<u64>(const<u64>(0))), const<u64>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8) -> u8>(%[[VALUE___builtin_stdc_bit_ceil]], reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(not<u32>(const<u32>(0))))), const<i32>(1)))))))), shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(1), sub<i32, overflow=ub>(const<i32>(8), const<i32>(1)))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8) -> u8>(%[[VALUE___builtin_stdc_bit_ceil]], reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(not<u32>(const<u32>(0))))), const<i32>(1)))))))), shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(1), sub<i32, overflow=ub>(const<i32>(8), const<i32>(1))))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_stdc_bit_ceil_2]], reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u16, reason=explicit, fits=unknown>(not<u32>(const<u32>(0))))), const<i32>(1)))))))), shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(1), sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(2), const<i32>(8)), const<i32>(1))))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_stdc_bit_ceil_2]], reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u16, reason=explicit, fits=unknown>(not<u32>(const<u32>(0))))), const<i32>(1)))))))), shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(1), sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(2), const<i32>(8)), const<i32>(1))))), ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_bit_ceil_3]], shr<u32, amount_out_of_range=ub, fill=zero_extend>(not<u32>(const<u32>(0)), const<i32>(1))), shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(1), sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(4), const<i32>(8)), const<i32>(1))))), ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_bit_ceil_3]], shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(1), sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(4), const<i32>(8)), const<i32>(1)))), shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(1), sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(4), const<i32>(8)), const<i32>(1))))), ne<u64>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_stdc_bit_ceil_4]], shr<u64, amount_out_of_range=ub, fill=zero_extend>(not<u64>(const<u64>(0)), const<i32>(1))), shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(8), const<i32>(8)), const<i32>(1))))), ne<u64>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_stdc_bit_ceil_4]], shr<u64, amount_out_of_range=ub, fill=zero_extend>(not<u64>(const<u64>(0)), const<i32>(1))), shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(8), const<i32>(8)), const<i32>(1))))), ne<u64>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_stdc_bit_ceil_5]], shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(8), const<i32>(8)), const<i32>(1)))), shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(8), const<i32>(8)), const<i32>(1))))), ne<u64>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_stdc_bit_ceil_5]], shr<u64, amount_out_of_range=ub, fill=zero_extend>(not<u64>(const<u64>(0)), const<i32>(1))), shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(8), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8) -> u8>(%[[VALUE___builtin_stdc_bit_ceil]], reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(1)))))), const<i32>(1)), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8) -> u8>(%[[VALUE___builtin_stdc_bit_ceil]], reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(2)))))), const<i32>(2))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_stdc_bit_ceil_2]], truncate<u16, reason=explicit, fits=always>(const<u32>(3))))), const<i32>(4))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_stdc_bit_ceil_2]], truncate<u16, reason=explicit, fits=always>(const<u32>(4))))), const<i32>(4))), ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_bit_ceil_3]], const<u32>(5)), const<u32>(8))), ne<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_stdc_bit_ceil_3]], const<u32>(269)), const<u32>(512))), ne<u64>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_stdc_bit_ceil_4]], const<u64>(511)), const<u64>(512))), ne<u64>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_stdc_bit_ceil_4]], const<u64>(512)), const<u64>(512))), ne<u64>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_stdc_bit_ceil_5]], const<u64>(513)), const<u64>(1024))), ne<u64>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_stdc_bit_ceil_5]], const<u64>(1025)), const<u64>(2048)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u128) -> u32>(%[[VALUE___builtin_stdc_leading_zeros_6]], reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=unknown>(mul<i32, overflow=ub>(const<i32>(16), const<i32>(8)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u128) -> u32>(%[[VALUE___builtin_stdc_leading_zeros_6]], not<u128>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(0))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u128) -> u32>(%[[VALUE___builtin_stdc_leading_ones_6]], reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u128) -> u32>(%[[VALUE___builtin_stdc_leading_ones_6]], not<u128>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(0))))), reinterpret<u32, reason=usual_arith, fits=unknown>(mul<i32, overflow=ub>(const<i32>(16), const<i32>(8)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u128) -> u32>(%[[VALUE___builtin_stdc_trailing_zeros_6]], reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=unknown>(mul<i32, overflow=ub>(const<i32>(16), const<i32>(8)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u128) -> u32>(%[[VALUE___builtin_stdc_trailing_zeros_6]], not<u128>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(0))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u128) -> u32>(%[[VALUE___builtin_stdc_trailing_ones_6]], reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u128) -> u32>(%[[VALUE___builtin_stdc_trailing_ones_6]], not<u128>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(0))))), reinterpret<u32, reason=usual_arith, fits=unknown>(mul<i32, overflow=ub>(const<i32>(16), const<i32>(8)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u128) -> u32>(%[[VALUE___builtin_stdc_first_leading_zero_6]], reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u128) -> u32>(%[[VALUE___builtin_stdc_first_leading_zero_6]], not<u128>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(0))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u128) -> u32>(%[[VALUE___builtin_stdc_first_leading_one_6]], reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u128) -> u32>(%[[VALUE___builtin_stdc_first_leading_one_6]], not<u128>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(0))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u128) -> u32>(%[[VALUE___builtin_stdc_first_trailing_zero_6]], reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u128) -> u32>(%[[VALUE___builtin_stdc_first_trailing_zero_6]], not<u128>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(0))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u128) -> u32>(%[[VALUE___builtin_stdc_first_trailing_one_6]], reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u128) -> u32>(%[[VALUE___builtin_stdc_first_trailing_one_6]], not<u128>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(0))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u128) -> u32>(%[[VALUE___builtin_stdc_count_zeros_6]], reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=unknown>(mul<i32, overflow=ub>(const<i32>(16), const<i32>(8)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u128) -> u32>(%[[VALUE___builtin_stdc_count_zeros_6]], not<u128>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(0))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u128) -> u32>(%[[VALUE___builtin_stdc_count_ones_6]], reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u128) -> u32>(%[[VALUE___builtin_stdc_count_ones_6]], not<u128>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(0))))), reinterpret<u32, reason=usual_arith, fits=unknown>(mul<i32, overflow=ub>(const<i32>(16), const<i32>(8)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(call<bool, signature=fn(u128) -> bool>(%[[VALUE___builtin_stdc_has_single_bit_6]], reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(0)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), call<bool, signature=fn(u128) -> bool>(%[[VALUE___builtin_stdc_has_single_bit_6]], not<u128>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(0))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u128) -> u32>(%[[VALUE___builtin_stdc_bit_width_6]], reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u128) -> u32>(%[[VALUE___builtin_stdc_bit_width_6]], not<u128>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(0))))), reinterpret<u32, reason=usual_arith, fits=unknown>(mul<i32, overflow=ub>(const<i32>(16), const<i32>(8)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<u128>(call<u128, signature=fn(u128) -> u128>(%[[VALUE___builtin_stdc_bit_floor_6]], reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(0)))), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(0)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u128>(call<u128, signature=fn(u128) -> u128>(%[[VALUE___builtin_stdc_bit_floor_6]], not<u128>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(0))))), shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(16), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u128>(call<u128, signature=fn(u128) -> u128>(%[[VALUE___builtin_stdc_bit_ceil_6]], reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(0)))), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(1)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u128>(call<u128, signature=fn(u128) -> u128>(%[[VALUE___builtin_stdc_bit_ceil_6]], reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1)))), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(1))))), ne<u128>(call<u128, signature=fn(u128) -> u128>(%[[VALUE___builtin_stdc_bit_ceil_6]], shr<u128, amount_out_of_range=ub, fill=zero_extend>(not<u128>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(0)))), const<i32>(1))), shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(16), const<i32>(8)), const<i32>(1))))), ne<u128>(call<u128, signature=fn(u128) -> u128>(%[[VALUE___builtin_stdc_bit_ceil_6]], not<u128>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(0))))), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(0)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE_a_15:[0-9]+]] a: u8 [storage=automatic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE128:[0-9]+]]: u8 [synthetic] = read<u8>(%[[VALUE_a_15]]);
// DEFAULT-NEXT:         let %[[VALUE129:[0-9]+]]: u8 [synthetic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE128]]))), const<i32>(1))));
// DEFAULT-NEXT:         write<u8>(%[[VALUE_a_15]], read<u8>(%[[VALUE129]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u8) -> u32>(%[[VALUE___builtin_stdc_bit_width]], read<u8>(%[[VALUE128]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_a_15]]))), const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE_b_15:[0-9]+]] b: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE130:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_b_15]]);
// DEFAULT-NEXT:         let %[[VALUE131:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE130]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_b_15]], read<u64>(%[[VALUE131]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_bit_width_5]], read<u64>(%[[VALUE130]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), ne<u64>(read<u64>(%[[VALUE_b_15]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE132:[0-9]+]]: u8 [synthetic] = read<u8>(%[[VALUE_a_15]]);
// DEFAULT-NEXT:         let %[[VALUE133:[0-9]+]]: u8 [synthetic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE132]]))), const<i32>(1))));
// DEFAULT-NEXT:         write<u8>(%[[VALUE_a_15]], read<u8>(%[[VALUE133]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8) -> u8>(%[[VALUE___builtin_stdc_bit_floor]], read<u8>(%[[VALUE132]])))), const<i32>(1)), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_a_15]]))), const<i32>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE134:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_b_15]]);
// DEFAULT-NEXT:         let %[[VALUE135:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE134]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_b_15]], read<u64>(%[[VALUE135]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<u64>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_stdc_bit_floor_5]], read<u64>(%[[VALUE134]])), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), ne<u64>(read<u64>(%[[VALUE_b_15]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE136:[0-9]+]]: u8 [synthetic] = read<u8>(%[[VALUE_a_15]]);
// DEFAULT-NEXT:         let %[[VALUE137:[0-9]+]]: u8 [synthetic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE136]]))), const<i32>(1))));
// DEFAULT-NEXT:         write<u8>(%[[VALUE_a_15]], read<u8>(%[[VALUE137]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8) -> u8>(%[[VALUE___builtin_stdc_bit_ceil]], read<u8>(%[[VALUE136]])))), const<i32>(2)), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_a_15]]))), const<i32>(3)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE138:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_b_15]]);
// DEFAULT-NEXT:         let %[[VALUE139:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE138]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_b_15]], read<u64>(%[[VALUE139]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<u64>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_stdc_bit_ceil_5]], read<u64>(%[[VALUE138]])), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), ne<u64>(read<u64>(%[[VALUE_b_15]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE140:[0-9]+]]: u8 [synthetic] = read<u8>(%[[VALUE_a_15]]);
// DEFAULT-NEXT:         let %[[VALUE141:[0-9]+]]: u8 [synthetic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE140]]))), const<i32>(1))));
// DEFAULT-NEXT:         write<u8>(%[[VALUE_a_15]], read<u8>(%[[VALUE141]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u8) -> u32>(%[[VALUE___builtin_stdc_leading_zeros]], read<u8>(%[[VALUE140]])), reinterpret<u32, reason=usual_arith, fits=unknown>(sub<i32, overflow=ub>(const<i32>(8), const<i32>(2)))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_a_15]]))), const<i32>(4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE142:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_b_15]]);
// DEFAULT-NEXT:         let %[[VALUE143:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE142]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_b_15]], read<u64>(%[[VALUE143]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_leading_zeros_5]], read<u64>(%[[VALUE142]])), reinterpret<u32, reason=usual_arith, fits=unknown>(sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(8), const<i32>(8)), const<i32>(2)))), ne<u64>(read<u64>(%[[VALUE_b_15]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE144:[0-9]+]]: u8 [synthetic] = read<u8>(%[[VALUE_a_15]]);
// DEFAULT-NEXT:         let %[[VALUE145:[0-9]+]]: u8 [synthetic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE144]]))), const<i32>(1))));
// DEFAULT-NEXT:         write<u8>(%[[VALUE_a_15]], read<u8>(%[[VALUE145]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u8) -> u32>(%[[VALUE___builtin_stdc_leading_ones]], read<u8>(%[[VALUE144]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_a_15]]))), const<i32>(5)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE146:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_b_15]]);
// DEFAULT-NEXT:         let %[[VALUE147:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE146]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_b_15]], read<u64>(%[[VALUE147]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_leading_ones_5]], read<u64>(%[[VALUE146]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), ne<u64>(read<u64>(%[[VALUE_b_15]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(5)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE148:[0-9]+]]: u8 [synthetic] = read<u8>(%[[VALUE_a_15]]);
// DEFAULT-NEXT:         let %[[VALUE149:[0-9]+]]: u8 [synthetic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE148]]))), const<i32>(1))));
// DEFAULT-NEXT:         write<u8>(%[[VALUE_a_15]], read<u8>(%[[VALUE149]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u8) -> u32>(%[[VALUE___builtin_stdc_trailing_zeros]], read<u8>(%[[VALUE148]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_a_15]]))), const<i32>(6)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE150:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_b_15]]);
// DEFAULT-NEXT:         let %[[VALUE151:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE150]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_b_15]], read<u64>(%[[VALUE151]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_trailing_zeros_5]], read<u64>(%[[VALUE150]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), ne<u64>(read<u64>(%[[VALUE_b_15]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(6)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE152:[0-9]+]]: u8 [synthetic] = read<u8>(%[[VALUE_a_15]]);
// DEFAULT-NEXT:         let %[[VALUE153:[0-9]+]]: u8 [synthetic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE152]]))), const<i32>(1))));
// DEFAULT-NEXT:         write<u8>(%[[VALUE_a_15]], read<u8>(%[[VALUE153]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u8) -> u32>(%[[VALUE___builtin_stdc_trailing_ones]], read<u8>(%[[VALUE152]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_a_15]]))), const<i32>(7)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE154:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_b_15]]);
// DEFAULT-NEXT:         let %[[VALUE155:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE154]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_b_15]], read<u64>(%[[VALUE155]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_trailing_ones_5]], read<u64>(%[[VALUE154]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), ne<u64>(read<u64>(%[[VALUE_b_15]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(7)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE156:[0-9]+]]: u8 [synthetic] = read<u8>(%[[VALUE_a_15]]);
// DEFAULT-NEXT:         let %[[VALUE157:[0-9]+]]: u8 [synthetic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE156]]))), const<i32>(1))));
// DEFAULT-NEXT:         write<u8>(%[[VALUE_a_15]], read<u8>(%[[VALUE157]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u8) -> u32>(%[[VALUE___builtin_stdc_first_leading_zero]], read<u8>(%[[VALUE156]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_a_15]]))), const<i32>(8)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE158:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_b_15]]);
// DEFAULT-NEXT:         let %[[VALUE159:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE158]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_b_15]], read<u64>(%[[VALUE159]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_first_leading_zero_5]], read<u64>(%[[VALUE158]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), ne<u64>(read<u64>(%[[VALUE_b_15]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE160:[0-9]+]]: u8 [synthetic] = read<u8>(%[[VALUE_a_15]]);
// DEFAULT-NEXT:         let %[[VALUE161:[0-9]+]]: u8 [synthetic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE160]]))), const<i32>(1))));
// DEFAULT-NEXT:         write<u8>(%[[VALUE_a_15]], read<u8>(%[[VALUE161]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u8) -> u32>(%[[VALUE___builtin_stdc_first_leading_one]], read<u8>(%[[VALUE160]])), reinterpret<u32, reason=usual_arith, fits=unknown>(sub<i32, overflow=ub>(const<i32>(8), const<i32>(3)))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_a_15]]))), const<i32>(9)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE162:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_b_15]]);
// DEFAULT-NEXT:         let %[[VALUE163:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE162]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_b_15]], read<u64>(%[[VALUE163]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_first_leading_one_5]], read<u64>(%[[VALUE162]])), reinterpret<u32, reason=usual_arith, fits=unknown>(sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(8), const<i32>(8)), const<i32>(3)))), ne<u64>(read<u64>(%[[VALUE_b_15]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(9)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE164:[0-9]+]]: u8 [synthetic] = read<u8>(%[[VALUE_a_15]]);
// DEFAULT-NEXT:         let %[[VALUE165:[0-9]+]]: u8 [synthetic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE164]]))), const<i32>(1))));
// DEFAULT-NEXT:         write<u8>(%[[VALUE_a_15]], read<u8>(%[[VALUE165]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u8) -> u32>(%[[VALUE___builtin_stdc_first_trailing_zero]], read<u8>(%[[VALUE164]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_a_15]]))), const<i32>(10)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE166:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_b_15]]);
// DEFAULT-NEXT:         let %[[VALUE167:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE166]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_b_15]], read<u64>(%[[VALUE167]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_first_trailing_zero_5]], read<u64>(%[[VALUE166]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))), ne<u64>(read<u64>(%[[VALUE_b_15]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(10)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE168:[0-9]+]]: u8 [synthetic] = read<u8>(%[[VALUE_a_15]]);
// DEFAULT-NEXT:         let %[[VALUE169:[0-9]+]]: u8 [synthetic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE168]]))), const<i32>(1))));
// DEFAULT-NEXT:         write<u8>(%[[VALUE_a_15]], read<u8>(%[[VALUE169]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u8) -> u32>(%[[VALUE___builtin_stdc_first_trailing_one]], read<u8>(%[[VALUE168]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_a_15]]))), const<i32>(11)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE170:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_b_15]]);
// DEFAULT-NEXT:         let %[[VALUE171:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE170]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_b_15]], read<u64>(%[[VALUE171]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_first_trailing_one_5]], read<u64>(%[[VALUE170]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))), ne<u64>(read<u64>(%[[VALUE_b_15]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(11)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE172:[0-9]+]]: u8 [synthetic] = read<u8>(%[[VALUE_a_15]]);
// DEFAULT-NEXT:         let %[[VALUE173:[0-9]+]]: u8 [synthetic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE172]]))), const<i32>(1))));
// DEFAULT-NEXT:         write<u8>(%[[VALUE_a_15]], read<u8>(%[[VALUE173]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u8) -> u32>(%[[VALUE___builtin_stdc_count_zeros]], read<u8>(%[[VALUE172]])), reinterpret<u32, reason=usual_arith, fits=unknown>(sub<i32, overflow=ub>(const<i32>(8), const<i32>(3)))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_a_15]]))), const<i32>(12)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE174:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_b_15]]);
// DEFAULT-NEXT:         let %[[VALUE175:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE174]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_b_15]], read<u64>(%[[VALUE175]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_count_zeros_5]], read<u64>(%[[VALUE174]])), reinterpret<u32, reason=usual_arith, fits=unknown>(sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(8), const<i32>(8)), const<i32>(3)))), ne<u64>(read<u64>(%[[VALUE_b_15]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(12)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE176:[0-9]+]]: u8 [synthetic] = read<u8>(%[[VALUE_a_15]]);
// DEFAULT-NEXT:         let %[[VALUE177:[0-9]+]]: u8 [synthetic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE176]]))), const<i32>(1))));
// DEFAULT-NEXT:         write<u8>(%[[VALUE_a_15]], read<u8>(%[[VALUE177]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u8) -> u32>(%[[VALUE___builtin_stdc_count_ones]], read<u8>(%[[VALUE176]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_a_15]]))), const<i32>(13)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE178:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_b_15]]);
// DEFAULT-NEXT:         let %[[VALUE179:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE178]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_b_15]], read<u64>(%[[VALUE179]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___builtin_stdc_count_ones_5]], read<u64>(%[[VALUE178]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))), ne<u64>(read<u64>(%[[VALUE_b_15]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(13)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE180:[0-9]+]]: u8 [synthetic] = read<u8>(%[[VALUE_a_15]]);
// DEFAULT-NEXT:         let %[[VALUE181:[0-9]+]]: u8 [synthetic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE180]]))), const<i32>(1))));
// DEFAULT-NEXT:         write<u8>(%[[VALUE_a_15]], read<u8>(%[[VALUE181]]));
// DEFAULT-NEXT:         if logical_or<bool>(call<bool, signature=fn(u8) -> bool>(%[[VALUE___builtin_stdc_has_single_bit]], read<u8>(%[[VALUE180]])), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_a_15]]))), const<i32>(14)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE182:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_b_15]]);
// DEFAULT-NEXT:         let %[[VALUE183:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE182]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_b_15]], read<u64>(%[[VALUE183]]));
// DEFAULT-NEXT:         if logical_or<bool>(call<bool, signature=fn(u64) -> bool>(%[[VALUE___builtin_stdc_has_single_bit_5]], read<u64>(%[[VALUE182]])), ne<u64>(read<u64>(%[[VALUE_b_15]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(14)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u1b) -> u32>(%[[VALUE___builtin_stdc_leading_zeros_7]], const<u1b>(0)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u1b) -> u32>(%[[VALUE___builtin_stdc_leading_zeros_7]], const<u1b>(1)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u1b) -> u32>(%[[VALUE___builtin_stdc_leading_ones_7]], const<u1b>(0)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u1b) -> u32>(%[[VALUE___builtin_stdc_leading_ones_7]], const<u1b>(1)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u1b) -> u32>(%[[VALUE___builtin_stdc_trailing_zeros_7]], const<u1b>(0)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u1b) -> u32>(%[[VALUE___builtin_stdc_trailing_zeros_7]], const<u1b>(1)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u1b) -> u32>(%[[VALUE___builtin_stdc_trailing_ones_7]], const<u1b>(0)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u1b) -> u32>(%[[VALUE___builtin_stdc_trailing_ones_7]], const<u1b>(1)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u1b) -> u32>(%[[VALUE___builtin_stdc_first_leading_zero_7]], const<u1b>(0)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u1b) -> u32>(%[[VALUE___builtin_stdc_first_leading_zero_7]], const<u1b>(1)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u1b) -> u32>(%[[VALUE___builtin_stdc_first_leading_one_7]], const<u1b>(0)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u1b) -> u32>(%[[VALUE___builtin_stdc_first_leading_one_7]], const<u1b>(1)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u1b) -> u32>(%[[VALUE___builtin_stdc_first_trailing_zero_7]], const<u1b>(0)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u1b) -> u32>(%[[VALUE___builtin_stdc_first_trailing_zero_7]], const<u1b>(1)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u1b) -> u32>(%[[VALUE___builtin_stdc_first_trailing_one_7]], const<u1b>(0)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u1b) -> u32>(%[[VALUE___builtin_stdc_first_trailing_one_7]], const<u1b>(1)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u1b) -> u32>(%[[VALUE___builtin_stdc_count_zeros_7]], const<u1b>(0)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u1b) -> u32>(%[[VALUE___builtin_stdc_count_zeros_7]], const<u1b>(1)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u1b) -> u32>(%[[VALUE___builtin_stdc_count_ones_7]], const<u1b>(0)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u1b) -> u32>(%[[VALUE___builtin_stdc_count_ones_7]], const<u1b>(1)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(call<bool, signature=fn(u1b) -> bool>(%[[VALUE___builtin_stdc_has_single_bit_7]], const<u1b>(0)), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), not<bool>(call<bool, signature=fn(u1b) -> bool>(%[[VALUE___builtin_stdc_has_single_bit_7]], const<u1b>(1)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u1b) -> u32>(%[[VALUE___builtin_stdc_bit_width_7]], const<u1b>(0)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u1b) -> u32>(%[[VALUE___builtin_stdc_bit_width_7]], const<u1b>(1)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=usual_arith, fits=unknown>(widen<u32, reason=usual_arith>(call<u1b, signature=fn(u1b) -> u1b>(%[[VALUE___builtin_stdc_bit_floor_7]], const<u1b>(0)))), const<i32>(0)), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<i32>(reinterpret<i32, reason=usual_arith, fits=unknown>(widen<u32, reason=usual_arith>(call<u1b, signature=fn(u1b) -> u1b>(%[[VALUE___builtin_stdc_bit_floor_7]], const<u1b>(1)))), const<i32>(1))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=usual_arith, fits=unknown>(widen<u32, reason=usual_arith>(call<u1b, signature=fn(u1b) -> u1b>(%[[VALUE___builtin_stdc_bit_ceil_7]], const<u1b>(0)))), const<i32>(1)), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<i32>(reinterpret<i32, reason=usual_arith, fits=unknown>(widen<u32, reason=usual_arith>(call<u1b, signature=fn(u1b) -> u1b>(%[[VALUE___builtin_stdc_bit_ceil_7]], const<u1b>(1)))), const<i32>(1))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE_c_15:[0-9]+]] c: u1b [storage=automatic] = reinterpret<u1b, reason=assign, fits=unknown>(truncate<i1b, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE184:[0-9]+]]: u1b [synthetic] = read<u1b>(%[[VALUE_c_15]]);
// DEFAULT-NEXT:         let %[[VALUE185:[0-9]+]]: u1b [synthetic] = reinterpret<u1b, reason=assign, fits=unknown>(truncate<i1b, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=usual_arith, fits=unknown>(widen<u32, reason=usual_arith>(read<u1b>(%[[VALUE184]]))), const<i32>(1))));
// DEFAULT-NEXT:         write<u1b>(%[[VALUE_c_15]], read<u1b>(%[[VALUE185]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(reinterpret<i32, reason=usual_arith, fits=unknown>(widen<u32, reason=usual_arith>(call<u1b, signature=fn(u1b) -> u1b>(%[[VALUE___builtin_stdc_bit_floor_7]], read<u1b>(%[[VALUE184]])))), const<i32>(0)), ne<i32>(reinterpret<i32, reason=usual_arith, fits=unknown>(widen<u32, reason=usual_arith>(read<u1b>(%[[VALUE_c_15]]))), const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE186:[0-9]+]]: u1b [synthetic] = read<u1b>(%[[VALUE_c_15]]);
// DEFAULT-NEXT:         let %[[VALUE187:[0-9]+]]: u1b [synthetic] = reinterpret<u1b, reason=assign, fits=unknown>(truncate<i1b, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=usual_arith, fits=unknown>(widen<u32, reason=usual_arith>(read<u1b>(%[[VALUE186]]))), const<i32>(1))));
// DEFAULT-NEXT:         write<u1b>(%[[VALUE_c_15]], read<u1b>(%[[VALUE187]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(reinterpret<i32, reason=usual_arith, fits=unknown>(widen<u32, reason=usual_arith>(call<u1b, signature=fn(u1b) -> u1b>(%[[VALUE___builtin_stdc_bit_floor_7]], read<u1b>(%[[VALUE186]])))), const<i32>(1)), ne<i32>(reinterpret<i32, reason=usual_arith, fits=unknown>(widen<u32, reason=usual_arith>(read<u1b>(%[[VALUE_c_15]]))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE188:[0-9]+]]: u1b [synthetic] = read<u1b>(%[[VALUE_c_15]]);
// DEFAULT-NEXT:         let %[[VALUE189:[0-9]+]]: u1b [synthetic] = reinterpret<u1b, reason=assign, fits=unknown>(truncate<i1b, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=usual_arith, fits=unknown>(widen<u32, reason=usual_arith>(read<u1b>(%[[VALUE188]]))), const<i32>(1))));
// DEFAULT-NEXT:         write<u1b>(%[[VALUE_c_15]], read<u1b>(%[[VALUE189]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(reinterpret<i32, reason=usual_arith, fits=unknown>(widen<u32, reason=usual_arith>(call<u1b, signature=fn(u1b) -> u1b>(%[[VALUE___builtin_stdc_bit_ceil_7]], read<u1b>(%[[VALUE188]])))), const<i32>(1)), ne<i32>(reinterpret<i32, reason=usual_arith, fits=unknown>(widen<u32, reason=usual_arith>(read<u1b>(%[[VALUE_c_15]]))), const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE190:[0-9]+]]: u1b [synthetic] = read<u1b>(%[[VALUE_c_15]]);
// DEFAULT-NEXT:         let %[[VALUE191:[0-9]+]]: u1b [synthetic] = reinterpret<u1b, reason=assign, fits=unknown>(truncate<i1b, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=usual_arith, fits=unknown>(widen<u32, reason=usual_arith>(read<u1b>(%[[VALUE190]]))), const<i32>(1))));
// DEFAULT-NEXT:         write<u1b>(%[[VALUE_c_15]], read<u1b>(%[[VALUE191]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(reinterpret<i32, reason=usual_arith, fits=unknown>(widen<u32, reason=usual_arith>(call<u1b, signature=fn(u1b) -> u1b>(%[[VALUE___builtin_stdc_bit_ceil_7]], read<u1b>(%[[VALUE190]])))), const<i32>(1)), ne<i32>(reinterpret<i32, reason=usual_arith, fits=unknown>(widen<u32, reason=usual_arith>(read<u1b>(%[[VALUE_c_15]]))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u512b) -> u32>(%[[VALUE___builtin_stdc_leading_zeros_8]], reinterpret<u512b, reason=explicit, fits=unknown>(widen<i512b, reason=explicit>(const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(512))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u373b) -> u32>(%[[VALUE___builtin_stdc_leading_zeros_9]], reinterpret<u373b, reason=explicit, fits=unknown>(widen<i373b, reason=explicit>(const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(373)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u512b) -> u32>(%[[VALUE___builtin_stdc_leading_zeros_8]], not<u512b>(reinterpret<u512b, reason=explicit, fits=unknown>(widen<i512b, reason=explicit>(const<i32>(0))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), ne<u32>(call<u32, signature=fn(u373b) -> u32>(%[[VALUE___builtin_stdc_leading_zeros_9]], not<u373b>(reinterpret<u373b, reason=explicit, fits=unknown>(widen<i373b, reason=explicit>(const<i32>(0))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u512b) -> u32>(%[[VALUE___builtin_stdc_leading_zeros_8]], reinterpret<u512b, reason=explicit, fits=unknown>(widen<i512b, reason=explicit>(const<i32>(275)))), reinterpret<u32, reason=usual_arith, fits=unknown>(sub<i32, overflow=ub>(const<i32>(512), const<i32>(9)))), ne<u32>(call<u32, signature=fn(u373b) -> u32>(%[[VALUE___builtin_stdc_leading_zeros_9]], reinterpret<u373b, reason=explicit, fits=unknown>(widen<i373b, reason=explicit>(const<i32>(512)))), reinterpret<u32, reason=usual_arith, fits=unknown>(sub<i32, overflow=ub>(const<i32>(373), const<i32>(10)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u512b) -> u32>(%[[VALUE___builtin_stdc_leading_ones_8]], reinterpret<u512b, reason=explicit, fits=unknown>(widen<i512b, reason=explicit>(const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u373b) -> u32>(%[[VALUE___builtin_stdc_leading_ones_9]], reinterpret<u373b, reason=explicit, fits=unknown>(widen<i373b, reason=explicit>(const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u512b) -> u32>(%[[VALUE___builtin_stdc_leading_ones_8]], not<u512b>(reinterpret<u512b, reason=explicit, fits=unknown>(widen<i512b, reason=explicit>(const<i32>(0))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(512))), ne<u32>(call<u32, signature=fn(u373b) -> u32>(%[[VALUE___builtin_stdc_leading_ones_9]], not<u373b>(reinterpret<u373b, reason=explicit, fits=unknown>(widen<i373b, reason=explicit>(const<i32>(0))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(373))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u512b) -> u32>(%[[VALUE___builtin_stdc_leading_ones_8]], not<u512b>(reinterpret<u512b, reason=explicit, fits=unknown>(widen<i512b, reason=explicit>(const<i32>(275))))), reinterpret<u32, reason=usual_arith, fits=unknown>(sub<i32, overflow=ub>(const<i32>(512), const<i32>(9)))), ne<u32>(call<u32, signature=fn(u373b) -> u32>(%[[VALUE___builtin_stdc_leading_ones_9]], not<u373b>(reinterpret<u373b, reason=explicit, fits=unknown>(widen<i373b, reason=explicit>(const<i32>(512))))), reinterpret<u32, reason=usual_arith, fits=unknown>(sub<i32, overflow=ub>(const<i32>(373), const<i32>(10)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u512b) -> u32>(%[[VALUE___builtin_stdc_trailing_zeros_8]], reinterpret<u512b, reason=explicit, fits=unknown>(widen<i512b, reason=explicit>(const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(512))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u373b) -> u32>(%[[VALUE___builtin_stdc_trailing_zeros_9]], reinterpret<u373b, reason=explicit, fits=unknown>(widen<i373b, reason=explicit>(const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(373)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u512b) -> u32>(%[[VALUE___builtin_stdc_trailing_zeros_8]], not<u512b>(reinterpret<u512b, reason=explicit, fits=unknown>(widen<i512b, reason=explicit>(const<i32>(0))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), ne<u32>(call<u32, signature=fn(u373b) -> u32>(%[[VALUE___builtin_stdc_trailing_zeros_9]], not<u373b>(reinterpret<u373b, reason=explicit, fits=unknown>(widen<i373b, reason=explicit>(const<i32>(0))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u512b) -> u32>(%[[VALUE___builtin_stdc_trailing_zeros_8]], reinterpret<u512b, reason=explicit, fits=unknown>(widen<i512b, reason=explicit>(const<i32>(256)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(8))), ne<u32>(call<u32, signature=fn(u373b) -> u32>(%[[VALUE___builtin_stdc_trailing_zeros_9]], reinterpret<u373b, reason=explicit, fits=unknown>(widen<i373b, reason=explicit>(const<i32>(512)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(9))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u512b) -> u32>(%[[VALUE___builtin_stdc_trailing_ones_8]], reinterpret<u512b, reason=explicit, fits=unknown>(widen<i512b, reason=explicit>(const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u373b) -> u32>(%[[VALUE___builtin_stdc_trailing_ones_9]], reinterpret<u373b, reason=explicit, fits=unknown>(widen<i373b, reason=explicit>(const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u512b) -> u32>(%[[VALUE___builtin_stdc_trailing_ones_8]], not<u512b>(reinterpret<u512b, reason=explicit, fits=unknown>(widen<i512b, reason=explicit>(const<i32>(0))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(512))), ne<u32>(call<u32, signature=fn(u373b) -> u32>(%[[VALUE___builtin_stdc_trailing_ones_9]], not<u373b>(reinterpret<u373b, reason=explicit, fits=unknown>(widen<i373b, reason=explicit>(const<i32>(0))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(373))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u512b) -> u32>(%[[VALUE___builtin_stdc_trailing_ones_8]], reinterpret<u512b, reason=explicit, fits=unknown>(widen<i512b, reason=explicit>(const<i32>(255)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(8))), ne<u32>(call<u32, signature=fn(u373b) -> u32>(%[[VALUE___builtin_stdc_trailing_ones_9]], shr<u373b, amount_out_of_range=ub, fill=zero_extend>(not<u373b>(reinterpret<u373b, reason=explicit, fits=unknown>(widen<i373b, reason=explicit>(const<i32>(0)))), const<i32>(2))), reinterpret<u32, reason=usual_arith, fits=unknown>(sub<i32, overflow=ub>(const<i32>(373), const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u512b) -> u32>(%[[VALUE___builtin_stdc_first_leading_zero_8]], reinterpret<u512b, reason=explicit, fits=unknown>(widen<i512b, reason=explicit>(const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u373b) -> u32>(%[[VALUE___builtin_stdc_first_leading_zero_9]], reinterpret<u373b, reason=explicit, fits=unknown>(widen<i373b, reason=explicit>(const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u512b) -> u32>(%[[VALUE___builtin_stdc_first_leading_zero_8]], not<u512b>(reinterpret<u512b, reason=explicit, fits=unknown>(widen<i512b, reason=explicit>(const<i32>(0))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), ne<u32>(call<u32, signature=fn(u373b) -> u32>(%[[VALUE___builtin_stdc_first_leading_zero_9]], not<u373b>(reinterpret<u373b, reason=explicit, fits=unknown>(widen<i373b, reason=explicit>(const<i32>(0))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u512b) -> u32>(%[[VALUE___builtin_stdc_first_leading_zero_8]], not<u512b>(reinterpret<u512b, reason=explicit, fits=unknown>(widen<i512b, reason=explicit>(const<i32>(511))))), reinterpret<u32, reason=usual_arith, fits=unknown>(sub<i32, overflow=ub>(const<i32>(512), const<i32>(8)))), ne<u32>(call<u32, signature=fn(u373b) -> u32>(%[[VALUE___builtin_stdc_first_leading_zero_9]], not<u373b>(reinterpret<u373b, reason=explicit, fits=unknown>(widen<i373b, reason=explicit>(const<i32>(1023))))), reinterpret<u32, reason=usual_arith, fits=unknown>(sub<i32, overflow=ub>(const<i32>(373), const<i32>(9)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u512b) -> u32>(%[[VALUE___builtin_stdc_first_leading_one_8]], reinterpret<u512b, reason=explicit, fits=unknown>(widen<i512b, reason=explicit>(const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u373b) -> u32>(%[[VALUE___builtin_stdc_first_leading_one_9]], reinterpret<u373b, reason=explicit, fits=unknown>(widen<i373b, reason=explicit>(const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u512b) -> u32>(%[[VALUE___builtin_stdc_first_leading_one_8]], not<u512b>(reinterpret<u512b, reason=explicit, fits=unknown>(widen<i512b, reason=explicit>(const<i32>(0))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), ne<u32>(call<u32, signature=fn(u373b) -> u32>(%[[VALUE___builtin_stdc_first_leading_one_9]], not<u373b>(reinterpret<u373b, reason=explicit, fits=unknown>(widen<i373b, reason=explicit>(const<i32>(0))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u512b) -> u32>(%[[VALUE___builtin_stdc_first_leading_one_8]], reinterpret<u512b, reason=explicit, fits=unknown>(widen<i512b, reason=explicit>(const<i32>(275)))), reinterpret<u32, reason=usual_arith, fits=unknown>(sub<i32, overflow=ub>(const<i32>(512), const<i32>(8)))), ne<u32>(call<u32, signature=fn(u373b) -> u32>(%[[VALUE___builtin_stdc_first_leading_one_9]], reinterpret<u373b, reason=explicit, fits=unknown>(widen<i373b, reason=explicit>(const<i32>(512)))), reinterpret<u32, reason=usual_arith, fits=unknown>(sub<i32, overflow=ub>(const<i32>(373), const<i32>(9)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u512b) -> u32>(%[[VALUE___builtin_stdc_first_trailing_zero_8]], reinterpret<u512b, reason=explicit, fits=unknown>(widen<i512b, reason=explicit>(const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u373b) -> u32>(%[[VALUE___builtin_stdc_first_trailing_zero_9]], reinterpret<u373b, reason=explicit, fits=unknown>(widen<i373b, reason=explicit>(const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u512b) -> u32>(%[[VALUE___builtin_stdc_first_trailing_zero_8]], not<u512b>(reinterpret<u512b, reason=explicit, fits=unknown>(widen<i512b, reason=explicit>(const<i32>(0))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), ne<u32>(call<u32, signature=fn(u373b) -> u32>(%[[VALUE___builtin_stdc_first_trailing_zero_9]], not<u373b>(reinterpret<u373b, reason=explicit, fits=unknown>(widen<i373b, reason=explicit>(const<i32>(0))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u512b) -> u32>(%[[VALUE___builtin_stdc_first_trailing_zero_8]], reinterpret<u512b, reason=explicit, fits=unknown>(widen<i512b, reason=explicit>(const<i32>(255)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(9))), ne<u32>(call<u32, signature=fn(u373b) -> u32>(%[[VALUE___builtin_stdc_first_trailing_zero_9]], reinterpret<u373b, reason=explicit, fits=unknown>(widen<i373b, reason=explicit>(const<i32>(511)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(10))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u512b) -> u32>(%[[VALUE___builtin_stdc_first_trailing_one_8]], reinterpret<u512b, reason=explicit, fits=unknown>(widen<i512b, reason=explicit>(const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u373b) -> u32>(%[[VALUE___builtin_stdc_first_trailing_one_9]], reinterpret<u373b, reason=explicit, fits=unknown>(widen<i373b, reason=explicit>(const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u512b) -> u32>(%[[VALUE___builtin_stdc_first_trailing_one_8]], not<u512b>(reinterpret<u512b, reason=explicit, fits=unknown>(widen<i512b, reason=explicit>(const<i32>(0))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), ne<u32>(call<u32, signature=fn(u373b) -> u32>(%[[VALUE___builtin_stdc_first_trailing_one_9]], not<u373b>(reinterpret<u373b, reason=explicit, fits=unknown>(widen<i373b, reason=explicit>(const<i32>(0))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u512b) -> u32>(%[[VALUE___builtin_stdc_first_trailing_one_8]], shl<u512b, overflow=wrap, amount_out_of_range=ub>(reinterpret<u512b, reason=explicit, fits=unknown>(widen<i512b, reason=explicit>(const<i32>(255))), const<i32>(175))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(176))), ne<u32>(call<u32, signature=fn(u373b) -> u32>(%[[VALUE___builtin_stdc_first_trailing_one_9]], shl<u373b, overflow=wrap, amount_out_of_range=ub>(not<u373b>(reinterpret<u373b, reason=explicit, fits=unknown>(widen<i373b, reason=explicit>(const<i32>(0)))), const<i32>(311))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(312))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u512b) -> u32>(%[[VALUE___builtin_stdc_count_zeros_8]], reinterpret<u512b, reason=explicit, fits=unknown>(widen<i512b, reason=explicit>(const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(512))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u373b) -> u32>(%[[VALUE___builtin_stdc_count_zeros_9]], reinterpret<u373b, reason=explicit, fits=unknown>(widen<i373b, reason=explicit>(const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(373)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u512b) -> u32>(%[[VALUE___builtin_stdc_count_zeros_8]], not<u512b>(reinterpret<u512b, reason=explicit, fits=unknown>(widen<i512b, reason=explicit>(const<i32>(0))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), ne<u32>(call<u32, signature=fn(u373b) -> u32>(%[[VALUE___builtin_stdc_count_zeros_9]], not<u373b>(reinterpret<u373b, reason=explicit, fits=unknown>(widen<i373b, reason=explicit>(const<i32>(0))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u512b) -> u32>(%[[VALUE___builtin_stdc_count_zeros_8]], reinterpret<u512b, reason=explicit, fits=unknown>(widen<i512b, reason=explicit>(const<i32>(1315)))), reinterpret<u32, reason=usual_arith, fits=unknown>(sub<i32, overflow=ub>(const<i32>(512), const<i32>(5)))), ne<u32>(call<u32, signature=fn(u373b) -> u32>(%[[VALUE___builtin_stdc_count_zeros_9]], reinterpret<u373b, reason=explicit, fits=unknown>(widen<i373b, reason=explicit>(const<i32>(3363)))), reinterpret<u32, reason=usual_arith, fits=unknown>(sub<i32, overflow=ub>(const<i32>(373), const<i32>(6)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u512b) -> u32>(%[[VALUE___builtin_stdc_count_ones_8]], reinterpret<u512b, reason=explicit, fits=unknown>(widen<i512b, reason=explicit>(const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u373b) -> u32>(%[[VALUE___builtin_stdc_count_ones_9]], reinterpret<u373b, reason=explicit, fits=unknown>(widen<i373b, reason=explicit>(const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u512b) -> u32>(%[[VALUE___builtin_stdc_count_ones_8]], not<u512b>(reinterpret<u512b, reason=explicit, fits=unknown>(widen<i512b, reason=explicit>(const<i32>(0))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(512))), ne<u32>(call<u32, signature=fn(u373b) -> u32>(%[[VALUE___builtin_stdc_count_ones_9]], not<u373b>(reinterpret<u373b, reason=explicit, fits=unknown>(widen<i373b, reason=explicit>(const<i32>(0))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(373))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u512b) -> u32>(%[[VALUE___builtin_stdc_count_ones_8]], not<u512b>(reinterpret<u512b, reason=explicit, fits=unknown>(widen<i512b, reason=explicit>(const<i32>(1315))))), reinterpret<u32, reason=usual_arith, fits=unknown>(sub<i32, overflow=ub>(const<i32>(512), const<i32>(5)))), ne<u32>(call<u32, signature=fn(u373b) -> u32>(%[[VALUE___builtin_stdc_count_ones_9]], not<u373b>(reinterpret<u373b, reason=explicit, fits=unknown>(widen<i373b, reason=explicit>(const<i32>(3363))))), reinterpret<u32, reason=usual_arith, fits=unknown>(sub<i32, overflow=ub>(const<i32>(373), const<i32>(6)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(call<bool, signature=fn(u512b) -> bool>(%[[VALUE___builtin_stdc_has_single_bit_8]], reinterpret<u512b, reason=explicit, fits=unknown>(widen<i512b, reason=explicit>(const<i32>(0)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), call<bool, signature=fn(u373b) -> bool>(%[[VALUE___builtin_stdc_has_single_bit_9]], reinterpret<u373b, reason=explicit, fits=unknown>(widen<i373b, reason=explicit>(const<i32>(0))))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(call<bool, signature=fn(u512b) -> bool>(%[[VALUE___builtin_stdc_has_single_bit_8]], not<u512b>(reinterpret<u512b, reason=explicit, fits=unknown>(widen<i512b, reason=explicit>(const<i32>(0))))), call<bool, signature=fn(u373b) -> bool>(%[[VALUE___builtin_stdc_has_single_bit_9]], not<u373b>(reinterpret<u373b, reason=explicit, fits=unknown>(widen<i373b, reason=explicit>(const<i32>(0))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(call<bool, signature=fn(u512b) -> bool>(%[[VALUE___builtin_stdc_has_single_bit_8]], shl<u512b, overflow=wrap, amount_out_of_range=ub>(reinterpret<u512b, reason=explicit, fits=unknown>(widen<i512b, reason=explicit>(const<i32>(1022))), const<i32>(279))), call<bool, signature=fn(u373b) -> bool>(%[[VALUE___builtin_stdc_has_single_bit_9]], shl<u373b, overflow=wrap, amount_out_of_range=ub>(reinterpret<u373b, reason=explicit, fits=unknown>(widen<i373b, reason=explicit>(const<i32>(12))), const<i32>(305))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(call<u32, signature=fn(u512b) -> u32>(%[[VALUE___builtin_stdc_bit_width_8]], reinterpret<u512b, reason=explicit, fits=unknown>(widen<i512b, reason=explicit>(const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u32>(call<u32, signature=fn(u373b) -> u32>(%[[VALUE___builtin_stdc_bit_width_9]], reinterpret<u373b, reason=explicit, fits=unknown>(widen<i373b, reason=explicit>(const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u512b) -> u32>(%[[VALUE___builtin_stdc_bit_width_8]], not<u512b>(reinterpret<u512b, reason=explicit, fits=unknown>(widen<i512b, reason=explicit>(const<i32>(0))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(512))), ne<u32>(call<u32, signature=fn(u373b) -> u32>(%[[VALUE___builtin_stdc_bit_width_9]], not<u373b>(reinterpret<u373b, reason=explicit, fits=unknown>(widen<i373b, reason=explicit>(const<i32>(0))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(373))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u512b) -> u32>(%[[VALUE___builtin_stdc_bit_width_8]], shl<u512b, overflow=wrap, amount_out_of_range=ub>(reinterpret<u512b, reason=explicit, fits=unknown>(widen<i512b, reason=explicit>(const<i32>(1023))), const<i32>(405))), reinterpret<u32, reason=usual_arith, fits=unknown>(add<i32, overflow=ub>(const<i32>(405), const<i32>(10)))), ne<u32>(call<u32, signature=fn(u373b) -> u32>(%[[VALUE___builtin_stdc_bit_width_9]], shl<u373b, overflow=wrap, amount_out_of_range=ub>(reinterpret<u373b, reason=explicit, fits=unknown>(widen<i373b, reason=explicit>(const<i32>(1024))), const<i32>(242))), reinterpret<u32, reason=usual_arith, fits=unknown>(add<i32, overflow=ub>(const<i32>(242), const<i32>(11)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u512b>(call<u512b, signature=fn(u512b) -> u512b>(%[[VALUE___builtin_stdc_bit_floor_8]], reinterpret<u512b, reason=explicit, fits=unknown>(widen<i512b, reason=explicit>(const<i32>(0)))), reinterpret<u512b, reason=usual_arith, fits=unknown>(widen<i512b, reason=usual_arith>(const<i32>(0)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u373b>(call<u373b, signature=fn(u373b) -> u373b>(%[[VALUE___builtin_stdc_bit_floor_9]], reinterpret<u373b, reason=explicit, fits=unknown>(widen<i373b, reason=explicit>(const<i32>(0)))), reinterpret<u373b, reason=usual_arith, fits=unknown>(widen<i373b, reason=usual_arith>(const<i32>(0))))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u512b>(call<u512b, signature=fn(u512b) -> u512b>(%[[VALUE___builtin_stdc_bit_floor_8]], not<u512b>(reinterpret<u512b, reason=explicit, fits=unknown>(widen<i512b, reason=explicit>(const<i32>(0))))), shl<u512b, overflow=wrap, amount_out_of_range=ub>(reinterpret<u512b, reason=explicit, fits=unknown>(widen<i512b, reason=explicit>(const<i32>(1))), sub<i32, overflow=ub>(const<i32>(512), const<i32>(1)))), ne<u373b>(call<u373b, signature=fn(u373b) -> u373b>(%[[VALUE___builtin_stdc_bit_floor_9]], not<u373b>(reinterpret<u373b, reason=explicit, fits=unknown>(widen<i373b, reason=explicit>(const<i32>(0))))), shl<u373b, overflow=wrap, amount_out_of_range=ub>(reinterpret<u373b, reason=explicit, fits=unknown>(widen<i373b, reason=explicit>(const<i32>(1))), sub<i32, overflow=ub>(const<i32>(373), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u512b>(call<u512b, signature=fn(u512b) -> u512b>(%[[VALUE___builtin_stdc_bit_floor_8]], shl<u512b, overflow=wrap, amount_out_of_range=ub>(reinterpret<u512b, reason=explicit, fits=unknown>(widen<i512b, reason=explicit>(const<i32>(511))), const<i32>(405))), shl<u512b, overflow=wrap, amount_out_of_range=ub>(reinterpret<u512b, reason=explicit, fits=unknown>(widen<i512b, reason=explicit>(const<i32>(256))), const<i32>(405))), ne<u373b>(call<u373b, signature=fn(u373b) -> u373b>(%[[VALUE___builtin_stdc_bit_floor_9]], shl<u373b, overflow=wrap, amount_out_of_range=ub>(reinterpret<u373b, reason=explicit, fits=unknown>(widen<i373b, reason=explicit>(const<i32>(512))), const<i32>(242))), shl<u373b, overflow=wrap, amount_out_of_range=ub>(reinterpret<u373b, reason=explicit, fits=unknown>(widen<i373b, reason=explicit>(const<i32>(512))), const<i32>(242))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u512b>(call<u512b, signature=fn(u512b) -> u512b>(%[[VALUE___builtin_stdc_bit_ceil_8]], reinterpret<u512b, reason=explicit, fits=unknown>(widen<i512b, reason=explicit>(const<i32>(0)))), reinterpret<u512b, reason=usual_arith, fits=unknown>(widen<i512b, reason=usual_arith>(const<i32>(1)))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))), ne<u373b>(call<u373b, signature=fn(u373b) -> u373b>(%[[VALUE___builtin_stdc_bit_ceil_9]], reinterpret<u373b, reason=explicit, fits=unknown>(widen<i373b, reason=explicit>(const<i32>(0)))), reinterpret<u373b, reason=usual_arith, fits=unknown>(widen<i373b, reason=usual_arith>(const<i32>(1))))), not<bool>(ne<i32>(const<i32>(1), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u512b>(call<u512b, signature=fn(u512b) -> u512b>(%[[VALUE___builtin_stdc_bit_ceil_8]], not<u512b>(reinterpret<u512b, reason=explicit, fits=unknown>(widen<i512b, reason=explicit>(const<i32>(0))))), reinterpret<u512b, reason=usual_arith, fits=unknown>(widen<i512b, reason=usual_arith>(const<i32>(0)))), ne<u373b>(call<u373b, signature=fn(u373b) -> u373b>(%[[VALUE___builtin_stdc_bit_ceil_9]], not<u373b>(reinterpret<u373b, reason=explicit, fits=unknown>(widen<i373b, reason=explicit>(const<i32>(0))))), reinterpret<u373b, reason=usual_arith, fits=unknown>(widen<i373b, reason=usual_arith>(const<i32>(0)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u512b>(call<u512b, signature=fn(u512b) -> u512b>(%[[VALUE___builtin_stdc_bit_ceil_8]], shl<u512b, overflow=wrap, amount_out_of_range=ub>(reinterpret<u512b, reason=explicit, fits=unknown>(widen<i512b, reason=explicit>(const<i32>(1))), sub<i32, overflow=ub>(const<i32>(512), const<i32>(1)))), shl<u512b, overflow=wrap, amount_out_of_range=ub>(reinterpret<u512b, reason=explicit, fits=unknown>(widen<i512b, reason=explicit>(const<i32>(1))), sub<i32, overflow=ub>(const<i32>(512), const<i32>(1)))), ne<u373b>(call<u373b, signature=fn(u373b) -> u373b>(%[[VALUE___builtin_stdc_bit_ceil_9]], shr<u373b, amount_out_of_range=ub, fill=zero_extend>(not<u373b>(reinterpret<u373b, reason=explicit, fits=unknown>(widen<i373b, reason=explicit>(const<i32>(0)))), const<i32>(1))), shl<u373b, overflow=wrap, amount_out_of_range=ub>(reinterpret<u373b, reason=explicit, fits=unknown>(widen<i373b, reason=explicit>(const<i32>(1))), sub<i32, overflow=ub>(const<i32>(373), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u512b>(call<u512b, signature=fn(u512b) -> u512b>(%[[VALUE___builtin_stdc_bit_ceil_8]], shl<u512b, overflow=wrap, amount_out_of_range=ub>(reinterpret<u512b, reason=explicit, fits=unknown>(widen<i512b, reason=explicit>(const<i32>(512))), const<i32>(405))), shl<u512b, overflow=wrap, amount_out_of_range=ub>(reinterpret<u512b, reason=explicit, fits=unknown>(widen<i512b, reason=explicit>(const<i32>(512))), const<i32>(405))), ne<u373b>(call<u373b, signature=fn(u373b) -> u373b>(%[[VALUE___builtin_stdc_bit_ceil_9]], shl<u373b, overflow=wrap, amount_out_of_range=ub>(reinterpret<u373b, reason=explicit, fits=unknown>(widen<i373b, reason=explicit>(const<i32>(513))), const<i32>(242))), shl<u373b, overflow=wrap, amount_out_of_range=ub>(reinterpret<u373b, reason=explicit, fits=unknown>(widen<i373b, reason=explicit>(const<i32>(1024))), const<i32>(242))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<u65535b>(call<u65535b, signature=fn(u65535b) -> u65535b>(%[[VALUE___builtin_stdc_bit_floor_10]], reinterpret<u65535b, reason=explicit, fits=unknown>(widen<i65535b, reason=explicit>(const<i32>(0)))), reinterpret<u65535b, reason=usual_arith, fits=unknown>(widen<i65535b, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<u65535b>(call<u65535b, signature=fn(u65535b) -> u65535b>(%[[VALUE___builtin_stdc_bit_floor_10]], not<u65535b>(reinterpret<u65535b, reason=explicit, fits=unknown>(widen<i65535b, reason=explicit>(const<i32>(0))))), shl<u65535b, overflow=wrap, amount_out_of_range=ub>(reinterpret<u65535b, reason=explicit, fits=unknown>(widen<i65535b, reason=explicit>(const<i32>(1))), sub<i32, overflow=ub>(const<i32>(65535), const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u65535b>(call<u65535b, signature=fn(u65535b) -> u65535b>(%[[VALUE___builtin_stdc_bit_floor_10]], shl<u65535b, overflow=wrap, amount_out_of_range=ub>(reinterpret<u65535b, reason=explicit, fits=unknown>(widen<i65535b, reason=explicit>(const<i32>(511))), const<i32>(405))), shl<u65535b, overflow=wrap, amount_out_of_range=ub>(reinterpret<u65535b, reason=explicit, fits=unknown>(widen<i65535b, reason=explicit>(const<i32>(256))), const<i32>(405))), ne<u65535b>(call<u65535b, signature=fn(u65535b) -> u65535b>(%[[VALUE___builtin_stdc_bit_floor_10]], shl<u65535b, overflow=wrap, amount_out_of_range=ub>(reinterpret<u65535b, reason=explicit, fits=unknown>(widen<i65535b, reason=explicit>(const<i32>(512))), const<i32>(405))), shl<u65535b, overflow=wrap, amount_out_of_range=ub>(reinterpret<u65535b, reason=explicit, fits=unknown>(widen<i65535b, reason=explicit>(const<i32>(512))), const<i32>(405))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<u65535b>(call<u65535b, signature=fn(u65535b) -> u65535b>(%[[VALUE___builtin_stdc_bit_ceil_10]], reinterpret<u65535b, reason=explicit, fits=unknown>(widen<i65535b, reason=explicit>(const<i32>(0)))), reinterpret<u65535b, reason=usual_arith, fits=unknown>(widen<i65535b, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<u65535b>(call<u65535b, signature=fn(u65535b) -> u65535b>(%[[VALUE___builtin_stdc_bit_ceil_10]], not<u65535b>(reinterpret<u65535b, reason=explicit, fits=unknown>(widen<i65535b, reason=explicit>(const<i32>(0))))), reinterpret<u65535b, reason=usual_arith, fits=unknown>(widen<i65535b, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<u65535b>(call<u65535b, signature=fn(u65535b) -> u65535b>(%[[VALUE___builtin_stdc_bit_ceil_10]], shl<u65535b, overflow=wrap, amount_out_of_range=ub>(reinterpret<u65535b, reason=explicit, fits=unknown>(widen<i65535b, reason=explicit>(const<i32>(1))), sub<i32, overflow=ub>(const<i32>(65535), const<i32>(1)))), shl<u65535b, overflow=wrap, amount_out_of_range=ub>(reinterpret<u65535b, reason=explicit, fits=unknown>(widen<i65535b, reason=explicit>(const<i32>(1))), sub<i32, overflow=ub>(const<i32>(65535), const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u65535b>(call<u65535b, signature=fn(u65535b) -> u65535b>(%[[VALUE___builtin_stdc_bit_ceil_10]], shl<u65535b, overflow=wrap, amount_out_of_range=ub>(reinterpret<u65535b, reason=explicit, fits=unknown>(widen<i65535b, reason=explicit>(const<i32>(512))), const<i32>(405))), shl<u65535b, overflow=wrap, amount_out_of_range=ub>(reinterpret<u65535b, reason=explicit, fits=unknown>(widen<i65535b, reason=explicit>(const<i32>(512))), const<i32>(405))), ne<u65535b>(call<u65535b, signature=fn(u65535b) -> u65535b>(%[[VALUE___builtin_stdc_bit_ceil_10]], shl<u65535b, overflow=wrap, amount_out_of_range=ub>(reinterpret<u65535b, reason=explicit, fits=unknown>(widen<i65535b, reason=explicit>(const<i32>(513))), const<i32>(405))), shl<u65535b, overflow=wrap, amount_out_of_range=ub>(reinterpret<u65535b, reason=explicit, fits=unknown>(widen<i65535b, reason=explicit>(const<i32>(1024))), const<i32>(405))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
