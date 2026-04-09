#pragma once

/**
   * Log the converted string to the log as an error.
   * 
   * @param averror The return value of an FFmpeg API function. If the value is an AVERROR, it will
   *                be logged.
   * @returns true if the return value was an error. Otherwise returns false if the return value was
   *          an expected condition.
   */
bool logAVERROR(int averr);