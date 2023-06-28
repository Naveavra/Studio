#include "Action.h"
#include <iostream>
#include "Studio.h"
extern Studio* backup;

using namespace std;


BaseAction::BaseAction()
{
}

ActionStatus BaseAction::getStatus() const
{
	return this->status;
}


void BaseAction::complete()
{
	this->status = COMPLETED;
}

void BaseAction::setStatus(ActionStatus status)
{
	this->status = status;
}

void BaseAction::error(std::string errorMsg)
{
	this->errorMsg = std::move(errorMsg);
	this->status = ERROR;
}

std::string BaseAction::getErrorMsg() const
{
	return errorMsg;
}




OpenTrainer::OpenTrainer(int id, std::vector<Customer*>& customersList):trainerId(id)
{
	for(Customer * cust: customersList)
	{
		customers.push_back(cust->clone());
	}
	for (Customer* cust : customersList)
	{
		delete cust;
		cust = nullptr;
	}
	customersList.clear();
	this->error("Workout session does not exist or is already open.");
}

void OpenTrainer::act(Studio& studio)
{
	if(studio.getTrainer(trainerId) == nullptr)
	{
		std::cout << getErrorMsg() << std::endl;
		studio.addAction(this);
		return;
	}
	for (Customer* customer : customers)
	{
		int cust_id = customer->getId();
		if(studio.getTrainer(trainerId)->isFull())
		{
			std::cout << getErrorMsg() << std::endl;
			break;
		}
		studio.getTrainer(trainerId)->addCustomer(customer->clone());
		args += studio.getTrainer(trainerId)->getCustomer(cust_id)->toString() + " ";
	}
	// customers.clear();
	if(studio.getTrainer(trainerId)->isOpen())
	{
		std::cout<<getErrorMsg()<<endl;
	}
	else
	{
		for(Customer* cust: studio.getTrainer(trainerId)->getCustomers())
		{
			cust->order(studio.getWorkoutOptions());
		}
		studio.getTrainer(trainerId)->openTrainer(); /**Toda le Itay**/
		this->complete();
	}
	studio.addAction(this);
	
}

std::string OpenTrainer::toString() const
{
	string res = "Open " + to_string(trainerId) + " " + args ;
	if(getStatus()==COMPLETED)
	{

		res += "COMPLETED";
	}else
	{
		res += "\nERROR: " + getErrorMsg();

	}
	return res;
	
}

OpenTrainer* OpenTrainer::clone()
{
	return new OpenTrainer(*this);
}

OpenTrainer::OpenTrainer(const OpenTrainer& other):trainerId(other.trainerId)
{
	this->args = other.args;
	this->error(other.getErrorMsg());
	this->setStatus(other.getStatus());
	for(Customer * cust: other.customers){
		customers.push_back(cust->clone());
	}
}

OpenTrainer::~OpenTrainer()
{
	for(Customer* cust:customers)
	{
		delete cust;
	}
}



Order::Order(int id):trainerId((id))
{
	this->error("Trainer does not exist or is not open");
}

void Order::act(Studio& studio)
{
	string ans;
	Trainer* trainer = studio.getTrainer(trainerId);
	if (trainer == nullptr || !trainer->isOpen())
	{
		std::cout << getErrorMsg() << std::endl;
	}
	else
	{
		for (Customer* nave : studio.getTrainer(trainerId)->getCustomers())
		{
			studio.getTrainer(trainerId)->order(nave->getId(), nave->order(studio.getWorkoutOptions()), studio.getWorkoutOptions());
			std::vector<OrderPair> orders = trainer->getOrders();
			for(int i = 0 ; i<static_cast<int>(orders.size()); i++)
			{
				if (orders[i].first == nave->getId())
				{
					cout << nave->getName() << " Is Doing " << orders[i].second.getName() << endl;
				}
			}
		}
		this->complete();
	}
	studio.addAction(this);
}


std::string Order::toString() const
{
	string res = "Order " + to_string(trainerId) + " "  ;
	if (getStatus() == COMPLETED)
	{
		
		res += "COMPLETED";
	}
	else
	{
		res += "\nERROR: " + getErrorMsg();

	}
	return res;
}

Order* Order::clone()
{
	return new Order(*this);
}

