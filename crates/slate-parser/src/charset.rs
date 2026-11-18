use crate::code_pages::{CODE_PAGES, CP1252};
use crate::gnu_charsets::GNU_CHARSETS;
use crate::target_info::Endian;

#[derive(Debug, Default, Clone, Copy, PartialEq, Eq)]
pub enum Charset {
    #[default]
    Utf8,
    CodePage(&'static CodePage),
    Table(&'static ByteTable),
}

#[derive(Debug)]
pub struct CodePage {
    pub(crate) number: u32,
    pub(crate) names: &'static [&'static str],
    pub(crate) high: [char; 128],
}

#[derive(Debug)]
pub struct ByteTable {
    pub(crate) number: u32,
    pub(crate) names: &'static [&'static str],
    pub(crate) table: [Option<char>; 256],
}

impl PartialEq for ByteTable {
    fn eq(&self, other: &Self) -> bool {
        self.number == other.number
    }
}

impl Eq for ByteTable {}

impl ByteTable {
    fn byte(&self, value: u32) -> Option<u8> {
        self.table
            .iter()
            .position(|&c| c.is_some_and(|c| c as u32 == value))
            .map(|index| index as u8)
    }
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
            .iter()
            .copied()
            .find(|page| {
                page.names.contains(&name.as_str())
                    || name.strip_prefix('.') == Some(page.number.to_string().as_str())
            })
            .map(Self::CodePage)
    }

    pub fn parse_gnu(name: &str) -> Option<Self> {
        let name = name.to_ascii_lowercase();
        if matches!(name.as_str(), "utf-8" | "utf8") {
            return Some(Self::Utf8);
        }
        GNU_CHARSETS
            .iter()
            .copied()
            .find(|table| table.names.contains(&name.as_str()))
            .map(Self::Table)
    }

    pub fn code_page(self) -> u32 {
        match self {
            Self::Utf8 => 65001,
            Self::CodePage(page) => page.number,
            Self::Table(table) => table.number,
        }
    }

    pub fn decode_byte(self, byte: u8) -> char {
        match (self, byte) {
            (Self::CodePage(page), 0x80..=0xFF) => page.high[usize::from(byte - 0x80)],
            (Self::Table(table), _) => table.table[usize::from(byte)].unwrap_or('\u{FFFD}'),
            _ => char::from(byte),
        }
    }

    pub fn represents(self, value: u32) -> bool {
        match self {
            Self::Utf8 => true,
            Self::CodePage(page) => page.byte(value).is_some(),
            Self::Table(table) => table.byte(value).is_some(),
        }
    }

    pub fn encode(self, value: u32, units: &mut Vec<u32>) {
        match self {
            Self::Utf8 => encode_utf8(value, units),
            Self::CodePage(page) => units.push(page.byte(value).map_or(0x3F, u32::from)),
            Self::Table(table) => units.push(table.byte(value).map_or(0x3F, u32::from)),
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct WideCharset {
    pub width: u32,
    pub endian: Endian,
}

impl WideCharset {
    pub fn parse_gnu(name: &str) -> Option<Self> {
        let (width, endian) = match name.to_ascii_uppercase().as_str() {
            "UTF-32LE" | "UCS-4LE" => (32, Endian::Little),
            "UTF-32BE" | "UCS-4BE" | "UCS-4" => (32, Endian::Big),
            "UTF-16LE" => (16, Endian::Little),
            "UTF-16BE" => (16, Endian::Big),
            _ => return None,
        };
        Some(Self { width, endian })
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
