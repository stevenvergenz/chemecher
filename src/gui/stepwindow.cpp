#include "stepwindow.h"

StepWindow::StepWindow(Step* step, QWidget* parent) : QFrame(parent), baseStep(step)
{
	ui.setupUi(this);

	//initialize the fields
	ui.txtName->setText( step->name() );
	ui.txtKPlus->setText( QString::number( step->kPlus(), 'g', 6 ) );
	ui.txtKMinus->setText( QString::number( step->kMinus(), 'g', 6 ) );
	updateReagentBoxes();

	//connect slots to keep step up to date
	connect( ui.txtName,	SIGNAL(textEdited(QString)),
		 baseStep,	SLOT(setName(QString))
	);
	connect( ui.txtKPlus,	SIGNAL(editingFinished()),
		 this,		SLOT(setBaseKPlus())
	);
	connect( ui.txtKMinus,	SIGNAL(editingFinished()),
		 this,		SLOT(setBaseKMinus())
	);

	//set up a feedback loop for the rate boxes
	connect( baseStep,	SIGNAL(kPlusChanged(double)),
		 this,		SLOT(setWindowKPlus(double))
	);
	connect( baseStep,	SIGNAL(kMinusChanged(double)),
		 this,		SLOT(setWindowKMinus(double))
	);

	setStatusTip("Put your steps here");
}

void StepWindow::updateReagentBoxes()
{
	//pull the new list of species from mix->CpdList
	QStringList cpdNameList;
	cpdNameList << "";
	int size = mix->CpdList.size();
	for( int i=0; i<size; i++ ){
		cpdNameList << mix->CpdList[i]->shortName();
	}

	//find the strings of the current indexes

	//update the contents of the combo boxes

	//reassign the index

}

void StepWindow::setBaseKPlus(){
	baseStep->setKPlus( ui.txtKPlus->text() );
}

void StepWindow::setBaseKMinus(){
	baseStep->setKMinus( ui.txtKMinus->text() );
}

void StepWindow::setWindowKPlus(double k){
	ui.txtKPlus->setText( QString::number(k, 'g', 6) );
}

void StepWindow::setWindowKMinus(double k){
	ui.txtKMinus->setText( QString::number(k, 'g', 6) );
}
