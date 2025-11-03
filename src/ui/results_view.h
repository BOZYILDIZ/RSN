#ifndef RESULTS_VIEW_H
#define RESULTS_VIEW_H

#include <QWidget>
#include <QTableView>
#include <QStandardItemModel>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSortFilterProxyModel>
#include <vector>
#include "../core/recovery_engine.h"

/**
 * @class ResultsView
 * @brief Qt6 Widget for displaying scan results in table format
 *
 * Displays recoverable files with:
 * - Filename
 * - File size
 * - File type (Directory/Regular/Link)
 * - Modification time
 * - Status (Active/Deleted)
 * - Recovery priority
 *
 * Features:
 * - Sortable columns
 * - Filterable by filename/type/status
 * - File preview capability
 * - Multi-selection for batch export
 * - Export to CSV
 * - Recovery statistics summary
 *
 * Signals:
 * - fileSelected(FileEntry) → file preview request
 * - exportRequested(vector<FileEntry>) → export selection
 *
 * Slots:
 * - PopulateResults(vector<FileEntry>) → add files to table
 * - ClearResults() → reset table
 * - OnFileSelected() → handle row selection
 * - OnExportClicked() → export selected files
 */
class ResultsView : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Constructor for ResultsView
     * @param parent Parent widget
     */
    explicit ResultsView(QWidget *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~ResultsView() override = default;

    /**
     * @brief Get number of results currently displayed
     * @return Count of rows in table
     */
    int GetResultCount() const;

    /**
     * @brief Get selected files from table
     * @return Vector of selected FileEntry objects
     *
     * TODO: Implement multi-selection
     */
    std::vector<FileEntry> GetSelectedFiles() const;

    /**
     * @brief Export results to CSV file
     * @param filepath Destination CSV file path
     * @param entries Files to export
     * @return true if export successful
     *
     * TODO: Implement CSV export with formatting
     */
    bool ExportToCSV(const QString &filepath, const std::vector<FileEntry> &entries) const;

public slots:
    /**
     * @brief Populate table with scan results
     * @param entries Vector of FileEntry objects from parser
     *
     * Adds rows to table for each file, with columns:
     * - Filename
     * - Size
     * - Type
     * - Modified
     * - Status
     * - Recovery Priority
     */
    void PopulateResults(const std::vector<FileEntry> &entries);

    /**
     * @brief Clear results table
     * Removes all rows, ready for new scan
     */
    void ClearResults();

    /**
     * @brief Update statistics display
     * @param total Total files recovered
     * @param deleted Deleted files recovered
     */
    void UpdateStatistics(int total, int deleted);

    /**
     * @brief Filter results by search term
     * @param search_term Filename or partial match
     *
     * TODO: Implement filtering with multiple fields
     */
    void FilterResults(const QString &search_term);

    /**
     * @brief Handle file selection in table
     * Called when user clicks on a file row
     */
    void OnFileSelected();

    /**
     * @brief Handle export button click
     */
    void OnExportClicked();

    /**
     * @brief Handle select all button click
     */
    void OnSelectAll();

    /**
     * @brief Handle deselect all button click
     */
    void OnDeselectAll();

signals:
    /**
     * @brief Signal emitted when file selected for preview
     * @param entry Selected FileEntry
     *
     * TODO: Implement file preview functionality
     */
    void fileSelected(const FileEntry &entry);

    /**
     * @brief Signal emitted when export requested
     * @param entries Files to export
     */
    void exportRequested(const std::vector<FileEntry> &entries);

private:
    // UI Components
    QLabel *results_label_;             // "Scan Results" header

    QTableView *results_table_;         // Main results table
    QStandardItemModel *table_model_;   // Data model for table
    QSortFilterProxyModel *proxy_model_;// For filtering and sorting

    QLabel *stats_label_;               // Statistics summary display

    QHBoxLayout *button_layout_;
    QPushButton *select_all_btn_;       // "Select All" button
    QPushButton *deselect_btn_;         // "Deselect All" button
    QPushButton *export_btn_;           // "Export Results" button
    QPushButton *preview_btn_;          // "Preview" button

    QLabel *status_label_;              // Status display

    // Layout
    QVBoxLayout *main_layout_;

    // Data storage
    std::vector<FileEntry> current_results_;
    QString last_filter_;

    // Statistics
    int total_files_;
    int total_deleted_;
    qint64 total_size_;

    /**
     * @brief Initialize UI components
     * Called in constructor
     */
    void InitializeUI();

    /**
     * @brief Setup table columns with headers
     *
     * Columns:
     * 0: Filename
     * 1: Size (bytes)
     * 2: Type (Directory/File/Link)
     * 3: Modified (timestamp)
     * 4: Status (Active/Deleted)
     * 5: Priority (Recovery priority)
     */
    void SetupTableColumns();

    /**
     * @brief Format file size for display
     * @param size Size in bytes
     * @return Human-readable size (e.g., "1.5 MB", "2 GB")
     */
    QString FormatFileSize(qint64 size) const;

    /**
     * @brief Format timestamp for display
     * @param timestamp Unix timestamp
     * @return Formatted date/time string
     */
    QString FormatTimestamp(time_t timestamp) const;

    /**
     * @brief Get file type string
     * @param is_directory true if directory
     * @return File type string (Directory/File)
     */
    QString GetFileTypeString(bool is_directory) const;

    /**
     * @brief Get status string for file
     * @param is_deleted true if file is deleted
     * @return Status string (Active/Deleted)
     */
    QString GetStatusString(bool is_deleted) const;

    /**
     * @brief Calculate recovery priority
     * @param is_deleted true if deleted
     * @param size File size
     * @param mtime Modification time
     * @return Priority string (High/Medium/Low)
     *
     * TODO: Implement priority algorithm
     * Priority based on:
     * - Deleted files: High priority
     * - Recent files: Medium priority
     * - Large files: Medium priority
     * - Old small files: Low priority
     */
    QString CalculatePriority(bool is_deleted, qint64 size, time_t mtime) const;
};

#endif  // RESULTS_VIEW_H