Order::Order(const Order& other) :trainerId(other.trainerId)
{
	this->setStatus(other.getStatus());

}




MoveCustomer::MoveCustomer(int src, int dst, int customerId): srcTrainer(src), dstTrainer(dst), id(customerId)
{
	this->error("Cannot move customer ");
}

void MoveCustomer::act(Studio& studio)
{
	if (studio.getTrainer(srcTrainer) == nullptr || studio.getTrainer(srcTrainer)->getCustomer(id) == nullptr)
	{
		std::cout << getErrorMsg() << std::endl;
			studio.addAction(this);
			return;
	}


	if (studio.getTrainer(srcTrainer)->getCustomer(id) == nullptr ||  studio.getTrainer(dstTrainer) == nullptr || studio.getTrainer(srcTrainer) == nullptr || !studio.getTrainer(srcTrainer)->isOpen() || !studio.getTrainer(dstTrainer)->isOpen() || studio.getTrainer(dstTrainer)->isFull())
	{
		std::cout << getErrorMsg() << std::endl;
		args += getErrorMsg();
	}
	else
	{
		studio.getTrainer(dstTrainer)->addCustomer(studio.getTrainer(srcTrainer)->getCustomer(id));
		studio.getTrainer(srcTrainer)->removeCustomer(id);
		studio.getTrainer(dstTrainer)->order(id, studio.getTrainer(dstTrainer)->getCustomer(id)->order(studio.getWorkoutOptions()), studio.getWorkoutOptions());
		if (studio.getTrainer(srcTrainer)->getCustomers().empty())
		{
			studio.getTrainer(srcTrainer)->closeTrainer();
		}
		this->complete();
	}
	studio.addAction(this);

}

std::string MoveCustomer::toString() const
{
	string res= "Move Customer " +to_string(id) + " From " + to_string(srcTrainer) + " to " + to_string(dstTrainer)+" ";
	if (getStatus() == COMPLETED)
	{
		res += "COMPLETED";
	}
	else
	{
		res += "\nERROR: " + getErrorMsg();

	}
	return res;
}

MoveCustomer* MoveCustomer::clone()
{
	return new MoveCustomer(*this);
}

MoveCustomer::MoveCustomer(const MoveCustomer& other) :srcTrainer(other.srcTrainer), dstTrainer(other.dstTrainer), id(other.id)
{
	this->error("Cannot move customer");
}




Close::Close(int id):trainerId(id)
{
	this->error("Error: Trainer does not exist or is not open");
}

void Close::act(Studio& studio)
{
	if (studio.getTrainer(trainerId) == nullptr || studio.getTrainer(trainerId)->isOpen() == false )
	{
		std::cout << getErrorMsg() << std::endl;
	}
	Trainer* trainer = studio.getTrainer(trainerId);
	trainer->closeTrainer();
	std::cout << "Trainer " << trainerId << " closed. Salary " << trainer->getSalary() << "NIS"  << std::endl;
	this->complete(); 
	studio.addAction(this);
}

std::string Close::toString() const
{
	string res = "Close " + to_string(trainerId) ;
	if (getStatus() == COMPLETED)
	{
		res += " COMPLETED";
	}
	else
	{
		res += "/nERROR: " + getErrorMsg();

	}
	return res;
}

Close* Close::clone()
{
	return new Close(*this);
}

Close::Close(const Close& other) :trainerId(other.trainerId)
{
}

CloseAll::CloseAll()
{
}

void CloseAll::act(Studio& studio)
{
	vector<Trainer*> trainers = studio.getTrainers();

	for (int i = 0; i<static_cast<int>(trainers.size()); i++)
	{
		if (trainers[i]->isOpen() == true)
		{
			Close* close = new Close(i);
			close->act(studio);
			trainers[i]->closeTrainer();
		}
		else {}
	}
	this->complete();
	studio.addAction(this);
	studio.set_open();
}


std::string CloseAll::toString() const
{
	return "CloseAll" ;
}

CloseAll* CloseAll::clone()
{
	return new CloseAll(*this);
}

CloseAll::CloseAll(const CloseAll& other)
{
}

PrintWorkoutOptions::PrintWorkoutOptions() = default;


