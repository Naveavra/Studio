#include "Customer.h"
#include <algorithm>
#include <limits>

using namespace std;

Customer::Customer(std::string c_name, int c_id): name(c_name), id(c_id)
{
}

Customer::Customer(Customer&& other):name(other.getName()),id(other.getId())
{
	delete &other;
}

std::string Customer::getName() const
{
	return std::string(name);
}

int Customer::getId() const
{
	return this->id;
}

SweatyCustomer::SweatyCustomer(std::string name, int id): Customer(name, id)
{
	this->type = "swt";
}

std::vector<int> SweatyCustomer::order(const std::vector<Workout>& workout_options)
{
	std::vector<int> res = std:: vector<int>();
	for (int i = 0; i< static_cast<int>(workout_options.size()); i++) {
		if (workout_options[i].getType() == CARDIO)
		{
			res.push_back(workout_options[i].getId());
		}
	}
	return  res;
}

std::string SweatyCustomer::toString() const
{
	std::string res;
	res += this->getName() + ",swt";
	return res;
}

SweatyCustomer* SweatyCustomer::clone()
{
	return (new SweatyCustomer(*this));
}

SweatyCustomer::SweatyCustomer(const SweatyCustomer& other): Customer(other.getName(), other.getId())
{
	this->type = other.type;
	//delete &other;
}

CheapCustomer::CheapCustomer(std::string name, int id): Customer(name, id)
{
	this->type = "chp";
}

std::vector<int> CheapCustomer::order(const std::vector<Workout>& workout_options)
{
	std::vector<int> res = std::vector<int>();
	res.push_back(workout_options[0].getType());
	int cheapest = workout_options[0].getPrice();
	int id_cheapest = workout_options[0].getId();
	for (int i = 0; i < static_cast<int>(workout_options.size()); i++) {
		if (workout_options[i].getPrice() < cheapest)
		{
			cheapest = workout_options[i].getPrice();
			id_cheapest = workout_options[i].getId();
			res.clear();
			res.push_back(workout_options[i].getId());
		}
		else if (workout_options[i].getPrice() == cheapest && workout_options[i].getId() < id_cheapest)
		{
			cheapest = workout_options[i].getPrice();
			id_cheapest = workout_options[i].getId();
			res.clear();
			res.push_back(workout_options[i].getId());
		}
		
	}
	return res;
}
std::string CheapCustomer::toString() const
{
	std::string res;
	res += this->getName() + ",chp";
	return res;
}

CheapCustomer* CheapCustomer::clone()
{
	return new CheapCustomer(*this);
}

CheapCustomer::CheapCustomer(const CheapCustomer& other): Customer(other.getName(), other.getId())
{
	this->type = other.type;
	//delete& other;
}

HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id): Customer(name, id)
{
	this->type = "mcl";
}

std::vector<int> HeavyMuscleCustomer::order(const std::vector<Workout>& workout_options)
{
	std::vector<int> res = std::vector<int>();
	std::vector<Workout> copy = std::vector<Workout>();
	for (int i = 0; i < static_cast<int>(workout_options.size()); i++)
	{
		copy.push_back(Workout(workout_options[i]));
	}
	for (int i = 0; i < static_cast<int>(copy.size()); i++) {

		// Last i elements are already in place
		for (int j = 0; j < static_cast<int>(copy.size() - 1); j++) {
			if (copy[j].getPrice() > static_cast<long>(copy[j + 1].getPrice())) {
				iter_swap(copy.begin() + i, copy.begin() + j);
			}
		}
	}
	for(int i = (int)copy.size() - 1; i >= 0; i--)
	{
		if (copy[i].getType() == ANAEROBIC)
		{
			res.push_back(copy[i].getId());
		}
	}
	return res;
}



std::string HeavyMuscleCustomer::toString() const
{
	std::string res;
	res += this->getName() + ",mcl";
	return res;
}

HeavyMuscleCustomer* HeavyMuscleCustomer::clone()
{
	return new HeavyMuscleCustomer(*this);
}

HeavyMuscleCustomer::HeavyMuscleCustomer(const HeavyMuscleCustomer& other): Customer(other.getName(), other.getId())
{
	this->type = other.type;
	//delete& other;
}

FullBodyCustomer::FullBodyCustomer(std::string name, int id): Customer(name, id)
{
	this->type = "fbd";
}

std::vector<int> FullBodyCustomer::order(const std::vector<Workout>& workout_options)
{
	int cheapest_cardio_id;
	int cheapest_cardio_price = numeric_limits<int>::max();
	int expensive_mixed_id;
	int expensive_mixed_price = numeric_limits<int>::min();
	int cheapest_anaerobic_id;
	int cheapest_anaerobic_price = numeric_limits<int>::max();
	for (Workout workout : workout_options)
	{
		if (workout.getType() == CARDIO)
		{
			if (workout.getPrice() < cheapest_cardio_price)
			{
				cheapest_cardio_price = workout.getPrice();
				cheapest_cardio_id = workout.getId();
			}
			else if (workout.getPrice() == cheapest_cardio_price && workout.getId() < cheapest_cardio_id)
			{
				cheapest_cardio_id = workout.getId();
			}
		}
		if (workout.getType() == MIXED)
		{
			if (workout.getPrice() > expensive_mixed_price)
			{
				expensive_mixed_price = workout.getPrice();
				expensive_mixed_id = workout.getId();
			}
			else if (workout.getPrice() == expensive_mixed_price && workout.getId() < expensive_mixed_id)
			{
				expensive_mixed_id = workout.getId();
			}
		}
		if (workout.getType() == ANAEROBIC)
		{
			if (workout.getPrice() < cheapest_anaerobic_price)
			{
				cheapest_anaerobic_price = workout.getPrice();
				cheapest_anaerobic_id = workout.getId();
			}
			else if (workout.getPrice() == cheapest_anaerobic_price && workout.getId() < cheapest_anaerobic_id)
			{
				cheapest_cardio_id = workout.getId();
			}
		}
	}
	vector<int> orders = vector<int>();
	if (cheapest_cardio_price != numeric_limits<int>::max())
		orders.push_back(cheapest_cardio_id);
	if (expensive_mixed_price != numeric_limits<int>::min())
		orders.push_back(expensive_mixed_id);
	if (cheapest_anaerobic_price != numeric_limits<int>::max())
		orders.push_back(cheapest_anaerobic_id);
	return orders;
}


std::string FullBodyCustomer::toString() const
{
	std::string res;
	res += this->getName() + ",fbd";
	return res;
}
FullBodyCustomer* FullBodyCustomer::clone()
{
	return new FullBodyCustomer(*this);
}
FullBodyCustomer::FullBodyCustomer(const FullBodyCustomer& other): Customer(other.getName(), other.getId())
{
	this->type = other.type;
	//delete& other;
}
