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
	} else if(timeSinceLastFrame > frameInterval && attacking){
		currentFrame = (currentFrame+1);
		if(currentFrame == attackLeftAnim.size()){
			currentFrame =0;
			attacking = false;
		}
		timeSinceLastFrame = 0;
	}

	//if no movement return resting frame at 0 and not rolling or attacking
	if(getVelocityX()==0 && getVelocityY()==0 && !rolling && !attacking){
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
	attackLeftAnim(RenderContext::getInstance()->getImages(name+"/attackLeftAnim") ),
	attackRightAnim(RenderContext::getInstance()->getImages(name+"/attackRightAnim") ),
	shovelLeftAnim(RenderContext::getInstance()->getImages(name+"/shovelLeftAnim") ),
	shovelRightAnim(RenderContext::getInstance()->getImages(name+"/shovelRightAnim") ),

	shadow(RenderContext::getInstance()->getImage(name+"/shadow")),
  movingLeft(false),
  movingDown(true),
	stopped(true),
	rolling(false),
	attacking(false),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
	initialVelocity(getVelocity()),
	previousVel()
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
	attackLeftAnim(s.attackLeftAnim),
	attackRightAnim(s.attackRightAnim),
	shovelLeftAnim(s.shovelLeftAnim),
	shovelRightAnim(s.shovelRightAnim),
	shadow(s.shadow),
	movingLeft(s.movingLeft),
	movingDown(s.movingDown),
	stopped(s.stopped),
	rolling(s.rolling),
	attacking(s.attacking),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  initialVelocity(s.getVelocity()),
	previousVel(s.previousVel)
  { }

Player& Player::operator=(const Player& s) {
  Drawable::operator=(s);
	observers = s.observers;
	shadow = (s.shadow);
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
	observers = s.observers;
  initialVelocity = s.initialVelocity;
	rolling = s.rolling;
	attacking = s.attacking;
	movingLeft = s.movingLeft;
	movingDown = s.movingDown;
	previousVel = s.previousVel;
  return *this;
}

void Player::draw() const {
	shadow->draw(getX(),getY(),getScale());

  if(rolling){
		if(movingLeft)rollLeftAnim[currentFrame]->draw(getX(),getY(),getScale());
		else rollRightAnim[currentFrame]->draw(getX(),getY(),getScale());
    return;
  }
	//TODO: does this logic make sense
	if(attacking){
		if(movingLeft){
			attackLeftAnim[currentFrame]->draw(getX()-32,getY()-32,getScale());
			shovelLeftAnim[currentFrame]->draw(getX()-32,getY()-32,getScale());
		}
		else {
			attackRightAnim[currentFrame]->draw(getX()-32,getY()-32,getScale());
			shovelRightAnim[currentFrame]->draw(getX()-32,getY()-32,getScale());
		}
		return;
	}

	if(movingLeft && movingDown) leftDownAnim[currentFrame]->draw(getX(), getY(),getScale());
	else if(!movingLeft && movingDown) rightDownAnim[currentFrame]->draw(getX(), getY(),getScale());
	else if(!movingLeft && !movingDown) rightUpAnim[currentFrame]->draw(getX(), getY(),getScale());
	else leftUpAnim[currentFrame]->draw(getX(), getY(),getScale());

}

void Player::stop() {
  setVelocity(Vector2f(0,0));
  stopped = true;
}

void Player::right() {
  if ( getX() < worldWidth-getScaledWidth() && !rolling) {
    setVelocityX(initialVelocity[0]);
    movingLeft = false;
  }
}
void Player::left()  {
  if ( getX() > 0 && !rolling) {
    setVelocityX(-initialVelocity[0]);
    movingLeft = true;
  }
}
void Player::up()    {
  if ( getY() > 0 && !rolling) {
    setVelocityY( -initialVelocity[1] );
    movingDown= false;
  }
}
void Player::down() {
  if ( getY() < worldHeight-getScaledHeight() && !rolling) {
    setVelocityY( initialVelocity[1] );
    movingDown = true;
  }
}
void Player::attack() {
	setVelocity(Vector2f(0,0));
	if(attacking == false){
		attacking = true;
		rolling = false;
		currentFrame = 0;
	}
}
void Player::roll() {
	if(!rolling){
		Vector2f rollVec = previousVel;
		std::cout<<rollVec;
		if(rollVec[0]==0 && rollVec[1]==0){
			std::cout<<"Entering"<<std::endl;
			if(movingLeft){
				setVelocity(Vector2f(-1*initialVelocity.magnitude(),0)*1.5);
			} else {
				setVelocity(Vector2f(initialVelocity.magnitude(),0)*1.5);
			}
		}else{
			rollVec = rollVec.normalize();
			setVelocity(Vector2f(rollVec[0],rollVec[1])*1.5*initialVelocity.magnitude());
			std::cout<<getVelocity()<<std::endl;
			std::cout<<"Being called"<<std::endl;
		}
	  rolling = true;
		currentFrame = 0;
	}
}

void Player::update(Uint32 ticks) {
	//save velocity for calculations
	previousVel = getVelocity();
	std::list<SmartSprite*>::iterator ptr = observers.begin();
	while ( ptr != observers.end() ) {
		(*ptr)->setPlayerPos( getPosition() );
		++ptr;
	}

  if ( !getCollided() ) advanceFrame(ticks);
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


	}


	if (getVelocityX()>0) movingLeft = false;
	else if(getVelocityX()<0) movingLeft = true;

	if (getVelocityY()>=0.01) movingDown = true;
	else if (getVelocityY()<0) movingDown = false;

	if(!rolling)stop();

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
