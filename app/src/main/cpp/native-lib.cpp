#include <jni.h>
#include <string>
#include <cpu-features.h>

extern "C"
JNIEXPORT jint JNICALL

Java_edu_asu_ame_meteor_speedytiltshift2018_SpeedyTiltShift_tiltshiftcppnative(JNIEnv *env,
                                                                               jobject instance,
                                                                               jintArray inputPixels_,
                                                                               jintArray outputPixels_,
                                                                               jint width,
                                                                               jint height,
                                                                               jfloat sigma_far,
                                                                               jfloat sigma_near,
                                                                               jint a0, jint a1,
                                                                               jint a2, jint a3) {
    jint *pixels = env->GetIntArrayElements(inputPixels_, NULL);
    jint *pixelsi = env->GetIntArrayElements(outputPixels_, NULL);
    jint *outputPixels = env->GetIntArrayElements(outputPixels_, NULL);
    int r_near = (int)ceil(2*sigma_near);
    int r_far = (int)ceil(2*sigma_far);

   int D=0;
   int r_far1;
   int r_near1;
   double summationD=0;
   double normalD = 0;
   int E=0;
   int F=0;
   int B=0;
   int G=0;
   int R=0;
   double summation=0;
   float sigma_one=0;
   jfloat factor=0;
    jfloat fact = 0;
    jfloat a_0 = 0;
    jfloat a_1 = 0;
    jfloat a_2 = 0;
    jfloat a_3 = 0;
    for (int j=0;j<height;j++){
        for (int i=0;i<width;i++) {
            if(j>r_far-1 /*&& j<height-3*/ && i>r_far-1 && i<width-r_far && sigma_far>0.6 && j<a0){

                sigma_one = sigma_far;
                for(int k = -r_far; k < r_far+1; k++){
                    summation += ((exp(-((k*k)/(2*sigma_one*sigma_one))))*(1/sqrt(2*3.14*sigma_one*sigma_one)));
                }

                for(int k = -r_far; k < r_far+1; k++){
                     B = pixels[j*width+(i+k)]&0xff;
                     G = (pixels[j*width+(i+k)]>>8)&0xff;
                     R = (pixels[j*width+(i+k)]>>16)&0xff;
                    summationD = ((exp(-((k*k)/(2*sigma_one*sigma_one))))*(1/sqrt(2*3.14*sigma_one*sigma_one)));
                    normalD = summationD/summation;
                    D += normalD*B;
                    E += normalD*G;
                    F += normalD*R;
                }

            }
            else if(/*j>2 && j<height-3*/ i>r_far-1 && i<width-r_far && sigma_far>0.6 && j>=a0 && j<a1){

                 fact = (jfloat)j;
                 a_0 = (jfloat)a0;
                 a_1 = (jfloat)a1;

                factor = (a_1-fact)*(1/(a_1-a_0));

                sigma_one = sigma_far*factor;
                r_far1 = (int)ceil(2*sigma_one);
                    for (int k = -r_far1; k < r_far1 + 1; k++) {
                        summation += ((exp(-((k * k) / (2 * sigma_one * sigma_one)))) *
                                      (1 / sqrt(2 * 3.14 * sigma_one * sigma_one)));
                    }

                    for (int k = -r_far1; k < r_far1 + 1; k++) {
                        B = pixels[j * width + (i + k)] & 0xff;
                        G = (pixels[j * width + (i + k)] >> 8) & 0xff;
                        R = (pixels[j * width + (i + k)] >> 16) & 0xff;
                        summationD = ((exp(-((k * k) / (2 * sigma_one * sigma_one)))) *
                                      (1 / sqrt(2 * 3.14 * sigma_one * sigma_one)));
                        normalD = summationD / summation;
                        D += normalD * B;
                        E += normalD * G;
                        F += normalD * R;
                    }


            }
            else if(/*j>2 && j<height-3 &&*/ i>r_near-1 && i<width-r_near && sigma_near>0.6 && j>a2 && j<=a3){

                fact = (jfloat)j;
                a_2 = (jfloat)a2;
                a_3 = (jfloat)a3;
                factor = (fact-a_2)*(1/(a_3-a_2));
                sigma_one = sigma_near*factor;
                r_near1 = (int)ceil(2*sigma_one);

                    for (int k = -r_near1; k < r_near1 + 1; k++) {
                        summation += ((exp(-((k * k) / (2 * sigma_one * sigma_one)))) *
                                      (1 / sqrt(2 * 3.14 * sigma_one * sigma_one)));
                    }

                    for (int k = -r_near1; k < r_near1 + 1; k++) {
                        B = pixels[j * width + (i + k)] & 0xff;
                        G = (pixels[j * width + (i + k)] >> 8) & 0xff;
                        R = (pixels[j * width + (i + k)] >> 16) & 0xff;
                        summationD = ((exp(-((k * k) / (2 * sigma_one * sigma_one)))) *
                                      (1 / sqrt(2 * 3.14 * sigma_one * sigma_one)));
                        normalD = summationD / summation;
                        D += normalD * B;
                        E += normalD * G;
                        F += normalD * R;
                    }



            }
            else if(/*j>2 &&*/ j<height-r_near && i>r_near-1 && i<width-r_near && sigma_near>0.6 && j>a3  ){

                sigma_one = sigma_near;
                for(int k = -r_near; k < r_near+1; k++){
                    summation += ((exp(-((k*k)/(2*sigma_one*sigma_one))))*(1/sqrt(2*3.14*sigma_one*sigma_one)));
                }

                for(int k = -r_near; k < r_near+1; k++){
                     B = pixels[j*width+(i+k)]&0xff;
                     G = (pixels[j*width+(i+k)]>>8)&0xff;
                     R = (pixels[j*width+(i+k)]>>16)&0xff;
                    summationD = ((exp(-((k*k)/(2*sigma_one*sigma_one))))*(1/sqrt(2*3.14*sigma_one*sigma_one)));
                    normalD = summationD/summation;
                    D += normalD*B;
                    E += normalD*G;
                    F += normalD*R;
                }

            }

            else{
                D = pixels[j*width+i]&0xff;;
                E = (pixels[j*width+i]>>8)&0xff;
                F = (pixels[j*width+i]>>16)&0xff;
            }

            int A = 0xff;
            int color = (A & 0xff) << 24 | (F & 0xff) << 16 | (E & 0xff) << 8 | (D & 0xff);

            pixelsi[j*width+i]=color;
            summation=0;
            D=0;
            E=0;
            F=0;
        }
    }

    for (int j=0;j<height;j++){
        for (int i=0;i<width;i++) {
            if(j>r_far-1/* && j<height-3 */&& i>r_far-1 && i<width-r_far && sigma_far>0.6 && j<a0){
                //int r = 1 + (int)ceil(2*sigma_far);
                sigma_one = sigma_far;
                for(int k = -r_far; k < r_far+1; k++){
                    summation += ((exp(-((k*k)/(2*sigma_one*sigma_one))))*(1/sqrt(2*3.14*sigma_one*sigma_one)));
                }

                for(int k = -r_far; k < r_far+1; k++){
                     B = pixelsi[(j+k)*width+i]&0xff;
                     G = (pixelsi[(j+k)*width+i]>>8)&0xff;
                     R = (pixelsi[(j+k)*width+i]>>16)&0xff;
                    summationD = ((exp(-((k*k)/(2*sigma_one*sigma_one))))*(1/sqrt(2*3.14*sigma_one*sigma_one)));
                    normalD = summationD/summation;
                    D += normalD*B;
                    E += normalD*G;
                    F += normalD*R;
                }

            }
            else if(/*j>2 && j<height-3 &&*/ i>r_far-1 && i<width-r_far && sigma_far>0.6 && j>=a0 && j<a1){

                fact = (jfloat)j;
                a_0 = (jfloat)a0;
                a_1 = (jfloat)a1;
                factor = (a_1-fact)*(1/(a_1-a_0));
                sigma_one = sigma_far*factor/*((a1-j)/(a1-a0))*/;
                r_far1 = (int)ceil(2*sigma_one);

                    for (int k = -r_far1; k < r_far1 + 1; k++) {
                        summation += ((exp(-((k * k) / (2 * sigma_one * sigma_one)))) *
                                      (1 / sqrt(2 * 3.14 * sigma_one * sigma_one)));
                    }

                    for (int k = -r_far1; k < r_far1 + 1; k++) {
                        B = pixelsi[(j + k) * width + i] & 0xff;
                        G = (pixelsi[(j + k) * width + i] >> 8) & 0xff;
                        R = (pixelsi[(j + k) * width + i] >> 16) & 0xff;
                        summationD = ((exp(-((k * k) / (2 * sigma_one * sigma_one)))) *
                                      (1 / sqrt(2 * 3.14 * sigma_one * sigma_one)));
                        normalD = summationD / summation;
                        D += normalD * B;
                        E += normalD * G;
                        F += normalD * R;
                    }

            }
            else if(/*j>2 && j<height-3 &&*/ i>r_near-1 && i<width-r_near && sigma_near>0.6 && j>a2 && j<=a3){

                fact = (jfloat)j;
                a_2 = (jfloat)a2;
                a_3 = (jfloat)a3;
                factor = (fact-a_2)*(1/(a_3-a_2));
                sigma_one = sigma_near*factor/*((j-a2)/(a3-a2))*/;
                r_near1 = (int)ceil(2*sigma_one);

                    for (int k = -r_near1; k < r_near1 + 1; k++) {
                        summation += ((exp(-((k * k) / (2 * sigma_one * sigma_one)))) *
                                      (1 / sqrt(2 * 3.14 * sigma_one * sigma_one)));
                    }

                    for (int k = -r_near1; k < r_near1 + 1; k++) {
                        B = pixelsi[(j + k) * width + i] & 0xff;
                        G = (pixelsi[(j + k) * width + i] >> 8) & 0xff;
                        R = (pixelsi[(j + k) * width + i] >> 16) & 0xff;
                        summationD = ((exp(-((k * k) / (2 * sigma_one * sigma_one)))) *
                                      (1 / sqrt(2 * 3.14 * sigma_one * sigma_one)));
                        normalD = summationD / summation;
                        D += normalD * B;
                        E += normalD * G;
                        F += normalD * R;
                    }

            }
            else if(/*j>2 && */j<height-r_near && i>r_near-1 && i<width-r_near && sigma_near>0.6 && j>a3){


                sigma_one = sigma_near;
                for(int k = -r_near; k < r_near+1; k++){
                    summation += ((exp(-((k*k)/(2*sigma_one*sigma_one))))*(1/sqrt(2*3.14*sigma_one*sigma_one)));
                }

                for(int k = -r_near; k < r_near+1; k++){
                     B = pixelsi[(j+k)*width+i]&0xff;
                     G = (pixelsi[(j+k)*width+i]>>8)&0xff;
                     R = (pixelsi[(j+k)*width+i]>>16)&0xff;
                    summationD = ((exp(-((k*k)/(2*sigma_one*sigma_one))))*(1/sqrt(2*3.14*sigma_one*sigma_one)));
                    normalD = summationD/summation;
                    D += normalD*B;
                    E += normalD*G;
                    F += normalD*R;
                }


            }
            else{
                D = pixels[j*width+i]&0xff;;
                E = (pixels[j*width+i]>>8)&0xff;
                F = (pixels[j*width+i]>>16)&0xff;
            }
            //int G = (pixels[j*width+i]>>8)%0xff;
            //int R = (pixels[j*width+i]>>16)%0xff;
            int A = 0xff;
            //int D=0;
            int color = (A & 0xff) << 24 | (F & 0xff) << 16 | (E & 0xff) << 8 | (D & 0xff);

            outputPixels[j*width+i]=color;
            summation=0;
            D=0;
            E=0;
            F=0;
        }
    }


    env->ReleaseIntArrayElements(inputPixels_, pixels, 0);
    env->ReleaseIntArrayElements(outputPixels_, pixelsi, 0);
    env->ReleaseIntArrayElements(outputPixels_, outputPixels, 0);
     return 0;
}

