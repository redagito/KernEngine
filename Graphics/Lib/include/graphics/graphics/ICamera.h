#pragma once

#include <glm/glm.hpp>

enum class FogType
{
  None,
  Linear,
  Exp,
  Exp2
};

enum class RenderMode
{
  Final,
  Color,
  Depth,
  Normals,
  Lights,
  GodRay
};

struct SFeatureInfo
{
  FogType fogType = FogType::None;
  bool fogActive = false; // TODO Use this flag instead FogType::NONE
  float fogBegin = 0.f;
  float fogEnd = 0.f;
  float fogDensity = 0.f;

  RenderMode renderMode = RenderMode::Final;

  bool shadowsActive = true;
  bool frustumCullingActive = true; /**< View frustum culling enabled flag. */

  bool dofActive = true;
  float dofNearFocus = 0.5f;
  float dofNearBlur = 0.1f;
  float dofFarFocus = 10.f;
  float dofFarBlur = 150.f;

  bool fxaaActive = true;
  bool lenseFlareActive = true;
  bool godRayActive = true;
  bool toonActive = false;

  bool normalMappingActive = true;
  // Hackyyy
  mutable unsigned int culledObjectCount = 0;
};

/**
 * \brief Camera interface class.
 */
class ICamera
{
public:
  virtual ~ICamera();

  /**
  * \brief Returns view matrix.
  */
  virtual const glm::mat4 &getView() const = 0;

  /**
  * \brief Returns projection matrix.
  */
  virtual const glm::mat4 &getProjection() const = 0;

  virtual const glm::vec3 &getPosition() const = 0;

  // TODO extract functionality to a better place
  virtual SFeatureInfo &getFeatureInfoForWrite()
  {
    return m_info;
  } // TODO Rename
  virtual const SFeatureInfo &getFeatureInfo() const { return m_info; }

private:
  SFeatureInfo m_info;
};
