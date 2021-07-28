#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QVector>
#include <vector>
#include "DailyStock.h"
#include "Date.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_pushButton_clicked();

    void on_dateEdit_userDateChanged(const QDate &date);

    void on_dateEdit_2_userDateChanged(const QDate &date);

    void on_pushButton_2_clicked();

    void on_lineEdit_textEdited(const QString &arg1);

    void on_lineEdit_2_textEdited(const QString &arg1);

    void on_lineEdit_3_textEdited(const QString &arg1);

private:
    Ui::MainWindow *ui;
    QString company;
    QString sDate = "06/30/2016";
    QString eDate = "06/29/2021";
    QString displayString;

};
#endif // MAINWINDOW_H
