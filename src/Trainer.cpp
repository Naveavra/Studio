#include "Trainer.h"
#include <vector>
#include <iostream>
#include "Action.h"


Trainer::Trainer(int t_capacity):capacity(t_capacity),open(false), salary(0) {
	this->customersList = std::vector<Customer*>();
    this->orderList = std::vector<OrderPair>();
    // damn boi you look fineeee as hell

}
Trainer::~Trainer()
{
    for (int i=0; i< static_cast<int>(customersList.size()); i++)
    {
        delete(this->customersList[i]);
    }

}
int Trainer::getCapacity() const {
    return capacity;
}
void Trainer::addCustomer(Customer* customer) {
    if (!isFull())
    {
        customersList.push_back(customer);
        salary = getSalary();
        
    }
}
void Trainer::removeCustomer(int id) {
    int idx = 0;
	for (Customer* cust:customersList)
    {
        if (cust->getId() == id)
        {
            customersList.erase(customersList.begin() + idx);
        }
        idx++;

    }
    std::vector<OrderPair> temp;
    for(OrderPair pair:orderList)
    {
	    if(pair.first!=id)
	    {
            temp.push_back(pair);
	    }
    }
    orderList.clear();

    for(OrderPair pair:temp)
    {
        orderList.push_back(pair);
    }
    temp.clear();
}
Customer* Trainer::getCustomer(int id)
{
    for (Customer * cust: customersList )
    {
	    if (cust->getId() == id)
	    {
            return cust;
	    }
    }
    return nullptr;
}
std::vector<Customer*>& Trainer::getCustomers()
{
    return customersList;
}
std::vector<OrderPair>& Trainer::getOrders()
{
    return orderList;
}
void Trainer::order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options)
{
    bool same_same=false;
    for (int i = 0; i < static_cast<int>(workout_ids.size()); i++) {
        OrderPair temp = std::make_pair(customer_id, workout_options[workout_ids[i]]);
        for(OrderPair op: this->getOrders())
        {
            if (op.first == temp.first && op.second.getId() == temp.second.getId())
	        {
                same_same = true;
	        }
        }
        if(!same_same)
        {
            orderList.push_back(std::make_pair(customer_id, workout_options[workout_ids[i]]));
        }
    	
    }
}
void Trainer::openTrainer()
{
    this->open = true;
}
void Trainer::closeTrainer()
{
    this->open = false;
}

Trainer& Trainer::operator=(const Trainer& other)
{
    if (this != &other)
    {
        this->salary = other.salary;
        this->open = other.open;
        this->capacity = other.capacity;
        this->customersList.clear();
        this->orderList.clear();
        for (Customer* cust : other.customersList)
        {
            this->customersList.push_back(cust->clone());
        }
        for (std::pair<int, Workout> pair : other.orderList)
        {
            orderList.push_back(std::make_pair(pair.first, pair.second));
        }
    }
    return *this; 
}
Trainer::Trainer(const Trainer& other): open(other.open), salary(other.salary)
{
    this->capacity = other.capacity;
    this->customersList = std::vector<Customer*>();
    this->orderList = std::vector<OrderPair>();
    for(Customer* cust: other.customersList)
    {
        customersList.push_back(cust->clone());
    }
    for(std::pair<int,Workout> pair:other.orderList)
    {
        orderList.push_back(std::make_pair(pair.first, pair.second));
    }
}
Trainer::Trainer(Trainer&& other) noexcept :
    capacity(other.capacity), open(other.open)
{
    this->customersList = std::vector<Customer*>();
    this->orderList = std::vector<OrderPair>();
    for (Customer* cust : other.customersList)
    {
        customersList.push_back(cust->clone());
    }
    for (std::pair<int, Workout> pair : other.orderList)
    {
        orderList.push_back(std::make_pair(pair.first, pair.second));
    }
    delete &other;
}

Trainer& Trainer::operator=(Trainer&& other) noexcept
{
    if (this != &other)
    {
        this->salary = other.salary;
        this->open = other.open;
        this->capacity = other.capacity;
        this->customersList.clear();
        this->orderList.clear();
        for (Customer* cust : other.customersList)
        {
            this->customersList.push_back(cust->clone());
        }
        for (std::pair<int, Workout> pair : other.orderList)
        {
            orderList.push_back(std::make_pair(pair.first, pair.second));
        }
    }
    delete& other;
	return *this;
}

int Trainer::getSalary()
    {
        salary = 0;
        for (int i = 0; i < static_cast<int>(orderList.size()); i++)
        {
            salary += orderList[i].second.getPrice();
        }
        return salary;
    }
bool Trainer::isOpen()
{
    return this->open;
}
bool Trainer::isFull()
{
    return static_cast<int>(customersList.size()) >= capacity;
}


