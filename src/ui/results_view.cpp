#include "results_view.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QDateTime>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <iomanip>

/**
 * @file results_view.cpp
 * @brief Implementation of ResultsView Qt6 widget
 *
 * Displays scan results in a table with:
 * - Filename, size, type, modification time, status
 * - Sortable columns
 * - Multi-selection
 * - Export functionality
 * - Statistics summary
 */

ResultsView::ResultsView(QWidget *parent)
    : QWidget(parent),
      results_label_(nullptr),
      results_table_(nullptr),
      table_model_(nullptr),
      proxy_model_(nullptr),
      stats_label_(nullptr),
      button_layout_(nullptr),
      select_all_btn_(nullptr),
      deselect_btn_(nullptr),
      export_btn_(nullptr),
      preview_btn_(nullptr),
      status_label_(nullptr),
      main_layout_(nullptr),
      last_filter_(""),
      total_files_(0),
      total_deleted_(0),
      total_size_(0) {

    InitializeUI();
    SetupTableColumns();
}

void ResultsView::InitializeUI() {
    /**
     * TODO: Implement full results UI with:
     * - Table styling (alternating row colors)
     * - Column headers with sort indicators
     * - Status icons/colors for deleted files
     * - File type icons
     * - Selection highlighting
     */

    main_layout_ = new QVBoxLayout(this);

    // === Results Header ===
    results_label_ = new QLabel("Scan Results", this);
    results_label_->setStyleSheet("QLabel { font-size: 14px; font-weight: bold; }");
    main_layout_->addWidget(results_label_);

    // === Results Table ===
    table_model_ = new QStandardItemModel(this);
    proxy_model_ = new QSortFilterProxyModel(this);
    proxy_model_->setSourceModel(table_model_);
    proxy_model_->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxy_model_->setFilterKeyColumn(-1);  // Search all columns

    results_table_ = new QTableView(this);
    results_table_->setModel(proxy_model_);
    results_table_->setSelectionBehavior(QAbstractItemView::SelectRows);
    results_table_->setSelectionMode(QAbstractItemView::MultiSelection);
    results_table_->setAlternatingRowColors(true);
    results_table_->horizontalHeader()->setStretchLastSection(true);
    results_table_->setMinimumHeight(200);

    results_table_->setStyleSheet(
        "QTableView {"
        "  alternate-background-color: #F5F5F5;"
        "  background-color: white;"
        "  gridline-color: #DDDDDD;"
        "}"
        "QTableView::item {"
        "  padding: 5px;"
        "}"
        "QHeaderView::section {"
        "  background-color: #E8E8E8;"
        "  padding: 5px;"
        "  border: 1px solid #CCCCCC;"
        "  font-weight: bold;"
        "}"
    );

    main_layout_->addWidget(results_table_);

    // === Statistics ===
    stats_label_ = new QLabel("Total: 0 files | Deleted: 0 | Size: 0 B", this);
    stats_label_->setStyleSheet("QLabel { color: #666666; font-size: 11px; }");
    main_layout_->addWidget(stats_label_);

    // === Action Buttons ===
    button_layout_ = new QHBoxLayout();

    select_all_btn_ = new QPushButton("Select All", this);
    deselect_btn_ = new QPushButton("Deselect", this);
    preview_btn_ = new QPushButton("Preview", this);
    export_btn_ = new QPushButton("Export Results", this);
    export_btn_->setStyleSheet(
        "QPushButton {"
        "  background-color: #0066CC;"
        "  color: white;"
        "  font-weight: bold;"
        "  border-radius: 3px;"
        "  padding: 5px;"
        "}"
        "QPushButton:hover { background-color: #0052A3; }"
    );

    button_layout_->addWidget(select_all_btn_);
    button_layout_->addWidget(deselect_btn_);
    button_layout_->addWidget(preview_btn_);
    button_layout_->addStretch();
    button_layout_->addWidget(export_btn_);

    main_layout_->addLayout(button_layout_);

    // === Status Display ===
    status_label_ = new QLabel("Ready", this);
    status_label_->setStyleSheet("QLabel { color: #008000; font-size: 10px; }");
    main_layout_->addWidget(status_label_);

    this->setLayout(main_layout_);

    // === Signal Connections ===
    connect(select_all_btn_, &QPushButton::clicked, this, &ResultsView::OnSelectAll);
    connect(deselect_btn_, &QPushButton::clicked, this, &ResultsView::OnDeselectAll);
    connect(export_btn_, &QPushButton::clicked, this, &ResultsView::OnExportClicked);
    connect(results_table_, &QTableView::clicked, this, &ResultsView::OnFileSelected);

    // Set window properties
    this->setWindowTitle("Results View");
    this->setMinimumWidth(700);
    this->setMinimumHeight(400);
}

