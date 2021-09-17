// Copyright ©  2018 - 2021 Joe Hsu. All rights reserved.
#include "SyntaxAnalysis.h"

void Constant_Format() {
    stringstream StringStream;
    string LineNumber;
    gDef[gDef.size() - 1].Ptr = NULL;
    if (uEntireToken.compare("[") == 0 &&
        uEntireTokenAttribute.compare("") == 0) {
        gDef[gDef.size() - 1].NameString = gDef[gDef.size() - 1].NameString + uEntireToken + " ";
        TheEntireToken();
        if (uEntireTokenAttribute.compare("Constant") == 0 &&
            uEntireTokenType == "int") {
            gDef[gDef.size() - 1].NameString = gDef[gDef.size() - 1].NameString + uEntireToken + " ";
            gDef[gDef.size() - 1].ArraySize = atoi(uEntireToken.c_str());
            gDef[gDef.size() - 1].Ptr = new string[atoi(uEntireToken.c_str())];
            TheEntireToken();
            if (uEntireToken.compare("]") == 0 &&
                uEntireTokenAttribute.compare("") == 0) {
                gDef[gDef.size() - 1].NameString = gDef[gDef.size() - 1].NameString + uEntireToken;
                TheEntireToken();
            } // if
            else {
                StringStream << uLineCounter;
                StringStream >> LineNumber;
                string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
                throw new ExceptionUnexpected(msg);
            } // else
        } // if
        else {
            StringStream << uLineCounter;
            StringStream >> LineNumber;
            string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
            throw new ExceptionUnexpected(msg);
        } // else
    } // if
} // Constant_Format()

void RestOfDeclarators() {
    // ex: int x; , int x[10]; , int x[BUFFER_SIZE];
    Constant_Format();
    stringstream StringStream;
    string LineNumber;
    while (uEntireToken.compare(",") == 0 &&
        uEntireTokenAttribute.compare("") == 0) {
        TheEntireToken();
        if (IDENT()) {
            // int x , y ;
            TableInfo Temp;
            Temp.Name = uEntireToken;
            Temp.NameString = uEntireToken;
            Temp.Attribute = "IDENT";
            Temp.ArraySize = -1;
            Temp.FunctionParameter = false;
            gDef.push_back(Temp);
            TheEntireToken();
            Constant_Format();
        } // if
        else { // int 123 ;
            StringStream << uLineCounter;
            StringStream >> LineNumber;
            string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
            throw new ExceptionUnexpected(msg);
        } // else
    } // while

    if (uEntireToken.compare(";") != 0 ||
        uEntireTokenAttribute.compare("") != 0) {
        StringStream << uLineCounter;
        StringStream >> LineNumber;
        string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
        throw new ExceptionUnexpected(msg);
    } // if
} // RestOfDeclarators()

void Recursive_Of_formal_parameter_list() {
    stringstream StringStream;
    string LineNumber;
    TableInfo Temp;
    Temp.Type = uEntireToken;
    TheEntireToken();
    if (uEntireToken.compare("&") == 0 &&
        uEntireTokenAttribute.compare("") == 0)
        TheEntireToken();

    if (IDENT()) {
        // Warning !!!!
        // ex: int Example( bool test ) {...}
        // the IDENT means test not Example
        Temp.Name = uEntireToken;
        Temp.Func = false;
        Temp.Attribute = "IDENT";
        Temp.ArraySize = -1;
        TheEntireToken();
        Constant_Format();
        gDef.push_back(Temp);
    } // if
    else {
        StringStream << uLineCounter;
        StringStream >> LineNumber;
        string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
        throw new ExceptionUnexpected(msg);
    } // else
} // Recursive_Of_formal_parameter_list()

void FormalParameterList() {
    int current_def = gDef.size();
    Recursive_Of_formal_parameter_list();
    while (uEntireToken.compare(",") == 0 &&
        uEntireTokenAttribute.compare("") == 0) {
        TheEntireToken();
        Recursive_Of_formal_parameter_list();
    } // while

    int parameter_counter = 1;
    for (int i = current_def; i < gDef.size(); i++) {
        gDef[i].FunctionParameter = true;
        gDef[i].ParameterSequenceCounter = parameter_counter;
        parameter_counter++;
    } // for

    if (uWalker != gFunctionToken.size() - 1) {
        // function call

        /*
        cout << "before: " << endl ;
        for ( int i = current_def ; i < gDef.size() ; i++ ) {
          cout << gDef[i].Name << " " << gDef[i].value << endl ;
        }
        */

        for (int i = current_def; i < gDef.size(); i++) {
            for (int j = 0; j < gParameter.size(); j++) {
                if (gDef[i].ParameterSequenceCounter ==
                    gParameter[j].ParameterSequenceCounter) {
                    gDef[i] = gParameter[j];
                    gDef[i].FunctionParameter = true;
                } // if
            } // for
        } // for

        /*
        cout << "after: " << endl ;
        for ( int i = current_def ; i < gDef.size() ; i++ ) {
          cout << gDef[i].Name << " " << gDef[i].value << endl ;
        }
        */
    } // if
} // FormalParameterList()

void Declaration() {
    // if ( SpecifierType( uEntireToken ) ) {

    uDefinitionType = uEntireToken;
    TheEntireToken();
    int current_pos = gDef.size();
    // use to save { ... } info
    // and store into gDef
    if (IDENT()) {
        TableInfo Temp;
        Temp.Name = uEntireToken;
        gDef.push_back(Temp);
        TheEntireToken();
        RestOfDeclarators();
        for (int i = current_pos; i < gDef.size(); i++) {
            gDef[i].Type = uDefinitionType;
            gDef[i].Attribute = "IDENT";
            gDef[i].FunctionParameter = false;
            gDef[i].Func = false;
            gDef[i].GlobalVariable = false;
            gDef[i].Layer = uLayer;
        } // for
    } // if
    else {
        stringstream StringStream;
        StringStream << uLineCounter;
        string LineNumber;
        StringStream >> LineNumber;
        string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
        throw new ExceptionUnexpected(msg);
    } // else
} // Declaration()

void Actual_parameter_list(TableInfo& ReturnValue) {
    BasicExpression(ReturnValue);

    while (uEntireToken.compare(",") == 0 &&
        uEntireTokenAttribute.compare("") == 0) {
        TheEntireToken();
        BasicExpression(ReturnValue);
    } // while
} // Actual_parameter_list()

void Signed_unary_exp(TableInfo& ReturnValue) {
    stringstream StringStream;
    string LineNumber;
    if (IDENT()) {
        if (IdentifierExist(uEntireToken)) {
            bool current_exp_is_func;
            int line_temp = 0;
            string reserved_word = uEntireToken;
            if (uGlobalPosition != -1 && uLocalPosition == -1) {
                current_exp_is_func = gIdentifierTable[uGlobalPosition].Func;
                ReturnValue = gIdentifierTable[uGlobalPosition];
            } // if
            else if (uGlobalPosition == -1 && uLocalPosition != -1) {
                current_exp_is_func = gDef[uLocalPosition].Func;
                ReturnValue = gDef[uLocalPosition];
            } // if
            else {
                cout << "Didn't exist at Signed_unary_exp( It should be catch )" << endl;
                exit(0);
            } // else

            int latest_value = StatePosition(ReturnValue.Name);
            if (latest_value >= 0) {
                // latest value at gState
                ReturnValue.value = gState[latest_value].value;
                ReturnValue.ValueString = gState[latest_value].ValueString;
                ReturnValue.Ptr = gState[latest_value].Ptr;
            } // if

            TheEntireToken();
            if (current_exp_is_func) {
                // is function
                if (uEntireToken.compare("(") != 0) {
                    StringStream << line_temp;
                    StringStream >> LineNumber;
                    string msg = "> Line " + LineNumber + " : undefined identifier " + "'" + reserved_word + "'";
                    if (line_temp != uLineCounter) // Done +
                        uTook = true;

                    throw new ExceptionUndefined(msg);
                } // if
            } // if
            else {
                // is variable
                if (uEntireToken.compare("(") == 0) {
                    StringStream << line_temp;
                    StringStream >> LineNumber;
                    string msg = "> Line " + LineNumber + " : undefined identifier " + "'" + reserved_word + "'";
                    if (line_temp != uLineCounter) // Done +
                        uTook = true;

                    throw new ExceptionUndefined(msg);
                } // if
            } // else

            if (uEntireToken.compare("(") == 0 &&
                uEntireTokenAttribute.compare("") == 0) {
                TheEntireToken();
                if (uEntireToken.compare(")") == 0 &&
                    uEntireTokenAttribute.compare("") == 0) {
                    // ()
                    TheEntireToken();
                } // if
                else if ((IDENT() || uEntireTokenAttribute.compare("Reserved") == 0) ||
                    ((PP(uEntireToken) || MM(uEntireToken)) &&
                        uEntireTokenAttribute.compare("") == 0) ||
                    Sign(uEntireToken) ||
                    uEntireTokenAttribute.compare("Constant") == 0 ||
                    (uEntireToken.compare("(") == 0 &&
                        uEntireTokenAttribute.compare("") == 0)) {
                    // actual_parameter_list
                    TableInfo Temp;
                    Actual_parameter_list(Temp);
                    if (uEntireToken.compare(")") == 0 &&
                        uEntireTokenAttribute.compare("") == 0) {
                        if (uDoFunctionCalculation) {
                            if (ReturnValue.Func) {
                                // function Name is ReturnValue.Name
                                // parameter is Temp.ValueString
                                // using ReturnValue.TokenInfoList to run again
                                int current_state_pos = gState.size();
                                int current_func_pos = gFunctionToken.size();
                                string current_token_name = uEntireToken;
                                string current_token_type = uEntireTokenType;
                                string current_token_attribute = uEntireTokenAttribute;
                                uWalker = current_func_pos;

                                // push back function token
                                // use these token to walk again
                                // syntax analysis for evaluate
                                for (int i = 0; i < ReturnValue.TokenInfoList.size(); i++)
                                    gFunctionToken.push_back(ReturnValue.TokenInfoList[i]);

                                Temp.FunctionParameter = true;
                                Temp.ParameterSequenceCounter = 1;
                                gParameter.push_back(Temp);

                                uEntireToken =
                                    gFunctionToken[uWalker].Name;
                                uEntireTokenAttribute =
                                    gFunctionToken[uWalker].Attribute;
                                uEntireTokenType =
                                    gFunctionToken[uWalker].Type;

                                Definition();

                                // restore initial state
                                for (int i = gFunctionToken.size() - current_func_pos; i > 0; i--)
                                    gFunctionToken.pop_back();

                                uWalker = gFunctionToken.size() - 1;
                                uEntireToken = current_token_name;
                                uEntireTokenAttribute = current_token_attribute;
                                uEntireTokenType = current_token_type;

                                // if statement is FunctionParameter
                                // and it's call by value
                                // erase that statement
                                // else not

                                vector<TableInfo>::iterator it = gState.begin() + current_state_pos;
                                for (int i = gState.size() - current_state_pos; i > 0; i--) {
                                    // erase located variable
                                    if (it->FunctionParameter) {
                                        gState.erase(it);
                                    } // if
                                    else {
                                        if (!it->GlobalVariable) {
                                            gState.erase(it);
                                        } // if
                                        else {
                                            it++;
                                        } // else
                                    } // else
                                } // for

                                if (ReturnValue.Type.compare("int") == 0 ||
                                    ReturnValue.Type.compare("char") == 0 ||
                                    ReturnValue.Type.compare("float") == 0 ||
                                    ReturnValue.Type.compare("string") == 0 ||
                                    ReturnValue.Type.compare("bool") == 0) {
                                    ReturnValue = gReturnValue[gReturnValue.size() - 1];
                                } // if
                                else
                                    cout << "not" << endl;
                            } // if
                            else {
                                cout << "not a function error" << endl;
                                exit(0);
                            } // else
                        } // if

                        TheEntireToken();
                    } // if
                    else {
                        StringStream << uLineCounter;
                        StringStream >> LineNumber;
                        string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
                        throw new ExceptionUnexpected(msg);
                    } // else
                } // else if
                else {
                    StringStream << uLineCounter;
                    StringStream >> LineNumber;
                    string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
                    throw new ExceptionUnexpected(msg);
                } // else
            } // if
            else if (uEntireToken.compare("[") == 0 &&
                uEntireTokenAttribute.compare("") == 0) {
                if (ReturnValue.Ptr != NULL) {
                    TheEntireToken();
                    if ((IDENT() || uEntireTokenAttribute.compare("Reserved") == 0) ||
                        ((PP(uEntireToken) || MM(uEntireToken)) &&
                            uEntireTokenAttribute.compare("") == 0) ||
                        Sign(uEntireToken) ||
                        uEntireTokenAttribute.compare("Constant") == 0 ||
                        (uEntireToken.compare("(") == 0 &&
                            uEntireTokenAttribute.compare("") == 0)) {
                        TableInfo Temp;
                        Expression(Temp);
                        if (Temp.Type.compare("int") == 0) {
                            if ((int)Temp.value < ReturnValue.ArraySize &&
                                (int)Temp.value >= 0) {
                                if (uEntireToken.compare("]") == 0 &&
                                    uEntireTokenAttribute.compare("") == 0) {
                                    int value = (int)Temp.value;
                                    int locate_at_state = StateArrayPosition(ReturnValue.Name, value);
                                    if (locate_at_state >= 0) {
                                        ReturnValue.ArraySize = (int)Temp.value;
                                        ReturnValue.value =
                                            gState[locate_at_state].value;
                                        ReturnValue.ValueString =
                                            gState[locate_at_state].ValueString;
                                        TheEntireToken();
                                    } // if
                                    else {
                                        ReturnValue.ArraySize = (int)Temp.value;
                                        ReturnValue.value = atof(ReturnValue.Ptr[(int)Temp.value].c_str());
                                        ReturnValue.ValueString = ReturnValue.Ptr[(int)Temp.value];
                                        TheEntireToken();
                                    } // else
                                } // if
                                else {
                                    StringStream << uLineCounter;
                                    StringStream >> LineNumber;
                                    string msg = "> Line " + LineNumber + " : unexpected token "
                                        + "'" + uConstantName + "'";
                                    throw new ExceptionUnexpected(msg);
                                } // else
                            } // if
                            else {
                                if (gCondition.size() != 0)
                                    TheEntireToken();
                            } // else
                        } // if
                        else {
                            StringStream << uLineCounter;
                            StringStream >> LineNumber;
                            string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
                            throw new ExceptionUnexpected(msg);
                        } // else
                    } // if
                    else {
                        StringStream << uLineCounter;
                        StringStream >> LineNumber;
                        string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
                        throw new ExceptionUnexpected(msg);
                    } // else
                } // if
                else { // int a; a[ , unexpected
                    StringStream << uLineCounter;
                    StringStream >> LineNumber;
                    string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
                    throw new ExceptionUnexpected(msg);
                } // else
            } // else if
        } // if
        else {
            StringStream << uLineCounter;
            StringStream >> LineNumber;
            string msg = "> Line " + LineNumber + " : undefined identifier " + "'" + uEntireToken + "'";
            throw new ExceptionUndefined(msg);
        } // else
    } // if
    else if (uEntireTokenAttribute.compare("Constant") == 0) {
        // 3 types: 112 11.2, "123", true false
        ReturnValue.Func = false;
        ReturnValue.Type = uEntireTokenType;
        ReturnValue.Attribute = "Constant";
        if (uEntireTokenType.compare("int") == 0 ||
            uEntireTokenType.compare("float") == 0)
            ReturnValue.value = atof(uEntireToken.c_str());
        else if (uEntireTokenType.compare("bool") == 0) {
            if (uEntireToken.compare("false") == 0)
                ReturnValue.value = 0;
            else
                ReturnValue.value = 1;
        } // else if
        else if (uEntireTokenType.compare("string") == 0 ||
            uEntireTokenType.compare("char") == 0)
            ReturnValue.ValueString = uEntireToken;
        TheEntireToken();
    } // else if
    else if (uEntireToken.compare("(") == 0 &&
        uEntireTokenAttribute.compare("") == 0) {
        TheEntireToken();
        if ((IDENT() || uEntireTokenAttribute.compare("Reserved") == 0) ||
            ((PP(uEntireToken) || MM(uEntireToken)) &&
                uEntireTokenAttribute.compare("") == 0) ||
            Sign(uEntireToken) ||
            uEntireTokenAttribute.compare("Constant") == 0 ||
            (uEntireToken.compare("(") == 0 &&
                uEntireTokenAttribute.compare("") == 0)) {
            Expression(ReturnValue);
            if (uEntireToken.compare(")") == 0 &&
                uEntireTokenAttribute.compare("") == 0)
                TheEntireToken();
            else {
                StringStream << uLineCounter;
                StringStream >> LineNumber;
                string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
                throw new ExceptionUnexpected(msg);
            } // else
        } // if
        else {
            StringStream << uLineCounter;
            StringStream >> LineNumber;
            string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
            throw new ExceptionUnexpected(msg);
        } // else
    } // else if
    else {
        StringStream << uLineCounter;
        StringStream >> LineNumber;
        string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
        throw new ExceptionUnexpected(msg);
    } // else
} // Signed_unary_exp()

