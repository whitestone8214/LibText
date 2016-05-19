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

/* gcc -shared -lm -o libtext.so libtext.c */

#include "libtext.h"

static void Q_text_array_free (char **array) {
	if (array == NULL) return;
	int _nth; while (array[_nth] != NULL) {free (array[_nth]); _nth++;}
	free (array);
}
static char **Q_text_array_append (char **array, char *content) {
	if (array == NULL) return NULL;
	int _length = 0; while (array[_length] != NULL) _length++;
	
	char **_array = (char **) malloc (sizeof (char *) * (_length + 1 + 1)); if (_array == NULL) return NULL;
	_length = 0; while (array[_length] != NULL) {_array[_length] = array[_length]; _length++;}
	_array[_length] = content; _array[_length + 1] = NULL; return _array;
}
static int *Q_number_array_append (int *array, int content) {
	if (array == NULL) return NULL;
	int _length = 0; while (array[_length] != -1) _length++;
	
	int *_array = (int *) malloc (sizeof (int) * (_length + 1 + 1)); if (_array == NULL) return NULL;
	_length = 0; while (array[_length] != -1) {_array[_length] = array[_length]; _length++;}
	_array[_length] = content; _array[_length + 1] = -1; return _array;
}
static char *Q_text_append (char *text, char *text2) {
	if ((text == NULL) && (text2 == NULL)) return NULL;
	if (text == NULL) return strdup (text2); if (text2 == NULL) return strdup (text);
	
	int _length = strlen (text) + strlen (text2); char _result[_length + 1]; _result[_length] = '\0'; int _spotText = 0;
	while (_spotText < strlen (text)) {_result[_spotText] = text[_spotText]; _spotText++;}
	while (_spotText - strlen (text) < strlen (text2)) {_result[_spotText] = text2[_spotText - strlen (text)]; _spotText++;}
	return strdup (_result);
}

Text *text_new () {
	Text *_session = (Text *) malloc (sizeof (Text)); if (_session == NULL) return NULL;
	_session->content = NULL; _session->length = 0;
}
void text_insert (Text *session, const char *text, int nth, char copy) {
	/* 1. Are given parameters sane? */
	if (session == NULL) return;
	int _nth = nth; if ((nth == -1) || (nth >= session->length)) _nth = session->length; if (nth < -1) _nth = 0;
	
	/* 2. Insert text into array. */
	char **_content = (char **) malloc (sizeof (char *) * (session->length + 1));
	int _spotContent; int _spotResult = 0; for (_spotContent = 0; _spotContent < session->length + 1; _spotContent++) {
		_content[_spotResult] = _spotContent == _nth ? (text != NULL ? (copy == 1 ? strdup (text) : (char *) text) : NULL) : session->content[_spotContent];
		_spotResult++;
	}
	free (session->content); session->content = _content; session->length++;
}
void text_withdraw (Text *session, int nth) {
	/* 1. Are given parameters sane? */
	if (session == NULL) return;
	int _nth = nth; if ((nth == -1) || (nth >= session->length)) _nth = session->length; if (nth < -1) _nth = 0;
	
	/* 2. Withdraw text from array. */
	char **_content = (char **) malloc (sizeof (char *) * (session->length - 1));
	int _spotContent = 0; int _spotResult; for (_spotResult = 0; _spotResult < session->length; _spotResult++) {
		if (_spotResult == nth) {if (session->content[_spotResult] != NULL) free (session->content[_spotResult]); continue;}
		_content[_spotContent] = session->content[_spotResult];
		_spotContent++;
	}
	free (session->content); session->content = _content; session->length--;
}
char *text_connect (Text *session, char *border) {
	/* 1. Are given parameters sane? */
	if (session == NULL) return NULL;
	if (session->length < 1) return NULL;
	
	/* 2. Create text. */
	int _nth = 0; char *_result = NULL; while (_nth < session->length) {
		char *_result1 = Q_text_append (_result, session->content[_nth]); if (_result != NULL) free (_result); _result = _result1;
		if ((_nth < session->length - 1) && (border != NULL)) {_result1 = Q_text_append (_result, border); if (_result != NULL) free (_result); _result = _result1;}
	}
	return _result;
}
void text_erase (Text *session) {
	/* 1. Are given parameters sane? */
	if (session == NULL) return;
	
	/* 2. Withdraw all texts from array. */
	int _spotContent; for (_spotContent = 0; _spotContent < session->length; _spotContent++) {if (session->content[_spotContent] != NULL) free (session->content[_spotContent]);}
	
	/* 3. Erase session. */
	free (session);
}

