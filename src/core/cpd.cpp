#include "cpd.h"

const QString Cpd::STATES[6] = {"(.)","(*)","(aq)","(s)","(l)", "(g)"};
const QString Cpd::TRANS[3] = {"none", "linear", "arctan"};

bool Cpd::isHomo(){ return (cpd_state==HOMO || cpd_state==AQ || cpd_state==G); }
bool Cpd::isHetero(){ return !isHomo(); }

// base constructor calls the other constructor with default parameters
Cpd::Cpd() : QObject(), shortname(""), longname(""),
cpd_state(Cpd::HOMO), thresh(0), sharp(0), trans(NONE)
{
	setInitialConc(0);
}

Cpd::Cpd(QString n, Cpd::State s) : QObject(), shortname(n), longname(""),
	cpd_state(s), thresh(0), sharp(0), trans(NONE), init_conc(0)
{
	setInitialConc(0);
}

const QString Cpd::toString(){
	//QString states[6] = {"(.)","(*)","(aq)","(s)","(l)", "(g)"};
	return shortname + STATES[(int)cpd_state];
}

const QString Cpd::tov3String(){
	return shortname           + " " + //QString().fill(' ',mix->maxCpdNameLen()+1-shortname.length()) + 
		STATES[(int)cpd_state] + " " + //QString().fill(' ',mix->maxStateLen()+1-STATES[(int)cpd_state].length()) + 
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
bool Cpd::setState(QString s, bool *ok ){
	for( int i=0; i<6; i++ )
		if( s==STATES[i] ) {
			cpd_state = (State)i;
			emit stateChanged(i);
			if(ok!=0) (*ok)=true;
			return true;
		}
	if(ok!=0) (*ok)=false;
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
bool Cpd::setTransition(QString s, bool *ok ){
	if(s=="none") thresh = NONE;
	else if(s=="linear") thresh = LINEAR;
	else if(s=="atan") thresh = ATAN;
	else {
		if(ok!=0) (*ok)=false;
		return false;
	}
	if(ok!=0) (*ok)=true;
	return true;
}

//initial concentration
double  Cpd::initialConc(){ return init_conc; }
void Cpd::setInitialConc(double c ){ init_conc = c; emit initialConcChanged(c);}
void Cpd::setInitialConc(QString c){ init_conc = c.toDouble(); emit initialConcChanged(init_conc);}

