// error.h
#ifndef ERROR_H
#define ERROR_H

extern "C" {
#include "rnp/rnp_err.h"
}

enum ErrorCodeShow {
    UnexceptedError= 1,
    BadPassphrase,
    NoKeyFound,
    DecryptFailed
};

ErrorCodeShow rnpErrorToErrorCodeShow(int rnpErrorCode) {
    switch (rnpErrorCode) {
    case RNP_ERROR_BAD_PASSWORD:
        return BadPassphrase;
    case RNP_ERROR_KEY_NOT_FOUND:
    case RNP_ERROR_NO_SUITABLE_KEY:
        return NoKeyFound;
    case RNP_ERROR_DECRYPT_FAILED:
        return DecryptFailed;
    default:
        return UnexceptedError;
    }
}

#endif // ERROR_H
