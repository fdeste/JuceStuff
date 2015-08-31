/*
 *  SequencerComponent.cpp
 *  syzygrd_sequencer1
 *
 *  Created by Matt Sonic on 6/17/10.
 *  Copyright 2010 SonicTransfer. All rights reserved.
 *
 */

#include "Cell.h"
#include "PluginAudioProcessor.h"
#include "MyTableListBox.h"
#include "Sequencer.h"
#include "CellComponent.h"

#include "SequencerComponent.h"

SequencerComponent::SequencerComponent (PluginAudioProcessor* pluginAudioProcessor_) :
Component ("SequencerComponent"),
pluginAudioProcessor (pluginAudioProcessor_),
sequencer (0),
lastPlayheadCol (-1)
{
	sequencer = pluginAudioProcessor->getSequencer();
	
	for (int i = 0; i < sequencer->getTotalRows(); i++) {
		Array<CellComponent*>* row;
		rows.add (row = new Array<CellComponent*>);
		for (int j = 0; j < sequencer->getTotalCols(); j++) {
			CellComponent* cell;
			addAndMakeVisible (cell = new CellComponent (sequencer->getCellAt (i, j)));
			row->add (cell);
		}
	}
	
	setSize (600, 400);
	setWantsKeyboardFocus (true);
	
	setAlwaysOnTop (true);
	
	startTimer (50);
}

SequencerComponent::~SequencerComponent()
{
	deleteAllChildren();
}

CellComponent* SequencerComponent::getCellAt (int row_, int col_)
{
	Array<CellComponent*>* row = rows[row_];
	CellComponent* cell = row->getUnchecked (col_);
	return cell;
}

// Component methods
void SequencerComponent::paint (Graphics& g)
{
	g.fillAll (Colours::black);	
	g.setColour (Colour::fromRGB (100, 220, 250));
	float cellWidth = (float)getWidth() / sequencer->getTotalCols();
	g.fillRect (lastPlayheadCol * cellWidth, 0.0f, cellWidth, (float)getHeight());
}

void SequencerComponent::resized()
{
	float cellWidth = (float)getWidth() / sequencer->getTotalCols();
	float cellHeight = (float)getHeight() / sequencer->getTotalRows();
	
	for (int i = 0; i < sequencer->getTotalRows(); i++) {
		for (int j = 0; j < sequencer->getTotalCols(); j++) {
			CellComponent* cell = getCellAt (i, j);
			cell->setBounds (j * cellWidth, i * cellHeight, cellWidth, cellHeight);
		}
	}
}

bool SequencerComponent::keyPressed (const KeyPress& key)
{
	return Component::keyPressed (key);
}

// Timer methods
void SequencerComponent::timerCallback()
{
	if (lastPlayheadCol != sequencer->getPlayheadCol()) {
		lastPlayheadCol = sequencer->getPlayheadCol();
		repaint();
	}
}
