#include <memory>
#include <cstdio>
#include <vector>
//#include <cstring>
#include <string>
#include <ctype.h>

#include "FileIO.h"
#include "version.h"

//using namespace std;

typedef long long int int64;
const unsigned int NOT_FOUNF_FILE = 1;

struct cmd
{
	 unsigned int sizeBuffer = 256*1024;  // 256 K bytes
	 bool makeBatFile;
	 //bool fileCutNumber;
	 bool splitSize;
	 int64 outFileSize;
	 int64 inFileSize;
	 std::string inFile;
	 std::string outDir;
	 std::string outFile;

	 cmd()
	 {
		  makeBatFile = false;
		  splitSize = false;
	 };

};
//
void argError(const char* arg);
char *GetFirstABC(char* str);
void StrToLower(char* str);

bool GetSetupCommand(struct cmd *setup, const int argc, char **argv);
void GetDirFromFullPath(const char* path, char* dir);

void GetFileFromFullPath(const char* path, char* file);

bool CreateBatFile(std::string &listBarFile, const char *outFile, const char *outDir);

void prefixFile(int numFile, char* resultBuffer);

int main(int argc, char *argv[])
{
	 std::auto_ptr<cmd> pSetup(new cmd);

	 if( GetSetupCommand( pSetup.get(), argc, argv) != false )
	 {
		  return 1;
	 }
	 else
	 {
		  try
		  {
			   std::auto_ptr<FileIO> pInFile(new FileIO);
				std::auto_ptr<FileIO> pOutFile(new FileIO);
				pInFile->FileOpen(pSetup->inFile.c_str(), GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING);

				unsigned int iSizeBuffer = pSetup->sizeBuffer;

				DWORD *pvBuffer = new DWORD[iSizeBuffer];

				std::string outNameFile = pSetup->outFile + ".";
				std::string outFile = pSetup->outDir;

				std::string CommandLine;

				DWORD nReadBytes = 0;
				DWORD nWriteBytes = 0;

				pInFile->SetPointer(0, FILE_BEGIN);
				pOutFile->SetPointer(0,FILE_BEGIN);

				int64 potSizeFile = pSetup->outFileSize;
				unsigned int nByteFofRead = iSizeBuffer;
				static int IndexFile = 0;
				bool bCreateFile = true;

				while(true)
				{
					 if(nByteFofRead >= potSizeFile)
					 {
						  nByteFofRead = potSizeFile;
						  potSizeFile = 0;
					 }

					 pInFile->Read(pvBuffer, nByteFofRead, &nReadBytes);

					 if(nReadBytes > 0)
					 {
						  if(bCreateFile)
						  {
								nByteFofRead = iSizeBuffer;
								++IndexFile;
								char temp[6] = {0};
								prefixFile(IndexFile, temp);

								outFile = pSetup->outDir + outNameFile + temp;

								if(pSetup->makeBatFile){
									 CommandLine += outNameFile + temp;
									 CommandLine += " + ";
								}

								pOutFile->FileOpen(outFile.c_str(), GENERIC_WRITE, FILE_SHARE_READ, CREATE_ALWAYS);
						  }
						  bCreateFile = false;
					 }
					 else
					 {
						  break;
					 }

					 if(nByteFofRead < potSizeFile)
					 {
						  potSizeFile -= iSizeBuffer;
					 }

					 pOutFile->Write(pvBuffer, nReadBytes, &nWriteBytes);

					 if(potSizeFile == 0)
					 {
						  bCreateFile = true;
						  potSizeFile = pSetup->outFileSize;
						  pOutFile->Close();
					 }
				}

//            delete pInFile;
//            delete pOutFile;
				delete []pvBuffer;
				if(pSetup->makeBatFile){
					 CreateBatFile(CommandLine, pSetup->outFile.c_str(), pSetup->outDir.c_str());
				}
		  }
		  catch (DWORD &codErro)
		  {
				std::auto_ptr<char> lpszBuffer(new(char[255]));
				DWORD cchBuffer = 255;
				::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, codErro, NULL, lpszBuffer.get(), cchBuffer, NULL);
				printf("Error Code: %d\nError Name: %s", static_cast<unsigned int>(codErro), lpszBuffer.get());
		  }
		  catch(...)
		  {
				printf("\ncatch(...)\n");
		  }
	 }


	 return 0;
}

