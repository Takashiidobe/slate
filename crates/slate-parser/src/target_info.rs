#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct TargetInfo {
    pub char_signed: bool,
    pub short_width: u32,
    pub int_width: u32,
    pub long_width: u32,
    pub long_long_width: u32,
    pub pointer_width: u32,
    pub wchar_signed: bool,
    pub wchar_width: u32,
}

impl Default for TargetInfo {
    fn default() -> Self {
        Self {
            char_signed: true,
            short_width: 16,
            int_width: 32,
            long_width: 64,
            long_long_width: 64,
            pointer_width: 64,
            wchar_signed: true,
            wchar_width: 32,
        }
    }
}