extern "C"
JNIEXPORT jint JNICALL
Java_edu_asu_ame_meteor_speedytiltshift2018_SpeedyTiltShift_tiltshiftneonnative(JNIEnv *env,
                                                                               jobject instance,
                                                                               jintArray inputPixels_,
                                                                               jintArray outputPixels_,
                                                                               jint width,
                                                                               jint height,
                                                                               jfloat sigma_far,
                                                                               jfloat sigma_near,
                                                                               jint a0, jint a1,
                                                                               jint a2, jint a3) {
    jint *pixels = env->GetIntArrayElements(inputPixels_, NULL);
    jint *outputPixels = env->GetIntArrayElements(outputPixels_, NULL);

    for (int j=0;j<height;j++){
        for (int i=0;i<width;i++) {
            int B = pixels[j*width+i]%0xff;
            int G = (pixels[j*width+i]>>8)%0xff;
            int R = (pixels[j*width+i]>>16)%0xff;
            int A = 0xff;
            R=0;
            int color = (A & 0xff) << 24 | (R & 0xff) << 16 | (G & 0xff) << 8 | (B & 0xff);

            outputPixels[j*width+i]=color;
        }
    }

    env->ReleaseIntArrayElements(inputPixels_, pixels, 0);
    env->ReleaseIntArrayElements(outputPixels_, outputPixels, 0);
    return 0;
}