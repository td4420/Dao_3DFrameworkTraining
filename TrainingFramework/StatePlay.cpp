#include "stdafx.h"
#include "StatePlay.h"
#include <chrono>

StatePlay::StatePlay()
{
	Object* upgradeButton = nullptr;
	Object* sellButton = nullptr;
	Object* nextWaveButton = nullptr;
	Object* bgPlay = nullptr;
	Object* background = nullptr;
	Object* hpIcon = nullptr;
	Object* moneyIcon = nullptr;
}

void StatePlay::init(int mapType)
{
	lives->init();
	money->init();
	//towerStat->init();
	gameOverText->init();
	winText->init();
	
	myShaders->Init("../Resources/Shaders/TriangleShaderVS.vs", "../Resources/Shaders/TriangleShaderFS.fs");
	
	pf.Init(myShaders, mapType);
	for (int i = 0; i < pf.mapHeight; i++)
	{
		for (int j = 0; j < pf.mapWidth; j++)
		{
			NumMap[i][j] = pf.currentNumMap[i][j];
		}
	}

	//add Button Tower 
	Frame* frameArcher = new Frame();
	Frame* frameMortar = new Frame();
	Frame* frameSlow = new Frame();
	Frame* frameWitch = new Frame();
	frameList.push_back(frameArcher);
	frameList.push_back(frameMortar);
	frameList.push_back(frameSlow);
	frameList.push_back(frameWitch);

	Object* archerTowerButton = new Object();
	archerTowerButton->o_Model = new Model("../Resources/model.nfg");
	archerTowerButton->o_Texture.push_back(new Texture("../ResourcesPacket/Textures/archerTowerButton.tga"));
	archerTowerButton->Build(9 * 0.15f, 1.5f * -0.2f);
	frameArcher->Build(9 * 0.15f, 1.5f * -0.2f);

	Object* mortarTowerButton = new Object();
	mortarTowerButton->o_Model = new Model("../Resources/model.nfg");
	mortarTowerButton->o_Texture.push_back(new Texture("../ResourcesPacket/Textures/mortarTowerButton.tga"));
	mortarTowerButton->Build(11 * 0.15f, 1.5f * -0.2f);
	frameMortar->Build(11 * 0.15f, 1.5f * -0.2f);

	Object* slowTowerButton = new Object();
	slowTowerButton->o_Model = new Model("../Resources/model.nfg");
	slowTowerButton->o_Texture.push_back(new Texture("../ResourcesPacket/Textures/slowTowerButton.tga"));
	slowTowerButton->Build(9 * 0.15f, 3.5f * -0.2f);
	frameSlow->Build(9 * 0.15f, 3.5f * -0.2f);

	Object* witchTowerButton = new Object();
	witchTowerButton->o_Model = new Model("../Resources/model.nfg");
	witchTowerButton->o_Texture.push_back(new Texture("../ResourcesPacket/Textures/witchTowerButton.tga"));
	witchTowerButton->Build(11 * 0.15f, 3.5f * -0.2f);
	frameWitch->Build(11 * 0.15f, 3.5f * -0.2f);

	towerButtonList.push_back(archerTowerButton);
	towerButtonList.push_back(mortarTowerButton);
	towerButtonList.push_back(slowTowerButton);
	towerButtonList.push_back(witchTowerButton);

	
	for (int i = 0; i < towerButtonList.size(); i++) {
		towerButtonList.at(i)->o_shaders = myShaders;
		towerButtonList.at(i)->InitObject();

		frameList.at(i)->o_shaders = myShaders;
		frameList.at(i)->InitObject();
	}

	

	// init upgrade button
	upgradeButton = new Object();
	upgradeButton->o_Model = new Model("../Resources/model.nfg");
	upgradeButton->o_Texture.push_back(new Texture("../ResourcesPacket/Textures/upgradeButton.tga"));
	upgradeButton->Build(12.0f * 0.15f, 0.0f * -0.2f);
	

	// init sell button
	sellButton = new Object();
	sellButton->o_Model = new Model("../Resources/model.nfg");
	sellButton->o_Texture.push_back(new Texture("../ResourcesPacket/Textures/sellButton.tga"));
	sellButton->Build(11.0f * 0.15f, 0.0f * -0.2f);

	//functionButtonList.push_back(bugFixButton);
	functionButtonList.push_back(sellButton);
	functionButtonList.push_back(upgradeButton);

	for (int i = 0; i < functionButtonList.size(); i++)
	{
		functionButtonList.at(i)->o_shaders = myShaders;
		functionButtonList.at(i)->InitObject();
	}

	// init next wave button
	nextWaveButton = new Object();
	nextWaveButton->o_Model = new Model("../Resources/model.nfg");
	nextWaveButton->o_Texture.push_back(new Texture("../ResourcesPacket/Textures/NextWaveButton.tga"));
	nextWaveButton->Build(8.0f * 0.15f, 0.0f * -0.2f);
	nextWaveButton->o_shaders = myShaders;
	nextWaveButton->InitObject();

	bgPlay = new Object();
	bgPlay->o_Model = new Model("../Resources/modelBackground.nfg");
	bgPlay->o_Texture.push_back(new Texture("../ResourcesPacket/Textures/bgPlay1.tga"));
	bgPlay->o_shaders = myShaders;
	bgPlay->Build(0 * 0.15f, 0 * 0.2f);
	bgPlay->InitObject();

	background = new Object();
	background->o_Model = new Model("../Resources/modelBackground.nfg");
	background->o_Texture.push_back(new Texture("../ResourcesPacket/Textures/bgPlay.tga"));
	background->o_shaders = myShaders;
	background->Build(0 * 0.15f, 0 * 0.2f);
	background->InitObject();

	hpIcon = new Object();
	hpIcon->o_Model = new Model("../Resources/model.nfg");
	hpIcon->o_Texture.push_back(new Texture("../ResourcesPacket/Textures/hp.tga"));
	hpIcon->Build(10 * 0.15f, 5.2f * -0.2f);
	hpIcon->o_shaders = myShaders;
	hpIcon->InitObject();

	moneyIcon = new Object();
	moneyIcon->o_Model = new Model("../Resources/model.nfg");
	moneyIcon->o_Texture.push_back(new Texture("../ResourcesPacket/Textures/money.tga"));
	moneyIcon->Build(10 * 0.15f, 6.2f * -0.2f);
	moneyIcon->o_shaders = myShaders;
	moneyIcon->InitObject();

}

