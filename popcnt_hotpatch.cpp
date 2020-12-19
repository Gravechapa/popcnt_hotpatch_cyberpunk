// popcnt_hotpatch.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "popcnt_hotpatch.h"
#include <bit>

// This is an example of an exported variable
POPCNT_HOTPATCH_API unsigned char icudt65_dat[] = {
	0x20, 0x00, 0xDA, 0x27, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x54, 0x6F, 0x43, 0x50,
	0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};


uint64_t GetModuleBase(const char *modname)
{
	HANDLE hModSnap;
	MODULEENTRY32 pe32;
	void *bs = NULL;

	// Take a snapshot of all processes in the system.
	hModSnap = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, GetCurrentProcessId() );
	if( hModSnap == INVALID_HANDLE_VALUE )
	{
		return NULL;
	}

	// Set the size of the structure before using it.
	pe32.dwSize = sizeof( MODULEENTRY32 );

	if( !Module32First( hModSnap, &pe32 ) )
	{
		CloseHandle( hModSnap );     // Must clean up the snapshot object!
		return NULL;
	}

	do
	{
		if (!StrCmpI(pe32.szModule, modname)) {
			bs = pe32.modBaseAddr;
			break;
		}
	} while( Module32Next( hModSnap, &pe32 ) );

	CloseHandle( hModSnap );

	return (uint64_t)bs;
}

uint64_t g_imageBase = 0;

#define HOTFIX(offset, dest, src, instr_size) \
	if (rip == g_imageBase + (offset)) { \
		(dest) = std::popcount((src)); \
		ctx->Rip += (instr_size); \
		return EXCEPTION_CONTINUE_EXECUTION; \
	}

