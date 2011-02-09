#include "iomanager.h"

IOManager *iomgr = new IOManager();

IOManager::IOManager()
{
	outputPrecision = 6;
	timePrecision = 3;
}
IOManager::~IOManager()
{
	data.device()->close();
}

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
	if( !mix->mechName.isEmpty() )
		mOut << "'Name: " + mix->mechName << endl;
	if( !mix->mechDesc.isEmpty() )
		mOut << "'Desc: " + mix->mechDesc << endl;
	mOut << endl;
	
	//write out the cpd list
	mOut << "'compound list" << endl;
	mOut << mix->CpdList.size() << " 'number of compounds" << endl;
	
	QStringList cpdlist;
	cpdlist.append("'name state thresh sharp");
	for(int i=0; i<mix->CpdList.size(); i++)
		cpdlist.append( mix->CpdList[i]->tov3String() );
	
	// line up the whitespace in the cpd list
	lineUpWhitespace( cpdlist, 4 );
	
	// output it
	for( int i=0; i<cpdlist.size(); i++ )
		mOut << cpdlist[i] << endl;
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
	sOut << "order=" << (mix->order>4 ? 4 : mix->order) << endl;
	sOut << "method=" << mix->method << endl;
	sOut << "transition=" << mix->transition << endl;
	sOut << endl;
	sOut << "autostep=" << (mix->autostep ? "yes" : "no") << endl;
	sOut << "gateband=" << QString::number(mix->gateband) << endl;
	sOut << "shifttest=" << QString::number(mix->shifttest) << endl;
	sOut << "maxreduce=" << QString::number(mix->maxreduce) << endl;
	sOut << "stepfactor=" << QString::number(mix->stepfactor) << endl;
	sOut << endl;

	//output the initial concentrations
	sOut << "'initial concentrations" << endl;
	sOut << "'any species not listed are assumed to have an initial concentration of zero" << endl;
	for(int i=0; i<mix->CpdList.size(); i++) {
		if(mix->CpdList[i]->initialConc() == 0) continue;
		sOut << mix->CpdList[i]->toString()
		<< QString().fill(' ',mix->maxCpdIdLen()+1-mix->CpdList[i]->toString().length())
		<< mix->CpdList[i]->initialConc() << endl;
	}

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

	// try to parse the name and description from the file
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
		return setError( PARSE_ERROR, "Positive integer expected", linecounter, QFileInfo(mech).fileName() );
	
	//read the species
	QStringList parts;
	for(int i=0; i<numcpds; i++)
	{
		parts = getLine( min, linecounter ).split(' ');
		if( parts.length()!=4 )
			return setError( PARSE_ERROR, "Wrong number of elements", linecounter, QFileInfo(mech).fileName() );
		Cpd *cpd = new Cpd();
		
		if( parts[0].length()>6 )
			return setError( PARSE_ERROR, "Short name too long (must be no more than 7 characters)", linecounter, QFileInfo(mech).fileName() );
		cpd->setShortName( parts[0] );
		cpd->setState( parts[1], &ok );
		if( !ok )
			return setError( PARSE_ERROR, "Invalid state identifier \"" + parts[1] + "\"", linecounter, QFileInfo(mech).fileName() );
		cpd->setThreshold( parts[2].toDouble(&ok) );
		if( !ok || cpd->threshold()<0 )
			return setError( PARSE_ERROR, "Positive double expected in 3rd field", linecounter, QFileInfo(mech).fileName() );
		cpd->setSharpness( parts[3].toDouble(&ok) );
		if( !ok || cpd->sharpness()<0 )
			return setError( PARSE_ERROR, "Positive double expected in 4th field", linecounter, QFileInfo(mech).fileName() );
		
		newmix.addCpd( cpd );
	}
	
	// read the number of steps
	buffer = getLine( min, linecounter );
	int numsteps = buffer.toInt(&ok);
	if( !ok || numsteps<1)
		return setError( PARSE_ERROR, "Positive integer expected", linecounter, QFileInfo(mech).fileName() );
	
	// read the steps
	for(int i=0; i<numsteps; i++)
	{
		// get the line
		buffer = getLine( min, linecounter, false );			
		
		// split the line
		parts = buffer.left( buffer.indexOf("'") ).simplified().split(' ');
		if( parts.length()!=3 )
			return setError( PARSE_ERROR, "Expected three elements", linecounter, QFileInfo(mech).fileName() );
		
		// declare and name the new step
		Step *step = new Step();
		if( buffer.mid( buffer.indexOf("'"), 6 ) == "'name:" )
			step->setName( buffer.mid(buffer.indexOf("'")+6).trimmed() );
		else
			step->setName( QString("Step %1").arg(i+1) );
		
		// get the lists of reactants and products
		if( parts[0].split(">").length()!=2 )
			return setError( PARSE_ERROR, "Improperly formed step", linecounter, QFileInfo(mech).fileName() );
		QStringList reactants = parts[0].split(">")[0].split("+");
		QStringList products  = parts[0].split(">")[1].split("+");
		
		if( reactants.length()>3 || products.length()>3 )
			return setError( PARSE_ERROR, "No more than three reactants or products allowed", linecounter, QFileInfo(mech).fileName() );
		
		// add the reactants and products
		for( int i=0; i<reactants.size(); i++ ) {
			if( !step->addReactant( newmix.getCpdById(reactants[i]) ) )
				return setError( PARSE_ERROR, "Undefined species \"" + reactants[i] + "\"", linecounter, QFileInfo(mech).fileName() );
		}
		for( int i=0; i<products.size(); i++ ) {
			if( !step->addProduct( newmix.getCpdById(products[i]) ) )
				return setError( PARSE_ERROR, "Undefined species \"" + reactants[i] + "\"", linecounter, QFileInfo(mech).fileName() );
		}
		
		step->setKPlus( parts[1].toDouble(&ok) );
		if( !ok || step->kPlus()<0 )
			return setError( PARSE_ERROR, "Positive double expected in 2rd field", linecounter, QFileInfo(mech).fileName() );
		step->setKMinus( parts[2].toDouble(&ok) );
		if( !ok || step->kPlus()<0 )
			return setError( PARSE_ERROR, "Positive double expected in 3rd field", linecounter, QFileInfo(mech).fileName() );
		
		newmix.addStep( step );
	}
	
	//// Parse the simulation file ////
	
	//open the sim file
	if( !sfile.open( QIODevice::ReadOnly | QIODevice::Text ) )
		return setError( FS_ERROR, "Opening file "+sim );
	
	//start reading
	QTextStream sin(&sfile);
	linecounter = 0;
	buffer = "";
	ok = true;
	
	// stores information about the various fields available in the simulation file
	QMap<QString,SimEntry> ents;
	SimEntry t;
	t.entered = false;
	
	// fill out the information
	t.vtype = v_double;
	t.doubleval = &newmix.timeStep;   ents["calcstep"]   = t;
	t.doubleval = &newmix.reportStep; ents["reportstep"] = t;
	t.doubleval = &newmix.endTime;    ents["maxtime"]    = t;
	t.vtype = v_int;
	t.intval = &newmix.gateband;      ents["gateband"]   = t;
	t.intval = &newmix.shifttest;     ents["shifttest"]  = t;
	t.intval = &newmix.maxreduce;     ents["maxreduce"]  = t;
	t.intval = &newmix.stepfactor;    ents["stepfactor"] = t;
	t.vtype = v_bool;
	t.boolval = &newmix.autostep;     ents["autostep"]   = t;
	t.vtype = v_order;
	t.intval = &newmix.order;         ents["order"]      = t;
	t.vtype = v_method;
	t.stringval = &newmix.method;     ents["method"]     = t;
	t.vtype = v_trans;
	t.stringval = &newmix.transition; ents["transition"] = t;
	
	// get the variables at the beginning
	for( int i=0; i<ents.count(); i++ ) {
		
		// get the key/value pair
		QList<QString> parts = getLine( sin, linecounter ).split("=");
		if( parts.length()!=2 )
			return setError( PARSE_ERROR, "Invalid format, expected key=value", linecounter, QFileInfo(sim).fileName() );
		
		// declare variables
		QString k = parts[0].toLower().trimmed();
		QString v = parts[1].toLower().trimmed();
		double vd; int vi; bool vb;
		
		// check that identifier exists and has not been encountered
		if( !ents.contains(k) )
			return setError( PARSE_ERROR, "Unrecognized identifier \"" + k + "\"", linecounter, QFileInfo(sim).fileName() );
		if( ents[k].entered )
			return setError( PARSE_ERROR, "Duplicate identifier \"" + k + "\"", linecounter, QFileInfo(sim).fileName() );
		
		// switch( variable type )
		switch( ents[k].vtype ) {
		
		// if it's a double
		case v_double:
			vd = v.toDouble(&ok);
			if( !ok || vd<0 )
				return setError( PARSE_ERROR, "Expected positive double", linecounter, QFileInfo(sim).fileName() );
			*ents[k].doubleval = vd;
			break;
		
		// if it's an int
		case v_order:
		case v_int:
			vi = v.toInt(&ok);
			
			// if it's an order and it's out of range, report an error
			if( ents[k].vtype==v_order && (vi<1 || vi>4) )
				return setError( PARSE_ERROR, "Expected integer between 1 and 4", linecounter, QFileInfo(sim).fileName() );
			if( vi<0 )
				return setError( PARSE_ERROR, "Expected positive integer", linecounter, QFileInfo(sim).fileName() );
			
			if(ok) *ents[k].intval = vi;
			break;
			
		// if it's a bool
		case v_bool:
			if( v=="yes" || v=="no" )
				vb = (v=="yes");
			else
				return setError( PARSE_ERROR, "Expected either \"yes\" or \"no\"", linecounter, QFileInfo(sim).fileName() );
			*ents[k].boolval = vb;
			break;
		
		// if it's a transition
		case v_trans:
			ok = ( v=="atan" || v=="linear" || v=="none" );
			if( !ok )
				return setError( PARSE_ERROR, "Transition must be either \"atan\", \"linear\", or \"none\"", linecounter, QFileInfo(sim).fileName() );
			else
				*ents[k].stringval = v;
			break;
			
		// if it's a method
		case v_method:
			if( !ents["order"].entered )
				return setError( PARSE_ERROR, "Method must be listed after order", linecounter, QFileInfo(sim).fileName() );
			ok = newmix.availableMethods()[newmix.order-1].contains(v);
			if( !ok )
				return setError( PARSE_ERROR, "For order=" + QString::number(newmix.order) + ", method must be one of the following: \""+newmix.availableMethods()[newmix.order-1].join("\", \"")+"\"", linecounter, QFileInfo(sim).fileName() );
			else
				*ents[k].stringval = v;
			break;
		}
		if( !ok )
			return setError( PARSE_ERROR, "Failed to parse value in key=value pair", linecounter, QFileInfo(sim).fileName() );
		
		// tell the program that this variable has been read
		ents[k].entered = true;
	}
	
	// get the initial concentrations
	buffer = getLine(sin, linecounter);
	while( buffer!="" ) {
		
		// split the line into its parts
		QList<QString> parts = buffer.split(" ");
		if( parts.count()!=2 )
			return setError( PARSE_ERROR, "Expected two elements", linecounter, QFileInfo(sim).fileName() );
		
		// search for the compound
		Cpd *cpd = newmix.getCpdById(parts[0]);
		if( cpd==0 )
			return setError( PARSE_ERROR, "Unrecognized species \"" + parts[0] + "\"", linecounter, QFileInfo(sim).fileName() );
		
		// set the concentration
		double conc = parts[1].toDouble(&ok);
		if( !ok || conc<0 )
			return setError( PARSE_ERROR, "Expected positive double", linecounter, QFileInfo(sim).fileName() );
		cpd->setInitialConc(conc);
		
		// get the next line
		buffer = getLine(sin, linecounter);
	}
	
	mix->clone( &newmix );
	status = LOADED_CM3;
	return true;
}

