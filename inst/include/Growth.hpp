//  Growth.hpp
//  Age Structured Assessment Modeling System (ASAMS)

#ifndef growth_h
#define growth_h

#include "Common.hpp"
#include <map>
#include <vector>
/*
 * growth namespace
 */
namespace asams{

/**
 * Growth base class. Inherits from modelbase.
 */
template <class T>

class growth_base:public model_base<T>{
protected:

public:
  virtual T evaluate(const T& age)=0;
};

  
  
  template <class T>
/* Empirical growth class implemnents growth_base
 * 
 * This function can be used when there is no provided growth curve but catch and survey empirical weight at age is used.
 * 
 */
class empirical_growth: public growth_base<T>{

  std::vector<std::map<T, T> > growth;
public:
  std::map<T, T> survey_growth;
  std::map<T, T> catch_growth;
  
  enum GROWTH_TYPE{
    SURVEY=0,
    CATCH
  };
    
  
  empirical_growth(){
    growth = {survey_growth,catch_growth};
  }
  
   virtual T evaluate(const T& age, GROWTH_TYPE t) {
     return this->growth[t][age];
   }

};
/**
 *von Bertalanffy implementation. Inherits from growth_base.
 */
template <class T>

/// A class describing a von Bertalanffy growth function.
///
/// Creates a growth curve
class vonBertalanffy: public growth_base<T>{

public:
  T m_Linf;
  T m_K;
  T m_t0;

  /*
   *Default constructor
   */
  vonBertalanffy(){

  }

  /**
   *Three parameter von Bertalanffy growth function used to calculate length-at-age.
   *@param Linf the asymptotic maximum length (cm); this is defined as the mean length at maximum age.
   *@param K the instantaneous growth rate; usually bounded between 0 and 1
   *@param t0 the time (years) when the fish is at size 0; this can be negative
   */
  vonBertalanffy(T Linf, T K, T t0){
    this->m_Linf = Linf;
    this->m_K = K;
    this->m_t0 = t0;
  }

  /**
   * Destructor; this method deleted the von Bertalanffy growth object.
   */
  ~ vonBertalanffy(){
    std::cout <<"I just deleted vonBertalanffy object" << std::endl;
  }

  /**
   *von Bertalanffy implementation of the virtual evaluate function. Required to be overloaded.
   *@param age an age to evaluate length at
   *@return length at age age
   *
   */
  virtual T evaluate(const T& age) {
    return m_Linf * (1 - asams::exp(-m_K * (age - m_t0)));
  }

};

}

#endif /*growth_h */
