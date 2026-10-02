#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Attributes.h"
#include "llvm/Support/raw_ostream.h"
#include <string>
#include <cstdio>
#include <optional>
#include <vector>
#include <utility>

using namespace llvm;

static void printEscaped(StringRef s) {
  outs() << "\"";
  for (char c : s) {
    if (c == '"' || c == '\\') outs() << '\\';
    outs() << c;
  }
  outs() << "\"";
}

using IITDescriptor = Intrinsic::IITDescriptor;

static std::pair<unsigned, std::optional<unsigned>>
consumeOne(ArrayRef<IITDescriptor> descs, unsigned i) {
  if (i >= descs.size()) return {0, std::nullopt};
  const IITDescriptor &d = descs[i];
  switch (d.Kind) {
  case IITDescriptor::Overloaded:
    return {1, d.getOverloadIndex()};
  case IITDescriptor::Void:
  case IITDescriptor::MMX:
  case IITDescriptor::Token:
  case IITDescriptor::Metadata:
  case IITDescriptor::Half:
  case IITDescriptor::BFloat:
  case IITDescriptor::Float:
  case IITDescriptor::Double:
  case IITDescriptor::Quad:
  case IITDescriptor::Integer:
  case IITDescriptor::Pointer:
  case IITDescriptor::AMX:
  case IITDescriptor::PPCQuad:
  case IITDescriptor::AArch64Svcount:
  case IITDescriptor::WasmExternref:
  case IITDescriptor::WasmFuncref:
  case IITDescriptor::Match:
  case IITDescriptor::Extend:
  case IITDescriptor::Trunc:
  case IITDescriptor::VecElement:
  case IITDescriptor::VecOfBitcastsToInt:
  case IITDescriptor::VecOfAnyPtrsToElt:
    return {1, std::nullopt};
  case IITDescriptor::Vector:
  case IITDescriptor::SameVecWidth: {
    auto [sub, _] = consumeOne(descs, i + 1);
    if (sub == 0) return {0, std::nullopt};
    return {1 + sub, std::nullopt};
  }
  case IITDescriptor::Struct: {
    unsigned total = 1;
    for (unsigned n = 0; n < d.StructNumElements; ++n) {
      auto [sub, _] = consumeOne(descs, i + total);
      if (sub == 0) return {0, std::nullopt};
      total += sub;
    }
    return {total, std::nullopt};
  }
  default:
    return {0, std::nullopt};
  }
}

static std::optional<std::vector<unsigned>>
overloadedPositions(ArrayRef<IITDescriptor> descs, unsigned numArgs) {
  std::vector<unsigned> positions;
  unsigned i = 0;
  for (unsigned pos = 0; pos <= numArgs; ++pos) {
    auto [consumed, overloadIndex] = consumeOne(descs, i);
    if (consumed == 0) return std::nullopt;
    if (overloadIndex.has_value()) positions.push_back(pos);
    i += consumed;
  }
  if (i != descs.size()) return std::nullopt;
  return positions;
}