// save the currently loaded mechanism in XML format
bool IOManager::saveToCM4(QString filename)
{
	//open the simulation file
	QFile sfile(filename);
	if( !sfile.open( QFile::WriteOnly | QFile::Truncate ) )
		return setError( FS_ERROR, "Error opening file "+filename );
	
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
	if( sfile.write( fcontents->toAscii() ) == -1 )
	{
		return setError(FS_ERROR, "Failed to write to file.");
	}
	
	sfile.close();
	return true;
}

bool IOManager::loadFromCM4(QString filename)
{
	Mix newmix;

	//open the simulation file
	QFile sfile(filename);
	if( !sfile.open( QFile::ReadOnly ) )
		return setError( FS_ERROR, "Error opening file "+filename );

	/*
	  The doc.setContent() function actually loads the file into a tree structure composed of QDomNodes.
	*/
	QDomDocument doc("mech");
	if( !doc.setContent(&sfile, &message) ){
		sfile.close();
		return setError(PARSE_ERROR, "Could not parse document");
	}

	QDomElement ele = doc.documentElement();
	if( ele.tagName()!="Mechanism")
		return setError(PARSE_ERROR, "Expected \"Mechanism\"", ele.lineNumber());
	newmix.mechName = ele.attribute("name");
	if( ele.elementsByTagName("MechDescription").isEmpty() )
		return setError(PARSE_ERROR, "Expected \"MechDescription\"");
	newmix.mechDesc = ele.elementsByTagName("MechDescription").at(0).toElement().text();

	/************** Read the species ***************/
	if( ele.elementsByTagName("CpdList").isEmpty())
		return setError(PARSE_ERROR, "Expected \"CpdList\"");
	QDomNode cpdlist = ele.elementsByTagName("CpdList").at(0);
	for(QDomNode cpd=cpdlist.firstChild(); !cpd.isNull(); cpd=cpd.nextSibling())
	{
		if( cpd.toElement().tagName()!="Cpd" )
			return setError(PARSE_ERROR, "Expected \"Cpd\", got \""+cpd.toElement().tagName()+"\"");
		
		// parse basic information
		Cpd* newcpd = new Cpd(); bool ok;
		newcpd->setShortName(cpd.toElement().attribute("name"));
		newcpd->setState(cpd.toElement().attribute("state"), &ok);
		if(!ok) return setError(ERROR, "Invalid state: "+cpd.toElement().attribute("state"), cpd.lineNumber());
		QDomNode child = cpd.firstChild();

		// parse components of a compound
		if(child.toElement().tagName()!="LongName")
			return setError(PARSE_ERROR, "Expected \"LongName\", got \""+cpd.toElement().tagName()+"\"", child.lineNumber());
		newcpd->setLongName(child.toElement().attribute("value"));
		child = child.nextSibling();
		if(child.toElement().tagName()!="Concentration")
			return setError(PARSE_ERROR, "Expected \"Concentration\", got \""+child.toElement().tagName()+"\"", child.lineNumber());
		newcpd->setInitialConc(child.toElement().attribute("value"));
		child = child.nextSibling();
		if(child.toElement().tagName()!="Threshold")
			return setError(PARSE_ERROR, "Expected \"Threshold\", got \""+child.toElement().tagName()+"\"", child.lineNumber());
		newcpd->setThreshold(child.toElement().attribute("value").toDouble());
		child = child.nextSibling();
		if(child.toElement().tagName()!="Sharpness")
			return setError(PARSE_ERROR, "Expected \"Sharpness\", got \""+child.toElement().tagName()+"\"", child.lineNumber());
		newcpd->setSharpness(child.toElement().attribute("value").toDouble());
		child = child.nextSibling();
		if(child.toElement().tagName()!="Transition")
			return setError(PARSE_ERROR, "Expected \"Transition\", got \""+child.toElement().tagName()+"\"", child.lineNumber());
		newcpd->setTransition(child.toElement().attribute("value"));

		// add the new cpd to the mix
		newmix.CpdList.append(newcpd);
	}

	/****************** Read the Step List *********************/
	if( ele.elementsByTagName("StepList").isEmpty() )
		return setError(PARSE_ERROR, "Expected \"StepList\"");
	QDomNode steplist = ele.elementsByTagName("StepList").at(0);
	for(QDomNode step=steplist.firstChild(); !step.isNull(); step=step.nextSibling())
	{
		if( step.toElement().tagName()!="Step" )
			return setError(PARSE_ERROR, "Expected \"Step\", got \""+step.toElement().tagName()+"\"", step.lineNumber());
		
		// parse essential data
		Step* newstep = new Step();
		newstep->setName( step.toElement().attribute("name") );
		
		// parse properties of the step
		QDomNode child = step.firstChild();
		if( child.toElement().tagName() != "Description" )
			return setError(PARSE_ERROR, "Expected \"Description\", got \""+child.toElement().tagName()+"\"", child.lineNumber());
		newstep->setDesc( child.toElement().text() );
		child = child.nextSibling();
		if( child.toElement().tagName() != "RatePlus" )
			return setError(PARSE_ERROR, "Expected \"RatePlus\", got \""+child.toElement().tagName()+"\"", child.lineNumber());
		newstep->setKPlus( child.toElement().attribute("value") );
		child = child.nextSibling();
		if( child.toElement().tagName() != "RateMinus" )
			return setError(PARSE_ERROR, "Expected \"RateMinus\", got \""+child.toElement().tagName()+"\"", child.lineNumber());
		newstep->setKMinus( child.toElement().attribute("value") );
		child = child.nextSibling();
		
		// parse the reactant list
		if( child.toElement().tagName() != "ReactantList" )
			return setError(PARSE_ERROR, "Expected \"ReactantList\", got \""+child.toElement().tagName()+"\"", child.lineNumber());
		for(QDomNode reactant=child.firstChild(); !reactant.isNull(); reactant=reactant.nextSibling())
		{
			if( reactant.toElement().tagName() != "Reactant" )
				return setError(PARSE_ERROR, "Expected \"Reactant\", got \""+reactant.toElement().tagName()+"\"", reactant.lineNumber());
			Cpd* temp = newmix.getCpdById( reactant.toElement().attribute("id") );
			if( temp!=NULL ) newstep->addReactant(temp);
			else return setError(ERROR, reactant.toElement().attribute("id")+" not found!", reactant.lineNumber());
		}
		child = child.nextSibling();
		
		// parse the product list
		if( child.toElement().tagName() != "ProductList" )
			return setError(PARSE_ERROR, "Expected \"ProductList\", got \""+child.toElement().tagName()+"\"", child.lineNumber());
		for(QDomNode product=child.firstChild(); !product.isNull(); product=product.nextSibling())
		{
			if( product.toElement().tagName() != "Product" )
				return setError(PARSE_ERROR, "Expected \"Product\", got \""+product.toElement().tagName()+"\"", product.lineNumber());
			Cpd* temp = newmix.getCpdById( product.toElement().attribute("id") );
			if( temp!=NULL ) newstep->addProduct(temp);
			else return setError(ERROR, product.toElement().attribute("id")+" not found!", product.lineNumber());
		}
		child = child.nextSibling();
		
		// lol, forgot to add the step to the mix
		newmix.addStep( newstep );
	}

	/****************** Read the Parameter List *********************/
	if( ele.elementsByTagName("SimParams").isEmpty() )
		return setError(PARSE_ERROR, "Expected \"SimParams\"");
	QDomNode param = ele.elementsByTagName("SimParams").at(0).firstChild();
	
	if( param.toElement().tagName() != "TimeStep" )
		return setError(PARSE_ERROR, "Expected \"TimeStep\", got \""+param.toElement().tagName()+"\"", param.lineNumber());
	newmix.timeStep = param.toElement().attribute("value").toDouble();
	param = param.nextSibling();
	if( param.toElement().tagName() != "ReportStep" )
		return setError(PARSE_ERROR, "Expected \"ReportStep\", got \""+param.toElement().tagName()+"\"", param.lineNumber());
	newmix.reportStep = param.toElement().attribute("value").toDouble();
	param = param.nextSibling();
	if( param.toElement().tagName() != "StartTime" )
		return setError(PARSE_ERROR, "Expected \"StartTime\", got \""+param.toElement().tagName()+"\"", param.lineNumber());
	newmix.startTime = param.toElement().attribute("value").toDouble();
	param = param.nextSibling();
	if( param.toElement().tagName() != "EndTime" )
		return setError(PARSE_ERROR, "Expected \"EndTime\", got \""+param.toElement().tagName()+"\"", param.lineNumber());
	newmix.endTime = param.toElement().attribute("value").toDouble();
	param = param.nextSibling();
	if( param.toElement().tagName() != "DebugStart" )
		return setError(PARSE_ERROR, "Expected \"DebugStart\", got \""+param.toElement().tagName()+"\"", param.lineNumber());
	newmix.debugStart = param.toElement().attribute("value").toDouble();
	param = param.nextSibling();
	if( param.toElement().tagName() != "DebugEnd" )
		return setError(PARSE_ERROR, "Expected \"DebugEnd\", got \""+param.toElement().tagName()+"\"", param.lineNumber());
	newmix.debugEnd = param.toElement().attribute("value").toDouble();
	param = param.nextSibling();
	if( param.toElement().tagName() != "Precision" )
		return setError(PARSE_ERROR, "Expected \"Precision\", got \""+param.toElement().tagName()+"\"", param.lineNumber());
	newmix.precision = param.toElement().attribute("value").toDouble();
	param = param.nextSibling();
	if( param.toElement().tagName() != "Order" )
		return setError(PARSE_ERROR, "Expected \"Order\", got \""+param.toElement().tagName()+"\"", param.lineNumber());
	newmix.order = param.toElement().attribute("value").toInt();
	param = param.nextSibling();
	if( param.toElement().tagName() != "Method" )
		return setError(PARSE_ERROR, "Expected \"Method\", got \""+param.toElement().tagName()+"\"", param.lineNumber());
	newmix.method = param.toElement().attribute("value");
	param = param.nextSibling();
	if( param.toElement().tagName() != "Transition" )
		return setError(PARSE_ERROR, "Expected \"Transition\", got \""+param.toElement().tagName()+"\"", param.lineNumber());
	newmix.transition = param.toElement().attribute("value");
	param = param.nextSibling();
	if( param.toElement().tagName() != "Autostep" )
		return setError(PARSE_ERROR, "Expected \"Autostep\", got \""+param.toElement().tagName()+"\"", param.lineNumber());
	newmix.autostep = param.toElement().attribute("value")=="true";
	param = param.nextSibling();
	if( param.toElement().tagName() != "Gateband" )
		return setError(PARSE_ERROR, "Expected \"Gateband\", got \""+param.toElement().tagName()+"\"", param.lineNumber());
	newmix.gateband = param.toElement().attribute("value").toDouble();
	param = param.nextSibling();
	if( param.toElement().tagName() != "ShiftTest" )
		return setError(PARSE_ERROR, "Expected \"ShiftTest\", got \""+param.toElement().tagName()+"\"", param.lineNumber());
	newmix.shifttest = param.toElement().attribute("value").toInt();
	param = param.nextSibling();
	if( param.toElement().tagName() != "MaxReduce" )
		return setError(PARSE_ERROR, "Expected \"MaxReduce\", got \""+param.toElement().tagName()+"\"", param.lineNumber());
	newmix.maxreduce = param.toElement().attribute("value").toInt();
	param = param.nextSibling();
	if( param.toElement().tagName() != "StepFactor" )
		return setError(PARSE_ERROR, "Expected \"StepFactor\", got \""+param.toElement().tagName()+"\"", param.lineNumber());
	newmix.stepfactor = param.toElement().attribute("value").toDouble();
	param = param.nextSibling();
	
	
	// finally done loading!
	mix->clone(&newmix);
	setError(LOADED_CM4, "Loaded successfully");
	return true;
}

