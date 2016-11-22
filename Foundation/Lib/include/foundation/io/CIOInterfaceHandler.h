#pragma once

#include "foundation/interface/AInterfaceHandler.h"

class CIOInterfaceHandler : public AInterfaceHandler
{
public:
  void onAttach() override;
  void onDetach() override;
  bool handle(const std::shared_ptr<AMessage> &msg) override;
  void open() override;
  void update() override;
  void close() override;
};
