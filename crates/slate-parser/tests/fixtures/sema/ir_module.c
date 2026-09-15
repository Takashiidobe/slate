// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

static short narrow(void) {
    return 65537;
}

double widen(void) {
    return 2;
}

int arithmetic(void) {
    1 + 2;
    {
        return 3 * 4;
    }
}

void empty(void) {
    return;
}

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     fn @narrow() -> i16 [linkage=internal] {
// IR-NEXT:         return truncate<i16, reason=return, fits=unknown>(const<i32>(65537));
// IR-NEXT:     }
// IR-NEXT:     fn @widen() -> f64 [linkage=external] {
// IR-NEXT:         return int_to_float<f64, reason=return, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2));
// IR-NEXT:     }
// IR-NEXT:     fn @arithmetic() -> i32 [linkage=external] {
// IR-NEXT:         add<i32, overflow=undefined>(const<i32>(1), const<i32>(2));
// IR-NEXT:         {
// IR-NEXT:             return mul<i32, overflow=undefined>(const<i32>(3), const<i32>(4));
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT:     fn @empty() -> void [linkage=external] {
// IR-NEXT:         return;
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
