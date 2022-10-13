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

#include "mkn/kul/map.hpp"
#include "mkn/kul/string.hpp"
#include "mkn/kul/wstring.hpp"
#include "mkn/kul/threads.hpp"

#include "irrlichtmt/irrlicht.h"

namespace irrlight {

enum TerminalUI {
  COMMAND_BOX,
  COMMAND__HISTORY_BOX,
};

class AScene {
 public:
  AScene(irr::IrrlichtDevice* d) : d(d) {}
  virtual ~AScene() {}
  virtual void setUp() {}
  virtual void tearDown() {}
  virtual void draw() {}
  virtual bool OnEvent(irr::SEvent const& event) { return false; }
  virtual bool keyDown(irr::EKEY_CODE keyCode) { return false; }
  virtual bool keyUp(irr::EKEY_CODE keyCode) { return false; }
  virtual void reset(irr::IrrlichtDevice* d) { this->d = nullptr; }

 protected:
  auto& device() { return d; }

 private:
  irr::IrrlichtDevice* d;
};

class SceneGraph : public irr::IEventReceiver {
 public:
  static SceneGraph& INSTANCE() {
    static SceneGraph instance;
    return instance;
  }

  auto& device() const { return this->d; }

  void device(irr::IrrlichtDevice* d) { this->d = d; }

  AScene* scene() const { return this->s.get(); }

  void scene(AScene* aScene) {
    if (this->s.get()) {
      this->s->tearDown();
    }
    this->s.reset(aScene);
    aScene->setUp();
  }

  void draw() const {
    if (s.get()) s->draw();
  }

  bool OnEvent(irr::SEvent const& event) {
    if (s.get()) {
      if (event.EventType == irr::EET_KEY_INPUT_EVENT && event.KeyInput.PressedDown)
        s->keyDown(event.KeyInput.Key);
      else if (event.EventType == irr::EET_KEY_INPUT_EVENT && !event.KeyInput.PressedDown)
        s->keyUp(event.KeyInput.Key);
      return s->OnEvent(event);
    }
    return false;
  }

 private:
  std::shared_ptr<AScene> s;
  irr::IrrlichtDevice* d;
};

class ATerminalCmd {
 public:
  virtual ~ATerminalCmd() {}
  virtual void perform(){};
  virtual void perform(std::vector<std::wstring> const& args){};
  virtual std::wstring output() { return L""; };
};

class TerminalHistory {
 public:
  wchar_t const* previous();
  wchar_t const* next();
  TerminalHistory* add(std::wstring currentCommand);
  TerminalHistory* reset() {
    it = history.size();
    return this;
  }

 private:
  static constexpr std::int32_t MINUS_ONE = -1;

  int it = -1;
  std::vector<std::wstring> history;
};

class Terminal {
 public:
  Terminal(irr::IrrlichtDevice* d) : d(d) {}
  void draw();
  void show() {
    s = true;
    history().reset();
    rollDown();
  }
  void hide() {
    s = false;
    rollUp();
  }
  bool shown() const { return s; }
  wchar_t const* text() const;
  void text(wchar_t const* text);
  const void add(std::string const& s, ATerminalCmd const& cmd) { cmds.insert(s, cmd); }
  mkn::kul::hash::map::S2T<ATerminalCmd>& commands() { return cmds; }
  TerminalHistory& history() { return th; }

 protected:
  void rollDown();
  void rollUp();

 private:
  bool s = false;
  std::vector<std::string> o;
  TerminalHistory th;
  mkn::kul::hash::map::S2T<ATerminalCmd> cmds;
  irr::IrrlichtDevice* d;
  irr::gui::IGUIWindow* w;
};

class TerminalStringHandler {
 public:
  static std::wstring getWStringFromWChar_TP(wchar_t const* cText, char upperLower = ' ',
                                             bool trim = false, bool replaceDoubleSpace = false) {
    irr::core::stringw text(cText);
    if (upperLower == 'U' || upperLower == 'u')
      text.make_upper();
    else if (upperLower == 'L' || upperLower == 'l')
      text.make_lower();
    std::wstring textString(text.c_str());
    std::string tS = mkn::kul::WString::toString(textString);
    mkn::kul::String::TRIM(tS);
    if (trim) mkn::kul::WString::trim(textString);
    if (replaceDoubleSpace) {
      wchar_t const* ds{L"  "};
      wchar_t const* ss{L" "};
      mkn::kul::WString::replaceAll(textString, std::wstring(ds), std::wstring(ss));
    }
    return textString;
  }
};

class TerminalEnterKeyHandler {
 public:
  static std::string handle(Terminal& terminal, irr::IrrlichtDevice* device) {
    std::wstring textString(
        TerminalStringHandler::getWStringFromWChar_TP(terminal.text(), 'L', true, true));
    if (textString.size() == 0) return "";
    terminal.text(L"");
    terminal.history().add(textString);
    return mkn::kul::WString::toString(textString);
  }
};

class TerminalKeyEntryHandler {
 public:
  static bool keyUp(irr::IrrlichtDevice* device, Terminal& terminal, irr::EKEY_CODE keyCode) {
    bool k = false;
    // if(keyCode == irr::KEY_RETURN) k = TerminalEnterKeyHandler(terminal,
    // device).handle();
    if (keyCode == irr::KEY_UP) {
      wchar_t const* txt = terminal.history().previous();
      if (txt) terminal.text(txt);
      k = true;
    } else if (keyCode == irr::KEY_DOWN) {
      wchar_t const* txt = terminal.history().next();
      if (!txt) txt = L"";
      terminal.text(txt);
      k = true;
    }
    return k;
  }
  static bool keyDown(irr::IrrlichtDevice* device, Terminal& terminal, irr::EKEY_CODE keyCode) {
    return false;
  }
};

class SceneThreadException : public mkn::kul::Exception {
 public:
  SceneThreadException(const char* f, const int l, std::string s) : mkn::kul::Exception(f, l, s) {}
};

class SceneThread {
 public:
  SceneThread(AScene& aScene) KTHROW(SceneThreadException)
      : aScene(aScene),
        device(irr::createDevice(irr::video::EDT_NULL, irr::core::dimension2d<irr::u32>(512, 384))),
        ref(*this),
        thread(ref) {
    this->check();
    thread.run();
  }
  irr::IrrlichtDevice* getDevice() { return this->device.get(); }
  AScene* getScene() { return &this->aScene; }
  bool isFinished() { return thread.finished(); }
  void operator()() { go(); }

