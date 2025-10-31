#pragma once

#include <string>
#include <vector>
#include <memory>

/**
 * @brief Core recovery engine for RecoverySoftNetz
 *
 * Orchestrates file recovery operations across different file systems
 * and coordinates with AI/ML components for optimized recovery.
 */
class RecoveryEngine
{
public:
  /**
   * @brief Constructor
   */
  RecoveryEngine();

  /**
   * @brief Destructor
   */
  ~RecoveryEngine();

  /**
   * @brief Start scanning a device for recoverable files
   *
   * @param device_path Path to device (e.g., /dev/disk1, C:\, /dev/sda)
   * @return True if scan started successfully
   */
  bool StartScan(const std::string& device_path);

  /**
   * @brief Stop ongoing scan
   *
   * @return True if scan was stopped successfully
   */
  bool StopScan();

  /**
   * @brief Get current scan progress (0-100)
   *
   * @return Progress percentage
   */
  int GetProgress() const;

  /**
   * @brief Get total recovered files found so far
   *
   * @return Number of recoverable files detected
   */
  int GetRecoveredFileCount() const;

private:
  std::string current_device_;
  bool is_scanning_;
  int progress_;
  int recovered_files_;
};
