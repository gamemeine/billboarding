#pragma once

#include <string>
#include <glad/glad.h>

class Texture
{
public:
    Texture();

    bool Load(
        const std::string& filepath, 
        GLint paramWrapS, 
        GLint paramWrapT, 
        GLint paramMinFilter, 
        GLuint paramMagFilter
    );

    GLuint GetID() const;
    void Bind(unsigned int unit);
    void Unbind();

private:
    GLuint _id;
};