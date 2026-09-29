/* Verify that Glibc <stdlib.h> declarations are handled correctly
   { dg-do compile }
   { dg-options "-Wall" } */

#define A(...) __attribute__ ((malloc (__VA_ARGS__), noipa))

typedef __SIZE_TYPE__ size_t;

/* All functions with the same standard deallocator are associated
   with each other.  */
void free (void*);
void* calloc (size_t, size_t);
void* malloc (size_t);
void* realloc (void*, size_t);

A (__builtin_free) void* aligned_alloc (size_t, size_t);

/* Like realloc(), reallocarray() is both an allocator and a deallocator.
   It must be associated with both free() and with itself, but nothing
   else.  */
A (__builtin_free) void* reallocarray (void*, size_t, size_t);
A (reallocarray) void* reallocarray (void*, size_t, size_t);

A (__builtin_free) extern char *canonicalize_file_name (const char*);


void dealloc (void*);
A (dealloc) void* alloc (size_t);


void sink (void*);
void* source (void);


void test_builtin_aligned_alloc (void *p)
{
  {
    void *q = __builtin_aligned_alloc (1, 2);
    sink (q);
    __builtin_free (q);
  }

  {
    void *q = __builtin_aligned_alloc (1, 2);
    sink (q);
    free (q);
  }

  {
    void *q = __builtin_aligned_alloc (1, 2);
    q = __builtin_realloc (q, 3);
    sink (q);
    free (q);
  }

  {
    void *q = __builtin_aligned_alloc (1, 2);
    q = realloc (q, 3);
    sink (q);
    free (q);
  }

  {
    void *q;
    q = __builtin_aligned_alloc (1, 2); // { dg-message "returned from '__builtin_aligned_alloc'" }
    sink (q);
    dealloc (q);                        // { dg-warning "'dealloc' called on pointer returned from a mismatched allocation function" }
  }
}


void test_aligned_alloc (void *p)
{
  {
    void *q = aligned_alloc (1, 2);
    sink (q);
    __builtin_free (q);
  }

  {
    void *q = aligned_alloc (1, 2);
    sink (q);
    free (q);
  }

  {
    void *q = aligned_alloc (1, 2);
    q = __builtin_realloc (q, 3);
    sink (q);
    free (q);
  }

  {
    void *q = aligned_alloc (1, 2);
    q = realloc (q, 3);
    sink (q);
    free (q);
  }

  {
    void *q = aligned_alloc (1, 2);     // { dg-message "returned from 'aligned_alloc'" }
    sink (q);
    dealloc (q);                        // { dg-warning "'dealloc' called on pointer returned from a mismatched allocation function" }
  }
}