char libtext_matches (const char *text, int where, const char *piece, char backward) {
	/* 1. Are given parameters sane? */
	if ((text == NULL) || (piece == NULL) || (strlen (text) < strlen (piece)) || (where < 0) || (where >= strlen (text))) return 0;
	if (((backward == 0) && (where + (strlen (piece) - 1) >= strlen (text))) || ((backward == 1) && (where - (strlen (piece) - 1) < 0))) return 0;
	
	/* 2. Is the part matches with given text? */
	int _spotPiece; for (_spotPiece = 0; _spotPiece < strlen (piece); _spotPiece++) {if (text[backward == 1 ? where - _spotPiece : where + _spotPiece] != piece[_spotPiece]) break;}
	return _spotPiece == strlen (piece) ? 1 : 0;
}

char libtext_escaped (const char *text, int nth) {
	/* 1. Are given parameters sane? */
	if ((text == NULL) || (strlen (text) <= 1) || (nth <= 0) || (nth >= strlen (text))) return 0;
	
	/* 2. Is (nth - 1)th character escape character(\)? */
	if (text[nth - 1] != '\\') return 0;
	
	/* 3. How many escape characters continued? */
	int _nth = nth - 1; int _count = 0;
	while (_nth >= 0) {if (text[_nth] == '\\') {_count++; _nth--;} else break;}
	if (_count % 2 == 1) return 1; else return 0;
}

char *libtext_connect (int amount, ...) {
	/* 1. Are given parameters sane? */
	if (amount < 1) return NULL;
	
	/* 2. Create text. */
	va_list _spotParameter; va_start (_spotParameter, amount); char *_result = NULL; 
	int _nthParameter; for (_nthParameter = 0; _nthParameter < amount; _nthParameter++) {
		char *_parameter = va_arg (_spotParameter, char *);
		char *_result1 = Q_text_append (_result, _parameter); if (_result != NULL) free (_result); _result = _result1;
	}
	return _result;
}

char *libtext_cut (const char *text, int from, int to, char take) {
	/* 1. Are given parameters sane? */
	int _to = to; if (to == -1) _to = strlen (text) - 1;
	if ((text == NULL) || (strlen (text) < 1) || (from < 0) || (_to >= strlen (text)) || (_to - from == strlen (text) - 1) || (from > _to)) return NULL;
	
	/* 2. Ready the room for text. */
	int _length = (_to - from) + 1; if (take == 1) _length = strlen (text) - _length; _length++; char _text[_length]; _text[_length - 1] = '\0';
	
	/* 3. Create text. */
	if (take == 1) {
		int _nthOriginal = 0; int _nthCopy = 0; while (_nthOriginal < strlen (text)) {
			if (_nthOriginal == from) _nthOriginal = _to + 1;
			_text[_nthCopy] = text[_nthOriginal]; _nthOriginal++; _nthCopy++;
		}
	}
	else {
		int _nth = from; while (_nth <= _to) {_text[_nth - from] = text[_nth]; _nth++;}
	}
	return strdup (_text);
}

int *libtext_find (const char *text, const char *piece) {
	/* 1. Are given parameters sane? */
	if ((text == NULL) || (piece == NULL)) return NULL;
	
	/* 2. Create the array. */
	int *_result = (int *) malloc (sizeof (int)); if (_result == NULL) return NULL; _result[0] = -1;
	int _spotText, _spotPiece; for (_spotText = 0; _spotText < strlen (text); _spotText++) {
		if ((text[_spotText] == piece[0]) && (libtext_escaped (text, _spotText) == 0)) {
			if (_spotText + (strlen (piece) - 1) >= strlen (text)) break;
			if (libtext_matches (text, _spotText, piece, 0) == 1) {
				int *_result1 = Q_number_array_append (_result, _spotText); free (_result); _result = _result1;
				_spotText = _spotText + (strlen (piece) - 1);
			}
		}
	}
	return _result;
}

