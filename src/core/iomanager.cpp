#include "iomanager.h"

IOManager *iomgr = new IOManager();

IOManager::IOManager()
{}

bool IOManager::saveToCM3(QString mech, QString sim)
{
	/***************** write to the mechanism file *****************/
	
	//open the mechanism file
	QFile mfile(mech);
	if( !mfile.open( QFile::WriteOnly ) )
		return setError( FS_ERROR, "Opening file "+mech );

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
		setError( FS_ERROR, "Opening file "+sim );
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

bool IOManager::loadFromCM3(QString mech, QString sim)
{
	Mix newmix;
	QFile mfile(mech), sfile(sim);

	/********************* start parsing the mech file ********************/

	//open the mech file
	if( !mfile.open( QIODevice::ReadOnly | QIODevice::Text ) )
		return setError( FS_ERROR, "Opening file "+mech );
	
	//start reading
	QTextStream min(&mfile);
	int linecounter = 0;
	QString buffer;
	bool ok = true;

	buffer = min.readLine();
	linecounter++;
	while( buffer.left( buffer.indexOf("'") ).simplified() == "" ) {
		
		if( buffer.left(6).toLower() == "'name:" ) {
			newmix.mechName = buffer.mid(6).trimmed();
		}
		if( buffer.left(6).toLower() == "'desc:" ) {
			newmix.mechDesc = buffer.mid(6).trimmed();
		}
		buffer = min.readLine();
		linecounter++;
	}
	
	buffer = buffer.left( buffer.indexOf("'") ).simplified();
	
	//read the number of species
	//buffer = getLine( min, linecounter ); // this is already done above
	int numcpds = buffer.toInt(&ok);
	if( !ok || numcpds<1)
		return setError( PARSE_ERROR, "Positive integer expected", linecounter );
	
	//read the species
	QStringList parts;
	for(int i=0; i<numcpds; i++)
	{
		parts = getLine( min, linecounter ).split(' ');
		if( parts.length()!=4 )
			return setError( PARSE_ERROR, "Wrong number of elements", linecounter );
		Cpd *cpd = new Cpd();
		
		if( parts[0].length()>6 )
			return setError( PARSE_ERROR, "Short name too long (must be no more than 7 characters)", linecounter );
		cpd->setShortName( parts[0] );
		cpd->setState( parts[1], ok );
		if( !ok )
			return setError( PARSE_ERROR, "Invalid state identifier \"" + parts[1] + "\"", linecounter );
		cpd->setThreshold( parts[2].toDouble(&ok) );
		if( !ok || cpd->threshold()<0 )
			return setError( PARSE_ERROR, "Positive double expected in 3rd field", linecounter );
		cpd->setSharpness( parts[3].toDouble(&ok) );
		if( !ok || cpd->sharpness()<0 )
			return setError( PARSE_ERROR, "Positive double expected in 4th field", linecounter );
		
		newmix.addCpd( cpd );
	}
	
	// read the number of steps
	buffer = getLine( min, linecounter );
	int numsteps = buffer.toInt(&ok);
	if( !ok || numsteps<1)
		return setError( PARSE_ERROR, "Positive integer expected", linecounter );
	
	// read the steps
	for(int i=0; i<numsteps; i++)
	{
		// get the line and split it
		parts = getLine( min, linecounter ).split(' ');
		if( parts.length()!=3 )
			return setError( PARSE_ERROR, "Expected three elements", linecounter );
		
		// declare and name the new step
		Step *step = new Step();
		step->setName( QString("Step %1").arg(i+1) );
		
		// get the lists of reactants and products
		if( parts[0].split(">").length()!=2 )
			return setError( PARSE_ERROR, "Improperly formed step", linecounter );
		QStringList reactants = parts[0].split(">")[0].split("+");
		QStringList products  = parts[0].split(">")[1].split("+");
		
		if( reactants.length()>3 || products.length()>3 )
			return setError( PARSE_ERROR, "No more than three reactants or products allowed", linecounter );
		
		// add the reactants and products
		for( int i=0; i<reactants.size(); i++ ) {
			if( !step->addReactant( newmix.getCpdById(reactants[i]) ) )
				return setError( PARSE_ERROR, "Undefined species \"" + reactants[i] + "\"", linecounter );
		}
		for( int i=0; i<products.size(); i++ ) {
			if( !step->addProduct( newmix.getCpdById(products[i]) ) )
				return setError( PARSE_ERROR, "Undefined species \"" + reactants[i] + "\"", linecounter );
		}
		
		step->setKPlus( parts[1].toDouble(&ok) );
		if( !ok || step->kPlus()<0 )
			return setError( PARSE_ERROR, "Positive double expected in 2rd field", linecounter );
		step->setKMinus( parts[2].toDouble(&ok) );
		if( !ok || step->kPlus()<0 )
			return setError( PARSE_ERROR, "Positive double expected in 3rd field", linecounter );
		
		newmix.addStep( step );
	}
	
	mix->clone( &newmix );
	status = LOADED_CM3;
	return true;
}

// gets the next line of valid input
QString IOManager::getLine(QTextStream& txt, int &linecounter)
{
	QString ret = "";
	while( ret=="" && !txt.atEnd() ) {
		ret = txt.readLine();
		// removes comments and simplifies whitespace
		ret = ret.left( ret.indexOf("'") ).simplified();
		linecounter++;
	}
	return ret;
}

// always returns false so that an error can be set with a single statement:
//    return setError( <stat>, <errmsg>, <linenum> );
bool IOManager::setError( Status stat, QString errmsg, int linenum )
{
	status = stat;
	message = "Error";
	if( linenum>=0 )
		message += " (" + QString::number(linenum) + ")";
	message += ": " + errmsg;
	return false;
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
	Mix newmix;

	//open the simulation file
	QFile sfile(filename);
	if( !sfile.open( QFile::ReadOnly ) ){
		status = FS_ERROR;
		message = "Error opening file "+filename;
		return false;
	}

	/*
	  The doc.setContent() function actually loads the file into a tree structure composed of QDomNodes.
	*/
	QDomDocument doc("mech");
	if( !doc.setContent(&sfile, &message) ){
		sfile.close();
		setError(PARSE_ERROR, "Could not parse document");
		return false;
	}

	QDomElement ele = doc.documentElement();
	if( ele.tagName()!="Mechanism"){
		setError(PARSE_ERROR, "Expected \"Mechanism\"");
		return false;
	}
	newmix.mechName = ele.attribute("name");
	if( ele.elementsByTagName("MechDescription").isEmpty() ){
		setError(PARSE_ERROR, "Expected \"Mechanism\"");
		return false;
	}
	newmix.mechDesc = ele.elementsByTagName("MechDescription").at(0).toElement().text();

	/************** Read the species ***************/
	if( ele.elementsByTagName("CpdList").isEmpty()){
		setError(PARSE_ERROR, "Expected \"CpdList\"");
		return false;
	}
	QDomNode cpdlist = ele.elementsByTagName("CpdList").at(0);
	for(QDomNode cpd=cpdlist.firstChild(); !cpd.isNull(); cpd=cpd.nextSibling())
	{
		// parse basic information
		Cpd* newcpd = new Cpd(); bool ok;
		newcpd->setShortName(cpd.toElement().attribute("name"));
		newcpd->setState(cpd.toElement().attribute("state"), ok);
		QDomNode child = cpd.firstChild();

		// parse components of a compound
		if(child.toElement().tagName()!="LongName"){
			setError(PARSE_ERROR, "Expected \"LongName\", got \""+child.toElement().tagName()+"\"");
			return false;
		}
		newcpd->setLongName(child.toElement().attribute("value"));
		child = child.nextSibling();
		if(child.toElement().tagName()!="Concentration"){
			setError(PARSE_ERROR, "Expected \"Concentration\", got \""+child.toElement().tagName()+"\"");
			return false;
		}
		newcpd->setInitialConc(child.toElement().attribute("value"));
		child = child.nextSibling();
		if(child.toElement().tagName()!="Threshold"){
			setError(PARSE_ERROR, "Expected \"Threshold\", got \""+child.toElement().tagName()+"\"");
			return false;
		}
		newcpd->setThreshold(child.toElement().attribute("value").toDouble());
		child = child.nextSibling();
		if(child.toElement().tagName()!="Sharpness"){
			setError(PARSE_ERROR, "Expected \"Sharpness\", got \""+child.toElement().tagName()+"\"");
			return false;
		}
		newcpd->setSharpness(child.toElement().attribute("value").toDouble());
		child = child.nextSibling();
		if(child.toElement().tagName()!="Transition"){
			setError(PARSE_ERROR, "Expected \"Transition\", got \""+child.toElement().tagName()+"\"");
			return false;
		}
		newcpd->setTransition(child.toElement().attribute("value"));

		// add the new cpd to the mix
		newmix.CpdList.append(newcpd);
	}

	/****************** Read the Step List *********************/
	if( ele.elementsByTagName("StepList").isEmpty() ){
		setError(PARSE_ERROR, "Expected \"StepList\"");
		return false;
	}
	QDomNode steplist = ele.elementsByTagName("StepList").at(0);
	for(QDomNode step=steplist.firstChild(); !step.isNull(); step=step.nextSibling())
	{

	}

	mix->clone(&newmix);
	setError(LOADED_CM4, "Loaded successfully");
	return true;
}

