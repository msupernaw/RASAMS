//  Population.hpp
//  Age Structured Assessment Modeling System (ASAMS)

#ifndef Population_h
#define Population_h

#include "Common.hpp"
#include "Growth.hpp"
#include "Recruitment.hpp"
#include "Mortality.hpp"
//#include "Maturity.hpp"
#include "Survey.hpp"
#include "Fleet.hpp"

namespace asams{
  
 template <class T>
  class population:public model_base<T>{
    public:
    
    //derived quantities
    std::vector<T> initial_numbers_at_age;
    std::vector<T> initial_deviations;
    std::vector<T> recruitment;
    std::vector<T> numbers_at_age;
    std::vector<T> weight_at_age;
    std::vector<T> ssb;
    std::vector<T> abundance;
    std::vector<T> biomass;
    std::vector<T> z;
    std::vector<T> natural_mortality;
    std::vector<T> fishing_mortality;
    std::vector<T> maturity;
    std::vector<T> z_at_age;
    std::vector<T> spawner_per_recruit;
    std::vector<T> spawning_biomass;
    T SB0;
    
    //life history submodels
    int nyears;
    int nages;
    std::vector<T> ages;
    growth_base<T>* growth_model;
    recruitment_base<T>* recruitment_model;
    mortality_base<T>* natural_mortality_model;
    //maturity_base<T>* maturity_model;
    
    population(){
      
    }
    
      population(int nyears, int nages ){  // constructor
          this-> nyears = nyears;
          this-> nages = nages;
          initial_numbers_at_age.resize(nages);
          recruitment.resize(nyears);
          numbers_at_age.resize(nyears*nages);
          z_at_age.resize(nyears*nages);
          weight_at_age.resize(nyears*nages);
          ssb.resize(nyears);
          abundance.resize(nyears);
          biomass.resize(nyears);
          z.resize(nyears*nages);
          natural_mortality.resize(nyears*nages);
          fishing_mortality.resize(nyears*nages);
          initial_deviations.resize(nages);
          spawner_per_recruit.resize(nages);
          maturity.resize(nages);
          spawning_biomass.resize(nyears);
      }
    
    void prepare(){                                 // reset all values in containers
        std::fill(initial_numbers_at_age.begin(), initial_numbers_at_age.end(),0);
        
        for(int i = 0; i < nyears; i++){
            recruitment[i]=0;
            ssb[i]=0;
            abundance[i]=0;
            biomass[i]=0;
            z[i]=0;
            
            for(int j = 0; j < nages; j++){
                int index = i*nages+j;
                numbers_at_age[index]=0;
                weight_at_age[index]=0;
                z_at_age[index]=0;
                natural_mortality[index]=0;
            }
            
        }
    }
    
    void calculate_mortality(int year, std::vector<fleet<T>*>& fleets){
        for (int a = 0; a < this->ages.size(); a++) {   // a is index of age vector
            int index = year*nages+a;
            
            this->natural_mortality[index] = this->natural_mortality_model(year, a);
            
            for(int i = 0; i < fleets.size(); i++){
                fleet<T> * F = fleets[i];
                T f_mult = F->fishing_mortality[year];
                T selectivity = F->selectivity_model->evaluate(ages[a]);
                T f_at_age = selectivity * f_mult;
                z[index] += f_at_age;
                }
            z[index] += natural_mortality[index];
        }
    }
    
    void calculate_maturity(int year){
        
        for (int a = 0; a < this->ages.size(); a++) {
            int index = year*nages+a;
            this->maturity[index] = this->maturity_model(a);
        }
        
    }
      
    void calculate_initial_numbers_equilibrium() {

                  //note initial_numbers and initial_equilibrium_numbers vectors
                  //are both size 3*ages.size()

        this->initial_equilibrium_numbers[0] = this->recruitment_model->R0;

        int a;
               for (a = 1; a < this->ages.size(); a++) {
                   this->initial_equilibrium_numbers[a] = this->initial_equilibrium_numbers[a - 1] *
                           asams::exp(static_cast<T> (-1.0) * (this->M[a - 1]));
               }
        variable m = this->M[a - 1];
        variable sum;
        for (a = this->ages.size(); a < this->initial_equilibrium_numbers.size(); a++) {
            this->initial_equilibrium_numbers[a] = this->initial_equilibrium_numbers[a - 1] * asams::exp(static_cast<T> (-1.0) * (m));
            sum += this->initial_equilibrium_numbers[a];
        }
        
        this->initial_equilibrium_numbers[ this->ages.size() - 1] += sum +
                            (this->initial_equilibrium_numbers[this->initial_equilibrium_numbers.size() - 1] * asams::exp(static_cast<T> (-1.0) * (m))) /
                            (1.0 - asams::exp(static_cast<T> (-1.0) * (m)));
    }
    
    void calculate_initial_numbers_at_age(){
        int a;
        for (a = 0; a < this->ages.size(); a++) {
            this->initial_numbers_at_age[a] = (asams::exp(static_cast<T> (-1.0) *
                    this->M[a] - this->initialF) * this->initial_equilibrium_numbers[a] *
                    asams::exp(initial_deviations[a] - static_cast<T> (0.5)
                    /* asams::pow(sigma_r, static_cast<T> (2.0))*/));
       }
        
    }
    
    void calculate_numbers_at_age(int year){
        
        for (int a = 1; a < this->ages.size(); a++) {
            int index = year*nages+a;
            int index2 = year*nages+a-1;
            this->numbers_at_age[index] =
            this->numbers_at_age[index2] *
            asams::exp(static_cast<T> (-1.0) * z[index2]);
        }
        
        int index = year*nages+nages-1;
        int index2 = year*nages+nages;
        this->numbers_at_age[index] +=
        this->numbers_at_age[index2] *
        asams::exp(static_cast<T> (-1.0) * z[index2]);
        
    }
    
    void calculate_growth(int year){
        for(int i = 0; i < nages; i++){
            int index = year*nages+i;
            weight_at_age[index]=this->growth_model->evaluate(ages[i]);
        }
    }
    
    void calculate_spawning_stock_biomass(int year){
        
        T SB = 0;
        
        for(int i = 0; i < nages; i++){
            int index = year*nages+i;
            ssb[index]=this->growth_model->evaluate(ages[i]);
            SB+=ssb[index];
        }
        
        spawning_biomass[year] = SB;
    }
    
    void calculate_recruitment(int year){
        this->recruitment[year] =
        this->recruitment_model->evaluate(SB0, spawning_biomass[year]);
    }
    
    void calculate_catch_at_age(int year, std::vector<fleet<T>*>& fleets){
        for(int i = 0; i < fleets.size(); i++){
            int index = year*nages+i;
            for(int a = 0; a < ages; a++){
                fleets[i]->catch_at_age[index] = (numbers_at_age[index] * (fleets[i]->f_at_age[index]
                                           / z[index]) * (1.0 - asams::exp(static_cast<T> (-1.0) * z[index])));
            }
        }
    }
    
    void calculate_survey_numbers_at_age(int year, std::vector<survey<T>*>& surveys){
        for(int i = 0; i < surveys.size(); i++){
            int index = year*nages+i;
            for(int a = 0; a < ages; a++){
                surveys[i]->survey_at_age[index] = (surveys[i]->q) *
                this->numbers_at_age[index] * surveys[i]-> selectivity_model-> evaluate(ages[a]) *
                asams::exp(static_cast<T> (-1.0) * this->z[index]);
            }
        }
    }
    
  };
}


#endif /* Population_h */
