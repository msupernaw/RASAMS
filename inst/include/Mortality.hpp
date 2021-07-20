//  Mortality.hpp
//  Age Structured Assessment Modeling System (ASAMS)

#ifndef Mortality_h
#define Mortality_h

#include "Common.hpp"

#include <iostream>
#include <vector>
#include <map>

namespace asams{
  
template <class T>
class mortality_base:public model_base<T>{
protected:
  typedef typename std::vector<std::vector<T> > matrix;
  typedef typename std::vector<T> vector;
  matrix mortality_m;
    
public:
  
    /*
    * Mortality base class
    * @param rows the number of rows (years) in the mortality matrix 
    * @param cols the number of columns (ages) in the mortality matrix
    */
    mortality_base(int rows = 0, int cols= 0){
        this->mortality_m = matrix(rows, vector(cols));
      }
     
    /*
    * Operator to extract a single mortality value from the mortality matrix.
    *@param i the row index (year index) of the mortality value to extract
    *@param j the column index (age index) of the mortality value to extract
    */
    inline T& operator()(size_t i, size_t j) {    //overloaded operator
      return this->mortality_m[i][j];
    }

};
  
}


#endif /* Mortality_h */
