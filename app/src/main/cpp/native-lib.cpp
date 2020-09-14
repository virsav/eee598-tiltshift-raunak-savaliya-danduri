#include <jni.h>
#include <string>
#include <cpu-features.h>
#include <cmath>

jint pixelval(jint y, jint x,jint* pixels,jint height,jint width);
jint finalPixelval(jint y, jint x,jint *pixels,jint height,jint width,jfloat sigma,jint flag);
jfloat Gk(jint k, jfloat sigma);
jfloat sigmacal(jint y,jfloat sigma_far,jfloat sigma_near,jint a0, jint a1,jint a2, jint a3);
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
    jint *outputPixels = env->GetIntArrayElements(outputPixels_, NULL);
    jint *intermediatepixels = env->GetIntArrayElements(outputPixels_, NULL);
    jint *temp1Arr=intermediatepixels,*temp2Arr=pixels;

    jfloat sigma=0;
    for(int times=0;times<2;times++){
        if(times==1){
            temp1Arr=outputPixels;temp2Arr=intermediatepixels;
        }
        for (jint y=0;y<height;y++){
            for (jint x=0;x<width;x++) {
                sigma=sigmacal(y,sigma_far,sigma_near,a0,a1,a2,a3);
                if((y>=a1&&y<=a2)||sigma<0.6)
                    temp1Arr[y*width+x]=pixels[y*width+x];
                else
                    temp1Arr[y*width+x]=finalPixelval(y,x,temp2Arr,height,width,sigma,times);
            }
        }
    }

    env->ReleaseIntArrayElements(inputPixels_, pixels, 0);
    env->ReleaseIntArrayElements(outputPixels_, outputPixels, 0);
    return 0;
}
jfloat sigmacal(jint y,jfloat sigma_far,jfloat sigma_near,jint a0, jint a1,jint a2, jint a3){
    jfloat sigma;
    if(y<a0)
        sigma=sigma_far;
    else if(y>=a0&&y<a1)
        sigma=sigma_far*((jfloat)((a1-y)/(jfloat)(a1-a0)));
    else if(y>a2&&y<=a3)
        sigma=sigma_near*((jfloat)((y-a2)/(jfloat)(a3-a2)));
    else if(y>a3)
        sigma=sigma_near;
    return sigma;
}
jint finalPixelval(jint y, jint x,jint *pixels,jint height,jint width,jfloat sigma,jint flag){
    jint r,color=0,B=0,G=0,R=0,A=0xff,val=0,B_=0,G_=0,R_=0;
    r=(jint)std::ceil((double)2*sigma);
    jfloat gk=0;
    for(jint i=-r;i<=r;i++){
        if(flag==0)
            val=pixelval(y+i,x,pixels,height,width);
        else if(flag==1)
            val=pixelval(y,x+i,pixels,height,width);
        gk=Gk(i,sigma);
        B = val & 0xff;
        G = (val>>8) & 0xff;
        R = (val>>16) & 0xff;
        B_ += B*gk;
        G_ += G*gk;
        R_ += R*gk;
    }
    color = (A & 0xff) << 24 | (R_ & 0xff) << 16 | (G_ & 0xff) << 8 | (B_ & 0xff);
    return color;
}
jfloat Gk(jint k, jfloat sigma){
    jfloat res,constant=(1/std::sqrt(2*3.14*std::pow(sigma,2)));
    res=constant*(std::exp(-1*(std::pow(k,2)/(2*std::pow(sigma,2)))));
    return  res;
}
jint pixelval(jint y, jint x,jint* pixels,jint height,jint width){
    if(y>=height||x>=width||x<0||y<0)
        return 0;
    else{
        return (pixels[y*width+x]);
    }
}

extern "C"
JNIEXPORT jint JNICALL
Java_edu_asu_ame_meteor_speedytiltshift2018_SpeedyTiltShift_tiltshiftneonnative(JNIEnv *env,
                                                                                jclass instance,
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
            int B = pixels[j*width+i]%0x100;
            int G = (pixels[j*width+i]>>8)%0x100;
            int R = (pixels[j*width+i]>>16)%0x100;
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