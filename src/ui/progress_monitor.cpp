#include "progress_monitor.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QTime>
#include <QDateTime>

/**
 * @file progress_monitor.cpp
 * @brief Implementation of ProgressMonitor Qt6 widget
 *
 * Provides real-time progress display during filesystem scans:
 * - Progress bar (0-100%)
 * - Current operation label
 * - File counters
 * - Scan speed
 * - Elapsed and estimated time
 */

ProgressMonitor::ProgressMonitor(QWidget *parent)
    : QWidget(parent),
      operation_label_(nullptr),
      progress_bar_(nullptr),
      files_label_(nullptr),
      files_value_(nullptr),
      deleted_label_(nullptr),
      deleted_value_(nullptr),
      speed_label_(nullptr),
      speed_value_(nullptr),
      time_label_(nullptr),
      time_value_(nullptr),
      eta_label_(nullptr),
      eta_value_(nullptr),
      status_label_(nullptr),
      main_layout_(nullptr),
      operation_layout_(nullptr),
      files_layout_(nullptr),
      deleted_layout_(nullptr),
      speed_layout_(nullptr),
      time_layout_(nullptr),
      eta_layout_(nullptr),
      timer_(nullptr),
      start_time_ms_(0),
      last_update_ms_(0),
      total_files_processed_(0),
      total_files_found_(0),
      total_deleted_files_(0),
      current_speed_mbps_(0.0) {

    InitializeUI();

    // Initialize timer for elapsed time tracking
    timer_ = new QTimer(this);
    connect(timer_, &QTimer::timeout, this, &ProgressMonitor::UpdateElapsedTime);
    timer_->setInterval(100);  // Update every 100ms
}

void ProgressMonitor::InitializeUI() {
    /**
     * TODO: Implement full progress UI with:
     * - Progress bar styling (smooth animation)
     * - Color-coded labels (green = good, yellow = warning)
     * - Font sizing and alignment
     * - Icon indicators (if applicable)
     */

    main_layout_ = new QVBoxLayout(this);

    // === Operation Display ===
    operation_layout_ = new QHBoxLayout();
    operation_label_ = new QLabel("Initializing scan...", this);
    operation_label_->setStyleSheet("QLabel { font-size: 12px; font-weight: bold; }");
    operation_layout_->addWidget(operation_label_);
    main_layout_->addLayout(operation_layout_);

    // === Progress Bar ===
    progress_bar_ = new QProgressBar(this);
    progress_bar_->setMinimum(0);
    progress_bar_->setMaximum(100);
    progress_bar_->setValue(0);
    progress_bar_->setStyleSheet(
        "QProgressBar {"
        "  border: 2px solid #CCCCCC;"
        "  border-radius: 5px;"
        "  background-color: #EEEEEE;"
        "  text-align: center;"
        "}"
        "QProgressBar::chunk {"
        "  background-color: #0066CC;"
        "  border-radius: 3px;"
        "}"
    );
    progress_bar_->setMinimumHeight(30);
    main_layout_->addWidget(progress_bar_);

    // === File Counters ===
    files_layout_ = new QHBoxLayout();
    files_label_ = new QLabel("Recoverable Files:", this);
    files_value_ = new QLabel("0 found", this);
    files_value_->setStyleSheet("QLabel { color: #0066CC; font-weight: bold; }");
    files_layout_->addWidget(files_label_);
    files_layout_->addWidget(files_value_);
    files_layout_->addStretch();
    main_layout_->addLayout(files_layout_);

    // === Deleted File Counter ===
    deleted_layout_ = new QHBoxLayout();
    deleted_label_ = new QLabel("Deleted Files:", this);
    deleted_value_ = new QLabel("0 recoverable", this);
    deleted_value_->setStyleSheet("QLabel { color: #FF6600; font-weight: bold; }");
    deleted_layout_->addWidget(deleted_label_);
    deleted_layout_->addWidget(deleted_value_);
    deleted_layout_->addStretch();
    main_layout_->addLayout(deleted_layout_);

    // === Scan Speed ===
    speed_layout_ = new QHBoxLayout();
    speed_label_ = new QLabel("Scan Speed:", this);
    speed_value_ = new QLabel("0.0 MB/s", this);
    speed_value_->setStyleSheet("QLabel { color: #008000; }");
    speed_layout_->addWidget(speed_label_);
    speed_layout_->addWidget(speed_value_);
    speed_layout_->addStretch();
    main_layout_->addLayout(speed_layout_);

    // === Elapsed Time ===
    time_layout_ = new QHBoxLayout();
    time_label_ = new QLabel("Elapsed Time:", this);
    time_value_ = new QLabel("00:00:00", this);
    time_value_->setStyleSheet("QLabel { color: #333333; }");
    time_layout_->addWidget(time_label_);
    time_layout_->addWidget(time_value_);
    time_layout_->addStretch();
    main_layout_->addLayout(time_layout_);

    // === Estimated Time ===
    eta_layout_ = new QHBoxLayout();
    eta_label_ = new QLabel("Estimated Time:", this);
    eta_value_ = new QLabel("Calculating...", this);
    eta_value_->setStyleSheet("QLabel { color: #666666; }");
    eta_layout_->addWidget(eta_label_);
    eta_layout_->addWidget(eta_value_);
    eta_layout_->addStretch();
    main_layout_->addLayout(eta_layout_);

    // === Status Display ===
    status_label_ = new QLabel("Ready to scan", this);
    status_label_->setStyleSheet("QLabel { color: #008000; font-size: 10px; }");
    main_layout_->addWidget(status_label_);

    main_layout_->addStretch();
    this->setLayout(main_layout_);

    // Set window properties
    this->setWindowTitle("Progress Monitor");
    this->setMinimumWidth(500);
    this->setMinimumHeight(350);
}

