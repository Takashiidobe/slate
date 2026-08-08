use crate::vendor::Vendor;
use crate::{Canonicalizable, Parsable};

#[test]
fn test_vendor_parse() {
    let cases = [
        ("microsoft", Vendor::Microsoft, 9),
        ("freescale", Vendor::Freescale, 9),
        ("imagtech", Vendor::ImagTech, 8),
        ("mipstech", Vendor::MipsTech, 8),
        ("unknown", Vendor::Unknown, 7),
        ("openemb", Vendor::OpenEmb, 7),
        ("nvidia", Vendor::Nvidia, 6),
        ("intel", Vendor::Intel, 5),
        ("apple", Vendor::Apple, 5),
        ("scei", Vendor::SCEI, 4),
        ("mesa", Vendor::Mesa, 4),
        ("suse", Vendor::SUSE, 4),
        ("meta", Vendor::Meta, 4),
        ("kvin", Vendor::Kvin, 4),
        ("ibm", Vendor::IBM, 3),
        ("csr", Vendor::CSR, 3),
        ("pc", Vendor::PC, 2),
    ];
    for (input, expected, expected_len) in cases {
        let (vendor, len) = Vendor::reduce(input).unwrap();
        assert_eq!(vendor, expected);
        assert_eq!(len, expected_len);
        assert_eq!(vendor.canonicalize(), input);
    }
}
