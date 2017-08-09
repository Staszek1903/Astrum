#pragma once

#include <entityx/entityx.h>
#include <SFML/Graphics.hpp>



struct Position : public entityx::Component<Position>
{
    Position(const sf::Vector2f & pos)
		:pos(pos), prevPos(pos) {}
	sf::Vector2f pos;
	sf::Vector2f prevPos;
};

struct Rotation : public entityx::Component<Rotation>
{
	Rotation(float degree)
		:degree(degree) {}		//in degrees!!!
	float degree;
};

struct Mass : public entityx::Component<Mass>
{
	Mass(float mass)
		:mass(mass) 
		{
		if (mass != 0)
			invMass = 1.0f / mass;
		else
			invMass = 0;
		}
	float mass,
		  invMass;
};

struct LinearVelocity : public entityx::Component<LinearVelocity>
{
    LinearVelocity(const sf::Vector2f & vel)
		:vel(vel) {}
	sf::Vector2f vel;
};

struct AngularVelocity : public entityx::Component<AngularVelocity>
{
	AngularVelocity(float degree)
		:degree(degree) {}  // in degrees!!!
	float degree;
	float radians() { return (degree / 57.29577f); }
};

struct Circle : public entityx::Component<Circle>
{
	Circle(float r, sf::Color color)
		:r(r), color(color) {}
	sf::Color color;
	float r;
};

struct Line : public entityx::Component<Line>
{
	Line(sf::Vector2f begin, sf::Vector2f end, sf::Color col)
		:line(sf::Lines, 2)
	{
		line[0].position = begin;
		line[1].position = end;
		line[0].color = col;
		line[1].color = col;
	}

	sf::VertexArray line;
};

struct Transform : entityx::Component<Transform>
{
	Transform(sf::Vector2f pos, float rot) {
		trans.rotate(rot);
		trans.translate(pos);
		defaultTrans.rotate(rot);
		defaultTrans.translate(pos);
	}
	sf::Transform trans, defaultTrans;
};

struct LinearForce : entityx::Component<LinearForce> 
{
	LinearForce() : force(sf::Vector2f(0.0, 0.0)) 
	{}
	sf::Vector2f force;
};

struct AngularForce : entityx::Component<AngularForce>
{
	AngularForce() : force(0.0f)
	{}
	float force;					// force in radians!!!
};

struct MOfInertia : entityx::Component<MOfInertia>
{
	MOfInertia(float i)
		:I(i)
	{
		if (I != 0)
			invI = 1.0f / I;
		else
			invI = 0;
	}

	float I;
	float invI;
};

struct Friction : entityx::Component<Friction>
{
	Friction(float fr1): fr(fr1) {}
	float fr;
};

struct VertexArray : entityx::Component<VertexArray>
{
	VertexArray(sf::VertexArray & v, std::vector <sf::Vector2f> &n)
		:vert(v), normals(n) {
		std::cout << "component check: " << normals.size() << std::endl;
	}
	sf::VertexArray & vert;
	std::vector <sf::Vector2f> & normals;
};

struct Type : entityx::Component<Type>
{
	Type(int type) : type(type){}
	int type;
	enum typ1{
		CIRCLE, 
		POLYGON
	};
};

struct IsResting : entityx::Component<IsResting>
{
	IsResting() : isIt(false)
	{}
		bool isIt;
};

struct Joint :entityx::Component<Joint>
{
    Joint(entityx::Entity en, entityx::Entity en1, sf::Vector2f pos, sf::Vector2f pos1, float lenght)
        :pos(pos), pos1(pos1), en(en), en1(en1), lenght(lenght){}

    sf::Vector2f pos;
    sf::Vector2f pos1;
    entityx::Entity en;
    entityx::Entity en1;
    float lenght;
};

struct ForcePoint : entityx::Component<ForcePoint>
{
	ForcePoint( sf::Vector2f force) : force(force)
	{}
	sf::Vector2f force;
};

struct AttachToPlayerPoint : entityx::Component<AttachToPlayerPoint>
{
	AttachToPlayerPoint(sf::Vector2f point) : point(point)
	{}
	sf::Vector2f point;
};

struct KeyAssigned : entityx::Component<KeyAssigned>
{
	// = sf::Keyboard::Key::B;
	KeyAssigned(sf::Keyboard::Key k) : key (k)
	{}
	sf::Keyboard::Key key;
};
//
struct isPlayer : entityx::Component<isPlayer>
{
};
struct isLegs : entityx::Component<isLegs>
{
};
struct isEngine : entityx::Component<isLegs>
{
};
struct isPlatform : entityx::Component<isPlatform>
{
};
//
struct isSlave : entityx::Component<isSlave>
{
    isSlave(entityx::Entity master)
        :master(master){}
    entityx::Entity master;
};

struct AdditionalAnim : entityx::Component<AdditionalAnim>
{
     AdditionalAnim(std::string name, sf::Texture * tex, sf::VertexArray & v_array, float fps)
		 :texture(tex), v_array(v_array), frame_x(0), frame_y(0), fps(fps), animate(false), time(sf::Time::Zero), nameOfAnim(name)
	 {	 
		 clock.restart();
	 }
	 AdditionalAnim(std::string name, sf::Texture * tex, sf::VertexArray & v_array, float fps, sf::Transform trans)
		 :texture(tex), v_array(v_array), frame_x(0), frame_y(0), fps(fps), animate(false), time(sf::Time::Zero), nameOfAnim(name), transMatrix(trans)
	 {
		 clock.restart();
	 }
	sf::Time time;
	sf::Clock clock;
	sf::Texture * texture;
    sf::VertexArray & v_array;
	sf::Transform transMatrix;
	std::string nameOfAnim; //name in info file in "resources" folder

    int frame_x, frame_y;
    float fps;
    bool animate;
};