char *libtext_replace (const char *text, const char *from, const char *to) {
	/* 1. Are given parameters sane? */
	if ((text == NULL) || (from == NULL)) return NULL;
	
	/* 2. Find 'from's. */
	int *_where = libtext_find (text, from); int _nFroms = 0; while (_where[_nFroms] != -1) _nFroms++;
	
	/* 3. Create text. */
	int _length = (strlen (text) - (_nFroms * strlen (from))) + (_nFroms * strlen (to)); char _result[_length + 1]; _result[_length] = '\0';
	int _spotText; int _spotFrom = 0; int _spotResult = 0; for (_spotText = 0; _spotText < strlen (text); _spotText++) {
		if (_spotText == _where[_spotFrom]) {
			int _spotTo; for (_spotTo = 0; _spotTo < strlen (to); _spotTo++) {_result[_spotResult + _spotTo] = to[_spotTo];}
			_spotText = _spotText + (strlen (from) - 1); _spotFrom++; _spotResult = _spotResult + strlen (to);
		}
		else {_result[_spotResult] = text[_spotText]; _spotResult++;}
	}
	free (_where); return strdup (_result);
}

char **libtext_split (const char *text, const char *border) {
	/* 1. Are given parameters sane? */
	if ((text == NULL) || (border == NULL)) return NULL;
	
	/* 2. Find 'border'. */
	int *_where = libtext_find (text, border);
	if (_where == NULL) {
		free (_where);
		char **_result = (char **) malloc (sizeof (char *) * 2); if (_result == NULL) return NULL;
		_result[0] = strdup (text); if (_result[0] == NULL) {Q_text_array_free (_result); return NULL;} _result[1] = NULL;
		return _result;
	}
	
	/* 3. Create the array. */
	char **_result = (char **) malloc (sizeof (char *)); if (_result == NULL) {free (_where); return NULL;} _result[0] = NULL;
	int _nthFound = 0; while (_where[_nthFound] != -1) {
		if (_nthFound == 0) {if (_where[0] != 0) {
			char **_result1 = Q_text_array_append (_result, libtext_cut (text, 0, _where[0] - 1, 0)); free (_result); _result = _result1;
		}}
		else if (_where[_nthFound] - (_where[_nthFound - 1] + strlen (border)) >= 1) {
			char **_result1 = Q_text_array_append (_result, libtext_cut (text, _where[_nthFound - 1] + strlen (border), _where[_nthFound] - 1, 0)); free (_result); _result = _result1;
		}
		if (_where[_nthFound + 1] == -1) {if (_where[_nthFound] + strlen (border) < strlen (text) - 1) {
			char **_result1 = Q_text_array_append (_result, libtext_cut (text, _where[_nthFound] + strlen (border), strlen (text) - 1, 0)); free (_result); _result = _result1;
		}}
		_nthFound++;
	}
	free (_where); return _result;
}
Text *libtext_split_V2 (const char *text, const char *border) {
	/* 1. Are given parameters sane? */
	if ((text == NULL) || (border == NULL)) return NULL;
	
	/* 2. Find 'border'. */
	int *_where = libtext_find (text, border); if (_where == NULL) return text_new ();
	
	/* 3. Create the array. */
	Text *_result = text_new (); if (_result == NULL) {free (_where); return NULL;}
	int _nthFound = 0; while (_nthFound >= 0) {
		if (_where[_nthFound] == 0) {_nthFound++; continue;}
		int _border = _where[_nthFound] == -1 ? strlen (text) : _where[_nthFound]; int _from = _nthFound == 0 ? 0 : _where[_nthFound - 1] + strlen (border);
		if (_border == _from) {_nthFound++; continue;}
		text_insert (_result, libtext_cut (text, _from, _border - 1, 0), -1, 0);
		if (_where[_nthFound] == -1) break; _nthFound++;
	}
	free (_where); return _result;
}

char *libtext_number (int number) {
	/* 1. How many digits this number has? */
	int _number = number; if (number < 0) _number = 0 - _number;
	int _digits = 0; while (_number >= pow (10, _digits)) _digits++;
	
	/* 2. Convert to text. */
	char _text[_digits]; int _nth = 0; while (_nth < _digits) {
		int _digit = _number % (int) pow (10, _nth + 1); int _lessThan = 9 * pow (10, _nth);
		while (_digit < _lessThan) _lessThan = _lessThan - (int) pow (10, _nth); _text[_digits - 1 - _nth] = 0x30 + (_lessThan / (int) pow (10, _nth));
		_nth++;
	}
	
	/* 3. Finish and prepend minus if needed. */
	int _length = _digits + 1; if (number < 0) _length++;
	_nth = 0; char _text1[_length]; _text1[_length - 1] = '\0'; if (number < 0) {_nth++; _text1[0] = '-';}
	while (_nth < _length - 1) {_text1[_nth] = _text[number < 0 ? _nth - 1 : _nth]; _nth++;}
	return strdup (_text1);
}
