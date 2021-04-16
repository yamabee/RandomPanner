/*
  ==============================================================================

    SliderLookAndFeel.h
    Created: 15 Apr 2021 1:01:33pm
    Author:  Erin Lindsey Yamabe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class SliderLookAndFeel : public LookAndFeel_V4 {

public:
    SliderLookAndFeel() {};
    virtual ~SliderLookAndFeel() {};


    virtual void drawLinearSlider(Graphics &g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const Slider::SliderStyle, Slider &slider) override {
//        auto trackWidth = jmin (6.0f, slider.isHorizontal() ? (float) height * 0.25f : (float) width * 0.25f);
//
//        Point<float> startPoint (slider.isHorizontal() ? (float) x : (float) x + (float) width * 0.5f,
//                                 slider.isHorizontal() ? (float) y + (float) height * 0.5f : (float) (height + y));
//
//        Point<float> endPoint (slider.isHorizontal() ? (float) (width + x) : startPoint.x,
//                               slider.isHorizontal() ? startPoint.y : (float) y);
//
//        Path backgroundTrack;
//        backgroundTrack.startNewSubPath (startPoint);
//        backgroundTrack.lineTo (endPoint);
//        g.setColour (slider.findColour (Slider::backgroundColourId));
//        g.strokePath (backgroundTrack, { trackWidth, PathStrokeType::curved, PathStrokeType::rounded });
//
//        Path valueTrack;
//        Point<float> minPoint, maxPoint, thumbPoint;
//
//        auto kx = slider.isHorizontal() ? sliderPos : ((float) x + (float) width * 0.5f);
//        auto ky = slider.isHorizontal() ? ((float) y + (float) height * 0.5f) : sliderPos;
//
//        minPoint = startPoint;
//        maxPoint = { kx, ky };
//
//        auto thumbWidth = getSliderThumbRadius (slider);
//
//        valueTrack.startNewSubPath (minPoint);
//        g.setColour (slider.findColour (Slider::trackColourId));
//        g.strokePath (valueTrack, { trackWidth, PathStrokeType::curved, PathStrokeType::rounded });
//
        drawLinearSliderBackground(g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, slider.getSliderStyle(), slider);
        drawLinearSliderThumb(g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, slider.getSliderStyle(), slider);
    }

    virtual void drawLinearSliderThumb(Graphics &g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const Slider::SliderStyle style, Slider &slider) override {

        Image fader;

        fader = ImageCache::getFromMemory(BinaryData::NewFaderLime_png, BinaryData::NewFaderLime_pngSize);
        
        auto sliderWidth = (float) (getSliderThumbRadius (slider));
        
        if (style == Slider::LinearVertical)
        {
            float kx, ky;

            kx = (float) x + (float) width * 0.5f;
            ky = sliderPos;

            g.drawImage (fader, kx-(18/2)+1, ky-(30/2), 18, 30, 0, 0, fader.getWidth(), fader.getHeight());
            
        }
    }
private:

};

//void LookAndFeel_V2::drawLinearSliderThumb (Graphics& g, int x, int y, int width, int height,
//                                            float sliderPos, float minSliderPos, float maxSliderPos,
//                                            const Slider::SliderStyle style, Slider& slider)
//{
//    auto sliderRadius = (float) (getSliderThumbRadius (slider) - 2);
//
//    auto knobColour = LookAndFeelHelpers::createBaseColour (slider.findColour (Slider::thumbColourId),
//                                                            slider.hasKeyboardFocus (false) && slider.isEnabled(),
//                                                            slider.isMouseOverOrDragging() && slider.isEnabled(),
//                                                            slider.isMouseButtonDown() && slider.isEnabled());
//
//    const float outlineThickness = slider.isEnabled() ? 0.8f : 0.3f;
//
//    if (style == Slider::LinearVertical)
//    {
//        float kx, ky;
//
//        if (style == Slider::LinearVertical)
//        {
//            kx = (float) x + (float) width * 0.5f;
//            ky = sliderPos;
//        }
//        else
//        {
//            kx = sliderPos;
//            ky = (float) y + (float) height * 0.5f;
//        }
//
//        drawGlassSphere (g,
//                         kx - sliderRadius,
//                         ky - sliderRadius,
//                         sliderRadius * 2.0f,
//                         knobColour, outlineThickness);
//    }
//    else
//    {
//        if (style == Slider::ThreeValueVertical)
//        {
//            drawGlassSphere (g, (float) x + (float) width * 0.5f - sliderRadius,
//                             sliderPos - sliderRadius,
//                             sliderRadius * 2.0f,
//                             knobColour, outlineThickness);
//        }
//        else if (style == Slider::ThreeValueHorizontal)
//        {
//            drawGlassSphere (g,sliderPos - sliderRadius,
//                             (float) y + (float) height * 0.5f - sliderRadius,
//                             sliderRadius * 2.0f,
//                             knobColour, outlineThickness);
//        }
//
//        if (style == Slider::TwoValueVertical || style == Slider::ThreeValueVertical)
//        {
//            auto sr = jmin (sliderRadius, (float) width * 0.4f);
//
//            drawGlassPointer (g, jmax (0.0f, (float) x + (float) width * 0.5f - sliderRadius * 2.0f),
//                              minSliderPos - sliderRadius,
//                              sliderRadius * 2.0f, knobColour, outlineThickness, 1);
//
//            drawGlassPointer (g,
//                              jmin ((float) x + (float) width - sliderRadius * 2.0f,
//                                    (float) x + (float) width * 0.5f),
//                              maxSliderPos - sr,
//                              sliderRadius * 2.0f,
//                              knobColour,
//                              outlineThickness,
//                              3);
//        }
//        else if (style == Slider::TwoValueHorizontal || style == Slider::ThreeValueHorizontal)
//        {
//            auto sr = jmin (sliderRadius, (float) height * 0.4f);
//
//            drawGlassPointer (g, minSliderPos - sr,
//                              jmax (0.0f, (float) y + (float) height * 0.5f - sliderRadius * 2.0f),
//                              sliderRadius * 2.0f, knobColour, outlineThickness, 2);
//
//            drawGlassPointer (g,
//                              maxSliderPos - sliderRadius,
//                              jmin ((float) y + (float) height - sliderRadius * 2.0f,
//                                    (float) y + (float) height * 0.5f),
//                              sliderRadius * 2.0f,
//                              knobColour,
//                              outlineThickness,
//                              4);
//        }
//    }
//}
