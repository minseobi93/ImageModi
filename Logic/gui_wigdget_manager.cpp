#include "gui_wigdget_manager.h"

GUIWidgetManager::GUIWidgetManager()
{

}

GUIWidgetManager::~GUIWidgetManager()
{
}

void GUIWidgetManager::SetProgressBarWidgetVisible()
{
	progressdialog.show();
}

void GUIWidgetManager::NotifyProgressBarSetThreadNum(unsigned int thread_num)
{
	progressdialog.SetThreadNum(thread_num);
}

void GUIWidgetManager::NotifyProgressBarSetTotalTaskSize(std::vector<std::string> listed_task)
{
	progressdialog.SetTotalTaskSize(listed_task);
}

void GUIWidgetManager::NotifyProgressBarSetConnectionToReadThread(ImageReadThread* readthread)
{
	progressdialog.SetReadConnection(readthread);
}

void GUIWidgetManager::NotifyProgressBarSetConnectionToWriteThread(ImageWriteThread* writethread)
{
	progressdialog.SetWriteConnection(writethread);
}

void GUIWidgetManager::SetImageModifyDialogVisible()
{
	imagemodifydialog.show();
	imagemodifydialog.ShowProperTable();
}

void GUIWidgetManager::NotifyImageModiDialogSetModel(MainWindow_Model* m_window_from_gui)
{
	imagemodifydialog.SetModel(m_window_from_gui);
}

void GUIWidgetManager::NotifyImageModiDialogSetTask(std::vector<std::string> listed_task)
{
	imagemodifydialog.SetTask(listed_task);
}

void GUIWidgetManager::NotifyImageModiDialogShowFileInfo(std::string FileInfo)
{
	imagemodifydialog.Showfileinfo(FileInfo);
}

void GUIWidgetManager::SetImageMergeDialogVisible()
{
	imagemergedialog.show();
}

void GUIWidgetManager::NotifyImageMergeDialogSetModel(MainWindow_Model* m_window_from_gui)
{
	imagemergedialog.SetModel(m_window_from_gui);
}

void GUIWidgetManager::NotifyImageMergeDialogSetTask(std::vector<std::string> listed_task)
{
	imagemergedialog.SetTask(listed_task);
}

void GUIWidgetManager::NotifyImageMergeDialogShowFileInfo(std::string FileInfo)
{
	imagemergedialog.Showfileinfo(FileInfo);
}

void GUIWidgetManager::NotifyImageHelpDialog()
{
	imagehelpdialog.show();
}