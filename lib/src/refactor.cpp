#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/AST/ASTContext.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::tooling;
using namespace llvm;
using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::driver;

#include <s2tr/refactor.h>

#include <algorithm>
#include <memory>


class RefactoringCustomer : public clang::ASTConsumer {
public:
  explicit RefactoringCustomer(ASTContext *Context, MatchFinder* matcher_) : matcher(matcher_) {}

  virtual void HandleTranslationUnit(clang::ASTContext &Context) {
    matcher->matchAST(Context);
  }

private:
  MatchFinder* matcher;
};


class RefactoringAction : public clang::ASTFrontendAction {

public:

  RefactoringAction(MatchFinder* matcher_, Rewriter* rewriter_) : matcher(matcher_), rewriter(rewriter_) {}

  virtual clang::ASTConsumer * CreateASTConsumer(clang::CompilerInstance &Compiler, llvm::StringRef InFile) {
    rewriter->setSourceMgr(Compiler.getSourceManager(), Compiler.getLangOpts());
    return new RefactoringCustomer(&Compiler.getASTContext(), matcher);
  }


private:
  MatchFinder* matcher;
  Rewriter* rewriter;
};


class IfStmtHandler : public MatchFinder::MatchCallback {
public:
  IfStmtHandler(Rewriter *rewriter_) : rewriter(rewriter_) {}

  virtual void run(const MatchFinder::MatchResult &Result) {
    // The matched 'if' statement was bound to 'ifStmt'.
    if (const IfStmt *IfS = Result.Nodes.getNodeAs<clang::IfStmt>("ifStmt")) {
      const Stmt *Then = IfS->getThen();
      llvm::outs() << "FOUND IF";
      // Replacement Rep(*(Result.SourceManager), Then->getLocStart(), 0,
      //                 "// the 'if' part\n");
      // Replace->insert(Rep);
            rewriter->InsertText(Then->getLocStart(), "// the 'if' part\n", true,
                             true);

      if (const Stmt *Else = IfS->getElse()) {
        llvm::outs() << "FOUND ELSE";
        // Replacement Rep(*(Result.SourceManager), Else->getLocStart(), 0,
        //                 "// the 'else' part\n");
        // Replace->insert(Rep);
        rewriter->InsertText(Else->getLocStart(), "// the 'else' part\n",
                               true, true);
      }
    }
  }


private:
  Rewriter *rewriter;
};


namespace s2tr {

  std::string refactor(const std::string& code, const std::vector<std::string>& args) {

    Rewriter rewriter;
    IfStmtHandler HandlerForIf(&rewriter);

    MatchFinder Finder;
    Finder.addMatcher(ifStmt().bind("ifStmt"), &HandlerForIf);

    auto action = new RefactoringAction(&Finder, &rewriter);
    runToolOnCodeWithArgs(action, code, args);

    rewriter.getEditBuffer(rewriter.getSourceMgr().getMainFileID()).write(errs());

    return code;
  }
}