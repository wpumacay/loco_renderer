

#pragma once

#include <CCommon.h>
#include <shaders/LShader.h>
#include <shaders/CShaderUniforms.h>

using namespace std;

namespace engine
{

    struct LShaderManager
    {

        private :

        LShaderManager();
        static LShaderManager* _INSTANCE;

        map< string, GLuint > m_programs;
        map< string, LShader* > m_programObjs;

        public :


        static void create();
        static void release();

        ~LShaderManager();

        GLuint createShader( const char* filename, int shaderType );
        GLuint createProgram( GLuint vShader, GLuint fShader );

        static LShader* getShader( const string& shaderName );
    };

}