// Copyright ©  2018 - 2021 Joe Hsu. All rights reserved.
#pragma once
#include <iomanip> 
#include "GetToken.h"

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

void Constant_Format();
void RestOfDeclarators();
void Recursive_Of_formal_parameter_list();
void FormalParameterList();
void Declaration();
void Actual_parameter_list(TableInfo& ReturnValue);
void Signed_unary_exp(TableInfo& ReturnValue);
void Unsigned_unary_exp(TableInfo& ReturnValue);
void Unary_exp(TableInfo& ReturnValue);
void Rest_of_maybe_mult_exp(TableInfo& ReturnValue);
void Maybe_mult_exp(TableInfo& ReturnValue);
void Rest_of_maybe_additive_exp(TableInfo& ReturnValue);
void Maybe_additive_exp(TableInfo& ReturnValue);
void Rest_of_maybe_shift_exp(TableInfo& ReturnValue);
void Maybe_shift_exp(TableInfo& ReturnValue);
void Rest_of_maybe_relational_exp(TableInfo& ReturnValue);
void Maybe_relational_exp(TableInfo& ReturnValue);
void Rest_of_maybe_equality_exp(TableInfo& ReturnValue);
void Maybe_equality_exp(TableInfo& ReturnValue);
void Rest_of_maybe_bit_AND_exp(TableInfo& ReturnValue);
void Maybe_bit_AND_exp(TableInfo& ReturnValue);
void Rest_of_maybe_bit_ex_OR_exp(TableInfo& ReturnValue);
void Maybe_bit_ex_OR_exp(TableInfo& ReturnValue);
void Rest_of_maybe_bit_OR_exp(TableInfo& ReturnValue);
void Maybe_bit_OR_exp(TableInfo& ReturnValue);
void Rest_of_maybe_logical_AND_exp(TableInfo& ReturnValue);
void Maybe_logical_AND_exp(TableInfo& ReturnValue);
void Rest_of_maybe_logical_OR_exp(TableInfo& ReturnValue);
void Romce_and_romloe(TableInfo& ReturnValue);
void Rest_of_Identifier_started_basic_exp(TableInfo& ReturnValue);
void Rest_of_PPMM_Identifier_started_basic_exp(TableInfo& ReturnValue);
void BasicExpression(TableInfo& ReturnValue);
void Expression(TableInfo& ReturnValue);
void Statement();
void CompoundStatement();
void FunctionDefinitionWithoutID();
void FunctionDefinitionOrDeclarators();
void Definition();
void UserInput();