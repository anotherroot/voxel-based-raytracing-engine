//
// Created by adolin on 26. 12. 20.
//


#include <glfw3.h>
#include <unordered_set>
#include "engine.h"
#include "input.h"

namespace arc {

    static std::unordered_set<int> s_pressed_keys;

    bool Input::IsKeyPressed(int key_code, bool repeat) {
        auto window = static_cast<GLFWwindow *> (Engine::window().native_window());

        auto state = glfwGetKey(window, key_code);

        if (s_pressed_keys.find(key_code) == s_pressed_keys.end()) {
            if (state == GLFW_PRESS || state == GLFW_REPEAT) {
                s_pressed_keys.insert(key_code);
                return state == GLFW_PRESS || repeat && state == GLFW_REPEAT;
            } else {
                return false;
            }
        } else {
            if (state == GLFW_PRESS || state == GLFW_REPEAT) {
                return repeat;
            } else {
                s_pressed_keys.erase((int) key_code);
                return false;
            }
        }

    }

    bool Input::IsMouseButtonPressed(int button) {
        auto window = static_cast<GLFWwindow *> (Engine::window().native_window());
        auto state = glfwGetMouseButton(window, button);

        return state == GLFW_PRESS;
    }

    float Input::GetMouseX() {
        return GetMousePosition().first;
    }

    float Input::GetMouseY() {
        return GetMousePosition().second;
    }

    std::pair<float, float> Input::GetMousePosition() {
        auto window = static_cast<GLFWwindow *> (Engine::window().native_window());
        double x_pos, y_pos;
        glfwGetCursorPos(window, &x_pos, &y_pos);
        return std::make_pair(x_pos, y_pos);
    }

    void AndroidInput::GetGyroData(float *data) {

    }

    void AndroidInput::GetAccelerometerData(float *data) {

    }

    void AndroidInput::GetProximityData(float *data) {

    }

    AndroidInput::POWER_STATE AndroidInput::GetBatteryInfo(int *secs, int *pct) {
        return AndroidInput::POWER_STATE::UNKNOWN;
    }

    void AndroidInput::GetLightSensorData(float *data) {

    }

    void AndroidInput::GetPressureData(float *data) {

    }

    void AndroidInput::GetHumidityData(float *data) {

    }

    void AndroidInput::GetTemperatureData(float *data) {

    }

    void AndroidInput::GetRotationData(float *data) {

    }

    void AndroidInput::GetMagnetometerData(float *data) {

    }
}
