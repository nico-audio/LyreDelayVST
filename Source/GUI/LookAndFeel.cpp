/*
  ==============================================================================

    LookAndFeel.cpp
    Created: 24 Sep 2025 9:42:12am
    Author:  Nico V.

  ==============================================================================
*/

#include "LookAndFeel.h"


//=============================================================================
// FONTS
//=============================================================================

const juce::Typeface::Ptr Fonts::interdimTypeface =
juce::Typeface::createSystemTypefaceFor(BinaryData::INTERDIM_TTF,BinaryData::INTERDIM_TTFSize);

const juce::Typeface::Ptr Fonts::inductionTypeface =
juce::Typeface::createSystemTypefaceFor(BinaryData::Induction_otf, BinaryData::Induction_otfSize);

const juce::Typeface::Ptr Fonts::patopianTypeface =
juce::Typeface::createSystemTypefaceFor(BinaryData::Patopian_1986_ttf, BinaryData::Patopian_1986_ttfSize);

const juce::Typeface::Ptr Fonts::fakeReceiptTypeface =
juce::Typeface::createSystemTypefaceFor(BinaryData::Fake_Receipt_otf, BinaryData::Fake_Receipt_otfSize);

juce::Font Fonts::getInterdim(float height)
{
    return juce::Font(interdimTypeface).withHeight(height);
}

juce::Font Fonts::getInduction(float height)
{
    return juce::Font(inductionTypeface).withHeight(height);
}

juce::Font Fonts::getPatopian(float height)
{
    return juce::Font(patopianTypeface).withHeight(height);
}

juce::Font Fonts::getFakeReceipt(float height)
{
    return juce::Font(fakeReceiptTypeface).withHeight(height);
}

//=============================================================================
// KNOBS
//=============================================================================

RotaryKnobLookAndFeel::RotaryKnobLookAndFeel()
{
    setColour(juce::Label::textColourId, Colors::Knob::label);
    setColour(juce::Slider::textBoxTextColourId, Colors::Knob::label);
    setColour(juce::Slider::rotarySliderFillColourId, Colors::Knob::trackActive);
    setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    setColour(juce::CaretComponent::caretColourId, Colors::Knob::caret);
}


void RotaryKnobLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, [[maybe_unused]] int height,
                       float sliderPos, float rotaryStartAngle,
                       float rotaryEndAngle, juce::Slider& slider)
{
    // bounds
    auto bounds = juce::Rectangle<int>(x, y, width, width).toFloat();
    auto knobRect = bounds.reduced(14.0f, 14.0f);
    
    // knob shadow
    auto path = juce::Path();
    path.addEllipse(knobRect);
    dropShadow.drawForPath(g, path);
    
    // knob color
    g.setColour(Colors::Knob::outline);
    g.drawEllipse(knobRect, 2.0f);

    // knob gradient
    auto innerRect = knobRect.reduced(2.0f, 2.0f);

    auto knobGrad = juce::ImageCache::getFromMemory(BinaryData::texture_knob_cop1_png, BinaryData::texture_knob_cop1_pngSize);
    g.setImageResamplingQuality(juce::Graphics::highResamplingQuality);
    g.setOpacity(1.0f);
    g.drawImage(knobGrad, innerRect.toFloat(), juce::RectanglePlacement::stretchToFit);
    

    // rotate knob image
    float knobValueRadians = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    g.saveState();
    juce::AffineTransform rotation = juce::AffineTransform::rotation(knobValueRadians, innerRect.getCentreX(), innerRect.getCentreY());
    g.addTransform(rotation);
    g.drawImage(knobGrad, innerRect.toFloat(), juce::RectanglePlacement::stretchToFit);
    g.restoreState();
   
    // draw track
    auto center = bounds.getCentre();
    auto radius = bounds.getWidth() / 2.0f;
    auto lineWidth = 2.0f;
    auto arcRadius = radius - lineWidth / 2.0f;
    
    juce::Path backgroundArc;
    backgroundArc.addCentredArc(center.x, center.y, arcRadius, arcRadius,
                                0.0f, rotaryStartAngle, rotaryEndAngle, true);
    
    auto strokeType = juce::PathStrokeType(lineWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded);

    g.setColour(Colors::Knob::trackBackground);
    g.strokePath(backgroundArc, strokeType);


    // draw dial - calculate angles
    auto dialRadius = innerRect.getHeight() / 2.0f - lineWidth;
    auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    juce::Point<float> dialStart(center.x + 10.0f * std::sin(toAngle), center.y - 10.0f * std::cos(toAngle));
    juce::Point<float> dialEnd(center.x + dialRadius * std::sin(toAngle), center.y - dialRadius * std::cos(toAngle));

    // dial path
    juce::Path dialPath;
   
    dialPath.startNewSubPath(dialStart);
    dialPath.lineTo(dialEnd);

    // dial glow path
    juce::Path glowPath;
    juce::PathStrokeType glowStroke(6.0f); // thickness of glow
    glowStroke.createStrokedPath(glowPath, dialPath);
    dialShadow.drawForPath(g, glowPath);

    // draw dial
    g.setColour(Colors::Knob::dial);
    g.strokePath(dialPath, strokeType);

    // track color
    if (slider.isEnabled()) {
        float fromAngle = rotaryStartAngle;
        if (slider.getProperties()["drawFromMiddle"]) {
            fromAngle += (rotaryEndAngle - rotaryStartAngle) / 2.0f;
        }

        juce::Path valueArc;
        
        valueArc.addCentredArc(center.x, center.y, arcRadius, arcRadius, 0.0f, fromAngle, toAngle, true);

        g.setColour(slider.findColour(juce::Slider::rotarySliderFillColourId));
        g.strokePath(valueArc, strokeType);
    }
}

