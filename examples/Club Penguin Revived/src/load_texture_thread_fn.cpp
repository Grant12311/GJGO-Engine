#include <string>

#include <stb/stb_image.h>

#include <CPRV/loaded_texture_details.hpp>

LoadedTextureDetails loadTextureThreadFn(const std::string &a_path)
{
    LoadedTextureDetails toReturn;
    stbi_set_flip_vertically_on_load(false);
    toReturn.data = stbi_load(a_path.c_str(), &toReturn.width, &toReturn.height, &toReturn.bpp, 4);
    return toReturn;
}
