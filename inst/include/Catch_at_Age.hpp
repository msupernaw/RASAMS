//  Catch_at_Age.hpp
//  Age Structured Assessment Modeling System (ASAMS)

#ifndef Catch_at_Age_h
#define Catch_at_Age_h

#include "Survey.hpp"
#include "Fleet.hpp"
#include "Population.hpp"
#include <vector>
#include <map>
#include <set>


namespace asams{
template <class T>
class catch_at_age:public model_base<T>{
public:
    // vectors
    std::set<fleet<T>*> fleet_list;
    std::set<survey<T>*> survey_list;
    std::map<int, std::vector<fleet<T>*> > fleets_m;
    std::map<int, std::vector<survey<T>*> > surveys_m;
    population<T>* population_m;
    int nyears;
    int nages;
    
    void compute_derived_quantities() {
        population_m->prepare();
        
        typename std::set <fleet<T>*>::iterator fleet_iterator;
        for(fleet_iterator = fleet_list.begin();
            fleet_iterator != fleet_list.end(); ++fleet_iterator){
            fleet_iterator->prepare();
        }
        
        typename std::set <survey<T>*>::iterator survey_iterator;
        for(survey_iterator = survey_list.begin();
            survey_iterator != survey_list.end(); ++survey_iterator){
            survey_iterator->prepare();
        }
            
        population_m->calculate_initial_numbers_at_age();
        for(int y=0;y<nyears;y++){
            population_m->calculate_growth();
            population_m->calculate_maturity(y);
            population_m->calculate_mortality(y, fleets_m[y]);
            population_m->calculate_numbers_at_age(y);
            population_m->calculate_spawning_stock_biomass(y);
            population_m->calculate_recruitment(y);
            population_m->calculate_catch_at_age(y,fleets_m[y]);
            population_m->calculate_survey_numbers_at_age(y,surveys_m[y]);

        }
    }
    
    void calculate_catch_at_age(int year){
        std::vector<fleet<T>*>& fleets = this->fleets_m[year];
        for(int i = 0; i < fleets.size(); i++){
            fleet<T> * F = fleets[i];
            
            for(int j = 0; j < nages; j++){
                
                
            }
        }
    }
    
    void calculate_survey_numbers_at_age(int year, std::vector<survey<T>*>& surveys){
    }
    
    T likelihood(){                // objective function
        T total_likelihood;
        this->compute_derived_quantities();
        
        typename std::set <fleet<T>*>::iterator fleet_iterator;
        for(fleet_iterator = fleet_list.begin();
            fleet_iterator != fleet_list.end(); ++fleet_iterator){
            total_likelihood += fleet_iterator->likelihood();
        }
        
        typename std::set <survey<T>*>::iterator survey_iterator;
        for(survey_iterator = survey_list.begin();
            survey_iterator != survey_list.end(); ++survey_iterator){
            total_likelihood += survey_iterator->likelihood();
        }
        
        return total_likelihood;
    }
};
}


#endif /* Catch_at_Age_h */
