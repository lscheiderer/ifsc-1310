

#include<iostream>
#include<vector>
#include<map>
#include<random>
#include<algorithm>


struct Room
{
	enum class Name { cell, gate, armory, jailers, exit };
	enum class Direction { N, S, E, W, none };
	std::string message;// the message displayed when in the room.
	std::map<Direction, Name> doorways;
	//NOTE:: these two vectors will eventually become a std::map after the STL level.
	bool hasKey{ false }; //whether or not the player has picked up the key.

	Room(const std::string& desc, const std::map<Direction, Name>& connections, bool visited)
		: message(desc), doorways(connections), hasKey(visited) {};
};
struct Player
{
	Room::Name currentRoom{ Room::Name::cell };
	int health{ 10 };
	bool hasKey{ false };
};
/* GUI Functions */

void clearConsole();
void pauseConsole();
void splashScreen();
void displayGameState(const Player& player, const std::map<Room::Name, Room>& map);
void displayGameDone(const Player& player, const std::map<Room::Name, Room>& map);
void displayIllegalMove(const Player& player, Room::Direction action);
Room::Direction getAction();

/* Engine Functions*/
std::map<Room::Name, Room> buildMap();
void randomizeKey(std::vector<Room>& map);
bool changeGameState(Room::Direction action, Player& player, std::map<Room::Name, Room>& map, bool haskey);
bool gameIsNotDone(const Player& player);

int main() {

	/*Splash Screen*/
	clearConsole();
	splashScreen();

	/*set up game*/
	std::map<Room::Name, Room> map{ buildMap() };
	Player player;

	/*Until Game Termination Condition*/
	while (gameIsNotDone(player))
	{

		/*Display Game State*/
		clearConsole();
		displayGameState(player, map);

		/*Collect Player Action*/
		Room::Direction action{ getAction() };

		/*Update Game State*/
		if (!changeGameState(action, player, map, {}))
		{
			displayIllegalMove(player, action);
		}
	}

	/*Display Termination Game State*/
	displayGameDone(player, map);

	return 0;

}


std::map<Room::Name, Room> buildMap()
{
	std::map<Room::Name, Room> dungeonMap;
	
	dungeonMap.emplace(Room::Name::cell, 
		Room("A small, dark prison cell with doors South and East.",
			{
				{Room::Direction::S, Room::Name::armory},
				{Room::Direction::E, Room::Name::gate},
			},
			false));
	
	dungeonMap.emplace(Room::Name::gate,
		Room("A large, torchlit room with doors West, South, and East. \n There is daylight entering under the door to the East.",
			{
				{Room::Direction::S, Room::Name::jailers},
				{Room::Direction::W, Room::Name::cell},
				{Room::Direction::E, Room::Name::exit},
			},
			false));
	
	dungeonMap.emplace(Room::Name::armory,
		Room("A store room with doors North and East.",
			{
				{Room::Direction::N, Room::Name::cell},
				{Room::Direction::E, Room::Name::jailers},
			},
			false));

	dungeonMap.emplace(Room::Name::jailers,
		Room("A jailer's barracks with doors West and North.",
			{
				{Room::Direction::W, Room::Name::armory},
				{Room::Direction::N, Room::Name::gate},
			},
			false));
	dungeonMap.emplace(Room::Name::exit,
		Room("YOU FOUND THE KEY AND ESCAPED!",
			{
				{}
			},
			false));

	return dungeonMap;
}

void clearConsole() {
	system("cls");
}

void pauseConsole() {
	system("PAUSE");
}

void splashScreen() {
	std::cout << "DUNGEON ADVENTURE" << std::endl;
	std::cout << std::endl;
	std::cout << "Your name here (2020)" << std::endl;
	std::cout << "CPSC 2376, Programming II, Level 1 Quest" << std::endl;
	std::cout << "UA Little Rock, Computer Science Dept." << std::endl;
	std::cout << std::endl;
	std::cout << "INSTRUCTIONS:" << std::endl;
	std::cout << std::endl;
	std::cout << "Find the key and get out of the dungeon!" << std::endl;
	std::cout << std::endl;
	std::cout << "        (North)          " << std::endl;
	std::cout << "           w             " << std::endl;
	std::cout << "           |             " << std::endl;
	std::cout << "(West) a --+-- d (East)  " << std::endl;
	std::cout << "           |             " << std::endl;
	std::cout << "           s             " << std::endl;
	std::cout << "        (South)          " << std::endl;
	std::cout << std::endl;
	pauseConsole();
}

