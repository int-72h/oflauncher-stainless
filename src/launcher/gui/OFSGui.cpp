//
// Created by walaryne on 6/14/20.
//

#include "OFSGui.h"
#include "OFSGuiActs.h"

//########### OFSGui ############
OFSGui::OFSGui() {
#ifndef INCLUDE_RESOURCES
	_orig_path = fs::current_path();
#endif
	_quit = false;
	_actStates.emplace(NOT_CLICKED, false);

	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
		throw SDLException("Can't Init SDL");

	_window =
		SDL_CreateWindow("Open Fortress Launcher", SDL_WINDOWPOS_UNDEFINED,
						 SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT,
						 SDL_WINDOW_SHOWN | SDL_WINDOW_UTILITY);
	// On my computer, I have "unredir-if-possible" enabled in my picom
	// config, and this program causes that to trigger unless you have the
	// SDL_WINDOW_UTILIY flag lol
	if(!_window)
		throw SDLException("Cannot create window");

	_renderer = SDL_CreateRenderer(
		_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if(_renderer == nullptr)
		throw SDLException("Can't create renderer");

	SDL_SetRenderDrawColor(_renderer, 0xFF, 0x00, 0xFF, 0xFF);
	if(TTF_Init() == -1)
		throw SDLTTFException("Can't Init SDL_ttf");

	preInstallLayout();
}

OFSGui::~OFSGui() {
	_elements.clear();
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
	SDL_Quit();
}

bool OFSGui::ifActivity(GuiActs actToCheck) {
	bool ret = _actStates[actToCheck];
	_actStates[actToCheck] = false;
	return ret;
}
GuiActs OFSGui::getLastAct() {
	GuiActs ga = _currAct;
	_currAct = NOT_CLICKED;
	return ga;
}
void OFSGui::bindActivity(GuiActs actToBind, GuiButtonMethod funcPoint) {
	_bindMeths.emplace(actToBind, funcPoint);
}

bool OFSGui::simulateButton(GuiActs actToSim) {
	bool ret = false;
	if(_actStates.count(actToSim))
		_actStates[actToSim] = true;
	else
		_actStates.emplace(actToSim, true);
	if(_bindMeths[actToSim]) {
#ifndef INCLUDE_RESOURCES
		fs::current_path(_orig_path);
#endif
		_bindMeths[actToSim](this);
		ret = true;
#ifndef INCLUDE_RESOURCES
		fs::current_path(_new_path);
#endif
	}
	return ret;
}

// void OFSGui::setProgress(const float &progress) {
// 	for(auto &i : _imgs)
// 		i->setProgress(progress);
// }

bool OFSGui::loop() {
#ifndef INCLUDE_RESOURCES
	_new_path = fs::current_path();
#endif
	SDL_Event e;

	// SDL_UpdateWindowSurface(window); // depricated.  delete later if need to

	SDL_RenderClear(_renderer);
	for(auto &i : _elements) {
		i->render(_renderer);
	}
	SDL_RenderPresent(_renderer);

	while(SDL_PollEvent(&e)) {
		switch(e.type) {
		case SDL_QUIT:
			_quit = true;
			break;
		case SDL_MOUSEBUTTONDOWN:
			for(auto &i : _elements) {
				int x, y;
				SDL_GetMouseState(&x, &y);
				std::shared_ptr<MouseGuiEvent> ev = std::make_shared<MouseGuiEvent>();
				ev->type = MOUSE_DOWN;
				ev->x = x;
				ev->y = y;
				i->onEvent(ev);
			}
			break;
		case SDL_MOUSEBUTTONUP:
			for(auto &i : _elements) {
				int x, y;
				SDL_GetMouseState(&x, &y);
				std::shared_ptr<MouseGuiEvent> ev = std::make_shared<MouseGuiEvent>();
				ev->type = MOUSE_UP;
				ev->x = x;
				ev->y = y;
				i->onEvent(ev);
			}
			break;
		case SDL_MOUSEMOTION:
			for(auto &i : _elements) {
				int x, y;
				SDL_GetMouseState(&x, &y);
				std::shared_ptr<MouseGuiEvent> ev = std::make_shared<MouseGuiEvent>();
				ev->type = MOUSE_MOVE;
				ev->x = x;
				ev->y = y;
				i->onEvent(ev);
			}
			break;
		default:
			printf("Unhandled Event!\n");
			// for(auto &i : _elements) {
			// 	// i->getHover();
			// }
			break;
		}
	}
	SDL_PumpEvents();

	return !_quit;
}

void OFSGui::addImage(resData data, const int &x, const int &y,
					  const int &NumOfSubImages) {
	_elements.emplace_back(
		new OFSGuiImage(data, _renderer, x, y, NumOfSubImages));
}

void OFSGui::addButton(resData data, GuiActs actToLink, const int &x = 0,
					   const int &y = 0, const int &NumOfSubImages = 0) {
	_elements.emplace_back(new OFSGuiButton(data, _renderer, actToLink,
												   x, y, NumOfSubImages));
}

void OFSGui::addText(const std::string &text, const int &text_size,
					 const int &x, const int &y) {
	_elements.emplace_back(
		new OFSGuiText(_renderer, text, text_size, x, y));
}
// void OFSGui::addTextEntry(const std::string &text, const int &x, const int &y,
// 						  const int &width) {
// 	_imgs.push_back(
// 		std::make_unique<OFSGuiTextEntry>(_renderer, text, x, y, width));
// }
// void OFSGui::addSpinny(resData data, const int &x, const int &y) {
// 	_imgs.push_back(std::make_unique<OFSGuiSpinny>(data, _renderer, x, y));
// }

// void OFSGui::addProgressBar(resData data, const int &x, const int &y,
// 							const int &width) {
// 	_imgs.push_back(
// 		std::make_unique<OFSGuiProgBar>(data, _renderer, x, y, width));
// }

// void OFSGui::setLastIndex(const int &i) {
// 	_imgs.back()->setIndex(i);
// }

void OFSGui::clearLayout() {
	_elements.clear();
	// bindFuncs.clear();
	_bindMeths.clear();
}
