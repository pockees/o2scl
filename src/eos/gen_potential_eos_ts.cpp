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

#include <o2scl/gen_potential_eos.h>
#include <o2scl/test_mgr.h>
#include <o2scl/nonrel_fermion.h>
#include <o2scl/deriv_gsl.h>

using namespace std;
using namespace o2scl;
using namespace o2scl_const;

int main(void) {
  gen_potential_eos go;
  thermo &th=go.def_thermo;
  test_mgr t;
  t.set_output_level(1);

  double dtemp, n0, ed_nuc, ed_neut;
  
  cout.setf(ios::scientific);

  cout << "MDI0:" << endl;
  {

    go.Au=-95.98/hc_mev_fm;
    go.Al=-120.57/hc_mev_fm;
    go.B=106.35/hc_mev_fm;
    go.Cu=-103.40/hc_mev_fm;
    go.Cl=-11.70/hc_mev_fm;
    go.sigma=4.0/3.0;
    go.x=0.0;
    go.rho0=0.16;
    go.Lambda=cbrt(1.5*pi2*go.rho0);
    go.form=go.mdi_form;

    n0=go.fn0(0.0,dtemp);
    cout << "n0:      " << n0 << endl;
    cout << "eoa:    " << dtemp*hc_mev_fm << endl;
    cout << "K:       " << go.fcomp(n0)*hc_mev_fm << endl;
    cout << "Esym(1): " << go.fesym(n0)*hc_mev_fm << endl;
    cout << "M*/M:    " << go.def_neutron.ms/go.def_neutron.m << endl;
    
    t.test_rel(n0,0.16,1.0e-2,"n0");
    t.test_rel(dtemp*hc_mev_fm,-16,1.0e-2,"eoa");
    t.test_rel(go.fcomp(n0)*hc_mev_fm,220.0,40.0,"comp");
    t.test_rel(go.fesym(n0)*hc_mev_fm,30.0,5.0,"esym");
    
    // alternative for symmetry energy
    go.def_neutron.n=n0/2.0;
    go.def_proton.n=n0/2.0;
    go.calc_e(go.def_neutron,go.def_proton,th);
    ed_nuc=th.ed;
    go.def_neutron.n=n0;
    go.def_proton.n=0.0;
    go.calc_e(go.def_neutron,go.def_proton,th);
    ed_neut=th.ed;
    cout << "Esym(2): " << (ed_neut-ed_nuc)/n0*hc_mev_fm << endl;
    cout << endl;
  }

  cout << "MDI1:" << endl;
  {

    go.Au=-187.27/hc_mev_fm;
    go.Al=-29.28/hc_mev_fm;
    go.B=106.35/hc_mev_fm;
    go.Cu=-103.40/hc_mev_fm;
    go.Cl=-11.70/hc_mev_fm;
    go.sigma=4.0/3.0;
    go.x=1.0;
    go.rho0=0.16;
    go.Lambda=cbrt(1.5*pi2*go.rho0);
    go.form=go.mdi_form;
  
    n0=go.fn0(0.0,dtemp);
    cout << "n0:      " << n0 << endl;
    cout << "eoa:    " << dtemp*hc_mev_fm << endl;
    cout << "K:       " << go.fcomp(n0)*hc_mev_fm << endl;
    cout << "Esym(1): " << go.fesym(n0)*hc_mev_fm << endl;
    cout << "M*/M:    " << go.def_neutron.ms/go.def_neutron.m << endl;
    
    t.test_rel(n0,0.16,1.0e-2,"n0");
    t.test_rel(dtemp*hc_mev_fm,-16,1.0e-2,"eoa");
    t.test_rel(go.fcomp(n0)*hc_mev_fm,220.0,40.0,"comp");
    t.test_rel(go.fesym(n0)*hc_mev_fm,30.0,5.0,"esym");
    
    // alternative for symmetry energy
    go.def_neutron.n=n0/2.0;
    go.def_proton.n=n0/2.0;
    go.calc_e(go.def_neutron,go.def_proton,th);
    ed_nuc=th.ed;
    go.def_neutron.n=n0;
    go.def_proton.n=0.0;
    go.calc_e(go.def_neutron,go.def_proton,th);
    ed_neut=th.ed;
    cout << "Esym(2): " << (ed_neut-ed_nuc)/n0*hc_mev_fm << endl;
    cout << endl;
  }

  cout << "BGBD_bombaci:" << endl;
  {

    go.A=-144.0/hc_mev_fm;
    go.B=203.3/hc_mev_fm;
    go.Bp=0.0;
    go.sigma=7.0/6.0;
    go.C1=-75.0/hc_mev_fm;
    go.C2=0.0;
    go.x0=0.0;
    go.x3=-0.4;
    go.z1=-3.42/hc_mev_fm;
    go.z2=0.0;
    go.rho0=0.16;
    go.Lambda=1.5*cbrt(1.5*pi2*go.rho0);
    go.Lambda2=3.0*cbrt(1.5*pi2*go.rho0);
    go.form=go.bgbd_form;
  
    n0=go.fn0(0.0,dtemp);
    cout << "n0:      " << n0 << endl;
    cout << "eoa:    " << dtemp*hc_mev_fm << endl;
    cout << "K:       " << go.fcomp(n0)*hc_mev_fm << endl;
    cout << "Esym(1): " << go.fesym(n0)*hc_mev_fm << endl;
    cout << "M*/M:    " << go.def_neutron.ms/go.def_neutron.m << endl;
    
    // alternative for symmetry energy
    go.def_neutron.n=n0/2.0;
    go.def_proton.n=n0/2.0;
    go.calc_e(go.def_neutron,go.def_proton,th);
    ed_nuc=th.ed;
    go.def_neutron.n=n0;
    go.def_proton.n=0.0;
    go.calc_e(go.def_neutron,go.def_proton,th);
    ed_neut=th.ed;
    cout << "Esym(2): " << (ed_neut-ed_nuc)/n0*hc_mev_fm << endl;
    cout << endl;
  }

  cout << "BGBD_das:" << endl;
  {

    go.Au=-192.0/hc_mev_fm;
    go.Al=-96.0/hc_mev_fm;
    go.B=203.3/hc_mev_fm;
    go.Cu=-84.53/hc_mev_fm;
    go.Cl=-65.472/hc_mev_fm;
    go.sigma=7.0/6.0;
    go.x=1.0/15.0;
    go.rho0=0.16;
    go.Lambda=1.5*cbrt(1.5*pi2*go.rho0);
    go.form=go.gbd_form;
  
    n0=go.fn0(0.0,dtemp);
    cout << "n0:      " << n0 << endl;
    cout << "eoa:    " << dtemp*hc_mev_fm << endl;
    cout << "K:       " << go.fcomp(n0)*hc_mev_fm << endl;
    cout << "Esym(1): " << go.fesym(n0)*hc_mev_fm << endl;
    cout << "M*/M:    " << go.def_neutron.ms/go.def_neutron.m << endl;
    
    // alternative for symmetry energy
    go.def_neutron.n=n0/2.0;
    go.def_proton.n=n0/2.0;
    go.calc_e(go.def_neutron,go.def_proton,th);
    ed_nuc=th.ed;
    go.def_neutron.n=n0;
    go.def_proton.n=0.0;
    go.calc_e(go.def_neutron,go.def_proton,th);
    ed_neut=th.ed;
    cout << "Esym(2): " << (ed_neut-ed_nuc)/n0*hc_mev_fm << endl;
    cout << endl;
  }

  cout << "BPAL11:" << endl;
  {

    go.A=75.94/hc_mev_fm;
    go.B=-30.880/hc_mev_fm;
    go.Bp=0.0;
    go.sigma=0.498;
    go.C1=-83.84/hc_mev_fm;
    go.C2=23.0/hc_mev_fm;
    go.x0=0.0;
    go.x3=0.0;
    go.z1=0.0;
    go.z2=0.0;
    go.sym_index=1;
    go.rho0=0.16;
    go.Lambda=1.5*cbrt(1.5*pi2*go.rho0);
    go.Lambda2=3.0*cbrt(1.5*pi2*go.rho0);
    go.form=go.bpal_form;
    
    n0=go.fn0(0.0,dtemp);
    cout << "n0:      " << n0 << endl;
    cout << "eoa:    " << dtemp*hc_mev_fm << endl;
    cout << "K:       " << go.fcomp(n0)*hc_mev_fm << endl;
    cout << "Esym(1): " << go.fesym(n0)*hc_mev_fm << endl;
    cout << "M*/M:    " << go.def_neutron.ms/go.def_neutron.m << endl;
    cout << endl;
    
  }
  
  cout << "BPAL31:" << endl;
  {

    go.A=-46.65/hc_mev_fm;
    go.B=39.54/hc_mev_fm;
    go.Bp=0.3;
    go.sigma=1.663;
    go.C1=-83.84/hc_mev_fm;
    go.C2=23.0/hc_mev_fm;
    go.x0=0.0;
    go.x3=0.0;
    go.z1=0.0;
    go.z2=0.0;
    go.sym_index=1;
    go.rho0=0.16;
    go.Lambda=1.5*cbrt(1.5*pi2*go.rho0);
    go.Lambda2=3.0*cbrt(1.5*pi2*go.rho0);
    go.form=go.bpal_form;
    
    n0=go.fn0(0.0,dtemp);
    cout << "n0:      " << n0 << endl;
    cout << "eoa:    " << dtemp*hc_mev_fm << endl;
    cout << "K:       " << go.fcomp(n0)*hc_mev_fm << endl;
    cout << "Esym(1): " << go.fesym(n0)*hc_mev_fm << endl;
    cout << "M*/M:    " << go.def_neutron.ms/go.def_neutron.m << endl;
    cout << endl;
    
  }
  
  cout << "BPAL32:" << endl;
  {

    go.A=-46.65/hc_mev_fm;
    go.B=39.54/hc_mev_fm;
    go.Bp=0.3;
    go.sigma=1.663;
    go.C1=-83.84/hc_mev_fm;
    go.C2=23.0/hc_mev_fm;
    go.x0=0.0;
    go.x3=0.0;
    go.z1=0.0;
    go.z2=0.0;
    go.sym_index=2;
    go.rho0=0.16;
    go.Lambda=1.5*cbrt(1.5*pi2*go.rho0);
    go.Lambda2=3.0*cbrt(1.5*pi2*go.rho0);
    go.form=go.bpal_form;
    
    n0=go.fn0(0.0,dtemp);
    cout << "n0:      " << n0 << endl;
    cout << "eoa:    " << dtemp*hc_mev_fm << endl;
    cout << "K:       " << go.fcomp(n0)*hc_mev_fm << endl;
    cout << "Esym(1): " << go.fesym(n0)*hc_mev_fm << endl;
    cout << "M*/M:    " << go.def_neutron.ms/go.def_neutron.m << endl;
    cout << endl;
    
  }

  cout << "BPAL33:" << endl;
  {

    go.A=-46.65/hc_mev_fm;
    go.B=39.54/hc_mev_fm;
    go.Bp=0.3;
    go.sigma=1.663;
    go.C1=-83.84/hc_mev_fm;
    go.C2=23.0/hc_mev_fm;
    go.x0=0.0;
    go.x3=0.0;
    go.z1=0.0;
    go.z2=0.0;
    go.sym_index=3;
    go.rho0=0.16;
    go.Lambda=1.5*cbrt(1.5*pi2*go.rho0);
    go.Lambda2=3.0*cbrt(1.5*pi2*go.rho0);
    go.form=go.bpal_form;
    
    n0=go.fn0(0.0,dtemp);
    cout << "n0:      " << n0 << endl;
    cout << "eoa:    " << dtemp*hc_mev_fm << endl;
    cout << "K:       " << go.fcomp(n0)*hc_mev_fm << endl;
    cout << "Esym(1): " << go.fesym(n0)*hc_mev_fm << endl;
    cout << "M*/M:    " << go.def_neutron.ms/go.def_neutron.m << endl;
    cout << endl;
    
  }

  cout << "BPALb11:" << endl;
  {

    go.A=75.94/hc_mev_fm;
    go.B=-30.880/hc_mev_fm;
    go.Bp=0.0;
    go.sigma=0.498;
    go.C1=-83.84/hc_mev_fm;
    go.C2=23.0/hc_mev_fm;
    go.x0=-1.361;
    go.x3=-0.244;
    go.z1=-13.91/hc_mev_fm;
    go.z2=16.69/hc_mev_fm;
    go.rho0=0.16;
    go.Lambda=1.5*cbrt(1.5*pi2*go.rho0);
    go.Lambda2=3.0*cbrt(1.5*pi2*go.rho0);
    go.form=go.bpalb_form;
  
    go.def_neutron.n=0.08;
    go.def_proton.n=0.08;
    n0=go.fn0(0.0,dtemp);
    cout << "n0:      " << n0 << endl;
    cout << "eoa:    " << dtemp*hc_mev_fm << endl;
    cout << "K:       " << go.fcomp(n0)*hc_mev_fm << endl;
    cout << "Esym(1): " << go.fesym(n0)*hc_mev_fm << endl;
    cout << "M*/M:    " << go.def_neutron.ms/go.def_neutron.m << endl;
    cout << endl;
    
  }

  cout << "BPALb12:" << endl;
  {

    go.A=75.94/hc_mev_fm;
    go.B=-30.880/hc_mev_fm;
    go.Bp=0.0;
    go.sigma=0.498;
    go.C1=-83.84/hc_mev_fm;
    go.C2=23.0/hc_mev_fm;
    go.x0=-1.361;
    go.x3=-0.244;
    go.z1=-13.91/hc_mev_fm;
    go.z2=16.69/hc_mev_fm;
    go.rho0=0.16;
    go.Lambda=1.5*cbrt(1.5*pi2*go.rho0);
    go.Lambda2=3.0*cbrt(1.5*pi2*go.rho0);
    go.form=go.bpalb_form;
  
    go.def_neutron.n=0.08;
    go.def_proton.n=0.08;
    n0=go.fn0(0.0,dtemp);
    cout << "n0:      " << n0 << endl;
    cout << "eoa:    " << dtemp*hc_mev_fm << endl;
    cout << "K:       " << go.fcomp(n0)*hc_mev_fm << endl;
    cout << "Esym(1): " << go.fesym(n0)*hc_mev_fm << endl;
    cout << "M*/M:    " << go.def_neutron.ms/go.def_neutron.m << endl;
    cout << endl;
    
  }

  cout << "BPALb13:" << endl;
  {

    go.A=75.94/hc_mev_fm;
    go.B=-30.880/hc_mev_fm;
    go.Bp=0.0;
    go.sigma=0.498;
    go.C1=-83.84/hc_mev_fm;
    go.C2=23.0/hc_mev_fm;
    go.x0=-1.903;
    go.x3=-1.056;
    go.z1=-1.83/hc_mev_fm;
    go.z2=5.09/hc_mev_fm;
    go.rho0=0.16;
    go.Lambda=1.5*cbrt(1.5*pi2*go.rho0);
    go.Lambda2=3.0*cbrt(1.5*pi2*go.rho0);
    go.form=go.bpalb_form;
  
    go.def_neutron.n=0.08;
    go.def_proton.n=0.08;
    n0=go.fn0(0.0,dtemp);
    cout << "n0:      " << n0 << endl;
    cout << "eoa:    " << dtemp*hc_mev_fm << endl;
    cout << "K:       " << go.fcomp(n0)*hc_mev_fm << endl;
    cout << "Esym(1): " << go.fesym(n0)*hc_mev_fm << endl;
    cout << "M*/M:    " << go.def_neutron.ms/go.def_neutron.m << endl;
    cout << endl;
    
  }

  cout << "BPALb21:" << endl;
  {

    go.A=440.94/hc_mev_fm;
    go.B=-213.41/hc_mev_fm;
    go.Bp=0.0;
    go.sigma=0.927;
    go.C1=-83.84/hc_mev_fm;
    go.C2=23.0/hc_mev_fm;
    go.x0=0.086;
    go.x3=0.561;
    go.z1=-18.4/hc_mev_fm;
    go.z2=46.27/hc_mev_fm;
    go.rho0=0.16;
    go.Lambda=1.5*cbrt(1.5*pi2*go.rho0);
    go.Lambda2=3.0*cbrt(1.5*pi2*go.rho0);
    go.form=go.bpalb_form;
  
    go.def_neutron.n=0.08;
    go.def_proton.n=0.08;
    n0=go.fn0(0.0,dtemp);
    cout << "n0:      " << n0 << endl;
    cout << "eoa:    " << dtemp*hc_mev_fm << endl;
    cout << "K:       " << go.fcomp(n0)*hc_mev_fm << endl;
    cout << "Esym(1): " << go.fesym(n0)*hc_mev_fm << endl;
    cout << "M*/M:    " << go.def_neutron.ms/go.def_neutron.m << endl;
    cout << endl;
    
  }

  cout << "BPALb22:" << endl;
  {

    go.A=440.94/hc_mev_fm;
    go.B=-213.41/hc_mev_fm;
    go.Bp=0.0;
    go.sigma=0.927;
    go.C1=-83.84/hc_mev_fm;
    go.C2=23.0/hc_mev_fm;
    go.x0=0.086;
    go.x3=0.561;
    go.z1=-18.4/hc_mev_fm;
    go.z2=46.27/hc_mev_fm;
    go.rho0=0.16;
    go.Lambda=1.5*cbrt(1.5*pi2*go.rho0);
    go.Lambda2=3.0*cbrt(1.5*pi2*go.rho0);
    go.form=go.bpalb_form;
  
    go.def_neutron.n=0.08;
    go.def_proton.n=0.08;
    n0=go.fn0(0.0,dtemp);
    cout << "n0:      " << n0 << endl;
    cout << "eoa:    " << dtemp*hc_mev_fm << endl;
    cout << "K:       " << go.fcomp(n0)*hc_mev_fm << endl;
    cout << "Esym(1): " << go.fesym(n0)*hc_mev_fm << endl;
    cout << "M*/M:    " << go.def_neutron.ms/go.def_neutron.m << endl;
    cout << endl;
    
  }

  cout << "BPALb23:" << endl;
  {

    go.A=440.94/hc_mev_fm;
    go.B=-213.41/hc_mev_fm;
    go.Bp=0.0;
    go.sigma=0.927;
    go.C1=-83.84/hc_mev_fm;
    go.C2=23.0/hc_mev_fm;
    go.x0=0.086;
    go.x3=0.561;
    go.z1=-18.4/hc_mev_fm;
    go.z2=46.27/hc_mev_fm;
    go.rho0=0.16;
    go.Lambda=1.5*cbrt(1.5*pi2*go.rho0);
    go.Lambda2=3.0*cbrt(1.5*pi2*go.rho0);
    go.form=go.bpalb_form;
  
    go.def_neutron.n=0.08;
    go.def_proton.n=0.08;
    n0=go.fn0(0.0,dtemp);
    cout << "n0:      " << n0 << endl;
    cout << "eoa:    " << dtemp*hc_mev_fm << endl;
    cout << "K:       " << go.fcomp(n0)*hc_mev_fm << endl;
    cout << "Esym(1): " << go.fesym(n0)*hc_mev_fm << endl;
    cout << "M*/M:    " << go.def_neutron.ms/go.def_neutron.m << endl;
    cout << endl;
    
  }

  cout << "BPALb31:" << endl;
  {

    go.A=-46.65/hc_mev_fm;
    go.B=39.45/hc_mev_fm;
    go.Bp=0.3;
    go.sigma=1.663;
    go.C1=-83.84/hc_mev_fm;
    go.C2=23.0/hc_mev_fm;
    go.x0=0.376;
    go.x3=0.246;
    go.z1=-12.23/hc_mev_fm;
    go.z2=-2.98/hc_mev_fm;
    go.rho0=0.16;
    go.Lambda=1.5*cbrt(1.5*pi2*go.rho0);
    go.Lambda2=3.0*cbrt(1.5*pi2*go.rho0);
    go.form=go.bpalb_form;
  
    go.def_neutron.n=0.08;
    go.def_proton.n=0.08;
    n0=go.fn0(0.0,dtemp);
    cout << "n0:      " << n0 << endl;
    cout << "eoa:    " << dtemp*hc_mev_fm << endl;
    cout << "K:       " << go.fcomp(n0)*hc_mev_fm << endl;
    cout << "Esym(1): " << go.fesym(n0)*hc_mev_fm << endl;
    cout << "M*/M:    " << go.def_neutron.ms/go.def_neutron.m << endl;
    cout << endl;
    
  }

  /*

    These next two don't work:

   */
  cout << "BPALb32:" << endl;
  if (false) {

    go.A=-46.65/hc_mev_fm;
    go.B=39.54/hc_mev_fm;
    go.Bp=0.3;
    go.sigma=0.498;
    go.C1=-83.84/hc_mev_fm;
    go.C2=23.0/hc_mev_fm;
    go.x0=0.0;
    go.x3=0.0;
    go.z1=0.0;
    go.z2=0.0;
    go.sym_index=2;
    go.rho0=0.16;
    go.Lambda=1.5*cbrt(1.5*pi2*go.rho0);
    go.Lambda2=3.0*cbrt(1.5*pi2*go.rho0);
    go.form=go.bpalb_form;
  
    n0=go.fn0(0.0,dtemp);
    cout << "n0:      " << n0 << endl;
    cout << "eoa:    " << dtemp*hc_mev_fm << endl;
    cout << "K:       " << go.fcomp(n0)*hc_mev_fm << endl;
    cout << "Esym(1): " << go.fesym(n0)*hc_mev_fm << endl;
    cout << "M*/M:    " << go.def_neutron.ms/go.def_neutron.m << endl;
    cout << endl;
    
  }

  cout << "BPALb33:" << endl;
  if (false) {

    go.A=-46.65/hc_mev_fm;
    go.B=39.54/hc_mev_fm;
    go.Bp=0.3;
    go.sigma=0.498;
    go.C1=-83.84/hc_mev_fm;
    go.C2=23.0/hc_mev_fm;
    go.x0=0.0;
    go.x3=0.0;
    go.z1=0.0;
    go.z2=0.0;
    go.sym_index=2;
    go.rho0=0.16;
    go.Lambda=1.5*cbrt(1.5*pi2*go.rho0);
    go.Lambda2=3.0*cbrt(1.5*pi2*go.rho0);
    go.form=go.bpalb_form;
  
    n0=go.fn0(0.0,dtemp);
    cout << "n0:      " << n0 << endl;
    cout << "eoa:    " << dtemp*hc_mev_fm << endl;
    cout << "K:       " << go.fcomp(n0)*hc_mev_fm << endl;
    cout << "Esym(1): " << go.fesym(n0)*hc_mev_fm << endl;
    cout << "M*/M:    " << go.def_neutron.ms/go.def_neutron.m << endl;
    cout << endl;
    
  }

  cout << "SL12:" << endl;
  {

    go.A=3.706/hc_mev_fm;
    go.B=-31.155/hc_mev_fm;
    go.Bp=0.0;
    go.sigma=0.453;
    go.C1=-41.28/hc_mev_fm;
    go.C2=23.0/hc_mev_fm;
    go.x0=-3.548;
    go.x3=-0.5;
    go.z1=-13.355/hc_mev_fm;
    go.z2=2.789/hc_mev_fm;
    go.rho0=0.16;
    go.Lambda=1.5*cbrt(1.5*pi2*go.rho0);
    go.Lambda2=3.0*cbrt(1.5*pi2*go.rho0);
    go.form=go.sl_form;
  
    n0=go.fn0(0.0,dtemp);
    cout << "n0:      " << n0 << endl;
    cout << "eoa:    " << dtemp*hc_mev_fm << endl;
    cout << "K:       " << go.fcomp(n0)*hc_mev_fm << endl;
    cout << "Esym(1): " << go.fesym(n0)*hc_mev_fm << endl;
    cout << "M*/M:    " << go.def_neutron.ms/go.def_neutron.m << endl;
    
    // alternative for symmetry energy
    go.def_neutron.n=n0/2.0;
    go.def_proton.n=n0/2.0;
    go.calc_e(go.def_neutron,go.def_proton,th);
    ed_nuc=th.ed;
    go.def_neutron.n=n0;
    go.def_proton.n=0.0;
    go.calc_e(go.def_neutron,go.def_proton,th);
    ed_neut=th.ed;
    cout << "Esym(2): " << (ed_neut-ed_nuc)/n0*hc_mev_fm << endl;
    cout << endl;
  }

  cout << "GBD0:" << endl;
  {

    go.Au=-109.85/hc_mev_fm;
    go.Al=-191.30/hc_mev_fm;
    go.B=205.66/hc_mev_fm;
    go.Cu=-118.80/hc_mev_fm;
    go.Cl=-26.26/hc_mev_fm;
    go.sigma=7.0/6.0;
    go.x=0.0;
    go.rho0=0.16;
    go.Lambda=1.5*cbrt(1.5*pi2*go.rho0);
    go.form=go.gbd_form;
  
    n0=go.fn0(0.0,dtemp);
    cout << "n0:      " << n0 << endl;
    cout << "eoa:    " << dtemp*hc_mev_fm << endl;
    cout << "K:       " << go.fcomp(n0)*hc_mev_fm << endl;
    cout << "Esym(1): " << go.fesym(n0)*hc_mev_fm << endl;
    cout << "M*/M:    " << go.def_neutron.ms/go.def_neutron.m << endl;
    
    t.test_rel(n0,0.16,1.0e-2,"n0");
    t.test_rel(dtemp*hc_mev_fm,-16,1.0e-2,"eoa");
    t.test_rel(go.fcomp(n0)*hc_mev_fm,220.0,40.0,"comp");
    t.test_rel(go.fesym(n0)*hc_mev_fm,30.0,5.0,"esym");
    
    // alternative for symmetry energy
    go.def_neutron.n=n0/2.0;
    go.def_proton.n=n0/2.0;
    go.calc_e(go.def_neutron,go.def_proton,th);
    ed_nuc=th.ed;
    go.def_neutron.n=n0;
    go.def_proton.n=0.0;
    go.calc_e(go.def_neutron,go.def_proton,th);
    ed_neut=th.ed;
    cout << "Esym(2): " << (ed_neut-ed_nuc)/n0*hc_mev_fm << endl;
    cout << endl;
  }

  cout << "GBD1:" << endl;
  {

    go.Au=-299.69/hc_mev_fm;
    go.Al=-1.46/hc_mev_fm;
    go.B=205.66/hc_mev_fm;
    go.Cu=-118.80/hc_mev_fm;
    go.Cl=-26.26/hc_mev_fm;
    go.sigma=7.0/6.0;
    go.x=1.0;
    go.rho0=0.16;
    go.Lambda=1.5*cbrt(1.5*pi2*go.rho0);
  
    n0=go.fn0(0.0,dtemp);
    cout << "n0:      " << n0 << endl;
    cout << "eoa:    " << dtemp*hc_mev_fm << endl;
    cout << "K:       " << go.fcomp(n0)*hc_mev_fm << endl;
    cout << "Esym(1): " << go.fesym(n0)*hc_mev_fm << endl;
    cout << "M*/M:    " << go.def_neutron.ms/go.def_neutron.m << endl;
    
    t.test_rel(n0,0.16,1.0e-2,"n0");
    t.test_rel(dtemp*hc_mev_fm,-16,1.0e-2,"eoa");
    t.test_rel(go.fcomp(n0)*hc_mev_fm,220.0,40.0,"comp");
    t.test_rel(go.fesym(n0)*hc_mev_fm,30.0,5.0,"esym");
    
    // alternative for symmetry energy
    go.def_neutron.n=n0/2.0;
    go.def_proton.n=n0/2.0;
    go.calc_e(go.def_neutron,go.def_proton,th);
    ed_nuc=th.ed;
    go.def_neutron.n=n0;
    go.def_proton.n=0.0;
    go.calc_e(go.def_neutron,go.def_proton,th);
    ed_neut=th.ed;
    cout << "Esym(2): " << (ed_neut-ed_nuc)/n0*hc_mev_fm << endl;
    cout << endl;
  }

  cout << "CKL: " << endl;

  go.B=106.35/hc_mev_fm;
  go.sigma=4.0/3.0;
  go.rho0=0.16;
  go.Cu=-103.40/hc_mev_fm;
  go.Cl=-11.70/hc_mev_fm;
  go.form=go.mdi_form;
  go.Lambda=cbrt(1.5*pi2*go.rho0);
  
  for(go.x=-2.0;go.x<1.01;go.x+=1.0) {
    
    
    go.Au=-95.98/hc_mev_fm-2.0*go.B*go.x/(go.sigma+1.0);
    go.Al=-120.75/hc_mev_fm+2.0*go.B*go.x/(go.sigma+1.0);
    
    {
      n0=go.fn0(0.0,dtemp);
      cout << "n0:      " << n0 << endl;
      cout << "eoa:    " << dtemp*hc_mev_fm << endl;
      cout << "K:       " << go.fcomp(n0)*hc_mev_fm << endl;
      cout << "Esym(1): " << go.fesym(n0)*hc_mev_fm << endl;
      cout << "M*/M:    " << go.def_neutron.ms/go.def_neutron.m << endl;

      t.test_rel(n0,0.16,1.0e-2,"n0");
      t.test_rel(dtemp*hc_mev_fm,-16,1.0e-2,"eoa");
      t.test_rel(go.fcomp(n0)*hc_mev_fm,220.0,40.0,"comp");
      t.test_rel(go.fesym(n0)*hc_mev_fm,30.0,5.0,"esym");
    
      // alternative for symmetry energy
      go.def_neutron.n=n0/2.0;
      go.def_proton.n=n0/2.0;
      go.calc_e(go.def_neutron,go.def_proton,th);
      ed_nuc=th.ed;
      go.def_neutron.n=n0;
      go.def_proton.n=0.0;
      go.calc_e(go.def_neutron,go.def_proton,th);
      ed_neut=th.ed;
      cout << "Esym(2): " << (ed_neut-ed_nuc)/n0*hc_mev_fm << endl;
      cout << endl;
    }
  }

  t.report();
  return 0;
}
