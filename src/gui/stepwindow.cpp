#include "stepwindow.h"

StepWindow::StepWindow(Step* base, QWidget* parent, bool isnew)
	: QWidget(parent), baseStep(base)
{
	ui->setupUi(this);
	
	// initialize reagent boxes;
	reactants = new ReagentBox_t;
	products  = new ReagentBox_t;
	reactants->lstCombos = QList<QComboBox*>();
	reactants->addButton = ui->pushAddReac;
	reactants->frame     = ui->frmReacs;
	reactants->layout    = ui->layReac;
	products->addButton  = ui->pushAddProd;
	products->frame      = ui->frmProds;
	products->layout     = ui->layProd;
	
	// validator for k textboxes
	QDoubleValidator *val = new QDoubleValidator(0., 1000000., 30, 0);
	ui->spinKMinus->setValidator(val);
	ui->spinKPlus->setValidator(val);
	
	// if the compound is new
	if( isnew ) {
		setWindowTitle( "New step" );
		ui->pushValidate->setText("&Add");
	}
	else {
		setWindowTitle( base->name() );
		ui->pushValidate->setText("&Update");
		
		// initialize the fields
		ui->txtName    ->setText( base->name()   );
		ui->txtDesc    ->setText( base->desc()   );
		ui->spinKPlus  ->setText( QString("%1").arg(base->kPlus())  );
		ui->spinKMinus ->setText( QString("%1").arg(base->kMinus()) );
		
		for( int i=0; i<baseStep->reactantList().size(); i++ ) {
			addCpd( reactants, false );
			reactants->lstCombos.at(i)->setCurrentIndex( mix->CpdList.indexOf(baseStep->reactantList().at(i)) );
		}
		for( int i=0; i<baseStep->productList().size(); i++ ) {
			addCpd( products, false );
			products ->lstCombos.at(i)->setCurrentIndex( mix->CpdList.indexOf(baseStep-> productList().at(i)) );
		}
	}
	
	/*******************
	 * SIGNALS & SLOTS *
	 *******************/
	
	// validation
	connect( ui->txtName, SIGNAL(textChanged(QString)), this, SLOT(checkValidationState()) );
	connect( ui->pushValidate, SIGNAL(clicked()), this, SLOT(validate()) );
	connect( this, SIGNAL(validated()), mix, SIGNAL(stepListChanged()) );
	
	// cpd list updating
	connect( mix, SIGNAL(cpdListChanged()), this, SLOT(updateCpdLists()) );
	
	updateCpdLists();
	
	// connect add reactants and products
	connect( ui->pushAddReac,   SIGNAL(clicked()), this, SLOT(addReac()) );
	connect( ui->pushAddProd,   SIGNAL(clicked()), this, SLOT(addProd()) );
	connect( ui->pushCreateCpd, SIGNAL(clicked()), this, SIGNAL(addCpdClicked()) );
	
	// connect slots to keep step up to date
	connect( ui->txtDesc,    SIGNAL(textEdited(QString)),
			 baseStep,      SLOT(setDesc(QString))  );
	connect( ui->spinKPlus,  SIGNAL(textEdited(QString)),
	         baseStep,      SLOT(setKPlus(QString))  );
	connect( ui->spinKMinus, SIGNAL(textEdited(QString)),
	         baseStep,      SLOT(setKMinus(QString)) );
	
	// get the combo boxes (if any) to display the list of compounds
	updateCpdLists();
	
	refreshReagentBoxConnections();
	checkValidationState();
}

StepWindow::~StepWindow()
{
	if( !mix->StepList.contains(baseStep) )
		delete baseStep;
	delete ui;
}

// makes sure name/state combo is valid, adds cpd
void StepWindow::validate()
{
	QString name = ui->txtName->text();
	
	if( name.length()==0 ) {
		QMessageBox::critical(this, "Error", "Name must not be empty!", QMessageBox::Ok);
		ui->txtName->setFocus();
		return;
	}
	
	/*for( int i=0; i<Cpd::STATES->size(); i++ ) {
		if( name.contains(Cpd::STATES[i]) ) {
			QMessageBox::critical(this, "Error", "Name cannot contain state label: " + Cpd::STATES[i], QMessageBox::Ok);
			ui->txtName->setFocus();
			return;
		}
	}*/
	
	// if name does not yet exist
	//qDebug() << mix->stepNameList();
	if( !mix->stepNameList().contains(name) ) {
		
		baseStep->setName(name);
		if( !mix->StepList.contains(baseStep) ) {
			// add the step
			mix->addStep(baseStep);
		}
		
		emit validated();
		
		checkValidationState();
		ui->txtDesc->setFocus();
		
		setWindowTitle( baseStep->name() );
		ui->pushValidate->setText("&Update");
	}
	else {
		QMessageBox::critical(this, "Error", "Name must be unique!", QMessageBox::Ok);
		ui->txtName->setText(baseStep->name());
		ui->txtName->setFocus();
		ui->txtName->setSelection(0, name.length());
		checkValidationState();
	}
}
void StepWindow::checkValidationState()
{
	if( ui->txtName->text()=="" || ui->txtName->text()!=baseStep->name() ) {
		setBottomEnabled(false);
		ui->pushValidate->setEnabled(true);
	}
	else {
		setBottomEnabled(true);
		ui->pushValidate->setEnabled(false);
	}
}
void StepWindow::setBottomEnabled( bool val )
{
	ui->lblDesc ->setEnabled(val);
	ui->txtDesc ->setEnabled(val);
	ui->frmKs   ->setEnabled(val);
	ui->frmStep ->setEnabled(val);
}

