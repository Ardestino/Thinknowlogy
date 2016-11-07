/*	Class:			MultipleWordList
 *	Parent class:	List
 *	Purpose:		To store multiple word items
 *	Version:		Thinknowlogy 2016r2 (Restyle)
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

#include "MultipleWordItem.cpp"
#include "List.h"

class MultipleWordList : private List
	{
	friend class WordItem;

	// Private functions

	bool hasFoundMultipleWordItem( unsigned short wordTypeNr, WordItem *multipleWordItem )
		{
		MultipleWordItem *searchMultipleWordItem = firstActiveMultipleWordItem();

		while( searchMultipleWordItem != NULL )
			{
			if( searchMultipleWordItem->wordTypeNr() == wordTypeNr &&
			searchMultipleWordItem->multipleWordItem() == multipleWordItem )
				return true;

			searchMultipleWordItem = searchMultipleWordItem->nextMultipleWordItem();
			}

		return false;
		}

	MultipleWordItem *firstActiveMultipleWordItem()
		{
		return (MultipleWordItem *)firstActiveItem();
		}

	protected:
	// Constructor

	MultipleWordList( CommonVariables *commonVariables, WordItem *myWordItem )
		{
		initializeListVariables( WORD_MULTIPLE_WORD_LIST_SYMBOL, "MultipleWordList", commonVariables, myWordItem );
		}

	~MultipleWordList()
		{
		MultipleWordItem *deleteMultipleWordItem;
		MultipleWordItem *searchMultipleWordItem = firstActiveMultipleWordItem();

		while( searchMultipleWordItem != NULL )
			{
			deleteMultipleWordItem = searchMultipleWordItem;
			searchMultipleWordItem = searchMultipleWordItem->nextMultipleWordItem();
			delete deleteMultipleWordItem;
			}

		if( firstInactiveItem() != NULL )
			fprintf( stderr, "\nError: Class MultipleWordList has inactive items." );

		if( firstArchivedItem() != NULL )
			fprintf( stderr, "\nError: Class MultipleWordList has archived items." );

		if( firstReplacedItem() != NULL )
			fprintf( stderr, "\nError: Class MultipleWordList has replaced items." );

		searchMultipleWordItem = (MultipleWordItem *)firstDeletedItem();

		while( searchMultipleWordItem != NULL )
			{
			deleteMultipleWordItem = searchMultipleWordItem;
			searchMultipleWordItem = searchMultipleWordItem->nextMultipleWordItem();
			delete deleteMultipleWordItem;
			}
		}


	// Protected functions

	unsigned short matchingMultipleSingularNounWordParts( char *sentenceString )
		{
		unsigned short currentLanguageNr = commonVariables()->currentLanguageNr;
		MultipleWordItem *searchMultipleWordItem = firstActiveMultipleWordItem();
		WordItem *multipleWordItem;
		char *multipleWordString;

		if( sentenceString != NULL )
			{
			while( searchMultipleWordItem != NULL )
				{
				if( searchMultipleWordItem->isSingularNoun() &&
				searchMultipleWordItem->wordTypeLanguageNr() == currentLanguageNr &&
				( multipleWordItem = searchMultipleWordItem->multipleWordItem() ) != NULL )
					{
					multipleWordString = multipleWordItem->singularNounString();

					if( multipleWordString != NULL &&
					strncmp( sentenceString, multipleWordString, strlen( multipleWordString ) ) == 0 )
						return searchMultipleWordItem->nWordParts();
					}

				searchMultipleWordItem = searchMultipleWordItem->nextMultipleWordItem();
				}
			}

		return 0;
		}

	ResultType addMultipleWord( unsigned short nWordParts, unsigned short wordTypeNr, WordItem *multipleWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "addMultipleWord";

		if( wordTypeNr <= NO_WORD_TYPE_NR ||
		wordTypeNr >= NUMBER_OF_WORD_TYPES )
			return startError( functionNameString, NULL, "The given word type number is undefined or out of bounds: ", wordTypeNr );

		if( multipleWordItem == NULL )
			return startError( functionNameString, NULL, "The given multiple word item is undefined" );

		if( !hasFoundMultipleWordItem( wordTypeNr, multipleWordItem ) )
			{
			if( addItemToList( QUERY_ACTIVE_CHAR, new MultipleWordItem( nWordParts, commonVariables()->currentLanguageNr, wordTypeNr, multipleWordItem, commonVariables(), this, myWordItem() ) ) != RESULT_OK )
				return addError( functionNameString, NULL, "I failed to add an active multiple word item" );
			}

		return RESULT_OK;
		}

	ResultType checkWordItemForUsage( WordItem *unusedWordItem )
		{
		MultipleWordItem *searchMultipleWordItem = firstActiveMultipleWordItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkWordItemForUsage";

		if( unusedWordItem == NULL )
			return startError( functionNameString, NULL, "The given unused word item is undefined" );

		while( searchMultipleWordItem != NULL )
			{
			if( searchMultipleWordItem->multipleWordItem() == unusedWordItem )
				return startError( functionNameString, NULL, "The multiple word item is still in use" );

			searchMultipleWordItem = searchMultipleWordItem->nextMultipleWordItem();
			}

		return RESULT_OK;
		}
/*
	ResultType storeChangesInFutureDatabase()
		{
		MultipleWordItem *searchMultipleWordItem = firstActiveMultipleWordItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "storeChangesInFutureDatabase";

		while( searchMultipleWordItem != NULL )
			{
			if( searchMultipleWordItem->hasCurrentCreationSentenceNr() )
				{
				if( searchMultipleWordItem->storeMultipleWordItemInFutureDatabase() != RESULT_OK )
					return addError( functionNameString, NULL, "I failed to store a multiple word item in the database" );
				}

			searchMultipleWordItem = searchMultipleWordItem->nextMultipleWordItem();
			}

		return RESULT_OK;
		}
*/	};

/*************************************************************************
 *	"The one thing I ask of the Lord -
 *	the thing I seek most -
 *	is to live in the house of the Lord all the days of my life,
 *	delighting in the Lord's perfections
 *	and meditating in his Temple." (Psalm 27:4)
 *************************************************************************/