void Unsigned_unary_exp(TableInfo& ReturnValue) {
    stringstream StringStream;
    string LineNumber;
    if (IDENT()) {
        if (IdentifierExist(uEntireToken)) {
            int line_temp = 0;
            string reserved_word = uEntireToken;
            if (uGlobalPosition != -1 && uLocalPosition == -1)
                ReturnValue = gIdentifierTable[uGlobalPosition];
            else if (uGlobalPosition == -1 && uLocalPosition != -1)
                ReturnValue = gDef[uLocalPosition];
            else {
                cout << "Didn't exist( It should be catch )" << endl;
                exit(0);
            } // else

            int latest_value = StatePosition(ReturnValue.Name);
            if (latest_value >= 0) { // latest value at gState
                ReturnValue.value = gState[latest_value].value;
                ReturnValue.ValueString = gState[latest_value].ValueString;
                ReturnValue.Ptr = gState[latest_value].Ptr;
            } // if

            TheEntireToken();
            if (ReturnValue.Func) { // is function
                if (uEntireToken.compare("(") != 0) {
                    StringStream << line_temp;
                    StringStream >> LineNumber;
                    string msg = "> Line " + LineNumber + " : undefined identifier " + "'" + reserved_word + "'";
                    if (line_temp != uLineCounter) // Done +
                        uTook = true;

                    throw new ExceptionUndefined(msg);
                } // if
            } // if
            else { // is variable
                if (uEntireToken.compare("(") == 0) {
                    StringStream << line_temp;
                    StringStream >> LineNumber;
                    string msg = "> Line " + LineNumber + " : undefined identifier " + "'" + reserved_word + "'";
                    if (line_temp != uLineCounter) // Done +
                        uTook = true;

                    throw new ExceptionUndefined(msg);
                } // if
            } // else

            if (uEntireToken.compare("(") == 0 &&
                uEntireTokenAttribute.compare("") == 0) {
                TheEntireToken();
                if (uEntireToken.compare(")") == 0 &&
                    uEntireTokenAttribute.compare("") == 0)
                    TheEntireToken();
                else if ((IDENT() || uEntireTokenAttribute.compare("Reserved") == 0) ||
                    ((PP(uEntireToken) || MM(uEntireToken)) &&
                        uEntireTokenAttribute.compare("") == 0) ||
                    Sign(uEntireToken) ||
                    uEntireTokenAttribute.compare("Constant") == 0 ||
                    (uEntireToken.compare("(") == 0 &&
                        uEntireTokenAttribute.compare("") == 0)) {
                    TableInfo Temp;
                    Actual_parameter_list(Temp);
                    if (uEntireToken.compare(")") == 0 &&
                        uEntireTokenAttribute.compare("") == 0) {
                        if (uDoFunctionCalculation) {
                            if (ReturnValue.Func) {
                                // function Name is ReturnValue.Name
                                // parameter is Temp.ValueString
                                // using ReturnValue.TokenInfoList to run again
                                int current_state_pos = gState.size();
                                int current_func_pos = gFunctionToken.size();
                                string current_token_name = uEntireToken;
                                string current_token_type = uEntireTokenType;
                                string current_token_attribute = uEntireTokenAttribute;
                                uWalker = current_func_pos;

                                // push back function token
                                // use these token to walk again
                                // syntax analysis for evaluate
                                for (int i = 0; i < ReturnValue.TokenInfoList.size(); i++)
                                    gFunctionToken.push_back(ReturnValue.TokenInfoList[i]);

                                Temp.FunctionParameter = true;
                                Temp.ParameterSequenceCounter = 1;
                                gParameter.push_back(Temp);

                                uEntireToken =
                                    gFunctionToken[uWalker].Name;
                                uEntireTokenAttribute =
                                    gFunctionToken[uWalker].Attribute;
                                uEntireTokenType =
                                    gFunctionToken[uWalker].Type;

                                Definition();

                                // restore initial state
                                for (int i = gFunctionToken.size() - current_func_pos; i > 0; i--)
                                    gFunctionToken.pop_back();

                                uWalker = current_func_pos - 1;
                                uEntireToken = current_token_name;
                                uEntireTokenAttribute = current_token_attribute;
                                uEntireTokenType = current_token_type;

                                // if statement is FunctionParameter
                                // and it's call by value
                                // erase that statement
                                // else not

                                vector<TableInfo>::iterator it = gState.begin() + current_state_pos;
                                for (int i = gState.size() - current_state_pos; i > 0; i--) {
                                    // erase located variable
                                    if (it->FunctionParameter) {
                                        gState.erase(it);
                                    } // if
                                    else {
                                        if (!it->GlobalVariable) {
                                            gState.erase(it);
                                        } // if
                                        else {
                                            it++;
                                        } // else
                                    } // else
                                } // for

                                if (ReturnValue.Type.compare("int") == 0 ||
                                    ReturnValue.Type.compare("char") == 0 ||
                                    ReturnValue.Type.compare("float") == 0 ||
                                    ReturnValue.Type.compare("string") == 0 ||
                                    ReturnValue.Type.compare("bool") == 0) {
                                    ReturnValue = gReturnValue[gReturnValue.size() - 1];
                                } // if
                                else
                                    cout << "not" << endl;
                            } // if
                            else {
                                cout << "not a function error" << endl;
                                exit(0);
                            } // else
                        } // if

                        TheEntireToken();
                    } // if
                    else {
                        StringStream << uLineCounter;
                        StringStream >> LineNumber;
                        string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
                        throw new ExceptionUnexpected(msg);
                    } // else
                } // else if
                else {
                    StringStream << uLineCounter;
                    StringStream >> LineNumber;
                    string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
                    throw new ExceptionUnexpected(msg);
                } // else
            } // if
            else if (uEntireToken.compare("[") == 0 &&
                uEntireTokenAttribute.compare("") == 0) {
                if (ReturnValue.Ptr != NULL) {
                    TheEntireToken();
                    if ((IDENT() || uEntireTokenAttribute.compare("Reserved") == 0) ||
                        ((PP(uEntireToken) || MM(uEntireToken)) &&
                            uEntireTokenAttribute.compare("") == 0) ||
                        Sign(uEntireToken) ||
                        uEntireTokenAttribute.compare("Constant") == 0 ||
                        (uEntireToken.compare("(") == 0 &&
                            uEntireTokenAttribute.compare("") == 0)) {
                        TableInfo Temp;
                        Expression(Temp);
                        if (Temp.Type.compare("int") == 0) {
                            if ((int)Temp.value < ReturnValue.ArraySize &&
                                (int)Temp.value >= 0) {
                                if (uEntireToken.compare("]") == 0 &&
                                    uEntireTokenAttribute.compare("") == 0) {
                                    int value = (int)Temp.value;
                                    int locate_at_state = StateArrayPosition(ReturnValue.Name, value);
                                    if (locate_at_state >= 0) {
                                        ReturnValue.ArraySize = (int)Temp.value;
                                        ReturnValue.value =
                                            gState[locate_at_state].value;
                                        ReturnValue.ValueString =
                                            gState[locate_at_state].ValueString;
                                        TheEntireToken();
                                    } // if
                                    else {
                                        ReturnValue.ArraySize = (int)Temp.value;
                                        ReturnValue.value = atof(ReturnValue.Ptr[(int)Temp.value].c_str());
                                        ReturnValue.ValueString = ReturnValue.Ptr[(int)Temp.value];
                                        TheEntireToken();
                                    } // else

                                    if ((PP(uEntireToken) || MM(uEntireToken)) &&
                                        uEntireTokenAttribute.compare("") == 0) {
                                        TableInfo Temp;
                                        Temp = ReturnValue;
                                        if (PP(uEntireToken)) {
                                            Temp.value++;
                                            gState.push_back(Temp);
                                        } // if
                                        else {
                                            Temp.value--;
                                            gState.push_back(Temp);
                                        } // else

                                        TheEntireToken();
                                    } // if
                                } // if
                                else {
                                    StringStream << uLineCounter;
                                    StringStream >> LineNumber;
                                    string msg = "> Line " + LineNumber + " : unexpected token "
                                        + "'" + uConstantName + "'";
                                    throw new ExceptionUnexpected(msg);
                                } // else
                            } // if
                            else {
                                // cout << "out of range at Unsigned_unary_exp" << endl ;
                                // exit( 0 ) ;
                                if (gCondition.size() != 0)
                                    TheEntireToken();
                            } // else
                        } // if
                        else {
                            StringStream << uLineCounter;
                            StringStream >> LineNumber;
                            string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
                            throw new ExceptionUnexpected(msg);
                        } // else
                    } // if
                    else {
                        StringStream << uLineCounter;
                        StringStream >> LineNumber;
                        string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
                        throw new ExceptionUnexpected(msg);
                    } // else
                } // if
                else { // int a; a[ , unexpected
                    StringStream << uLineCounter;
                    StringStream >> LineNumber;
                    string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
                    throw new ExceptionUnexpected(msg);
                } // else
            } // else if
            else if ((PP(uEntireToken) || MM(uEntireToken)) &&
                uEntireTokenAttribute.compare("") == 0) {
                TableInfo Temp;
                Temp = ReturnValue;
                if (PP(uEntireToken)) {
                    Temp.value++;
                    gState.push_back(Temp);
                } // if
                else {
                    Temp.value--;
                    gState.push_back(Temp);
                } // else

                TheEntireToken();
            } // else if
        } // if
        else {
            StringStream << uLineCounter;
            StringStream >> LineNumber;
            string msg = "> Line " + LineNumber + " : undefined identifier " + "'" + uEntireToken + "'";
            throw new ExceptionUndefined(msg);
        } // else
    } // if
    else if (uEntireTokenAttribute.compare("Constant") == 0) {
        // 3 types: 112 11.2, "123", true false
        ReturnValue.Func = false;
        ReturnValue.Type = uEntireTokenType;
        ReturnValue.Attribute = "Constant";
        if (uEntireTokenType.compare("int") == 0 ||
            uEntireTokenType.compare("float") == 0)
            ReturnValue.value = atof(uEntireToken.c_str());
        else if (uEntireTokenType.compare("bool") == 0) {
            if (uEntireToken.compare("false") == 0)
                ReturnValue.value = 0;
            else
                ReturnValue.value = 1;
        } // else if
        else if (uEntireTokenType.compare("string") == 0 ||
            uEntireTokenType.compare("char") == 0)
            ReturnValue.ValueString = uEntireToken;
        TheEntireToken();
    } // else if
    else if (uEntireToken.compare("(") == 0 &&
        uEntireTokenAttribute.compare("") == 0) {
        TheEntireToken();
        if ((IDENT() || uEntireTokenAttribute.compare("Reserved") == 0) ||
            ((PP(uEntireToken) || MM(uEntireToken)) &&
                uEntireTokenAttribute.compare("") == 0) ||
            Sign(uEntireToken) ||
            uEntireTokenAttribute.compare("Constant") == 0 ||
            (uEntireToken.compare("(") == 0 &&
                uEntireTokenAttribute.compare("") == 0)) {
            Expression(ReturnValue);
            if (uEntireToken.compare(")") == 0 &&
                uEntireTokenAttribute.compare("") == 0) {
                TheEntireToken();
            } // if
            else {
                StringStream << uLineCounter;
                StringStream >> LineNumber;
                string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
                throw new ExceptionUnexpected(msg);
            } // else
        } // if
        else {
            StringStream << uLineCounter;
            StringStream >> LineNumber;
            string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
            throw new ExceptionUnexpected(msg);
        } // else
    } // else if
    else {
        StringStream << uLineCounter;
        StringStream >> LineNumber;
        string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
        throw new ExceptionUnexpected(msg);
    } // else
} // Unsigned_unary_exp()

