#include <jni.h>
#include <stdio.h>
#include "Bla.h"
 
JNIEXPORT void JNICALL Java_Bla_bla(JNIEnv *env, jobject thisObj) {
   printf("blaaaah\n");
   return;
}
