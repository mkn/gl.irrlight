/*
 * console.hpp
 *
 *  Created on: Oct 04, 2013
 *      Author: philip
 */
/*
#include "irrlight/dev/console.hpp"
*/

#ifndef _IRRLIGHT_DEV_CONSOLE_HPP_
#define _IRRLIGHT_DEV_CONSOLE_HPP_

#include <string>
#include <vector>
#include "irrlicht.h"

#include "kul/hash.hpp"
#include "kul/string.hpp"

namespace irrlight{namespace dev{

enum ConsoleUI{
	COMMAND_BOX,
	COMMAND__HISTORY_BOX,
};

class AConsoleCmd{
	public:
		virtual ~AConsoleCmd(){}
		virtual void 			perform() 	{};
		virtual void 			perform(const std::vector<std::wstring>& args){};
		virtual std::wstring 	getOutput() { return L"";};
};

class ConsoleCmdHistory{
	private:
		static int MINUS_ONE;
		int it;
		std::vector<std::wstring> history;		
	public:
		ConsoleCmdHistory() : it(-1){}
		~ConsoleCmdHistory(){}

		const wchar_t*	getPrevious ()  ;
		const wchar_t*	getNext 	()  ;
		ConsoleCmdHistory* addNew(std::wstring currentCommand) ; // andDeleteOldestIfApplicable
		ConsoleCmdHistory* reset() { it = history.size(); return this;}
};

class Console{
	private:
		bool shown;
		std::vector<std::string> output;
		ConsoleCmdHistory commandHistory;
		kul::hash::map::S2T<AConsoleCmd> cmds;
	protected:
		void rollDown	(irr::IrrlichtDevice * device);
		void rollUp		(irr::IrrlichtDevice * device);
	public:
		Console() : shown(false){}		
		void draw(irr::IrrlichtDevice * device);
		void show(irr::IrrlichtDevice * device) { shown = true; 	history().reset(); rollDown(device); }
		void hide(irr::IrrlichtDevice * device) { shown = false;	rollUp(device);}
		const wchar_t * getText(irr::IrrlichtDevice *device) const;
		void setText(irr::IrrlichtDevice *device, const wchar_t* text);

		bool isShown() const { return shown; }
		const void addCommand(const std::string& s, const AConsoleCmd& cmd){ cmds.insert(s, cmd); } 
		kul::hash::map::S2T<AConsoleCmd>& commands() { return cmds;} 
		ConsoleCmdHistory& history() { return commandHistory; }
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
			std::string tS = kul::WString::toString(textString);
			kul::String::trim(tS);
			if(trim)
				kul::WString::trim(textString);
			if(replaceDoubleSpace){
				const wchar_t* ds{L"  "};
				const wchar_t* ss{L" "};
				kul::WString::replaceAll(textString, std::wstring(ds), std::wstring(ss));
			}
			return textString;
		}
};

class ConsoleEnterKeyHandler{
	public:
		static std::string handle(Console& console, irr::IrrlichtDevice* device){
			std::wstring textString(ConsoleStringHandler::getWStringFromWChar_TP(console.getText(device), 'L', true, true));
			if(textString.size() == 0) return "";
			console.setText(device, L"");
			console.history().addNew(textString);
			return kul::WString::toString(textString);
		}
};

class ConsoleKeyEntryHandler{
	public:
		static bool keyUp(irr::IrrlichtDevice *device, Console& console, irr::EKEY_CODE keyCode){
			bool k = false;
			//if(keyCode == irr::KEY_RETURN) k = ConsoleEnterKeyHandler(console, device).handle();
			if(keyCode == irr::KEY_UP){
				const wchar_t* txt = console.history().getPrevious();
				if(txt) console.setText(device, txt);				
				k = true;
			}else if(keyCode == irr::KEY_DOWN){
				const wchar_t* txt = console.history().getNext();
				if(!txt) txt = L"";
				console.setText(device, txt);
				k = true;
			}
			return k;
		}
		static bool keyDown	(irr::IrrlichtDevice *device, Console& console, irr::EKEY_CODE keyCode){
			return false;
		}
};


};};
#endif /* _IRRLIGHT_DEV_CONSOLE_HPP_ */
