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
#ifndef GRAPHICAL_STEP_SEQUENCER_H
#define GRAPHICAL_STEP_SEQUENCER_H

#include "JuceHeader.h"
#include "mopo.h"
#include <vector>

class GraphicalStepSequencer : public Component, public Timer, public SliderListener {
  public:
    GraphicalStepSequencer();
    ~GraphicalStepSequencer();

    void timerCallback() override;
    void setNumStepsSlider(Slider* num_steps_slider);
    void setStepSliders(std::vector<Slider*> sliders);
    void sliderValueChanged(Slider* moved_slider) override;

    void showRealtimeFeedback();

    void paint(Graphics& g) override;
    void paintBackground(Graphics& g);
    void resized() override;
    void mouseMove(const MouseEvent& e) override;
    void mouseExit(const MouseEvent& e) override;
    void mouseDown(const MouseEvent& e) override;
    void mouseDrag(const MouseEvent& e) override;

  private:
    int getHoveredStep(Point<int> position);
    void updateHover(int step_index);
    void changeStep(const MouseEvent& e);
    void ensureMinSize();
    void resetBackground();

    int num_steps_;
    mopo::Processor::Output* step_generator_output_;
    int last_step_;
    Slider* num_steps_slider_;
    int highlighted_step_;
    std::vector<Slider*> sequence_;
    Point<int> last_edit_position_;

    Image background_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GraphicalStepSequencer)
};

#endif // GRAPHICAL_STEP_SEQUENCER_H