#include "step.h"

Step::Step() : QObject(), stepname(""), kplus(0), kminus(0)
{}

// adding and removing reagents

bool Step::addReactant( Cpd* cpd ) {
	if( reactants.size()==3 )
		return false;
	
	reactants.append( cpd );
	cpd->stoiVals[this]--;
}
bool Step::addProduct ( Cpd *cpd ) {
	if( products.size()==3 )
		return false;
	
	products.append( cpd );
	cpd->stoiVals[this]++;
}

bool Step::removeReagent( Cpd* cpd ) {
	if( cpd==0 || !hasCpd(cpd) ) return false;
	
}
bool Step::removeReagent( int cpd ) {
	if( cpd < 0 || cpd >= getReactantList().size() )
		return false;
	return removeReagent( getReactantList().at(cpd) );
}
bool Step::swapReagents( Cpd* cpd1, Cpd* cpd2 ) {
	
}
bool Step::swapReagents( int cpd1, int cpd2 ) {
	
}

bool Step::setStoi( Cpd* cpd, int stoi ) {
	
}
bool Step::addStoi( Cpd* cpd, int diff ) {
	
}

QString Step::name(){ return stepname; }
void Step::setName(QString n){ stepname = n; emit nameChanged(n);}
double Step::kPlus(){ return kplus; }
void Step::setKPlus(double k){ kplus = k; emit kPlusChanged(kplus);}
double Step::kMinus(){ return kminus; }
void Step::setKMinus(double k){ kminus = k; emit kMinusChanged(kminus);}

QList<Cpd*> Step::getReactantList()
{ return reactants; }
QList<Cpd*> Step::getProductList()
{ return products; }

bool Step::hasCpd(Cpd *cpd) {
	return ( getReactantList().contains(cpd) || getProductList().contains(cpd) );
}

QString Step::toString()
{
	
}
