// Copyright ©  2018 - 2021 Joe Hsu. All rights reserved.
#include "SyntaxAnalysis.h"

char uPeekToken = '\0';
string uEntireToken = "";
string uEntireTokenType = ""; // ex: int, char, bool, floar, string
string uEntireTokenAttribute = ""; // ex: IDENT or Constant
string uDefinitionType = "";
int uGlobalPosition = -1;
int uLocalPosition = -1;
bool uQuit = false;
bool uCompareExpression = false;
bool uIsDoneID = false;
bool uCout = false;
bool uCin = false;
bool uIsListAllVariables = false;
bool uIsListVariable = false;
bool uIsListAllFunctions = false;
bool uIsListFunction = false;
int  uLineCounter = 1;
bool uLineBegin = false;
bool uTook = false;
string uConstantName; // "abc" take "abc"
bool uDoNotCompare = true;
// use to determine expression whether appear
// && || ! < <= > >= operator
int  uWalker = -1; // used for while, walk again
int  uLayer = 1;
// tag token belong to which Layer
// used to pop
bool uTokenTook = false;

string uListVariableName = "";
string uListFunctionName = "";
bool uDoFunctionCalculation = false;

vector<TableInfo>        gState;
vector<TableInfo>        gIdentifierTable;
vector<TableInfo>        gDef;
vector<TokenInfo>        gFunctionToken;
vector<string>           gCoutToken;
vector<ConditionInfo>    gCondition;
// it may be more than one condition
vector<TableInfo>        gParameter;
vector<TableInfo>        gReturnValue;

int main() {
    
    cout << "Copyright 2018 - 2021 Joe Hsu" << endl;
    cout << "Interpreter Sample" << endl << ">";

    try {
        uPeekToken = NextToken();
        int TestCase;
        TheEntireToken();  // use uPeekToken to get entire string , and the next uPeekToken is update
        if (uEntireToken.compare("1") == 0) {
            TestCase = atoi(uEntireToken.c_str());
            cout << "Our-C running ..." << endl;
            UserInput();

        } // if
        else if (uEntireToken.compare("2") == 0) {
            TestCase = atoi(uEntireToken.c_str());
            cout << "Our-C running ..." << endl;
            UserInput();
        } // else if
        else if (uEntireToken.compare("3") == 0) {
            TestCase = atoi(uEntireToken.c_str());
            cout << "Our-C running ..." << endl;
            UserInput();

        } // else if
        else {
            cout << "Our-C running ..." << endl;
            UserInput();
        } // else if

        cout << "> Our-C exited ..." << endl;
    } // try
    catch (ExceptionEOF* e) {
        return 0;
        cout << "> Our-C exited ..." << endl;
    } // catch

    system("pause");
    return 0;
} // main()
