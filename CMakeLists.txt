cmake_minimum_required(VERSION 3.5)

project(ImageModi)

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# The source code for the model
SET(Model_CPP
  Model/mainwindow_model.cpp
)

# The headers for the model
SET(Model_HEADERS
  Model/mainwindow_model.h
)

# The source code for the view
SET(GUI_CPP
  GUI/mainwindow.cpp
  GUI/progressdialog.cpp
  GUI/imagemodifydialog.cpp
  GUI/mergeimagedialog.cpp
  GUI/helpdialog.cpp
)

# The headers for the view
SET(GUI_HEADERS
  GUI/mainwindow.h
  GUI/progressdialog.h
  GUI/imagemodifydialog.h
  GUI/mergeimagedialog.h
  GUI/helpdialog.h
)

# The ui for the view
SET(GUI_UI
  GUI/mainwindow.ui
  GUI/progressdialog.ui
  GUI/imagemodifydialog.ui
  GUI/mergeimagedialog.ui
  GUI/helpdialog.ui
)

# The Files for the logic

SET(Logic_CPP
  Logic/mainlogic.cpp
  Logic/gui_wigdget_manager.cpp
  Logic/ImageProcessing/imageReadThread.cpp
  Logic/ImageProcessing/imageWriteThread.cpp
)

SET(Logic_HEADERS
   Logic/constant.h
   Logic/status_def.h
   Logic/task_manager.h 
   Logic/gui_wigdget_manager.h
   Logic/IO/file_manager.h
   Logic/IO/inputfile_manager.h
   Logic/IO/outputfile_manager.h
   Logic/ImageProcessing/itkCommonEnums.h
   Logic/ImageProcessing/imageReadBase.h
   Logic/ImageProcessing/imageReadThread.h
   Logic/ImageProcessing/imageWriteBase.h
   Logic/ImageProcessing/imageWriteThread.h
   Logic/ImageProcessing/imageMergeBase.h
)

SET(ImageModi_MAIN_SRC
   Logic/main.cpp
 )

# Model library
ADD_LIBRARY(model ${Model_CPP} ${Model_HEADERS})
target_link_libraries(model)

# View library
ADD_LIBRARY(view ${GUI_CPP} ${GUI_HEADERS} ${GUI_UI})
target_link_libraries(view PRIVATE Qt5::Widgets)

# Logic library
ADD_LIBRARY(logic ${Logic_CPP} ${Logic_HEADERS})
target_link_libraries(logic PRIVATE Qt5::Widgets)

SET(ImageModi_INTERNAL_LIBS model view logic)

add_executable (ImageModi ${ImageModi_MAIN_SRC} icons/icon.rc)
target_link_libraries(ImageModi PRIVATE Qt5::Widgets ${ImageModi_INTERNAL_LIBS} ${ITK_LIBRARIES})