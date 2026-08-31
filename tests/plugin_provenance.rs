use serde_json::Value;
use std::collections::BTreeMap;
use std::path::{Path, PathBuf};
use std::process::Command;

struct Case {
    dir: PathBuf,
    source: PathBuf,
}

impl Case {
    fn new(source: &str, headers: &[(&str, &str)]) -> Self {
        let dir = std::env::temp_dir().join(format!(
            "slate-plugin-provenance-{}-{}",
            std::process::id(),
            std::time::SystemTime::now()
                .duration_since(std::time::UNIX_EPOCH)
                .expect("clock before epoch")
                .as_nanos()
        ));
        std::fs::create_dir(&dir).expect("create case directory");
        let source_path = dir.join("input.c");
        std::fs::write(&source_path, source).expect("write input.c");
        for (name, contents) in headers {
            std::fs::write(dir.join(name), contents).expect("write header");
        }
        Self {
            dir,
            source: source_path,
        }
    }

    fn events(&self) -> Vec<Value> {
        self.tagged_events(
            "FUNCTION_PROVENANCE ",
            &["-I", self.dir.to_str().expect("UTF-8 case path")],
        )
    }

    fn events_with_args(&self, args: &[&str]) -> Vec<Value> {
        self.tagged_events("FUNCTION_PROVENANCE ", args)
    }

    fn include_events(&self) -> Vec<Value> {
        self.tagged_events(
            "INCLUDE_PROVENANCE ",
            &["-I", self.dir.to_str().expect("UTF-8 case path")],
        )
    }

    fn macro_events(&self) -> Vec<Value> {
        self.tagged_events(
            "MACRO_EXPANSION ",
            &["-I", self.dir.to_str().expect("UTF-8 case path")],
        )
    }

    fn pack_events(&self) -> Vec<Value> {
        self.tagged_events("RECORD_PACKING ", &[])
    }

    fn tagged_events(&self, prefix: &str, args: &[&str]) -> Vec<Value> {
        let output = Command::new(clang())
            .arg(format!("-fplugin={}", plugin().display()))
            .arg("-fsyntax-only")
            .args(args)
            .arg(&self.source)
            .output()
            .expect("run clang");
        assert!(
            output.status.success(),
            "clang failed:\n{}",
            String::from_utf8_lossy(&output.stderr)
        );
        String::from_utf8_lossy(&output.stderr)
            .lines()
            .filter_map(|line| line.strip_prefix(prefix))
            .map(|json| serde_json::from_str(json).expect("valid provenance JSON"))
            .collect()
    }
}

impl Drop for Case {
    fn drop(&mut self) {
        std::fs::remove_dir_all(&self.dir).expect("remove case directory");
    }
}

fn clang() -> String {
    std::env::var("SLATE_CLANG").unwrap_or_else(|_| {
        format!(
            "{}/llvm-project/build-cir/bin/clang",
            std::env::var("HOME").expect("HOME not set")
        )
    })
}

fn plugin() -> PathBuf {
    if let Ok(path) = std::env::var("SLATE_MACRO_DUMP_PLUGIN") {
        return PathBuf::from(path);
    }
    Path::new(&clang())
        .parent()
        .and_then(Path::parent)
        .expect("clang must be in a build bin directory")
        .join("lib/SlateMacroDump.so")
}

fn by_name(events: Vec<Value>, name: &str) -> Vec<Value> {
    events
        .into_iter()
        .filter(|event| event.get("name").and_then(Value::as_str) == Some(name))
        .collect()
}

fn event(case: &Case, name: &str) -> Value {
    let events = by_name(case.events(), name);
    assert_eq!(events.len(), 1, "events: {events:#?}");
    events.into_iter().next().expect("one event")
}

fn provenance(event: &Value) -> &str {
    event
        .get("provenance")
        .and_then(Value::as_str)
        .expect("provenance string")
}

fn headers(event: &Value) -> Vec<&str> {
    event
        .get("headers")
        .and_then(Value::as_array)
        .expect("headers array")
        .iter()
        .map(|header| header.as_str().expect("header string"))
        .collect()
}

fn reasons(event: &Value) -> Vec<&str> {
    event
        .get("reasons")
        .and_then(Value::as_array)
        .expect("reasons array")
        .iter()
        .map(|reason| reason.as_str().expect("reason string"))
        .collect()
}

