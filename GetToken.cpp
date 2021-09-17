// Copyright ©  2018 - 2021 Joe Hsu. All rights reserved.
#include "GetToken.h"

char NextToken() {
    // the next non-white-space char
    char ch = Scanner();
    string Msg = "";

    while (ch != EOF && WhiteSpace(ch))
        ch = Scanner();
    if (ch == EOF) {
        Msg = "Eof!!!";
        throw new ExceptionEOF(Msg);
    } // if

    return ch;
} // NextToken()

void DelimiterLexical() {
    // delimiter always scan a char , then its rule must to confirm
    stringstream StringStream;
    string LineNumber;
    TokenInfo Temp;
    if (uPeekToken == '/') {
        // ex: /, //, /=
        TokenClear();
        uEntireToken = uPeekToken;
        uConstantName = uPeekToken;
        uPeekToken = Scanner();
        if (uPeekToken == '/') {
            // comment
            while (uPeekToken != '\n')
                uPeekToken = Scanner();
            if (!uLineBegin) {
                uLineBegin = true;
                uPeekToken = Scanner();
                uLineCounter = 1;
            } // if

            TheEntireToken();
        } // if
        else if (uPeekToken == '=') {
            uConstantName = uConstantName + uPeekToken;
            uEntireToken = uEntireToken + uPeekToken;
            uPeekToken = Scanner();

            Temp.Name = uEntireToken;
            Temp.Attribute = uEntireTokenAttribute;
            Temp.Type = uEntireTokenType;
            gFunctionToken.push_back(Temp);
            uWalker++;
        } // else if
        else {
            // Divide operator
            Temp.Name = uEntireToken;
            Temp.Attribute = uEntireTokenAttribute;
            Temp.Type = uEntireTokenType;
            gFunctionToken.push_back(Temp);
            uWalker++;
        } // else
    } // if
    else if (uPeekToken == '&') {
        // & Bit operator
        // && Logical operator
        TokenClear();
        uEntireToken = uPeekToken;
        uConstantName = uPeekToken;
        uPeekToken = Scanner();
        if (uPeekToken == '&') {
            // Logical operator
            uConstantName = uConstantName + uPeekToken;
            uEntireToken = uEntireToken + uPeekToken;
            uPeekToken = Scanner();
        } // if

        Temp.Name = uEntireToken;
        Temp.Attribute = uEntireTokenAttribute;
        Temp.Type = uEntireTokenType;
        gFunctionToken.push_back(Temp);
        uWalker++;
    } // else if
    else if (uPeekToken == '|') {
        // | Bit operator
        // || Logical operator
        TokenClear();
        uConstantName = uPeekToken;
        uEntireToken = uPeekToken;
        uPeekToken = Scanner();
        if (uPeekToken == '|') {
            // Logical operator
            uConstantName = uConstantName + uPeekToken;
            uEntireToken = uEntireToken + uPeekToken;
            uPeekToken = Scanner();
        } // if

        Temp.Name = uEntireToken;
        Temp.Attribute = uEntireTokenAttribute;
        Temp.Type = uEntireTokenType;
        gFunctionToken.push_back(Temp);
        uWalker++;
    } // else if
    else if (uPeekToken == ';') {
        TokenClear();
        uConstantName = uPeekToken;
        uEntireToken = uPeekToken;
        uPeekToken = Scanner();
        Temp.Name = uEntireToken;
        Temp.Attribute = uEntireTokenAttribute;
        Temp.Type = uEntireTokenType;
        gFunctionToken.push_back(Temp);
        uWalker++;
    } // else if
    else if (uPeekToken == '>') {
        // >= , > , >>
        TokenClear();
        uConstantName = uPeekToken;
        uEntireToken = uPeekToken;
        uPeekToken = Scanner();
        if (uPeekToken == '=') {
            // >=
            uConstantName = uConstantName + uPeekToken;
            uEntireToken = uEntireToken + uPeekToken;
            uPeekToken = Scanner();
        } // if
        else if (uPeekToken == '>') {
            // >>
            uConstantName = uConstantName + uPeekToken;
            uEntireToken = uEntireToken + uPeekToken;
            uPeekToken = Scanner();
        } // else if

        Temp.Name = uEntireToken;
        Temp.Attribute = uEntireTokenAttribute;
        Temp.Type = uEntireTokenType;
        gFunctionToken.push_back(Temp);
        uWalker++;
    } // else if
    else if (uPeekToken == '<') {
        // <= , < , <<
        TokenClear();
        uConstantName = uPeekToken;
        uEntireToken = uPeekToken;
        uPeekToken = Scanner();
        if (uPeekToken == '=') {
            // <=
            uConstantName = uConstantName + uPeekToken;
            uEntireToken = uEntireToken + uPeekToken;
            uPeekToken = Scanner();
        } // if
        else if (uPeekToken == '<') {
            // <<
            uConstantName = uConstantName + uPeekToken;
            uEntireToken = uEntireToken + uPeekToken;
            uPeekToken = Scanner();
        } // else if

        Temp.Name = uEntireToken;
        Temp.Attribute = uEntireTokenAttribute;
        Temp.Type = uEntireTokenType;
        gFunctionToken.push_back(Temp);
        uWalker++;
    } // else if
    else if (uPeekToken == '.') {
        TokenClear();
        uConstantName = uPeekToken;
        uEntireToken = uPeekToken;
        uPeekToken = Scanner();
        if (Number(uPeekToken)) {
            while (Number(uPeekToken)) {
                uConstantName = uConstantName + uPeekToken;
                uEntireToken = uEntireToken + uPeekToken;
                uPeekToken = Scanner();
            } // while

            uEntireTokenType = "float";
            uEntireTokenAttribute = "Constant";
            Temp.Name = uEntireToken;
            Temp.Attribute = uEntireTokenAttribute;
            Temp.Type = uEntireTokenType;
            gFunctionToken.push_back(Temp);
            uWalker++;
        } // if
        else {
            if (Delimiter(uPeekToken) || BeginWithLetter(uPeekToken) ||
                WhiteSpace(uPeekToken)) {
                StringStream << uLineCounter;
                StringStream >> LineNumber;
                string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
                throw new ExceptionUnexpected(msg);
            } // if
            else {
                // unrecognized
                StringStream << uLineCounter;
                StringStream >> LineNumber;
                string msg = "> Line " + LineNumber + " : unrecognized token with first char : '";
                string msg2 = msg + uPeekToken;
                msg2 = msg2 + "'";
                throw new ExceptionUnrecognized(msg2);
            } // else
        } // else
    } // else if
    else if (uPeekToken == '*') {
        // *, *=
        TokenClear();
        uConstantName = uPeekToken;
        uEntireToken = uPeekToken;
        uPeekToken = Scanner();
        if (uPeekToken == '=') {
            // *=
            uConstantName = uConstantName + uPeekToken;
            uEntireToken = uEntireToken + uPeekToken;
            uPeekToken = Scanner();
        } // if

        Temp.Name = uEntireToken;
        Temp.Attribute = uEntireTokenAttribute;
        Temp.Type = uEntireTokenType;
        gFunctionToken.push_back(Temp);
        uWalker++;
    } // else if
    else if (uPeekToken == '%') {
        // %=, %
        TokenClear();
        uConstantName = uPeekToken;
        uEntireToken = uPeekToken;
        uPeekToken = Scanner();
        if (uPeekToken == '=') {
            // %=
            uConstantName = uConstantName + uPeekToken;
            uEntireToken = uEntireToken + uPeekToken;
            uPeekToken = Scanner();
        } // if

        Temp.Name = uEntireToken;
        Temp.Attribute = uEntireTokenAttribute;
        Temp.Type = uEntireTokenType;
        gFunctionToken.push_back(Temp);
        uWalker++;
    } // else if
    else if (uPeekToken == '=') {
        // =, ==
        TokenClear();
        uConstantName = uPeekToken;
        uEntireToken = uPeekToken;
        uPeekToken = Scanner();
        if (uPeekToken == '=') {
            // ==
            uConstantName = uConstantName + uPeekToken;
            uEntireToken = uEntireToken + uPeekToken;
            uPeekToken = Scanner();
        } // if

        Temp.Name = uEntireToken;
        Temp.Attribute = uEntireTokenAttribute;
        Temp.Type = uEntireTokenType;
        gFunctionToken.push_back(Temp);
        uWalker++;
    } // else if
    else if (uPeekToken == '!') {
        // !, !=
        TokenClear();
        uConstantName = uPeekToken;
        uEntireToken = uPeekToken;
        uPeekToken = Scanner();
        if (uPeekToken == '=') {
            // !=
            uConstantName = uConstantName + uPeekToken;
            uEntireToken = uEntireToken + uPeekToken;
            uPeekToken = Scanner();
        } // if

        Temp.Name = uEntireToken;
        Temp.Attribute = uEntireTokenAttribute;
        Temp.Type = uEntireTokenType;
        gFunctionToken.push_back(Temp);
        uWalker++;
    } // else if
    else if (uPeekToken == '+') {
        // +, +=, ++
        TokenClear();
        uConstantName = uPeekToken;
        uEntireToken = uPeekToken;
        uPeekToken = Scanner();
        if (uPeekToken == '+') {
            // ++
            uConstantName = uConstantName + uPeekToken;
            uEntireToken = uEntireToken + uPeekToken;
            uPeekToken = Scanner();
        } // if
        else if (uPeekToken == '=') {
            // +=
            uConstantName = uConstantName + uPeekToken;
            uEntireToken = uEntireToken + uPeekToken;
            uPeekToken = Scanner();
        } // else if

        Temp.Name = uEntireToken;
        Temp.Attribute = uEntireTokenAttribute;
        Temp.Type = uEntireTokenType;
        gFunctionToken.push_back(Temp);
        uWalker++;
    } // else if
    else if (uPeekToken == '-') {
        // -, -=, --
        TokenClear();
        uConstantName = uPeekToken;
        uEntireToken = uPeekToken;
        uPeekToken = Scanner();
        if (uPeekToken == '-') {
            // --
            uConstantName = uConstantName + uPeekToken;
            uEntireToken = uEntireToken + uPeekToken;
            uPeekToken = Scanner();
        } // if
        else if (uPeekToken == '=') {
            // -=
            uConstantName = uConstantName + uPeekToken;
            uEntireToken = uEntireToken + uPeekToken;
            uPeekToken = Scanner();
        } // else if

        Temp.Name = uEntireToken;
        Temp.Attribute = uEntireTokenAttribute;
        Temp.Type = uEntireTokenType;
        gFunctionToken.push_back(Temp);
        uWalker++;
    } // else if
    else if (uPeekToken == '\'') {
        // char
        TokenClear();
        uConstantName = uPeekToken;
        uPeekToken = Scanner();
        while (uPeekToken != EOF && uPeekToken != '\'' && uPeekToken != '\n') {
            uEntireToken = uPeekToken;
            uConstantName = uConstantName + uPeekToken;
            uPeekToken = Scanner();
        } // while

        if (uPeekToken == EOF) {
            string msg = "Eof!!!";
            throw new ExceptionEOF(msg);
        } // if
        else if (uPeekToken == '\n') {
            StringStream << uLineCounter;
            StringStream >> LineNumber;
            string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
            throw new ExceptionUnexpected(msg);
        } // else if
        else {
            uConstantName = uConstantName + uPeekToken;
            uEntireTokenType = "char";
            uEntireTokenAttribute = "Constant";
            Temp.Name = uEntireToken;
            Temp.Attribute = uEntireTokenAttribute;
            Temp.Type = uEntireTokenType;
            gFunctionToken.push_back(Temp);
            uWalker++;
            uPeekToken = Scanner();
        } // else
    } // else if
    else if (uPeekToken == '"') {
        // string
        TokenClear();
        uConstantName = uPeekToken;
        uPeekToken = Scanner();
        while (uPeekToken != EOF && uPeekToken != '"' && uPeekToken != '\n') {
            if (uPeekToken == '\\') {
                uPeekToken = Scanner();
                if (uPeekToken == 'n') // \n
                    uEntireToken = uEntireToken + "\n";
                uPeekToken = Scanner();
            } // if
            else {
                uEntireToken = uEntireToken + uPeekToken;
                uConstantName = uConstantName + uPeekToken;
                uPeekToken = Scanner();
            } // else
        } // while

        if (uPeekToken == EOF) {
            string msg = "Eof!!!";
            throw new ExceptionEOF(msg);
        } // if
        else if (uPeekToken == '\n') {
            StringStream << uLineCounter;
            StringStream >> LineNumber;
            string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
            throw new ExceptionUnexpected(msg);
        } // else if
        else {
            uConstantName = uConstantName + uPeekToken;
            uEntireTokenType = "string";
            uEntireTokenAttribute = "Constant";
            Temp.Name = uEntireToken;
            Temp.Attribute = uEntireTokenAttribute;
            Temp.Type = uEntireTokenType;
            gFunctionToken.push_back(Temp);
            uWalker++;
            uPeekToken = Scanner();
        } // else
    } // else if
    else {
        TokenClear();
        uEntireToken = uPeekToken;
        uConstantName = uPeekToken;
        Temp.Name = uEntireToken;
        Temp.Attribute = uEntireTokenAttribute;
        Temp.Type = uEntireTokenType;
        gFunctionToken.push_back(Temp);
        uWalker++;
        uPeekToken = Scanner();
    } // else

} // DelimiterLexical()

