// import 'package:flutter/foundation.dart';
// import 'package:flutter/services.dart';

// import 'secugen_windows_platform_interface.dart';

// /// An implementation of [SecugenWindowsPlatform] that uses method channels.
// class MethodChannelSecugenWindows extends SecugenWindowsPlatform {
//   /// The method channel used to interact with the native platform.
//   @visibleForTesting
//   final methodChannel = const MethodChannel('secugen_windows');

//   @override
//   Future<String?> getPlatformVersion() async {
//     final version = await methodChannel.invokeMethod<String>('getPlatformVersion');
//     return version;
//   }
// }
