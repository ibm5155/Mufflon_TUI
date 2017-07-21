#pragma once
#include "Text.h"

char *ReadEOFCVAR(Texto *Regra, char *Name);
char *ReadCVAR(Texto *Regra, char *Name);
bool ReadBooleanCVAR(Texto *Regra, char *Name);
int ReadIntegerCVAR(Texto *Regra, char *Name);