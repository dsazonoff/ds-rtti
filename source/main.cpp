// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "MyAction.h"

#include <clang/Frontend/FrontendActions.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <llvm/Support/CommandLine.h>

#include <exception>
#include <iostream>
#include <string>

using namespace clang::tooling;
using namespace llvm;
using namespace std::string_view_literals;


int main(int argc, const char ** argv)
{
    try
    {
        auto category_options = cl::OptionCategory{"ds-rtti options"sv, "todo description"sv};
        auto expected_parser = CommonOptionsParser::create(argc, argv, category_options);
        if (!expected_parser)
        {
            errs() << expected_parser.takeError();
            return 1;
        }
        auto & options_parser = expected_parser.get();
        auto tool = ClangTool(options_parser.getCompilations(), options_parser.getSourcePathList());
        return tool.run(clang::tooling::newFrontendActionFactory<TypeFrontendAction>().get());
    }
    catch (const std::exception & ex)
    {
        std::cerr << "Exception!\n" << ex.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cerr << "Unknown exception!\n" << std::endl;
        return 1;
    }

    return 0;
}