void StatePlay::Draw(Shaders * textShaders)
{
	if (pf.gameOver)
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

		background->DrawObject();
		gameOverText->RenderText(textShaders);

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
	}

	if (pf.uWin)
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

		background->DrawObject();
		winText->RenderText(textShaders);

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
	}

	if (!pf.gameOver && !pf.uWin) {
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

		for (int i = 0; i < towerList.size(); i++) {
			towerList.at(i)->DrawObject();
			towerList.at(i)->Shoot();
		}

		for (int i = 0; i < frameList.size(); i++) {
			frameList.at(i)->DrawObject();
		}

		for (int i = 0; i < towerButtonList.size(); i++) {
			towerButtonList.at(i)->DrawObject();
		}

		for (int i = 0; i < functionButtonList.size(); i++)
		{
			functionButtonList.at(i)->DrawObject();
		}
		
		pf.Draw(textShaders);

		if (pf.waveEnd) {
			nextWaveButton->DrawObject();
		}
		bgPlay->DrawObject();
		hpIcon->DrawObject();
		moneyIcon->DrawObject();

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);

		lives->RenderText(textShaders);
		money->RenderText(textShaders);

		/*if (mouseOnTower)
		{
			towerStat->RenderText(textShaders);
		}*/
	}
}

void StatePlay::Update(float deltaTime)
{
	if (!pf.uWin && !pf.gameOver) {

		pf.Update(deltaTime);

		for (int i = 0; i < towerList.size(); i++) {
			towerList.at(i)->Update(pf.enemyWave);
		}

		strLives = std::to_string(pf.HP);
		lives->text = strLives.c_str();

		strMoney = std::to_string(pf.money);
		money->text = strMoney.c_str();
	}
}

