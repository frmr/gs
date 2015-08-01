#include <fstream>
#include <iostream>
#include <sstream>

#include "gsShader.h"

using std::cerr;
using std::endl;

bool gs::Shader::CompileShader( const GLuint shader, const string& filename )
{
    const string sourceString = LoadShaderFromFile( filename );
    const char* const sourceChar = sourceString.c_str();
    const GLint length = sourceString.size();
    glShaderSource( shader, 1, &sourceChar, &length );
    glCompileShader( shader );
    PrintShaderLog( shader );

    GLint status = 0;
    glGetShaderiv( shader, GL_COMPILE_STATUS, &status );
    if ( status == GL_FALSE )
    {
        cerr << "gs::Shader::CompileShader() in gsShader.cpp: Failed to compile " << filename << " when loading " << name << "." << endl;
        return false;
    }
    return true;
}

string gs::Shader::LoadShaderFromFile( const string &filename ) const
{
    std::ifstream shaderFile( filename );
    std::ostringstream buffer;
    buffer << shaderFile.rdbuf();
    return buffer.str();
}

void gs::Shader::PrintProgramLog() const
{
    GLsizei length;
    glGetProgramiv( program, GL_INFO_LOG_LENGTH, &length );
    GLchar* log = new GLchar[length+1];
    glGetProgramInfoLog( program, length, &length, log );
    log[length] = (GLchar)'\0';
    fprintf( stderr, "%s\n", log );
    delete[] log;
}

void gs::Shader::PrintShaderLog( const GLuint shader ) const
{
    GLsizei length;
    glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &length );
    GLchar* log = new GLchar[length+1];
    glGetShaderInfoLog( shader, length, &length, log );
    log[length] = (GLchar)'\0';
    fprintf( stderr, "%s\n",  log );
    delete[] log;
}

GLuint gs::Shader::GetAttribLocation( const string& attrib )
{
    if ( linked )
    {
        cerr << "gs::Shader::GetUniformLocation() in gsShader.cpp: Cannot bind attrib " << attrib << " in " << name << " after linking." << endl;
        return 0;
    }
    glBindAttribLocation( program, nextAttribLocation, attrib.c_str() );
    return nextAttribLocation++;
}

GLint gs::Shader::GetUniformLocation( const string& uniform ) const
{
    if ( !linked )
    {
        cerr << "gs::Shader::GetUniformLocation() in gsShader.cpp: Cannot get uniform variable " << uniform << " before linking in " << name << "." << endl;
        return -1;
    }
    GLint location = glGetUniformLocation( program, uniform.c_str() );
    if ( location == -1 )
    {
        cerr << "gs::Shader::GetUniformLocation() in gsShader.cpp: Failed to get uniform variable " << uniform << " in " << name << "." << endl;
    }
    return location;
}

bool gs::Shader::Link()
{
    glLinkProgram( program );
    PrintProgramLog();

    glDeleteShader( vertexShader );
    glDeleteShader( fragmentShader );

    GLint status = 0;
    glGetProgramiv( program, GL_LINK_STATUS, &status );

    if ( status == GL_FALSE )
    {
        cerr << "gs::Shader::Link() in gsShader.cpp: Failed to link " << name << "." << endl;
        return false;
    }

    linked = true;
    return true;
}

void gs::Shader::SetFragOutput( const string& attrib )
{
    glBindFragDataLocation( program, 0, attrib.c_str() );
}

bool gs::Shader::Use() const
{
    if ( !linked )
    {
        cerr << "gs::Shader::Use() in gsShader.cpp: Cannot use " << name << " before linking." << endl;
        return false;
    }
    glUseProgram( program );
    return true;
}

gs::Shader::Shader( const string& name, const string& vertexFilename, const string& fragmentFilename )
    :   nextAttribLocation( 1 ),
        linked( false ),
        name( name ),
        vertexShader( glCreateShader( GL_VERTEX_SHADER ) ),
        fragmentShader( glCreateShader( GL_FRAGMENT_SHADER ) ),
        program( glCreateProgram() )
{
    CompileShader( vertexShader, vertexFilename );
    CompileShader( fragmentShader, fragmentFilename );
    glAttachShader( program, vertexShader );
    glAttachShader( program, fragmentShader );
}

gs::Shader::~Shader()
{
    glDeleteProgram( program ); //TODO: uncomment
}