#[test]
fn reports_resolved_record_pack_attributes() {
    let source = "#pragma pack(push, 2)\nstruct P2 { char tag; int value; };\n#pragma pack(push, 1)\nstruct P1 { char tag; int value; };\n#pragma pack(pop)\nstruct P2Again { char tag; int value; };\n#pragma pack(pop)\nstruct Natural { char tag; int value; };\n";
    let case = Case::new(source, &[]);
    let events = case.pack_events();
    assert_eq!(by_name(events.clone(), "P2")[0]["alignment_bits"], 16);
    assert_eq!(by_name(events.clone(), "P1")[0]["alignment_bits"], 8);
    assert_eq!(by_name(events.clone(), "P2Again")[0]["alignment_bits"], 16);
    assert!(by_name(events, "Natural").is_empty());
}

#[test]
fn reports_toolchain_string_header_provenance() {
    let source = "#include <string.h>\n__SIZE_TYPE__ f(const char *s) { return strlen(s); }\n";
    let case = Case::new(source, &[]);
    let event = event(&case, "strlen");
    assert_eq!(provenance(&event), "trusted_header");
    assert!(headers(&event).contains(&"string.h"));
    let evidence = event
        .get("header_evidence")
        .and_then(Value::as_array)
        .expect("header evidence array");
    assert!(evidence.iter().any(|header| {
        header.get("written").and_then(Value::as_str) == Some("string.h")
            && header
                .get("resolved")
                .and_then(Value::as_str)
                .is_some_and(|path| path.ends_with("/string.h"))
            && header.get("angled").and_then(Value::as_bool) == Some(true)
            && header.get("system").and_then(Value::as_bool) == Some(true)
            && header
                .get("identity")
                .and_then(Value::as_str)
                .is_some_and(|identity| identity.contains(':'))
    }));
    assert!(
        event
            .get("declarations")
            .and_then(Value::as_array)
            .is_some_and(|declarations| !declarations.is_empty())
    );
    assert_eq!(
        event.get("offset").and_then(Value::as_u64),
        Some(source.find("strlen(s)").expect("call offset") as u64)
    );
    assert!(
        event
            .get("file")
            .and_then(Value::as_str)
            .is_some_and(|file| file.ends_with("/input.c"))
    );
    assert!(
        event
            .get("canonical_type")
            .and_then(Value::as_str)
            .is_some_and(|ty| ty.contains("const char *"))
    );
}

#[test]
fn reports_toolchain_macro_header_provenance() {
    let source = "#include <float.h>\nlong double f(void) { return LDBL_TRUE_MIN; }\n";
    let case = Case::new(source, &[]);
    let events = by_name(case.macro_events(), "LDBL_TRUE_MIN");
    assert_eq!(events.len(), 1, "events: {events:#?}");
    let event = &events[0];
    assert!(headers(event).contains(&"float.h"));
    assert_eq!(
        event.get("definition_system").and_then(Value::as_bool),
        Some(true)
    );
    assert!(
        event
            .get("definition_file")
            .and_then(Value::as_str)
            .is_some_and(|file| file.ends_with("/__float_float.h"))
    );
}

#[test]
fn rejects_project_definition_with_a_libc_name() {
    let case = Case::new(
        "#include <stdio.h>\n__SIZE_TYPE__ strlen(const char *s) { return 5; }\nint main(void) { return (int)strlen(\"hello world\"); }\n",
        &[],
    );
    let event = event(&case, "strlen");
    assert_eq!(provenance(&event), "unknown");
    assert!(reasons(&event).contains(&"untrusted_definition"));
}

#[test]
fn rejects_project_header_declaration() {
    let case = Case::new(
        "#include \"strings.h\"\nint f(const char *s) { return (int)strlen(s); }\n",
        &[("strings.h", "__SIZE_TYPE__ strlen(const char *s);\n")],
    );
    let event = event(&case, "strlen");
    assert_eq!(provenance(&event), "unknown");
    assert!(reasons(&event).contains(&"no_trusted_header"));
}

#[test]
fn accepts_project_wrapper_around_toolchain_header() {
    let case = Case::new(
        "#include \"wrapper.h\"\n__SIZE_TYPE__ f(const char *s) { return strlen(s); }\n",
        &[("wrapper.h", "#include <string.h>\n")],
    );
    let event = event(&case, "strlen");
    assert_eq!(provenance(&event), "trusted_header");
    assert!(headers(&event).contains(&"string.h"));
}

