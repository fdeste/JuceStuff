/* Copyright 2013-2015 Matt Tytel
 *
 * helm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * helm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with helm.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#ifndef WAVE_VIEWER_H
#define WAVE_VIEWER_H

#include "JuceHeader.h"
#include "wave.h"
#include "helm_common.h"

class WaveViewer  : public Component, public Timer, public SliderListener {
  public:
    WaveViewer(int resolution);
    ~WaveViewer();

    void timerCallback() override;
    void setWaveSlider(Slider* slider);
    void setAmplitudeSlider(Slider* slider);
    void resetWavePath();
    void sliderValueChanged(Slider* sliderThatWasMoved) override;
    void showRealtimeFeedback();

    void paint(Graphics& g) override;
    void paintBackground(Graphics& g);
    void resized() override;
    void mouseDown(const MouseEvent& e) override;

  private:
    float phaseToX(float phase);

    Slider* wave_slider_;
    Slider* amplitude_slider_;
    mopo::Processor::Output* wave_phase_;
    mopo::Processor::Output* wave_amp_;
    Path wave_path_;
    int resolution_;
    float phase_;
    float amp_;
    Image background_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveViewer)
};

#endif // WAVE_VIEWER_H
