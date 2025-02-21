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
     */
    void unzipFailed();

private:
    std::unique_ptr<QSemaphore> m_sem; /**< Semaphore for managing concurrent operations. */

public:
    /**
     * @brief Constructor for the Utils class.
     */
    Utils();

    /**
     * @brief Start a job to unzips a ZIP file to the specified output directory.
     *
     * @param zip_url The URL of the ZIP file to unzip.
     * @param dir_out The output directory where the contents of the ZIP file should be extracted.
     * @return `true` if the unzipping job is started successfullly, `false` otherwise.
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

    /**
     * @brief Removes a file located at the specified URL.
     *
     * @param file_url The URL of the file to remove.
     * @return `true` if the file was successfully removed; `false` otherwise.
     */
    Q_INVOKABLE bool rmFile(QUrl file_url);

    /**
     * @brief Removes a directory located at the specified URL.
     *
     * @param dir_url The URL of the directory to remove.
     * @return `true` if the directory was successfully removed; `false` otherwise.
     */
    Q_INVOKABLE bool rmDir(QUrl dir_url);

    /**
     * @brief Verify that file exists at the specified URL.
     *
     * @param path The URL of the file to verfidy.
     * @return `true` if the file exist; `false` otherwise.
     */
    Q_INVOKABLE bool fileExists(QUrl path);

};

#endif
