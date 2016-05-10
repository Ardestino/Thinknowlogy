/*	Class:			GrammarResultType
 *	Purpose:		To return justification variables of a method
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

class GrammarResultType
	{
	// Protected variables

	protected byte result;

	protected boolean hasFoundWordEnding;

	protected short guideByGrammarSelectionNr;
	protected short guideByGrammarWordOrderNr;

	protected int singularNounWordStringLength;

	protected GrammarItem createdGrammarItem;
	protected GrammarItem foundGrammarItem;

	protected String singularNounWordString;

	// Constructor / deconstructor

	GrammarResultType()
		{
		result = Constants.RESULT_OK;

		hasFoundWordEnding = false;

		guideByGrammarSelectionNr = 0;
		guideByGrammarWordOrderNr = Constants.NO_ORDER_NR;

		singularNounWordStringLength = 0;

		createdGrammarItem = null;
		foundGrammarItem = null;

		singularNounWordString = null;
		}
	};

/*************************************************************************
 *	"Sing praises to God, sing praises;
 *	sing praises to our King, sing praises.
 *	For God is the King over all the earth.
 *	Praise him with a psalm!" (Psalm 47:6-7)
 *************************************************************************/