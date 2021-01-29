#include <GJGOpch.hpp>

#include <GJGO/log.hpp>
#include <GJGO/profiler.hpp>
#include <GJGO/texture.hpp>

namespace GJGO
{
    static std::unordered_map<std::string, std::unique_ptr<Texture>> textures;

    Texture::~Texture()
    {
        GJGO_PROFILE_FUNCTION();

        glDeleteTextures(1, &this->p_ID);
    }

    unsigned int Texture::getID()
    {
        GJGO_PROFILE_FUNCTION();

        return this->p_ID;
    }

    Texture* Texture::get(const std::string &a_path)
    {
        GJGO_PROFILE_FUNCTION();

        assert(textures.find(a_path) != textures.end());
        return textures[a_path].get();
    }

    void Texture2D::bind(const unsigned int a_slot)
    {
        GJGO_PROFILE_FUNCTION();

        glActiveTexture(GL_TEXTURE0 + a_slot);
        glBindTexture(GL_TEXTURE_2D, this->p_ID);
    }

    void Texture2D::unbind()
    {
        GJGO_PROFILE_FUNCTION();

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Texture2D::setMagFilter(const unsigned int a_magFilter)
    {
        GJGO_PROFILE_FUNCTION();

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, a_magFilter);
    }

    void Texture2D::setMinFilter(const unsigned int a_minFilter)
    {
        GJGO_PROFILE_FUNCTION();

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, a_minFilter);
    }

    void Texture2D::setFilters(const unsigned int a_minFilter, const unsigned int a_magFilter)
    {
        GJGO_PROFILE_FUNCTION();

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, a_minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, a_magFilter);
    }

    Texture2D* Texture2D::create(const std::string &a_path, const bool a_flipY, const unsigned int a_minFilter, const unsigned int a_magFilter, const unsigned int a_wrapS, const unsigned int a_wrapT)
    {
        GJGO_PROFILE_FUNCTION();

        assert(textures.find(a_path) == textures.end());

        Texture2D* const texture = new Texture2D;
        textures[a_path].reset(texture);

        stbi_set_flip_vertically_on_load(a_flipY);
        unsigned char* const data = stbi_load(a_path.c_str(), &texture->p_width, &texture->p_height, &texture->p_bpp, 4);

        if (data)
        {
            glGenTextures(1, &texture->p_ID);
            texture->bind();
            texture->setFilters(a_minFilter, a_magFilter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, a_wrapS);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, a_wrapT);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->p_width, texture->p_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            stbi_image_free(data);
        }else{
            GJGO_LOG_ERROR("Texture at ", a_path, " could not be created!");
        }

        return texture;
    }
}
