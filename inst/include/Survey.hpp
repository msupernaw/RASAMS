//  Survey.hpp
//  Age Structured Assessment Modeling System (ASAMS)

#ifndef Survey_h
#define Survey_h

#include "Common.hpp"
#include "Data.hpp"
#include "Selectivity.hpp"
#include "Data.hpp"

/**
 * Age Structured Assessment Modeling System (asams) namespace
 */

namespace asams{

/**
 * survey class. Inherits from model_base.
 */
template<class T>
 class survey: public model_base<T>{
  public:
    index_data<T>* observed_index_data;
    agecomp_data<T>* observed_agecomp_data;
    
    std::vector<T> survey_at_age;
    /// Derived quantity: survey index over years
    std::vector<T> derived_index_data;
    /// Derived quantity: survey age composition over years
    std::vector<T> derived_agecomp_data;
    
    /// Operators
    selectivity_base<T>* selectivity_model;
    
    T q;
    int nyears;
    int nages;
     
     /**
      * Constructor 
      * @param nyears the number of years 
      * @param nages the number of ages 
      */
     survey(int nyears, int nages ){  // constructor
         this-> nyears = nyears;
         this-> nages = nages;
         
         // Resize containers
         survey_at_age.resize(nyears*nages);
         derived_index_data.resize(nyears);
         derived_agecomp_data.resize(nyears*nages);
     }
     
     /// Reset all values in containers
     void prepare(){                                 
         
         for(int i = 0; i < nyears; i++){
             derived_index_data[i]=0;
             
             for(int j = 0; j < nages; j++){
                 int index = i*nages+j;
                 survey_at_age[index]=0;
                 derived_agecomp_data[index]=0;
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


#endif /* Survey_h */
