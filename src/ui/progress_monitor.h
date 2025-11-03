#ifndef PROGRESS_MONITOR_H
#define PROGRESS_MONITOR_H

#include <QWidget>
#include <QProgressBar>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QString>
#include <QTimer>

/**
 * @class ProgressMonitor
 * @brief Qt6 Widget for real-time scan progress display
 *
 * Displays scan progress with:
 * - Overall progress bar (0-100%)
 * - Current operation label
 * - File count (processed vs. found)
 * - Deleted file count
 * - Scan speed (MB/s)
 * - Elapsed time
 * - Estimated time remaining
 *
 * Features:
 * - Real-time update via signals
 * - Smooth progress animation
 * - Recoverable file counter
 * - Deleted file counter
 * - Speed display
 * - Time estimation
 *
 * Signals:
 * - None (receives updates only)
 *
 * Slots:
 * - SetProgress(int) → update progress bar
 * - SetCurrentOperation(QString) → update operation label
 * - SetFileCount(int, int) → update file counters
 * - SetDeletedCount(int) → update deleted file count
 * - SetSpeed(double) → update scan speed
 * - OnScanCompleted() → finalize display
 */
class ProgressMonitor : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Constructor for ProgressMonitor
     * @param parent Parent widget
     */
    explicit ProgressMonitor(QWidget *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~ProgressMonitor() override = default;

    /**
     * @brief Reset progress display for new scan
     */
    void ResetProgress();

    /**
     * @brief Start timer for elapsed time tracking
     */
    void StartTimer();

    /**
     * @brief Stop timer
     */
    void StopTimer();

public slots:
    /**
     * @brief Set overall progress percentage
     * @param progress Value 0-100
     */
    void SetProgress(int progress);

    /**
     * @brief Update current operation label
     * @param operation Description of current operation
     *
     * Examples:
     * - "Reading superblock..."
     * - "Parsing inode table..."
     * - "Extracting directory entries..."
     */
    void SetCurrentOperation(const QString &operation);

    /**
     * @brief Update file count display
     * @param processed Number of files processed so far
     * @param found Number of recoverable files found
     */
    void SetFileCount(int processed, int found);

    /**
     * @brief Update deleted file counter
     * @param deleted_count Number of deleted files found
     */
    void SetDeletedCount(int deleted_count);

    /**
     * @brief Update scan speed display
     * @param speed_mbps Speed in MB/s
     */
    void SetSpeed(double speed_mbps);

    /**
     * @brief Finalize and display completion status
     * @param success true if scan completed successfully
     * @param total_files Total files recovered
     * @param total_deleted Total deleted files recovered
     */
    void OnScanCompleted(bool success, int total_files, int total_deleted);

    /**
     * @brief Update elapsed time display
     * Called by internal timer
     */
    void UpdateElapsedTime();

private:
    // UI Components
    QLabel *operation_label_;           // Current operation display
    QProgressBar *progress_bar_;        // Main progress bar

    QLabel *files_label_;               // "Recoverable Files:" label
    QLabel *files_value_;               // File count display (e.g., "245 found")

    QLabel *deleted_label_;             // "Deleted Files:" label
    QLabel *deleted_value_;             // Deleted file count display

    QLabel *speed_label_;               // "Scan Speed:" label
    QLabel *speed_value_;               // Speed display (e.g., "125.5 MB/s")

    QLabel *time_label_;                // "Elapsed Time:" label
    QLabel *time_value_;                // Elapsed time display (e.g., "01:23:45")

    QLabel *eta_label_;                 // "Estimated Time:" label
    QLabel *eta_value_;                 // ETA display

    QLabel *status_label_;              // Status message

    // Layout
    QVBoxLayout *main_layout_;
    QHBoxLayout *operation_layout_;
    QHBoxLayout *files_layout_;
    QHBoxLayout *deleted_layout_;
    QHBoxLayout *speed_layout_;
    QHBoxLayout *time_layout_;
    QHBoxLayout *eta_layout_;

    // Timer for elapsed time tracking
    QTimer *timer_;
    qint64 start_time_ms_;              // Scan start time in milliseconds
    qint64 last_update_ms_;             // Last update time

    // Statistics
    int total_files_processed_;
    int total_files_found_;
    int total_deleted_files_;
    double current_speed_mbps_;

    /**
     * @brief Initialize UI components
     * Called in constructor
     */
    void InitializeUI();

    /**
     * @brief Format time duration to HH:MM:SS
     * @param milliseconds Duration in milliseconds
     * @return Formatted time string
     */
    QString FormatTime(qint64 milliseconds) const;

    /**
     * @brief Calculate estimated time remaining
     * @param progress Current progress (0-100)
     * @param elapsed_ms Elapsed time in milliseconds
     * @return Formatted ETA string
     */
    QString CalculateETA(int progress, qint64 elapsed_ms) const;
};

#endif  // PROGRESS_MONITOR_H
