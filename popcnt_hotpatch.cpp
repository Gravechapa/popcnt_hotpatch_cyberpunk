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
	
	////////////////////////////////////////////////1.05///////////////////////////////////
	switch (rebasedRip)
	{
		case 0x045977D: HOTFIX(ctx->Rax, ctx->Rcx, 5);
		case 0x0489940: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5);
		case 0x0489952: HOTFIX(ctx->R10, *reinterpret_cast<uint64_t*>(ctx->Rsp + 0x18), 7);
		case 0x0489961: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5);
		case 0x0489972: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rsp + 0x38), 7);
		case 0x048BB10: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rdx), 5);
		case 0x048BB22: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rbp - 0x41), 6);
		case 0x048BB40: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rdx), 5);
		case 0x048BB52: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rbp - 0x21), 6);
		case 0x048BC20: HOTFIX(ctx->Rdx, *reinterpret_cast<uint64_t*>(ctx->R8), 5);
		case 0x048BC32: HOTFIX(ctx->Rdx, *reinterpret_cast<uint64_t*>(ctx->Rbp - 0x1), 6);
		case 0x048BC50: HOTFIX(ctx->Rdx, *reinterpret_cast<uint64_t*>(ctx->R8), 5);
		case 0x048BC61: HOTFIX(ctx->Rdx, *reinterpret_cast<uint64_t*>(ctx->Rbp + 0x1f), 6);
		case 0x048BE10: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5);
		case 0x048BE21: HOTFIX(ctx->R10, *reinterpret_cast<uint64_t*>(ctx->Rbp - 0x49), 6);
		case 0x048BE40: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5);
		case 0x048BE51: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rbp - 0x29), 6);
		case 0x048BF70: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5);
		case 0x048BF81: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rbp - 0x9), 6);
		case 0x048BFA0: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5);
		case 0x048BFB2: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rbp + 0x17), 6);
		case 0x048C3B0: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rdx), 5);
		case 0x048C3C2: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rsp + 0x48), 7);
		case 0x048C3E0: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rdx), 5);
		case 0x048C3F2: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rsp + 0x68), 7);
		case 0x048C490: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rdx), 5);
		case 0x048C4A2: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rbp - 0x78), 6);
		case 0x048C4C0: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rdx), 5);
		case 0x048C4D2: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rbp - 0x58), 6);
		case 0x048C590: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rdx), 5);
		case 0x048C5A2: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rbp - 0x58), 6);
		case 0x048C5C0: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rdx), 5);
		case 0x048C5D2: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rbp - 0x78), 6);
		case 0x048C670: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rdx), 5);
		case 0x048C682: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rsp + 0x68), 7);
		case 0x048C6A0: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rdx), 5);
		case 0x048C6B2: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rsp + 0x48), 7);
		case 0x048C760: HOTFIX(ctx->R8, *reinterpret_cast<uint64_t*>(ctx->R9), 5);
		case 0x048C772: HOTFIX(ctx->R8, *reinterpret_cast<uint64_t*>(ctx->Rbp - 0x58), 6);
		case 0x048C790: HOTFIX(ctx->R8, *reinterpret_cast<uint64_t*>(ctx->R9), 5);
		case 0x048C7A2: HOTFIX(ctx->R8, *reinterpret_cast<uint64_t*>(ctx->Rbp - 0x78), 6);
		case 0x048C840: HOTFIX(ctx->R8, *reinterpret_cast<uint64_t*>(ctx->R9), 5);
		case 0x048C852: HOTFIX(ctx->R8, *reinterpret_cast<uint64_t*>(ctx->Rsp + 0x68), 7);
		case 0x048C870: HOTFIX(ctx->R8, *reinterpret_cast<uint64_t*>(ctx->R9), 5);
		case 0x048C882: HOTFIX(ctx->R8, *reinterpret_cast<uint64_t*>(ctx->Rsp + 0x48), 7);
		case 0x048C960: HOTFIX(ctx->R8, *reinterpret_cast<uint64_t*>(ctx->R9), 5);
		case 0x048C972: HOTFIX(ctx->R8, *reinterpret_cast<uint64_t*>(ctx->Rbp - 0x38), 6);
		case 0x048C990: HOTFIX(ctx->R8, *reinterpret_cast<uint64_t*>(ctx->R9), 5);
		case 0x048C9A2: HOTFIX(ctx->R8, *reinterpret_cast<uint64_t*>(ctx->Rbp - 0x18), 6);
		case 0x048CA30: HOTFIX(ctx->R8, *reinterpret_cast<uint64_t*>(ctx->R9), 5);
		case 0x048CA42: HOTFIX(ctx->R8, *reinterpret_cast<uint64_t*>(ctx->Rbp + 0x8), 6);
		case 0x048CA60: HOTFIX(ctx->R8, *reinterpret_cast<uint64_t*>(ctx->R9), 5);
		case 0x048CA72: HOTFIX(ctx->R8, *reinterpret_cast<uint64_t*>(ctx->Rbp + 0x28), 6);
		case 0x048CD70: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5);
		case 0x048CD81: HOTFIX(ctx->R10, *reinterpret_cast<uint64_t*>(ctx->Rbp - 0x28), 6);
		case 0x048CDA0: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5);
		case 0x048CDB1: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rbp - 0x8), 6);
		case 0x048CEE0: HOTFIX(ctx->Rcx, *reinterpret_cast<uint64_t*>(ctx->Rdx), 5);
		case 0x048CEF2: HOTFIX(ctx->Rcx, *reinterpret_cast<uint64_t*>(ctx->Rbp - 0x8), 6);
		case 0x048CF10: HOTFIX(ctx->Rcx, *reinterpret_cast<uint64_t*>(ctx->Rdx), 5);
		case 0x048CF22: HOTFIX(ctx->Rcx, *reinterpret_cast<uint64_t*>(ctx->Rbp - 0x28), 6);
		case 0x048F1D0: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5);
		case 0x048F1E1: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->R13 + 0x30), 6);
		case 0x04911F0: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5);
		case 0x0491209: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->R11 + ctx->R10 * 1 + 0x30), 7);
		case 0x04939B0: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5);
		case 0x04939C1: HOTFIX(ctx->R8, *reinterpret_cast<uint64_t*>(ctx->Rdi + 0x30), 6);
		case 0x0495990: HOTFIX(ctx->Rdx, *reinterpret_cast<uint64_t*>(ctx->Rax), 5);
		case 0x04959A2: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx + 0x18), 6);
		case 0x0497330: HOTFIX(ctx->Rcx, *reinterpret_cast<uint64_t*>(ctx->Rdx), 5);
		case 0x0497341: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rbp + 0x10), 6);
		case 0x0497450: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5);
		case 0x0497460: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rsp + 0x68), 7);
		case 0x04979D0: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5);
		case 0x04979E1: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->R12 + ctx->R9 * 1 + 0x30), 7);
		case 0x0499070: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5);
		case 0x0499081: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->R12 + 0x30), 7);
		case 0x049C600: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5);
		case 0x049C610: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->R12 + 0x18), 7);
		case 0x049CEF0: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5);
		case 0x049CF01: HOTFIX(ctx->R8, *reinterpret_cast<uint64_t*>(ctx->Rbx + 0x30), 6);
		case 0x04BC4DB: HOTFIX(ctx->Rcx, ctx->Rax, 5);

		/////////////32bit reg to 64bit////////////////////////////////////////
		case 0x0B2878A: HOTFIX(ctx->Rcx, *reinterpret_cast<uint32_t*>(ctx->Rcx), 4);

		case 0x0B96CA0: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5);
		case 0x0B96CD5: HOTFIX(ctx->Rax, ctx->Rcx, 5);
		case 0x0B975D0: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rdx), 5);
		case 0x0B97608: HOTFIX(ctx->Rax, ctx->R8, 5);
		case 0x0BCEAE0: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5);
		case 0x0BCEB13: HOTFIX(ctx->Rax, ctx->Rcx, 5);
		case 0x0BCEB40: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5);
		case 0x0BCEB73: HOTFIX(ctx->Rax, ctx->Rdx, 5);
		case 0x0BCF0F0: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5);
		case 0x0BCF126: HOTFIX(ctx->Rax, ctx->Rcx, 5);
		case 0x0FBEF7D: HOTFIX(ctx->Rcx, *reinterpret_cast<uint64_t*>(ctx->Rcx + 0xa0), 9);
		case 0x11457F0: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5);
		case 0x1145801: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->R13 + 0x48), 6);

		case 0x1164971: HOTFIX(ctx->Rax, ctx->Rbx, 5);

		/////////////32bit reg to 64bit/////////////////////////////////////////////////
		case 0x1167FDF: HOTFIX(ctx->Rax, ctx->Rdx & 0xFFFFFFFFULL, 4);
		case 0x118E7F2: HOTFIX(ctx->Rcx, ctx->Rax & 0xFFFFFFFFULL, 4);
		case 0x118E7F6: HOTFIX(ctx->Rax, ctx->R9 & 0xFFFFFFFFULL, 5);

		case 0x175CF30: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->R8), 5);
		case 0x175D4C0: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5);

		/////////////32bit reg to 64bit/////////////////////////////////////////////////
		case 0x175DAC0: HOTFIX(ctx->Rax, *reinterpret_cast<uint32_t*>(ctx->Rcx), 4);
		case 0x175DACF: HOTFIX(ctx->Rax, *reinterpret_cast<uint32_t*>(ctx->Rsi + 0xfc), 8);
		case 0x1B06D18: HOTFIX(ctx->Rax, *reinterpret_cast<uint32_t*>(ctx->R15 + 0x18), 6);
		case 0x1B07392: HOTFIX(ctx->Rax, *reinterpret_cast<uint32_t*>(ctx->R14 + 0x4), 6);
		case 0x1B08535: HOTFIX(ctx->Rax, *reinterpret_cast<uint32_t*>(ctx->R8), 5);
		case 0x1B6F265: HOTFIX(ctx->Rax, *reinterpret_cast<uint32_t*>(ctx->Rcx + 0x6c), 5);
		case 0x1B6F7C1: HOTFIX(ctx->Rax, *reinterpret_cast<uint32_t*>(ctx->Rcx + 0x98), 8);
		case 0x1B70032: HOTFIX(ctx->Rax, *reinterpret_cast<uint32_t*>(ctx->Rcx + 0x68), 5);
		case 0x1B705C0: HOTFIX(ctx->Rcx, *reinterpret_cast<uint32_t*>(ctx->Rdi + 0x98), 8);
		case 0x2247C7D: HOTFIX(ctx->Rbx, *reinterpret_cast<uint32_t*>(ctx->Rsi + 0x7c), 5);

		case 0x246A6A0: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5);
		case 0x246A6D7: HOTFIX(ctx->Rax, ctx->Rdx, 5);
		case 0x246C950: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5);
		case 0x246C982: HOTFIX(ctx->Rax, ctx->Rdx, 5);
		case 0x246CC60: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5);
		case 0x246CC92: HOTFIX(ctx->Rax, ctx->Rdx, 5);
		case 0x2619A80: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5);
		case 0x2619ABA: HOTFIX(ctx->Rax, ctx->Rdx, 5);
		case 0x26D8D70: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5);
		case 0x26D8D9C: HOTFIX(ctx->Rax, ctx->Rcx, 5);
		case 0x2A4B840: HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5);
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