juce::Font RotaryKnobLookAndFeel::getLabelFont([[maybe_unused]] juce::Label& label)
{
    return Fonts::getInterdim(14.0f);
}

class RotaryKnobLabel : public juce::Label
{
public:
    RotaryKnobLabel() : juce::Label() {}
    void mouseWheelMove(const juce::MouseEvent&,
                        const juce::MouseWheelDetails&) override {}
    std::unique_ptr<juce::AccessibilityHandler>createAccessibilityHandler() override
    {
        return createIgnoredAccessibilityHandler(*this);
    }

    // customize inline editor
    juce::TextEditor* createEditorComponent() override
    {
        auto* editorComponent = new juce::TextEditor(getName());
        editorComponent->applyFontToAllText(getLookAndFeel().getLabelFont(*this));
        copyAllExplicitColoursTo(*editorComponent);

        editorComponent->setBorder(juce::BorderSize<int>());
        editorComponent->setIndents(2, 1);
        editorComponent->setJustification(juce::Justification::centredTop);

        editorComponent->setPopupMenuEnabled(false);
        editorComponent->setInputRestrictions(8);

        return editorComponent;
    }
};


juce::Label* RotaryKnobLookAndFeel::createSliderTextBox(juce::Slider& slider)
{
    auto l = new RotaryKnobLabel();
    l->setJustificationType(juce::Justification::centred);
    l->setKeyboardType(juce::TextInputTarget::decimalKeyboard);
    l->setColour(juce::Label::textColourId, slider.findColour(juce::Slider::textBoxTextColourId));
    l->setColour(juce::TextEditor::textColourId, Colors::Knob::value);
    l->setColour(juce::TextEditor::highlightedTextColourId, Colors::Knob::value);
    l->setColour(juce::TextEditor::highlightColourId, slider.findColour(juce::Slider::rotarySliderFillColourId));
    l->setColour(juce::TextEditor::backgroundColourId, Colors::Knob::textBoxBackground);
    return l;
}

void RotaryKnobLookAndFeel::fillTextEditorBackground(
    juce::Graphics& g, [[maybe_unused]] int width, [[maybe_unused]] int height,
    juce::TextEditor& textEditor)
{
    g.setColour(Colors::Knob::textBoxBackground);
    g.fillRoundedRectangle(textEditor.getLocalBounds().reduced(4, 0).toFloat(), 4.0f);
}

