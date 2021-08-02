//
// Created by adolin on 26. 12. 20.
//

#ifndef ARC_ENGINE_INPUT_H
#define ARC_ENGINE_INPUT_H
namespace arc{
    class Input {
    public:
        static bool IsKeyPressed(int key_code, bool repeat = true);
        static bool IsMouseButtonPressed(int button);
        static float GetMouseX();
        static float GetMouseY();
        static std::pair<float, float> GetMousePosition();
    };

    class AndroidInput {
    public:
        enum class POWER_STATE {
            UNKNOWN, ON_BATTERY, NO_BATTERY, CHARGING, CHARGED
        };
        static void GetGyroData(float *data);
        static void GetAccelerometerData(float *data);
        static void GetMagnetometerData(float *data);
        static void GetRotationData(float *data);
        static POWER_STATE GetBatteryInfo(int *secs, int *pct);
        static void GetLightSensorData(float *data);
        static void GetProximityData(float *data);
        static void GetPressureData(float *data);
        static void GetHumidityData(float *data);
        static void GetTemperatureData(float *data);
    };


}


#endif //ARC_ENGINE_INPUT_H
