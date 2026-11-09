/* { dg-options "/Zc:preprocessor /O2 /MT /TC" } */
#include <windows.h>
#include <string.h>
void abort(void);

#pragma comment(linker, "/section:.rdata,W")

static DWORD characteristics(const char *name) {
  char *base = (char *)GetModuleHandleA(NULL);
  IMAGE_NT_HEADERS *nt = (IMAGE_NT_HEADERS *)(base + ((IMAGE_DOS_HEADER *)base)->e_lfanew);
  IMAGE_SECTION_HEADER *section = IMAGE_FIRST_SECTION(nt);
  for (int i = 0; i < nt->FileHeader.NumberOfSections; i++)
    if (strncmp((const char *)section[i].Name, name, 8) == 0)
      return section[i].Characteristics;
  return 0;
}

int main(void) {
  DWORD flags = characteristics(".rdata");
  if (flags == 0 || !(flags & IMAGE_SCN_MEM_WRITE))
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-pc-windows-msvc
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "/Zc:preprocessor /O2 /MT /TC" }
// CHECK-NEXT: mod __slate_oldnames {
// CHECK-NEXT:     #[link(name = "oldnames")]
// CHECK-NEXT:     unsafe extern "C" {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn GetModuleHandleA([[ARG_0:arg[0-9]+]]: *const i8) -> *mut HINSTANCE__;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, packed(2))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct _IMAGE_DOS_HEADER {
// CHECK-NEXT:     e_magic: u16,
// CHECK-NEXT:     e_cblp: u16,
// CHECK-NEXT:     e_cp: u16,
// CHECK-NEXT:     e_crlc: u16,
// CHECK-NEXT:     e_cparhdr: u16,
// CHECK-NEXT:     e_minalloc: u16,
// CHECK-NEXT:     e_maxalloc: u16,
// CHECK-NEXT:     e_ss: u16,
// CHECK-NEXT:     e_sp: u16,
// CHECK-NEXT:     e_csum: u16,
// CHECK-NEXT:     e_ip: u16,
// CHECK-NEXT:     e_cs: u16,
// CHECK-NEXT:     e_lfarlc: u16,
// CHECK-NEXT:     e_ovno: u16,
// CHECK-NEXT:     e_res: [u16; 4],
// CHECK-NEXT:     e_oemid: u16,
// CHECK-NEXT:     e_oeminfo: u16,
// CHECK-NEXT:     e_res2: [u16; 10],
// CHECK-NEXT:     e_lfanew: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct _IMAGE_FILE_HEADER {
// CHECK-NEXT:     Machine: u16,
// CHECK-NEXT:     NumberOfSections: u16,
// CHECK-NEXT:     TimeDateStamp: u32,
// CHECK-NEXT:     PointerToSymbolTable: u32,
// CHECK-NEXT:     NumberOfSymbols: u32,
// CHECK-NEXT:     SizeOfOptionalHeader: u16,
// CHECK-NEXT:     Characteristics: u16,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct _IMAGE_DATA_DIRECTORY {
// CHECK-NEXT:     VirtualAddress: u32,
// CHECK-NEXT:     Size: u32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, packed(4))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct _IMAGE_OPTIONAL_HEADER64 {
// CHECK-NEXT:     Magic: u16,
// CHECK-NEXT:     MajorLinkerVersion: u8,
// CHECK-NEXT:     MinorLinkerVersion: u8,
// CHECK-NEXT:     SizeOfCode: u32,
// CHECK-NEXT:     SizeOfInitializedData: u32,
// CHECK-NEXT:     SizeOfUninitializedData: u32,
// CHECK-NEXT:     AddressOfEntryPoint: u32,
// CHECK-NEXT:     BaseOfCode: u32,
// CHECK-NEXT:     ImageBase: u64,
// CHECK-NEXT:     SectionAlignment: u32,
// CHECK-NEXT:     FileAlignment: u32,
// CHECK-NEXT:     MajorOperatingSystemVersion: u16,
// CHECK-NEXT:     MinorOperatingSystemVersion: u16,
// CHECK-NEXT:     MajorImageVersion: u16,
// CHECK-NEXT:     MinorImageVersion: u16,
// CHECK-NEXT:     MajorSubsystemVersion: u16,
// CHECK-NEXT:     MinorSubsystemVersion: u16,
// CHECK-NEXT:     Win32VersionValue: u32,
// CHECK-NEXT:     SizeOfImage: u32,
// CHECK-NEXT:     SizeOfHeaders: u32,
// CHECK-NEXT:     CheckSum: u32,
// CHECK-NEXT:     Subsystem: u16,
// CHECK-NEXT:     DllCharacteristics: u16,
// CHECK-NEXT:     SizeOfStackReserve: u64,
// CHECK-NEXT:     SizeOfStackCommit: u64,
// CHECK-NEXT:     SizeOfHeapReserve: u64,
// CHECK-NEXT:     SizeOfHeapCommit: u64,
// CHECK-NEXT:     LoaderFlags: u32,
// CHECK-NEXT:     NumberOfRvaAndSizes: u32,
// CHECK-NEXT:     DataDirectory: [_IMAGE_DATA_DIRECTORY; 16],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct _IMAGE_NT_HEADERS64 {
// CHECK-NEXT:     Signature: u32,
// CHECK-NEXT:     FileHeader: _IMAGE_FILE_HEADER,
// CHECK-NEXT:     OptionalHeader: _IMAGE_OPTIONAL_HEADER64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct _IMAGE_SECTION_HEADER {
// CHECK-NEXT:     Name: [u8; 8],
// CHECK-NEXT:     Misc: __SlateRecord_winnt_h_20113,
// CHECK-NEXT:     VirtualAddress: u32,
// CHECK-NEXT:     SizeOfRawData: u32,
// CHECK-NEXT:     PointerToRawData: u32,
// CHECK-NEXT:     PointerToRelocations: u32,
// CHECK-NEXT:     PointerToLinenumbers: u32,
// CHECK-NEXT:     NumberOfRelocations: u16,
// CHECK-NEXT:     NumberOfLinenumbers: u16,
// CHECK-NEXT:     Characteristics: u32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union __SlateRecord_winnt_h_20113 {
// CHECK-NEXT:     PhysicalAddress: u32,
// CHECK-NEXT:     VirtualSize: u32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct HINSTANCE__ {
// CHECK-NEXT:     unused: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: use slate_support::abort;
// CHECK-NEXT: use slate_support::strncmp;
// CHECK-EMPTY:
// CHECK-NEXT: core::arch::global_asm!(".section .drectve,\"yni\"\n.ascii \" /section:.rdata,W\"");
// CHECK-EMPTY:
// CHECK-NEXT: fn characteristics(name: *const i8) -> u32 {
// CHECK-NEXT:     let base: *mut i8 = (unsafe { GetModuleHandleA(std::ptr::null::<i8>()) }) as *mut i8;
// CHECK-NEXT:     let nt: *mut _IMAGE_NT_HEADERS64 =
// CHECK-NEXT:         (unsafe { base.offset((*(base as *mut _IMAGE_DOS_HEADER)).e_lfanew as isize) })
// CHECK-NEXT:             as *mut _IMAGE_NT_HEADERS64;
// CHECK-NEXT:     let section: *mut _IMAGE_SECTION_HEADER = (nt as u64)
// CHECK-NEXT:         .wrapping_add(24)
// CHECK-NEXT:         .wrapping_add(((unsafe { (*nt).FileHeader.SizeOfOptionalHeader }) as i32) as u64)
// CHECK-NEXT:         as *mut _IMAGE_SECTION_HEADER;
// CHECK-NEXT:     let mut i: i32 = 0;
// CHECK-NEXT:     while i < ((unsafe { (*nt).FileHeader.NumberOfSections }) as i32) {
// CHECK-NEXT:         if (unsafe {
// CHECK-NEXT:             strncmp(
// CHECK-NEXT:                 (((*section.offset(i as isize)).Name.as_mut_ptr() as *mut u8) as *const i8)
// CHECK-NEXT:                     as *const core::ffi::c_char,
// CHECK-NEXT:                 name as *const core::ffi::c_char,
// CHECK-NEXT:                 8u64 as usize,
// CHECK-NEXT:             ) as i32
// CHECK-NEXT:         }) == 0
// CHECK-NEXT:         {
// CHECK-NEXT:             return unsafe { (*section.offset(i as isize)).Characteristics };
// CHECK-NEXT:         }
// CHECK-NEXT:         i += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let flags: u32 = characteristics(c".rdata".as_ptr());
// CHECK-NEXT:     if flags == 0 || flags & 2147483648 == 0 {
// CHECK-NEXT:         unsafe { std::process::abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
