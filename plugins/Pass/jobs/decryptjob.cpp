#include "decryptjob.h"

DecryptJob::DecryptJob(QString path, QString keyfile):
    m_path(path)
{
    this->setObjectName("DecryptJob");
}


void DecryptJob::run()
{
    this->load_sec_keyring();
    rnp_input_from_path(&keyfile, "secring.pgp"));
    qFatal("To be implemented !")
}
