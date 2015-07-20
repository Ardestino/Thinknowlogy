/*
 *	Class:			CollectionResultType
 *	Purpose:		To return word type variables of a method
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

class CollectionResultType
	{
	// Protected variables

	protected byte result;

	protected boolean isAmbiguousCollection;
	protected boolean isCollected;

	protected int createdCollectionNr;

	protected WordItem foundGeneralizationWordItem;

	// Constructor / deconstructor

	CollectionResultType()
		{
		result = Constants.RESULT_OK;

		isAmbiguousCollection = false;
		isCollected = false;

		createdCollectionNr = Constants.NO_COLLECTION_NR;

		foundGeneralizationWordItem = null;
		}
	};

/*************************************************************************
 *	"Oh, the joys of those who are kind to the poor!
 *	The Lord rescues them when they are in trouble." (Psalm 41:1)
 *************************************************************************/