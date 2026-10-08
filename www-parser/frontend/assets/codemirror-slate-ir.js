CodeMirror.defineMode('slate-ir', () => {
  const words = (text) => new Set(text.split(/\s+/));
  const keywords = words('module target pointer storage type struct union enum global extern fn let return yield if else while do for switch case default label break continue goto asm init condition increment body template rejected clobbers labels in out lateout inout inlateout from comment param');
  const types = words('void bool va_list complex imaginary vector fixed sat_fixed ptr array vla pair quad coerce byval byref sret');
  const operations = words('const null code_units aggregate copy read write store update compare_exchange old addr_of array_decay function_decay label_addr ptr_offset ptr_diff conditional sequence call va_arg va_start va_end va_copy widen truncate reinterpret bit_cast from_bool int_to_float float_widen float_narrow float_convert float_to_int pointer_cast address_space_cast ptr_to_int int_to_ptr enum_to_int int_to_enum real_to_complex complex_to_real complex_to_imag complex_convert real_to_imaginary imaginary_to_real imaginary_to_complex complex_to_imaginary imaginary_convert vector_splat vector_bit_cast int_to_fixed fixed_to_int float_to_fixed fixed_to_float fixed_convert add sub mul div rem and or xor shl shr minnum maxnum minimum maximum minimum_num maximum_num neg not eq ne lt le gt ge logical_and logical_or overflow_add overflow_sub overflow_mul float_class lane shuffle intrinsic deref index real imag swizzle compound_literal temporary fence dynamic ret addr sym mem entry_label');
  const atoms = words('true false Some None inf nan undef incomplete unprototyped omitted const volatile atomic restrict constexpr synthetic unsequenced automatic static thread internal external little big required ub wrap trap saturate nearest_even environment toward_zero ignore observable always unknown off on fast basic full sign_extend zero_extend relaxed consume acquire release acq_rel seq_cst device workgroup wavefront single cluster write_back success new assign arg vararg promotion usual_arith explicit ret_zero ret_void ub_if_used emitted inline_only hint never none scalar direct native_c sysv64 win64 x86_cdecl x86_win32 aapcs64 win_arm64 aapcs32 aapcs32_hard_float stdcall fastcall vectorcall thiscall ptr32_sptr ptr32_uptr ptr64 leading trailing detached doc att intel pure nomem readonly nostack preserves_flags may_unwind');

  function string(stream, state) {
    while (!stream.eol()) {
      const character = stream.next();
      if (character === '\\') stream.next();
      else if (character === '"') { state.string = false; break; }
    }
    return 'string';
  }

  return {
    startState: () => ({ string: false, comment: false }),
    token(stream, state) {
      if (state.string) return string(stream, state);
      if (state.comment) {
        if (stream.skipTo('*/')) { stream.match('*/'); state.comment = false; }
        else stream.skipToEnd();
        return 'comment';
      }
      if (stream.eatSpace()) return null;
      if (stream.sol() && stream.match(/===.*===/)) return 'comment';
      if (stream.match('//')) { stream.skipToEnd(); return 'comment'; }
      if (stream.match('/*')) { state.comment = true; return 'comment'; }
      if (stream.eat('"')) { state.string = true; return string(stream, state); }
      if (stream.match(/@type\d+\b/)) return 'type';
      if (stream.match(/@[A-Za-z_][\w.]*/)) return 'def';
      if (stream.match(/%[A-Za-z]?\d+\b/)) return 'variable-2';
      if (stream.match(/(?:[iu]\d+b?|bf16|[fd]\d+)\b/)) return 'type';
      if (stream.match(/-?(?:0x[\da-f]+|\d+(?:\.\d+)?(?:[eE][+-]?\d+)?)/i)) return 'number';
      if (stream.match(/llvm\.[\w.]+/)) return 'builtin';
      if (stream.match(/[A-Za-z_][\w]*/)) {
        const word = stream.current();
        if (stream.match(/^\s*=/, false)) return 'property';
        if (types.has(word)) return 'type';
        if (keywords.has(word)) return 'keyword';
        if (operations.has(word) || /^(?:field|bitfield|index)\d+$/.test(word)) return 'builtin';
        if (atoms.has(word)) return 'atom';
        return 'variable';
      }
      if (stream.match(/->|\.\.=?|[=+*/<>|&!^-]/)) return 'operator';
      stream.next();
      return null;
    },
  };
});

CodeMirror.defineMIME('text/x-slate-ir', 'slate-ir');
