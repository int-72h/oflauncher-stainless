#include "OFSGuiImage.h"

//####### OFSGuiImage ############
OFSGuiImage::OFSGuiImage(const std::string &image_file, SDL_Renderer *renderer,
						 const int &x = 0, const int &y = 0,
						 const int &NumOfSubImages = 0) {
	_subImages = NumOfSubImages;
	int w, h;
	SDL_Surface *textureSurface = SDL_LoadBMP(image_file.c_str());
	if(textureSurface == nullptr)
		throw SDLException("OFSGuiImage");
	_texture = SDL_CreateTextureFromSurface(renderer, textureSurface);
	if(_texture == nullptr)
		throw SDLException("OFSGuiImage");

	SDL_QueryTexture(_texture, nullptr, nullptr, &w, &h);
	_size.h = h / (_subImages + 1);
	_size.w = w;
	_size.x = x;
	_size.y = y;

	_src.h = h / (_subImages + 1);
	_src.w = w;
	_src.x = 0;
	_src.y = 0;

	SDL_FreeSurface(textureSurface);
}

OFSGuiImage::~OFSGuiImage() {
	SDL_DestroyTexture(_texture);
}

void OFSGuiImage::setIndex(const int &i) {
	if(i >= 0 && i <= _subImages)
		_src.y = _size.h * i;
}

void OFSGuiImage::renderCopy(SDL_Renderer *renderer) {
	SDL_RenderCopy(renderer, _texture, &_src, &_size);
}

void OFSGuiImage::getClickedDown() {
}

GuiActs OFSGuiImage::getClickedUp() {
	return NOT_CLICKED;
}

void OFSGuiImage::getHover() {
}