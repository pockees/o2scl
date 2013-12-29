/*
  -------------------------------------------------------------------
  
  Copyright (C) 2006-2013, Andrew W. Steiner
  
  This file is part of O2scl.
  
  O2scl is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.
  
  O2scl is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with O2scl. If not, see <http://www.gnu.org/licenses/>.

  -------------------------------------------------------------------
*/
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <o2scl/nonrel_fermion.h>
#include <o2scl/classical.h>

using namespace std;
using namespace o2scl;
using namespace o2scl_const;

nonrel_fermion::nonrel_fermion() {
  density_root=&def_density_root;
}

nonrel_fermion::~nonrel_fermion() {
}

void nonrel_fermion::calc_mu_zerot(fermion &f) {
  if (f.non_interacting) { f.nu=f.mu; f.ms=f.m; }
  if (f.inc_rest_mass) {
    f.kf=sqrt(2.0*f.ms*(f.nu-f.m));
  } else {
    f.kf=sqrt(2.0*f.ms*f.nu);
  }
  f.n=f.kf*f.kf*f.kf*f.g/6.0/pi2;
  f.ed=f.g*pow(f.kf,5.0)/20.0/pi2/f.ms;
  if (f.inc_rest_mass) f.ed+=f.n*f.m;
  f.pr=-f.ed+f.n*f.nu;
  f.en=0.0;
  return;
}

void nonrel_fermion::calc_density_zerot(fermion &f) {
  if (f.non_interacting) { f.ms=f.m; }
  kf_from_density(f);
  f.nu=f.kf*f.kf/2.0/f.ms;
  f.ed=f.g*pow(f.kf,5.0)/20.0/pi2/f.ms;
  if (f.inc_rest_mass) {
    f.ed+=f.n*f.m;
    f.nu+=f.m;
  }
  f.pr=-f.ed+f.n*f.nu;
  f.en=0.0;
  
  if (f.non_interacting) { f.mu=f.nu; }
  return;
}

void nonrel_fermion::calc_mu(fermion &f, double temper) {
  double y, sy, spi, ey, int1, int2;

  fp=&f;
  T=temper;

  if (f.non_interacting) { f.nu=f.mu; f.ms=f.m; }
  if (temper<=0.0) {
    calc_mu_zerot(f);
    return;
  }

  if (f.inc_rest_mass) {
    y=(f.nu-f.m)/temper;
  } else {
    y=f.nu/temper;
  }

  // Number density
  f.n=gsl_sf_fermi_dirac_half(y)*sqrt(pi)/2.0;
  f.n*=f.g*pow(2.0*f.ms*temper,1.5)/4.0/pi2;

  // Energy density:
  f.ed=gsl_sf_fermi_dirac_3half(y)*0.75*sqrt(pi);

  if (f.inc_rest_mass) {
    
    // Finish energy density
    f.ed*=f.g*pow(2.0*f.ms*temper,2.5)/8.0/pi2/f.ms;
    f.ed+=f.n*f.m;
    
    // entropy density
    f.en=(5.0*(f.ed-f.n*f.m)/3.0-(f.nu-f.m)*f.n)/temper;
    
    // pressure
    f.pr=2.0*(f.ed-f.n*f.m)/3.0;
    
  } else {

    // Finish energy density
    f.ed*=f.g*pow(2.0*f.ms*temper,2.5)/8.0/pi2/f.ms;

    // entropy density
    f.en=(5.0*f.ed/3.0-f.nu*f.n)/temper;
    
    // pressure
    f.pr=2.0*f.ed/3.0;
    
  }

  if (!o2scl::is_finite(f.nu) || !o2scl::is_finite(f.n)) {
    O2SCL_ERR2("Chemical potential or density in ",
		   "nonrel_fermion::calc_mu().",exc_efailed);
  }
  
  return;
}

