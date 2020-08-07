#pragma once
#include <Windows.h>

#define PEHEADER_SIZE (0x1000)
#define ALIGN_SIZE_UP(Size, Alignment) (((ULONG_PTR)(Size) + Alignment - 1) & ~(Alignment - 1))
#define MEM_OFFSET(EndAddr, StartAddr)((ULONG)((ULONG_PTR)EndAddr - (ULONG_PTR)StartAddr))


class PEImage {
public:
	BOOL is32bitPE;
	BOOL closeFileAfterLoading = TRUE;
	//DWORD dwPageSize;
	HANDLE hPEFile;
	DWORD fileSize;
	HANDLE hProc;
	WORD sectionCount;
	PBYTE peImageData;
	UINT64 EntryPoint;
	UINT64 ImageBase;


	// Structures:
	PIMAGE_DOS_HEADER DosHeader;

	PIMAGE_NT_HEADERS32 NtHeader32;
	PIMAGE_NT_HEADERS64 NtHeader64;

	PIMAGE_FILE_HEADER FileHeader;

	PIMAGE_OPTIONAL_HEADER32 OptHeader32;
	PIMAGE_OPTIONAL_HEADER64 OptHeader64;

	PIMAGE_DATA_DIRECTORY DataDirArr[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
	PIMAGE_DATA_DIRECTORY& iidExport = DataDirArr[IMAGE_DIRECTORY_ENTRY_EXPORT];
	PIMAGE_DATA_DIRECTORY& iidImport = DataDirArr[IMAGE_DIRECTORY_ENTRY_IMPORT];
	PIMAGE_DATA_DIRECTORY& iidResource = DataDirArr[IMAGE_DIRECTORY_ENTRY_RESOURCE];
	PIMAGE_DATA_DIRECTORY& iidException = DataDirArr[IMAGE_DIRECTORY_ENTRY_EXCEPTION];
	PIMAGE_DATA_DIRECTORY& iidSecurity = DataDirArr[IMAGE_DIRECTORY_ENTRY_SECURITY];
	PIMAGE_DATA_DIRECTORY& iidBaseReloc = DataDirArr[IMAGE_DIRECTORY_ENTRY_BASERELOC];
	PIMAGE_DATA_DIRECTORY& iidDebug = DataDirArr[IMAGE_DIRECTORY_ENTRY_DEBUG];
	PIMAGE_DATA_DIRECTORY& iidArchitecture = DataDirArr[IMAGE_DIRECTORY_ENTRY_ARCHITECTURE];
	PIMAGE_DATA_DIRECTORY& iidGlobalPTR = DataDirArr[IMAGE_DIRECTORY_ENTRY_GLOBALPTR];
	PIMAGE_DATA_DIRECTORY& iidTLS = DataDirArr[IMAGE_DIRECTORY_ENTRY_TLS];
	PIMAGE_DATA_DIRECTORY& iidLoadConfig = DataDirArr[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG];
	PIMAGE_DATA_DIRECTORY& iidBountImport = DataDirArr[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT];
	PIMAGE_DATA_DIRECTORY& iidIAT = DataDirArr[IMAGE_DIRECTORY_ENTRY_IAT];
	PIMAGE_DATA_DIRECTORY& iidDelayImport = DataDirArr[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT];
	PIMAGE_DATA_DIRECTORY& iidComDescriptor = DataDirArr[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR];

	PIMAGE_SECTION_HEADER* SectionHeadersArr;

	PBYTE loadPEImage(HANDLE hFile);
	PBYTE loadPEImage(CHAR* PEPath);
	BOOL checkIs32bit(PBYTE fileBytes);
	BOOL VerifyImage(PBYTE imgData);


	PEImage();
	~PEImage();

	PIMAGE_SECTION_HEADER LocationSectionByRVA(int VirtualAddr);
	PBYTE getPtrInBufferByVA(int VirtualAddr);

	int getImportNum();
	DWORD Rva2Raw(DWORD va);
private:
	int iidImportCnt;
};
