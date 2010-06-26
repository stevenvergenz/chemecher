#include "stepwindow.h"

StepWindow::StepWindow(Step* base, QWidget* parent, bool isnew)
	: QWidget(parent), baseStep(base)
{
	ui.setupUi(this);

	// fill the reagent boxes
	reactants->addButton = ui.pushAddReac;
	reactants->frame     = ui.frmReacs;
	reactants->layout    = ui.layReac;
	products->addButton  = ui.pushAddProd;
	products->frame      = ui.frmProds;
	products->layout     = ui.layProd;
	
	// set the window title
	this->setWindowTitle( isnew ? "New step" : base->name() );
	
	// initialize the fields
	ui.txtName    ->setText(  base->name()   );
	ui.spinKPlus  ->setValue( base->kPlus()  );
	ui.spinKMinus ->setValue( base->kMinus() );
	
	addCpd(reactants);
	addCpd(products );
	
	//connect added or removed compound to updateCpdLists();
	
	updateCpdLists();
	
	// connect add reactants and products
	connect( ui.pushAddReac, SIGNAL(clicked()),
	         this,           SLOT(addReac()) );
	connect( ui.pushAddProd, SIGNAL(clicked()),
	         this,           SLOT(addProd()) );
	         
	
	// connect slots to keep step up to date
	connect( ui.txtName,    SIGNAL(textEdited(QString)),
	         baseStep,      SLOT(setName(QString))  );
	connect( ui.spinKPlus,  SIGNAL(valueChanged(double)),
	         baseStep,      SLOT(setKPlus(double))  );
	connect( ui.spinKMinus, SIGNAL(valueChanged(double)),
	         baseStep,      SLOT(setKMinus(double)) );

	// set up a feedback loop for the rate boxes
	connect( baseStep,      SIGNAL(kPlusChanged(double)),
	         ui.spinKPlus,  SLOT(setValue(double)) );
	connect( baseStep,	    SIGNAL(kMinusChanged(double)),
	         ui.spinKMinus, SLOT(setValue(double)) );
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
		rem->setEnabled(false);
		break;
	case 1:
		// enable previous remove button
		r->lstRems.last()->setEnabled(true);
		
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
/*void StepWindow::addProduct()
{
	if( lstComboProds.size() > 2 )
		return;
	
	QLabel *plus = 0;
	
	// set up combo box and remove button
	QComboBox   *prod    = new QComboBox(   ui.frmProds );
	prod->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
	prod->setMinimumHeight(prod->height());
	prod->setMaximumWidth(1000);
	QPushButton *remProd = new QPushButton( ui.frmProds );
	remProd->setMaximumSize(20,20);
	remProd->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
	remProd->setText("x");
	
	switch( lstComboProds.size() ) {
	case 0:
		// disable remove button
		remProd->setEnabled(false);
		break;
	case 1:
		// enable previous remove button
		lstPushRemProds.last()->setEnabled(true);
		
		// NO BREAK, case 1 does both
		
	case 2:
		// set up plus sign
		plus = new QLabel("+", ui.frmProds);
		plus->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
		plus->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	}
	
	// add the plus sign
	if( plus != 0 ) {
		ui.layProd->addWidget( plus,    ui.layProd->rowCount(), 0, 1, 1, Qt::AlignHCenter | Qt::AlignVCenter );
		lstLblPlusProds.append(plus);
	}
	
	// add combo box and remove button
	ui.layProd    ->addWidget( prod,    ui.layProd->rowCount(),   0, 1, 1, Qt::AlignCenter );
	ui.layProd    ->addWidget( remProd, ui.layProd->rowCount()-1, 1, 1, 1, Qt::AlignCenter );
	ui.layProd    ->setColumnStretch(0, 5);
	ui.layProd    ->setColumnStretch(1, 1);
	lstComboProds   .append(prod);
	lstPushRemProds .append(remProd);
	
	//connect( remProd, SIGNAL(pressed()), this, SLOT );
	
	// disable add button if already three prodtants
	if( this->lstComboProds.size()==3 )
		ui.pushAddProd->setEnabled(false);
}*/

void StepWindow::updateCpdLists()
{
	
}

//
////

// drag and drop stuff ////
//

void StepWindow::dragEnterEvent(QDragEnterEvent *event)
{
	if( event->mimeData()->hasText() )
		event->acceptProposedAction();
}

// event called when something is dropped onto the form
void StepWindow::dropEvent(QDropEvent *event)
{
	/*QPoint pos = event->pos() - ui.fraStep->pos();
	if (event->mimeData()->hasText() && ui.lblReac1->geometry().contains(pos)) {
		
		qDebug() << ":D";
		
		QString cpdid = event->mimeData()->text();
		Cpd *cpd = mix->getCpdById(event->mimeData()->text());
		
		ui.lblReac1->setText(cpd->toString());
		ui.lblReac1->setBackgroundRole(QPalette::Window);
	}
	else
		event->ignore();*/
}

void StepWindow::dragMoveEvent(QDragMoveEvent *event)
{
	// get the relative mouse position
	/*QPoint pos = event->pos() - ui.fraStep->pos();
	
	// if drag event is text and mouse is within label
	if( event->mimeData()->hasText() && ui.lblReac1->geometry().contains(pos) ) {
		// darken the label background
		ui.lblReac1->setBackgroundRole(QPalette::Midlight);
	}
	// otherwise reset the background color
	else {
		ui.lblReac1->setBackgroundRole(QPalette::Window);
	}*/
}
