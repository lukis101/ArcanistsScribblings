
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <GL/glew.h>

#include "globals.h"
#include "glsl.h"

unsigned sh_program;

unsigned sh_projmat;
unsigned sh_transmat;

unsigned sh_mode;
unsigned sh_texture;
unsigned sh_mask;
unsigned sh_texsize;
unsigned sh_coords1;
unsigned sh_coords2;
unsigned sh_color1;
unsigned sh_color2;
unsigned sh_color3;
unsigned sh_color4;

unsigned sh_v_pos;
unsigned sh_v_uv;
unsigned sh_v_color;

glm::mat4 ProjMat = glm::ortho( 0.f, float(SCREEN_W), float(SCREEN_H), 0.f, -100.f, 100.f );

std::string ReadFromFile( std::string filename )
{
    std::ifstream file( filename );
	if( file )
	{
	    std::stringstream ss;
        ss << file.rdbuf();
        return ss.str();
    }
    else
        std::cout<<"Unable to read from \""<<filename<<"\"!\n";
    return "";
}

void OutputShaderLog( unsigned id, std::string filename="" )
{
    int loglength=0, maxlength=0;
    glGetShaderiv( id, GL_INFO_LOG_LENGTH, &maxlength );
    char* log = new char[ maxlength ];
    glGetShaderInfoLog( id, maxlength, &loglength, log );
    if( loglength > 0 )
    {
        if ( filename!="" ) std::cout<<filename<<"\n";
        std::cout<<"Log:\n"<<log<<"\n";
    }
    delete[] log;
}

unsigned CreateShader( int type, std::string filename )
{
    unsigned id = glCreateShader( type );
    std::string source = ReadFromFile( filename );
    if ( source != "" )
    {
        const char* csource = source.c_str();
        glShaderSource( id, 1, &csource, NULL );
        glCompileShader( id );

        GLint status = GL_FALSE;
        glGetShaderiv( id, GL_COMPILE_STATUS, &status );
        if( status != GL_TRUE )
        {
            std::cout<<"Unable to compile \""<<filename<<"\"!\n";
            OutputShaderLog(id);

            // Delete traces
            glDeleteShader( id );
            return 0;
        }
        else if ( DEBUG_GLSL )
        {
            OutputShaderLog(id,filename);
        }
    }
    else return 0;

    return id;
}

