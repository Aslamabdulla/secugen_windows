#include "secugen_windows_plugin.h"

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <windows.h>
#include "include/secugen_windows/sgfplib.h"
#include <vector>
#include <thread>
using namespace flutter:
namespace secugen_windows {

void SecugenWindowsPlugin::RegisterWithRegistrar(
    flutter::PluginRegistrarWindows *registrar) {
  auto channel = std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
      registrar->messenger(), "secugen_windows",
      &flutter::StandardMethodCodec::GetInstance());

  auto plugin = std::make_unique<SecugenWindowsPlugin>();

  channel->SetMethodCallHandler(
      [plugin_pointer = plugin.get()](const auto &call, auto result) {
        plugin_pointer->HandleMethodCall(call, std::move(result));
      });

  registrar->AddPlugin(std::move(plugin));
}

SecugenWindowsPlugin::SecugenWindowsPlugin() : auto_on_running_(true) {
  // Initialize the device
  InitializeDevice();

  // Start Auto-On thread
  auto_on_thread_ = std::thread([this]() {
    while (auto_on_running_) {
      HandleAutoOnEvent();
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  });
}

SecugenWindowsPlugin::~SecugenWindowsPlugin() {
  auto_on_running_ = false;
  if (auto_on_thread_.joinable()) {
    auto_on_thread_.join();
  }
}

void SecugenWindowsPlugin::HandleMethodCall(
    const flutter::MethodCall<flutter::EncodableValue> &method_call,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
  if (method_call.method_name().compare("initializeDevice") == 0) {
    InitializeDevice();
    result->Success();
  } else if (method_call.method_name().compare("captureFingerprint") == 0) {
    auto fingerprintData = CaptureFingerprint();
    result->Success(flutter::EncodableValue(fingerprintData));
  } else if (method_call.method_name().compare("matchFingerprint") == 0) {
    const auto* args = std::get_if<flutter::EncodableList>(method_call.arguments());
    if (args && args->size() == 2 && std::holds_alternative<std::vector<uint8_t>>((*args)[0]) && std::holds_alternative<std::vector<uint8_t>>((*args)[1])) {
      auto fingerprintData = std::get<std::vector<uint8_t>>((*args)[0]);
      auto storedFingerprintData = std::get<std::vector<uint8_t>>((*args)[1]);
      bool match = MatchFingerprint(fingerprintData, storedFingerprintData);
      result->Success(flutter::EncodableValue(match));
    } else {
      result->Error("Invalid Arguments", "Expected a list with two elements of type uint8_t array.");
    }
  } else if (method_call.method_name().compare("getImageQuality") == 0) {
    const auto* args = std::get_if<flutter::EncodableList>(method_call.arguments());
    if (args && args->size() == 1 && std::holds_alternative<std::vector<uint8_t>>((*args)[0])) {
      auto fingerprintImage = std::get<std::vector<uint8_t>>((*args)[0]);
      int quality = GetImageQuality(fingerprintImage);
      result->Success(flutter::EncodableValue(quality));
    } else {
      result->Error("Invalid Arguments", "Expected a list with one element of type uint8_t array.");
    }
  } else {
    result->NotImplemented();
  }
}

void SecugenWindowsPlugin::InitializeDevice() {
  // Initialize the SecuGen device
  SGFPM_Init();
  SGFPM_OpenDevice(SG_DEV_AUTO);
}

std::vector<uint8_t> SecugenWindowsPlugin::CaptureFingerprint() {
  // Capture fingerprint logic
  BYTE* imageBuffer = new BYTE[IMAGE_SIZE];
  SGFPM_GetImage(imageBuffer);
  auto fingerprintData = ConvertImageToVector(imageBuffer, IMAGE_SIZE);
  delete[] imageBuffer;
  return fingerprintData;
}

bool SecugenWindowsPlugin::MatchFingerprint(const std::vector<uint8_t>& fingerprintData, const std::vector<uint8_t>& storedFingerprintData) {
  // Match fingerprint logic
  bool match = false;
  // ... (matching logic using SGFPM_MatchTemplate)
  return match;
}

int SecugenWindowsPlugin::GetImageQuality(const std::vector<uint8_t>& fingerprintImage) {
  // Get image quality logic
  DWORD quality;
  SGFPM_GetImageQuality(fingerprintImage.data(), &quality);
  return static_cast<int>(quality);
}

void SecugenWindowsPlugin::HandleAutoOnEvent() {
  // Handle Auto-On event logic
  // Check if a finger is placed on the device
  if (SGFPM_IsFingerPresent()) {
    // Capture fingerprint
    auto fingerprintData = CaptureFingerprint();
    // Send fingerprint data to Dart side for further processing
    // (This part needs to be implemented to communicate with Dart side)
  }
}

std::vector<uint8_t> SecugenWindowsPlugin::ConvertImageToVector(BYTE* image, int size) {
  return std::vector<uint8_t>(image, image + size);
}

}  // namespace secugen_windows