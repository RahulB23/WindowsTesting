#ifndef TEST_DLL_H
#define TEST_DLL_H

#ifdef __cplusplus
extern "C" {
#endif

BOOL WINAPI DllMain(HMODULE hmodule, DWORD reason_for_call, LPVOID lpres);

#ifdef __cplusplus
}
#endif

#endif  // TEST_DLL_H
