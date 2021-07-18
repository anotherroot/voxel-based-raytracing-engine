//
// Created by adolin on 23. 01. 21.
//

#ifndef ARC_ENGINE_FUNCTIONS_H
#define ARC_ENGINE_FUNCTIONS_H

namespace arc{
    namespace Color{
        inline glm::vec4 FromHex(unsigned int hex, float alpha = 1){ //TODO: check lifetime of vec4
            unsigned int r = (hex & 0xff0000)>>16;
            unsigned int g = (hex & 0x00ff00)>>8;
            unsigned int b = hex & 0x0000ff;

            return {(float)r/255.0f,(float)g/255.0f,(float)b/255.0f,alpha};
        }

        inline void FromHex(unsigned int hex, uint8_t* data, uint8_t alpha = 255){ //TODO: check lifetime of vec4
            unsigned int r = (hex & 0xff0000)>>16;
            unsigned int g = (hex & 0x00ff00)>>8;
            unsigned int b = hex & 0x0000ff;
            data[0] = r;
            data[1] = g;
            data[2] = b;
            data[3] = alpha;
        }

        inline glm::vec3 RGBToHSV(const glm::vec3& rgb) {
            glm::vec3 hsv;
            float fCMax = std::max(std::max(rgb.r, rgb.g), rgb.b);
            float fCMin = std::min(std::min(rgb.r, rgb.g), rgb.b);
            float fDelta = fCMax - fCMin;

            if(fDelta > 0.0f) {
                if(fCMax == rgb.r) {
                    hsv.x = 60.0f * (fmodf(((rgb.g - rgb.b) / fDelta), 6.0f));
                } else if(fCMax == rgb.g) {
                    hsv.x = 60.0f * (((rgb.b - rgb.r) / fDelta) + 2.0f);
                } else if(fCMax == rgb.b) {
                    hsv.x = 60.0f * (((rgb.r - rgb.g) / fDelta) + 4.0f);
                }

                if(fCMax > 0.0f) {
                    hsv.y = fDelta / fCMax;
                } else {
                    hsv.y = 0.0f;
                }

                hsv.z = fCMax;
            } else {
                hsv.x = 0.0f;
                hsv.y = 0.0f;
                hsv.z = fCMax;
            }

            if(hsv.x < 0.0f) {
                hsv.x += 360.0f;
            }
            return hsv;
        }

        inline glm::vec3 HSVToRGB(const glm::vec3& hsv){
            float H = hsv.x, S = hsv.y, V = hsv.z;
            float s = S;
            float v = V;
            float C = s*v;
            float X = C*(1.0f-std::abs(fmodf(H/60.0f, 2.0f)-1.0f));
            float m = v-C;
            float r,g,b;
            if(H >= 0 && H < 60){
                r = C,g = X,b = 0;
            }
            else if(H >= 60 && H < 120){
                r = X,g = C,b = 0;
            }
            else if(H >= 120 && H < 180){
                r = 0,g = C,b = X;
            }
            else if(H >= 180 && H < 240){
                r = 0,g = X,b = C;
            }
            else if(H >= 240 && H < 300){
                r = X,g = 0,b = C;
            }
            else{
                r = C,g = 0,b = X;
            }
            return {(r+m),(g+m),(b+m)};
        }

        inline glm::vec4 InterpolateArc(unsigned int hex_from, unsigned int hex_to, float value, float alpha_from = 1, float alpha_to = 1){
            glm::vec3 hsv_from = RGBToHSV(FromHex(hex_from));
            glm::vec3 hsv_to = RGBToHSV(FromHex(hex_to));

            hsv_to = hsv_from*(1-value) + hsv_to*value;
            alpha_to = alpha_from*(1-value) + alpha_to*value;

            auto rgb = HSVToRGB(hsv_to);
            return glm::vec4(rgb,alpha_to);
        }


        inline glm::vec4 InterpolateRGB(unsigned int hex_from, unsigned int hex_to, float value, float alpha_from = 1, float alpha_to = 1){
            glm::vec3 rgb = FromHex(hex_from)*(1-value) + FromHex(hex_to)*value;
            alpha_to = alpha_from*(1-value) + alpha_to*value;
            return glm::vec4(rgb,alpha_to);
        }


        inline void InterpolateArc(unsigned int hex_from, unsigned int hex_to, float value, uint8_t *data,float alpha_from = 1, float alpha_to = 1){
            glm::vec3 hsv_from = RGBToHSV(FromHex(hex_from));
            glm::vec3 hsv_to = RGBToHSV(FromHex(hex_to));

            hsv_to = hsv_from*(1-value) + hsv_to*value;
            alpha_to = alpha_from*(1-value) + alpha_to*value;

            auto rgb = HSVToRGB(hsv_to);

            data[0] = rgb.r*255;
            data[1] = rgb.g*255;
            data[2] = rgb.b*255;
            data[3] = alpha_to*255;
        }


    }
}
#endif //ARC_ENGINE_FUNCTIONS_H