//=============================================================================
// MAIN LOOK AND FEEL
//=============================================================================

MainLookAndFeel::MainLookAndFeel()
{
}

void MainLookAndFeel::drawGroupComponentOutline(juce::Graphics& g, int width, int height, 
                                                const juce::String& text, const juce::Justification& position, 
                                                juce::GroupComponent& group)
{
    const float cornerSize = 8.0f;
    const float outlineThickness = 1.4f;

    auto bounds = juce::Rectangle<float>(0, 0, (float)width, (float)height).reduced(outlineThickness * 0.5f);

    // Border
    const float engraveOffset = 1.0f;

    g.setColour(juce::Colours::black.withAlpha(0.55f));
    g.drawRoundedRectangle(bounds.translated(-engraveOffset, -engraveOffset), cornerSize, outlineThickness);

    g.setColour(juce::Colours::white.withAlpha(0.15f));
    g.drawRoundedRectangle(bounds.translated(engraveOffset, engraveOffset), cornerSize, outlineThickness);

    g.setColour(Colors::Group::outline.withAlpha(0.8f));
    g.drawRoundedRectangle(bounds, cornerSize, outlineThickness * 0.75f);

    // Group label
    if (text.isEmpty()) {
        return;
    }

    auto font = Fonts::getInterdim(13.0f);
    g.setFont(font);
    const int textWidth = (int)juce::GlyphArrangement::getStringWidth(font, text);
    const int textPadding = 4;

    const int bottomMargin = 8;
    const int textX = (width - textWidth) / 2;
    const int textY = height - (int)font.getHeight() - bottomMargin;
    juce::Rectangle<int> textBounds(textX, textY, textWidth + textPadding, (int)font.getHeight());

    g.setColour(juce::Colours::black.withAlpha(0.5f));
    g.drawText(text, textBounds.translated(-engraveOffset, -engraveOffset), juce::Justification::centred, false);

    g.setColour(juce::Colours::white.withAlpha(0.15f));
    g.drawText(text, textBounds.translated(engraveOffset, engraveOffset), juce::Justification::centred, false);

    g.setColour(Colors::Group::label.withAlpha(0.75f));
    g.drawText(text, textBounds, juce::Justification::centred, false);
    
}

//=============================================================================
// BUTTONS
//=============================================================================

ButtonLookAndFeel::ButtonLookAndFeel()
{
    setColour(juce::TextButton::textColourOffId, Colors::Button::text);
    setColour(juce::TextButton::textColourOnId, Colors::Button::textToggled);
    setColour(juce::TextButton::buttonColourId, Colors::Button::background);
    setColour(juce::TextButton::buttonOnColourId, Colors::Button::backgroundToggled);
}

juce::Font ButtonLookAndFeel::getLabelFont([[maybe_unused]] juce::Label& label)
{
    return Fonts::getInterdim(12.0f);
}

void ButtonLookAndFeel::drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour, 
                                             [[maybe_unused]] bool shouldDrawButtonAsHighlighted,
                                             bool shouldDrawButtonAsDown)
{
    auto bounds = button.getLocalBounds().toFloat();
    auto cornerSize = bounds.getHeight() * 0.25f;
    auto buttonRect = bounds.reduced(1.0f, 1.0f).withTrimmedBottom(1.0f);

    if (shouldDrawButtonAsDown) {
        buttonRect.translate(0.0f, 1.0f);
    }

    g.setColour(backgroundColour);
    g.fillRoundedRectangle(buttonRect, cornerSize);

    g.setColour(Colors::Button::outline);
    g.drawRoundedRectangle(buttonRect, cornerSize, 0.5f);
}