bool StatePlay::CheckSelectionOption(int x, int y)
{
	for (int i = 0; i < towerButtonList.size(); i++) {
		float x_tower = towerButtonList.at(i)->o_position.x / 0.15f * 70;
		float y_tower = towerButtonList.at(i)->o_position.y / -0.2f * 70;
		if (x_tower <= x && x <= x_tower + 70 && y_tower <= y && y <= y_tower + 70) {
			selectMenuOption = i;
			//printf("\ntower selection is: %d", i);
			return true;
		}
	}
	float x_upgrade_button = upgradeButton->o_position.x / 0.15f * 70;
	float y_upgrade_button = upgradeButton->o_position.y / -0.2f * 70;
	if (x_upgrade_button <= x && x <= x_upgrade_button + 70
		&& y_upgrade_button <= y && y <= y_upgrade_button + 70) {
		selectMenuOption = towerButtonList.size();
		//printf("\nupgrade button is selected ");
		return true;
	}

	float x_sell_button = sellButton->o_position.x / 0.15f * 70;
	float y_sell_button = sellButton->o_position.y / -0.2f * 70;
	if (x_sell_button <= x && x <= x_sell_button + 70
		&& y_sell_button <= y && y <= y_sell_button + 70) {
		selectMenuOption = towerButtonList.size() + 1;
		return true;
	}
	float x_wave_button = nextWaveButton->o_position.x / 0.15f * 70;
	float y_wave_button = nextWaveButton->o_position.y / -0.2f * 70;
	if (x_wave_button <= x && x <= x_wave_button + 70
		&& y_wave_button <= y && y <= y_wave_button + 70) {
		selectMenuOption = towerButtonList.size() + 2;
		return true;
	}
	return false;
}

bool StatePlay::IsBuildable(int xPos, int yPos)
{
	for (int i = 0; i < towerList.size(); i++) {
		Vector3 o_position = towerList.at(i)->o_position;
		if (xPos * 0.15f == o_position.x && yPos * -0.2f == o_position.y) {
			return false;
		}
	}
	return true;
}
int StatePlay::FindIndexOfTower(int x, int y)
{
	int xPos = static_cast<int>(std::round(x / 70));
	int yPos = static_cast<int>(std::round(y / 70));
	for (int i = 0; i < towerList.size(); i++) {
		Vector3 o_positon = towerList.at(i)->o_position;
		if (xPos * 0.15f == o_positon.x && yPos * -0.2f == o_positon.y && towerList.at(i)!=nullptr) {
			/*mouseOnTower = true;
			
			strTowerDmg = std::to_string(towerList.at(i)->damage);
			strTowerSellFor = std::to_string(towerList.at(i)->cost / 2);
			strTowerStats = "Damage " + strTowerDmg + " " + strTowerSellFor + "$";

			towerStat->text = strTowerStats.c_str();*/
			//cout << "Mouse on tower at tile " << xPos << " " << yPos << endl;
			return i;
		}
		else
		{
			mouseOnTower = false;
		}
	}
	return -1;
}

