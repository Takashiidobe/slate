#![feature(register_tool)]
#![register_tool(slate)]
#![allow(dead_code)]
#![allow(unused_imports, unused_variables, reason = "UI fixture")]
#![cfg_attr(all(), allow(unused_mut))]
#![cfg_attr(any(), allow(unused_assignments))]

#[allow(non_snake_case)]
fn LocalAllow() {}

mod local_allow {
    #![allow(non_snake_case)]

    fn InnerAllow() {}
}

fn main() {}
