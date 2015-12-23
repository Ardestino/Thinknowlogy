/*
 *	Class:			GeneralizationItem
 *	Parent class:	Item
 *	Purpose:		To store info about generalizations of a word,
 *					which are the "parents" of that word,
 *					and is the opposite direction of its specifications
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

#ifndef GENERALIZATIONITEM
#define GENERALIZATIONITEM 1
#include "WordItem.h"

class GeneralizationItem : private Item
	{
	friend class AdminAssumption;
	friend class AdminAuthorization;
	friend class AdminConclusion;
	friend class AdminContext;
	friend class AdminWriteSpecification;
	friend class GeneralizationList;
	friend class WordAssignment;
	friend class WordCollection;
	friend class WordSpecification;
	friend class WordItem;

	// Private loadable variables

	bool isLanguageWord_;
	bool isRelation_;

	unsigned short languageNr_;
	unsigned short specificationWordTypeNr_;
	unsigned short generalizationWordTypeNr_;

	WordItem *generalizationWordItem_;


	protected:
	// Constructor / deconstructor

	GeneralizationItem( bool isLanguageWord, bool isRelation, unsigned short languageNr, unsigned short specificationWordTypeNr, unsigned short generalizationWordTypeNr, WordItem *generalizationWordItem, CommonVariables *commonVariables, List *myList, WordItem *myWordItem )
		{
		initializeItemVariables( NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, "GeneralizationItem", commonVariables, myList, myWordItem );

		// Private loadable variables

		isLanguageWord_ = isLanguageWord;
		isRelation_ = isRelation;

		languageNr_ = languageNr;

		specificationWordTypeNr_ = specificationWordTypeNr;
		generalizationWordTypeNr_ = generalizationWordTypeNr;

		generalizationWordItem_ = generalizationWordItem;

		if( generalizationWordItem_ == NULL )
			startSystemError( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, NULL, "The given generalization word item is undefined" );
		}


	// Protected virtual functions

	virtual void showWordReferences( bool isReturnQueryToPosition )
		{
		char *wordString;
		char statusString[2] = SPACE_STRING;
		statusString[0] = statusChar();

		if( generalizationWordItem_ != NULL &&
		( wordString = generalizationWordItem_->wordTypeString( true, generalizationWordTypeNr_ ) ) != NULL )
			{
			if( commonVariables()->hasFoundQuery )
				strcat( commonVariables()->queryString, ( isReturnQueryToPosition ? NEW_LINE_STRING : QUERY_SEPARATOR_SPACE_STRING ) );

			// Show status if not active
			if( !isActiveItem() )
				strcat( commonVariables()->queryString, statusString );

			commonVariables()->hasFoundQuery = true;
			strcat( commonVariables()->queryString, wordString );
			}
		}

	virtual bool hasFoundReferenceItemById( unsigned int querySentenceNr, unsigned int queryItemNr )
		{
		return ( generalizationWordItem_ == NULL ? false :
					( querySentenceNr == NO_SENTENCE_NR ? true : generalizationWordItem_->creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == NO_ITEM_NR ? true : generalizationWordItem_->itemNr() == queryItemNr ) );
		}

	virtual bool hasFoundWordType( unsigned short queryWordTypeNr )
		{
		return ( specificationWordTypeNr_ == queryWordTypeNr ||
				generalizationWordTypeNr_ == queryWordTypeNr );
		}

	virtual ReferenceResultType findMatchingWordReferenceString( char *queryString )
		{
		ReferenceResultType referenceResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "findMatchingWordReferenceString";

		if( generalizationWordItem_ != NULL )
			{
			if( ( referenceResult = generalizationWordItem_->findMatchingWordReferenceString( queryString ) ).result != RESULT_OK )
				addError( functionNameString, NULL, "I failed to find a matching word reference string for the generalization word" );
			}

		return referenceResult;
		}

	virtual char *toString( unsigned short queryWordTypeNr )
		{
		char *wordString;
		char *languageNameString = myWordItem()->languageNameString( languageNr_ );
		char *generalizationWordTypeString = myWordItem()->wordTypeNameString( generalizationWordTypeNr_ );
		char *specificationWordTypeString = myWordItem()->wordTypeNameString( specificationWordTypeNr_ );

		Item::toString( queryWordTypeNr );

		if( isLanguageWord_ )
			{
			strcat( commonVariables()->queryString, QUERY_SEPARATOR_STRING );
			strcat( commonVariables()->queryString, "isLanguageWord" );
			}

		if( isRelation_ )
			{
			strcat( commonVariables()->queryString, QUERY_SEPARATOR_STRING );
			strcat( commonVariables()->queryString, "isRelation" );
			}

		if( languageNr_ > NO_LANGUAGE_NR )
			{
			if( languageNameString == NULL )
				sprintf( tempString, "%clanguageNr:%u", QUERY_SEPARATOR_CHAR, languageNr_ );
			else
				sprintf( tempString, "%clanguage:%s", QUERY_SEPARATOR_CHAR, languageNameString );

			strcat( commonVariables()->queryString, tempString );
			}

		if( specificationWordTypeString == NULL )
			sprintf( tempString, "%cspecificationWordType:%c%u", QUERY_SEPARATOR_CHAR, QUERY_WORD_TYPE_CHAR, specificationWordTypeNr_ );
		else
			sprintf( tempString, "%cspecificationWordType:%s%c%u", QUERY_SEPARATOR_CHAR, specificationWordTypeString, QUERY_WORD_TYPE_CHAR, specificationWordTypeNr_ );

		strcat( commonVariables()->queryString, tempString );

		if( generalizationWordTypeString == NULL )
			sprintf( tempString, "%cgeneralizationWordType:%c%u", QUERY_SEPARATOR_CHAR, QUERY_WORD_TYPE_CHAR, generalizationWordTypeNr_ );
		else
			sprintf( tempString, "%cgeneralizationWordType:%s%c%u", QUERY_SEPARATOR_CHAR, generalizationWordTypeString, QUERY_WORD_TYPE_CHAR, generalizationWordTypeNr_ );

		strcat( commonVariables()->queryString, tempString );

		if( generalizationWordItem_ != NULL )
			{
			sprintf( tempString, "%cgeneralizationWord%c%u%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_REF_ITEM_START_CHAR, generalizationWordItem_->creationSentenceNr(), QUERY_SEPARATOR_CHAR, generalizationWordItem_->itemNr(), QUERY_REF_ITEM_END_CHAR );
			strcat( commonVariables()->queryString, tempString );

			if( ( wordString = generalizationWordItem_->wordTypeString( true, generalizationWordTypeNr_ ) ) != NULL )
				{
				sprintf( tempString, "%c%s%c", QUERY_WORD_REFERENCE_START_CHAR, wordString, QUERY_WORD_REFERENCE_END_CHAR );
				strcat( commonVariables()->queryString, tempString );
				}
			}

		return commonVariables()->queryString;
		}


	// Protected functions

	bool isRelation()
		{
		return isRelation_;
		}

	unsigned short generalizationWordTypeNr()
		{
		return generalizationWordTypeNr_;
		}

	unsigned short languageNr()
		{
		return languageNr_;
		}

	WordItem *generalizationWordItem()
		{
		return generalizationWordItem_;
		}

	GeneralizationItem *getGeneralizationItem( bool isIncludingThisItem, bool isOnlySelectingCurrentLanguage, bool isOnlySelectingNoun, bool isRelation )
		{
		unsigned short currentLanguageNr = commonVariables()->currentLanguageNr;
		GeneralizationItem *searchItem = ( isIncludingThisItem ? this : nextGeneralizationItem() );

		while( searchItem != NULL )
			{
			if( searchItem->isRelation_ == isRelation &&

			( !isOnlySelectingCurrentLanguage ||
			searchItem->languageNr_ == currentLanguageNr ) &&

			( !isOnlySelectingNoun ||
			isSingularOrPluralNoun( searchItem->generalizationWordTypeNr_ ) ) )
				return searchItem;

			searchItem = searchItem->nextGeneralizationItem();
			}

		return NULL;
		}

	GeneralizationItem *nextGeneralizationItem()
		{
		return (GeneralizationItem *)nextItem;
		}

	GeneralizationItem *nextNounSpecificationGeneralizationItem()
		{
		return getGeneralizationItem( false, false, true, false );
		}

	GeneralizationItem *nextSpecificationGeneralizationItem()
		{
		return getGeneralizationItem( false, false, false, false );
		}

	GeneralizationItem *nextRelationGeneralizationItem()
		{
		return getGeneralizationItem( false, false, false, true );
		}
	};
#endif

/*************************************************************************
 *	"Give thanks to him who made the heavens so skillfully.
 *	His faithful love endures forever." (Psalm 136:5)
 *************************************************************************/
