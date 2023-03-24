#pragma once

#ifdef __cplusplus
#include <cstdio>
#else
#include <stdio.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

int install_id();
int install_num();

extern FILE *yyin;

extern int yylex(void);

#ifdef __cplusplus
}
#endif