// gets the next line of valid input
QString IOManager::getLine(QTextStream& txt, int &linecounter, bool stripcomments)
{
	QString ret = "";
	while( ret.left(ret.indexOf("'"))=="" && !txt.atEnd() ) {
		ret = txt.readLine();
		// removes comments and simplifies whitespace
		ret = ret.simplified();
		if( stripcomments )
			ret = ret.left( ret.indexOf("'") );
		linecounter++;
	}
	return ret;
}

// always returns false so that an error can be set with a single statement:
//    return setError( <stat>, <errmsg>, <linenum> );
bool IOManager::setError( Status stat, QString errmsg, int linenum, QString filename )
{
	status = stat;
	message = "Error";
	if( linenum>=0 || filename!="" )
		message += " (";
	if( filename!="" )
		message += filename;
	if( filename!="" && linenum>=0 )
		message += ", ";
	if( linenum>=0 )
		message += "line " + QString::number(linenum);
	if( linenum>=0 || filename!="" )
		message += ")";
	message += ": " + errmsg;
	return false;
}

/** lineUpWhitespace
    given a QStringList of lines to be outputted, modifies the
    list to line up the whitespace in the lines
	
     pre:  A (.) 1.94 0
           Bo (.) 0 1.99
           C (aq) 0.023 0
           D (.) 0 0
     post: A  (.)  1.94  0
           Bo (.)  0     1.99
           C  (aq) 0.023 0
           D  (.)  0     0
		 
    Note: may be a bit slow, the user should be allowed to disable this
          Runs in O( lines.count() * numcols ) time
 */