void StatePlay::OnMouseClick(int x, int y)
{

	if (selectMenuOption == -1) {
		CheckSelectionOption(x, y);
	}

	//upgrade or build tower
	if (selectMenuOption != -1) {
		//build Tower
		if (selectMenuOption > -1 && selectMenuOption < towerButtonList.size()) {
			int xPos = static_cast<int>(std::round(x / 70));
			int yPos = static_cast<int>(std::round(y / 70));
			if (-1 < xPos && xPos < 8 && -1 < yPos && yPos < 7) {
				if (NumMap[yPos][xPos] == 0 && IsBuildable(xPos, yPos)) {
		
					Tower* t = new Tower(selectMenuOption);
					if (pf.money >= t->cost) {
						//Shaders s = Shaders();
						//s.Init("../Resources/Shaders/TriangleShaderVS.vs", "../Resources/Shaders/TriangleShaderFS.fs");
						t->o_shaders = myShaders;
						t->Build(xPos, yPos);
						t->InitObject();
						towerList.push_back(t);

						pf.money -= t->cost;
					}
					else {
						//not enought money
						cout << "Not enough money!" << endl;
						t->bullet->CleanUp();
						delete t->bullet;
						t->CleanUp();
						delete t;
					}
					
				}
				selectMenuOption = -1;
			}
			else if (!CheckSelectionOption(x, y)) selectMenuOption = -1;

		}
		else if (selectMenuOption == towerButtonList.size()) {
			int index = FindIndexOfTower(x, y);
			if (index != -1) {
				if (towerList.at(index)->upgrade < 2 && pf.money >= towerList.at(index)->cost / 2) {
					pf.money -= towerList.at(index)->cost / 2;
					towerList.at(index)->Upgrade();
				}

				selectMenuOption = -1;
			}
			if (!CheckSelectionOption(x, y)) selectMenuOption = -1;

		}
		//sell Button
		else if (selectMenuOption == towerButtonList.size() + 1) {
			int index = FindIndexOfTower(x, y);
			if (index != -1) {
				pf.money += towerList.at(index)->cost / 2;
				cout << "Tower sold for: " << towerList.at(index)->cost / 2 << "$" << endl;
				towerList.at(index)->projectileOnScreen.clear();
				delete towerList.at(index)->bullet;
				delete towerList.at(index);
				towerList.erase(towerList.begin() + index);
				SoundController::GetInstance()->sellTowerS.play();
				selectMenuOption = -1;
			}
			if (!CheckSelectionOption(x, y)) selectMenuOption = -1;
			
		}
		else if (selectMenuOption == towerButtonList.size() + 2) {

			if (pf.waveEnd) {
				pf.nextWave = true;
				//printf("\n next wave btn");
			}

			if (!CheckSelectionOption(x, y)) selectMenuOption = -1;
		}
	}
}

void StatePlay::OnMouseOver(int x, int y) {
	FindIndexOfTower(x, y);
}
void StatePlay::CleanUp()
{
	delete myShaders;
	background->CleanUp();
	delete background;
	bgPlay->CleanUp();
	delete bgPlay;

	for (int i = 0; i < towerList.size(); i++) {
		towerList.at(i)->projectileOnScreen.clear();
		towerList.at(i)->bullet->CleanUp();
		delete towerList.at(i)->bullet;
		towerList.at(i)->CleanUp();
		delete towerList.at(i);
	}

	for (int i = 0; i < towerButtonList.size(); i++) {
		towerButtonList.at(i)->CleanUp();
		delete towerButtonList.at(i);
		frameList.at(i)->CleanUp();
		delete frameList.at(i);
	}

	for (int i = 0; i < functionButtonList.size(); i++)
	{
		functionButtonList.at(i)->CleanUp();
		delete functionButtonList.at(i);
	}
	nextWaveButton->CleanUp();
	delete nextWaveButton;

	hpIcon->CleanUp();
	delete hpIcon;

	moneyIcon->CleanUp();
	delete moneyIcon;

	//lives->CleanUp();
	delete[] lives->fileFont;
	delete lives;
	//money->CleanUp();
	delete[] money->fileFont;
	delete money;
	//delete[] towerStat->fileFont;
	//delete towerStat;

	gameOverText->CleanUp();
	delete gameOverText;
	winText->CleanUp();
	delete winText;

	pf.CleanUp();
}

void StatePlay::CleanUpIfNotInit()
{
	delete myShaders;

	//lives->CleanUp();
	delete[] lives->fileFont;
	delete lives;
	//money->CleanUp();
	delete[] money->fileFont;
	delete money;
	//delete[] towerStat->fileFont;
	//delete towerStat;

	gameOverText->CleanUp();
	delete gameOverText;
	winText->CleanUp();
	delete winText;

	pf.CleanUpEnemies();
}