void StepWindow::refreshReagentBoxConnections()
{
	disconnectReagentBoxes();
	connectReagentBoxes();
}
void StepWindow::disconnectReagentBoxes()
{
	for( int i=0; i<reactants->lstCombos.size(); i++ ) {
		disconnect( reactants->lstCombos[i], 0, 0, 0 );
		disconnect( this, 0, reactants->lstCombos[i], 0 );
		disconnect( reactants->lstRems[i], 0, 0, 0 );
		disconnect( this, 0, reactants->lstRems[i], 0 );
	}
	for( int i=0; i<products->lstCombos.size(); i++ ) {
		disconnect( products->lstCombos[i], 0, 0, 0 );
		disconnect( this, 0, products->lstCombos[i], 0 );
		disconnect( products->lstRems[i], 0, 0, 0 );
		disconnect( this, 0, products->lstRems[i], 0 );
	}
}
void StepWindow::connectReagentBoxes()
{
	for( int i=0; i<reactants->lstCombos.size(); i++ ) {		
		connect( reactants->lstCombos[i], SIGNAL(currentIndexChanged(int)), this, SLOT(setReagents()) );
		connect( reactants->lstRems[i], SIGNAL(pressed()), this, SLOT(remReacButton()) );
	}
	for( int i=0; i<products->lstCombos.size(); i++ ) {
		connect( products->lstCombos[i], SIGNAL(currentIndexChanged(int)), this, SLOT(setReagents()) );
		connect( products->lstRems[i], SIGNAL(pressed()), this, SLOT(remProdButton()) );
	}
}

void StepWindow::setReagents()
{
	QList<Cpd*> list = baseStep->reactantList();
	if( list.size()!=reactants->lstCombos.size() )
		return;
	for( int i=0; i<list.size(); i++ ) {
		if( list[i]->toString()!=reactants->lstCombos[i]->currentText() )
			baseStep->setReactant( i, mix->getCpdById(reactants->lstCombos[i]->currentText()) );
	}
	list = baseStep->productList();
	if( list.size()!=products->lstCombos.size() )
		return;
	for( int i=0; i<list.size(); i++ ) {
		if( list[i]->toString()!=products->lstCombos[i]->currentText() )
			baseStep->setProduct( i, mix->getCpdById(products->lstCombos[i]->currentText()) );
	}
}

// slots
void StepWindow::addReac()
{addCpd(reactants);}
void StepWindow::addProd()
{addCpd(products );}

void StepWindow::addCpd(ReagentBox_t* r, bool addtobase)
{
	if( r->lstCombos.size() > 2 )
		return;
	
	QLabel *plus = 0;
	
	// set up combo box
	QComboBox *combo = new QComboBox( r->frame );
	combo->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
	combo->setMinimumHeight(combo->height());
	combo->setMaximumWidth(1000);
	combo->addItems(mix->cpdIdList());
	
	// set up remove button
	QPushButton *rem = new QPushButton( r->frame );
	rem->setMaximumSize(20,20);
	rem->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
	rem->setText("x");
	
	if( r->lstCombos.size()==1 || r->lstCombos.size()==2 ) {
		// set up plus sign
		plus = new QLabel("+", r->frame);
		plus->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
		plus->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	}
	
	// add the plus sign
	if( plus != 0 ) {
		r->layout->addWidget( plus, r->layout->rowCount(), 0, 1, 1, Qt::AlignHCenter | Qt::AlignVCenter );
		r->lstPlus.append(plus);
	}
	
	// add combo box and remove button
	r->layout ->addWidget( combo, r->layout->rowCount(),   0, 1, 1, Qt::AlignCenter );
	r->layout ->addWidget( rem,   r->layout->rowCount()-1, 1, 1, 1, Qt::AlignCenter );
	r->layout ->setColumnStretch(0, 5);
	r->layout ->setColumnStretch(1, 1);
	r->lstCombos .append(combo);
	r->lstRems   .append(rem);
	
	if( addtobase ) {
		if( r==reactants )
			baseStep->addReactant( mix->getCpdById(r->lstCombos.last()->currentText()) );
		else
			baseStep->addProduct ( mix->getCpdById(r->lstCombos.last()->currentText()) );
		updateCpdLists();
	}
	
	refreshReagentBoxConnections();
	
	// disable add button if already three reactants
	if( r->lstCombos.size()==3 )
		r->addButton->setEnabled(false);
}

