﻿#include <iostream>
#include <cstdlib> // rand(), srand()
#include <ctime>
#include "GameManager.h"

#include "../Character/Character.h"
#include "../item/AttackBoost.h"
#include "../item/HealthPotion.h"
#include "../Monster/Monster.h"
#include "../shop/shop.h"

using namespace std;

Monster* GameManager::generateMonster(int level)
{
	srand(time(0)); //랜덤 값 초기화 //같은 값만 나온다면 메인으로
	int random = rand() % 3;  //랜덤한 수를 3으로 모듈러해서 1,2,3만 생성

	if (level == 10)
	{
	    return new BossMonster(level);
	}
	else if (random == 0)
	{
		return new Goblin(level);
	}
	else if (random == 1)
	{
		return new Orc(level);
	}
	else if (random == 2)
	{
		return new Troll(level);
	}
	else
	{
		return nullptr; //의도하지 않은 상황일 때
	}

}

void GameManager::battle(Character* player)
{
	
	// 몬스터 생성
	Monster* monster = generateMonster(player -> getLevel());
	std::cout << monster->getName() << " 등장! 체력: " << monster->getHealth() << ", 공격력 : " << monster->getAttack() << std::endl;  //몬스터 생성 알림
	std::cout << "======== ======== ========" << endl;

	// 전투 시스템
	while (monster->getHealth() > 0 && player->getHealth() > 0)
	{
		// 플레이어가 공격
		monster->takeDamage(player->getAttack());
		std::cout << player->getName() << "이(가) " << monster->getName() << "을(를) 공격했습니다. 남은 체력: " << monster->getHealth() << std::endl;
		std::cout << "======== ======== ========" << endl;

		// 체력 50% 이하 -> 힐 아이템 사용 (인벤토리 자동 탐색)
		if (player->getHealth() <= player->getMaxHealth()*0.5f)
		{
			for (int i = 0; i < player->getInventorySize(); i++)
			{
				Item* item = player->getItem(i);
				if (item->getName() == "HealthPotion")
				{
					item->use(player);
					std::cout << item->getName() << " 사용!" << std::endl;
					std::cout << "======== ======== ========" << endl;
					player->removeItem(i);
					break;
				}
			}
		}
		// 체력 75% 이하? 
		else if (player->getHealth() <= player->getMaxHealth()*0.75)
		{
			for (int i = 0; i < player->getInventorySize(); i++)
			{
				Item* item = player->getItem(i);
				if (item->getName() == "AttackBoost")
				{
					item->use(player);
					std::cout << item->getName() << " 사용!" << std::endl;
					std::cout << "======== ======== ========" << endl;
					player->removeItem(i);
					break;
				}
			}
		}

		// 몬스터 죽음 체크
		if (monster->getHealth() <= 0)
		{
			if (monster->getName() == "Dragon")
			{
				clear = true;
				break;
			}
			std:: cout << monster->getName() << "와(과) 싸움에서 승리했다!" << endl;
			std:: cout << "======== ======== ========" << endl;
			int gold = 10 + rand() % 11;   //10~20 골드 랜덤 획득
			player->addGold(gold);
			player->addExperience(50);   //임의의 숫자 50 경험치 얻기 //여기 숫자 조절하기
			if (player->getExperience() == 100)  //100이 되면 레벨업
			{
				player->levelUp();
			}

			// 캐릭터 상태 명시
			std::cout << player->getName() << "이(가) 50 EXP와 " << gold << " 골드를 획득했습니다!" << std::endl;
			std::cout << "======== ======== ========" << endl;
				
			// 아이템 드랍
			std::unique_ptr<Item> dropped = monster->dropItem();
			if (dropped) {
				std::cout << dropped->getName() << " 아이템을 획득했습니다!" << std::endl;
				std::cout << "======== ======== ========" << endl;
				player->addItem(std::move(dropped));
				
				
			}
			
			delete monster;
			break;
		}
		
		player->takeDamage(monster->getAttack());   //몬스터가 공격할 때
		std:: cout << monster->getName() << "이(가) " << player->getName() << "을(를) 공격했습니다. 남은 체력: " << player->getHealth() << endl;
		std:: cout << "======== ======== ========" << endl;

		if (player->getHealth() <= 0)
		{
			std:: cout << player->getName() << "이(가) 사망했습니다. 게임 오버!" << endl;
			std::cout << "======== ======== ========" << endl;
			delete monster;
			Dead = true;
			break;
		};
		
	}
}
bool GameManager::isAllEnemyDefeated()
{
	return clear;
}
bool GameManager::isDead()
{
	return Dead;
}
void GameManager::displayInventory(Character* player)
{
	player->displayInventory();
}

void GameManager::visitShop(Character* player)
{
    Shop shop(player); // Shop 객체 생성 시 player 전달
    shop.interact();   // Shop과 상호작용
}
