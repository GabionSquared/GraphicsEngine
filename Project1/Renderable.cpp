#include "Renderable.h"

SDL_Texture* LoadTexture(std::string path)
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(Renderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}
	SDL_RenderCopy(Renderer, newTexture, NULL, NULL);

	//SDL_DestroyTexture( Texture );

	return newTexture;
}

/*
class Renderable {

public:
	SDL_Rect* dest = new SDL_Rect;
	SDL_Texture* Texture;

}
*/

SDL_Rect* Renderable::GetDest() {
	return dest;
}

std::string Renderable::GetDestVerbose() {
	return "x: " + std::to_string(dest->x) + "   y: " + std::to_string(dest->y);
}

void Renderable::SetDest(int x, int y) {
	dest->x = x;
	dest->y = y;
}

SDL_Texture* Renderable::GetText() {
	return Texture;
}

void Renderable::SetText(SDL_Texture* tex) {
	Texture = tex;
	SDL_Point size;
	SDL_QueryTexture(tex, NULL, NULL, &size.x, &size.y);
	dest->w = size.x;
	dest->h = size.y;
	//std::cout << "width: " << dest->w;
}

Renderable::Renderable(std::string  filename, int x = 100, int y = 50) {
	dest = new SDL_Rect;

	//std::string path = R"(Sunkist.png)";
	std::string path = filename;

	SetText(LoadTexture(path));

	SetDest(x - (dest->w / 2), y - (dest->h / 2));
}