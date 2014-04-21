#include <windows.h>
#include <iostream>
#include <string>
#include <math.h>
#include "FileIO.h"
//#include "SystemErrorCodes.h"

using namespace std;

void argError(const char* arg);
void printError(int errorCode);
BOOL DirectoryExists(const char* dirName);

int main(int argc, char **argv)
{
    char parametertTextCut[]  = "-c";
    char parametertTextJoin[] = "-j";
    char parametertTextSize[] = "-s";

    if( argc >= 4){
        if ( strcmp(parametertTextCut, argv[1]) == NULL ){
            if( strcmp(parametertTextSize, argv[2]) == NULL ){

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

        }else{
            if( strcmp(parametertTextJoin, argv[1]) == NULL){
                    cout<< "test ver. " << endl;
            }else{
                argError(argv[1]);
                return 1;
            }
        }
    }

    return 0;
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
