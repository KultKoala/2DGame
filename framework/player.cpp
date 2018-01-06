//TODO add refactory period for roll
//TODO cut last couple of frames off of both rolls. Remember to fix xml


#include "player.h"
#include "gamedata.h"
#include "renderContext.h"
#include "helperFunctions.h"


void Player::advanceFrame(Uint32 ticks) {
	updateCurrentAnim();
	timeSinceLastFrame += ticks;

	if(exp){
		currentFrame = currentFrame+1;
		return;
	}

	if (timeSinceLastFrame > animInterval[playerAnim::ROLLLEFT] && pState ==playerState::ROLLING) {
		currentFrame = (currentFrame+1);
		if(currentFrame >= animMap[playerAnim::ROLLLEFT].size()){
			currentFrame =0;
			pState = playerState::MOVING;
			return;
		}
		timeSinceLastFrame = 0;
	} else if(timeSinceLastFrame > animInterval[playerAnim::ATTACKLEFT] && pState==playerState::ATTACKING){
		currentFrame = (currentFrame+1);
		if(currentFrame >=animMap[playerAnim::ATTACKLEFT].size()){
			currentFrame =0;
			pState = playerState::MOVING;
			return;
		}
		timeSinceLastFrame = 0;
		return;
	}

	//if no movement return resting frame at 0 and not rolling or attacking
	if(getVelocityX()==0 && getVelocityY()==0 && pState == playerState::STOPPED){
		timeSinceLastFrame =0;
		currentFrame = 0;
		return;
	}

	if (timeSinceLastFrame > animInterval.at(currentAnim) && pState == playerState::MOVING) {
    currentFrame = (currentFrame+1) % animMap[currentAnim].size();
		timeSinceLastFrame = 0;
	}
}

Player::Player( const std::string& name) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
	exp(nullptr),
	observers(),
	currentRoom(),
	animMap(),
  animInterval(),
	shovelMap(),
	shadow(RenderContext::getInstance()->getImage(name+"/shadow")),
  mX(moveX::RIGHT),
	mY(moveY::DOWN),
	pState(playerState::NONE),
	currentAnim(playerAnim::RIGHTDOWN),
  currentFrame(0),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
	initialVelocity(getVelocity()),
	previousVel(),
	godMode(false),
	lives(3)
{
	//animation read in
	animMap[playerAnim::LEFTUP] = RenderContext::getInstance()->getImages(name+"/leftUpAnim");
	animMap[playerAnim::RIGHTUP] = RenderContext::getInstance()->getImages(name+"/rightUpAnim");
	animMap[playerAnim::RIGHTDOWN] = RenderContext::getInstance()->getImages(name+"/rightDownAnim");
	animMap[playerAnim::LEFTDOWN] = RenderContext::getInstance()->getImages(name+"/leftDownAnim");
	animMap[playerAnim::ROLLLEFT] = RenderContext::getInstance()->getImages(name+"/rollLeftAnim");
	animMap[playerAnim::ROLLRIGHT] = RenderContext::getInstance()->getImages(name+"/rollRightAnim");
	animMap[playerAnim::ATTACKLEFT] = RenderContext::getInstance()->getImages(name+"/attackLeftAnim");
	animMap[playerAnim::ATTACKRIGHT] = RenderContext::getInstance()->getImages(name+"/attackRightAnim");

	//anim interval size read in
	animInterval[playerAnim::LEFTUP] = Gamedata::getInstance().getXmlInt(name+"/leftUpAnim/frameInterval");
	animInterval[playerAnim::RIGHTUP] = Gamedata::getInstance().getXmlInt(name+"/rightUpAnim/frameInterval");
	animInterval[playerAnim::RIGHTDOWN] = Gamedata::getInstance().getXmlInt(name+"/rightDownAnim/frameInterval");
	animInterval[playerAnim::LEFTDOWN] = Gamedata::getInstance().getXmlInt(name+"/leftDownAnim/frameInterval");
	animInterval[playerAnim::ROLLLEFT] = Gamedata::getInstance().getXmlInt(name+"/rollLeftAnim/frameInterval");
	animInterval[playerAnim::ROLLRIGHT] = Gamedata::getInstance().getXmlInt(name+"/rollRightAnim/frameInterval");
	animInterval[playerAnim::ATTACKLEFT] = Gamedata::getInstance().getXmlInt(name+"/attackLeftAnim/frameInterval");
	animInterval[playerAnim::ATTACKRIGHT] = Gamedata::getInstance().getXmlInt(name+"/attackRightAnim/frameInterval");

	shovelMap[playerAnim::ATTACKLEFT] = RenderContext::getInstance()->getImages(name+"/shovelLeftAnim");
	shovelMap[playerAnim::ATTACKRIGHT] = 	RenderContext::getInstance()->getImages(name+"/shovelRightAnim");
	setScale(Gamedata::getInstance().getXmlInt(name+"/scale"));
 }

