#include "mergeimagedialog.h"
#include "ui_mergeimagedialog.h"

MergeImageDialog::MergeImageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MergeImageDialog)
{
    ui->setupUi(this);
	qRegisterMetaType<Status>("Status"); // set std::string for Qmetadata
	merging_thread = new MergeThread();
    this->Initialize();
}

MergeImageDialog::~MergeImageDialog()
{
    delete ui;
}

void MergeImageDialog::Initialize()
{
	this->m_window = NULL;
    save_filepath.clear();
	tasks.clear();

	ui->lineEdit->clear();
    ui->textEdit->clear();
	ui->textEdit_2->clear();

	ui->progressBar->setMaximum(100);
	ui->progressBar->setMinimum(0);
	ui->progressBar->setValue(0);
	ui->progressBar->hide();

	QObject::disconnect(merging_thread, SIGNAL(MergingStart(std::string)), this, SLOT(WriteStartMessage(std::string)));
	QObject::disconnect(merging_thread, SIGNAL(MergingFinished(Status)), this, SLOT(WriteFinishedMessage(Status)));
	QObject::connect(merging_thread, SIGNAL(MergingStart(std::string)), this, SLOT(WriteStartMessage(std::string)));
	QObject::connect(merging_thread, SIGNAL(MergingFinished(Status)), this, SLOT(WriteFinishedMessage(Status)));
}

void MergeImageDialog::WriteStartMessage(std::string message)
{
	ui->textEdit_2->setText(QString::fromStdString(message));
	ui->progressBar->setMaximum(0);
	ui->progressBar->setMinimum(0);
	ui->progressBar->setValue(0);
	ui->progressBar->show();
}

void MergeImageDialog::WriteFinishedMessage(Status status)
{
	ui->progressBar->setMaximum(100);
	ui->progressBar->setMinimum(0);
	ui->progressBar->setValue(100);

	QMessageBox *alert = new QMessageBox;
	alert->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
	
	switch (status)
	{
		case NORMAL:
			  alert->setText(QString::fromStdString("Merging Successfully Done!"));
		      break;
		case FILE_NOT_READABLE:
			  alert->setText(QString::fromStdString("Merging Failed ! Input Files Invalid! "));
			  break;
		case FILE_NOT_WRITEABLE:
			 alert->setText(QString::fromStdString("Merging Failed ! Not Writable!"));
			 break;
	};
	alert->exec();
	this->Initialize();
	this->hide();
}

void MergeImageDialog::SetModel(MainWindow_Model* m_window_from_gui)
{
    this->m_window = m_window_from_gui;
}

void MergeImageDialog::SetTask(std::vector<std::string> listed_tasks)
{
    this->tasks = listed_tasks;
}

void MergeImageDialog::Showfileinfo(std::string fileinfo)
{
    std::string text = "Merge : ";
    text.append(fileinfo);
    ui->lineEdit->setText(QString::fromStdString(text));
}

void MergeImageDialog::on_pushButton_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Save Document"), QDir::currentPath(), 
    tr("Analyze(*.img);;NiFTI(*.nii);;GIPL(*.gipl);;MetaImage(*.mha);;NRRD(*.nrrd);;VoxBo CUB(*.cub);;VTK Image(*.vtk)"));

    FileManager <QString>* filemanager = new FileManager<QString>();
    Status folderpath_status = filemanager->IsValidSavePath(filename, m_window->GetAllSourcePathValue());

    if (folderpath_status == NORMAL)
    {
        ui->textEdit->setText(filename);
        this->save_filepath = filename.toStdString();
    }
}

itk::IOComponentEnum MergeImageDialog::GetIOPixelTypeFromString(std::string DataTypeStr)
{
    if (DataTypeStr == "unsigned_char") return itk::IOComponentEnum::UCHAR;
    else if (DataTypeStr == "char") return itk::IOComponentEnum::CHAR;
    else if (DataTypeStr == "unsigned_short") return itk::IOComponentEnum::USHORT;
    else if (DataTypeStr == "short") return itk::IOComponentEnum::SHORT;
    else if (DataTypeStr == "unsigned_int") return itk::IOComponentEnum::UINT;
    else if (DataTypeStr == "int") return itk::IOComponentEnum::INT;
    else if (DataTypeStr == "unsigned_long") return itk::IOComponentEnum::ULONG;
    else if (DataTypeStr == "long") return itk::IOComponentEnum::LONG;
    else if (DataTypeStr == "float") return itk::IOComponentEnum::FLOAT;
    else if (DataTypeStr == "double") return itk::IOComponentEnum::DOUBLE;
    else { return itk::IOComponentEnum::UNKNOWNCOMPONENTTYPE; }
}

itk::IOComponentEnum MergeImageDialog::GetComponent(std::vector<std::string> listed_task)
{
    std::map<std::string, unsigned int> component_map;
    std::string chosen_compo = "";
    int max = 0;
  
    for (auto taskname : listed_task)
    {
        for (auto& file : this->m_window->AccessListedFilesVec())
        {
            if (taskname == file.GetInputPathValueAsString())
            {
                std::string datatype = file.GetInputDataTypeAsString();
                std::map<std::string, unsigned int>::iterator it = component_map.find(datatype);

                if (it == component_map.end()) //first insert
                {
                    component_map.insert(std::pair<std::string, unsigned int>(datatype, 1));
                }
                else 
                {
                    (it->second)++;
                }
            }
        }
    }

    for (const auto compo : component_map)
    {
        if (compo.second > max)
        {
            chosen_compo = compo.first;
            max = compo.second;
        }
    }

    return GetIOPixelTypeFromString(chosen_compo);
}

unsigned int MergeImageDialog::GetDimension(std::vector<std::string> listed_task)
{
    unsigned int dim = 2;

    for (auto taskname : listed_task)
    {
        for (auto& file : this->m_window->AccessListedFilesVec())
        {
            if (taskname == file.GetInputPathValueAsString())
            {
                dim = stoul(file.GetInputDimensionAsString(), nullptr, 0);
            }
        }
    }

    return dim;
}

void MergeImageDialog::on_pushButton_2_clicked()
{ 
	ImageMerge Infile, Outfile;

	Infile.status = NONE;
	for (const auto filename : this->tasks) Infile.filenames.push_back(filename);
	Infile.component = GetComponent(Infile.filenames);
	Infile.dimension = GetDimension(Infile.filenames);

	Outfile.status = NONE;
	Outfile.filenames.push_back(this->save_filepath);
	Outfile.component = Infile.component;
	Outfile.dimension = 3;

	merging_thread->SetMergeTask(Infile, Outfile);
	merging_thread->start();
}

MergeThread::MergeThread(QObject* parent) : QThread(parent) {}
MergeThread::~MergeThread(){}

void MergeThread::SetMergeTask(ImageMerge Infile, ImageMerge Outfile)
{
	in.status = Infile.status;
	in.filenames = Infile.filenames;
	in.component = Infile.component;
	in.dimension = Infile.dimension;

	out.status = Outfile.status;
	out.filenames = Outfile.filenames;
	out.component = Outfile.component;
	out.dimension = Outfile.dimension;
}

void MergeThread::run()
{
	Status status = NONE;
	emit MergingStart("Merging Images... Please Wait!");
	if (in.dimension == 2)
	{
		status = MergeScalarImage<2>(in, out);
	}
	else if (in.dimension == 3)
	{
		status =  MergeScalarImage<3>(in, out);
	}
	this->in.init(), this->out.init();
	emit MergingFinished(status);
}