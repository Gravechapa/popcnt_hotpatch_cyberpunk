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
    
    //////////////////////////////////////1.3////////////////////////////////////////////
    //Automatically generated by the script
    switch (rebasedRip)
    {
        case 0x46da5d:
        case 0xbd3985:
        case 0xbd46d6:
        case 0xbd8d82:
        case 0xc0df23:
        case 0xc0e536:
        case 0x2889aac:
            HOTFIX(ctx->Rax, ctx->Rcx, 5)
        case 0x49f3e0:
        case 0x49f401:
        case 0x4a18b0:
        case 0x4a18e0:
        case 0x4a1a10:
        case 0x4a1a40:
        case 0x4a2810:
        case 0x4a2840:
        case 0x4a4ce0:
        case 0x4a6f90:
        case 0x4a98b0:
        case 0x4ad320:
        case 0x4ad8b0:
        case 0x4aef40:
        case 0x4b2810:
        case 0x4b3100:
        case 0xbd3950:
        case 0xbd46a0:
        case 0xbd8d50:
        case 0xbd8db0:
        case 0xbd8e20:
        case 0xc0def0:
        case 0xc0df50:
        case 0xc0e500:
        case 0x11364c0:
        case 0x1229cd0:
        case 0x26046d0:
        case 0x2606980:
        case 0x2606c90:
        case 0x27c0020:
        case 0x2889a80:
        case 0x2c031b0:
            HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx), 5)
        case 0x49f412:
            HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rsp + 0x38), 7)
        case 0x4a15b0:
        case 0x4a15e0:
        case 0x4a1e50:
        case 0x4a1e80:
        case 0x4a1f30:
        case 0x4a1f60:
        case 0x4a2030:
        case 0x4a2060:
        case 0x4a2110:
        case 0x4a2140:
        case 0xbd42f0:
        case 0x1139190:
            HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rdx), 5)
        case 0x4a15c2:
            HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rbp - 0x41), 6)
        case 0x4a15f2:
            HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rbp - 0x21), 6)
        case 0x4a18f1:
            HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rbp - 0x29), 6)
        case 0x4a1a21:
            HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rbp - 0x9), 6)
        case 0x4a1a52:
            HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rbp + 0x17), 6)
        case 0x4a1e62:
        case 0x4a2152:
            HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rsp + 0x48), 7)
        case 0x4a1e92:
        case 0x4a2122:
        case 0x4ad330:
            HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rsp + 0x68), 7)
        case 0x4a1f42:
        case 0x4a2072:
            HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rbp - 0x78), 6)
        case 0x4a1f72:
        case 0x4a2042:
            HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rbp - 0x58), 6)
        case 0x4a2851:
            HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rbp - 0x8), 6)
        case 0x4a4cf1:
            HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->R13 + 0x30), 6)
        case 0x4a6fa9:
            HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->R11 + ctx->R10 * 1 + 0x30), 7)
        case 0x4ab8d2:
            HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx + 0x18), 6)
        case 0x4ad201:
            HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rbp + 0x10), 6)
        case 0x4ad8c1:
            HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->R9 + ctx->R12 * 1 + 0x30), 7)
        case 0x4aef51:
            HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->R12 + 0x30), 7)
        case 0x4b2821:
            HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->R12 + 0x18), 7)
        case 0xbd4328:
            HOTFIX(ctx->Rax, ctx->R8, 5)
        case 0xbd8ded:
        case 0xbd8e5a:
        case 0xc0df83:
        case 0x2604707:
        case 0x26069b2:
        case 0x2606cc2:
        case 0x27c005a:
            HOTFIX(ctx->Rax, ctx->Rdx, 5)
        case 0x11364d1:
            HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rbx + 0xe0), 9)
        case 0x11391a2:
            HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rcx + 0xe0), 9)
        case 0x1229b90:
        case 0x186e7a0:
            HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->R8), 5)
        case 0x1229ba2:
        case 0x1229ce1:
            HOTFIX(ctx->Rax, *reinterpret_cast<uint64_t*>(ctx->Rsi + 0x18), 6)
        case 0x124d761:
            HOTFIX(ctx->Rax, ctx->Rbx, 5)
        case 0x1250d0f:
            HOTFIX(ctx->Rax, ctx->Rdx & 0xFFFFFFFFULL, 4)
        case 0x126c113:
            HOTFIX(ctx->Rax, ctx->Rax & 0xFFFFFFFFULL, 4)
        case 0x1277a16:
            HOTFIX(ctx->Rax, ctx->R9 & 0xFFFFFFFFULL, 5)
        case 0x186f100:
            HOTFIX(ctx->Rax, *reinterpret_cast<uint32_t*>(ctx->Rcx), 4)
        case 0x186f10f:
            HOTFIX(ctx->Rax, *reinterpret_cast<uint32_t*>(ctx->Rsi + 0xfc), 8)
        case 0x1c51258:
            HOTFIX(ctx->Rax, *reinterpret_cast<uint32_t*>(ctx->R15 + 0x18), 6)
        case 0x1c518d2:
            HOTFIX(ctx->Rax, *reinterpret_cast<uint32_t*>(ctx->R14 + 0x4), 6)
        case 0x1c52785:
            HOTFIX(ctx->Rax, *reinterpret_cast<uint32_t*>(ctx->R8), 5)
        case 0x1cc1395:
            HOTFIX(ctx->Rax, *reinterpret_cast<uint32_t*>(ctx->Rcx + 0x6c), 5)
        case 0x1cc18f1:
            HOTFIX(ctx->Rax, *reinterpret_cast<uint32_t*>(ctx->Rcx + 0x98), 8)
        case 0x1cc2162:
            HOTFIX(ctx->Rax, *reinterpret_cast<uint32_t*>(ctx->Rcx + 0x68), 5)
        case 0x49f3f2:
            HOTFIX(ctx->R10, *reinterpret_cast<uint64_t*>(ctx->Rsp + 0x18), 7)
        case 0x4a18c1:
            HOTFIX(ctx->R10, *reinterpret_cast<uint64_t*>(ctx->Rbp - 0x49), 6)
        case 0x4a2821:
            HOTFIX(ctx->R10, *reinterpret_cast<uint64_t*>(ctx->Rbp - 0x28), 6)
        case 0x4a16c0:
        case 0x4a16f0:
            HOTFIX(ctx->Rdx, *reinterpret_cast<uint64_t*>(ctx->R8), 5)
        case 0x4a16d2:
            HOTFIX(ctx->Rdx, *reinterpret_cast<uint64_t*>(ctx->Rbp - 0x1), 6)
        case 0x4a1701:
            HOTFIX(ctx->Rdx, *reinterpret_cast<uint64_t*>(ctx->Rbp + 0x1f), 6)
        case 0x4ab8c0:
            HOTFIX(ctx->Rdx, *reinterpret_cast<uint64_t*>(ctx->Rax), 5)
        case 0x15adb71:
            HOTFIX(ctx->Rdx, *reinterpret_cast<uint64_t*>(ctx->R8 + 0x3f8), 9)
        case 0x4a2200:
        case 0x4a2230:
        case 0x4a22e0:
        case 0x4a2310:
        case 0x4a2400:
        case 0x4a2430:
        case 0x4a24d0:
        case 0x4a2500:
            HOTFIX(ctx->R8, *reinterpret_cast<uint64_t*>(ctx->R9), 5)
        case 0x4a2212:
            HOTFIX(ctx->R8, *reinterpret_cast<uint64_t*>(ctx->Rbp - 0x58), 6)
        case 0x4a2242:
            HOTFIX(ctx->R8, *reinterpret_cast<uint64_t*>(ctx->Rbp - 0x78), 6)
        case 0x4a22f2:
            HOTFIX(ctx->R8, *reinterpret_cast<uint64_t*>(ctx->Rsp + 0x68), 7)
        case 0x4a2322:
            HOTFIX(ctx->R8, *reinterpret_cast<uint64_t*>(ctx->Rsp + 0x48), 7)
        case 0x4a2412:
            HOTFIX(ctx->R8, *reinterpret_cast<uint64_t*>(ctx->Rbp - 0x38), 6)
        case 0x4a2442:
            HOTFIX(ctx->R8, *reinterpret_cast<uint64_t*>(ctx->Rbp - 0x18), 6)
        case 0x4a24e2:
            HOTFIX(ctx->R8, *reinterpret_cast<uint64_t*>(ctx->Rbp + 0x8), 6)
        case 0x4a2512:
            HOTFIX(ctx->R8, *reinterpret_cast<uint64_t*>(ctx->Rbp + 0x28), 6)
        case 0x4a98c1:
            HOTFIX(ctx->R8, *reinterpret_cast<uint64_t*>(ctx->Rdi + 0x30), 6)
        case 0x4b3111:
            HOTFIX(ctx->R8, *reinterpret_cast<uint64_t*>(ctx->Rbx + 0x30), 6)
        case 0x4a2980:
        case 0x4a29b0:
        case 0x4ad1f0:
            HOTFIX(ctx->Rcx, *reinterpret_cast<uint64_t*>(ctx->Rdx), 5)
        case 0x4a2992:
            HOTFIX(ctx->Rcx, *reinterpret_cast<uint64_t*>(ctx->Rbp - 0x8), 6)
        case 0x4a29c2:
            HOTFIX(ctx->Rcx, *reinterpret_cast<uint64_t*>(ctx->Rbp - 0x28), 6)
        case 0x4d397b:
            HOTFIX(ctx->Rcx, ctx->Rax, 5)
        case 0xb63a8a:
            HOTFIX(ctx->Rcx, *reinterpret_cast<uint32_t*>(ctx->Rcx), 4)
        case 0x1099bad:
            HOTFIX(ctx->Rcx, *reinterpret_cast<uint64_t*>(ctx->Rcx + 0xa0), 9)
        case 0x126cab8:
        case 0x1277a12:
            HOTFIX(ctx->Rcx, ctx->Rax & 0xFFFFFFFFULL, 4)
        case 0x15adb60:
            HOTFIX(ctx->Rcx, *reinterpret_cast<uint64_t*>(ctx->Rax), 5)
        case 0x1cc26f0:
            HOTFIX(ctx->Rcx, *reinterpret_cast<uint32_t*>(ctx->Rdi + 0x98), 8)
        case 0x23cbfad:
            HOTFIX(ctx->Rbx, *reinterpret_cast<uint32_t*>(ctx->Rsi + 0x7c), 5)
    }
    /////////////////////////////////////////////////////////////////////////////////////

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
