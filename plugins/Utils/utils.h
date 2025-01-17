#ifndef UTILS_H
#define UTILS_H

#include <QObject>
#include <QUrl>
#include <QQuickWindow>

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

public:
    /**
     * @brief Default constructor for the Utils class.
     */
    Utils() = default;

    /**
     * @brief Default destructor for the Utils class.
     */
    ~Utils() override = default;

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
     * @brief Removes a file at the specified URL.
     *
     * This method deletes a file at the specified URL.
     *
     * @param file_url The URL of the file to delete.
     * @return `true` if the file was successfully removed, `false` otherwise.
     */
    Q_INVOKABLE bool rmFile(QUrl file_url);

    /**
     * @brief Removes a directory at the specified URL.
     *
     * This method deletes a directory at the specified URL, along with its contents.
     *
     * @param dir_url The URL of the directory to remove.
     * @return `true` if the directory was successfully removed, `false` otherwise.
     */
    Q_INVOKABLE bool rmDir(QUrl dir_url);


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
