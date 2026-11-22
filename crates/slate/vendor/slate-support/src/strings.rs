use core::ffi::{CStr, c_char, c_long, c_longlong};

fn ato(bytes: &[u8]) -> i128 {
    let mut rest = bytes;
    while let [byte, tail @ ..] = rest
        && (byte.is_ascii_whitespace() || *byte == 0x0b)
    {
        rest = tail;
    }
    let negative = rest.first() == Some(&b'-');
    if let [b'+' | b'-', tail @ ..] = rest {
        rest = tail;
    }
    let mut value: i128 = 0;
    for &byte in rest.iter().take_while(|byte| byte.is_ascii_digit()) {
        value = value
            .saturating_mul(10)
            .saturating_add(i128::from(byte - b'0'));
    }
    if negative { -value } else { value }
}

pub unsafe fn __slate_atoi(s: *const c_char) -> i32 {
    let value = ato(unsafe { CStr::from_ptr(s) }.to_bytes());
    value.clamp(i32::MIN.into(), i32::MAX.into()) as i32
}

pub unsafe fn __slate_atol(s: *const c_char) -> c_long {
    let value = ato(unsafe { CStr::from_ptr(s) }.to_bytes());
    value.clamp(c_long::MIN.into(), c_long::MAX.into()) as c_long
}

pub unsafe fn __slate_atoll(s: *const c_char) -> c_longlong {
    let value = ato(unsafe { CStr::from_ptr(s) }.to_bytes());
    value.clamp(c_longlong::MIN.into(), c_longlong::MAX.into()) as c_longlong
}

pub fn __slate_strspn(s: impl AsRef<[u8]>, accept: impl AsRef<[u8]>) -> usize {
    let accept = accept.as_ref();
    s.as_ref()
        .iter()
        .take_while(|byte| accept.contains(byte))
        .count()
}

pub fn __slate_strcspn(s: impl AsRef<[u8]>, reject: impl AsRef<[u8]>) -> usize {
    let reject = reject.as_ref();
    s.as_ref()
        .iter()
        .take_while(|byte| !reject.contains(byte))
        .count()
}
