#include "device_wizard.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QStringList>
#include <algorithm>

/**
 * @file device_wizard.cpp
 * @brief Implementation of DeviceWizard Qt6 widget
 *
 * Provides UI for:
 * - Device selection and listing
 * - Filesystem type auto-detection
 * - Capacity display
 * - Scan initiation
 */

DeviceWizard::DeviceWizard(QWidget *parent)
    : QWidget(parent),
      device_group_(nullptr),
      device_label_(nullptr),
      device_combo_(nullptr),
      refresh_btn_(nullptr),
      filesystem_label_(nullptr),
      filesystem_value_(nullptr),
      capacity_label_(nullptr),
      capacity_value_(nullptr),
      scan_btn_(nullptr),
      status_label_(nullptr),
      main_layout_(nullptr),
      device_layout_(nullptr),
      filesystem_layout_(nullptr),
      capacity_layout_(nullptr),
      button_layout_(nullptr),
      current_device_(""),
      detected_filesystem_("Unknown") {

    // Initialize UI
    InitializeUI();
    ConnectSignals();

    // Load initial device list
    RefreshDeviceList();
}

void DeviceWizard::InitializeUI() {
    /**
     * TODO: Implement full UI initialization with:
     * - Device dropdown population
     * - Filesystem detection icons
     * - Capacity display formatting
     * - Status indicator
     * - Buttons styling
     */

    // Create main layout
    main_layout_ = new QVBoxLayout(this);

    // === Device Selection Group ===
    device_group_ = new QGroupBox("Device Selection", this);
    QVBoxLayout *group_layout = new QVBoxLayout(device_group_);

    // Device row: label + combo + refresh button
    device_layout_ = new QHBoxLayout();
    device_label_ = new QLabel("Select Device:", this);
    device_combo_ = new QComboBox(this);
    refresh_btn_ = new QPushButton("Refresh", this);
    refresh_btn_->setMaximumWidth(100);

    device_layout_->addWidget(device_label_);
    device_layout_->addWidget(device_combo_);
    device_layout_->addWidget(refresh_btn_);
    group_layout->addLayout(device_layout_);

    // Filesystem type display
    filesystem_layout_ = new QHBoxLayout();
    filesystem_label_ = new QLabel("Filesystem Type:", this);
    filesystem_value_ = new QLabel("Unknown", this);
    filesystem_value_->setStyleSheet("QLabel { color: #0066CC; font-weight: bold; }");

    filesystem_layout_->addWidget(filesystem_label_);
    filesystem_layout_->addWidget(filesystem_value_);
    filesystem_layout_->addStretch();
    group_layout->addLayout(filesystem_layout_);

    // Capacity display
    capacity_layout_ = new QHBoxLayout();
    capacity_label_ = new QLabel("Capacity:", this);
    capacity_value_ = new QLabel("- GB", this);
    capacity_value_->setStyleSheet("QLabel { color: #333333; }");

    capacity_layout_->addWidget(capacity_label_);
    capacity_layout_->addWidget(capacity_value_);
    capacity_layout_->addStretch();
    group_layout->addLayout(capacity_layout_);

    main_layout_->addWidget(device_group_);

    // === Action Buttons ===
    button_layout_ = new QHBoxLayout();
    scan_btn_ = new QPushButton("Start Scan", this);
    scan_btn_->setMinimumHeight(40);
    scan_btn_->setStyleSheet(
        "QPushButton {"
        "  background-color: #0066CC;"
        "  color: white;"
        "  font-weight: bold;"
        "  border-radius: 5px;"
        "  padding: 10px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #0052A3;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #003D7A;"
        "}"
    );

    button_layout_->addStretch();
    button_layout_->addWidget(scan_btn_, 1);
    button_layout_->addStretch();
    main_layout_->addLayout(button_layout_);

    // === Status Display ===
    status_label_ = new QLabel("Ready to scan", this);
    status_label_->setStyleSheet("QLabel { color: #008000; font-size: 11px; }");
    main_layout_->addWidget(status_label_);

    main_layout_->addStretch();
    this->setLayout(main_layout_);

    // Set window properties
    this->setWindowTitle("Device Wizard");
    this->setMinimumWidth(500);
    this->setMinimumHeight(300);
}

void DeviceWizard::ConnectSignals() {
    /**
     * TODO: Connect UI signals to slots and external signals
     */

    // Combo box signals
    connect(device_combo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &DeviceWizard::OnDeviceChanged);

    // Button signals
    connect(refresh_btn_, &QPushButton::clicked,
            this, &DeviceWizard::OnRefreshDevices);

    connect(scan_btn_, &QPushButton::clicked,
            this, &DeviceWizard::OnStartScan);
}

QString DeviceWizard::GetSelectedDevice() const {
    return current_device_;
}

QString DeviceWizard::GetDetectedFileSystem() const {
    return detected_filesystem_;
}