void nonrel_fermion::nu_from_n(fermion &f, double temper) {

  fp=&f;
  T=temper;

  // Use initial value of nu for initial guess
  double nex;
  if (f.inc_rest_mass) {
    nex=-(f.nu-f.m)/temper;
  } else {
    nex=-f.nu/temper;
  } 
  
  // Make a correction if nex is too small and negative
  // (Note GSL_LOG_DBL_MIN is about -708)
  if (nex>-GSL_LOG_DBL_MIN*0.9) nex=-GSL_LOG_DBL_MIN/2.0;
  
  funct_mfptr<nonrel_fermion> mf(this,&nonrel_fermion::solve_fun);
  
  // Turn off convergence errors temporarily, since we'll
  // try again if it fails
  bool enc=density_root->err_nonconv;
  density_root->err_nonconv=false;
  int ret=density_root->solve(nex,mf);
  density_root->err_nonconv=enc;

  if (ret!=0) {

    // If it failed, try to get a guess from classical particle
    
    classical cl;
    cl.calc_density(f,temper);
    if (f.inc_rest_mass) {
      nex=-(f.nu-f.m)/temper;
    } else {
      nex=-f.nu/temper;
    } 
    ret=density_root->solve(nex,mf);
    
    // If it failed again, add error information
    if (ret!=0) {
      O2SCL_ERR("Solver failed in nonrel_fermion::nu_from_n().",ret);
    }
  }

  if (f.inc_rest_mass) {
    f.nu=-nex*temper+f.m;
  } else {
    f.nu=-nex*temper;
  }
  
  return;
}

void nonrel_fermion::calc_density(fermion &f, double temper) {
  double y, spi, ey, sy;

  if (f.n<=0.0) {
    f.nu=0.0;
    f.mu=0.0;
    f.ed=0.0;
    f.en=0.0;
    f.pr=0.0;
    return;
  }
  
  if (f.non_interacting==true) { f.nu=f.mu; f.ms=f.m; }
  if (temper<=0.0) {
    return calc_density_zerot(f);
  }

  double guess=f.nu;
  nu_from_n(f,temper);
  
  if (f.non_interacting) { f.mu=f.nu; }

  if (f.inc_rest_mass) {
    y=-(f.nu-f.m)/temper;
  } else {
    y=-f.nu/temper;
  }

  // energy density
  f.ed=gsl_sf_fermi_dirac_3half(-y)*sqrt(pi)*0.75;

  if (f.inc_rest_mass) {
    
    // Finish energy density
    f.ed*=f.g*pow(2.0*f.ms*temper,2.5)/8.0/pi2/f.ms;
    f.ed+=f.n*f.m;
    
    // entropy density
    f.en=(5.0*(f.ed-f.n*f.m)/3.0-(f.nu-f.m)*f.n)/temper;
    
    // pressure
    f.pr=2.0*(f.ed-f.n*f.m)/3.0;
    
  } else {

    // Finish energy density
    f.ed*=f.g*pow(2.0*f.ms*temper,2.5)/8.0/pi2/f.ms;

    // entropy density
    f.en=(5.0*f.ed/3.0-f.nu*f.n)/temper;
    
    // pressure
    f.pr=2.0*f.ed/3.0;
    
  }
  
  if (f.non_interacting==true) { f.mu=f.nu; }
  
  return;

}

double nonrel_fermion::solve_fun(double x) {
  double nden;
  
  // If the argument to gsl_sf_fermi_dirac_half() is less
  // than GSL_LOG_DBL_MIN (which is about -708), then 
  // an underflow occurs. We just set nden to zero in this 
  // case
  
  if (x>fabs(GSL_LOG_DBL_MIN)) nden=0.0;
  else nden=gsl_sf_fermi_dirac_half(-x)*sqrt(pi)/2.0;

  nden*=fp->g*pow(2.0*fp->ms*T,1.5)/4.0/pi2;
  return nden/fp->n-1.0;
}

