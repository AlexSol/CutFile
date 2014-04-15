#include <windows.h>
#include <iostream>
#include <string>
#include <math.h>
#include "FileIO.h"
//#include "winerror.h"
//#include "SystemErrorCodes.h"

using namespace std;

int main(int argc, char **argv)
{
    if(argc == 4){
        char* inFileName = argv[1];
        char* ouFileName = argv[2];
        FileIO f;
        FileIO fout;

       f.FileOpen(inFileName, GENERIC_READ);

        fout.FileOpen(ouFileName, GENERIC_WRITE, FILE_SHARE_READ, CREATE_ALWAYS);
        cout<< argv[0] << endl
            << argv[1] << endl
            << argv[2] << endl
            << argv[3] << endl;
        TCHAR outFileName[] = "";
        TCHAR outFileNameMaska[] = "001";

        return 0;

        unsigned int sizeBuffer =   4096*1024;
        unsigned char* buffer = new unsigned char[sizeBuffer];

        DWORD dwBytesRead = sizeBuffer;
        DWORD dwBytesResult = 0;
        DWORD dwBytesWriteResult = 0;

        LONGLONG inputFile = 0;
        LONGLONG uotFile = 0;

        f.SetPointer( inputFile, FILE_BEGIN);
        fout.SetPointer( uotFile, FILE_BEGIN);


        while(f.Read(buffer, dwBytesRead, dwBytesResult) != NULL)
        {

            fout.Write(buffer,dwBytesResult, &dwBytesWriteResult);

            inputFile += dwBytesRead;
            f.SetPointer( inputFile, FILE_BEGIN);

            uotFile += dwBytesResult;
            fout.SetPointer( uotFile, FILE_BEGIN);
        }

        delete []buffer;

        return 0;
    }
    else{
        cout<< "not opt!" << endl;
        return 1;
    }
}