bool GetSetupCommand(struct cmd *setup, const int argc, char **argv)
{
	 if(argc == 1)
	 {
		  printf("%s\n\n",PROGRAM_NAME_STRING " v" VERSION_STRING );
		  printf(
				"Usege: mSplit [-sb] [SIZE suffix] [in-file_name] [out-directory]\n\n"
				"Commands:\n"
				"  -s: write SIZE bytes per output file\n"
				"  -b: make Bat-file\n"
				"\n"
				"Suffixes:\n"
				"  b: byte\n"
				"  k: kilobyte\n"
				"  m: megabyte\n"
				"  g: gigabyte\n"
				"  t: terabyte\n"
				"\n"
				"Examples: mSplit -sb 50m filename\n"
		  );
		  return true;
	 }

	 if(argc < 4)
	 {
		  printf("incorrect command");
		  return true;
	 }

	 try
	 {
		  if(strlen(argv[1]) <=1 || strlen(argv[1]) > 3)
		  {
				throw argv[1];
		  }

		  std::auto_ptr<char> command(new char[strlen(argv[1])+2]);
		  strcpy(command.get(), argv[1]);
		  StrToLower(command.get());

		  if(command.get()[0] != '-')
		  {
				throw argv[1];
		  }
		  else
		  {
//        char *p = strchr(command.get(), 'b');
//            if(p == 0)
//       if( *(  ) != 'b' ){
//          throw argv[1];
//       }else{
//          setup->makeBatFile = true;
//       }
				if( command.get()[1] != 's' )
				{
					 throw argv[1];
				}
				else
				{
					 if( command.get()[2] != 'b' && command.get()[2] != 0 )
					 {
						  throw argv[1];
					 }
					 else
					 {
						  if(command.get()[2] == 'b')
						  {
								setup->makeBatFile = true;
						  }
					 }
					 setup->splitSize = true;

					 unsigned int nsize = atoi(argv[2]);
					 setup->outFileSize = nsize;

					 if(nsize == 0)
					 {
						  throw argv[2];
					 }
					 else
					 {
						  std::auto_ptr<char> suffixCommand(new char[strlen(argv[2])+1]);
						  strcpy(suffixCommand.get(), argv[2]);

						  const char *pStr = GetFirstABC(suffixCommand.get());
						  const int len = strlen(pStr);

						  if(pStr == NULL || len >1)
						  {
								throw argv[2];
						  }
						  else
						  {
								StrToLower(suffixCommand.get());
								const char opt = *pStr;
								switch (opt)
								{
								case 'b':
									 break;
								case 'k':
									 setup->outFileSize <<= 10;
									 if( setup->outFileSize <= 0) {throw 1;}
									 break;
								case 'm':
									 setup->outFileSize <<= 20;
									 if( setup->outFileSize <= 0) {throw 1;}
									 break;
								case 'g':
									 setup->outFileSize <<= 30;
									 if( setup->outFileSize <= 0) {throw 1;}
									 break;
								case 't':
									 setup->outFileSize <<= 40;
									 if( setup->outFileSize <= 0) {throw 1;}
									 break;
								default:
									 throw argv[2];
									 break;
								}
						  }
					 }
				}
		  }
	 }
    catch (const char *str)
	 {
		  argError(str);
		  return true;
	 }
    catch (const int OverValue)
	 {
		  printf("%s\n", "overflow value-> size outFile");
		  return true;
	 }



	 std::auto_ptr<FileIO> inFile( new FileIO );
	 if( inFile->FileOpen(argv[3],GENERIC_READ) == false )
	 {
		  printf("can not open input file: ");
		  printf(argv[3]);
		  printf("\n");
		  return true;
	 }
	 else
	 {
		  setup->inFile = argv[3];
		  setup->inFileSize = inFile->GetSize();

		  char *tempNameFile = new char[strlen(argv[3])+1];

		  GetFileFromFullPath(argv[3], tempNameFile);
		  setup->outFile = tempNameFile;

		  delete[] tempNameFile;
	 }

	 if(argc != 4)
	 {
		  if( ::GetFileAttributes(argv[4]) == INVALID_FILE_ATTRIBUTES)
		  {
				printf("can not found directory: ");
				printf(argv[4]);
				printf("\n");
				return 1;
		  }
		  else
		  {
				setup->outDir = argv[4];
				return false;
		  }
	 }
	 else
	 {
		  char *tempPath = new char[strlen(argv[3])+1];
		  GetDirFromFullPath(argv[3], tempPath);
		  setup->outDir = tempPath;
		  delete[] tempPath;
	 }
	 return false;
}

void argError(const char* arg)
{
	 printf("incorrect command: ");
	 printf(arg);
	 printf("\n\n");
}

char* GetFirstABC(char *str)
{
	 while(*str)
	 {
//        if( *str >= 'a' && *str <= 'z' ) return str;
		  if( isalpha(*str) ) return str;
		  ++str;
	 }
	 return NULL;
}

void StrToLower(char *str)
{
	 while (*str)
	 {
		  *str = tolower(*str);
		  ++str;
	 }
	 *(str) = '\0';
}
void GetDirFromFullPath(const char* path, char* dir)
{
	 strcpy(dir, path);
	 char *p = strrchr(dir, '\\');
	 *(++p) = '\0';
}

void GetFileFromFullPath(const char* path, char* file)
{
	 const char *p = strrchr(path, '\\');
	 ++p;
	 strcpy(file, p);
}

void prefixFile(int numFile, char* resultBuffer)
{
	 char templatePrefix[16] ="000";

	 int indexFile = numFile;
	 strcpy(resultBuffer, templatePrefix);

	 const int sizeBuffer = 10;
	 char buffer[sizeBuffer]= {0};

	 sprintf(buffer, "%d", indexFile);

	 int lenBuffer = strlen(buffer);
	 int lenShablon = strlen(resultBuffer);

	 if(lenBuffer > lenShablon)
	 {
		  char strTemp[sizeof(resultBuffer)] = "0";
		  strcat(resultBuffer, strTemp);
	 }

	 lenShablon = strlen(resultBuffer);
	 int offset = lenShablon - lenBuffer;

	 strncpy(resultBuffer+offset, buffer, sizeBuffer);
}

bool CreateBatFile(std::string &CommandLine, const char *outFile, const char *outDir)
{
	 std::string::size_type len = CommandLine.size();

	while (true)
	{
		  if(CommandLine[len] == '+') {CommandLine[len] = '\0'; break;}
		  --len;
	}

	 CommandLine = CommandLine.c_str();
	 std::string commadBatFile = "COPY /B /-Y " + CommandLine + outFile;
	 commadBatFile +=  '\n';
	 commadBatFile +=  "PAUSE";

	 std::string batFile = outDir;
	 batFile += outFile;
	 batFile += ".bat";

	 FILE * pBatFile;
	 pBatFile = fopen (batFile.c_str(),"w");

	 if (pBatFile!=NULL)
	 {
		  fputs (commadBatFile.c_str(),pBatFile);
		  fclose (pBatFile);
	 }
	 return false;
}

