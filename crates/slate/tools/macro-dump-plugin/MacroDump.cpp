#include "clang/AST/ASTConsumer.h"
#include "clang/AST/Attr.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Lex/Preprocessor.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/Support/JSON.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/raw_ostream.h"

#include <memory>
#include <set>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

using namespace clang;

namespace {

struct HeaderEvidence {
  std::string Written;
  std::string Resolved;
  std::string Identity;

  bool operator<(const HeaderEvidence &Other) const {
    return std::tie(Written, Resolved, Identity) <
           std::tie(Other.Written, Other.Resolved, Other.Identity);
  }
};

std::string fileIdentity(FileEntryRef File) {
  const auto &ID = File.getUniqueID();
  return std::to_string(ID.getDevice()) + ":" + std::to_string(ID.getFile());
}

bool isUnderRoot(StringRef Path, StringRef Root) {
  StringRef NormalizedRoot = Root;
  while (NormalizedRoot.size() > 1 &&
        llvm::sys::path::is_separator(NormalizedRoot.back()))
    NormalizedRoot = NormalizedRoot.drop_back();
  if (NormalizedRoot.empty() || !Path.starts_with(NormalizedRoot))
    return false;
  StringRef Rest = Path.drop_front(NormalizedRoot.size());
  return Rest.empty() || llvm::sys::path::is_separator(Rest.front());
}

class ProvenanceState {
  std::vector<std::string> TrustedRoots;
  llvm::DenseMap<const FileEntry *, std::set<HeaderEvidence>> TrustedHeaders;
  llvm::DenseMap<unsigned, std::vector<std::string>> Macros;

public:
  explicit ProvenanceState(std::vector<std::string> TrustedRoots)
      : TrustedRoots(std::move(TrustedRoots)) {}

  bool isTrustedPath(StringRef ResolvedPath) const {
    if (TrustedRoots.empty())
      return true;
    return llvm::any_of(TrustedRoots, [&](const std::string &Root) {
      return isUnderRoot(ResolvedPath, Root);
    });
  }

  void recordTrustedHeader(FileEntryRef File, StringRef Written) {
    TrustedHeaders[&File.getFileEntry()].insert(
        {Written.str(), File.getName().str(), fileIdentity(File)});
  }

  const std::set<HeaderEvidence> *headers(const FileEntry &File) const {
    auto It = TrustedHeaders.find(&File);
    return It == TrustedHeaders.end() ? nullptr : &It->second;
  }

  void recordMacro(unsigned Offset, StringRef Name) {
    auto &Names = Macros[Offset];
    if (!llvm::is_contained(Names, Name))
      Names.push_back(Name.str());
  }

  const std::vector<std::string> *macros(unsigned Offset) const {
    auto It = Macros.find(Offset);
    return It == Macros.end() ? nullptr : &It->second;
  }
};

llvm::json::Array headerNames(const std::set<HeaderEvidence> &Headers) {
  llvm::json::Array Out;
  for (const HeaderEvidence &Header : Headers)
    Out.push_back(Header.Written);
  return Out;
}

class MacroDumpCallbacks : public PPCallbacks {
  SourceManager &SM;
  std::shared_ptr<ProvenanceState> State;

  std::set<HeaderEvidence> headersFor(SourceLocation Loc) const {
    std::set<HeaderEvidence> Headers;
    Loc = SM.getSpellingLoc(Loc);
    if (Loc.isInvalid())
      return Headers;
    FileID Current = SM.getFileID(Loc);
    while (!Current.isInvalid()) {
      if (auto File = SM.getFileEntryRefForID(Current))
        if (const auto *Found = State->headers(File->getFileEntry()))
          Headers.insert(Found->begin(), Found->end());
      SourceLocation Include = SM.getIncludeLoc(Current);
      if (Include.isInvalid())
        break;
      Current = SM.getFileID(SM.getExpansionLoc(Include));
    }
    return Headers;
  }

public:
  MacroDumpCallbacks(SourceManager &SM, std::shared_ptr<ProvenanceState> State)
      : SM(SM), State(std::move(State)) {}

