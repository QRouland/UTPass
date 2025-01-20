#ifndef RMJOB_H
#define RMJOB_H

#include <QThread>
#include <QDir>

/**
 * @class RmJob
 * @brief A class to handle removing recursively a path in a separate thread.
 *
 */
class RmJob : public QThread
{
    Q_OBJECT

    /**
     * @brief The main function that performs the rm operation.
     *
     * Handles the process of removing recursively a target path.
     */
    void run() override;

signals:
    /**
     * @brief Signal emitted when the rm operation is complete.
     *
     * @param err A boolean indicating whether an error occurred during cloning.
     *        `true` if an error occurred, `false` if the clone was successful.
     */
    void resultReady(const bool err);

private:
    QString m_path; ///< The path to be removed.

public:
    /**
     * @brief Constructor for the RmJob class.
     *
     * Initializes the RmJob with the specified path to be removed.
     *
     * @param path Path to be remove.
     */
    RmJob(QString path);
};

#endif // RMJOB_H
