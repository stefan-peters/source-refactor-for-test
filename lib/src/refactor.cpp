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

using namespace clang::tooling;
using namespace llvm;
using namespace clang;
using namespace clang::ast_matchers;

#include <s2tr/refactor.h>

#include <algorithm>
#include <memory>

std::string pdump(const SourceLocation& sl, SourceManager& mgr) {
  return mgr.getCharacterData(sl);
  //llvm::outs() << std::string(" ", mgr.getPresumedColumnNumber(sl) -1) << "^\n";
}

bool embrace(const Stmt* stmt, const MatchFinder::MatchResult &Result, Replacements* replacements) {
      
      if(stmt and not isa<CompoundStmt>(stmt)) {
        Replacement add_first(*(Result.SourceManager), stmt->getLocStart(), 0 , "{");
        replacements->insert(add_first);

        llvm::outs() << "THEN START: " << pdump(stmt->getLocStart(), *Result.SourceManager) << "\n";

        SourceLocation end = stmt->getLocEnd();
        llvm::outs() << "THEN END: " << pdump(end, *Result.SourceManager) << "\n";
        const int offset = Lexer::MeasureTokenLength(end,
                                           *(Result.SourceManager),
                                           Result.Context->getLangOpts()) + 1;
        Replacement add_last(*(Result.SourceManager), end.getLocWithOffset(offset), 0 , "}");
        replacements->insert(add_last);
        return true;
      }
      else {
        return false;
      }
}


void append_else(const Stmt* stmt, const MatchFinder::MatchResult &Result, Replacements* replacements) {
        SourceLocation end = stmt->getLocEnd();
        //llvm::outs() << end.printToString(*Result.SourceManager) << "\n";
        //pdump(end, *Result.SourceManager);

        int offset = Lexer::MeasureTokenLength(end,
                                           *(Result.SourceManager),
                                           Result.Context->getLangOpts());
        Replacement add_last(*(Result.SourceManager), end.getLocWithOffset(offset), 0 , "}else {}");
        replacements->insert(add_last);
}


class IfStmtHandler : public MatchFinder::MatchCallback {

public:

  IfStmtHandler(Replacements* re) : Replace(re) {}

  virtual void run(const MatchFinder::MatchResult &Result) {
    
    if (const IfStmt *IfS = Result.Nodes.getNodeAs<clang::IfStmt>("ifStmt")) {

      embrace(IfS->getThen(), Result, Replace);

      if (const Stmt *Else = IfS->getElse()) {
        embrace(Else, Result, Replace);
      }
      else{
        append_else(IfS, Result, Replace);
      }
    }
  }


private:
  Replacements* Replace;
};


namespace s2tr {

  std::string refactor(const std::string& code, const std::vector<std::string>& args) {

    Replacements re;
    IfStmtHandler HandlerForIf(&re);

    MatchFinder Finder;
    Finder.addMatcher(ifStmt().bind("ifStmt"), &HandlerForIf);
    auto Factory(newFrontendActionFactory(&Finder));

    runToolOnCodeWithArgs(Factory->create(), code, args);
    return applyAllReplacements(code, re);
  }
}