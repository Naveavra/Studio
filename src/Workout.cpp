#include "Workout.h"

	

Workout::Workout(int w_id, std::string w_name, int w_price, WorkoutType w_type) : id(w_id), name(w_name),price(w_price),type(w_type){}
	
	

	int Workout::getId() const
	{
		return id;
	}

	std::string Workout::getName() const
	{
		return name;
	}

	int Workout::getPrice() const
	{
		return price;
	}

	Workout::~Workout() = default;
	

	Workout::Workout(const Workout & other) : id(other.getId()), name(other.getName()), price(other.getPrice()), type(other.getType())
	{

	}

	WorkoutType Workout::getType() const
	{
		return type;
	}

Workout& Workout::operator=(const Workout& other)
{
	return *this;
}
