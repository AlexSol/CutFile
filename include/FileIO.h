#ifndef FILEIO_H
#define FILEIO_H
#include <windows.h>

typedef BOOL (WINAPI *FileSize)(HANDLE, PLARGE_INTEGER);

class FileIO
{
    public:
        FileIO();
        ~FileIO();

        DWORD GetErro();

       	bool FileOpen(const TCHAR* FileName, DWORD dwDesiredAccess);

       	bool FileOpen(const TCHAR* , DWORD dwDesiredAccess, DWORD dwShareMode, DWORD dwCreationDisposition);

       	bool Read(void *lpBuffer, const DWORD dwBytesToRead, DWORD *nBytesRead );

       	bool Write(void *lpBuffer, const  DWORD dwBytesToWrite, DWORD *nBytesWrite);

       	void Close(){ if(hFile != NULL){ ::CloseHandle(hFile);} };

       	bool SetPointer(LONGLONG llDistanceToMove, DWORD dwMoveMethod);

       	bool Delete(TCHAR* FileName);

        LONGLONG GetSize();


    protected:
    private:
        HANDLE hFile;

        DWORD ErrorCode;
        HMODULE hKernel32;
        FileSize GetFileSizeEx;
       // char* FileName;
};

#endif // FILEIO_H
