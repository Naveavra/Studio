#include "Studio.h"
#include <fstream>
#include <sstream>
	
Studio::Studio()
{}

Studio::~Studio()
{
	clean();
}

Studio& Studio::operator=(const Studio& studio)
{

	if (this != &studio) {
		this->clean();
		this->copy(studio);
	}
	return *this;
}

Studio& Studio::operator=(Studio&& studio) noexcept
{
	if (this != &studio) {
		this->copy(studio);
		delete& studio;
	}
	return *this;
}

Studio::Studio(const Studio& studio): open(studio.open)
{
	this->trainers =  std::vector<Trainer*>() ;
	this->actionsLog = std::vector<BaseAction*>();
	this->workout_options = std::vector<Workout>();
	this->cust_id = studio.cust_id;
	this->copy(studio);
}

Studio::Studio(Studio&& studio)
{
	this->trainers = std::vector<Trainer*>();
	this->actionsLog = std::vector<BaseAction*>();
	this->workout_options = std::vector<Workout>();
	this->cust_id = studio.cust_id;
	this->copy(studio);
	delete& studio;
}


Studio::Studio(const std::string& configFilePath)
{
	this->trainers = std::vector<Trainer*>();
	this->actionsLog = std::vector<BaseAction*>();
	this->workout_options = std::vector<Workout>();
	this->open = true;
	this->cust_id = 0;
	// File Parse
	std::ifstream file(configFilePath);
	std::string fileLine;
	int num_trainers = 0;
	std::vector<std::string> myLines=std::vector<std::string>();
	std::vector<std::string> work_options= std::vector<std::string>();
	int help = 0;
	while (std::getline(file, fileLine))
	{
		if (fileLine[0] == '\0')
		{
			continue;
		}
		if (fileLine[0] == '#')
		{
			continue;
		}
		if(help==0)
		{
			num_trainers =fileLine[0]-'0';
			help += 1;
			continue;
		}
		if(help==1)
		{
			create_trainers(num_trainers, fileLine);
			help += 1;
		}
		else { myLines.push_back(fileLine); }
	}
	
	for (int i = 0; i < static_cast<int>(myLines.size()); i++)
	{
		std::string res;
		for (char c : myLines[i])
		{
			if (c == ',' || c == ' ')
			{
				//cout << res;
				if (res != "" && c == ' ')
				{
					continue;
				}
				if (res != "")
				{
					if(res == "RopeJumps")
					{
						res = "Rope Jumps";
					}
					work_options.push_back(res);
				}

				res = "";
				continue;
			}
			res += c;
		}
		work_options.push_back(res);
	}

	int counter = 0;
	for(int i =0; i< static_cast<int>(work_options.size());i+=3)
	{
		if(work_options[i+1]== "Anaerobic")
		{
			workout_options.push_back(Workout(counter, work_options[i], std::stoi(work_options[i + 2]), ANAEROBIC));
		}
		else if(work_options[i+1]=="Mixed")
		{
			workout_options.push_back(Workout(counter, work_options[i], std::stoi(work_options[i + 2]), MIXED));
		}
		else
		{
			workout_options.push_back(Workout(counter, work_options[i], std::stoi(work_options[i + 2]), CARDIO));
		}
		counter += 1;
	}
	work_options.clear();
	myLines.clear();
}

