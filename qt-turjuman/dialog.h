#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QStringList>
#include <QString>
#include <QMessageBox>
#include "connection.h"
#include <QMap>
#include <QList>
// #include <QTableWidgetItem>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    enum Mode{ExactSearch, SimilarSearch};
    void fillWordsTable();
    void translateWord();
    void displayWordsList();
    void displayWordsTable();

private slots:
    void on_translateButton_clicked();
    // void on_wordInput_editingFinished();
    void on_exactSearchButton_toggled(bool checked);
    void on_similarSearchButton_toggled(bool checked);
    void returnPressed();
    void on_helpButton_clicked();

private:
    Ui::Dialog *ui;
    QStringList words;
    QMap<QString, QString> wordsMap;
    Connection connection;
    Mode mode;
    int defaultRowCount;
};

#endif // DIALOG_H
