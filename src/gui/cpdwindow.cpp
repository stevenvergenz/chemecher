#include "mainwindow.h"

CpdWindow::CpdWindow(Cpd* base, QWidget* parent) : QFrame(parent), baseCpd(base)
{
	ui.setupUi(this);

	//initialize all the fields to the appropriate values
	//ui.txtLongName->setText( base->longName() );
	ui.txtShortName->setText( base->shortName() );
	ui.comboState->setCurrentIndex( (int)base->state() );
	ui.comboTrans->setCurrentIndex( (int)base->transition() );
	ui.spinThresh->setValue( base->threshold() );
	ui.spinSharp->setValue( base->sharpness() );
	ui.spinConc->setValue( base->initialConc() );
	setWindowTitle( base->toString() );

	//hide unused fields
	hideOnHomo((int)base->state());

	//connect the various widgets to modify the appropriate fields in base
	/*connect( ui.txtLongName, SIGNAL(textEdited(QString)),
		 baseCpd,	 SLOT(setLongName(QString))
	);*/
	connect( ui.txtShortName,SIGNAL(textEdited(QString)),
		 baseCpd,	 SLOT(setShortName(QString))
	);
	connect( ui.comboState,	 SIGNAL(currentIndexChanged(int)),
		 baseCpd,	 SLOT(setState(int))
	);
	connect( ui.comboTrans,	 SIGNAL(currentIndexChanged(int)),
		 baseCpd,	 SLOT(setTransition(int))
	);
	connect( ui.spinThresh,	 SIGNAL(valueChanged(double)),
		 baseCpd,	 SLOT(setThreshold(double))
	);
	connect( ui.spinSharp,	 SIGNAL(valueChanged(double)),
		 baseCpd,	 SLOT(setSharpness(double))
	);
	connect( ui.spinConc,	 SIGNAL(valueChanged(double)),
		 baseCpd,	 SLOT(setInitialConc(double))
	);

	//set it up to hide unused fields when the combos are changed
	connect( ui.comboState,	 SIGNAL(currentIndexChanged(int)),
		 this,		 SLOT(hideOnHomo(int))
	);
	connect( ui.comboTrans,	 SIGNAL(currentIndexChanged(int)),
		 this,		 SLOT(hideOnTrans(int))
	);

	//set up the formatting replacements


	setStatusTip("Put your species here");
}

void CpdWindow::hideOnHomo(int newstate)
{
	Cpd::State state = (Cpd::State)newstate;

	//hide unneeded fields based on the state
	if(state == Cpd::HOMO || state == Cpd::AQ || state == Cpd::G)
	{
		ui.lblTrans->setVisible(false);
		ui.comboTrans->setVisible(false);
	}
	else
	{
		ui.lblTrans->setVisible(true);
		ui.comboTrans->setVisible(true);
	}

	hideOnTrans( ui.comboTrans->currentIndex());
}

void CpdWindow::hideOnTrans(int newtrans)
{
	//make sure it doesn't override state toggle
	if( !ui.comboTrans->isVisible()) newtrans = 0;

	//hide fields based on the transition
	switch(newtrans){
	case 0: ui.lblThresh->setVisible(false); ui.spinThresh->setVisible(false);
		ui.lblSharp->setVisible(false); ui.spinSharp->setVisible(false);
		break;
	case 1: ui.lblThresh->setVisible(true); ui.spinThresh->setVisible(true);
		ui.lblSharp->setVisible(false); ui.spinSharp->setVisible(false);
		break;
	case 2: ui.lblThresh->setVisible(true); ui.spinThresh->setVisible(true);
		ui.lblSharp->setVisible(true); ui.spinSharp->setVisible(true);
		break;
	};
}

/*
TO USE FOR FOCUS HANDLING:

MainWindow::MainWindow(QWidget *parent) :
QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->comboBox->installEventFilter(this);
    .
    .
    .
}

bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::FocusOut)
    {
	if (object == ui->comboBox)
	{
	    qWarning(object->objectName().toLatin1().data());
	}
    }
    return false;
}
*/

void CpdWindow::toHTML()
{
	/*
	<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd">
	<html><head><meta name="qrichtext" content="1" />
	<style type="text/css">	p, li { white-space: pre-wrap; }</style></head>
	<body style=" font-family:'Sans'; font-size:10pt; font-weight:400; font-style:normal;">
	<p align="center" style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;">
	NH<span style=" vertical-align:sub;">4</span><span style=" vertical-align:super;">+</span></p>
	</body></html>
	*/
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
