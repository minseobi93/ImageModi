/*=========================================================================
 * Author : Minseob Lee
 * 2020.11.13 Created a class with constructors
 * 2020.11.14 Added File Import functions
 * 2020.11.15 Added Warning Display functions
 *=========================================================================*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessagebox>
#include <QFileDialog>

#include "../Logic/status_def.h"
#include "../Logic/gui_wigdget_manager.h"
#include "../Logic/task_manager.h"
#include "../Logic/IO/outputfile_manager.h"
#include "../Logic/IO/inputfile_manager.h"
#include "../Model/mainwindow_model.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:

    void on_checkBox_clicked();

    void on_actionSave_Files_triggered();

    void on_actionOpen_File_triggered();

    void on_AddButton_clicked();

    void on_RemoveButton_clicked();

    void on_listWidget_itemSelectionChanged();

    void on_action_modify_images_triggered();

    void on_actionMerge_Images_triggered();

    void on_actionAbout_ImageModi_triggered();

private:

    bool QFileWarningMacro(Status status);
    void QWarning(std::string message);
    void QIllegalFilesReport(QStringList Illegal_FileNames);
    void QlistWidgetUpdate();
    void UpdatelistWidgetInfo();
    void initTableWidget();
    void ReadImages();

    GUIWidgetManager* gui_widget_manager;
    MainWindow_Model* m_window;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
