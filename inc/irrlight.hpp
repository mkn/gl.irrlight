/**

./inc/irrlight.hpp

Created on: 16 April 2014

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
#ifndef _IRRLIGHT_HPP_
#define _IRRLIGHT_HPP_

#include <string>
#include <vector>
#include "irrlicht.h"

#include "kul/hash.hpp"
#include "kul/string.hpp"
#include "kul/threads.hpp"

namespace irrlight{

enum TerminalUI{
	COMMAND_BOX,
	COMMAND__HISTORY_BOX,
};

class AScene{
	public:
		virtual ~AScene(){};
		virtual void setUp 		(irr::IrrlichtDevice *device){}
		virtual void tearDown	(irr::IrrlichtDevice *device){}
		virtual void draw		(irr::IrrlichtDevice *device){}
		virtual bool OnEvent	(irr::IrrlichtDevice *device, const irr::SEvent& event)   { return false; }
		virtual bool keyDown	(irr::IrrlichtDevice *device, irr::EKEY_CODE keyCode){ return false; }
		virtual bool keyUp		(irr::IrrlichtDevice *device, irr::EKEY_CODE keyCode){ return false; }
};

class AGUIScene : public AScene {
	public:
		virtual ~AGUIScene(){};
		virtual void setUpUI(irr::gui::IGUIEnvironment* env) = 0;
};

class SceneGraph : public irr::IEventReceiver{
	private:
		std::shared_ptr<AScene> scene;
		std::shared_ptr<irr::IrrlichtDevice> device;
		SceneGraph() : scene(0), device(irr::createDevice(irr::video::EDT_NULL, irr::core::dimension2d<irr::u32>(512, 384))){}
		static SceneGraph *instance;
	public:
		~SceneGraph();

		irr::IrrlichtDevice  * getDevice()   const { return this->device.get();}
		AScene		  	* getScene()	const { return this->scene.get();}
		void setDevice  (irr::IrrlichtDevice* device);
		void setScene   (AScene  * aScene);
		void draw	   (/*IDrawableContext idc*/)  const;
		bool OnEvent	(const irr::SEvent& event);
		static SceneGraph * getInstance();
};

class ATerminalCmd{
	public:
		virtual ~ATerminalCmd(){}
		virtual void 			perform() 	{};
		virtual void 			perform(const std::vector<std::wstring>& args){};
		virtual std::wstring 	getOutput() { return L"";};
};

class TerminalHistory{
	private:
		static int MINUS_ONE;
		int it;
		std::vector<std::wstring> history;		
	public:
		TerminalHistory() : it(-1){}
		~TerminalHistory(){}

		const wchar_t*	getPrevious ()  ;
		const wchar_t*	getNext 	()  ;
		TerminalHistory* addNew(std::wstring currentCommand) ; // andDeleteOldestIfApplicable
		TerminalHistory* reset() { it = history.size(); return this;}
};

class Terminal{
	private:
		bool shown;
		std::vector<std::string> output;
		TerminalHistory th;
		kul::hash::map::S2T<ATerminalCmd> cmds;
	protected:
		void rollDown	(irr::IrrlichtDevice * device);
		void rollUp		(irr::IrrlichtDevice * device);
	public:
		Terminal() : shown(false){}		
		void draw(irr::IrrlichtDevice * device);
		void show(irr::IrrlichtDevice * device) { shown = true; 	history().reset(); rollDown(device); }
		void hide(irr::IrrlichtDevice * device) { shown = false;	rollUp(device);}
		const wchar_t * getText(irr::IrrlichtDevice *device) const;
		void setText(irr::IrrlichtDevice *device, const wchar_t* text);

		bool isShown() const { return shown; }
		const void addCommand(const std::string& s, const ATerminalCmd& cmd){ cmds.insert(s, cmd); } 
		kul::hash::map::S2T<ATerminalCmd>& commands() { return cmds;} 
		TerminalHistory& history() { return th; }
};

