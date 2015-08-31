/*
 *  CellComponent.cpp
 *  syzygrd_sequencer1
 *
 *  Created by Matt Sonic on 6/17/10.
 *  Copyright 2010 SonicTransfer. All rights reserved.
 *
 */

#include "Cell.h"

#include "CellComponent.h"

CellComponent::CellComponent (Cell* cell_) :
Component ("CellComponent"),
cell (cell_)
{
}

CellComponent::~CellComponent()
{
}

// Component methods
void CellComponent::paint (Graphics& g)
{
	g.setColour (Colour::fromRGB (100, 140, 110));
	g.fillRoundedRectangle (5, 5, getWidth() - 10, getHeight() - 10, 3.0);

	String label;
	label << cell->getRow() << ", " << cell->getCol();
	g.setColour (Colour::fromRGB (255, 255, 255));
	g.setFont (11);
	g.drawText (label, 5, 5, getWidth() - 10, getHeight() - 10, Justification::centred, false);
}

void CellComponent::resized()
{
}

