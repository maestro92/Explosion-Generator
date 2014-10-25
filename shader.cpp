#include "shader.h"

#include <stdio.h>
#include <string.h>

string dir_path = "./shaders/";
string p_str;


void shader::loadFile(const char* fn, string & str)
{
    p_str = dir_path;
    string mstr(fn);
    p_str = p_str + mstr;
//    cout << "p_str is " << p_str << endl;


    ifstream in(p_str);

    if(!in.is_open())
    {
        cout << "The file " << fn << "cannot be openned\n" << endl;
        return;
    }

    char tmp[300];
    while(!in.eof())
    {
        in.getline(tmp,300);
        str+=tmp;
        str+='\n';
    }

}

// need glew to run Shader
unsigned int shader::loadShader(string& source, unsigned int shaderType)
{
    // id of current shader
    unsigned int id;

    // Specifices the type of shader to be created
    id = glCreateShader(shaderType);

    const char* csource = source.c_str();


    // replace the source code in a shader object
    // 1: number of elements in the string and length arrays
    glShaderSource(id, 1, &csource, NULL);
    glCompileShader(id);

    char error[1000];
    glGetShaderInfoLog(id, 1000, NULL, error);

    cout << "Shader Compile Status: \n" << error << endl;

    return id;
}


shader::shader(const char* vs_source, const char* fs_source)
{
    string source;
    cout << "vs: " << vs_source << ", fs: " << fs_source << endl;
    loadFile(vs_source, source);

    // source code and Mode
    vs = loadShader(source, GL_VERTEX_SHADER);

    source = "";
    // load the fragment shader
    loadFile(fs_source, source);
    fs = loadShader(source, GL_FRAGMENT_SHADER);

    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);

    // links the program object
    glLinkProgram(program);
    // this will use this shader program
    glUseProgram(program);


}





shader::shader(const char* vs_source, const char* gs_source, const char* fs_source)
{
    string source;
    cout << "vs: " << vs_source << ", gs:" << gs_source << ", fs: " << fs_source << endl;
    loadFile(vs_source, source);
    // source code and Mode
    vs = loadShader(source, GL_VERTEX_SHADER);


    source = "";
    // load the geometry shader
    loadFile(gs_source, source);
    gs = loadShader(source, GL_GEOMETRY_SHADER);


    source = "";
    // load the fragment shader
    loadFile(fs_source, source);
    fs = loadShader(source, GL_FRAGMENT_SHADER);


    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, gs);
    glAttachShader(program, fs);


    glBindAttribLocation(program, SlotPosition, "Position");
    glBindAttribLocation(program, SlotTexCoord, "TexCoord");
    // links the program object
    glLinkProgram(program);
    // this will use this shader program
    glUseProgram(program);
}



// deletes the shader
shader::~shader()
{
    glDetachShader(program, vs);
    glDetachShader(program, gs);
    glDetachShader(program, fs);

    glDeleteShader(vs);
    glDeleteShader(gs);
    glDeleteShader(fs);
    glDeleteProgram(program);
}

void shader::linkShader()
{
    // links the program object
    glLinkProgram(program);
}



void shader::useShader()
{
	glUseProgram(program);
}


unsigned int shader::getProgramId()
{
	return program;
}


void shader::delShader()
{
	glUseProgram(0);
}



