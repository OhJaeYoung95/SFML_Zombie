#include "stdafx.h"
#include "Zombie.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "Player.h"
#include "SceneDev1.h"

const std::string Zombie::textureIds[3] =
{
	"graphics/bloater.png",
	"graphics/chaser.png",
	"graphics/crawler.png",
};

const float Zombie::speedStats[3] = { 40.f, 70.f, 20.f };
const int Zombie::damageStats[3] = { 1, 5, 3 };
const int Zombie::hpStats[3] = { 100, 75, 50 };



Zombie::Zombie(const std::string n)
    : SpriteGo("", n)
{
}

Zombie::~Zombie()
{
}

void Zombie::Init()
{
    SpriteGo::Init();
    SetOrigin(Origins::MC);
}

void Zombie::Release()
{
    SpriteGo::Release();
}

void Zombie::Reset()
{
    SpriteGo::Reset();
    hp = maxHp;
}

void Zombie::Update(float dt)
{
    SpriteGo::Update(dt);

    //if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num1))
    //{
    //    SetType(Types::Bloater);
    //    Reset();
    //}
    if (player == nullptr)
        return;

    float distance = Utils::Distance(player->GetPosition(), position);

    // �÷��̾� �ٶ󺸱�
    sf::Vector2f playerScreenPos = SCENE_MGR.GetCurrScene()->WorldPosToScreen(player->GetPosition());
    sf::Vector2f zombieScreenPos = SCENE_MGR.GetCurrScene()->WorldPosToScreen(position);
    //look = Utils::Normalize(playerScreenPos - zombieScreenPos);
    look = direction = Utils::Normalize(player->GetPosition() - position);
    sprite.setRotation(Utils::Angle(look));

    // �÷��̾� ���󰡱�
    if (distance > 25.f)
    {
        position += direction * speed * dt;
        sprite.setPosition(position);
    }
    
    // ������ ������
    damageTick += dt;
    if (sprite.getGlobalBounds().intersects(player->sprite.getGlobalBounds())&& damageTick > 1.f)
    {
        isHit = true;
        damageTick = 0.f;
        player->sprite.setColor(sf::Color::Red);
        player->HpDecrease(damage);
    }

    if (damageTick > 1.0f && isHit)
    {
        isHit = false;
        player->sprite.setColor(sf::Color::White);
    }
}

void Zombie::Draw(sf::RenderWindow& window)
{
    SpriteGo::Draw(window);
}

void Zombie::SetType(Types t)
{
    zombieType = t;
    int index = (int)zombieType;

    textureId = textureIds[index];
    speed = speedStats[index];
    maxHp = hpStats[index];
    damage = damageStats[index];
}

Zombie::Types Zombie::GetType() const
{
    return zombieType;
}

void Zombie::SetPlayer(Player* player)
{
    this->player = player;
}

void Zombie::OnHitBullet(int damage)
{
    hp -= damage;
    if (hp <= 0)
    {
        Scene* scene = SCENE_MGR.GetCurrScene();
        SceneDev1* sceneDev1 = dynamic_cast<SceneDev1*>(scene);
        if (sceneDev1 != nullptr)
        {
            sceneDev1->OnDieZombie(this);
        }
    }
}