  void MacroExpands(const Token &MacroNameTok, const MacroDefinition &MD,
                    SourceRange Range, const MacroArgs *Args) override {
    SourceLocation Loc = Range.getBegin();
    if (!SM.isWrittenInMainFile(Loc))
      return;
    State->recordMacro(SM.getFileOffset(Loc),
                       MacroNameTok.getIdentifierInfo()->getName());
    llvm::json::Object Event{
        {"name", MacroNameTok.getIdentifierInfo()->getName().str()},
        {"file", SM.getFilename(Loc).str()},
        {"offset", static_cast<int64_t>(SM.getFileOffset(Loc))},
    };
    if (const MacroInfo *Info = MD.getMacroInfo()) {
      SourceLocation Definition = SM.getSpellingLoc(Info->getDefinitionLoc());
      if (Definition.isValid()) {
        Event["definition_file"] = SM.getFilename(Definition).str();
        Event["definition_system"] = SM.isInSystemHeader(Definition);
        Event["headers"] = headerNames(headersFor(Definition));
      }
    }
    llvm::errs() << "MACRO_EXPANSION " << llvm::json::Value(std::move(Event))
                 << "\n";
  }

  void InclusionDirective(SourceLocation HashLoc, const Token &,
                          StringRef FileName, bool IsAngled, CharSourceRange,
                          OptionalFileEntryRef File, StringRef, StringRef,
                          const Module *, bool,
                          SrcMgr::CharacteristicKind FileType) override {
    SourceLocation Loc = SM.getExpansionLoc(HashLoc);
    llvm::json::Object Event{
        {"written", FileName.str()},
        {"angled", IsAngled},
        {"system", SrcMgr::isSystem(FileType)},
        {"includer", SM.getFilename(Loc).str()},
        {"offset", static_cast<int64_t>(SM.getFileOffset(Loc))},
    };
    if (File) {
      Event["resolved"] = File->getName().str();
      Event["identity"] = fileIdentity(*File);
    }
    llvm::errs() << "INCLUDE_PROVENANCE " << llvm::json::Value(std::move(Event))
                 << "\n";
    if (File && IsAngled && SrcMgr::isSystem(FileType) &&
        State->isTrustedPath(File->getName()))
      State->recordTrustedHeader(*File, FileName);
  }
};

llvm::json::Array headerDetails(const std::set<HeaderEvidence> &Headers) {
  llvm::json::Array Out;
  for (const HeaderEvidence &Header : Headers)
    Out.push_back(llvm::json::Object{{"written", Header.Written},
                                     {"resolved", Header.Resolved},
                                     {"identity", Header.Identity},
                                     {"angled", true},
                                     {"system", true}});
  return Out;
}

llvm::json::Object sourcePoint(SourceManager &SM, SourceLocation Loc) {
  PresumedLoc Presumed = SM.getPresumedLoc(Loc);
  return llvm::json::Object{
      {"file", Presumed.getFilename()},
      {"offset", static_cast<int64_t>(SM.getFileOffset(Loc))},
      {"line", static_cast<int64_t>(Presumed.getLine())},
      {"column", static_cast<int64_t>(Presumed.getColumn())}};
}

class ProvenanceVisitor : public RecursiveASTVisitor<ProvenanceVisitor> {
  SourceManager &SM;
  ASTContext &Context;
  const ProvenanceState &State;

  static llvm::json::Object floatingValue(const llvm::APFloat &Value) {
    llvm::SmallString<32> Text;
    Value.toString(Text);
    llvm::APInt Bits = Value.bitcastToAPInt();
    llvm::SmallString<32> UnpaddedBits;
    Bits.toStringUnsigned(UnpaddedBits, 16);
    std::string BitString((Bits.getBitWidth() + 3) / 4 - UnpaddedBits.size(),
                          '0');
    BitString.append(UnpaddedBits.data(), UnpaddedBits.size());
    return llvm::json::Object{
        {"value", Text.str().str()},
        {"bit_width", static_cast<int64_t>(Bits.getBitWidth())},
        {"bits", std::move(BitString)},
    };
  }

  void collectLongDoubleValues(Stmt *Node, llvm::json::Array &Values) {
    if (!Node)
      return;
    if (auto *Expression = dyn_cast<Expr>(Node);
        Expression && !Expression->getType().isNull() &&
            Expression->getType()->isSpecificBuiltinType(
                          BuiltinType::LongDouble)) {
      Expr::EvalResult Result;
      if (Expression->EvaluateAsRValue(Result, Context) &&
          Result.Val.isFloat()) {
        Values.push_back(floatingValue(Result.Val.getFloat()));
        return;
      }
    }
    for (Stmt *Child : Node->children())
      collectLongDoubleValues(Child, Values);
  }

