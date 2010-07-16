#include "iomanager.h"

IOManager *iomgr = new IOManager();

IOManager::IOManager()
{}

bool IOManager::saveToCM3(QString mech, QString sim)
{
	/***************** write to the mechanism file *****************/
	
	//open the mechanism file
	QFile mfile(mech);
	if( !mfile.open( QFile::WriteOnly ) ){
		status = FS_ERROR;
		message = "Error opening file "+mech;
		return false;
	}

	//print header
	QTextStream mOut( &mfile );
	mOut << "'Generated by CheMecher v4 on ";
	mOut << QDateTime::currentDateTime().toString("ddd MMMM d yyyy hh:mm ap") << endl;
	mOut << "'Created in conjunction with simulation: "+QFileInfo(sim).fileName() << endl;
	mOut << "'Name: " + (mix->mechName.isEmpty() ? "<empty>" : mix->mechName) << endl;
	mOut << "'Desc: " + (mix->mechDesc.isEmpty() ? "<empty>" : mix->mechDesc) << endl;
	mOut << endl;
	
	//write out the cpd list
	mOut << "'compound list" << endl;
	mOut << mix->CpdList.size() << " 'number of compounds" << endl;
	for(int i=0; i<mix->CpdList.size(); i++)
		mOut << mix->CpdList[i]->tov3String() << endl;
	mOut << endl;

	//write out the step list
	mOut << "'step list" << endl;
	mOut << mix->StepList.size() << " 'number of steps" << endl;
	for(int i=0; i<mix->StepList.size(); i++)
		mOut << mix->StepList[i]->tov3String() << endl;
	mOut << endl;

	mfile.close();

	/****************** write to the simulation file *****************/

	//open the simulation file
	QFile sfile(sim);
	if( !sfile.open( QFile::WriteOnly ) ){
		status = FS_ERROR;
		message = "Error opening file "+sim;
		return false;
	}

	//print header
	QTextStream sOut( &sfile );
	sOut << "'Generated by CheMecher v4 on ";
	sOut << QDateTime::currentDateTime().toString("ddd MMMM d yyyy hh:mm ap") << endl;
	sOut << "'Created in conjunction with mechanism: "+QFileInfo(mech).fileName() << endl;
	sOut << endl;

	//output parameters
	sOut << "calcstep=" << mix->timeStep << endl;
	sOut << "reportstep=" << mix->reportStep << endl;
	sOut << "maxtime=" << mix->endTime-mix->startTime
		<< "   'from "<<mix->startTime<<" to "<<mix->endTime << endl;
	sOut << "order=" << mix->order << endl;
	sOut << "method=" << mix->method << endl;
	sOut << "transition=" << mix->transition << endl;
	sOut << endl;
	sOut << "autostep=" << (mix->autostep ? "yes" : "no") << endl;
	sOut << "gateband=" << (mix->autostep ? QString::number(mix->gateband) : "0") << endl;
	sOut << "shifttest=" << (mix->autostep ? QString::number(mix->shifttest) : "0") << endl;
	sOut << "maxreduce=" << (mix->autostep ? QString::number(mix->maxreduce) : "0") << endl;
	sOut << "stepfactor=" << (mix->autostep ? QString::number(mix->stepfactor) : "0") << endl;
	sOut << endl;

	//output the initial concentrations
	sOut << "'initial concentrations" << endl;
	sOut << "'any species not listed are assumed to have an initial concentration of zero" << endl;
	for(int i=0; i<mix->CpdList.size(); i++)
		sOut << mix->CpdList[i]->toString()
		<< QString().fill(' ',mix->maxCpdIdLen()+1-mix->CpdList[i]->toString().length())
		<< mix->CpdList[i]->initialConc() << endl;

	//done writing, close file
	sfile.close();

	return true;
}

// function declaration
QString getLine(QTextStream&,int &);

bool IOManager::loadFromCM3(QString sim, QString mech)
{
	Mix newmix;
	QFile sfile(sim), mfile(mech);

	/********************* start parsing the mech file ********************/

	//open the mech file
	if( !mfile.open( QFile::WriteOnly ) ){
		status = FS_ERROR;
		message = "Error opening file "+mech;
		return false;
	}

	//start reading
	QTextStream min(&mfile);
	int linecounter = 0;
	QString buffer;
	bool ok = true;

	//read the number of species
	buffer = getLine( min, linecounter );
	int numspecies = buffer.toInt(&ok, 10);
	if( !ok || numspecies<1){
		status = PARSE_ERROR;
		message = "Error line "+QString::number(linecounter)+": positive integer expected";
		return false;
	}

	//read the species
	for(int i=0; i<numspecies; i++)
	{

	}

	return true;
}

// gets the next line of valid input
QString getLine(QTextStream& txt, int &linecounter)
{
	QString ret = "";
	while( ret=="" && !txt.atEnd() ) {
		ret = txt.readLine();
		ret = ret.left( ret.indexOf("'") ).simplified();
		linecounter++;
	}
	return ret;
}

