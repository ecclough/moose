/****************************************************************/
/*               DO NOT MODIFY THIS HEADER                      */
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*           (c) 2010 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#ifndef SUBPROBLEMINTERFACE_H
#define SUBPROBLEMINTERFACE_H

// Moose
#include "MooseVariable.h"
#include "ParallelUniqueId.h"
#include "VariableWarehouse.h"
#include "DiracKernelInfo.h"

// libMesh
#include "elem.h"
#include "node.h"
#include "MooseMesh.h"
#include "equation_systems.h"

// system
#include <string>

class MooseMesh;
class GeometricSearchData;
class Problem;

class SubProblemInterface
{
public:
  virtual EquationSystems & es() = 0;
  virtual MooseMesh & mesh() = 0;
  virtual Problem * parent() = 0;

  virtual AssemblyData & assembly(THREAD_ID tid) = 0;
  virtual QBase * & qRule(THREAD_ID tid) = 0;
  virtual const std::vector<Point> & points(THREAD_ID tid) = 0;
  virtual const std::vector<Point> & physicalPoints(THREAD_ID tid) = 0;
  virtual const std::vector<Real> & JxW(THREAD_ID tid) = 0;
  virtual QBase * & qRuleFace(THREAD_ID tid) = 0;
  virtual const std::vector<Point> & pointsFace(THREAD_ID tid) = 0;
  virtual const std::vector<Real> & JxWFace(THREAD_ID tid) = 0;
  virtual const Elem * & elem(THREAD_ID tid) = 0;
  virtual unsigned int & side(THREAD_ID tid) = 0;
  virtual const Elem * & sideElem(THREAD_ID tid) = 0;
  virtual const Node * & node(THREAD_ID tid) = 0;
  virtual DiracKernelInfo & diracKernelInfo() { return _dirac_kernel_info; }
  virtual Real final_nonlinear_residual() { return 0; }
  virtual unsigned int n_nonlinear_iterations() { return 0; }

  // Geom Search
  virtual void updateGeomSearch() = 0;
  virtual GeometricSearchData & geomSearchData() = 0;

  virtual void meshChanged() { mooseError("This system does not support changing the mesh"); }
protected:
  DiracKernelInfo _dirac_kernel_info;
};

#endif /* SUBPROBLEMINTERFACE_H */
