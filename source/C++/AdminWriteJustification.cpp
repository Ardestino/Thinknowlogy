/*
 *	Class:			AdminWriteJustification
 *	Supports class:	AdminItem
 *	Purpose:		To write justification reports for the
 *					self-generated knowledge
 *	Version:		Thinknowlogy 2015r1beta (Coraz�n)
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

#include "AdminItem.h"
#include "Presentation.cpp"
#include "SpecificationItem.cpp"

class AdminWriteJustification
	{
	friend class AdminItem;

	// Private constructible variables

	SpecificationItem *previousPrimarySpecificationItem_;

	AdminItem *adminItem_;
	CommonVariables *commonVariables_;
	char moduleNameString_[FUNCTION_NAME_LENGTH];


	// Private functions

	ResultType writeSpecificationJustifications( bool isWritingPrimarySpecification, JustificationItem *writeJustificationItem )
		{
		bool isWritingCurrentSpecificationWordOnly;
		SpecificationItem *anotherPrimarySpecificationItem;
		SpecificationItem *secondarySpecificationItem;
		SpecificationItem *anotherSecondarySpecificationItem;
		SpecificationItem *primarySpecificationItem = NULL;
		WordItem *generalizationWordItem;
		WordItem *feminineOrMasculineProperNameEndingWordItem = NULL;
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeSpecificationJustifications";

		if( writeJustificationItem != NULL )
			{
			if( isWritingPrimarySpecification )
				{
				if( ( primarySpecificationItem = writeJustificationItem->primarySpecificationItem() ) != NULL )
					{
					if( ( generalizationWordItem = primarySpecificationItem->generalizationWordItem() ) != NULL )
						{
						if( !primarySpecificationItem->isHiddenSpecification() )
							{
							if( generalizationWordItem->writeJustificationSpecification( ( !primarySpecificationItem->isExclusiveSpecification() && !primarySpecificationItem->isPossessive() ), primarySpecificationItem ) != RESULT_OK )
								return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write the definition justification specification" );
							}
						else
							return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The primary specification item is a hidden specification" );
						}
					else
						return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The generalization word of the given primary specification item is undefined" );
					}

				if( ( anotherPrimarySpecificationItem = writeJustificationItem->anotherPrimarySpecificationItem() ) != NULL )
					{
					if( ( generalizationWordItem = anotherPrimarySpecificationItem->generalizationWordItem() ) != NULL )
						{
						if( !anotherPrimarySpecificationItem->isHiddenSpecification() )
							{
							if( generalizationWordItem->writeJustificationSpecification( false, anotherPrimarySpecificationItem ) != RESULT_OK )
								return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write the another definition justification specification" );
							}
						else
							return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The another primary specification item is a hidden specification" );
						}
					else
						return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The generalization word of the given another primary specification item is undefined" );
					}
				}

			if( ( secondarySpecificationItem = writeJustificationItem->secondarySpecificationItem() ) != NULL )
				{
				if( ( generalizationWordItem = secondarySpecificationItem->generalizationWordItem() ) != NULL )
					{
					if( !secondarySpecificationItem->isHiddenSpecification() )
						{
						isWritingCurrentSpecificationWordOnly = ( secondarySpecificationItem->isPartOf() ||

																( !secondarySpecificationItem->isExclusiveSpecification() &&
																secondarySpecificationItem->isGeneralizationProperName() &&
																!secondarySpecificationItem->isPossessive() ) );

						if( generalizationWordItem->writeJustificationSpecification( isWritingCurrentSpecificationWordOnly, secondarySpecificationItem ) != RESULT_OK )
							return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write the secondary justification specification" );
						}
					else
						return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The secondary specification item is a hidden specification" );
					}
				else
					return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The generalization word of the given secondary specification item is undefined" );
				}

			if( ( anotherSecondarySpecificationItem = writeJustificationItem->anotherSecondarySpecificationItem() ) != NULL )
				{
				if( ( generalizationWordItem = anotherSecondarySpecificationItem->generalizationWordItem() ) != NULL )
					{
					if( !anotherSecondarySpecificationItem->isHiddenSpecification() )
						{
						if( generalizationWordItem->writeJustificationSpecification( false, anotherSecondarySpecificationItem ) != RESULT_OK )
							return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write the another secondary justification specification" );
						}
					else
						return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The another secondary specification item is a hidden specification" );
					}
				else
					return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The generalization word of the given another secondary specification item is undefined" );
				}

			if( isWritingPrimarySpecification &&
			writeJustificationItem->hasFeminineOrMasculineProperNameEnding() )
				{
				if( ( feminineOrMasculineProperNameEndingWordItem = ( primarySpecificationItem != NULL && secondarySpecificationItem != NULL && secondarySpecificationItem->isPossessive() ? secondarySpecificationItem->myWordItem() : writeJustificationItem->myWordItem() ) ) != NULL )
					{
					if( commonVariables_->presentation->writeInterfaceText( PRESENTATION_PROMPT_WRITE, INTERFACE_JUSTIFICATION_SENTENCE_START, EMPTY_STRING, INTERFACE_JUSTIFICATION_FEMININE_OR_MASCULINE_PROPER_NAME_ENDING, feminineOrMasculineProperNameEndingWordItem->anyWordTypeString(), ( feminineOrMasculineProperNameEndingWordItem->hasFeminineProperNameEnding() ? INTERFACE_JUSTIFICATION_FEMININE_PROPER_NAME_ENDING : INTERFACE_JUSTIFICATION_MASCULINE_PROPER_NAME_ENDING ) ) != RESULT_OK )
						return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write the justification sentence start string" );
					}
				else
					return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "I couldn't find the feminine or masculine proper name ending word" );
				}
			}
		else
			return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given write justification item is undefined" );

		return RESULT_OK;
		}

	ResultType writeJustificationSpecifications( bool isWritingPrimarySpecification, bool isWritingExtraSeparator, JustificationItem *startJustificationItem )
		{
		bool isQuestionJustification;
		bool isExclusiveSecondarySpecification;
		bool isFirstTime = true;
		bool isStop = false;
		JustificationItem *currentJustificationItem;
		SpecificationItem *secondarySpecificationItem;
		SpecificationItem *primarySpecificationItem;
		WordItem *generalizationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeJustificationSpecifications";

		if( startJustificationItem != NULL )
			{
			currentJustificationItem = startJustificationItem;

			do	{
				isExclusiveSecondarySpecification = false;
				isQuestionJustification = currentJustificationItem->isQuestionJustification();

				primarySpecificationItem = currentJustificationItem->primarySpecificationItem();

				if( ( secondarySpecificationItem = currentJustificationItem->secondarySpecificationItem() ) != NULL )
					{
					if( secondarySpecificationItem->isExclusiveSpecification() )
						isExclusiveSecondarySpecification = true;
					}

				if( isWritingPrimarySpecification ||

				( ( !isQuestionJustification &&

				( primarySpecificationItem == NULL ||

				( isFirstTime &&
				primarySpecificationItem != previousPrimarySpecificationItem_ ) ) ) ||

				( !isExclusiveSecondarySpecification &&
				primarySpecificationItem != NULL &&
				secondarySpecificationItem != NULL &&
				previousPrimarySpecificationItem_ != NULL &&
				!primarySpecificationItem->isCorrectedAssumption() &&
				primarySpecificationItem->specificationCollectionNr() == previousPrimarySpecificationItem_->specificationCollectionNr() &&

				// Skip possessive secondary specifications without relation
				( !secondarySpecificationItem->isPossessive() ||
				secondarySpecificationItem->hasRelationContext() ) ) ) )
					{
					if( isWritingExtraSeparator )
						{
						if( commonVariables_->presentation->writeInterfaceText( false, true, PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_JUSTIFICATION_AND ) != RESULT_OK )
							return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write a justification string" );
						}

					if( writeSpecificationJustifications( isWritingPrimarySpecification, currentJustificationItem ) == RESULT_OK )
						{
						isFirstTime = false;
						isWritingPrimarySpecification = false;
						currentJustificationItem->hasBeenWritten = true;

						previousPrimarySpecificationItem_ = primarySpecificationItem;

						if( secondarySpecificationItem != NULL &&
						primarySpecificationItem != NULL &&
						primarySpecificationItem->isExclusiveSpecification() &&
						primarySpecificationItem->isQuestion() &&
						( generalizationWordItem = primarySpecificationItem->generalizationWordItem() ) != NULL )
							{
							if( generalizationWordItem->writeRelatedJustificationSpecifications( currentJustificationItem->justificationTypeNr(), secondarySpecificationItem->specificationCollectionNr() ) != RESULT_OK )
								return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write the related justification specifications" );
							}
						}
					else
						return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write the current justification specification" );
					}
				else
					isStop = true;
				}
			while( !isStop &&
			( currentJustificationItem = currentJustificationItem->nextJustificationItemWithSameTypeAndOrderNr() ) != NULL );
			}
		else
			return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given start justification item is undefined" );

		return RESULT_OK;
		}

	ResultType writeJustificationType( bool isFirstJustificationType, char *justificationSentenceString, JustificationItem *writeJustificationItem, SpecificationItem *selfGeneratedSpecificationItem )
		{
		bool isExclusiveSpecification;
		bool isNegative;
		bool isPossessive;
		bool isSelfGenerated;
		bool isWritingPrimarySpecificationOrExtraSeparator;
		bool isFirstTime = true;
		bool hasNonExclusiveCollection = false;
		unsigned short assumptionLevel;
		unsigned int specificationCollectionNr;
		unsigned int generalizationContextNr;
		unsigned int specificationContextNr;
		unsigned int relationContextNr;
		JustificationItem *currentJustificationItem;
		SpecificationItem *currentSpecificationItem;
		SpecificationItem *currentPrimarySpecificationItem;
		SpecificationItem *currentSecondarySpecificationItem;
		SpecificationItem *firstSecondarySpecificationItem;
		WordItem *generalizationWordItem;
		WordItem *secondarySpecificationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeJustificationType";

		previousPrimarySpecificationItem_ = NULL;

		if( justificationSentenceString != NULL )
			{
			if( writeJustificationItem != NULL )
				{
				if( selfGeneratedSpecificationItem != NULL )
					{
					if( ( generalizationWordItem = selfGeneratedSpecificationItem->generalizationWordItem() ) != NULL )
						{
						if( isFirstJustificationType )
							{
							if( commonVariables_->presentation->writeDiacriticalText( PRESENTATION_PROMPT_WRITE, justificationSentenceString ) != RESULT_OK )
								return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write the justification sentence" );
							}

						specificationCollectionNr = selfGeneratedSpecificationItem->specificationCollectionNr();

						if( ( firstSecondarySpecificationItem = writeJustificationItem->secondarySpecificationItem() ) != NULL )
							{
							if( ( secondarySpecificationWordItem = firstSecondarySpecificationItem->specificationWordItem() ) != NULL )
								hasNonExclusiveCollection = secondarySpecificationWordItem->hasNonExclusiveCollection( specificationCollectionNr );
							}

						if( commonVariables_->presentation->writeInterfaceText( false, !isFirstJustificationType, PRESENTATION_PROMPT_NOTIFICATION, ( isFirstJustificationType ? INTERFACE_JUSTIFICATION_BECAUSE : INTERFACE_JUSTIFICATION_AND ) ) == RESULT_OK )
							{
							if( writeJustificationSpecifications( true, false, writeJustificationItem ) == RESULT_OK )
								{
								if( ( hasNonExclusiveCollection ||
								!writeJustificationItem->isNegativeAssumptionOrConclusion() ) &&

								( currentSpecificationItem = generalizationWordItem->firstSelectedSpecificationItem( false, selfGeneratedSpecificationItem->isAssignment(), selfGeneratedSpecificationItem->isInactiveAssignment(), selfGeneratedSpecificationItem->isArchivedAssignment(), selfGeneratedSpecificationItem->questionParameter() ) ) != NULL )
									{
									isExclusiveSpecification = selfGeneratedSpecificationItem->isExclusiveSpecification();
									isNegative = selfGeneratedSpecificationItem->isNegative();
									isPossessive = selfGeneratedSpecificationItem->isPossessive();
									isSelfGenerated = selfGeneratedSpecificationItem->isSelfGenerated();

									assumptionLevel = selfGeneratedSpecificationItem->assumptionLevel();

									generalizationContextNr = selfGeneratedSpecificationItem->generalizationContextNr();
									specificationContextNr = selfGeneratedSpecificationItem->specificationContextNr();
									relationContextNr = selfGeneratedSpecificationItem->relationContextNr();

									do	{
										// Self-generated
										if( ( currentJustificationItem = currentSpecificationItem->firstJustificationItem() ) != NULL &&
										currentSpecificationItem->isRelatedSpecification( isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, assumptionLevel, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr ) )
											{
											if( !currentJustificationItem->hasBeenWritten )
												{
												currentPrimarySpecificationItem = currentJustificationItem->primarySpecificationItem();
												currentSecondarySpecificationItem = currentJustificationItem->secondarySpecificationItem();

												isWritingPrimarySpecificationOrExtraSeparator = ( currentPrimarySpecificationItem == NULL ||
																								currentPrimarySpecificationItem->isQuestion() ||

																								( isFirstTime &&
																								firstSecondarySpecificationItem != NULL &&
																								currentSecondarySpecificationItem != NULL &&
																								firstSecondarySpecificationItem->specificationCollectionNr() != currentSecondarySpecificationItem->specificationCollectionNr() &&
																								firstSecondarySpecificationItem->nInvolvedSpecificationWords( false, false ) != currentSecondarySpecificationItem->nInvolvedSpecificationWords( false, false ) ) ||

																								( currentSecondarySpecificationItem != NULL &&
																								currentSecondarySpecificationItem->isGeneralizationProperName() ) );

												if( writeJustificationSpecifications( isWritingPrimarySpecificationOrExtraSeparator, isWritingPrimarySpecificationOrExtraSeparator, currentJustificationItem ) == RESULT_OK )
													isFirstTime = false;
												else
													return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write the current justification specifications" );
												}
											}
										}
									while( ( currentSpecificationItem = currentSpecificationItem->nextSelectedSpecificationItem() ) != NULL );
									}
								}
							else
								return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write the justification specifications" );
							}
						else
							return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write the justification start string" );
						}
					else
						return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The generalization word of the given self-generated specification item is undefined" );
					}
				else
					return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given self-generated specification item is undefined" );
				}
			else
				return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given write justification is undefined" );
			}
		else
			return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given justification sentence string is undefined" );

		return RESULT_OK;
		}


	protected:
	// Constructor / deconstructor

	AdminWriteJustification( AdminItem *adminItem, CommonVariables *commonVariables )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		previousPrimarySpecificationItem_ = NULL;

		adminItem_ = adminItem;
		commonVariables_ = commonVariables;
		strcpy( moduleNameString_, "AdminWriteJustification" );

		if( commonVariables_ == NULL )
			strcpy( errorString, "The given common variables is undefined" );

		if( adminItem_ == NULL )
			strcpy( errorString, "The given admin is undefined" );

		if( strlen( errorString ) > 0 )
			{
			if( adminItem_ != NULL )
				adminItem_->startSystemErrorInItem( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, moduleNameString_, errorString );
			else
				{
			if( commonVariables_ != NULL )
				commonVariables_->result = RESULT_SYSTEM_ERROR;
				fprintf( stderr, "\nClass:%s\nFunction:\t%s\nError:\t\t%s.\n", moduleNameString_, PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, errorString );
				}
			}
		}


	// Protected functions

	ResultType writeJustificationSpecification( char *justificationSentenceString, SpecificationItem *selfGeneratedSpecificationItem )
		{
		bool isFirstJustificationType = true;
		JustificationItem *firstJustificationItem;
		JustificationItem *currentJustificationItem;
		WordItem *generalizationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeJustificationSpecification";

		if( selfGeneratedSpecificationItem != NULL )
			{
			if( ( firstJustificationItem = selfGeneratedSpecificationItem->firstJustificationItem() ) != NULL )
				{
				if( ( generalizationWordItem = selfGeneratedSpecificationItem->generalizationWordItem() ) != NULL )
					{
					generalizationWordItem->clearJustificationHasBeenWritten();
					currentJustificationItem = firstJustificationItem;

					do	{
						if( writeJustificationType( isFirstJustificationType, justificationSentenceString, currentJustificationItem, selfGeneratedSpecificationItem ) == RESULT_OK )
							isFirstJustificationType = false;
						else
							return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write a justification type of a specification" );
						}
					while( ( currentJustificationItem = currentJustificationItem->nextJustificationItemWithDifferentTypeOrOrderNr( firstJustificationItem ) ) != NULL );
					}
				else
					return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The generalization word of the given self-generated specification item is undefined" );
				}
			else
				return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The first justification of the given self-generated specification item is undefined" );
			}
		else
			return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given self-generated specification item is undefined" );

		return RESULT_OK;
		}
	};

/*************************************************************************
 *	"Oh, the joys of those who trust the Lord,
 *	who have no confidence of the proud
 *	or in those who worship idols." (Psalm 40:4)
 *************************************************************************/