void PrintWorkoutOptions::act(Studio& studio)
{
	std::string ans = " ";
	string type;
	std::vector<Workout> work_opt =  studio.getWorkoutOptions();
	for (int i=0; i<static_cast<int>(work_opt.size()); i++)
	{
		if(work_opt[i].getType() == CARDIO)
		{
			
			type = "Cardio";
		}
		else if(work_opt[i].getType() == ANAEROBIC)
		{
			type = "Anaerobic";
		}
		else
		{
			type = "Mixed";

		}
		this->complete();
		cout<< work_opt[i].getName() << ", " << type << ", " << to_string(work_opt[i].getPrice()) <<endl;
		
	}
}

std::string PrintWorkoutOptions::toString() const
{
	return "Workout Options";
}

PrintWorkoutOptions* PrintWorkoutOptions::clone()
{
	return new PrintWorkoutOptions(*this);
}

PrintTrainerStatus* PrintTrainerStatus::clone()
{
	return new PrintTrainerStatus(*this);
}

PrintTrainerStatus::PrintTrainerStatus(const PrintTrainerStatus& other):trainerId(other.trainerId)
{
}

PrintWorkoutOptions::PrintWorkoutOptions(const PrintWorkoutOptions& other)
{
}



PrintTrainerStatus::PrintTrainerStatus(int id) : trainerId(id) {  }

void PrintTrainerStatus::act(Studio& studio)
{
	Trainer* trainer = studio.getTrainer(trainerId);

	vector <Customer*>& customers = trainer->getCustomers();
	vector<OrderPair>& orders = trainer->getOrders();
	if (!trainer->isOpen())
	{
		cout << "Trainer " << to_string(trainerId) << " status: closed" << endl;
	}
	else
	{
		cout << "Trainer " << to_string(trainerId) << " status: open " << endl;
		cout << "Customers: " << endl;
		for (int i = 0; i < static_cast<int>(customers.size()); i++)
		{
			cout << customers[i]->getId() << " " << customers[i]->getName() << endl;
		}
		cout << "Orders: " << endl;
		for (int i = 0; i < static_cast<int>(customers.size()); i++)
		{
			for (int j = 0; j < static_cast<int>(orders.size()); j++)
			{
				if (orders[j].first == customers[i]->getId()) {
					cout << orders[j].second.getName()<<' '<< orders[j].second.getPrice() << "NIS " << customers[i]->getId() << endl;
				}
				else{}
			}
		}
		this->complete();
		cout << "Current Trainer's Salary: " << to_string(trainer->getSalary()) << "NIS" << endl;
	}
	studio.addAction(this);

}

std::string PrintTrainerStatus::toString() const
{
	return "Print Trainer Status " + to_string(trainerId)  ;
}



PrintActionsLog::PrintActionsLog()
{
}

void PrintActionsLog::act(Studio& studio)
{
	std::vector<BaseAction*> act_log = studio.getActionsLog();
	for(int i = 0; i< static_cast<int>(act_log.size());i++)
	{
		cout << act_log[i]->toString()<< endl;
	}
}

std::string PrintActionsLog::toString() const
{
	return "BLANK";
}

PrintActionsLog* PrintActionsLog::clone()
{
	return new PrintActionsLog(*this);
}

BackupStudio* BackupStudio::clone()
{
	return new BackupStudio(*this);
}

RestoreStudio* RestoreStudio::clone()
{
	return new RestoreStudio(*this);
}

RestoreStudio::RestoreStudio(const RestoreStudio& other)
{
}

BackupStudio::BackupStudio(const BackupStudio& other)
{
}

PrintActionsLog::PrintActionsLog(const PrintActionsLog& other)
{
}


BackupStudio::BackupStudio()
{
}

void BackupStudio::act(Studio& studio)
{
	if(backup==nullptr)
	{
		backup = new Studio();
	}
	*backup = studio;
	this->complete();
	studio.addAction(this);
	//delete this;

}

std::string BackupStudio::toString() const
{
	return "Backup studio completed";
}

RestoreStudio::RestoreStudio()
{
	this->error("No backup available");
}

void RestoreStudio::act(Studio& studio)
{
	if (backup != nullptr)
	{
		studio = *backup;
		this->complete();
	}
	else {
		cout << getErrorMsg() << endl;
	}
	studio.addAction(this);
}

std::string RestoreStudio::toString() const
{
	return std::string("Studio has been Restored successfully");
}
