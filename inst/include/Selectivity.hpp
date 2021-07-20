//  Selectivity.hpp
//  Age Structured Assessment Modeling System (ASAMS)

#ifndef Selectivity_h
#define Selectivity_h

#include "Common.hpp"

namespace asams{

/**
 *Selectivity base class. Inherits from modelbase.
 */

template <class T>
class selectivity_base:public model_base<T>{
protected:
    
public:
    virtual T evaluate(const T& age)=0;
};

/**
 *Logistic selectivity implementation. Inherits from selectivity base.
 */

template <class T>
class logistic_selectivity:public selectivity_base<T>{
    
public:
    T a50_m; // m denotes member of class
    T slope_m;
    
    logistic_selectivity(){
    
    }
    
    /**
     *Constructor that takes arguments
     *@param a50 Age at which selectivity = 50%
     *@param slope Slope
     */
    
    logistic_selectivity(T a50, T slope){  // constructor
        this->a50_m=a50;
        this->slope_m=slope;
    }
    
    /**
     *Deconstructor
     */
    
    ~ logistic_selectivity(){    //destructor
        std::cout <<"I just deleted"<<std::endl;
    }
    
    /**
     *@brief Logistic selectivity implementation of the virtual evaluate function. Required to be overloaded.
     *
     *Logistic selectivity implementation \f$ \frac{1.0}{1.0+exp(-1.0*\frac{age-a50}{slope}}\f$
     *@param age Age
     */
    
    virtual T evaluate(const T& age){
        return asams::logistic(a50_m, slope_m, age);
    }
    
};

/**
 *Double logistic selectivity implementation. Inherits from selectivity base.
 */

template <class T>
class doublelogistic_selectivity:public selectivity_base<T>{

public:
    T alpha_asc_m;
    T beta_asc_m;
    T alpha_desc_m;
    T beta_desc_m;
    
    doublelogistic_selectivity(){       // default constructor
        
    }
    
    /**
     *Constructor that takes arguments
     *@param alpha_asc \f$ \alpha_1\f$ Inflection point of the first logistic curve
     *@param beta_asc \f$ \beta_1\f$ Slope of the first logistic curve
     *@param alpha_desc \f$ \alpha_2\f$ Inflection point of second logistic curve
     *@param beta_desc \f$ \beta_2\f$ Slope of the second logistic curve
     */
    
    doublelogistic_selectivity(T alpha_asc, T beta_asc,T alpha_desc,T beta_desc){
            this->alpha_asc_m=alpha_asc;
            this->beta_asc_m=beta_asc;
            this->alpha_desc_m=alpha_desc;
            this->beta_desc_m=beta_desc;
        }
    
    /**
     *Deconstructor
     */
    
    ~doublelogistic_selectivity(){
        std::cout <<"I just deleted"<< std::endl;
    }
    
    /**
     *@brief Double logistic selectivity implementation of the virtual evaluate function. Required to be overloaded.
     *
     *Double logistic selectivity implementation\f$ \frac{1.0}{1.0+exp(-\beta_1*(age-\alpha_1)}*1-\frac{1}{1+exp(-\beta_2*(age-\alpha_2)}\f$
     *@param age Age
     */
    
        virtual T evaluate(const T& age){
            return (1.0/ (1.0+asams::exp(-beta_asc_m * (age - alpha_asc_m)))) * (1.0- (1.0/ (1.0+asams::exp(-beta_desc_m * (age - alpha_desc_m)))));
        }
};
}

#endif /* Selectivity_h */

