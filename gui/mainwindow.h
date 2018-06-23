#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiSubWindow>
#include <QFileDialog>
#include <QMessageBox>

#include "widgets/mylistwidget.h"
#include "models/repository/repository.h"
#include "models/actions/randomizer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    static QList<ListState> toGuiFormat(QList<RandomItemList> lists);
    static QList<RandomItemList> toModelFormat(QList<ListState> listsStates);
    static QString toFileFormat(QList<RandomItemList> lists);

private slots:

    void on_actionOpen_triggered();

    void on_randomizePushButton_clicked();

    void on_actionClear_triggered();

    void on_actionSave_triggered();

private:
    Ui::MainWindow *ui;

    QList<MyListWidget*> m_listsWidgets;

    Repository m_repo;
    QString path;

    void initWidgets();
    void clear();
};

#endif // MAINWINDOW_H