Player::Player(const Player& s) :

  Drawable(s),
	exp(s.exp),
	observers(s.observers),
	currentRoom(s.currentRoom),
	animMap(s.animMap),
	animInterval(s.animInterval),
	shovelMap(s.shovelMap),
	shadow(s.shadow),
	mX(s.mX),
	mY(s.mY),
	pState(s.pState),
	currentAnim(s.currentAnim),
  currentFrame(s.currentFrame),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  initialVelocity(s.getVelocity()),
	previousVel(s.previousVel),
	godMode(s.godMode),
	lives(s.lives)
  { }

Player& Player::operator=(const Player& s) {
  Drawable::operator=(s);
	observers = s.observers;
	currentRoom = s.currentRoom;
	animMap = s.animMap;
	animInterval = s.animInterval;
	shovelMap = s.shovelMap;
	shadow = s.shadow;
	mX = s.mX;
	mY = s.mY;
	pState = s.pState;
  currentFrame = (s.currentFrame);
  frameInterval = ( s.frameInterval );
  timeSinceLastFrame = ( s.timeSinceLastFrame );
  worldWidth = ( s.worldWidth );
  worldHeight = ( s.worldHeight );
	observers = s.observers;
  initialVelocity = s.initialVelocity;
	previousVel = s.previousVel;
	lives = s.lives;
  return *this;
}

void Player::draw() const {
	if(exp){
		exp->draw();
		return;
	}

	shadow->draw(getX(),getY(),getScale());
	if(currentAnim ==playerAnim::ATTACKLEFT || currentAnim == playerAnim::ATTACKRIGHT){
		getCurrentAnim()[currentFrame]->draw(getX(),getY(),getScale());
		getWeaponAnim()[currentFrame]->draw(getX(),getY(),getScale());
		return;
	}
	getCurrentAnim()[currentFrame]->draw(getX(),getY(),getScale());
}

void Player::explode() {
  if ( !exp && !godMode ){
		 exp = new ExplodingSprite(Sprite(getName(),getPosition(),getVelocity(),getImage()));
		 if(lives>0){
			lives--;
			reset();
			// 	reset();
		} else {
			lives =3;
			spiderEnemy::spiderCount =10;
			reset();

		}
	 }
}

void Player::reset(){
	setPosition(Vector2f(Gamedata::getInstance().getXmlInt(getName()+"/startLoc/x"),
					 Gamedata::getInstance().getXmlInt(getName()+"/startLoc/y")));

	for(auto ob :observers){
		ob->reset();
	}
}

void Player::stop() {
  setVelocity(Vector2f(0,0));
  pState = playerState::STOPPED;
}

