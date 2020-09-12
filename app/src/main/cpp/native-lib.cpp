#include <jni.h>
#include <string>
#include <cpu-features.h>
#include <cmath>

jint pixelval(jint x, jint y,jint * pixels,jint height,jint width);
jint finalPixelval(jint x, jint y,jint *pixels,jint height,jint width,jfloat sigma,jint flag);
jfloat Gk(jint k, jfloat sigma);
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
    jfloat sigma=0;
    jint noblur=0;
    for (jint j=0;j<height;j++){
        for (jint i=0;i<width;i++) {
            if(j<a0)
                sigma=sigma_far;
            else if(j>=a0&&j<a1)
                sigma=sigma_far*((jfloat)((a1-j)/(jfloat)(a1-a0)));
            else if(j>=a1&&j<=a2)
                noblur=1;
            else if(j>a2&&j<=a3)
                sigma=sigma_near*((jfloat)((j-a2)/(jfloat)(a3-a2)));
            else if(j>a3)
                sigma=sigma_near;
            if((noblur==1)||sigma<0.6)
                intermediatepixels[j*width+i]=pixels[j*width+i];
            else
                intermediatepixels[j*width+i]=finalPixelval(j,i,pixels,height,width,sigma,0);
            noblur=0;
            sigma=0;
        }
    }
    for (jint j=0;j<height;j++){
        for (jint i=0;i<width;i++) {
            if(j<=a0)
                sigma=sigma_far;
            else if(j>a0&&j<=a1)
                sigma=sigma_far*((jfloat)((a1-j)/(jfloat)(a1-a0)));
            else if(j>a1&&j<=a2)
                noblur=1;
            else if(j>a2&&j<=a3)
                sigma=sigma_near*((jfloat)((j-a2)/(jfloat)(a3-a2)));
            else if(j>a3)
                sigma=sigma_near;
            if((noblur==1)||sigma<0.6)
                outputPixels[j*width+i]=pixels[j*width+i];
            else
                outputPixels[j*width+i]=finalPixelval(j,i,intermediatepixels,height,width,sigma,1);
            noblur=0;
            sigma=0;
        }
    }

    env->ReleaseIntArrayElements(inputPixels_, pixels, 0);
    env->ReleaseIntArrayElements(outputPixels_, outputPixels, 0);
    return 0;
}
jint finalPixelval(jint x, jint y,jint *pixels,jint height,jint width,jfloat sigma,jint flag){
    jint r,color=0,B=0,G=0,R=0,A=0xff,val=0,B_=0,G_=0,R_=0;
    r=(jint)std::ceil((double)2*sigma);
    jfloat gk=0;
    if(flag==0){
        for(jint i=-r;i<=r;i++){
            val=pixelval(x+i,y,pixels,height,width);
            gk=Gk(i,sigma);
            B = val & 0xff;
            G = (val>>8) & 0xff;
            R = (val>>16) & 0xff;
            B_ += B*gk;
            G_ += G*gk;
            R_ += R*gk;
        }
    }
    else if(flag==1){
        for(jint i=-r;i<=r;i++){
            val=pixelval(x,y+i,pixels,height,width);
            gk=Gk(i,sigma);
            B = val & 0xff;
            G = (val>>8) & 0xff;
            R = (val>>16) & 0xff;
            B_ += B*gk;
            G_ += G*gk;
            R_ += R*gk;
        }
    }
    color = (A & 0xff) << 24 | (R_ & 0xff) << 16 | (G_ & 0xff) << 8 | (B_ & 0xff);
    return color;
}
jfloat Gk(jint k, jfloat sigma){
    jfloat res,constant=(1/std::sqrt(2*3.14*std::pow(sigma,2)));
    res=constant*(std::exp(-1*(std::pow(k,2)/(2*std::pow(sigma,2)))));
    return  res;
}

jint pixelval(jint x, jint y,jint* pixels,jint height,jint width){
    if(x>=height||y>=width||x<0||y<0)
        return 0;
    else{
        return (pixels[x*width+y]);
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