#[test]
fn reports_resolved_include_edges() {
    let case = Case::new(
        "#include \"wrapper.h\"\n__SIZE_TYPE__ f(const char *s) { return strlen(s); }\n",
        &[("wrapper.h", "#include <string.h>\n")],
    );
    let events = case.include_events();
    let wrapper = events
        .iter()
        .find(|event| event.get("written").and_then(Value::as_str) == Some("wrapper.h"))
        .expect("wrapper include");
    assert_eq!(wrapper.get("angled").and_then(Value::as_bool), Some(false));
    assert_eq!(wrapper.get("system").and_then(Value::as_bool), Some(false));
    assert!(
        wrapper
            .get("resolved")
            .and_then(Value::as_str)
            .is_some_and(|path| path.ends_with("/wrapper.h"))
    );
    assert!(
        wrapper
            .get("identity")
            .and_then(Value::as_str)
            .is_some_and(|identity| identity.contains(':'))
    );
    let string = events
        .iter()
        .find(|event| event.get("written").and_then(Value::as_str) == Some("string.h"))
        .expect("string include");
    assert_eq!(string.get("angled").and_then(Value::as_bool), Some(true));
    assert_eq!(string.get("system").and_then(Value::as_bool), Some(true));
    assert!(
        string
            .get("includer")
            .and_then(Value::as_str)
            .is_some_and(|path| path.ends_with("/wrapper.h"))
    );
}

#[test]
fn rejects_project_definition_after_trusted_declaration() {
    let case = Case::new(
        "#include <string.h>\n__SIZE_TYPE__ strlen(const char *s) { return 5; }\nint f(void) { return (int)strlen(\"hello world\"); }\n",
        &[],
    );
    let event = event(&case, "strlen");
    assert_eq!(provenance(&event), "unknown");
    assert!(headers(&event).contains(&"string.h"));
    assert!(reasons(&event).contains(&"untrusted_definition"));
}

#[test]
fn rejects_shadowed_angle_header_from_project_include_path() {
    let case = Case::new(
        "#include <string.h>\nint f(const char *s) { return (int)strlen(s); }\n",
        &[("string.h", "__SIZE_TYPE__ strlen(const char *s);\n")],
    );
    let event = event(&case, "strlen");
    assert_eq!(provenance(&event), "unknown");
    assert!(reasons(&event).contains(&"no_trusted_header"));
}

#[test]
fn accepts_compatible_project_redeclaration_without_a_body() {
    let case = Case::new(
        "#include <string.h>\nextern __SIZE_TYPE__ strlen(const char *s);\n__SIZE_TYPE__ f(const char *s) { return strlen(s); }\n",
        &[],
    );
    let event = event(&case, "strlen");
    assert_eq!(provenance(&event), "trusted_header");
    assert!(reasons(&event).is_empty());
}

#[test]
fn reports_indirect_calls_as_unknown() {
    let case = Case::new(
        "#include <string.h>\n__SIZE_TYPE__ f(const char *s) { __SIZE_TYPE__ (*p)(const char *) = strlen; return p(s); }\n",
        &[],
    );
    let events = case.events();
    let indirect = events
        .iter()
        .find(|event| event.get("direct").and_then(Value::as_bool) == Some(false))
        .expect("indirect call event");
    assert_eq!(provenance(indirect), "unknown");
    assert!(reasons(indirect).contains(&"indirect_call"));
}

#[test]
fn carries_trusted_root_through_an_internal_header() {
    let case = Case::new(
        "#include <root.h>\n__SIZE_TYPE__ f(const char *s) { return wrapped(s); }\n",
        &[
            ("root.h", "#include <internal.h>\n"),
            ("internal.h", "__SIZE_TYPE__ wrapped(const char *s);\n"),
        ],
    );
    let include_dir = case.dir.to_str().expect("UTF-8 case path");
    let events = case.events_with_args(&["-isystem", include_dir]);
    let event = by_name(events, "wrapped")
        .into_iter()
        .next()
        .expect("wrapped event");
    assert_eq!(provenance(&event), "trusted_header");
    assert!(headers(&event).contains(&"root.h"));
    assert!(headers(&event).contains(&"internal.h"));
}

#[test]
fn rejects_system_header_outside_configured_trusted_root() {
    let case = Case::new(
        "#include <string.h>\n__SIZE_TYPE__ f(const char *s) { return strlen(s); }\n",
        &[("string.h", "__SIZE_TYPE__ strlen(const char *s);\n")],
    );
    let include_dir = case.dir.to_str().expect("UTF-8 case path");
    let events = case.events_with_args(&[
        "-isystem",
        include_dir,
        "-Xclang",
        "-plugin-arg-macro-dump",
        "-Xclang",
        "-trusted-root=/nonexistent-slate-trusted-root",
    ]);
    let event = by_name(events, "strlen")
        .into_iter()
        .next()
        .expect("strlen event");
    assert_eq!(provenance(&event), "unknown");
    assert!(reasons(&event).contains(&"no_trusted_header"));
}

