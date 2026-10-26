use crate::code_pages::{CODE_PAGES, CP1252};

#[derive(Debug, Default, Clone, Copy, PartialEq, Eq)]
pub enum Charset {
    #[default]
    Utf8,
    CodePage(&'static CodePage),
}

#[derive(Debug)]
pub struct CodePage {
    pub(crate) number: u32,
    pub(crate) names: &'static [&'static str],
    pub(crate) high: [char; 128],
}

impl PartialEq for CodePage {
    fn eq(&self, other: &Self) -> bool {
        self.number == other.number
    }
}

impl Eq for CodePage {}

impl CodePage {
    fn byte(&self, value: u32) -> Option<u8> {
        match value {
            0..=0x7F => Some(value as u8),
            _ => self
                .high
                .iter()
                .position(|&c| c as u32 == value)
                .map(|index| 0x80 + index as u8),
        }
    }
}

impl Charset {
    pub const WINDOWS_1252: Self = Self::CodePage(&CP1252);

    pub fn parse_msvc(name: &str) -> Option<Self> {
        let name = name.to_ascii_lowercase();
        if name == "utf-8" || name == ".65001" {
            return Some(Self::Utf8);
        }
        CODE_PAGES
            .into_iter()
            .find(|page| {
                page.names.contains(&name.as_str())
                    || name.strip_prefix('.') == Some(page.number.to_string().as_str())
            })
            .map(Self::CodePage)
    }

    pub fn code_page(self) -> u32 {
        match self {
            Self::Utf8 => 65001,
            Self::CodePage(page) => page.number,
        }
    }

    pub fn decode_byte(self, byte: u8) -> char {
        match (self, byte) {
            (Self::CodePage(page), 0x80..=0xFF) => page.high[usize::from(byte - 0x80)],
            _ => char::from(byte),
        }
    }

    pub fn represents(self, value: u32) -> bool {
        match self {
            Self::Utf8 => true,
            Self::CodePage(page) => page.byte(value).is_some(),
        }
    }

    pub fn encode(self, value: u32, units: &mut Vec<u32>) {
        match self {
            Self::Utf8 => encode_utf8(value, units),
            Self::CodePage(page) => units.push(page.byte(value).map_or(0x3F, u32::from)),
        }
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
