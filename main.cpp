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

int main(int argc, char **argv)
{
    Commandline(argc, argv);
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

        }else{
            if( strcmp(ParametertCreateBatText, argv[1]) == NULL){
                    cout<< "test ver. " << endl;
            }else{
                argError(argv[1]);
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

bool askNumber(const char* str)
{
    if( *str >= '1' && *str <= '9' ){
            return true;
    }

    return false;
}

bool Commandline(int argc, char **argv)
{
    char ParMakeBat[]   = "-b";
    char ParSize[]      = "-s";
    char ParNumber[]    = "-n";
    //char ParametertCutText[]       = "-c";

    char byte[]     = "b";
    char kilobyte[] = "kb";
    char megabyte[] = "mb";
    char gigabyte[] = "gb";
    //char terabyte[] = "tb";

    if( argc == 1){
        cout<< "not parameterts" << endl;
        return true;
    }

    for( int i = 1; i<argc; i++)
    {
        if( strcmp(argv[i], ParMakeBat) == NULL ){ setup.MakeBatFile = true;};

        if( strcmp(argv[i], ParNumber) == NULL ){
            int number = atoi(argv[i+1]);
            if( number != 0 ){
                setup.FileCutNumber = true;
                setup.NumberFile = number;
                cout<< setup.NumberFile << endl;
                i++;
            }else{
                cout<< "invalid number file - "<< argv[i+1]<< endl;;
            }
        }

        if( strcmp(argv[i], ParSize) == NULL ){
            int nsize = atoi(argv[i+1]);
            if( nsize != 0){
                setup.FileCutSize = true;
                setup.SizeFile = nsize;
                for(int j = 1; j<strlen(argv[i+1]); j++)
                {
                    if( argv[i+1][j] >= 'A'){
                            if( argv[i+1][j] == 'b' ){
                                //setup.SizeFile = nsize;
                                cout<< byte << endl;
                                break;
                            }
                            if( argv[i+1][j] == 'k' && argv[i+1][j+1] == 'b' ){
                                setup.SizeFile <<= 10;
                                cout<< kilobyte << endl;
                                break;
                            }
                            if( argv[i+1][j] == 'm' && argv[i+1][j+1] == 'b' ){
                                setup.SizeFile <<= 20;
                                cout<< megabyte << endl;
                                break;
                            }
                            if( argv[i+1][j] == 'g' && argv[i+1][j+1] == 'b' ){
                                setup.SizeFile <<= 30;
                                cout<< gigabyte << endl;
                                break;
                            }
                           /* if( argv[i+1][j] == 't' && argv[i+1][j+1] == 'b' ){
                                setup.SizeFile <<= 40;
                                cout<< gigabyte << endl;
                                break;
                           }
                           */
                            cout<< "invalid storage unit - "<< argv[i+1] << argv[i+2] << endl;
                            break;
                    }
                }
                i++;
            }else{
                cout<< "invalid size - " << argv[i+1] << endl;
            }
        }

       cout<< setup.SizeFile << endl;
    }
}
