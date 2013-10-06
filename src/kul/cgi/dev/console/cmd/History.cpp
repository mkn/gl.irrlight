
/*
 * History.cpp
 *
 *  Created on: Oct 04, 2013
 *      Author: philip
 */


#include "kul/cgi/dev/console.hpp"

int kul::cgi::dev::ConsoleCmdHistory::MINUS_ONE = -(1);

const wchar_t* kul::cgi::dev::ConsoleCmdHistory::getPreviousHistoryOrReturnTheSameString(const wchar_t* cText)  {
	if(--it > MINUS_ONE)
		return history[it].c_str();
	it++;
	return cText;
}
const wchar_t* kul::cgi::dev::ConsoleCmdHistory::getNextHistoryOrReturnAnEmptyString	(const wchar_t* cText)  {
	if(++it < history.size())
		return history[it].c_str();
	reset();
	return L"";
}

kul::cgi::dev::ConsoleCmdHistory* kul::cgi::dev::ConsoleCmdHistory::addNew(std::wstring cmd){ // andDeleteOldestIfApplicable

	//we'll go with 10 for the moment - but up it later
	if(history.size() > 9)	history.erase(history.begin());
	history.push_back(cmd);
	return reset();
}
