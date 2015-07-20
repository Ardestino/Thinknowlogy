/*
 *	Class:			AdminContext
 *	Supports class:	AdminItem
 *	Purpose:		To create context structures
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

class AdminContext
	{
	// Private constructible variables

	private AdminItem adminItem_;
	private String moduleNameString_;


	// Private methods

	private ContextResultType findPossessiveReversibleConclusionRelationContextOfInvolvedWords( boolean isPossessive, int nContextRelations, int relationContextNr, SpecificationItem foundSpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		ContextResultType contextResult = new ContextResultType();
		boolean hasFoundAllRelationWords;
		boolean isFoundRelationContext;
		int currentRelationContextNr;
		int nContextWords;
		GeneralizationItem currentGeneralizationItem;
		SpecificationItem currentSpecificationItem;
		WordItem currentGeneralizationWordItem;

		if( relationContextNr > Constants.NO_CONTEXT_NR )
			{
			if( generalizationWordItem != null )
				{
				if( specificationWordItem != null )
					{
					if( ( currentGeneralizationItem = generalizationWordItem.firstRelationGeneralizationItem() ) != null )
						{
						do	{
							if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) != null )
								{
								if( ( currentSpecificationItem = currentGeneralizationWordItem.firstNonQuestionSpecificationItem() ) != null )
									{
									do	{
										if( currentSpecificationItem.hasRelationContext() &&
										currentSpecificationItem.isRelatedSpecification( false, false, isPossessive, specificationWordItem ) )
											{
											currentRelationContextNr = currentSpecificationItem.relationContextNr();
											nContextWords = adminItem_.nContextWordsInAllWords( currentRelationContextNr, specificationWordItem );

											// This relation word will be the last one
											hasFoundAllRelationWords = ( nContextWords + 1 == nContextRelations );
											isFoundRelationContext = ( currentRelationContextNr == relationContextNr );

											if( currentSpecificationItem.isPossessiveReversibleConclusion() )
												{
												if( ( isPossessive &&
												!isFoundRelationContext ) ||

												( !isPossessive &&
												isFoundRelationContext &&
												!hasFoundAllRelationWords ) )
													{
													if( hasFoundAllRelationWords )
														{
														contextResult.contextNr = relationContextNr;
														contextResult.conclusionSpecificationItem = currentSpecificationItem;
														}
													else
														contextResult.contextNr = currentRelationContextNr;
													}
												}
											else
												{
												if( !isPossessive &&
												!isFoundRelationContext &&
												nContextWords == nContextRelations )
													{
													contextResult.contextNr = currentRelationContextNr;
													contextResult.conclusionSpecificationItem = foundSpecificationItem;
													}
												}
											}
										}
									while( contextResult.contextNr == Constants.NO_CONTEXT_NR &&
									( currentSpecificationItem = currentSpecificationItem.nextSelectedSpecificationItem() ) != null );
									}
								}
							else
								adminItem_.startErrorInItem( 1, moduleNameString_, "I've found an undefined generalization word" );
							}
						while( CommonVariables.result == Constants.RESULT_OK &&
						( currentGeneralizationItem = currentGeneralizationItem.nextRelationGeneralizationItem() ) != null );
						}
					}
				else
					adminItem_.startErrorInItem( 1, moduleNameString_, "The given specification word item is undefined" );
				}
			else
				adminItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );
			}
		else
			adminItem_.startErrorInItem( 1, moduleNameString_, "The given relation context number is undefined" );

		contextResult.result = CommonVariables.result;
		return contextResult;
		}

	private ContextItem firstContextItemInAllWords( boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, WordItem specificationWordItem )
		{
		boolean hasFoundCurrentContext = false;
		boolean isPossessiveUserSpecification = adminItem_.isPossessiveUserSpecification();
		ContextItem currentContextItem;
		ContextItem foundContextItem = null;
		WordItem currentWordItem;

		if( specificationWordItem != null )
			{
			if( ( currentWordItem = CommonVariables.lastPredefinedWordItem ) != null )
				{
				// Do for all words
				do	{
					if( ( currentContextItem = currentWordItem.contextItem( specificationWordItem ) ) != null )
						{
						if( isPossessive &&
						!isPossessiveUserSpecification )
							{
							if( foundContextItem == null )
								foundContextItem = currentContextItem;

							if( !currentContextItem.isOlderItem() )
								hasFoundCurrentContext = true;
							}
						else
							return currentContextItem;
						}
					}
				while( !hasFoundCurrentContext &&
				( currentWordItem = currentWordItem.nextWordItem() ) != null );
				}

			if( !hasFoundCurrentContext &&
			foundContextItem != null &&
			( currentWordItem = CommonVariables.lastPredefinedWordItem ) != null )
				{
				// Do for all words
				do	{
					if( !currentWordItem.isUserGeneralizationWord &&
					!currentWordItem.isUserRelationWord &&
					currentWordItem.isProperName() &&
					currentWordItem.bestMatchingRelationContextNrSpecificationItem( true, isArchivedAssignment, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, foundContextItem.contextNr(), specificationWordItem ) != null )
						// Found context is already in use by other words
						return null;
					}
				while( ( currentWordItem = currentWordItem.nextWordItem() ) != null );
				}
			}

		return foundContextItem;
		}


	// Constructor / deconstructor

	protected AdminContext( AdminItem adminItem )
		{
		String errorString = null;

		adminItem_ = adminItem;
		moduleNameString_ = this.getClass().getName();

		if( adminItem_ == null )
			errorString = "The given admin is undefined";

		if( errorString != null )
			{
			if( adminItem_ != null )
				adminItem_.startSystemErrorInItem( 1, moduleNameString_, errorString );
			else
				{
				CommonVariables.result = Constants.RESULT_SYSTEM_ERROR;
				Console.addError( "\nClass:" + moduleNameString_ + "\nMethod:\t" + Constants.PRESENTATION_ERROR_CONSTRUCTOR_METHOD_NAME + "\nError:\t\t" + errorString + ".\n" );
				}
			}
		}


	// Protected context methods

	protected ContextResultType getRelationContext( boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, boolean isQuestion, boolean isUserSentence, short specificationWordTypeNr, int nContextRelations, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem, ReadItem startRelationReadItem )
		{
		ContextResultType contextResult = new ContextResultType();
		boolean hasFoundRelationContext;
		boolean hasFoundRelationWordInThisList;
		boolean hasSameNumberOrMoreRelationWords;
		boolean isSkippingThisContext = false;
		int currentRelationContextNr;
		int foundRelationContextNr;
		ContextItem currentRelationContextItem;
		ReadItem relationWordReadItem = null;
		SpecificationItem foundSpecificationItem;
		SpecificationItem existingSpecificationItem = null;
		WordItem currentWordItem;
		WordItem currentRelationWordItem = relationWordItem;

		if( generalizationWordItem != null )
			{
			if( specificationWordItem != null )
				{
				if( startRelationReadItem != null )
					{
					if( ( relationWordReadItem = startRelationReadItem.firstRelationWordReadItem() ) != null )
						currentRelationWordItem = relationWordReadItem.readWordItem();
					else
						adminItem_.startErrorInItem( 1, moduleNameString_, "The read word of the first relation word is undefined" );
					}

				if( CommonVariables.result == Constants.RESULT_OK )
					{
					if( currentRelationWordItem != null )
						{
						if( ( currentRelationContextItem = currentRelationWordItem.firstActiveContextItem() ) != null )
							{
							// Do for all relation context items in the first relation context word
							do	{
								currentRelationContextNr = currentRelationContextItem.contextNr();

								if( currentRelationWordItem.hasContextInWord( currentRelationContextNr, specificationWordItem ) )
									{
									if( ( currentWordItem = CommonVariables.lastPredefinedWordItem ) != null )
										{
										hasFoundRelationWordInThisList = false;
										isSkippingThisContext = false;

										// Do for all words, either in the current relation list or outside this list
										do	{
											foundSpecificationItem = ( isUserSentence ? null : generalizationWordItem.firstActiveAssignmentOrSpecificationItem( true, false, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationWordItem ) );

											if( foundSpecificationItem == null ||
											!foundSpecificationItem.isSelfGeneratedConclusion() )
												{
												if( relationWordReadItem != null )
													hasFoundRelationWordInThisList = relationWordReadItem.hasFoundRelationWordInThisList( currentWordItem );

												hasFoundRelationContext = currentWordItem.hasContextInWord( currentRelationContextNr, specificationWordItem );

												// Word is one of the relation words in this list, but doesn't have current context
												if( ( !hasFoundRelationContext &&
												hasFoundRelationWordInThisList ) ||

												// Word is in not current list of relation words, but has current context
												( hasFoundRelationContext &&
												!hasFoundRelationWordInThisList ) )
													isSkippingThisContext = true;
												}
											}
										while( !isSkippingThisContext &&
										( currentWordItem = currentWordItem.nextWordItem() ) != null );

										// The relation words in the list contain this context exclusively. (So, no other words)
										if( !isSkippingThisContext )
											{
											existingSpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( isArchivedAssignment, isArchivedAssignment, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, Constants.NO_CONTEXT_NR, specificationWordItem );

											if( existingSpecificationItem != null &&
											existingSpecificationItem.hasRelationContext() &&
											existingSpecificationItem.isUserSpecification() &&
											existingSpecificationItem.relationContextNr() != currentRelationContextNr &&

											// Skip mix-up singular/plural noun
											( specificationWordTypeNr != Constants.WORD_TYPE_NOUN_SINGULAR ||
											!existingSpecificationItem.isSpecificationPluralNoun() ) )
												contextResult.contextNr = existingSpecificationItem.relationContextNr();
											else
												contextResult.contextNr = currentRelationContextNr;
											}
										}
									else
										adminItem_.startErrorInItem( 1, moduleNameString_, "The first word item is undefined" );
									}
								else
									{
									if( isUserSentence &&
									!currentRelationContextItem.isOlderItem() )
										contextResult.contextNr = currentRelationContextNr;
									}
								}
							while( CommonVariables.result == Constants.RESULT_OK &&
							contextResult.contextNr == Constants.NO_CONTEXT_NR &&
							( currentRelationContextItem = currentRelationContextItem.nextContextItem() ) != null );
							}

						if( CommonVariables.result == Constants.RESULT_OK &&
						!isQuestion &&
						contextResult.contextNr == Constants.NO_CONTEXT_NR &&
						( foundSpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, isArchivedAssignment, false, isPossessive, isPossessive, specificationWordItem, null ) ) != null )
							{
							if( ( foundRelationContextNr = foundSpecificationItem.relationContextNr() ) > Constants.NO_CONTEXT_NR )
								{
								hasSameNumberOrMoreRelationWords = ( adminItem_.nContextWordsInAllWords( foundRelationContextNr, specificationWordItem ) >= CommonVariables.nUserRelationWords );

								if( hasSameNumberOrMoreRelationWords ||
								generalizationWordItem.isUserRelationWord )
									{
									contextResult.contextNr = foundRelationContextNr;

									if( foundSpecificationItem.isExclusiveSpecification() )
										// Already existing static (exclusive) semantic ambiguity
										contextResult.isExclusiveContext = true;
									else
										{
										if( !foundSpecificationItem.isActiveAssignment() )
											{
											if( relationWordItem == null )
												{
												if( isSkippingThisContext )
													// Didn't confirm all relation words
													// Create new context number
													contextResult.contextNr = Constants.NO_CONTEXT_NR;
												else
													{
													// Static (exclusive) semantic ambiguity
													if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_SENTENCE_NOTIFICATION_I_NOTICED_SEMANTIC_AMBIGUITY_START, generalizationWordItem.anyWordTypeString(), Constants.INTERFACE_SENTENCE_NOTIFICATION_STATIC_SEMANTIC_AMBIGUITY_END ) == Constants.RESULT_OK )
														contextResult.isExclusiveContext = true;
													else
														adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to write an interface notification" );
													}
												}
											else
												{
												if( hasSameNumberOrMoreRelationWords )
													{
													// Try to find the relation context of a possessive reversible conclusion
													if( ( contextResult = findPossessiveReversibleConclusionRelationContextOfInvolvedWords( isPossessive, nContextRelations, contextResult.contextNr, foundSpecificationItem, relationWordItem, specificationWordItem ) ).result != Constants.RESULT_OK )
														adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to find a possessive reversible conclusion relation context of involved words" );
													}
												}
											}
										}
									}
								}
							}

						if( CommonVariables.result == Constants.RESULT_OK &&
						contextResult.contextNr == Constants.NO_CONTEXT_NR )
							{
							if( ( contextResult.contextNr = adminItem_.highestContextNrInAllWords() ) < Constants.MAX_CONTEXT_NR )
								// Create new context number
								contextResult.contextNr++;
							else
								adminItem_.startSystemErrorInItem( 1, moduleNameString_, "Context number overflow" );
							}
						}
					else
						adminItem_.startErrorInItem( 1, moduleNameString_, "I couldn't find any relation word" );
					}
				}
			else
				adminItem_.startErrorInItem( 1, moduleNameString_, "The given specification word item is undefined" );
			}
		else
			adminItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );

		contextResult.result = CommonVariables.result;
		return contextResult;
		}

	protected ContextResultType getSpecificationRelationContext( boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isCompoundCollectionCollectedWithItself, boolean isNegative, boolean isPossessive, boolean isSelfGeneratedAssumption, int specificationCollectionNr, int relationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		ContextResultType contextResult = new ContextResultType();
		boolean isSpecificationCollectedWithItself;
		int foundRelationContextNr = Constants.NO_CONTEXT_NR;
		ContextItem foundContextItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem existingSpecificationItem = null;

		if( generalizationWordItem != null )
			{
			if( specificationWordItem != null )
				{
				if( relationWordItem != null )
					{
					// Try to find relation context with same number of relation words as in the user sentence
					if( ( foundContextItem = relationWordItem.contextItem( isCompoundCollectionCollectedWithItself, CommonVariables.nUserRelationWords, specificationWordItem ) ) == null )
						{
						if( isCompoundCollectionCollectedWithItself )
							existingSpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, isArchivedAssignment, false, isArchivedAssignment, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationCollectionNr, Constants.NO_CONTEXT_NR, specificationWordItem );
						else
							existingSpecificationItem = generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( false, isArchivedAssignment, isArchivedAssignment, isNegative, isPossessive, specificationCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, specificationWordItem );

						isSpecificationCollectedWithItself = specificationWordItem.isNounWordCollectedWithItself();

						if( existingSpecificationItem == null &&
						contextResult.contextNr == Constants.NO_CONTEXT_NR &&
						CommonVariables.nUserRelationWords > 1 &&

						( !isSpecificationCollectedWithItself ||
						CommonVariables.nUserGeneralizationWords > 1 ||
						generalizationWordItem.isFemale() ||
						!relationWordItem.isOlderItem() ) )
							contextResult.contextNr = relationWordItem.contextNr( isCompoundCollectionCollectedWithItself, specificationWordItem );

						// Not found yet
						if( contextResult.contextNr == Constants.NO_CONTEXT_NR &&
						( foundContextItem = firstContextItemInAllWords( isArchivedAssignment, isNegative, isPossessive, specificationWordItem ) ) != null )
							{
							// Find specification with found context word as relation word
							if( ( foundSpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( isArchivedAssignment, isArchivedAssignment, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, Constants.NO_CONTEXT_NR, specificationWordItem ) ) != null )
								{
								if( existingSpecificationItem != null &&

								( !foundSpecificationItem.hasRelationContext() ||

								( isSpecificationCollectedWithItself &&
								existingSpecificationItem.isHiddenSpecification() ) ) )
									foundRelationContextNr = existingSpecificationItem.relationContextNr();
								else
									{
									if( ( !isSpecificationCollectedWithItself ||
									specificationCollectionNr == Constants.NO_COLLECTION_NR ||
									foundSpecificationItem.isUserSpecification() ||
									foundSpecificationItem.specificationCollectionNr() == specificationCollectionNr ) &&

									foundSpecificationItem.isArchivedAssignment() == isArchivedAssignment )
										foundRelationContextNr = foundSpecificationItem.relationContextNr();
									}
								}

							// Cross-collected afterwards
							if( foundRelationContextNr > Constants.NO_CONTEXT_NR &&
							contextResult.contextNr == Constants.NO_CONTEXT_NR &&
							existingSpecificationItem != null )
								{
								if( generalizationWordItem.hasConfirmedSpecification() )
									{
									if( existingSpecificationItem.isSelfGeneratedAssumption() )
										// Found reversed
										contextResult.contextNr = existingSpecificationItem.relationContextNr();
									}
								else
									{
									if( isSpecificationCollectedWithItself ||
									existingSpecificationItem.isConcludedAssumption() ||
									// Feminine word of a specification word collected with itself
									existingSpecificationItem.hasSpecificationCompoundCollection() )
										contextResult.contextNr = foundRelationContextNr;
									else
										{
										// Skip on difference in assumption / conclusion
										if( existingSpecificationItem.isSelfGeneratedAssumption() == isSelfGeneratedAssumption )
											contextResult.contextNr = ( relationWordItem.hasAnsweredQuestion() ? foundRelationContextNr : existingSpecificationItem.relationContextNr() );
										}
									}
								}
							}
						}
					else
						{
						contextResult.contextNr = foundContextItem.contextNr();

						if( isAssignment &&
						// Has no relation context collection
						adminItem_.collectionNrInAllWords( relationContextNr ) == Constants.NO_COLLECTION_NR &&
						// Check if assignment already exists
						generalizationWordItem.firstNonQuestionAssignmentItem( ( !isInactiveAssignment && !isArchivedAssignment ), isInactiveAssignment, isArchivedAssignment, isNegative, isPossessive, contextResult.contextNr, specificationWordItem ) == null &&
						// Check also recently replaced assignments
						generalizationWordItem.firstRecentlyReplacedAssignmentItem( isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, contextResult.contextNr, specificationWordItem ) == null )
							{
							// Dynamic semantic ambiguity
							if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_SENTENCE_NOTIFICATION_I_NOTICED_SEMANTIC_AMBIGUITY_START, relationWordItem.anyWordTypeString(), Constants.INTERFACE_SENTENCE_NOTIFICATION_DYNAMIC_SEMANTIC_AMBIGUITY_END ) == Constants.RESULT_OK )
								{
								contextResult.isAmbiguousRelationContext = true;
								// Create new context number
								contextResult.contextNr = Constants.NO_CONTEXT_NR;
								}
							else
								adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to write an interface notification" );
							}
						}

					if( CommonVariables.result == Constants.RESULT_OK &&
					contextResult.contextNr == Constants.NO_CONTEXT_NR )
						{
						if( ( contextResult.contextNr = adminItem_.highestContextNrInAllWords() ) < Constants.MAX_CONTEXT_NR )
							// Create new context number
							contextResult.contextNr++;
						else
							adminItem_.startSystemErrorInItem( 1, moduleNameString_, "Context number overflow" );
						}
					}
				else
					adminItem_.startErrorInItem( 1, moduleNameString_, "The given relation word item is undefined" );
				}
			else
				adminItem_.startErrorInItem( 1, moduleNameString_, "The given specification word item is undefined" );
			}
		else
			adminItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );

		contextResult.result = CommonVariables.result;
		return contextResult;
		}
	};

/*************************************************************************
 *	"Praise the Lord!
 *	How joyful are those who fear the Lord
 *	and delight in obeying his commands." (Psalm 112:1)
 *************************************************************************/