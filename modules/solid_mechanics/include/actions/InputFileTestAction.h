// Header file for InputFileAction
#pragma once

#include "Action.h"

class InputFileTestAction : public Action
{
public:
  static InputParameters validParams();

  InputFileTestAction(const InputParameters & params);

  void act() override;
};