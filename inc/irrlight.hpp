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
	private:
		std::shared_ptr<irr::IrrlichtDevice> d;
	protected:
		irr::IrrlichtDevice* device(){ return d.get(); }
	public:
		AScene(irr::IrrlichtDevice* d) : d(d){}
		virtual ~AScene(){}
		virtual void setUp 		(){}
		virtual void tearDown	(){}
		virtual void draw		(){}
		virtual bool OnEvent	(const irr::SEvent& event)	{ return false; }
		virtual bool keyDown	(irr::EKEY_CODE keyCode)	{ return false; }
		virtual bool keyUp		(irr::EKEY_CODE keyCode)	{ return false; }
		virtual void reset 		(irr::IrrlichtDevice* d)	{ this->d.reset(d); }
};

class SceneGraph : public irr::IEventReceiver{
	private:
		std::shared_ptr<AScene> s;
		std::shared_ptr<irr::IrrlichtDevice> d;
		SceneGraph() : s(0), d(irr::createDevice(irr::video::EDT_NULL, irr::core::dimension2d<irr::u32>(512, 384))){}
		static SceneGraph *instance;
	public:
		irr::IrrlichtDevice* 	device 	()	const { return this->d.get();}
		void 					device 	(irr::IrrlichtDevice* d);
		AScene* 				scene 	()	const { return this->s.get();}
		void 					scene 	(AScene* aScene);
		void 					draw 	() const { if(s.get()) s->draw();}
		bool 					OnEvent	(const irr::SEvent& event);
		static SceneGraph& 		INSTANCE(){
			static SceneGraph instance;
			return instance;
		}
};

class ATerminalCmd{
	public:
		virtual ~ATerminalCmd(){}
		virtual void 			perform() 	{};
		virtual void 			perform(const std::vector<std::wstring>& args){};
		virtual std::wstring 	output() { return L"";};
};

class TerminalHistory{
	private:
		static int MINUS_ONE;
		int it;
		std::vector<std::wstring> history;		
	public:
		TerminalHistory() : it(-1){}
		~TerminalHistory(){}
		const wchar_t*	previous();
		const wchar_t*	next();
		TerminalHistory* add(std::wstring currentCommand);
		TerminalHistory* reset() { it = history.size(); return this;}
};

class Terminal{
	private:
		bool s;
		std::vector<std::string> o;
		TerminalHistory th;
		kul::hash::map::S2T<ATerminalCmd> cmds;
		std::shared_ptr<irr::IrrlichtDevice> d;
		irr::gui::IGUIWindow* w;
	protected:
		void rollDown	();
		void rollUp		();
	public:
		Terminal(irr::IrrlichtDevice* d) : s(false), d(d){}		
		void 			draw();
		void 			show()						{ s = true; 	history().reset(); rollDown(); }
		void 			hide()						{ s = false;	rollUp();}
		bool 			shown() const 				{ return s; }
		const wchar_t*	text() const;
		void 			text(const wchar_t* text);
		const void 		add(const std::string& s, const ATerminalCmd& cmd){ 
			cmds.insert(s, cmd); 
		} 
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
			std::wstring textString(TerminalStringHandler::getWStringFromWChar_TP(terminal.text(), 'L', true, true));
			if(textString.size() == 0) return "";
			terminal.text(L"");
			terminal.history().add(textString);
			return kul::WString::toString(textString);
		}
};

