#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <utility>

#include <glad/glad.h>

#include <stb/stb_image.h>

#include <GJGO/log.hpp>
#include <GJGO/texture.hpp>

namespace GJGO
{
    static std::unordered_map<std::string, Texture*> textures;

    Texture::~Texture()
    {
        glDeleteTextures(1, &this->p_ID);
    }

    [[nodiscard]]
    unsigned int Texture::getID()
    {
        return this->p_ID;
    }

    [[nodiscard]]
    unsigned int Texture::getSettings()
    {
        return this->p_settings;
    }

    void Texture::shutdown()
    {
        for (const std::pair<std::string, Texture*> &a_pair : textures)
        {
            delete a_pair.second;
        }
    }

    void Texture2D::bind(const unsigned int a_slot)
    {
        glActiveTexture(GL_TEXTURE0 + a_slot);
        glBindTexture(GL_TEXTURE_2D, this->p_ID);
    }

    void Texture2D::unbind()
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Texture2D::setMagFilter(const unsigned int a_magFilter)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, a_magFilter);
    }

    void Texture2D::setMinFilter(const unsigned int a_minFilter)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, a_minFilter);
    }

    void Texture2D::setFilters(const unsigned int a_minFilter, const unsigned int a_magFilter)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, a_minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, a_magFilter);
    }

    Texture2D* Texture2D::create(const std::string &a_path, const unsigned int a_settings, const unsigned int a_minFilter, const unsigned int a_magFilter, const unsigned int a_wrapS, const unsigned int a_wrapT)
    {
        Texture2D* texture;
        int width, height, bpp;

        stbi_set_flip_vertically_on_load(a_settings & TextureSettings::flipY);
        unsigned char* const data = stbi_load(a_path.c_str(), &width, &height, &bpp, 4);

        if (data)
        {
            texture = create(a_path, data, width, height, a_settings, a_minFilter, a_magFilter, a_wrapS, a_wrapT);
            stbi_image_free(data);
        }else{
            GJGO_LOG_ERROR("Texture at ", a_path, " could not be created!");
        }

        return texture;
    }

    Texture2D* Texture2D::create(const std::string &a_name, unsigned char* const a_data, const int a_width, const int a_height, const unsigned int a_settings, const unsigned int a_minFilter, const unsigned int a_magFilter, const unsigned int a_wrapS, const unsigned int a_wrapT)
    {
        assert(textures.find(a_name) == textures.end());
        assert(a_data);

        Texture2D* const texture = new Texture2D;
        textures[a_name] = texture;

        texture->p_width = a_width;
        texture->p_height = a_height;

        texture->p_settings = a_settings;

        glGenTextures(1, &texture->p_ID);
        texture->bind();
        texture->setFilters(a_minFilter, a_magFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, a_wrapS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, a_wrapT);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->p_width, texture->p_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, a_data);
        glGenerateMipmap(GL_TEXTURE_2D);

        return texture;
    }

    [[nodiscard]]
    Texture2D* Texture2D::get(const std::string &a_path)
    {
        assert(textures.find(a_path) != textures.end());
        return static_cast<Texture2D*>(textures[a_path]);
    }
}