void ButtonLookAndFeel::drawButtonText(juce::Graphics& g, juce::TextButton& button,
                                       [[maybe_unused]] bool shouldDrawButtonAsHighlighted,
                                       bool shouldDrawButtonAsDown)
{
    auto bounds = button.getLocalBounds().toFloat();
    auto buttonRect = bounds.reduced(1.0f, 1.0f).withTrimmedBottom(1.0f);

    if (shouldDrawButtonAsDown) {
        buttonRect.translate(0.0f, 1.0f);
    }

    if (button.getToggleState()) {
        g.setColour(button.findColour(juce::TextButton::textColourOnId));
    }
    else {
        g.setColour(button.findColour(juce::TextButton::textColourOffId));
    }

    g.setFont(Fonts::getInterdim(12.0f));
    g.drawText(button.getButtonText(), buttonRect, juce::Justification::centred, false);
}

//=============================================================================
// COMBO BOX
//=============================================================================

ComboBoxLookAndFeel::ComboBoxLookAndFeel()
{
}

void ComboBoxLookAndFeel::drawComboBox(juce::Graphics& g, int width, int height,
                                       [[maybe_unused]] bool isButtonDown, [[maybe_unused]] int buttonX,
                                       int buttonY, int buttonW, int buttonH, juce::ComboBox& box) 
{
    auto bounds = juce::Rectangle<int>(0, 0, width, height).reduced(1);

    // Box background
    constexpr float cornerRadius = 6.0f;
    constexpr float padding = cornerRadius - 2.0f;

    juce::ColourGradient boxGradient(Colors::PresetPanel::comboBoxGradTop, 0.0f, bounds.getY(), 
                                     Colors::PresetPanel::comboBoxGradBtm, 0.0f, bounds.getBottom(), false);
    
    
    g.setGradientFill(boxGradient);
    g.fillRoundedRectangle(bounds.toFloat(), cornerRadius);

    // Top Highlight
    g.setColour(juce::Colours::white.withAlpha(0.05f));
    g.drawLine(bounds.getX() + padding, bounds.getY() + 1.0f, bounds.getRight() - padding, bounds.getY() + 1.0f);
    
    // Bottom shadow
    g.setColour(juce::Colours::black.withAlpha(0.15f));
    g.drawLine(bounds.getX() + padding, bounds.getBottom() - 1.0f, bounds.getRight() - padding, bounds.getBottom() - 1.0f);

    // Outline
    g.setColour(Colors::PresetPanel::comboBoxOutline.withAlpha(0.55f));
    g.drawRoundedRectangle(bounds.toFloat(), 6.0f, 1.0f);

    // Arrow
    juce::Path arrow;

    float arrowX = width - 16.0f;
    float arrowY = height * 0.5f;

    arrow.startNewSubPath(arrowX - 4.0f, arrowY - 2.0f);
    arrow.lineTo(arrowX, arrowY + 2.0f);
    arrow.lineTo(arrowX + 4.0f, arrowY - 2.0f);

    g.setColour(Colors::PresetPanel::arrow);
    g.strokePath(arrow, juce::PathStrokeType(1.8f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
}

void ComboBoxLookAndFeel::positionComboBoxText(juce::ComboBox& box, juce::Label& label)
{
    label.setBounds(12, 0, box.getWidth() - 34, box.getHeight());
    label.setFont(Fonts::getFakeReceipt(14.0f));
    label.setJustificationType(juce::Justification::centredLeft);
}

//=============================================================================
// POP-UP MENU
//=============================================================================

void ComboBoxLookAndFeel::drawPopupMenuBackground(juce::Graphics& g, int width, int height)
{
    auto bounds = juce::Rectangle<float>(0.0f, 0.0f, (float)width, (float)height);
    constexpr float cornerRadius = 1.0f;
    constexpr float padding = cornerRadius - 2.0f;

    juce::ColourGradient bgGradient(Colors::PresetPanel::comboBoxGradTop, 0.0f, bounds.getY(),
                                    Colors::PresetPanel::comboBoxGradBtm, 0.0f, bounds.getBottom(), false);
    g.setGradientFill(bgGradient);
    g.fillRoundedRectangle(bounds, cornerRadius);

    g.setColour(juce::Colours::white.withAlpha(0.05f));
    g.drawLine(bounds.getX() + padding, bounds.getY() + 1.0f, bounds.getRight() - padding, bounds.getY() + 1.0f);
    g.setColour(juce::Colours::black.withAlpha(0.15f));
    g.drawLine(bounds.getX() + padding, bounds.getBottom() - 1.0f, bounds.getRight() - padding, bounds.getBottom() - 1.0f);

    // Outline
    g.setColour(Colors::PresetPanel::comboBoxOutline.withAlpha(0.55f));
    g.drawRoundedRectangle(bounds.reduced(0.5f), cornerRadius, 1.0f);
}

void ComboBoxLookAndFeel::drawPopupMenuSectionHeader(juce::Graphics& g, const juce::Rectangle<int>& area, const juce::String& sectionName)
{
    g.setColour(Colors::PresetPanel::arrow.withAlpha(0.5f));
    g.setFont(juce::Font(juce::FontOptions(11.0f)).boldened().withExtraKerningFactor(0.06f));
    g.drawText(sectionName.toUpperCase(), area.reduced(14, 0), juce::Justification::centredLeft, true);
}

void ComboBoxLookAndFeel::drawPopupMenuItem(juce::Graphics& g, const juce::Rectangle<int>& area, bool isSeparator, 
                                            bool isActive, bool isHighlighted, bool isTicked, bool hasSubMenu, const juce::String& text, 
                                            const juce::String& shotcutKeyText, const juce::Drawable* icon, const juce::Colour* textColour)
{
    float selectableAlpha = 0.92f;
    float inactiveAlpha = 0.35f;
    int namePadding = 6;  // preset name text
    
    
    if (isSeparator){
        auto rect  = area.reduced(14, 0);
        rect.removeFromTop(juce::roundToInt(((float)rect.getHeight() * 0.5f) - 0.5f));
        g.setColour(Colors::PresetPanel::comboBoxOutline.withAlpha(0.25f));
        g.fillRect(rect.removeFromTop(1));
        return;
    }

    auto rect = area.reduced(4, 1).toFloat();

    if (isHighlighted && isActive) {
        g.setColour(Colors::PresetPanel::arrow.withAlpha(0.14f));
        g.fillRoundedRectangle(rect, 4.0f);
        g.setColour(Colors::PresetPanel::arrow.withAlpha(0.35f));
        g.drawRoundedRectangle(rect, 4.0f, 1.0f);
    }

    auto textColourToUse = textColour != nullptr ? *textColour : juce::Colours::white;
    textColourToUse = textColourToUse.withAlpha(isActive ? selectableAlpha : inactiveAlpha);

    auto textArea = area.reduced(14, 0);

    if (isTicked) {
        auto selectionDot = textArea.removeFromLeft(10).withSizeKeepingCentre(5, 5).toFloat();
        g.setColour(Colors::PresetPanel::arrow);
        g.fillEllipse(selectionDot);
        textArea.removeFromLeft(namePadding);
    }

        g.setColour(textColourToUse);
        g.setFont(getPopupMenuFont());
        g.drawText(text, textArea, juce::Justification::centredLeft, true);

}

juce::Font ComboBoxLookAndFeel::getPopupMenuFont()
{
    return Fonts::getFakeReceipt(14.0f);
}

void ComboBoxLookAndFeel::getIdealPopupMenuItemSize(const juce::String& text, bool isSeparator, int standardMenuItemHeight, int& idealWidth, int& idealHeight)
{
    LookAndFeel_V4::getIdealPopupMenuItemSize(text, isSeparator, standardMenuItemHeight, idealWidth, idealHeight);
    idealHeight = isSeparator ? 9 : 28;
    idealWidth += 24;
}

juce::PopupMenu::Options ComboBoxLookAndFeel::getOptionsForComboBoxPopupMenu(juce::ComboBox& box, juce::Label& label)
{
    auto options = LookAndFeel_V4::getOptionsForComboBoxPopupMenu(box, label).withMaximumNumColumns(1).withMinimumWidth(box.getWidth());

    auto screenBounds = box.getScreenBounds();
    auto maxHeight = 200;

    return options.withTargetScreenArea(screenBounds.withHeight(maxHeight).withY(screenBounds.getBottom()));
}
