#ifndef EG_ORBIT_CAMERA_H_
#define EG_ORBIT_CAMERA_H_



#include "EG_ThirdPersonPovCamera.h"


#define NO_SDL_GLEXT
#include <GL/glew.h>

#include "EG_Camera.h"
#include "EG_Skybox.h"
#include "sceneLoader.h"

#include <cmath>
#include <iostream>
#include <SDL/SDL.h>
#include "pipeline.h"
#include "define.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include "EG_Renderer.h"
#include "EG_WorldAnimatedObject.h"
#include "EG_Model.h"
#include "EG_ImportedAnimatedModel.h"
#include <algorithm>
/*

// Orbit mode rotates the camera around a target position. This mode can be
// used to simulate a third person camera. Orbit mode supports 2 modes of
// operation: orbiting about the target's Y axis, and free orbiting. The former
// mode only allows pitch and yaw. All yaw changes are relative to the target's
// local Y axis. This means that camera yaw changes won't be affected by any
// rolling. The latter mode allows the camera to freely orbit the target. The
// camera is free to pitch, yaw, and roll. All yaw changes are relative to the
// camera's orientation (in space orbiting the target).

*/



using namespace std;


class EG_OrbitCamera : public EG_ThirdPersonPovCamera
{
    public:
        EG_OrbitCamera();
        ~EG_OrbitCamera();

        glm::vec3 m_pivotOffset;

        glm::vec2 m_mouseCurrent;
        glm::vec2 m_mousePrevious;
        glm::vec2 m_mouseDelta;

        float maxf(float f1, float f2);
        float minf(float f1, float f2);

        float m_mouseMovementScale;
        bool m_leftMouseDown;
        bool f;




        virtual void Control(pipeline& m_pipeline, EG_SkyBox& skybox);
        virtual void Control(pipeline& m_pipeline, EG_SkyBox& skybox, bool GuiFlag);
        virtual void Control(pipeline& m_pipeline, EG_SkyBox& skybox, int screen_w, int screen_h, int palette_w, int palette_h);
        void update(pipeline& m_pipeline, float elapsedTimeSec, float pitchChange, float yawChange, EG_SkyBox& skybox);
};



#endif







