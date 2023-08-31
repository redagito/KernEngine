#pragma once

/**
 * \brief Possible listener events.
 */
enum class ResourceEvent
{
    Create, /**< Specified resource was created. */
    Change, /**< Specified resource was changed. */
    Delete  /**< Specified resource was deleted. */
};
