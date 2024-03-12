//
//  main.cpp
//  snippets/boiler/libclang
//
// Copyright (c) 2024 wry
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <clang-c/Index.h>

#include <cstring>

#include <iostream>
#include <map>

struct StructInfo
{
    std::string structName;
};

struct UserContext
{
    std::string macro;
    std::map<std::string,StructInfo> userMap;
};

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cout << "$0 <macro> <path>" << std::endl;
        return 1;
    }

    CXIndex index = clang_createIndex(0, 0);
    CXTranslationUnit unit = clang_parseTranslationUnit(
        index,
        argv[2], nullptr, 0,
        nullptr, 0,
        CXTranslationUnit_None);

    if (unit == nullptr)
    {
        std::cerr << "Unable to parse translation unit. Quitting." << std::endl;
        return 1;
    }

    UserContext userContext = {};
    userContext.macro = argv[1];

    CXCursor cursor = clang_getTranslationUnitCursor(unit);

    clang_visitChildren(
        cursor,
        [](CXCursor current_cursor, CXCursor parent_cursor, CXClientData client_data)
        {
            UserContext *userContext = (UserContext *)client_data;

            std::string cursorName;
            {
                CXString current_display_name = clang_getCursorDisplayName(current_cursor);
                cursorName = clang_getCString(current_display_name);
                clang_disposeString(current_display_name);
            }

            // std::cout << "Visiting " << cursorName << "(" << std::to_string(clang_getCursorKind(current_cursor)) << ")" << std::endl;

            std::string parentStructName;
            if (clang_getCursorKind(parent_cursor) == CXCursor_StructDecl)
            {
                CXString parent_display_name = clang_getCursorDisplayName(parent_cursor);
                parentStructName = clang_getCString(parent_display_name);
                clang_disposeString(parent_display_name);

                std::cout << userContext->macro << "(" << parentStructName << ", " << cursorName << ")\n";
            }
            
            bool isStruct = clang_getCursorKind(current_cursor) == CXCursor_StructDecl;
            if (isStruct && userContext->userMap.count(cursorName) == 0)
            {
                StructInfo info { cursorName };
                userContext->userMap[cursorName] = info;
            }

            return isStruct ? CXChildVisit_Recurse : CXChildVisit_Continue;
        },
        &userContext);

    clang_disposeTranslationUnit(unit);
    clang_disposeIndex(index);

    return 0;
}
