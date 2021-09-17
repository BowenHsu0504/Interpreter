// Copyright ©  2018 - 2021 Joe Hsu. All rights reserved.
#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <vector> 
#include <sstream>
#include <iostream>
#include "Structure.h"

using namespace std;

extern bool uTokenTook;
extern string uEntireTokenAttribute;
extern string uEntireTokenType;
extern string uConstantName;
extern int  uWalker;
extern vector<TokenInfo> gFunctionToken;
extern char uPeekToken;
extern string uEntireToken;
extern int uLineCounter;
extern bool uLineBegin;
extern vector<TableInfo> gState;
extern int uGlobalPosition;
extern vector<TableInfo> gIdentifierTable;
extern int uLocalPosition;
extern vector<TableInfo> gDef;
extern vector<string> gCoutToken;

bool PP(string Name);
bool MM(string Name);
void TokenClear();
bool Number(char ch);
bool ReservedWords(string Name);
int StatePosition(string Name);
int StateArrayPosition(string Name, int ArrayIndex);
bool SaveIdentifier(string Name);
bool IDENT();
bool IdentifierExist(string Name);
char Scanner();
bool Delimiter(char ch);
bool BeginWithLetter(char ch);
bool WhiteSpace(char ch);
bool IDChar(char ch);
bool SpecifierType(string Name);
bool AssignmentOperator(string Name); 
bool Sign(string Name);
bool CompareTrueFalse(double Value1, double Value2, string Sign);
void Insert(vector<TableInfo>& Temp, TableInfo Item);
void DoAssignmentOperator(string Sign, TableInfo& ReturnValue, TableInfo& Temp);
bool FunctionReservedWord(string Name);
void StoreData();
void CoutOutput();