// Copyright ©  2018 - 2021 Joe Hsu. All rights reserved.
#pragma once
# include <string>
# include <vector>     // vector, push_back

using namespace std; 

struct TokenInfo {
    string  Name;
    string  Attribute;
    string  Type;
};

struct ConditionInfo {
    bool   ConditionResult;
    int    Layer;
    bool   ConditionComplete;
    // used to check if, else if
};

struct TableInfo {
    string  Name;
    string  NameString;
    // NameString is used to print the
    // whole word when there is an error
    string  ValueString;
    // ValueString is used to assign
    // add the constant token to ValueString
    string  Type;
    string  Attribute;
    double  value;
    bool    Func; // true: function, false : variable
    bool    GlobalVariable;
    int     VariablePosition;
    vector<TokenInfo> TokenInfoList;
    int     ArraySize;
    string* Ptr;
    int     Layer;
    bool    FunctionParameter;
    int     ParameterSequenceCounter;
    // like: void a( int parameter1, int parameter2 )
};