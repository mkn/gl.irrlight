/*
 * console.hpp
 *
 *  Created on: Oct 04, 2013
 *      Author: philip
 */
/*
#include "kul/cgi/dev/console.hpp"
*/

#ifndef _KUL_CGI_DEV_CONSOLE_HPP_
#define _KUL_CGI_DEV_CONSOLE_HPP_

#include "glog/logging.h"

#include <string>
#include <vector>
#include "irrlicht.h"

#include "kul/string.hpp"
#include "kul/ext/google.hpp"


namespace kul { namespace cgi{ namespace dev{

enum ConsoleUI{
	COMMAND_BOX,
	COMMAND__HISTORY_BOX,
};

class AConsoleCmd{
	public:
		virtual ~AConsoleCmd(){}
		virtual void 			perform() 	= 0;
		virtual std::wstring 	getOutput() = 0;
};

class ConsoleCmdHistory{
	private:
		static int MINUS_ONE;
		int it;
		std::vector<std::wstring> history;		
	public:
		ConsoleCmdHistory() : it(-1){}
		~ConsoleCmdHistory(){}

		const wchar_t*	getPreviousHistoryOrReturnTheSameString	(const wchar_t* cText)  ;
		const wchar_t*	getNextHistoryOrReturnAnEmptyString		(const wchar_t* cText)  ;
		ConsoleCmdHistory* addNew(std::wstring currentCommand) ; // andDeleteOldestIfApplicable
		ConsoleCmdHistory* reset() { it = history.size(); return this;}
};

class Console{
	private:
		bool shown;
		std::vector<std::string> output;
		ConsoleCmdHistory commandHistory;
		const kul::ext::goo_gle::StringToTGMap<AConsoleCmd*> cmds;

		void rollDown	(irr::IrrlichtDevice * device);
		void rollUp		(irr::IrrlichtDevice * device);
	public:
		Console(const kul::ext::goo_gle::StringToTGMap<AConsoleCmd*> cmds) : shown(false), cmds(cmds){}		
		Console() : shown(false){}
		void draw(irr::IrrlichtDevice * device);
		void show(irr::IrrlichtDevice * device) { shown = true; 	history().reset(); rollDown(device); }
		void hide(irr::IrrlichtDevice * device) { shown = false;	rollUp(device);}
		const wchar_t * getText(irr::IrrlichtDevice *device) const;
		void setText(irr::IrrlichtDevice *device, const wchar_t* text);

		bool isShown() { return shown; }

		ConsoleCmdHistory& history() { return commandHistory; }
};


/*
class AConsoleKeyHandler{
	protected:
		irr::IrrlichtDevice& 	device;
	public:
		AConsoleKeyHandler(irr::IrrlichtDevice& device) : device(device) {}
		virtual ~AConsoleKeyHandler(){}
		virtual bool handle() = 0;
};

class ConsoleEnterKeyHandler : public AConsoleKeyHandler{
	private:
		IsometricOverlay& overlay;
	public:
		ConsoleEnterKeyHandler(IsometricOverlay& overlay, irr::IrrlichtDevice* device) : overlay(overlay), ADevConsoleKeyHandler(*device) {}
		~ConsoleEnterKeyHandler(){}
		bool handle(){
			std::wstring textString(ConsoleStringHandler::getWStringFromWChar_TP(overlay.getConsole().getText(&device), 'L', true, true));
			if(textString.size() == 0) return false;

			overlay.getConsole().setText(&device, L"");
			std::vector<std::wstring> bits = kul::st_d::String.split(textString, ' ');
			//boost::split(bits, textString, boost::is_any_of(" ") );

			std::auto_ptr<AConsoleCmd> cmd;
			//if	(bits.size() > 1) 	cmd.reset(DeveloperConsoleArgumentRequiredCommandResolver	::getInstance()->resolve(bits		, overlay, device));
			//else					cmd.reset(DeveloperConsoleCommandResolver					::getInstance()->resolve(textString	, overlay, device));
			cmd->perform();
			std::wcout << cmd->getOutput() << std::endl;

			overlay.getConsole().history().addNewCommandAndDeleteOldestIfApplicable(textString);
			return true;
		}
};

class ConsoleKeyEntryHandler{
	public:
		static bool keyUp(irr::IrrlichtDevice *device, IsometricOverlay& overlay, irr::EKEY_CODE keyCode){
			bool k = false;
			if(keyCode == irr::KEY_RETURN)
				return DevConsoleEnterKeyHandler(overlay, device).handle();
			else 
			if(keyCode == irr::KEY_UP)		{
				overlay.getConsole().setText(device, overlay.getConsole().history().getPreviousHistoryOrReturnTheSameString	(overlay.getConsole().getText(device)));
				k = true;
			}
			else
			if(keyCode == irr::KEY_DOWN)	{
				overlay.getConsole().setText(device, overlay.getConsole().history().getNextHistoryOrReturnAnEmptyString		(overlay.getConsole().getText(device)));
				k = true;
			}
			return k;
		}
		static bool keyDown	(irr::IrrlichtDevice *device, IsometricOverlay& overlay, irr::EKEY_CODE keyCode){
			return false;
		}
};

class ConsoleStringHandler{
	public:
		static std::wstring getWStringFromWChar_TP(const wchar_t* cText, char upperLower = ' ', bool trim = false, bool replaceDoubleSpace = false){

			irr::core::stringw text(cText);
			if		(upperLower == 'U' || upperLower == 'u')
				text.make_upper();
			else if	(upperLower == 'L' || upperLower == 'l')
				text.make_lower();

			std::wstring textString(text.c_str());

			if(trim){
				kul::st_d::trim(textString)
				//boost::trim(textString);
			}

			if(replaceDoubleSpace){
				kul::st_d::replaceAll(textString, "  ", " ");
				//while(boost::contains(textString, "  ")) boost::replace_all(textString, "  ", " ");
			}

			return textString;
		}
};
*/

};};};
#endif /* _KUL_CGI_DEV_CONSOLE_HPP_ */
