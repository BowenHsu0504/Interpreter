// Copyright ©  2018 - 2021 Joe Hsu. All rights reserved.
#pragma once
# include <string>

using namespace std;

class Exception {
public:
    string Msg;
    Exception() {  }

    Exception(string x) {
        Msg = x;
    } // Exception()

}; // class Expception

class ExceptionUnrecognized : public Exception {
public:
    ExceptionUnrecognized(string x) {
        Msg = x;
    } // ExceptionUnrecognized()
}; // class ExceptionUnrecognized

class ExceptionUnexpected : public Exception {
public:
    ExceptionUnexpected(string x) {
        Msg = x;
    } // ExceptionUnexpected()
}; // class ExceptionUnexpected

class ExceptionUndefined : public Exception {
public:
    ExceptionUndefined(string x) {
        Msg = x;
    } // ExceptionUndefined()
}; // class ExceptionUndefined

class ExceptionDivisionZero : public Exception {
public:
    ExceptionDivisionZero(string x) {
        Msg = x;
    } // ExceptionDivisionZero()
}; // class ExceptionDivisionZero

class ExceptionEOF : public Exception {
public:
    ExceptionEOF(string x) {
        Msg = x;
    } // ExceptionEOF()
}; // class ExceptionEOF
