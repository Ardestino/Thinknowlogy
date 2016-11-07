/*	Class:			WordType
 *	Supports class:	WordItem
 *	Purpose:		To create word type structures
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

#include "WordTypeList.cpp"

class WordType
	{
	friend class WordItem;

	// Private constructed variables

	bool hasFeminineWordEnding_;
	bool hasMasculineWordEnding_;

	// This string is returned by a function. So, it must be "static".
	char wordItemNameString_[MAX_SENTENCE_STRING_LENGTH];

	CommonVariables *commonVariables_;
	WordItem *myWordItem_;
	char moduleNameString_[FUNCTION_NAME_LENGTH];


	// Private functions

	ResultType checkOnFeminineAndMasculineWordEnding( bool isSingularNoun, char *wordString )
		{
		GrammarResultType grammarResult;
		WordItem *currentLanguageWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkOnFeminineAndMasculineWordEnding";

		hasFeminineWordEnding_ = false;
		hasMasculineWordEnding_ = false;

		if( wordString == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given word string is undefined" );

		if( ( currentLanguageWordItem = commonVariables_->currentLanguageWordItem ) == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The current language word item is undefined" );

		if( ( grammarResult = currentLanguageWordItem->analyzeWordEnding( ( isSingularNoun ? WORD_FEMININE_SINGULAR_NOUN_ENDING : WORD_FEMININE_PROPER_NAME_ENDING ), 0, wordString ) ).result != RESULT_OK )
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to check on feminine word ending" );

		if( grammarResult.hasFoundWordEnding )
			{
			hasFeminineWordEnding_ = true;

			if( isSingularNoun )
				{
				if( myWordItem_->markWordAsFeminine() != RESULT_OK )
					return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to mark my word as feminine" );
				}
			}
		else
			{
			if( ( grammarResult = currentLanguageWordItem->analyzeWordEnding( ( isSingularNoun ? WORD_MASCULINE_SINGULAR_NOUN_ENDING : WORD_MASCULINE_PROPER_NAME_ENDING ), 0, wordString ) ).result != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to check on masculine word ending" );

			if( grammarResult.hasFoundWordEnding )
				{
				hasMasculineWordEnding_ = true;

				if( isSingularNoun )
					{
					if( myWordItem_->markWordAsMasculine() != RESULT_OK )
						return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to mark my word as masculine" );
					}
				}
			}

		return RESULT_OK;
		}


	protected:
	// Constructor

	WordType( CommonVariables *commonVariables, WordItem *myWordItem )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		hasFeminineWordEnding_ = false;
		hasMasculineWordEnding_ = false;

		strcpy( wordItemNameString_, EMPTY_STRING );

		commonVariables_ = commonVariables;
		myWordItem_ = myWordItem;
		strcpy( moduleNameString_, "WordType" );

		if( commonVariables_ == NULL )
			strcpy( errorString, "The given common variables is undefined" );

		if( myWordItem_ == NULL )
			strcpy( errorString, "The given my word is undefined" );

		if( strlen( errorString ) > 0 )
			{
			if( myWordItem_ != NULL )
				myWordItem_->startSystemErrorInWord( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, moduleNameString_, errorString );
			else
				{
			if( commonVariables_ != NULL )
				commonVariables_->result = RESULT_SYSTEM_ERROR;
				fprintf( stderr, "\nClass:%s\nFunction:\t%s\nError:\t\t%s.\n", moduleNameString_, PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, errorString );
				}
			}
		}


	// Protected functions

	WordResultType addWordType( bool isLanguageWord, bool isMultipleWord, bool isProperNamePrecededByDefiniteArticle, unsigned short adjectiveParameter, unsigned short definiteArticleParameter, unsigned short indefiniteArticleParameter, unsigned short wordTypeNr, size_t wordLength, char *wordTypeString )
		{
		WordResultType wordResult;
		bool isSingularNoun;
		char functionNameString[FUNCTION_NAME_LENGTH] = "addWordType";

		hasFeminineWordEnding_ = false;
		hasMasculineWordEnding_ = false;

		if( myWordItem_->wordTypeList == NULL )
			{
			// Create list
			if( ( myWordItem_->wordTypeList = new WordTypeList( commonVariables_, myWordItem_ ) ) == NULL )
				return myWordItem_->startWordResultErrorInWord( functionNameString, moduleNameString_, "I failed to create a word type list" );

			myWordItem_->wordListArray[WORD_TYPE_LIST] = myWordItem_->wordTypeList;
			}
		else
			{
			// Check if word type already exists
			if( ( wordResult = findWordType( false, wordTypeNr, wordTypeString ) ).result != RESULT_OK )
				return myWordItem_->addWordResultErrorInWord( functionNameString, moduleNameString_, "I failed to find the given word type" );
			}

		// Add word type if it doesn't exist yet
		if( wordResult.foundWordTypeItem == NULL )
			{
			isSingularNoun = ( wordTypeNr == WORD_TYPE_NOUN_SINGULAR );

			if( isSingularNoun ||

			( !isLanguageWord &&
			wordTypeNr == WORD_TYPE_PROPER_NAME ) )
				{
				if( checkOnFeminineAndMasculineWordEnding( isSingularNoun, wordTypeString ) != RESULT_OK )
					return myWordItem_->addWordResultErrorInWord( functionNameString, moduleNameString_, "I failed to check the string on feminine or masculine word ending" );
				}

			if( ( wordResult = myWordItem_->wordTypeList->createWordTypeItem( ( hasFeminineWordEnding_ && !isMultipleWord ), hasMasculineWordEnding_, isProperNamePrecededByDefiniteArticle, adjectiveParameter, definiteArticleParameter, indefiniteArticleParameter, wordTypeNr, wordLength, wordTypeString ) ).result != RESULT_OK )
				return myWordItem_->addWordResultErrorInWord( functionNameString, moduleNameString_, "I failed to create a word type item" );
			}

		return wordResult;
		}

	WordResultType findWordType( bool isCheckingAllLanguages, unsigned short wordTypeNr, char *wordTypeString )
		{
		WordResultType wordResult;
		size_t currentWordTypeStringLength;
		size_t wordTypeStringLength;
		char *currentWordTypeString;
		WordTypeItem *currentWordTypeItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "findWordType";

		if( wordTypeString == NULL )
			return myWordItem_->startWordResultErrorInWord( functionNameString, moduleNameString_, "The given word type string is undefined" );

		if( ( wordTypeStringLength = strlen( wordTypeString ) ) == 0 )
			return myWordItem_->startWordResultErrorInWord( functionNameString, moduleNameString_, "The given word type string is empty" );

		if( ( currentWordTypeItem = myWordItem_->activeWordTypeItem( isCheckingAllLanguages, wordTypeNr ) ) != NULL )
			{
			do	{
				// Skip hidden word type
				if( ( currentWordTypeString = currentWordTypeItem->itemString() ) != NULL )
					{
					if( ( currentWordTypeStringLength = strlen( currentWordTypeString ) ) == 0 )
						return myWordItem_->startWordResultErrorInWord( functionNameString, moduleNameString_, "The active word type string is empty" );

					if( wordTypeStringLength == currentWordTypeStringLength &&
					strcmp( wordTypeString, currentWordTypeString ) == 0 )
						{
						wordResult.foundWordItem = myWordItem_;
						wordResult.foundWordTypeItem = currentWordTypeItem;
						}
					}
				}
			while( wordResult.foundWordItem == NULL &&
			( currentWordTypeItem = currentWordTypeItem->nextWordTypeItem( wordTypeNr ) ) != NULL );
			}

		return wordResult;
		}

	char *wordTypeString( bool isCheckingAllLanguages, unsigned short wordTypeNr )
		{
		char *wordTypeString = ( myWordItem_->wordTypeList == NULL ? NULL : myWordItem_->wordTypeList->wordTypeString( isCheckingAllLanguages, wordTypeNr ) );

		if( wordTypeString == NULL )
			{
			sprintf( wordItemNameString_, "%c%u%c%u%c", QUERY_ITEM_START_CHAR, myWordItem_->creationSentenceNr(), QUERY_SEPARATOR_CHAR, myWordItem_->itemNr(), QUERY_ITEM_END_CHAR );
			return wordItemNameString_;
			}

		return wordTypeString;
		}
	};

/*************************************************************************
 *	"Let them praise your great and awesome name.
 *	Your name is holy!" (Psalm 99:3)
 *************************************************************************/
