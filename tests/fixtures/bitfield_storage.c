union U {
  struct {
    unsigned first  : 1;
    unsigned second : 1;
  } bits;
  unsigned raw;
};

int main(void) {
  union U value     = {0};
  // @lowering-begin
  // @rewrite-begin
  value.bits.second = 1;
  // @rewrite-end
  // @lowering-end
  return value.bits.second != 1;
}

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: u32 = 1;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: u32 = ({{__v[0-9]+}} as u32) << 31 >> 31;
// COMMON-LOWERING-DAG: unsafe {
// COMMON-LOWERING-DAG:     value
// COMMON-LOWERING-DAG:         .bits
// COMMON-LOWERING-DAG:         .__bitfield_0
// COMMON-LOWERING-DAG:         .set_second(({{__v[0-9]+}} as u32) << 31 >> 31);
// COMMON-LOWERING-DAG: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: unsafe {
// COMMON-REWRITES-DAG:     value.bits.__bitfield_0.set_second((1 as u32) << 31 >> 31);
// COMMON-REWRITES-DAG: }
// SLATE-FILECHECK-END common-rewrites
