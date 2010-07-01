#include "stepwindow.h"

StepWindow::StepWindow(Step* base, QWidget* parent, bool isnew)
	: QWidget(parent), baseStep(base)
{
	ui.setupUi(this);
	
	// initialize reagent boxes;
	reactants = new ReagentBox_t;
	products  = new ReagentBox_t;
	reactants->addButton = ui.pushAddReac;
	reactants->frame     = ui.frmReacs;
	reactants->layout    = ui.layReac;
	products->addButton  = ui.pushAddProd;
	products->frame      = ui.frmProds;
	products->layout     = ui.layProd;
	
	/*******************
	 * SIGNALS & SLOTS *
	 *******************/
	
	// validation
	connect( ui.txtName, SIGNAL(textChanged(QString)), this, SLOT(checkValidationState()) );
	connect( ui.pushValidate, SIGNAL(clicked()), this, SLOT(validate()) );
	connect( this, SIGNAL(validated()), mix, SIGNAL(stepListChanged()) );
	
	// cpd list updating
	connect( mix, SIGNAL(cpdListChanged()), this, SLOT(updateCpdLists()) );
	updateCpdLists();
	
	// connect add reactants and products
	connect( ui.pushAddReac, SIGNAL(clicked()),
	         this,           SLOT(addReac()) );
	connect( ui.pushAddProd, SIGNAL(clicked()),
	         this,           SLOT(addProd()) );
	         
	
	// connect slots to keep step up to date
	//connect( ui.txtName,    SIGNAL(textEdited(QString)),
	//         baseStep,      SLOT(setName(QString))  );
	connect( ui.txtDesc,    SIGNAL(textEdited(QString)),
			 baseStep,      SLOT(setDesc(QString))  );
	connect( ui.spinKPlus,  SIGNAL(valueChanged(double)),
	         baseStep,      SLOT(setKPlus(double))  );
	connect( ui.spinKMinus, SIGNAL(valueChanged(double)),
	         baseStep,      SLOT(setKMinus(double)) );

	// set up a feedback loop for the rate boxes
	connect( baseStep,      SIGNAL(kPlusChanged(double)),
	         ui.spinKPlus,  SLOT(setValue(double)) );
	connect( baseStep,	    SIGNAL(kMinusChanged(double)),
	         ui.spinKMinus, SLOT(setValue(double)) );
	
	
	if( isnew ) {
		setWindowTitle( "New step" );
	}
	else {
		setWindowTitle( base->name() );
		// initialize the fields
		ui.txtName    ->setText(  base->name()   );
		ui.txtDesc    ->setPlainText(  base->desc()   );
		ui.spinKPlus  ->setValue( base->kPlus()  );
		ui.spinKMinus ->setValue( base->kMinus() );
		for( int i=0; i<baseStep->reactantList().size(); i++ )
			addCpd( reactants );
		for( int i=0; i<baseStep->productList().size(); i++ )
			addCpd( products );
	}
	
	checkValidationState();
}

// makes sure name/state combo is valid, adds cpd
void StepWindow::validate()
{
	QString name = ui.txtName->text();
	
	if( name.length()==0 ) {
		QMessageBox::critical(this, "Error", "Name must not be empty!", QMessageBox::Ok);
		ui.txtName->setFocus();
		return;
	}
	
	// if name does not yet exist
	if( !mix->stepNameList().contains(name) ) {
		
		baseStep->setName(name);
		if( !mix->StepList.contains(baseStep) ) {
			// add the step
			mix->addStep(baseStep);
		}
		
		emit validated();
		
		checkValidationState();
		ui.txtDesc->setFocus();
		
		setWindowTitle( baseStep->name() );
	}
	else {
		QMessageBox::critical(this, "Error", "Name must be unique!", QMessageBox::Ok);
		ui.txtName->setText(baseStep->name());
		ui.txtName->setFocus();
		ui.txtName->setSelection(0, name.length());
		checkValidationState();
	}
}
void StepWindow::checkValidationState()
{
	if( ui.txtName->text()=="" || ui.txtName->text()!=baseStep->name() ) {
		setBottomEnabled(false);
		ui.pushValidate->setEnabled(true);
	}
	else {
		setBottomEnabled(true);
		ui.pushValidate->setEnabled(false);
	}
}
void StepWindow::setBottomEnabled( bool val )
{
	ui.lblDesc ->setEnabled(val);
	ui.txtDesc ->setEnabled(val);
	ui.frmKs   ->setEnabled(val);
	ui.frmStep ->setEnabled(val);
}

// slots
void StepWindow::remReac1(){remCpd(reactants,0);}
void StepWindow::remReac2(){remCpd(reactants,1);}
void StepWindow::remReac3(){remCpd(reactants,2);}
void StepWindow::remProd1(){remCpd(products ,0);}
void StepWindow::remProd2(){remCpd(products ,1);}
void StepWindow::remProd3(){remCpd(products ,2);}

// base function
void StepWindow::remCpd(ReagentBox_t* r, int i)
{
	
}

// slots
void StepWindow::addReac()
{addCpd(reactants);}
void StepWindow::addProd()
{addCpd(products );}

// base function
void StepWindow::addCpd(ReagentBox_t* r)
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
	
	switch( r->lstCombos.size() ) {
	case 0:
		// disable remove button
		//rem->setEnabled(false);
		break;
	case 1:
		// enable previous remove button
		//r->lstRems.last()->setEnabled(true);
		
		// NO BREAK, case 1 does both
		
	case 2:
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
	
	//connect( remReac, SIGNAL(pressed()), this, SLOT );
	
	// disable add button if already three reactants
	if( r->lstCombos.size()==3 )
		r->addButton->setEnabled(false);
}

void StepWindow::updateCpdLists()
{
	QComboBox *combo;
	qDebug() << ":D";
	for( int i=0; i<reactants->lstCombos.size(); i++ ) {
		combo = reactants->lstCombos[i];
		QString temp = combo->currentText();
		combo->clear();
		combo->addItems(mix->cpdIdList());
		if( combo->findText(temp) != -1 )
			combo->setCurrentIndex(combo->findText(temp));
	}
	for( int i=0; i<products->lstCombos.size(); i++ ) {
		combo = products->lstCombos[i];
		QString temp = combo->currentText();
		combo->clear();
		combo->addItems(mix->cpdIdList());
		if( combo->findText(temp) != -1 )
			combo->setCurrentIndex(combo->findText(temp));
	}
	qDebug() << ":D";
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
	QPoint pos = event->pos() - ui.fraStep->pos();
	if (event->mimeData()->hasText() && ui.lblReac1->geometry().contains(pos)) {
		
		qDebug() << ":D";
		
		QString cpdid = event->mimeData()->text();
		Cpd *cpd = mix->getCpdById(event->mimeData()->text());
		
		ui.lblReac1->setText(cpd->toString());
		ui.lblReac1->setBackgroundRole(QPalette::Window);
	}
	else
		event->ignore();
}

void StepWindow::dragMoveEvent(QDragMoveEvent *event)
{
	// get the relative mouse position
	QPoint pos = event->pos() - ui.fraStep->pos();
	
	// if drag event is text and mouse is within label
	if( event->mimeData()->hasText() && ui.lblReac1->geometry().contains(pos) ) {
		// darken the label background
		ui.lblReac1->setBackgroundRole(QPalette::Midlight);
	}
	// otherwise reset the background color
	else {
		ui.lblReac1->setBackgroundRole(QPalette::Window);
	}
}
*/
