#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Constants.h"
#include "Types.h"

func_result_t loadVersion(buf_t* verInfo, FILE* verFile); //положить в verInfo все доступные данные
func_result_t handleFile(char fname[FNAME_LEN], int version, buf_t* verInfo);