#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLibrary>
#include <QDir>
#include <QFileDialog>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_loadBtn_clicked();

    void on_libBrowseBtn_clicked();

    void on_unloadBtn_clicked();

    void closeWindows();

    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

    void on_symCB_activated(const QString &arg1);

    void on_libCB_activated(const QString &arg1);

    void updateSymbol(QString symbol);

private:

    Ui::MainWindow *ui;

    QWidget *window = new QWidget(); //widget that will be retrieved from the library

    QLibrary *lib = new QLibrary(); //library containing the widget

    QString libDir; //directory of library with file type

    QString windowName; //the name of the window to be retrieved from the library
};

#endif // MAINWINDOW_H