void Unary_exp(TableInfo& ReturnValue) {
    stringstream StringStream;
    string LineNumber;
    if (Sign(uEntireToken)) {
        vector<string> sign_;
        sign_.push_back(uEntireToken);
        TheEntireToken();
        while (Sign(uEntireToken)) {
            sign_.push_back(uEntireToken);
            TheEntireToken();
        } // while

        Signed_unary_exp(ReturnValue);
        for (int i = sign_.size() - 1; i >= 0; i--) {
            if (sign_[i].compare("+") == 0)
                ReturnValue.value = +ReturnValue.value;
            else if (sign_[i].compare("-") == 0)
                ReturnValue.value = -ReturnValue.value;
            else if (sign_[i].compare("!") == 0)
                ReturnValue.value = !ReturnValue.value;
        } // for
    } // if
    else if ((PP(uEntireToken) || MM(uEntireToken)) &&
        uEntireTokenAttribute.compare("") == 0) {
        string sign_ = uEntireToken;
        TheEntireToken();
        if (IDENT()) {
            if (IdentifierExist(uEntireToken)) {
                int line_temp = 0;
                string reserved_word = uEntireToken;
                if (uGlobalPosition != -1 && uLocalPosition == -1)
                    ReturnValue = gIdentifierTable[uGlobalPosition];
                else if (uGlobalPosition == -1 && uLocalPosition != -1)
                    ReturnValue = gDef[uLocalPosition];
                else {
                    cout << "Didn't exist( It should be catch )" << endl;
                    exit(0);
                } // else

                int latest_value = StatePosition(ReturnValue.Name);
                if (latest_value >= 0) {
                    // latest value at gState
                    ReturnValue.value = gState[latest_value].value;
                    ReturnValue.ValueString = gState[latest_value].ValueString;
                    ReturnValue.Ptr = gState[latest_value].Ptr;
                } // if

                TheEntireToken();
                if (ReturnValue.Func) {
                    // is function
                    if (uEntireToken.compare("(") != 0) {
                        StringStream << line_temp;
                        StringStream >> LineNumber;
                        string msg = "> Line " + LineNumber + " : undefined identifier "
                            + "'" + reserved_word + "'";
                        if (line_temp != uLineCounter) // Done +
                            uTook = true;

                        throw new ExceptionUndefined(msg);
                    } // if
                } // if
                else {
                    // is variable
                    if (uEntireToken.compare("(") == 0) {
                        StringStream << line_temp;
                        StringStream >> LineNumber;
                        string msg = "> Line " + LineNumber + " : undefined identifier "
                            + "'" + reserved_word + "'";
                        if (line_temp != uLineCounter) // Done +
                            uTook = true;

                        throw new ExceptionUndefined(msg);
                    } // if
                } // else

                if (uEntireToken.compare("[") == 0 &&
                    uEntireTokenAttribute.compare("") == 0) {
                    if (ReturnValue.Ptr != NULL) {
                        TheEntireToken();
                        if ((IDENT() || uEntireTokenAttribute.compare("Reserved") == 0) ||
                            ((PP(uEntireToken) || MM(uEntireToken)) &&
                                uEntireTokenAttribute.compare("") == 0) ||
                            Sign(uEntireToken) ||
                            uEntireTokenAttribute.compare("Constant") == 0 ||
                            (uEntireToken.compare("(") == 0 &&
                                uEntireTokenAttribute.compare("") == 0)) {
                            TableInfo Temp;
                            Expression(Temp);
                            if (Temp.Type.compare("int") == 0) {
                                if ((int)Temp.value < ReturnValue.ArraySize &&
                                    (int)Temp.value >= 0) {
                                    if (uEntireToken.compare("]") == 0 &&
                                        uEntireTokenAttribute.compare("") == 0) {
                                        int value = (int)Temp.value;
                                        int locate_at_state = StateArrayPosition(ReturnValue.Name, value);
                                        if (locate_at_state >= 0) {
                                            ReturnValue.ArraySize = (int)Temp.value;
                                            ReturnValue.value =
                                                gState[locate_at_state].value;
                                            ReturnValue.ValueString =
                                                gState[locate_at_state].ValueString;
                                            TheEntireToken();
                                        } // if
                                        else {
                                            ReturnValue.ArraySize = (int)Temp.value;
                                            ReturnValue.value = atof(ReturnValue.Ptr[(int)Temp.value].c_str());
                                            ReturnValue.ValueString = ReturnValue.Ptr[(int)Temp.value];
                                            TheEntireToken();
                                        } // else
                                    } // if
                                    else {
                                        StringStream << uLineCounter;
                                        StringStream >> LineNumber;
                                        string msg = "> Line " + LineNumber + " : unexpected token "
                                            + "'" + uConstantName + "'";
                                        throw new ExceptionUnexpected(msg);
                                    } // else
                                } // if
                                else {
                                    cout << "out of range at Unary_exp" << endl;
                                    exit(0);
                                    StringStream << uLineCounter;
                                    StringStream >> LineNumber;
                                    string msg = "> Line " + LineNumber + " : unexpected token "
                                        + "'" + uConstantName + "'";
                                    throw new ExceptionUnexpected(msg);
                                } // else
                            } // if
                            else {
                                StringStream << uLineCounter;
                                StringStream >> LineNumber;
                                string msg = "> Line " + LineNumber + " : unexpected token "
                                    + "'" + uConstantName + "'";
                                throw new ExceptionUnexpected(msg);
                            } // else
                        } // if
                        else { // int a; a[ , unexpected
                            StringStream << uLineCounter;
                            StringStream >> LineNumber;
                            string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
                            throw new ExceptionUnexpected(msg);
                        } // else
                    } // if
                    else {
                        StringStream << uLineCounter;
                        StringStream >> LineNumber;
                        string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
                        throw new ExceptionUnexpected(msg);
                    } // else
                } // if

                if (sign_.compare("++") == 0)
                    ++ReturnValue.value;
                else if (sign_.compare("--") == 0)
                    --ReturnValue.value;
                gState.push_back(ReturnValue);
            } // if
            else {
                StringStream << uLineCounter;
                StringStream >> LineNumber;
                string msg = "> Line " + LineNumber + " : undefined identifier " + "'" + uEntireToken + "'";
                throw new ExceptionUndefined(msg);
            } // else
        } // if
        else {
            StringStream << uLineCounter;
            StringStream >> LineNumber;
            string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
            throw new ExceptionUnexpected(msg);
        } // else
    } // if
    else
        Unsigned_unary_exp(ReturnValue);
} // Unary_exp()

void Rest_of_maybe_mult_exp(TableInfo& ReturnValue) {

    while ((uEntireToken.compare("*") == 0 || uEntireToken.compare("/") == 0 ||
        uEntireToken.compare("%") == 0) &&
        uEntireTokenAttribute.compare("") == 0) {
        string sign_ = uEntireToken;
        TheEntireToken();
        TableInfo Temp;
        Unary_exp(Temp);
        if (sign_.compare("*") == 0)
            ReturnValue.value = ReturnValue.value * Temp.value;
        else if (sign_.compare("/") == 0) {
            ReturnValue.value = ReturnValue.value / Temp.value;
            if (ReturnValue.Type.compare("int") == 0) {
                int Temp = (int)ReturnValue.value;
                ReturnValue.value = (double)Temp;
            } // if
        } // else if
        else if (sign_.compare("%") == 0)
            ReturnValue.value = (int)ReturnValue.value % (int)Temp.value;
        else {
            cout << "unexpected at Rest_of_maybe_mult_exp" << endl;
            exit(0);
        } // else
    } // while
} // Rest_of_maybe_mult_exp()

void Maybe_mult_exp(TableInfo& ReturnValue) {

    Unary_exp(ReturnValue);
    Rest_of_maybe_mult_exp(ReturnValue);

} // Maybe_mult_exp()

void Rest_of_maybe_additive_exp(TableInfo& ReturnValue) {

    Rest_of_maybe_mult_exp(ReturnValue);

    while ((uEntireToken.compare("+") == 0 || uEntireToken.compare("-") == 0) &&
        uEntireTokenAttribute.compare("") == 0) {
        string sign_ = uEntireToken;
        TheEntireToken();
        TableInfo Temp;
        Maybe_mult_exp(Temp);
        if (sign_.compare("+") == 0) {
            ReturnValue.value = ReturnValue.value + Temp.value;
            ReturnValue.ValueString = ReturnValue.ValueString + Temp.ValueString;
        } // if
        else if (sign_.compare("-") == 0)
            ReturnValue.value = ReturnValue.value - Temp.value;
        else {
            cout << "unexpected at Rest_of_maybe_additive_exp" << endl;
            exit(0);
        } // else
    } // while

} // Rest_of_maybe_additive_exp()

void Maybe_additive_exp(TableInfo& ReturnValue) {

    Maybe_mult_exp(ReturnValue);

    while ((uEntireToken.compare("+") == 0 || uEntireToken.compare("-") == 0) &&
        uEntireTokenAttribute.compare("") == 0) {
        string sign_ = uEntireToken;
        TheEntireToken();
        TableInfo Temp;
        Maybe_mult_exp(Temp);
        if (sign_.compare("+") == 0) {
            ReturnValue.value = ReturnValue.value + Temp.value;
            ReturnValue.ValueString = ReturnValue.ValueString + Temp.ValueString;
        } // if
        else if (sign_.compare("-") == 0)
            ReturnValue.value = ReturnValue.value - Temp.value;
        else {
            cout << "unexpected at Maybe_additive_exp" << endl;
            exit(0);
        } // else
    } // while

} // Maybe_additive_exp()

void Rest_of_maybe_shift_exp(TableInfo& ReturnValue) {
    // << may represent multiply 2^, >> may represent divide 2^
    Rest_of_maybe_additive_exp(ReturnValue);
    stringstream StringStream;
    string LineNumber;
    while ((uEntireToken.compare("<<") == 0 || uEntireToken.compare(">>") == 0) &&
        uEntireTokenAttribute.compare("") == 0) {
        bool print = uCout;
        string sign_ = uEntireToken;
        TheEntireToken();
        TableInfo Temp;
        Maybe_additive_exp(Temp);
        if (Temp.Attribute.compare("Constant") == 0 &&
            Temp.Type.compare("int") == 0) {
            // shift
            if (sign_.compare("<<") == 0) {
                ReturnValue.value = (int)ReturnValue.value << (int)Temp.value;
            } // if
            else {
                ReturnValue.value = (int)ReturnValue.value >> (int)Temp.value;
            } // else
        } // if
        else {
            if (print) {
                // push to output_table
                // used to print

                if (Temp.Type.compare("string") == 0 ||
                    Temp.Type.compare("char") == 0) {
                    gCoutToken.push_back(Temp.ValueString);
                } // if
                else {
                    if (uDoNotCompare && Temp.Type.compare("bool") != 0) {
                        stringstream StringStream;
                        if (Temp.Type.compare("float") == 0)
                            Temp.value = (round(Temp.value * pow(10, 3))) / pow(10, 3);
                        else if (Temp.Type.compare("int") == 0)
                            Temp.value = floor(Temp.value);
                        StringStream << fixed << setprecision(0);
                        StringStream << Temp.value;
                        string s_value;
                        StringStream >> s_value;
                        gCoutToken.push_back(s_value);
                    } // if
                    else {
                        if (Temp.value != 0)
                            gCoutToken.push_back("true");
                        else
                            gCoutToken.push_back("false");
                    } // else
                } // else
            } // if
            else if (uCin) {
                cout << "not yet handle \">>\" at Rest_of_maybe_shift_exp" << endl;
                exit(0);
            } // else if
        } // else
    } // while
} // Rest_of_maybe_shift_exp()

