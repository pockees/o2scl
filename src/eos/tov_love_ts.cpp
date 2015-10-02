/*
  -------------------------------------------------------------------
  
  Copyright (C) 2015, Andrew W. Steiner
  
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

#include <o2scl/test_mgr.h>
#include <o2scl/tov_love.h>
#include <o2scl/tov_solve.h>
#include <o2scl/eos_had_apr.h>
#include <o2scl/nstar_cold.h>

using namespace std;
using namespace o2scl;
using namespace o2scl_const;

int main(void) {

  cout.setf(ios::scientific);

  test_mgr t;
  t.set_output_level(2);

  eos_had_apr apr;
  apr.pion=1;
  
  nstar_cold nst;
  nst.set_eos(apr);
  nst.calc_eos();
  
  o2_shared_ptr<table_units<> >::type te=nst.get_eos_results();

  eos_tov_interp eti;
  eti.default_low_dens_eos();
  eti.read_table(*te,"ed","pr","nb");

  tov_solve ts;
  ts.verbose=0;
  ts.set_eos(eti);
  ts.calc_gpot=true;
  ts.ang_vel=true;
  ts.fixed(1.4);

  o2_shared_ptr<table_units<> >::type profile=ts.get_results();
  profile->summary(&cout);

  tov_love tl;
  tl.tab=profile;
  double yR, beta, k2, lambda_km5, lambda_cgs;
  cout << "Here." << endl;
  tl.calc_H(yR,beta,k2,lambda_km5,lambda_cgs);

  cout << lambda_cgs << endl;

  t.report();
  
  return 0;
}