void StepWindow::remReacButton() {
	for( int i=0; i<reactants->lstRems.size(); i++ )
		if( reactants->lstRems[i]->isDown() )
			remCpd( reactants, i );
}
void StepWindow::remProdButton() {
	for( int i=0; i<products->lstRems.size(); i++ )
		if( products->lstRems[i]->isDown() )
			remCpd( products,  i );
}

// base function
void StepWindow::remCpd(ReagentBox_t* r, int i)
{
	qDebug() << (r==reactants?"react":"prod") << ", " << i;

	int size = r->lstCombos.size();	
	if( size < i+1 )
		return;
	
	r->lstCombos[i]->~QComboBox();
	r->lstCombos.removeAt(i);
	r->lstRems[i]->~QPushButton();
	r->lstRems.removeAt(i);
	if( r==reactants )
		baseStep->removeReactant(i);
	else
		baseStep->removeProduct (i);
	
	if( size>1 ) {
		if( i==0 ) i++;
		r->lstPlus[i-1]->~QLabel();
		r->lstPlus.removeAt(i-1);
	}
	
	r->addButton->setEnabled(true);
	
	refreshReagentBoxConnections();
}
void StepWindow::remAllCpds() {
	while( reactants->lstCombos.size()>0 )
		remCpd(reactants, 0);
	while( products->lstCombos.size()>0 )
		remCpd(products, 0);
}

// slots
/*void StepWindow::remReac1(){remCpd(reactants,0);}
void StepWindow::remReac2(){remCpd(reactants,1);}
void StepWindow::remReac3(){remCpd(reactants,2);}
void StepWindow::remProd1(){remCpd(products ,0);}
void StepWindow::remProd2(){remCpd(products ,1);}
void StepWindow::remProd3(){remCpd(products ,2);}*/

void StepWindow::updateCpdLists()
{
	QComboBox *combo;
	disconnectReagentBoxes();
	for( int i=0; i<reactants->lstCombos.size(); i++ ) {
		combo = reactants->lstCombos[i];
		disconnectReagentBoxes();
		combo->clear();
		combo->addItems(mix->cpdIdList());
		QString temp = baseStep->reactantList()[i]->toString();
		if( combo->findText(temp) == -1 )
			remCpd(reactants, i);
		else
			combo->setCurrentIndex(combo->findText(temp));
	}
	for( int i=0; i<products->lstCombos.size(); i++ ) {
		combo = products->lstCombos[i];
		combo->clear();
		combo->addItems(mix->cpdIdList());
		QString temp = baseStep->productList()[i]->toString();
		if( combo->findText(temp) == -1 )
			remCpd(products, i);
		else
			combo->setCurrentIndex(combo->findText(temp));
	}
	connectReagentBoxes();
	ui->pushAddProd->setEnabled(mix->CpdList.size()>0);
	ui->pushAddReac->setEnabled(mix->CpdList.size()>0);
	if( mix->CpdList.size()>0 )
		setReagents();
	else
		remAllCpds();
}

//
////

// drag and drop stuff ////
//

/*void StepWindow::dragEnterEvent(QDragEnterEvent *event)
{
	if( event->mimeData()->hasText() )
		event->acceptProposedAction();
}

// event called when something is dropped onto the form
void StepWindow::dropEvent(QDropEvent *event)
{
	QPoint pos = event->pos() - ui->fraStep->pos();
	if (event->mimeData()->hasText() && ui->lblReac1->geometry().contains(pos)) {
		
		qDebug() << ":D";
		
		QString cpdid = event->mimeData()->text();
		Cpd *cpd = mix->getCpdById(event->mimeData()->text());
		
		ui->lblReac1->setText(cpd->toString());
		ui->lblReac1->setBackgroundRole(QPalette::Window);
	}
	else
		event->ignore();
}

void StepWindow::dragMoveEvent(QDragMoveEvent *event)
{
	// get the relative mouse position
	QPoint pos = event->pos() - ui->fraStep->pos();
	
	// if drag event is text and mouse is within label
	if( event->mimeData()->hasText() && ui->lblReac1->geometry().contains(pos) ) {
		// darken the label background
		ui->lblReac1->setBackgroundRole(QPalette::Midlight);
	}
	// otherwise reset the background color
	else {
		ui->lblReac1->setBackgroundRole(QPalette::Window);
	}
}
*/