void Player::right() {
  if ( getX() < worldWidth-getScaledWidth() && pState != playerState::ROLLING) {
    setVelocityX(initialVelocity[0]);
    mX = moveX::RIGHT;
		if(pState == playerState::STOPPED){
			pState = playerState::MOVING;
		}
  }
}
void Player::left()  {
  if ( getX() > 0 && pState != playerState::ROLLING ) {
    setVelocityX(-initialVelocity[0]);
    mX = moveX::LEFT;
		if(pState == playerState::STOPPED){
			pState = playerState::MOVING;
		}
  }
}
void Player::up()    {
  if ( getY() > 0 && pState != playerState::ROLLING) {
    setVelocityY( -initialVelocity[1] );
    mY =moveY::UP;
		if(pState == playerState::STOPPED){
			pState = playerState::MOVING;
		}
  }
}
void Player::down() {
  if ( getY() < worldHeight-getScaledHeight() && pState != playerState::ROLLING) {
    setVelocityY( initialVelocity[1] );
    mY = moveY::DOWN;
		if(pState == playerState::STOPPED){
				pState = playerState::MOVING;
		}
  }
}
void Player::attack() {
	setVelocity(Vector2f(0,0));
	if(pState != playerState::ATTACKING){
		pState = playerState::ATTACKING;
		currentFrame = 0;
	}
}
void Player::roll() {
	if(pState!=playerState::ROLLING){
		Vector2f rollVec = previousVel;
		if(rollVec[0]==0 && rollVec[1]==0){
			if(mX == moveX::LEFT){
				setVelocity(Vector2f(-1*initialVelocity.magnitude(),0)*1.5);
			} else {
				setVelocity(Vector2f(initialVelocity.magnitude(),0)*1.5);
			}
		}else{
			rollVec = rollVec.normalize();
			setVelocity(Vector2f(rollVec[0],rollVec[1])*1.5*initialVelocity.magnitude());
		}
	  pState = playerState::ROLLING;
		currentFrame = 0;
	}
}


void Player::update(Uint32 ticks) {
	if ( exp ) {
		exp->update(ticks);
		if ( exp->chunkCount() == 0 ) {
			delete exp;
			exp = NULL;
		}
		return;
	}


	//save velocity for calculations
	previousVel = getVelocity();
	std::list<spiderEnemy*>::iterator ptr = observers.begin();
	while ( ptr != observers.end() ) {
		(*ptr)->setPlayerPos( getPosition() );
		++ptr;
	}

  advanceFrame(ticks);
	if(!getShadowCollided()){
		Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  	setPosition(getPosition() + incr);
	}
	else{
		//move back to center away from border
		float x= getX()+getScaledWidth()/2;
		float y= getY()+getScaledHeight()/2;
		float ex= currentRoom->getX()+currentRoom->getScaledWidth()/2;
		float ey= currentRoom->getY()+currentRoom->getScaledHeight()/2;
		Vector2f vec(ex-x,ey-y);
		vec[0]=vec[0]/2;
		vec =  vec.normalize()* 0.001* initialVelocity.magnitude()*static_cast<float>(ticks);
		setPosition(getPosition()+vec);
		updateCurrentAnim();


	}


	if (getVelocityX()>0) mX = moveX::RIGHT;
	else if(getVelocityX()<0) mX = moveX::LEFT;

	if (getVelocityY()>=0.01) mY = moveY::DOWN;
	else if (getVelocityY()<0) mY = moveY::UP;

	if(pState != playerState::ROLLING &&pState != playerState::ATTACKING)stop();

}
void Player::detach( spiderEnemy* o ) {
  std::list<spiderEnemy*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    if ( *ptr == o ) {
      ptr = observers.erase(ptr);
      return;
    }
    ++ptr;
  }
}

void Player::setCurrentRoom(std::vector<Room *> r, doorPlace p, int &index){
	Vector2f offset;
	doorPlace op;

	//set player position to opposite door in room to be transferred
	if(p ==doorPlace::N){
		op =doorPlace::S;
		offset = Vector2f(0,-60);
	} else if(p==doorPlace::S){
		op=doorPlace::N;
		offset = Vector2f(0,60);
	} else if(p==doorPlace::W){
		op=doorPlace::E;
		offset= Vector2f(-60,0);
		index--;
	} else if(p==doorPlace::E){
		op=doorPlace::W;
		offset = Vector2f(60,0);
		index++;
	}
	currentRoom = r[index];
	//set position somewhat offset from door
	Vector2f newPos = r[index]->getDoorloc(op)+offset;
	setPosition(newPos);

	for(auto enemy:r[index]->getEnemies()){
		attach(enemy);
	}


}

void checkDoors();