  std::set<HeaderEvidence> headersFor(SourceLocation Loc) const {
    std::set<HeaderEvidence> Headers;
    Loc = SM.getExpansionLoc(Loc);
    if (Loc.isInvalid())
      return Headers;
    FileID Current = SM.getFileID(Loc);
    while (!Current.isInvalid()) {
      if (auto File = SM.getFileEntryRefForID(Current))
        if (const auto *Found = State.headers(File->getFileEntry()))
          Headers.insert(Found->begin(), Found->end());
      SourceLocation Include = SM.getIncludeLoc(Current);
      if (Include.isInvalid())
        break;
      Current = SM.getFileID(SM.getExpansionLoc(Include));
    }
    return Headers;
  }

  static bool changesSymbol(const FunctionDecl &Decl) {
    return Decl.hasAttr<AliasAttr>() || Decl.hasAttr<AsmLabelAttr>() ||
           Decl.hasAttr<IFuncAttr>() || Decl.hasAttr<WeakAttr>() ||
           Decl.hasAttr<WeakRefAttr>();
  }

  static std::string foreignName(const FunctionDecl &Decl) {
    if (const auto *Attr = Decl.getAttr<AsmLabelAttr>())
      return Attr->getLabel().str();
    if (const auto *Attr = Decl.getAttr<AliasAttr>())
      return Attr->getAliasee().str();
    if (const auto *Attr = Decl.getAttr<WeakRefAttr>();
        Attr && !Attr->getAliasee().empty())
      return Attr->getAliasee().str();
    return Decl.getNameAsString();
  }

  static llvm::json::Array availability(const FunctionDecl &Decl) {
    llvm::json::Array Values;
    for (const auto *Attr : Decl.specific_attrs<AvailabilityAttr>()) {
      llvm::json::Object Value{
          {"platform", Attr->getPlatform()->getName().str()},
          {"unavailable", Attr->getUnavailable()},
          {"strict", Attr->getStrict()},
      };
      if (!Attr->getIntroduced().empty())
        Value["introduced"] = Attr->getIntroduced().getAsString();
      if (!Attr->getDeprecated().empty())
        Value["deprecated"] = Attr->getDeprecated().getAsString();
      if (!Attr->getObsoleted().empty())
        Value["obsoleted"] = Attr->getObsoleted().getAsString();
      Values.push_back(std::move(Value));
    }
    return Values;
  }

  llvm::json::Object
  declarationEvidence(const FunctionDecl &Decl,
                      const std::set<HeaderEvidence> &Headers) {
    SourceLocation Loc = SM.getExpansionLoc(Decl.getLocation());
    llvm::json::Object Out{{"definition", Decl.doesThisDeclarationHaveABody()},
                           {"symbol_override", changesSymbol(Decl)},
                           {"foreign_name", foreignName(Decl)},
                           {"weak_import", Decl.hasAttr<WeakImportAttr>()},
                           {"availability", availability(Decl)},
                           {"canonical", Decl.isCanonicalDecl()},
                           {"type", Decl.getType().getAsString()},
                           {"headers", headerNames(Headers)}};
    if (Loc.isValid()) {
      Out["file"] = SM.getFilename(Loc).str();
      Out["offset"] = static_cast<int64_t>(SM.getFileOffset(Loc));
      Out["system"] = SM.isInSystemHeader(Loc);
    }
    return Out;
  }

public:
  ProvenanceVisitor(SourceManager &SM, ASTContext &Context,
                    const ProvenanceState &State)
      : SM(SM), Context(Context), State(State) {}

  bool VisitFloatingLiteral(FloatingLiteral *Literal) {
    SourceLocation Expansion = SM.getExpansionLoc(Literal->getExprLoc());
    SourceLocation Spelling = SM.getSpellingLoc(Literal->getExprLoc());
    if (Expansion.isInvalid() || Spelling.isInvalid() ||
        !SM.isWrittenInMainFile(Expansion))
      return true;

    llvm::json::Object Event = floatingValue(Literal->getValue());
    Event["spelling"] = sourcePoint(SM, Spelling);
    Event["expansion"] = sourcePoint(SM, Expansion);
    Event["type"] = Literal->getType().getAsString();
    llvm::errs() << "FLOATING_LITERAL "
                 << llvm::json::Value(std::move(Event)) << "\n";
    return true;
  }

