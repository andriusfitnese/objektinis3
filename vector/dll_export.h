#pragma once

#include "vector.hpp"
#include "vectorh.h"

#ifdef SPAUSDINIMO_EXPORTS
#define SPAUSDINIMO_API __declspec(dllexport)
#else
#define SPAUSDINIMO_API __declspec(dllimport)
#endif

SPAUSDINIMO_API int g_printMode;


extern "C" {
        SPAUSDINIMO_API void isvedimas(int pas, int pasmv, const Vector<Stud>& grupe);
}