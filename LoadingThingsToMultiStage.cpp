#include "LoadingThingsToMultiStage.h"
#include "ResourceManager.h"


LoadingThingsToMultiStage::LoadingThingsToMultiStage(Phisics_2D &ph): phisics(ph)
{
}
void LoadingThingsToMultiStage::update(entityx::EntityManager & en, entityx::EventManager & ev, double dt)
{
	ConfigParser parser, parser3;

	auto levelInfo = ResourcesManager::getInstanceRef().levelInfo;
	auto shipInfo = ResourcesManager::getInstanceRef().shipInfo;

	if (!parser.load(levelInfo))
	{
		assert(false);
	}
	std::string typeOfElement, pathToShip = "resources/parts/";
	float xPos, yPos, xVel, yVel, rot, mass;

	//LOADING WORLD ELEMENTS

	parser.setSection("world");

	while (!parser.EndOfSection())
	{
		typeOfElement = parser.getString();
		xPos = parser.getFloat();
		yPos = parser.getFloat();
		xVel = parser.getFloat();
		yVel = parser.getFloat();
		rot = parser.getFloat();
		mass = parser.getFloat();
		auto poly = en.create();

		poly.assign<DontCollideWith>(2);//lests set "2" as a deafult number to not collide witch enviroment
		//assigning component type
		if (typeOfElement == "platform")
		{
			poly.assign<isPlatform>(parser.getFloat());
			std::cout << "PLATFORM LOADED" << std::endl;
		}
		else if (typeOfElement == "wall")
		{
			std::cout << " WALL LOADED" << std::endl;
		}
		else if (typeOfElement == "bomb")
		{
			poly.assign< Hookable >();
			poly.assign< Cargo >(1);
		}
		else if (typeOfElement == "cargo_space")
		{
			poly.assign<Position>(sf::Vector2f(xPos, yPos));
			poly.assign<CargoSpace>(sf::Vector2f(xVel, yVel), 1);//nie wiem o co chodzi ale dzia�a tylko "1" parser.getFloat() to b��d :c
			continue;
		}
		else if (typeOfElement == "shooting_camera")
		{
			poly.assign<isEnemyCam>();

			phisics.createPolygon(poly, sf::Vector2f(xPos, yPos),
				sf::Vector2f(xVel, yVel), rot, mass, typeOfElement);

			VertexArray::Handle H = poly.component<VertexArray>();

			H->vert[0].color = sf::Color::Red;
			H->vert[1].color = sf::Color::Red;
			H->vert[2].color = sf::Color::Red;
			H->vert[3].color = sf::Color::Red;
			continue;
		}
		else if (typeOfElement == "sliding_doors")
		{
			poly.assign<isSlidingDoors>();
			isSlidingDoors::Handle door = poly.component<isSlidingDoors>();
			std::string s;
			float f;
			while (!parser.EndOfLine())
			{
				s = parser.getString();
				f = parser.getFloat();
			}
			door->add(s, f);
		}
		phisics.createPolygon(poly, sf::Vector2f(xPos, yPos),
			sf::Vector2f(xVel, yVel), rot, mass, typeOfElement);
	}
	parser.setSection("camera");
	ResourcesManager::getInstanceRef().isMovingCameraOn = parser.getFloat();
	//LOADING SHIP ELEMENTS
	if (!parser.load(shipInfo))
	{
		assert(false);
	}
	std::cout << shipInfo << std::endl;
	parser.setSection("body_info");

	std::string typeOfShip, shipColor;
	float actualEngineForce;

	while (!parser.EndOfLine())
	{
		typeOfShip = parser.getString();
	}

	if (typeOfShip.size() == 0)
	{
		auto &stage = ResourcesManager::getInstanceRef();
		std::cout << "WYBIERZ JAKIS STATEK" << std::endl;
		stage.creator_stage.set();
		return;
	}
	parser.setSection("color");

	while (!parser.EndOfSection())
	{
		shipColor = parser.getString();
	}

	auto playerEn = en.create();
	playerEn.assign<isPlayer>();
	playerEn.assign<DontCollideWith>(1);
	phisics.createPolygon(playerEn, sf::Vector2f(4, 8),
		sf::Vector2f(0, 0), 0, 1, typeOfShip);

	//PARTS
	parser.setSection("parts_info");
	unsigned int partID;
	std::string partName, partKey, pathToPart, partColor, partVert;
	Conversion conversion;
	float partPosX, partPosY, partDegree;

	while (!parser.EndOfSection())
	{
		typeOfShip = parser.getString();//type of part
		partPosX = parser.getFloat();
		partPosY = parser.getFloat();
		partDegree = parser.getFloat();
		partKey = parser.getString();
		partID = parser.getFloat();
		ConfigParser parser2;

		//PART INFO LOAD engine maybe legs or anything else
		pathToPart.clear();
		pathToPart.insert(pathToPart.size(), "resources/parts/");
		pathToPart.insert(pathToPart.size(), typeOfShip);
		pathToPart.insert(pathToPart.size(), ".cfg");

		if (!parser2.load(pathToPart))
		{
			assert(false);
		}
		while (!parser2.EndOfSection())
		{
			parser2.setSection("name");
			partVert = parser2.getString();
		}

		auto &container = ResourcesManager::getInstanceRef().vertCont;

		auto partEn = en.create();

		parser2.setSection("engineForce");
		actualEngineForce = parser2.getFloat();
		std::cout << " ENGINE FORCE :" << actualEngineForce << std::endl;

		sf::Vector2f engineForce = sf::Vector2f(0, -actualEngineForce);

		sf::Transform transForce;
		transForce.rotate(partDegree);
		engineForce = transForce * engineForce;

		partEn.assign<AttachToPlayerPoint>(sf::Vector2f(partPosX, partPosY));
		partEn.assign<PolyName>(partVert);
		partEn.assign<partId>(partID);
		partEn.assign<Rotation>(partDegree);
		partEn.assign<VertexArray>(container.getPoly(partVert), container.getNormals(partVert));
		partEn.assign<Transform>(sf::Vector2f(0, 0), 0);
		partEn.assign<LinearVelocity>(sf::Vector2f(0, 0));

		std::cout << std::endl;
		if (partVert == "crane")
		{
			partEn.assign<isCrane>();

			partEn.assign<KeyAssigned>(conversion.string_to_sf_key[partKey]);
		}
		else if (partVert == "small_engine" || partVert == "large_engine")
		{
			std::cout << "engine loaded" << std::endl;
			partEn.assign<isEngine>();

			partEn.assign<ForcePoint>(engineForce);

			partEn.assign<KeyAssigned>(conversion.string_to_sf_key[partKey]);
		}
		else if (partVert == "gun")
		{
			std::cout << "gun loaded" << std::endl;

			partEn.assign<isGun>();

			partEn.assign<KeyAssigned>(conversion.string_to_sf_key[partKey]);
		}
		else {
			std::cout << "legs loaded" << std::endl;

			partEn.assign<isSlave>(playerEn);
			phisics.createPolygon(partEn, sf::Vector2f(0, 0), sf::Vector2f(0, 0), partDegree, 1, partVert);
			continue;
		}
	}
}


LoadingThingsToMultiStage::~LoadingThingsToMultiStage()
{
}
