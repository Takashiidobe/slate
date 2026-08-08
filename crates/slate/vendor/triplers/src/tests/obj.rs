use crate::obj::Obj;
use crate::{Canonicalizable, Parsable};

#[test]
fn test_obj_parse() {
    let cases = [
        ("dxcontainer", Obj::DXContainer, 11),
        ("macho", Obj::MachO, 5),
        ("spirv", Obj::SPIRV, 5),
        ("xcoff", Obj::XCOFF, 5),
        ("coff", Obj::COFF, 4),
        ("goff", Obj::GOFF, 4),
        ("wasm", Obj::Wasm, 4),
        ("elf", Obj::ELF, 3),
    ];
    for (input, expected, expected_len) in cases {
        let (obj, len) = Obj::reduce(input).unwrap();
        assert_eq!(obj, expected);
        assert_eq!(len, expected_len);
        assert_eq!(obj.canonicalize(), input);
    }
}