void Maybe_shift_exp(TableInfo& ReturnValue) {

    Maybe_additive_exp(ReturnValue);
    stringstream StringStream;
    string LineNumber;
    while ((uEntireToken.compare("<<") == 0 || uEntireToken.compare(">>") == 0) &&
        uEntireTokenAttribute.compare("") == 0) {
        bool print = uCout;
        string sign_ = uEntireToken;
        TheEntireToken();
        TableInfo Temp;
        Maybe_additive_exp(Temp);
        if (Temp.Attribute.compare("Constant") == 0 &&
            Temp.Type.compare("int") == 0) {
            // shift
            if (sign_.compare("<<") == 0) {
                ReturnValue.value = (int)ReturnValue.value << (int)Temp.value;
            } // if
            else {
                ReturnValue.value = (int)ReturnValue.value >> (int)Temp.value;
            } // else
        } // if
        else {
            if (print) {
                // push to output_table
                // used to print
                if (Temp.Type.compare("string") == 0 ||
                    Temp.Type.compare("char") == 0) {
                    gCoutToken.push_back(Temp.ValueString);
                } // if
                else {
                    if (uDoNotCompare && Temp.Type.compare("bool") != 0) {
                        stringstream StringStream;
                        if (Temp.Type.compare("float") == 0)
                            Temp.value = (round(Temp.value * pow(10, 3))) / pow(10, 3);
                        else if (Temp.Type.compare("int") == 0)
                            Temp.value = floor(Temp.value);
                        StringStream << Temp.value;
                        string s_value;
                        StringStream >> s_value;
                        gCoutToken.push_back(s_value);
                    } // if
                    else {
                        if (Temp.value != 0)
                            gCoutToken.push_back("true");
                        else
                            gCoutToken.push_back("false");
                    } // else
                } // else
            } // if
            else if (uCin) {
                cout << "not yet handle \">>\" at Rest_of_maybe_shift_exp" << endl;
                exit(0);
            } // else if
        } // else
    } // while
} // Maybe_shift_exp()

void Rest_of_maybe_relational_exp(TableInfo& ReturnValue) {

    Rest_of_maybe_shift_exp(ReturnValue);

    while ((uEntireToken.compare("<") == 0 || uEntireToken.compare(">") == 0 ||
        uEntireToken.compare("<=") == 0 || uEntireToken.compare(">=") == 0)
        && uEntireTokenAttribute.compare("") == 0) {
        string sign_ = uEntireToken;
        uDoNotCompare = false;
        TheEntireToken();
        TableInfo Temp;
        Maybe_shift_exp(Temp);
        ReturnValue.value = CompareTrueFalse(ReturnValue.value, Temp.value, sign_);
    } // while

} // Rest_of_maybe_relational_exp()

void Maybe_relational_exp(TableInfo& ReturnValue) {

    Maybe_shift_exp(ReturnValue);

    while ((uEntireToken.compare("<") == 0 || uEntireToken.compare(">") == 0 ||
        uEntireToken.compare("<=") == 0 || uEntireToken.compare(">=") == 0)
        && uEntireTokenAttribute.compare("") == 0) {
        string sign_ = uEntireToken;
        uDoNotCompare = false;
        TheEntireToken();
        TableInfo Temp;
        Maybe_shift_exp(Temp);
        ReturnValue.value = CompareTrueFalse(ReturnValue.value, Temp.value, sign_);
    } // while

} // Maybe_relational_exp()

void Rest_of_maybe_equality_exp(TableInfo& ReturnValue) {

    Rest_of_maybe_relational_exp(ReturnValue);

    while ((uEntireToken.compare("==") == 0 || uEntireToken.compare("!=") == 0)
        && uEntireTokenAttribute.compare("") == 0) {
        string sign_ = uEntireToken;
        uDoNotCompare = false;
        TheEntireToken();
        TableInfo Temp;
        Maybe_relational_exp(Temp);
        ReturnValue.value = CompareTrueFalse(ReturnValue.value, Temp.value, sign_);
    } // while

} // Rest_of_maybe_equality_exp()

void Maybe_equality_exp(TableInfo& ReturnValue) {

    Maybe_relational_exp(ReturnValue);

    while ((uEntireToken.compare("==") == 0 || uEntireToken.compare("!=") == 0)
        && uEntireTokenAttribute.compare("") == 0) {
        string sign_ = uEntireToken;
        uDoNotCompare = false;
        TheEntireToken();
        TableInfo Temp;
        Maybe_relational_exp(Temp);
        ReturnValue.value = CompareTrueFalse(ReturnValue.value, Temp.value, sign_);
    } // while

} // Maybe_equality_exp()

void Rest_of_maybe_bit_AND_exp(TableInfo& ReturnValue) {  // Question !!!!!!!

    Rest_of_maybe_equality_exp(ReturnValue);

    while (uEntireToken.compare("&") == 0 &&
        uEntireTokenAttribute.compare("") == 0) {     // What is "&" mean
        string sign_ = uEntireToken;
        TheEntireToken();
        TableInfo Temp;
        Maybe_equality_exp(Temp);
        // need do some here !!!
    } // while

} // Rest_of_maybe_bit_AND_exp()

void Maybe_bit_AND_exp(TableInfo& ReturnValue) {          // Question !!!!!!!

    Maybe_equality_exp(ReturnValue);

    while (uEntireToken.compare("&") == 0 &&
        uEntireTokenAttribute.compare("") == 0) {     // What is "&" mean
        string sign_ = uEntireToken;
        TheEntireToken();
        TableInfo Temp;
        Maybe_equality_exp(Temp);
        // need do some here !!!
    } // while

} // Maybe_bit_AND_exp()

void Rest_of_maybe_bit_ex_OR_exp(TableInfo& ReturnValue) {
    // Question !!!!!!!
    Rest_of_maybe_bit_AND_exp(ReturnValue);

    while (uEntireToken.compare("^") == 0 &&
        uEntireTokenAttribute.compare("") == 0) {
        // What is "^" mean
        string sign_ = uEntireToken;
        TheEntireToken();
        TableInfo Temp;
        Maybe_bit_AND_exp(Temp);
        // need do some here !!!
    } // while

} // Rest_of_maybe_bit_ex_OR_exp()

void Maybe_bit_ex_OR_exp(TableInfo& ReturnValue) {
    // Question !!!!!!!
    Maybe_bit_AND_exp(ReturnValue);

    while (uEntireToken.compare("^") == 0 &&
        uEntireTokenAttribute.compare("") == 0) {
        // What is "^" mean
        string sign_ = uEntireToken;
        TheEntireToken();
        TableInfo Temp;
        Maybe_bit_AND_exp(Temp);
        // need do some here !!!
    } // while

} // Maybe_bit_ex_OR_exp()

void Rest_of_maybe_bit_OR_exp(TableInfo& ReturnValue) {
    // Question !!!!!!!
    Rest_of_maybe_bit_ex_OR_exp(ReturnValue);

    while (uEntireToken.compare("|") == 0 &&
        uEntireTokenAttribute.compare("") == 0) {
        // What is "|" mean
        string sign_ = uEntireToken;
        TheEntireToken();
        TableInfo Temp;
        Maybe_bit_ex_OR_exp(Temp);
        // need do some here !!!
    } // while

} // Rest_of_maybe_bit_OR_exp()

void Maybe_bit_OR_exp(TableInfo& ReturnValue) {
    // Question !!!!!!!
    Maybe_bit_ex_OR_exp(ReturnValue);

    while (uEntireToken.compare("|") == 0 &&
        uEntireTokenAttribute.compare("") == 0) {
        // What is "|" mean
        string sign_ = uEntireToken;
        TheEntireToken();
        TableInfo Temp;
        Maybe_bit_ex_OR_exp(Temp);
        // need do some here !!!
    } // while

} // Maybe_bit_OR_exp()

void Rest_of_maybe_logical_AND_exp(TableInfo& ReturnValue) {

    Rest_of_maybe_bit_OR_exp(ReturnValue);

    while (uEntireToken.compare("&&") == 0 &&
        uEntireTokenAttribute.compare("") == 0) {
        uDoNotCompare = false;
        TheEntireToken();
        TableInfo Temp;
        Maybe_bit_OR_exp(Temp);
        ReturnValue.value = ReturnValue.value && Temp.value;
    } // while

} // Rest_of_maybe_logical_AND_exp()

void Maybe_logical_AND_exp(TableInfo& ReturnValue) {

    Maybe_bit_OR_exp(ReturnValue);

    while (uEntireToken.compare("&&") == 0 &&
        uEntireTokenAttribute.compare("") == 0) {
        uDoNotCompare = false;
        TheEntireToken();
        TableInfo Temp;
        Maybe_bit_OR_exp(Temp);
        ReturnValue.value = ReturnValue.value && Temp.value;
    } // while

} // Maybe_logical_AND_exp()

void Rest_of_maybe_logical_OR_exp(TableInfo& ReturnValue) {

    Rest_of_maybe_logical_AND_exp(ReturnValue);

    while (uEntireToken.compare("||") == 0 &&
        uEntireTokenAttribute.compare("") == 0) {
        uDoNotCompare = false;
        TheEntireToken();
        TableInfo Temp;
        Maybe_logical_AND_exp(Temp);
        ReturnValue.value = ReturnValue.value || Temp.value;
    } // while

} // Rest_of_maybe_logical_OR_exp()

void Romce_and_romloe(TableInfo& ReturnValue) {

    Rest_of_maybe_logical_OR_exp(ReturnValue);
    stringstream StringStream;
    string LineNumber;
    if (uEntireToken.compare("?") == 0 &&
        uEntireTokenAttribute.compare("") == 0) {
        int before_left_expression_size = gState.size();
        int before_left_expression_cout = gCoutToken.size();
        // use to true, false pop
        TheEntireToken();
        if ((IDENT() || uEntireTokenAttribute.compare("Reserved") == 0) ||
            ((PP(uEntireToken) || MM(uEntireToken)) &&
                uEntireTokenAttribute.compare("") == 0) ||
            Sign(uEntireToken) ||
            uEntireTokenAttribute.compare("Constant") == 0 ||
            (uEntireToken.compare("(") == 0 &&
                uEntireTokenAttribute.compare("") == 0)) {
            TableInfo temp1;
            BasicExpression(temp1);
            if (uEntireToken.compare(":") == 0 &&
                uEntireTokenAttribute.compare("") == 0) {
                if (ReturnValue.value == 0) {
                    // false, pop after_left_expression_size

                    int i = gState.size() - before_left_expression_size;
                    for (; i > 0; i--)
                        gState.pop_back();

                    int j = gCoutToken.size() - before_left_expression_cout;
                    for (; j > 0; j--)
                        gCoutToken.pop_back();

                } // if

                int before_right_expression_size = gState.size();
                TheEntireToken();
                if ((IDENT() || uEntireTokenAttribute.compare("Reserved") == 0) ||
                    ((PP(uEntireToken) || MM(uEntireToken)) &&
                        uEntireTokenAttribute.compare("") == 0) ||
                    Sign(uEntireToken) ||
                    uEntireTokenAttribute.compare("Constant") == 0 ||
                    (uEntireToken.compare("(") == 0 &&
                        uEntireTokenAttribute.compare("") == 0)) {
                    TableInfo temp2;
                    BasicExpression(temp2);
                    if (ReturnValue.value != 0) {
                        // true, pop after_right_expression_size

                        int i = gState.size() - before_right_expression_size;
                        for (; i > 0; i--)
                            gState.pop_back();

                        int j = gCoutToken.size() - before_left_expression_cout;
                        for (; j > 0; j--)
                            gCoutToken.pop_back();
                    } // if

                    if (ReturnValue.value != 0)
                        ReturnValue = temp1;
                    else
                        ReturnValue = temp2;

                    uDoNotCompare = true;
                } // if
                else {
                    StringStream << uLineCounter;
                    StringStream >> LineNumber;
                    string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
                    throw new ExceptionUnexpected(msg);
                } // else
            } // if
            else {
                StringStream << uLineCounter;
                StringStream >> LineNumber;
                string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
                throw new ExceptionUnexpected(msg);
            } // else
        } // if
        else {
            StringStream << uLineCounter;
            StringStream >> LineNumber;
            string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
            throw new ExceptionUnexpected(msg);
        } // else
    } // if
} // Romce_and_romloe()

