#ifndef GJGO_TEXTURE_H
#define GJGO_TEXTURE_H

#include <glad/glad.h>

namespace GJGO
{
    namespace TextureSettings
    {
        constexpr unsigned int standard = 0;
        constexpr unsigned int flipY = 1 << 0;
        constexpr unsigned int hasTransparency = 1 << 1;
    };

    class Texture
    {
    public:
        virtual ~Texture();

        virtual void bind(const unsigned int a_slot = 0) = 0;
        virtual void unbind() = 0;

        virtual void setMagFilter(const unsigned int a_magFilter) = 0;
        virtual void setMinFilter(const unsigned int a_minFilter) = 0;
        virtual void setFilters(const unsigned int a_magFilter, const unsigned int a_minFilter) = 0;

        unsigned int getID();
        unsigned int getSettings();
    protected:
        unsigned int p_ID;
        int p_width, p_height;
        unsigned int p_settings;
    };

    class Texture2D : public Texture
    {
    public:
        virtual void bind(const unsigned int a_slot = 0) override;
        virtual void unbind() override;

        virtual void setMagFilter(const unsigned int a_magFilter) override;
        virtual void setMinFilter(const unsigned int a_minFilter) override;
        virtual void setFilters(const unsigned int a_minFilter, const unsigned int a_magFilter) override;

        static Texture2D* create(const std::string &a_path, const unsigned int a_settings = TextureSettings::hasTransparency, const unsigned int a_minFilter = GL_LINEAR, const unsigned int a_magFilter = GL_LINEAR, const unsigned int a_wrapS = GL_CLAMP_TO_EDGE, const unsigned int a_wrapT = GL_CLAMP_TO_EDGE);
        static Texture2D* create(const std::string &a_name, unsigned char* const a_data, const int a_width, const int a_height, const unsigned int a_settings = TextureSettings::hasTransparency, const unsigned int a_minFilter = GL_LINEAR, const unsigned int a_magFilter = GL_LINEAR, const unsigned int a_wrapS = GL_CLAMP_TO_EDGE, const unsigned int a_wrapT = GL_CLAMP_TO_EDGE);

        static Texture2D* get(const std::string &a_path);
    };
}

#endif
