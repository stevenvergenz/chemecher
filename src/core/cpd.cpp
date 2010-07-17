#include "cpd.h"

const QString Cpd::STATES[6] = {"(.)","(*)","(aq)","(s)","(l)", "(g)"};
const QString Cpd::TRANS[3] = {"none", "linear", "arctan"};

bool Cpd::isHomo(){ return cpd_state==HOMO || cpd_state==AQ || cpd_state==G; }
bool Cpd::isHetero(){ return !isHomo(); }

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

const QString Cpd::toString(){
	//QString states[6] = {"(.)","(*)","(aq)","(s)","(l)", "(g)"};
	return shortname + STATES[(int)cpd_state];
}

const QString Cpd::tov3String(){
	return shortname           + QString().fill(' ',mix->maxCpdNameLen()+1-shortname.length()) + 
		STATES[(int)cpd_state] + QString().fill(' ',mix->maxStateLen()+1-STATES[(int)cpd_state].length()) + 
		(isHomo() ? "0 0" : QString("%1 %2").arg(thresh).arg(sharp) );
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
bool Cpd::setState(QString s) {
	for( int i=0; i<6; i++ )
		if( s==STATES[i] ) {
			setState(i);
			return true;
		}
	return false;
}

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
bool Cpd::setTransition(QString t) {
	for( int i=0; i<3; i++ )
		if( t==TRANS[i] ) {
			setTransition(i);
			return true;
		}
	return false;
}

//initial concentration
double  Cpd::initialConc(){ return conc; }
void Cpd::setInitialConc(double c ){ conc = c; emit initialConcChanged(c);}
void Cpd::setInitialConc(QString c){ conc = c.toDouble(); emit initialConcChanged(conc);}
