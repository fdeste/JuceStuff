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

#include "tempo_selector.h"
#include "synth_gui_interface.h"

namespace {
  enum MenuIds {
    kCancel = 0,
    kHertz,
    kTempo,
    kTempoDotted,
    kTempoTriplet
  };

} // namespace

TempoSelector::TempoSelector(String name) : SynthSlider(name),
                                            free_slider_(0), tempo_slider_(0) { }

void TempoSelector::mouseDown(const MouseEvent& e) {
  if (e.mods.isPopupMenu()) {
    SynthSlider::mouseDown(e);
    return;
  }
  PopupMenu m;
  m.addItem(kHertz, "Hertz");
  m.addItem(kTempo, "Tempo");
  m.addItem(kTempoDotted, "Tempo Dotted");
  m.addItem(kTempoTriplet, "Tempo Triplet");

  int result = m.showAt(this);
  if (result > 0)
    setValue(result - 1);
}

void TempoSelector::valueChanged() {
  bool free_slider = getValue() == (kHertz - 1);

  if (free_slider_)
    free_slider_->setVisible(free_slider);
  if (tempo_slider_)
    tempo_slider_->setVisible(!free_slider);
}

void TempoSelector::paint(Graphics& g) {
  g.setColour(Colour(0xffbbbbbb));
  g.fillRect(0, 0, getWidth(), getHeight());

  g.setColour(Colour(0xff222222));
  g.fillPath(arrow_);

  int value = getValue() + 1;
  if (value == kHertz)
    g.fillPath(clock_);
  else if (value == kTempo || value == kTempoDotted) {
    g.fillEllipse(getWidth() / 3.0f, getHeight() / 2.0f,
                  getWidth() / 3.0f, getHeight() / 4.0f);
    g.fillRect(2.0f * getWidth() / 3.0f - 1.0f, getHeight() / 6.0f,
               1.0f, 11.0f * getHeight() / 24.0f);
  }
  else if (value == kTempoTriplet) {
    float width = getWidth() / 4.0f;
    float height = getHeight() / 6.0f;
    float x = getWidth() / 4.0f - getWidth() / 16.0f;
    float y = getHeight() / 2.0f + getHeight() / 16.0f;

    g.fillRect(x + width - 1.0f, y + height / 2.0f - getHeight() / 3.0f,
               getWidth() / 2.0f, 1.0f);

    g.fillEllipse(x, y, width, height);
    g.fillRect(x + width - 1.0f, y + height / 2.0f - getHeight() / 3.0f,
               1.0f, getHeight() / 3.0f);
    g.fillEllipse(x + getWidth() / 4.0f, y, width, height);
    g.fillRect(x + width - 1.0f + getWidth() / 4.0f, y + height / 2.0f - getHeight() / 3.0f,
               1.0f, getHeight() / 3.0f);
    g.fillEllipse(x + getWidth() / 2.0f, y, width, height);
    g.fillRect(x + width - 1.0f + getWidth() / 2.0f, y + height / 2.0f - getHeight() / 3.0f,
               1.0f, getHeight() / 3.0f);
  }
  if (value == kTempoDotted) {
    g.fillEllipse(3.0f * getWidth() / 4.0f, getHeight() / 2.0f,
                  getWidth() / 6.0f, getHeight() / 6.0f);
  }
}

void TempoSelector::resized() {
  static const float clock_angle = 1.0f;

  arrow_.clear();
  arrow_.startNewSubPath(getWidth() / 8.0f, getHeight() / 8.0f);
  arrow_.lineTo(3.0f * getWidth() / 8.0f, getHeight() / 8.0f);
  arrow_.lineTo(2.0f * getWidth() / 8.0f, 2.0f * getHeight() / 8.0f);

  clock_.clear();
  clock_.addPieSegment(7.0f * getWidth() / 24.0f, getHeight() / 4.0f,
                       getWidth() / 2.0f, getHeight() / 2.0f,
                       0.0f, clock_angle - 2.0f * mopo::PI, 0.0f);
}
