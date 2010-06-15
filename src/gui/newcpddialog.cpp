#include "newcpddialog.h"

NewCpdDialog::NewCpdDialog(QWidget *parent) : QDialog(parent)
{
    ui.setupUi(this);
    //ui.txtNewName->selectAll();
    //ui.txtNewName->setFocus();
}

void NewCpdDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
	ui.retranslateUi(this);
        break;
    default:
        break;
    }
}

//overloaded accept() function to validate input
/*void NewCpdDialog::accept()
{
	const QString states[6] = {"(.)", "(*)", "(aq)", "(s)", "(l)", "(g)"};
	const QString newName = ui.txtNewName->text() + states[ui.comboNewState->currentIndex()];

	//ensure that the combination is unique by checking existing
	for(int i=0; i<Mix::CpdList.size(); i++)
	{
		//if the entry is already in the list
		if(Mix::CpdList[i]->toString() == newName){
			ui.lblMessage->setText("ERROR: that specie already exists.\nPlease note that this combination must be unique.");
			ui.txtNewName->selectAll();
			ui.txtNewName->setFocus();
			ui.comboNewState->setCurrentIndex(0);
			return;
		}
	}

	//the new name/state pair is unique, accept
	QDialog::accept();
}

QString NewCpdDialog::getName(){
	return ui.txtNewName->text();
}

int NewCpdDialog::getState(){
	return ui.comboNewState->currentIndex();
}
*/
