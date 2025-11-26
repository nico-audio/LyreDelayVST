/*
  ==============================================================================

    LookAndFeel.h
    Created: 24 Sep 2025 9:42:12am
    Author:  Nico V.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

namespace Colors
{
    const juce::Colour background{ 9, 7, 18 };
    const juce::Colour header{ 11, 2, 0 };
    
    namespace Knob
    {
        const juce::Colour trackBackground{ 205, 200, 195 };
        const juce::Colour trackActive{ 229, 168, 106 };
        const juce::Colour outline{ 30, 31, 32 };
        const juce::Colour gradientTop{ 110, 112, 113 };
        const juce::Colour gradientBottom{ 156, 157, 158 };
        const juce::Colour dial{ 100, 100, 100 };
        const juce::Colour dropShadow{ 82, 68, 109 };
        const juce::Colour label{ 255, 255, 240 };
        const juce::Colour textBoxBackground{ 80, 80, 80 };
        const juce::Colour value{ 240, 240, 240 };
        const juce::Colour caret{ 255, 255, 255 };
    }
    namespace Group
    {
        const juce::Colour label{ 255, 255, 240 };
        const juce::Colour outline{ 122, 84, 50 };
    }

    namespace Button
    {
        const juce::Colour text{ 255, 255, 240 };
        const juce::Colour textToggled{ 255, 255, 240 };
        const juce::Colour background{ 74, 54, 23 };
        const juce::Colour backgroundToggled{ 206, 148, 92 };
        const juce::Colour outline{ 69, 69, 69 };
    }
}

class Fonts
{
public:
    Fonts() = delete;
    static juce::Font getFont(float height = 14.0f);
private:
    static const juce::Typeface::Ptr typeface;
};

class RotaryKnobLookAndFeel : public juce::LookAndFeel_V4
{
public:
    RotaryKnobLookAndFeel();
    static RotaryKnobLookAndFeel* get()
    {
        static RotaryKnobLookAndFeel instance;
        return &instance;
    }
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                          float sliderPos, float rotaryStartAngle,
                          float rotaryEndAngle, juce::Slider& slider) override;
    
    juce::Font getLabelFont(juce::Label&) override;
    juce::Label* createSliderTextBox(juce::Slider&) override;

    void drawTextEditorOutline(juce::Graphics&, int, int, juce::TextEditor&) override {}
    void fillTextEditorBackground(juce::Graphics&, int width, int height, juce::TextEditor&) override;
    
private:
    juce::DropShadow dropShadow{ Colors::Knob::dropShadow, 6, { 0, 3 } };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RotaryKnobLookAndFeel)
};

class MainLookAndFeel : public juce::LookAndFeel_V4
{
public:
    MainLookAndFeel();

    juce::Font getLabelFont(juce::Label&) override;
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainLookAndFeel)
};

class ButtonLookAndFeel : public juce::LookAndFeel_V4
{
public:
    ButtonLookAndFeel();
    
    static ButtonLookAndFeel* get()
    {
        static ButtonLookAndFeel instance;
        return &instance;
    }

    void drawButtonBackground(juce::Graphics & g, juce::Button & button,
                              const juce::Colour & backgroundColour,
                              bool shouldDrawButtonAsHighlighted,
                              bool shouldDrawButtonAsDown) override;
    
    void drawButtonText(juce::Graphics & g, juce::TextButton & button,
                        bool shouldDrawButtonAsHighlighted,
                        bool shouldDrawButtonAsDown) override;
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ButtonLookAndFeel)
};
