cmake_minimum_required (VERSION 2.6) 
project (Ringeader) 
  
find_package(OpenGL REQUIRED) 
find_package(Qt4 REQUIRED QtGui QtOpenGL QtXml Phonon) 
find_package(QtMobility REQUIRED MultimediaKit) 
include_directories(${QTMOBILITY_INCLUDE_DIRS}) 
find_package(OpenCV REQUIRED core imgproc video highgui) 
# find_package(OpenMP REQUIRED) 
  
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}") 
  
  
file(GLOB DAD_SOURCES   
        *.cpp *.c  *.vert *.frag *.cu) 
file(GLOB DAD_HEADERS   
        *.h *.hpp) 
file(GLOB DAD_UIS       
        *.ui) 
    
  
SET(QEXTSERIALPORT_SOURCES "QExtSerialPort-1.2rc/qextserialport.cpp;QExtSerialPort-1.2rc/qextserialenumerator.cpp") 
  
IF(WIN32) 
    SET(QEXTSERIALPORT_LIBS "setupapi;advapi32;user32") 
    SET(QEXTSERIALPORT_SOURCES "${QEXTSERIALPORT_SOURCES};QExtSerialPort-1.2rc/qextserialenumerator_win.cpp;QExtSerialPort-1.2rc/qextserialport_win.cpp") 
ELSEIF(APPLE) 
    SET(QEXTSERIALPORT_LIBS "-framework IOKit -framework CoreFoundation") 
    SET(QEXTSERIALPORT_SOURCES "${QEXTSERIALPORT_SOURCES};QExtSerialPort-1.2rc/qextserialenumerator_osx.cpp;QExtSerialPort-1.2rc/qextserialport_unix.cpp") 
ELSE() 
    SET(QEXTSERIALPORT_SOURCES "${QEXTSERIALPORT_SOURCES};QExtSerialPort-1.2rc/qextserialenumerator_linux.cpp;QExtSerialPort-1.2rc/qextserialport_unix.cpp") 
  
