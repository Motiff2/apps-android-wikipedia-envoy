#include "secrets.hpp"

#include <jni.h>

#include "sha256.hpp"
#include "sha256.cpp"

/* Copyright (c) 2020-present Klaxit SAS
*
* Permission is hereby granted, free of charge, to any person
* obtaining a copy of this software and associated documentation
* files (the "Software"), to deal in the Software without
* restriction, including without limitation the rights to use,
* copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following
* conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
* HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*/

char *customDecode(char *str) {
    /* Add your own logic here
    * To improve your key security you can encode it before to integrate it in the app.
    * And then decode it with your own logic in this function.
    */
    return str;
}

jstring getOriginalKey(
        char *obfuscatedSecret,
        int obfuscatedSecretSize,
        jstring obfuscatingJStr,
        JNIEnv *pEnv) {

    // Get the obfuscating string SHA256 as the obfuscator
    const char *obfuscatingStr = pEnv->GetStringUTFChars(obfuscatingJStr, NULL);
    char buffer[2 * SHA256::DIGEST_SIZE + 1];

    sha256(obfuscatingStr, buffer);
    const char *obfuscator = buffer;

    // Apply a XOR between the obfuscated key and the obfuscating string to get original string
    char out[obfuscatedSecretSize + 1];
    for (int i = 0; i < obfuscatedSecretSize; i++) {
        out[i] = obfuscatedSecret[i] ^ obfuscator[i % strlen(obfuscator)];
    }

    // Add string terminal delimiter
    out[obfuscatedSecretSize] = 0x0;

    // (Optional) To improve key security
    return pEnv->NewStringUTF(customDecode(out));
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_greatfire_wikiunblocked_Secrets_geturlStart(
        JNIEnv *pEnv,
        jobject pThis,
        jstring packageName) {
    char obfuscatedSecret[] = { 0x1b, 0x50 };
    return getOriginalKey(obfuscatedSecret, sizeof(obfuscatedSecret), packageName, pEnv);
}
extern "C"
JNIEXPORT jstring JNICALL
Java_org_greatfire_wikiunblocked_Secrets_geturlEnd(
        JNIEnv* pEnv,
        jobject pThis,
        jstring packageName) {
     char obfuscatedSecret[] = { 0x1b, 0x50 };
     return getOriginalKey(obfuscatedSecret, sizeof(obfuscatedSecret), packageName, pEnv);
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_greatfire_wikiunblocked_Secrets_geturlSources(
        JNIEnv* pEnv,
        jobject pThis,
        jstring packageName) {
     char obfuscatedSecret[] = { 0x5e, 0x15, 0x47, 0x49, 0x10, 0xd, 0x16, 0x49, 0x1, 0x5d, 0x56, 0x1b, 0x58, 0x10, 0x56, 0x3, 0xe, 0x50, 0x13, 0x44, 0x1b, 0x56, 0x7, 0x11, 0x4c, 0x3, 0x5b, 0x18, 0x45, 0x51, 0x48, 0x5c, 0x54, 0x54, 0x5c, 0x1, 0x46, 0x5, 0x4, 0x6, 0x4b, 0x79, 0x5d, 0x12, 0x53, 0xd, 0x61, 0x42, 0x44, 0xd, 0x50, 0x5a, 0x5e, 0x74, 0x53, 0x16, 0x55, 0x1e, 0x7, 0x4e, 0x47, 0x41, 0x1a, 0x0, 0x19, 0x2, 0x5c, 0x57, 0x5, 0x5e, 0x5e, 0x49, 0x57, 0xe, 0x17, 0x56, 0x5d, 0xd, 0x54, 0xf, 0x5, 0x17, 0xf, 0x45, 0x5a, 0x56 };
     return getOriginalKey(obfuscatedSecret, sizeof(obfuscatedSecret), packageName, pEnv);
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_greatfire_wikiunblocked_Secrets_geturlInterval(
        JNIEnv* pEnv,
        jobject pThis,
        jstring packageName) {
     char obfuscatedSecret[] = { 0x1 };
     return getOriginalKey(obfuscatedSecret, sizeof(obfuscatedSecret), packageName, pEnv);
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_greatfire_wikiunblocked_Secrets_gethystCert(
        JNIEnv* pEnv,
        jobject pThis,
        jstring packageName) {
     char obfuscatedSecret[] = { 0x7b, 0x28, 0x7a, 0x7c, 0x19, 0x5d, 0x7a, 0x25, 0x23, 0x43, 0x59, 0x52, 0x73, 0x14, 0x7b, 0x24, 0x23, 0x5e, 0x2c, 0x64, 0x74, 0x71, 0x15, 0x20, 0x48, 0x9, 0x1, 0x73, 0x1c, 0x5, 0x4e, 0x45, 0x43, 0x65, 0x6d, 0x5, 0x67, 0x78, 0x5f, 0x18, 0x5, 0x0, 0x6b, 0x13, 0x72, 0x30, 0x6c, 0x7b, 0x7a, 0xc, 0x6b, 0x7e, 0x5f, 0x42, 0x0, 0x76, 0x24, 0x61, 0x70, 0x2d, 0x72, 0x65, 0x74, 0x46, 0x1a, 0x7, 0x49, 0x7c, 0x6, 0x7a, 0x7b, 0x11, 0x25, 0x78, 0x9, 0x60, 0x77, 0x20, 0x5a, 0x2b, 0x34, 0x5b, 0x32, 0x42, 0x5f, 0x62, 0x3a, 0x2f, 0x53, 0x2d, 0x74, 0x65, 0x5b, 0x50, 0x55, 0x65, 0x4a, 0x57, 0x4, 0x26, 0x40, 0x6e, 0x62, 0x2, 0x54, 0x7e, 0x77, 0x2a, 0x74, 0x2c, 0x66, 0x5e, 0x46, 0x28, 0x70, 0x6e, 0x73, 0x62, 0x32, 0x69, 0x29, 0x74, 0x76, 0x27, 0x4a, 0x6d, 0x7, 0x0, 0x77, 0x4d, 0x51, 0x6d, 0x25, 0x40, 0x5a, 0xb, 0x5a, 0x4c, 0x5a, 0x72, 0xb, 0x9, 0x6b, 0x31, 0x15, 0x5e, 0x2e, 0x70, 0x7b, 0x8, 0x38, 0x3d, 0x39, 0x8, 0x51, 0x5e, 0x75, 0x7a, 0x61, 0x3, 0x6b, 0x43, 0x55, 0x9, 0x72, 0x47, 0x6f, 0x74, 0xf, 0x4f, 0x7f, 0x30, 0x77, 0x17, 0x77, 0x56, 0x7f, 0x35, 0x73, 0x76, 0x7a, 0x79, 0x28, 0x7f, 0x54, 0x42, 0x6c, 0x53, 0x66, 0x4d, 0x51, 0x72, 0x74, 0x1b, 0x1f, 0x60, 0x51, 0x6, 0x78, 0x4, 0x36, 0x7f, 0x4f, 0x56, 0x5f, 0x5b, 0x47, 0x4, 0x25, 0x0, 0xf, 0x6f, 0x62, 0x4f, 0x5, 0x2e, 0x25, 0x2a, 0x3, 0x6d, 0x6f, 0x6e, 0x54, 0x51, 0x50, 0x77, 0x79, 0x3d, 0x4, 0x66, 0x43, 0x55, 0x9, 0x71, 0x41, 0x3e, 0x75, 0xa, 0x42, 0x79, 0x59, 0x0, 0x7f, 0x7a, 0x5d, 0x7d, 0x14, 0x75, 0x31, 0x79, 0x6, 0x2c, 0x74, 0x71, 0x5, 0x7f, 0x62, 0x30, 0x6, 0x15, 0x34, 0x5f, 0x5a, 0x28, 0x2f, 0x43, 0x71, 0x42, 0x7f, 0x37, 0x7b, 0x55, 0x2f, 0x7d, 0x20, 0x6, 0x7b, 0x6c, 0x33, 0x50, 0x34, 0xe, 0x71, 0x18, 0x7a, 0x66, 0xc, 0x44, 0x71, 0x74, 0x6e, 0x20, 0x62, 0x65, 0x64, 0x7c, 0x21, 0x4f, 0x64, 0x10, 0x57, 0x53, 0x7b, 0x5d, 0x52, 0xd, 0x60, 0x50, 0x6d, 0x73, 0x35, 0xa, 0x3f, 0x67, 0x4d, 0x17, 0x53, 0x73, 0x4, 0x5d, 0x54, 0xf, 0x62, 0x5e, 0x4f, 0x66, 0x9, 0x23, 0x1a, 0x72, 0x52, 0x74, 0x5d, 0x21, 0x55, 0x28, 0x34, 0x7b, 0x24, 0x45, 0x78, 0x71, 0x3a, 0x2b, 0x9, 0x6, 0x66, 0x75, 0x43, 0x79, 0x60, 0x71, 0x40, 0x57, 0x4e, 0x51, 0x47, 0x56, 0x7, 0x79, 0xc, 0x55, 0x71, 0x25, 0x59, 0x34, 0x6, 0x63, 0x5d, 0x7, 0x5c, 0x61, 0x42, 0x7d, 0x26, 0x9, 0xf, 0x62, 0x72, 0x58, 0x45, 0x6d, 0x62, 0x49, 0x5d, 0x2a, 0x67, 0x7c, 0x1b, 0x1b, 0x74, 0x25, 0x5a, 0x7e, 0x79, 0x4, 0x67, 0x26, 0x73, 0x11, 0x15, 0x56, 0x7, 0x61, 0x41, 0x52, 0x38, 0x3d, 0x29, 0x54, 0x7a, 0x7f, 0x79, 0x5e, 0x5a, 0x66, 0x7b, 0x41, 0x7a, 0x3c, 0x76, 0x4d, 0x57, 0x4e, 0x51, 0x44, 0x50, 0x56, 0x78, 0x9, 0x57, 0x72, 0x70, 0xc, 0x64, 0x4, 0x65, 0x58, 0x7, 0x55, 0x33, 0x45, 0x7c, 0x24, 0x1, 0x5e, 0x67, 0x76, 0xf, 0x14, 0x6a, 0x6e, 0x1b, 0x5c, 0x15, 0x2d, 0x36, 0x7a, 0x7b, 0x74, 0x53, 0x2a, 0x45, 0x22, 0x33, 0x60, 0x2f, 0x7d, 0x5a, 0x62, 0x2b, 0xd, 0x15, 0x7, 0x7d, 0x76, 0x66, 0x71, 0x7a, 0x71, 0x68, 0x74, 0x73, 0x3, 0x53, 0x73, 0x65, 0x76, 0x20, 0x74, 0x71, 0x25, 0x6f, 0xe, 0x76, 0x56, 0x56, 0x24, 0x73, 0x76, 0x79, 0x50, 0x48, 0x55, 0x28, 0x73, 0xc, 0xa, 0x61, 0x63, 0x42, 0x79, 0x1d, 0x9, 0x18, 0x12, 0x15, 0x5a, 0x6a, 0x4a, 0x29, 0x52, 0x49, 0x43, 0x3, 0x1b, 0x57, 0x4, 0x2a, 0x72, 0xb, 0x55, 0x7e, 0x6c, 0x4d, 0x2f, 0x22, 0x25, 0x41, 0x1, 0x5b, 0x76, 0x7f, 0x1c, 0x76, 0xc, 0x63, 0x52, 0x62, 0x4, 0x7e, 0x72, 0x3e, 0x63, 0x55, 0x29, 0x53, 0x37, 0x60, 0x5, 0x69, 0x24, 0x5, 0x74, 0x5, 0x77, 0x35, 0x68, 0x37, 0x4a, 0x57, 0x2, 0x55, 0x75, 0x71, 0x62, 0x62, 0x2f, 0x0, 0xf, 0x16, 0x5, 0x52, 0x54, 0x4e, 0x12, 0x6c, 0x5a, 0x75, 0x6, 0x62, 0x50, 0x4, 0x7c, 0x6, 0x19, 0x46, 0x42, 0x55, 0x36, 0x27, 0x55, 0x66, 0x51, 0x18, 0x6c, 0x4d, 0x5, 0x58, 0x6f, 0x74, 0x16, 0x71, 0x41, 0x40, 0x7c, 0x5c, 0x56, 0x7a, 0x7, 0x5e, 0x2, 0x5b, 0x3, 0x1a, 0x21, 0x56, 0x54, 0x61, 0x1, 0xb, 0x5d, 0xc, 0x7b, 0x7c, 0x11, 0x61, 0x73, 0x7d, 0x67, 0x5c, 0x27, 0x49, 0x7a, 0x4, 0x52, 0x43, 0x5f, 0x55, 0x15, 0x4f, 0x6d, 0x56, 0x0, 0x7c, 0x9, 0x15, 0x5c, 0x17, 0x46, 0x62, 0x68, 0x56, 0x57, 0x34, 0x2f, 0x6, 0x4e, 0x5d, 0x6, 0x5d, 0x0, 0x12, 0x63, 0x7c, 0x26, 0x5b, 0x5e, 0x7d, 0x58, 0x11, 0x41, 0x61, 0x26, 0x44, 0x35, 0x5f, 0x2, 0x74, 0x2, 0x68, 0x76, 0x59, 0x1, 0xd, 0x69, 0x29, 0x59, 0x7c, 0x11, 0x59, 0x3, 0x6f, 0x40, 0x7e, 0xc, 0x42, 0xe, 0x2d, 0x58, 0x4b, 0x29, 0xa, 0x6a, 0x14, 0x59, 0x56, 0x2, 0x71, 0x2d, 0x2d, 0xf, 0x11, 0x46, 0xd, 0x74, 0x30, 0x34, 0x3b, 0x54, 0x6b, 0x6, 0x4, 0x72, 0x74, 0x3, 0x56, 0xd, 0x5f, 0x2a, 0x7e, 0x56, 0x2, 0x50, 0x3e, 0x42, 0x61, 0x3d, 0x4e, 0x15, 0x6, 0x7f, 0x4b, 0xc, 0x61, 0x7, 0x6d, 0x77, 0x6, 0x73, 0x3, 0x54, 0xc, 0x2d, 0x51, 0x3, 0x1, 0x1, 0xf, 0x14, 0x3, 0x63, 0x21, 0x62, 0x4c, 0x33, 0x10, 0x6e, 0x54, 0x19, 0x59, 0x53, 0x3, 0x1, 0x32, 0x51, 0x4e, 0x0, 0x66, 0x49, 0x10, 0x13, 0x10, 0x15, 0x60, 0x51, 0x4, 0x75, 0x56, 0x43, 0x41, 0x43, 0x5b, 0x12, 0x61, 0x52, 0x45, 0x5a, 0x55, 0x52, 0xb, 0x28, 0x50, 0x34, 0x6f, 0x54, 0x1, 0x30, 0x1e, 0x7d, 0x6, 0x7b, 0x3a, 0x77, 0xe, 0x76, 0x7, 0x30, 0x54, 0x65, 0x1e, 0x46, 0x5a, 0x1b, 0x7b, 0x63, 0x10, 0x6e, 0x40, 0x1e, 0x38, 0xb, 0x4d, 0x56, 0x1e, 0x8, 0x60, 0x31, 0x2e, 0x60, 0x7, 0x47, 0x6d, 0x53, 0x24, 0x5c, 0x50, 0x3c, 0x0, 0x78, 0x5, 0x7c, 0x8, 0x60, 0x52, 0x5f, 0x6e, 0x2f, 0x76, 0x7, 0x65, 0x71, 0x2f, 0x54, 0x65, 0x2a, 0x53, 0x34, 0x51, 0x45, 0x2, 0x2f, 0x7b, 0x3, 0x5b, 0x7a, 0x11, 0x51, 0x16, 0x68, 0x1e, 0x33, 0x1b, 0x7e, 0x61, 0x64, 0x1d, 0x55, 0x6b, 0x49, 0x24, 0x6e, 0x43, 0x8, 0xc, 0x70, 0x5d, 0x57, 0x74, 0x4f, 0x1d, 0x2c, 0x16, 0x16, 0x30, 0xf, 0x54, 0x7e, 0x9, 0x0, 0xd, 0xf, 0x76, 0x4, 0x7d, 0x7c, 0x41, 0x55, 0x5c, 0xc, 0x64, 0x20, 0x51, 0x50, 0x74, 0x46, 0x31, 0x61, 0x7d, 0x5d, 0x5c, 0x57, 0x6d, 0x76, 0x63, 0x25, 0x62, 0x7c, 0x2, 0x78, 0x16, 0x57, 0x3f, 0x43, 0x6d, 0xe, 0x75, 0x45, 0x47, 0x40, 0x6f, 0x52, 0x77, 0x61, 0x1, 0x62, 0x63, 0x32, 0x11, 0x63, 0x5a, 0x5e, 0x68, 0x51, 0x1e, 0x28, 0x34, 0x4d, 0x8, 0x7b, 0x1e, 0x1, 0x4d, 0x7, 0x0, 0x11, 0x71, 0x4f, 0x73, 0x53, 0x7a, 0x4b, 0x6c, 0x72, 0x59, 0x12, 0x64, 0x78, 0x67, 0x56, 0x2c, 0x78, 0xb, 0x3d, 0x5, 0x13, 0x5e, 0x5b, 0x52, 0x48, 0x9, 0x66, 0x55, 0x0, 0x53, 0x51, 0xc, 0x52, 0x62, 0x59, 0x61, 0x7d, 0x71, 0x70, 0x67, 0x20, 0x71, 0x56, 0x53, 0x62, 0x4e, 0x37, 0x18, 0x78, 0x77, 0x77, 0x55, 0x2d, 0x64, 0x4a, 0x2a, 0x68, 0x5d, 0x74, 0x74, 0x5e, 0x5a, 0x20, 0x21, 0x25, 0x7e, 0x74, 0x76, 0x53, 0x69, 0x44, 0x7c, 0x52, 0x6e, 0x20, 0x62, 0x66, 0x5, 0x63, 0x25, 0x66, 0x7a, 0x4b, 0x74, 0x20, 0x52, 0x46, 0x73, 0x4, 0x74, 0x75, 0x18, 0x43, 0x2a, 0x7a, 0x24, 0x74, 0x74, 0x5, 0x72, 0x53, 0x7b, 0x67, 0x7e, 0x30, 0x7, 0x7c, 0x25, 0x50, 0x68, 0x33, 0x1b, 0x58, 0x7f, 0x72, 0x0, 0x32, 0x61, 0xa, 0x4e, 0x57, 0x17, 0x5, 0x63, 0x4b, 0x2d, 0x35, 0x7, 0x4f, 0x4, 0x72, 0x40, 0x52, 0x40, 0x60, 0x70, 0x64, 0x0, 0x31, 0x43, 0x70, 0x64, 0x6e, 0x2e, 0x7c, 0x5d, 0x3e, 0x7f, 0x9, 0x43, 0x52, 0x7f, 0x22, 0x60, 0x72, 0x7b, 0x76, 0x32, 0x79, 0x21, 0x57, 0x52, 0x26, 0x72, 0x75, 0x74, 0x8, 0x1, 0x0, 0x5b, 0xa, 0xc, 0x2, 0x7c, 0x33, 0x15, 0x40, 0x17, 0x79, 0x7c, 0x30, 0x59, 0x35, 0x29, 0x15, 0x28, 0x73, 0x6c, 0x6a, 0x27, 0x11, 0x39, 0x2, 0x69, 0x47, 0x1, 0x5b, 0x42, 0x7, 0x70, 0x5f, 0x73, 0x29, 0x77, 0x7f, 0x7b, 0xe, 0x22, 0x7c, 0x46, 0x2f, 0x55, 0x10, 0x59, 0x41, 0x53, 0x1a, 0x7b, 0x61, 0x5b, 0x4e, 0x4c, 0x59, 0xd, 0x79, 0x60, 0x58, 0x1f, 0x65, 0x71, 0x40, 0x75, 0x2a, 0x50, 0x58, 0x22, 0x7d, 0x6f, 0x2a, 0xf, 0x6b, 0xd, 0x2, 0x54, 0x39, 0x1d, 0x17, 0xb, 0x56, 0x49, 0x7e, 0x7b, 0x69, 0x1, 0x8, 0x52, 0x1d, 0x45, 0x76, 0x1, 0x60, 0x54, 0x43, 0x4b, 0x5b, 0x40, 0x29, 0x7c, 0x7a, 0x41, 0x67, 0x2b, 0x4, 0x51, 0x1c, 0x45, 0x8, 0x4, 0x41, 0x1, 0x27, 0x6, 0x72, 0x58, 0x52, 0x22, 0x41, 0x55, 0x5f, 0x74, 0x2, 0x62, 0x44, 0x6, 0x7f, 0x51, 0x35, 0x7c, 0x6e, 0x13, 0x6f, 0xe, 0x1c, 0x36, 0x49, 0x5c, 0x7, 0x42, 0x2d, 0x7b, 0x13, 0x26, 0x40, 0x53, 0x1a, 0x59, 0x55, 0x32, 0x54, 0xa, 0x26, 0x58, 0x62, 0x4f, 0x52, 0x60, 0x5c, 0x4b, 0x74, 0x79, 0x4f, 0x79, 0x66, 0x4, 0x64, 0x1e, 0x72, 0x65, 0x3d, 0x67, 0xf, 0x7, 0x5a, 0x2, 0xb, 0x52, 0x7f, 0x52, 0x47, 0x11, 0x4e, 0x31, 0x4a, 0x44, 0x26, 0x6a, 0x59, 0x56, 0x67, 0x4f, 0x33, 0x77, 0x50, 0xb, 0x7b, 0x6e, 0x2, 0x55, 0x5b, 0x6c, 0x50, 0x5b, 0x1b, 0x75, 0x29, 0x57, 0x41, 0x5d, 0x41, 0x19, 0x5e, 0x2c, 0x2b, 0x34, 0x30, 0x64, 0x1c, 0x64, 0x50, 0xe, 0x47, 0x8, 0x43, 0x67, 0x32, 0x66, 0x1f, 0x44, 0x75, 0x13, 0x64, 0x7e, 0x23, 0x63, 0x2a, 0x78, 0x49, 0x5e, 0x35, 0x54, 0x52, 0x59, 0x55, 0x33, 0x0, 0x35, 0x68, 0x5b, 0x57, 0x45, 0x1, 0x77, 0x75, 0x4c, 0x2f, 0x58, 0x4e, 0x39, 0x7c, 0x68, 0x2b, 0x35, 0x68, 0x42, 0x73, 0x5e, 0x17, 0x19, 0x22, 0x33, 0x75, 0x53, 0x7, 0x4f, 0x14, 0x54, 0x11, 0x56, 0x2b, 0x66, 0x0, 0x4, 0x77, 0x61, 0x54, 0x73, 0x2, 0x6f, 0x2d, 0x7f, 0x42, 0x7e, 0x79, 0x4f, 0x52, 0x74, 0x2b, 0x71, 0x58, 0x59, 0x47, 0x68, 0x4, 0x5d, 0x58, 0xf, 0x78, 0x1a, 0x7c, 0x2f, 0x7, 0x1, 0x30, 0x52, 0x4e, 0x5b, 0x67, 0x5e, 0x4e, 0x7b, 0x5a, 0x14, 0x4d, 0x49, 0x15, 0x55, 0x4d, 0xb, 0x64, 0x70, 0x4c, 0x61, 0x51, 0x33, 0x8, 0xc, 0x5b, 0x40, 0x5d, 0x4e, 0x52, 0xd, 0x57, 0x5b, 0x7e, 0x79, 0x44, 0x9, 0x74, 0x4e, 0x71, 0x50, 0x32, 0x59, 0x72, 0x5e, 0x5c, 0x54, 0x58, 0x7b, 0x23, 0xe, 0x4a, 0x46, 0x4, 0x4b, 0x56, 0x5, 0x5f, 0x4f, 0x77, 0x2a, 0x79, 0x27, 0x57, 0x62, 0x3b, 0x43, 0x76, 0x47, 0x3, 0x40, 0x15, 0x74, 0x75, 0x15, 0x59, 0xa, 0x4d, 0x1a, 0x7c, 0x7c, 0x52, 0x74, 0x17, 0x60, 0x15, 0x3b, 0x0, 0x2b, 0x2, 0x65, 0x6c, 0x2d, 0x49, 0x33, 0x36, 0x7b, 0x46, 0x18, 0x1b, 0x7a, 0x7b, 0x4f, 0x61, 0x5d, 0x22, 0x40, 0xd, 0x45, 0x40, 0x3e, 0x44, 0x7, 0xf, 0x1d, 0x24, 0x77, 0x44, 0x8, 0x28, 0x2, 0x7e, 0x7, 0x63, 0x39, 0x4f, 0x57, 0x60, 0x77, 0x36, 0x58, 0x4e, 0x79, 0x58, 0x7a, 0x20, 0x4, 0x69, 0x7, 0x5c, 0x7d, 0xf, 0x26, 0x4f, 0x68, 0x42, 0x7, 0x10, 0x78, 0x31, 0x5a, 0x9, 0x13, 0x79, 0x63, 0x57, 0x5a, 0x12, 0x4f, 0x8, 0x67, 0x7e, 0x5a, 0xd, 0x13, 0x5f, 0x41, 0x5f, 0x5, 0x30, 0x55, 0x51, 0x5c, 0x46, 0x7, 0x67, 0x53, 0x32, 0x64, 0x23, 0x60, 0x76, 0x6, 0x0, 0x5c, 0x7e, 0x4f, 0x1f, 0xa, 0x6d, 0x23, 0x60, 0x5b, 0x11, 0x44, 0x40, 0x5b, 0x41, 0xe, 0x32, 0x45, 0x6b, 0x34, 0x5b, 0x68, 0x5b, 0x5f };
     return getOriginalKey(obfuscatedSecret, sizeof(obfuscatedSecret), packageName, pEnv);
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_greatfire_wikiunblocked_Secrets_getdefProxy(
        JNIEnv* pEnv,
        jobject pThis,
        jstring packageName) {
     char obfuscatedSecret[] = { 0x53, 0xf, 0x45, 0x56, 0x1a, 0xd, 0x16, 0x49, 0x5d, 0x4c, 0x4a, 0x59, 0xf, 0xb, 0x46, 0x12, 0x12, 0x4a, 0x40, 0x5, 0x74, 0x1d, 0x50, 0x23, 0x46, 0x56, 0x75, 0x55, 0x45, 0x51, 0x59, 0x40, 0x4d, 0x45, 0x5b, 0x5, 0x40, 0x51, 0x1b, 0x56, 0xe, 0x59, 0x54, 0x4a, 0x59, 0x13, 0x52, 0x14, 0x3, 0x25, 0x46, 0x5e, 0x5c, 0x5d, 0x13, 0x5d, 0x1, 0x59, 0x54, 0x44, 0x2, 0x72, 0x13, 0x50, 0x52, 0x5, 0x41, 0x5c, 0x10, 0x44, 0x4, 0x57, 0x52, 0xd, 0x16, 0x7, 0x3, 0x4d, 0x4, 0x5f, 0x4c, 0xe, 0x57, 0x10, 0x5d, 0x5d, 0x3, 0x1, 0x6, 0x16, 0x6c, 0x7f, 0x58, 0x47, 0x4c, 0xe, 0x58, 0x57, 0x54, 0x4a, 0x55, 0x5e, 0x5b, 0x51, 0x4a, 0x58, 0x40, 0x3, 0x1a, 0x4, 0x5b, 0x47, 0x5e, 0x1a, 0xb, 0x18, 0x18, 0xb, 0x16, 0x4a, 0x9, 0xd, 0x5d, 0x15, 0x44, 0x44, 0x46, 0x14, 0x5, 0x20, 0x16, 0xb, 0x25, 0x12, 0xb, 0x20, 0x11, 0x4d, 0x59, 0x41, 0x57, 0xe, 0x57, 0x8, 0x16, 0x17, 0x4, 0x5c, 0x5b, 0x5e, 0x4c, 0xa, 0x11, 0x3, 0x16, 0x5, 0x71, 0x43, 0x51, 0x58, 0x50, 0x45, 0x52, 0x0, 0x5d, 0x55, 0x10, 0x5, 0x22, 0x11, 0x53, 0x0, 0x52, 0x13, 0x50, 0x42, 0x42, 0x5e, 0x0, 0x0, 0x5, 0x1a, 0x55, 0xf, 0x4b, 0x2, 0x5, 0x57, 0x1e, 0x5, 0x6, 0x17, 0x5e, 0x4, 0x52, 0x5d, 0x6, 0x45, 0x66, 0x2e, 0xd, 0x4a, 0x4c, 0x8, 0x53, 0x1, 0x51, 0x48, 0x3, 0x53, 0xb, 0x50, 0x1b, 0x57, 0x10, 0x2, 0x4f, 0x1, 0x5d, 0x41, 0x58, 0x4d, 0x2, 0x1c, 0x16, 0xa, 0x42, 0x16, 0x58, 0x9, 0x5d, 0x43, 0x10, 0x47, 0x41, 0x41, 0x5, 0x20, 0x10, 0x3, 0x77, 0x46, 0x3, 0x71, 0x40, 0x55, 0x4, 0x57, 0xb, 0x1e, 0x4c, 0x14, 0x40, 0x78, 0x1b, 0x5e, 0x44, 0x6, 0x16, 0xb, 0x25, 0x40, 0x50, 0xd, 0xb, 0x49, 0x5d, 0x51, 0x5b, 0x2, 0x17, 0x54, 0x24, 0x1f, 0x4, 0x52, 0x51, 0x4a, 0x7, 0x16, 0x10, 0x59, 0x2, 0x7, 0x3, 0x1a, 0xa, 0x2, 0x17, 0x6, 0x5, 0x4a, 0x5, 0x1, 0x3, 0x11, 0xc, 0x52, 0x53, 0x0, 0x53, 0x13, 0x6a, 0x79, 0x5e, 0x10, 0x45, 0xa, 0x56, 0x56, 0x0, 0x16, 0x1c, 0x45, 0x45, 0x2d, 0x1e, 0x5b, 0x47, 0x56, 0x1a, 0x4, 0x5d, 0x4f, 0xc, 0x4e, 0x3, 0x49, 0x4d, 0x6, 0x4d, 0x47, 0x5e, 0x5e, 0x5a, 0x12, 0x16, 0x49, 0x16, 0x13, 0x6, 0x79, 0x47, 0x57, 0x25, 0x41, 0x1, 0x71, 0x55, 0x55, 0x4a, 0x1d, 0x40, 0x40, 0x47, 0x28, 0x1a, 0x5b, 0x47, 0x50, 0x41, 0x5, 0x74, 0x13, 0x5f, 0xa, 0x5c, 0x41, 0x54, 0x7, 0x58, 0x56, 0x12, 0x6, 0x25, 0x1e, 0x4, 0x54, 0x51, 0x13, 0x55, 0x47, 0x46, 0xc, 0x7, 0x56, 0x1, 0x17, 0x55, 0x0, 0x17, 0x54, 0x50, 0xa, 0x16, 0x7, 0x1, 0x54, 0x14, 0xe, 0x7, 0x58, 0x1, 0x53, 0x47, 0x67, 0x2a, 0xa, 0x10, 0x10, 0xe, 0x56, 0x55, 0x57, 0x16, 0x4a, 0x4c, 0x45, 0x7b, 0x4a, 0x5b, 0x46, 0x52, 0x1b, 0x1, 0x59, 0x44, 0xb, 0x4f, 0x5b, 0x1a, 0x1e, 0xe, 0x16, 0x43, 0x5b, 0xa, 0x5c, 0x17, 0x4c, 0x15, 0x43, 0x10, 0x52, 0x71, 0x11, 0x7, 0x77, 0x13, 0x53, 0x75, 0x5d, 0xa, 0x54, 0x4d, 0xf, 0xd, 0x57, 0x16, 0x44, 0x57, 0xc, 0x47, 0x48, 0xd, 0x4b, 0x2, 0x13, 0x7, 0x7e, 0x15, 0xc, 0x8, 0xd, 0x43, 0x52, 0x53, 0x5d, 0x59, 0x16, 0xb, 0x73, 0x11, 0x5, 0x50, 0x50, 0x47, 0x52, 0x17, 0x44, 0xf, 0x55, 0x6, 0x55, 0x1b, 0x3, 0x0, 0x4d, 0x2, 0xe, 0x19, 0x6, 0x57, 0xe, 0x43, 0x58, 0x50, 0x0, 0x54, 0x51, 0x47, 0x6e, 0x7e, 0xe, 0x40, 0x4d, 0x5e, 0x56, 0x5b, 0x5, 0x4c, 0x48, 0x5d, 0x5a, 0x47, 0x4d, 0x5d, 0x14, 0x5, 0x15, 0x0, 0x58, 0x43, 0x57, 0x1b, 0x5f, 0x4c, 0x4b, 0xc, 0x42, 0x45, 0x58, 0x5, 0x5b, 0x4d, 0x41, 0x47, 0x17, 0x11, 0x7, 0x74, 0x12, 0x56, 0x71, 0x17, 0x56, 0x70, 0x3, 0x40, 0x5d, 0x5a, 0x4d, 0x40, 0x52, 0x58, 0x41, 0x4d, 0x57, 0x17, 0x57, 0x10, 0x53, 0x76, 0x43, 0x5c, 0x5a, 0x5f, 0x11, 0x56, 0x5d, 0xa, 0x56, 0x1c, 0x54, 0x24, 0x1f, 0x59, 0x51, 0x56, 0x11, 0x57, 0x15, 0x11, 0x4, 0x54, 0x1, 0x7, 0x16, 0x54, 0x52, 0x4d, 0x55, 0x4, 0x5, 0x19, 0x0, 0xd, 0x15, 0x51, 0x50, 0x56, 0x0, 0x51, 0x46, 0x6a, 0x7f, 0xb, 0x44, 0x46, 0x59, 0x57, 0x3, 0x56, 0x1f, 0x40, 0x6, 0x5e, 0x42, 0x19, 0x5b, 0x11, 0x5f, 0x49, 0x55, 0x5b, 0x17, 0x5f, 0x4d, 0xf, 0x1e, 0x19, 0x5e, 0x46, 0x4b, 0xf, 0xa, 0x51, 0x12, 0x16, 0x49, 0x4b, 0x10, 0x1, 0x22, 0x17, 0x54, 0x24, 0x1c, 0x57, 0x70, 0x56, 0x57, 0xe, 0x9, 0x6, 0x7, 0x47, 0x58, 0x45, 0x1a, 0x5c, 0x5a, 0x43, 0x58, 0x19, 0xb, 0x46, 0x53, 0x10, 0x5, 0x22, 0x40, 0x5b, 0xf, 0x5f, 0x11, 0x50, 0x55, 0x58, 0x2, 0x14, 0x5, 0x71, 0x12, 0x2, 0x5c, 0x1, 0x42, 0x50, 0x12, 0x43, 0x9, 0x4, 0x1, 0x2, 0x4f, 0x1, 0x8, 0x4d, 0xe, 0x17, 0x5e, 0x52, 0x1f, 0x50, 0x50, 0x53, 0x7, 0x57, 0x14, 0x3d, 0x71, 0xa, 0x45, 0x41, 0x5, 0x3, 0x7, 0x0, 0x4a, 0x57, 0x5e, 0x4d, 0x59, 0x16, 0x5c, 0x4b, 0x52, 0x1b, 0x1, 0x5a, 0x42, 0x5a, 0x4e, 0x5e, 0x18, 0x1d, 0x5b, 0x43, 0x13, 0x59, 0xc, 0x59, 0x17, 0x45, 0x47, 0x44, 0x11, 0x50, 0x79, 0x40, 0x2, 0x73, 0x44, 0x2, 0x72, 0x46, 0x52, 0x5e, 0xe, 0x5c, 0x55, 0xb, 0x58, 0x4c, 0x15, 0x7, 0x17, 0x5c, 0x5c, 0x48, 0xe, 0x1c, 0x9, 0x10, 0x5e, 0x40, 0x4, 0x73, 0x4f, 0xb, 0xe, 0xa, 0x14, 0x56, 0x53, 0x5e, 0x55, 0x1d, 0x1, 0x7f, 0x13, 0x56, 0x0, 0x50, 0x46, 0x50, 0x44, 0x17, 0xa, 0x3, 0x53, 0x4, 0x4f, 0x3, 0x6, 0x1f, 0x52, 0x9, 0xe, 0x19, 0x0, 0x54, 0x1e, 0xd, 0x55, 0x54, 0x5, 0x55, 0x46, 0x6a, 0x79, 0x59, 0x12, 0x47, 0x4, 0x2, 0x55, 0x5a, 0x48, 0x6, 0x50, 0x42, 0x58, 0x1c, 0xc, 0x40, 0x1, 0x4e, 0x5c, 0xb, 0x40, 0x5a, 0x41, 0x58, 0x4a, 0x4c, 0x5b, 0x46, 0x45, 0x5b, 0x9, 0x50, 0x47, 0x4d, 0x45, 0x44, 0x41, 0x7, 0x75, 0x10, 0x5, 0x22, 0x12, 0x0, 0x22, 0x5e, 0x4, 0x54, 0x55, 0x5d, 0xa, 0x5f, 0x52, 0x19, 0x46, 0xa, 0x4b, 0xd, 0x55, 0x5b, 0x6, 0x5a, 0x5d, 0x5b, 0x1f, 0x55, 0xe, 0x5e, 0x1c, 0x51, 0x71, 0x4e, 0xf, 0x9, 0x50, 0x48, 0x50, 0x56, 0xa, 0x53, 0x43, 0x50, 0x7f, 0x43, 0x57, 0x51, 0x5c, 0x10, 0x0, 0x10, 0x17, 0xe, 0x6, 0x7, 0x0, 0x16, 0x1, 0x8, 0x1b, 0x6, 0x52, 0x1a, 0x5, 0xd, 0xe, 0x42, 0x5f, 0x57, 0x5, 0x52, 0x4, 0x47, 0x6e, 0x79, 0xc, 0x42, 0x43, 0xa, 0x55, 0x1, 0x5b, 0x4b, 0x42, 0x5c, 0x12, 0x58, 0x51, 0x5b, 0x56, 0x5c, 0x8, 0x5d, 0x17, 0x0, 0x58, 0x54, 0x4a, 0x7, 0x57, 0x4e, 0x5a, 0x4b, 0x59, 0x1d, 0x49, 0x5d, 0x4c, 0x17, 0x5a, 0x8, 0x50, 0x16, 0x11, 0x13, 0x17, 0x16, 0x4, 0x76, 0x11, 0xa, 0x75, 0x1c, 0x7, 0x71, 0x2, 0x51, 0x58, 0x5a, 0x59, 0x1d, 0x19, 0x40, 0xd, 0x45, 0x9, 0x50, 0x5f, 0x56, 0x9, 0x58, 0x59, 0x19, 0x57, 0xc, 0x55, 0x40, 0x2, 0x73, 0x16, 0x59, 0x5f, 0x5c, 0x41, 0x53, 0x5, 0x5a, 0x58, 0x46, 0x5, 0x7f, 0x40, 0x3, 0x5d, 0x5c, 0x47, 0x57, 0x10, 0x41, 0x5b, 0x53, 0xe, 0x57, 0x18, 0x3, 0xf, 0x4c, 0x57, 0x52, 0x51, 0x1d, 0x6, 0x3, 0x5, 0x1e, 0x5b, 0x5c, 0x54, 0x53, 0x1, 0x46, 0x6b, 0x7d, 0x58, 0x17, 0x43, 0xf, 0x5, 0x54, 0x2, 0x1b, 0x43, 0x58, 0x10, 0x59, 0x52, 0x59, 0x53, 0x9, 0x51, 0xb, 0x1e, 0x56, 0xe, 0x5d, 0x18, 0x50, 0x5f, 0x40, 0xe, 0x4a, 0x3, 0x4c, 0x18, 0x6, 0x13, 0x10, 0x55, 0x5, 0x5d, 0x46, 0x17, 0x42, 0x15, 0x47, 0xa, 0x24, 0x13, 0x7, 0x7e, 0x47, 0x57, 0x25, 0x6, 0x52, 0x44, 0x52, 0x58, 0x51, 0x5d, 0x5c, 0x1b, 0x51, 0x0, 0x59, 0x4d, 0x1b, 0x59, 0x1, 0x43, 0x17, 0x56, 0x70, 0x16, 0x5c, 0x5a, 0x58, 0x13, 0x54, 0x53, 0x5e, 0x55, 0x46, 0xa, 0x23, 0x16, 0x54, 0x5, 0x54, 0x46, 0x50, 0x42, 0x45, 0x5c, 0x2, 0xe, 0x51, 0x19, 0xf, 0x51, 0x4c, 0xb, 0x9, 0x4, 0x1c, 0x51, 0x1, 0x57, 0x44, 0x51, 0x0, 0x57, 0x51, 0x5d, 0x10, 0x3a, 0x2b, 0xb, 0x40, 0x43, 0xa, 0x55, 0x5a, 0x50, 0x17, 0x53, 0x53, 0x9, 0x4d, 0x1a, 0x5b, 0x52, 0x10, 0x1b, 0x57, 0xa, 0x40, 0xe, 0x4c, 0xb, 0x1e, 0x4c, 0xe, 0x42, 0x45, 0x58, 0x5e, 0x50, 0x11, 0x44, 0x45, 0x12, 0x15, 0x7, 0x74, 0x14, 0x4, 0x27, 0x16, 0xb, 0x25, 0x44, 0x5c, 0x17, 0x17, 0x5c, 0x56, 0x56, 0x57, 0x4d, 0x54, 0x2, 0xf, 0x40, 0x4b, 0x58, 0x50, 0x4c, 0x47, 0x57, 0x25, 0x13, 0x5a, 0x5c, 0x5e, 0x44, 0x5d, 0x57, 0x50, 0x54, 0x12, 0x56, 0x72, 0x12, 0x54, 0x53, 0x0, 0x45, 0x57, 0x17, 0x45, 0x5c, 0x4, 0x1, 0x5, 0x4d, 0x3, 0x6, 0x19, 0x3, 0x54, 0x16, 0x57, 0x1, 0x3, 0x47, 0x58, 0x51, 0x54, 0x55, 0x53, 0x13, 0x6c, 0x71, 0xc, 0x44, 0x4d, 0x5b, 0x3, 0x5b, 0x5b, 0x1b, 0x54, 0x7, 0x5f, 0x1f, 0x4c, 0x57, 0x0, 0x42, 0x19, 0x5d, 0xc, 0x13, 0xc, 0x1d, 0x9, 0x18, 0x18, 0xb, 0x4d, 0x41, 0x55, 0x8, 0x5f, 0x10, 0x40, 0x44, 0x46, 0x12, 0x57, 0x76, 0x17, 0x56, 0x70, 0x44, 0x7, 0x77, 0x5d, 0xa, 0x53, 0x52, 0x45, 0x40, 0x1a, 0x16, 0x53, 0x6, 0xd, 0x57, 0x1, 0x1a, 0x56, 0x52, 0x13, 0x53, 0x75, 0x4e, 0xa, 0x5c, 0x50, 0x16, 0x7, 0x5d, 0x51, 0x54, 0x17, 0x51, 0x74, 0x40, 0x3, 0x5d, 0x1, 0x44, 0x50, 0x4b, 0x11, 0x58, 0x52, 0x53, 0x1, 0x19, 0x1, 0x3, 0x16, 0x2, 0xd, 0x4, 0x19, 0x51, 0xc, 0x12, 0x5d, 0x52, 0x5, 0x53, 0x57, 0x16, 0x69, 0x29, 0x5a, 0x42, 0x45, 0x5e, 0x50, 0x55, 0x54, 0x1a, 0x55, 0xe, 0x5d, 0x6, 0x4, 0x4f, 0x53, 0x57, 0x19, 0x54, 0x58, 0x17, 0x5c, 0x40, 0x59, 0x18, 0x16, 0x59, 0x17, 0x4b, 0x54, 0x8, 0x5a, 0x17, 0x46, 0x16, 0x11, 0x1c, 0x56, 0x77, 0x10, 0xa, 0x24, 0x40, 0x51, 0x22, 0x50, 0x58, 0x59, 0x50, 0x51, 0x47, 0x50, 0x5a, 0x59, 0x4a, 0x2, 0x2, 0xd, 0x1, 0x55, 0x19, 0x51, 0x7, 0x13, 0x53, 0x73, 0x46, 0x58, 0x8, 0x58, 0x47, 0x52, 0x50, 0xa, 0x59, 0x40, 0x2, 0x73, 0x47, 0x51, 0x50, 0x51, 0x43, 0x53, 0x12, 0x40, 0x4, 0x52, 0x7, 0xd, 0x48, 0x50, 0x8, 0x16, 0x3, 0x0, 0x4d, 0x0, 0x52, 0x5b, 0x1f, 0xd, 0x53, 0x54, 0x5c, 0x7, 0x17, 0x3c, 0x2c, 0x5c, 0x44, 0x43, 0x9, 0x59, 0x51, 0x5a, 0x1b, 0x1, 0x52, 0xc, 0x2, 0x4, 0x19, 0x7, 0x54, 0x1e, 0x1, 0x58, 0x17, 0x5a, 0x48, 0xb, 0x4c, 0x1e, 0x8, 0x42, 0x46, 0xf, 0x5, 0xd, 0x44, 0x41, 0x11, 0x43, 0x11, 0x6, 0x70, 0x13, 0x53, 0x75, 0x1c, 0x51, 0x71, 0x5a, 0x9, 0xf, 0x5b, 0x51, 0x5b, 0x57, 0x4d, 0x4b, 0xf, 0x13, 0x50, 0x4, 0x58, 0x79, 0x51, 0xc, 0x2, 0x1a, 0xd, 0x1d, 0x54, 0x58, 0x59, 0x1d, 0x1, 0x7f, 0x42, 0x5e, 0xf, 0x5d, 0x44, 0x50, 0x53, 0xd, 0x56, 0x17, 0x56, 0x70, 0x47, 0x54, 0x55, 0x55, 0x11, 0x54, 0x44, 0x44, 0x9, 0x52, 0x8, 0x51, 0x1e, 0x7, 0x50, 0x1e, 0x3, 0xd, 0x1f, 0xf, 0x57, 0x15, 0x51, 0x6, 0x56, 0x5d, 0x3, 0x10, 0x66, 0x70, 0x5a, 0x41, 0x17, 0xf, 0x7, 0x0, 0x5a, 0x4b, 0x4f, 0x5c, 0x49, 0xb, 0x4, 0xd, 0x28, 0x5a, 0x59, 0x50, 0x4d, 0x51, 0x1d, 0x5a, 0x5a, 0x5a, 0x48, 0x51, 0x5a, 0x43, 0x58, 0x1d, 0xd, 0x1d, 0x4b, 0x9, 0x14, 0x47, 0x5d, 0xc, 0xb, 0x45, 0x43, 0x47, 0x47, 0x46, 0xb, 0x24, 0x15, 0x7, 0x27, 0x15, 0x6, 0x73, 0x58, 0x58, 0x2, 0x41, 0x5c, 0x2, 0x44, 0x5c, 0x48, 0x1b, 0x50, 0x49, 0x5c, 0x53, 0xd, 0x7e, 0xf, 0xc, 0x5e, 0x1c, 0x5f, 0x1b, 0x5b, 0xd, 0x8, 0x46, 0x56, 0x75, 0x40, 0x5e, 0x5f, 0x51, 0x43, 0x5c, 0x51, 0x5e, 0x5, 0x11, 0x6, 0x73, 0x11, 0x5, 0x53, 0x56, 0x16, 0x53, 0x12, 0x46, 0xc, 0x0, 0x54, 0x3, 0x19, 0x1, 0x3, 0x4d, 0xa, 0x54, 0x9, 0x1b, 0x53, 0x3, 0xd, 0x13, 0x59, 0x53, 0x0, 0x57, 0x5c, 0x11, 0x68, 0x71, 0x9, 0x11, 0x4d, 0x5, 0x54, 0x50, 0x0, 0x1c, 0x1f, 0xb, 0x48, 0xc, 0x57, 0x5b, 0x74, 0xb, 0xb, 0x4, 0x1d, 0x5a, 0x19, 0x54, 0x5b, 0x55, 0x1f, 0x5c, 0x5b, 0x41, 0xb, 0x4d, 0xe, 0x1a, 0x18, 0x5b, 0x42, 0x40, 0x8, 0xb, 0x9, 0x41, 0x45, 0x41, 0x10, 0x14, 0x4, 0x76, 0x11, 0x51, 0x7e, 0x40, 0x2, 0x73, 0x2, 0x5f, 0x5a, 0x41, 0x54, 0x45, 0x15, 0x1d, 0x53, 0xa, 0x59, 0x5f, 0x3, 0xc, 0x5e, 0x55, 0x5c, 0x5c, 0x4, 0x1c, 0x5, 0xd, 0x54, 0x40, 0x4, 0x73, 0x4f, 0xb, 0xe, 0xa, 0x14, 0x56, 0x53, 0x5e, 0x55, 0x1d, 0x1, 0x7f, 0x13, 0x56, 0x0, 0x50, 0x46, 0x50, 0x44, 0x17, 0xa, 0x3, 0x54, 0x2, 0x4f, 0x7, 0x0, 0x1f, 0x54, 0x3, 0x19, 0x6, 0x3, 0x52, 0x1e, 0xd, 0x55, 0x54, 0x5, 0x55, 0x46, 0x6a, 0x79, 0x59, 0x12, 0x47, 0x4, 0x2, 0x55, 0x5a, 0x48, 0x8, 0x50, 0x56, 0x53, 0x57, 0xd, 0x55, 0xb, 0xb, 0x57, 0x2, 0x18, 0x56, 0x57, 0xf, 0x49, 0x6, 0xa, 0x45, 0x58, 0x4e, 0xe, 0x17, 0x1c, 0x6, 0x40, 0x45, 0x8, 0x9, 0x5c, 0x41, 0x43, 0x14, 0x44, 0x17, 0x57, 0x77, 0x44, 0x7, 0x77, 0x14, 0x51, 0x77, 0x43, 0x52, 0x47, 0x17, 0x51, 0x8, 0x5f, 0x5b, 0x18, 0x1e, 0x5e, 0x5c, 0x5f, 0x50, 0x4, 0x5d, 0x5e, 0xe, 0x5e, 0x57, 0x1, 0x4c, 0x5a, 0x57, 0x58, 0x17, 0x51, 0x74, 0x11, 0xb, 0x52, 0xc, 0x46, 0x50, 0x5c, 0xb, 0x4, 0x46, 0x56, 0x75, 0x11, 0x56, 0x50, 0x5c, 0x41, 0x5c, 0x46, 0x44, 0x59, 0x5, 0x3, 0x7, 0x19, 0x52, 0x0, 0x1c, 0x55, 0xe, 0x56, 0x1b, 0x2, 0x7, 0x45, 0x59, 0x52, 0x56, 0x50, 0x6, 0x4a, 0x3a, 0x78, 0x5a, 0x12, 0x44, 0x9, 0x54, 0x53, 0x55, 0x4f, 0x59, 0x50, 0xd, 0x51, 0x5c, 0x8, 0x5, 0x54, 0x51, 0x5b, 0x55, 0x4d, 0x51, 0x9, 0xf, 0x15, 0x16, 0x45, 0xf, 0x17, 0x4d, 0x3c, 0x51, 0xc, 0x5b, 0x6e, 0x5, 0x5c, 0x50, 0x7e, 0x53, 0x74, 0x43, 0x5, 0x63, 0x62, 0x5, 0x6d, 0xd, 0x6, 0x45, 0x6, 0x4, 0x19, 0x0, 0x7c, 0x65, 0x2e, 0x46, 0x79, 0x63, 0x44, 0x53, 0x59, 0x22, 0x9, 0x5, 0x38, 0x67, 0x58, 0x42, 0x7f, 0x71, 0x37, 0x43, 0x60, 0xe, 0x71, 0x49, 0x2b, 0x2a, 0x51, 0x50, 0x54, 0x65, 0x29, 0x3, 0x5, 0x3a, 0x6f, 0x15, 0x7b, 0x62, 0x54, 0xe, 0x4, 0x50, 0x31, 0x0, 0x61, 0x70, 0xd, 0x4e, 0x52, 0x4e, 0x8, 0xa, 0x24, 0x5, 0x7, 0xc, 0x19, 0x55, 0x1, 0x0, 0x4a, 0x0, 0x52, 0x1b, 0x3, 0x0, 0x53, 0xb, 0x5, 0xf, 0x7, 0x55, 0x0, 0x49, 0x46, 0x7, 0x12, 0x42, 0x40, 0x45, 0xb, 0x19, 0x4e, 0x2, 0xa, 0x5a, 0x19, 0x8, 0x50, 0x50, 0x17, 0xe, 0x6, 0x1c, 0x51, 0x2, 0x5e, 0x58, 0xb, 0x56, 0xe, 0x4, 0xf, 0x5d, 0xc, 0x7, 0x59, 0x51, 0x53, 0x2, 0x56, 0xd, 0xa, 0x5d, 0x6, 0x1a, 0x5, 0x7, 0x1, 0x56, 0x1a, 0x50, 0x6, 0x5, 0x2, 0x1b, 0x59, 0x53, 0x3, 0x1, 0x4e, 0x1, 0xf, 0x0, 0x2, 0x6, 0xc, 0x7, 0x5, 0x54, 0x58, 0x51, 0x55, 0x19, 0x47, 0x4, 0x16, 0x56, 0x5a, 0xb, 0x56, 0x4d, 0x5c, 0x4d, 0x16, 0x9, 0x6, 0xb, 0x4d, 0x3, 0x50, 0x50, 0x17, 0x51, 0x4, 0x1b, 0xa, 0x53, 0x54, 0x59, 0x51, 0x4, 0xf, 0xe, 0x3, 0x7, 0x5a, 0x5d, 0x8, 0x55, 0x0, 0x1, 0x56, 0x0, 0xe, 0x0, 0x4, 0x1f, 0x5, 0x5, 0x54, 0x56, 0x1c, 0x5, 0x52, 0x6, 0x51, 0x1a, 0xc, 0x5, 0xa, 0x55, 0x1d, 0x5, 0x59, 0x7, 0x2, 0x50, 0x5, 0x54, 0x54, 0x52, 0x0, 0x2, 0x56, 0x15, 0x15, 0xc, 0x56, 0x4f, 0x53, 0x5e, 0x2, 0x59, 0x3, 0x58, 0x16, 0x4a, 0x9, 0x57, 0x4a, 0xd, 0xe, 0x6, 0x16, 0xe, 0x5f, 0x43, 0x40, 0x48, 0x40, 0x3, 0x1a, 0x18, 0x5, 0x56, 0x57, 0x1b, 0x55, 0x14, 0x5a, 0x5d, 0x4a, 0x59, 0x13, 0x52, 0x1e, 0x17, 0x17, 0x44, 0x59, 0x59, 0x51, 0xf, 0x5, 0xd, 0x44, 0x41, 0x11, 0x43, 0xe, 0x1a, 0x1e, 0x57, 0x3, 0x50, 0x17, 0x1a, 0x42, 0x58, 0x8, 0x8, 0x50, 0x59, 0x4d, 0x5b, 0x2, 0x5d, 0x48, 0x1, 0x56, 0x8, 0x19, 0x42, 0x51, 0x9, 0xc, 0x13, 0x1, 0x57, 0x5e, 0x56, 0x1b, 0x1e, 0x55, 0x4b, 0x5a, 0x59, 0x10, 0x9, 0x1a, 0x57, 0x47, 0x9, 0x58, 0x1c, 0xb, 0x44, 0x6 };
     return getOriginalKey(obfuscatedSecret, sizeof(obfuscatedSecret), packageName, pEnv);
}
