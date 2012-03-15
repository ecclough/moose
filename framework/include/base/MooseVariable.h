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

#ifndef MOOSEVARIABLE_H
#define MOOSEVARIABLE_H

#include "Moose.h"
#include "MooseArray.h"
#include "ParallelUniqueId.h"

// libMesh
#include "fe.h"
#include "quadrature.h"
#include "dense_vector.h"
#include "dense_matrix.h"
#include "numeric_vector.h"
#include "sparse_matrix.h"
#include "elem.h"
#include "node.h"


typedef MooseArray<Real>               VariableValue;
typedef MooseArray<RealGradient>       VariableGradient;
typedef MooseArray<RealTensor>         VariableSecond;


class Assembly;
class SubProblem;
class SystemBase;

/**
 * Class for stuff related to variables
 *
 * Each variable can compute nodal or elemental (at QPs) values.
 */
class MooseVariable
{
public:
  MooseVariable(unsigned int var_num, unsigned int mvn, const FEType & fe_type, SystemBase & sys, Assembly & assembly, Moose::VarKindType var_kind);
  virtual ~MooseVariable();

  void prepare();
  void prepareNeighbor();
  void prepare_aux();
  void reinit_node();
  void reinit_nodeNeighbor();
  void reinit_aux();
  void reinitNodes(const std::vector<unsigned int> & nodes);

  /// Get the variable number
  unsigned int number() { return _moose_var_num; }
//  int dimension() { return _dim; }

  /// Get the variable number
  const std::string & name();

  /// Kind of the variable (Nonlinear, Auxiliary, ...)
  Moose::VarKindType kind() { return _var_kind; }

  const std::set<subdomain_id_type> & activeSubdomains();

  /// Get the type of finite element object
  const FEType feType() { return _fe->get_fe_type(); }

  /// Get the order of this variable
  Order getOrder() const { return _fe->get_order(); }

  /// is this variable nodal
  bool isNodal();

  // Read-only access to FE object used by this variable
  FEBase * const & currentFE() const { return _fe; }
  /// Current element this variable is evaluated at
  const Elem * & currentElem() { return _elem; }
  /// Current side this variable is being evaluated on
  unsigned int & currentSide() { return _current_side; }
  /// Current neighboring element
  const Elem * & neighbor() { return _neighbor; }

  const std::vector<std::vector<Real> > & phi() { return _phi; }
  const std::vector<std::vector<RealGradient> > & gradPhi() { return _grad_phi; }
  const std::vector<std::vector<RealTensor> > & secondPhi() { _second_phi = &_fe->get_d2phi(); return *_second_phi; }

  const std::vector<std::vector<Real> > & phiFace() { return _phi_face; }
  const std::vector<std::vector<RealGradient> > & gradPhiFace() { return _grad_phi_face; }
  const std::vector<std::vector<RealTensor> > & secondPhiFace() { _second_phi_face = &_fe_face->get_d2phi(); return *_second_phi_face; }

  const std::vector<std::vector<Real> > & phiFaceNeighbor() { return _phi_face_neighbor; }
  const std::vector<std::vector<RealGradient> > & gradPhiFaceNeighbor() { return _grad_phi_face_neighbor; }
  const std::vector<std::vector<RealTensor> > & secondPhiFaceNeighbor() { _second_phi_face_neighbor = &_fe_face_neighbor->get_d2phi(); return *_second_phi_face_neighbor; }

  const std::vector<Point> & normals() { return _normals; }

  // damping
  VariableValue & increment() { return _increment; }

  VariableValue & sln() { return _u; }
  VariableValue & slnOld() { _need_u_old = true; return _u_old; }
  VariableValue & slnOlder() { _need_u_older = true;return _u_older; }
  VariableGradient  & gradSln() { return _grad_u; }
  VariableGradient  & gradSlnOld() { _need_grad_old = true; return _grad_u_old; }
  VariableGradient  & gradSlnOlder() { _need_grad_older = true; return _grad_u_older; }
  VariableSecond & secondSln() { _need_second = true; secondPhi(); secondPhiFace(); return _second_u; }
  VariableSecond & secondSlnOld() { _need_second_old = true; secondPhi(); secondPhiFace(); return _second_u_old; }
  VariableSecond & secondSlnOlder() { _need_second_older = true; secondPhi(); secondPhiFace(); return _second_u_older; }

  VariableValue & uDot() { return _u_dot; }
  VariableValue & duDotDu() { return _du_dot_du; }