void TheEntireToken() {
    // use uPeekToken to get the entire token
    stringstream StringStream;
    string LineNumber;
    TokenInfo Token;
    string Msg = "";

    if (!uTokenTook) {
        uEntireTokenAttribute = "";
        uEntireTokenType = "";
        uConstantName = "";
        if (uWalker == gFunctionToken.size() - 1) {
            if (Delimiter(uPeekToken)) {
                // delimiter
                DelimiterLexical();
            } // if
            else if (WhiteSpace(uPeekToken)) {
                if (!uLineBegin && uPeekToken == '\n') {
                    uLineBegin = true;
                    uPeekToken = Scanner();
                    uLineCounter = 1;
                    TheEntireToken();
                } // if
                else {
                    uPeekToken = Scanner();
                    TheEntireToken();
                } // else
            } // else if
            else if (BeginWithLetter(uPeekToken)) {
                // IDENT
                TokenClear();
                uConstantName = uPeekToken;
                uEntireToken = uPeekToken;
                uPeekToken = Scanner();
                while (IDChar(uPeekToken)) {
                    uConstantName = uConstantName + uPeekToken;
                    uEntireToken = uEntireToken + uPeekToken;
                    uPeekToken = Scanner();
                } // while

                if (uEntireToken.compare("true") == 0 ||
                    uEntireToken.compare("false") == 0) {
                    uEntireTokenAttribute = "Constant";
                    uEntireTokenType = "bool";
                } // if
                else if (ReservedWords(uEntireToken))
                    uEntireTokenAttribute = "Reserved";
                else
                    uEntireTokenAttribute = "IDENT";

                Token.Name = uEntireToken;
                Token.Attribute = uEntireTokenAttribute;
                Token.Type = uEntireTokenType;
                gFunctionToken.push_back(Token);
                uWalker++;
            } // else if
            else if (Number(uPeekToken)) {
                // number
                TokenClear();
                uEntireTokenAttribute = "Constant";
                uConstantName = uPeekToken;
                uEntireToken = uPeekToken;
                uPeekToken = Scanner();
                int FloatingPointCount = 0;
                uEntireTokenType = "int";
                while ((Number(uPeekToken) || uPeekToken == '.') &&
                    FloatingPointCount != 2) {
                    if (uPeekToken == '.') {
                        FloatingPointCount++;
                        uEntireTokenType = "float";
                    } // if

                    if (FloatingPointCount != 2) {
                        uConstantName = uConstantName + uPeekToken;
                        uEntireToken = uEntireToken + uPeekToken;
                        uPeekToken = Scanner();
                    } // if
                } // while

                Token.Name = uEntireToken;
                Token.Attribute = uEntireTokenAttribute;
                Token.Type = uEntireTokenType;
                gFunctionToken.push_back(Token);
                uWalker++;
            } // else if
            else {
                StringStream << uLineCounter;
                StringStream >> LineNumber;
                Msg = "> Line " + LineNumber + " : unrecognized token with first char : '";
                Msg += uPeekToken;
                Msg += "'";
                throw new ExceptionUnrecognized(Msg);
            } // else
        } // if
        else {
            uWalker++;
            uEntireTokenAttribute = gFunctionToken[uWalker].Attribute;
            uEntireToken = gFunctionToken[uWalker].Name;
            uEntireTokenType = gFunctionToken[uWalker].Type;
        } // else
    } // if
    else {
        uTokenTook = false;
    } // else
} // TheEntireToken()

