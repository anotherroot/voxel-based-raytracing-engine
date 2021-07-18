//
// Created by martines on 23. 01. 21.
//

#include "../include/image.h"
#ifdef ARC_PLATFORM_ANDROID
#include "SDL.h"
#define STBI_NO_STDIO // We do not need STDIO. Instead we use SDL_RWops to open files
#endif
#define STB_IMAGE_IMPLEMENTATION   // Use of stb functions once and for all
#include "../include/stb_image.h"

#ifdef ARC_PLATFORM_ANDROID
typedef struct {
    SDL_RWops* src;
    stbi_io_callbacks stb_cbs;
    int atEOF; // defaults to 0; 1: reached EOF or error on read, 2: error on seek
} RWops_stb;

// fill 'data' with 'size' bytes.  return number of bytes actually read
static int io_read(void* user, char* data, int size)
{
    RWops_stb* io = (RWops_stb*)user;

    int num = SDL_RWread(io->src, data, sizeof(char), size);
    if(num == 0)
    {
        // we're at EOF or some error happened
        io->atEOF = 1;
    }
    return (int)num*sizeof(char);
}

// skip the next 'n' bytes, or 'unget' the last -n bytes if negative
static void io_skip(void* user, int n)
{
    RWops_stb* io = (RWops_stb*)user;

    if(SDL_RWseek(io->src, n, RW_SEEK_CUR) == -1)
    {
        // an error happened during seeking, hopefully setting EOF will make stb_image abort
        io->atEOF = 2; // set this to 2 for "aborting because seeking failed" (stb_image only cares about != 0)
    }
}

// returns nonzero if we are at end of file/data
static int io_eof(void* user)
{
    RWops_stb* io = (RWops_stb*)user;
    return io->atEOF;
}
#endif

namespace arc {
    Image::~Image() {
        if (_imgData)
            stbi_image_free(_imgData);
    }

    void Image::Load(const std::string &path, int *width, int *height, int *channels) {
        stbi_set_flip_vertically_on_load(1);
        #ifdef ARC_PLATFORM_ANDROID
            SDL_RWops* src = SDL_RWFromFile(path.c_str(), "rb");
            int bppToUse = 0;
            RWops_stb cbData;
            Sint64 srcOffset = 0;
            srcOffset = SDL_RWtell(src);

            // Set EOF and SDL_RWops
            cbData.src = src;
            cbData.atEOF = 0;
            // Set stb_callbacks
            cbData.stb_cbs.read = io_read;
            cbData.stb_cbs.skip = io_skip;
            cbData.stb_cbs.eof = io_eof;

            // Get info about data
            stbi_info_from_callbacks(&cbData.stb_cbs, &cbData, width, height, channels);

            // Go back to start
            SDL_RWseek(src, srcOffset, RW_SEEK_SET);

            // Reset eof so we can read the data
            cbData.atEOF = 0;

            // Check number of channels
            bppToUse = (*channels == 1 || *channels == 3) ? 3 : 4;

            // Read data
            _imgData = stbi_load_from_callbacks(&cbData.stb_cbs, &cbData, width, height, channels, bppToUse);
            *channels = bppToUse;
        #else
            _imgData = stbi_load(path.c_str(), width, height, channels, 0);
        #endif


    }
}
