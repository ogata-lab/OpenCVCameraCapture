// -*- C++ -*-
/*!
 * @file  CameraCapture.cpp
 * @brief Camera Capture Component
 * @date $Date$
 *
 * $Id$
 */

#include "CameraCapture.h"

// Module specification
// <rtc-template block="module_spec">
static const char* cameracapture_spec[] =
  {
    "implementation_id", "CameraCapture",
    "type_name",         "CameraCapture",
    "description",       "Camera Capture Component",
    "version",           "1.0.0",
    "vendor",            "Sugar Sweet Robotics",
    "category",          "Experimenta",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.debug", "0",
    "conf.default.width", "320",
    "conf.default.height", "240",
    // Widget
    "conf.__widget__.debug", "text",
    "conf.__widget__.width", "text",
    "conf.__widget__.height", "text",
    // Constraints
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
CameraCapture::CameraCapture(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_outOut("out", m_out)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
CameraCapture::~CameraCapture()
{
}



RTC::ReturnCode_t CameraCapture::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  
  // Set OutPort buffer
  addOutPort("out", m_outOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("debug", m_debug, "0");
  bindParameter("width", m_width, "320");
  bindParameter("height", m_height, "240");
  // </rtc-template>


  m_pCapture = NULL;

  if(m_pCapture == NULL) {
    std::cout << "[OpenCVCameraCapture] Capture From CAM" << std::endl;
    m_pCapture = cvCaptureFromCAM( 0 );
    //m_pCapture = new cv::VideoCapture(0);
    if(!m_pCapture) {
      std::cout << "[OpenCVCameraCapture] Failed Capture From CAM" << std::endl;
      return RTC::RTC_ERROR;
    }
    std::cout << "[OpenCVCameraCapture] CAM Open Success" << std::endl;
  }

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t CameraCapture::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t CameraCapture::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t CameraCapture::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t CameraCapture::onActivated(RTC::UniqueId ec_id)
{
  //cvSetCaptureProperty (m_pCapture, CV_CAP_PROP_FRAME_WIDTH,  m_width);
  //  cvSetCaptureProperty (m_pCapture, CV_CAP_PROP_FRAME_HEIGHT, m_height);

  //cv::Mat mat;
  //  *m_pCapture >> mat;

 
  IplImage* frame = cvQueryFrame(m_pCapture);
  if (frame == NULL) {
    std::cerr << "[OpenCVCameraCapture] Failed To Query Frame." << std::endl;
    return RTC::RTC_ERROR;
  }
  /*
  if (m_width != frame->width || m_height != frame->height) {
    std::cerr << "[OpenCVCameraCapture] Invalid Width/Height setting." << std::endl;
    return RTC::RTC_ERROR;
    }*/

  m_out.width = frame->width;
  m_out.height = frame->height;
  int len = frame->width * frame->height;
  m_out.pixels.length(frame->width * frame->height * frame->nChannels);

  return RTC::RTC_OK;
}


RTC::ReturnCode_t CameraCapture::onDeactivated(RTC::UniqueId ec_id)
{
  //cvReleaseCapture(&m_pCapture);
  m_pCapture = NULL;
  return RTC::RTC_OK;
}


RTC::ReturnCode_t CameraCapture::onExecute(RTC::UniqueId ec_id)
{
  std::cout << "onExecute" << std::endl;
  IplImage* frame = cvQueryFrame(m_pCapture);
  if (frame == NULL) {
    std::cerr << "[OpenCVCameraCapture] Failed To Query Frame." << std::endl;
    return RTC::RTC_ERROR;
  }
  std::cout << "Captured(" << frame->width << "x" << frame->height << "x" << frame->nChannels << ")" << std::endl;
  int len = frame->width* frame->height;
  m_out.width = frame->width;
  m_out.height = frame->height;
  m_out.pixels.length(frame->width * frame->height * frame->nChannels);
  if (frame->origin == IPL_ORIGIN_TL) {
    memcpy((void*)&(m_out.pixels[0]), frame->imageData, len * frame->nChannels);
  } else {
    std::cout << "Fliped" << std::endl;
    for (int i = 0;i < len;i++ ) {

      int index = i * frame->nChannels;
      for(int j = 0;j < frame->nChannels;j++) {
	m_out.pixels[index + j] = frame->imageData[(len-i-1)*frame->nChannels +j];
      }
    }
  }
 m_outOut.write();

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t CameraCapture::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t CameraCapture::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t CameraCapture::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t CameraCapture::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t CameraCapture::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void CameraCaptureInit(RTC::Manager* manager)
  {
    coil::Properties profile(cameracapture_spec);
    manager->registerFactory(profile,
                             RTC::Create<CameraCapture>,
                             RTC::Delete<CameraCapture>);
  }
  
};


