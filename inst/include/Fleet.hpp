//  Fleet.hpp
//  Age Structured Assessment Modeling System (ASAMS)

#ifndef Fleet_h
#define Fleet_h


#include "Common.hpp"
#include "Selectivity.hpp"
#include "Data.hpp"
#include "Mortality.hpp"
#include "likelihood.hpp"

/**
 * Age Structured Assessment Modeling System (asams) namespace
 */
namespace asams{


/**
 * fleet class. Inherits from model_base.
 */
template<class T>
 class fleet: public model_base<T>{
  public:
    index_data<T>* observed_index_data;
    agecomp_data<T>* observed_agecomp_data;
     
     likelihood_base<T>* index_likelihood;
     likelihood_base<T>* agecomp_likelihood;
    
    /// Derived quantity: landings over years
    std::vector<T> landings; 
    /// Derived quantity: catch at age over years
    std::vector<T> catch_at_age; 
    /// Derived quantity: total mortality at age over years
    std::vector<T> z_at_age; 
    /// Derived quantity: fleet index over years 
    std::vector<T> derived_index_data; 
    /// Derived quantity: fleet age composition over years
    std::vector<T> derived_agecomp_data; 
    std::vector<T> f_at_age; 
    /// Derived quantity: fishing mortality over years
    std::vector<T> fishing_mortality;
    
    /// Operator
    selectivity_base<T>* selectivity_model; 
    
    int nyears;
    int nages;
     
     /**
      * Constructor 
      * @param nyears the number of years 
      * @param nages the number of ages 
      */
     fleet(int nyears, int nages ){  
         this-> nyears = nyears;
         this-> nages = nages;
         
         // Resize containers
         landings.resize(nyears);
         catch_at_age.resize(nyears*nages);
         z_at_age.resize(nyears*nages);
         derived_index_data.resize(nyears);
         derived_agecomp_data.resize(nyears*nages);
         fishing_mortality.resize(nyears);
     }
     
     /// Reset all values in containers
     void prepare(){                                 
         for(int i = 0; i < nyears; i++){
             landings[i]=0;
             fishing_mortality[i]=0;
             derived_index_data[i]=0;
             
             for(int j = 0; j < nages; j++){
                 int index = i*nages+j;
                 catch_at_age[index]=0;
                 derived_agecomp_data[index]=0;
                 z_at_age[index]=0;
             }
             
         }
     }
     
     T likelihood(){
         return this->index_likelihood->evaluate(observed_index_data->data_m, this->derived_index_data)
         + this->agecomp_likelihood->evaluate(observed_agecomp_data->data_m,this->derived_agecomp_data, observed_agecomp_data->sample_size_m, nyears, nages);
         // plan to revisit this -- 'observed'
     }
    
 };

}

#endif /* Fleet_h */

