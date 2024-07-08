#include "include/secugen_windows/secugen_windows_plugin_c_api.h"

#include <flutter/plugin_registrar_windows.h>

#include "secugen_windows_plugin.h"

void SecugenWindowsPluginCApiRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  secugen_windows::SecugenWindowsPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