void ResultsView::SetupTableColumns() {
    /**
     * TODO: Configure table columns with proper sizing and formatting
     */

    QStringList headers = {
        "Filename",
        "Size",
        "Type",
        "Modified",
        "Status",
        "Priority"
    };

    table_model_->setHorizontalHeaderLabels(headers);

    // Set column widths
    results_table_->setColumnWidth(0, 250);  // Filename
    results_table_->setColumnWidth(1, 80);   // Size
    results_table_->setColumnWidth(2, 80);   // Type
    results_table_->setColumnWidth(3, 130);  // Modified
    results_table_->setColumnWidth(4, 80);   // Status
    results_table_->setColumnWidth(5, 80);   // Priority
}

int ResultsView::GetResultCount() const {
    /**
     * TODO: Return visible result count after filtering
     */

    return proxy_model_->rowCount();
}

std::vector<FileEntry> ResultsView::GetSelectedFiles() const {
    /**
     * TODO: Extract selected FileEntry objects from table selection
     */

    std::vector<FileEntry> selected;

    QModelIndexList selected_indices = results_table_->selectionModel()->selectedRows();
    for (const QModelIndex &index : selected_indices) {
        if (index.row() < static_cast<int>(current_results_.size())) {
            selected.push_back(current_results_[index.row()]);
        }
    }

    return selected;
}