void ListAllVariables() {
    vector<TableInfo> Temp;
    for (int i = 0; i < gIdentifierTable.size(); i++)
        if (!gIdentifierTable[i].Func)
            Insert(Temp, gIdentifierTable[i]);

    cout << "> ";
    for (int i = 0; i < Temp.size(); i++)
        cout << Temp[i].Name << endl;
} // ListAllVariables()

void ListAllFunctions() {
    vector<TableInfo> Temp; // use to sort
    for (int i = 0; i < gIdentifierTable.size(); i++)
        if (gIdentifierTable[i].Func)
            Insert(Temp, gIdentifierTable[i]);

    cout << "> ";
    for (int i = 0; i < Temp.size(); i++)
        cout << Temp[i].Name << "()" << endl;
} // ListAllFunctions()

void ListVariable(string Name) {
    if (SaveIdentifier(Name) && !gIdentifierTable[uGlobalPosition].Func) {
        cout << "> ";
        for (int i = 0; i < gIdentifierTable[uGlobalPosition].TokenInfoList.size(); i++) {
            cout << gIdentifierTable[uGlobalPosition].TokenInfoList[i].Name;
            if (i == gIdentifierTable[uGlobalPosition].TokenInfoList.size() - 1)
                cout << endl;
            else
                cout << " ";
        } // for
    } // if
    else {
        stringstream StringStream;
        string LineNumber;
        StringStream << uLineCounter;
        StringStream >> LineNumber;
        string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
        throw new ExceptionUnexpected(msg);
    } // else
} // ListVariable()