//randomly place the key in either the cell, the armory, the jailer's barrack, or the gate room.
void randomizeKey(std::vector<Room>& map)
{
	static std::random_device seed;
	static std::default_random_engine engine(seed());
	std::uniform_int_distribution<int> randomRoom(0, map.size() - 2);
	map.at(randomRoom(engine)).hasKey = true;
}

//output information as demonstrated in the instructions.
//You will need to deal with display changes that depend on the key.
//NOTE:: player and map are const. Update key in changeGameState
void displayGameState(const Player& player, const std::map<Room::Name, Room>& map)
{
	auto croom = map.at(player.currentRoom);

	std::cout << "View:   ";

	std::cout << croom.message << std::endl;
	if (croom.hasKey)
	{
		std::cout << "  There is a key on the floor.You pick it up." << std::endl;
	}
	std::cout << "Health: " << player.health << std::endl;
	std::cout << "Equip:  ";
	if (player.hasKey) {
		std::cout << "1 jailer's key";
	}
	std::cout << std::endl;
}

//output messages depending on if the player has one or lost.
void displayGameDone(const Player& player, const std::map<Room::Name, Room>& map)
{

	clearConsole();
	auto croom = map.at(player.currentRoom);

	if (player.health > 0)
	{
		if (player.currentRoom == Room::Name::exit)
		{
			std::cout << croom.message << std::endl;
		}
	}
	else
	{
		std::cout << "YOU DIED...RIP." << std::endl;
	}

	std::cout << std::endl;
	pauseConsole();
}


void displayIllegalMove(const Player& player, Room::Direction action)
{

	clearConsole();

	if (player.currentRoom == Room::Name::gate && !player.hasKey && action == Room::Direction::E)
	{
		std::cout << "The door is locked. \n Perhaps if you had a key???" << std::endl;
	}
	else
	{
		std::cout << "You cannot go that way." << std::endl;
	}
	std::cout << std::endl;

	pauseConsole();

}

Room::Direction getAction()
{
	char action;
	std::cout << std::endl;
	std::cout << "Select action: ";
	std::cin >> action;
	switch (action)
	{
	case 'w':
		return Room::Direction::N;
		break;
	case 'a':
		return Room::Direction::W;
		break;
	case 'd':
		return Room::Direction::E;
		break;
	case 's':
		return Room::Direction::S;
		break;

	default:
		return Room::Direction::none;
		break;
	}
}

//1. Pick up key if it is in the current room
//2. check to see if the player is illegally trying to get into the exit room.
//3. compare direction player wants to go to doors in room.
//   If legal, update room and decrement health.
//Return true if makes legal move, otherwise, return false.
bool changeGameState(Room::Direction action, Player& player, std::map<Room::Name, Room>& map, bool haskey)
{
	auto croom = map.at(player.currentRoom);

	//pick up the key if it is in this room.
	if (croom.hasKey)
	{
		player.hasKey = true;
		croom.hasKey = false;
	}

	//if we are trying to go to the exit, but we don't have the key, it is illegal.
	if (player.currentRoom == Room::Name::gate && !player.hasKey && action == Room::Direction::E)
	{
		return false;
	}
	//check for door;

	for (int i{ 0 }; i < croom.doorways.size(); i++)
	{
		if (map.at((int)player.currentRoom).doors.at(i) == action)
		{
			//found a door in the direction we are trying to go, so go that way.
			player.currentRoom = map.at((int)player.currentRoom).connectedRoom[i];
			player.health--;
			return true;
		}
	}
	return false; //if there is not a connected room in that direction, it is illegal.

}



//Check the end-of-game conditions.  If the player is out
//of health or the player has reached the exit
//then the game is over.
bool gameIsNotDone(const Player& player)
{
	return !(player.health <= 0 || player.currentRoom == Room::Name::exit);
}

