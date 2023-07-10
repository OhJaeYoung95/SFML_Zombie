#include "stdafx.h"
#include "Zombie.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "Player.h"
#include "SceneDev1.h"
#include "DataTableMgr.h"
#include "DataTable.h"
#include "ZombieTable.h"

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
    attackTimer = attackRate;
}

void Zombie::Update(float dt)
{
    SpriteGo::Update(dt);

    Scene* scene = SCENE_MGR.GetCurrScene();
    SceneDev1* sceneDev1 = dynamic_cast<SceneDev1*>(scene);
    if (sceneDev1->GetIsPause())
    {
        return;
    }

    if (player == nullptr)
        return;

    // ���� �ൿ
    LookAtPlayer();
    FollowPlayer(dt);
    HitPlayer(dt);
}

void Zombie::Draw(sf::RenderWindow& window)
{
    SpriteGo::Draw(window);
}

void Zombie::SetType(Types t)
{
    zombieType = t;

    auto info = DATATABLE_MGR.Get<ZombieTable>(DataTable::Ids::Zombie)->Get(t);
    //int index = (int)zombieType;
    //zombieType = info.zombieType;
    textureId = info.textureId;
    speed = info.speed;
    damage = info.damage;
    maxHp = info.maxHp;
    attackRate = info.attackRate;
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

void Zombie::LookAtPlayer()
{    // �÷��̾� �ٶ󺸱�
    sf::Vector2f playerScreenPos = SCENE_MGR.GetCurrScene()->WorldPosToScreen(player->GetPosition());
    sf::Vector2f zombieScreenPos = SCENE_MGR.GetCurrScene()->WorldPosToScreen(position);
    //look = Utils::Normalize(playerScreenPos - zombieScreenPos);
    look = direction = Utils::Normalize(player->GetPosition() - position);
    sprite.setRotation(Utils::Angle(look));

}

void Zombie::FollowPlayer(float dt)
{
    // �÷��̾� ���󰡱�
    float distance = Utils::Distance(player->GetPosition(), position);
    if (distance > 25.f && isActive)
    {
        position += direction * speed * dt;
        sprite.setPosition(position);
    }
}

void Zombie::HitPlayer(float dt)
{
    // ������ ������
    damageTick += dt;
    if (sprite.getGlobalBounds().intersects(player->sprite.getGlobalBounds()) && damageTick > 1.f)
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
    // �������ڵ�
    //attackTimer += dt;
    //if (attackTimer > attackRate)
    //{
    //    if (player->isAlive && sprite.getGlobalBounds().intersects(player->sprite.getGlobalBounds()))
    //    {
    //        attackTimer = 0.f;
    //        // �÷��̾� �ǰ�
    //        player->OnHitted(damage);
    //    }
    //}
}
