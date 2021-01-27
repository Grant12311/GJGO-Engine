#ifndef GJGO_TEXTURE_H
#define GJGO_TEXTURE_H

namespace GJGO
{
    class Texture2D;

    class Texture
    {
    protected:
        unsigned int p_ID;
        int p_width, p_height, p_bpp;
    public:
        virtual ~Texture();

        virtual void bind(const unsigned int a_slot = 0) = 0;
        virtual void unbind() = 0;

        virtual void setMagFilter(const unsigned int a_magFilter) = 0;
        virtual void setMinFilter(const unsigned int a_minFilter) = 0;
        virtual void setFilters(const unsigned int a_magFilter, const unsigned int a_minFilter) = 0;

        unsigned int getID();

        static Texture* get(const std::string &a_path);
    };

    class Texture2D : public Texture
    {
    public:
        virtual void bind(const unsigned int a_slot = 0) override;
        virtual void unbind() override;

        virtual void setMagFilter(const unsigned int a_magFilter) override;
        virtual void setMinFilter(const unsigned int a_minFilter) override;
        virtual void setFilters(const unsigned int a_minFilter, const unsigned int a_magFilter) override;

        static Texture2D* create(const std::string &a_path, const bool a_flipY = false, const unsigned int a_minFilter = GL_LINEAR, const unsigned int a_magFilter = GL_LINEAR, const unsigned int a_wrapS = GL_CLAMP_TO_EDGE, const unsigned int a_wrapT = GL_CLAMP_TO_EDGE);
    };
}

#endif
