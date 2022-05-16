#ifndef BEST_INVERSION_HPP
#define BEST_INVERSION_HPP

#include "theory.hpp"
#include "cmath"

using namespace theory;


class Best_Inversion{
    public:
        Chord get_previous(){
            return this->previous;
        }

        void set_previous(Chord a){
            this->previous = a;
        }
        
        Note get_root(){
            return this->root;
        }

        void set_root(Note a){
          this->root = a;
        }

        int next_best_inversion(Chord next){
            int return_value = 0;
            double scalar = 0.5;
            float least_sum = -1;
            for(int i=0; i<3; i++){
                if(least_sum < 0){
                    least_sum = score_transition(next, previous) + displacement(next, root);
                    return_value = 0;
                }
                else{
                    if(least_sum > score_transition(next, previous) + scalar * displacement(next, root)){
                        return_value=i;
                    }
                }
            }
            next.invert(return_value);
            set_previous(next);
            return return_value;     
        }


    private:
        Chord previous = Chord("Cmaj", 3);
        Note root = Note("C4");

        float score_transition(Chord A, Chord B){
            float sum = 0.f;
            for(int i=0; i<3; i++){
                sum+= abs(A.notes[i].frequency() - B.notes[i].frequency()); 
            }
            return sum;
        }

        float displacement(Chord A, Note C){
            float sum = 0.f;
            for(int i=0; i<3; i++){
                sum+= abs(A.notes[i].frequency() - C.frequency()); 
            }
            return sum;
        }

};

#endif

