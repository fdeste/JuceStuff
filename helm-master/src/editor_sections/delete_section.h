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
#ifndef DELETE_SECTION_H
#define DELETE_SECTION_H

#include "JuceHeader.h"

class DeleteSection : public Component {
  public:
    DeleteSection(String name);
    ~DeleteSection() { }
    void paint(Graphics& g) override;
    void resized() override;

    Rectangle<int> getDeleteRect();

    void mouseUp(const MouseEvent& e) override;

  private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeleteSection)
};

#endif // DELETE_SECTION_H
