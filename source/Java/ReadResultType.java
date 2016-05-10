/*	Class:			ReadResultType
 *	Purpose:		To return read word variables of a method
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

class ReadResultType
	{
	// Protected variables

	protected byte result;

	protected boolean hasFoundGrammarDefinition;
	protected boolean hasFoundMatchingWordType;
	protected boolean hasFoundMoreInterpretations;
	protected boolean hasCreatedAllReadWords;

	protected short nReadWordReferences;

	protected int startWordPosition;
	protected int nextWordPosition;
	protected int wordLength;

	protected ReadItem createdReadItem;

	// Constructor / deconstructor

	ReadResultType()
		{
		result = Constants.RESULT_OK;

		hasFoundGrammarDefinition = false;
		hasFoundMatchingWordType = false;
		hasFoundMoreInterpretations = false;
		hasCreatedAllReadWords = false;

		nReadWordReferences = 0;

		startWordPosition = 0;
		nextWordPosition = 0;
		wordLength = 0;

		createdReadItem = null;
		}
	};

/*************************************************************************
 *	"As a deer longs for streams of water,
 *	so I long for you, O God.
 *	I thirst for God, the living God.
 *	When can I go and stand before him?" (Psalm 42:1-2)
 *************************************************************************/