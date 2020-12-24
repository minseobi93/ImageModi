#include "progressdialog.h"
#include "ui_progressdialog.h"

ProgressDialog::ProgressDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProgressDialog)
{
    qRegisterMetaType<std::string>("std::string"); // set std::string 
	ui->setupUi(this);
	ui->progressTask->setReadOnly(true);
	ui->progressBar->setValue(0);

	(this->threaddone) = 0;
	(this->threadnum) = 0;
	(this->processed_tasksize) = 0;
	(this->total_tasksize) = 0;
}

ProgressDialog::~ProgressDialog()
{
    delete ui;
}

void ProgressDialog::Initialize()
{
	ui->progressBar->setValue(0);

	(this->threaddone) = 0;
	(this->threadnum) = 0;
	(this->processed_tasksize) = 0;
	(this->total_tasksize) = 0;
}

void ProgressDialog::SetThreadNum(unsigned int thread_num)
{
	(this->threadnum) = thread_num;
}

void ProgressDialog::SetReadConnection(ImageReadThread* readthread)
{
	QObject::disconnect(readthread, SIGNAL(R_SendStartTask(std::string)), this, SLOT(WriteTaskName(std::string)));
	QObject::disconnect(readthread, SIGNAL(R_SendFinishedTask(std::string)), this, SLOT(WritePercent(std::string)));
	QObject::disconnect(readthread, SIGNAL(R_SendJobDone(bool)), this, SLOT(ReceiveThreadDone(bool)));

	QObject::connect(readthread, SIGNAL(R_SendStartTask(std::string)), this, SLOT(WriteTaskName(std::string)));
	QObject::connect(readthread, SIGNAL(R_SendFinishedTask(std::string)), this, SLOT(WritePercent(std::string)));
	QObject::connect(readthread, SIGNAL(R_SendJobDone(bool)), this, SLOT(ReceiveThreadDone(bool)));
}

void ProgressDialog::SetWriteConnection(ImageWriteThread* writethread)
{
	QObject::disconnect(writethread, SIGNAL(W_SendStartTask(std::string)), this, SLOT(WriteTaskName(std::string)));
	QObject::disconnect(writethread, SIGNAL(W_SendFinishedTask(std::string)), this, SLOT(WritePercent(std::string)));
	QObject::disconnect(writethread, SIGNAL(W_SendJobDone(bool)), this, SLOT(ReceiveThreadDone(bool)));

	QObject::connect(writethread, SIGNAL(W_SendStartTask(std::string)), this, SLOT(WriteTaskName(std::string)));
	QObject::connect(writethread, SIGNAL(W_SendFinishedTask(std::string)), this, SLOT(WritePercent(std::string)));
	QObject::connect(writethread, SIGNAL(W_SendJobDone(bool)), this, SLOT(ReceiveThreadDone(bool)));
}

void ProgressDialog::SetTotalTaskSize(std::vector<std::string> trimmed_tasks)
{
	for (const auto filename : trimmed_tasks)
	{
		FILE* fp = fopen(filename.data(), "r");
		fseek(fp, 0, SEEK_END);
		(this->total_tasksize) += ftell(fp);
		fclose(fp);
	}
}

void ProgressDialog::WriteTaskName(std::string processing_message)
{
    ui->progressTask->setText(QString::fromStdString(processing_message));
}

void ProgressDialog::WritePercent(std::string processed_taskname)
{
	FILE* fp = fopen(processed_taskname.data(), "r");
	fseek(fp, 0, SEEK_END);
	processed_tasksize += ftell(fp);
	fclose(fp);
	int percent = 100 * (static_cast<double>(this->processed_tasksize) / static_cast<double>(this->total_tasksize));
    ui->progressBar->setValue(percent);
}

void ProgressDialog::ReceiveThreadDone(bool thread_done)
{
	if (thread_done) this->threaddone += 1;

	if (this->threadnum == this->threaddone) // job finished
	{
		this->Initialize();
		this->hide();
	}
}