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

#define HOTFIX(dest, src, instr_size) \
		(dest) = std::popcount((src)); \
		ctx->Rip += (instr_size); \
		return EXCEPTION_CONTINUE_EXECUTION; \

LONG NTAPI vec_handler(EXCEPTION_POINTERS *ExceptionInfo)
{
	uint64_t rebasedRip = ExceptionInfo->ContextRecord->Rip - g_imageBase;
	if (ExceptionInfo->ExceptionRecord->ExceptionCode != 0xc000001d)
		return EXCEPTION_CONTINUE_SEARCH;

	PCONTEXT ctx = ExceptionInfo->ContextRecord;
	
	////////////////////////////////////////////////1.04///////////////////////////////////
	switch (rebasedRip)
	{
		case 0x045AA2D: HOTFIX(ctx->Rax, ctx->Rcx, 5);
		case 0x048ABF0: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5);
		case 0x048AC02: HOTFIX(ctx->R10, *reinterpret_cast<uint64_t*>(ctx->Rsp + 0x18), 7);
		case 0x048AC11: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5);
		case 0x048AC22: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rsp + 0x38), 7);
		case 0x048CDC0: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rdx), 5);
		case 0x048CDD2: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rbp - 0x41), 6);
		case 0x048CDF0: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rdx), 5);
		case 0x048CE02: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rbp - 0x21), 6);
		case 0x048CED0: HOTFIX(ctx->Rdx, *reinterpret_cast<uint64_t*>(ctx->R8), 5);
		case 0x048CEE2: HOTFIX(ctx->Rdx, *reinterpret_cast<uint64_t*>(ctx->Rbp - 0x1), 6);
		case 0x048CF00: HOTFIX(ctx->Rdx, *reinterpret_cast<uint64_t*>(ctx->R8), 5);
		case 0x048CF11: HOTFIX(ctx->Rdx, *reinterpret_cast<uint64_t*>(ctx->Rbp + 0x1f), 6);
		case 0x048D0C0: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5);
		case 0x048D0D1: HOTFIX(ctx->R10, *reinterpret_cast<uint64_t*>(ctx->Rbp - 0x49), 6);
		case 0x048D0F0: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5);
		case 0x048D101: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rbp - 0x29), 6);
		case 0x048D220: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5);
		case 0x048D231: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rbp - 0x9), 6);
		case 0x048D250: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5);
		case 0x048D262: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rbp + 0x17), 6);
		case 0x048D660: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rdx), 5);
		case 0x048D672: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rsp + 0x48), 7);
		case 0x048D690: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rdx), 5);
		case 0x048D6A2: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rsp + 0x68), 7);
		case 0x048D740: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rdx), 5);
		case 0x048D752: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rbp - 0x78), 6);
		case 0x048D770: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rdx), 5);
		case 0x048D782: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rbp - 0x58), 6);
		case 0x048D840: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rdx), 5);
		case 0x048D852: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rbp - 0x58), 6);
		case 0x048D870: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rdx), 5);
		case 0x048D882: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rbp - 0x78), 6);
		case 0x048D920: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rdx), 5);
		case 0x048D932: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rsp + 0x68), 7);
		case 0x048D950: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rdx), 5);
		case 0x048D962: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rsp + 0x48), 7);
		case 0x048DA10: HOTFIX(ctx->R8, *reinterpret_cast<uint64_t*>(ctx->R9), 5);
		case 0x048DA22: HOTFIX(ctx->R8, *reinterpret_cast<uint64_t*>(ctx->Rbp - 0x58), 6);
		case 0x048DA40: HOTFIX(ctx->R8, *reinterpret_cast<uint64_t*>(ctx->R9), 5);
		case 0x048DA52: HOTFIX(ctx->R8, *reinterpret_cast<uint64_t*>(ctx->Rbp - 0x78), 6);
		case 0x048DAF0: HOTFIX(ctx->R8, *reinterpret_cast<uint64_t*>(ctx->R9), 5);
		case 0x048DB02: HOTFIX(ctx->R8, *reinterpret_cast<uint64_t*>(ctx->Rsp + 0x68), 7);
		case 0x048DB20: HOTFIX(ctx->R8, *reinterpret_cast<uint64_t*>(ctx->R9), 5);
		case 0x048DB32: HOTFIX(ctx->R8, *reinterpret_cast<uint64_t*>(ctx->Rsp + 0x48), 7);
		case 0x048DC10: HOTFIX(ctx->R8, *reinterpret_cast<uint64_t*>(ctx->R9), 5);
		case 0x048DC22: HOTFIX(ctx->R8, *reinterpret_cast<uint64_t*>(ctx->Rbp - 0x38), 6);
		case 0x048DC40: HOTFIX(ctx->R8, *reinterpret_cast<uint64_t*>(ctx->R9), 5);
		case 0x048DC52: HOTFIX(ctx->R8, *reinterpret_cast<uint64_t*>(ctx->Rbp - 0x18), 6);
		case 0x048DCE0: HOTFIX(ctx->R8, *reinterpret_cast<uint64_t*>(ctx->R9), 5);
		case 0x048DCF2: HOTFIX(ctx->R8, *reinterpret_cast<uint64_t*>(ctx->Rbp + 0x8), 6);
		case 0x048DD10: HOTFIX(ctx->R8, *reinterpret_cast<uint64_t*>(ctx->R9), 5);
		case 0x048DD22: HOTFIX(ctx->R8, *reinterpret_cast<uint64_t*>(ctx->Rbp + 0x28), 6);
		case 0x048E020: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5);
		case 0x048E031: HOTFIX(ctx->R10, *reinterpret_cast<uint64_t*>(ctx->Rbp - 0x28), 6);
		case 0x048E050: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5);
		case 0x048E061: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rbp - 0x8), 6);
		case 0x048E190: HOTFIX(ctx->Rcx, *reinterpret_cast<uint64_t*>(ctx->Rdx), 5);
		case 0x048E1A2: HOTFIX(ctx->Rcx, *reinterpret_cast<uint64_t*>(ctx->Rbp - 0x8), 6);
		case 0x048E1C0: HOTFIX(ctx->Rcx, *reinterpret_cast<uint64_t*>(ctx->Rdx), 5);
		case 0x048E1D2: HOTFIX(ctx->Rcx, *reinterpret_cast<uint64_t*>(ctx->Rbp - 0x28), 6);
		case 0x0490480: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5);
		case 0x0490491: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->R13 + 0x30), 6);
		case 0x04924A0: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5);
		case 0x04924B9: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->R11 + ctx->R10 * 1 + 0x30), 7);
		case 0x0494C60: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5);
		case 0x0494C71: HOTFIX(ctx->R8, *reinterpret_cast<uint64_t*>(ctx->Rdi + 0x30), 6);
		case 0x0496C50: HOTFIX(ctx->Rdx, *reinterpret_cast<uint64_t*>(ctx->Rax), 5);
		case 0x0496C62: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx + 0x18), 6);
		case 0x04985F0: HOTFIX(ctx->Rcx, *reinterpret_cast<uint64_t*>(ctx->Rdx), 5);
		case 0x0498601: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rbp + 0x10), 6);
		case 0x0498710: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5);
		case 0x0498720: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rsp + 0x68), 7);
		case 0x0498C90: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5);
		case 0x0498CA1: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->R12 + ctx->R9 * 1 + 0x30), 7);
		case 0x049A330: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5);
		case 0x049A341: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->R12 + 0x30), 7);
		case 0x049D8C0: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5);
		case 0x049D8D0: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->R12 + 0x18), 7);
		case 0x049E1B0: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5);
		case 0x049E1C1: HOTFIX(ctx->R8, *reinterpret_cast<uint64_t*>(ctx->Rbx + 0x30), 6);
		case 0x04BD7AB: HOTFIX(ctx->Rcx, ctx->Rax, 5);

		/////////////32bit reg to 64bit////////////////////////////////////////
		case 0x0B490FA: HOTFIX(ctx->Rcx, *reinterpret_cast<uint32_t*>(ctx->Rcx), 4);

		case 0x0BBB2A0: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5);
		case 0x0BBB2D5: HOTFIX(ctx->Rax, ctx->Rcx, 5);
		case 0x0BBBBD0: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rdx), 5);
		case 0x0BBBC08: HOTFIX(ctx->Rax, ctx->R8, 5);
		case 0x0BF30E0: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5);
		case 0x0BF3113: HOTFIX(ctx->Rax, ctx->Rcx, 5);
		case 0x0BF3140: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5);
		case 0x0BF3173: HOTFIX(ctx->Rax, ctx->Rdx, 5);
		case 0x0BF36F0: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5);
		case 0x0BF3726: HOTFIX(ctx->Rax, ctx->Rcx, 5);
		case 0x0FE35BD: HOTFIX(ctx->Rcx, *reinterpret_cast<uint64_t*>(ctx->Rcx + 0xa0), 9);
		case 0x116A2B0: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5);
		case 0x116A2C1: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->R13 + 0x48), 6);

		case 0x1189431: HOTFIX(ctx->Rax, ctx->Rbx, 5);

		/////////////32bit reg to 64bit/////////////////////////////////////////////////
		case 0x118CA9F: HOTFIX(ctx->Rax, ctx->Rdx & 0xFFFFFFFFULL, 4);
		case 0x11B32B2: HOTFIX(ctx->Rcx, ctx->Rax & 0xFFFFFFFFULL, 4);
		case 0x11B32B6: HOTFIX(ctx->Rax, ctx->R9 & 0xFFFFFFFFULL, 5);

		case 0x1783A40: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->R8), 5);
		case 0x1783FD0: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5);

		/////////////32bit reg to 64bit/////////////////////////////////////////////////
		case 0x17845D0: HOTFIX(ctx->Rax, *reinterpret_cast<uint32_t*>(ctx->Rcx), 4);
		case 0x17845DF: HOTFIX(ctx->Rax, *reinterpret_cast<uint32_t*>(ctx->Rsi + 0xfc), 8);
		case 0x1B321C8: HOTFIX(ctx->Rax, *reinterpret_cast<uint32_t*>(ctx->R15 + 0x18), 6);
		case 0x1B32842: HOTFIX(ctx->Rax, *reinterpret_cast<uint32_t*>(ctx->R14 + 0x4), 6);
		case 0x1B339E5: HOTFIX(ctx->Rax, *reinterpret_cast<uint32_t*>(ctx->R8), 5);
		case 0x1B9A715: HOTFIX(ctx->Rax, *reinterpret_cast<uint32_t*>(ctx->Rcx + 0x6c), 5);
		case 0x1B9AC71: HOTFIX(ctx->Rax, *reinterpret_cast<uint32_t*>(ctx->Rcx + 0x98), 8);
		case 0x1B9B4E2: HOTFIX(ctx->Rax, *reinterpret_cast<uint32_t*>(ctx->Rcx + 0x68), 5);
		case 0x1B9BA70: HOTFIX(ctx->Rcx, *reinterpret_cast<uint32_t*>(ctx->Rdi + 0x98), 8);
		case 0x22752DD: HOTFIX(ctx->Rbx, *reinterpret_cast<uint32_t*>(ctx->Rsi + 0x7c), 5);

		case 0x2497820: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5);
		case 0x2497857: HOTFIX(ctx->Rax, ctx->Rdx, 5);
		case 0x2499AD0: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5);
		case 0x2499B02: HOTFIX(ctx->Rax, ctx->Rdx, 5);
		case 0x2499DE0: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5);
		case 0x2499E12: HOTFIX(ctx->Rax, ctx->Rdx, 5);
		case 0x264ED20: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5);
		case 0x264ED5A: HOTFIX(ctx->Rax, ctx->Rdx, 5);
		case 0x270E000: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5);
		case 0x270E02C: HOTFIX(ctx->Rax, ctx->Rcx, 5);
		case 0x2A823C0: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5);
	}
	/////////////////////////////////////////////////////////////////////////////////////

	// TODO: Add your new hotfixes here, the log should help

	FILE *fp = fopen("hotpatch.log", "at");
	fprintf(fp, "Unknown: %016I64X\n", rebasedRip);
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