class TerminalKeyEntryHandler{
	public:
		static bool keyUp(irr::IrrlichtDevice *device, Terminal& terminal, irr::EKEY_CODE keyCode){
			bool k = false;
			//if(keyCode == irr::KEY_RETURN) k = TerminalEnterKeyHandler(terminal, device).handle();
			if(keyCode == irr::KEY_UP){
				const wchar_t* txt = terminal.history().previous();
				if(txt) terminal.text(txt);				
				k = true;
			}else if(keyCode == irr::KEY_DOWN){
				const wchar_t* txt = terminal.history().next();
				if(!txt) txt = L"";
				terminal.text(txt);
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

class SceneThread{
	public:
		SceneThread(AScene& aScene) throw(SceneThreadException) : aScene(aScene)
			, device(irr::createDevice(irr::video::EDT_NULL, irr::core::dimension2d<irr::u32>(512, 384))), ref(*this), thread(ref){
			this->check();
			thread.run();
		}
		irr::IrrlichtDevice* getDevice(){ return this->device.get();}
		AScene* getScene()				{ return &this->aScene;}
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
			this->aScene.setUp();
		}

		SceneThread();

		void check() throw(SceneThreadException){
			if (!&aScene || !&device){
				KEXCEPT(SceneThreadException, "Scene or device cannot be null");
			}
		}

		// prevent copying and assignment; not implemented
		SceneThread& operator=(const AScene &);
		SceneThread& operator=(irr::IrrlichtDevice &);
};

class DrawException : public kul::Exception{
	public:
		DrawException(const char*f, const int l, std::string s) : kul::Exception(f, l, s){}
};

class ADrawable{
	private:
		uint x1, y1, w, h;
	public:
		ADrawable(const uint& x, const uint& y, const uint& w, const uint& h) : x1(x) , y1(y), w(w), h(h){}
		ADrawable() : x1(0), y1(0), w(0), h(0){}
		virtual ~ADrawable(){}
		virtual void draw(irr::IrrlichtDevice *device) throw(DrawException) = 0;
		void x(const uint& x)		{ this->x1 = x;}
		void y(const uint& y) 		{ this->y1 = y;}
		void width(const uint& w)	{ this->w = w;}
		void height(const uint& h)	{ this->h = h;}
		const uint& x()		const 	{ return x1;}
		const uint& y()		const 	{ return y1;}
		const uint& width()	const 	{ return w;}
		const uint& height()const 	{ return h;}
};

class AContainer{
	private:
		std::vector<ADrawable*> es;
	public:
		void add(ADrawable* d) { es.push_back(d);}
		const std::vector<ADrawable*>& entities() { return es;}
};

class Application : public ADrawable, public AContainer{
	private:
		bool c, m, r;
		uint lFps = -1;
		std::string reg;
	public:	
		Application() : c(0), m(0), r(0){}
		const bool& condensed()					{ return c; }
		void 		condensed(const bool& c) 	{ this->c = c; }
		const bool& minimised()					{ return m; }
		void 		minimised(const bool& m)	{ this->m = m; }
		const bool& resizable()					{ return r; }
		void 		resizable(const bool& r)	{ this->r = r; }
		void draw(irr::IrrlichtDevice *device) throw(DrawException){
			if (device->isWindowActive()){
				irr::video::IVideoDriver * driver = device->getVideoDriver();
				driver->beginScene(true, true, irr::video::SColor(0, 255, 255, 255));		
				
				uint fps = driver->getFPS();
				if(lFps != fps){
					irr::core::stringw c(L"irrwyg in irrlicht - [");
					c += driver->getName() ;
					c += L"] FPS: ";
					c += fps;
					device->setWindowCaption(c.c_str());
				}
				// if(terminal.isShown()) {
				// 	terminal.draw(device);
				// 	device->getGUIEnvironment()->drawAll();
				// }
				else{
					// for(const Quadrant& q : pixels)
					// 	driver->drawPixel(q.x, q.y, irr::video::SColor(255, q.r, q.g, q.b));
				}
				driver->endScene();
			}else{
				device->yield();
			}
		}

};

class Window : public ADrawable, public AContainer{
	private:
		bool m;
	public:
		Window(const bool& m) : m(m){}
		Window() : m(0){}
		bool& modal(const bool& m){return this->m = m;}
		const bool& modal() const {return m;}
};

class Tab : public ADrawable{
	public:
		void draw() throw(DrawException){}
};

class Toolbar : public ADrawable{
	public:
		void draw(irr::IrrlichtDevice *device) throw(DrawException){}
};
class ToolbarItem : public ADrawable{
	public:
		void draw(irr::IrrlichtDevice *device) throw(DrawException){}
};
class Menu : public ADrawable{
	public:
		void draw(irr::IrrlichtDevice *device) throw(DrawException){}
};
class MenuItem : public ADrawable{
	public:
		void draw(irr::IrrlichtDevice *device) throw(DrawException){}
};

class Tree{};
class FileTree : public Tree{};

class Button : public ADrawable{
	public:
		void draw(irr::IrrlichtDevice *device) throw(DrawException){}
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