void ProgressMonitor::ResetProgress() {
    /**
     * TODO: Reset all counters and displays for new scan
     */

    progress_bar_->setValue(0);
    operation_label_->setText("Initializing scan...");
    files_value_->setText("0 found");
    deleted_value_->setText("0 recoverable");
    speed_value_->setText("0.0 MB/s");
    time_value_->setText("00:00:00");
    eta_value_->setText("Calculating...");
    status_label_->setText("Scan in progress");
    status_label_->setStyleSheet("QLabel { color: #FF8800; font-size: 10px; }");

    total_files_processed_ = 0;
    total_files_found_ = 0;
    total_deleted_files_ = 0;
    current_speed_mbps_ = 0.0;

    start_time_ms_ = QDateTime::currentMSecsSinceEpoch();
    last_update_ms_ = start_time_ms_;
}

void ProgressMonitor::StartTimer() {
    /**
     * TODO: Start elapsed time tracking timer
     */

    start_time_ms_ = QDateTime::currentMSecsSinceEpoch();
    timer_->start();
}

void ProgressMonitor::StopTimer() {
    /**
     * TODO: Stop elapsed time tracking timer
     */

    if (timer_->isActive()) {
        timer_->stop();
    }
}

void ProgressMonitor::SetProgress(int progress) {
    /**
     * TODO: Update progress bar with smooth animation
     */

    if (progress >= 0 && progress <= 100) {
        progress_bar_->setValue(progress);
    }
}

void ProgressMonitor::SetCurrentOperation(const QString &operation) {
    /**
     * TODO: Update operation label and timestamp
     */

    operation_label_->setText(operation);
    operation_label_->setStyleSheet("QLabel { font-size: 12px; font-weight: bold; color: #0066CC; }");
}

void ProgressMonitor::SetFileCount(int processed, int found) {
    /**
     * TODO: Update file counter display
     */

    total_files_processed_ = processed;
    total_files_found_ = found;

    QString count_str = QString::number(found);
    if (found > 1) {
        count_str += " files found";
    } else if (found == 1) {
        count_str += " file found";
    } else {
        count_str = "0 found";
    }

    files_value_->setText(count_str);
}

void ProgressMonitor::SetDeletedCount(int deleted_count) {
    /**
     * TODO: Update deleted file counter
     */

    total_deleted_files_ = deleted_count;

    QString deleted_str = QString::number(deleted_count);
    if (deleted_count > 1) {
        deleted_str += " deleted, recoverable";
    } else if (deleted_count == 1) {
        deleted_str += " deleted, recoverable";
    } else {
        deleted_str = "0 recoverable";
    }

    deleted_value_->setText(deleted_str);
    deleted_value_->setStyleSheet(
        deleted_count > 0
        ? "QLabel { color: #FF6600; font-weight: bold; }"
        : "QLabel { color: #CCCCCC; font-weight: bold; }"
    );
}

void ProgressMonitor::SetSpeed(double speed_mbps) {
    /**
     * TODO: Update scan speed display with formatting
     */

    current_speed_mbps_ = speed_mbps;

    QString speed_str = QString::number(speed_mbps, 'f', 1);
    speed_str += " MB/s";
    speed_value_->setText(speed_str);
}

void ProgressMonitor::OnScanCompleted(bool success, int total_files, int total_deleted) {
    /**
     * TODO: Finalize progress display and show completion status
     */

    StopTimer();
    progress_bar_->setValue(100);

    if (success) {
        status_label_->setText(
            QString("✓ Scan completed successfully | %1 files recovered | %2 deleted")
            .arg(total_files).arg(total_deleted)
        );
        status_label_->setStyleSheet("QLabel { color: #008000; font-size: 10px; font-weight: bold; }");
        operation_label_->setText("Scan completed");
        operation_label_->setStyleSheet("QLabel { font-size: 12px; font-weight: bold; color: #008000; }");
    } else {
        status_label_->setText("✗ Scan failed or cancelled");
        status_label_->setStyleSheet("QLabel { color: #CC0000; font-size: 10px; font-weight: bold; }");
    }
}

void ProgressMonitor::UpdateElapsedTime() {
    /**
     * TODO: Update elapsed time display every 100ms
     */

    qint64 current_ms = QDateTime::currentMSecsSinceEpoch();
    qint64 elapsed_ms = current_ms - start_time_ms_;

    time_value_->setText(FormatTime(elapsed_ms));

    // Calculate and update ETA
    int progress = progress_bar_->value();
    if (progress > 0 && progress < 100) {
        eta_value_->setText(CalculateETA(progress, elapsed_ms));
    }
}

QString ProgressMonitor::FormatTime(qint64 milliseconds) const {
    /**
     * TODO: Format milliseconds to HH:MM:SS
     */

    qint64 total_seconds = milliseconds / 1000;
    int hours = total_seconds / 3600;
    int minutes = (total_seconds % 3600) / 60;
    int seconds = total_seconds % 60;

    return QString("%1:%2:%3")
        .arg(hours, 2, 10, QChar('0'))
        .arg(minutes, 2, 10, QChar('0'))
        .arg(seconds, 2, 10, QChar('0'));
}

QString ProgressMonitor::CalculateETA(int progress, qint64 elapsed_ms) const {
    /**
     * TODO: Calculate estimated time remaining
     * Formula: ETA = (elapsed_ms * (100 - progress)) / progress
     */

    if (progress <= 0) {
        return "Calculating...";
    }

    double eta_ms = (static_cast<double>(elapsed_ms) * (100.0 - progress)) / progress;
    return FormatTime(static_cast<qint64>(eta_ms));
}
