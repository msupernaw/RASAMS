//  Recruitment.hpp
//  Age Structured Assessment Modeling System (ASAMS)

#ifndef Recruitment_h
#define Recruitment_h

namespace asams{

/**
 *Recruitment base class. Inherits from modelbase.
 */

template <class T>
class recruitment_base:public model_base<T>{
protected:  //when class inherits from recruitment, can see
    T R0_m;
    T h_m;
    
public:
    virtual T evaluate(const T& SB0, const T& SB)=0;
};

/**
 *Beverton-Holt implementation. Inherits from recruitment base.
 */

template <class T>
class beverton_holt:public recruitment_base<T>{
    
public:
    
    beverton_holt(){
        
    }
    /**
     *Constructor that takes arguments
     *@param R0 \f$ R_0\f$ Recruitment at t=0
     *@param h Steepness
     */
    
    beverton_holt(T R0, T h){  // constructor
        this->R0_m=R0;
        this->h_m=h;
    }
    
    /**
     *Deconstructor
     */
    
    ~ beverton_holt(){    //destructor
        std::cout <<"I just deleted"<<std::endl;
    }
    
    /**
     *@brief Beverton-Holt implementation of the virtual evaluate function. Required to be overloaded.
     *
     *The Beverton-Holt stock-recruit implementation \f$ \frac{0.8*R_0*h*SB}{0.2*SB_0*(1-h) + SB*(h-0.2)}\f$
     *@param SB0 \f$ SB_0\f$ Spawning stock biomass at t=0
     *@param SB Spawning stock biomass
     */
    
    virtual T evaluate(const T& SB0, const T& SB){
        return (0.8*this->R0_m*this->h_m*SB)/(0.2*SB0*(1-this->h_m) + SB*(this->h_m-0.2));
    }
    
};

/**
 *Ricker implementation. Inherits from recruitment base.
 */

template <class T>
class ricker:public recruitment_base<T>{
    
public:
    
    ricker(){
        
    }
    
    /**
     *Constructor that takes arguments
     *@param R0 \f$ R_0\f$ Recruitment at t=0
     *@param h Steepness
     */
    
    ricker(T R0, T h){  // constructor
        this->R0_m=R0;
        this->h_m=h;
    }
    
    /**
     *Deconstructor
     */
    
    ~ ricker(){    //destructor
        std::cout <<"I just deleted"<<std::endl;
    }
    
    /**
     *@brief Ricker implementation of the virtual evaluate function. Required to be overloaded.
     *
     *The Ricker stock-recruit implementation \f$ \frac{SB}{\frac{SB_0}{R_0}}*exp(h*\frac{1-SB}{R_0*\frac{SB_0}{R_0}})\f$
     *@param SB0 \f$ SB_0\f$ Spawning stock biomass at t=0
     *@param SB Spawning stock biomass
     */
    
    virtual T evaluate(const T& SB0, const T& SB){
        return SB/(SB0/this->R0_m)*exp(this->h_m*(1-SB/(this->R0_m*(SB0/this->R0_m))));
    }
    
};

}

#endif /* Recruitment_h */
