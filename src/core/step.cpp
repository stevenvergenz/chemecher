#include "step.h"

Step::Step() : QObject(), stepname(""), kplus(0), kminus(0)
{
	mix->connect( this, SIGNAL(reagentsChanged()), mix, SIGNAL(stepListChanged()) );
}

QList<Cpd*> Step::reactantList()
{ return reactants; }
QList<Cpd*> Step::productList()
{ return products; }

bool Step::isValid()
{
	if( reactants.size()<1 || reactants.size()>3 )
		return false;
	if( products.size()<1 || products.size()>3 )
		return false;
	
	return true;
}

// reagent checkers
bool Step::hasCpd(Cpd *cpd)
{ return ( hasReactant(cpd) || hasProduct(cpd) ); }
bool Step::hasReactant(Cpd *cpd)
{ return reactantList().contains(cpd); }
bool Step::hasProduct(Cpd *cpd)
{ return productList().contains(cpd); }

// adding and removing reagents
bool Step::addReactant( Cpd* cpd ) {
	if( reactants.size()==3 )
		return false;
	reactants.append( cpd );
	cpd->stoiVals[this]--;
	emit reagentsChanged();
	return true;
}
bool Step::addProduct ( Cpd *cpd ) {
	if( products.size()==3 )
		return false;
	products.append( cpd );
	cpd->stoiVals[this]++;
	emit reagentsChanged();
	return true;
}
bool Step::setReactant( int i, Cpd* cpd )
{
	if( reactants.size()<i+1 )
		return false;
	reactants[i] = cpd;
	emit reagentsChanged();
	return true;
}
bool Step::setProduct( int i, Cpd* cpd )
{
	if( products.size()<i+1 )
		return false;
	products[i] = cpd;
	emit reagentsChanged();
	return true;
}
bool Step::removeReactant( int cpd ) {
	if( cpd < 0 || cpd >= reactantList().size() )
		return false;
	reactants.removeAt( cpd );
	emit reagentsChanged();
	return true;
}
bool Step::removeProduct( int cpd ) {
	if( cpd < 0 || cpd >= productList().size() )
		return false;
	products.removeAt( cpd );
	emit reagentsChanged();
	return true;
}

// getters and setters
QString Step::name(){ return stepname; }
void Step::setName(QString n){ stepname = n; emit nameChanged(n);}
QString Step::desc(){ return stepdesc; }
void Step::setDesc(QString n){ stepdesc = n; emit descChanged(n);}
double Step::kPlus(){ return kplus; }
void Step::setKPlus(double k){ kplus = k; emit kPlusChanged(kplus);}
double Step::kMinus(){ return kminus; }
void Step::setKMinus(double k){ kminus = k; emit kMinusChanged(kminus);}

// returns the reaction equation
QString Step::toString()
{
	QString ret = "";
	int i;
	for( i=0; i<reactants.size(); i++ )
		ret += reactants[i]->toString() + " + ";
	if( i>0 )
		ret = ret.left(ret.length()-2) + "<-> ";
	for( i=0; i<products.size(); i++ )
		ret += products[i]->toString() + " + ";
	if( i>0 )
		ret = ret.left(ret.length()-3);
	return ret;
}
