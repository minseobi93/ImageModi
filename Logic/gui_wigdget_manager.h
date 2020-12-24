#ifndef GUIWIDGWTMAGER_H
#define GUIWIDGWTMAGER_H

#include "../Model/mainwindow_model.h"
#include "../GUI/progressdialog.h"
#include "../GUI/imagemodifydialog.h"
#include "../GUI/mergeimagedialog.h"
#include "../GUI/helpdialog.h"
#include "../Logic/ImageProcessing/imageReadThread.h"
#include "../Logic/ImageProcessing/imageWriteThread.h"

class GUIWidgetManager 
{
public:

	GUIWidgetManager();
	~GUIWidgetManager();

	void SetProgressBarWidgetVisible();
	void NotifyProgressBarSetThreadNum(unsigned int threadnum);
	void NotifyProgressBarSetTotalTaskSize(std::vector<std::string> listed_task);
	void NotifyProgressBarSetConnectionToReadThread(ImageReadThread* readthread);
	void NotifyProgressBarSetConnectionToWriteThread(ImageWriteThread* writethread);
	
	void SetImageModifyDialogVisible();
	void NotifyImageModiDialogSetModel(MainWindow_Model* m_window_from_gui);
	void NotifyImageModiDialogSetTask(std::vector<std::string> listed_task);
	void NotifyImageModiDialogShowFileInfo(std::string FileInfo);

	void SetImageMergeDialogVisible();
	void NotifyImageMergeDialogSetModel(MainWindow_Model* m_window_from_gui);
	void NotifyImageMergeDialogSetTask(std::vector<std::string> listed_task);
	void NotifyImageMergeDialogShowFileInfo(std::string FileInfo);

	void NotifyImageHelpDialog();

private :

	ProgressDialog progressdialog;
	ImageModifyDialog imagemodifydialog;
	MergeImageDialog imagemergedialog;
	HelpDialog imagehelpdialog;
};

#endif // GUIWIDGWTMAGER_H
