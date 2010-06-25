#include "step.h"

Step::Step() : QObject(), stepname(""), kplus(0), kminus(0)
{}

Cpd* Step::getReagent(int n){
	return reagents[n-1];
}

bool Step::setReagent(int n, Cpd* cpd)
{
	if( n>=1 && n <=5 ) //if valid index
	{
		//refresh involved species
		if( n>=1 && n<=3 ){
			if(reagents[n-1]!=NULL) reagents[n-1]->removeAsReactant(this);
			if(cpd!=NULL) cpd->addAsReactant(this);
		}
		else{
			if(reagents[n-1]!=NULL) reagents[n-1]->removeAsProduct(this);
			if(cpd!=NULL) cpd->addAsProduct(this);
		}
		//add the reagent to the array
		reagents[n-1] = cpd;

		return true;
	} else return false;
}

QString Step::name(){ return stepname; }
void Step::setName(QString n){ stepname = n; emit nameChanged(n);}

double Step::kPlus(){ return kplus; }
void Step::setKPlus(double k){ kplus = k; emit kPlusChanged(kplus);}
void Step::setKPlus(QString k){ kplus = k.toDouble(); emit kPlusChanged(kplus);}

double Step::kMinus(){ return kminus; }
void Step::setKMinus(double k){ kminus = k; emit kMinusChanged(kminus);}
void Step::setKMinus(QString k){ kminus = k.toDouble(); emit kMinusChanged(kminus);}

QString Step::toString()
{
	QString ret = "";
	for( int i=1; i<=3; i++ )
		if( getReagent(i)!=0 )
			ret += getReagent(i)->toString() + " + ";
	ret = ret.left(ret.length()-2) + "⇌ ";
	for( int i=4; i<=6; i++ )
		if( getReagent(i)!=0 )
			ret += getReagent(i)->toString() + " + ";
	ret = ret.left(ret.length()-3);
	return ret;
}
