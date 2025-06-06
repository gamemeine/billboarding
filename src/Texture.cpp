#include "Texture.h"

#include <iostream>
#include <filesystem>

#include <stb_image.h>


Texture::Texture() : _id(0)
{

}


bool Texture::Load(const std::string &filepath, GLint paramWrapS, GLint paramWrapT, GLint paramMinFilter, GLuint paramMagFilter)
{
/*
    paramWrapS, paramWrapT = 
            - GL_CLAMP_TO_EDGE
            - GL_CLAMP_TO_BORDER
            - GL_MIRRORED_REPEAT
            - GL_REPEAT
            - GL_MIRROR_CLAMP_TO_EDGE

    paramMinFilter =
            - GL_NEAREST
            - GL_LINEAR
            - GL_NEAREST_MIPMAP_NEAREST
            - GL_LINEAR_MIPMAP_NEAREST
            - GL_NEAREST_MIPMAP_LINEAR
            - GL_LINEAR_MIPMAP_LINEAR

    paramMagFilter = 
            - GL_NEAREST
            - GL_LINEAR

*/

    int imageWidth;
    int imageHeight;
    int imageChannels;

    stbi_set_flip_vertically_on_load(true);

    auto path = std::filesystem::path(filepath).string();
    unsigned char* image = stbi_load(path.c_str(), &imageWidth, &imageHeight, &imageChannels, STBI_rgb_alpha);

    if (!image)
    {
        std::cerr << "Error loading texture  " << filepath << std::endl;
        return false;
    }
        
    glGenTextures(1, &_id);

    glBindTexture(GL_TEXTURE_2D, _id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, paramWrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, paramWrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, paramMinFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, paramMagFilter);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    if (paramMinFilter == GL_NEAREST_MIPMAP_NEAREST ||
        paramMinFilter == GL_LINEAR_MIPMAP_NEAREST ||
        paramMinFilter == GL_NEAREST_MIPMAP_LINEAR ||
        paramMinFilter == GL_LINEAR_MIPMAP_LINEAR )
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(image);

    return true;
}

GLuint Texture::GetID() const
{
    return _id;
}

void Texture::Bind(unsigned int unit)
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, _id);
}

void Texture::Unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
