#include "EG_Utility.h"

void EG_Utility::debug(string s)
{
    cout << s << endl;
}

void EG_Utility::debug(string s, string s2)
{
    cout << s << endl;
    cout << s2 << endl << endl;
}

void EG_Utility::debug(string s, unsigned int i)
{
    cout << s << "  " << i << endl;
}

void EG_Utility::debug(string s, int i)
{
    cout << s << endl;
    cout << i << endl << endl;
}

void EG_Utility::debug(string s, float f)
{
    cout << s << " " << f << endl;
}

void EG_Utility::debug(string s, glm::vec2 v)
{
    cout << s << endl;
    cout << v.x << " " << v.y << endl << endl;
}

void EG_Utility::debug(string s, glm::vec3 v)
{
    // cout << s << endl;
    // cout << v.x << " " << v.y << " " << v.z << endl << endl;

    cout << s << "  " << v.x << " " << v.y << " " << v.z << endl;
}

void EG_Utility::debug(string s, glm::mat4 m)
{
    cout << s << endl;
    for (int i=0; i<4; i++)
        cout << m[i][0] << " " << m[i][1] << " " << m[i][2] << " " << m[i][3] << endl;

    cout << endl << endl;
}