void test_reallocarray (void *p)
{
  {
    void *q = __builtin_aligned_alloc (1, 2);
    q = reallocarray (q, 2, 3);
    sink (q);
    free (q);
  }

  {
    void *q = aligned_alloc (1, 2);
    q = reallocarray (q, 2, 3);
    sink (q);
    free (q);
  }

  {
    void *q = __builtin_calloc (1, 2);
    q = reallocarray (q, 2, 3);
    sink (q);
    free (q);
  }

  {
    void *q = calloc (1, 2);
    q = reallocarray (q, 2, 3);
    sink (q);
    free (q);
  }

  {
    void *q = __builtin_malloc (1);
    q = reallocarray (q, 2, 3);
    sink (q);
    free (q);
  }

  {
    void *q = malloc (1);
    q = reallocarray (q, 2, 3);
    sink (q);
    free (q);
  }

  {
    void *q = __builtin_realloc (p, 1);
    q = reallocarray (q, 2, 3);
    sink (q);
    free (q);
  }

  {
    p = source ();
    void *q = realloc (p, 1);
    q = reallocarray (q, 2, 3);
    sink (q);
    free (q);
  }

  {
    void *q = __builtin_strdup ("abc");
    q = reallocarray (q, 3, 4);
    sink (q);
    free (q);
  }

  {
    void *q = __builtin_strndup ("abcd", 3);
    q = reallocarray (q, 4, 5);
    sink (q);
    free (q);
  }

  {
    void *q = source ();
    q = reallocarray (q, 5, 6);
    sink (q);
    free (q);
  }

  {
    void *q = alloc (1);                // { dg-message "returned from 'alloc'" }
    q = reallocarray (q, 6, 7);         // { dg-warning "'reallocarray' called on pointer returned from a mismatched allocation function" }
    sink (q);
    free (q);
  }

  {
    p = source ();
    void *q = reallocarray (p, 7, 8);
    q = __builtin_realloc (q, 9);
    sink (q);
    free (q);
  }

  {
    p = source ();
    void *q = reallocarray (p, 7, 8);
    q = realloc (q, 9);
    sink (q);
    free (q);
  }

  {
    p = source ();
    void *q = reallocarray (p, 8, 9);
    q = reallocarray (q, 3, 4);
    sink (q);
    free (q);
  }

  {
    p = source ();
    void *q = reallocarray (p, 9, 10);
    q = reallocarray (q, 3, 4);
    sink (q);
    dealloc (q);                        // { dg-warning "'dealloc' called on pointer returned from a mismatched allocation function" }
  }
}


void test_canonicalize_filename (void *p)
{
  {
    void *q = canonicalize_file_name ("a");
    sink (q);
    __builtin_free (q);
  }

  {
    void *q = canonicalize_file_name ("b");
    sink (q);
    free (q);
  }

  {
    void *q = canonicalize_file_name ("c");
    q = __builtin_realloc (q, 2);
    sink (q);
    free (q);
  }

  {
    void *q = canonicalize_file_name ("d");
    q = realloc (q, 3);
    sink (q);
    free (q);
  }

  {
    void *q = canonicalize_file_name ("e");
    q = reallocarray (q, 4, 5);
    sink (q);
    free (q);
  }

  {
    void *q;
    q = canonicalize_file_name ("f");   // { dg-message "returned from 'canonicalize_file_name'" }
    sink (q);
    dealloc (q);                        // { dg-warning "'dealloc' called on pointer returned from a mismatched allocation function" }
  }
}

// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([97, 98, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([97, 98, 99, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([97, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_6:[0-9]+]] .str[[VALUE_str_6]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_7:[0-9]+]] .str[[VALUE_str_7]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_8:[0-9]+]] .str[[VALUE_str_8]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([102, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_free:[0-9]+]] @free(%[[VALUE0:[0-9]+]] <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_calloc:[0-9]+]] @calloc(%[[VALUE1:[0-9]+]] <unnamed>: u64, %[[VALUE2:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_malloc:[0-9]+]] @malloc(%[[VALUE3:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_realloc:[0-9]+]] @realloc(%[[VALUE4:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE5:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_free:[0-9]+]] @__builtin_free(%[[VALUE6:[0-9]+]] <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_aligned_alloc:[0-9]+]] @aligned_alloc(%[[VALUE7:[0-9]+]] <unnamed>: u64, %[[VALUE8:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external] [deallocator=%[[VALUE___builtin_free]], argument=0];
// DEFAULT-NEXT:     fn %[[VALUE_reallocarray:[0-9]+]] @reallocarray(%[[VALUE9:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE10:[0-9]+]] <unnamed>: u64, %[[VALUE11:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external] [deallocator=%[[VALUE___builtin_free]], argument=0] [deallocator=%[[VALUE_reallocarray]], argument=0];
// DEFAULT-NEXT:     fn %[[VALUE_canonicalize_file_name:[0-9]+]] @canonicalize_file_name(%[[VALUE12:[0-9]+]] <unnamed>: ptr<const i8>) -> ptr<i8> [linkage=external] [deallocator=%[[VALUE___builtin_free]], argument=0];
// DEFAULT-NEXT:     fn %[[VALUE_dealloc:[0-9]+]] @dealloc(%[[VALUE13:[0-9]+]] <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_alloc:[0-9]+]] @alloc(%[[VALUE14:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external] [deallocator=%[[VALUE_dealloc]], argument=0];
// DEFAULT-NEXT:     fn %[[VALUE_sink:[0-9]+]] @sink(%[[VALUE15:[0-9]+]] <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_source:[0-9]+]] @source() -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_realloc:[0-9]+]] @__builtin_realloc(%[[VALUE16:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE17:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_builtin_aligned_alloc:[0-9]+]] @test_builtin_aligned_alloc(%[[VALUE_p:[0-9]+]] p: ptr<void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_q:[0-9]+]] q: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_aligned_alloc]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_q]]));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE___builtin_free]], read<ptr<void>>(%[[VALUE_q]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_q_2:[0-9]+]] q: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_aligned_alloc]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_q_2]]));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], read<ptr<void>>(%[[VALUE_q_2]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_q_3:[0-9]+]] q: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_aligned_alloc]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))));
// DEFAULT-NEXT:             write<ptr<void>>(%[[VALUE_q_3]], call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%[[VALUE___builtin_realloc]], read<ptr<void>>(%[[VALUE_q_3]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%[[VALUE___builtin_realloc]], read<ptr<void>>(%[[VALUE_q_3]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_q_3]]));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], read<ptr<void>>(%[[VALUE_q_3]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_q_4:[0-9]+]] q: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_aligned_alloc]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))));
// DEFAULT-NEXT:             write<ptr<void>>(%[[VALUE_q_4]], call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%[[VALUE_realloc]], read<ptr<void>>(%[[VALUE_q_4]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%[[VALUE_realloc]], read<ptr<void>>(%[[VALUE_q_4]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_q_4]]));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], read<ptr<void>>(%[[VALUE_q_4]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_q_5:[0-9]+]] q: ptr<void> [storage=automatic];
// DEFAULT-NEXT:             write<ptr<void>>(%[[VALUE_q_5]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_aligned_alloc]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_aligned_alloc]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_q_5]]));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_dealloc]], read<ptr<void>>(%[[VALUE_q_5]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_aligned_alloc:[0-9]+]] @test_aligned_alloc(%[[VALUE_p_2:[0-9]+]] p: ptr<void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_q_6:[0-9]+]] q: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_aligned_alloc]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_q_6]]));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE___builtin_free]], read<ptr<void>>(%[[VALUE_q_6]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_q_7:[0-9]+]] q: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_aligned_alloc]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_q_7]]));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], read<ptr<void>>(%[[VALUE_q_7]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_q_8:[0-9]+]] q: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_aligned_alloc]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))));
// DEFAULT-NEXT:             write<ptr<void>>(%[[VALUE_q_8]], call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%[[VALUE___builtin_realloc]], read<ptr<void>>(%[[VALUE_q_8]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%[[VALUE___builtin_realloc]], read<ptr<void>>(%[[VALUE_q_8]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_q_8]]));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], read<ptr<void>>(%[[VALUE_q_8]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_q_9:[0-9]+]] q: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_aligned_alloc]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))));
// DEFAULT-NEXT:             write<ptr<void>>(%[[VALUE_q_9]], call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%[[VALUE_realloc]], read<ptr<void>>(%[[VALUE_q_9]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%[[VALUE_realloc]], read<ptr<void>>(%[[VALUE_q_9]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_q_9]]));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], read<ptr<void>>(%[[VALUE_q_9]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_q_10:[0-9]+]] q: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_aligned_alloc]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_q_10]]));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_dealloc]], read<ptr<void>>(%[[VALUE_q_10]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_calloc:[0-9]+]] @__builtin_calloc(%[[VALUE18:[0-9]+]] <unnamed>: u64, %[[VALUE19:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_malloc:[0-9]+]] @__builtin_malloc(%[[VALUE20:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_strdup:[0-9]+]] @__builtin_strdup(%[[VALUE21:[0-9]+]] <unnamed>: ptr<const i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_strndup:[0-9]+]] @__builtin_strndup(%[[VALUE22:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE23:[0-9]+]] <unnamed>: u64) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_reallocarray:[0-9]+]] @test_reallocarray(%[[VALUE_p_3:[0-9]+]] p: ptr<void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_q_11:[0-9]+]] q: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_aligned_alloc]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))));
// DEFAULT-NEXT:             write<ptr<void>>(%[[VALUE_q_11]], call<ptr<void>, signature=fn(ptr<void>, u64, u64) -> ptr<void>>(%[[VALUE_reallocarray]], read<ptr<void>>(%[[VALUE_q_11]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, u64, u64) -> ptr<void>>(%[[VALUE_reallocarray]], read<ptr<void>>(%[[VALUE_q_11]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_q_11]]));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], read<ptr<void>>(%[[VALUE_q_11]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_q_12:[0-9]+]] q: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_aligned_alloc]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))));
// DEFAULT-NEXT:             write<ptr<void>>(%[[VALUE_q_12]], call<ptr<void>, signature=fn(ptr<void>, u64, u64) -> ptr<void>>(%[[VALUE_reallocarray]], read<ptr<void>>(%[[VALUE_q_12]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, u64, u64) -> ptr<void>>(%[[VALUE_reallocarray]], read<ptr<void>>(%[[VALUE_q_12]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_q_12]]));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], read<ptr<void>>(%[[VALUE_q_12]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_q_13:[0-9]+]] q: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE___builtin_calloc]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))));
// DEFAULT-NEXT:             write<ptr<void>>(%[[VALUE_q_13]], call<ptr<void>, signature=fn(ptr<void>, u64, u64) -> ptr<void>>(%[[VALUE_reallocarray]], read<ptr<void>>(%[[VALUE_q_13]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, u64, u64) -> ptr<void>>(%[[VALUE_reallocarray]], read<ptr<void>>(%[[VALUE_q_13]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_q_13]]));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], read<ptr<void>>(%[[VALUE_q_13]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_q_14:[0-9]+]] q: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_calloc]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))));
// DEFAULT-NEXT:             write<ptr<void>>(%[[VALUE_q_14]], call<ptr<void>, signature=fn(ptr<void>, u64, u64) -> ptr<void>>(%[[VALUE_reallocarray]], read<ptr<void>>(%[[VALUE_q_14]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, u64, u64) -> ptr<void>>(%[[VALUE_reallocarray]], read<ptr<void>>(%[[VALUE_q_14]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_q_14]]));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], read<ptr<void>>(%[[VALUE_q_14]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_q_15:[0-9]+]] q: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE___builtin_malloc]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:             write<ptr<void>>(%[[VALUE_q_15]], call<ptr<void>, signature=fn(ptr<void>, u64, u64) -> ptr<void>>(%[[VALUE_reallocarray]], read<ptr<void>>(%[[VALUE_q_15]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, u64, u64) -> ptr<void>>(%[[VALUE_reallocarray]], read<ptr<void>>(%[[VALUE_q_15]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_q_15]]));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], read<ptr<void>>(%[[VALUE_q_15]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_q_16:[0-9]+]] q: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_malloc]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:             write<ptr<void>>(%[[VALUE_q_16]], call<ptr<void>, signature=fn(ptr<void>, u64, u64) -> ptr<void>>(%[[VALUE_reallocarray]], read<ptr<void>>(%[[VALUE_q_16]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, u64, u64) -> ptr<void>>(%[[VALUE_reallocarray]], read<ptr<void>>(%[[VALUE_q_16]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_q_16]]));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], read<ptr<void>>(%[[VALUE_q_16]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_q_17:[0-9]+]] q: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%[[VALUE___builtin_realloc]], read<ptr<void>>(%[[VALUE_p_3]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:             write<ptr<void>>(%[[VALUE_q_17]], call<ptr<void>, signature=fn(ptr<void>, u64, u64) -> ptr<void>>(%[[VALUE_reallocarray]], read<ptr<void>>(%[[VALUE_q_17]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, u64, u64) -> ptr<void>>(%[[VALUE_reallocarray]], read<ptr<void>>(%[[VALUE_q_17]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_q_17]]));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], read<ptr<void>>(%[[VALUE_q_17]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             write<ptr<void>>(%[[VALUE_p_3]], call<ptr<void>, signature=fn() -> ptr<void>>(%[[VALUE_source]]));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn() -> ptr<void>>(%[[VALUE_source]]);
// DEFAULT-NEXT:             let %[[VALUE_q_18:[0-9]+]] q: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%[[VALUE_realloc]], read<ptr<void>>(%[[VALUE_p_3]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:             write<ptr<void>>(%[[VALUE_q_18]], call<ptr<void>, signature=fn(ptr<void>, u64, u64) -> ptr<void>>(%[[VALUE_reallocarray]], read<ptr<void>>(%[[VALUE_q_18]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, u64, u64) -> ptr<void>>(%[[VALUE_reallocarray]], read<ptr<void>>(%[[VALUE_q_18]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_q_18]]));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], read<ptr<void>>(%[[VALUE_q_18]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_q_19:[0-9]+]] q: ptr<void> [storage=automatic] = pointer_cast<ptr<void>, reason=assign>(call<ptr<i8>, signature=fn(ptr<const i8>) -> ptr<i8>>(%[[VALUE___builtin_strdup]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str]]))));
// DEFAULT-NEXT:             write<ptr<void>>(%[[VALUE_q_19]], call<ptr<void>, signature=fn(ptr<void>, u64, u64) -> ptr<void>>(%[[VALUE_reallocarray]], read<ptr<void>>(%[[VALUE_q_19]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, u64, u64) -> ptr<void>>(%[[VALUE_reallocarray]], read<ptr<void>>(%[[VALUE_q_19]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_q_19]]));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], read<ptr<void>>(%[[VALUE_q_19]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_q_20:[0-9]+]] q: ptr<void> [storage=automatic] = pointer_cast<ptr<void>, reason=assign>(call<ptr<i8>, signature=fn(ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE___builtin_strndup]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_2]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3)))));
// DEFAULT-NEXT:             write<ptr<void>>(%[[VALUE_q_20]], call<ptr<void>, signature=fn(ptr<void>, u64, u64) -> ptr<void>>(%[[VALUE_reallocarray]], read<ptr<void>>(%[[VALUE_q_20]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, u64, u64) -> ptr<void>>(%[[VALUE_reallocarray]], read<ptr<void>>(%[[VALUE_q_20]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_q_20]]));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], read<ptr<void>>(%[[VALUE_q_20]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_q_21:[0-9]+]] q: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn() -> ptr<void>>(%[[VALUE_source]]);
// DEFAULT-NEXT:             write<ptr<void>>(%[[VALUE_q_21]], call<ptr<void>, signature=fn(ptr<void>, u64, u64) -> ptr<void>>(%[[VALUE_reallocarray]], read<ptr<void>>(%[[VALUE_q_21]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, u64, u64) -> ptr<void>>(%[[VALUE_reallocarray]], read<ptr<void>>(%[[VALUE_q_21]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_q_21]]));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], read<ptr<void>>(%[[VALUE_q_21]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_q_22:[0-9]+]] q: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_alloc]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:             write<ptr<void>>(%[[VALUE_q_22]], call<ptr<void>, signature=fn(ptr<void>, u64, u64) -> ptr<void>>(%[[VALUE_reallocarray]], read<ptr<void>>(%[[VALUE_q_22]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(7)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, u64, u64) -> ptr<void>>(%[[VALUE_reallocarray]], read<ptr<void>>(%[[VALUE_q_22]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(7))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_q_22]]));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], read<ptr<void>>(%[[VALUE_q_22]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             write<ptr<void>>(%[[VALUE_p_3]], call<ptr<void>, signature=fn() -> ptr<void>>(%[[VALUE_source]]));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn() -> ptr<void>>(%[[VALUE_source]]);
// DEFAULT-NEXT:             let %[[VALUE_q_23:[0-9]+]] q: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(ptr<void>, u64, u64) -> ptr<void>>(%[[VALUE_reallocarray]], read<ptr<void>>(%[[VALUE_p_3]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(7))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8))));
// DEFAULT-NEXT:             write<ptr<void>>(%[[VALUE_q_23]], call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%[[VALUE___builtin_realloc]], read<ptr<void>>(%[[VALUE_q_23]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(9)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%[[VALUE___builtin_realloc]], read<ptr<void>>(%[[VALUE_q_23]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(9))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_q_23]]));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], read<ptr<void>>(%[[VALUE_q_23]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             write<ptr<void>>(%[[VALUE_p_3]], call<ptr<void>, signature=fn() -> ptr<void>>(%[[VALUE_source]]));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn() -> ptr<void>>(%[[VALUE_source]]);
// DEFAULT-NEXT:             let %[[VALUE_q_24:[0-9]+]] q: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(ptr<void>, u64, u64) -> ptr<void>>(%[[VALUE_reallocarray]], read<ptr<void>>(%[[VALUE_p_3]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(7))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8))));
// DEFAULT-NEXT:             write<ptr<void>>(%[[VALUE_q_24]], call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%[[VALUE_realloc]], read<ptr<void>>(%[[VALUE_q_24]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(9)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%[[VALUE_realloc]], read<ptr<void>>(%[[VALUE_q_24]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(9))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_q_24]]));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], read<ptr<void>>(%[[VALUE_q_24]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             write<ptr<void>>(%[[VALUE_p_3]], call<ptr<void>, signature=fn() -> ptr<void>>(%[[VALUE_source]]));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn() -> ptr<void>>(%[[VALUE_source]]);
// DEFAULT-NEXT:             let %[[VALUE_q_25:[0-9]+]] q: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(ptr<void>, u64, u64) -> ptr<void>>(%[[VALUE_reallocarray]], read<ptr<void>>(%[[VALUE_p_3]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(9))));
// DEFAULT-NEXT:             write<ptr<void>>(%[[VALUE_q_25]], call<ptr<void>, signature=fn(ptr<void>, u64, u64) -> ptr<void>>(%[[VALUE_reallocarray]], read<ptr<void>>(%[[VALUE_q_25]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, u64, u64) -> ptr<void>>(%[[VALUE_reallocarray]], read<ptr<void>>(%[[VALUE_q_25]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_q_25]]));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], read<ptr<void>>(%[[VALUE_q_25]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             write<ptr<void>>(%[[VALUE_p_3]], call<ptr<void>, signature=fn() -> ptr<void>>(%[[VALUE_source]]));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn() -> ptr<void>>(%[[VALUE_source]]);
// DEFAULT-NEXT:             let %[[VALUE_q_26:[0-9]+]] q: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(ptr<void>, u64, u64) -> ptr<void>>(%[[VALUE_reallocarray]], read<ptr<void>>(%[[VALUE_p_3]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(9))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(10))));
// DEFAULT-NEXT:             write<ptr<void>>(%[[VALUE_q_26]], call<ptr<void>, signature=fn(ptr<void>, u64, u64) -> ptr<void>>(%[[VALUE_reallocarray]], read<ptr<void>>(%[[VALUE_q_26]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, u64, u64) -> ptr<void>>(%[[VALUE_reallocarray]], read<ptr<void>>(%[[VALUE_q_26]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_q_26]]));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_dealloc]], read<ptr<void>>(%[[VALUE_q_26]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_canonicalize_filename:[0-9]+]] @test_canonicalize_filename(%[[VALUE_p_4:[0-9]+]] p: ptr<void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_q_27:[0-9]+]] q: ptr<void> [storage=automatic] = pointer_cast<ptr<void>, reason=assign>(call<ptr<i8>, signature=fn(ptr<const i8>) -> ptr<i8>>(%[[VALUE_canonicalize_file_name]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_3]]))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_q_27]]));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE___builtin_free]], read<ptr<void>>(%[[VALUE_q_27]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_q_28:[0-9]+]] q: ptr<void> [storage=automatic] = pointer_cast<ptr<void>, reason=assign>(call<ptr<i8>, signature=fn(ptr<const i8>) -> ptr<i8>>(%[[VALUE_canonicalize_file_name]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_4]]))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_q_28]]));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], read<ptr<void>>(%[[VALUE_q_28]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_q_29:[0-9]+]] q: ptr<void> [storage=automatic] = pointer_cast<ptr<void>, reason=assign>(call<ptr<i8>, signature=fn(ptr<const i8>) -> ptr<i8>>(%[[VALUE_canonicalize_file_name]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_5]]))));
// DEFAULT-NEXT:             write<ptr<void>>(%[[VALUE_q_29]], call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%[[VALUE___builtin_realloc]], read<ptr<void>>(%[[VALUE_q_29]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%[[VALUE___builtin_realloc]], read<ptr<void>>(%[[VALUE_q_29]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_q_29]]));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], read<ptr<void>>(%[[VALUE_q_29]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_q_30:[0-9]+]] q: ptr<void> [storage=automatic] = pointer_cast<ptr<void>, reason=assign>(call<ptr<i8>, signature=fn(ptr<const i8>) -> ptr<i8>>(%[[VALUE_canonicalize_file_name]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_6]]))));
// DEFAULT-NEXT:             write<ptr<void>>(%[[VALUE_q_30]], call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%[[VALUE_realloc]], read<ptr<void>>(%[[VALUE_q_30]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%[[VALUE_realloc]], read<ptr<void>>(%[[VALUE_q_30]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_q_30]]));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], read<ptr<void>>(%[[VALUE_q_30]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_q_31:[0-9]+]] q: ptr<void> [storage=automatic] = pointer_cast<ptr<void>, reason=assign>(call<ptr<i8>, signature=fn(ptr<const i8>) -> ptr<i8>>(%[[VALUE_canonicalize_file_name]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_7]]))));
// DEFAULT-NEXT:             write<ptr<void>>(%[[VALUE_q_31]], call<ptr<void>, signature=fn(ptr<void>, u64, u64) -> ptr<void>>(%[[VALUE_reallocarray]], read<ptr<void>>(%[[VALUE_q_31]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, u64, u64) -> ptr<void>>(%[[VALUE_reallocarray]], read<ptr<void>>(%[[VALUE_q_31]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_q_31]]));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], read<ptr<void>>(%[[VALUE_q_31]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_q_32:[0-9]+]] q: ptr<void> [storage=automatic];
// DEFAULT-NEXT:             write<ptr<void>>(%[[VALUE_q_32]], pointer_cast<ptr<void>, reason=assign>(call<ptr<i8>, signature=fn(ptr<const i8>) -> ptr<i8>>(%[[VALUE_canonicalize_file_name]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_8]])))));
// DEFAULT-NEXT:             pointer_cast<ptr<void>, reason=assign>(call<ptr<i8>, signature=fn(ptr<const i8>) -> ptr<i8>>(%[[VALUE_canonicalize_file_name]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_8]]))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_q_32]]));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_dealloc]], read<ptr<void>>(%[[VALUE_q_32]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
