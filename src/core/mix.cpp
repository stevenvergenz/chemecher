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

bool Mix::calculateRKF()
{
	return true;
}

bool Mix::calculateLegacy()
{
	// debugging fio
	/*iomgr->openRunOutputFile();
	iomgr->openLogFile();
	iomgr->printMechSummary( iomgr->data );
	iomgr->printMechSummary( iomgr->log );
	iomgr->printData(0);
	iomgr->data.device()->close();
	return true;*/
	
	cout << "State of A: " << (int)CpdList.at(0)->state() << endl;
	cout << "Is it homo? " << CpdList.at(0)->isHomo() << endl;
	
	// begin true calculation
	bool overflow       = false;	// is set if a concentration reaches too high
	double time	    = startTime;// goes from mix.startTime to mix.endTime
	double timePrev     = 0;		// the timestep before the previous
	double lastReport   = 0;		// the last time the concentration was printed
	double dTime        = 0;		// handles the increment amount, adjusted when autostepping
	int exp             = 0;		// handles rate of change in timestep
	int shiftCount      = 0;		// the number of times dT has been stepped from original
	int maxOrder	    = mix->order; // placeholder for class property
	int order           = 0;		// loop variable for each order
	QList<Cpd*>::const_iterator acpd;
	QList<Step*>::const_iterator astep;

	// populate concentration set with initial concentrations
	for( acpd = CpdList.constBegin(); acpd != CpdList.constEnd(); acpd++ ){
		(*acpd)->prevConc = (*acpd)->initialConc();
		(*acpd)->finalConc = (*acpd)->initialConc();
	}

	// open the appropriate files: log, output, and debug
	if( !iomgr->openLogFile() ){
		cout << "Could not open log file, exiting!" << endl;
		return false;
	}
	iomgr->printMechSummary( iomgr->log );
	iomgr->openRunOutputFile();

	// set constants based on order and method
	if( !setCalcConstants() ){
		cout << "Could not initialize constants, exiting!" << endl;
		return false;
	}
	
	// print the column headers and initial concentrations
	iomgr->printData( 0 );

	/////////////////////////////////////////////////////////
	//               MAIN CALCULATION LOOP                 //
	/////////////////////////////////////////////////////////

	while( time <= endTime && !overflow )
	{
		// save the calculated concentrations and time
		for( QList<Cpd*>::const_iterator i = CpdList.constBegin(); i != CpdList.constEnd(); i++ )
			(*i)->prevConc = (*i)->finalConc;

		timePrev = time;

DownStep:	// autostep reentry point

		// create the new timestep
		if( autostep ){
			dTime = timeStep * pow( stepfactor, -exp );
			shiftCount++;
		}
		else dTime = timeStep;

		// increment time
		time += dTime;

		// begin order loop
		for( order=1; order<=maxOrder; order++ )
		{
			// calculate the forward and reverse rates for each step
			for( astep = StepList.begin(); astep != StepList.end(); astep++ )
			{
				// calculate the forward rate
				double ra = (*astep)->kPlus();

				for( acpd = (*astep)->reactantList().begin(); acpd != (*astep)->reactantList().end(); acpd++ )
				{
					cout << "In loop homo test for " << (*acpd)->toString().toStdString()
					        << " " << (*acpd)->isHomo() << endl;
					
					if( (*acpd)->isHomo()!=0 ){
						// for homogeneous states (easier)
						if( order > 1 )
							ra *= (*acpd)->partialConc[order-1];
						else
							ra *= (*acpd)->prevConc;
					}
					else {
						// for heterogeneous states
					}
				}
				// store the calculated velocity in the step
				(*astep)->velocityPlus[order] = ra;

				// calculate the reverse rate
				ra = (*astep)->kMinus();
				for( acpd = (*astep)->productList().begin(); acpd != (*astep)->productList().end(); acpd++ )
				{
					if( (*acpd)->isHomo() ){
						// for homogeneous states (easier)
						if( order > 1 )
							ra *= (*acpd)->partialConc[order-1];
						else
							ra *= (*acpd)->prevConc;
					}
					else {
						// for heterogeneous states
					}
				}
				// store the calculated velocity in the step
				(*astep)->velocityMinus[order] = ra;

			} // end each step loop

			// after determining rates, calculate concentrations
			for( acpd = CpdList.constBegin(); acpd != CpdList.constEnd(); acpd++ )
			{
				// calculate rate for each specie
				double ra = 0;
				for( QMap<Step*,int>::const_iterator i = (*acpd)->stoiVals.constBegin(); i != (*acpd)->stoiVals.constEnd(); i++ )
				{
					ra += i.value() * ( i.key()->velocityPlus[order] - i.key()->velocityMinus[order]  );
				}
				(*acpd)->rate[order] = ra;

				// calculate a partial concentration
				double ca = 0;
				if( order < maxOrder ){
					for( int i=1; i<=order; i++){
						ca = ca + a[order+1][i-1] * (*acpd)->rate[i];
					}
					ca = ca * dTime + (*acpd)->prevConc;
					(*acpd)->partialConc[order] = ca;
				}
				
				// sum up the final concentration
				if( ca > 1000000000.0 ) overflow = true;
				(*acpd)->finalConc += dTime * b[order] * (*acpd)->rate[order];
				
			} // end for each cpd
		} // end order loop

		// autostep mechanism here
		if( autostep ){
			for( acpd=CpdList.begin(); acpd!=CpdList.end(); acpd++ ){
				// refine timestep
				if( ((*acpd)->prevConc < (*acpd)->threshold() || (*acpd)->finalConc < (*acpd)->threshold())
					&& abs((*acpd)->finalConc - (*acpd)->prevConc) > gateband * (*acpd)->threshold()
					&& exp < maxreduce
				){
					exp++;
					shiftCount = 0;
					goto DownStep;
				}
				
				// enlarge timestep
				if( exp > 0 && shiftCount >= shifttest){
					exp--;
					shiftCount = 0;
				}
			}
		}
		
		// print final concentrations to file
		if( time - lastReport >= reportStep ){
			iomgr->printData(time);
			lastReport += timeStep;
		}
		
	} // time loop

	/////////////////////////////////////////////////////////
	//                END CALCULATION LOOP                 //
	/////////////////////////////////////////////////////////

	// print the mech summary to the data file
	iomgr->data << endl;
	iomgr->printMechSummary( iomgr->data );

	// close the appropriate files
	iomgr->data.device()->close();
	iomgr->log.device()->close();

	return true;
}