void Rest_of_Identifier_started_basic_exp(TableInfo& ReturnValue) {
    stringstream StringStream;
    string LineNumber;
    if (uEntireToken.compare("(") == 0 &&
        uEntireTokenAttribute.compare("") == 0) {
        TheEntireToken();
        if (uIsDoneID) {
            if (uEntireToken.compare(")") == 0 &&
                uEntireTokenAttribute.compare("") == 0) {
                TheEntireToken();
                if (uEntireToken.compare(";") == 0 &&
                    uEntireTokenAttribute.compare("") == 0) {
                    ;
                } // if
                else {
                    uIsDoneID = false;
                    StringStream << uLineCounter;
                    StringStream >> LineNumber;
                    string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
                    throw new ExceptionUnexpected(msg);
                } // else
            } // if
            else {
                uIsDoneID = false;
                StringStream << uLineCounter;
                StringStream >> LineNumber;
                string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
                throw new ExceptionUnexpected(msg);
            } // else
        } // if
        else if (uIsListAllVariables) {
            if (uEntireToken.compare(")") == 0 &&
                uEntireTokenAttribute.compare("") == 0) {
                TheEntireToken();
                if (uEntireToken.compare(";") == 0 &&
                    uEntireTokenAttribute.compare("") == 0) {
                    ;
                } // if
                else {
                    uIsListAllVariables = false;
                    StringStream << uLineCounter;
                    StringStream >> LineNumber;
                    string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
                    throw new ExceptionUnexpected(msg);
                } // else
            } // if
            else {
                uIsListAllVariables = false;
                StringStream << uLineCounter;
                StringStream >> LineNumber;
                string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
                throw new ExceptionUnexpected(msg);
            } // else
        } // else if
        else if (uIsListAllFunctions) {
            if (uEntireToken.compare(")") == 0 &&
                uEntireTokenAttribute.compare("") == 0) {
                TheEntireToken();
                if (uEntireToken.compare(";") == 0 &&
                    uEntireTokenAttribute.compare("") == 0) {
                    ;
                } // if
                else {
                    uIsListAllFunctions = false;
                    StringStream << uLineCounter;
                    StringStream >> LineNumber;
                    string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
                    throw new ExceptionUnexpected(msg);
                } // else
            } // if
            else {
                uIsListAllFunctions = false;
                StringStream << uLineCounter;
                StringStream >> LineNumber;
                string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
                throw new ExceptionUnexpected(msg);
            } // else
        } // else if
        else {
            if (uEntireToken.compare(")") == 0 &&
                uEntireTokenAttribute.compare("") == 0) {
                // function call without parameter

                if (uDoFunctionCalculation) {
                    if (ReturnValue.Func) {
                        // function Name is ReturnValue.Name
                        // using ReturnValue.TokenInfoList to run again

                        int current_state_pos = gState.size();
                        int current_func_pos = gFunctionToken.size();
                        string current_token_name = uEntireToken;
                        string current_token_type = uEntireTokenType;
                        string current_token_attribute = uEntireTokenAttribute;
                        uWalker = current_func_pos;

                        // push back function token
                        // use these token to walk again
                        // syntax analysis for evaluate

                        for (int i = 0; i < ReturnValue.TokenInfoList.size(); i++)
                            gFunctionToken.push_back(ReturnValue.TokenInfoList[i]);

                        uEntireToken =
                            gFunctionToken[uWalker].Name;
                        uEntireTokenAttribute =
                            gFunctionToken[uWalker].Attribute;
                        uEntireTokenType =
                            gFunctionToken[uWalker].Type;

                        Definition();
                        //cout << "come out" << endl;

                        // restore initial state
                        for (int i = gFunctionToken.size() - current_func_pos; i > 0; i--)
                            gFunctionToken.pop_back();

                        uWalker = current_func_pos - 1;
                        uEntireToken = current_token_name;
                        uEntireTokenAttribute = current_token_attribute;
                        uEntireTokenType = current_token_type;

                        // if statement is FunctionParameter
                        // ( but if mainthen not erase )
                        // and it's call by value
                        // erase that statement
                        // else not

                        if (ReturnValue.Name.compare("main") != 0) {
                            vector<TableInfo>::iterator it = gState.begin() + current_state_pos;
                            for (int i = gState.size() - current_state_pos; i > 0; i--) {
                                // erase located variable
                                if (it->FunctionParameter) {
                                    gState.erase(it);
                                } // if
                                else {
                                    if (!it->GlobalVariable) {
                                        gState.erase(it);
                                    } // if
                                    else {
                                        it++;
                                    } // else
                                } // else
                            } // for

                            if (ReturnValue.Type.compare("int") == 0 ||
                                ReturnValue.Type.compare("char") == 0 ||
                                ReturnValue.Type.compare("float") == 0 ||
                                ReturnValue.Type.compare("string") == 0 ||
                                ReturnValue.Type.compare("bool") == 0) {
                                ReturnValue = gReturnValue[gReturnValue.size() - 1];
                            } // if
                        } // if
                        else {
                            cout << "is main" << endl;
                        } // else
                    } // if
                    else {
                        cout << "not a function error" << endl;
                        exit(0);
                    } // else
                } // if

                TheEntireToken(); // ex: int x = a() ;
                Romce_and_romloe(ReturnValue);
            } // if
            else if ((IDENT() || uEntireTokenAttribute.compare("Reserved") == 0) ||
                ((PP(uEntireToken) || MM(uEntireToken)) &&
                    uEntireTokenAttribute.compare("") == 0) ||
                Sign(uEntireToken) ||
                uEntireTokenAttribute.compare("Constant") == 0 ||
                (uEntireToken.compare("(") == 0 &&
                    uEntireTokenAttribute.compare("") == 0)) {
                // function call with parameter
                TableInfo Temp;
                Actual_parameter_list(Temp);
                if (uEntireToken.compare(")") == 0 &&
                    uEntireTokenAttribute.compare("") == 0) {
                    TheEntireToken();
                    if (uIsListVariable) {
                        if (uEntireToken.compare(";") == 0 &&
                            uEntireTokenAttribute.compare("") == 0) {
                            uListVariableName = Temp.ValueString;
                        } // if
                        else {
                            uIsListVariable = false;
                            StringStream << uLineCounter;
                            StringStream >> LineNumber;
                            string msg = "> Line " + LineNumber + " : unexpected token " + "'"
                                + uConstantName + "'";
                            throw new ExceptionUnexpected(msg);
                        } // else
                    } // if
                    else if (uIsListFunction) {
                        if (uEntireToken.compare(";") == 0 &&
                            uEntireTokenAttribute.compare("") == 0) {
                            uListFunctionName = Temp.ValueString;
                        } // if
                        else {
                            uIsListFunction = false;
                            StringStream << uLineCounter;
                            StringStream >> LineNumber;
                            string msg = "> Line " + LineNumber + " : unexpected token "
                                + "'" + uConstantName + "'";
                            throw new ExceptionUnexpected(msg);
                        } // else
                    } // else if
                    else {
                        if (uDoFunctionCalculation) {
                            if (ReturnValue.Func) {
                                // function Name is ReturnValue.Name
                                // parameter is Temp.ValueString
                                // using ReturnValue.TokenInfoList to run again
                                int current_state_pos = gState.size();
                                int current_func_pos = gFunctionToken.size();
                                string current_token_name = uEntireToken;
                                string current_token_type = uEntireTokenType;
                                string current_token_attribute = uEntireTokenAttribute;
                                uWalker = current_func_pos;

                                // push back function token
                                // use these token to walk again
                                // syntax analysis for evaluate
                                for (int i = 0; i < ReturnValue.TokenInfoList.size(); i++)
                                    gFunctionToken.push_back(ReturnValue.TokenInfoList[i]);

                                Temp.FunctionParameter = true;
                                Temp.ParameterSequenceCounter = 1;
                                gParameter.push_back(Temp);

                                uEntireToken =
                                    gFunctionToken[uWalker].Name;
                                uEntireTokenAttribute =
                                    gFunctionToken[uWalker].Attribute;
                                uEntireTokenType =
                                    gFunctionToken[uWalker].Type;

                                Definition();

                                // restore initial state
                                for (int i = gFunctionToken.size() - current_func_pos; i > 0; i--)
                                    gFunctionToken.pop_back();

                                uWalker = current_func_pos - 1;
                                uEntireToken = current_token_name;
                                uEntireTokenAttribute = current_token_attribute;
                                uEntireTokenType = current_token_type;

                                // if statement is FunctionParameter
                                // and it's call by value
                                // erase that statement
                                // else not

                                vector<TableInfo>::iterator it = gState.begin() + current_state_pos;
                                for (int i = gState.size() - current_state_pos; i > 0; i--) {
                                    // erase located variable
                                    if (it->FunctionParameter) {
                                        gState.erase(it);
                                    } // if
                                    else {
                                        if (!it->GlobalVariable) {
                                            gState.erase(it);
                                        } // if
                                        else {
                                            it++;
                                        } // else
                                    } // else
                                } // for

                                if (ReturnValue.Type.compare("int") == 0 ||
                                    ReturnValue.Type.compare("char") == 0 ||
                                    ReturnValue.Type.compare("float") == 0 ||
                                    ReturnValue.Type.compare("string") == 0 ||
                                    ReturnValue.Type.compare("bool") == 0) {
                                    ReturnValue = gReturnValue[gReturnValue.size() - 1];
                                } // if
                            } // if
                            else {
                                cout << "not a function error" << endl;
                                exit(0);
                            } // else
                        } // if

                        Romce_and_romloe(ReturnValue);
                    } // else
                } // if
                else { // unexpected ')'
                    StringStream << uLineCounter;
                    StringStream >> LineNumber;
                    string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
                    throw new ExceptionUnexpected(msg);
                } // else
            } // else if
            else {
                StringStream << uLineCounter;
                StringStream >> LineNumber;
                string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
                throw new ExceptionUnexpected(msg);
            } // else
        } // else
    } // if
    else {
        // x[10] = 10, x = 10
        // we can use ReturnValue.ArraySize
        // to know which index
        if (uEntireToken.compare("[") == 0 &&
            uEntireTokenAttribute.compare("") == 0) {
            if (ReturnValue.Ptr != NULL) {
                TheEntireToken();
                if ((IDENT() || uEntireTokenAttribute.compare("Reserved") == 0) ||
                    ((PP(uEntireToken) || MM(uEntireToken)) &&
                        uEntireTokenAttribute.compare("") == 0) ||
                    Sign(uEntireToken) ||
                    uEntireTokenAttribute.compare("Constant") == 0 ||
                    (uEntireToken.compare("(") == 0 &&
                        uEntireTokenAttribute.compare("") == 0)) {
                    TableInfo Temp;
                    Expression(Temp);
                    if (Temp.Type.compare("int") == 0) {
                        if ((int)Temp.value < ReturnValue.ArraySize &&
                            (int)Temp.value >= 0) {
                            if (uEntireToken.compare("]") == 0 &&
                                uEntireTokenAttribute.compare("") == 0) {
                                int value = (int)Temp.value;
                                int locate_at_state = StateArrayPosition(ReturnValue.Name, value);
                                if (locate_at_state >= 0) {
                                    ReturnValue.ArraySize = (int)Temp.value;
                                    ReturnValue.value =
                                        gState[locate_at_state].value;
                                    ReturnValue.ValueString =
                                        gState[locate_at_state].ValueString;
                                    TheEntireToken();
                                } // if
                                else {
                                    ReturnValue.ArraySize = (int)Temp.value;
                                    ReturnValue.value = atof(ReturnValue.Ptr[(int)Temp.value].c_str());
                                    ReturnValue.ValueString = ReturnValue.Ptr[(int)Temp.value];
                                    TheEntireToken();
                                } // else
                            } // if
                            else {
                                StringStream << uLineCounter;
                                StringStream >> LineNumber;
                                string msg = "> Line " + LineNumber + " : unexpected token "
                                    + "'" + uConstantName + "'";
                                throw new ExceptionUnexpected(msg);
                            } // else
                        } // if
                        else {
                            // base on walk once, so every step needs to evaluate
                            // but some condition is false, but program always evaluate
                            // so may happen access out of range index
                            // ex: while( i < 10 ) a[i] = 10;
                            // when i = 10, happen access out of range

                            // Solution: if condition is false, walk else( the following )
                            // only pass by

                            if (gCondition.size() != 0) {
                                if (!gCondition[gCondition.size() - 1].ConditionResult)
                                    TheEntireToken();
                            } // if
                            else {
                                cout << "index out of range at Rest_of_Identifier_started_basic_exp" << endl;
                                exit(0);
                            } // else
                        } // else
                    } // if
                    else {
                        StringStream << uLineCounter;
                        StringStream >> LineNumber;
                        string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
                        throw new ExceptionUnexpected(msg);
                    } // else
                } // if
                else {
                    StringStream << uLineCounter;
                    StringStream >> LineNumber;
                    string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
                    throw new ExceptionUnexpected(msg);
                } // else
            } // if
            else { // int a; a[ , unexpected
                StringStream << uLineCounter;
                StringStream >> LineNumber;
                string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
                throw new ExceptionUnexpected(msg);
            } // else
        } // if

        if (AssignmentOperator(uEntireToken)) {
            string operator_ = uEntireToken;
            TheEntireToken();
            if ((IDENT() || uEntireTokenAttribute.compare("Reserved") == 0) ||
                ((PP(uEntireToken) || MM(uEntireToken)) &&
                    uEntireTokenAttribute.compare("") == 0) ||
                Sign(uEntireToken) ||
                uEntireTokenAttribute.compare("Constant") == 0 ||
                (uEntireToken.compare("(") == 0 &&
                    uEntireTokenAttribute.compare("") == 0)) {
                TableInfo right_exp;
                BasicExpression(right_exp);
                DoAssignmentOperator(operator_, ReturnValue, right_exp);
                gState.push_back(ReturnValue);
            } // if
            else {
                StringStream << uLineCounter;
                StringStream >> LineNumber;
                string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
                throw new ExceptionUnexpected(msg);
            } // else
        } // if
        else {
            string sign_;
            if ((PP(uEntireToken) || MM(uEntireToken)) &&
                uEntireTokenAttribute.compare("") == 0) {
                sign_ = uEntireToken;
                TheEntireToken();
            } // if

            TableInfo Temp;
            Temp = ReturnValue;
            if (sign_.compare("++") == 0) {
                Temp.value++;
                gState.push_back(Temp);
            } // if
            else if (sign_.compare("--") == 0) {
                Temp.value--;
                gState.push_back(Temp);
            } // else if

            Romce_and_romloe(ReturnValue);
        } // else
    } // else
} // Rest_of_Identifier_started_basic_exp()

