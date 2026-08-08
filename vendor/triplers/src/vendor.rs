crate::triple_part! {
    Vendor {
        Microsoft,
        Freescale,
        ImagTech,
        MipsTech,
        NodeLlvm,
        Unknown,
        OpenEmb,
        Nvidia,
        Intel,
        Apple,
        SCEI,
        Mesa,
        SUSE,
        Meta,
        Kvin,
        IBM,
        CSR,
        PC,
    }

    canon(self) {
        self.into()
    }

    parse(src) {
        if prefix!("microsoft" src) { return Some((Vendor::Microsoft, 9)); }
        if prefix!("freescale" src) { return Some((Vendor::Freescale, 9)); }
        if prefix!("imagtech"  src) { return Some((Vendor::ImagTech,  8)); }
        if prefix!("mipstech"  src) { return Some((Vendor::MipsTech,  8)); }
        if prefix!("nodellvm"  src) { return Some((Vendor::NodeLlvm,  8)); }
        if prefix!("unknown"   src) { return Some((Vendor::Unknown,   7)); }
        if prefix!("openemb"   src) { return Some((Vendor::OpenEmb,   7)); }
        if prefix!("nvidia"    src) { return Some((Vendor::Nvidia,    6)); }
        if prefix!("intel"     src) { return Some((Vendor::Intel,     5)); }
        if prefix!("apple"     src) { return Some((Vendor::Apple,     5)); }
        if prefix!("scei"      src) { return Some((Vendor::SCEI,      4)); }
        if prefix!("mesa"      src) { return Some((Vendor::Mesa,      4)); }
        if prefix!("suse"      src) { return Some((Vendor::SUSE,      4)); }
        if prefix!("meta"      src) { return Some((Vendor::Meta,      4)); }
        if prefix!("kvin"      src) { return Some((Vendor::Kvin,      4)); }
        if prefix!("ibm"       src) { return Some((Vendor::IBM,       3)); }
        if prefix!("csr"       src) { return Some((Vendor::CSR,       3)); }
        if prefix!("pc"        src) { return Some((Vendor::PC,        2)); }
        None
    }

    bits(self) { 0 }
}
