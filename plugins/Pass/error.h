// error.h
#ifndef ERROR_H
#define ERROR_H

extern "C" {
#include "rnp/rnp_err.h"
}

// Enum for general error codes
enum class ErrorCode {
    Error = 1,  ///< Generic error code
};

// Enum for errors related to showing errors (e.g., password issues, key issues)
enum class ErrorCodeShow {
    UnexpectedError = 1,  ///< Unknown or unexpected error
    BadPassphrase,        ///< Invalid passphrase error
    NoKeyFound,           ///< Key not found error
    DecryptFailed         ///< Decryption failure error
};

/**
* Convert an RNP error code to a corresponding ErrorCodeShow
* @param rnpErrorCode The RNP error code
* @return Corresponding ErrorCodeShow integer value
*/
inline ErrorCodeShow rnpErrorToErrorCodeShow(int rnpErrorCode) {
    switch (rnpErrorCode) {
    case RNP_ERROR_BAD_PASSWORD:
        return ErrorCodeShow::BadPassphrase;   ///< Bad passphrase error
    case RNP_ERROR_KEY_NOT_FOUND:
    case RNP_ERROR_NO_SUITABLE_KEY:
        return ErrorCodeShow::NoKeyFound;      ///< No key found error
    case RNP_ERROR_DECRYPT_FAILED:
        return ErrorCodeShow::DecryptFailed;    ///< Decryption failure error
    default:
        return ErrorCodeShow::UnexpectedError; ///< Default to unexpected error
    }
}

// Enum for errors related to importing key files
enum class ErrorCodeImportKeyFile {
    UnexpectedError = 1,  ///< Unknown or unexpected error
    BadFormat,            ///< Bad format error when importing the key file
};

/**
* Convert an RNP error code to a corresponding ErrorCodeImportKeyFile
* @param rnpErrorCode The RNP error code
* @return Corresponding ErrorCodeImportKeyFile integer value
*/
inline ErrorCodeImportKeyFile rnpErrorToErrorCodeImportKeyFile(int rnpErrorCode) {
    switch (rnpErrorCode) {
    case RNP_ERROR_BAD_FORMAT:
        return ErrorCodeImportKeyFile::BadFormat; ///< Bad format error
    default:
        return ErrorCodeImportKeyFile::UnexpectedError; ///< Default to unexpected error
    }
}

#endif // ERROR_H
