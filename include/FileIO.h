#ifndef FILEIO_H
#define FILEIO_H
#include <windows.h>
#include <iostream>
#include <string>
class FileIO
{
    public:
        FileIO();
        virtual ~FileIO();

        DWORD GetErro();

       	bool FileOpen(const TCHAR* FileName, DWORD dwDesiredAccess);

       	bool FileOpen(const TCHAR* , DWORD dwDesiredAccess, DWORD dwShareMode, DWORD dwCreationDisposition);

       	bool Read(void* lpBuffer, DWORD dwBytesToRead, DWORD& nBytesRead );

       	bool Write(void* lpBuffer, DWORD dwBytesToWrite, DWORD* nBytesWrite);

       	bool SetPointer(LONGLONG llDistanceToMove, DWORD dwMoveMethod);

       	bool Delete(TCHAR* FileName);

        LONGLONG GetSize();


    protected:
    private:
        HANDLE hFile;

        DWORD ErrorCode;
       // char* FileName;
};
#endif // FILEIO_H