class TerminalStringHandler{
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

class TerminalEnterKeyHandler{
	public:
		static std::string handle(Terminal& terminal, irr::IrrlichtDevice* device){
			std::wstring textString(TerminalStringHandler::getWStringFromWChar_TP(terminal.getText(device), 'L', true, true));
			if(textString.size() == 0) return "";
			terminal.setText(device, L"");
			terminal.history().addNew(textString);
			return kul::WString::toString(textString);
		}
};

class TerminalKeyEntryHandler{
	public:
		static bool keyUp(irr::IrrlichtDevice *device, Terminal& terminal, irr::EKEY_CODE keyCode){
			bool k = false;
			//if(keyCode == irr::KEY_RETURN) k = TerminalEnterKeyHandler(terminal, device).handle();
			if(keyCode == irr::KEY_UP){
				const wchar_t* txt = terminal.history().getPrevious();
				if(txt) terminal.setText(device, txt);				
				k = true;
			}else if(keyCode == irr::KEY_DOWN){
				const wchar_t* txt = terminal.history().getNext();
				if(!txt) txt = L"";
				terminal.setText(device, txt);
				k = true;
			}
			return k;
		}
		static bool keyDown	(irr::IrrlichtDevice *device, Terminal& terminal, irr::EKEY_CODE keyCode){
			return false;
		}
};

class SceneThreadException : public kul::Exception{
	public:
		SceneThreadException(const char*f, const int l, std::string s) : kul::Exception(f, l, s){}
};

class SceneThread  {
	public:
		SceneThread(AScene& aScene) throw(SceneThreadException) : aScene(aScene)
			, device(irr::createDevice(irr::video::EDT_NULL, irr::core::dimension2d<irr::u32>(512, 384))), ref(*this), thread(ref){
			this->check();
			thread.run();
		}
		~SceneThread() {
			thread.interrupt();
		}
		irr::IrrlichtDevice * getDevice(){ return this->device.get();}
		AScene   * getScene()		{ return &this->aScene;}
		bool isFinished(){
			return thread.finished();
		}
		void operator()() { go(); }

	protected:
		AScene& aScene;
		std::shared_ptr<irr::IrrlichtDevice> device;
		kul::Ref<SceneThread> ref;
		kul::Thread thread;
		void go(){
			thread.sleep(1000);
			this->aScene.setUp(this->getDevice());
		}

		SceneThread();

		void check() throw(SceneThreadException){
			if (!&aScene || !&device){
				throw SceneThreadException(__FILE__, __LINE__, "Scene or device cannot be null");
			}
		}

		// prevent copying and assignment; not implemented
		SceneThread& operator=(const AScene &);
		SceneThread& operator=(irr::IrrlichtDevice &);
};

class GUISceneThread  : public SceneThread{
	public:
		GUISceneThread(AGUIScene & aGUIScene): SceneThread (aGUIScene){}
		~GUISceneThread(){}
		void operator()(){ 
			go(); 
		}
	protected:
		void go(){
			((AGUIScene*)this->getScene())->setUpUI(this->getDevice()->getGUIEnvironment());
		}
};


class DisplayException : public kul::Exception{
	public:
		DisplayException(const char*f, const int l, std::string s) : kul::Exception(f, l, s){}
};

class ADisplayable{
	private:
		uint x1;
		uint y1;
		uint w;
		uint h;
	public:
		ADisplayable(const uint& x, const uint& y, const uint& w, const uint& h) : x1(x) , y1(y), w(w), h(h){}
		ADisplayable() : x1(0), y1(0), w(0), h(0){}
		virtual ~ADisplayable(){}
		virtual void display() throw(DisplayException) = 0;
		uint& x(const uint& x) 		{ return this->x1 = x;}
		uint& y(const uint& y) 		{ return this->y1 = y;}
		uint& width(const uint& w)  { return this->w = w;}
		uint& height(const uint& h) { return this->h = h;}
		const uint& x()		const { return x1;}
		const uint& y()		const { return y1;}
		const uint& width()	const { return w;}
		const uint& height()const { return h;}
};

class AContainer{
	private:
		std::vector<ADisplayable*> es;
	public:
		void add(ADisplayable* d) { es.push_back(d);}
		const std::vector<ADisplayable*>& entities() { return es;}
};

class Application{
};

class Window : public ADisplayable, public AContainer{
	private:
		bool m;
	public:
		Window(const bool& m) : m(m){}
		Window() : m(0){}
		bool& modal(const bool& m){return this->m = m;}
		const bool& modal() const {return m;}
};

class Tab : public ADisplayable{
	public:
		void display() throw(DisplayException){}
};

class Toolbar : public ADisplayable{
	public:
		void display() throw(DisplayException){}
};
class ToolbarItem : public ADisplayable{
	public:
		void display() throw(DisplayException){}
};
class Menu : public ADisplayable{
	public:
		void display() throw(DisplayException){}
};
class MenuItem : public ADisplayable{
	public:
		void display() throw(DisplayException){}
};

class Tree{};
class FileTree : public Tree{};

class Button : public ADisplayable{
	public:
		void display() throw(DisplayException){}
};
class TextButton : public Button{};
class ImgButton : public Button{};
class ToolBarButton : public ImgButton{}; 

class EventException : public kul::Exception{
	public:
		EventException(const char*f, const int l, std::string s) : kul::Exception(f, l, s){}
};

class Event{
	public:
		virtual ~Event(){}
		virtual void act() throw(EventException) = 0;
};

class ButtonClick : public Event{
	public:
		void act() throw(EventException);
};

};
#endif /* _IRRLIGHT_HPP_ */
