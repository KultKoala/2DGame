//TODO add refactory period for roll
//TODO cut last couple of frames off of both rolls. Remember to fix xml



#include "gamedata.h"
#include "renderContext.h"
#include "helperFunctions.h"
#include "spiderEnemy.h"


void spiderEnemy::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > animInterval.at(currentAnim)) {
    currentFrame = (currentFrame+1) % animMap[currentAnim].size();
		timeSinceLastFrame = 0;
	}
}

// spiderEnemy::spiderEnemy( const std::string& name) :
//   Drawable(name,
//            Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
//                     Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
//            Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
//                     Gamedata::getInstance().getXmlInt(name+"/speedY"))
//            ),
// 	animMap(),
//   animInterval(),
// 	shadow(RenderContext::getInstance()->getImage(name+"/shadow")),
//   mX(moveX::RIGHT),
// 	mY(moveY::DOWN),
// 	pState(playerState::NONE),
// 	currentAnim(playerAnim::RIGHTDOWN),
//   currentFrame(0),
//   numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
//   frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
//   timeSinceLastFrame( 0 ),
//   worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
//   worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
// 	initialVelocity(getVelocity()),
// 	previousVel(),
//   safeDistance(Gamedata::getInstance().getXmlFloat(name+"/safeDistance"))
// {
// 	//animation read in
// 	animMap[playerAnim::LEFTUP] = RenderContext::getInstance()->getImages(name+"/idleAnim");
// 	animMap[playerAnim::RIGHTUP] = RenderContext::getInstance()->getImages(name+"/idleAnim");
// 	animMap[playerAnim::RIGHTDOWN] = RenderContext::getInstance()->getImages(name+"/idleAnim");
// 	animMap[playerAnim::LEFTDOWN] = RenderContext::getInstance()->getImages(name+"/idleAnim");
// 	animMap[playerAnim::ATTACKLEFT] = RenderContext::getInstance()->getImages(name+"/attackAnim");
// 	animMap[playerAnim::ATTACKRIGHT] = RenderContext::getInstance()->getImages(name+"/attackAnim");
//
// 	//anim interval size read in
// 	animInterval[playerAnim::LEFTUP] = Gamedata::getInstance().getXmlInt(name+"/idleAnim/frameInterval");
// 	animInterval[playerAnim::RIGHTUP] = Gamedata::getInstance().getXmlInt(name+"/idleAnim/frameInterval");
// 	animInterval[playerAnim::RIGHTDOWN] = Gamedata::getInstance().getXmlInt(name+"/idleAnim/frameInterval");
// 	animInterval[playerAnim::LEFTDOWN] = Gamedata::getInstance().getXmlInt(name+"/idleAnim/frameInterval");
// 	animInterval[playerAnim::ATTACKLEFT] = Gamedata::getInstance().getXmlInt(name+"/attackAnim/frameInterval");
// 	animInterval[playerAnim::ATTACKRIGHT] = Gamedata::getInstance().getXmlInt(name+"/attackAnim/frameInterval");
//
// 	setScale(Gamedata::getInstance().getXmlInt(name+"/scale"));
//  }

 spiderEnemy::spiderEnemy( const std::string& name, const Vector2f& pos, int w, int h) :
   Drawable(name,
            Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
                     Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
            Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                     Gamedata::getInstance().getXmlInt(name+"/speedY"))
            ),
  playerPos(pos),
  playerWidth(w),
  playerHeight(h),
 	animMap(),
  animInterval(),
 	shadow(RenderContext::getInstance()->getImage(name+"/shadow")),
  mX(moveX::RIGHT),
 	mY(moveY::DOWN),
 	pState(playerState::MOVING),
 	currentAnim(playerAnim::RIGHTDOWN),
   currentFrame(0),
   numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
   frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
   timeSinceLastFrame( 0 ),
   worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
   worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
 	initialVelocity(getVelocity()),
 	previousVel(),
   safeDistance(Gamedata::getInstance().getXmlFloat(name+"/safeDistance"))
 {
 	//animation read in
 	animMap[playerAnim::LEFTUP] = RenderContext::getInstance()->getImages(name+"/idleAnim");
 	animMap[playerAnim::RIGHTUP] = RenderContext::getInstance()->getImages(name+"/idleAnim");
 	animMap[playerAnim::RIGHTDOWN] = RenderContext::getInstance()->getImages(name+"/idleAnim");
 	animMap[playerAnim::LEFTDOWN] = RenderContext::getInstance()->getImages(name+"/idleAnim");
 	animMap[playerAnim::ATTACKLEFT] = RenderContext::getInstance()->getImages(name+"/attackAnim");
 	animMap[playerAnim::ATTACKRIGHT] = RenderContext::getInstance()->getImages(name+"/attackAnim");

 	//anim interval size read in
 	animInterval[playerAnim::LEFTUP] = Gamedata::getInstance().getXmlInt(name+"/idleAnim/frameInterval");
 	animInterval[playerAnim::RIGHTUP] = Gamedata::getInstance().getXmlInt(name+"/idleAnim/frameInterval");
 	animInterval[playerAnim::RIGHTDOWN] = Gamedata::getInstance().getXmlInt(name+"/idleAnim/frameInterval");
 	animInterval[playerAnim::LEFTDOWN] = Gamedata::getInstance().getXmlInt(name+"/idleAnim/frameInterval");
 	animInterval[playerAnim::ATTACKLEFT] = Gamedata::getInstance().getXmlInt(name+"/attackAnim/frameInterval");
 	animInterval[playerAnim::ATTACKRIGHT] = Gamedata::getInstance().getXmlInt(name+"/attackAnim/frameInterval");

 	setScale(Gamedata::getInstance().getXmlInt(name+"/scale"));
  }