 protected:
  AScene& aScene;
  std::shared_ptr<irr::IrrlichtDevice> device;
  std::reference_wrapper<SceneThread> ref;
  mkn::kul::Thread thread;
  void go() {
    mkn::kul::this_thread::sleep(1000);
    this->aScene.setUp();
  }

  SceneThread();

  void check() KTHROW(SceneThreadException) {
    if (!&aScene || !&device) {
      KEXCEPT(SceneThreadException, "Scene or device cannot be null");
    }
  }

  // prevent copying and assignment; not implemented
  SceneThread& operator=(const AScene&);
  SceneThread& operator=(irr::IrrlichtDevice&);
};

class DrawException : public mkn::kul::Exception {
 public:
  DrawException(const char* f, const int l, std::string s) : mkn::kul::Exception(f, l, s) {}
};

class ADrawable {
 private:
  uint x1, y1, w, h;

 public:
  ADrawable(const uint& x, const uint& y, const uint& w, const uint& h)
      : x1(x), y1(y), w(w), h(h) {}
  ADrawable() : x1(0), y1(0), w(0), h(0) {}
  virtual ~ADrawable() {}
  virtual void draw(irr::IrrlichtDevice* device) KTHROW(DrawException) = 0;
  void x(const uint& x) { this->x1 = x; }
  void y(const uint& y) { this->y1 = y; }
  void width(const uint& w) { this->w = w; }
  void height(const uint& h) { this->h = h; }
  const uint& x() const { return x1; }
  const uint& y() const { return y1; }
  const uint& width() const { return w; }
  const uint& height() const { return h; }
};

class AContainer {
 private:
  std::vector<ADrawable*> es;

 public:
  void add(ADrawable* d) { es.push_back(d); }
  const std::vector<ADrawable*>& entities() { return es; }
};

class Application : public ADrawable, public AContainer {
 private:
  bool c, m, r;
  uint lFps = -1;
  std::string reg;

 public:
  Application() : c(0), m(0), r(0) {}
  const bool& condensed() { return c; }
  void condensed(const bool& c) { this->c = c; }
  const bool& minimised() { return m; }
  void minimised(const bool& m) { this->m = m; }
  const bool& resizable() { return r; }
  void resizable(const bool& r) { this->r = r; }
  void draw(irr::IrrlichtDevice* device) KTHROW(DrawException) {
    if (device->isWindowActive()) {
      irr::video::IVideoDriver* driver = device->getVideoDriver();
      driver->beginScene(true, true, irr::video::SColor(0, 255, 255, 255));

      uint fps = driver->getFPS();
      if (lFps != fps) {
        irr::core::stringw c(L"irrwyg in irrlicht - [");
        c += driver->getName();
        c += L"] FPS: ";
        c += fps;
        device->setWindowCaption(c.c_str());
      }
      // if(terminal.isShown()) {
      // 	terminal.draw(device);
      // 	device->getGUIEnvironment()->drawAll();
      // }
      else {
        // for(const Quadrant& q : pixels)
        // 	driver->drawPixel(q.x, q.y, irr::video::SColor(255, q.r, q.g,
        // q.b));
      }
      driver->endScene();
    } else {
      device->yield();
    }
  }
};

class Window : public ADrawable, public AContainer {
 private:
  bool m;

 public:
  Window(const bool& m) : m(m) {}
  Window() : m(0) {}
  bool& modal(const bool& m) { return this->m = m; }
  const bool& modal() const { return m; }
};

class Tab : public ADrawable {
 public:
  void draw() KTHROW(DrawException) {}
};

class Toolbar : public ADrawable {
 public:
  void draw(irr::IrrlichtDevice* device) KTHROW(DrawException) {}
};
class ToolbarItem : public ADrawable {
 public:
  void draw(irr::IrrlichtDevice* device) KTHROW(DrawException) {}
};
class Menu : public ADrawable {
 public:
  void draw(irr::IrrlichtDevice* device) KTHROW(DrawException) {}
};
class MenuItem : public ADrawable {
 public:
  void draw(irr::IrrlichtDevice* device) KTHROW(DrawException) {}
};

class Tree {};
class FileTree : public Tree {};

class Button : public ADrawable {
 public:
  void draw(irr::IrrlichtDevice* device) KTHROW(DrawException) {}
};
class TextButton : public Button {};
class ImgButton : public Button {};
class ToolBarButton : public ImgButton {};

class EventException : public mkn::kul::Exception {
 public:
  EventException(const char* f, const int l, std::string s) : mkn::kul::Exception(f, l, s) {}
};

class Event {
 public:
  virtual ~Event() {}
  virtual void act() KTHROW(EventException) = 0;
};

class ButtonClick : public Event {
 public:
  void act() KTHROW(EventException);
};

};     // namespace irrlight
#endif /* _IRRLIGHT_HPP_ */
