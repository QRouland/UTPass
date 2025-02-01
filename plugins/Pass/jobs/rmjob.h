#ifndef RMJOB_H
#define RMJOB_H

#include <QThread>
#include <QDir>

/**
 * @class RmJob
 * @brief A job to handle the recursive removal of a path in a separate thread.
 *
 */
class RmJob : public QThread
{
    Q_OBJECT

    /**
     * @brief Executes the recursive remove operation.
     *
     * This method performs the recursive removal of the specified target path.
     * The operation is performed in the background to prevent blocking of the main
     * application thread.
     */
    void run() override;

signals:
    /**
     * @brief Emitted when the remove operation completes.
     *
     * This signal is emitted once the removal process is complete, indicating
     * whether the operation succeeded or failed.
     *
     * @param err A boolean indicating whether an error occurred during the removal.
     *        `true` if an error occurred, `false` if the operation was successful.
     */
    void resultReady(const bool err);

private:
    QString m_path; /**< The path to be removed. */

public:
    /**
     * @brief Constructs an RmJob object with the specified path.
     *
     * This constructor initializes the job with the path of the directory or file to be
     * removed. The job will be executed in a separate thread when started.
     *
     * @param path The path to the file or directory that should be removed.
     */
    RmJob(QString path);
};

#endif // RMJOB_H
