//#include <windows.h>
#include "FileIO.h"
FileIO::FileIO()
{
    this->hKernel32 = ::GetModuleHandle("kernel32.dll");
    this->GetFileSizeEx = (FileSize)::GetProcAddress(this->hKernel32, "GetFileSizeEx");
}

FileIO::~FileIO()
{
    if(hFile != NULL)
        ::CloseHandle(hFile);
}

DWORD FileIO::GetErro(){
    return this->ErrorCode;
};

bool FileIO::FileOpen(const TCHAR* FileName, DWORD dwDesiredAccess)
{
    this->hFile = ::CreateFile(FileName, dwDesiredAccess,FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL ,NULL);
    this->ErrorCode =  ::GetLastError();
    if(this->ErrorCode == 0) {return true;}
    return false;
    if( this->hFile == INVALID_HANDLE_VALUE){
        this->ErrorCode =  ::GetLastError();
        return false;
    }
	return true;
}

bool FileIO::FileOpen(const TCHAR* FileName, DWORD dwDesiredAccess, DWORD dwShareMode, DWORD dwCreationDisposition)
{
    this->hFile = ::CreateFile(FileName, dwDesiredAccess, dwShareMode, NULL, dwCreationDisposition , /*FILE_FLAG_SEQUENTIAL_SCAN | */FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH,NULL);
    this->ErrorCode =  ::GetLastError();
    if(this->ErrorCode == 0) {return true;}
    return false;
    if( this->hFile == INVALID_HANDLE_VALUE){
        this->ErrorCode = ::GetLastError();
        return false;
    }
	return true;
}

bool FileIO::Read(void *lpBuffer, const DWORD dwBytesToRead, DWORD *nBytesRead )
{
   bool bResult = ::ReadFile( this->hFile, lpBuffer, dwBytesToRead, nBytesRead, NULL);
    this->ErrorCode = ::GetLastError();

    if( bResult != 0 && nBytesRead == 0){
        return false;
    }
    return true;
}

bool FileIO::Write(void *lpBuffer, const DWORD dwBytesToWrite, DWORD *nBytesWrite)
{
    bool bResult;

    bResult = ::WriteFile(this->hFile, lpBuffer, dwBytesToWrite, nBytesWrite, NULL);

    this->ErrorCode = ::GetLastError();

    if(bResult == 0){
        return false;
    }
    return true;
}

bool FileIO::Delete(TCHAR* FileName)
{
   if( DeleteFile(FileName) != 0 ){
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

    if(bResult != 0)
    {
        return true;
    }
    return false;

}

LONGLONG FileIO::GetSize()
{
    LARGE_INTEGER dataSize;
    if(GetFileSizeEx != NULL){
        if((*GetFileSizeEx)(hFile, &dataSize) != 0 ){
            this->ErrorCode =  GetLastError();
                return dataSize.QuadPart;
        }
    }
    return 0;
}
