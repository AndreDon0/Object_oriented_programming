#ifndef COLOR_H
#define COLOR_H

#include <stdexcept>
#include <tuple>

class BoundedFloat
{
public:
    BoundedFloat(float value, float min_value, float max_value);

    BoundedFloat& operator=(float v);

    float operator()() const;
    operator float() const;

private:
    float value;
    float min_value;
    float max_value;
    
    float clamp(float v) const;
};

class Color
{
public:
    // --- Constructors ---
    Color(unsigned char r, unsigned char g, unsigned char b);     // RGB input
    Color(float h, float s, float v);                             // HSV input

    // --- Getters (RGB) ---
    unsigned char GetRed() const;
    unsigned char GetGreen() const;
    unsigned char GetBlue() const;
    std::tuple<unsigned char, unsigned char, unsigned char> GetRGB() const;

    // --- Getters (HSV) ---
    float GetHue() const;
    float GetSaturation() const;
    float GetValue() const;
    std::tuple<float, float, float> GetHSV() const;

    // --- Setters (RGB) ---
    void SetRed(unsigned char value);
    void SetGreen(unsigned char value);
    void SetBlue(unsigned char value);
    void SetRGB(std::tuple<unsigned char, unsigned char, unsigned char> rgb);

    // --- Setters (HSV) ---
    void SetHue(float value);
    void SetSaturation(float value);
    void SetValue(float value);
    void SetHSV(std::tuple<float, float, float> hsv);

private:
    unsigned char red;
    unsigned char green;
    unsigned char blue;

    BoundedFloat hue;        // [0, 360]
    BoundedFloat saturation; // [0, 1]
    BoundedFloat value;      // [0, 1]

    void RGBtoHSV();
    void HSVtoRGB();
};

#endif // COLOR_H
