
/*
 * History.cpp
 *
 *  Created on: Oct 04, 2013
 *      Author: philip
 */


#include "irrlight/dev/console.hpp"

int irrlight::dev::ConsoleCmdHistory::MINUS_ONE = -(1);

const wchar_t* irrlight::dev::ConsoleCmdHistory::getPrevious(){
	if(--it > MINUS_ONE)
		return history[it].c_str();
	it++;
	return 0;
}
const wchar_t* irrlight::dev::ConsoleCmdHistory::getNext	(){
	if(++it < history.size())
		return history[it].c_str();
	reset();
	return 0;
}

irrlight::dev::ConsoleCmdHistory* irrlight::dev::ConsoleCmdHistory::addNew(std::wstring cmd){ // andDeleteOldestIfApplicable

	//we'll go with 10 for the moment - but up it later
	if(history.size() > 9)	history.erase(history.begin());
	history.push_back(cmd);
	return reset();
}
