#ifndef FLUTTER_PLUGIN_SECUGEN_WINDOWS_PLUGIN_H_
#define FLUTTER_PLUGIN_SECUGEN_WINDOWS_PLUGIN_H_

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>

#include <memory>
#include <vector>
#include <thread>

namespace secugen_windows {

class SecugenWindowsPlugin : public flutter::Plugin {
 public:
  static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

  SecugenWindowsPlugin();

  virtual ~SecugenWindowsPlugin();

  // Disallow copy and assign.
  SecugenWindowsPlugin(const SecugenWindowsPlugin&) = delete;
  SecugenWindowsPlugin& operator=(const SecugenWindowsPlugin&) = delete;

  // Called when a method is called on this plugin's channel from Dart.
  void HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue> &method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

 private:
  // Method to initialize the SecuGen device
  void InitializeDevice();

  // Method to capture fingerprint
  std::vector<uint8_t> CaptureFingerprint();

  // Method to match fingerprint
  bool MatchFingerprint(const std::vector<uint8_t>& fingerprintData, const std::vector<uint8_t>& storedFingerprintData);

  // Method to get image quality
  int GetImageQuality(const std::vector<uint8_t>& fingerprintImage);

  // Method to handle Auto-On event
  void HandleAutoOnEvent();

  // Helper method to convert image to vector
  std::vector<uint8_t> ConvertImageToVector(BYTE* image, int size);

  // Auto-On thread
  std::thread auto_on_thread_;
  bool auto_on_running_;
};

}  // namespace secugen_windows

#endif  // FLUTTER_PLUGIN_SECUGEN_WINDOWS_PLUGIN_H_