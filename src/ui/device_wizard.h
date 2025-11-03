#ifndef DEVICE_WIZARD_H
#define DEVICE_WIZARD_H

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QString>
#include <vector>
#include <memory>
#include "../filesystems/filesystem_interface.h"

/**
 * @class DeviceWizard
 * @brief Qt6 Widget for device selection and filesystem detection
 *
 * Responsibilities:
 * - Display available block devices (/dev/sda*, /dev/nvme*, etc.)
 * - Auto-detect filesystem type (NTFS, APFS, ext4)
 * - Provide UI for initiating recovery scan
 * - Communicate with RecoveryEngine
 *
 * Features:
 * - Device dropdown with refresh capability
 * - Filesystem type indicator (auto-detected)
 * - Capacity display
 * - "Start Scan" button to initiate recovery
 *
 * Signals:
 * - deviceSelected(QString) → emitted when device selected
 * - scanRequested(QString) → emitted when scan button clicked
 */
class DeviceWizard : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Constructor for DeviceWizard
     * @param parent Parent widget
     */
    explicit DeviceWizard(QWidget *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~DeviceWizard() override = default;

    /**
     * @brief Get currently selected device path
     * @return Device path (e.g., /dev/sda1, /dev/nvme0n1p1)
     */
    QString GetSelectedDevice() const;

    /**
     * @brief Get detected filesystem type for selected device
     * @return Filesystem type (NTFS, APFS, ext4, or "Unknown")
     */
    QString GetDetectedFileSystem() const;

    /**
     * @brief Refresh device list from system
     * Scans /dev for available block devices
     */
    void RefreshDeviceList();

    /**
     * @brief Enable/disable UI controls
     * @param enabled true to enable, false to disable
     *
     * Used to disable UI during scan operations
     */
    void SetUIEnabled(bool enabled);

signals:
    /**
     * @brief Signal emitted when device selection changes
     * @param device_path Path to selected device
     */
    void deviceSelected(const QString &device_path);

    /**
     * @brief Signal emitted when scan is requested
     * @param device_path Path to device to scan
     */
    void scanRequested(const QString &device_path);

private slots:
    /**
     * @brief Slot for device dropdown change
     * @param index Index in dropdown
     */
    void OnDeviceChanged(int index);

    /**
     * @brief Slot for "Refresh Devices" button
     */
    void OnRefreshDevices();

    /**
     * @brief Slot for "Start Scan" button
     */
    void OnStartScan();

private:
    // UI Components
    QGroupBox *device_group_;           // Main group box
    QLabel *device_label_;              // "Select Device:" label
    QComboBox *device_combo_;           // Device dropdown
    QPushButton *refresh_btn_;          // "Refresh" button

    QLabel *filesystem_label_;          // "Filesystem Type:" label
    QLabel *filesystem_value_;          // Filesystem type display
    QLabel *capacity_label_;            // "Capacity:" label
    QLabel *capacity_value_;            // Capacity display

    QPushButton *scan_btn_;             // "Start Scan" button
    QLabel *status_label_;              // Status message display

    // Layout
    QVBoxLayout *main_layout_;
    QHBoxLayout *device_layout_;
    QHBoxLayout *filesystem_layout_;
    QHBoxLayout *capacity_layout_;
    QHBoxLayout *button_layout_;

    // Internal State
    std::vector<QString> device_paths_;  // List of available devices
    QString current_device_;             // Currently selected device
    QString detected_filesystem_;        // Detected filesystem type

    /**
     * @brief Initialize UI components
     * Called in constructor
     */
    void InitializeUI();

    /**
     * @brief Connect UI signals and slots
     * Called in constructor
     */
    void ConnectSignals();

    /**
     * @brief Detect filesystem type for device
     * @param device_path Path to device
     * @return Detected filesystem type (NTFS/APFS/ext4/Unknown)
     *
     * TODO: Integrate with actual parser detection:
     * - NTFSParser::CanHandle()
     * - APFSParser::CanHandle()
     * - EXT4Parser::CanHandle()
     */
    QString DetectFileSystem(const QString &device_path);

    /**
     * @brief Get device capacity/size
     * @param device_path Path to device
     * @return Human-readable capacity string (e.g., "1 TB", "500 GB")
     *
     * TODO: Implement device size detection
     */
    QString GetDeviceCapacity(const QString &device_path);

    /**
     * @brief Update UI display based on selected device
     * @param device_path Path to device
     */
    void UpdateDeviceInfo(const QString &device_path);
};

#endif  // DEVICE_WIZARD_H
