[[deprecated("old")]] int old_api;
[[nodiscard]] int status(void);
[[maybe_unused]] int unused_data;
[[vendor::custom(7)]] int vendor_data;

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: polyvariant:
// DEFAULT-NEXT: decl[0]: declaration type=int declarator=name=old_api [attributes=deprecated("old")]
// DEFAULT-NEXT: decl[1]: declaration type=int declarator=function(name=status,params=,variadic=false) [attributes=nodiscard]
// DEFAULT-NEXT: decl[2]: declaration type=int declarator=name=unused_data [attributes=maybe_unused]
// DEFAULT-NEXT: decl[3]: declaration type=int declarator=name=vendor_data [attributes=vendor::custom(7)]
// DEFAULT-NEXT: concrete:
// DEFAULT-NEXT: decl[0]: declaration type=int declarator=name=old_api [attributes=deprecated("old")]
// DEFAULT-NEXT: decl[1]: declaration type=int declarator=function(name=status,params=,variadic=false) [attributes=nodiscard]
// DEFAULT-NEXT: decl[2]: declaration type=int declarator=name=unused_data [attributes=maybe_unused]
// DEFAULT-NEXT: decl[3]: declaration type=int declarator=name=vendor_data [attributes=vendor::custom(7)]
// SLATE-FILECHECK-END DEFAULT
