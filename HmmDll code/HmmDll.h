/*********************************************************************************************/
// HmmDll.h : Defines the exported functions for the DLL application.
//
/*********************************************************************************************/
#pragma once

#ifdef HMMDLL_EXPORTS
#define HMMDLL_API __declspec(dllexport)
#else
#define HMMDLL_API __declspec(dllimport)
#endif

extern "C" HMMDLL_API void createSeq(int , const char*);

extern "C" HMMDLL_API int startLoad();

extern "C" HMMDLL_API int checkProgrameState();

extern "C" HMMDLL_API void getLastDate(char* , int );

extern "C" HMMDLL_API double getPrediction(int, const char*, int);

extern "C" HMMDLL_API double getStatPrediction(int , int , const char* , int , int , const char* );
