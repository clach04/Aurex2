/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class jp_ruputer_AndroAurex_JniIF */

#ifndef _Included_jp_ruputer_AndroAurex_JniIF
#define _Included_jp_ruputer_AndroAurex_JniIF
#ifdef __cplusplus
extern "C" {
#endif
#undef jp_ruputer_AndroAurex_JniIF_BUTTON_ENTER
#define jp_ruputer_AndroAurex_JniIF_BUTTON_ENTER 8L
#undef jp_ruputer_AndroAurex_JniIF_BUTTON_EL
#define jp_ruputer_AndroAurex_JniIF_BUTTON_EL 4L
#undef jp_ruputer_AndroAurex_JniIF_BUTTON_FILE
#define jp_ruputer_AndroAurex_JniIF_BUTTON_FILE 1L
#undef jp_ruputer_AndroAurex_JniIF_BUTTON_MENU
#define jp_ruputer_AndroAurex_JniIF_BUTTON_MENU 2L
#undef jp_ruputer_AndroAurex_JniIF_BUTTON_DOWN
#define jp_ruputer_AndroAurex_JniIF_BUTTON_DOWN 128L
#undef jp_ruputer_AndroAurex_JniIF_BUTTON_UP
#define jp_ruputer_AndroAurex_JniIF_BUTTON_UP 32L
#undef jp_ruputer_AndroAurex_JniIF_BUTTON_RIGHT
#define jp_ruputer_AndroAurex_JniIF_BUTTON_RIGHT 64L
#undef jp_ruputer_AndroAurex_JniIF_BUTTON_LEFT
#define jp_ruputer_AndroAurex_JniIF_BUTTON_LEFT 16L
/*
 * Class:     jp_ruputer_AndroAurex_JniIF
 * Method:    pushButtonDown
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_jp_ruputer_AndroAurex_JniIF_pushButtonDown
  (JNIEnv *, jobject, jint);

/*
 * Class:     jp_ruputer_AndroAurex_JniIF
 * Method:    pushButtonUp
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_jp_ruputer_AndroAurex_JniIF_pushButtonUp
  (JNIEnv *, jobject, jint);

/*
 * Class:     jp_ruputer_AndroAurex_JniIF
 * Method:    aurexjnimain
 * Signature: (Landroid/graphics/Bitmap;)V
 */
JNIEXPORT void JNICALL Java_jp_ruputer_AndroAurex_JniIF_aurexjnimain
(JNIEnv *, jobject, jobject, jint);

/*
 * Class:     jp_ruputer_AndroAurex_JniIF
 * Method:    aurexjniresume
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_jp_ruputer_AndroAurex_JniIF_aurexjniresume
  (JNIEnv *, jobject);

/*
 * Class:     jp_ruputer_AndroAurex_JniIF
 * Method:    aurexjnipause
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_jp_ruputer_AndroAurex_JniIF_aurexjnipause
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif