#include "kern/graphics/collision/Collidable.h"

Collidable::Collidable(unsigned int group, const AABBox &box)
    : m_boxTransformed(box), m_box(box), m_groupId(group)
{
}

float Collidable::getDamage() const { return m_damageDealt; }

// Returns accumuated damage and resets damage counter
float Collidable::getDamageReceived()
{
    float temp = m_damageReceived;
    m_damageReceived = 0.f;
    return temp;
}

// Returns group id
unsigned int Collidable::getGroupId() const { return m_groupId; }

// Add damage to the internal counter
void Collidable::receiveDamage(float damage) { m_damageReceived += damage; }

// Sets the damage dealt by this entity
void Collidable::setDamage(float damage) { m_damageDealt = damage; }

const AABBox &Collidable::getAABBox() const { return m_boxTransformed; }

void Collidable::setScale(const glm::vec3 &scale)
{
    m_boxTransformed.setHalfWidths(scale);
    return;
}

void Collidable::setTranslation(const glm::vec3 &translate)
{
    m_boxTransformed.setMid(translate);
    return;
}

// Sets collidable state
void Collidable::setCollidable(bool state) { m_collidable = state; }

// Returns collidable state
bool Collidable::isCollidable() const { return m_collidable; }

bool Collidable::deleteRequested() const { return m_deleteMe; }

void Collidable::markDeleted() { m_deleteMe = true; }
