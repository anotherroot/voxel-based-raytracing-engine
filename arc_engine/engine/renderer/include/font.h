//
// Created by adolin on 3. 01. 21.
//

#ifndef ARC_ENGINE_FONT_H
#define ARC_ENGINE_FONT_H
#include <glm/glm.hpp>
#include "orthographic_camera.h"
#include "vertex_array.h"
#include "shader.h"
#include "texture.h"
#include <map>
namespace arc{

    struct Character {
        float ax; // advance.x
        float ay; // advance.y

        float bw; // bitmap.width;
        float bh; // bitmap.rows;

        float bl; // bitmap_left;
        float bt; // bitmap_top;

        float tx; // x offset of glyph in texture coordinates
    };



    class Font: public Caller{
        typedef void(VoidMemberUintFn)(void*,uint);
    public:
        void Create(const std::string& font_file,int font_height = 48);
        ~Font();
        inline void Dispose(){
            if(initialized){
                //TODO: dispose
                characters_.clear();
            }
        }
        void Bind(int slot);
        inline VoidMemberUintFn* GetBindFunction() const{
            return (VoidMemberUintFn*)GetVTable(this)[3];
        }




    private:
        bool initialized{false};
        std::map<char, Character> characters_;
        uint render_id_;
        uint width_, height_;
        Texture texture;


        friend class BatchRenderer;
    };
}


#endif //ARC_ENGINE_FONT_H
