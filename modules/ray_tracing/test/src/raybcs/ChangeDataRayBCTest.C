//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ChangeDataRayBCTest.h"

registerMooseObject("RayTracingTestApp", ChangeDataRayBCTest);

InputParameters
ChangeDataRayBCTest::validParams()
{
  auto params = GeneralRayBC::validParams();
  params.addRequiredParam<std::string>("data_name", "Name of the data to change");
  params.addParam<Real>("add_value", 0, "Value to add to the data");
  params.addParam<Real>("scale_value", 1, "Value to scale the data with");
  return params;
}

ChangeDataRayBCTest::ChangeDataRayBCTest(const InputParameters & params)
  : GeneralRayBC(params),
    _ray_data_index(_study.getRayDataIndex(getParam<std::string>("data_name"))),
    _add_value(getParam<Real>("add_value")),
    _scale_value(getParam<Real>("scale_value"))
{
  if (params.isParamSetByUser("add_value") && params.isParamSetByUser("scale_value"))
    mooseError(_error_prefix, ": Cannot add and scale value");
}

void
ChangeDataRayBCTest::onBoundary(const unsigned int /* num_applying */)
{
  currentRay()->data(_ray_data_index) *= _scale_value;
  currentRay()->data(_ray_data_index) += _add_value;
}
