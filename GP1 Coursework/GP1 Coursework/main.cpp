#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#define GLX_GLXEXT_LEGACY //Must be declared so that our local glxext.h is picked up, rather than the system one


#include <windows.h>
#include "windowOGL.h"
#include "GameConstants.h"
#include "cWNDManager.h"
#include "cInputMgr.h"
#include "cSoundMgr.h"
#include "cFontMgr.h"
#include "cSprite.h"
#include "asteroidsGame.h"
#include "cRocket.h"
#include <sstream>


int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR cmdLine,
                   int cmdShow)
{
	//To convert the int to a character
	char buffer[20];
	//The result of the conversion
	LPCSTR result;


    //Set our window settings
    const int windowWidth = 1024;
    const int windowHeight = 768;
    const int windowBPP = 16;

	bool MAINMENU = true;
	bool PLAYING = false;
	bool ENDED = false;



    //This is our window
	static cWNDManager* pgmWNDMgr = cWNDManager::getInstance();

	// This is the input manager
	static cInputMgr* theInputMgr = cInputMgr::getInstance();

	// This is the sound manager
	static cSoundMgr* theSoundMgr = cSoundMgr::getInstance();

	// This is the Font manager
	static cFontMgr* theFontMgr = cFontMgr::getInstance();

	//The example OpenGL code
    windowOGL theOGLWnd;
	
    //Attach our the OpenGL window
	pgmWNDMgr->attachOGLWnd(&theOGLWnd);

	// Attach the keyboard manager
	pgmWNDMgr->attachInputMgr(theInputMgr);

    //Attempt to create the window
	if (!pgmWNDMgr->createWND(windowWidth, windowHeight, windowBPP))
    {
        //If it fails

        MessageBox(NULL, "Unable to create the OpenGL Window", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); //Reset the display and exit
        return 1;
    }

	if (!theOGLWnd.initOGL(windowWidth, windowHeight)) //Initialize our example
    {
        MessageBox(NULL, "Could not initialize the application", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); //Reset the display and exit
        return 1;
    }

	//Clear key buffers
	theInputMgr->clearBuffers(theInputMgr->KEYS_DOWN_BUFFER | theInputMgr->KEYS_PRESSED_BUFFER);

	/* initialize random seed: */
	srand((unsigned int)time(NULL));

	// Create vector array of textures
	LPCSTR texturesToUse[] = { "Images\\Alien.jpg", "Images\\asteroid2.png", "Images\\asteroid3.png", "Images\\asteroid4.png", "Images\\bullet.png" };
	for (int tCount = 0; tCount < 5; tCount++)
	{
		theGameTextures.push_back(new cTexture());
		theGameTextures[tCount]->createTexture(texturesToUse[tCount]);
	}

	// load game sounds
	// Load Sound
	LPCSTR gameSounds[3] = { "Audio/themeMusic.wav", "Audio/shoot.wav", "Audio/invaderkilled.wav" };

	theSoundMgr->add("ThemeMusic", gameSounds[0]);
	theSoundMgr->add("Shoot", gameSounds[1]);
	theSoundMgr->add("InvaderKilled", gameSounds[2]);


	// load game fontss
	// Load Fonts
	LPCSTR gameFonts[1] = { "Fonts/ca.ttf" };

	theFontMgr->addFont("SpaceInvaders", gameFonts[0], 32);

	// Create vector array of textures

	for (int alien = 0; alien < 15; alien++)
	{
		theAliens.push_back(new cAsteroid);
		theAliens[alien]->setSpritePos(glm::vec2(800/ (rand() % 5 + 1), 600 / (rand() % 5 + 1)));
		theAliens[alien]->setSpriteTranslation(glm::vec2(0.0f, 0.0f));
		//int randAsteroid = rand() % 4;
		theAliens[alien]->setTexture(theGameTextures[0]->getTexture());
		theAliens[alien]->setTextureDimensions(theGameTextures[0]->getTWidth(), theGameTextures[0]->getTHeight());
		theAliens[alien]->setSpriteCentre();
		theAliens[alien]->setAsteroidVelocity(glm::vec2(glm::vec2(0.0f, 0.0f)));
		theAliens[alien]->setActive(true);
		theAliens[alien]->setMdlRadius();
	}


	cTexture textureBkgd;
	textureBkgd.createTexture("Images\\GameBackground.png");
	cBkGround spriteBkgd;
	spriteBkgd.setSpritePos(glm::vec2(0.0f, 0.0f));
	spriteBkgd.setTexture(textureBkgd.getTexture());
	spriteBkgd.setTextureDimensions(textureBkgd.getTWidth(), textureBkgd.getTHeight());

	cTexture menuBackground;
	menuBackground.createTexture("Images\\MainMenu.png");
	cBkGround mainMenuSprite;
	mainMenuSprite.setSpritePos(glm::vec2(0.0f, 0.0f));
	mainMenuSprite.setTexture(menuBackground.getTexture());
	mainMenuSprite.setTextureDimensions(menuBackground.getTWidth(), menuBackground.getTHeight());

	cTexture endMenu;
	endMenu.createTexture("Images\\EndMenu.png");
	cBkGround endMenuSprite;
	endMenuSprite.setSpritePos(glm::vec2(0.0f, 0.0f));
	endMenuSprite.setTexture(endMenu.getTexture());
	endMenuSprite.setTextureDimensions(endMenu.getTWidth(), endMenu.getTHeight());

	cTexture rocketTxt;
	rocketTxt.createTexture("Images\\Ship.png");
	cRocket rocketSprite;
	rocketSprite.attachInputMgr(theInputMgr); // Attach the input manager to the sprite
	rocketSprite.setSpritePos(glm::vec2(512.0f, 700.0f));
	rocketSprite.setTexture(rocketTxt.getTexture());
	rocketSprite.setTextureDimensions(rocketTxt.getTWidth(), rocketTxt.getTHeight());
	rocketSprite.setSpriteCentre();
	rocketSprite.setRocketVelocity(glm::vec2(0.0f, 0.0f));

	// Attach sound manager to rocket sprite
	rocketSprite.attachSoundMgr(theSoundMgr);

	//Attach sound manager to rocket sprite
	spriteBkgd.attachSoundMgr(theSoundMgr);

	//Attach sound manager to mainMenuSprite
	mainMenuSprite.attachSoundMgr(theSoundMgr);

	//Attach sound manaer to endMenuSprite
	endMenuSprite.attachSoundMgr(theSoundMgr);

	theSoundMgr->getSnd("ThemeMusic")->playAudio(AL_TRUE);



    //This is the mainloop, we render frames until isRunning returns false
	while (pgmWNDMgr->isWNDRunning())
    {

		pgmWNDMgr->processWNDEvents(); //Process any window events

        //We get the time that passed since the last frame
		float elapsedTime = pgmWNDMgr->getElapsedSeconds();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Checks if the current scene is the main menu then checks if the player has pressed the Play button and then transitions to the game scene
		if (MAINMENU)
		{
			mainMenuSprite.render();

			if (theInputMgr->wasKeyPressed(VK_RETURN))
			{
				MAINMENU = false;
				PLAYING = true;

				rocketSprite.score = 0;
			}

			for (int alien = 0; alien < 15; alien++)
			{
				theAliens.push_back(new cAsteroid);
				theAliens[alien]->setSpritePos(glm::vec2(800 / (rand() % 5 + 1), 600 / (rand() % 5 + 1)));
				theAliens[alien]->setSpriteTranslation(glm::vec2(0.0f, 0.0f));
				//int randAsteroid = rand() % 4;
				theAliens[alien]->setTexture(theGameTextures[0]->getTexture());
				theAliens[alien]->setTextureDimensions(theGameTextures[0]->getTWidth(), theGameTextures[0]->getTHeight());
				theAliens[alien]->setSpriteCentre();
				theAliens[alien]->setAsteroidVelocity(glm::vec2(glm::vec2(0.0f, 0.0f)));
				theAliens[alien]->setActive(true);
				theAliens[alien]->setMdlRadius();
			}
		}

		//Checks if the current scene is the game scene and once all enemies are dead it transitions to the end menu
		if (PLAYING)
		{
			spriteBkgd.render();
			rocketSprite.update(elapsedTime);
			vector<cAsteroid*>::iterator asteroidIterator = theAliens.begin();
			while (asteroidIterator != theAliens.end())
			{
				if ((*asteroidIterator)->isActive() == false)
				{
					asteroidIterator = theAliens.erase(asteroidIterator);
				}
				else
				{
					(*asteroidIterator)->update(elapsedTime);
					(*asteroidIterator)->render();
					++asteroidIterator;
				}
			}

			if (theInputMgr->wasKeyPressed(VK_TAB))
			{
				ENDED = true;
				PLAYING = false;
			}
			
			rocketSprite.render();
			theFontMgr->getFont("SpaceInvaders")->printText("Score: ", FTPoint(0, -24, 0.0f));
		}


		//Checks that the current scene is the end menu and then checks if the player has pressed Replay and then loads the mainmenu again
		if (ENDED)
		{
			endMenuSprite.render();

			//Converts the score int variable to a LPCSTR format so it can be displayed on the screen.
			_itoa_s(rocketSprite.score, buffer, 20, 10);
			result = buffer;

			//Displays the score with the specified font.
			theFontMgr->getFont("SpaceInvaders")->printText(result, FTPoint(690, -445, 0.0f));

			if (theInputMgr->wasKeyPressed(VK_RETURN))
			{
				ENDED = false;
				MAINMENU = true;
			}
			
		}

			pgmWNDMgr->swapBuffers();
			theInputMgr->clearBuffers(theInputMgr->KEYS_DOWN_BUFFER | theInputMgr->KEYS_PRESSED_BUFFER);
    }

	theOGLWnd.shutdown(); //Free any resources
	pgmWNDMgr->destroyWND(); //Destroy the program window

    return 0; //Return success
}
