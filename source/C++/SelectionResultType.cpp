/*
 *	Class:		SelectionResultType
 *	Purpose:	To return selection variables of a function
 *	Version:	Thinknowlogy 2015r1beta (Coraz�n)
 *************************************************************************/
/*	Copyright (C) 2009-2015, Menno Mafait
 *	Your suggestions, modifications and bug reports are welcome at
 *	http://mafait.org
 *************************************************************************/
/*	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License along
 *	with this program; if not, write to the Free Software Foundation, Inc.,
 *	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *************************************************************************/

#ifndef SELECTIONRESULTTYPE
#define SELECTIONRESULTTYPE 1
#include "Item.h"
// Class declarations needed by some compilers
class SelectionItem;

class SelectionResultType
	{
	friend class AdminItem;
	friend class AdminSelection;
	friend class AdminSolve;
	friend class ScoreList;
	friend class SelectionList;
	protected:
	// Protected variables

	ResultType result;

	bool hasFoundDuplicateSelection;
	bool isConditionSatisfied;

	unsigned int duplicateConditionSentenceNr;

	SelectionItem *bestActionItem;
	SelectionItem *firstExecutionItem;
	SelectionItem *lastCreatedSelectionItem;

	protected:
	// Constructor / deconstructor

	SelectionResultType()
		{
		result = RESULT_OK;

		hasFoundDuplicateSelection = false;
		isConditionSatisfied = false;

		duplicateConditionSentenceNr = NO_SENTENCE_NR;

		bestActionItem = NULL;
		firstExecutionItem = NULL;
		lastCreatedSelectionItem = NULL;
		}
	};
#endif

/*************************************************************************
 *	"The Lords protects them
 *	and keeps them alive.
 *	and rescues them from their enimies.
 *	The Lord nurses them when they are sick
 *	and restores them to health." (Psalm 41:2-3)
 *************************************************************************/