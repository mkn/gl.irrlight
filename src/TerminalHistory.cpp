/**

./src/TerminalHistory.cpp

Created on: 16 May 2013

Copyright (c) 2013, Philip Deegan

This file is part of irrlight.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "irrlight.hpp"

int irrlight::TerminalHistory::MINUS_ONE = -(1);

const wchar_t* irrlight::TerminalHistory::getPrevious(){
	if(--it > MINUS_ONE) return history[it].c_str();
	it++;
	return 0;
}
const wchar_t* irrlight::TerminalHistory::getNext	(){
	if(++it < history.size()) return history[it].c_str();
	reset();
	return 0;
}

irrlight::TerminalHistory* irrlight::TerminalHistory::addNew(std::wstring cmd){ // andDeleteOldestIfApplicable

	//we'll go with 10 for the moment - but up it later
	if(history.size() > 9)	history.erase(history.begin());
	history.push_back(cmd);
	return reset();
}
