/*	Class:			ContextList
 *	Parent class:	List
 *	Purpose:		To store context items
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

class ContextList extends List
	{
	// Private methods

	private boolean hasContext( boolean isCompoundCollectionSpanishAmbiguous, int contextNr, WordItem specificationWordItem )
		{
		ContextItem searchItem = firstActiveContextItem();

		// In case of a pronoun context, the given specification word item will be undefined

		if( contextNr > Constants.NO_CONTEXT_NR )
			{
			while( searchItem != null )
				{
				if( searchItem.contextNr() == contextNr &&
				searchItem.specificationWordItem() == specificationWordItem &&
				searchItem.isCompoundCollectionSpanishAmbiguous() == isCompoundCollectionSpanishAmbiguous )
					return true;

				searchItem = searchItem.nextContextItem();
				}
			}

		return false;
		}

	// Constructor / deconstructor

	protected ContextList( WordItem myWordItem )
		{
		initializeListVariables( Constants.WORD_CONTEXT_LIST_SYMBOL, myWordItem );
		}


	// Protected methods

	protected boolean hasContext( int contextNr )
		{
		ContextItem searchItem = firstActiveContextItem();

		if( contextNr > Constants.NO_CONTEXT_NR )
			{
			while( searchItem != null )
				{
				if( searchItem.contextNr() == contextNr )
					return true;

				searchItem = searchItem.nextContextItem();
				}
			}

		return false;
		}

	protected boolean hasContext( int contextNr, WordItem specificationWordItem )
		{
		ContextItem searchItem = firstActiveContextItem();

		// In case of a pronoun context, the given specification word item will be undefined

		if( contextNr > Constants.NO_CONTEXT_NR )
			{
			while( searchItem != null )
				{
				if( searchItem.contextNr() == contextNr &&
				searchItem.specificationWordItem() == specificationWordItem )
					return true;

				searchItem = searchItem.nextContextItem();
				}
			}

		return false;
		}

	protected boolean hasContextCurrentlyBeenUpdated( int contextNr, WordItem specificationWordItem )
		{
		ContextItem searchItem = firstActiveContextItem();

		// In case of a pronoun context, the given specification word item will be undefined

		if( contextNr > Constants.NO_CONTEXT_NR )
			{
			while( searchItem != null )
				{
				if( searchItem.hasCurrentCreationSentenceNr() &&
				searchItem.contextNr() == contextNr &&
				searchItem.specificationWordItem() == specificationWordItem )
					return true;

				searchItem = searchItem.nextContextItem();
				}
			}

		return false;
		}

	protected boolean isContextSubset( int subsetContextNr, int fullSetContextNr )
		{
		ContextItem searchItem = firstActiveContextItem();

		if( fullSetContextNr > Constants.NO_CONTEXT_NR &&
		subsetContextNr > Constants.NO_CONTEXT_NR )
			{
			while( searchItem != null )
				{
				if( searchItem.contextNr() == subsetContextNr &&
				hasContext( fullSetContextNr, searchItem.specificationWordItem() ) )
					return true;

				searchItem = searchItem.nextContextItem();
				}
			}

		return false;
		}

	protected int contextNr( WordItem specificationWordItem )
		{
		ContextItem searchItem = firstActiveContextItem();

		// In case of a pronoun context, the given specification word item will be undefined

		while( searchItem != null )
			{
			if( searchItem.specificationWordItem() == specificationWordItem )
				return searchItem.contextNr();

			searchItem = searchItem.nextContextItem();
			}

		return Constants.NO_CONTEXT_NR;
		}

	protected int contextNr( boolean isCompoundCollectionSpanishAmbiguous, WordItem specificationWordItem )
		{
		ContextItem searchItem = firstActiveContextItem();

		// In case of a pronoun context, the given specification word item will be undefined

		while( searchItem != null )
			{
			if( searchItem.specificationWordItem() == specificationWordItem &&
			searchItem.isCompoundCollectionSpanishAmbiguous() == isCompoundCollectionSpanishAmbiguous )
				return searchItem.contextNr();

			searchItem = searchItem.nextContextItem();
			}

		return Constants.NO_CONTEXT_NR;
		}

	protected int highestContextNr()
		{
		int highestContextNr = Constants.NO_CONTEXT_NR;
		ContextItem searchItem = firstActiveContextItem();

		while( searchItem != null )
			{
			if( searchItem.contextNr() > highestContextNr )
				highestContextNr = searchItem.contextNr();

			searchItem = searchItem.nextContextItem();
			}

		return highestContextNr;
		}

	protected byte addContext( boolean isCompoundCollectionSpanishAmbiguous, short contextWordTypeNr, short specificationWordTypeNr, int contextNr, WordItem specificationWordItem )
		{
		if( contextNr > Constants.NO_CONTEXT_NR )
			{
			if( !hasContext( isCompoundCollectionSpanishAmbiguous, contextNr, specificationWordItem ) )
				{
				if( contextWordTypeNr > Constants.WORD_TYPE_UNDEFINED &&
				contextWordTypeNr < Constants.NUMBER_OF_WORD_TYPES )
					{
					if( specificationWordItem == null ||

					( specificationWordTypeNr > Constants.WORD_TYPE_UNDEFINED &&
					specificationWordTypeNr < Constants.NUMBER_OF_WORD_TYPES ) )
						{
						if( CommonVariables.currentItemNr < Constants.MAX_ITEM_NR )
							{
							if( addItemToList( Constants.QUERY_ACTIVE_CHAR, new ContextItem( isCompoundCollectionSpanishAmbiguous, contextWordTypeNr, ( specificationWordTypeNr == Constants.WORD_TYPE_NOUN_PLURAL ? Constants.WORD_TYPE_NOUN_SINGULAR : specificationWordTypeNr ), contextNr, specificationWordItem, this, myWordItem() ) ) != Constants.RESULT_OK )
								return addError( 1, null, "I failed to add an active context item" );
							}
						else
							return startError( 1, null, "The current item number is undefined" );
						}
					else
						return startError( 1, null, "The given specification word type is undefined or out of bounds" );
					}
				else
					return startError( 1, null, "The given context word type is undefined or out of bounds" );
				}
			}
		else
			return startError( 1, null, "The given context number is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte checkWordItemForUsage( WordItem unusedWordItem )
		{
		ContextItem searchItem = firstActiveContextItem();

		if( unusedWordItem != null )
			{
			while( searchItem != null )
				{
				if( searchItem.specificationWordItem() == unusedWordItem )
					return startError( 1, null, "The specification word item is still in use" );

				searchItem = searchItem.nextContextItem();
				}
			}
		else
			return startError( 1, null, "The given unused word item is undefined" );

		return Constants.RESULT_OK;
		}
/*
	protected byte storeChangesInFutureDatabase()
		{
		ContextItem searchItem = firstActiveContextItem();

		while( searchItem != null )
			{
			if( searchItem.hasCurrentCreationSentenceNr() )
				{
				if( searchItem.storeContextItemInFutureDatabase() != Constants.RESULT_OK )
					return addError( 1, null, "I failed to store a context item in the database" );
				}

			searchItem = searchItem.nextContextItem();
			}

		return Constants.RESULT_OK;
		}
*/
	protected ContextItem firstActiveContextItem()
		{
		return (ContextItem)firstActiveItem();
		}

	protected ContextItem contextItem( int contextNr )
		{
		ContextItem searchItem = firstActiveContextItem();

		if( contextNr > Constants.NO_CONTEXT_NR )
			{
			while( searchItem != null )
				{
				if( searchItem.contextNr() == contextNr )
					return searchItem;

				searchItem = searchItem.nextContextItem();
				}
			}

		return null;
		}

	protected ContextItem contextItem( WordItem specificationWordItem )
		{
		ContextItem searchItem = firstActiveContextItem();

		// In case of a pronoun context, the given specification word item will be undefined

		while( searchItem != null )
			{
			if( searchItem.specificationWordItem() == specificationWordItem )
				return searchItem;

			searchItem = searchItem.nextContextItem();
			}

		return null;
		}

	protected ContextItem contextItem( boolean isCompoundCollectionSpanishAmbiguous, int nContextWords, WordItem specificationWordItem )
		{
		ContextItem searchItem = firstActiveContextItem();
		WordItem anyWordItem = myWordItem();

		// In case of a pronoun context, the given specification word item will be undefined

		if( nContextWords > 0 )
			{
			while( searchItem != null )
				{
				if( searchItem.specificationWordItem() == specificationWordItem &&
				searchItem.isCompoundCollectionSpanishAmbiguous() == isCompoundCollectionSpanishAmbiguous &&
				anyWordItem.nContextWordsInAllWords( searchItem.contextNr(), specificationWordItem ) == nContextWords )
					return searchItem;

				searchItem = searchItem.nextContextItem();
				}
			}

		return null;
		}
	};

/*************************************************************************
 *	"O Lord my God, you have performed many wonders for us.
 *	Your plans for us are too numerous to list.
 *	You have no equal.
 *	I have tried to recite all your wonderful deeds,
 *	I would never come to the end of them." (Psalm 40:5)
 *************************************************************************/