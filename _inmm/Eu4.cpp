#include "stdafx.h"
#include "injector\assembly.hpp"

// helper that will automatically apply the hook upon dll attach
#define DEF_HOOK(NAME,AT,END) \
struct NAME {void operator()(injector::reg_pack &regs) const;};\
struct NAME##_REG {NAME##_REG(){injector::MakeInline<NAME>(AT,END);}};\
NAME##_REG g_##NAME##_REG;\
void NAME::operator()(injector::reg_pack &regs) const

#define CMP(R, I, REG_HOLDER) \
if (R == I){\
    REG_HOLDER.ef |= (1 << REG_HOLDER.zero_flag);}\
else{\
REG_HOLDER.ef -= (REG_HOLDER.ef & (1 << REG_HOLDER.zero_flag));}

#define VALUE(R) (*(uint32_t*)(R))

#define FLAG_EXTRA_SPACES 1
#define CHAR_INFO_OFFSET 0xb4
// Funtor里只支持fastcall
void __fastcall InitCharInfoArray(BYTE*** pppCharBase, DWORD index, BYTE* pCharInfo)
{
    // pppCharBase is the address of where the 0x00 character charinfo suppose to stay if it was not crasked.
    // ppCharInfoHolder is a 65535 * sizeof(byte*) array.
    BYTE** ppCharInfoHolder = *pppCharBase;
    // ppCharInfoHolder[0] (not *ppCharInfoHolder[0]) will store info about last character info.
    // ppCharInfoHolder[1] (not *ppCharInfoHolder[1]) will store the substituted char.
    // ppCharInfoHolder[2] to ppCharInfoHolder[6] (not *ppCharInfoHolder[1], etc) will be used for a temporary fake character info if necessary.
    if (!ppCharInfoHolder)
    {
        *pppCharBase = (BYTE**)malloc(65535 * sizeof(BYTE*));
        ppCharInfoHolder = *pppCharBase;
        if (!ppCharInfoHolder)
            return;
        for (int i = 0; i < 65535; ++i)
            ppCharInfoHolder[i] = 0;
    }
    // memleak if there's too many characters!
    if (index > 65535)
        return;
    ppCharInfoHolder[index] = pCharInfo;
    if (index == 0x20)
    {
        BYTE** ppOriginalCharInfoHolder = (BYTE**)pppCharBase;
        ppOriginalCharInfoHolder[0x20] = pCharInfo;
    }
    else if (index == 0x40)
    {
        // fill the regular space with the info of 0x40 ('@') to detect errors
        BYTE** ppOriginalCharInfoHolder = (BYTE**)pppCharBase;
        for (unsigned i = 0x21; i < 0xff; ++i)
        {
            ppOriginalCharInfoHolder[i] = (BYTE*)malloc(16);
            for (int j = 0; j < 4; ++j)
                ((DWORD*)ppOriginalCharInfoHolder[i])[j] = ((DWORD*)pCharInfo)[j];
        }
    }
}

inline void __fastcall InitCharInfoArray(uint32_t a, uint32_t b, uint32_t c)
{
    InitCharInfoArray((BYTE***)a, (DWORD)b, (BYTE*)c);
}

// Check if the character is part of a Chinese character or not. 
// If it is, substitute it to a dummy character to bypass all special symbol checks.
// return the fake character.
unsigned char __fastcall LoadCharInfoPhase1(unsigned char* str, int index, BYTE*** pppCharBase, int /*flag*/)
{
    // pppCharBase is the address of where the 0x00 character charinfo suppose to stay if it was not crasked.
    // ppCharInfoHolder is a 65535 * sizeof(byte*) array.
    BYTE** ppCharInfoHolder = *pppCharBase;
    // ppCharInfoHolder[0] (not *ppCharInfoHolder[0]) will store info about last character info.
    // ppCharInfoHolder[1] (not *ppCharInfoHolder[1]) will store the substituted char.
    // ppCharInfoHolder[2] to ppCharInfoHolder[6] (not *ppCharInfoHolder[1], etc) will be used for a temporary fake character info if necessary.
    DWORD& lastCharInfo = (DWORD&)(ppCharInfoHolder[0]);
    unsigned char& sub = (unsigned char&)(ppCharInfoHolder[1]);
    BYTE* fakeChar = (BYTE*)&ppCharInfoHolder[2];

    // always clear the sub character;
    sub = 0;
    if (index == 0)
        lastCharInfo = 0;
    if (lastCharInfo != 0)
    {
        // second character of a Chinese character.
        // substitute the character and let the phase 2 process the rest.
        sub = str[index];
        str[index] = 0x2e;
    }
    else
    {
        unsigned char fc = str[index];
        if (fc < 128)
        {
            // let ascii go
        }
        else
        {
            // 0xa3 + pic name(probably ascii): external picture. 
            // 0xa4: currency sign, probably followed by space
            // 0xa7 + color code (probably ascii): change color
            if (fc == 0xa7 || fc == 0xa3 || fc == 0xa4)
            {
                unsigned char sc = str[index + 1];
                if (sc > 128)
                {
                    // 0xa3/0xa4/0xa7 + larger than 128 second byte: probably a chinese character.
                    sub = str[index];
                    str[index] = 0x2e;
                }
            }
            else
            {
                // probably Chinese character
                sub = str[index];
                str[index] = 0x2e;
            }
        }
    }

    return str[index];
}
inline unsigned char __fastcall LoadCharInfoPhase1(uint32_t str, uint32_t index, uint32_t pppCharBase, uint32_t flag)
{
    return LoadCharInfoPhase1((unsigned char* )str, (int) index, (BYTE***) pppCharBase, (int) flag);
}

