#include <cassert>
#include <sstream>
#include <iostream>
#include <fstream>

#include "foundation/debug/CProfiler.h"
#include "foundation/util/Time.h"

std::mutex CProfiler::s_mutex;

std::unordered_map<std::string, CProfiler::SData> CProfiler::s_profileData;

CProfiler::CProfiler(const std::string &name) : m_time(getTime()), m_name(name)
{
  return;
}

CProfiler::~CProfiler()
{
  m_time = (getTime() - m_time) * 1000.0;

  // Lock map
  // TODO Per entry locking
  std::lock_guard<std::mutex> lock(s_mutex);

  // Get entry
  CProfiler::SData &data = s_profileData[m_name];

  // Update call count and call time
  data.callCount++;
  data.totalCallTime += m_time;

  // Update max and min times
  if (data.maxCallTime < m_time)
  {
    data.maxCallTime = m_time;
  }
  if (data.minCallTime > m_time)
  {
    data.minCallTime = m_time;
  }
}

bool CProfiler::write(std::ostream &stream)
{
  stream << "[Profile data]" << std::endl;

  // Lock map
  std::lock_guard<std::mutex> lock(s_mutex);
  // Write profiling statistics
  for (const auto &data : s_profileData)
  {
    stream << "Name: " << data.first << std::endl;
    stream << "Call count: " << data.second.callCount << std::endl;
    stream << "Total call time: " << data.second.totalCallTime << " [ms]"
           << std::endl;
    stream << "Min call time: " << data.second.minCallTime << " [ms]"
           << std::endl;
    stream << "Max call time: " << data.second.maxCallTime << " [ms]"
           << std::endl;
    stream << "Average call time: "
           << data.second.totalCallTime / data.second.callCount << " [ms]"
           << std::endl;
    stream << "---------------------------------" << std::endl;
  }
  return stream.good();
}
