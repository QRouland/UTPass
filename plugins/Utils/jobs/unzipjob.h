#ifndef RMJOB_H
#define RMJOB_H

#include "qurl.h"
#include <QThread>
#include <QDir>

/**
 * @class RmJob
 * @brief A class to handle removing recursively a path in a separate thread.
 *
 */
class UnzipJob : public QThread
{
    Q_OBJECT

    /**
     * @brief The main function that performs the unzip operation.
     *
     * Handles the process of unziping a archive to a target directory.
     */
    void run() override;

signals:
    /**
     * @brief Signal emitted when the unzip operation is complete.
     *
     * @param err A boolean indicating whether an error occurred during unzipping.
     *        `true` if an error occurred, `false` if the clone was successful.
     */
    void resultReady(const bool err);

private:
    QUrl m_zip_url; ///< The url of the archive.
    QDir m_dir_out; ///< The directory where the content of the archive will be unzip.

public:
    /**
     * @brief Constructor for the UnzipJob class.
     *
     * Initializes the UnzipJob with the specified target path to be removed.
     *
     * @param zip_url Url of the archive to be unzip.
     * @param dir_out Target directory where the content of the archive must be extracted.
     */
    UnzipJob(QUrl zip_url, QDir dir_out);
};

#endif // RMJOB_H
