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

#include "file_list_box_model.h"
#include "load_save.h"

int FileListBoxModel::getNumRows() {
  return files_.size();
}

void FileListBoxModel::paintListBoxItem(int row_number, Graphics& g,
                                        int width, int height, bool selected) {
  static Font list_font(Typeface::createSystemTypefaceFor(BinaryData::DroidSansMono_ttf,
                                                          BinaryData::DroidSansMono_ttfSize));
  g.fillAll(Colour(0xff323232));
  g.setColour(Colour(0xffdddddd));
  if (selected) {
    g.fillAll(Colour(0xff212121));
    g.setColour(Colour(0xff03a9f4));
  }

  g.setFont(list_font.withPointHeight(12.0f));
  g.drawText(files_[row_number].getFileName(),
             5, 0, width, height,
             Justification::centredLeft, true);

  g.setColour(Colour(0x88000000));
  g.fillRect(0.0f, height - 1.0f, 1.0f * width, 1.0f);
}

void FileListBoxModel::selectedRowsChanged(int last_selected_row) {
  if (listener_)
    listener_->selectedFilesChanged(this);
}

void FileListBoxModel::rescanFiles(const Array<File>& folders,
                                   String search,
                                   bool find_files) {
  static const FileSorterAscending file_sorter;
  files_.clear();

  for (File folder : folders) {
    if (folder.isDirectory()) {
      Array<File> child_folders;
      if (find_files)
        folder.findChildFiles(child_folders, File::findFiles, false, search);
      else
        folder.findChildFiles(child_folders, File::findDirectories, false);
      files_.addArray(child_folders);
    }
  }

  files_.sort(file_sorter);
}
