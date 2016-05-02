/*
	LibText - A collection of text-related functions.

	Copyright (C) 2014-2016 Minho Jo <whitestone8214@openmailbox.org>

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdarg.h>
#include <malloc.h>
#include <math.h>
#include <string.h>

#define VERSION_LIBTEXT "0.1"

/*
	Checks whether specific part of 'text' matches with 'piece'.
	
	text: Text
	where: Position of character to start check
	piece: Text to compare to
	backward: Switch to decide whether to compare backward or forward. If this set to 1, the function will compare backward from 'where'. Otherwise the function will compare forward from 'where'.
	
	Return value: 1 if it is matching, 0 if it isn't
*/
char libtext_matches (const char *text, int where, const char *piece, char backward);

/*
	Checks whether 'nth' character in 'text' is escaped or not.
	
	text: Text
	nth: Position of character you want to check
	
	Return value: 1 if it is escaped, 0 if it isn't
*/
char libtext_escaped (const char *text, int nth);

/*
	Cuts part from 'from' to 'to' in 'text'.
	
	text: Text
	from: Position you want to start cut at
	to: Position you want to end cut at
	take: Switch to decide which part should be returned. If this set to 1, the function will return rest part. Otherwise the function will return cut part.
	
	Return value: See 'take'. You must free() it after use, as it uses dynamic memory allocation.
*/
char *libtext_cut (const char *text, int from, int to, char take);

/*
	Finds given 'piece' on 'text'.
	
	text: Text
	piece: Text to find
	
	Return value: An array of position of first letter for each 'piece's. Last member is -1. You must free() the array after use, as it uses dynamic memory allocation.
*/
int *libtext_find (const char *text, const char *piece);

/*
	Connects given texts.
	
	amount: Amount of texts
	...: Texts
	
	Return value: Connected text. You must free() it after use, as it uses dynamic memory allocation.
*/
char *libtext_connect (int amount, ...);

/*
	Replaces all 'from's from 'text' with 'to'.
	
	text: Text
	from: Text to remove
	to: Text to insert where 'from' was
	
	Return value: Text that replaced each 'from's with 'to'. You must free() it after use, as it uses dynamic memory allocation.
*/
char *libtext_replace (const char *text, const char *from, const char *to);

/*
	Splits given 'text' into an array, on the basis of 'border's.
	
	text: Text
	border: Basis for spliting text
	
	Return value: An array of splited texts. Last member is NULL. You must free() each members except last and then also array itself after use, as it uses dynamic memory allocation.
*/
char **libtext_split (const char *text, const char *border);

/*
	Converts given 'number' to text.
	
	number: Number
	
	Return value: Created text. You must free() it after use, as it uses dynamic memory allocation.
*/
char *libtext_number (int number);
