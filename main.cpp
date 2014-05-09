#include <windows.h>
#include <iostream>
#include <string>
#include <math.h>
#include "FileIO.h"
//#include "SystemErrorCodes.h"

using namespace std;

struct setting
    {
        bool MakeBatFile;
        bool FileCutNumber;
        bool FileCutSize;
        unsigned int NumberFile;
        unsigned __int64 SizeFile;
    } setup;

void argError(const char* arg);
void printError(int errorCode);
BOOL DirectoryExists(const char* dirName);
bool askNumber(const char*);
bool Commandline(int argc, char **argv);
bool ParSize(char *argv);

void strlow(char* str);
char* firstABC(char* str);

int main(int argc, char **argv)
{
    char *argv1[] = {"tst.exe","-b","-s","1mB"};
    int argc1 = 4;
    Commandline(argc1, argv1);
    return 0;
/*
    if( argc >= 4){
        if ( strcmp(ParametertCutText, argv[1]) == NULL ){
            if( strcmp(ParametertSizeText, argv[2]) == NULL ){

                if( atoi(argv[3]) == 0 ){ //size outFile
                    cout<< "outFile does not have zero size" << endl;
                }else{
                    FileIO inFile;

                    if( inFile.FileOpen(argv[4], GENERIC_READ) != false ){

                        if(DirectoryExists(argv[5]) == NULL){
                            cout<< "Error: out directory name is invalid." << endl;
                        }else{

                        }


                    }else{
                        printError(inFile.GetErro());
                        return 1;
                    }

                }
            }else{
                argError(argv[2]);
                return 1;
            }

        }
    }
*/
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

void argError(const char* arg)
{
    cout<<"CutFile does not have this parameter - "<< arg << endl;
}

void printError(int errorCode)
{
    cout<< "Error code: " << errorCode << "\n\n";

    switch (errorCode)
    {
        case 2:
            cout<< "ERROR_FILE_NOT_FOUND" << endl
            << "The system cannot find the file specified." << endl;
        break;

        case 3:
            cout<< "ERROR_PATH_NOT_FOUND" << endl
            << "The system cannot find the path specified." << endl;
        break;

        case 4:
            cout<< "ERROR_TOO_MANY_OPEN_FILES" << endl
            << "The system cannot open the file." << endl;
        break;

        case 5:
            cout<< "ERROR_ACCESS_DENIED" << endl
            << "Access is denied." << endl;
        break;

        case 32:
            cout<< "ERROR_SHARING_VIOLATION" << endl
            << "The process cannot access the file because it is being used by another process." << endl;
        break;
    }
}

BOOL DirectoryExists(const char* dirName) {
      DWORD attribs = ::GetFileAttributesA(dirName);
      if (attribs == INVALID_FILE_ATTRIBUTES) {
        return false;
      }
      return (attribs & FILE_ATTRIBUTE_DIRECTORY);
}

bool Commandline(int argc, char **argv)
{
    const char ParMakeBat[]   = "-b";
    const char ParNumber[]    = "-n";

    if( argc == 1){
        cout<< "not parameterts" << endl;
        return true;
    }

    for( int i = 1; i<argc-1; i++)
    {
        if( strcmp(argv[i], ParMakeBat ) == NULL ){
                setup.MakeBatFile = true;
                continue;
        }

        if( (strcmp(argv[i], ParNumber) == NULL) && setup.FileCutSize == false ){
            int number = atoi(argv[i+1]);

            if( number != 0 ){
                setup.FileCutNumber = true;
                setup.NumberFile = number;
                cout<< setup.NumberFile << endl;
                continue;
            }else{
                cout<< "Error in command syntax. - "<< argv[i+1]<< endl;
                break;
            }
        }

        ParSize(argv[i]);
    }
}

bool ParSize(char *argv)
{
    const char ParSize[]      = "-s";

    const char byte[]     = "b";
    const char kilobyte[] = "kb";
    const char megabyte[] = "mb";
    const char gigabyte[] = "gb";

    if( (strcmp(argv, ParSize ) == NULL) && (setup.FileCutNumber == false) ){
        int nsize = atoi(argv+1);

        if( nsize != 0 ){
            setup.FileCutSize = true;
            setup.SizeFile = nsize;

            const char *pStr = firstABC(argv+1);
            const int len = strlen(pStr);

            if( (pStr != NULL) && (len <= 2) ){
                char str[3] = {0};
                strncpy(str, pStr, 2);
                strlow(str);

                if( len == 1){
                    if( strcmp(str, byte) == NULL ){
                        return 0;
                    }else{
                        cout<< "Error in command syntax." << endl;
                        return 1;
                    }
                }else{
                    if( strcmp(str, kilobyte) == NULL ){
                        setup.SizeFile <<= 10;
                        return 0;
                    }

                    if( strcmp(str, megabyte) == NULL ){
                        setup.SizeFile <<= 20;
                        return 0;
                    }

                    if( strcmp(str, gigabyte) == NULL ){
                        setup.SizeFile <<= 30;
                        return 0;
                    }
                    cout<< "Error in command syntax." << endl;
                    return 1;
                    }
                }else{
                    cout<< "Error in command syntax." << endl;
                    return 1;
                }
            }else{
                cout<< "invalid size - " << *argv+1 << endl;
                return 1;
            }
        }else{
            cout<< "Error in command syntax." << endl;
            return 1;
    }
}

void strlow(char* str)
{
    while( *str ){
        if( *str >= 'A' && *str <= 'Z' ) *str += 32;
        ++str;
    }
}

char* firstABC(char* str)
{
    while( *str ){
       if( (*str >= 'a' && *str <= 'z') || (*str >= 'A' && *str <= 'Z') ) return str;
       str++;
    }
    return NULL;
}

bool askNumber(const char* str)
{
    if( *str >= '1' && *str <= '9' ){
            return true;
    }

    return false;
}

