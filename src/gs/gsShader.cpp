#include <fstream>
#include <iostream>

#include "gsShader.h"

using std::cerr;
using std::endl;

bool gs::Shader::CompileShader( const GLuint shader, const string& filename )
{
    const char* source = LoadShaderFromFile( filename ).c_str();
    glShaderSource( shader, 1, &source, NULL );
    glCompileShader( shader );

    GLint success = 0;
    glGetShaderiv( shader, GL_COMPILE_STATUS, &success );
    if ( success != 0 )
    {
        cerr << "gs::Shader::CompileShader() in gsShader.cpp: Failed to compile " << filename << " when loading " << name << "." << endl;
        return false;
    }

    return true;
}

string gs::Shader::LoadShaderFromFile( const string &filename ) const
{
    string content;
    std::ifstream fileStream( filename.c_str(), std::ios::in );

    if( !fileStream.is_open() )
    {
        cerr << "gs::Shader::LoadShaderFromFile() in gsShader.cpp: Could not open " << filename << " when loading " << name << "." << endl;
        return "";
    }

    string line = "";
    while( !fileStream.eof() )
    {
        std::getline( fileStream, line );
        content.append( line + "\n" );
    }

    fileStream.close();
    return content;
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
    glDeleteShader( vertexShader );
    glDeleteShader( fragmentShader );

    int success = 0;
    glGetProgramiv( program, GL_LINK_STATUS, &success );
    if ( success != 0 )
    {
        cerr << "gs::Shader::Link() in gsShader.cpp: Failed to link " << name << "." << endl;
        return false;
    }

    linked = true;
    return true;
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
    :   nextAttribLocation( 0 ),
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
    glDeleteProgram( program );
}
