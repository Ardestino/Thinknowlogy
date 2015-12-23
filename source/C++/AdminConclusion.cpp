/*
 *	Class:			AdminConclusion
 *	Supports class:	AdminItem
 *	Purpose:		To autonomously draw grammar-based conclusions
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

#include "AdminItem.h"
#include "GeneralizationItem.cpp"
#include "SpecificationItem.cpp"

class AdminConclusion
	{
	friend class AdminItem;

	// Private constructible variables

	bool hasFoundSpecificationWordCollectedWithItself_;

	unsigned int missingNegativeConclusionSpecificationCollectionNr_;

	SpecificationItem *missingNegativeConclusionPrimarySpecificationItem_;
	SpecificationItem *missingNegativeConclusionAnotherPrimarySpecificationItem_;

	AdminItem *adminItem_;
	CommonVariables *commonVariables_;
	char moduleNameString_[FUNCTION_NAME_LENGTH];


	// Private functions

	void clearMissingNegativeConclusionVariables()
		{
		missingNegativeConclusionSpecificationCollectionNr_ = NO_COLLECTION_NR;
		missingNegativeConclusionPrimarySpecificationItem_ = NULL;
		missingNegativeConclusionAnotherPrimarySpecificationItem_ = NULL;
		}

	ResultType askSpecificationSubstitutionQuestionOrDrawNegativeConclusion( bool isArchivedAssignment, bool isExclusiveSpecification, bool isNegativeConclusion, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		SpecificationResultType specificationResult;
		bool hasGeneralizationWordFeminineProperNameEnding;
		bool hasGeneralizationWordMasculineProperNameEnding;
		bool isCommonWordCollectedWithItself;
		bool isSpecificationCollectedWithItself;
		unsigned int anotherPrimarySpecificationCollectionNr;
		unsigned int nonCompoundSpecificationCollectionNr;
		unsigned int specificationCollectionNr;
		SpecificationItem *firstNonCompoundCollectionSpecificationItem;
		SpecificationItem *questionSpecificationItem;
		SpecificationItem *relatedSpecificationItem;
		SpecificationItem *existingSpecificationItem = NULL;
		WordItem *anotherPrimarySpecificationWordItem;
		WordItem *commonWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "askSpecificationSubstitutionQuestionOrDrawNegativeConclusion";

		if( primarySpecificationItem != NULL )
			{
			if( anotherPrimarySpecificationItem != NULL )
				{
				if( generalizationWordItem != NULL )
					{
					if( specificationWordItem != NULL )
						{
						if( ( anotherPrimarySpecificationCollectionNr = anotherPrimarySpecificationItem->specificationCollectionNr() ) > NO_COLLECTION_NR )
							{
							if( ( anotherPrimarySpecificationWordItem = anotherPrimarySpecificationItem->specificationWordItem() ) != NULL )
								{
								if( ( commonWordItem = anotherPrimarySpecificationWordItem->commonWordItem( anotherPrimarySpecificationCollectionNr ) ) != NULL )
									{
									if( ( specificationCollectionNr = commonWordItem->collectionNr( specificationWordTypeNr, specificationWordItem ) ) == NO_COLLECTION_NR )
										specificationCollectionNr = anotherPrimarySpecificationCollectionNr;

									if( ( specificationResult = generalizationWordItem->findRelatedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, NO_QUESTION_PARAMETER, specificationCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, anotherPrimarySpecificationWordItem ) ).result == RESULT_OK )
										{
										isCommonWordCollectedWithItself = commonWordItem->isNounWordCollectedWithItself();
										isSpecificationCollectedWithItself = specificationWordItem->isNounWordCollectedWithItself();
										relatedSpecificationItem = specificationResult.relatedSpecificationItem;

										// Current question part isn't related to an answer or a similar question
										if( isCommonWordCollectedWithItself &&
										relatedSpecificationItem == NULL &&
										!adminItem_->isPossessiveUserSpecification() &&
										( nonCompoundSpecificationCollectionNr = commonWordItem->nonCompoundCollectionNr( specificationWordTypeNr ) ) > NO_COLLECTION_NR )
											{
											if( nonCompoundSpecificationCollectionNr != specificationCollectionNr )
												{
												if( ( specificationResult = generalizationWordItem->findRelatedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, NO_QUESTION_PARAMETER, nonCompoundSpecificationCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, anotherPrimarySpecificationWordItem ) ).result == RESULT_OK )
													relatedSpecificationItem = specificationResult.relatedSpecificationItem;
												else
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find a related specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\" that is collected with itself" );
												}
											}

										if( relatedSpecificationItem == NULL )
											{
											hasGeneralizationWordFeminineProperNameEnding = generalizationWordItem->hasFeminineProperNameEnding();
											hasGeneralizationWordMasculineProperNameEnding = generalizationWordItem->hasMasculineProperNameEnding();

											// Negative feminine conclusion
											if( ( hasGeneralizationWordMasculineProperNameEnding &&
											anotherPrimarySpecificationWordItem->isFemale() ) ||

											// Negative masculine conclusion
											( hasGeneralizationWordFeminineProperNameEnding &&
											anotherPrimarySpecificationWordItem->isMale() ) ||

											// Specification is collected with itself
											( isSpecificationCollectedWithItself &&
											!generalizationWordItem->hasCorrectedAssumptionByOppositeQuestion() &&

											( adminItem_->isUserQuestion() ||
											hasGeneralizationWordFeminineProperNameEnding ) ) )
												{
												if( !isSpecificationCollectedWithItself ||
												missingNegativeConclusionSpecificationCollectionNr_ == NO_COLLECTION_NR )
													{
													missingNegativeConclusionSpecificationCollectionNr_ = anotherPrimarySpecificationCollectionNr;
													missingNegativeConclusionPrimarySpecificationItem_ = primarySpecificationItem;
													missingNegativeConclusionAnotherPrimarySpecificationItem_ = anotherPrimarySpecificationItem;
													}
												}
											else
												{
												if( isSpecificationCollectedWithItself )
													existingSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, false, NO_QUESTION_PARAMETER, specificationWordItem );

												// Skip invalid questions
												if( ( !isNegativeConclusion ||

												( isSpecificationCollectedWithItself &&

												( existingSpecificationItem == NULL ||
												existingSpecificationItem->isHiddenSpecification() ) ) ) &&

												( ( !hasGeneralizationWordFeminineProperNameEnding &&
												!hasGeneralizationWordMasculineProperNameEnding ) ||

												primarySpecificationItem->hasSpecificationCompoundCollection() ) &&

												( !isSpecificationCollectedWithItself ||
												anotherPrimarySpecificationItem->hasSpecificationNonCompoundCollection() ||
												anotherPrimarySpecificationWordItem->lastCompoundCollectionNr( specificationWordTypeNr ) == anotherPrimarySpecificationCollectionNr ) )
													{
													if( isSpecificationCollectedWithItself &&
													!primarySpecificationItem->isExclusiveSpecification() &&
													primarySpecificationItem->isUserSpecification() )
														{
														missingNegativeConclusionSpecificationCollectionNr_ = anotherPrimarySpecificationCollectionNr;
														missingNegativeConclusionPrimarySpecificationItem_ = primarySpecificationItem;
														missingNegativeConclusionAnotherPrimarySpecificationItem_ = anotherPrimarySpecificationItem;
														}
													else
														{
														if( primarySpecificationItem->isUserSpecification() ||
														generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( true, isArchivedAssignment, isArchivedAssignment, false, false, false, NO_QUESTION_PARAMETER, primarySpecificationItem->specificationCollectionNr(), NO_CONTEXT_NR, specificationWordItem ) == NULL )
															{
															if( adminItem_->askSpecificationSubstitutionQuestion( isArchivedAssignment, isExclusiveSpecification, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, primarySpecificationItem, anotherPrimarySpecificationItem, generalizationWordItem, specificationWordItem ) != RESULT_OK )
																return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a specification substitution question about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
															}
														}
													}
												}
											}
										else
											{
											if( anotherPrimarySpecificationItem->isExclusiveSpecification() )
												{
												if( !isCommonWordCollectedWithItself &&
												( firstNonCompoundCollectionSpecificationItem = generalizationWordItem->firstNonCompoundCollectionSpecificationItem( anotherPrimarySpecificationCollectionNr ) ) != NULL )
													relatedSpecificationItem = firstNonCompoundCollectionSpecificationItem;

												if( missingNegativeConclusionSpecificationCollectionNr_ == anotherPrimarySpecificationCollectionNr )
													{
													if( drawNegativeConclusion( isArchivedAssignment, false, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, missingNegativeConclusionPrimarySpecificationItem_, missingNegativeConclusionAnotherPrimarySpecificationItem_, relatedSpecificationItem, generalizationWordItem ) == RESULT_OK )
														clearMissingNegativeConclusionVariables();
													else
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a negative conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
													}

												if( isCommonWordCollectedWithItself &&
												isSpecificationCollectedWithItself &&
												// specificationWordItem == commonWordItem &&
												!primarySpecificationItem->hasSpecificationNonCompoundCollection() &&
												primarySpecificationItem->specificationCollectionNr() != anotherPrimarySpecificationCollectionNr )
													{
													questionSpecificationItem = generalizationWordItem->firstActiveAssignmentOrSpecificationItem( isArchivedAssignment, false, false, WORD_PARAMETER_SINGULAR_VERB_IS, anotherPrimarySpecificationWordItem );

													if( ( questionSpecificationItem == NULL ||
													questionSpecificationItem->isOlderItem() ) &&

													primarySpecificationItem->isUserSpecification() )
														{
														if( adminItem_->askSpecificationSubstitutionQuestion( isArchivedAssignment, isExclusiveSpecification, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, primarySpecificationItem, anotherPrimarySpecificationItem, generalizationWordItem, specificationWordItem ) != RESULT_OK )
															return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a specification substitution question about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", specificationWordItem->anyWordTypeString(), "\" that is collected with itself" );
														}
													}
												else
													{
													if( !isSpecificationCollectedWithItself ||
													generalizationWordItem->isUserGeneralizationWord ||
													anotherPrimarySpecificationItem->hasSpecificationNonCompoundCollection() ||
													anotherPrimarySpecificationWordItem->lastCompoundCollectionNr( specificationWordTypeNr ) == anotherPrimarySpecificationCollectionNr )
														{
														if( drawNegativeConclusion( isArchivedAssignment, isSpecificationCollectedWithItself, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, primarySpecificationItem, anotherPrimarySpecificationItem, relatedSpecificationItem, generalizationWordItem ) != RESULT_OK )
															return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a negative conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
														}
													}
												}
											}
										}
									else
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find a related specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
									}
								else
									return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find the common word of the given specification collection number" );
								}
							else
								return adminItem_->startError( functionNameString, moduleNameString_, "The given another primary specification item has no specification word item" );
							}
						else
							return adminItem_->startError( functionNameString, moduleNameString_, "The given another primary specification item has no specification collection" );
						}
					else
						return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );
					}
				else
					return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );
				}
			else
				return adminItem_->startError( functionNameString, moduleNameString_, "The given another primary specification item is undefined" );
			}
		else
			return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification item is undefined" );

		return RESULT_OK;
		}

	ResultType drawNegativeConclusion( bool isArchivedAssignment, bool isSpecificationCollectedWithItself, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, SpecificationItem *secondarySpecificationItem, WordItem *generalizationWordItem )
		{
		SpecificationResultType specificationResult;
		bool isPrimarySameAsUserSpecification;
		bool isPrimarySpecificationHidden;
		bool isUserGeneralizationWord;
		bool hasAnotherSecondarySpecificationWithCompoundCollection = false;
		bool isSelectingUserSpecification = false;
		bool isUserSpecificationCollectedWithItself = false;
		unsigned int anotherPrimarySpecificationCollectionNr;
		JustificationItem *createdJustificationItem;
		JustificationItem *negativeAssumptionOrConclusionJustificationItem;
		JustificationItem *questionJustificationItem;
		SpecificationItem *conclusionPrimarySpecificationItem;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *existingNegativeSpecificationItem;
		SpecificationItem *existingPositiveSpecificationItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *foundNegativeAssumptionOrConclusionPrimarySpecificationItem;
		SpecificationItem *relatedSpecificationItem;
		SpecificationItem *tempSpecificationItem;
		SpecificationItem *anotherSecondarySpecificationItem = NULL;
		SpecificationItem *oppositePossessiveDefinitionSpecificationItem = NULL;
		SpecificationItem *userSpecificationItem = NULL;
		WordItem *primarySpecificationWordItem;
		WordItem *anotherPrimarySpecificationWordItem;
		WordItem *secondarySpecificationWordItem;
		WordItem *userSpecificationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawNegativeConclusion";

		if( primarySpecificationItem != NULL )
			{
			if( anotherPrimarySpecificationItem != NULL )
				{
				if( secondarySpecificationItem != NULL )
					{
					if( generalizationWordItem != NULL )
						{
						if( !primarySpecificationItem->isPossessive() &&
						!primarySpecificationItem->isQuestion() )
							{
							if( ( primarySpecificationWordItem = primarySpecificationItem->specificationWordItem() ) != NULL )
								{
								if( ( anotherPrimarySpecificationCollectionNr = anotherPrimarySpecificationItem->specificationCollectionNr() ) > NO_COLLECTION_NR )
									{
									if( ( anotherPrimarySpecificationWordItem = anotherPrimarySpecificationItem->specificationWordItem() ) != NULL )
										{
										// If exists, replace self-generated assumption for conclusion
										if( primarySpecificationItem->isSelfGeneratedAssumption() &&
										( conclusionPrimarySpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( true, primarySpecificationItem->isArchivedAssignment(), primarySpecificationItem->isNegative(), primarySpecificationItem->isPossessive(), false, primarySpecificationWordItem, NULL ) ) != NULL )
											primarySpecificationItem = conclusionPrimarySpecificationItem;

										isPrimarySpecificationHidden = primarySpecificationItem->isHiddenSpecification();
										existingPositiveSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, false, NO_QUESTION_PARAMETER, anotherPrimarySpecificationWordItem );
										foundNegativeAssumptionOrConclusionPrimarySpecificationItem = NULL;

										if( ( negativeAssumptionOrConclusionJustificationItem = generalizationWordItem->negativeAssumptionOrConclusionJustificationItem( anotherPrimarySpecificationItem ) ) != NULL )
											foundNegativeAssumptionOrConclusionPrimarySpecificationItem = negativeAssumptionOrConclusionJustificationItem->primarySpecificationItem();

										// No positive specification exists with this specification word
										if( ( existingPositiveSpecificationItem == NULL ||
										existingPositiveSpecificationItem->isHiddenSpecification() ) &&

										// Prefer primary specification with the least uncertainty
										( !primarySpecificationItem->hasSpecificationCollection() ||
										foundNegativeAssumptionOrConclusionPrimarySpecificationItem == NULL ||
										primarySpecificationItem->assumptionLevel() < foundNegativeAssumptionOrConclusionPrimarySpecificationItem->assumptionLevel() ) &&

										// Select correct compound specification collection when collected with itself
										( !isSpecificationCollectedWithItself ||
										generalizationWordItem->isUserRelationWord ||
										anotherPrimarySpecificationItem->hasSpecificationNonCompoundCollection() ||
										anotherPrimarySpecificationWordItem->lastCompoundCollectionNr( specificationWordTypeNr ) == anotherPrimarySpecificationCollectionNr ) &&

										// No question exists with this specification word
										generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, false, WORD_PARAMETER_SINGULAR_VERB_IS, anotherPrimarySpecificationWordItem ) == NULL )
											{
											if( ( secondarySpecificationWordItem = secondarySpecificationItem->specificationWordItem() ) != NULL )
												{
												isPrimarySameAsUserSpecification = ( primarySpecificationWordItem == secondarySpecificationWordItem );
												isUserGeneralizationWord = generalizationWordItem->isUserGeneralizationWord;

												if( !isPrimarySameAsUserSpecification )
													anotherSecondarySpecificationItem = secondarySpecificationWordItem->bestMatchingSpecificationWordSpecificationItem( true, false, false, false, false, anotherPrimarySpecificationCollectionNr, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, NULL );

												if( isUserGeneralizationWord &&
												( tempSpecificationItem = adminItem_->userSpecificationItem() ) != NULL )
													{
													if( ( userSpecificationWordItem = tempSpecificationItem->specificationWordItem() ) != NULL )
														{
														if( userSpecificationWordItem->isNounWordCollectedWithItself() )
															isUserSpecificationCollectedWithItself = true;

														if( !isPrimarySpecificationHidden ||

														( isUserSpecificationCollectedWithItself &&
														!tempSpecificationItem->isHiddenSpecification() ) )
															{
															userSpecificationItem = tempSpecificationItem;

															if( anotherSecondarySpecificationItem != NULL &&
															userSpecificationItem->hasSpecificationCompoundCollection() &&
															userSpecificationWordItem->hasCollectionNr( anotherPrimarySpecificationCollectionNr ) )
																isSelectingUserSpecification = true;
															}
														else
															{
															if( isSpecificationCollectedWithItself &&
															!anotherPrimarySpecificationWordItem->isNounWordCollectedWithItself() )
																{
																userSpecificationItem = adminItem_->simpleUserSpecificationItem();

																if( anotherSecondarySpecificationItem != NULL &&
																!anotherSecondarySpecificationItem->isGeneralizationProperName() &&
																anotherPrimarySpecificationItem->hasSpecificationCompoundCollection() &&
																anotherPrimarySpecificationCollectionNr == anotherSecondarySpecificationItem->specificationCollectionNr() )
																	hasAnotherSecondarySpecificationWithCompoundCollection = true;
																}
															}

														if( userSpecificationItem != NULL &&
														userSpecificationItem->isPossessive() )
															oppositePossessiveDefinitionSpecificationItem = adminItem_->oppositePossessiveDefinitionSpecificationItem();
														}
													}

												existingNegativeSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( isArchivedAssignment, isArchivedAssignment, true, false, NO_QUESTION_PARAMETER, anotherPrimarySpecificationWordItem );

												if( isSelectingUserSpecification ||

												( ( existingNegativeSpecificationItem == NULL ||

												( !isPrimarySpecificationHidden &&

												( existingNegativeSpecificationItem->isOlderItem() ||
												existingNegativeSpecificationItem->hasSpecificationNonCompoundCollection() ) ) ) &&

												generalizationWordItem->primarySpecificationJustificationItem( true, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION, primarySpecificationItem ) == NULL ) ||

												( isUserGeneralizationWord &&
												userSpecificationItem != NULL &&

												( ( !primarySpecificationItem->hasRelationContext() &&

												( existingNegativeSpecificationItem == NULL ||
												existingNegativeSpecificationItem->isOlderItem() ) ) ||

												( isPrimarySpecificationHidden &&
												existingNegativeSpecificationItem != NULL &&

												( existingNegativeSpecificationItem->hasSpecificationNonCompoundCollection() ||

												( existingNegativeSpecificationItem->isOlderItem() &&
												!existingNegativeSpecificationItem->isConcludedAssumption() &&
												existingNegativeSpecificationItem->hasSpecificationCompoundCollection() ) ) ) ) ) )
													{
													// Draw a negative conclusion
													// Not yet explained in the design
													if( ( specificationResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, primarySpecificationItem->isExclusiveSpecification(), false, true, false, false, false, false, false, NO_ASSUMPTION_LEVEL, ( isArchivedAssignment && isUserSpecificationCollectedWithItself && primarySpecificationItem->isSelfGeneratedAssumption() ? JUSTIFICATION_TYPE_NEGATIVE_CONCLUSION : JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION ), JUSTIFICATION_TYPE_NEGATIVE_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, WORD_TYPE_UNDEFINED, anotherPrimarySpecificationCollectionNr, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, ( isPrimarySpecificationHidden ? ( hasAnotherSecondarySpecificationWithCompoundCollection || userSpecificationItem == NULL ? anotherPrimarySpecificationItem : userSpecificationItem ) : ( isPrimarySameAsUserSpecification ? anotherPrimarySpecificationItem : primarySpecificationItem ) ), ( isPrimarySpecificationHidden ? oppositePossessiveDefinitionSpecificationItem : ( isPrimarySameAsUserSpecification ? NULL : anotherPrimarySpecificationItem ) ), ( isPrimarySpecificationHidden && userSpecificationItem != NULL ? ( hasAnotherSecondarySpecificationWithCompoundCollection ? userSpecificationItem : anotherPrimarySpecificationItem ) : ( isSelectingUserSpecification ? userSpecificationItem : secondarySpecificationItem ) ), ( isPrimarySpecificationHidden && userSpecificationItem != NULL ? ( hasAnotherSecondarySpecificationWithCompoundCollection ? anotherSecondarySpecificationItem : secondarySpecificationItem ) : ( isSelectingUserSpecification ? NULL : anotherSecondarySpecificationItem ) ), generalizationWordItem, anotherPrimarySpecificationWordItem, NULL ) ).result == RESULT_OK )
														{
														createdJustificationItem = specificationResult.createdJustificationItem;

														// Found specification
														if( ( createdSpecificationItem = specificationResult.createdSpecificationItem ) == NULL )
															{
															if( createdJustificationItem != NULL )
																{
																// Cleanup after found specification
																if( ( foundSpecificationItem = specificationResult.foundSpecificationItem ) != NULL )
																	{
																	if( negativeAssumptionOrConclusionJustificationItem == NULL )
																		{
																		if( foundSpecificationItem->isSelfGeneratedAssumption() )
																			{
																			if( createdJustificationItem->isConclusionJustification() )
																				{
																				// Replace justification of negative conclusion / assumption
																				if( foundSpecificationItem->markAsConcludedAssumption() != RESULT_OK )
																					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to mark the found specification as concluded assumption" );
																				}
																			}
																		else
																			{
																			if( !generalizationWordItem->isJustificationInUse( createdJustificationItem ) )
																				{
																				if( generalizationWordItem->replaceOrDeleteJustification( createdJustificationItem ) != RESULT_OK )
																					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete the created justification" );
																				}
																			}
																		}
																	else
																		{
																		// Replace justification of negative conclusion / assumption
																		if( generalizationWordItem->replaceJustification( negativeAssumptionOrConclusionJustificationItem, createdJustificationItem, foundSpecificationItem ) != RESULT_OK )
																			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace a negative assumption or conclusion justification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
																		}
																	}
																else
																	return adminItem_->startError( functionNameString, moduleNameString_, "The found specification item is undefined" );
																}
															}
														else
															{
															// Created specification
															// Replace corrected negative conclusion
															if( generalizationWordItem->hasCorrectedAssumptionByKnowledge() )
																{
																if( ( specificationResult = generalizationWordItem->findRelatedSpecification( false, createdSpecificationItem ) ).result == RESULT_OK )
																	{
																	relatedSpecificationItem = specificationResult.relatedSpecificationItem;

																	if( relatedSpecificationItem != NULL &&
																	relatedSpecificationItem->isOlderItem() )
																		{
																		if( generalizationWordItem->replaceOrDeleteSpecification( relatedSpecificationItem, createdSpecificationItem ) != RESULT_OK )
																			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace a corrected negative conclusion in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
																		}
																	}
																else
																	return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find out if a negative conclusion in generalization word \"", generalizationWordItem->anyWordTypeString(), "\" is related to the created negative conclusion" );
																}
															else
																{
																// Replace question justification
																if( anotherSecondarySpecificationItem != NULL &&
																anotherPrimarySpecificationItem->hasSpecificationCompoundCollection() )
																	{
																	if( ( questionJustificationItem = generalizationWordItem->secondarySpecificationJustificationItem( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, anotherPrimarySpecificationItem ) ) == NULL )
																		{
																		if( isSelectingUserSpecification )
																			{
																			if( generalizationWordItem->changeJustificationOfNegativeAssumptions( userSpecificationItem ) != RESULT_OK )
																				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace a question justification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
																			}
																		}
																	else
																		{
																		if( generalizationWordItem->replaceJustification( questionJustificationItem, createdJustificationItem, createdSpecificationItem ) != RESULT_OK )
																			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace a question justification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
																		}
																	}
																}
															}
														}
													else
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to create a negative conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", anotherPrimarySpecificationWordItem->anyWordTypeString(), "\"" );
													}
												}
											else
												return adminItem_->startError( functionNameString, moduleNameString_, "The given related specification item has no specification word item" );
											}
										}
									else
										return adminItem_->startError( functionNameString, moduleNameString_, "The given another primary specification item has no specification word item" );
									}
								else
									return adminItem_->startError( functionNameString, moduleNameString_, "The given another primary specification item has no specification collection" );
								}
							else
								return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification item has no specification word item" );
							}
						else
							return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification item shouldn't be possessive, nor a question, nor a hidden specification" );
						}
					else
						return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );
					}
				else
					return adminItem_->startError( functionNameString, moduleNameString_, "The given secondary specification item is undefined" );
				}
			else
				return adminItem_->startError( functionNameString, moduleNameString_, "The given another primary specification item is undefined" );
			}
		else
			return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification item is undefined" );

		return RESULT_OK;
		}

	ResultType drawPossessiveReversibleConclusion( bool isArchivedAssignment, bool isExclusiveSpecification, bool isPossessive, bool isUniqueUserRelation, unsigned short userAssumptionLevel, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *generalizationWordItem, WordItem *secondarySpecificationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		ContextResultType contextResult;
		SpecificationResultType specificationResult;
		bool hasFeminineProperNameEnding;
		bool isAssignment;
		bool isConclusionAssignment;
		bool isForcingNewJustification;
		bool isSecondarySpecificationWordCollectedWithItself;
		bool isAssumption = ( userAssumptionLevel > NO_ASSUMPTION_LEVEL );
		unsigned int foundRelationContextNr;
		unsigned int nContextRelations;
		unsigned int generalizationContextNr = NO_CONTEXT_NR;
		unsigned int specificationCollectionNr = NO_COLLECTION_NR;
		GeneralizationItem *currentGeneralizationItem;
		JustificationItem *createdJustificationItem;
		JustificationItem *firstJustificationItem;
		JustificationItem *obsoleteJustificationItem;
		SpecificationItem *conclusionSpecificationItem;
		SpecificationItem *createdOrFoundSpecificationItem;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *existingUserSpecificationItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *secondarySpecificationItem;
		WordItem *collectedWithItselfCommonWordItem;
		WordItem *currentGeneralizationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawPossessiveReversibleConclusion";

		if( relationContextNr > NO_CONTEXT_NR )
			{
			if( generalizationWordItem != NULL )
				{
				if( secondarySpecificationWordItem != NULL )
					{
					if( specificationWordItem != NULL )
						{
						if( relationWordItem != NULL )
							{
							existingSpecificationItem = relationWordItem->bestMatchingRelationContextNrSpecificationItem( false, !isPossessive, NO_QUESTION_PARAMETER, specificationWordItem, generalizationWordItem );

							if( existingSpecificationItem == NULL ||
							// Allow existing conclusion specification to change its relation context
							existingSpecificationItem->isSelfGenerated() )
								{
								if( ( foundRelationContextNr = generalizationWordItem->contextNr( specificationWordItem ) ) > NO_CONTEXT_NR )
									{
									if( ( nContextRelations = adminItem_->nContextWordsInAllWords( foundRelationContextNr, specificationWordItem ) ) > 1 )
										{
										if( ( contextResult = adminItem_->getRelationContext( isArchivedAssignment, false, !isPossessive, false, false, specificationWordTypeNr, nContextRelations, relationWordItem, specificationWordItem, generalizationWordItem, NULL ) ).result == RESULT_OK )
											generalizationContextNr = contextResult.contextNr;
										else
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to get the relation context number" );
										}
									else
										{
										if( existingSpecificationItem != NULL )
											generalizationContextNr = foundRelationContextNr;
										}
									}

								isSecondarySpecificationWordCollectedWithItself = secondarySpecificationWordItem->isNounWordCollectedWithItself();

								conclusionSpecificationItem = contextResult.conclusionSpecificationItem;
								existingUserSpecificationItem = relationWordItem->firstUserSpecificationItem( false, !isPossessive, NO_COLLECTION_NR, NO_CONTEXT_NR, secondarySpecificationWordItem );

								// Find the non-reversible specification (active assignments)
								if( isSecondarySpecificationWordCollectedWithItself )
									secondarySpecificationItem = generalizationWordItem->firstUserSpecificationItem( false, isPossessive, NO_COLLECTION_NR, NO_CONTEXT_NR, secondarySpecificationWordItem );
								else
									secondarySpecificationItem = generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, true, isArchivedAssignment, false, isPossessive, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, secondarySpecificationWordItem );

								// Skip if possessive user specification exists
								if( secondarySpecificationItem != NULL &&

								( !isSecondarySpecificationWordCollectedWithItself ||
								secondarySpecificationItem->hasSpecificationNonCompoundCollection() ||
								existingUserSpecificationItem == NULL ||
								!existingUserSpecificationItem->hasRelationContext() ) )
									{
									if( ( existingSpecificationItem == NULL ||
									secondarySpecificationItem->isUserSpecification() ||
									existingSpecificationItem->relationContextNr() == generalizationContextNr ) &&

									( secondarySpecificationItem->assumptionLevel() != userAssumptionLevel ||
									secondarySpecificationItem->hasNewInformation() ) )
										{
										hasFeminineProperNameEnding = ( isPossessive &&
																		isSecondarySpecificationWordCollectedWithItself &&
																		relationWordItem->hasFeminineProperNameEnding() );

										isConclusionAssignment = ( ( isArchivedAssignment &&
																	isAssumption ) ||

																	( !isAssumption &&

																	( isArchivedAssignment ||
																	secondarySpecificationItem->hasExclusiveGeneralizationCollection() ) ) );

										isAssignment =				( isConclusionAssignment ||

																	( existingUserSpecificationItem != NULL &&
																	existingUserSpecificationItem->isAssignment() ) );

										isForcingNewJustification = ( isSecondarySpecificationWordCollectedWithItself ||

																	( !isPossessive &&
																	existingSpecificationItem != NULL &&

																	( conclusionSpecificationItem != NULL ||
																	secondarySpecificationItem->isActiveAssignment() ||
																	existingSpecificationItem->relationContextNr() != generalizationContextNr ) ) );

										if( isSecondarySpecificationWordCollectedWithItself &&
										secondarySpecificationWordItem == specificationWordItem &&

										( !isPossessive ||
										!relationWordItem->isMale() ) )
											specificationCollectionNr = secondarySpecificationItem->specificationCollectionNr();

										// Draw a possessive reversible conclusion
										// See design: paragraph 2.3.2 in the theory document or http://mafait.org/theory_2_3_2/
										if( ( specificationResult = adminItem_->addSelfGeneratedSpecification( hasFeminineProperNameEnding, isAssignment, isArchivedAssignment, false, isExclusiveSpecification, isForcingNewJustification, false, false, !isPossessive, isUniqueUserRelation, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, relationWordTypeNr, WORD_TYPE_NOUN_SINGULAR, generalizationWordTypeNr, specificationCollectionNr, NO_CONTEXT_NR, specificationContextNr, generalizationContextNr, NULL, NULL, secondarySpecificationItem, NULL, relationWordItem, specificationWordItem, generalizationWordItem ) ).result == RESULT_OK )
											{
											if( conclusionSpecificationItem == NULL )
												{
												if( specificationResult.createdSpecificationItem == NULL )
													{
													if( generalizationWordItem->replaceOrDeleteObsoleteJustification() == RESULT_OK )
														createdOrFoundSpecificationItem = specificationResult.foundSpecificationItem;
													else
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace an obsolete justification item in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
													}
												else
													createdOrFoundSpecificationItem = specificationResult.createdSpecificationItem;

												if( createdOrFoundSpecificationItem != NULL )
													{
													if( isPossessive )
														{
														if( adminItem_->makePossessiveConditionalSpecificationAssumption( isArchivedAssignment, isExclusiveSpecification, true, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, createdOrFoundSpecificationItem, NULL, generalizationWordItem, relationWordItem ) == RESULT_OK )
															{
															if( !isSecondarySpecificationWordCollectedWithItself ||
															( collectedWithItselfCommonWordItem = createdOrFoundSpecificationItem->collectedWithItselfCommonWordItem() ) == NULL )
																{
																if( specificationResult.foundSpecificationItem != NULL )
																	{
																	if( generalizationWordItem->replaceOrDeleteObsoleteJustification() != RESULT_OK )
																		return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace an obsolete justification item in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
																	}
																}
															else
																{
																// If the reversed specification doesn't already exist
																if( generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( false, true, NO_QUESTION_PARAMETER, specificationWordItem, relationWordItem ) == NULL )
																	{
																	// Generate the reversed specification
																	if( adminItem_->drawPossessiveReversibleConclusion( isArchivedAssignment, isExclusiveSpecification, false, false, createdOrFoundSpecificationItem->assumptionLevel(), relationWordTypeNr, specificationWordTypeNr, generalizationWordTypeNr, specificationContextNr, createdOrFoundSpecificationItem->relationContextNr(), relationWordItem, specificationWordItem, generalizationWordItem ) != RESULT_OK )
																		return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a possessive reversible conclusion from relation word \"", relationWordItem->anyWordTypeString(), "\" to specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
																	}

																if( ( currentGeneralizationItem = collectedWithItselfCommonWordItem->firstNounSpecificationGeneralizationItem() ) != NULL )
																	{
																	do	{
																		if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) != NULL )
																			{
																			if( ( foundSpecificationItem = currentGeneralizationWordItem->firstAssignmentOrSpecificationItem( isConclusionAssignment, isArchivedAssignment, false, false, NO_QUESTION_PARAMETER, collectedWithItselfCommonWordItem ) ) != NULL )
																				{
																				if( foundSpecificationItem->isPartOf() &&

																				( !createdOrFoundSpecificationItem->hasAssumptionLevel() ||
																				// Skip if 'part of' conclusion exists
																				relationWordItem->firstSelfGeneratedCheckSpecificationItem( true, isArchivedAssignment, false, false, false, currentGeneralizationWordItem, NULL ) == NULL ) )
																					{
																					// Draw a 'part of' generalization conclusion
																					// Not yet explained in the design
																					if( adminItem_->addSelfGeneratedSpecification( false, isConclusionAssignment, isArchivedAssignment, false, false, false, false, true, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, relationWordTypeNr, specificationWordTypeNr, WORD_TYPE_UNDEFINED, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, createdOrFoundSpecificationItem, NULL, foundSpecificationItem, NULL, relationWordItem, currentGeneralizationWordItem, NULL ).result != RESULT_OK )
																						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a self-generated reversible 'part of' specification to relation word \"", relationWordItem->anyWordTypeString(), "\" to specification word \"", currentGeneralizationWordItem->anyWordTypeString(), "\"" );
																					}
																				}
																			}
																		else
																			return adminItem_->startError( functionNameString, moduleNameString_, "I have found an undefined generalization word" );
																		}
																	while( ( currentGeneralizationItem = currentGeneralizationItem->nextNounSpecificationGeneralizationItem() ) != NULL );
																	}

																if( ( primarySpecificationItem = relationWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, NO_QUESTION_PARAMETER, collectedWithItselfCommonWordItem, generalizationWordItem ) ) != NULL &&
																( secondarySpecificationItem = collectedWithItselfCommonWordItem->bestMatchingSpecificationWordSpecificationItem( true, false, false, false, false, createdOrFoundSpecificationItem->specificationCollectionNr(), NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, specificationWordItem ) ) != NULL )
																	{
																	if( drawNegativeConclusion( isArchivedAssignment, false, relationWordTypeNr, secondarySpecificationItem->specificationWordTypeNr(), NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, secondarySpecificationItem, createdOrFoundSpecificationItem, relationWordItem ) != RESULT_OK )
																		return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a negative conclusion about relation word \"", relationWordItem->anyWordTypeString(), "\"" );
																	}
																}
															}
														else
															return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make an exclusive specification substitution assumption from generalization word \"", generalizationWordItem->anyWordTypeString(), "\" to specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
														}
													else
														{
														if( secondarySpecificationItem->isUserSpecification() &&
														createdOrFoundSpecificationItem->isOlderItem() &&
														createdOrFoundSpecificationItem->isSelfGeneratedAssumption() &&
														adminItem_->nContextWordsInAllWords( createdOrFoundSpecificationItem->relationContextNr(), specificationWordItem ) == 1 )
															{
															if( createdOrFoundSpecificationItem->markAsConcludedAssumption() == RESULT_OK )
																{
																if( relationWordItem->writeUpdatedSpecification( true, true, false, false, createdOrFoundSpecificationItem ) != RESULT_OK )
																	return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the concluded assumption in relation word \"", relationWordItem->anyWordTypeString(), "\"" );
																}
															else
																return adminItem_->addError( functionNameString, moduleNameString_, "I failed to mark the found specification as concluded assumption" );
															}
														}

													if( generalizationWordItem->isUserGeneralizationWord &&
													specificationWordItem->isUserSpecificationWord )
														{
														if( isSecondarySpecificationWordCollectedWithItself &&
														( existingSpecificationItem = relationWordItem->bestMatchingRelationContextNrSpecificationItem( true, true, false, !isPossessive, NO_QUESTION_PARAMETER, NO_CONTEXT_NR, specificationWordItem ) ) != NULL )
															{
															if( existingSpecificationItem->isHiddenSpecification() &&
															existingSpecificationItem->specificationCollectionNr() != createdOrFoundSpecificationItem->specificationCollectionNr() )
																{
																// Add context to hidden specification
																if( generalizationWordItem->addContext( false, relationWordTypeNr, specificationWordTypeNr, existingSpecificationItem->relationContextNr(), specificationWordItem ) == RESULT_OK )
																	{
																	if( relationWordItem->writeUpdatedSpecification( true, false, false, false, existingSpecificationItem ) != RESULT_OK )
																		return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the hidden assumption in relation word \"", relationWordItem->anyWordTypeString(), "\"" );
																	}
																else
																	return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a relation context to word \"", generalizationWordItem->anyWordTypeString(), "\"" );
																}
															}

														if( adminItem_->makeExclusiveSpecificationSubstitutionAssumption( isArchivedAssignment, isExclusiveSpecification, false, !isPossessive, relationWordTypeNr, specificationWordTypeNr, generalizationWordTypeNr, specificationContextNr, relationWordItem, specificationWordItem, generalizationWordItem ) != RESULT_OK )
															return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make an exclusive specification substitution assumption with relation word \"", relationWordItem->anyWordTypeString(), "\" and specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
														}
													}
												else
													return adminItem_->startError( functionNameString, moduleNameString_, "No specification item was found or created" );
												}
											else
												{
												if( ( foundSpecificationItem = specificationResult.foundSpecificationItem ) != NULL )
													{
													if( foundSpecificationItem->isSelfGeneratedAssumption() )
														{
														if( foundSpecificationItem->markAsConcludedAssumption() == RESULT_OK )
															{
															// Write adjusted specification
															if( relationWordItem->writeUpdatedSpecification( true, true, false, false, foundSpecificationItem ) == RESULT_OK )
																{
																if( ( firstJustificationItem = conclusionSpecificationItem->firstJustificationItem() ) != NULL )
																	{
																	if( ( createdJustificationItem = specificationResult.createdJustificationItem ) != NULL )
																		{
																		// Attach older conclusion justifications to the created conclusion justification item
																		if( createdJustificationItem->changeAttachedJustification( firstJustificationItem ) == RESULT_OK )
																			{
																			if( relationWordItem->replaceOrDeleteSpecification( conclusionSpecificationItem, foundSpecificationItem ) == RESULT_OK )
																				{
																				firstJustificationItem = foundSpecificationItem->firstJustificationItem();

																				if( firstJustificationItem != NULL &&
																				!firstJustificationItem->isPossessiveReversibleConclusion() )
																					{
																					if( firstJustificationItem->changeAttachedJustification( createdJustificationItem ) != RESULT_OK )
																						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to attach older conclusion justification items to the created conclusion justification item" );
																					}

																				while( ( obsoleteJustificationItem = relationWordItem->obsoleteJustificationItem() ) != NULL )
																					{
																					if( relationWordItem->replaceOrDeleteJustification( obsoleteJustificationItem ) != RESULT_OK )
																						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete an obsolete assumption justification item in relation word \"", relationWordItem->anyWordTypeString(), "\"" );
																					}
																				}
																			else
																				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace or delete a specification in relation word \"", relationWordItem->anyWordTypeString(), "\"" );
																			}
																		else
																			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to attach older conclusion justification items to the created conclusion justification item" );
																		}
																	else
																		return adminItem_->startError( functionNameString, moduleNameString_, "The created conclusion justification item is undefined" );
																	}
																else
																	return adminItem_->startError( functionNameString, moduleNameString_, "The conclusion specification item has no justifications" );
																}
															else
																return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an adjusted specification in relation word \"", relationWordItem->anyWordTypeString(), "\"" );
															}
														else
															return adminItem_->addError( functionNameString, moduleNameString_, "I failed to mark the replacing specification as concluded assumption in relation word \"", relationWordItem->anyWordTypeString(), "\"" );
														}
													}
												else
													return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find an older specification to replace" );
												}
											}
										else
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a self-generated possessive reversible specification from relation word \"", relationWordItem->anyWordTypeString(), "\" to specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
										}
									}
								}
							}
						else
							return adminItem_->startError( functionNameString, moduleNameString_, "The given relation word item is undefined" );
						}
					else
						return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );
					}
				else
					return adminItem_->startError( functionNameString, moduleNameString_, "The given secondary specification word item is undefined" );
				}
			else
				return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );
			}
		else
			return adminItem_->startError( functionNameString, moduleNameString_, "The given relation context number is undefined" );

		return RESULT_OK;
		}

	ResultType drawSpecificationGeneralizationConclusion( unsigned short assumptionLevel, SpecificationItem *secondarySpecificationItem )
		{
		bool isArchivedAssignment;
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawSpecificationGeneralizationConclusion";

		if( secondarySpecificationItem != NULL )
			{
			if( secondarySpecificationItem->isGeneralizationNoun() &&
			secondarySpecificationItem->isSpecificationNoun() )
				{
				secondarySpecificationItem = secondarySpecificationItem->updatedSpecificationItem();
				isArchivedAssignment = secondarySpecificationItem->isArchivedAssignment();

				// Draw a specification generalization conclusion
				// Not yet explained in the design
				if( adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, secondarySpecificationItem->isExclusiveSpecification(), false, secondarySpecificationItem->isNegative(), secondarySpecificationItem->isPartOf(), secondarySpecificationItem->isPossessive(), false, false, true, assumptionLevel, JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, WORD_TYPE_NOUN_PLURAL, secondarySpecificationItem->generalizationWordTypeNr(), WORD_TYPE_UNDEFINED, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NULL, NULL, secondarySpecificationItem, NULL, secondarySpecificationItem->specificationWordItem(), secondarySpecificationItem->generalizationWordItem(), NULL ).result != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a self-generated specification generalization substitution specification" );
				}
			else
				return adminItem_->startError( functionNameString, moduleNameString_, "The generalization word and/or specification word of the given primary specification item isn't a noun" );
			}
		else
			return adminItem_->startError( functionNameString, moduleNameString_, "The given secondary specification item is undefined" );

		return RESULT_OK;
		}


	protected:
	// Constructor / deconstructor

	AdminConclusion( AdminItem *adminItem, CommonVariables *commonVariables )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		hasFoundSpecificationWordCollectedWithItself_ = false;

		missingNegativeConclusionSpecificationCollectionNr_ = NO_COLLECTION_NR;

		missingNegativeConclusionPrimarySpecificationItem_ = NULL;
		missingNegativeConclusionAnotherPrimarySpecificationItem_ = NULL;

		adminItem_ = adminItem;
		commonVariables_ = commonVariables;
		strcpy( moduleNameString_, "AdminConclusion" );

		if( commonVariables_ == NULL )
			strcpy( errorString, "The given common variables is undefined" );

		if( adminItem_ == NULL )
			strcpy( errorString, "The given admin is undefined" );

		if( strlen( errorString ) > 0 )
			{
			if( adminItem_ != NULL )
				adminItem_->startSystemError( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, moduleNameString_, errorString );
			else
				{
			if( commonVariables_ != NULL )
				commonVariables_->result = RESULT_SYSTEM_ERROR;
				fprintf( stderr, "\nClass:%s\nFunction:\t%s\nError:\t\t%s.\n", moduleNameString_, PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, errorString );
				}
			}
		}


	// Protected functions

	void initializeAdminConclusionVariables()
		{
		hasFoundSpecificationWordCollectedWithItself_ = false;
		clearMissingNegativeConclusionVariables();
		}

	bool hasFoundUnprocessedNegativeConclusion()
		{
		return ( missingNegativeConclusionSpecificationCollectionNr_ > NO_COLLECTION_NR );
		}

	ResultType drawNegativeConclusionsFromAnsweredQuestions( SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem )
		{
		JustificationItem *currentJustificationItem;
		SpecificationItem *answeredQuestionSpecificationItem;
		WordItem *answeredQuestionSpecificationWordItem;
		WordItem *userSpecificationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawNegativeConclusionsFromAnsweredQuestions";

		if( userSpecificationItem != NULL )
			{
			if( generalizationWordItem != NULL )
				{
				if( ( userSpecificationWordItem = userSpecificationItem->specificationWordItem() ) != NULL )
					{
					if( ( answeredQuestionSpecificationItem = generalizationWordItem->firstRecentlyAnsweredQuestionSpecificationItem( WORD_PARAMETER_SINGULAR_VERB_IS ) ) != NULL )
						{
						do	{
							if( userSpecificationWordItem->hasCollectionNr( answeredQuestionSpecificationItem->specificationCollectionNr() ) )
								{
								if( ( answeredQuestionSpecificationWordItem = answeredQuestionSpecificationItem->specificationWordItem() ) != NULL )
									{
									// Check if already exists as positive specification
									if( generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, false, NO_QUESTION_PARAMETER, answeredQuestionSpecificationWordItem ) == NULL )
										{
										if( ( currentJustificationItem = answeredQuestionSpecificationItem->firstJustificationItem() ) != NULL )
											{
											// Do for all justifications
											do	{
												if( !currentJustificationItem->hasPrimaryQuestion() &&
												!currentJustificationItem->hasPossessivePrimarySpecification() )
													{
													if( drawNegativeConclusion( answeredQuestionSpecificationItem->isArchivedAssignment(), false, answeredQuestionSpecificationItem->generalizationWordTypeNr(), answeredQuestionSpecificationItem->specificationWordTypeNr(), answeredQuestionSpecificationItem->generalizationContextNr(), answeredQuestionSpecificationItem->specificationContextNr(), currentJustificationItem->primarySpecificationItem(), currentJustificationItem->secondarySpecificationItem(), userSpecificationItem, generalizationWordItem ) != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a negative conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
													}
												}
											while( ( currentJustificationItem = currentJustificationItem->attachedJustificationItem() ) != NULL );
											}
										else
											return adminItem_->startError( functionNameString, moduleNameString_, "The answered question specification item has no justification" );
										}
									}
								else
									return adminItem_->startError( functionNameString, moduleNameString_, "The answered question specification item has no specification word" );
								}
							}
						while( ( answeredQuestionSpecificationItem = answeredQuestionSpecificationItem->recentlyAnsweredQuestionSpecificationItem( false ) ) != NULL );
						}
					else
						return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find any answered question specification" );
					}
				else
					return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item has no specification word" );
				}
			else
				return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );
			}
		else
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		return RESULT_OK;
		}

	ResultType drawOnlyOptionLeftConclusion( bool isInactiveAssignment, bool isArchivedAssignment, unsigned int specificationCollectionNr, WordItem *generalizationWordItem )
		{
		bool hasFoundAssignment;
		bool hasFoundMoreOptionsLeft = false;
		SpecificationItem *currentAssignmentItem;
		SpecificationItem *currentSpecificationItem;
		SpecificationItem *firstNegativeOnlyOptionLeftAssignmentItem = NULL;
		SpecificationItem *firstOnlyOptionLeftSpecificationItem = NULL;
		SpecificationItem *foundOnlyOptionLeftSpecificationItem = NULL;
		WordItem *specificationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawOnlyOptionLeftConclusion";

		if( generalizationWordItem != NULL )
			{
			if( specificationCollectionNr > NO_COLLECTION_NR )
				{
				if( ( currentSpecificationItem = generalizationWordItem->firstNonQuestionSpecificationItem() ) != NULL )
					{
					firstOnlyOptionLeftSpecificationItem = currentSpecificationItem;

					do	{
						if( currentSpecificationItem->specificationCollectionNr() == specificationCollectionNr &&
						( currentAssignmentItem = generalizationWordItem->firstSelectedSpecificationItem( true, isInactiveAssignment, isArchivedAssignment, false ) ) != NULL )
							{
							hasFoundAssignment = false;
							specificationWordItem = currentSpecificationItem->specificationWordItem();

							if( currentAssignmentItem->isNegative() &&
							firstNegativeOnlyOptionLeftAssignmentItem == NULL )
								firstNegativeOnlyOptionLeftAssignmentItem = currentAssignmentItem;

							do	{
								if( currentAssignmentItem->isNegative() &&
								currentAssignmentItem->specificationCollectionNr() == specificationCollectionNr &&
								currentAssignmentItem->specificationWordItem() == specificationWordItem )
									hasFoundAssignment = true;
								}
							while( !hasFoundAssignment &&
							( currentAssignmentItem = currentAssignmentItem->nextSelectedSpecificationItem() ) != NULL );

							if( !hasFoundAssignment )
								{
								if( foundOnlyOptionLeftSpecificationItem == NULL )
									// Found the first option left
									foundOnlyOptionLeftSpecificationItem = currentSpecificationItem;
								else
									{
									// There are more options left
									hasFoundMoreOptionsLeft = true;
									// So, clear the found option
									foundOnlyOptionLeftSpecificationItem = NULL;
									}
								}
							}
						}
					while( !hasFoundMoreOptionsLeft &&
					( currentSpecificationItem = currentSpecificationItem->nextSelectedSpecificationItem() ) != NULL );

					if( firstNegativeOnlyOptionLeftAssignmentItem != NULL &&
					foundOnlyOptionLeftSpecificationItem != NULL )
						{
						// Draw an only option left conclusion
						// Not yet explained in the design
						if( adminItem_->addSelfGeneratedSpecification( false, true, isArchivedAssignment, false, true, false, false, false, foundOnlyOptionLeftSpecificationItem->isPossessive(), false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_ASSUMPTION, JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, firstOnlyOptionLeftSpecificationItem->generalizationWordTypeNr(), firstOnlyOptionLeftSpecificationItem->specificationWordTypeNr(), WORD_TYPE_UNDEFINED, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, firstOnlyOptionLeftSpecificationItem, NULL, firstNegativeOnlyOptionLeftAssignmentItem, NULL, generalizationWordItem, foundOnlyOptionLeftSpecificationItem->specificationWordItem(), NULL ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw an only option left conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
						}
					}
				}
			else
				return adminItem_->startError( functionNameString, moduleNameString_, "The given specification collection number is undefined" );
			}
		else
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		return RESULT_OK;
		}

	ResultType drawPossessiveReversibleConclusion( bool isArchivedAssignment, bool isExclusiveSpecification, bool isPossessive, bool isUniqueUserRelation, unsigned short userAssumptionLevel, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		WordItem *feminineSpecificationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawPossessiveReversibleConclusion";

		if( generalizationWordItem != NULL )
			{
			if( specificationWordItem != NULL )
				{
				if( relationWordItem != NULL )
					{
					if( drawPossessiveReversibleConclusion( isArchivedAssignment, isExclusiveSpecification, isPossessive, isUniqueUserRelation, userAssumptionLevel, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationContextNr, relationContextNr, generalizationWordItem, specificationWordItem, specificationWordItem, relationWordItem ) == RESULT_OK )
						{
						if( isPossessive &&
						specificationWordItem->isNounWordCollectedWithItself() &&
						relationWordItem->hasFeminineProperNameEnding() &&
						( feminineSpecificationWordItem = specificationWordItem->feminineCollectionWordItem() ) != NULL )
							{
							if( drawPossessiveReversibleConclusion( isArchivedAssignment, isExclusiveSpecification, isPossessive, isUniqueUserRelation, userAssumptionLevel, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationContextNr, relationContextNr, generalizationWordItem, specificationWordItem, feminineSpecificationWordItem, relationWordItem ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a feminine possessive reversible conclusion" );
							}
						}
					else
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a possessive reversible conclusion" );
					}
				else
					return adminItem_->startError( functionNameString, moduleNameString_, "The given relation word item is undefined" );
				}
			else
				return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );
			}
		else
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		return RESULT_OK;
		}

	ResultType drawSpecificationGeneralizationConclusion( bool isInactiveAssignment, bool isArchivedAssignment, bool isPossessive, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, SpecificationItem *secondarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		bool isAssignment = ( isInactiveAssignment || isArchivedAssignment );
		unsigned short assumptionLevel;
		GeneralizationItem *currentGeneralizationItem;
		SpecificationItem *currentSpecificationItem = NULL;
		WordItem *currentGeneralizationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawSpecificationGeneralizationConclusion";

		if( secondarySpecificationItem != NULL )
			{
			if( generalizationWordItem != NULL )
				{
				if( specificationWordItem != NULL )
					{
					if( ( currentGeneralizationItem = specificationWordItem->firstNounSpecificationGeneralizationItem() ) != NULL )
						{
						do	{
							if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) != NULL )
								{
								if( currentGeneralizationWordItem != generalizationWordItem &&
								( currentSpecificationItem = currentGeneralizationWordItem->firstAssignmentOrSpecificationItem( isAssignment, isArchivedAssignment, false, false, NO_QUESTION_PARAMETER, specificationWordItem ) ) != NULL )
									{
									if( !currentSpecificationItem->hasSpecificationCollection() &&
									!currentSpecificationItem->isExclusiveSpecification() )
										{
										assumptionLevel = ( secondarySpecificationItem->assumptionLevel() < currentSpecificationItem->assumptionLevel() ? currentSpecificationItem->assumptionLevel() : secondarySpecificationItem->assumptionLevel() );

										if( adminItem_->collectGeneralizationWordWithPreviousOne( isPossessive, generalizationWordTypeNr, specificationWordTypeNr, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, generalizationWordItem, specificationWordItem ) == RESULT_OK )
											{
											if( drawSpecificationGeneralizationConclusion( assumptionLevel, currentSpecificationItem ) == RESULT_OK )
												{
												if( drawSpecificationGeneralizationConclusion( assumptionLevel, secondarySpecificationItem ) != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a specification generalization conclusion about the given sentence" );
												}
											else
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a specification generalization conclusion about an earlier sentence" );
											}
										else
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to collect a generalization word with a previous one" );
										}
									}
								}
							else
								return adminItem_->startError( functionNameString, moduleNameString_, "I have found an undefined generalization word" );
							}
						while( ( currentGeneralizationItem = currentGeneralizationItem->nextNounSpecificationGeneralizationItem() ) != NULL );
						}
					}
				else
					return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );
				}
			else
				return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );
			}
		else
			return adminItem_->startError( functionNameString, moduleNameString_, "The given secondary specification item is undefined" );

		return RESULT_OK;
		}

	ResultType drawSpecificationSubstitutionConclusionOrAskQuestion( bool isAssumption, bool isInactiveAssignment, bool isArchivedAssignment, bool isExclusiveSpecification, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		SpecificationResultType specificationResult;
		bool hasCorrectedAssumptionByKnowledge;
		bool hasCorrectedAssumptionByOppositeQuestion;
		bool hasGeneralizationWordActiveQuestionWithCompoundCollection;
		bool hasGeneralizationWordFeminineProperNameEnding;
		bool hasGeneralizationWordMasculineProperNameEnding;
		bool hasCurrentSpecificationCompoundCollection;
		bool isExclusiveCurrentSpecification;
		bool isFemaleOrMaleGeneralizationWord;
		bool isFoundPrimaryHiddenSpecification;
		bool isGeneralizationWordUserRelationWord;
		bool isIncludingRelationWord;
		bool isSpecificationCollectedWithItself;
		bool isUserGeneralizationWord;
		bool hasRelationWord = ( relationWordItem != NULL );
		bool isAssignment = ( isInactiveAssignment || isArchivedAssignment );
		bool isDefinitionSpecification = adminItem_->isSingularOrPluralNoun( generalizationWordTypeNr );
		bool isExclusiveUserSpecification = false;
		bool isGeneralizationProperName = ( generalizationWordTypeNr == WORD_TYPE_PROPER_NAME );
		bool isNegativeConclusion = false;
		bool isPossessiveUserSpecification = adminItem_->isPossessiveUserSpecification();
		bool isQuestion = ( questionParameter > NO_QUESTION_PARAMETER );
		bool isUserSpecificationWordCollectedWithItself = false;
		unsigned int currentSpecificationCollectionNr;
		unsigned int relationContextNr;
		SpecificationItem *alternativeAnotherPrimarySpecificationItem;
		SpecificationItem *alternativeJustificationSpecificationItem;
		SpecificationItem *bestMatchingRelationSpecificationItem;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *currentSpecificationItem;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *feminineOrMasculineSpecificationItem;
		SpecificationItem *foundPrimarySpecificationItem;
		SpecificationItem *oppositePossessiveDefinitionSpecificationItem;
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *collectedWithItselfSpecificationItem = NULL;
		SpecificationItem *userSpecificationItem = adminItem_->userSpecificationItem();
		WordItem *currentSpecificationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawSpecificationSubstitutionConclusionOrAskQuestion";

		if( adminItem_->isGeneralizationReasoningWordType( generalizationWordTypeNr ) )
			{
			if( adminItem_->isSingularOrPluralNoun( specificationWordTypeNr ) )
				{
				if( generalizationWordItem != NULL )
					{
					if( specificationWordItem != NULL )
						{
						hasCorrectedAssumptionByKnowledge = generalizationWordItem->hasCorrectedAssumptionByKnowledge();
						isGeneralizationWordUserRelationWord = generalizationWordItem->isUserRelationWord;
						isSpecificationCollectedWithItself = specificationWordItem->isNounWordCollectedWithItself();
						isUserGeneralizationWord = generalizationWordItem->isUserGeneralizationWord;

						// Skip definition sentence with the same generalization and specification word
						if( !hasFoundSpecificationWordCollectedWithItself_ ||
						!hasCorrectedAssumptionByKnowledge )
							{
							if( adminItem_->makeSpecificationSubstitutionPartOfAssumption( isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, generalizationWordItem, specificationWordItem ) == RESULT_OK )
								{
								if( isSpecificationCollectedWithItself )
									hasFoundSpecificationWordCollectedWithItself_ = true;
								}
							else
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a specification substitution 'part of' assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
							}

						if( ( currentSpecificationItem = specificationWordItem->firstNonQuestionSpecificationItem() ) != NULL )
							{
							hasCorrectedAssumptionByOppositeQuestion = generalizationWordItem->hasCorrectedAssumptionByOppositeQuestion();

							hasGeneralizationWordActiveQuestionWithCompoundCollection = generalizationWordItem->hasActiveQuestionWithCompoundCollection();
							hasGeneralizationWordFeminineProperNameEnding = generalizationWordItem->hasFeminineProperNameEnding();
							hasGeneralizationWordMasculineProperNameEnding = generalizationWordItem->hasMasculineProperNameEnding();

							isFemaleOrMaleGeneralizationWord = generalizationWordItem->isFemaleOrMale();

							if( userSpecificationItem != NULL )
								{
								isExclusiveUserSpecification = userSpecificationItem->isExclusiveSpecification();
								isUserSpecificationWordCollectedWithItself = userSpecificationItem->isSpecificationWordCollectedWithItself();
								}

							if( isSpecificationCollectedWithItself )
								collectedWithItselfSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( isAssignment, isArchivedAssignment, false, false, NO_QUESTION_PARAMETER, specificationWordItem );

							oppositePossessiveDefinitionSpecificationItem = adminItem_->oppositePossessiveDefinitionSpecificationItem();

							do	{
								// Select non-possessive definition
								if( !currentSpecificationItem->isPossessive() &&
								currentSpecificationItem->isSpecificationNoun() &&
								!currentSpecificationItem->isSpecificationGeneralization() &&
								( currentSpecificationWordItem = currentSpecificationItem->specificationWordItem() ) != NULL )
									{
									isExclusiveCurrentSpecification = currentSpecificationItem->isExclusiveSpecification();
									currentSpecificationCollectionNr = currentSpecificationItem->specificationCollectionNr();

									if( ( bestMatchingRelationSpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( true, false, isAssignment, isInactiveAssignment, isArchivedAssignment, false, false, questionParameter, currentSpecificationCollectionNr, NO_CONTEXT_NR, currentSpecificationWordItem ) ) != NULL )
										{
										if( !isQuestion &&
										!bestMatchingRelationSpecificationItem->isExclusiveSpecification() )
											isNegativeConclusion = true;
										}

									if( ( foundPrimarySpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( ( isAssignment || hasCorrectedAssumptionByOppositeQuestion ), isArchivedAssignment, false, false, questionParameter, specificationWordItem ) ) == NULL )
										{
										// Check for more conflicts
										if( generalizationWordItem->checkForSpecificationConflict( isArchivedAssignment, isExclusiveSpecification, false, false, specificationWordTypeNr, currentSpecificationCollectionNr, currentSpecificationItem->generalizationContextNr(), currentSpecificationItem->specificationContextNr(), currentSpecificationItem->relationContextNr(), currentSpecificationWordItem ) != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to check for a specification conflict in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
										}
									else
										{
										isFoundPrimaryHiddenSpecification = foundPrimarySpecificationItem->isHiddenSpecification();

										if( isExclusiveCurrentSpecification &&
										!isFemaleOrMaleGeneralizationWord &&
										( feminineOrMasculineSpecificationItem = currentSpecificationWordItem->firstFeminineOrMasculineSpecificationItem() ) != NULL )
											{
											// Matching feminine word
											if( ( ( hasGeneralizationWordFeminineProperNameEnding &&
											currentSpecificationWordItem->isFemale() ) ||

											// Matching masculine word
											( hasGeneralizationWordMasculineProperNameEnding &&
											currentSpecificationWordItem->isMale() ) ) &&

											( ( isFoundPrimaryHiddenSpecification &&
											isUserGeneralizationWord ) ||

											( !isFoundPrimaryHiddenSpecification &&

											( ( !isUserSpecificationWordCollectedWithItself &&
											!hasGeneralizationWordActiveQuestionWithCompoundCollection ) ||

											( !isPossessiveUserSpecification &&
											foundPrimarySpecificationItem->hasRelationContext() ) ) ) ) &&

											// Select correct compound word to avoid assumption to be made if a question should be asked
											currentSpecificationWordItem->compoundGeneralizationWordItem( currentSpecificationCollectionNr ) != specificationWordItem )
												{
												isIncludingRelationWord = ( hasRelationWord &&
																			feminineOrMasculineSpecificationItem->hasSpecificationCollection() );

												existingSpecificationItem = ( hasRelationWord ? generalizationWordItem->firstAssignmentOrSpecificationItem( isArchivedAssignment, isArchivedAssignment, false, false, NO_QUESTION_PARAMETER, currentSpecificationWordItem ) : NULL );

												if( !hasRelationWord ||
												isPossessiveUserSpecification ||

												( isIncludingRelationWord &&

												( existingSpecificationItem == NULL ||
												existingSpecificationItem->assumptionLevel() >= foundPrimarySpecificationItem->assumptionLevel() ) ) )
													{
													// Make a feminine or masculine proper name ending assumption
													// Not yet explained in the design
													if( ( specificationResult = adminItem_->addSelfGeneratedSpecification( true, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, foundPrimarySpecificationItem->prepositionParameter(), NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, ( isIncludingRelationWord ? relationWordTypeNr : WORD_TYPE_UNDEFINED ), NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, ( isFoundPrimaryHiddenSpecification ? userSpecificationItem : foundPrimarySpecificationItem ), ( isFoundPrimaryHiddenSpecification ? oppositePossessiveDefinitionSpecificationItem : NULL ), currentSpecificationItem, NULL, generalizationWordItem, currentSpecificationWordItem, ( isIncludingRelationWord ? relationWordItem : NULL ) ) ).result == RESULT_OK )
														{
														if( ( createdSpecificationItem = specificationResult.createdSpecificationItem ) == NULL )
															{
															if( generalizationWordItem->replaceOrDeleteObsoleteJustification() == RESULT_OK )
																relationContextNr = ( specificationResult.foundSpecificationItem == NULL ? NO_CONTEXT_NR : specificationResult.foundSpecificationItem->relationContextNr() );
															else
																return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace or delete an obsolete assumption justification item in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
															}
														else
															{
															relationContextNr = createdSpecificationItem->relationContextNr();

															if( !isUserSpecificationWordCollectedWithItself &&
															// Has missed drawing some negative conclusions
															missingNegativeConclusionSpecificationCollectionNr_ == currentSpecificationCollectionNr )
																{
																// Draw missing negative conclusions
																if( drawSpecificationSubstitutionConclusionOrAskQuestion( isAssumption, isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, specificationWordItem, relationWordItem ) != RESULT_OK )
																	return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find a specification substitution conclusion or question for generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
																}
															}

														if( isIncludingRelationWord &&
														relationContextNr > NO_CONTEXT_NR )
															{
															if( drawPossessiveReversibleConclusion( isArchivedAssignment, false, false, false, NO_ASSUMPTION_LEVEL, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationContextNr, relationContextNr, generalizationWordItem, currentSpecificationWordItem, relationWordItem ) != RESULT_OK )
																return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a possessive reversible conclusion from generalization word \"", generalizationWordItem->anyWordTypeString(), "\" to specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );
															}
														}
													else
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a feminine or masculine substitution assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );
													}
												}
											}

										if( !isExclusiveCurrentSpecification ||
										bestMatchingRelationSpecificationItem == NULL ||

										// Definition collected with itself
										( generalizationWordItem == specificationWordItem &&
										!specificationWordItem->isUserSpecificationWord &&
										!currentSpecificationItem->isOlderItem() &&
										currentSpecificationItem->isUserSpecification() ) )
											{
											if( isGeneralizationProperName ||
											currentSpecificationCollectionNr > NO_COLLECTION_NR )
												{
												if( ( !isExclusiveSpecification &&
												isExclusiveCurrentSpecification ) ||

												( isGeneralizationProperName &&

												( ( isExclusiveSpecification &&
												currentSpecificationCollectionNr > NO_COLLECTION_NR ) ||

												( isSpecificationCollectedWithItself &&
												!isFoundPrimaryHiddenSpecification &&

												( ( isExclusiveSpecification &&

												( isGeneralizationWordUserRelationWord ||
												isPossessiveUserSpecification ) ) ||

												( isGeneralizationWordUserRelationWord &&
												isPossessiveUserSpecification &&
												foundPrimarySpecificationItem->hasSpecificationNonCompoundCollection() ) ) ) ) ) )
													{
													hasCurrentSpecificationCompoundCollection = ( currentSpecificationWordItem->compoundCollectionNr( specificationWordTypeNr ) > NO_COLLECTION_NR );

													// Ask a question or draw a negative conclusion
													if( isGeneralizationProperName &&
													currentSpecificationCollectionNr > NO_COLLECTION_NR &&

													( adminItem_->hasFoundSuggestiveQuestionAssumption() ||
													generalizationWordItem->hasCorrectedAssumptionByOppositeQuestion() ||

													// Specification not collected with itself
													( !isSpecificationCollectedWithItself &&

													( !hasCurrentSpecificationCompoundCollection ||
													!hasGeneralizationWordActiveQuestionWithCompoundCollection ||
													isExclusiveUserSpecification ||
													foundPrimarySpecificationItem->hasSpecificationCollection() ||

													( !hasGeneralizationWordFeminineProperNameEnding &&
													isUserSpecificationWordCollectedWithItself ) ) ) ||

													// Specification collected with itself
													( isSpecificationCollectedWithItself &&

													( !isExclusiveCurrentSpecification ||
													isNegativeConclusion ||

													( foundPrimarySpecificationItem->hasSpecificationNonCompoundCollection() &&

													( !hasCurrentSpecificationCompoundCollection ||
													foundPrimarySpecificationItem->hasRelationContext() ) ) ) ) ) )
														{
														if( askSpecificationSubstitutionQuestionOrDrawNegativeConclusion( isArchivedAssignment, isExclusiveSpecification, isNegativeConclusion, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, foundPrimarySpecificationItem, currentSpecificationItem, generalizationWordItem, specificationWordItem ) != RESULT_OK )
															return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a specification substitution question or to draw a negative conclusion in generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
														}
													}
												else
													{
													// Make an assumption or draw a conclusion
													if( isAssumption ||
													currentSpecificationItem->hasAssumptionLevel() )
														{
														alternativeAnotherPrimarySpecificationItem = NULL;
														primarySpecificationItem = NULL;

														if( isSpecificationCollectedWithItself )
															primarySpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( isArchivedAssignment, isArchivedAssignment, false, false, NO_QUESTION_PARAMETER, specificationWordItem );
														else
															{
															primarySpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, isArchivedAssignment, false, false, false, specificationWordItem, NULL );

															if( primarySpecificationItem == NULL ||
															primarySpecificationItem ->isUserAssignment() )
																primarySpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( isArchivedAssignment, isArchivedAssignment, false, false, NO_QUESTION_PARAMETER, NO_CONTEXT_NR, specificationWordItem );
															}

														if( primarySpecificationItem != NULL )
															{
															alternativeJustificationSpecificationItem = ( hasRelationWord || isSpecificationCollectedWithItself ? adminItem_->alternativeJustificationSpecificationItem( hasGeneralizationWordFeminineProperNameEnding, true, false, false, false, userSpecificationItem, generalizationWordItem, specificationWordItem ) : NULL );
															existingSpecificationItem = ( hasRelationWord ? generalizationWordItem->firstAssignmentOrSpecificationItem( isArchivedAssignment, isArchivedAssignment, false, false, NO_QUESTION_PARAMETER, currentSpecificationWordItem ) : NULL );

															if( ( isSpecificationCollectedWithItself &&
															primarySpecificationItem->isHiddenSpecification() ) ||

															( existingSpecificationItem != NULL &&
															alternativeJustificationSpecificationItem != NULL &&
															alternativeJustificationSpecificationItem->specificationWordItem() == specificationWordItem &&

															( ( isSpecificationCollectedWithItself &&
															existingSpecificationItem->hasPrimarySpecificationJustification( primarySpecificationItem ) ) ||

															( !isSpecificationCollectedWithItself &&
															!existingSpecificationItem->hasPrimarySpecificationJustification( alternativeJustificationSpecificationItem ) &&

															( generalizationWordItem->isUserGeneralizationWord ||
															!specificationWordItem->isCollectionCollectedWithItself( alternativeJustificationSpecificationItem->specificationCollectionNr() ) ) ) ) ) )
																{
																if( alternativeJustificationSpecificationItem != NULL )
																	alternativeAnotherPrimarySpecificationItem = adminItem_->alternativeAnotherPrimarySpecificationItem();

																if( isSpecificationCollectedWithItself &&
																generalizationWordItem->isUserRelationWord &&
																alternativeAnotherPrimarySpecificationItem != NULL &&
																existingSpecificationItem != NULL &&
																userSpecificationItem != NULL &&
																primarySpecificationItem->isHiddenSpecification() &&
																!userSpecificationItem->isArchivedAssignment() )
																	{
																	primarySpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, false, false, false, false, true, NO_QUESTION_PARAMETER, userSpecificationItem->specificationCollectionNr(), NO_CONTEXT_NR, userSpecificationItem->specificationWordItem() );

																	if( primarySpecificationItem != NULL &&
																	// Check if justification already exists
																	existingSpecificationItem->hasPrimarySpecificationJustification( primarySpecificationItem ) )
																		primarySpecificationItem = alternativeJustificationSpecificationItem;
																	}
																else
																	primarySpecificationItem = alternativeJustificationSpecificationItem;
																}
															else
																{
																if( userSpecificationItem != NULL &&
																userSpecificationItem->isPossessive() &&
																primarySpecificationItem->hasAssumptionLevel() )
																	{
																	existingSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( isArchivedAssignment, isArchivedAssignment, false, false, NO_QUESTION_PARAMETER, currentSpecificationWordItem );

																	if( existingSpecificationItem != NULL &&
																	existingSpecificationItem->olderJustificationItem( JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION ) != NULL )
																		// Skip if similar justification already exists, without relation word
																		primarySpecificationItem = NULL;
																	}
																}
															}

														if( primarySpecificationItem != NULL &&

														( currentSpecificationCollectionNr == NO_COLLECTION_NR ||
														primarySpecificationItem == foundPrimarySpecificationItem ) &&

														// Skip if already exists as user specification
														generalizationWordItem->firstUserSpecificationItem( false, false, currentSpecificationCollectionNr, NO_CONTEXT_NR, currentSpecificationWordItem ) == NULL )
															{
															// Make a compound specification substitution assumption, which is actually an uncertain conclusion
															// See design:	- paragraph 2.3.1 in the theory document or http://mafait.org/theory_2_3_1/,
															//				- paragraph 2.3.1.1 in the theory document or http://mafait.org/theory_2_3_1_1/
															if( ( specificationResult = adminItem_->addSelfGeneratedSpecification( false, isAssignment, isArchivedAssignment, false, isExclusiveSpecification, false, false, false, false, false, isSpecificationCollectedWithItself, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, WORD_TYPE_UNDEFINED, NO_COLLECTION_NR, generalizationContextNr, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, alternativeAnotherPrimarySpecificationItem, currentSpecificationItem, NULL, generalizationWordItem, currentSpecificationWordItem, NULL ) ).result == RESULT_OK )
																{
																if( specificationResult.createdSpecificationItem == NULL )
																	{
																	if( generalizationWordItem->replaceOrDeleteObsoleteJustification() != RESULT_OK )
																		return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace or delete an obsolete assumption justification item in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
																	}
																}
															else
																return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a suggestive question specification to generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );
															}
														}
													else
														{
														if( ( !isExclusiveSpecification &&
														bestMatchingRelationSpecificationItem == NULL &&

														( !isSpecificationCollectedWithItself ||

														( !isFoundPrimaryHiddenSpecification &&
														collectedWithItselfSpecificationItem != NULL &&
														collectedWithItselfSpecificationItem->hasCurrentCreationSentenceNr() ) ) &&

														// Skip if already exists as user specification
														generalizationWordItem->firstUserSpecificationItem( false, false, currentSpecificationCollectionNr, NO_CONTEXT_NR, currentSpecificationWordItem ) == NULL ) ||

														// Check if exclusive conclusion already exists
														( isExclusiveSpecification &&
														generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( true, isAssignment, isArchivedAssignment, false, false, true, NO_QUESTION_PARAMETER, currentSpecificationCollectionNr, NO_CONTEXT_NR, currentSpecificationWordItem ) == NULL ) )
															{
															primarySpecificationItem = generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, false, isArchivedAssignment, false, false, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, specificationWordItem );

															if( primarySpecificationItem != NULL &&
															primarySpecificationItem->isHiddenSpecification() &&
															( primarySpecificationItem = adminItem_->alternativeJustificationSpecificationItem( hasGeneralizationWordFeminineProperNameEnding, true, false, false, false, userSpecificationItem, generalizationWordItem, specificationWordItem ) ) != NULL )
																alternativeAnotherPrimarySpecificationItem = adminItem_->alternativeAnotherPrimarySpecificationItem();

															if( primarySpecificationItem != NULL )
																{
																if( !primarySpecificationItem->isPartOf() )
																	{
																	// Draw a compound specification substitution conclusion
																	// See design:	- paragraph 2.3.1 in the theory document or http://mafait.org/theory_2_3_1/,
																	//				- paragraph 2.3.1.1 in the theory document or http://mafait.org/theory_2_3_1_1/
																	if( ( specificationResult = adminItem_->addSelfGeneratedSpecification( false, isAssignment, isArchivedAssignment, primarySpecificationItem->isEveryGeneralization(), isExclusiveSpecification, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION, NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, currentSpecificationItem->specificationWordTypeNr(), WORD_TYPE_UNDEFINED, NO_COLLECTION_NR, currentSpecificationItem->generalizationContextNr(), currentSpecificationItem->specificationContextNr(), NO_CONTEXT_NR, primarySpecificationItem, NULL, currentSpecificationItem, NULL, generalizationWordItem, currentSpecificationWordItem, NULL ) ).result == RESULT_OK )
																		{
																		if( ( createdSpecificationItem = specificationResult.createdSpecificationItem ) != NULL )
																			{
																			if( hasCorrectedAssumptionByKnowledge )
																				{
																				if( ( specificationResult = generalizationWordItem->findRelatedSpecification( false, createdSpecificationItem ) ).result == RESULT_OK )
																					{
																					if( specificationResult.relatedSpecificationItem != NULL )
																						{
																						// Replace incorrect assumption
																						if( generalizationWordItem->replaceOrDeleteSpecification( specificationResult.relatedSpecificationItem, createdSpecificationItem ) != RESULT_OK )
																							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace a corrected specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
																						}
																					}
																				else
																					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find out if a specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\" is related to the created specification" );
																				}
																			else
																				{
																				if( !isGeneralizationProperName )
																					{
																					if( adminItem_->makeSpecificationSubstitutionPartOfAssumption( isArchivedAssignment, currentSpecificationItem->generalizationWordTypeNr(), specificationWordTypeNr, generalizationContextNr, specificationWordItem, currentSpecificationWordItem ) != RESULT_OK )
																						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a specification substitution 'part of' assumption in specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
																					}
																				}
																			}
																		}
																	else
																		return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a specification substitution conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );
																	}
																}
															else
																return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find a primary specification item of a conclusion or question" );
															}
														}
													}

												if( !commonVariables_->hasShownWarning &&
												currentSpecificationCollectionNr > NO_COLLECTION_NR &&

												( !isExclusiveCurrentSpecification ||

												( isDefinitionSpecification &&
												isExclusiveSpecification ) ) )
													{
													if( adminItem_->makeGeneralizationAssumptionByGeneralization( isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, generalizationWordItem, currentSpecificationWordItem ) != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a generalization assumption by generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and with specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );
													}
												}
											else
												{
												if( !isExclusiveSpecification &&
												isGeneralizationProperName &&
												currentSpecificationItem->isUserSpecification() )
													{
													if( generalizationWordItem->checkForSpecificationConflict( isArchivedAssignment, isExclusiveSpecification, false, false, specificationWordTypeNr, currentSpecificationCollectionNr, currentSpecificationItem->generalizationContextNr(), currentSpecificationItem->specificationContextNr(), currentSpecificationItem->relationContextNr(), currentSpecificationWordItem ) != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to check for a specification conflict in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
													}
												}
											}
										}
									}
								}
							while( ( currentSpecificationItem = currentSpecificationItem->nextSelectedSpecificationItem() ) != NULL );
							}
						}
					else
						return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );
					}
				else
					return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );
				}
			else
				return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word type isn't a noun" );
			}
		else
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word type isn't a reasoning type" );

		return RESULT_OK;
		}

	SpecificationResultType drawCompoundSpecificationSubstitutionConclusion( unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		SpecificationResultType specificationResult;
		GeneralizationItem *currentGeneralizationItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *relatedSpecificationItem;
		SpecificationItem *previousSpecificationItem = NULL;
		SpecificationItem *lastCreatedSpecificationItem = NULL;
		WordItem *currentGeneralizationWordItem;
		WordItem *compoundGeneralizationWordItem = NULL;
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawCompoundSpecificationSubstitutionConclusion";

		if( adminItem_->isSingularOrPluralNoun( specificationWordTypeNr ) )
			{
			if( specificationWordItem != NULL )
				{
				do	{
					if( ( currentGeneralizationItem = specificationWordItem->firstSpecificationGeneralizationItem( false ) ) != NULL )
						{
						previousSpecificationItem = lastCreatedSpecificationItem;

						do	{
							if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) != NULL )
								{
								if( ( foundSpecificationItem = currentGeneralizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, true, true, false, false, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ) != NULL )
									{
									if( drawSpecificationSubstitutionConclusionOrAskQuestion( foundSpecificationItem->hasAssumptionLevel(), foundSpecificationItem->isInactiveAssignment(), foundSpecificationItem->isArchivedAssignment(), foundSpecificationItem->isExclusiveSpecification(), NO_QUESTION_PARAMETER, foundSpecificationItem->generalizationWordTypeNr(), specificationWordTypeNr, WORD_TYPE_UNDEFINED, generalizationContextNr, specificationContextNr, currentGeneralizationWordItem, specificationWordItem, NULL ) == RESULT_OK )
										{
										if( !commonVariables_->hasShownWarning )
											{
											if( ( specificationResult = currentGeneralizationWordItem->findRelatedSpecification( false, foundSpecificationItem ) ).result == RESULT_OK )
												{
												if( ( relatedSpecificationItem = specificationResult.relatedSpecificationItem ) != NULL )
													{
													if( drawSpecificationSubstitutionConclusionOrAskQuestion( relatedSpecificationItem->hasAssumptionLevel(), relatedSpecificationItem->isInactiveAssignment(), relatedSpecificationItem->isArchivedAssignment(), relatedSpecificationItem->isExclusiveSpecification(), relatedSpecificationItem->questionParameter(), relatedSpecificationItem->generalizationWordTypeNr(), specificationWordTypeNr, WORD_TYPE_UNDEFINED, generalizationContextNr, specificationContextNr, currentGeneralizationWordItem, relatedSpecificationItem->specificationWordItem(), NULL ) == RESULT_OK )
														compoundGeneralizationWordItem = currentGeneralizationWordItem;
													else
														adminItem_->addError( functionNameString, moduleNameString_, "I failed to find a specification substitution conclusion or question for generalization word \"", currentGeneralizationWordItem->anyWordTypeString(), "\" and specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
													}
												}
											else
												adminItem_->addError( functionNameString, moduleNameString_, "I failed to find out if a specification in generalization word \"", currentGeneralizationWordItem->anyWordTypeString(), "\" is related to the found specification" );
											}
										}
									else
										adminItem_->addError( functionNameString, moduleNameString_, "I failed to find a specification substitution conclusion or question for generalization word \"", currentGeneralizationWordItem->anyWordTypeString(), "\" and specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
									}
								}
							else
								adminItem_->startError( functionNameString, moduleNameString_, "I have found an undefined generalization word" );
							}
						while( commonVariables_->result == RESULT_OK &&
						!commonVariables_->hasShownWarning &&
						( currentGeneralizationItem = currentGeneralizationItem->nextSpecificationGeneralizationItem() ) != NULL );
						}
					}
				while( commonVariables_->result == RESULT_OK &&
				!commonVariables_->hasShownWarning &&
				// Do until no more conclusions or questions are created
				previousSpecificationItem != NULL );
				}
			else
				adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );
			}
		else
			adminItem_->startError( functionNameString, moduleNameString_, "The given specification word type isn't a noun" );

		specificationResult.compoundGeneralizationWordItem = compoundGeneralizationWordItem;
		specificationResult.result = commonVariables_->result;
		return specificationResult;
		}
	};

/*************************************************************************
 *	"He forgives all my sins and heals all my diseases.
 *	He redeems me from death and crowns me with love and
 *	tender mercies. He fills my life with good things.
 *	My youth is renewed like the eagle's!" (Psalm 103:3-5)
 *************************************************************************/
