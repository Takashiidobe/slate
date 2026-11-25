from generate_msvc_builtins import alphabetical_entries, documented_name, family_members


def test_alphabetical_entries():
    listing = (
        "[\x60_BitScanForward\x60, \x60_BitScanForward64\x60](bitscanforward)\n"
        "[\x60_InterlockedAnd\x60 intrinsic functions](interlockedand)\n"
        "[\x60__mul128\x60](mul128)\n"
    )
    assert alphabetical_entries(listing) == [
        ("_BitScanForward", False, "bitscanforward"),
        ("_BitScanForward64", False, "bitscanforward"),
        ("_InterlockedAnd", True, "interlockedand"),
        ("__mul128", False, "mul128"),
    ]
    assert documented_name("__mul128") == "_mul128"


def test_family_members():
    assert family_members(
        "_InterlockedAnd",
        "_InterlockedAnd8 _InterlockedAnd64_nf _InterlockedAndOther",
        ["_InterlockedAnd16", "_InterlockedAndExtra"],
    ) == {
        "_InterlockedAnd",
        "_InterlockedAnd8",
        "_InterlockedAnd16",
        "_InterlockedAnd64_nf",
    }


if __name__ == "__main__":
    test_alphabetical_entries()
    test_family_members()
