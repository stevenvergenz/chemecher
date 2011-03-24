#include "mainwindow.h"

CpdWindow::CpdWindow(Cpd* base, QWidget* parent, bool isnew) : QWidget(parent), baseCpd(base)
{
	ui.setupUi(this);
	
	// connect short name and state
	connect( ui.txtShortName, SIGNAL(textEdited(QString)),
			 this,            SLOT(checkValidationState()) );
	connect( ui.comboState,	  SIGNAL(currentIndexChanged(int)),
			 this,            SLOT(checkValidationState()) );
	connect( ui.pushValidate, SIGNAL(clicked()),
			 this,            SLOT(validate()) );
	connect( this, SIGNAL(validated()), mix, SIGNAL(cpdListChanged()) );
	
	// connect the various widgets to modify the appropriate fields in base
	connect( ui.txtLongName,  SIGNAL(textEdited(QString)),
	         baseCpd,         SLOT(setLongName(QString)) );
	connect( ui.comboTrans,   SIGNAL(currentIndexChanged(int)),
			 baseCpd,         SLOT(setTransition(int)) );
	connect( ui.spinThresh,   SIGNAL(valueChanged(double)),
			 baseCpd,         SLOT(setThreshold(double)) );
	connect( ui.spinSharp,    SIGNAL(valueChanged(double)),
			 baseCpd,         SLOT(setSharpness(double)) );

	//set it up to hide unused fields when the combos are changed
	connect( ui.comboState, SIGNAL(currentIndexChanged(int)),
			 this,          SLOT(updateForm()) );
	connect( ui.comboTrans, SIGNAL(currentIndexChanged(int)),
			 this,          SLOT(updateForm()) );
	
	// if the compound is being added
	if( isnew ) {
		setWindowTitle("New specie");
		ui.pushValidate->setText("&Add");
	}
	else {
		setWindowTitle(baseCpd->toString());
		ui.pushValidate->setText("&Update");
		ui.txtShortName->setText(baseCpd->shortName());
		ui.comboState->setCurrentIndex((int)baseCpd->state());
		ui.comboTrans->setCurrentIndex((int)baseCpd->transition());
		ui.spinThresh->setValue(baseCpd->threshold());
		ui.spinSharp->setValue(baseCpd->sharpness());
		ui.txtLongName->setText(baseCpd->longName());
	}

	checkValidationState();
	updateForm();
	
	ui.txtShortName->setFocus();
}

// makes sure name/state combo is valid, adds cpd
void CpdWindow::validate()
{
	QString name = ui.txtShortName->text();
	int state = ui.comboState->currentIndex();
	QRegExp regex("^[A-Za-z][A-Za-z0-9\\-\\+]*$");
	if( regex.indexIn(name)==-1 ) {
		QMessageBox::critical(this, "Error", 
			"Name is not valid! Names may contain letters, numbers, and +/-, "
			"and must begin with a letter.",
			QMessageBox::Ok);
		ui.txtShortName->setFocus();
		return;
	}
	
	// if name/state combo does not yet exist
	if( !mix->cpdIdList().contains(name+Cpd::STATES[state]) ) {
		
		baseCpd->setShortName(name);
		baseCpd->setState(state);
		if( !mix->CpdList.contains(baseCpd) ) {
			// add the compound
			mix->addCpd(baseCpd);
		}
		
		emit validated();
		
		checkValidationState();
		ui.txtLongName->setFocus();
		
		setWindowTitle( baseCpd->toString() );
		ui.pushValidate->setText("&Update");
	}
	else {
		QMessageBox::critical(this, "Error", "Name/state combination must be unique!", QMessageBox::Ok);
		ui.txtShortName->setText(baseCpd->shortName());
		ui.comboState->setCurrentIndex((int)baseCpd->state());
		ui.txtShortName->setFocus();
		ui.txtShortName->setSelection(0, name.length());
		checkValidationState();
	}
}

void CpdWindow::checkValidationState()
{
	if( ui.txtShortName->text()=="" || ui.txtShortName->text()!=baseCpd->shortName()
		|| ui.comboState->currentIndex()!=(int)baseCpd->state() ) {
		setBottomEnabled(false);
		ui.pushValidate->setEnabled(true);
	}
	else {
		setBottomEnabled(true);
		ui.pushValidate->setEnabled(false);
	}
}

void CpdWindow::setBottomEnabled( bool val )
{
	ui.frm1->setEnabled(val);
	ui.frm2->setEnabled(val);
	updateForm();
}

// updates the transition, thresh, and sharp rows
void CpdWindow::updateForm()
{
	
	// get current selections
	Cpd::State state = (Cpd::State)ui.comboState->currentIndex();
	int trans = ui.comboTrans->currentIndex();
	
	// hide all fields first
	ui.lblTrans   ->setEnabled(false);
	ui.comboTrans ->setEnabled(false);
	ui.lblThresh  ->setEnabled(false);
	ui.spinThresh ->setEnabled(false);
	ui.lblSharp   ->setEnabled(false);
	ui.spinSharp  ->setEnabled(false);
	
	// re-show fields as necessary
	if(state != Cpd::HOMO && state != Cpd::AQ && state != Cpd::G) {
		
		// show the trans combo box
		ui.lblTrans   ->setEnabled(true);
		ui.comboTrans ->setEnabled(true);
		
		switch(trans) {
		case 0:
			ui.comboTrans ->setCurrentIndex(0);
			ui.spinThresh ->setValue(0);
			ui.spinSharp  ->setValue(0);
			break;
		case 2: // show both sharp and thresh
			ui.lblSharp   ->setEnabled(true);
			ui.spinSharp  ->setEnabled(true);
			// NO BREAK, moves to case 1
		case 1: // show just thresh
			ui.lblThresh  ->setEnabled(true);
			ui.spinThresh ->setEnabled(true);
		}
	}
}

/*
void CpdWindow::toHTML()
{
	<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd">
	<html><head><meta name="qrichtext" content="1" />
	<style type="text/css">	p, li { white-space: pre-wrap; }</style></head>
	<body style=" font-family:'Sans'; font-size:10pt; font-weight:400; font-style:normal;">
	<p align="center" style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;">
	NH<span style=" vertical-align:sub;">4</span><span style=" vertical-align:super;">+</span></p>
	</body></html>
	
	QString script = ui.txtLongName->toHtml();

	//insert the subscripts
	script.replace( QRegExp("_(\\S)"), "<span style=\" vertical-align:sub;\">\\1</span>");

	//insert the superscripts
	script.replace( QRegExp("\\^(\\S)"), "<span style=\" vertical-align:super;\">\\1</span>");

	//insert header and footer
	script.prepend("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\"><html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">p, li { white-space: pre-wrap; }</style></head><body style=\" font-family:'Sans'; font-size:10pt; font-weight:400; font-style:normal;\"><p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">");
	script.append("</p></body></html>");

	ui.txtLongName->setHtml(script);
}

void CpdWindow::toEditable()
{

}
*/