  bool VisitExpr(Expr *Expression) {
    if (Expression->getType().isNull() || isa<FloatingLiteral>(Expression) ||
        !Expression->getType()->isSpecificBuiltinType(BuiltinType::LongDouble))
      return true;
    Expr::EvalResult Result;
    if (!Expression->EvaluateAsRValue(Result, Context) || !Result.Val.isFloat())
      return true;
    for (SourceLocation Location : {Expression->getExprLoc(),
                                    Expression->getBeginLoc(),
                                    Expression->getEndLoc()}) {
      SourceLocation Expansion = SM.getExpansionLoc(Location);
      SourceLocation Spelling = SM.getSpellingLoc(Location);
      if (Expansion.isInvalid() || Spelling.isInvalid() ||
          !SM.isWrittenInMainFile(Expansion))
        continue;
      llvm::json::Object Event = floatingValue(Result.Val.getFloat());
      Event["spelling"] = sourcePoint(SM, Spelling);
      Event["expansion"] = sourcePoint(SM, Expansion);
      Event["type"] = Expression->getType().getAsString();
      llvm::errs() << "FLOATING_LITERAL "
                   << llvm::json::Value(std::move(Event)) << "\n";
    }
    return true;
  }

  bool VisitVarDecl(VarDecl *Decl) {
    if (!Decl->hasGlobalStorage() || !Decl->hasInit())
      return true;
    SourceLocation Loc = SM.getExpansionLoc(Decl->getLocation());
    if (Loc.isInvalid() || !SM.isWrittenInMainFile(Loc))
      return true;
    llvm::json::Array Values;
    collectLongDoubleValues(Decl->getInit(), Values);
    if (Values.empty())
      return true;
    llvm::json::Object Event{{"name", Decl->getNameAsString()},
                             {"values", std::move(Values)}};
    llvm::errs() << "GLOBAL_LONG_DOUBLE "
                 << llvm::json::Value(std::move(Event)) << "\n";
    return true;
  }

  bool VisitGCCAsmStmt(GCCAsmStmt *Stmt) {
    if (!Stmt->isAsmGoto())
      return true;
    SourceLocation Loc = SM.getExpansionLoc(Stmt->getAsmLoc());
    if (Loc.isInvalid() || !SM.isWrittenInMainFile(Loc))
      return true;
    llvm::json::Array Labels;
    for (unsigned I = 0; I < Stmt->getNumLabels(); ++I) {
      llvm::json::Object Label{{"name", Stmt->getLabelName(I)}};
      SourceLocation Target =
          SM.getExpansionLoc(Stmt->getLabelExpr(I)->getLabel()->getLocation());
      if (Target.isValid())
        Label["target"] = sourcePoint(SM, Target);
      Labels.push_back(std::move(Label));
    }
    llvm::json::Object Event{
        {"file", SM.getFilename(Loc).str()},
        {"offset", static_cast<int64_t>(SM.getFileOffset(Loc))},
        {"labels", std::move(Labels)},
    };
    llvm::errs() << "ASM_GOTO " << llvm::json::Value(std::move(Event))
                 << "\n";
    return true;
  }