spiderEnemy::spiderEnemy(const spiderEnemy& s) :

  Drawable(s),
	playerPos(s.playerPos),
	playerWidth(s.playerWidth),
	playerHeight(s.playerHeight),
	currentRoom(s.currentRoom),
	animMap(s.animMap),
	animInterval(s.animInterval),
	shadow(s.shadow),
	mX(s.mX),
	mY(s.mY),
	pState(s.pState),
	currentAnim(s.currentAnim),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  initialVelocity(s.getVelocity()),
	previousVel(s.previousVel),
  safeDistance(s.safeDistance)
  { }

spiderEnemy& spiderEnemy::operator=(const spiderEnemy& s) {
  Drawable::operator=(s);
	currentRoom = s.currentRoom;
	shadow = s.shadow;
	mX = s.mX;
	mY = s.mY;
	pState = s.pState;
  currentFrame = (s.currentFrame);
  numberOfFrames = ( s.numberOfFrames );
  frameInterval = ( s.frameInterval );
  timeSinceLastFrame = ( s.timeSinceLastFrame );
  worldWidth = ( s.worldWidth );
  worldHeight = ( s.worldHeight );
  initialVelocity = s.initialVelocity;
	previousVel = s.previousVel;
  safeDistance=s.safeDistance;
  return *this;
}

void spiderEnemy::draw() const {
	shadow->draw(getX(),getY(),getScale());
	if(currentAnim ==playerAnim::ATTACKLEFT || currentAnim == playerAnim::ATTACKRIGHT){
		getCurrentAnim()[currentFrame]->draw(getX(),getY(),getScale());
		return;
	}
	getCurrentAnim()[currentFrame]->draw(getX(),getY(),getScale());
}

void spiderEnemy::stop() {
  setVelocity(Vector2f(0,0));
}

void spiderEnemy::right() {
  if ( getX() < worldWidth-getScaledWidth() && pState != playerState::ROLLING) {
    setVelocityX(initialVelocity[0]);
    mX = moveX::RIGHT;
		if(pState == playerState::STOPPED){
			pState = playerState::MOVING;
		}
  }
}
void spiderEnemy::left()  {
  if ( getX() > 0 ) {
    setVelocityX(-initialVelocity[0]);
    mX = moveX::LEFT;
  }
}
void spiderEnemy::up()    {
  if ( getY() > 0) {
    setVelocityY( -initialVelocity[1] );
    mY =moveY::UP;
  }
}
void spiderEnemy::down() {
  if ( getY() < worldHeight-getScaledHeight()) {
    setVelocityY( initialVelocity[1] );
    mY = moveY::DOWN;
  }
}
void spiderEnemy::attack() {
}

void spiderEnemy::update(Uint32 ticks) {
	//save velocity for calculations
	previousVel = getVelocity();

  if ( !getCollided() ) advanceFrame(ticks);
	if(!getShadowCollided()){
		Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  	setPosition(getPosition() + incr);
    float x= getX()+getImage()->getWidth()/2;
    float y= getY()+getImage()->getHeight()/2;
    float ex= playerPos[0]+playerWidth/2;
    float ey= playerPos[1]+playerHeight/2;
    float distanceToEnemy = helperFunctions::distance( x, y, ex, ey );

    if  ( pState == playerState::MOVING ) {
      if(distanceToEnemy < safeDistance) pState = playerState::ATTACKING;
    }
		//attack player
    else if  ( pState == playerState::ATTACKING ) {
      if(distanceToEnemy > safeDistance) {
				pState = playerState::MOVING;
				currentFrame=0;
			}
      else {
        Vector2f vec(ex-x,ey-y);
        vec[0]=vec[0]/2;
        vec =  vec.normalize()* 0.001* initialVelocity.magnitude()*static_cast<float>(ticks);
        setPosition(getPosition()+vec);
      }
    }
	}
	else{
		//move back to center away from border
		float x2= getX()+getScaledWidth()/2;
		float y2= getY()+getScaledHeight()/2;
		float ex2= currentRoom->getX()+currentRoom->getScaledWidth()/2;
		float ey2= currentRoom->getY()+currentRoom->getScaledHeight()/2;
		Vector2f vec2(ex2-x2,ey2-y2);
		vec2[0]=vec2[0]/2;
		vec2 =  vec2.normalize()* 0.001* initialVelocity.magnitude()*static_cast<float>(ticks);
		setPosition(getPosition()+vec2);

	}


	if (getVelocityX()>0) mX = moveX::RIGHT;
	else if(getVelocityX()<0) mX = moveX::LEFT;

	if (getVelocityY()>=0.01) mY = moveY::DOWN;
	else if (getVelocityY()<0) mY = moveY::UP;
	if(pState != playerState::ATTACKING)stop();
	updateCurrentAnim();

}
