/*	Class:		WriteResultType
 *	Purpose:	To return write variables,
 *				as the result of a function call
 *	Version:	Thinknowlogy 2016r2 (Restyle)
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

#include "Constants.h"

class WriteResultType
	{
	friend class Item;
	friend class WordWriteSentence;
	friend class WordWriteWords;
	protected:
	// Protected variables

	ResultType result;
	bool hasFoundWordToWrite;
	bool isSkippingClearWriteLevel;

	protected:
	// Constructor

	WriteResultType()
		{
		result = RESULT_OK;
		hasFoundWordToWrite = false;
		isSkippingClearWriteLevel = false;
		}
	};

/*************************************************************************
 *	"Surely your goodness and unfailing love will pursue me
 *	all the days of my life,
 *	and I will live in the house of the Lord
 *	forever." (Psalm 23:6)
 *************************************************************************/