  bool VisitCallExpr(CallExpr *Call) {
    SourceLocation Loc = SM.getExpansionLoc(Call->getExprLoc());
    if (Loc.isInvalid() || !SM.isWrittenInMainFile(Loc))
      return true;

    const FunctionDecl *Callee = Call->getDirectCallee();
    std::set<HeaderEvidence> Headers;
    std::set<std::string> Reasons;
    llvm::json::Array Declarations;
    std::string Name;
    std::string ForeignName;
    bool WeakImport = false;
    llvm::json::Array Availability;

    if (!Callee) {
      Reasons.insert("indirect_call");
    } else {
      Name = Callee->getNameAsString();
      ForeignName = Name;
      bool HasTrustedDeclaration = false;
      bool HasUntrustedDefinition = false;
      bool HasSymbolOverride = false;
      for (const FunctionDecl *Redecl : Callee->redecls()) {
        std::set<HeaderEvidence> DeclHeaders =
            headersFor(Redecl->getLocation());
        HasTrustedDeclaration |= !DeclHeaders.empty();
        HasUntrustedDefinition |=
            Redecl->doesThisDeclarationHaveABody() && DeclHeaders.empty();
        HasSymbolOverride |= changesSymbol(*Redecl);
        if (changesSymbol(*Redecl))
          ForeignName = foreignName(*Redecl);
        WeakImport |= Redecl->hasAttr<WeakImportAttr>();
        llvm::json::Array DeclAvailability = availability(*Redecl);
        for (llvm::json::Value &Value : DeclAvailability)
          Availability.push_back(std::move(Value));
        Headers.insert(DeclHeaders.begin(), DeclHeaders.end());
        Declarations.push_back(declarationEvidence(*Redecl, DeclHeaders));
      }
      if (!HasTrustedDeclaration)
        Reasons.insert("no_trusted_header");
      if (HasUntrustedDefinition)
        Reasons.insert("untrusted_definition");
      if (HasSymbolOverride)
        Reasons.insert("symbol_override");
    }

    llvm::json::Array ReasonValues;
    for (const std::string &Reason : Reasons)
      ReasonValues.push_back(Reason);
    bool Trusted = Callee && Reasons.empty();
    unsigned Offset = SM.getFileOffset(Loc);
    llvm::json::Array SourceMacros;
    std::string SourceName = Name;
    if (const auto *Macros = State.macros(Offset)) {
      for (const std::string &Macro : *Macros)
        SourceMacros.push_back(Macro);
      if (!Macros->empty())
        SourceName = Macros->front();
    }
    llvm::json::Object Event{
        {"name", Name},
        {"source_name", SourceName},
        {"foreign_name", ForeignName},
        {"source_macros", std::move(SourceMacros)},
        {"symbol_override", ForeignName != Name},
        {"weak_import", WeakImport},
        {"availability", std::move(Availability)},
        {"file", SM.getFilename(Loc).str()},
        {"offset", static_cast<int64_t>(Offset)},
        {"direct", Callee != nullptr},
        {"provenance", Trusted ? "trusted_header" : "unknown"},
        {"headers", headerNames(Headers)},
        {"header_evidence", headerDetails(Headers)},
        {"declarations", std::move(Declarations)},
        {"reasons", std::move(ReasonValues)},
    };
    if (Callee)
      Event["canonical_type"] =
          Callee->getCanonicalDecl()->getType().getAsString();
    llvm::errs() << "FUNCTION_PROVENANCE "
                 << llvm::json::Value(std::move(Event)) << "\n";
    return true;
  }

  bool VisitRecordDecl(RecordDecl *Decl) {
    if (!Decl->isCompleteDefinition())
      return true;
    const auto *Attr = Decl->getAttr<MaxFieldAlignmentAttr>();
    if (!Attr)
      return true;
    SourceLocation Loc = SM.getExpansionLoc(Decl->getLocation());
    if (Loc.isInvalid())
      return true;
    llvm::json::Object Event{
        {"name", Decl->getNameAsString()},
        {"file", SM.getFilename(Loc).str()},
        {"offset", static_cast<int64_t>(SM.getFileOffset(Loc))},
        {"alignment_bits", static_cast<int64_t>(Attr->getAlignment())},
    };
    llvm::errs() << "RECORD_PACKING "
                 << llvm::json::Value(std::move(Event)) << "\n";
    return true;
  }
};

class ProvenanceConsumer : public ASTConsumer {
  SourceManager &SM;
  std::shared_ptr<ProvenanceState> State;

public:
  ProvenanceConsumer(SourceManager &SM, std::shared_ptr<ProvenanceState> State)
      : SM(SM), State(std::move(State)) {}

  void HandleTranslationUnit(ASTContext &Context) override {
    ProvenanceVisitor Visitor(SM, Context, *State);
    Visitor.TraverseDecl(Context.getTranslationUnitDecl());
  }
};

class MacroDumpAction : public PluginASTAction {
  std::vector<std::string> TrustedRoots;

protected:
  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                 StringRef) override {
    auto State = std::make_shared<ProvenanceState>(TrustedRoots);
    CI.getPreprocessor().addPPCallbacks(
        std::make_unique<MacroDumpCallbacks>(CI.getSourceManager(), State));
    return std::make_unique<ProvenanceConsumer>(CI.getSourceManager(), State);
  }

  bool ParseArgs(const CompilerInstance &,
                 const std::vector<std::string> &Args) override {
    for (StringRef Arg : Args) {
      StringRef Root = Arg;
      if (Root.consume_front("-trusted-root="))
        TrustedRoots.push_back(Root.str());
    }
    return true;
  }

  ActionType getActionType() override { return AddBeforeMainAction; }
};

} // namespace

static FrontendPluginRegistry::Add<MacroDumpAction>
    X("macro-dump",
      "emit macro, include, and function provenance as line-oriented JSON");