void IOManager::lineUpWhitespace(QList<QString> &lines, int numcols)
{	
	for( int i=0; i<lines.size(); i++ )
		lines[i] = lines[i].simplified();
	
	int max=0, colpos=0;
	QString temp;
	for( int c=0; c<numcols; c++ ) {
		if( c>0 )
			colpos = colpos + max+1;
		max=0;
		// get the length of the longest cth field field
		for( int i=0; i<lines.count(); i++ ) {
			temp = lines[i].split(" ",QString::SkipEmptyParts)[c];
			if( temp.length()>max )
				max = temp.length();
		}
		// insert any needed spaces
		for( int i=0; i<lines.count(); i++ ) {
			temp = lines[i].split(" ",QString::SkipEmptyParts)[c];
			lines[i].insert( colpos + temp.length(), QString(" ").repeated(max-temp.length()) );
		}
	}
}

/*
	The following functions handle the output of runtime data
	from the calculation loop.
*/
bool IOManager::openRunOutputFile()
{
	//open the simulation file
        QFile* out = new QFile(outputFile);
        if( !out->open( QFile::WriteOnly ) )
		return setError( FS_ERROR, "Error opening file "+outputFile );

	// open text buffer
        data.setDevice(out);

	return true;
}

bool IOManager::openDebugOutputFile()
{
	QFile* out = new QFile(debugFile);
	if( !out->open( QFile::WriteOnly ) )
		return setError( FS_ERROR, "Error opening file "+debugFile );

	// set text buffer
	debug.setDevice(out);

	return true;
}

