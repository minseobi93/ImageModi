#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

#include <QDialog>
#include <QProgressBar>
#include "windows.h"
#include "../Logic/ImageProcessing/imageReadThread.h"
#include "../Logic/ImageProcessing/imageWriteThread.h"

class ImageReadThread;
class ImageWriteThread;

namespace Ui {
class ProgressDialog;
}

class ProgressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProgressDialog(QWidget *parent = nullptr);
    ~ProgressDialog();

	void SetThreadNum(unsigned int threadnum);
    void SetTotalTaskSize(std::vector<std::string> trimmed_tasks);
    void SetReadConnection(ImageReadThread* readthread);
    void SetWriteConnection(ImageWriteThread* writethread);

private:

	void Initialize();
	unsigned int threaddone;
	unsigned int threadnum;
    DWORDLONG processed_tasksize;
    DWORDLONG total_tasksize;
    Ui::ProgressDialog *ui;

private slots:

    void WriteTaskName(std::string);
    void WritePercent(std::string);
	void ReceiveThreadDone(bool);
};

#endif // PROGRESSDIALOG_H
