#include "System.h"
#include "Logger.h"


CString System::GetStringRegKey(HKEY key, PCWSTR subKey, PCWSTR valueName)
{
    DWORD keyType = 0;
    DWORD dataSize = 0;
    const DWORD flags = RRF_RT_REG_SZ;
    LONG result = ::RegGetValue(
        key,
        subKey,
        valueName,
        flags,
        &keyType,
        nullptr,
        &dataSize);

    if (result != ERROR_SUCCESS)
        AtlThrow(HRESULT_FROM_WIN32(result));

    ATLASSERT(keyType == REG_SZ);

    CString text;
    const DWORD bufferLength = dataSize / sizeof(WCHAR);
    WCHAR* const textBuffer = text.GetBuffer(bufferLength);
    ATLASSERT(textBuffer != nullptr);

    result = ::RegGetValue(
        key,
        subKey,
        valueName,
        flags,
        nullptr,
        textBuffer,
        &dataSize);

    if (result != ERROR_SUCCESS)
        AtlThrow(HRESULT_FROM_WIN32(result));

    const DWORD actualStringLength = dataSize / sizeof(WCHAR);
    text.ReleaseBufferSetLength(actualStringLength - 1);

    return text;
}