#include "cpd.h"

const QString Cpd::STATES[6] = {"(.)","(*)","(aq)","(s)","(l)", "(g)"};

// base constructor calls the other constructor with default parameters
Cpd::Cpd() { Cpd::Cpd("", Cpd::HOMO); }

Cpd::Cpd(QString n, Cpd::State s) : QObject(), shortname(n), longname(""),
	cpd_state(s), thresh(0), sharp(0), trans(NONE), conc(0), saved_conc(0)
{
	//initialize partial_concs
	partial_conc[0] = 0; rate[0] = 0;
	partial_conc[1] = 0; rate[1] = 0;
	partial_conc[2] = 0; rate[2] = 0;
	partial_conc[3] = 0; rate[3] = 0;
	partial_conc[4] = 0; rate[4] = 0;
	partial_conc[5] = 0; rate[5] = 0;
}

/*void Cpd::addAsReactant(Step* step)
{
	//check to see if it's already in the set step as a reactant
	for(int i=0; i<reactantset.size(); i++)
		if(reactantset[i]->step == step){
			reactantset[i]->stoi--; //if so, decrement v_pw
			return;
		}

	//it is not already listed, create a new pair
	StepPair* newpair = new StepPair(step, -1);
	reactantset.append( newpair );
}

void Cpd::removeAsReactant(Step* step)
{
	//search the set of reactants
	for(int i=0; i<reactantset.size(); i++)
	{
		//for a particular step
		if(reactantset[i]->step == step)
		{
			reactantset[i]->stoi++; //decrement the amount of the reactant

			//remove if there was only one of that reactant
			if(reactantset[i]->stoi == 0)
			{
				delete reactantset[i];
				reactantset.removeAt(i);
			}//remove stub
			return;
		}//step found
	}//for each step in reactants
}//function

void Cpd::addAsProduct(Step* step)
{
	//check to see if it's already in the set step as a reactant
	for(int i=0; i<productset.size(); i++)
		if(productset[i]->step == step){
			productset[i]->stoi++; //if so, increment v_pw
			return;
		}

	//it is not already listed, create a new pair
	StepPair* newpair = new StepPair(step, 1);
	productset.append( newpair );
}

void Cpd::removeAsProduct(Step* step)
{
	//search the set of products
	for(int i=0; i<productset.size(); i++)
	{
		//for a particular step
		if(productset[i]->step == step)
		{
			productset[i]->stoi--; //decrement the amount of the product

			//remove if there was only one of that reactant
			if(reactantset[i]->stoi == 0)
			{
				delete productset[i];
				productset.removeAt(i);
			}//remove stub
			return;
		}//step found
	}//for each step in products
}*/

QString Cpd::toString(){
	//QString states[6] = {"(.)","(*)","(aq)","(s)","(l)", "(g)"};
	return shortname + STATES[(int)cpd_state];
}

/********************** Getters and Setters *********************/
//short name
QString Cpd::shortName(){ return shortname; }
void Cpd::setShortName(QString s){ shortname = s; emit shortNameChanged(s);}

//long name
QString Cpd::longName(){ return longname; }
void Cpd::setLongName(QString s){ longname = s; emit longNameChanged(s);}

//state
Cpd::State Cpd::state(){ return cpd_state; }
void Cpd::setState(Cpd::State s){ cpd_state = s; emit stateChanged((int)s);}
void Cpd::setState(int s){ cpd_state = (State)s; emit stateChanged(s);}

//theshold
double Cpd::threshold(){ return thresh; }
void Cpd::setThreshold(double t){ thresh = t; emit thresholdChanged(t);}

//sharpness
double Cpd::sharpness(){ return sharp; }
void Cpd::setSharpness(double s){ sharp = s; emit sharpnessChanged(s);}

//transition
Cpd::Transition Cpd::transition(){ return trans; }
void Cpd::setTransition(Transition t){ trans = t; emit transitionChanged((int)t);}
void Cpd::setTransition(int t){ trans = (Transition)t; emit transitionChanged(t);}

//initial concentration
double  Cpd::initialConc(){ return conc; }
void Cpd::setInitialConc(double c ){ conc = c; emit initialConcChanged(c);}
void Cpd::setInitialConc(QString c){ conc = c.toDouble(); emit initialConcChanged(conc);}
