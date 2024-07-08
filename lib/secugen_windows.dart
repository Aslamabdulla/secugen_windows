import 'dart:async';
import 'dart:typed_data';
import 'package:flutter/services.dart';

class SecugenWindows {
  static const MethodChannel _channel = MethodChannel('secugen_windows');

  /// Initializes the SecuGen device.
  static Future<void> initializeDevice() async {
    try {
      await _channel.invokeMethod('initializeDevice');
    } on PlatformException catch (e) {
      throw 'Failed to initialize device: ${e.message}';
    }
  }

  /// Captures a fingerprint and returns the fingerprint data as a list of bytes.
  static Future<Uint8List> captureFingerprint() async {
    try {
      final Uint8List fingerprintData = await _channel.invokeMethod('captureFingerprint');
      return fingerprintData;
    } on PlatformException catch (e) {
      throw 'Failed to capture fingerprint: ${e.message}';
    }
  }

  /// Matches the captured fingerprint data with stored fingerprint data.
  /// Returns true if the fingerprints match, false otherwise.
  static Future<bool> matchFingerprint(Uint8List fingerprintData, Uint8List storedFingerprintData) async {
    try {
      final bool match = await _channel.invokeMethod('matchFingerprint', [fingerprintData, storedFingerprintData]);
      return match;
    } on PlatformException catch (e) {
      throw 'Failed to match fingerprint: ${e.message}';
    }
  }

  /// Gets the quality of the fingerprint image.
  /// Returns an integer representing the quality of the fingerprint image.
  static Future<int> getImageQuality(Uint8List fingerprintImage) async {
    try {
      final int quality = await _channel.invokeMethod('getImageQuality', [fingerprintImage]);
      return quality;
    } on PlatformException catch (e) {
      throw 'Failed to get image quality: ${e.message}';
    }
  }
}