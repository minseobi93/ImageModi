#ifndef MERGEIMAGEDIALOG_H
#define MERGEIMAGEDIALOG_H

#include <QThread>
#include <condition_variable>
#include <map>
#include <QMessagebox>
#include <QFileDialog>
#include <QDialog>
#include <QProgressBar>
#include "../Model/mainwindow_model.h"
#include "../Logic/ImageProcessing/imageMergeBase.h"
#include "../Logic/IO/file_manager.h"
#include "../Logic/status_def.h"

namespace Ui {
class MergeImageDialog;
}

class MergeThread : public QThread
{
	Q_OBJECT

public:

	explicit  MergeThread(QObject* parent = 0);
	~MergeThread();

	void SetMergeTask(ImageMerge Infile, ImageMerge Outfile);
	void run();

	ImageMerge in, out;

signals:

	void MergingStart(std::string message);
	void MergingFinished(Status status);
};

class MergeImageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MergeImageDialog(QWidget *parent = nullptr);
    ~MergeImageDialog();
   
    void SetModel(MainWindow_Model* m_window_from_gui);
    void SetTask(std::vector<std::string> listed_tasks);
    void Showfileinfo(std::string fileinfo);

private slots:

	void WriteStartMessage(std::string message);

	void WriteFinishedMessage(Status status);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:

    void Initialize();
    itk::IOComponentEnum GetIOPixelTypeFromString(std::string DataTypeStr);
    itk::IOComponentEnum GetComponent(std::vector<std::string> listed_task);
    unsigned int GetDimension(std::vector<std::string> listed_task);

	MainWindow_Model* m_window;
	MergeThread* merging_thread;
    Ui::MergeImageDialog *ui;
    std::vector<std::string> tasks;
    std::string save_filepath;
};


#endif // MERGEIMAGEDIALOG_H
