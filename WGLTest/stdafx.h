// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#define NOMINMAX
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <cstdint>
#include <assert.h>
#include <map>
#include <vector>
#include <algorithm>

// TODO: reference additional headers your program requires here
#include "../wgl_grabber_gen.h"
#include "../helper/helper.h"
#include "../helper/helper_text.h"
#include "../helper/helper_gldx.h"

#include "af_math.h"
#include "matrix_man.h"
#include "shader_man.h"
#include "water_surface.h"
#include "app.h"
#include "tex_man.h"