void Rest_of_PPMM_Identifier_started_basic_exp(TableInfo& ReturnValue) {
    stringstream StringStream;
    string LineNumber;
    if (uEntireToken.compare("[") == 0 &&
        uEntireTokenAttribute.compare("") == 0) {
        TheEntireToken();
        if ((IDENT() || uEntireTokenAttribute.compare("Reserved") == 0) ||
            ((PP(uEntireToken) || MM(uEntireToken)) &&
                uEntireTokenAttribute.compare("") == 0) ||
            Sign(uEntireToken) ||
            uEntireTokenAttribute.compare("Constant") == 0 ||
            (uEntireToken.compare("(") == 0 &&
                uEntireTokenAttribute.compare("") == 0)) {
            TableInfo Temp;
            Expression(Temp);
            if (uEntireToken.compare("]") == 0 &&
                uEntireTokenAttribute.compare("") == 0) {
                TheEntireToken();
            } // if
            else {
                StringStream << uLineCounter;
                StringStream >> LineNumber;
                string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
                throw new ExceptionUnexpected(msg);
            } // else
        } // if
        else {
            StringStream << uLineCounter;
            StringStream >> LineNumber;
            string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
            throw new ExceptionUnexpected(msg);
        } // else
    } // if

    Romce_and_romloe(ReturnValue);

} // Rest_of_PPMM_Identifier_started_basic_exp()

void BasicExpression(TableInfo& ReturnValue) {
    stringstream StringStream;
    string LineNumber;
    if (IDENT() || uEntireTokenAttribute.compare("Reserved") == 0) {
        if (uEntireTokenAttribute.compare("Reserved") == 0) {
            int temp_line = uLineCounter;
            string reserved_word;
            if (FunctionReservedWord(uEntireToken)) {
                reserved_word = uEntireToken;
                if (uEntireToken.compare("Done") == 0) // uEntireToken is "Done"
                    uIsDoneID = true;
                else if (uEntireToken.compare("ListAllVariables") == 0)
                    uIsListAllVariables = true;
                else if (uEntireToken.compare("ListVariable") == 0)
                    uIsListVariable = true;
                else if (uEntireToken.compare("ListAllFunctions") == 0)
                    uIsListAllFunctions = true;
                else if (uEntireToken.compare("ListFunction") == 0)
                    uIsListFunction = true;

                TheEntireToken();
                if (uEntireToken.compare("(") == 0 &&
                    uEntireTokenAttribute.compare("") == 0) {
                    Rest_of_Identifier_started_basic_exp(ReturnValue);
                } // if
                else {
                    StringStream << temp_line;
                    StringStream >> LineNumber;
                    string msg = "> Line " + LineNumber + " : undefined identifier " + "'" + reserved_word + "'";
                    if (temp_line != uLineCounter) // Done +
                        uTook = true;

                    throw new ExceptionUndefined(msg);
                } // else
            } // if
            else if (uEntireToken.compare("cout") == 0) {
                reserved_word = uEntireToken;
                uCout = true;
                TheEntireToken();
                if (uEntireToken.compare("<<") == 0 &&
                    uEntireTokenAttribute.compare("") == 0) {
                    Rest_of_Identifier_started_basic_exp(ReturnValue);
                } // if
                else {
                    StringStream << temp_line;
                    StringStream >> LineNumber;
                    string msg = "> Line " + LineNumber + " : undefined identifier " + "'" + reserved_word + "'";
                    if (temp_line != uLineCounter) // Done +
                        uTook = true;

                    throw new ExceptionUndefined(msg);
                } // else
            } // else if
            else if (uEntireToken.compare("cin") == 0) {
                reserved_word = uEntireToken;
                uCin = true;
                TheEntireToken();
                if (uEntireToken.compare(">>") == 0 &&
                    uEntireTokenAttribute.compare("") == 0) {
                    Rest_of_Identifier_started_basic_exp(ReturnValue);
                } // if
                else {
                    StringStream << temp_line;
                    StringStream >> LineNumber;
                    string msg = "> Line " + LineNumber + " : undefined identifier " + "'" + reserved_word + "'";
                    if (temp_line != uLineCounter) // Done +
                        uTook = true;

                    throw new ExceptionUndefined(msg);
                } // else
            } // else if
            else {
                StringStream << uLineCounter;
                StringStream >> LineNumber;
                string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
                throw new ExceptionUnexpected(msg);
            } // else
        } // if  Reserved_Word
        else {
            if (IdentifierExist(uEntireToken)) {
                int line_temp = uLineCounter;
                string reserved_word = uEntireToken;
                if (uGlobalPosition != -1 && uLocalPosition == -1) {
                    // global variable
                    ReturnValue = gIdentifierTable[uGlobalPosition];
                } // if
                else if (uGlobalPosition == -1 && uLocalPosition != -1) {
                    // local variable
                    ReturnValue = gDef[uLocalPosition];
                } // if
                else {
                    cout << "Didn't exist( It should be catch )" << endl;
                    exit(0);
                } // else

                int latest_value = StatePosition(ReturnValue.Name);
                if (latest_value >= 0) { // latest value at gState
                    ReturnValue.value = gState[latest_value].value;
                    ReturnValue.ValueString = gState[latest_value].ValueString;
                    ReturnValue.Ptr = gState[latest_value].Ptr;
                } // if

                TheEntireToken();
                if (ReturnValue.Func) { // is function
                    if (uEntireToken.compare("(") != 0) {
                        StringStream << line_temp;
                        StringStream >> LineNumber;
                        string msg = "> Line " + LineNumber + " : undefined identifier " + "'" + reserved_word + "'";
                        if (line_temp != uLineCounter) // Done +
                            uTook = true;

                        throw new ExceptionUndefined(msg);
                    } // if
                } // if
                else { // is variable
                    if (uEntireToken.compare("(") == 0) {
                        StringStream << line_temp;
                        StringStream >> LineNumber;
                        string msg = "> Line " + LineNumber + " : undefined identifier " + "'" + reserved_word + "'";
                        if (line_temp != uLineCounter) // Done +
                            uTook = true;

                        throw new ExceptionUndefined(msg);
                    } // if
                } // else

                Rest_of_Identifier_started_basic_exp(ReturnValue);
            } // if
            else {
                StringStream << uLineCounter;
                StringStream >> LineNumber;
                string msg = "> Line " + LineNumber + " : undefined identifier " + "'" + uEntireToken + "'";
                throw new ExceptionUndefined(msg);
            } // else
        } // else
    } // if
    else if ((PP(uEntireToken) || MM(uEntireToken)) &&
        uEntireTokenAttribute.compare("") == 0) {  // PP | MM
        string sign_ = uEntireToken;
        TheEntireToken();
        if (IDENT()) {
            if (IdentifierExist(uEntireToken)) {
                bool is_global_id;
                int id_located;
                int line_temp = uLineCounter;
                string reserved_word = uEntireToken;
                bool current_exp_is_func;
                if (uGlobalPosition != -1 && uLocalPosition == -1) {
                    current_exp_is_func = gIdentifierTable[uGlobalPosition].Func;
                    ReturnValue = gIdentifierTable[uGlobalPosition];
                } // if
                else if (uGlobalPosition == -1 && uLocalPosition != -1) {
                    current_exp_is_func = gDef[uLocalPosition].Func;
                    ReturnValue = gDef[uLocalPosition];
                } // if
                else {
                    cout << "Didn't exist( It should be catch )" << endl;
                    exit(0);
                } // else

                int latest_value = StatePosition(ReturnValue.Name);
                if (latest_value >= 0) { // latest value at gState
                    ReturnValue.value = gState[latest_value].value;
                    ReturnValue.ValueString = gState[latest_value].ValueString;
                    ReturnValue.Ptr = gState[latest_value].Ptr;
                } // if

                TheEntireToken();
                if (current_exp_is_func) { // is function
                    if (uEntireToken.compare("(") != 0) {
                        StringStream << line_temp;
                        StringStream >> LineNumber;
                        string msg = "> Line " + LineNumber + " : undefined identifier " + "'" + reserved_word + "'";
                        if (line_temp != uLineCounter) // Done +
                            uTook = true;

                        throw new ExceptionUndefined(msg);
                    } // if
                } // if
                else { // is variable
                    if (uEntireToken.compare("(") == 0) {
                        StringStream << line_temp;
                        StringStream >> LineNumber;
                        string msg = "> Line " + LineNumber + " : undefined identifier " + "'" + reserved_word + "'";
                        if (line_temp != uLineCounter) // Done +
                            uTook = true;

                        throw new ExceptionUndefined(msg);
                    } // if
                } // else

                if (sign_.compare("++") == 0)
                    ++ReturnValue.value;
                else
                    --ReturnValue.value;

                gState.push_back(ReturnValue);
                Rest_of_PPMM_Identifier_started_basic_exp(ReturnValue);
            } // if
            else {
                StringStream << uLineCounter;
                StringStream >> LineNumber;
                string msg = "> Line " + LineNumber + " : undefined identifier " + "'" + uEntireToken + "'";
                throw new ExceptionUndefined(msg);
            } // else
        } // if
        else {
            StringStream << uLineCounter;
            StringStream >> LineNumber;
            string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
            throw new ExceptionUnexpected(msg);
        } // else
    } // else if
    else if (Sign(uEntireToken)) {  // Sign represent it's value is positive or negative
        vector<string> sign_;
        sign_.push_back(uEntireToken);
        TheEntireToken();
        while (Sign(uEntireToken)) {
            sign_.push_back(uEntireToken);
            TheEntireToken();
        } // while

        Signed_unary_exp(ReturnValue);
        for (int i = sign_.size() - 1; i >= 0; i--) {
            if (sign_[i].compare("+") == 0)
                ReturnValue.value = +ReturnValue.value;
            else if (sign_[i].compare("-") == 0)
                ReturnValue.value = -ReturnValue.value;
            else if (sign_[i].compare("!") == 0)
                ReturnValue.value = !ReturnValue.value;
        } // for

        Romce_and_romloe(ReturnValue);
    } // else if
    else {
        if (uEntireTokenAttribute.compare("Constant") == 0) {
            // 3 types: 112 11.2, "123", true false
            ReturnValue.Func = false;
            ReturnValue.Type = uEntireTokenType;
            ReturnValue.Attribute = "Constant";
            if (uEntireTokenType.compare("int") == 0 ||
                uEntireTokenType.compare("float") == 0)
                ReturnValue.value = atof(uEntireToken.c_str());
            else if (uEntireTokenType.compare("bool") == 0) {
                if (uEntireToken.compare("false") == 0)
                    ReturnValue.value = 0;
                else
                    ReturnValue.value = 1;
            } // else if
            else if (uEntireTokenType.compare("string") == 0 ||
                uEntireTokenType.compare("char") == 0)
                ReturnValue.ValueString = uEntireToken;

            TheEntireToken();
        } // if
        else {
            // ( expression )
            TheEntireToken();
            if ((IDENT() || uEntireTokenAttribute.compare("Reserved") == 0) ||
                ((PP(uEntireToken) || MM(uEntireToken)) &&
                    uEntireTokenAttribute.compare("") == 0) ||
                Sign(uEntireToken) ||
                uEntireTokenAttribute.compare("Constant") == 0 ||
                (uEntireToken.compare("(") == 0 &&
                    uEntireTokenAttribute.compare("") == 0)) {
                Expression(ReturnValue);
                if (uEntireToken.compare(")") == 0 &&
                    uEntireTokenAttribute.compare("") == 0)
                    TheEntireToken();
                else {
                    StringStream << uLineCounter;
                    StringStream >> LineNumber;
                    string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
                    throw new ExceptionUnexpected(msg);
                } // else
            } // if
            else {
                StringStream << uLineCounter;
                StringStream >> LineNumber;
                string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
                throw new ExceptionUnexpected(msg);
            } // else
        } // else

        Romce_and_romloe(ReturnValue);
    } // else
} // BasicExpression()

