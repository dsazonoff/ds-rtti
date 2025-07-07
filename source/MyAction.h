#pragma once

#include <clang/AST/ASTConsumer.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendAction.h>

class TypeVisitor final
    : public clang::RecursiveASTVisitor<TypeVisitor>
{
public:
    explicit TypeVisitor(clang::ASTContext * context);
    bool VisitCXXRecordDecl(clang::CXXRecordDecl * decl);

private:
    clang::ASTContext * context{};
};


class TypeASTConsumer final
    : public clang::ASTConsumer
{
public:
    explicit TypeASTConsumer(clang::ASTContext * context);

    auto HandleTranslationUnit(clang::ASTContext & context) -> void override;

private:
    TypeVisitor visitor;
};

class TypeFrontendAction final
    : public clang::ASTFrontendAction
{
public:
    auto CreateASTConsumer(clang::CompilerInstance & CI, llvm::StringRef filename) -> std::unique_ptr<clang::ASTConsumer> override;
};