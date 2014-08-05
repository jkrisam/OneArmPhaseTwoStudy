#ifndef RESULT_H
#define RESULT_H

#include <Rcpp.h>
#include <string>
#include <map>
#include <vector>
//Data-class containing all information regarding one simon's two-stage design.
//Also all information regarding (non)-stochastic curtailment can be stored.
class Result
{
public:
    // more than "ri" responses under "n" enrolled patients are needed to proceed with the study
    struct StoppingRule  
    {
        int ri; 
        int n; 
        double pet;
    };

    struct Curtailment
    {
        float cut;  // chosen cut
        float en_sc; //expected sample size (with choosen cut)
        float pet_sc; //probability of early termination (with choosen cut)
        float type1_errorRate;
        float type2_errorRate;
        // variables for the 95%-confidence intervall
        float en_lower;
        float en_upper;
        float pet_lower;
        float pet_upper;
        float alpha_lower;
        float alpha_upper;
        float beta_lower;
        float beta_upper;
        std::vector<float*> *stoppingRulesNSC;
    };

    // Default constructor
    Result();
    // constructor
    // n: number of patients enrolled in the whole trial.
    // r: critical value for the whole trial (more than "r" responses needed at the end of the study to reject the null hypothesis).    
    // n1: number of patients enrolled in the first stage.    
    // r1: critical value for the first stage (more than "r1" responses needed to proceed to the second stage).    
    // alpha: type I error rate
    // beta: type II error rate
    // petP1: probability of early termination under the alternativ hypothesis
    // petP0: probability of early termination under the null hypothesis
    // enP1: expected samplesize under the alternative hypothesis
    // enP0: expected samplesize under the null hypothesis
    // iD: ID of the design
    // p0: response probability under the null hypothesis
    // p1: response probability under the alternativ hypothesis
    Result(int n, int r, int n1, int r1, double alpha, double beta, double petP1, double petP0, double enP1, double enP0, int iD, double p0, double p1);
    ~Result();
    // Sets the start and stop value for wich the weight q minimizes the rist p = q * n + (1-q)* enP0 
    void setAdmissible(double start, double stop);
    // Sets the start and stop value for wich the weight q minimizes the rist p = q * n + (1-q)* enP0
    // And sets a name for the admissible design (such as "minimax", "optimal" or "admissible" should be used)
    void setAdmissible(double start, double stop, std::string name);
    // Adds a curtailment result to the map "curtailmentResults"
    void addCurtailmentResult(Curtailment curResult);
    void setUseCurtailment(bool useCurtailment);
    void setCut(int cut);

    SEXP get_R_Representation();

    bool getAdmissible();
    int getN();
    int getR();
    int getN1();
    int getR1();
    double getEnP0();
    double getEnP1();
    double getPetP0();
    std::map<int, Curtailment>* getCurtailmentResults();

private:    
    int iD;
    int n, r, n1, r1;
    double alpha, beta, petP1, petP0, enP1, enP0, admissibleStart, admissibleStop, p0, p1;
    bool admissible;
    std::string *name;
    std::vector<StoppingRule> *stoppingRulesNSC;
    std::map<int, Curtailment> *curtailmentResults;

    // Should (non-)stochastic curtailment be used?
    bool useCurtailment;
    // Specifies the threshold in percent if (non-)stochastic curtailment is used.
    int cut;
};

#endif // RESULT_H