void StandardOutput(vector<TokenInfo> TokenInfoList) {
    int start_pos = 0;
    string one_space = " ";
    bool set_line_begin_space = true;
    bool first_word = true;
    bool set_one_line = false; // if, else, do, while
    bool do_not_handle_current_token = false; // do not handle flag
    for (int i = 0; i < TokenInfoList.size(); i++) {
        if (TokenInfoList[i].Name.compare("#") == 0) {
            // flag appears, condition if where to print \n
            if (TokenInfoList[i + 1].Name.compare("{") != 0) {
                cout << endl;
                start_pos = start_pos + 2;
                set_one_line = true;
                set_line_begin_space = true;
                first_word = true;
            } // if
            else
                set_one_line = false;

            do_not_handle_current_token = true;
        } // if
        else
            do_not_handle_current_token = false;

        // setting begin of line needs how many spaces
        if (!do_not_handle_current_token) {
            if (TokenInfoList[i].Name.compare("}") == 0) {
                start_pos = start_pos - 2;
                for (int i = start_pos; i > 0; i--)
                    cout << one_space;
                cout << TokenInfoList[i].Name << endl;
                set_line_begin_space = true;
                first_word = true;
            } // if
            else {
                if (set_line_begin_space) {
                    for (int i = start_pos; i > 0; i--)
                        cout << one_space;
                    set_line_begin_space = false;
                } // if

                if (TokenInfoList[i].Name.compare("[") == 0)
                    cout << TokenInfoList[i].Name;
                else if (TokenInfoList[i].Name.compare(",") == 0)
                    cout << TokenInfoList[i].Name;
                else if (TokenInfoList[i].Name.compare("(") == 0) {
                    if (TokenInfoList[i - 1].Name.compare("while") == 0 ||
                        TokenInfoList[i - 1].Name.compare("if") == 0)
                        cout << " " << TokenInfoList[i].Name;
                    else
                        cout << TokenInfoList[i].Name;
                } // else if
                else if (TokenInfoList[i].Name.compare(")") == 0) {
                    if (TokenInfoList[i - 1].Name.compare("(") != 0)
                        cout << " " << TokenInfoList[i].Name;
                    else
                        cout << TokenInfoList[i].Name;
                } // else if
                else if (PP(TokenInfoList[i].Name) || MM(TokenInfoList[i].Name))
                    cout << TokenInfoList[i].Name;
                else if (TokenInfoList[i].Name.compare("{") == 0 ||
                    TokenInfoList[i].Name.compare(";") == 0) {
                    cout << " " << TokenInfoList[i].Name << endl;
                    if (TokenInfoList[i].Name.compare("{") == 0) {
                        start_pos = start_pos + 2;
                    } // if
                    else {
                        if (set_one_line && TokenInfoList[i + 1].Name.compare("do") != 0)
                            start_pos = start_pos - 2;
                    } // else

                    set_line_begin_space = true;
                    first_word = true;
                } // else if
                else {
                    if (first_word) {
                        cout << TokenInfoList[i].Name;
                        first_word = false;
                    } // if
                    else
                        cout << " " << TokenInfoList[i].Name;
                } // else
            } // else
        } // if
    } // for
} // StandardOutput()

void ListFunction(string Name) {

    if (SaveIdentifier(Name) && gIdentifierTable[uGlobalPosition].Func) {
        cout << "> ";
        StandardOutput(gIdentifierTable[uGlobalPosition].TokenInfoList);
    } // if
    else {
        stringstream StringStream;
        string LineNumber;
        StringStream << uLineCounter;
        StringStream >> LineNumber;
        string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
        throw new ExceptionUnexpected(msg);
    } // else
} // ListFunction()