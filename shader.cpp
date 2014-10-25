#include "shader.h"






void shader::loadFile(const char* fn, string & str)
{
    ifstream in(fn);

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


// deletes the shader
shader::~shader()
{
    glDetachShader(program, vs);
    glDetachShader(program, fs);
    glDeleteShader(vs);
    glDeleteShader(fs);
    glDeleteProgram(program);
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



