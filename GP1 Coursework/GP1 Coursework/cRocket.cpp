/*
=================
cRocket.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cRocket.h"
#include "cFontMgr.h"
#include <sstream>


//The score variable to be displayed on screen
int score = 0;

//To convert the int to a character
char buffer[20];
//The result of the conversion
LPCSTR result;

void cRocket::render()
{
	glPushMatrix();

	glTranslatef(spritePos2D.x, spritePos2D.y, 0.0f);
	glRotatef(spriteRotation, 0.0f, 0.0f, 1.0f);
	glScalef(spriteScaling.x, spriteScaling.y, 1.0f);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, GLTextureID); // Binding of GLtexture name 

	glBegin(GL_QUADS);
	glColor3f(255.0f, 255.0f, 255.0f);
	glTexCoord2f(spriteTexCoordData[0].x, spriteTexCoordData[0].y);
	glVertex2f(-(textureWidth / 2), -(textureHeight / 2));
	glTexCoord2f(spriteTexCoordData[1].x, spriteTexCoordData[1].y);
	glVertex2f((textureWidth / 2), -(textureHeight / 2));
	glTexCoord2f(spriteTexCoordData[2].x, spriteTexCoordData[2].y);
	glVertex2f((textureWidth / 2), (textureHeight / 2));
	glTexCoord2f(spriteTexCoordData[3].x, spriteTexCoordData[3].y);
	glVertex2f(-(textureWidth / 2), (textureHeight / 2));

	glEnd();
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}
/*
=================================================================
Update the sprite position
=================================================================
*/

void cRocket::update(float deltaTime)
{


	static cFontMgr* theFontMgr = cFontMgr::getInstance();

	//Takes the players input and moves the ship left or right depnding on the input otherwise the ship stays still.
	if (m_InputMgr->isKeyDown(VK_RIGHT))
	{
		spriteTranslation = (glm::vec2(25.0f, 0.0f));
	}
	else
	{
		spriteTranslation = (glm::vec2(0.0f, 0.0f));
	}
	if (m_InputMgr->isKeyDown(VK_LEFT))
	{
		spriteTranslation = -(glm::vec2(25.0f, 0.0f));
	}

	if (m_InputMgr->isKeyDown(VK_SPACE))
	{
		// Add new bullet sprite to the vector array
		theRocketBullets.push_back(new cBullet);
		int numBullets = theRocketBullets.size() - 1;
		theRocketBullets[numBullets]->setSpritePos(glm::vec2(spritePos2D.x , spritePos2D.y - textureHeight + 25.0f));
		theRocketBullets[numBullets]->setSpriteTranslation(glm::vec2(2.0f, 100.0f));
		theRocketBullets[numBullets]->setTexture(theGameTextures[4]->getTexture());
		theRocketBullets[numBullets]->setTextureDimensions(theGameTextures[4]->getTWidth(), theGameTextures[4]->getTHeight());
		theRocketBullets[numBullets]->setSpriteCentre();
		theRocketBullets[numBullets]->setBulletVelocity(glm::vec2(0.0f, 0.0f));
		theRocketBullets[numBullets]->setSpriteRotation(getSpriteRotation());
		theRocketBullets[numBullets]->setActive(true);
		theRocketBullets[numBullets]->setMdlRadius();
		// play the firing sound
		m_SoundMgr->getSnd("Shoot")->playAudio(AL_TRUE);		
	}


	glm::vec2 spriteVelocityAdd = glm::vec2(0.0f, 0.0f);

	spriteVelocityAdd.x += spriteTranslation.x;
	spriteVelocityAdd.y += spriteTranslation.y;

	rocketVelocity += spriteVelocityAdd;

	spritePos2D += rocketVelocity * deltaTime;

	rocketVelocity *= 0.95;

	/*
	==============================================================
	| Check for collisions
	==============================================================
	*/
	for (vector<cBullet*>::iterator bulletIterartor = theRocketBullets.begin(); bulletIterartor != theRocketBullets.end(); ++bulletIterartor)
	{
		(*bulletIterartor)->update(deltaTime);
		for (vector<cAsteroid*>::iterator asteroidIterator = theAliens.begin(); asteroidIterator != theAliens.end(); ++asteroidIterator)
		{
			if ((*asteroidIterator)->collidedWith((*asteroidIterator)->getBoundingRect(), (*bulletIterartor)->getBoundingRect()))
			{
				// if a collision set the bullet and asteroid to false
				(*asteroidIterator)->setActive(false);
				(*bulletIterartor)->setActive(false);
			}
		}
	}

	//Converts the score int variable to a LPCSTR format so it can be displayed on the screen.
	_itoa_s(score, buffer, 20, 10);
	result = buffer;

	//Displays the score with the specified font.
	theFontMgr->getFont("SpaceInvaders")->printText(result, FTPoint(150, -24, 0.0f));

	vector<cBullet*>::iterator bulletIterartor = theRocketBullets.begin();
	while (bulletIterartor != theRocketBullets.end())
	{
		if ((*bulletIterartor)->isActive() == false)
		{
			bulletIterartor = theRocketBullets.erase(bulletIterartor);
			// play the Invader Killed sound.
			m_SoundMgr->getSnd("InvaderKilled")->playAudio(AL_TRUE);
			score += 2;
		}
		else
		{
			//(*bulletIterartor)->update(deltaTime);
			(*bulletIterartor)->render();
			++bulletIterartor;
		}
	}
}
/*
=================================================================
  Sets the velocity for the rocket
=================================================================
*/
void cRocket::setRocketVelocity(glm::vec2 rocketVel)
{
	rocketVelocity = rocketVel;
}
/*
=================================================================
  Gets the rocket velocity
=================================================================
*/
glm::vec2 cRocket::getRocketVelocity()
{
	return rocketVelocity;
}