  const Node * & node() { return _node; }
  unsigned int & nodalDofIndex() { return _nodal_dof_index; }
  bool isNodalDefined() { return _is_defined; }
  VariableValue & nodalSln() { return _nodal_u; }
  VariableValue & nodalSlnOld() { return _nodal_u_old; }
  VariableValue & nodalSlnOlder() { return _nodal_u_older; }
  VariableValue & nodalSlnDot() { return _nodal_u_dot; }
  VariableValue & nodalSlnDuDotDu() { return _nodal_du_dot_du; }

  VariableValue & slnNeighbor() { return _u_neighbor; }
  VariableValue & slnOldNeighbor() { _need_u_old_neighbor = true; return _u_old_neighbor; }
  VariableValue & slnOlderNeighbor() { _need_u_older_neighbor = true; return _u_older_neighbor; }
  VariableGradient & gradSlnNeighbor() { return _grad_u_neighbor; }
  VariableGradient & gradSlnOldNeighbor() { _need_grad_old_neighbor = true; return _grad_u_old_neighbor; }
  VariableGradient & gradSlnOlderNeighbor() { _need_grad_older_neighbor = true; return _grad_u_older_neighbor; }
  VariableSecond & secondSlnNeighbor() { _need_second_neighbor = true; secondPhiFaceNeighbor(); return _second_u_neighbor; }
  VariableSecond & secondSlnOldNeighbor() { _need_second_old_neighbor = true; secondPhiFaceNeighbor(); return _second_u_old_neighbor; }
  VariableSecond & secondSlnOlderNeighbor() { _need_second_older_neighbor = true; secondPhiFaceNeighbor(); return _second_u_older_neighbor; }

  const Node * & nodeNeighbor() { return _node_neighbor; }
  unsigned int & nodalDofIndexNeighbor() { return _nodal_dof_index_neighbor; }
  bool isNodalNeighborDefined() { return _is_defined_neighbor; }
  VariableValue & nodalSlnNeighbor() { return _nodal_u_neighbor; }
  VariableValue & nodalSlnOldNeighbor() { return _nodal_u_old_neighbor; }
  VariableValue & nodalSlnOlderNeighbor() { return _nodal_u_older_neighbor; }

  /// Compute values at interior quadrature points
  void computeElemValues();
  /// Compute values at facial quadrature points
  void computeElemValuesFace();
  /// Compute values at facial quadrature points for the neighbor
  void computeNeighborValuesFace();
  /// Compute values at quadrature points for the neighbor
  void computeNeighborValues();
  /// Compute nodal values of this variable
  void computeNodalValues();
  /// Compute nodal values of this variable in the neighbor
  void computeNodalNeighborValues();
  /// Set the nodal value for this variable (to keep everything up to date
  void setNodalValue(Number value);
  /// Compute damping for this variable based on increment_vec
  void computeDamping(const NumericVector<Number> & increment_vec);

  /// get DOF indices for currently selected element
  std::vector<unsigned int> & dofIndices() { return _dof_indices; }
  /// get DOF indices for currently selected element
  std::vector<unsigned int> & dofIndicesNeighbor() { return _dof_indices_neighbor; }

  void insert(NumericVector<Number> & residual);

  /// Get the value of this variable at given node
  Number getNodalValue(const Node & node);
  /// Get the old value of this variable at given node
  Number getNodalValueOld(const Node & node);
  /// Get the t-2 value of this variable at given node
  Number getNodalValueOlder(const Node & node);

  /// Set the scaling factor for this variable
  void scalingFactor(Real factor) { _scaling_factor = factor; }
  /// Get the scaling factor for this variable
  Real scalingFactor() { return _scaling_factor; }

  /**
   * Whether or not this variable is actually using the shape function value.
   *
   * Currently hardcoded to true because we always compute the value.
   */
  bool usesPhi() { return true; }

  /**
   * Whether or not this variable is actually using the shape function gradient.
   *
   * Currently hardcoded to true because we always compute the value.
   */
  bool usesGradPhi() { return true; }

  /**
   * Whether or not this variable is actually using the shape function second derivative.
   */
  bool usesSecondPhi() { return _need_second || _need_second_old || _need_second_older; }

protected:
  THREAD_ID _tid;                                               ///< Thread ID
  unsigned int _var_num;                                        ///< variable number (from libMesh)
  unsigned int _moose_var_num;                                  ///< variable number (MOOSE)
  Moose::VarKindType _var_kind;
  SubProblem & _subproblem;                                     ///< Problem this variable is part of
  SystemBase & _sys;                                            ///< System this variable is part of

