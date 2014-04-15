#include "FileIO.h"
#include <iostream>

FileIO::FileIO()
{
    //ctor
}

FileIO::~FileIO()
{
    if(hFile != NULL)
        CloseHandle(hFile);
}

DWORD FileIO::GetErro(){
    return this->ErrorCode;
};

bool FileIO::FileOpen(const TCHAR* FileName, DWORD dwDesiredAccess)
{
    this->hFile = ::CreateFile(FileName, dwDesiredAccess,FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL ,NULL);

    if( this->hFile == INVALID_HANDLE_VALUE){
        this->ErrorCode =  ::GetLastError();
        return false;
    }
	return true;
}

bool FileIO::FileOpen(const TCHAR* FileName, DWORD dwDesiredAccess, DWORD dwShareMode, DWORD dwCreationDisposition)
{
    this->hFile = ::CreateFile(FileName, dwDesiredAccess, dwShareMode, NULL, dwCreationDisposition , FILE_FLAG_SEQUENTIAL_SCAN | FILE_ATTRIBUTE_NORMAL,NULL);

    if( this->hFile == INVALID_HANDLE_VALUE){
        this->ErrorCode =  ::GetLastError();
        return false;
    }
	return true;
}

bool FileIO::Read(void* lpBuffer, DWORD dwBytesToRead, DWORD& nBytesRead )
{
   bool bResult = ::ReadFile( this->hFile, lpBuffer, dwBytesToRead, &nBytesRead, NULL);
    this->ErrorCode = ::GetLastError();

    if( bResult != NULL && nBytesRead == 0){
        return false;
    }
    return true;
}

bool FileIO::Write(void* lpBuffer, DWORD dwBytesToWrite, DWORD* nBytesWrite)
{
    bool bResult;

    bResult = ::WriteFile(this->hFile, lpBuffer, dwBytesToWrite, nBytesWrite, NULL);

    this->ErrorCode = ::GetLastError();

    if(bResult == NULL){
        return false;
    }
    return true;
}

bool FileIO::Delete(TCHAR* FileName)
{
   if( DeleteFile(FileName) != NULL){
        return true;
   }

   this->ErrorCode = GetLastError();

   return false;
}

bool FileIO::SetPointer(LONGLONG llDistanceToMove, DWORD dwMoveMethod)
{
    LARGE_INTEGER pointer;
    pointer.QuadPart = llDistanceToMove;

    bool bResult = ::SetFilePointerEx(this->hFile, pointer, NULL, dwMoveMethod);
    this->ErrorCode = ::GetLastError();

    if(bResult != NULL)
    {
        return true;
    }
    return false;

}

LONGLONG FileIO::GetSize()
{
    typedef BOOL (WINAPI *FileSize)(HANDLE, PLARGE_INTEGER);

    FileSize GetFileSizeEx;
    LARGE_INTEGER dataSize;

    HMODULE hKernel32 = ::GetModuleHandle("kernel32.dll");

    if(hKernel32 != NULL){
            GetFileSizeEx = (FileSize)GetProcAddress(hKernel32, "GetFileSizeEx");

            if(GetFileSizeEx != NULL){
                if((*GetFileSizeEx)(hFile, &dataSize) != 0 ){
                    ErrorCode =  GetLastError();
                    return dataSize.QuadPart;
                }
            }
    }
    return 0;
}
