#pragma once
#include "Object.h"
#include "Tile.h"
#include "Texture.h"
#include "Shaders.h"
#include "Vertex.h"
#include "Animation.h"
class Enemies :
    public Object
{
public:
	/*enum EnemyType
	{
		NORMAL, FAST, TANK
	};*/
	int enemyType;//1:normal, 2:fast, 3:tank
	Texture enemyTexture = Texture("../Resources/enemy.tga");
	Shaders enemyShaders;
	Vertex enemyVertices[4];
	int locationX = 0, locationY = 0;
	int lastLocationX = 0, lastLocationY = 0;
	int maxHP = 60;
	int currentHP = 60;

	float movementSpeed = 0.001f;
	int reward = 3;

	//Tile spawnTile;
	//EnemyType type;

	//float xPos;
	//float yPos;
	//Vector2 movingDirection;// x, y direction

	bool alive = true;
	bool reachedExit = false;

	GLuint enemyVBO;
	//GLuint enemyIBO;

	bool toLeft = false; //Check the last step is go to left or right
	Animation animation;

	Enemies();
	Enemies(int type);
	Enemies(Enemies* e);
	Enemies(Shaders myShaders, int maxHP, float speed, int reward);
	Enemies(Shaders myShaders, int maxHP, float speed, int reward, Tile spawner);
	
	void Bind();
	void SetShaders(Shaders myShaders);
	void SetSpawner(Tile spawner);
	void MoveEnemies();
	void MoveToLeft();
	void MoveToRight();
	void MoveUp();
	void MoveDown();
	void Draw();
	void Update();
	void Kill();
};

