typedef enum { POSITION_ASIS, POSITION_UNSPECIFIED } unit_position;

typedef enum { STATUS_UNKNOWN, STATUS_UNSPECIFIED } unit_status;

typedef struct {
  unit_position position;
  unit_status   status;
} unit_flags;

extern void abort(void);

void new_unit(unit_flags *flags) {
  if (flags->status == STATUS_UNSPECIFIED)
    flags->status = STATUS_UNKNOWN;

  if (flags->position == POSITION_UNSPECIFIED)
    flags->position = POSITION_ASIS;

  switch (flags->status) {
  case STATUS_UNKNOWN:
    break;

  default:
    abort();
  }
}

int main() {
  unit_flags f;
  f.status = STATUS_UNSPECIFIED;
  new_unit(&f);
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
// DEFAULT-NEXT:     type @type0 = enum : u32 {
// DEFAULT-NEXT:         %0 POSITION_ASIS = const<i32>(0);
// DEFAULT-NEXT:         %1 POSITION_UNSPECIFIED = const<i32>(1);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type1 unit_position = @type0;
// DEFAULT-NEXT:     type @type2 = enum : u32 {
// DEFAULT-NEXT:         %0 STATUS_UNKNOWN = const<i32>(0);
// DEFAULT-NEXT:         %1 STATUS_UNSPECIFIED = const<i32>(1);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type3 unit_status = @type2;
// DEFAULT-NEXT:     type @type4 = struct {
// DEFAULT-NEXT:         field0 position: @type0;
// DEFAULT-NEXT:         field1 status: @type2;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type5 unit_flags = @type4;
// DEFAULT-NEXT:     fn %10 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %11 @new_unit(%12 flags: ptr<@type4>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<u32>(enum_to_int<u32, reason=promotion>(read<@type2>(field1(deref(read<ptr<@type4>>(%12))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))
// DEFAULT-NEXT:             write<@type2>(field1(deref(read<ptr<@type4>>(%12))), int_to_enum<@type2, reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if eq<u32>(enum_to_int<u32, reason=promotion>(read<@type0>(field0(deref(read<ptr<@type4>>(%12))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))
// DEFAULT-NEXT:             write<@type0>(field0(deref(read<ptr<@type4>>(%12))), int_to_enum<@type0, reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         switch %15 enum_to_int<u32, reason=promotion>(read<@type2>(field1(deref(read<ptr<@type4>>(%12)))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %15 const<u32>(0):
// DEFAULT-NEXT:                     break %15;
// DEFAULT-NEXT:                 default %15:
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%10);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %14 f: @type4 [storage=automatic];
// DEFAULT-NEXT:         write<@type2>(field1(%14), int_to_enum<@type2, reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type4>) -> void>(%11, addr_of<ptr<@type4>>(%14));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
