// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the _INMM_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// _INMM_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef _INMM_EXPORTS
#define _INMM_API __declspec(dllexport)
#else
#define _INMM_API __declspec(dllimport)
#endif

// This class is exported from the _inmm.dll
//class _INMM_API C_inmm {
//public:
//	C_inmm(void);
//	// TODO: add your methods here.
//};
//
//extern _INMM_API int n_inmm;

extern "C"
{

//_INMM_API int fn_inmm(void);

_INMM_API DWORD WINAPI mciSendCommandA(
    UINT IDDevice,
    UINT uMsg,
    DWORD fdwCommand,
    DWORD_PTR dwParam
);

_INMM_API DWORD WINAPI timeBeginPeriod(
  UINT uPeriod  
);
_INMM_API DWORD WINAPI timeEndPeriod(
    UINT uPeriod
    );

_INMM_API DWORD WINAPI timeGetDevCaps(
  UINT ptc,  
  UINT cbtc        
);

_INMM_API DWORD WINAPI timeGetTime(void);

_INMM_API DWORD WINAPI timeKillEvent(UINT uTimerID );

_INMM_API DWORD WINAPI timeSetEvent(
UINT           uDelay,      
  UINT           uResolution, 
  UINT lpTimeProc,  
  UINT      dwUser,      
  UINT           fuEvent      
);
_INMM_API DWORD WINAPI waveOutOpen(
    UINT phwo,
    UINT uDeviceID,
    UINT pwfx,
    UINT dwCallback,
    UINT dwCallbackInstance,
    UINT fdwOpen
    );

_INMM_API DWORD WINAPI waveOutClose(
    UINT hwo
    );
_INMM_API DWORD WINAPI waveOutPrepareHeader(
    UINT hwo,
    UINT pwh,
    UINT cbwh
    );
_INMM_API DWORD WINAPI waveOutUnprepareHeader(
    UINT hwo,
    UINT pwh,
    UINT cbwh
    ); 
_INMM_API DWORD WINAPI waveOutWrite(
    UINT hwo,
    UINT pwh,
    UINT cbwh
    );
_INMM_API DWORD WINAPI waveOutGetNumDevs();
_INMM_API DWORD WINAPI waveOutGetDevCapsW(
    UINT uDeviceID,
    UINT pwoc,
    UINT cbwoc
    );
_INMM_API DWORD WINAPI waveOutGetErrorTextW(
    UINT mmrError,
    UINT pszText,
    UINT cchText
    );
_INMM_API DWORD WINAPI waveInGetNumDevs();
_INMM_API DWORD WINAPI waveInGetDevCapsW(
    UINT uDeviceID,
    UINT pwic,
    UINT cbwic
    );
_INMM_API DWORD WINAPI waveInOpen(
    UINT phwi,
    UINT uDeviceID,
    UINT pwfx,
    UINT dwCallback,
    UINT dwCallbackInstance,
    UINT fdwOpen
    );

_INMM_API DWORD WINAPI waveInClose(
    UINT hwi
    );
}