bool IOManager::openLogFile()
{
	QFile* out = new QFile(logFile);
	if( !out->open( QFile::WriteOnly | QFile::Append ) )
		return setError( FS_ERROR, "Error opening file "+logFile );

	log.setDevice(out);

	return true;
}

void IOManager::setTimeFormat(QTextStream& fp)
{
	// set formatting
	fp << reset;
	fp << qSetFieldWidth(6+timePrecision)
		<< qSetPadChar(' ') 
		<< qSetRealNumberPrecision(timePrecision)
		<< right << fixed << forcepoint;

}

void IOManager::setDataFormat(QTextStream& fp)
{
	// set formatting
	fp << reset;
	fp << qSetFieldWidth(6+outputPrecision)
		<< qSetPadChar(' ') 
		<< qSetRealNumberPrecision(outputPrecision)
		<< right << fixed << forcepoint;

}

void IOManager::printMechSummary(QTextStream& ofp)
{
	// output the general information
	ofp << reset << endl
		<< QString("Mechanism %1, %2")
			.arg(mix->mechName).arg(mix->mechDesc) << endl
		<< QString("Run on %1")
			.arg(QDateTime::currentDateTime().toString()) << endl
		<< QString("Generated by CheMecher version %1").arg(qApp->applicationVersion()) << endl
		<< QString("Original Filename: %1").arg(outputFile) << endl;
		
	// interpret fields to natural language
	QString order, method;
	switch(mix->order){
	case 1: order = "1st Order"; break;
	case 2: order = "2nd Order"; break;
	case 3: order = "3rd Order"; break;
	case 4: order = "4th Order"; break;
	case 5: order = "4th Order Embedded"; break;
	default: order = "Invalid Order";
	}
	
	// interpret method word to a good string
	if(mix->method == "euler")               method = "Euler's Method";
	else if(mix->method == "modified euler") method = "Modified Euler's Method";
	else if(mix->method == "heun")           method = "Heun's Method";
	else if(mix->method == "ralston")        method = "Ralston's Method";
	else if(mix->method == "runge-kutta")    method = "Runge-Kutta Method";
	else if(mix->method == "runge")          method = "Runge Method";
	else if(mix->method == "kutta")          method = "Kutta Method";
	else if(mix->method == "gill")           method = "Gill Method";
	else if(mix->order == 5)                 method = "Runge-Kutta-Felhberg Method";
	else method = "";

	ofp << QString("%1, %2").arg(order).arg(method) << endl;
	
	// output the step equations
	for(int i=0; i<mix->StepList.size(); i++){
		ofp << QString("%1\tk+=%2, k-=%3\t// %4")
				.arg(mix->StepList[i]->toString())
				.arg(mix->StepList[i]->kPlus())
				.arg(mix->StepList[i]->kMinus())
				.arg(mix->StepList[i]->name())
			<< endl;
	}
	
	// output the non-zero species concentrations
	ofp << "Initial non-zero concentrations:" << endl;
	for(int i=0; i<mix->CpdList.size(); i++){
		if( mix->CpdList[i]->initialConc() != 0 ){
			ofp << mix->CpdList[i]->toString() << "\t"
				<< mix->CpdList[i]->initialConc() << endl;
		}
	}

}

void IOManager::printData( double curTime )
{
	// print the column headers if need be
	if( curTime == 0 ){
		setTimeFormat(data);
		data << "Time:";
		
		setDataFormat(data);
		for(int i=0; i<mix->CpdList.size(); i++){
			data << mix->CpdList[i]->toString();
		}
		data << endl;
	}

	// print the timestamp for this step
	setTimeFormat(data);
	data << curTime;

	// print the concentrations of all species
	setDataFormat(data);
	for(int i=0; i<mix->CpdList.size(); i++){
		data << mix->CpdList[i]->finalConc;
	}
	data << endl;
}
