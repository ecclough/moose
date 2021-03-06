//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "SinglePhaseFluidPropertiesTestUtils.h"
#include "FlinakFluidPropertiesTest.h"

/**
 * Test that the fluid name is correctly returned
 */
TEST_F(FlinakFluidPropertiesTest, fluidName) { EXPECT_EQ(_fp->fluidName(), "flinak"); }

/**
 * Test that the molar mass is correctly returned
 */
TEST_F(FlinakFluidPropertiesTest, molarMass)
{
  ABS_TEST(_fp->molarMass(), 41.291077435E-3, REL_TOL_SAVED_VALUE);
}

/**
 * Test that the thermal conductivity and its derivatives are
 * correctly computed.
 */
TEST_F(FlinakFluidPropertiesTest, thermalConductivity)
{
  const Real T = 800.0;
  const Real p = 3.0 * 101325;
  const Real e = _fp->e_from_p_T(p, T);
  const Real v = 1. / _fp->rho_from_p_T(p, T);

  REL_TEST(_fp->k_from_v_e(v, e), 0.83, REL_TOL_SAVED_VALUE);
  REL_TEST(_fp->k_from_p_T(p, T), 0.83, REL_TOL_SAVED_VALUE);
  DERIV_TEST(_fp->k_from_p_T, p, T, REL_TOL_DERIVATIVE);
}

/**
 * Test that the viscosity and its derivatives are correctly computed.
 */
TEST_F(FlinakFluidPropertiesTest, viscosity)
{
  const Real T = 800.0;
  const Real p = 3.0 * 101325;
  const Real e = _fp->e_from_p_T(p, T);
  const Real v = 1. / _fp->rho_from_p_T(p, T);

  REL_TEST(_fp->mu_from_v_e(v, e), 0.0073420946394096, REL_TOL_SAVED_VALUE);
  REL_TEST(_fp->mu_from_p_T(p, T), 0.0073420946394096, REL_TOL_SAVED_VALUE);
  DERIV_TEST(_fp->mu_from_p_T, p, T, REL_TOL_DERIVATIVE);
}

/**
 * Test that the isobaric specific heat and its derivatives are correctly computed.
 */
TEST_F(FlinakFluidPropertiesTest, isobaricSpecificHeat)
{
  const Real T = 800.0;
  const Real p = 3.0 * 101325;
  const Real e = _fp->e_from_p_T(p, T);
  const Real v = 1. / _fp->rho_from_p_T(p, T);

  REL_TEST(_fp->cp_from_v_e(v, e), 2010.0, REL_TOL_SAVED_VALUE);
  REL_TEST(_fp->cp_from_p_T(p, T), 2010.0, REL_TOL_SAVED_VALUE);
  DERIV_TEST(_fp->cp_from_p_T, p, T, REL_TOL_DERIVATIVE);
  DERIV_TEST(_fp->cp_from_v_e, v, e, REL_TOL_DERIVATIVE);
}

/**
 * Test that the isochoric specific heat and its derivatives are correctly computed.
 */
TEST_F(FlinakFluidPropertiesTest, isochoricSpecificHeat)
{
  const Real T = 800.0;
  const Real p = 3.0 * 101325;
  const Real e = _fp->e_from_p_T(p, T);
  const Real v = 1. / _fp->rho_from_p_T(p, T);

  REL_TEST(_fp->cv_from_v_e(v, e), 45.55316141330309, REL_TOL_SAVED_VALUE);
  DERIV_TEST(_fp->cv_from_v_e, v, e, REL_TOL_DERIVATIVE);
}

/**
 * Test that the density and its derivatives are correctly computed.
 */
TEST_F(FlinakFluidPropertiesTest, density)
{
  const Real T = 800.0;
  const Real p = 3.0 * 101325;

  ABS_TEST(_fp->rho_from_p_T(p, T), 2145.035107086000153, REL_TOL_SAVED_VALUE);
  DERIV_TEST(_fp->rho_from_p_T, p, T, 4.0e-6);
}

/**
 * Test that the specific internal energy and its derivatives are correctly computed.
 */
TEST_F(FlinakFluidPropertiesTest, specificInternalEnergy)
{
  const Real T = 800.0;
  const Real p = 3.0 * 101325;

  ABS_TEST(_fp->e_from_p_T(p, T), 1607858.289032661588863, REL_TOL_SAVED_VALUE);
  DERIV_TEST(_fp->e_from_p_T, p, T, REL_TOL_DERIVATIVE);
}

/**
 * Test that the pressure and its derivatives are correctly computed.
 */
TEST_F(FlinakFluidPropertiesTest, pressure)
{
  const Real T = 800.0;
  const Real p = 3.0 * 101325;
  const Real e = _fp->e_from_p_T(p, T);
  const Real v = 1. / _fp->rho_from_p_T(p, T);

  // test pressure and its derivatives with respect to v and e. We need a slightly
  // larger tolerance for pressure because the compressibility is so small we are
  // polluted by a little numerical error.
  REL_TEST(_fp->p_from_v_e(v, e), p, 6e-11);
  DERIV_TEST(_fp->p_from_v_e, v, e, 2e-9);
}

/**
 * Test that the temperature and its derivatives are correctly computed.
 */
TEST_F(FlinakFluidPropertiesTest, temperature)
{
  const Real T = 800.0;
  const Real p = 3.0 * 101325;
  const Real e = _fp->e_from_p_T(p, T);
  const Real v = 1. / _fp->rho_from_p_T(p, T);

  REL_TEST(_fp->T_from_v_e(v, e), T, REL_TOL_SAVED_VALUE);
  DERIV_TEST(_fp->T_from_v_e, v, e, REL_TOL_DERIVATIVE);
}

/**
 * Test that the specific enthalpy and its derivatives are correctly computed.
 */
TEST_F(FlinakFluidPropertiesTest, specificEnthalpy)
{
  const Real T = 800.0;
  const Real p = 3.0 * 101325;

  ABS_TEST(_fp->h_from_p_T(p, T), 1.608e+06, REL_TOL_SAVED_VALUE);
  DERIV_TEST(_fp->h_from_p_T, p, T, REL_TOL_DERIVATIVE);
}
