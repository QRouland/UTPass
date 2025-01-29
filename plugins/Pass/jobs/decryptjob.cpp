#include "decryptjob.h"

DecryptJob::DecryptJob(QString path, QString keyfile):
    m_path(path)
{
    this->setObjectName("DecryptJob");
}


void DecryptJob::run()
{
    rnp_input_from_path(&keyfile, "secring.pgp"));
    qFatal("To be implemented !")
}
