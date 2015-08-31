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
#ifndef FILE_LIST_BOX_MODEL_H
#define FILE_LIST_BOX_MODEL_H

#include "JuceHeader.h"

class FileListBoxModel : public ListBoxModel {
  public:
    class FileListBoxModelListener {
      public:
        virtual ~FileListBoxModelListener() { }

        virtual void selectedFilesChanged(FileListBoxModel* model) = 0;
    };

    FileListBoxModel() : listener_(nullptr), sort_ascending_(true) { }

    int getNumRows() override;
    void paintListBoxItem(int row_number, Graphics& g,
                          int width, int height, bool selected) override;
    void selectedRowsChanged(int last_selected_row) override;

    void rescanFiles(const Array<File>& folders, String search = "*", bool find_files = false);
    File getFileAtRow(int row) { return files_[row]; }
    void setListener(FileListBoxModelListener* listener) { listener_ = listener; }
    Array<File> getAllFiles() { return files_; }

  private:
    Array<File> files_;
    FileListBoxModelListener* listener_;
    bool sort_ascending_;
};

#endif // FILE_LIST_BOX_MODEL_H
