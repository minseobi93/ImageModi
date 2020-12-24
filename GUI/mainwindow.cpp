#include <iostream>
#include <QDeadlineTimer>
#include <QDebug>
#include <QTextEdit>

#include "./ui_mainwindow.h"
#include "mainwindow.h"
#include "../Logic/constant.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_window = new MainWindow_Model();
    gui_widget_manager = new GUIWidgetManager();

    ui->literal_table_in->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->literal_table_out->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->num_table_in->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->num_table_out->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::UpdatelistWidgetInfo()
{
    unsigned int total_itemnum = 0, selected_itemnum = 0;
    double selected_size = 0;

    total_itemnum = ui->listWidget->count();
    selected_itemnum = ui->listWidget->selectedItems().count();

    FileManager <QStringList>* filemanager = new FileManager<QStringList>();
    QStringList filelist;
    for (const auto filename : ui->listWidget->selectedItems()) filelist.push_back(filename->text());
    selected_size = filemanager->GetTotalSizeAsBytes(filelist);
    delete filemanager;

    std::string info = "( " + std::to_string(selected_itemnum) + " / " + std::to_string(total_itemnum) + " ) "+ " Image(s) selected " + filemanager->GetFileSizeAsString(selected_size);
    ui->ListwidgetInfoText->setText(QString::fromStdString(info));
}

void MainWindow::QlistWidgetUpdate()
{
    ui->listWidget->clear();
    InputFileManager <QStringList>* inputfilemanager = new InputFileManager<QStringList>();
    QStringList filenames = inputfilemanager->GetInputFilenamesValue(m_window);
    for (const auto m_inputfilename : filenames)
    {
        ui->listWidget->addItem(m_inputfilename);
    }
}

void MainWindow::initTableWidget()
{
    for (int i = 0; i < literal_tablerow; i++)
    {
        for (int j = 0; j < literal_tablecolumn; j++)
        {
            ui->literal_table_in->setItem(i, j, new QTableWidgetItem(""));
            ui->literal_table_out->setItem(i, j, new QTableWidgetItem(""));
        }
    }

    for (int i = 0; i < numeric_tablerow; i++)
    {
        for (int j = 0; j < numeric_tablecolumn; j++)
        {
            ui->num_table_in->setItem(i, j, new QTableWidgetItem(""));
            ui->num_table_out->setItem(i, j, new QTableWidgetItem(""));
        }
    }
}

void MainWindow::QWarning(std::string message)
{
    if (message.empty()) return; // nothing to display

    QMessageBox *alert = new QMessageBox;
    alert->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    QString qmessagestr = QString::fromStdString(message);
    alert->setText(qmessagestr);
    alert->exec();
}

UserResponse MainWindow::QRequest(std::string message)
{
	QMessageBox *alert = new QMessageBox;
	alert->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
	QString qmessagestr = QString::fromStdString(message);
	alert->setText(qmessagestr);
	int response = alert->exec();
	if (response == QMessageBox::Ok) return OK;
	else if (response == QMessageBox::Cancel) return CANCEL;
	return CANCEL;
}

bool MainWindow::QFileWarningMacro(Status status)
{
    switch (status)
    {
        case EMPTY:
            QWarning("Empty Path");
            return false;
		case NORMAL:
			return true;
        case FILE_NOT_SELECTED:
            QWarning("File Not Selected!");
            return false;
		case FILE_NOT_FOUND:
			QWarning("File Not Found!");
			return false;
        case FILE_NOT_READ:
            QWarning("File Not Read!");
            return false;
		case FILE_NOT_READABLE:
			QWarning("File Not Readable!");
			return false;
		case FILE_NOT_WRITEABLE:
			QWarning("File Not Writable!");
			return false;
        case FILE_ALREADY_EXISTS:
            QWarning("Files Already Exist!");
            return false;
    }
    return true;
}

void MainWindow::QIllegalFilesReport(QStringList Illegal_FileNames)
{
    if (!Illegal_FileNames.empty()) // If there are illegal files returned,
    {
        for (const auto filename : Illegal_FileNames) // Display a list of illegal files
        {
            QWarning('"' + filename.toStdString() + '"' + " is not a Valid File.");
        }
    }
}

