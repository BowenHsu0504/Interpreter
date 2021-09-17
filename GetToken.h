// Copyright ©  2018 - 2021 Joe Hsu. All rights reserved.
#pragma once
#include "Exception.h"
#include "BasicFunction.h"

extern string uDefinitionType;
extern int uLayer;
extern vector<TableInfo> gParameter;
extern bool uTook;
extern bool uDoFunctionCalculation;
extern vector<ConditionInfo> gCondition;
extern vector<TableInfo> gReturnValue;
extern bool uCout;
extern bool uDoNotCompare;
extern bool uCin;
extern bool uIsDoneID;
extern bool uIsListAllVariables;
extern bool uIsListAllFunctions;
extern bool uIsListVariable;
extern string uListVariableName;
extern bool uIsListFunction;
extern string uListFunctionName;
extern bool uQuit;
extern bool uCompareExpression;

char NextToken();
void DelimiterLexical();
void TheEntireToken();

void ListAllVariables();
void ListAllFunctions();
void ListVariable(string Name);
void StandardOutput(vector<TokenInfo> TokenInfoList);
void ListFunction(string Name);