#include "SystemErrorCodes.h"

typedef unsigned long DWORD;

struct ErrorCode
{
    DWORD Code;
    char* ErrorMes;
};

SystemErrorCodes::SystemErrorCodes()
{

    //ctor
}

SystemErrorCodes::~SystemErrorCodes()
{
    //dtor
}

ErrorCode Code[]={
    0x0,"The operation completed successfully.",
    0x1, "Incorrect function.",
};

