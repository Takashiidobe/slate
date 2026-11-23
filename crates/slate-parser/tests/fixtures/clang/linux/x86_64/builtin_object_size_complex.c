// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

#define SIZES(p) (__builtin_object_size(p, 0) + \
                  100UL * __builtin_object_size(p, 1) + \
                  10000UL * __builtin_object_size(p, 2) + \
                  1000000UL * __builtin_object_size(p, 3))

struct Record {
  int head;
  double _Complex value;
  int tail;
};

unsigned long real_sizes(void) {
  double _Complex z;
  return SIZES(&__real__ z);
}

unsigned long imag_sizes(void) {
  double _Complex z;
  return SIZES(&__imag__ z);
}

unsigned long field_sizes(void) {
  struct Record r;
  return SIZES(&__imag__ r.value);
}

unsigned long element_sizes(void) {
  double _Complex a[2];
  return SIZES(&__real__ a[0]);
}

unsigned long wide_imag(void) {
  long double _Complex z;
  return __builtin_dynamic_object_size(&__imag__ z, 3);
}

unsigned long unknown_component(double _Complex *z) {
  return __builtin_object_size(&__real__ *z, 3);
}

unsigned long unknown_record(struct Record *r) {
  return __builtin_object_size(&__imag__ r->value, 3);
}

unsigned long component_offset(void) {
  double _Complex z;
  return __builtin_object_size(&__real__ z + 1, 3);
}

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type[[TYPE_Record:[0-9]+]] Record = struct {
// IR-NEXT:         field0 head: i32;
// IR-NEXT:         field1 value: complex<f64>;
// IR-NEXT:         field2 tail: i32;
// IR-NEXT:     } [size=32, align=8, offsets=[0, 8, 24]];
// IR-NEXT:     fn %[[VALUE_real_sizes:[0-9]+]] @real_sizes() -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_z:[0-9]+]] z: complex<f64> [storage=automatic];
// IR-NEXT:         return reinterpret<u64>(add<u64>(add<u64>(add<u64>(const<u64>(16) [c="__size_t"], reinterpret<u64>(mul<u64>(const<u64>(100), reinterpret<u64>(const<u64>(16) [c="__size_t"]))) [c="__size_t"]) [c="__size_t"], reinterpret<u64>(mul<u64>(const<u64>(10000), reinterpret<u64>(const<u64>(16) [c="__size_t"]))) [c="__size_t"]) [c="__size_t"], reinterpret<u64>(mul<u64>(const<u64>(1000000), reinterpret<u64>(const<u64>(16) [c="__size_t"]))) [c="__size_t"]) [c="__size_t"]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_imag_sizes:[0-9]+]] @imag_sizes() -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_z_2:[0-9]+]] z: complex<f64> [storage=automatic];
// IR-NEXT:         return reinterpret<u64>(add<u64>(add<u64>(add<u64>(const<u64>(8) [c="__size_t"], reinterpret<u64>(mul<u64>(const<u64>(100), reinterpret<u64>(const<u64>(8) [c="__size_t"]))) [c="__size_t"]) [c="__size_t"], reinterpret<u64>(mul<u64>(const<u64>(10000), reinterpret<u64>(const<u64>(8) [c="__size_t"]))) [c="__size_t"]) [c="__size_t"], reinterpret<u64>(mul<u64>(const<u64>(1000000), reinterpret<u64>(const<u64>(8) [c="__size_t"]))) [c="__size_t"]) [c="__size_t"]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_field_sizes:[0-9]+]] @field_sizes() -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_r:[0-9]+]] r: @type[[TYPE_Record]] [storage=automatic];
// IR-NEXT:         return reinterpret<u64>(add<u64>(add<u64>(add<u64>(const<u64>(16) [c="__size_t"], reinterpret<u64>(mul<u64>(const<u64>(100), reinterpret<u64>(const<u64>(8) [c="__size_t"]))) [c="__size_t"]) [c="__size_t"], reinterpret<u64>(mul<u64>(const<u64>(10000), reinterpret<u64>(const<u64>(16) [c="__size_t"]))) [c="__size_t"]) [c="__size_t"], reinterpret<u64>(mul<u64>(const<u64>(1000000), reinterpret<u64>(const<u64>(8) [c="__size_t"]))) [c="__size_t"]) [c="__size_t"]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_element_sizes:[0-9]+]] @element_sizes() -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_a:[0-9]+]] a: array<complex<f64>, 2> [storage=automatic] [align=16];
// IR-NEXT:         return reinterpret<u64>(add<u64>(add<u64>(add<u64>(const<u64>(32) [c="__size_t"], reinterpret<u64>(mul<u64>(const<u64>(100), reinterpret<u64>(const<u64>(16) [c="__size_t"]))) [c="__size_t"]) [c="__size_t"], reinterpret<u64>(mul<u64>(const<u64>(10000), reinterpret<u64>(const<u64>(32) [c="__size_t"]))) [c="__size_t"]) [c="__size_t"], reinterpret<u64>(mul<u64>(const<u64>(1000000), reinterpret<u64>(const<u64>(16) [c="__size_t"]))) [c="__size_t"]) [c="__size_t"]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_wide_imag:[0-9]+]] @wide_imag() -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_z_3:[0-9]+]] z: complex<f80> [storage=automatic];
// IR-NEXT:         return reinterpret<u64>(const<u64>(16) [c="__size_t"]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_unknown_component:[0-9]+]] @unknown_component(%[[VALUE_z_4:[0-9]+]] z: ptr<complex<f64>>) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return reinterpret<u64>(const<u64>(0) [c="__size_t"]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_unknown_record:[0-9]+]] @unknown_record(%[[VALUE_r_2:[0-9]+]] r: ptr<@type[[TYPE_Record]]>) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return reinterpret<u64>(const<u64>(8) [c="__size_t"]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_component_offset:[0-9]+]] @component_offset() -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_z_5:[0-9]+]] z: complex<f64> [storage=automatic];
// IR-NEXT:         return reinterpret<u64>(const<u64>(8) [c="__size_t"]);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
