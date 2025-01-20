#include "rmjob.h"

RmJob::RmJob(QString path):
    m_path(path)
{
    this->setObjectName("RmJob");
}


void RmJob::run()
{
    auto info = QFileInfo(this->m_path);
    if (info.isFile()) {
        auto file = QFile(this->m_path);
        file.remove();
        emit resultReady(false);
    } else if (info.isDir()) {
        auto dir = QDir(this->m_path);
        dir.removeRecursively();
        emit resultReady(false);
    } else {
        emit resultReady(true);
    }
}
