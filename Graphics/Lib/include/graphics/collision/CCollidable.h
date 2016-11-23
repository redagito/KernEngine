#pragma once

#include <glm/glm.hpp>

#include "graphics/collision/CAABBox.h"

/**
* \brief Represents a collision entity in the collision system.
*
* A collidable entity deals a certain amount of damage to the other entity.
* This is used for modeling projectile with ship collision.
*/
class CCollidable
{
public:
  /**
  * \brief The entity is created with its group id and the untransformed
  * bounding volume
  */
  CCollidable(unsigned int groupId, const CAABBox &box);

  /**
  * \brief Returns the group id
  */
  unsigned int getGroupId() const;

  /**
  * \brief Sets the amount of damage the entity deals on collision.
  */
  void setDamage(float damage);

  /**
  * \brief Returns the amount of damage the entity deals.
  */
  float getDamage() const;

  /**
  * \brief Returns the amount of damage the entity received since the last call.
  *
  * Calling this function resets the damage counter on the entity.
  */
  float getDamageReceived();

  /**
  * \brief Stores the amount of received damage.
  */
  void receiveDamage(float damage);

  /**
  * \brief Returns the bounding box.
  */
  const CAABBox &getAABBox() const;

  /**
  * \brief Sets the translation matrix for the entity
  */
  void setTranslation(const glm::vec3 &translation);

  /**
  * \brief Sets the scale matrix
  */
  void setScale(const glm::vec3 &scale);

  /**
  * \brief Sets the collision status.
  *
  * If this is set to false, the entity will get ignored while collision testing
  */
  void setCollidable(bool);

  /**
  * \brief Returns whether or not the entity should be used for collision
  * testinG
  */
  bool isCollidable() const;

  /**
  * \brief Requests deletion of the object
  */
  void markDeleted();

  /**
  * \brief Checks for deletion request
  */
  bool deleteRequested() const;

protected:
  CAABBox m_boxTransformed;   /**< Transformed collision volume, recalculated on
                                 setTranslation call. */
  CAABBox m_box;              /**< The untransformed collision volume. */
  unsigned int m_groupId = 0; /**< Collision group id. */

  float m_damageReceived =
      0.f; /**< Accumulated damage that was received since the last call to
              getDamageReceived. */
  float m_damageDealt = 0.f; /**< Amount of damage this entity deals on
                                collision to the other entity. */

  bool m_deleteMe = false; /**< Stores deletion state. */
  bool m_collidable =
      true; /**< Stores whether or not the collision entity is active. */
};