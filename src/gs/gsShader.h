#ifndef GS_SHADER_H
#define GS_SHADER_H

#include <string>

#include <GL/gl3w.h>

using std::string;

namespace gs
{
    class Shader
    {
    private:
        GLuint          nextAttribLocation;
        bool            linked;

    public:
        const string    name;
        const GLuint    vertexShader;
        const GLuint    fragmentShader;
        const GLuint    program;

    private:
        bool    CompileShader( const GLuint shader, const string& filename );
        string  LoadShaderFromFile( const string &filename ) const;
        void    PrintProgramLog() const;
        void    PrintShaderLog( const GLuint shader ) const;

    public:
        GLuint  GetAttribLocation( const string& attrib );
        GLint   GetUniformLocation( const string& uniform ) const;
        bool    Link();
        void    SetFragOutput( const string& attrib );
        bool    Use() const;

    public:
        Shader( const string& name, const string& vertexFilename, const string& fragmentFilename );
        ~Shader();
    };
}

#endif // FRMR_SHADER_H
