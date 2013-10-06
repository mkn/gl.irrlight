/*
 * console.hpp
 *
 *  Created on: Oct 04, 2013
 *      Author: philip
 */
/*
#include "kul/cgi/dev/console/cmds.hpp"
*/

#ifndef _KUL_CGI_DEV_CONSOLE_CMDS_HPP_
#define _KUL_CGI_DEV_CONSOLE_CMDS_HPP_

#include "glog/logging.h"

namespace kul { namespace cgi{ namespace dev{
/*
enum ConsoleCmds{
	NONE
};


class ConsoleCmdResolver{
	private:
		static DevConsoleCmdResolver* instance;
	public:
		ADevConsoleCmd* resolve(std::wstring command, A3DScene& scene, irr::IrrlichtDevice& device);
		static DevConsoleCmdResolver* getInstance(){ return instance; }
};

class AConsoleArgRequiredCommand : public ADevConsoleCmd{
	private:
		ADevConsoleArgRequiredCommand(){}

	protected:
		std::vector<std::wstring> args;

	public:
		ADevConsoleArgRequiredCommand(std::vector<std::wstring> args) : args(args){}
		virtual ~ADevConsoleArgRequiredCommand() {}

		virtual void 			perform() 	= 0;
		virtual std::wstring 	getOutput() = 0;
};

class ConsoleArgRequiredCmdResolver{
	private:
		static DevConsoleArgRequiredCmdResolver* instance;
	public:
		ADevConsoleCmd* resolve(std::vector<std::wstring> command, A3DScene& scene, irr::IrrlichtDevice& device);
		static DevConsoleArgRequiredCmdResolver* getInstance(){ return instance; }
};

*/
};};};
#endif /* _KUL_CGI_DEV_CONSOLE_CMDS_HPP_ */
