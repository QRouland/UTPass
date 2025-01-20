#ifndef UTILS_H
#define UTILS_H

#include <QObject>
#include <QUrl>
#include <QQuickWindow>
#include <memory>
#include <QSemaphore>

/**
 * @class Utils
 * @brief A utility class that provides helper functions for file and directory operations.
 *
 * The `Utils` class contains various helper methods such as for managing files and directories, including unzipping files
 * and removing files or directories.
 */
class Utils : public QObject
{
    Q_OBJECT


private slots:
    /**
     * @brief Slot to handle the result of a unzip operation.
     * @param err True if an error occurred during the operation.
     */
    void unzipResult(bool err);

signals:
    /**
     * @brief Emitted when the archive is successfully extracted.
     */
    void unzipSucceed();

    /**
     * @brief Emitted when the unzipping operation fails.
     * @param message The error message describing the failure.
     */
    void unzipFailed(QString message);

private:
    std::unique_ptr<QSemaphore> m_sem; /**< Semaphore for managing concurrent operations. */

public:
    /**
     * @brief Constructor for the Utils class.
     */
    Utils();

    /**
     * @brief Unzips a ZIP file to the specified output directory.
     *
     * This method extracts the contents of a ZIP file from the specified URL and saves them to the provided
     * output directory path.
     *
     * @param zip_url The URL of the ZIP file to unzip.
     * @param dir_out The output directory where the contents of the ZIP file should be extracted.
     * @return `true` if the unzipping operation was successful, `false` otherwise.
     */
    Q_INVOKABLE bool unzip(QUrl zip_url, QString dir_out);



    /**
     * @brief Retrieves the path to the manifest data.
     *
     * This function returns the full path to the manifest file used by the application.
     *
     * @return A QString containing the manifest file path.
     */
    Q_INVOKABLE QString manifestPath();

};

#endif
