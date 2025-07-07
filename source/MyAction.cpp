// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "MyAction.h"

#include <iostream>

TypeVisitor::TypeVisitor(clang::ASTContext * context)
    : context(context)
{
}

bool TypeVisitor::VisitCXXRecordDecl(clang::CXXRecordDecl * decl)
{
    if (!decl->isCompleteDefinition() || decl->isImplicit())
        return true;
    if (decl->isClass() || decl->isStruct())
    {
        llvm::outs() << "Type: " << decl->getQualifiedNameAsString() << "\n";
        // Fields
        for (const auto * field : decl->fields())
        {
            llvm::outs() << "  Field: " << field->getType().getAsString()
                         << " " << field->getNameAsString() << "\n";
        }
        // Methods
        for (const auto * method : decl->methods())
        {
            llvm::outs() << "  Method: " << method->getReturnType().getAsString()
                         << " " << method->getNameAsString() << "(";
            bool first = true;
            for (const auto * param : method->parameters())
            {
                if (!first)
                    llvm::outs() << ", ";
                llvm::outs() << param->getType().getAsString();
                first = false;
            }
            llvm::outs() << ")\n";
        }
        llvm::outs() << "\n";
    }
    return true;
}

TypeASTConsumer::TypeASTConsumer(clang::ASTContext * context)
    : visitor(context)
{
}

void TypeASTConsumer::HandleTranslationUnit(clang::ASTContext & context)
{
    visitor.TraverseDecl(context.getTranslationUnitDecl());
}

std::unique_ptr<clang::ASTConsumer> TypeFrontendAction::CreateASTConsumer(clang::CompilerInstance & CI, llvm::StringRef filename)
{
    return std::make_unique<TypeASTConsumer>(&CI.getASTContext());
}
