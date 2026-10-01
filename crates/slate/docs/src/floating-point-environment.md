# Floating-point environment

Floating-point pragmas and environment operations are target-sensitive. Their
semantic facts originate in slate-parser IR; Slate lowering and C bridges must
preserve them where supported.

The runtime support includes `frontend/shims/fenv.c`. Use `lowering-barriers`
and the differential fixtures to establish support for the exact pragma,
operation and target. Availability of `<fenv.h>` declarations alone is not an
end-to-end guarantee.
