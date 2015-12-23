/*
 *	Class:			WordList
 *	Parent class:	List
 *	Purpose:		To store word items
 *	Version:		Thinknowlogy 2015r1 (Esperanza)
 *************************************************************************/
/*	Copyright (C) 2009-2015, Menno Mafait. Your suggestions, modifications
 *	and bug reports are welcome at http://mafait.org
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

#ifndef WORDLIST
#define WORDLIST 1
#include "List.h"
#include "WordItem.h"

class WordList : private List
	{
	friend class AdminCleanup;
	friend class AdminItem;
	friend class AdminQuery;
	friend class AdminReadCreateWords;
	friend class AdminSolve;

	// Private cleanup functions

	void deleteRollbackInfoInWordList( WordItem *searchItem )
		{
		while( searchItem != NULL )
			{
			searchItem->deleteRollbackInfo();
			searchItem = searchItem->nextWordItem();
			}
		}

	void getHighestInUseSentenceNrInWordList( bool isIncludingDeletedItems, bool isIncludingTemporaryLists, unsigned int highestSentenceNr, WordItem *searchItem )
		{
		while( searchItem != NULL &&
		commonVariables()->highestInUseSentenceNr < highestSentenceNr )
			{
			searchItem->getHighestInUseSentenceNrInWord( isIncludingDeletedItems, isIncludingTemporaryLists, highestSentenceNr );
			searchItem = searchItem->nextWordItem();
			}
		}

	void setCurrentItemNrInWordList( WordItem *searchItem )
		{
		while( searchItem != NULL )
			{
			searchItem->setCurrentItemNrInWord();
			searchItem = searchItem->nextWordItem();
			}
		}

	unsigned int highestSentenceNrInWordList( WordItem *searchItem )
		{
		unsigned int tempSentenceNr;
		unsigned int highestSentenceNr = NO_SENTENCE_NR;

		while( searchItem != NULL )
			{
			if( ( tempSentenceNr = searchItem->highestSentenceNr() ) > highestSentenceNr )
				highestSentenceNr = tempSentenceNr;

			searchItem = searchItem->nextWordItem();
			}

		return highestSentenceNr;
		}

	ResultType deleteSentencesInWordList( bool isAvailableForRollback, unsigned int lowestSentenceNr, WordItem *searchItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "deleteSentencesInWordList";

		while( searchItem != NULL )
			{
			if( searchItem->deleteSentencesInWord( isAvailableForRollback, lowestSentenceNr ) == RESULT_OK )
				searchItem = searchItem->nextWordItem();
			else
				return addError( functionNameString, NULL, "I failed to delete sentences in a word" );
			}

		return RESULT_OK;
		}

	ResultType removeFirstRangeOfDeletedItemsInWordList( WordItem *searchItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "removeFirstRangeOfDeletedItemsInWordList";

		while( searchItem != NULL &&
		commonVariables()->nDeletedItems == 0 )
			{
			if( searchItem->removeFirstRangeOfDeletedItems() == RESULT_OK )
				searchItem = searchItem->nextWordItem();
			else
				return addError( functionNameString, NULL, "I failed to remove the first deleted items in a word" );
			}

		return RESULT_OK;
		}

	ResultType rollbackDeletedRedoInfoInWordList( WordItem *searchItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "rollbackDeletedRedoInfoInWordList";

		while( searchItem != NULL )
			{
			if( searchItem->rollbackDeletedRedoInfo() == RESULT_OK )
				searchItem = searchItem->nextWordItem();
			else
				return addError( functionNameString, NULL, "I failed to rollback a deleted item in a word" );
			}

		return RESULT_OK;
		}

	ResultType decrementSentenceNrsInWordList( unsigned int startSentenceNr, WordItem *searchItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "decrementSentenceNrsInWordList";

		while( searchItem != NULL )
			{
			if( searchItem->decrementSentenceNrsInWord( startSentenceNr ) == RESULT_OK )
				searchItem = searchItem->nextWordItem();
			else
				return addError( functionNameString, NULL, "I failed to decrement the sentence numbers from the current sentence number in a word" );
			}

		return RESULT_OK;
		}

	ResultType decrementItemNrRangeInWordList( unsigned int decrementSentenceNr, unsigned int decrementItemNr, unsigned int decrementOffset, WordItem *searchItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "decrementItemNrRangeInWordList";

		while( searchItem != NULL )
			{
			if( searchItem->decrementItemNrRangeInWord( decrementSentenceNr, decrementItemNr, decrementOffset ) == RESULT_OK )
				searchItem = searchItem->nextWordItem();
			else
				return addError( functionNameString, NULL, "I failed to decrement item numbers in a word" );
			}

		return RESULT_OK;
		}


	// Private query functions

	void countQueryInWordList( WordItem *searchItem )
		{
		while( searchItem != NULL )
			{
			searchItem->countQuery();
			searchItem = searchItem->nextWordItem();
			}
		}

	void clearQuerySelectionsInWordList( WordItem *searchItem )
		{
		while( searchItem != NULL )
			{
			searchItem->clearQuerySelections();
			searchItem = searchItem->nextWordItem();
			}
		}

	ResultType wordQueryInWordList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, char *wordNameString, WordItem *searchItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "wordQueryInWordList";

		while( searchItem != NULL )
			{
			if( searchItem->wordQueryInWord( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, wordNameString ) == RESULT_OK )
				searchItem = searchItem->nextWordItem();
			else
				return addError( functionNameString, NULL, "I failed to query a word" );
			}

		return RESULT_OK;
		}

	ResultType itemQueryInWordList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, bool isReferenceQuery, unsigned int querySentenceNr, unsigned int queryItemNr, WordItem *searchItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "itemQueryInWordList";

		while( searchItem != NULL )
			{
			if( searchItem->itemQueryInWord( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isReferenceQuery, querySentenceNr, queryItemNr ) == RESULT_OK )
				searchItem = searchItem->nextWordItem();
			else
				return addError( functionNameString, NULL, "I failed to query item numbers in a word" );
			}

		return RESULT_OK;
		}

	ResultType listQueryInWordList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, char *queryListString, WordItem *searchItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "listQueryInWordList";

		while( searchItem != NULL )
			{
			if( searchItem->listQueryInWord( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryListString ) == RESULT_OK )
				searchItem = searchItem->nextWordItem();
			else
				return addError( functionNameString, NULL, "I failed to query lists in a word" );
			}

		return RESULT_OK;
		}

	ResultType wordTypeQueryInWordList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, unsigned short queryWordTypeNr, WordItem *searchItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "wordTypeQueryInWordList";

		while( searchItem != NULL )
			{
			if( searchItem->wordTypeQueryInWord( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryWordTypeNr ) == RESULT_OK )
				searchItem = searchItem->nextWordItem();
			else
				return addError( functionNameString, NULL, "I failed to query word types in a word" );
			}

		return RESULT_OK;
		}

	ResultType parameterQueryInWordList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, unsigned int queryParameter, WordItem *searchItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "parameterQueryInWordList";

		while( searchItem != NULL )
			{
			if( isSelectingOnFind &&
			searchItem->hasFoundParameter( queryParameter ) )
				searchItem->isSelectedByQuery = true;

			if( searchItem->parameterQueryInWord( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryParameter ) == RESULT_OK )
				searchItem = searchItem->nextWordItem();
			else
				return addError( functionNameString, NULL, "I failed to query parameters in a word" );
			}

		return RESULT_OK;
		}

	ResultType wordReferenceQueryInWordList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, bool isSelectingAttachedJustifications, bool isSelectingJustificationSpecifications, char *wordReferenceNameString, WordItem *searchItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "wordReferenceQueryInWordList";

		while( searchItem != NULL )
			{
			if( searchItem->wordReferenceQueryInWord( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isSelectingAttachedJustifications, isSelectingJustificationSpecifications, wordReferenceNameString ) == RESULT_OK )
				searchItem = searchItem->nextWordItem();
			else
				return addError( functionNameString, NULL, "I failed to query word references in a word" );
			}

		return RESULT_OK;
		}

	ResultType stringQueryInWordList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, char *queryString, WordItem *searchItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "stringQueryInWordList";

		while( searchItem != NULL )
			{
			if( searchItem->stringQueryInWord( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryString ) == RESULT_OK )
				searchItem = searchItem->nextWordItem();
			else
				return addError( functionNameString, NULL, "I failed to query strings in a word" );
			}

		return RESULT_OK;
		}

	ResultType showQueryResultInWordList( bool isOnlyShowingWords, bool isOnlyShowingWordReferences, bool isOnlyShowingStrings, bool isReturnQueryToPosition, unsigned short promptTypeNr, unsigned short queryWordTypeNr, size_t queryWidth, WordItem *searchItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "showQueryResultInWordList";

		while( searchItem != NULL )
			{
			if( searchItem->showQueryResultInWord( isOnlyShowingWords, isOnlyShowingWordReferences, isOnlyShowingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth ) == RESULT_OK )
				searchItem = searchItem->nextWordItem();
			else
				return addError( functionNameString, NULL, "I failed to show the query result in a word" );
			}

		return RESULT_OK;
		}


	// Private common functions

	WordItem *firstActiveWordItem()
		{
		return (WordItem *)firstActiveItem();
		}

	WordItem *firstReplacedWordItem()
		{
		return (WordItem *)firstReplacedItem();
		}

	WordItem *firstDeletedWordItem()
		{
		return (WordItem *)firstDeletedItem();
		}


	protected:
	// Constructor / deconstructor

	WordList( CommonVariables *commonVariables, WordItem *myWordItem )
		{
		initializeListVariables( ADMIN_WORD_LIST_SYMBOL, "WordList", commonVariables, myWordItem );
		}

	~WordList()
		{
		WordItem *deleteItem;
		WordItem *searchItem = firstActiveWordItem();

		while( searchItem != NULL )
			{
			deleteItem = searchItem;
			searchItem = searchItem->nextWordItem();
			delete deleteItem;
			}

		if( firstInactiveItem() != NULL )
			fprintf( stderr, "\nError: Class WordList has inactive items." );

		if( firstArchivedItem() )
			fprintf( stderr, "\nError: Class WordList has archived items." );

		searchItem = firstReplacedWordItem();

		while( searchItem != NULL )
			{
			deleteItem = searchItem;
			searchItem = searchItem->nextWordItem();
			delete deleteItem;
			}

		searchItem = firstDeletedWordItem();

		while( searchItem != NULL )
			{
			deleteItem = searchItem;
			searchItem = searchItem->nextWordItem();
			delete deleteItem;
			}
		}


	// Protected assignment functions

	ResultType createNewAssignmentLevelInWordList()
		{
		WordItem *searchItem = firstActiveWordItem();

		while( commonVariables()->result == RESULT_OK &&
		searchItem != NULL )
			{
			searchItem->createNewAssignmentLevel();
			searchItem = searchItem->nextWordItem();
			}

		return commonVariables()->result;
		}


	// Protected cleanup functions

	void deleteRollbackInfoInWordList()
		{
		deleteRollbackInfoInWordList( firstActiveWordItem() );
		deleteRollbackInfoInWordList( firstReplacedWordItem() );
		}

	void getHighestInUseSentenceNrInWordList( bool isIncludingDeletedItems, bool isIncludingTemporaryLists, unsigned int highestSentenceNr )
		{
		getHighestInUseSentenceNrInWordList( isIncludingDeletedItems, isIncludingTemporaryLists, highestSentenceNr, firstActiveWordItem() );

		if( commonVariables()->highestInUseSentenceNr < highestSentenceNr )
			{
			getHighestInUseSentenceNrInWordList( isIncludingDeletedItems, isIncludingTemporaryLists, highestSentenceNr, firstReplacedWordItem() );

			if( isIncludingDeletedItems &&
			commonVariables()->highestInUseSentenceNr < highestSentenceNr )
				getHighestInUseSentenceNrInWordList( isIncludingDeletedItems, isIncludingTemporaryLists, highestSentenceNr, firstDeletedWordItem() );
			}
		}

	void setCurrentItemNrInWordList()
		{
		setCurrentItemNrInWordList( firstActiveWordItem() );
		setCurrentItemNrInWordList( firstReplacedWordItem() );
		setCurrentItemNrInWordList( firstDeletedWordItem() );
		}

	unsigned int highestSentenceNrInWordList()
		{
		unsigned int tempSentenceNr;
		unsigned int highestSentenceNr = highestSentenceNrInWordList( firstActiveWordItem() );

		if( ( tempSentenceNr = highestSentenceNrInWordList( firstReplacedWordItem() ) ) > highestSentenceNr )
			highestSentenceNr = tempSentenceNr;

		if( ( tempSentenceNr = highestSentenceNrInWordList( firstDeletedWordItem() ) ) > highestSentenceNr )
			highestSentenceNr = tempSentenceNr;

		return highestSentenceNr;
		}

	ResultType decrementItemNrRangeInWordList( unsigned int decrementSentenceNr, unsigned int decrementItemNr, unsigned int decrementOffset )
		{
		if( decrementItemNrRangeInWordList( decrementSentenceNr, decrementItemNr, decrementOffset, firstActiveWordItem() ) == RESULT_OK &&
		decrementItemNrRangeInWordList( decrementSentenceNr, decrementItemNr, decrementOffset, firstReplacedWordItem() ) == RESULT_OK )
			return decrementItemNrRangeInWordList( decrementSentenceNr, decrementItemNr, decrementOffset, firstDeletedWordItem() );

		return commonVariables()->result;
		}

	ResultType decrementSentenceNrsInWordList( unsigned int startSentenceNr )
		{
		if( decrementSentenceNrsInWordList( startSentenceNr, firstActiveWordItem() ) == RESULT_OK &&
		decrementSentenceNrsInWordList( startSentenceNr, firstReplacedWordItem() ) == RESULT_OK )
			return decrementSentenceNrsInWordList( startSentenceNr, firstDeletedWordItem() );

		return commonVariables()->result;
		}

	ResultType deleteSentencesInWordList( bool isAvailableForRollback, unsigned int lowestSentenceNr )
		{
		if( deleteSentencesInWordList( isAvailableForRollback, lowestSentenceNr, firstActiveWordItem() ) == RESULT_OK &&
		deleteSentencesInWordList( isAvailableForRollback, lowestSentenceNr, firstReplacedWordItem() ) == RESULT_OK )
			return deleteSentencesInWordList( isAvailableForRollback, lowestSentenceNr, firstDeletedWordItem() );

		return commonVariables()->result;
		}

	ResultType removeFirstRangeOfDeletedItemsInWordList()
		{
		if( removeFirstRangeOfDeletedItemsInWordList( firstActiveWordItem() ) == RESULT_OK &&
		removeFirstRangeOfDeletedItemsInWordList( firstReplacedWordItem() ) == RESULT_OK )
			return removeFirstRangeOfDeletedItemsInWordList( firstDeletedWordItem() );

		return commonVariables()->result;
		}

	ResultType rollbackDeletedRedoInfoInWordList()
		{
		if( rollbackDeletedRedoInfoInWordList( firstActiveWordItem() ) == RESULT_OK &&
		rollbackDeletedRedoInfoInWordList( firstReplacedWordItem() ) == RESULT_OK )
			return rollbackDeletedRedoInfoInWordList( firstDeletedWordItem() );

		return commonVariables()->result;
		}


	// Protected database connection functions
/*
	ResultType storeChangesInFutureDatabase()
		{
		WordItem *searchItem = firstActiveWordItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "storeChangesInFutureDatabase";

		while( searchItem != NULL )
			{
			// Do for all active words
			if( searchItem->storeWordItemInFutureDatabase() != RESULT_OK )
				return addError( functionNameString, NULL, "I failed to store a word item in the database" );

			searchItem = searchItem->nextWordItem();
			}

		searchItem = firstReplacedWordItem();

		while( searchItem != NULL )
			{
			if( searchItem->hasCurrentCreationSentenceNr() )
				{
				if( searchItem->storeWordItemInFutureDatabase() != RESULT_OK )
					return addError( functionNameString, NULL, "I failed to modify a replaced word item in the database" );
				}

			searchItem = searchItem->nextWordItem();
			}

		return RESULT_OK;
		}
*/

	// Protected query functions

	void countQueryInWordList()
		{
		countQueryInWordList( firstActiveWordItem() );
		countQueryInWordList( firstReplacedWordItem() );
		countQueryInWordList( firstDeletedWordItem() );
		}

	void clearQuerySelectionsInWordList()
		{
		clearQuerySelectionsInWordList( firstActiveWordItem() );
		clearQuerySelectionsInWordList( firstReplacedWordItem() );
		clearQuerySelectionsInWordList( firstDeletedWordItem() );
		}

	ResultType wordQueryInWordList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, char *wordNameString )
		{
		if( wordQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, wordNameString, firstActiveWordItem() ) == RESULT_OK &&
		wordQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, wordNameString, firstReplacedWordItem() ) == RESULT_OK )
			return wordQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, wordNameString, firstDeletedWordItem() );

		return commonVariables()->result;
		}

	ResultType itemQueryInWordList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, bool isReferenceQuery, unsigned int querySentenceNr, unsigned int queryItemNr )
		{
		if( itemQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isReferenceQuery, querySentenceNr, queryItemNr, firstActiveWordItem() ) == RESULT_OK &&
		itemQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isReferenceQuery, querySentenceNr, queryItemNr, firstReplacedWordItem() ) == RESULT_OK )
			return itemQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isReferenceQuery, querySentenceNr, queryItemNr, firstDeletedWordItem() );

		return commonVariables()->result;
		}

	ResultType listQueryInWordList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, char *queryListString )
		{
		if( listQueryInList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryListString ) == RESULT_OK &&
		listQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryListString, firstActiveWordItem() ) == RESULT_OK &&
		listQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryListString, firstReplacedWordItem() ) == RESULT_OK )
			return listQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryListString, firstDeletedWordItem() );

		return commonVariables()->result;
		}

	ResultType wordTypeQueryInWordList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, unsigned short queryWordTypeNr )
		{
		if( wordTypeQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryWordTypeNr, firstActiveWordItem() ) == RESULT_OK &&
		wordTypeQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryWordTypeNr, firstReplacedWordItem() ) == RESULT_OK )
			return wordTypeQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryWordTypeNr, firstDeletedWordItem() );

		return commonVariables()->result;
		}

	ResultType parameterQueryInWordList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, unsigned int queryParameter )
		{
		if( parameterQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryParameter, firstActiveWordItem() ) == RESULT_OK &&
		parameterQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryParameter, firstReplacedWordItem() ) == RESULT_OK )
			return parameterQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryParameter, firstDeletedWordItem() );

		return commonVariables()->result;
		}

	ResultType wordReferenceQueryInWordList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, bool isSelectingAttachedJustifications, bool isSelectingJustificationSpecifications, char *wordReferenceNameString )
		{
		if( wordReferenceQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isSelectingAttachedJustifications, isSelectingJustificationSpecifications, wordReferenceNameString, firstActiveWordItem() ) == RESULT_OK &&
		wordReferenceQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isSelectingAttachedJustifications, isSelectingJustificationSpecifications, wordReferenceNameString, firstReplacedWordItem() ) == RESULT_OK )
			return wordReferenceQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isSelectingAttachedJustifications, isSelectingJustificationSpecifications, wordReferenceNameString, firstDeletedWordItem() );

		return commonVariables()->result;
		}

	ResultType stringQueryInWordList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, char *queryString )
		{
		if( stringQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryString, firstActiveWordItem() ) == RESULT_OK &&
		stringQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryString, firstReplacedWordItem() ) == RESULT_OK )
			return stringQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryString, firstDeletedWordItem() );

		return commonVariables()->result;
		}

	ResultType showQueryResultInWordList( bool isOnlyShowingWords, bool isOnlyShowingWordReferences, bool isOnlyShowingStrings, bool isReturnQueryToPosition, unsigned short promptTypeNr, unsigned short queryWordTypeNr, size_t queryWidth )
		{
		if( showQueryResultInWordList( isOnlyShowingWords, isOnlyShowingWordReferences, isOnlyShowingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth, firstActiveWordItem() ) == RESULT_OK &&
		showQueryResultInWordList( isOnlyShowingWords, isOnlyShowingWordReferences, isOnlyShowingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth, firstReplacedWordItem() ) == RESULT_OK )
			return showQueryResultInWordList( isOnlyShowingWords, isOnlyShowingWordReferences, isOnlyShowingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth, firstDeletedWordItem() );

		return commonVariables()->result;
		}


	// Protected read word functions

	WordResultType createWordItem( bool isLanguageWord, unsigned short wordParameter )
		{
		WordResultType wordResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "createWordItem";

		if( commonVariables()->currentItemNr < MAX_ITEM_NR )
			{
			// Create word
			if( ( wordResult.createdWordItem = new WordItem( isLanguageWord, wordParameter, commonVariables(), this ) ) != NULL )
				{
				if( addItemToList( QUERY_ACTIVE_CHAR, wordResult.createdWordItem ) != RESULT_OK )
					addError( functionNameString, NULL, "I failed to add an active word item" );
				}
			else
				startError( functionNameString, NULL, "I failed to create a word item" );
			}
		else
			startError( functionNameString, NULL, "The current item number is undefined" );

		wordResult.result = commonVariables()->result;
		return wordResult;
		}
	};
#endif

/*************************************************************************
 *	"They share freely and give generously to those in need.
 *	Their good deeds will be remembered forever.
 *	They will have influence and honor." (Psalm 112:9)
 *************************************************************************/