double Mix::hBal(Cpd* acpd)
{
	return 0.0;
}

bool Mix::setCalcConstants()
{
	QList<QStringList> methods = availableMethods();
	
	switch(order){
	case 1:
		// euler
		if( method == methods.at(0).at(0) ){
			b[1] = 1; a[1][0] = 1;
		}
		else return false;
		break;
	case 2:
		// modified euler
		if( method == methods.at(1).at(0) ){
			b[1] = 0; a[1][0] = 1;
			b[2] = 1; a[2][0] = 1; a[2][1] = 1/2;
		}
		// heun
		else if( method == methods.at(1).at(1) ){
			b[1] = 1/2; a[1][0] = 1;
			b[2] = 1/2; a[2][0] = 1; a[2][1] = 1;
		}
		// ralston
		else if( method == methods.at(1).at(2) ){
			b[1] = 1/3; a[1][0] = 1; 
			b[2] = 2/3; a[2][0] = 1; a[2][1] = 3/4;
		}
		else return false;
		break;
	case 3:
		// runge-kutta
		if( method == methods.at(2).at(0) ){
			b[1] = 1/6; a[1][0] = 1; 
			b[2] = 2/3; a[2][0] = 1; a[2][1] = 1/2;
			b[3] = 1/6; a[3][0] = 1; a[3][1] = -1; a[3][2] = 2;
		}
		else return false;
		break;
	case 4:
		// runge
		if( method == methods.at(3).at(0) ){
			b[1] = 1/6; a[1][0] = 1;
			b[2] = 1/3; a[2][0] = 1; a[2][1] = 1/2;
			b[3] = 1/3; a[3][0] = 1; a[3][1] = 0; a[3][2] = 1/2;
			b[4] = 1/6; a[4][0] = 1; a[4][1] = 0; a[4][2] = 0; a[4][3] = 1;
		}
		// kutta
		else if( method == methods.at(3).at(1) ){
			b[1] = 1/8; a[1][0] = 1;
			b[2] = 3/8; a[2][0] = 1; a[2][1] = 1/3;
			b[3] = 3/8; a[3][0] = 1; a[3][1] =-1/3; a[3][2] = 1;
			b[4] = 1/8; a[4][0] = 1; a[4][1] = 1;   a[4][2] =-1; a[4][3] = 1;
		}
		// gill
		else if( method == methods.at(3).at(2) ){
			b[1] = 1/6; a[1][0] = 1;
			b[2] = (2-sqrt(2))/6; a[2][0] = 1; a[2][1] = 1/2;
			b[3] = (2+sqrt(2))/6; a[3][0] = 1; a[3][1] = (sqrt(2)-1)/2; a[3][2] = (2-sqrt(2))/2;
			b[4] = 1/6; a[4][0] = 1; a[4][1] = 0; a[4][2] = -sqrt(2)/2; a[4][3] = (2+sqrt(2))/2;
		}
		else return false;
		break;
	default:
		return false;
	}
	
	return true;
}