void Studio::start()
{
	this->open = true;
	std::cout << "Studio is now open!\n";
	while(this->open)
	{
		std::string user_input1;
		std::string c;
		int help = 0;
		std::string action;
		int t_id;
		int dst;
		int cid;
		int max_cap = 0;
		std::vector<Customer*> customersList;
		std::getline(std::cin, user_input1);
		for (int i = 0; i < static_cast<int>(user_input1.length()); i++)
		{
			if(user_input1[i] != ' ')
			{
				c += user_input1[i];
			}
			else
			{
				if(help==0)
				{
					action = c;
					help++;
				}
				else if(help==1)
				{
					t_id = stoi(c);
					help++;
				}
				else
				{
					if (action == "move")
					{
						if (help == 2){
							dst = stoi(c);
						help++;
					}
					}
					else {
						if(getTrainer(t_id)!=nullptr && getTrainer(t_id)->getCapacity()<=max_cap)
						{
							break;
						}
						customersList.push_back(create_customers(c, cust_id));
						cust_id++;
						max_cap++;
					}
				}
				c = "";
			}
		}
		if(help==0)
		{
			action = c;
		}
		if(help==1)
		{
			t_id = stoi(c);
		}
		if(help==3 && action=="move")
		{
			cid = stoi(c);
			help++;
		}
		else if(help == 2 )
		{
			Customer* cust = create_customers(c, cust_id);
			cust_id++;
			customersList.push_back(cust);
			c = "";
		}
		if (action == "open")
		{
			if(!c.empty())
			{
				Customer* cust = create_customers(c, cust_id);
				customersList.push_back(cust);
				cust_id++;
			}
			auto otrainer = new OpenTrainer(t_id, customersList);
			
			otrainer->act(*this);

		}
		else if (action == "order")
		{
			Order* order = new Order(t_id);
			order->act(*this);

		}
		else if (action == "move")
		{
			MoveCustomer* move_customer = new MoveCustomer(t_id, dst, cid);
			move_customer->act(*this);
		}
		else if (action == "close")

		{
			Close* close = new Close(t_id);
			close->act(*this);
		}
		else if (action == "closeall")
		{
			CloseAll* c_all = new CloseAll();
			c_all->act(*this);
		}
		else if (action == "status")
		{
			PrintTrainerStatus* pts = new PrintTrainerStatus(t_id);
			pts->act(*this);
		}
		else if (action == "log")
		{
			PrintActionsLog* pal = new PrintActionsLog();
			pal->act(*this);
			delete pal;
		}
		else if (action == "workout_options")
		{
			PrintWorkoutOptions* pwo = new PrintWorkoutOptions();
			pwo->act(*this);
		}
		else if(action == "restore")
		{
			RestoreStudio* restore = new RestoreStudio();
			restore->act(*this);
		}
		else if(action == "backup")
		{
			BackupStudio* backup = new BackupStudio();
			backup->act(*this);
		}
	}
	

}


int Studio::getNumOfTrainers() const
{
	return (int)trainers.size();
}

Trainer* Studio::getTrainer(int tid)
{
	if (tid < getNumOfTrainers()) {
		return trainers[tid];
	}
	return nullptr;
}

const std::vector<BaseAction*>& Studio::getActionsLog() const
{
	return actionsLog;
}

std::vector<Workout>& Studio::getWorkoutOptions()
{
	return workout_options;
}

void Studio::addAction(BaseAction* action)
{
	this->actionsLog.push_back(action);
}

Customer* Studio::create_customers(std::string cust, int id)
{
	std::string temp;
	std::string cust_name;
	std::string cust_type;
	for (char d : cust)
	{
		if (d == ',')
		{
			cust_name = temp;
			temp = "";
			continue;
		}
		temp += d;
	}
	cust_type = temp;

	if(cust_type=="swt")
	{
		return new SweatyCustomer(cust_name, id);
	}
	else if(cust_type=="chp")
	{
		return new CheapCustomer(cust_name, id);
	}
	else if(cust_type=="mcl")
	{
		return new HeavyMuscleCustomer(cust_name, id);
	}
	else if(cust_type=="fbd")
	{
		return new FullBodyCustomer(cust_name, id);
	}
	return nullptr;
}

void Studio::create_trainers(int num, std::string capacities)
{
	for (char c : capacities)
	{
		if (c == ',')
		{
			continue;
		}
		Trainer* t = new Trainer(c-'0');
		getTrainers().push_back(t);
	}
}

void Studio::create_workouts(std::string line)
{
	std::vector<std::string> work_options = std::vector<std::string>();

}

void Studio::clean()
{
	workout_options.clear();
	for (int i = 0; i < static_cast<int>(trainers.size()); i++)
	{
		delete trainers[i];
	}
	trainers.clear();
	for (BaseAction* act : actionsLog)
	{
		delete act;
	}
	actionsLog.clear();

}

void Studio::copy(const Studio&studio)
{
	if (this != &studio) {
		this->open = studio.open;
		this->cust_id = studio.cust_id;
		for (int i = 0; i < static_cast<int>(studio.workout_options.size()); i++)
		{
			this->workout_options.push_back(studio.workout_options[i]);
		}
		for (Trainer* trainer : studio.trainers)
		{
			
			this->trainers.push_back(new Trainer(*trainer));
		}
		for (BaseAction* act : studio.getActionsLog())
		{
			this->actionsLog.push_back(act->clone());
		}
	}
}

std::vector<Trainer*>& Studio::getTrainers()
{
	return this->trainers;
}

void Studio::set_open()
{
	this->open = false;
}
