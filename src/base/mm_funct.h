/*
  -------------------------------------------------------------------
  
  Copyright (C) 2006-2016, Andrew W. Steiner
  
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
#ifndef O2SCL_MM_FUNCT_H
#define O2SCL_MM_FUNCT_H

/** \file mm_funct.h
    \brief Function object classes for multi-dimensional functions
*/

#include <string>

#include <boost/numeric/ublas/vector.hpp>

#include <o2scl/shunting-yard.h>

#ifndef DOXYGEN_NO_O2NS
namespace o2scl {
#endif

  /// Array of multi-dimensional functions typedef
  typedef std::function<
    int(size_t,const boost::numeric::ublas::vector<double> &,
	boost::numeric::ublas::vector<double> &) > mm_funct11;

  /** \brief Array of multi-dimensional functions in an array of strings
   */
  class mm_funct11_strings {

  public:
      
    /** \brief Specify the strings
     */
    template<class vec_string_t=std::vector<std::string> >
      mm_funct11_strings(int nv, vec_string_t &formulas,
			 vec_string_t &var_arr) {

      st_nv=nv;
      st_forms.resize(nv);
      st_vars.resize(nv);
      calc.resize(nv);
      for (int i=0;i<nv;i++) {
	calc[i].compile(formulas[i],&vars);
	st_vars[i]=var_arr[i];
	st_forms[i]=formulas[i];
      }
    }
      
    virtual ~mm_funct11_strings() {
    };
      
    /** \brief Set the values of the auxilliary parameters that were
	specified in 'parms' in the constructor
    */
    int set_parm(std::string name, double val) {
      vars[name]=val;
      return 0;
    }
      
    /** \brief Compute \c nv functions, \c y, of \c nv variables
	stored in \c x with parameter \c pa.
    */
    template<class vec_t=boost::numeric::ublas::vector<double> >
      int operator()(size_t nv, const vec_t &x, vec_t &y) {

      for(int i=0;i<nv;i++) {
	vars[st_vars[i]]=x[i];
      }
      for(int i=0;i<nv;i++) {
	y[i]=calc[i].eval(&vars);
      }
      return 0;
    }
      
    /// Set the functions
    template<class vec_string_t=std::vector<std::string> >
      void set_function(int nv, vec_string_t &formulas,
			vec_string_t &var_arr) {

      st_nv=nv;
      st_forms.resize(nv);
      st_vars.resize(nv);
      calc.resize(nv);
      for (int i=0;i<nv;i++) {
	calc[i].compile(formulas[i],&vars);
	st_vars[i]=var_arr[i];
	st_forms[i]=formulas[i];
      }

      return;
    }
      
#ifndef DOXYGEN_INTERNAL
      
  protected:
      
    /// The function parser
    std::vector<calculator> calc;
      
    /// Desc
    std::map<std::string,double> vars;
      
    /// The formulas
    std::vector<std::string> st_forms;
      
    /// The variables
    std::vector<std::string> st_vars;
      
    /// Desc
    int st_nv;
      
    mm_funct11_strings() {};
      
  private:
      
    mm_funct11_strings(const mm_funct11_strings &);
    mm_funct11_strings& operator=(const mm_funct11_strings&);
      
#endif
      
  };
    
#ifdef O2SCL_NEVER_DEFINED
  /** \brief A wrapper to specify \ref o2scl::mm_funct11-like objects 
      to GSL
  */
  template<class vec_t>
    class mm_funct_gsl : public gsl_multiroot_function {
    
  public:
    
    typedef std::function<int(size_t,const vec_t &, vec_t &)> func_t;

  protected:
    
    /// The function wrapper
    static int funct_wrap(const gsl_vector *x, void *params,
			  gsl_vector *f) {
      func_t *fp=(func_t *)params;
      vec_t x2(x->size), f2(x->size);
      o2scl::vector_copy<double *,vec_t>(x->size,x.data,x2);
      int ret=(*fp)(x->size,x2,f2);
      o2scl::vector_copy<vec_t,double *>(x->size,f2,f.data);
      return ret;
    }

  public:

    /// Create an object based on the specified function, \c f
    funct_gsl(func_t &f) {
      function=&funct_wrap;
      params=&f;
    }
    
  };
#endif

#ifndef DOXYGEN_NO_O2NS
}
#endif

#endif