#[test]
fn accepts_system_header_under_configured_trusted_root() {
    let case = Case::new(
        "#include <string.h>\n__SIZE_TYPE__ f(const char *s) { return strlen(s); }\n",
        &[("string.h", "__SIZE_TYPE__ strlen(const char *s);\n")],
    );
    let include_dir = case.dir.to_str().expect("UTF-8 case path");
    let events = case.events_with_args(&[
        "-isystem",
        include_dir,
        "-Xclang",
        "-plugin-arg-macro-dump",
        "-Xclang",
        &format!("-trusted-root={include_dir}"),
    ]);
    let event = by_name(events, "strlen")
        .into_iter()
        .next()
        .expect("strlen event");
    assert_eq!(provenance(&event), "trusted_header");
    assert!(reasons(&event).is_empty());
}

#[test]
fn rejects_macro_generated_project_declaration() {
    let case = Case::new(
        "#include \"strings.h\"\nint f(const char *s) { return (int)strlen(s); }\n",
        &[(
            "strings.h",
            "#define DECLARE(name) __SIZE_TYPE__ name(const char *s);\nDECLARE(strlen)\n",
        )],
    );
    let event = event(&case, "strlen");
    assert_eq!(provenance(&event), "unknown");
    assert!(reasons(&event).contains(&"no_trusted_header"));
}

#[test]
fn rejects_symbol_changing_redeclaration() {
    let case = Case::new(
        "#include <string.h>\nextern __SIZE_TYPE__ strlen(const char *s) __asm__(\"project_strlen\");\n__SIZE_TYPE__ f(const char *s) { return strlen(s); }\n",
        &[],
    );
    let event = event(&case, "strlen");
    assert_eq!(provenance(&event), "unknown");
    assert!(reasons(&event).contains(&"symbol_override"));
}

#[test]
fn reports_source_and_assembler_symbol_names() {
    let case = Case::new(
        "#define public_api(value) internal_api(value)\nextern int internal_api(int) __asm__(\"internal_api$DARWIN_EXTSN\");\nint f(int value) { return public_api(value); }\n",
        &[],
    );
    let event = event(&case, "internal_api");
    assert_eq!(event["source_name"], "public_api");
    assert_eq!(event["foreign_name"], "internal_api$DARWIN_EXTSN");
    assert_eq!(event["symbol_override"], true);
    assert_eq!(event["source_macros"][0], "public_api");
}

#[test]
fn reports_freebsd_symver_resolution() {
    let case = Case::new(
        "extern int compat_api(int);\n__asm__(\".symver compat_api, public_api@FBSD_1.2\");\n#define source_api compat_api\nint f(int value) { return source_api(value); }\n",
        &[],
    );
    let event = event(&case, "compat_api");
    assert_eq!(event["source_name"], "source_api");
    assert_eq!(event["foreign_name"], "public_api");
    assert_eq!(event["symbol_version"], "FBSD_1.2");
    assert_eq!(event["symbol_override"], true);
}

#[test]
fn reports_weak_import_and_availability() {
    let case = Case::new(
        "extern int future_api(void) __attribute__((weak_import, availability(macos, introduced=12.0)));\nint f(void) { return future_api(); }\n",
        &[],
    );
    let events = case.events_with_args(&[
        "--target=arm64-apple-macos11.0",
        "-I",
        case.dir.to_str().expect("UTF-8 case path"),
    ]);
    let event = by_name(events, "future_api")
        .into_iter()
        .next()
        .expect("future_api event");
    assert_eq!(event["weak_import"], true);
    assert_eq!(event["availability"][0]["platform"], "macos");
    assert_eq!(event["availability"][0]["introduced"], "12.0");
}

#[test]
fn output_is_deterministic() {
    let case = Case::new(
        "#include <string.h>\nint f(const char *a, const char *b) { return strcmp(a, b) + (int)strlen(a); }\n",
        &[],
    );
    let first = case.events();
    let second = case.events();
    assert_eq!(first.len(), 2);
    let normalize = |events: Vec<Value>| {
        events
            .into_iter()
            .map(|event| {
                let object = event.as_object().expect("event object");
                object
                    .iter()
                    .map(|(key, value)| (key.clone(), value.clone()))
                    .collect::<BTreeMap<_, _>>()
            })
            .collect::<Vec<_>>()
    };
    assert_eq!(normalize(first), normalize(second));
}
