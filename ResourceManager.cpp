#include "ResourceManager.h"

ResourcesManager* ResourcesManager::instance = nullptr;

ResourcesManager::ResourcesManager() : gameplay_stage(vertCont), levelInfo("resources/levelData/level_1.cfg"), shipInfo("resources/levelData/ship_1.cfg")
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	
    window.create(sf::VideoMode(1366, 768, 32), "Astrum", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize, settings);

	window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);

    font.loadFromFile("resources/sansation.ttf");

    loadContainer();
}

ResourcesManager::~ResourcesManager()
{
}

void ResourcesManager::loadContainer()
{
    std::map <std::string, sf::Color> sf_colors;
    sf_colors["BLACK"] = sf::Color::Black;
    sf_colors["RED"] = sf::Color::Red;
    sf_colors["MAGENTA"] = sf::Color::Magenta;
    sf_colors["CYAN"] = sf::Color::Cyan;
    sf_colors["BLUE"] = sf::Color::Blue;
    sf_colors["GREEN"] = sf::Color::Green;
    sf_colors["WHITE"] = sf::Color::White;
    sf_colors["YELLOW"] = sf::Color::Yellow;


    std::string dir("resources/parts/");
    std::vector < std::string > filenames = {
        "triangle_body.cfg",
        "long_body.cfg",
        "cobra_body.cfg",
        "large_engine.cfg",
        "small_engine.cfg",
        "landing_legs.cfg",
        "crane.cfg",
        "wall.cfg"
    };

    ConfigParser parser;

    for(auto & filename: filenames)
    {
        std::vector <sf::Vector2f> vert_vec;
        std::string name;
        parser.load(dir+filename);
        parser.setSection("name");
        name = parser.getString();
        std::cout<<"NAME: "<<name<<std::endl;
        parser.setSection("vertex");
        while(!parser.EndOfSection())
        {
            sf::Vector2f v;
            v.x = parser.getFloat();
            v.y = parser.getFloat();
            std::cout<<v.x<<" "<<v.y<<std::endl;
            vert_vec.push_back(v);
        }

        sf::Color color;
        if(parser.setSection("color"))
        {
            std::string c = parser.getString();
            if(sf_colors.find(c) == sf_colors.end())
            {
                std::cout<<"zły kolor w pliku"<<std::endl;
                assert(false);
            }
            color = sf_colors[c];
        }

        unsigned int vec_size = vert_vec.size();
        sf::VertexArray v_array(sf::TriangleFan, vec_size);
        for(int i=0; i<vec_size; ++i)
        {
            v_array[i].position = vert_vec[i];
            v_array[i].color = color;
        }

        vertCont.addPoly(v_array, vec_size, name);
    }
}

ResourcesManager* ResourcesManager::getInstance()
{
	if (!instance) instance = new ResourcesManager();

	return instance;
}

ResourcesManager& ResourcesManager::getInstanceRef()
{
	return *getInstance();
}

void ResourcesManager::deleteInstance()
{
	delete instance;
}
