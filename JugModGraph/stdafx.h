// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

//#define USE_CV
#define SILENT

#ifdef USE_CV
#include <opencv/cv.h>
#endif

#include "targetver.h"
#include "dpTree.h"
#include "SmartF.h"

#include <vector>
#include <random>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <time.h>
#include <stdexcept>
#include <stdio.h>
#include <tchar.h>




// TODO: reference additional headers your program requires here