LONG NTAPI vec_handler(EXCEPTION_POINTERS *ExceptionInfo)
{
	uint64_t rip = ExceptionInfo->ContextRecord->Rip;
	if (ExceptionInfo->ExceptionRecord->ExceptionCode != 0xc000001d)
		return EXCEPTION_CONTINUE_SEARCH;

	PCONTEXT ctx = ExceptionInfo->ContextRecord;
	
	////1.04///
	HOTFIX(0x045AA2D, ctx->Rax, ctx->Rcx, 5);
	HOTFIX(0x048ABF0, ctx->Rax, *((uint64_t*)ctx->Rcx), 5);
	HOTFIX(0x048AC02, ctx->R10, *((uint64_t*)(ctx->Rsp + 0x18)), 7);
	HOTFIX(0x048AC11, ctx->Rax, *((uint64_t*)ctx->Rcx), 5);
	HOTFIX(0x048AC22, ctx->Rax, *((uint64_t*)(ctx->Rsp + 0x38)), 7);
	HOTFIX(0x048CDC0, ctx->Rax, *((uint64_t*)ctx->Rdx), 5);
	HOTFIX(0x048CDD2, ctx->Rax, *((uint64_t*)(ctx->Rbp - 0x41)), 6);
	HOTFIX(0x048CDF0, ctx->Rax, *((uint64_t*)ctx->Rdx), 5);
	HOTFIX(0x048CE02, ctx->Rax, *((uint64_t*)(ctx->Rbp - 0x21)), 6);
	HOTFIX(0x048CED0, ctx->Rdx, *((uint64_t*)ctx->R8), 5);
	HOTFIX(0x048CEE2, ctx->Rdx, *((uint64_t*)(ctx->Rbp - 0x1)), 6);
	HOTFIX(0x048CF00, ctx->Rdx, *((uint64_t*)ctx->R8), 5);
	HOTFIX(0x048CF11, ctx->Rdx, *((uint64_t*)(ctx->Rbp + 0x1f)), 6);
	HOTFIX(0x048D0C0, ctx->Rax, *((uint64_t*)ctx->Rcx), 5);
	HOTFIX(0x048D0D1, ctx->R10, *((uint64_t*)(ctx->Rbp - 0x49)), 6);
	HOTFIX(0x048D0F0, ctx->Rax, *((uint64_t*)ctx->Rcx), 5);
	HOTFIX(0x048D101, ctx->Rax, *((uint64_t*)(ctx->Rbp - 0x29)), 6);
	HOTFIX(0x048D220, ctx->Rax, *((uint64_t*)ctx->Rcx), 5);
	HOTFIX(0x048D231, ctx->Rax, *((uint64_t*)(ctx->Rbp - 0x9)), 6);
	HOTFIX(0x048D250, ctx->Rax, *((uint64_t*)ctx->Rcx), 5);
	HOTFIX(0x048D262, ctx->Rax, *((uint64_t*)(ctx->Rbp + 0x17)), 6);
	HOTFIX(0x048D660, ctx->Rax, *((uint64_t*)ctx->Rdx), 5);
	HOTFIX(0x048D672, ctx->Rax, *((uint64_t*)(ctx->Rsp + 0x48)), 7);
	HOTFIX(0x048D690, ctx->Rax, *((uint64_t*)ctx->Rdx), 5);
	HOTFIX(0x048D6A2, ctx->Rax, *((uint64_t*)(ctx->Rsp + 0x68)), 7);
	HOTFIX(0x048D740, ctx->Rax, *((uint64_t*)ctx->Rdx), 5);
	HOTFIX(0x048D752, ctx->Rax, *((uint64_t*)(ctx->Rbp - 0x78)), 6);
	HOTFIX(0x048D770, ctx->Rax, *((uint64_t*)ctx->Rdx), 5);
	HOTFIX(0x048D782, ctx->Rax, *((uint64_t*)(ctx->Rbp - 0x58)), 6);
	HOTFIX(0x048D840, ctx->Rax, *((uint64_t*)ctx->Rdx), 5);
	HOTFIX(0x048D852, ctx->Rax, *((uint64_t*)(ctx->Rbp - 0x58)), 6);
	HOTFIX(0x048D870, ctx->Rax, *((uint64_t*)ctx->Rdx), 5);
	HOTFIX(0x048D882, ctx->Rax, *((uint64_t*)(ctx->Rbp - 0x78)), 6);
	HOTFIX(0x048D920, ctx->Rax, *((uint64_t*)ctx->Rdx), 5);
	HOTFIX(0x048D932, ctx->Rax, *((uint64_t*)(ctx->Rsp + 0x68)), 7);
	HOTFIX(0x048D950, ctx->Rax, *((uint64_t*)ctx->Rdx), 5);
	HOTFIX(0x048D962, ctx->Rax, *((uint64_t*)(ctx->Rsp + 0x48)), 7);
	HOTFIX(0x048DA10, ctx->R8, *((uint64_t*)ctx->R9), 5);
	HOTFIX(0x048DA22, ctx->R8, *((uint64_t*)(ctx->Rbp - 0x58)), 6);
	HOTFIX(0x048DA40, ctx->R8, *((uint64_t*)ctx->R9), 5);
	HOTFIX(0x048DA52, ctx->R8, *((uint64_t*)(ctx->Rbp - 0x78)), 6);
	HOTFIX(0x048DAF0, ctx->R8, *((uint64_t*)ctx->R9), 5);
	HOTFIX(0x048DB02, ctx->R8, *((uint64_t*)(ctx->Rsp + 0x68)), 7);
	HOTFIX(0x048DB20, ctx->R8, *((uint64_t*)ctx->R9), 5);
	HOTFIX(0x048DB32, ctx->R8, *((uint64_t*)(ctx->Rsp + 0x48)), 7);
	HOTFIX(0x048DC10, ctx->R8, *((uint64_t*)ctx->R9), 5);
	HOTFIX(0x048DC22, ctx->R8, *((uint64_t*)(ctx->Rbp - 0x38)), 6);
	HOTFIX(0x048DC40, ctx->R8, *((uint64_t*)ctx->R9), 5);
	HOTFIX(0x048DC52, ctx->R8, *((uint64_t*)(ctx->Rbp - 0x18)), 6);
	HOTFIX(0x048DCE0, ctx->R8, *((uint64_t*)ctx->R9), 5);
	HOTFIX(0x048DCF2, ctx->R8, *((uint64_t*)(ctx->Rbp + 0x8)), 6);
	HOTFIX(0x048DD10, ctx->R8, *((uint64_t*)ctx->R9), 5);
	HOTFIX(0x048DD22, ctx->R8, *((uint64_t*)(ctx->Rbp + 0x28)), 6);
	HOTFIX(0x048E020, ctx->Rax, *((uint64_t*)ctx->Rcx), 5);
	HOTFIX(0x048E031, ctx->R10, *((uint64_t*)(ctx->Rbp - 0x28)), 6);
	HOTFIX(0x048E050, ctx->Rax, *((uint64_t*)ctx->Rcx), 5);
	HOTFIX(0x048E061, ctx->Rax, *((uint64_t*)(ctx->Rbp - 0x8)), 6);
	HOTFIX(0x048E190, ctx->Rcx, *((uint64_t*)ctx->Rdx), 5);
	HOTFIX(0x048E1A2, ctx->Rcx, *((uint64_t*)(ctx->Rbp - 0x8)), 6);
	HOTFIX(0x048E1C0, ctx->Rcx, *((uint64_t*)ctx->Rdx), 5);
	HOTFIX(0x048E1D2, ctx->Rcx, *((uint64_t*)(ctx->Rbp - 0x28)), 6);
	HOTFIX(0x0490480, ctx->Rax, *((uint64_t*)ctx->Rcx), 5);
	HOTFIX(0x0490491, ctx->Rax, *((uint64_t*)(ctx->R13 + 0x30)), 6);
	HOTFIX(0x04924A0, ctx->Rax, *((uint64_t*)ctx->Rcx), 5);
	HOTFIX(0x04924B9, ctx->Rax, *((uint64_t*)(ctx->R11 + ctx->R10*1 + 0x30)), 7);
	HOTFIX(0x0494C60, ctx->Rax, *((uint64_t*)ctx->Rcx), 5);
	HOTFIX(0x0494C71, ctx->R8, *((uint64_t*)(ctx->Rdi + 0x30)), 6);
	HOTFIX(0x0496C50, ctx->Rdx, *((uint64_t*)ctx->Rax), 5);
	HOTFIX(0x0496C62, ctx->Rax, *((uint64_t*)(ctx->Rcx + 0x18)), 6);
	HOTFIX(0x04985F0, ctx->Rcx, *((uint64_t*)ctx->Rdx), 5);
	HOTFIX(0x0498601, ctx->Rax, *((uint64_t*)(ctx->Rbp + 0x10)), 6);
	HOTFIX(0x0498710, ctx->Rax, *((uint64_t*)ctx->Rcx), 5);
	HOTFIX(0x0498720, ctx->Rax, *((uint64_t*)(ctx->Rsp + 0x68)), 7);
	HOTFIX(0x0498C90, ctx->Rax, *((uint64_t*)ctx->Rcx), 5);
	HOTFIX(0x0498CA1, ctx->Rax, *((uint64_t*)(ctx->R12 + ctx->R9*1 + 0x30)), 7);
	HOTFIX(0x049A330, ctx->Rax, *((uint64_t*)ctx->Rcx), 5);
	HOTFIX(0x049A341, ctx->Rax, *((uint64_t*)(ctx->R12 + 0x30)), 7);
	HOTFIX(0x049D8C0, ctx->Rax, *((uint64_t*)ctx->Rcx), 5);
	HOTFIX(0x049D8D0, ctx->Rax, *((uint64_t*)(ctx->R12 + 0x18)), 7);
	HOTFIX(0x049E1B0, ctx->Rax, *((uint64_t*)ctx->Rcx), 5);
	HOTFIX(0x049E1C1, ctx->R8, *((uint64_t*)(ctx->Rbx + 0x30)), 6);
	HOTFIX(0x04BD7AB, ctx->Rcx, ctx->Rax, 5);

	//HOTFIX(0x0B490FA, ctx->Ecx, *((uint32_t*)ctx->Rcx), 4);
	HOTFIX(0x0B490FA, ctx->Rcx, *((uint32_t*)ctx->Rcx), 4);

	HOTFIX(0x0BBB2A0, ctx->Rax, *((uint64_t*)ctx->Rcx), 5);	
	HOTFIX(0x0BBB2D5, ctx->Rax, ctx->Rcx, 5);
	HOTFIX(0x0BBBBD0, ctx->Rax, *((uint64_t*)ctx->Rdx), 5);
	HOTFIX(0x0BBBC08, ctx->Rax, ctx->R8, 5);
	HOTFIX(0x0BF30E0, ctx->Rax, *((uint64_t*)ctx->Rcx), 5);
	HOTFIX(0x0BF3113, ctx->Rax, ctx->Rcx, 5);
	HOTFIX(0x0BF3140, ctx->Rax, *((uint64_t*)ctx->Rcx), 5);
	HOTFIX(0x0BF3173, ctx->Rax, ctx->Rdx, 5);
	HOTFIX(0x0BF36F0, ctx->Rax, *((uint64_t*)ctx->Rcx), 5);
	HOTFIX(0x0BF3726, ctx->Rax, ctx->Rcx, 5);
	HOTFIX(0x0FE35BD, ctx->Rcx, *((uint64_t*)(ctx->Rcx + 0xa0)), 9);
	HOTFIX(0x116A2B0, ctx->Rax, *((uint64_t*)ctx->Rcx), 5);
	HOTFIX(0x116A2C1, ctx->Rax, *((uint64_t*)(ctx->R13 + 0x48)), 6);
	
	HOTFIX(0x1189431, ctx->Rax, ctx->Rbx, 5);

	/*HOTFIX(0x118CA9F, ctx->Eax, ctx->Edx, 4);
	HOTFIX(0x11B32B2, ctx->Ecx, ctx->Eax, 4);
	HOTFIX(0x11B32B6, ctx->Eax, ctx->R9d, 5);*/
	HOTFIX(0x118CA9F, ctx->Rax, ctx->Rdx & 0xFFFFFFFFULL, 4);
	HOTFIX(0x11B32B2, ctx->Rcx, ctx->Rax & 0xFFFFFFFFULL, 4);
	HOTFIX(0x11B32B6, ctx->Rax, ctx->R9 & 0xFFFFFFFFULL, 5);

	HOTFIX(0x1783A40, ctx->Rax, *((uint64_t*)ctx->R8), 5);
	HOTFIX(0x1783FD0, ctx->Rax, *((uint64_t*)ctx->Rcx), 5);

	/*HOTFIX(0x17845D0, ctx->Eax, *((uint32_t *)ctx->Rcx), 4);
	HOTFIX(0x17845DF, ctx->Eax, *((uint32_t *)(ctx->Rsi+0xfc)), 8);
	HOTFIX(0x1B321C8, ctx->Eax, *((uint32_t *)(ctx->R15+0x18)), 6);
	HOTFIX(0x1B32842, ctx->Eax, *((uint32_t *)(ctx->R14+0x4)), 6);
	HOTFIX(0x1B339E5, ctx->Eax, *((uint32_t *)ctx->R8), 5);
	HOTFIX(0x1B9A715, ctx->Eax, *((uint32_t *)(ctx->Rcx+0x6c)), 5);
	HOTFIX(0x1B9AC71, ctx->Eax, *((uint32_t *)(ctx->Rcx+0x98)), 8);
	HOTFIX(0x1B9B4E2, ctx->Eax, *((uint32_t *)(ctx->Rcx+0x68)), 5);
	HOTFIX(0x1B9BA70, ctx->Ecx, *((uint32_t *)(ctx->Rdi+0x98)), 8);
	HOTFIX(0x22752DD, ctx->Ebx, *((uint32_t *)(ctx->Rsi+0x7c)), 5);*/
	HOTFIX(0x17845D0, ctx->Rax, *((uint32_t*)ctx->Rcx), 4);
	HOTFIX(0x17845DF, ctx->Rax, *((uint32_t*)(ctx->Rsi + 0xfc)), 8);
	HOTFIX(0x1B321C8, ctx->Rax, *((uint32_t*)(ctx->R15 + 0x18)), 6);
	HOTFIX(0x1B32842, ctx->Rax, *((uint32_t*)(ctx->R14 + 0x4)), 6);
	HOTFIX(0x1B339E5, ctx->Rax, *((uint32_t*)ctx->R8), 5);
	HOTFIX(0x1B9A715, ctx->Rax, *((uint32_t*)(ctx->Rcx + 0x6c)), 5);
	HOTFIX(0x1B9AC71, ctx->Rax, *((uint32_t*)(ctx->Rcx + 0x98)), 8);
	HOTFIX(0x1B9B4E2, ctx->Rax, *((uint32_t*)(ctx->Rcx + 0x68)), 5);
	HOTFIX(0x1B9BA70, ctx->Rcx, *((uint32_t*)(ctx->Rdi + 0x98)), 8);
	HOTFIX(0x22752DD, ctx->Rbx, *((uint32_t*)(ctx->Rsi + 0x7c)), 5);
	
	HOTFIX(0x2497820, ctx->Rax, *((uint64_t*)ctx->Rcx), 5);
	HOTFIX(0x2497857, ctx->Rax, ctx->Rdx, 5);
	HOTFIX(0x2499AD0, ctx->Rax, *((uint64_t*)ctx->Rcx), 5);
	HOTFIX(0x2499B02, ctx->Rax, ctx->Rdx, 5);
	HOTFIX(0x2499DE0, ctx->Rax, *((uint64_t*)ctx->Rcx), 5);
	HOTFIX(0x2499E12, ctx->Rax, ctx->Rdx, 5);
	HOTFIX(0x264ED20, ctx->Rax, *((uint64_t*)ctx->Rcx), 5);
	HOTFIX(0x264ED5A, ctx->Rax, ctx->Rdx, 5);
	HOTFIX(0x270E000, ctx->Rax, *((uint64_t*)ctx->Rcx), 5);
	HOTFIX(0x270E02C, ctx->Rax, ctx->Rcx, 5);
	HOTFIX(0x2A823C0, ctx->Rax, *((uint64_t*)ctx->Rcx), 5);
	///////////

	// TODO: Add your new hotfixes here, the log should help

	FILE *fp = fopen("hotpatch.log", "at");
	fprintf(fp, "Unknown: %016I64X\n", rip - g_imageBase);
	fclose(fp);

	return EXCEPTION_CONTINUE_SEARCH;
}

// This is an example of an exported function.
POPCNT_HOTPATCH_API int install_hooks(void)
{
	g_imageBase = GetModuleBase("Cyberpunk2077.exe");
	AddVectoredExceptionHandler(1, vec_handler);
	return 0;
}