void Expression(TableInfo& ReturnValue) {

    BasicExpression(ReturnValue);
    while (uEntireToken.compare(",") == 0 &&
        uEntireTokenAttribute.compare("") == 0) {
        TheEntireToken();
        BasicExpression(ReturnValue);
    } // while
} // Expression()

void Statement() {
    TableInfo ReturnValue;
    stringstream StringStream;
    string LineNumber;
    if (uEntireToken.compare(";") == 0 &&
        uEntireTokenAttribute.compare("") == 0) // ex: ;
        ;
    else if (uEntireToken.compare("return") == 0 &&
        uEntireTokenAttribute.compare("Reserved") == 0) {
        uDoNotCompare = true;
        TheEntireToken();
        if (uEntireToken.compare(";") == 0 &&
            uEntireTokenAttribute.compare("") == 0)
            ;
        else {
            if ((IDENT() || uEntireTokenAttribute.compare("Reserved") == 0) ||
                ((PP(uEntireToken) || MM(uEntireToken)) &&
                    uEntireTokenAttribute.compare("") == 0) ||
                Sign(uEntireToken) ||
                uEntireTokenAttribute.compare("Constant") == 0 ||
                (uEntireToken.compare("(") == 0 &&
                    uEntireTokenAttribute.compare("") == 0)) {
                Expression(ReturnValue);
                if (uEntireToken.compare(";") == 0 &&
                    uEntireTokenAttribute.compare("") == 0) {
                    if (gCondition.size() == 0) {
                        gReturnValue.push_back(ReturnValue);
                    } // if
                    else {
                        if (gCondition[gCondition.size() - 1].ConditionResult &&
                            !gCondition[gCondition.size() - 1].ConditionComplete) {
                            gReturnValue.push_back(ReturnValue);
                        } // if
                    } // else
                } // if
            } // if
            else {
                StringStream << uLineCounter;
                StringStream >> LineNumber;
                string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
                throw new ExceptionUnexpected(msg);
            } // else
        } // else
    } // else if
    else if (uEntireToken.compare("if") == 0 &&
        uEntireTokenAttribute.compare("Reserved") == 0) {
        TheEntireToken();
        if (uEntireToken.compare("(") == 0 &&
            uEntireTokenAttribute.compare("") == 0) {
            TheEntireToken();
            if ((IDENT() || uEntireTokenAttribute.compare("Reserved") == 0) ||
                ((PP(uEntireToken) || MM(uEntireToken)) &&
                    uEntireTokenAttribute.compare("") == 0) ||
                Sign(uEntireToken) ||
                uEntireTokenAttribute.compare("Constant") == 0 ||
                (uEntireToken.compare("(") == 0 &&
                    uEntireTokenAttribute.compare("") == 0)) {
                Expression(ReturnValue); // necessary
                if (uEntireToken.compare(")") == 0 &&
                    uEntireTokenAttribute.compare("") == 0) {
                    // gFunctionToken.push_back( "#" ) ; // flag, use to standard output, where to \n

                    ConditionInfo temp_if;
                    temp_if.Layer = uLayer;

                    // handle else if
                    if (gCondition.size() != 0) {
                        // if ( gCondition[gCondition.size()-1].condition_type.compare( "else" ) == 0 ) {

                        if (!gCondition[gCondition.size() - 1].ConditionComplete) {
                            if (gCondition[gCondition.size() - 1].ConditionResult) {
                                if (ReturnValue.value != 0) {
                                    temp_if.ConditionResult = true;
                                    temp_if.ConditionComplete = false;
                                } // if
                                else {
                                    temp_if.ConditionResult = false;
                                    temp_if.ConditionComplete = false;
                                } // else
                            } // if
                            else {
                                temp_if.ConditionResult = false;
                                temp_if.ConditionComplete = true;
                            } // else
                        } // if
                        else {
                            temp_if.ConditionResult = false;
                            temp_if.ConditionComplete = true;
                        } // else
                    } // if
                    else {
                        if (ReturnValue.value != 0) {
                            temp_if.ConditionResult = true;
                            temp_if.ConditionComplete = false;
                        } // if
                        else {
                            temp_if.ConditionResult = false;
                            temp_if.ConditionComplete = false;
                        } // else
                    } // else

                    int current_state_pos = gState.size();
                    int current_cout_pos = gCoutToken.size();

                    gCondition.push_back(temp_if);

                    uLayer++;
                    TheEntireToken();

                    bool cout_state = uCout;
                    uCout = false;
                    Statement();
                    uCout = cout_state;

                    gCondition.pop_back();
                    if (temp_if.ConditionComplete || !temp_if.ConditionResult) {
                        for (int i = gState.size() - current_state_pos; i > 0; i--) {
                            gState.pop_back();
                        } // for

                        for (int i = gCoutToken.size() - current_cout_pos; i > 0; i--) {
                            gCoutToken.pop_back();
                        } // for

                    } // if
                    else {
                        vector<TableInfo>::iterator it = gState.begin() + current_state_pos;
                        for (int i = gState.size() - current_state_pos; i > 0; i--) {
                            // erase located variable
                            if (it->Layer == uLayer)
                                gState.erase(it);
                            else
                                it++;
                        } // for
                    } // else

                    uLayer--;

                    TheEntireToken();

                    if (uEntireToken.compare("else") == 0 &&
                        uEntireTokenAttribute.compare("Reserved") == 0) {
                        // gFunctionToken.push_back( "#" ) ; // flag, use to standard output, where to \n
                        ConditionInfo temp_else;

                        temp_else.Layer = uLayer;

                        if (temp_if.ConditionComplete) {
                            temp_else.ConditionResult = false;
                            temp_else.ConditionComplete = true;
                        } // if
                        else {
                            if (temp_if.ConditionResult) {
                                temp_else.ConditionResult = false;
                                temp_else.ConditionComplete = true;
                            } // if
                            else {
                                temp_else.ConditionResult = true;
                                temp_else.ConditionComplete = false;
                            } // else
                        } // else

                        int current_state_pos_else = gState.size();
                        int current_cout_pos_else = gCoutToken.size();

                        gCondition.push_back(temp_else);

                        uLayer++;
                        TheEntireToken();

                        bool cout_state = uCout;
                        uCout = false;
                        Statement();
                        uCout = cout_state;

                        gCondition.pop_back();

                        if (temp_else.ConditionComplete || !temp_else.ConditionResult) {
                            for (int i = gState.size() - current_state_pos_else; i > 0; i--) {
                                gState.pop_back();
                            } // for

                            for (int i = gCoutToken.size() - current_cout_pos_else; i > 0; i--) {
                                gCoutToken.pop_back();
                            } // for
                        } // if
                        else {
                            vector<TableInfo>::iterator it = gState.begin() + current_state_pos_else;
                            for (int i = gState.size() - current_state_pos_else; i > 0; i--) {
                                // erase located variable
                                if (it->Layer == uLayer)
                                    gState.erase(it);
                                else
                                    it++;
                            } // for
                        } // else

                        uLayer--;
                    } // if
                    else {
                        uTokenTook = true;
                    } // else
                } // if
                else {  // Unexpected token ')'
                    StringStream << uLineCounter;
                    StringStream >> LineNumber;
                    string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
                    throw new ExceptionUnexpected(msg);
                } // else
            } // if
            else {
                StringStream << uLineCounter;
                StringStream >> LineNumber;
                string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
                throw new ExceptionUnexpected(msg);
            } // else
        } // if
        else {  // Unexpected token '('
            StringStream << uLineCounter;
            StringStream >> LineNumber;
            string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
            throw new ExceptionUnexpected(msg);
        } // else
    } // else if
    else if (uEntireToken.compare("while") == 0 &&
        uEntireTokenAttribute.compare("Reserved") == 0) {
        // to decide whether doing again
        // Ex : if ( 1 < 0 ) {
        //        while ( true )
        //          a++ ;
        //      }
        // "if" condition is false, but "while" is true
        // it will never leave while loop

        ConditionInfo Temp;
        int while_begin_pos = uWalker;

        TheEntireToken();

        if (uEntireToken.compare("(") == 0 &&
            uEntireTokenAttribute.compare("") == 0) {
            TheEntireToken();
            if ((IDENT() || uEntireTokenAttribute.compare("Reserved") == 0) ||
                ((PP(uEntireToken) || MM(uEntireToken)) &&
                    uEntireTokenAttribute.compare("") == 0) ||
                Sign(uEntireToken) ||
                uEntireTokenAttribute.compare("Constant") == 0 ||
                (uEntireToken.compare("(") == 0 &&
                    uEntireTokenAttribute.compare("") == 0)) {
                Expression(ReturnValue); // necessary
                if (uEntireToken.compare(")") == 0 &&
                    uEntireTokenAttribute.compare("") == 0) {
                    // gFunctionToken.push_back( "#" ) ; // flag, use to standard output, where to \n

                    Temp.Layer = uLayer;

                    if (gCondition.size() == 0) {
                        if (ReturnValue.value != 0) {
                            Temp.ConditionResult = true;
                            Temp.ConditionComplete = false;
                        } // if
                        else {
                            Temp.ConditionResult = false;
                            Temp.ConditionComplete = false;
                        } // else
                    } // if
                    else {
                        if (gCondition[gCondition.size() - 1].ConditionComplete) {
                            Temp.ConditionResult = false;
                            Temp.ConditionComplete = true;
                        } // if
                        else {
                            if (gCondition[gCondition.size() - 1].ConditionResult) {
                                if (ReturnValue.value != 0) {
                                    Temp.ConditionResult = true;
                                    Temp.ConditionComplete = false;
                                } // if
                                else {
                                    Temp.ConditionResult = false;
                                    Temp.ConditionComplete = false;
                                } // else
                            } // if
                            else {
                                Temp.ConditionResult = false;
                                Temp.ConditionComplete = true;
                            } // else
                        } // else
                    } // else

                    int current_cout_pos = gCoutToken.size();
                    int current_state_pos = gState.size();

                    gCondition.push_back(Temp);

                    uLayer++;
                    TheEntireToken();

                    bool cout_state = uCout;
                    uCout = false;
                    Statement();
                    uCout = cout_state;

                    gCondition.pop_back();

                    if (Temp.ConditionResult && !Temp.ConditionComplete) {
                        // do again

                        vector<TableInfo>::iterator it = gState.begin() + current_state_pos;
                        for (int i = gState.size() - current_state_pos; i > 0; i--) {
                            // erase located variable
                            if (it->Layer == uLayer) {
                                gState.erase(it);
                            } // if
                            else {
                                it++;
                            } // else
                        } // for

                        uLayer--;

                        uWalker = while_begin_pos;
                        uEntireToken =
                            gFunctionToken[uWalker].Name;
                        uEntireTokenAttribute =
                            gFunctionToken[uWalker].Attribute;
                        uEntireTokenType =
                            gFunctionToken[uWalker].Type;
                        // call TheEntireToken() but not to get token

                        bool cout_state = uCout;
                        uCout = false;
                        Statement();
                        uCout = cout_state;
                    } // if
                    else {
                        // pop gState info

                        uLayer--;

                        for (int i = gState.size() - current_state_pos; i > 0; i--) {
                            gState.pop_back();
                        } // for

                        for (int i = gCoutToken.size() - current_cout_pos; i > 0; i--) {
                            gCoutToken.pop_back();
                        } // for
                    } // else
                } // if
                else {  // Unexpected ')'
                    StringStream << uLineCounter;
                    StringStream >> LineNumber;
                    string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
                    throw new ExceptionUnexpected(msg);
                } // else
            } // if
            else {
                StringStream << uLineCounter;
                StringStream >> LineNumber;
                string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
                throw new ExceptionUnexpected(msg);
            } // else
        } // if
        else {  // Unexpected '('
            StringStream << uLineCounter;
            StringStream >> LineNumber;
            string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
            throw new ExceptionUnexpected(msg);
        } // else
    } // else if
    else if (uEntireToken.compare("do") == 0 &&
        uEntireTokenAttribute.compare("Reserved") == 0) {
        // gFunctionToken.push_back( "#" ) ; // flag, use to standard output, where to \n
        TheEntireToken();

        bool cout_state = uCout;
        uCout = false;
        Statement();
        uCout = cout_state;

        TheEntireToken();

        if (uEntireToken.compare("while") == 0 &&
            uEntireTokenAttribute.compare("Reserved") == 0) {
            TheEntireToken();
            if (uEntireToken.compare("(") == 0 &&
                uEntireTokenAttribute.compare("") == 0) {
                TheEntireToken();
                if ((IDENT() || uEntireTokenAttribute.compare("Reserved") == 0) ||
                    ((PP(uEntireToken) || MM(uEntireToken)) &&
                        uEntireTokenAttribute.compare("") == 0) ||
                    Sign(uEntireToken) ||
                    uEntireTokenAttribute.compare("Constant") == 0 ||
                    (uEntireToken.compare("(") == 0 &&
                        uEntireTokenAttribute.compare("") == 0)) {
                    Expression(ReturnValue); // necessary
                    if (uEntireToken.compare(")") == 0 &&
                        uEntireTokenAttribute.compare("") == 0) {
                        TheEntireToken();
                        if (uEntireToken.compare(";") == 0 &&
                            uEntireTokenAttribute.compare("") == 0) {
                            ;
                        } // if
                        else {
                            StringStream << uLineCounter;
                            StringStream >> LineNumber;
                            string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
                            throw new ExceptionUnexpected(msg);
                        } // else
                    } // if
                    else { // Unexpected ")"
                        StringStream << uLineCounter;
                        StringStream >> LineNumber;
                        string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
                        throw new ExceptionUnexpected(msg);
                    } // else
                } // if
                else {
                    StringStream << uLineCounter;
                    StringStream >> LineNumber;
                    string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
                    throw new ExceptionUnexpected(msg);
                } // else
            } // if
            else {  // Unexpected "("
                StringStream << uLineCounter;
                StringStream >> LineNumber;
                string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
                throw new ExceptionUnexpected(msg);
            } // else
        } // if
        else {  // Unexpected "while"
            StringStream << uLineCounter;
            StringStream >> LineNumber;
            string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
            throw new ExceptionUnexpected(msg);
        } // else
    } // else if
    else if (uEntireToken.compare("{") == 0 &&
        uEntireTokenAttribute.compare("") == 0) {
        CompoundStatement();
    } // else if
    else if ((IDENT() || uEntireTokenAttribute.compare("Reserved") == 0) ||
        ((PP(uEntireToken) || MM(uEntireToken)) &&
            uEntireTokenAttribute.compare("") == 0) ||
        Sign(uEntireToken) ||
        uEntireTokenAttribute.compare("Constant") == 0 ||
        (uEntireToken.compare("(") == 0 &&
            uEntireTokenAttribute.compare("") == 0)) {
        Expression(ReturnValue);
        if (uEntireToken.compare(";") == 0 &&
            uEntireTokenAttribute.compare("") == 0) {
            ;
        } // if
        else {
            StringStream << uLineCounter;
            StringStream >> LineNumber;
            string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
            throw new ExceptionUnexpected(msg);
        } // else
    } // else if
    else {
        StringStream << uLineCounter;
        StringStream >> LineNumber;
        string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
        throw new ExceptionUnexpected(msg);
    } // else
} // Statement()