  const DofMap & _dof_map;                                      ///< DOF map
  Assembly & _assembly;                                         ///< Assembly data

  QBase * & _qrule;                                             /// Quadrature rule for interior
  QBase * & _qrule_face;                                        /// Quadrature rule for the face

  FEBase * & _fe;                                               /// libMesh's FE object for this variable
  FEBase * & _fe_face;                                          /// libMesh's FE object for this variable on a face
  FEBase * & _fe_face_neighbor;                                 /// libMesh's FE object for this variable on a face on the neighboring element

  const Elem * & _elem;                                         /// current element
  unsigned int & _current_side;                                 /// the side of the current element (valid when doing face assembly)

  const Elem * & _neighbor;                                     /// neighboring element

  std::vector<unsigned int> _dof_indices;                       /// DOF indices
  std::vector<unsigned int> _dof_indices_neighbor;              /// DOF indices (neighbor)

  bool _is_nl;                                                  /// true if this varaible is non-linear


  bool _need_u_old;
  bool _need_u_older;

  bool _need_grad_old;
  bool _need_grad_older;

  bool _need_second;
  bool _need_second_old;
  bool _need_second_older;


  bool _need_u_old_neighbor;
  bool _need_u_older_neighbor;

  bool _need_grad_old_neighbor;
  bool _need_grad_older_neighbor;

  bool _need_second_neighbor;
  bool _need_second_old_neighbor;
  bool _need_second_older_neighbor;

  // Shape function values, gradients. second derivatives
  const std::vector<std::vector<Real> > & _phi;
  const std::vector<std::vector<RealGradient> > & _grad_phi;
  const std::vector<std::vector<RealTensor> > * _second_phi;

  // Values, gradients and second derivatives of shape function on faces
  const std::vector<std::vector<Real> > & _phi_face;
  const std::vector<std::vector<RealGradient> > & _grad_phi_face;
  const std::vector<std::vector<RealTensor> > * _second_phi_face;

  // Values, gradients and second derivatives of shape function on faces
  const std::vector<std::vector<Real> > & _phi_face_neighbor;
  const std::vector<std::vector<RealGradient> > & _grad_phi_face_neighbor;
  const std::vector<std::vector<RealTensor> > * _second_phi_face_neighbor;

  const std::vector<Point> & _normals;                                  ///< Normals at QPs on faces

  VariableValue _u;
  VariableValue _u_old;
  VariableValue _u_older;
  VariableGradient  _grad_u;
  VariableGradient  _grad_u_old;
  VariableGradient  _grad_u_older;
  VariableSecond _second_u;
  VariableSecond _second_u_old;
  VariableSecond _second_u_older;

  VariableValue _u_neighbor;
  VariableValue _u_old_neighbor;
  VariableValue _u_older_neighbor;
  VariableGradient _grad_u_neighbor;
  VariableGradient _grad_u_old_neighbor;
  VariableGradient _grad_u_older_neighbor;
  VariableSecond _second_u_neighbor;
  VariableSecond _second_u_old_neighbor;
  VariableSecond _second_u_older_neighbor;

  // time derivatives
  VariableValue _u_dot;                                                 ///< u_dot (time derivative)
  VariableValue _du_dot_du;                                             ///< derivative of u_dot wrt u

  // nodal stuff
  bool _is_defined;                                                     ///< If the variable is defined at the node (used in compute nodal values)
  bool _has_nodal_value;                                                ///< If true, the nodal value gets inserted on calling insert()
  const Node * & _node;
  unsigned int _nodal_dof_index;
  VariableValue _nodal_u;
  VariableValue _nodal_u_old;
  VariableValue _nodal_u_older;
  VariableValue _nodal_u_dot;                                           ///< nodal values of u_dot
  VariableValue _nodal_du_dot_du;                                       ///< nodal values of derivative of u_dot wrt u

  bool _is_defined_neighbor;                                            ///< If the variable is defined at the neighbor node (used in compute nodal values)
  const Node * & _node_neighbor;
  unsigned int _nodal_dof_index_neighbor;
  VariableValue _nodal_u_neighbor;
  VariableValue _nodal_u_old_neighbor;
  VariableValue _nodal_u_older_neighbor;

  // damping
  VariableValue _increment;

  Real _scaling_factor;                                                 ///< scaling factor for this variable
};

#endif /* MOOSEVARIABLE_H */
