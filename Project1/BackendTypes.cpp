#include "BackendTypes.h"

#pragma region Renderable
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
		SDL_Point size{};
		SDL_QueryTexture(tex, NULL, NULL, &size.x, &size.y);
		dest->w = size.x;
		dest->h = size.y;
		//std::cout << "width: " << dest->w;
	}

	Renderable::Renderable(std::string  filename, SDL_Renderer* r, int x = 100, int y = 50) {

		Renderer = r;

		dest = new SDL_Rect;
		//std::string path = R"(Sunkist.png)";
		std::string path = filename;

		SetText(LoadTexture(path));

		SetDest(x - (dest->w / 2), y - (dest->h / 2));
	}

	SDL_Texture* Renderable::LoadTexture(std::string path)
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
#pragma endregion

#pragma region ActiveScript
	ActiveScript::ActiveScript() {};
	void ActiveScript::ProcessEvents(const Uint8* currentKeyStates, double deltaTime) {};
	void ActiveScript::Kill() {};
#pragma endregion

#pragma region -
#pragma endregion

#pragma region TowardZero
	double TowardZero(double num, double strength) {
		return num == 0 || (num < strength ? num * -1 : num) < strength ? 0 : num > strength ? num - strength : num + strength;
		//if num = 0 or abs(num) < str, return 0. num > str, return num-str. else, return num+str.

		//this has started causing passive force and i dont know why
	}

	double TowardZeroD(double num, double strength) {

		double abs = num;

		//std::cout << TowardZero(num, strength) << std::endl;

		if (num < 0) {
			abs = num * -1;
		}
		//std::cout << "abs: " << abs <<"| str: "<< strength << "\n";
		if (abs < strength || num == 0) {
			//std::cout << "return 0 \n";
			return 0;
		}
		if (num < strength) {
			//std::cout << "return num+str \n";
			return num + strength;
		}
		if (num > strength) {
			//std::cout << "return num-str \n";
			return num - strength;
		}
		return -1;
	}
#pragma endregion

Moveable :: Moveable(std::string filename, int x = 100, int y = 50, bool mirror = false, int velX = 40, int velY = 40) : Renderable(filename, Renderer, x, y) {
	thisfilename = filename;

	velocityX = velX;
	velocityY = velY;

	maxVelocityX = 40;
	maxVelocityY = 40;
	/// This is attached to the std::round in Move()
	/// The problem with this is we currently can't move less than 1px a frame
	/// need a function that can decide to rythmically drop frames.

	blockBoarderMovement = true;
	mirrorsAtBoarders = true;
	//leave both of these false to be deleted offscreen
	isMirror = mirror;
	hasMirror = false;
	isInGameZone = true;

	defaultBoarder = 0.3f;
	if (isMirror) {
		boarder = 0;
	}
	else {
		boarder = defaultBoarder;
	}

	RQM.Push(this);
	ASM.Push(this);
}

void Moveable::Update(double deltaTime) {

	velocityX = TowardZeroD(velocityX, 20);
	velocityY = TowardZeroD(velocityY, 20);

#pragma region moving
	dest->x = std::round(dest->x + (velocityX * deltaTime));
	dest->y = std::round(dest->y + (velocityY * deltaTime));
#pragma endregion

#pragma region countering boarder movement
	if (blockBoarderMovement) {
		if ((dest->x <= 0) || (dest->x + dest->w >= screenWidth)) {
			dest->x -= velocityX * deltaTime;
			velocityX = 0;
		}
		if ((dest->y <= 0) || (dest->y + dest->h >= screenHeight)) {
			dest->y -= velocityY * deltaTime;
			velocityY = 0;
		}
	}
#pragma endregion

#pragma region Mirroring

	if (mirrorsAtBoarders && !isMirror && !hasMirror) {
		//std::cout << "check ";
		if (dest->x <= 0 - (dest->w * boarder)) {											   // Left
			//Moveable* plr = new Moveable((dest->w * boarder) + screenWidth + 10, dest->y, true, velocityX, velocityY);
			Moveable* plr = new Moveable(thisfilename, screenWidth - 1, dest->y, true, velocityX, velocityY);
			hasMirror = true;
			isInGameZone = false;
			//std::cout << "left ";
		}
		else if (dest->x + dest->w >= screenWidth + (dest->w * boarder)) {						// Right
			//Moveable* plr = new Moveable((dest->w * boarder) - screenWidth - 10,dest->y,true, velocityX, velocityY);
			Moveable* plr = new Moveable(thisfilename, screenWidth + 1, dest->y, true, velocityX, velocityY);
			hasMirror = true;
			isInGameZone = false;
			//std::cout << "right ";
		}
		else if (dest->y <= 0 - (dest->h * boarder)) {													// Up
			//Moveable* plr = new Moveable(dest->x, (dest->h * boarder) + screenHeight + 1, true, velocityX, velocityY);
			Moveable* plr = new Moveable(thisfilename, dest->x, screenHeight - 1, true, velocityX, velocityY);
			hasMirror = true;
			isInGameZone = false;
			//std::cout << "up ";
		}
		else if (dest->y + dest->h >= screenHeight + (dest->h * boarder)) {								// Down
			//Moveable* plr = new Moveable(dest->x, (dest->h * boarder) - screenHeight - 1, true, velocityX, velocityY);
			Moveable* plr = new Moveable(thisfilename, dest->x, screenHeight + 1, true, velocityX, velocityY);
			hasMirror = true;
			isInGameZone = false;
			//std::cout << "down ";
		}
	}
#pragma endregion

#pragma region ScreenPosition
	if (dest->x > 0 && dest->x + dest->w < screenWidth && dest->y > 0 && dest->y + dest->h < screenHeight) { //detects completly in screen. This is dumb.
		//std::cout << "inside ";
		isInGameZone = true;
		isMirror = false;
		hasMirror = false;
		boarder = defaultBoarder;
	}
	else if (dest->x + dest->w < 0 || dest->x > screenWidth || dest->y + dest->h < 0 || dest->y > screenHeight) { //completly off edge
		SR.Push(this);
		//std::cout << "outside ";
	}
	else {
		//std::cout << "middling ";
	}

	std::cout << std::endl;
#pragma endregion
}

void Moveable::ProcessEvents(const Uint8* currentKeyStates, double deltaTime) {

	if (currentKeyStates[SDL_SCANCODE_UP] || currentKeyStates[SDL_SCANCODE_W])
	{
		velocityY -= maxVelocityY;
	}
	if (currentKeyStates[SDL_SCANCODE_DOWN] || currentKeyStates[SDL_SCANCODE_S])
	{
		velocityY += maxVelocityY;
	}
	if (currentKeyStates[SDL_SCANCODE_LEFT] || currentKeyStates[SDL_SCANCODE_A])
	{
		velocityX -= maxVelocityX;
	}
	if (currentKeyStates[SDL_SCANCODE_RIGHT] || currentKeyStates[SDL_SCANCODE_D])
	{
		velocityX += maxVelocityX;
	}

	Update(deltaTime);
}

void Moveable::Kill() {
	if (isMirror) {
		RQM.PopFront();
		ASM.PopFront();
	}
	else {
		RQM.PopBack();
		ASM.PopBack();
	}
	//std::cout << "being murdered" << std::endl;
	delete this;
}