void* __fastcall LoadCharInfoPhase2(unsigned char* str, int index, BYTE*** pppCharBase, int flag)
{
    // pppCharBase is the address of where the 0x00 character charinfo suppose to stay if it was not crasked.
    // ppCharInfoHolder is a 65535 * sizeof(byte*) array.
    BYTE** ppCharInfoHolder = *pppCharBase;
    void* returnValue;
    // ppCharInfoHolder[0] (not *ppCharInfoHolder[0]) will store info about last character info.
    // ppCharInfoHolder[1] (not *ppCharInfoHolder[1]) will store the substituted char.
    // ppCharInfoHolder[2] to ppCharInfoHolder[6] (not *ppCharInfoHolder[1], etc) will be used for a temporary fake character info if necessary.
    DWORD& lastCharInfo = (DWORD&)(ppCharInfoHolder[0]);
    unsigned char& sub = (unsigned char&)(ppCharInfoHolder[1]);
    BYTE* fakeChar = (BYTE*)&ppCharInfoHolder[2];

    // recover substituted character
    if (sub != 0)
    {
        str[index] = sub;
        sub = 0;
    }
    if (index == 0)
        lastCharInfo = 0;
    if (lastCharInfo != 0)
    {
        // second character of a Chinese character.
        // prepare fackChar according to flags.
        if ((flag & FLAG_EXTRA_SPACES) == FLAG_EXTRA_SPACES)
        {
            if (str[index] != 0x20)
            {
                returnValue = ppCharInfoHolder[0x2e];
                lastCharInfo = 0;
            }
            else
                returnValue = ppCharInfoHolder[0x20];
        }
        else
        {
            returnValue = fakeChar;
            lastCharInfo = 0;
        }

    }
    else
    {
        unsigned char fc = str[index];
        if (fc == 0)
        {
            returnValue = nullptr;
        }
        else if (fc < 128)
        {
            // ascii
            returnValue = ppCharInfoHolder[(unsigned)fc];
        }
        else
        {
            unsigned char sc = str[index + 1];
            if (sc == 0)
            {
                returnValue = ppCharInfoHolder[(unsigned)fc];
            }
            else
            {
                if (sc == 0x20 && ((flag & FLAG_EXTRA_SPACES) == FLAG_EXTRA_SPACES))
                {
                    for (int i = index + 2; str[i] != 0; ++i)
                    {
                        if (str[i] != 0x20)
                        {
                            sc = str[i];
                            /*str[index + 1] = str[i];
                            str[i] = 0x2e;*/
                            break;
                        }
                    }
                    // sc = str[index + 1];
                }
                returnValue = ppCharInfoHolder[((unsigned)sc) << 8 | fc];
                if (returnValue)
                {
                    lastCharInfo = fc;
                }
                else
                {
                    returnValue = ppCharInfoHolder[(unsigned)fc];
                }
            }
        }
    }

    return returnValue;
}

inline void* __fastcall LoadCharInfoPhase2(uint32_t str, uint32_t index, uint32_t pppCharBase, uint32_t flag)
{
    return LoadCharInfoPhase2((unsigned char*)str, (int)index, (BYTE***)pppCharBase, (int)flag);
}

//void __fastcall Utf8ToGB(unsigned char* str)
//{
//    unsigned char* cur = str;
//    while (*cur != 0)
//        ++cur;
//    int size = cur - str;
//    wchar_t buf[200];
//
//    if (MultiByteToWideChar(CP_UTF8, 0, (LPCCH)str, -1, buf, 200) > 0)
//    {
//        WideCharToMultiByte(936, 0, buf, -1, (LPSTR)str, size, nullptr, nullptr);
//    }
//}



DEF_HOOK(InitCharInfoArrayHook, 0x010C047B, 0x010C0482)
{
    InitCharInfoArray(regs.edx + CHAR_INFO_OFFSET, regs.ecx, regs.eax);
}

DEF_HOOK(LoadCharInfoPhase1Hook, 0x010C0771, 0x010C0776)
{
    unsigned char c = LoadCharInfoPhase1(regs.eax, regs.esi, regs.edi + CHAR_INFO_OFFSET, 0);
    CMP(c, 0xa7, regs);
}

DEF_HOOK(LoadCharInfoPhase2Hook, 0x010C0B49, 0x010C0B56)
{
    regs.eax = (uint32_t)LoadCharInfoPhase2(regs.eax, regs.esi, regs.edi + CHAR_INFO_OFFSET, 0);
}

DEF_HOOK(LoadCharInfoPhase1Hook_2, 0x010C1749, 0x010C1750)
{
    unsigned char c = LoadCharInfoPhase1(regs.eax, regs.edi, VALUE(regs.ebp - 0x8c), 0);
    CMP(c, 0xa7, regs);
    regs.eax = (regs.ebp - 0x48);
}

DEF_HOOK(LoadCharInfoPhase2Hook_2, 0x010C1B86, 0x010C1B8D)
{
    regs.eax = (uint32_t)LoadCharInfoPhase2(regs.eax, regs.edi, regs.ecx, 0);
}

DEF_HOOK(LoadCharInfoPhase2Hook_3, 0x010C683B, 0x010C6846)
{
    regs.edx = (uint32_t)LoadCharInfoPhase2(regs.eax, regs.edi, regs.ecx + CHAR_INFO_OFFSET, FLAG_EXTRA_SPACES);
}

DEF_HOOK(LoadCharInfoPhase2Hook_4, 0x010C6B59, 0x010C6B60)
{
    regs.edi = (uint32_t)LoadCharInfoPhase2(regs.eax, regs.esi, regs.ebx, FLAG_EXTRA_SPACES);
}