static std::optional<std::string>
renderType(ArrayRef<IITDescriptor> descs, unsigned &i) {
  if (i >= descs.size()) return std::nullopt;
  const auto &d = descs[i++];
  switch (d.Kind) {
  case IITDescriptor::Void: return "void";
  case IITDescriptor::Half: return "half";
  case IITDescriptor::BFloat: return "bfloat";
  case IITDescriptor::Float: return "float";
  case IITDescriptor::Double: return "double";
  case IITDescriptor::Quad: return "fp128";
  case IITDescriptor::Integer: return "i" + std::to_string(d.IntegerWidth);
  case IITDescriptor::Pointer:
    if (d.PointerAddressSpace == 0) return "ptr";
    return std::nullopt;
  case IITDescriptor::Overloaded: {
    auto [vectorConstraint, elementConstraint] = d.getOverloadConstraints();
    std::string vector = vectorConstraint == IITDescriptor::VC_Vector ? "vector" :
                         vectorConstraint == IITDescriptor::VC_Scalar ? "scalar" : "any";
    std::string element = elementConstraint == IITDescriptor::EC_Integer ? "int" :
                          elementConstraint == IITDescriptor::EC_Float ? "float" :
                          elementConstraint == IITDescriptor::EC_Pointer ? "ptr" : "any";
    return "overload:" + std::to_string(d.getOverloadIndex()) + ":" + vector + ":" + element;
  }
  case IITDescriptor::Match:
    return "match:" + std::to_string(d.getOverloadIndex());
  case IITDescriptor::Vector: {
    if (d.VectorWidth.isScalable()) return std::nullopt;
    auto element = renderType(descs, i);
    if (!element) return std::nullopt;
    return "<" + std::to_string(d.VectorWidth.getFixedValue()) + " x " + *element + ">";
  }
  default: return std::nullopt;
  }
}

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "usage: %s <prefix>\n", argv[0]);
    return 1;
  }
  StringRef selector(argv[1]);
  std::string prefix = std::string("llvm.") + argv[1] + ".";
  static constexpr StringLiteral targetNamespaces[] = {
      "aarch64", "amdgcn", "arc", "arm", "avr", "bpf", "hexagon",
      "lanai", "loongarch", "mips", "msp430", "nvvm", "ppc", "ptrauth",
      "r600", "riscv", "s390", "spv", "sparc", "systemz", "ve", "wasm",
      "webgpu", "x86", "xcore", "xtensa"};
  LLVMContext ctx;

  unsigned numIntrinsics = Intrinsic::num_intrinsics;
  bool first = true;
  outs() << "[\n";
  for (unsigned raw = 1; raw < numIntrinsics; ++raw) {
    Intrinsic::ID id = static_cast<Intrinsic::ID>(raw);
    StringRef name = Intrinsic::getName(id);
    bool matches = false;
    if (selector == "__generic__") {
      if (name.starts_with("llvm.")) {
        StringRef rest = name.drop_front(strlen("llvm."));
        StringRef namespaceName = rest.split('.').first;
        matches = true;
        for (StringLiteral targetNamespace : targetNamespaces)
          if (namespaceName == targetNamespace) {
            matches = false;
            break;
          }
      }
    } else {
      matches = name.starts_with(prefix);
    }
    if (!matches) continue;

    bool overloaded = Intrinsic::isOverloaded(id);
    if (!first) outs() << ",\n";
    first = false;
    outs() << "  {\"name\": ";
    printEscaped(name);
    outs() << ", \"overloaded\": " << (overloaded ? "true" : "false");

    if (!overloaded) {
      FunctionType *FT = Intrinsic::getType(ctx, id, {});
      AttributeList attrs = Intrinsic::getAttributes(ctx, id, FT);

      std::string retStr;
      raw_string_ostream retOs(retStr);
      FT->getReturnType()->print(retOs);
      outs() << ", \"ret\": ";
      printEscaped(retStr);

      outs() << ", \"params\": [";
      for (unsigned i = 0; i < FT->getNumParams(); ++i) {
        if (i) outs() << ", ";
        std::string tyStr;
        raw_string_ostream tyOs(tyStr);
        FT->getParamType(i)->print(tyOs);
        outs() << "{\"type\": ";
        printEscaped(tyStr);
        bool immarg = attrs.hasParamAttr(i, Attribute::ImmArg);
        outs() << ", \"immarg\": " << (immarg ? "true" : "false") << "}";
      }
      outs() << "]";
    } else {
      SmallVector<IITDescriptor, 8> table;
      auto [descs, numArgs, isVarArg] =
          Intrinsic::getIntrinsicInfoTableEntries(id, table);
      unsigned cursor = 0;
      auto ret = renderType(descs, cursor);
      std::vector<std::string> params;
      bool complete = ret.has_value() && !isVarArg;
      for (unsigned n = 0; complete && n < numArgs; ++n) {
        auto param = renderType(descs, cursor);
        complete = param.has_value();
        if (param) params.push_back(*param);
      }
      complete = complete && cursor == descs.size();
      outs() << ", \"ret\": ";
      if (complete) printEscaped(*ret); else outs() << "null";
      outs() << ", \"params\": ";
      if (complete) {
        outs() << "[";
        for (unsigned n = 0; n < params.size(); ++n) {
          if (n) outs() << ", ";
          outs() << "{\"type\": ";
          printEscaped(params[n]);
          outs() << ", \"immarg\": false}";
        }
        outs() << "]";
      } else {
        outs() << "null";
      }
      auto positions = isVarArg ? std::nullopt
                                 : overloadedPositions(descs, numArgs);
      outs() << ", \"overloaded_positions\": ";
      if (positions) {
        outs() << "[";
        for (size_t i = 0; i < positions->size(); ++i) {
          if (i) outs() << ", ";
          outs() << (*positions)[i];
        }
        outs() << "]";
      } else {
        outs() << "null";
      }
    }
    outs() << "}";
  }
  outs() << "\n]\n";
  return 0;
}
