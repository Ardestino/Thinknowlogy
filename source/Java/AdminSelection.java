/*	Class:			AdminSelection
 *	Supports class:	AdminItem
 *	Purpose:		To process selections
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

class AdminSelection
	{
	// Private constructed variables

	private SelectionItem firstSelectionItem_;

	private AdminItem adminItem_;
	private String moduleNameString_;


	// Private methods

	private byte removeDuplicateSelection()
		{
		SelectionList actionList;
		SelectionList alternativeList;
		SelectionList conditionList;

		if( ( conditionList = adminItem_.conditionList ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The condition list isn't created yet" );

		if( ( actionList = adminItem_.actionList ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The action list isn't created yet" );

		if( conditionList.deleteActiveItemsWithCurrentSentenceNr() != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to remove the condition of a selection" );

		if( actionList.deleteActiveItemsWithCurrentSentenceNr() != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to remove the action of a selection" );

		if( ( alternativeList = adminItem_.alternativeList ) != null )
			{
			if( alternativeList.deleteActiveItemsWithCurrentSentenceNr() != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to remove the alternative of a selection" );
			}

		return Constants.RESULT_OK;
		}

	private SelectionItem firstCondition()
		{
		SelectionList conditionList;

		if( ( conditionList = adminItem_.conditionList ) != null )
			return conditionList.firstActiveSelectionItem();

		return null;
		}


	// Constructor

	protected AdminSelection( AdminItem adminItem )
		{
		String errorString = null;

		firstSelectionItem_ = null;

		adminItem_ = adminItem;
		moduleNameString_ = this.getClass().getName();

		if( adminItem_ == null )
			errorString = "The given admin is undefined";

		if( errorString != null )
			{
			if( adminItem_ != null )
				adminItem_.startSystemError( 1, moduleNameString_, errorString );
			else
				{
				CommonVariables.result = Constants.RESULT_SYSTEM_ERROR;
				Console.addError( "\nClass:" + moduleNameString_ + "\nMethod:\t" + Constants.PRESENTATION_ERROR_CONSTRUCTOR_METHOD_NAME + "\nError:\t\t" + errorString + ".\n" );
				}
			}
		}


	// Protected methods

	protected byte checkForDuplicateSelection()
		{
		SelectionResultType selectionResult;
		boolean hasFoundDuplicateSelection = false;
		int duplicateConditionSentenceNr;
		SelectionList actionList;
		SelectionList alternativeList;
		SelectionList conditionList;

		if( ( conditionList = adminItem_.conditionList ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The condition list isn't created yet" );

		if( ( actionList = adminItem_.actionList ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The action list isn't created yet" );

		if( ( selectionResult = conditionList.checkDuplicateCondition() ).result != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to check if the condition selection part is duplicate" );

		if( ( duplicateConditionSentenceNr = selectionResult.duplicateConditionSentenceNr ) > Constants.NO_SENTENCE_NR )
			{
			if( ( selectionResult = actionList.checkDuplicateSelectionPart( duplicateConditionSentenceNr ) ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to check if the action selection part is duplicate" );

			if( selectionResult.hasFoundDuplicateSelection )
				{
				if( ( alternativeList = adminItem_.alternativeList ) == null )
					hasFoundDuplicateSelection = true;
				else
					{
					if( ( selectionResult = alternativeList.checkDuplicateSelectionPart( duplicateConditionSentenceNr ) ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to check if the alternative selection part is duplicate" );

					if( selectionResult.hasFoundDuplicateSelection )
						hasFoundDuplicateSelection = true;
					}
				}

			if( hasFoundDuplicateSelection )
				{
				if( removeDuplicateSelection() != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to remove a duplicate selection" );
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte createSelectionPart( boolean isAction, boolean isAssignedOrClear, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isFirstComparisonPart, boolean isNewStart, boolean isNegative, boolean isPossessive, boolean isSpecificationGeneralization, boolean isUniqueUserRelation, boolean isValueSpecification, short assumptionLevel, short selectionLevel, short selectionListNr, short imperativeVerbParameter, short prepositionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int nContextRelations, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem, String specificationString )
		{
		SelectionResultType selectionResult;

		if( generalizationWordItem == null &&
		specificationString == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word or specification string is undefined" );

		switch( selectionListNr )
			{
			case Constants.ADMIN_CONDITION_LIST:
				if( adminItem_.conditionList == null )
					{
					// Create list
					if( ( adminItem_.conditionList = new SelectionList( Constants.ADMIN_CONDITION_LIST_SYMBOL, adminItem_ ) ) == null )
						return adminItem_.startError( 1, moduleNameString_, "I failed to create an admin condition list" );

					CommonVariables.adminConditionList = adminItem_.conditionList;
					adminItem_.adminListArray[Constants.ADMIN_CONDITION_LIST] = adminItem_.conditionList;
					}

				if( ( selectionResult = adminItem_.conditionList.createSelectionItem( isAction, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isFirstComparisonPart, isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, assumptionLevel, selectionLevel, imperativeVerbParameter, prepositionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, generalizationWordItem, specificationWordItem, relationWordItem, specificationString ) ).result != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to create a copy of a temporary generalization noun selection item in the admin condition list" );

				if( firstSelectionItem_ == null )
					firstSelectionItem_ = selectionResult.lastCreatedSelectionItem;

				break;

			case Constants.ADMIN_ACTION_LIST:
				if( adminItem_.actionList == null )
					{
					// Create list
					if( ( adminItem_.actionList = new SelectionList( Constants.ADMIN_ACTION_LIST_SYMBOL, adminItem_ ) ) == null )
						return adminItem_.startError( 1, moduleNameString_, "I failed to create an admin action list" );

					CommonVariables.adminActionList = adminItem_.actionList;
					adminItem_.adminListArray[Constants.ADMIN_ACTION_LIST] = adminItem_.actionList;
					}

				if( ( selectionResult = adminItem_.actionList.createSelectionItem( false, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isFirstComparisonPart, isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, assumptionLevel, selectionLevel, imperativeVerbParameter, prepositionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, generalizationWordItem, specificationWordItem, relationWordItem, specificationString ) ).result != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to create a copy of a temporary generalization noun selection item in the admin action list" );

				if( firstSelectionItem_ == null )
					firstSelectionItem_ = selectionResult.lastCreatedSelectionItem;

				break;

			case Constants.ADMIN_ALTERNATIVE_LIST:
				if( adminItem_.alternativeList == null )
					{
					// Create list
					if( ( adminItem_.alternativeList = new SelectionList( Constants.ADMIN_ALTERNATIVE_LIST_SYMBOL, adminItem_ ) ) == null )
						return adminItem_.startError( 1, moduleNameString_, "I failed to create an admin alternative list" );

					CommonVariables.adminAlternativeList = adminItem_.alternativeList;
					adminItem_.adminListArray[Constants.ADMIN_ALTERNATIVE_LIST] = adminItem_.alternativeList;
					}

				if( ( selectionResult = adminItem_.alternativeList.createSelectionItem( false, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isFirstComparisonPart, isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, assumptionLevel, selectionLevel, imperativeVerbParameter, prepositionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, generalizationWordItem, specificationWordItem, relationWordItem, specificationString ) ).result != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to create a copy of a temporary generalization noun selection item in the admin alternative list" );

				if( firstSelectionItem_ == null )
					firstSelectionItem_ = selectionResult.lastCreatedSelectionItem;

				break;

			default:
				return adminItem_.startError( 1, moduleNameString_, "The given list number is invalid: " + selectionListNr );
			}

		return Constants.RESULT_OK;
		}

	protected byte executeSelection( int endSolveProgress, SelectionItem actionSelectionItem )
		{
		SelectionResultType selectionResult;
		boolean hasDoneLastExecution;
		boolean isInitializeVariables;
		boolean isNewStart;
		boolean isSatisfied;
		boolean isWaitingForNewStart;
		boolean isWaitingForNewLevel;
		boolean isWaitingForExecution;
		short executionLevel;
		short executionListNr;
		short selectionLevel;
		short nSelectionExecutions = 0;
		int executionSentenceNr;
		WordItem generalizationWordItem;
		SelectionItem conditionSelectionItem;
		SelectionItem executionSelectionItem;
		SelectionList actionList;
		SelectionList alternativeList;

		do	{
			hasDoneLastExecution = false;
			CommonVariables.isAssignmentChanged = false;

			if( ( conditionSelectionItem = firstCondition() ) != null )
				{
				isSatisfied = false;
				isWaitingForNewStart = false;
				isWaitingForNewLevel = false;
				isWaitingForExecution = false;
				executionLevel = conditionSelectionItem.selectionLevel();
				executionSentenceNr = conditionSelectionItem.activeSentenceNr();

				do	{
					if( conditionSelectionItem == null ||
					executionSentenceNr != conditionSelectionItem.activeSentenceNr() )
						{
						executionListNr = ( isSatisfied ? Constants.ADMIN_ACTION_LIST : Constants.ADMIN_ALTERNATIVE_LIST );
						executionSelectionItem = null;

						if( isSatisfied )
							{
							if( ( actionList = adminItem_.actionList ) != null )
								executionSelectionItem = actionList.executionStartEntry( executionLevel, executionSentenceNr );
							}
						else
							{
							if( ( alternativeList = adminItem_.alternativeList ) != null )
								executionSelectionItem = alternativeList.executionStartEntry( executionLevel, executionSentenceNr );
							}

						if( executionSelectionItem != null )
							{
							isInitializeVariables = true;

							do	{
								if( adminItem_.executeImperative( isInitializeVariables, executionListNr, executionSelectionItem.imperativeVerbParameter(), Constants.NO_WORD_PARAMETER, executionSelectionItem.specificationWordTypeNr(), endSolveProgress, executionSelectionItem.specificationString(), executionSelectionItem.generalizationWordItem(), executionSelectionItem.specificationWordItem(), null, null, executionSelectionItem, actionSelectionItem ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to execute an imperative" );

								isInitializeVariables = false;
								}
							while( !adminItem_.hasRequestedRestart() &&
							!CommonVariables.hasDisplayedWarning &&
							( executionSelectionItem = executionSelectionItem.nextExecutionItem( executionLevel, executionSentenceNr ) ) != null );
							}

						// Found new condition
						if( conditionSelectionItem != null )
							{
							isSatisfied = false;
							isWaitingForNewStart = false;
							isWaitingForNewLevel = false;
							isWaitingForExecution = false;
							executionLevel = conditionSelectionItem.selectionLevel();
							}
						}

					if( conditionSelectionItem == null )
						hasDoneLastExecution = true;
					else
						{
						isNewStart = conditionSelectionItem.isNewStart();
						selectionLevel = conditionSelectionItem.selectionLevel();

						if( isNewStart &&
						isWaitingForNewStart &&
						// Found new start
						executionLevel == selectionLevel )
							isWaitingForNewStart = false;

						if( isWaitingForNewLevel &&
						// Found new level
						executionLevel != selectionLevel )
							isWaitingForNewLevel = false;

						if( !isWaitingForNewStart &&
						!isWaitingForNewLevel &&
						!isWaitingForExecution )
							{
							if( !isNewStart &&
							!isSatisfied &&
							executionLevel == selectionLevel )
								// Skip checking of this condition part and wait for a new start to come on this level
								isWaitingForNewStart = true;
							else
								{
								if( isNewStart &&
								isSatisfied &&
								executionLevel == selectionLevel )
									// Skip checking of this condition part and wait for a new level to come
									isWaitingForNewLevel = true;
								else
									{
									if( executionLevel != selectionLevel &&
									isSatisfied != conditionSelectionItem.isAction() )
										// Skip checking of this condition and wait for the next condition sentence number to come
										isWaitingForExecution = true;
									else
										{
										generalizationWordItem = conditionSelectionItem.generalizationWordItem();

										if( generalizationWordItem == null )
											return adminItem_.startError( 1, moduleNameString_, "I have found an undefined condition word" );

										if( ( selectionResult = generalizationWordItem.checkSelectionCondition( conditionSelectionItem ) ).result != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to check the condition of a selection in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

										isSatisfied = selectionResult.isConditionSatisfied;
										executionLevel = selectionLevel;
										executionSentenceNr = conditionSelectionItem.activeSentenceNr();
										}
									}
								}
							}

						conditionSelectionItem = conditionSelectionItem.nextSelectionItem();
						}
					}
				while( !hasDoneLastExecution &&
				!adminItem_.hasRequestedRestart() &&
				!CommonVariables.hasDisplayedWarning );
				}
			}
		while( !adminItem_.hasRequestedRestart() &&
		!CommonVariables.hasDisplayedWarning &&
		CommonVariables.isAssignmentChanged &&
		++nSelectionExecutions < Constants.MAX_SELECTION_EXECUTIONS );

		if( CommonVariables.isAssignmentChanged &&
		nSelectionExecutions == Constants.MAX_SELECTION_EXECUTIONS )
			return adminItem_.startError( 1, moduleNameString_, "There is probably an endless loop in the selections" );

		return Constants.RESULT_OK;
		}
	};

/*************************************************************************
 *	"Praise his name with dancing,
 *	accompanied by tambourine and harp.
 *	For the Lord delights in his people;
 *	he crowns the humble with victory." (Psalm 149:3-4)
 *************************************************************************/
