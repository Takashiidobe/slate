#[derive(Debug, Default, Clone, Copy, PartialEq, Eq)]
pub enum Charset {
    #[default]
    Utf8,
    Windows1252,
}

const WINDOWS_1252_HIGH: [char; 32] = [
    '\u{20AC}', '\u{0081}', '\u{201A}', '\u{0192}', '\u{201E}', '\u{2026}', '\u{2020}', '\u{2021}',
    '\u{02C6}', '\u{2030}', '\u{0160}', '\u{2039}', '\u{0152}', '\u{008D}', '\u{017D}', '\u{008F}',
    '\u{0090}', '\u{2018}', '\u{2019}', '\u{201C}', '\u{201D}', '\u{2022}', '\u{2013}', '\u{2014}',
    '\u{02DC}', '\u{2122}', '\u{0161}', '\u{203A}', '\u{0153}', '\u{009D}', '\u{017E}', '\u{0178}',
];

impl Charset {
    pub fn parse_msvc(name: &str) -> Option<Self> {
        match name.to_ascii_lowercase().as_str() {
            "utf-8" | ".65001" => Some(Self::Utf8),
            "windows-1252" | ".1252" => Some(Self::Windows1252),
            _ => None,
        }
    }

    pub fn code_page(self) -> u32 {
        match self {
            Self::Utf8 => 65001,
            Self::Windows1252 => 1252,
        }
    }

    pub fn decode_byte(self, byte: u8) -> char {
        match (self, byte) {
            (Self::Windows1252, 0x80..=0x9F) => WINDOWS_1252_HIGH[usize::from(byte - 0x80)],
            _ => char::from(byte),
        }
    }

    pub fn encode(self, value: u32, units: &mut Vec<u32>) {
        match self {
            Self::Utf8 => encode_utf8(value, units),
            Self::Windows1252 => units.push(windows_1252_byte(value).map_or(0x3F, u32::from)),
        }
    }
}

fn windows_1252_byte(value: u32) -> Option<u8> {
    match value {
        0..=0x7F | 0xA0..=0xFF => Some(value as u8),
        _ => WINDOWS_1252_HIGH
            .iter()
            .position(|&c| c as u32 == value)
            .map(|index| 0x80 + index as u8),
    }
}

pub(crate) fn encode_utf8(value: u32, units: &mut Vec<u32>) {
    match value {
        0..=0x7F => units.push(value),
        0x80..=0x7FF => units.extend([0xC0 | value >> 6, 0x80 | value & 0x3F]),
        0x800..=0xFFFF => units.extend([
            0xE0 | value >> 12,
            0x80 | (value >> 6) & 0x3F,
            0x80 | value & 0x3F,
        ]),
        _ => units.extend([
            0xF0 | value >> 18,
            0x80 | (value >> 12) & 0x3F,
            0x80 | (value >> 6) & 0x3F,
            0x80 | value & 0x3F,
        ]),
    }
}
