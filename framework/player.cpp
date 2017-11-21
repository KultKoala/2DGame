//TODO add refactory period for roll
//TODO cut last couple of frames off of both rolls. Remember to fix xml


#include "player.h"
#include "gamedata.h"
#include "renderContext.h"
#include "helperFunctions.h"


void Player::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;

	if (timeSinceLastFrame > frameInterval && rolling) {
		currentFrame = (currentFrame+1);
		if(currentFrame >= rollLeftAnim.size()){
			currentFrame =0;
			rolling = false;
		}
		timeSinceLastFrame = 0;
	}

	//if no movement return resting frame at 0 and not rolling or attacking
	if(getVelocityX()==0 && getVelocityY()==0 && !rolling){
		timeSinceLastFrame =0;
		currentFrame = 0;
		return;
	}

	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
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

	observers(),
	currentRoom(),
	leftUpAnim( RenderContext::getInstance()->getImages(name+"/leftUpAnim") ),
	rightUpAnim( RenderContext::getInstance()->getImages(name+"/rightUpAnim") ),
 	rightDownAnim( RenderContext::getInstance()->getImages(name+"/rightDownAnim") ),
	leftDownAnim( RenderContext::getInstance()->getImages(name+"/leftDownAnim") ),
	rollLeftAnim(RenderContext::getInstance()->getImages(name+"/rollLeftAnim") ),
	rollRightAnim(RenderContext::getInstance()->getImages(name+"/rollRightAnim") ),
	shadow(RenderContext::getInstance()->getImage(name+"/shadow")),
  movingLeft(false),
  movingDown(true),
	stopped(true),
	rolling(false),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  collision(false),
	shadowCollision(false),
	initialVelocity(getVelocity())
{ setScale(Gamedata::getInstance().getXmlInt(name+"/scale")); }

Player::Player(const Player& s) :

  Drawable(s),
	observers(s.observers),
	currentRoom(s.currentRoom),
	leftUpAnim(s.leftUpAnim),
	rightUpAnim(s.rightUpAnim),
	rightDownAnim(s.rightDownAnim),
	leftDownAnim(s.leftDownAnim),
	rollLeftAnim(s.rollLeftAnim),
	rollRightAnim(s.rollRightAnim),
	shadow(s.shadow),
	movingLeft(s.movingLeft),
	movingDown(s.movingDown),
	stopped(s.stopped),
	rolling(s.rolling),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  collision(s.collision),
	shadowCollision(s.shadowCollision),
  initialVelocity(s.getVelocity())
  { }

Player& Player::operator=(const Player& s) {
  Drawable::operator=(s);
	observers = s.observers;
	shadow = (s.shadow);
	shadowCollision = (s.shadowCollision);
  leftUpAnim = (s.leftUpAnim);
  leftDownAnim = (s.leftDownAnim);
  rightUpAnim = (s.rightUpAnim);
  rightDownAnim = (s.rightDownAnim);

	rollLeftAnim = (s.rollLeftAnim);
	rollRightAnim =(s.rollRightAnim);

  currentFrame = (s.currentFrame);
  numberOfFrames = ( s.numberOfFrames );
  frameInterval = ( s.frameInterval );
  timeSinceLastFrame = ( s.timeSinceLastFrame );
  worldWidth = ( s.worldWidth );
  worldHeight = ( s.worldHeight );
  collision = s.collision;
	observers = s.observers;
  initialVelocity = s.initialVelocity;

	rolling = s.rolling;
	movingLeft = s.movingLeft;
	movingDown = s.movingDown;
  return *this;
}

void Player::draw() const {
	shadow->draw(getX(),getY());

  if(rolling){
		if(movingLeft)rollLeftAnim[currentFrame]->draw(getX(),getY());
		else rollRightAnim[currentFrame]->draw(getX(),getY());
    return;
  }

	if(movingLeft && movingDown) leftDownAnim[currentFrame]->draw(getX(), getY());
	else if(!movingLeft && movingDown) rightDownAnim[currentFrame]->draw(getX(), getY());
	else if(!movingLeft && !movingDown) rightUpAnim[currentFrame]->draw(getX(), getY());
	else leftUpAnim[currentFrame]->draw(getX(), getY());

}

void Player::stop() {
  setVelocity(Vector2f(0,0));
  stopped = true;
}

void Player::right() {
  if ( getX() < worldWidth-getScaledWidth()) {
    setVelocityX(initialVelocity[0]);
    movingLeft = false;
  }
}
void Player::left()  {
  if ( getX() > 0) {
    setVelocityX(-initialVelocity[0]);
    movingLeft = true;
  }
}
void Player::up()    {
  if ( getY() > 0) {
    setVelocityY( -initialVelocity[1] );
    movingDown= false;
  }
}
void Player::down() {
  if ( getY() < worldHeight-getScaledHeight()) {
    setVelocityY( initialVelocity[1] );
    movingDown = true;
  }
}
void Player::attack() {}
void Player::roll() {

	Vector2f rollVec = getVelocity();
	if(rollVec[0]==0 && rollVec[1]==0){
		if(movingLeft){
			setVelocity(Vector2f(-1*initialVelocity[0],0)*3);
		} else {
			setVelocity(Vector2f(initialVelocity[0],0)*3);
		}
	}
	else{
		rollVec = rollVec.normalize();
		setVelocity(rollVec *initialVelocity.magnitude()*3);
	}
  rolling = true;
}

void Player::update(Uint32 ticks) {

	std::list<SmartSprite*>::iterator ptr = observers.begin();
	while ( ptr != observers.end() ) {
		(*ptr)->setPlayerPos( getPosition() );
		++ptr;
	}

  if ( !collision ) advanceFrame(ticks);
	if(!shadowCollision){
		if(rolling){
			Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.002;
			setPosition(getPosition() + incr);
		}
	  else{
			Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
	  	setPosition(getPosition() + incr);
		}
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


	}


	if (getVelocityX()>0) movingLeft = false;
	else if(getVelocityX()<0) movingLeft = true;

	if (getVelocityY()>=0.01) movingDown = true;
	else if (getVelocityY()<0) movingDown = false;

  stop();

}
void Player::detach( SmartSprite* o ) {
  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    if ( *ptr == o ) {
      ptr = observers.erase(ptr);
      return;
    }
    ++ptr;
  }
}

void Player::setCurrentRoom(Room *r){currentRoom = r;}
