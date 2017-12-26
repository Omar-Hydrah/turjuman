#include "dialog.h"
#include "ui_dialog.h"
#include <QDir>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    setWindowTitle("Turjuman translator");
    // qDebug() << "Current dir: " << QDir::currentPath();

    defaultRowCount = 4;

    // Sets the program in exact search mode.
    // Example: If an english word is translated, the result 
    // will be only a QStringList with russian words.

    // Similar search mode will display the list of words,
    // and their meaning in the other language. 
    ui->exactSearchButton->setChecked(true);
    ui->wordsTable->hide();
    mode = ExactSearch;

    // ui->wordsList->hide();
    // mode = SimilarSearch;

    // Column labels
    QStringList labels;
    labels << "English";
    labels << "Russian";

    connect(ui->wordInput, SIGNAL(returnPressed()), this, SLOT(returnPressed()));

	// Row and column count must be set.
    ui->wordsTable->setHorizontalHeaderLabels(labels);
    // ui->wordsTable->setColumnCount(2);
    // ui->wordsTable->setRowCount(defaultRowCount);
}


Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_translateButton_clicked(){
	translateWord();
}

/*void Dialog::on_wordInput_editingFinished(){
	translateWord();
}*/

void Dialog::returnPressed(){
	// qDebug() << "Return pressed";
	translateWord();
}

void Dialog::translateWord(){
	// Clearing any previous words.
	ui->wordsList->clear();
	ui->wordsTable->clear();

	// Resetting row count for wordsTable.
	ui->wordsTable->setRowCount(defaultRowCount);

	bool russian = ui->russianButton->isChecked();
	bool english = ui->englishButton->isChecked();
	QString word = ui->wordInput->text();

	if(russian){
		// qDebug() << "russian";
		if(mode == ExactSearch){
			words = connection.translateWord("russian",  word);
			ui->wordsList->addItems(words);
		}else{
			// QMap<QString, QString> 
			wordsMap = connection.translateWordDouble("russian", word);

			fillWordsTable();
		}

	}else{
		// Chosing english is assumed.
		// qDebug() << "english";
		if(mode == ExactSearch){
			words = connection.translateWord("english", word);
			ui->wordsList->addItems(words);
		}else{
			// QMap<QString, QString> 
			wordsMap = connection.translateWordDouble("english", word);
			fillWordsTable();
		}
	}

	// qDebug() << word << " being translated";
}

void Dialog::fillWordsTable(){
	// qDebug() << mode;
	// qDebug() << "element visible: " << ui->wordsTable->isVisible();  

	// Changing the row count for the wordsTable 
	// to accept all available words.
	ui->wordsTable->setRowCount(wordsMap.size());

	QMapIterator<QString, QString> iter(wordsMap);

	// To iterate over ui->wordsTable columns
	int counter = 0;

	while(iter.hasNext()){
		iter.next();
		// qDebug() << iter.key() << "-" << iter.value();
		QTableWidgetItem *english = new QTableWidgetItem(iter.key());
		QTableWidgetItem *russian = new QTableWidgetItem(iter.value());

        ui->wordsTable->setItem(counter, 0, english);
        ui->wordsTable->setItem(counter, 1, russian);

		counter++;

	}
}

void Dialog::on_exactSearchButton_toggled(bool checked){
	if(checked){
		displayWordsList();
		mode = ExactSearch;
	}
}

void Dialog::on_similarSearchButton_toggled(bool checked){
	if(checked){
		displayWordsTable();
        mode = SimilarSearch;
	}
}

void Dialog::displayWordsTable(){
	// Hides the ui->wordsList,
	// as the results will be two columns displayed into ui->wordsTable.
	ui->wordsList->hide();
	ui->wordsTable->show();

}

void Dialog::displayWordsList(){
	// Hides the ui->wordsTable, 
	// as the result will be a list of words placed into ui->wordsList.
	ui->wordsTable->hide();
	ui->wordsList->show();
	
}


// To explain to the user how to use the program.
void Dialog::on_helpButton_clicked(){
	QString help = "You can search a word using 'Exact Search', or 'Similar Search.'\n";
	help += "Exact search will find only words that match the searched words.\n";
	help += "Similar search will look for all possible matches.\nFor example: ";
	help += "Using similar search to translate 'a' will find all words that begin with an a.";

	QMessageBox::information(this, "Help/ Instructions", help);
}