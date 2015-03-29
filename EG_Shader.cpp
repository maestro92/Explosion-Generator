#include "EG_Shader.h"

#include <stdio.h>
#include <string.h>

string dir_path = "./shaders/";
string p_str;




Shader::Shader(const char* vs_source, const char* fs_source)
{
    string source;
    cout << "vs: " << vs_source << ", fs: " << fs_source << endl;
    loadFile(vs_source, source);

    // source code and Mode
    vs = loadShader(source, GL_VERTEX_SHADER);

    source = "";
    // load the fragment Shader
    loadFile(fs_source, source);
    fs = loadShader(source, GL_FRAGMENT_SHADER);

    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);

    // links the program object
    glLinkProgram(program);
    // this will use this Shader program
    glUseProgram(program);


}



Shader::Shader(const char* vs_source, const char* gs_source, const char* fs_source)
{
    string source;
    cout << "vs: " << vs_source << ", gs:" << gs_source << ", fs: " << fs_source << endl << endl;
    loadFile(vs_source, source);
    // source code and Mode
    vs = loadShader(source, GL_VERTEX_SHADER);


    source = "";
    // load the geometry Shader
    loadFile(gs_source, source);
    gs = loadShader(source, GL_GEOMETRY_SHADER);


    source = "";
    // load the fragment Shader
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
    // this will use this Shader program
    glUseProgram(program);
}



// deletes the Shader
Shader::~Shader()
{
    glDetachShader(program, vs);
    glDetachShader(program, gs);
    glDetachShader(program, fs);

    glDeleteShader(vs);
    glDeleteShader(gs);
    glDeleteShader(fs);
    glDeleteProgram(program);
}


void Shader::loadFile(const char* fn, string & str)
{
    p_str = dir_path;
    string mstr(fn);
    p_str = p_str + mstr;
 //   cout << "p_str is " << endl << p_str << endl;


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

    //    if(p_str == "./shaders/shadow_SecondRender_dir_pt_include.fs")
     //   {
       //     cout << tmp << endl;
        if(loadIncludeFiles(str, tmp))
            continue;
      //  }

        str+=tmp;
        str+='\n';
    }

/*
    if(p_str == "./shaders/shadow_SecondRender_dir_pt_include.fs")
    {
        cout << str << endl;
    }
*/
}

/*
void Shader::loadFile(const char* fn, string & str)
{
    p_str = dir_path;
    string mstr(fn);
    p_str = p_str + mstr;
 //   cout << "p_str is " << endl << p_str << endl;


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

    if(p_str == "./shaders/shadow_SecondRender_dir_pt_include.fs")
    {
        cout << str << endl;
    }
}
*/


bool Shader::loadIncludeFiles(string & str, string newLine)
{

    istringstream iss(newLine);

    int i = 0;
    string includeFileName;

    bool includeFlag = false;

    while(iss)
    {
        string s;
        iss >> s;
        if(s == "#include")
            includeFlag = true;
       //     cout << newLine << endl;

        if(i==1 && includeFlag)
        {
            includeFileName = s.substr(1,s.length()-2);
            cout << includeFileName << endl;

            const char* f = includeFileName.c_str();
            loadFile(f, str);
 //           cout << str << endl;
        }
        i++;
    }


/*
    string delimiter = " ";
    istringstream iss(newLine);
    string s;

    while(getline(iss, s, ' '))
    {
        if(s == "#include")
            cout << newLine << endl;
    }
*/
    return includeFlag;
}




// need glew to run Shader
unsigned int Shader::loadShader(string& source, unsigned int ShaderType)
{
    // id of current Shader
    unsigned int id;

    // Specifices the type of Shader to be created
    id = glCreateShader(ShaderType);

    const char* csource = source.c_str();


    // replace the source code in a Shader object
    // 1: number of elements in the string and length arrays
    glShaderSource(id, 1, &csource, NULL);
    glCompileShader(id);

    char error[1000];
    glGetShaderInfoLog(id, 1000, NULL, error);

   // if(strlen(error) != 0)
   // {
        cout << "Shader Compile Status: \n" << error << endl;
   // }

    return id;
}


void Shader::linkShader()
{
    // links the program object
    glLinkProgram(program);
}



void Shader::useShader()
{
	glUseProgram(program);
}


unsigned int Shader::getProgramId()
{
	return program;
}


void Shader::delShader()
{
	glUseProgram(0);
}