void CompoundStatement() {

    int current_def_pos = gDef.size();
    int current_state_pos = gState.size() - 1;
    int current_cout_pos = gCoutToken.size() - 1;
    if (uEntireToken.compare("{") == 0 &&
        uEntireTokenAttribute.compare("") == 0) {
        TheEntireToken();
        while (uEntireToken.compare("}") != 0 ||
            uEntireTokenAttribute.compare("") != 0) {

            // uCin = false ;
            uDoNotCompare = true;
            if (SpecifierType(uEntireToken)) {
                Declaration(); // store the def into current gIdentifierTable.compound_state
            } // if
            else {
                bool cout_state = uCout;
                uCout = false;
                Statement();
                uCout = cout_state;
            } // else

            TheEntireToken();
        } // while

        for (int i = gDef.size() - current_def_pos; i > 0; i--)
            gDef.pop_back();
    } // if
} // CompoundStatement()

void FunctionDefinitionWithoutID() {
    // ex: int Example( bool test ) { ... }
    stringstream StringStream;
    string LineNumber;
    if (uEntireToken.compare("(") == 0 &&
        uEntireTokenAttribute.compare("") == 0) {
        TheEntireToken();
        gDef[gDef.size() - 1].Func = true;
        if (uEntireToken.compare(")") == 0 &&
            uEntireTokenAttribute.compare("") == 0) {
            TheEntireToken();
            if (uEntireToken.compare("{") == 0 &&
                uEntireTokenAttribute.compare("") == 0) {
                CompoundStatement();
            } // if
            else {
                StringStream << uLineCounter;
                StringStream >> LineNumber;
                string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
                throw new ExceptionUnexpected(msg);
            } // else
        } // if
        else {
            int current_pos = gDef.size();
            if (uEntireToken.compare("void") == 0 &&
                uEntireTokenAttribute.compare("Reserved") == 0) {
                TheEntireToken();
            } // if
            else if (SpecifierType(uEntireToken))
                FormalParameterList();
            else {
                StringStream << uLineCounter;
                StringStream >> LineNumber;
                string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
                throw new ExceptionUnexpected(msg);
            } // else

            if (uEntireToken.compare(")") == 0 &&
                uEntireTokenAttribute.compare("") == 0) {
                TheEntireToken();
                if (uEntireToken.compare("{") == 0 && uEntireTokenAttribute.compare("") == 0) {
                    CompoundStatement();
                    // the following is used to pop function's parameter
                    for (int i = gDef.size() - current_pos; i > 0; i--)
                        gDef.pop_back();
                    // the definition is belong to function parameter
                    // ex: int abc( int parameter1, int parameter2 ) => parameter1,2
                } // if
                else {
                    StringStream << uLineCounter;
                    StringStream >> LineNumber;
                    string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
                    throw new ExceptionUnexpected(msg);
                } // else
            } // if
            else {
                StringStream << uLineCounter;
                StringStream >> LineNumber;
                string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
                throw new ExceptionUnexpected(msg);
            } // else
        } // else
    } // if
} // FunctionDefinitionWithoutID()

void FunctionDefinitionOrDeclarators() {
    if (uEntireToken.compare("(") == 0 &&
        uEntireTokenAttribute.compare("") == 0)
        FunctionDefinitionWithoutID();
    else
        RestOfDeclarators();
} // FunctionDefinitionOrDeclarators()

void Definition() {
    stringstream StringStream;
    string LineNumber;
    if (uEntireToken.compare("void") == 0 &&
        uEntireTokenAttribute.compare("Reserved") == 0) {
        uDefinitionType = uEntireToken;
        TheEntireToken();
        if (IDENT()) {    // ex: void Example() { ... }
            TableInfo Temp;
            Temp.Name = uEntireToken;
            Temp.NameString = uEntireToken;
            Temp.Type = uDefinitionType;
            Temp.Attribute = "IDENT";
            Temp.FunctionParameter = false;
            Temp.ArraySize = -1;
            gDef.push_back(Temp);

            TheEntireToken();
            if (uEntireToken.compare("(") == 0 &&
                uEntireTokenAttribute.compare("") == 0) {
                FunctionDefinitionWithoutID();
            } // if
            else {
                StringStream << uLineCounter;
                StringStream >> LineNumber;
                string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
                throw new ExceptionUnexpected(msg);
            } // else
        } // if
        else {
            StringStream << uLineCounter;
            StringStream >> LineNumber;
            string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
            throw new ExceptionUnexpected(msg);
        } // else
    } // if
    else if (SpecifierType(uEntireToken)) {
        uDefinitionType = uEntireToken;
        TheEntireToken();
        int current_pos;
        if (IDENT()) {  // warning int int , int x; and then float x; , bool test ;
            current_pos = gDef.size();
            TableInfo Temp;
            Temp.Name = uEntireToken;
            Temp.NameString = uEntireToken;
            Temp.Type = uDefinitionType;
            Temp.Attribute = "IDENT";
            Temp.ArraySize = -1;
            Temp.FunctionParameter = false;
            gDef.push_back(Temp);
            bool is_func = false;
            TheEntireToken();
            if (uEntireToken.compare("(") == 0 &&
                uEntireTokenAttribute.compare("") == 0)
                is_func = true;

            FunctionDefinitionOrDeclarators();
            if (!is_func) {
                for (int i = current_pos; i < gDef.size(); i++) {
                    gDef[i].Type = uDefinitionType;
                    gDef[i].Attribute = "IDENT";
                    gDef[i].Func = false; // variables
                    gDef[i].GlobalVariable = false;
                    gDef[i].Layer = uLayer;
                } // for
            } // if
        } // if
        else {
            StringStream << uLineCounter;
            StringStream >> LineNumber;
            string msg = "> Line " + LineNumber + " : unexpected token " + "'" + uConstantName + "'";
            throw new ExceptionUnexpected(msg);
        } // else
    } // else if

} // Definition()

void UserInput() {  // Finished

    bool already_take = false;
    gFunctionToken.clear();
    uWalker = -1;
    TheEntireToken();
    while (!uQuit) {
        try {
            bool def = false;
            if ((uEntireToken.compare("void") == 0 &&
                uEntireTokenAttribute.compare("Reserved") == 0) ||
                SpecifierType(uEntireToken)) {
                Definition();
                def = true;
                uDoFunctionCalculation = false;
            } // if
            else {
                uDoFunctionCalculation = true;
                Statement();
            } // else

            if (def) {
                cout << "> ";
                for (int i = 0; i < gDef.size(); i++) {
                    if (SaveIdentifier(gDef[i].Name)) {
                        gDef[i].GlobalVariable = true;
                        gIdentifierTable[uGlobalPosition] = gDef[i];
                        if (gDef[i].Func)
                            cout << "New definition of " << gDef[i].Name << "()" << " entered ..." << endl;
                        else
                            cout << "New definition of " << gDef[i].Name << " entered ..." << endl;
                    } // if
                    else {
                        gDef[i].GlobalVariable = true;
                        gDef[i].VariablePosition = gIdentifierTable.size();
                        gDef[i].TokenInfoList = gFunctionToken;
                        gIdentifierTable.push_back(gDef[i]);
                        if (gDef[i].Func)
                            cout << "Definition of " << gDef[i].Name << "()" << " entered ..." << endl;
                        else
                            cout << "Definition of " << gDef[i].Name << " entered ..." << endl;
                    } // else
                } // for

                uLineBegin = false;
                uLineCounter = 1;
                gFunctionToken.clear();
                uWalker = -1;
                TheEntireToken();
                uLineBegin = true;
            } // if
            else {
                if (uIsDoneID)
                    uQuit = true;
                else if (uIsListVariable) {
                    ListVariable(uListVariableName);
                    cout << "Statement executed ..." << endl;
                } // else if
                else if (uIsListFunction) {
                    ListFunction(uListFunctionName);
                    cout << "Statement executed ..." << endl;
                } // else if
                else if (uIsListAllVariables) {
                    ListAllVariables();
                    cout << "Statement executed ..." << endl;
                } // else if
                else if (uIsListAllFunctions) {
                    ListAllFunctions();
                    cout << "Statement executed ..." << endl;
                } // else if
                else {
                    // Update the new data
                    // and if there is output then print
                    cout << "> ";
                    StoreData();
                    CoutOutput();
                    cout << endl << "Statement executed ..." << endl;
                } // else

                if (!uQuit) {
                    uLineBegin = false;
                    uLineCounter = 1;
                    uWalker = -1;
                    if (!uTokenTook) {  // handle statement'sa if "else"
                        gFunctionToken.clear();                 // else need to get token to check
                        TheEntireToken();                   // if not then not to get again
                    } // if
                    else {
                        TokenInfo Temp;
                        Temp = gFunctionToken[gFunctionToken.size() - 1];
                        gFunctionToken.clear();
                        gFunctionToken.push_back(Temp);
                        uWalker = 0;
                    } // else

                    uLineBegin = true;
                } // if
            } // else
        } // try
        catch (ExceptionUnrecognized* e) {
            cout << e->Msg << endl;
            while (uPeekToken != '\n')
                uPeekToken = Scanner();
            uPeekToken = Scanner();
            uLineCounter = 1;
            uLineBegin = true;
            gFunctionToken.clear();
            uWalker = -1;
            TheEntireToken();
        } // catch
        catch (ExceptionUnexpected* e) {
            cout << e->Msg << endl;
            while (uPeekToken != '\n')
                uPeekToken = Scanner();
            uPeekToken = Scanner();
            uLineCounter = 1;
            uLineBegin = true;
            gFunctionToken.clear();
            uWalker = -1;
            TheEntireToken();
        } // catch
        catch (ExceptionUndefined* e) {
            cout << e->Msg << endl;
            if (!uTook) {
                while (uPeekToken != '\n')
                    uPeekToken = Scanner();
                uPeekToken = Scanner();
                uLineCounter = 1;
                uLineBegin = true;
                gFunctionToken.clear();
                uWalker = -1;
                TheEntireToken();
            } // if
            else {
                uLineCounter = 1;
                uWalker = 0;
                uLineBegin = true;
            } // else
        } // catch
        catch (ExceptionDivisionZero* e) {
            cout << e->Msg << endl;
            while (uPeekToken != '\n')
                uPeekToken = Scanner();
            uPeekToken = Scanner();
            uLineCounter = 1;
            uLineBegin = true;
            gFunctionToken.clear();
            uWalker = -1;
            TheEntireToken();
        } // catch
        catch (ExceptionEOF* e) {
            uQuit = true;
        } // catch

        uDoNotCompare = true;
        uCompareExpression = false;
        uIsDoneID = false;
        uCout = false;
        uCin = false;
        uIsListAllVariables = false;
        uIsListVariable = false;
        uIsListAllFunctions = false;
        uIsListFunction = false;
        uTook = false;
        gDef.clear();
        gState.clear();
        gCoutToken.clear();
        gCondition.clear();
        gReturnValue.clear();
        uLayer = 1;
    } // while
} // UserInput()