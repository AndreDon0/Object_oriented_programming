#include <iostream>
#include <tuple>
#include <stdexcept>
#include <format>
#include <cmath>
#include <algorithm>

using namespace std;


class BoundedFloat
{
public:
    BoundedFloat(float value, float min_value, float max_value) 
        : min_value(min_value), max_value(max_value) 
    {
        if (min_value > max_value)
            throw invalid_argument("min_value cannot be greater than max_value");
        this->value = clamp(value);
    }

    BoundedFloat& operator=(float v) {
        value = clamp(v);
        return *this;
    }

    float operator()() const { return value; }
    operator float() const { return value; }

private:
    float value;
    float min_value;
    float max_value;
    
    float clamp(float v) const 
    {
        if (v < this->min_value || v > this->max_value)
            throw out_of_range("Value is out of bounds");
        return v;
    }
};


class Color
{
public:
    Color(const unsigned char r, const unsigned char g, const unsigned char b)
        : red(r), green(g), blue(b),
          hue(0.0f, 0.0f, 360.0f),
          saturation(0.0f, 0.0f, 1.0f),
          value(0.0f, 0.0f, 1.0f)
    {
        RGBtoHSV();
    }

    Color(const float h, const float s, const float v)
        : hue(h, 0.0f, 360.0f),
          saturation(s, 0.0f, 1.0f),
          value(v, 0.0f, 1.0f)
    {
        HSVtoRGB();
    }

    unsigned char GetRed() const { return this->red; }
    unsigned char GetGreen() const { return this->green; }
    unsigned char GetBlue() const { return this->blue; }

    float GetHue() const { return this->hue; }
    float GetSaturation() const { return this->saturation; }
    float GetValue() const { return this->value; }

    tuple<unsigned char, unsigned char, unsigned char> GetRGB() const { return {red, green, blue}; }
    tuple<float, float, float> GetHSV() const { return {hue, saturation, value}; }

    void SetRed(const unsigned char value) { this->red = value; RGBtoHSV(); }
    void SetGreen(const unsigned char value) { this->green = value; RGBtoHSV(); }
    void SetBlue(const unsigned char value) { this->blue = value; RGBtoHSV(); }

    void SetHue(const float value) { this->hue = value; HSVtoRGB(); }
    void SetSaturation(const float value) { this->saturation = value; HSVtoRGB(); }
    void SetValue(const float value) { this->value = value; HSVtoRGB(); }

    void SetRGB(tuple<const unsigned char, const unsigned char, const unsigned char> arr) { this->red = get<0>(arr); this->green = get<1>(arr); this->blue = get<2>(arr); RGBtoHSV(); }
    void SetHSV(tuple<const float, const float, const float> arr) { this->hue = get<0>(arr); this->saturation = get<1>(arr); this->value = get<2>(arr); HSVtoRGB(); }

private:
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    BoundedFloat hue;
    BoundedFloat saturation;
    BoundedFloat value;

    void RGBtoHSV()
    {
        float r = red / 255.0f;
        float g = green / 255.0f;
        float b = blue / 255.0f;

        float c_max = max({r, g, b});
        float c_min = min({r, g, b});
        float delta = c_max - c_min;

        this->value = c_max;
        this->saturation = (c_max == 0) ? 0 : delta / c_max;
        
        float new_hue = (float)this->hue;
        if (delta == 0)
            new_hue = 0;
        else if (c_max == r)
            new_hue = 60 * fmod((g - b) / delta, 6);
        else if (c_max == g)
            new_hue = 60 * ((b - r) / delta + 2);
        else if (c_max == b)
            new_hue = 60 * ((r - g) / delta + 4);

        if (new_hue < 0)
            new_hue += 360;
        
        this->hue = new_hue;
    }

    void HSVtoRGB()
    {
        float c = (float)this->value * (float)this->saturation;
        float x = c * (1 - fabs(fmod((this->hue / 60.0f), 2)) - 1);
        float m = this->value - c;

        float new_red = 0;
        float new_green = 0;
        float new_blue = 0;

        if (hue < 60)
            new_red = c, new_green = x, new_blue = 0;
        else if (hue < 120)
            new_red = x, new_green = c, new_blue = 0;
        else if (hue < 180)
            new_red = 0, new_green = c, new_blue = x;
        else if (hue < 240)
            new_red = 0, new_green = x, new_blue = c;
        else if (hue < 300)
            new_red = x, new_green = 0, new_blue = c;
        else
            new_red = c, new_green = 0, new_blue = x;
        
        this->red = (new_red + m) * 255;
        this->green = (new_green + m) * 255;
        this->blue = (new_blue + m) * 255;
    }
};


int main()
{
    try {
        // ---- Test BoundedFloat ----
        cout << "Testing BoundedFloat:" << endl;
        BoundedFloat bf(5.0f, 0.0f, 10.0f);
        cout << "Initial value: " << bf() << endl;
        bf = 7.5f;
        cout << "After assignment: " << bf() << endl;

        // This should throw
        try {
            bf = 15.0f;
        } catch (const out_of_range &e) {
            cout << "Caught exception (as expected): " << e.what() << endl;
        }

        // ---- Test Color (RGB -> HSV) ----
        cout << "\nTesting Color (RGB to HSV):" << endl;
        Color red_color((unsigned char)255, (unsigned char)0, (unsigned char)0);
        auto [h1, s1, v1] = red_color.GetHSV();
        cout << "RGB(255,0,0) -> HSV("
             << h1 << ", " << s1 << ", " << v1 << ")" << endl;

        Color gray_color((unsigned char)128, (unsigned char)128, (unsigned char)128);
        auto [h2, s2, v2] = gray_color.GetHSV();
        cout << "RGB(128,128,128) -> HSV("
             << h2 << ", " << s2 << ", " << v2 << ")" << endl;

        // ---- Test Color (HSV -> RGB) ----
        cout << "\nTesting Color (HSV to RGB):" << endl;
        Color cyan(180.0f, 1.0f, 1.0f);
        auto [r1, g1, b1] = cyan.GetRGB();
        cout << "HSV(180,1,1) -> RGB("
             << (int)r1 << ", " << (int)g1 << ", " << (int)b1 << ")" << endl;

        // Modify hue and check
        cyan.SetHue(300.0f);
        auto [r2, g2, b2] = cyan.GetRGB();
        cout << "After SetHue(300): RGB("
             << (int)r2 << ", " << (int)g2 << ", " << (int)b2 << ")" << endl;

    } catch (const exception &e) {
        cerr << "Unexpected exception: " << e.what() << endl;
    }

    return 0;
}
