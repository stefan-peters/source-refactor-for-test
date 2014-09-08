#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Tooling/Refactoring.h"
#include "clang/Lex/Lexer.h"
#include <clang/Frontend/TextDiagnosticPrinter.h>
#include <clang/Rewrite/Core/Rewriter.h>
#include <llvm/Support/MemoryBuffer.h>

using namespace clang::tooling;
using namespace llvm;
using namespace clang;
using namespace clang::ast_matchers;

#include <s2tr/refactor.h>
#include "ReplacementHandler.h"

#include <algorithm>
#include <memory>

typedef std::vector<Replacement> ReplacementList;

std::string applyAllReplacements(StringRef Code,
                                 const ReplacementList &Replaces) {
  FileManager Files((FileSystemOptions()));
  DiagnosticsEngine Diagnostics(
      IntrusiveRefCntPtr<DiagnosticIDs>(new DiagnosticIDs),
      new DiagnosticOptions);
  Diagnostics.setClient(new TextDiagnosticPrinter(
      llvm::outs(), &Diagnostics.getDiagnosticOptions()));
  SourceManager SourceMgr(Diagnostics, Files);
  Rewriter Rewrite(SourceMgr, LangOptions());
  llvm::MemoryBuffer *Buf = llvm::MemoryBuffer::getMemBuffer(Code, "<stdin>");
  const clang::FileEntry *Entry =
      Files.getVirtualFile("<stdin>", Buf->getBufferSize(), 0);
  SourceMgr.overrideFileContents(Entry, std::move(Buf));
  FileID ID =
      SourceMgr.createFileID(Entry, SourceLocation(), clang::SrcMgr::C_User);
  for (ReplacementList::const_iterator I = Replaces.begin(), E = Replaces.end();
       I != E; ++I) {
    Replacement Replace("<stdin>", I->getOffset(), I->getLength(),
                        I->getReplacementText());
    if (!Replace.apply(Rewrite))
      return "";
  }
  std::string Result;
  llvm::raw_string_ostream OS(Result);
  Rewrite.getEditBuffer(ID).write(OS); 
  OS.flush();
  return Result;
}

std::string pdump(const SourceLocation &sl, SourceManager &mgr) {
  return std::string(mgr.getCharacterData(SourceRange(sl).getBegin())) + ":" +
         std::string(mgr.getCharacterData(SourceRange(sl).getEnd()));
  // llvm::outs() << std::string(" ", mgr.getPresumedColumnNumber(sl) -1) <<
  // "^\n";
}

std::string show(const SourceLocation &s, const SourceLocation &e,
                 SourceManager &mgr) {
  return std::string(mgr.getCharacterData(s),
                     mgr.getCharacterData(e) - mgr.getCharacterData(s));
}

void dumpLocation(const std::string pre, const Stmt *stmt, SourceManager &mgr) {

  std::string l = "null-pointer";
  if (stmt) {
    l = show(stmt->getLocStart(), stmt->getLocEnd(), mgr);
  }
  llvm::outs() << pre << " : '" << l << "'\n";
}

void dumpLocation(const std::string pre, const SourceLocation &s,
                  const SourceLocation &e, SourceManager &mgr) {

  const std::string l = show(s, e, mgr);
  llvm::outs() << pre << " : '" << l << "'\n";
}

bool embrace(const Stmt *stmt, const MatchFinder::MatchResult &Result,
             Replacements *replacements) {

  if (stmt and not isa<CompoundStmt>(stmt)) {
    Replacement add_first(*(Result.SourceManager), stmt->getLocStart(), 0, "{");
    replacements->insert(add_first);

    // llvm::outs() << "THEN START: " << pdump(stmt->getLocStart(),
    // *Result.SourceManager) << "\n";

    SourceLocation end = stmt->getLocEnd();
    // llvm::outs() << "THEN END: " << pdump(end, *Result.SourceManager) <<
    // "\n";
    const int offset =
        Lexer::MeasureTokenLength(end, *(Result.SourceManager),
                                  Result.Context->getLangOpts()) +
        1;

    for (int i = 0; i < 4; ++i) {
      dumpLocation(std::string("with offset ") + std::to_string(i),
                   stmt->getLocStart(), end.getLocWithOffset(i),
                   *(Result.SourceManager));
    }

    {
      Replacement add_last(*(Result.SourceManager), end.getLocWithOffset(1), 0,
                           "[");
      replacements->insert(add_last);
    }
    {
      Replacement add_last(*(Result.SourceManager), end.getLocWithOffset(1), 0,
                           "]");
      replacements->insert(add_last);
    }

    return true;
  } else {
    return false;
  }
}

void append_else(const IfStmt *stmt, const MatchFinder::MatchResult &Result,
                 Replacements *replacements) {
  SourceLocation end = stmt->getThen()->getLocEnd();
  // llvm::outs() << end.printToString(*Result.SourceManager) << "\n";
  // llvm::outs() << "ELSE: " << pdump(end, *Result.SourceManager) << "\n";

  int offset = 1;

  if (not isa<CompoundStmt>(stmt->getThen())) {
    offset = Lexer::MeasureTokenLength(end, *(Result.SourceManager),
                                       Result.Context->getLangOpts());
  }

  Replacement add_last(*(Result.SourceManager), end.getLocWithOffset(offset), 0,
                       " else {}");
  replacements->insert(add_last);
}

class IfStmtHandler : public MatchFinder::MatchCallback {

public:
  IfStmtHandler(Replacements *re) : Replace(re) {}

  virtual void run(const MatchFinder::MatchResult &Result) {

    if (const IfStmt *IfS = Result.Nodes.getNodeAs<clang::IfStmt>("ifStmt")) {

      dumpLocation("IF: ", IfS, *(Result.SourceManager));

      const Stmt *Else = IfS->getElse();
      dumpLocation("ELSE: ", Else, *(Result.SourceManager));

      if (not Else) {
        append_else(IfS, Result, Replace);
      } else if (not isa<CompoundStmt>(Else)) {
        embrace(Else, Result, Replace);
      }

      if (not isa<CompoundStmt>(IfS->getThen())) {
        embrace(IfS->getThen(), Result, Replace);
      }
    }
  }

private:
  Replacements *Replace;
};

namespace s2tr {

std::string refactor(const std::string &code,
                     const std::vector<std::string> &args) {

  Replacements re;
  IfStmtHandler HandlerForIf(&re);

  MatchFinder Finder;
  Finder.addMatcher(ifStmt().bind("ifStmt"), &HandlerForIf);
  auto Factory(newFrontendActionFactory(&Finder));

  runToolOnCodeWithArgs(Factory->create(), code, args);
  // for(auto r : re) {
  //   llvm::outs() << r.toString() << "\n";
  // }
  return applyAllReplacements(code, re);
}
}