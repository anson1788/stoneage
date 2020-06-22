#ifndef _MMOGprotect_H
#define _MMOGprotect_H

/*#ifdef MMOGPROTECT_EXPORTS
  #define MMOGP_API __declspec(dllexport)
#else
  #define MMOGP_API __declspec(dllimport)
#endif
*/
#define MMOGP_API

extern MMOGP_API BOOL DisableCheated(void);

extern MMOGP_API void InitialInputScript(void);
extern MMOGP_API void QuitingInputScript(void);
extern MMOGP_API BOOL DisableInputScript(void);
extern MMOGP_API BOOL IsLMouseButtonDown(void);
extern MMOGP_API BOOL IsRMouseButtonDown(void);

extern MMOGP_API BOOL ChangedLibrary(void);
extern MMOGP_API BOOL RestoreLibrary(void);

extern MMOGP_API void InitRestore(void);
extern MMOGP_API void QuitRestore(void);
extern MMOGP_API BOOL RestoreCounter(int ratio);

#endif