void MainWindow::on_actionOpen_File_triggered()
{
    unsigned int pre_num = 0, post_num = 0;
    pre_num = m_window->GetFilesNum();

    InputFileManager <QStringList> * inputfilemanager = new InputFileManager<QStringList>();
    QStringList InputFileNames = QFileDialog::getOpenFileNames(this, "Select Files to Open", "/", "*");
    QStringList Invalid_InputFileNames = inputfilemanager->ImportFiles(InputFileNames, m_window);
    QIllegalFilesReport(Invalid_InputFileNames);
    delete inputfilemanager;

    post_num = m_window->GetFilesNum();

    if (pre_num != post_num)                 // ListWidget Updated.
    {
        QlistWidgetUpdate();                 // Update listwidget only with legal files.
        ReadImages();						 // Now, Read Images.
        UpdatelistWidgetInfo();
    }
}

void MainWindow::on_AddButton_clicked()
{
    on_actionOpen_File_triggered(); // it works as same as on_actionOpen_File_triggered()
}

void MainWindow::on_actionSave_Files_triggered()
{
	unsigned int selected_items = ui->listWidget->selectedItems().count();
	if (selected_items == 0)
	{
		QFileWarningMacro(FILE_NOT_SELECTED);
		return; // nothing to save
	}

	UserResponse response = QRequest("Save " + std::to_string(selected_items) + " Image Files ? ");
	if (response == CANCEL) return;
	
    OutputFileManager <QString>* outputfilemanager = new OutputFileManager<QString>();
    QString Save_Folder_Path = QFileDialog::getExistingDirectory(this, tr("Select a folder"),
        ".", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    QDir dir(Save_Folder_Path);
    dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    dir.setSorting(QDir::Size | QDir::Reversed);

    Status folderpath_status = outputfilemanager->IsValidSavePath(Save_Folder_Path, m_window->GetAllSourcePathValue());

    if (QFileWarningMacro(folderpath_status))
    {
        outputfilemanager->SetOutputPath(Save_Folder_Path, m_window);
		std::set<std::string> listed_tasks;
		for (auto item : ui->listWidget->selectedItems())
		{
			std::string task = item->text().toStdString();
			listed_tasks.insert(task);
		}
        TaskManager* taskmanager = new TaskManager();
        taskmanager->ImageWriteThreading(Save_Folder_Path, listed_tasks, m_window, gui_widget_manager);
        delete taskmanager;
    }
}

void MainWindow::on_checkBox_clicked() // select all items
{
    bool is_checked = ui->checkBox->isChecked();

    if (is_checked) 
    {
		ui->listWidget->selectAll();
    }
    else 
    { 
        ui->listWidget->clearSelection(); 
    }

    UpdatelistWidgetInfo();
}

void MainWindow::on_RemoveButton_clicked()
{
    QList<QListWidgetItem*> selected_files = ui->listWidget->selectedItems();
    if (selected_files.empty())
    {
        QWarning("Nothing to Delete.");
        return;
    }
    else
    {
        QStringList dummy_filenames;
        for (const auto filename : selected_files) dummy_filenames.push_back(filename->text());
        InputFileManager <QStringList> * inputfilemanager = new InputFileManager<QStringList>();
        inputfilemanager->DeportFile(dummy_filenames, m_window);
        QlistWidgetUpdate();
        UpdatelistWidgetInfo();
        delete inputfilemanager;
    }
}

void MainWindow::ReadImages()
{
    std::set<std::string> listed_tasks = m_window->GetAllSourcePathValue();
    TaskManager* taskmanager = new TaskManager();
    taskmanager->ImageReadThreading(listed_tasks, m_window, gui_widget_manager);
    delete taskmanager;
}

void MainWindow::on_listWidget_itemSelectionChanged()
{
    UpdatelistWidgetInfo();

    if (ui->listWidget->currentItem() == NULL || ui->listWidget->selectedItems().count() == 0)
    {
        initTableWidget();
        return;
    }
    else
    {
        QString clicked_filename = ui->listWidget->currentItem()->text();
        InputFileManager <QString>* inputfilemanager = new InputFileManager<QString>();
        OutputFileManager <QString>* outputfilemanager = new OutputFileManager<QString>();

		QFileWarningMacro(inputfilemanager->GetInputFileStatus(clicked_filename, m_window));
		QFileWarningMacro(outputfilemanager->GetOutputFileStatus(clicked_filename, m_window));

        std::vector<std::string> inputfile_literal_info = inputfilemanager->GetInputFileLiteralInfo(clicked_filename, m_window);
        std::vector<std::string> outputfile_literal_info = outputfilemanager->GetOutputFileLiteralInfo(clicked_filename, m_window);

        std::vector<std::vector<std::string>> inputfile_numeric_info = inputfilemanager->GetInputFileNumericInfo(clicked_filename, m_window);
        std::vector<std::vector<std::string>> outputfile_numeric_info = outputfilemanager->GetOutputFileNumericInfo(clicked_filename, m_window);

        if (inputfile_numeric_info.empty() || outputfile_numeric_info.empty())
        {
			QFileWarningMacro(FILE_NOT_FOUND);
            return;
        }

        for (int i = 0; i < literal_tablerow; i++)
        {
            for (int j = 0; j < literal_tablecolumn; j++)
            {
                QString inputitem = QString::fromStdString(inputfile_literal_info[i]);
                QString outputitem = QString::fromStdString(outputfile_literal_info[i]);

                ui->literal_table_in->setItem(i, j, new QTableWidgetItem(inputitem));
                ui->literal_table_out->setItem(i, j, new QTableWidgetItem(outputitem));
            }
        }

        for (int i = 0; i < numeric_tablerow; i++)
        {
            for (int j = 0; j < numeric_tablecolumn; j++)
            {
                QString inputitem = QString::fromStdString((inputfile_numeric_info[i])[j]);
                QString outputitem = QString::fromStdString((outputfile_numeric_info[i])[j]);

                ui->num_table_in->setItem(i, j, new QTableWidgetItem(inputitem));
                ui->num_table_out->setItem(i, j, new QTableWidgetItem(outputitem));
            }
        }

        unsigned int InputDimension = inputfilemanager->GetDimension(clicked_filename, m_window);
        unsigned int OutputDimension = outputfilemanager->GetDimension(clicked_filename, m_window);

        ui->num_table_in->setColumnHidden(show_or_hide_col, InputDimension < numeric_tablecolumn);
        ui->num_table_out->setColumnHidden(show_or_hide_col, OutputDimension < numeric_tablecolumn);
    }
}

void MainWindow::on_action_modify_images_triggered()
{
    if (ui->listWidget->selectedItems().count() == 0)
    {
        QFileWarningMacro(FILE_NOT_SELECTED);
        return;
    }
    else
    {
        QString listWidgetInfo = ui->ListwidgetInfoText->toPlainText();
        std::vector<std::string> listed_tasks;
        for (const auto selected_file : ui->listWidget->selectedItems())
        {
            listed_tasks.push_back((selected_file->text().toStdString()));
        }
        TaskManager* taskmanager = new TaskManager();
        taskmanager->ModifyImage(listWidgetInfo, listed_tasks, m_window, gui_widget_manager);
        delete taskmanager;
    }
}

void MainWindow::on_actionMerge_Images_triggered()
{
    if (ui->listWidget->selectedItems().count() == 0)
    {
        QFileWarningMacro(FILE_NOT_SELECTED);
        return;
    }
    else
    {
        QString listWidgetInfo = ui->ListwidgetInfoText->toPlainText();
        std::vector<std::string> listed_tasks;
        for (const auto selected_file : ui->listWidget->selectedItems())
        {
            listed_tasks.push_back((selected_file->text().toStdString()));
        }
        TaskManager* taskmanager = new TaskManager();
        taskmanager->MergeImages(listWidgetInfo, listed_tasks, m_window, gui_widget_manager);
        delete taskmanager;
    }
}

void MainWindow::on_actionAbout_ImageModi_triggered()
{
    TaskManager* taskmanager = new TaskManager();
    taskmanager->ShowHelpDialog(gui_widget_manager);
    delete taskmanager;
}
