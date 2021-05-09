#pragma once

#include "camera_structs.h"

class CameraGeneric
{
public:
  virtual ~CameraGeneric() {};

  /**
   * @brief Get the camera capabilities.
   * @return return capabilities within CameraCapabilities struct
   */
  virtual struct CameraCapabilities get_camera_capabilities() = 0;

  /**
   * @brief Initiate the connection with the camera.
   * Ensure to have at least an argument '--camhelp' providing other arguments
   * @param [in]argc : size of arguments array.
   * @param [in]argv : arguments array.
   * @return false on success and true on failure.
   */
  virtual bool initiate_connection(int argc, char *argv[]) = 0;

  /**
   * @brief Close the connection with the camera.
   * @return false on success and true on failure.
   */
  virtual bool close_connection() = 0;

  /**
   * @brief Get the camera information.
   * @return camera information within CameraInformation struct.
   */
  virtual struct CameraInformation get_camera_information() = 0;

  /**
   * @brief Get the current image at JPEG format.
   * @return image data within Image struct.
   */
  virtual struct Image get_current_image() = 0;

  //////////
  // ZOOM //
  //////////

  /**
   * @brief Get the current zoom in percent.
   * @return zoom percent under uint8_t.
   */
  virtual uint8_t get_zoom_percent() { return 0; }

  /**
   * @brief Get the current zoom in percent.
   * @param [in]percent : zoom position in percent to set.
   * @return false on success and true on failure.
   */
  virtual bool zoom_to(uint8_t /*percent*/) { return false; }

  /**
   * @brief Zoom in in continuous, don't forget to stop with
   * `zoom_stop` method.
   * @return false on success and true on failure.
   */
  virtual bool zoom_in() { return false; }

  /**
   * @brief Zoom out in continuous, don't forget to stop with
   * `zoom_stop` method.
   * @return false on success and true on failure.
   */
  virtual bool zoom_out() { return false; }

  /**
   * @brief Stop the zoom action.
   * @return false on success and true on failure.
   */
  virtual bool zoom_stop() { return false; }

  /////////
  // PAN //
  /////////

  /**
   * @brief Get the pan position in degree.
   * @return pan position in degree under int16_t
   */
  virtual int16_t get_pan_degree() { return 0; }

  /**
   * @brief Set absolute pan position in degree.
   * @param [in]degree : pan position in degree to set.
   * @return false on success and true on failure.
   */
  virtual bool pan_to(int16_t /*degree*/) { return false; }

  /**
   * @brief Activate continuous pan movement to the left, don't forget
   * to stop with `pan_stop` method.
   * @return false on success and true on failure.
   */
  virtual bool pan_left() { return false; }

  /**
   * @brief Activate continuous pan movement to the right, don't forget
   * to stop with `pan_stop` method.
   * @return false on success and true on failure.
   */
  virtual bool pan_right() { return false; }

  /**
   * @brief Stop the pan action.
   * @return false on success and true on failure.
   */
  virtual bool pan_stop() { return false; }

  //////////
  // TILT //
  //////////

  /**
   * @brief Get the tilt position in degree.
   * @return tilt position in degree under int16_t
   */
  virtual int16_t get_tilt_degree() { return 0; }

  /**
   * @brief Set absolute tilt position in degree.
   * @param [in]degree : tilt position in degree to set.
   * @return false on success and true on failure.
   */
  virtual bool tilt_to(int16_t /*degree*/) { return false; }

  /**
   * @brief Activate continuous tilt movement up, don't forget
   * to stop with `tilt_stop` method.
   * @return false on success and true on failure.
   */
  virtual bool tilt_up() { return false; }

  /**
   * @brief Activate continuous tilt movement down, don't forget
   * to stop with `tilt_stop` method.
   * @return false on success and true on failure.
   */
  virtual bool tilt_down() { return false; }

  /**
   * @brief Stop the pan action.
   * @return false on success and true on failure.
   */
  virtual bool tilt_stop() { return false; }

  ///////////
  // FOCUS //
  ///////////

  /**
   * @brief Set focus mode of the camera.
   * @param [in]mode : focus mode from Mode enum.
   * @return false on success and true on failure.
   */
  virtual bool set_focus_mode(enum Mode /*mode*/) { return false; }

  /**
   * @brief Get focus mode of the camera.
   * @return focus mode from Mode enum.
   */
  virtual enum Mode get_focus_mode() { return Mode::AUTO; }

  /**
   * @brief Get focus value.
   * @return focus mode value
   * TODO what is the focus value metrics
   */
  virtual int get_focus() { return 0; }

  /**
   * @brief Set focus to absolute value.
   * @param [in]depth : absolute value to move the focus.
   * @return false on succes and true on failure.
   * TODO what is the focus value metrics
   */
  virtual bool focus_to(int /*depth*/) { return false; }

  /**
   * @brief Focus in the camera.
   * @return false on succes and true on failure.
   */
  virtual bool focus_in() { return false; }

  /**
   * @brief Focus out the camera.
   * @return false on succes and true on failure.
   */
  virtual bool focus_out() { return false; }

  ///////////////////
  // WHITE BALANCE //
  ///////////////////

  /**
   * @brief Get the white balance mode.
   * @return the white balance from Mode enum.
   */
  virtual enum Mode get_white_balance_mode() { return Mode::AUTO; }

  /**
   * @brief Set white balance mode.
   * @param [in]mode : white balance mode from Mode enum.
   * @return false on success and true on failure.
   */
  virtual bool set_white_balance_mode(enum Mode /*mode*/) { return false; }

  /**
   * @brief Get color temperature in Kelvin.
   * @return Color temperature in Kelvin under uint16_t.
   */
  virtual uint16_t get_color_temperature() { return 0; }

  /**
   * @brief Set color temperature in Kelvin.
   * @param [in]kelvin : Color temperature to be set in Kelvin.
   * @return false on success and true on failure.
   */
  virtual bool set_color_temperature(uint16_t /*kelvin*/) { return false; }

  //////////////
  // EXPOSURE //
  //////////////

  /**
   * @brief Get exposure mode.
   * @return exposure mode from Mode enum.
   */
  virtual enum Mode get_exposure_mode() { return Mode::AUTO; }

  /**
   * @brief Get exposure mode.
   * @param [in]mode : exposure mode from Mode enum.
   * @return false on success and true on failure.
   */
  virtual bool set_exposure_mode(enum Mode /*mode*/) { return false; }

  /**
   * @brief Get exposure value.
   * @return Exposure value under int.
   * TODO what is unit of exposure ?
   */
  virtual int get_exposure() { return 0; }

  /**
   * @brief Get exposure value.
   * @param [in]time : Exposure to set.
   * @return false on success and true on error.
   * TODO what is unit of exposure ?
   */
  virtual bool set_exposure(int /*time*/) { return false; }

protected:
  std::string name;
  std::string manufacturer;
};
