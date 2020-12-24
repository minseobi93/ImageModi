/*=========================================================================
 * Author : Minseob Lee
 * 2020.11.13 Created abastract class "file_manager"
 * 2020.11.16 Added function for getting ram memory as byte
 * 2020.11.17 Added function for getting Logical CPU
 * 2020.11.17 Added function for getting the ideal thread num
 *=========================================================================*/
#pragma once

#ifndef _TASKMANAGER_
#define _TASKMANAGER_

#include <vector>
#include <set>
#include <iostream>
#include "status_def.h"
#include "windows.h"
#include "constant.h"

#include "../Logic/ImageProcessing/ImageReadThread.h"
#include "../Logic/ImageProcessing/ImageWriteThread.h"
#include "../GUI/mergeimagedialog.h"
#include "../Logic/gui_wigdget_manager.h"
#include "../Logic/IO/file_manager.h"
#include "../Model/mainwindow_model.h"

class TaskManager 
{

public:

	TaskManager::TaskManager(){}

	DWORDLONG TaskManager::GetAvailRAMByte()
	{
		MEMORYSTATUSEX memInfo;
		memInfo.dwLength = sizeof(MEMORYSTATUSEX);
		GlobalMemoryStatusEx(&memInfo);
		return memInfo.ullAvailPhys;
	}

	unsigned int TaskManager::GetLogicalCoreNum()
	{
		SYSTEM_INFO sysinfo;
		GetSystemInfo(&sysinfo);
		return sysinfo.dwNumberOfProcessors;
	}

	unsigned int TaskManager::GetOptimalThreadNum()
	{
		DWORDLONG AvailRAMByte = GetAvailRAMByte();
		DWORDLONG thread_num = GetLogicalCoreNum();

		while (thread_num > 1)
		{
			DWORDLONG EXPECTED_RAMSIZE = thread_num * EXPECTED_MEMORY_PER_THREAD;
			if (EXPECTED_RAMSIZE < AvailRAMByte) break; // found a proper thread num 
			thread_num--; // decrease the thread num
		}
		return thread_num;
	}

	std::vector<std::string> TaskManager::TrimAnalyzeFormatTask(std::set<std::string>& listed_filenames)
	{
		std::vector<std::string> trimmed_list_vec;
		std::set<std::string> filename_without_ext_set;
	
		FileManager<QString> * filemanager = new FileManager<QString>();

		for (const auto filename : listed_filenames)
		{
			std::string filename_without_ext_str = filemanager->GetFileNameWithoutExtension(filename);
			
			if (filename_without_ext_set.find(filename_without_ext_str) == filename_without_ext_set.end()) // Not found, first time to be enlisted
			{
				filename_without_ext_set.insert(filename_without_ext_str);
				trimmed_list_vec.push_back(filename.data());
			}
		}

		return trimmed_list_vec;
	}

	void TaskManager::ImageReadThreading(std::set<std::string>& listed_tasks, MainWindow_Model* m_window_from_gui, GUIWidgetManager* gui_widget_manager)
	{
		std::vector<std::string> trimmed_tasks = TrimAnalyzeFormatTask(listed_tasks);
		unsigned int task_num = trimmed_tasks.size();
		unsigned int thread_num = GetOptimalThreadNum();

		ImageReadThread* rthreads = new ImageReadThread[thread_num];

		gui_widget_manager->SetProgressBarWidgetVisible();
		gui_widget_manager->NotifyProgressBarSetThreadNum(thread_num);
		gui_widget_manager->NotifyProgressBarSetTotalTaskSize(trimmed_tasks);

		for (unsigned int i = 0; i < task_num; i++)
		{
			unsigned int assigned_index = (i % thread_num);
			rthreads[assigned_index]->SetTasksPerThread(trimmed_tasks[i]);
		}

		for (unsigned int i = 0; i < thread_num; i++)
		{
			gui_widget_manager->NotifyProgressBarSetConnectionToReadThread(&rthreads[i]);
			rthreads[i]->SetModel(m_window_from_gui);
			rthreads[i]->start();
		}
	}

	void TaskManager::ImageWriteThreading(QString Save_Folder_Path, std::set<std::string>& listed_tasks, MainWindow_Model* m_window_from_gui, GUIWidgetManager* gui_widget_manager)
	{
		std::string save_folder_path_str = Save_Folder_Path.toStdString();
		std::vector<std::string> trimmed_tasks = TrimAnalyzeFormatTask(listed_tasks);

		unsigned int task_num = trimmed_tasks.size();
		unsigned int thread_num = GetOptimalThreadNum();

		ImageWriteThread* wthreads = new ImageWriteThread[thread_num];

		gui_widget_manager->SetProgressBarWidgetVisible();
		gui_widget_manager->NotifyProgressBarSetThreadNum(thread_num);
		gui_widget_manager->NotifyProgressBarSetTotalTaskSize(trimmed_tasks);

		for (unsigned int i = 0; i < task_num; i++)
		{
			unsigned int assigned_index = (i % thread_num);
			wthreads[assigned_index]->SetTasksPerThread(trimmed_tasks[i]);
		}

		for (unsigned int i = 0; i < thread_num; i++)
		{
			gui_widget_manager->NotifyProgressBarSetConnectionToWriteThread(&wthreads[i]);
			wthreads[i]->SetModel(m_window_from_gui);
			wthreads[i]->ConfigureTask();
			wthreads[i]->start();
		}
	}

	void TaskManager::ModifyImage(QString listWidgetInfo, std::vector<std::string> listed_tasks, MainWindow_Model* m_window_from_gui, GUIWidgetManager* gui_widget_manager)
	{
		gui_widget_manager->NotifyImageModiDialogSetModel(m_window_from_gui);
		gui_widget_manager->NotifyImageModiDialogSetTask(listed_tasks);
		gui_widget_manager->SetImageModifyDialogVisible();
		gui_widget_manager->NotifyImageModiDialogShowFileInfo(listWidgetInfo.toStdString());
	}

	void TaskManager::MergeImages(QString listWidgetInfo, std::vector<std::string> listed_tasks, MainWindow_Model* m_window_from_gui, GUIWidgetManager* gui_widget_manager)
	{
		std::set<std::string> task_set;
		for (const auto item : listed_tasks) task_set.insert(item);
		std::vector<std::string> trimmed_tasks = TrimAnalyzeFormatTask(task_set);

		gui_widget_manager->NotifyImageMergeDialogSetModel(m_window_from_gui);
		gui_widget_manager->NotifyImageMergeDialogSetTask(trimmed_tasks);
		gui_widget_manager->SetImageMergeDialogVisible();
		gui_widget_manager->NotifyImageMergeDialogShowFileInfo(listWidgetInfo.toStdString());
	}

	void TaskManager::ShowHelpDialog (GUIWidgetManager* gui_widget_manager)
	{
		gui_widget_manager->NotifyImageHelpDialog();
	}

};

#endif 