ENDIF() 
file(GLOB QEXTSERIALPORT_HEADERS QExtSerialPort-1.2rc/*.h) 
  
SOURCE_GROUP(QEXTSERIALPORT FILES ${QEXTSERIALPORT_SOURCES} ${QEXTSERIALPORT_HEADERS}) 
QT4_WRAP_CPP( QEXTSERIALPORT_HEADERS_MOC ${QEXTSERIALPORT_HEADERS} ) 
  
######## Qt stuff ######## 
ADD_DEFINITIONS(${QT_DEFINITIONS}) 
include_directories(${QT_INCLUDES}) 
include(${QT_USE_FILE}) 
  
QT4_WRAP_UI( MY_UI_HDRS ${DAD_UIS} ) 
QT4_WRAP_CPP( MY_MOC_SRCS ${DAD_HEADERS} ${QEXTSERIALPORT_HEADERS_MOC} ) 
SOURCE_GROUP(QT_MOC_SRCS FILES ${MY_MOC_SRCS} ) 
  
INCLUDE_DIRECTORIES( ${CMAKE_BINARY_DIR} ) #qt wrapping creates some sources 
######################### 
  
include_directories(${OpenGL_INCLUDE_DIRS}) 
IF(APPLE) 
   include_directories( 
                /usr/local/include/ 
                /usr/local/include/eigen3/ 
                ) 
   INCLUDE_DIRECTORIES ( /System/Library/Frameworks ) 
   FIND_LIBRARY(COCOA_LIBRARY Cocoa) 
   FIND_LIBRARY(GLUT_LIBRARY GLUT ) 
   FIND_LIBRARY(OpenGL_LIBRARY OpenGL ) 
#   MARK_AS_ADVANCED (COCOA_LIBRARY 
#                     GLUT_LIBRARY 
#                     OpenGL_LIBRARY) 
   SET(OpenGL_LIBS ${COCOA_LIBRARY} ${GLUT_LIBRARY} ${OpenGL_LIBRARY}) 
    
   FIND_LIBRARY(QGLVIEWER QGLViewer) 
    IF(NOT QGLVIEWER) 
        message(FATAL_ERROR "QGLVIEWER not found") 
    ENDIF(NOT QGLVIEWER) 
ENDIF (APPLE) 
  
set(Boost_USE_STATIC_LIBS        ON) 
FIND_PACKAGE(Boost COMPONENTS system REQUIRED) 
include_directories(${Boost_INCLUDE_DIR}) 
  
SET(TESSERACT_DIR_HINT "" CACHE PATH "TESSERACT lookup dir") 
FIND_LIBRARY(TESSERACT_LIB tesseract PATHS ${TESSERACT_DIR_HINT} "${TESSERACT_DIR_HINT}/lib" "${TESSERACT_DIR_HINT}/.libs") 
FIND_PATH(TESSERACT_INCLUDE "tesseract/baseapi.h" PATHS ${TESSERACT_DIR_HINT} "${TESSERACT_DIR_HINT}/include" "${TESSERACT_DIR_HINT}/../include") 
include_directories(${TESSERACT_INCLUDE}) 
  
  
# FIND_PACKAGE(FFmpeg QUIET) 
# IF(NOT FFmpeg) 
        # SET(FFMPEG_DIR_HINT "" CACHE PATH "FFMPEG lookup dir") 
        # FIND_LIBRARY(FFMPEG_libavcodec avcodec PATHS  ${FFMPEG_DIR_HINT}) 
        # FIND_LIBRARY(FFMPEG_libavfilter avfilter PATHS  ${FFMPEG_DIR_HINT}) 
        # FIND_LIBRARY(FFMPEG_libavdevice avdevice PATHS  ${FFMPEG_DIR_HINT}) 
        # FIND_LIBRARY(FFMPEG_libavformat avformat PATHS  ${FFMPEG_DIR_HINT}) 
        # FIND_LIBRARY(FFMPEG_libavutil avutil PATHS  ${FFMPEG_DIR_HINT}) 
        # FIND_LIBRARY(FFMPEG_libswscale swscale PATHS  ${FFMPEG_DIR_HINT}) 
        # FIND_LIBRARY(FFMPEG_libswresample swresample PATHS  ${FFMPEG_DIR_HINT}) 
        # FIND_PATH(FFMPEG_INCLUDE_DIRS "libavcodec/avcodec.h" PATHS ${FFMPEG_DIR_HINT} "${FFMPEG_DIR_HINT}/../" "${FFMPEG_DIR_HINT}/../include") 
        # SET(FFMPEG_LIBRARIES "${FFMPEG_libavcodec};${FFMPEG_libavfilter};${FFMPEG_libavdevice};${FFMPEG_libavformat};${FFMPEG_libavutil};${FFMPEG_libswscale};${FFMPEG_libswresample}") 
        # MESSAGE(STATUS "ffmpeg lib ${FFMPEG_LIBRARIES}") 
# ENDIF() 
# include_directories(${FFMPEG_INCLUDE_DIRS}) 
  
############# Find MathGL ############# 
FIND_LIBRARY(MathGL_LIB mgl) 
FIND_PATH(MathGL_INCLUDE "mgl2/mgl.h") 
if(MathGL_LIB) 
        include_directories(${MathGL_INCLUDE}) 
        ADD_DEFINITIONS("-DHAVE_MATHGL") 
else() 
        set(MathGL_LIB "") 
        set(MathGL_INCLUDE "") 
        message(STATUS "not using MathGL") 
endif() 
####################################### 
  
############ Find ESPEAK TTS ############ 

find_path(LIBESPEAK_INCLUDE_DIRS 
          NAMES speak_lib.h
          HINTS /usr/include/espeak)
find_library(LIBESPEAK_LIBRARIES
             NAMES espeak
             HINTS /usr/lib/ /usr/x86_64-linux-gnu/
             PATH_SUFFIXES lib)

######################################## 
  
  
add_executable(Ringeader 
        ${DAD_SOURCES} 
        ${DAD_HEADERS} 
        ${MY_UI_HDRS} 
        ${MY_MOC_SRCS} 
    ${MY_CUDA_COMPILED_FILES} 
    ${QEXTSERIALPORT_SOURCES} ${QEXTSERIALPORT_HEADERS} 
    ) 
  
  
target_link_libraries(Ringeader 
        ${OpenGL_LIBS} 
        ${QT_LIBRARIES} 
        ${QGLVIEWER} 
    ${Boost} 
    ${OpenCV_LIBS} 
        ${TESSERACT_LIB} 
    ${FFMPEG_LIBRARIES} 
    ${MathGL_LIB} 
   # ${Flite_LIBS} 
    #${ESPEAK_LIBRARIES}
    ${LIBESPEAK_LIBRARIES} 
    ${QEXTSERIALPORT_LIBS} 
    ${QTMOBILITY_LIBRARIES} 
    udev 
         
) 
 
 
 
 
 
 
 
