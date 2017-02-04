#pragma once

#include <string>
#include <unordered_map>
#include <cstdint>
#include <mutex>

/**
 * \brief Simple profiler class.
 *
 * TODO Make thread safe?
 */
class CProfiler
{
public:
  CProfiler(const std::string &functionName);
  ~CProfiler();

  /**
   * Write formatted profile data.
   */
  static bool write(std::ostream &stream);
  static bool hasData();

private:
  /**
  * \brief Profile data.
  */
  struct SData
  {
    uint64_t callCount = 0;         /**< Total number of calls. */
    double totalCallTime = 0.f;     /**< Total time of calls. */
    double minCallTime = 9999999.f; /**< Min call time. */
    double maxCallTime = 0.f;       /**< Max call time. */
  };

  using FunctionProfiles = std::unordered_map<std::string, SData>;

  double m_time = 0.0; /**< Start time. */
  std::string m_name;  /**< Profile name. */
  static FunctionProfiles s_profileData; /**< Profile data. */
  static std::mutex s_mutex;
};
