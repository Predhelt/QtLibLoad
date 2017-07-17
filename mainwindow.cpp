/* This main window allows the user to search for QLibrary file directories and open them
 * in a new window.  The graphics are provided in the library, they just need to be implemented
 *
 * A QWidget or QDialog function in the library could be used to build a custom window,
 * this class would just need a known name so that the widget can be properly implemented.
 *
 * The symbols are the names of the functions (i.e. createWidget, newDialog) etc. in the library.
 * Use QLibrary::resolve("symbol") to load the correct symbols.
 * NOTE: This is only the case when the funtion in the library has the prefix 'extern "C"'.
 *
 * Example libraries are provided in this folder.  The names of the funcionts can be found in the
 * associated project, which in this instance are the SharedWindow project.
*/


#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeWindows()
{
    window->close();
    close();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    (void)event;
    window->close();
}

void MainWindow::on_loadBtn_clicked()
{
    libDir = ui->libCB->currentText();

    if(libDir != "") //if there's a libDir...
    {
        ui->placementLbl->setText("Loading library...");

        bool sameFile = (lib->fileName().compare(libDir) == 0);

        lib->setFileName(libDir.split(".").front());

        typedef QWidget *(*MyPrototype)();
        updateSymbol(ui->symCB->currentText());
        MyPrototype myFunction = (MyPrototype)lib->resolve(windowName.toLatin1().data());
        if(lib->load())
        {
            if(myFunction != 0) //if symbol is resolved...
            {
                window = myFunction();
                window->show();
                if(sameFile) //if the same window is being re-loaded...
                    ui->placementLbl->setText("Window re-loaded");
                else //if a different window is being loaded...
                    ui->placementLbl->setText("Showing new window");
                ui->loadBtn->setEnabled(true);
                ui->unloadBtn->setEnabled(true);
            }
            else //if given symbol isn't resolved...
                ui->placementLbl->setText("Error: unknow symbol");
        }
        else //if library doesn't load...
            ui->placementLbl->setText("Error: unable to load library");
    }
    else //if no libDir...
        ui->placementLbl->setText("Error: No library selected");
}

void MainWindow::on_unloadBtn_clicked()
{
    lib->unload();
    window->close();
    ui->unloadBtn->setEnabled(false);
}

void MainWindow::on_libBrowseBtn_clicked()
{
    QString currDir;
    if(ui->libCB->currentText() != "")
        currDir = ui->libCB->currentText();
    else
        currDir = QDir::currentPath();

    libDir =
            QDir::toNativeSeparators(QFileDialog::getOpenFileName(this, tr("Find Library File"), currDir,
                                                                  "All (*.dll *.DLL *.so *.a *.sl *.dylib *.bundle);;"
                                                                  "Windows (*.dll *.DLL);;Unix/Linux (*.so);;AIX (*.a);;"
                                                                  "HP-UX (*.sl *.so (HP-UXi));;macOS and iOS (*.dylib *.bundle *.so)"));

        if (!libDir.isEmpty()) {
            if (ui->libCB->findText(libDir) == -1)
                ui->libCB->addItem(libDir);
            ui->libCB->setCurrentIndex(ui->libCB->findText(libDir));
            if(!windowName.isEmpty())
                ui->placementLbl->setText("Directory found.\nHit load to load the custom window.");
            else
                ui->placementLbl->setText("Directory found.\nPlease type a valid window name\nbased on the library");
        }
}

void MainWindow::on_symCB_activated(const QString &arg1)
{
    updateSymbol(arg1);
    if(windowName.isEmpty())
        ui->placementLbl->setText("Please select a valid window name.");
    else
    {
        if(!libDir.isEmpty())
            ui->placementLbl->setText("Window name selected.\nHit load to load the custom window.");
        else
            ui->placementLbl->setText("Window name selected.\nPlease select a directory");
    }
}

void MainWindow::updateSymbol(QString symbol)
{
    windowName = symbol;
    if(ui->symCB->findText(symbol) == -1)
        ui->symCB->addItem(symbol);
}

void MainWindow::on_libCB_activated(const QString &arg1)
{
    libDir = arg1;
    if(ui->libCB->findText(arg1) == -1)
            ui->libCB->addItem(arg1);
    if (!libDir.isEmpty()) {
        if (ui->libCB->findText(libDir) == -1)
            ui->libCB->addItem(libDir);
        ui->libCB->setCurrentIndex(ui->libCB->findText(libDir));
        if(!windowName.isEmpty())
            ui->placementLbl->setText("Directory found.\nHit load to load the custom window.");
        else
            ui->placementLbl->setText("Directory found.\nPlease type a valid window name\nbased on the library");
    }
    else
        ui->placementLbl->setText("Please select a directory");
}
