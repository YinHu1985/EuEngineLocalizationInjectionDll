// _inmm.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "_inmm.h"
#include <string>

HMODULE hWinmm = NULL;
wchar_t fileNameBuffer[MAX_PATH];
void GetHWinmm()
{
    if (hWinmm == NULL)
    {
        GetSystemDirectory(fileNameBuffer, MAX_PATH);
        hWinmm = ::LoadLibraryEx((std::wstring(fileNameBuffer) + L"\\winmm.dll").c_str(), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
    }
}

typedef DWORD (__stdcall *pmciSendCommandA)(UINT,
                                            UINT,
                                            DWORD,
                                            DWORD_PTR); 
typedef DWORD (__stdcall *ptimeBeginPeriod)(UINT); 
typedef DWORD(__stdcall *ptimeEndPeriod)(UINT);
typedef DWORD (__stdcall *ptimeGetDevCaps)( UINT,  
                                           UINT); 
typedef DWORD (__stdcall *ptimeGetTime)(void); 
typedef DWORD (__stdcall *ptimeKillEvent)(UINT); 
typedef DWORD (__stdcall *ptimeSetEvent)(UINT, 
                                         UINT  , 
                                         UINT ,  
                                         UINT   ,      
                                         UINT     ); 
typedef DWORD(__stdcall *pwaveOutOpen)(UINT,
    UINT,
    UINT,
    UINT,
    UINT,
    UINT);
typedef DWORD(__stdcall *pwaveOutClose)(UINT);
typedef DWORD(__stdcall *pwaveOutPrepareHeader)(UINT,UINT,UINT);
typedef DWORD(__stdcall *pwaveOutUnprepareHeader)(UINT, UINT, UINT);
typedef DWORD(__stdcall *pwaveOutWrite)(UINT, UINT, UINT);
typedef DWORD(__stdcall *pwaveOutGetNumDevs)();
typedef DWORD(__stdcall *pwaveOutGetDevCapsW)(UINT, UINT, UINT);
typedef DWORD(__stdcall *pwaveOutGetErrorTextW)(UINT, UINT, UINT);
typedef DWORD(__stdcall *pwaveInGetNumDevs)();
typedef DWORD(__stdcall *pwaveInGetDevCapsW)(UINT, UINT, UINT);
typedef DWORD(__stdcall *pwaveInOpen)(UINT,
    UINT,
    UINT,
    UINT,
    UINT,
    UINT);
typedef DWORD(__stdcall *pwaveInClose)(UINT);

extern "C" _INMM_API DWORD WINAPI mciSendCommandA(
    UINT IDDevice,
    UINT uMsg,
    DWORD fdwCommand,
    DWORD_PTR dwParam
    )
{
    GetHWinmm();
    pmciSendCommandA faddr;
    faddr = (pmciSendCommandA)GetProcAddress(hWinmm,"mciSendCommandA");
    return (faddr)( IDDevice,
        uMsg,
        fdwCommand,
        dwParam
        );
}

extern "C" _INMM_API DWORD WINAPI timeBeginPeriod(
    UINT uPeriod  
    )
{
    GetHWinmm();
    ptimeBeginPeriod faddr;
    faddr = (ptimeBeginPeriod)GetProcAddress(hWinmm,"timeBeginPeriod");
    return (faddr)(uPeriod );
}

extern "C" _INMM_API DWORD WINAPI timeEndPeriod(
    UINT uPeriod
    )
{
    GetHWinmm();
    ptimeEndPeriod faddr;
    faddr = (ptimeEndPeriod)GetProcAddress(hWinmm, "timeEndPeriod");
    return (faddr)(uPeriod);
}

extern "C" _INMM_API DWORD WINAPI timeGetDevCaps(
    UINT ptc,  
    UINT cbtc        
    )
{
    GetHWinmm();
    ptimeGetDevCaps faddr;
    faddr = (ptimeGetDevCaps)GetProcAddress(hWinmm,"timeGetDevCaps");
    return (faddr)(ptc,cbtc);
}

extern "C" _INMM_API DWORD WINAPI timeGetTime(void)
{
    GetHWinmm();
    ptimeGetTime faddr;
    faddr = (ptimeGetTime)GetProcAddress(hWinmm,"timeGetTime");
    return (faddr)();
}

extern "C" _INMM_API DWORD WINAPI timeKillEvent(UINT uTimerID )
{
    GetHWinmm();
    ptimeKillEvent faddr;
    faddr = (ptimeKillEvent)GetProcAddress(hWinmm,"timeKillEvent");
    return (faddr)(uTimerID);
}

extern "C" _INMM_API DWORD WINAPI timeSetEvent(
    UINT           uDelay,      
    UINT           uResolution, 
    UINT lpTimeProc,  
    UINT      dwUser,      
    UINT           fuEvent      
    )
{
    GetHWinmm();
    ptimeSetEvent faddr;
    faddr = (ptimeSetEvent)GetProcAddress(hWinmm,"timeSetEvent");
    return (faddr)(uDelay,uResolution,lpTimeProc,dwUser, fuEvent     );
}

extern "C" _INMM_API DWORD WINAPI waveOutOpen(
    UINT phwo,
    UINT uDeviceID,
    UINT pwfx,
    UINT dwCallback,
    UINT dwCallbackInstance,
    UINT fdwOpen
    )
{
    GetHWinmm();
    pwaveOutOpen faddr;
    faddr = (pwaveOutOpen)GetProcAddress(hWinmm, "waveOutOpen");
    return (faddr)(phwo, uDeviceID, pwfx, dwCallback, dwCallbackInstance, fdwOpen);
}

extern "C" _INMM_API DWORD WINAPI waveOutClose(UINT hwo)
{
    GetHWinmm();
    pwaveOutClose faddr;
    faddr = (pwaveOutClose)GetProcAddress(hWinmm, "waveOutClose");
    return (faddr)(hwo);
}

extern "C" _INMM_API DWORD WINAPI waveOutPrepareHeader(
    UINT hwo,
    UINT pwh,
    UINT cbwh
    )
{
    GetHWinmm();
    pwaveOutPrepareHeader faddr;
    faddr = (pwaveOutPrepareHeader)GetProcAddress(hWinmm, "waveOutPrepareHeader");
    return (faddr)(hwo, pwh, cbwh);
}

extern "C" _INMM_API DWORD WINAPI waveOutUnprepareHeader(
    UINT hwo,
    UINT pwh,
    UINT cbwh
    )
{
    GetHWinmm();
    pwaveOutUnprepareHeader faddr;
    faddr = (pwaveOutUnprepareHeader)GetProcAddress(hWinmm, "waveOutUnprepareHeader");
    return (faddr)(hwo, pwh, cbwh);
}

extern "C" _INMM_API DWORD WINAPI waveOutWrite(
    UINT hwo,
    UINT pwh,
    UINT cbwh
    )
{
    GetHWinmm();
    pwaveOutWrite faddr;
    faddr = (pwaveOutWrite)GetProcAddress(hWinmm, "waveOutWrite");
    return (faddr)(hwo, pwh, cbwh);
}
extern "C" _INMM_API DWORD WINAPI waveOutGetNumDevs()
{
    GetHWinmm();
    pwaveOutGetNumDevs faddr;
    faddr = (pwaveOutGetNumDevs)GetProcAddress(hWinmm, "waveOutGetNumDevs");
    return (faddr)();
}
extern "C" _INMM_API DWORD WINAPI waveOutGetDevCapsW(
    UINT uDeviceID,
    UINT pwoc,
    UINT cbwoc
    )
{
    GetHWinmm();
    pwaveOutGetDevCapsW faddr;
    faddr = (pwaveOutGetDevCapsW)GetProcAddress(hWinmm, "pwaveOutGetDevCapsW");
    return (faddr)(uDeviceID, pwoc, cbwoc);
}
extern "C" _INMM_API DWORD WINAPI waveOutGetErrorTextW(
    UINT mmrError,
    UINT pszText,
    UINT cchText
    )
{
    GetHWinmm();
    pwaveOutGetErrorTextW faddr;
    faddr = (pwaveOutGetErrorTextW)GetProcAddress(hWinmm, "waveOutGetErrorTextW");
    return (faddr)(mmrError,pszText,cchText);
}
extern "C" _INMM_API DWORD WINAPI waveInGetNumDevs()
{
    GetHWinmm();
    pwaveInGetNumDevs faddr;
    faddr = (pwaveInGetNumDevs)GetProcAddress(hWinmm, "waveInGetNumDevs");
    return (faddr)();
}
extern "C" _INMM_API DWORD WINAPI waveInGetDevCapsW(
    UINT uDeviceID,
    UINT pwic,
    UINT cbwic
    )
{
    GetHWinmm();
    pwaveInGetDevCapsW faddr;
    faddr = (pwaveInGetDevCapsW)GetProcAddress(hWinmm, "waveInGetDevCapsW");
    return (faddr)(uDeviceID, pwic, cbwic);
}
extern "C" _INMM_API DWORD WINAPI waveInOpen(
    UINT phwi,
    UINT uDeviceID,
    UINT pwfx,
    UINT dwCallback,
    UINT dwCallbackInstance,
    UINT fdwOpen
    )
{
    GetHWinmm();
    pwaveInOpen faddr;
    faddr = (pwaveInOpen)GetProcAddress(hWinmm, "waveInOpen");
    return (faddr)(phwi, uDeviceID, pwfx, dwCallback, dwCallbackInstance, fdwOpen);
}

extern "C" _INMM_API DWORD WINAPI waveInClose(
    UINT hwi
    )
{
    GetHWinmm();
    pwaveInClose faddr;
    faddr = (pwaveInClose)GetProcAddress(hWinmm, "waveInClose");
    return (faddr)(hwi);
}
