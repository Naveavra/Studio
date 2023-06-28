#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>
#include "Customer.h"
#include "vector"

enum ActionStatus {
    COMPLETED, ERROR
};

//Forward declaration
class Studio;

class BaseAction {
public:
    virtual  ~BaseAction()=default;
	BaseAction();
    ActionStatus getStatus() const;
    virtual void act(Studio& studio) = 0;
    virtual std::string toString() const = 0;
    std::string getErrorMsg() const;
    std::string args ;
    virtual BaseAction* clone() = 0;
    void setStatus(ActionStatus status);
protected:
    void complete();
    void error(std::string errorMsg);
private:
    std::string errorMsg;
    ActionStatus status;
};


class OpenTrainer : public BaseAction {
public:
    OpenTrainer(int id, std::vector<Customer*>& customersList);
    void act(Studio& studio);
    virtual ~OpenTrainer();
    std::string toString() const;
    OpenTrainer* clone();
    OpenTrainer(const OpenTrainer& other);
private:
    const int trainerId;
    std::vector<Customer*> customers;
};


class Order : public BaseAction {
public:
    Order(int id);
    void act(Studio& studio);
    std::string toString() const;
    Order* clone();
    Order(const Order& other);
private:
    const int trainerId;
};


class MoveCustomer : public BaseAction {
public:
    MoveCustomer(int src, int dst, int customerId);
    void act(Studio& studio);
    std::string toString() const;
    MoveCustomer* clone();
    MoveCustomer(const MoveCustomer& other);
private:
    const int srcTrainer;
    const int dstTrainer;
    const int id;
};


class Close : public BaseAction {
public:
    Close(int id);
    void act(Studio& studio);
    std::string toString() const;
    Close* clone();
    Close(const Close& other);
private:
    const int trainerId;
};


class CloseAll : public BaseAction {
public:
    CloseAll();
    void act(Studio& studio);
    std::string toString() const;
    CloseAll* clone();
    CloseAll(const CloseAll& other);
private:
};


class PrintWorkoutOptions : public BaseAction {
public:
    PrintWorkoutOptions();
    void act(Studio& studio);
    std::string toString() const;
    PrintWorkoutOptions* clone();
    PrintWorkoutOptions(const PrintWorkoutOptions& other);
private:
};


class PrintTrainerStatus : public BaseAction {
public:
    PrintTrainerStatus(int id);
    void act(Studio& studio);
    std::string toString() const;
    PrintTrainerStatus* clone();
    PrintTrainerStatus(const PrintTrainerStatus& other);
private:
    const int trainerId;
};


class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog();
    void act(Studio& studio);
    std::string toString() const;
    PrintActionsLog* clone();
    PrintActionsLog(const PrintActionsLog& other);
private:
};


class BackupStudio : public BaseAction {
public:
    BackupStudio();
    void act(Studio& studio);
    std::string toString() const;
    BackupStudio* clone();
    BackupStudio(const BackupStudio& other);
private:
};


class RestoreStudio : public BaseAction {
public:
    RestoreStudio();
    void act(Studio& studio);
    std::string toString() const;
    RestoreStudio* clone();
    RestoreStudio(const RestoreStudio& other);
};


#endif