bool ResultsView::ExportToCSV(const QString &filepath, const std::vector<FileEntry> &entries) const {
    /**
     * TODO: Implement CSV export with proper formatting
     */

    QFile file(filepath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    // Write CSV header
    file.write("Filename,Size (bytes),Type,Modified,Status,Recovery Priority\n");

    // Write data rows
    for (const auto &entry : entries) {
        QString filename = QString::fromStdString(entry.filename);
        QString size = QString::number(entry.file_size);
        QString type = GetFileTypeString(entry.is_directory);
        QString modified = FormatTimestamp(entry.modification_time);
        QString status = GetStatusString(entry.is_deleted);
        QString priority = CalculatePriority(entry.is_deleted, entry.file_size, entry.modification_time);

        QString line = QString("\"%1\",%2,%3,%4,%5,%6\n")
            .arg(filename, size, type, modified, status, priority);

        file.write(line.toUtf8());
    }

    file.close();
    return true;
}

void ResultsView::PopulateResults(const std::vector<FileEntry> &entries) {
    /**
     * TODO: Add file entries to table with formatting
     * - Convert FileEntry to table rows
     * - Format sizes and timestamps
     * - Color-code deleted files
     * - Calculate priorities
     */

    ClearResults();
    current_results_ = entries;

    total_files_ = 0;
    total_deleted_ = 0;
    total_size_ = 0;

    for (const auto &entry : entries) {
        QList<QStandardItem*> row;

        // Filename
        QStandardItem *filename_item = new QStandardItem(QString::fromStdString(entry.filename));
        if (entry.is_deleted) {
            filename_item->setForeground(QBrush(QColor("#CC0000")));  // Red for deleted
        }
        row.append(filename_item);

        // Size
        QString size_str = FormatFileSize(entry.file_size);
        row.append(new QStandardItem(size_str));

        // Type
        QString type_str = GetFileTypeString(entry.is_directory);
        row.append(new QStandardItem(type_str));

        // Modified
        QString modified_str = FormatTimestamp(entry.modification_time);
        row.append(new QStandardItem(modified_str));

        // Status
        QString status_str = GetStatusString(entry.is_deleted);
        QStandardItem *status_item = new QStandardItem(status_str);
        if (entry.is_deleted) {
            status_item->setForeground(QBrush(QColor("#FF6600")));
        }
        row.append(status_item);

        // Priority
        QString priority_str = CalculatePriority(entry.is_deleted, entry.file_size, entry.modification_time);
        row.append(new QStandardItem(priority_str));

        table_model_->appendRow(row);

        // Update statistics
        total_files_++;
        if (entry.is_deleted) {
            total_deleted_++;
        }
        total_size_ += entry.file_size;
    }

    UpdateStatistics(total_files_, total_deleted_);
}

void ResultsView::ClearResults() {
    /**
     * TODO: Clear all data from table
     */

    table_model_->removeRows(0, table_model_->rowCount());
    current_results_.clear();
    total_files_ = 0;
    total_deleted_ = 0;
    total_size_ = 0;
}

void ResultsView::UpdateStatistics(int total, int deleted) {
    /**
     * TODO: Update statistics display with formatting
     */

    QString stats_text = QString("Total: %1 files | Deleted: %2 | Size: %3")
        .arg(total)
        .arg(deleted)
        .arg(FormatFileSize(total_size_));

    stats_label_->setText(stats_text);
}

void ResultsView::FilterResults(const QString &search_term) {
    /**
     * TODO: Implement multi-field filtering
     */

    last_filter_ = search_term;
    proxy_model_->setFilterWildcard(search_term);
}

void ResultsView::OnFileSelected() {
    /**
     * TODO: Handle file selection and emit signal for preview
     */

    QModelIndexList selected = results_table_->selectionModel()->selectedRows();
    if (!selected.isEmpty()) {
        int row = selected[0].row();
        if (row < static_cast<int>(current_results_.size())) {
            emit fileSelected(current_results_[row]);
        }
    }
}

void ResultsView::OnExportClicked() {
    /**
     * TODO: Show file dialog and export selected files
     */

    auto selected = GetSelectedFiles();
    if (selected.empty()) {
        QMessageBox::warning(this, "No Selection", "Please select files to export.");
        return;
    }

    QString filepath = QFileDialog::getSaveFileName(
        this,
        "Export Results",
        "",
        "CSV Files (*.csv);;All Files (*)"
    );

    if (!filepath.isEmpty()) {
        if (ExportToCSV(filepath, selected)) {
            status_label_->setText("✓ Export successful");
            status_label_->setStyleSheet("QLabel { color: #008000; font-size: 10px; }");
            QMessageBox::information(this, "Export Successful",
                                   QString("Exported %1 files to %2").arg(selected.size()).arg(filepath));
        } else {
            status_label_->setText("✗ Export failed");
            status_label_->setStyleSheet("QLabel { color: #CC0000; font-size: 10px; }");
            QMessageBox::warning(this, "Export Failed", "Could not write to file.");
        }
    }
}

void ResultsView::OnSelectAll() {
    /**
     * TODO: Select all visible rows
     */

    results_table_->selectAll();
    status_label_->setText(QString("Selected all %1 files").arg(GetResultCount()));
}

void ResultsView::OnDeselectAll() {
    /**
     * TODO: Deselect all rows
     */

    results_table_->clearSelection();
    status_label_->setText("Selection cleared");
}

QString ResultsView::FormatFileSize(qint64 size) const {
    /**
     * TODO: Format bytes to human-readable format (B, KB, MB, GB, TB)
     */

    if (size < 1024) {
        return QString("%1 B").arg(size);
    } else if (size < 1024 * 1024) {
        return QString("%1 KB").arg(size / 1024.0, 0, 'f', 1);
    } else if (size < 1024 * 1024 * 1024) {
        return QString("%1 MB").arg(size / (1024.0 * 1024), 0, 'f', 1);
    } else if (size < 1024LL * 1024 * 1024 * 1024) {
        return QString("%1 GB").arg(size / (1024.0 * 1024 * 1024), 0, 'f', 1);
    } else {
        return QString("%1 TB").arg(size / (1024.0 * 1024 * 1024 * 1024), 0, 'f', 2);
    }
}

QString ResultsView::FormatTimestamp(time_t timestamp) const {
    /**
     * TODO: Format Unix timestamp to readable date/time
     */

    if (timestamp == 0) {
        return "Unknown";
    }

    QDateTime dt = QDateTime::fromSecsSinceEpoch(timestamp);
    return dt.toString("yyyy-MM-dd HH:mm:ss");
}

QString ResultsView::GetFileTypeString(bool is_directory) const {
    /**
     * TODO: Return file type string
     */

    return is_directory ? "Directory" : "File";
}

QString ResultsView::GetStatusString(bool is_deleted) const {
    /**
     * TODO: Return status string
     */

    return is_deleted ? "Deleted" : "Active";
}

QString ResultsView::CalculatePriority(bool is_deleted, qint64 size, time_t mtime) const {
    /**
     * TODO: Implement priority algorithm
     * - Deleted: High
     * - Recent + large: Medium
     * - Other: Low
     */

    if (is_deleted) {
        return "High";
    }

    // Get time difference from now
    time_t now = time(nullptr);
    double days_ago = difftime(now, mtime) / 86400.0;

    if (days_ago < 30 && size > 1024 * 1024) {  // Less than 30 days and > 1MB
        return "Medium";
    }

    return "Low";
}
