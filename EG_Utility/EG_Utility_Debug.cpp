#include "EG_Utility.h"


void EG_Utility::debugLn(int l)
{
    for(int i=0; i<l; i++)
        cout << endl;
}

void EG_Utility::debugLn(string s, int l)
{
    debug(s);
    debugLn(l);
}

void EG_Utility::debug(string s)
{
    cout << s << endl;
}

void EG_Utility::debug(string s, string s2)
{
    cout << s << "  "  << s2 << endl;
}

void EG_Utility::debug(string s, unsigned int i)
{
    cout << s << "  " << i << endl;
}

void EG_Utility::debug(string s, int i)
{
    cout << s << " " << i << endl;
}

void EG_Utility::debug(string s, float f)
{
    cout << s << " " << f << endl;
}

void EG_Utility::debug(string s, glm::vec2 v)
{
    cout << s << " " << v.x << " " << v.y << endl << endl;
}

void EG_Utility::debug(string s, glm::vec3 v)
{
    cout << s << "  " << v.x << " " << v.y << " " << v.z << endl;
}

void EG_Utility::debug(string s, glm::vec4 v)
{
    cout << s << "  " << v.x << " " << v.y << " " << v.z << " " << v.w << endl;
}

void EG_Utility::debug(string s, glm::mat3 m)
{
    cout << s << endl;
    for (int i=0; i<3; i++)
        cout << m[i][0] << " " << m[i][1] << " " << m[i][2] << endl;

    cout << endl << endl;
}

void EG_Utility::debug(string s, glm::mat4 m)
{
    cout << s << endl;
    for (int i=0; i<4; i++)
        cout << m[i][0] << " " << m[i][1] << " " << m[i][2] << " " << m[i][3] << endl;

    cout << endl << endl;
}
