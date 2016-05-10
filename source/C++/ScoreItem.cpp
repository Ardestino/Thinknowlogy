/*	Class:			ScoreItem
 *	Parent class:	Item
 *	Purpose:		To temporarily store scoring info during
 *					solving (= assigning) words according the selections
 *	Version:		Thinknowlogy 2016r1 (Huguenot)
 *************************************************************************/
/*	Copyright (C) 2009-2016, Menno Mafait. Your suggestions, modifications,
 *	corrections and bug reports are welcome at http://mafait.org/contact/
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

#include "SelectionItem.cpp"

class ScoreItem : private Item
	{
	friend class AdminSolve;
	friend class ScoreList;

	// Private loadable variables

	unsigned short assignmentLevel_;

	protected:
	// Protected loadable variables

	bool isMarked;
	bool isChecked;

	unsigned int oldSatisfiedScore;
	unsigned int newSatisfiedScore;
	unsigned int oldDissatisfiedScore;
	unsigned int newDissatisfiedScore;
	unsigned int oldNotBlockingScore;
	unsigned int newNotBlockingScore;
	unsigned int oldBlockingScore;
	unsigned int newBlockingScore;

	SelectionItem *referenceSelectionItem;


	private:
	// Private functions

	ScoreItem *possibilityItem( bool isIncludingThisItem )
		{
		unsigned short currentAssignmentLevel = commonVariables()->currentAssignmentLevel;
		ScoreItem *searchItem = ( isIncludingThisItem ? this : nextScoreItem() );

		while( searchItem != NULL &&
		searchItem->assignmentLevel_ >= currentAssignmentLevel )
			{
			if( searchItem->assignmentLevel_ == currentAssignmentLevel )
				return searchItem;

			searchItem = searchItem->nextScoreItem();
			}

		return NULL;
		}


	protected:
	// Constructor / deconstructor

	ScoreItem( bool _isChecked, unsigned short assignmentLevel, unsigned int _oldSatisfiedScore, unsigned int _newSatisfiedScore, unsigned int _oldDissatisfiedScore, unsigned int _newDissatisfiedScore, unsigned int _oldNotBlockingScore, unsigned int _newNotBlockingScore, unsigned int _oldBlockingScore, unsigned int _newBlockingScore, SelectionItem *_selectionReference, CommonVariables *commonVariables, List *myList, WordItem *myWordItem )
		{
		initializeItemVariables( NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, "ScoreItem", commonVariables, myList, myWordItem );

		// Private loadable variables

		assignmentLevel_ = assignmentLevel;

		// Protected loadable variables

		isMarked = false;
		isChecked = _isChecked;

		oldSatisfiedScore = _oldSatisfiedScore;
		newSatisfiedScore = _newSatisfiedScore;
		oldDissatisfiedScore = _oldDissatisfiedScore;
		newDissatisfiedScore = _newDissatisfiedScore;
		oldNotBlockingScore = _oldNotBlockingScore;
		newNotBlockingScore = _newNotBlockingScore;
		oldBlockingScore = _oldBlockingScore;
		newBlockingScore = _newBlockingScore;

		referenceSelectionItem = _selectionReference;
		}


	// Protected virtual functions

	virtual bool hasFoundParameter( unsigned int queryParameter )
		{
		return ( assignmentLevel_ == queryParameter ||
				oldSatisfiedScore == queryParameter ||
				newSatisfiedScore == queryParameter ||
				oldDissatisfiedScore == queryParameter ||
				newDissatisfiedScore == queryParameter ||
				oldNotBlockingScore == queryParameter ||
				newNotBlockingScore == queryParameter ||
				oldBlockingScore == queryParameter ||
				newBlockingScore == queryParameter ||

				( queryParameter == MAX_QUERY_PARAMETER &&

				( assignmentLevel_ > NO_ASSIGNMENT_LEVEL ||
				oldSatisfiedScore > NO_SCORE ||
				newSatisfiedScore > NO_SCORE ||
				oldDissatisfiedScore > NO_SCORE ||
				newDissatisfiedScore > NO_SCORE ||
				oldNotBlockingScore > NO_SCORE ||
				newNotBlockingScore > NO_SCORE ||
				oldBlockingScore > NO_SCORE ||
				newBlockingScore > NO_SCORE ) ) );
		}

	virtual bool hasFoundReferenceItemById( unsigned int querySentenceNr, unsigned int queryItemNr )
		{
		return ( referenceSelectionItem == NULL ? false :
				( querySentenceNr == NO_SENTENCE_NR ? true : referenceSelectionItem->creationSentenceNr() == querySentenceNr ) &&
				( queryItemNr == NO_ITEM_NR ? true : referenceSelectionItem->itemNr() == queryItemNr ) );
		}

	virtual bool isSorted( Item *nextSortItem )
		{
		return ( nextSortItem != NULL &&
				// Descending assignmentLevel_
				assignmentLevel_ > ( (ScoreItem *)nextSortItem )->assignmentLevel_ );
		}

	virtual char *toString( unsigned short queryWordTypeNr )
		{
		char *queryString;

		Item::toString( queryWordTypeNr );

		queryString = commonVariables()->queryString;

		if( isMarked )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isMarked" );
			}

		if( isChecked )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isChecked" );
			}

		if( oldSatisfiedScore > NO_SCORE )
			{
			sprintf( tempString, "%coldsatisfiedscore:%u", QUERY_SEPARATOR_CHAR, oldSatisfiedScore );
			strcat( queryString, tempString );
			}

		if( newSatisfiedScore > NO_SCORE )
			{
			sprintf( tempString, "%cnewsatisfiedscore:%u", QUERY_SEPARATOR_CHAR, newSatisfiedScore );
			strcat( queryString, tempString );
			}

		if( oldDissatisfiedScore > NO_SCORE )
			{
			sprintf( tempString, "%colddissatisfiedscore:%u", QUERY_SEPARATOR_CHAR, oldDissatisfiedScore );
			strcat( queryString, tempString );
			}

		if( newDissatisfiedScore > NO_SCORE )
			{
			sprintf( tempString, "%cnewdissatisfiedscore:%u", QUERY_SEPARATOR_CHAR, newDissatisfiedScore );
			strcat( queryString, tempString );
			}

		if( oldNotBlockingScore > NO_SCORE )
			{
			sprintf( tempString, "%coldnotblockingscore:%u", QUERY_SEPARATOR_CHAR, oldNotBlockingScore );
			strcat( queryString, tempString );
			}

		if( newNotBlockingScore > NO_SCORE )
			{
			sprintf( tempString, "%cnewnotblockingscore:%u", QUERY_SEPARATOR_CHAR, newNotBlockingScore );
			strcat( queryString, tempString );
			}

		if( oldBlockingScore > NO_SCORE )
			{
			sprintf( tempString, "%coldblockingscore:%u", QUERY_SEPARATOR_CHAR, oldBlockingScore );
			strcat( queryString, tempString );
			}

		if( newBlockingScore > NO_SCORE )
			{
			sprintf( tempString, "%cnewblockingscore:%u", QUERY_SEPARATOR_CHAR, newBlockingScore );
			strcat( queryString, tempString );
			}

		if( referenceSelectionItem != NULL )
			{
			sprintf( tempString, "%cselectionReference%c%u%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_REF_ITEM_START_CHAR, referenceSelectionItem->creationSentenceNr(), QUERY_SEPARATOR_CHAR, referenceSelectionItem->itemNr(), QUERY_REF_ITEM_END_CHAR );
			strcat( queryString, tempString );
			}

		return queryString;
		}


	// Protected functions

	bool hasOldSatisfiedScore()
		{
		return ( oldSatisfiedScore > NO_SCORE );
		}

	bool hasNewSatisfiedScore()
		{
		return ( newSatisfiedScore > NO_SCORE );
		}

	bool hasOldDissatisfiedScore()
		{
		return ( oldDissatisfiedScore > NO_SCORE );
		}

	bool hasNewDissatisfiedScore()
		{
		return ( newDissatisfiedScore > NO_SCORE );
		}

	bool hasOldNotBlockingScore()
		{
		return ( oldNotBlockingScore > NO_SCORE );
		}

	bool hasNewNotBlockingScore()
		{
		return ( newNotBlockingScore > NO_SCORE );
		}

	bool hasOldBlockingScore()
		{
		return ( oldBlockingScore > NO_SCORE );
		}

	bool hasNewBlockingScore()
		{
		return ( newBlockingScore > NO_SCORE );
		}

	unsigned short assignmentLevel()
		{
		return assignmentLevel_;
		}

	ScoreItem *firstPossibilityItem()
		{
		return possibilityItem( true );
		}

	ScoreItem *nextPossibilityItem()
		{
		return possibilityItem( false );
		}

	ScoreItem *nextScoreItem()
		{
		return (ScoreItem *)nextItem;
		}

	SelectionItem *scoreReference()
		{
		return referenceSelectionItem;
		}
	};

/*************************************************************************
 *	"The Lord rules over the floodwaters.
 *	The Lord reigns as king forever." (Psalm 29:10)
 *************************************************************************/