// save the currently loaded mechanism in XML format
bool IOManager::saveToCM4(QString filename)
{
	//open the simulation file
	QFile sfile(filename);
	if( !sfile.open( QFile::WriteOnly ) ){
		status = FS_ERROR;
		message = "Error opening file "+filename;
		return false;
	}
	
	QString *fcontents = new QString();
	
	/************* Open the file, write headers ************/
	QXmlStreamWriter stream(fcontents);
	stream.setAutoFormatting(true);
	stream.writeStartDocument();
	
	stream.writeStartElement("Mechanism");
	stream.writeAttribute("name", mix->mechName);
	stream.writeTextElement("MechDescription", mix->mechDesc);
	
	int i;
	
	/************ Write the Species List ***************/
	stream.writeStartElement("CpdList");
	for(i=0; i<mix->CpdList.size(); i++)
	{
		stream.writeStartElement("Cpd");
		stream.writeAttribute("name", mix->CpdList[i]->shortName());
		stream.writeAttribute("state", Cpd::STATES[mix->CpdList[i]->state()]);
		stream.writeEmptyElement("LongName");
		stream.writeAttribute("value", mix->CpdList[i]->longName());
		stream.writeEmptyElement("Concentration");
		stream.writeAttribute("value", QString::number(mix->CpdList[i]->initialConc()));
		stream.writeEmptyElement("Threshold");
		stream.writeAttribute("value", QString::number(mix->CpdList[i]->threshold()));
		stream.writeEmptyElement("Sharpness");
		stream.writeAttribute("value", QString::number(mix->CpdList[i]->sharpness()));
		stream.writeEmptyElement("Transition");
		stream.writeAttribute("value", Cpd::TRANS[mix->CpdList[i]->transition()]);
		stream.writeEndElement(); //Cpd
	}
	stream.writeEndElement(); //CpdList
	
	/******************* Write the Step List ******************/
	stream.writeStartElement("StepList");
	for(i=0; i<mix->StepList.size(); i++)
	{
		stream.writeStartElement("Step");
		stream.writeAttribute("name", mix->StepList[i]->name());
		stream.writeTextElement("Description", mix->StepList[i]->desc());
		stream.writeEmptyElement("RatePlus");
		stream.writeAttribute("value", QString::number(mix->StepList[i]->kPlus()));
		stream.writeEmptyElement("RateMinus");
		stream.writeAttribute("value", QString::number(mix->StepList[i]->kMinus()));
		
		//write the reagents of the step
		int j;
		stream.writeStartElement("ReactantList");
		for(j=0; j<mix->StepList[i]->reactantList().size(); j++){
			stream.writeEmptyElement("Reactant");
			stream.writeAttribute("id", mix->StepList[i]->reactantList()[j]->toString());
		}
		stream.writeEndElement(); //ReactantList
		
		stream.writeStartElement("ProductList");
		for(j=0; j<mix->StepList[i]->productList().size(); j++){
			stream.writeEmptyElement("Product");
			stream.writeAttribute("id", mix->StepList[i]->productList()[j]->toString());
		}
		stream.writeEndElement(); //ProductList
		
		stream.writeEndElement(); //Step
	}
	stream.writeEndElement(); //StepList
	
	/************** Start writing the simparams **************/
	stream.writeStartElement("SimParams");

	stream.writeEmptyElement("TimeStep");
	stream.writeAttribute("value", QString::number(mix->timeStep));
	stream.writeEmptyElement("ReportStep");
	stream.writeAttribute("value", QString::number(mix->reportStep));
	stream.writeEmptyElement("StartTime");
	stream.writeAttribute("value", QString::number(mix->startTime));
	stream.writeEmptyElement("EndTime");
	stream.writeAttribute("value", QString::number(mix->endTime));
	stream.writeEmptyElement("DebugStart");
	stream.writeAttribute("value", QString::number(mix->debugStart));
	stream.writeEmptyElement("DebugEnd");
	stream.writeAttribute("value", QString::number(mix->debugEnd));
	stream.writeEmptyElement("Precision");
	stream.writeAttribute("value", QString::number(mix->precision));
	stream.writeEmptyElement("Order");
	stream.writeAttribute("value", QString::number(mix->order));
	stream.writeEmptyElement("Method");
	stream.writeAttribute("value", mix->method);
	stream.writeEmptyElement("Transition");
	stream.writeAttribute("value", mix->transition);
	stream.writeEmptyElement("Autostep");
	stream.writeAttribute("value", (mix->autostep ? "true" : "false"));
	stream.writeEmptyElement("Gateband");
	stream.writeAttribute("value", QString::number(mix->gateband));
	stream.writeEmptyElement("ShiftTest");
	stream.writeAttribute("value", QString::number(mix->shifttest));
	stream.writeEmptyElement("MaxReduce");
	stream.writeAttribute("value", QString::number(mix->maxreduce));
	stream.writeEmptyElement("StepFactor");
	stream.writeAttribute("value", QString::number(mix->stepfactor));

	stream.writeEndElement(); //SimParams
	
	
	stream.writeEndElement(); //Mechanism
	
	//finish writing
	stream.writeEndDocument();
	
	// output to the file
	sfile.write( fcontents->toAscii() );
	sfile.close();
	return true;
}

bool IOManager::loadFromCM4(QString filename)
{
	
	return false;
}