void DeviceWizard::RefreshDeviceList() {
    /**
     * TODO: Implement actual device enumeration:
     * - Linux: scan /dev/sd*, /dev/nvme*, /dev/mmcblk*
     * - macOS: use diskutil list or IOKit
     * - Windows: use GetLogicalDrives() or WMI
     *
     * Current stub: add mock devices for testing
     */

    device_combo_->clear();
    device_paths_.clear();

    // Mock devices for testing (TODO: replace with real device discovery)
    QStringList mock_devices = {
        "/dev/sda1 - SATA Disk (500 GB)",
        "/dev/nvme0n1p1 - NVMe SSD (1 TB)",
        "/dev/sdb1 - USB Drive (32 GB)"
    };

    device_paths_ = {
        "/dev/sda1",
        "/dev/nvme0n1p1",
        "/dev/sdb1"
    };

    for (const auto &device : mock_devices) {
        device_combo_->addItem(device);
    }

    if (!device_paths_.empty()) {
        device_combo_->setCurrentIndex(0);
        OnDeviceChanged(0);
    }

    status_label_->setText("Devices refreshed");
    status_label_->setStyleSheet("QLabel { color: #008000; font-size: 11px; }");
}

void DeviceWizard::SetUIEnabled(bool enabled) {
    /**
     * TODO: Implement UI disable during scan operations
     */

    device_combo_->setEnabled(enabled);
    refresh_btn_->setEnabled(enabled);
    scan_btn_->setEnabled(enabled);
}

void DeviceWizard::OnDeviceChanged(int index) {
    /**
     * TODO: Update UI based on selected device
     * - Detect filesystem
     * - Get capacity
     * - Update display
     */

    if (index >= 0 && index < static_cast<int>(device_paths_.size())) {
        current_device_ = device_paths_[index];
        UpdateDeviceInfo(current_device_);
        emit deviceSelected(current_device_);
    }
}

void DeviceWizard::OnRefreshDevices() {
    /**
     * TODO: Re-scan devices and update list
     */

    status_label_->setText("Scanning devices...");
    status_label_->setStyleSheet("QLabel { color: #FF8800; font-size: 11px; }");

    RefreshDeviceList();

    status_label_->setText("Devices refreshed successfully");
    status_label_->setStyleSheet("QLabel { color: #008000; font-size: 11px; }");
}

void DeviceWizard::OnStartScan() {
    /**
     * TODO: Validate device and emit scan signal
     * - Check device accessibility
     * - Verify filesystem type
     * - Disable UI during scan
     * - Emit scanRequested signal
     */

    if (current_device_.isEmpty()) {
        QMessageBox::warning(this, "No Device Selected",
                           "Please select a device before starting scan.");
        return;
    }

    status_label_->setText("Scan in progress...");
    status_label_->setStyleSheet("QLabel { color: #FF8800; font-size: 11px; }");

    SetUIEnabled(false);
    emit scanRequested(current_device_);
}

QString DeviceWizard::DetectFileSystem(const QString &device_path) {
    /**
     * TODO: Integrate with actual parser detection:
     * 1. Try NTFSParser::CanHandle(device_path)
     * 2. Try APFSParser::CanHandle(device_path)
     * 3. Try EXT4Parser::CanHandle(device_path)
     * 4. Return detected type or "Unknown"
     *
     * Current stub: simple pattern matching
     */

    if (device_path.contains("sda") || device_path.contains("sdb")) {
        return "ext4";
    } else if (device_path.contains("nvme")) {
        return "ext4";
    } else if (device_path.contains("Disk0")) {
        return "APFS";
    }

    return "Unknown";
}

QString DeviceWizard::GetDeviceCapacity(const QString &device_path) {
    /**
     * TODO: Implement platform-specific capacity detection:
     * - Linux: read /sys/block/*/size
     * - macOS: use diskutil info
     * - Windows: GetDiskFreeSpaceEx()
     *
     * Current stub: return mock values
     */

    if (device_path.contains("sda")) {
        return "500 GB";
    } else if (device_path.contains("nvme")) {
        return "1 TB";
    } else if (device_path.contains("sdb")) {
        return "32 GB";
    }

    return "Unknown";
}

void DeviceWizard::UpdateDeviceInfo(const QString &device_path) {
    /**
     * TODO: Update UI display with device information
     */

    detected_filesystem_ = DetectFileSystem(device_path);
    filesystem_value_->setText(detected_filesystem_);

    QString capacity = GetDeviceCapacity(device_path);
    capacity_value_->setText(capacity);

    // Update filesystem color based on type
    if (detected_filesystem_ == "NTFS") {
        filesystem_value_->setStyleSheet("QLabel { color: #0066CC; font-weight: bold; }");
    } else if (detected_filesystem_ == "APFS") {
        filesystem_value_->setStyleSheet("QLabel { color: #A2AAAD; font-weight: bold; }");
    } else if (detected_filesystem_ == "ext4") {
        filesystem_value_->setStyleSheet("QLabel { color: #FF6600; font-weight: bold; }");
    } else {
        filesystem_value_->setStyleSheet("QLabel { color: #999999; font-weight: bold; }");
    }
}
