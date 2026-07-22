#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Basic/SourceManager.h"
#include "llvm/Support/JSON.h"
#include "llvm/Support/raw_ostream.h"

using namespace clang;

namespace {

class MacroDumpCallbacks : public PPCallbacks {
  SourceManager &SM;

public:
  explicit MacroDumpCallbacks(SourceManager &SM) : SM(SM) {}

  void MacroExpands(const Token &MacroNameTok, const MacroDefinition &MD,
                     SourceRange Range, const MacroArgs *Args) override {
    SourceLocation Loc = Range.getBegin();
    if (!SM.isWrittenInMainFile(Loc))
      return;
    llvm::json::Object Event{
        {"name", MacroNameTok.getIdentifierInfo()->getName().str()},
        {"file", SM.getFilename(Loc).str()},
        {"line", SM.getSpellingLineNumber(Loc)},
        {"col", SM.getSpellingColumnNumber(Loc)},
    };
    llvm::errs() << "MACRO_EXPANSION " << llvm::json::Value(std::move(Event))
                 << "\n";
  }
};

class MacroDumpAction : public PluginASTAction {
protected:
  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                  StringRef) override {
    CI.getPreprocessor().addPPCallbacks(
        std::make_unique<MacroDumpCallbacks>(CI.getSourceManager()));
    return std::make_unique<ASTConsumer>();
  }

  bool ParseArgs(const CompilerInstance &,
                 const std::vector<std::string> &) override {
    return true;
  }

  ActionType getActionType() override { return AddBeforeMainAction; }
};

} // namespace

static FrontendPluginRegistry::Add<MacroDumpAction>
    X("macro-dump",
      "emit one JSON object per macro expansion in the main file, with the "
      "macro name and its exact (file, line, col) invocation site");
