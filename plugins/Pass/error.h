// error.h
#ifndef ERROR_H
#define ERROR_H

extern "C" {
#include "rnp/rnp_err.h"
}

enum class ErrorCodeShow {
    Success= 0,
    UnexceptedError,
    BadPassphrase,
    NoKeyFound,
    DecryptFailed
};

int rnpErrorToErrorCodeShow(int rnpErrorCode) {
    switch (rnpErrorCode) {
    case RNP_SUCCESS:
        return static_cast<int>(ErrorCodeShow::Success);
    case RNP_ERROR_BAD_PASSWORD:
        return static_cast<int>(ErrorCodeShow::BadPassphrase);
    case RNP_ERROR_KEY_NOT_FOUND:
    case RNP_ERROR_NO_SUITABLE_KEY:
        return static_cast<int>(ErrorCodeShow::NoKeyFound);
    case RNP_ERROR_DECRYPT_FAILED:
        return static_cast<int>(ErrorCodeShow::DecryptFailed);
    default:
        return static_cast<int>(ErrorCodeShow::UnexceptedError);
    }
}

enum class ErrorCodeImportKeyFile {
    Success= 0,
    UnexceptedError,
    BadFormat,
};

int rnpErrorToErrorCodeImportKeyFile(int rnpErrorCode) {
    switch (rnpErrorCode) {
    case RNP_SUCCESS:
        return static_cast<int>(ErrorCodeShow::Success);
    case RNP_ERROR_BAD_FORMAT:
        return static_cast<int>(ErrorCodeImportKeyFile::BadFormat);
    default:
        return static_cast<int>(ErrorCodeImportKeyFile::UnexceptedError);
    }
}


enum class ErrorCodeUnexvepted {
    Success= 0,
    UnexceptedError,
};

int rnpErrorToErrorCodeGeneric(int rnpErrorCode) {
    switch (rnpErrorCode) {
    case RNP_SUCCESS:
        return static_cast<int>(ErrorCodeShow::Success);
    default:
        return static_cast<int>(ErrorCodeImportKeyFile::UnexceptedError);
    }
}

enum class ErrorCode
{
    Success= 0,
    Error,
};

#endif // ERROR_H
