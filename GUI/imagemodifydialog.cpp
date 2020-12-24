#include <array>
#include "imagemodifydialog.h"
#include "ui_imagemodifydialog.h"
#include "../Logic/constant.h"

ImageModifyDialog::ImageModifyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImageModifyDialog)
{
    ui->setupUi(this);
	Initialize();
}

ImageModifyDialog::~ImageModifyDialog()
{
    delete ui;
}

void ImageModifyDialog::Initialize()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			ui->modi_out_table->setItem(i, j, new QTableWidgetItem("No Change"));
		}
	}

	ui->modi_out_table->setEditTriggers(QAbstractItemView::AllEditTriggers);
	for (int j = 0; j < 3; j++) ui->modi_out_table->item(3, j)->setCheckState(Qt::Unchecked);

	this->tasks.clear();
	this->m_window = NULL;
	this->HideTable = false;
}

void ImageModifyDialog::SetModel(MainWindow_Model* m_window_from_gui)
{
	this->m_window = m_window_from_gui;
}

void ImageModifyDialog::ShowProperTable()
{
	for (const auto taskname : this->tasks)
	{
		for (auto& File : m_window->AccessListedFilesVec())
		{
			if (taskname == File.GetInputPathValueAsString())
			{
				unsigned int InputDimension = std::stoul(File.GetInputDimensionAsString(), nullptr, 0);
				if (InputDimension == show_or_hide_col)
				{
					this->HideTable = true;
					break;
				}
			}
		}
	}

	ui->modi_out_table->setColumnHidden(show_or_hide_col, this->HideTable);
}

void ImageModifyDialog::Showfileinfo(std::string fileinfo)
{
	std::string text = "Modify : ";
	text.append(fileinfo);
	ui->selectedfileinfo->setText(QString::fromStdString(text));
}

void ImageModifyDialog::SetTask(std::vector<std::string> listed_tasks)
{
	this->tasks = listed_tasks;
}

void ImageModifyDialog::on_buttonBox_accepted()
{
	std::string selected_datatype_str = ui->datatype_2->currentText().toStdString();
	bool compo_valid[4][3] = { {false, false, false}, {false, false, false}, {false, false, false}, {true, true, true}};

	unsigned int imagesize[3] = {0,0,0};
	double imagespacing[3] = {0,0,0};
	double imageorigin[3] = {0,0,0};
	bool imageflip[3] = { false, false, false };
	
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			QTableWidgetItem* item = (ui->modi_out_table->item(i, j));
			if (item->text().toStdString() == "No Change") continue;
			bool conversion_ok = true;

			if (i == 0)
			{
				int dec = item->text().toInt(&conversion_ok, 10); // to decimal
				if (conversion_ok) imagesize[j] = dec;
			}
			else if (i == 1)
			{
				double spacing_val = item->text().toDouble(&conversion_ok);
				if (conversion_ok) imagespacing[j] = spacing_val;
			}
			else if (i == 2)
			{
				double origin_val = item->text().toDouble(&conversion_ok);
				if (conversion_ok) imageorigin[j] = origin_val;
			}
			else if (i == 3)
			{
				if (item->checkState() == Qt::Checked) imageflip[j] = true;
			}
			
			compo_valid[i][j] = conversion_ok;
		}
	}

	for (const auto taskname : this->tasks)
	{
		for (auto& File : m_window->AccessListedFilesVec())
		{
			if (taskname == File.GetInputPathValueAsString())
			{
				File.SetOutputDataType(true, selected_datatype_str);
				File.SetOutputSize(compo_valid[0][0], compo_valid[0][1], compo_valid[0][2],imagesize[0], imagesize[1], imagesize[2]);
				File.SetOutputSpacing(compo_valid[1][0], compo_valid[1][1], compo_valid[1][2], imagespacing[0], imagespacing[1], imagespacing[2]);
				File.SetOutputOrigin(compo_valid[2][0], compo_valid[2][1], compo_valid[2][2], imageorigin[0], imageorigin[1], imageorigin[2]);
				File.SetOutputFlipAxis(compo_valid[3][0], compo_valid[3][1], compo_valid[3][2], imageflip[0], imageflip[1], imageflip[2]);
				break;
			}
		}
	}

	this->Initialize();
}

void ImageModifyDialog::on_ImageModifyDialog_rejected()
{
	this->Initialize();
}

void ImageModifyDialog::on_pushButton_2_clicked() // reset
{
	this->Initialize();
}

void ImageModifyDialog::on_modi_out_table_cellChanged(int row, int column)
{
	QTableWidgetItem* changed_item = ui->modi_out_table->item(row, column);
	if (changed_item == NULL) return;

	QString changed_item_str = changed_item->text();
	bool is_conversion_ok = true;
	
	if (row == 0) changed_item_str.toInt(&is_conversion_ok, 10);
	if (row == 1 || row == 2) changed_item_str.toDouble(&is_conversion_ok);
	if (changed_item_str.toStdString() == "No Change" || changed_item_str.toStdString() == "Auto") is_conversion_ok = true;

	if (is_conversion_ok)
	{
		if (row == 0)
		{
			for (int i = 1; i < 3; i++)
			{
				ui->modi_out_table->item(i, column)->setText(QString::fromStdString("Auto"));
				ui->modi_out_table->item(i, column)->setFlags(false && Qt::ItemIsEditable);
			}
		}
		if (row == 3)
		{
			if (changed_item->checkState() == Qt::Checked)
			{
				changed_item->setText(QString::fromStdString("Flip"));
			}
			else
			{
				changed_item->setText(QString::fromStdString("No Change"));
			}
		}
	 }
	 else
	 {
		changed_item->setText(QString::fromStdString("Invalid Input"));
	 }
}
