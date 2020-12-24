#ifndef IMAGEMODIFYDIALOG_H
#define IMAGEMODIFYDIALOG_H

#include <iostream>
#include <vector>
#include <QDialog>

#include "../Model/mainwindow_model.h"

namespace Ui {
class ImageModifyDialog;
}

class ImageModifyDialog : public QDialog
{
    Q_OBJECT

public:

	void SetModel(MainWindow_Model* m_window_from_gui);
	void SetTask(std::vector<std::string> listed_tasks);
	void Showfileinfo(std::string fileinfo);
	void ShowProperTable();
    explicit ImageModifyDialog(QWidget *parent = nullptr);
    ~ImageModifyDialog();

private slots:

    void on_buttonBox_accepted();

    void on_pushButton_2_clicked();

    void on_ImageModifyDialog_rejected();

    void on_modi_out_table_cellChanged(int row, int column);

private:

	void Initialize();
	bool HideTable;
	MainWindow_Model* m_window;
	Ui::ImageModifyDialog *ui;
	std::vector<std::string> tasks;

};

#endif // IMAGEMODIFYDIALOG_H
