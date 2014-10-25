
#pragma once;


#include <string>
#include <iostream>
#include <ctime>
#include <vector>
#include <map>

#define _USE_MATH_DEFINES
#include <math.h>


#define GLM_SWIZZLE_XYZW
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
//#include "glm/gtc/swizzle.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "glm/gtx/compatibility.hpp"
#include "glm/gtx/matrix_operation.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "glm/gtx/norm.hpp"

#include "SOIL.h"



#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))
#define INVALID_UNIFORM_LOCATION -1

//# define M_PI           3.14159265358979323846  /* pi */
#define ToRadian(x) (float)(((x) * M_PI / 180.0f))
#define ToDegree(x) (float)(((x) * 180.0f / M_PI))
