#ifndef STUDIO_H_
#define STUDIO_H_

#include <vector>
#include <string>
#include "Workout.h"
#include "Trainer.h"
#include "Action.h"

#include  <iostream>

class Studio {
public:
    Studio();
    ~Studio();
    Studio& operator=(const Studio& studio);
    Studio& operator=( Studio&& studio) noexcept;
    Studio(const  Studio& studio);
    Studio(Studio&& studio);
    Studio(const std::string& configFilePath);
    void start();
    int getNumOfTrainers() const;
    Trainer* getTrainer(int tid);
    const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Trainer*>& getTrainers();
    void set_open();
    std::vector<Workout>& getWorkoutOptions();
    void addAction(BaseAction* action);
    Customer* create_customers(std::string cust,int id);
    void create_trainers(int num, std::string capacities);
    void create_workouts(std::string line);
    void clean();
    void copy(const Studio&studio);
    
private:
    bool open;
    int cust_id;
    std::vector<Trainer*> trainers;
    std::vector<Workout> workout_options;
    std::vector<BaseAction*> actionsLog;
};

#endif
