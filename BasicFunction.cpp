// Copyright ©  2018 - 2021 Joe Hsu. All rights reserved.
#include "BasicFunction.h"

# define BUFFER_SIZE 30
char gDelimiterTable[BUFFER_SIZE] = { '+' , '-' , '*' , '/' ,
                                        ',' , ';' , '(' , ')' ,
                                        ':' , '=' , '<' , '>' ,
                                        '.' , '"' , '{' , '}' ,
                                        '[' , ']' , '%' , '^' ,
                                        '&' , '|' , '?' , '\'',
                                        '!'
};

string uAssignmentOperatorTable[BUFFER_SIZE] = { "=", "*=", "/=", "%=", "+=", "-=" };
string gSpecifierType[BUFFER_SIZE] = { "int" , "char" , "float" , "string" , "bool" };

bool PP(string Name) {
    if (Name.compare("++") == 0)
        return true;
    else
        return false;
} // PP()

bool MM(string Name) {
    if (Name.compare("--") == 0)
        return true;
    else
        return false;
} // MM()

int StatePosition(string Name) {
    // if IDENT was found in gState,
    // then take that value
    // because that was the latest value
    // but not yet store to global
    int i = gState.size() - 1;
    for (; i >= 0; i--)
        if (gState[i].Name.compare(Name) == 0)
            return i;
    return i;
} // StatePosition()

int StateArrayPosition(string Name, int ArrayIndex) {
    // if IDENT was found in gState,
    // and it was an array
    // then take that value
    // because that was the latest value
    // but not yet store to global
    int i = gState.size() - 1;
    for (; i >= 0; i--)
        if (gState[i].Name.compare(Name) == 0)
            if (gState[i].ArraySize == ArrayIndex)
                return i;
    return i;
} // StateArrayPosition()

bool SaveIdentifier(string Name) {
    // push identifier to table
    uGlobalPosition = -1;
    for (int i = gIdentifierTable.size() - 1; i >= 0; i--) {
        if (gIdentifierTable[i].Name.compare(Name) == 0) {
            uGlobalPosition = i;
            return true;
        } // if
    } // for

    return false;
} // SaveIdentifier()

bool IDENT() {
    // ID can not be a gSpecifierType
    // like: int int;
    if (uEntireTokenAttribute.compare("IDENT") == 0)
        return true;
    else
        return false;
} // IDENT()

bool IdentifierExist(string Name) {
    // first, search gDef
    // second, search gIdentifierTable
    // else didn't exist
    uGlobalPosition = -1;
    uLocalPosition = -1;
    for (int i = gDef.size() - 1; i >= 0; i--) {
        if (gDef[i].Name.compare(Name) == 0) {
            uLocalPosition = i;
            return true;
        } // if
    } // for

    for (int i = gIdentifierTable.size() - 1; i >= 0; i--) {
        if (gIdentifierTable[i].Name.compare(Name) == 0) {
            uGlobalPosition = i;
            return true;
        } // if
    } // for

    return false;

} // IdentifierExist()

void TokenClear() {
    uEntireToken = "";
} // TokenClear()

char Scanner() {
    char ch;
    scanf("%c", &ch);
    return ch;
} // Scanner()

bool Delimiter(char ch) {
    for (int i = 0; i < BUFFER_SIZE; i++) {
        if (gDelimiterTable[i] == ch)
            return true;
    } // for

    return false;
} // Delimiter()

bool Number(char ch) {
    if (ch >= '0' && ch <= '9')
        return true;
    else
        return false;
} // Number()

bool WhiteSpace(char ch) {  // these operand need to stop and check if token exist
    if (ch == '\n')
        uLineCounter++;

    if (ch == ' ' || ch == '\t' || ch == '\n')
        return true;
    else
        return false;
} // WhiteSpace()

bool BeginWithLetter(char ch) {
    if ((ch >= 'a' && ch <= 'z') ||
        (ch >= 'A' && ch <= 'Z'))
        return true;
    else
        return false;
} // BeginWithLetter()

bool IDChar(char ch) {
    if ((ch >= 'a' && ch <= 'z') ||
        (ch >= 'A' && ch <= 'Z') ||
        (ch >= '0' && ch <= '9') ||
        (ch == '_'))
        return true;
    else
        return false;
} // IDChar()

bool ReservedWords(string Name) {
    if (Name.compare("if") == 0)
        return true;
    else if (Name.compare("else") == 0)
        return true;
    else if (Name.compare("do") == 0)
        return true;
    else if (Name.compare("while") == 0)
        return true;
    else if (Name.compare("return") == 0)
        return true;
    else if (Name.compare("void") == 0)
        return true;
    else if (Name.compare("int") == 0)
        return true;
    else if (Name.compare("char") == 0)
        return true;
    else if (Name.compare("float") == 0)
        return true;
    else if (Name.compare("string") == 0)
        return true;
    else if (Name.compare("bool") == 0)
        return true;
    else if (Name.compare("true") == 0)
        return true;
    else if (Name.compare("false") == 0)
        return true;
    else if (Name.compare("Done") == 0)
        return true;
    else if (Name.compare("ListAllVariables") == 0)
        return true;
    else if (Name.compare("ListVariable") == 0)
        return true;
    else if (Name.compare("ListAllFunctions") == 0)
        return true;
    else if (Name.compare("ListFunction") == 0)
        return true;
    else if (Name.compare("cin") == 0)
        return true;
    else if (Name.compare("cout") == 0)
        return true;
    else
        return false;
} // ReservedWords()

