#include "mix.h"

Mix *mix = new Mix();

Mix::Mix()
	: isActive( false ),
	  mechName(""), mechDesc(""),
	  timeStep(0), reportStep(0),
	  startTime(0), endTime(0),
	  debugStart(0), debugEnd(0),
	  precision(0.001),
	  order(5), method("runge"),
	  transition("none"),
	  autostep(true), gateband(0),
	  shifttest(0), maxreduce(0), stepfactor(0)
{
	connect( this, SIGNAL(cpdListChanged()), this, SIGNAL(stepListChanged()) );
}

QStringList Mix::cpdIdList() {
	QStringList ret;
	for( int i=0; i<CpdList.size(); i++ )
		ret.append(CpdList[i]->toString());
	return ret;
}
Cpd* Mix::addCpd(Cpd *cpd) {
	CpdList.append(cpd);
	emit cpdListChanged();
	return cpd;
}
Cpd* Mix::removeCpd(Cpd *cpd)
{
	CpdList.removeOne(cpd);
	emit cpdListChanged();
	return cpd;
}
void Mix::swapCpds(int first, int second)
{
	CpdList.swap(first,second);
	emit cpdListChanged();
}
Cpd* Mix::getCpdById(QString id)
{
	Cpd *ret = 0;
	for( int i=0; i<CpdList.size(); i++ ) {
		if( CpdList[i]->toString() == id )
			ret = CpdList[i];
	}
	return ret;
}

QStringList Mix::stepNameList() {
	QStringList ret;
	for( int i=0; i<StepList.size(); i++ )
		ret.append(StepList[i]->name());
	return ret;
}
QStringList Mix::stepStringList() {
	QStringList ret;
	for( int i=0; i<StepList.size(); i++ )
		ret.append(StepList[i]->toString());
	return ret;
}

Step* Mix::addStep(Step *step) {
	StepList.append(step);
	emit stepListChanged();
	return step;
}
Step* Mix::removeStep(Step *step)
{
	StepList.removeOne(step);
	emit stepListChanged();
	return step;
}
void Mix::swapSteps(int first, int second)
{
	StepList.swap(first,second);
	emit stepListChanged();
}
Step* Mix::getStepByString(QString string)
{
	Step *ret = 0;
	for( int i=0; i<StepList.size(); i++ ) {
		if( StepList[i]->toString() == string )
			ret = StepList[i];
	}
	return ret;
}
Step* Mix::getStepByName(QString name)
{
	Step *ret = 0;
	for( int i=0; i<StepList.size(); i++ ) {
		if( StepList[i]->name() == name )
			ret = StepList[i];
	}
	return ret;
}

void Mix::clone(Mix* newmix)
{
	//deep copy both lists
	StepList.clear();
	StepList.append(newmix->StepList);
	CpdList.clear();
	CpdList.append(newmix->CpdList);

	//current mix identifiers
	mechName = newmix->mechName;
	mechDesc = newmix->mechDesc;

	//time data
	timeStep = newmix->timeStep;
	reportStep = newmix->reportStep;
	startTime = newmix->startTime;
	endTime = newmix->endTime;
	debugStart = newmix->debugStart;
	debugEnd = newmix->debugEnd;
	
	//accuracy data
	precision = newmix->precision;

	//legacy parameters
	order = newmix->order;
	method = newmix->method;
	transition = newmix->transition;
	autostep = newmix->autostep;
	gateband = newmix->gateband;
	shifttest = newmix->shifttest;
	maxreduce = newmix->maxreduce;
	stepfactor = newmix->stepfactor;
	
	// sql data
	db = newmix->db;

	//accuracy data
	precision = newmix->precision;

	emit stepListChanged();
	emit cpdListChanged();
}

// i/o stuff
int Mix::maxCpdNameLen() {
	int maxlen=0;
	for( int i=0; i<mix->CpdList.length(); i++ )
		if( mix->CpdList[i]->shortName().length()>maxlen )
			maxlen = mix->CpdList[i]->shortName().length();
	return maxlen;
}
int Mix::maxCpdIdLen() {
	int maxlen=0;
	for( int i=0; i<mix->CpdList.length(); i++ )
		if( mix->CpdList[i]->toString().length()>maxlen )
			maxlen = mix->CpdList[i]->toString().length();
	return maxlen;
}
int Mix::maxStateLen() {
	int maxlen=0;
	for( int i=0; i<mix->CpdList.length(); i++ )
		if( Cpd::STATES[(int)mix->CpdList[i]->state()].length()>maxlen )
			maxlen = Cpd::STATES[(int)mix->CpdList[i]->state()].length();
	return maxlen;
}
// gets the position of the arrow in the step where it is highest
int Mix::maxStepArrowPos() {
	int maxlen = 0;
	for( int i=0; i<mix->StepList.size(); i++ ) {
		QString string = mix->StepList[i]->toString(">").replace(" ","");
		if( string.indexOf(">")>maxlen )
			maxlen = string.indexOf(">");
	}
	return maxlen;
}
// gets the length of step output in the longest step
int Mix::maxStepLen() {
	int maxlen = 0;
	for( int i=0; i<mix->StepList.size(); i++ ) {
		QString string = mix->StepList[i]->toString(">").replace(" ","");
		string = QString().fill(' ',mix->maxStepArrowPos()-string.indexOf(">")) + string;
		if( string.length()>maxlen )
			maxlen = string.length();
	}
	return maxlen;
}

void Mix::setAutostep(bool val){autostep = val;}
void Mix::setName( QString val ){mechName = val;}
void Mix::setDesc( QString val ){mechDesc = val;}

QList<QStringList> Mix::availableMethods()
{
	QList<QStringList> methods;
	methods.append( QStringList() << "euler" );
	methods.append( QStringList() << "modified euler" << "heun" << "ralston" );
	methods.append( QStringList() << "runge-kutta" );
	methods.append( QStringList() << "runge" << "kutta" << "gill" );
	return methods;
}

void Mix::initialize()
{

}

void Mix::calculateRKF()
{

}

void Mix::calculateLegacy()
{
	// declare local variables
	double curTime = 0, prevTime = startTime;
	bool overflow = false;
	int expon = 0; shiftCnt = 0;
	int curOrder = 0, maxOrder = order;
	int dTime = 0;
	
	// print the log header for this calculation run
	iomgr->log << endl;
	iomgr->printMechSummary( iomgr->log );
	
	// print initial concentrations to the output file
	iomgr->printData(0);
	
	/*************************************************
	#             MAIN PROCESSING LOOP               #
	*************************************************/
	do { // time loop
	
		// save previous concentrations
		for(int i=0; i<CpdList.size(); i++){
			CpdList[i]->saveConc();
		}
		
DownStep:
		
		// autostepping mechanism
		if( autostep ){
			dTime = pow(timeStep*stepfactor, -expon);
		}
		else{
			dTime = timeStep;
		}
		
		// increment time
		curTime = prevTime + dTime;
		
		for( curOrder=1; curOrder<=maxOrder; curOrder++)
		{
			// calculate the rates
			for(int i=0; i<StepList.size(); i++){
				StepList[i]->calcRates();
			}
			
			// calculate concentrations based on rates
			for(int i=0; i<CpdList.size(); i++){
			
			}
		}
		
	// finish time loop
	} while( curTime < endTime && !overflow );
	/*************************************************
	#         FINISH MAIN PROCESSING LOOP            #
	*************************************************/
}
































