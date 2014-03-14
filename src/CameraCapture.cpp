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

#include <opencv/cv.h>
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
  
  m_alive = true;
  m_active = false;

  //initCapture();
  return RTC::RTC_OK;

}

void CameraCapture::initCapture() {
  m_pCapture = NULL;

  if(m_pCapture == NULL) {
    std::cout << "[OpenCVCameraCapture] Capture From CAM" << std::endl;
    m_pCapture = cvCreateCameraCapture(0);
    if(!m_pCapture) {
      std::cout << "[OpenCVCameraCapture] Failed Capture From CAM" << std::endl;
      return;
    }
    std::cout << "[OpenCVCameraCapture] CAM Open Success" << std::endl;
  }
  m_pOutFrame = cvCreateImage(cvSize(m_width, m_height), IPL_DEPTH_8U, 3);

  cvSetCaptureProperty (m_pCapture, CV_CAP_PROP_FRAME_WIDTH,  m_width);
  m_old_width = m_width;
  cvSetCaptureProperty (m_pCapture, CV_CAP_PROP_FRAME_HEIGHT, m_height);
  m_old_height = m_height;

  m_out.width = m_width;
  m_out.height = m_height;
  m_out.pixels.length(m_width * m_height * 3);

  //cvNamedWindow("Capture", CV_WINDOW_AUTOSIZE);
}


RTC::ReturnCode_t CameraCapture::onFinalize()
{
  m_alive = false;

  //finiCapture();
  return RTC::RTC_OK;
}


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
void CameraCapture::capture()
{
  if (m_old_width != m_width || m_old_height != m_height) {
    cvSetCaptureProperty (m_pCapture, CV_CAP_PROP_FRAME_WIDTH,  m_width);
    m_old_width = m_width;
    cvSetCaptureProperty (m_pCapture, CV_CAP_PROP_FRAME_HEIGHT, m_height);
    m_old_height = m_height;

    cvReleaseImage(&m_pOutFrame);
    m_pOutFrame = cvCreateImage(cvSize(m_width, m_height), IPL_DEPTH_8U, 3);

    m_out.width = m_width;
    m_out.height = m_height;
    m_out.pixels.length(m_width * m_height * 3);
  }


  m_pFrame = cvQueryFrame(m_pCapture);
  if (m_pFrame == NULL) {
    std::cerr << "[OpenCVCameraCapture] Failed To Query Frame." << std::endl;
  }
  if (m_debug) {
    //cvShowImage("Capture", m_pFrame);
    cvWaitKey(1);
  }

  if (isActive()) {
    int len = m_width * m_height * 3;

    cvCvtColor(m_pFrame, m_pOutFrame, CV_BGRA2RGB);

    memcpy((void*)&(m_out.pixels[0]), m_pOutFrame->imageData, len);// * m_pFrame->nChannels);
    m_outOut.write();
  }
}

RTC::ReturnCode_t CameraCapture::onActivated(RTC::UniqueId ec_id)
{
  m_active = true;
  return RTC::RTC_OK;
}


void CameraCapture::finiCapture()
{
  std::cout << "Finilize Capture..." << std::endl;
  cvReleaseCapture(&m_pCapture);
  //cvDestroyWindow("Capture");

}

RTC::ReturnCode_t CameraCapture::onDeactivated(RTC::UniqueId ec_id)
{
  m_active = false;
  return RTC::RTC_OK;
}


RTC::ReturnCode_t CameraCapture::onExecute(RTC::UniqueId ec_id)
{
  //std::cout << "onExecute" << std::endl;
  //capture();
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