bool AssignmentOperator(string Name) {
    for (int i = 0; i < BUFFER_SIZE; i++)
        if (Name.compare(uAssignmentOperatorTable[i]) == 0 &&
            uEntireTokenAttribute.compare("") == 0)
            return true;
    return false;
} // AssignmentOperator()

bool SpecifierType(string Name) {
    for (int i = 0; i < BUFFER_SIZE; i++) {
        if (Name.compare(gSpecifierType[i]) == 0 &&
            uEntireTokenAttribute.compare("Reserved") == 0)
            return true;
    } // for

    return false;
} // SpecifierType()

void Insert(vector<TableInfo>& Temp, TableInfo Item) {
    bool inserted = false;
    for (int i = 0; i < Temp.size() && !inserted; i++) {
        if (Item.Name < Temp[i].Name) {
            Temp.insert(Temp.begin() + i, Item);
            inserted = true;
        } // if
    } // for

    if (!inserted)
        Temp.insert(Temp.end(), Item);

} // Insert()

void DoAssignmentOperator(string Sign, TableInfo& ReturnValue, TableInfo& Temp) {
    if (Sign.compare("=") == 0) {
        ReturnValue.value = Temp.value;
        ReturnValue.ValueString = Temp.ValueString;
    } // if
    else if (Sign.compare("*=") == 0)
        ReturnValue.value *= Temp.value;
    else if (Sign.compare("/=") == 0) {
        ReturnValue.value /= Temp.value;
        if (ReturnValue.Type.compare("int") == 0) {
            int Temp = (int)ReturnValue.value;
            ReturnValue.value = (double)Temp;
        } // if
    } // else if
    else if (Sign.compare("%=") == 0)
        ReturnValue.value = (int)ReturnValue.value % (int)Temp.value;
    else if (Sign.compare("+=") == 0) {
        ReturnValue.value += Temp.value;
        ReturnValue.ValueString += Temp.ValueString;
    } // else if
    else if (Sign.compare("-=") == 0)
        ReturnValue.value -= Temp.value;
} // DoAssignmentOperator()

bool FunctionReservedWord(string Name) {
    if (Name.compare("Done") == 0)
        return true;
    else if (Name.compare("ListAllVariables") == 0)
        return true;
    else if (Name.compare("ListVariable") == 0)
        return true;
    else if (Name.compare("ListAllFunctions") == 0)
        return true;
    else if (Name.compare("ListFunction") == 0)
        return true;
    else
        return false;
} // FunctionReservedWord()

void StoreData() {
    for (int i = 0; i < gState.size(); i++) {
        if (gState[i].GlobalVariable) {
            if (gState[i].ArraySize == -1)
                gIdentifierTable[gState[i].VariablePosition] = gState[i];
            else {
                if (gState[i].Type.compare("int") == 0 ||
                    gState[i].Type.compare("float") == 0 ||
                    gState[i].Type.compare("bool") == 0) {
                    stringstream StringStream;
                    StringStream << gState[i].value;
                    string string_value;
                    StringStream >> string_value;
                    gIdentifierTable[gState[i].VariablePosition].Ptr[gState[i].ArraySize]
                        = string_value;
                } // if
                else {
                    gIdentifierTable[gState[i].VariablePosition].Ptr[gState[i].ArraySize]
                        = gState[i].ValueString;
                } // else
            } // else
        } // if
        else {
            cout << "local" << endl;
            gDef[gState[i].VariablePosition] = gState[i];
        } // else
    } // for
} // StoreData()

void CoutOutput() {
    for (int i = 0; i < gCoutToken.size(); i++)
        cout << gCoutToken[i];
} // CoutOutput()

bool Sign(string Name) {
    if ((Name.compare("+") == 0 || Name.compare("-") == 0 ||
        Name.compare("!") == 0) &&
        uEntireTokenAttribute.compare("") == 0)
        return true;
    else
        return false;
} // Sign()

bool CompareTrueFalse(double Value1, double Value2, string Sign) {
    if (Sign.compare("<") == 0)
        if (Value1 < Value2)
            return true;
        else
            return false;
    else if (Sign.compare(">") == 0)
        if (Value1 > Value2)
            return true;
        else
            return false;
    else if (Sign.compare(">=") == 0)
        if (Value1 >= Value2)
            return true;
        else
            return false;
    else if (Sign.compare("<=") == 0)
        if (Value1 <= Value2)
            return true;
        else
            return false;
    else if (Sign.compare("==") == 0)
        if (Value1 == Value2)
            return true;
        else
            return false;
    else if (Sign.compare("!=") == 0)
        if (Value1 != Value2)
            return true;
        else
            return false;
    else {
        cout << "something unexpected" << endl;
        exit(0);
    } // else
} // CompareTrueFalse()