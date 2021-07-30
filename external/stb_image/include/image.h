//
// Created by frumpkin on 4/25/20.
//

#ifndef ARCGAMEENGINE_0_1_0_IMAGE_H
#define ARCGAMEENGINE_0_1_0_IMAGE_H

#include <string>
namespace arc {
    class Image {
    public:
        Image() : _imgData(nullptr) {}

        virtual ~Image();

        void Load(const std::string &path, int *width, int *height, int *channels);

        unsigned char *get_data() const { return _imgData; }

    protected:
        unsigned char *_imgData;
    };
}


#endif //ARCGAMEENGINE_0_1_0_IMAGE_H
