#include <jni.h>
#include <string>
#include <cpu-features.h>
#include <cmath>
#include<arm_neon.h>
jint pixelval(jint y, jint x,jint* pixels,jint height,jint width);
jint finalPixelval(jint y, jint x,jint *pixels,jint height,jint width,jfloat sigma,jint flag);
jfloat Gk(jint k, jfloat sigma);
jfloat sigmacal(jint y,jfloat sigma_far,jfloat sigma_near,jint a0, jint a1,jint a2, jint a3);

uint8x16x4_t vectorLoad_neon(int y,int x,int height, int width,int *pixels);
uint8x16x4_t vectorMulGk_neon(float Gk,uint8x16x4_t pixelChannels );
float Gk_neon(int k, float sigma);
uint32_t * intermediatePixelCalc_neon(int y, int x,int height, int width, float sigma,int *pixels);
void pixelFiller_neon(int y, int x, int height, int width,int *pixels, uint32_t *colorArr);
uint32_t * finalPixelCalc_neon(int y, int x,int height, int width, float sigma,int *pixels);
void rotateImage_neon(int *orginalImage,int *roatedImage,int height, int width );

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
    uint8_t * arrayInPtr = (uint8_t *)pixels;
    uint8_t * arrayOutPtr = (uint8_t *)outputPixels;

    jint *intermediatepixels=(int *)malloc(sizeof(int)*width*height);
    int *roatedpixels=(int *)malloc(sizeof(int)*width*height);

    uint32_t *colorArr;

    for(uint32_t y=0;y<height;y++){
        for(uint32_t x=0;x<width;x=x+16){
            colorArr= intermediatePixelCalc_neon(y,x,height,width,sigma_far,pixels);
            pixelFiller_neon(y,x,height,width,intermediatepixels,colorArr);

        }
    }

    rotateImage_neon(intermediatepixels,roatedpixels,height,width );

    for(uint32_t y=0;y<width;y++){
        for(uint32_t x=0;x<height;x=x+16){
            colorArr= intermediatePixelCalc_neon(y,x,width,height,sigma_far,roatedpixels);
            pixelFiller_neon(y,x,width,height,intermediatepixels,colorArr);

        }
    }

    rotateImage_neon(intermediatepixels,(int *)outputPixels,width,height );


    env->ReleaseIntArrayElements(inputPixels_, pixels, 0);
    env->ReleaseIntArrayElements(outputPixels_, outputPixels, 0);
    return 0;
}


float Gk_neon(int k, float sigma){
    float res,constant=(1/std::sqrt(2*3.14*std::pow(sigma,2)));
    res=constant*(std::exp(-1*(std::pow(k,2)/(2*std::pow(sigma,2)))));
    return  res;
}


uint8x16x4_t vectorMulGk_neon(float Gk,uint8x16x4_t pixelChannels ){

    uint8x16x4_t res;

    uint8x16_t Avector = pixelChannels.val[3];
    uint8x16_t Rvector = pixelChannels.val[2];
    uint8x16_t Gvector = pixelChannels.val[1];
    uint8x16_t Bvector = pixelChannels.val[0];

    uint8x8_t Alow = vget_low_u8(Avector);
    uint8x8_t AHigh = vget_high_u8(Avector);
    uint8x8_t Rlow = vget_low_u8(Rvector);
    uint8x8_t RHigh = vget_high_u8(Rvector);
    uint8x8_t Glow = vget_low_u8(Gvector);
    uint8x8_t GHigh = vget_high_u8(Gvector);
    uint8x8_t Blow = vget_low_u8(Bvector);
    uint8x8_t BHigh = vget_high_u8(Bvector);

    uint16x8_t Alow16 = vmovl_u8(Alow);
    uint16x8_t AHigh16 = vmovl_u8(AHigh);
    uint16x8_t Rlow16 = vmovl_u8(Rlow);
    uint16x8_t RHigh16 = vmovl_u8(RHigh);
    uint16x8_t Glow16 = vmovl_u8(Glow);
    uint16x8_t GHigh16 = vmovl_u8(GHigh);
    uint16x8_t Blow16 = vmovl_u8(Blow);
    uint16x8_t BHigh16 = vmovl_u8(BHigh);

    Alow16 = vmulq_n_u16(Alow16,(uint16_t)(Gk*64));
    AHigh16 = vmulq_n_u16(AHigh16,(uint16_t)(Gk*64));
    Rlow16 = vmulq_n_u16(Rlow16,(uint16_t)(Gk*64));
    RHigh16 = vmulq_n_u16(RHigh16,(uint16_t)(Gk*64));
    Glow16 = vmulq_n_u16(Glow16,(uint16_t)(Gk*64));
    GHigh16 = vmulq_n_u16(GHigh16,(uint16_t)(Gk*64));
    Blow16 = vmulq_n_u16(Blow16,(uint16_t)(Gk*64));
    BHigh16 = vmulq_n_u16(BHigh16,(uint16_t)(Gk*64));

    Alow16 = vshrq_n_u16(Alow16,6);
    AHigh16 = vshrq_n_u16(AHigh16,6);
    Rlow16 = vshrq_n_u16(Rlow16,6);
    RHigh16 = vshrq_n_u16(RHigh16,6);
    Glow16 = vshrq_n_u16(Glow16,6);
    GHigh16 = vshrq_n_u16(GHigh16,6);
    Blow16 = vshrq_n_u16(Blow16,6);
    BHigh16 = vshrq_n_u16(BHigh16,6);

    Alow = vqmovn_u16(Alow16);
    AHigh = vqmovn_u16(AHigh16);
    Rlow = vqmovn_u16(Rlow16);
    RHigh = vqmovn_u16(RHigh16);
    Glow = vqmovn_u16(Glow16);
    GHigh = vqmovn_u16(GHigh16);
    Blow = vqmovn_u16(Blow16);
    BHigh = vqmovn_u16(BHigh16);

    uint8x16_t A = vcombine_u8(Alow,AHigh);
    uint8x16_t R = vcombine_u8(Rlow,RHigh);
    uint8x16_t G = vcombine_u8(Glow,GHigh);
    uint8x16_t B = vcombine_u8(Blow,BHigh);

    res.val[3] = A;
    res.val[2] = R;
    res.val[1] = G;
    res.val[0] = B;

    return res;
}

uint32_t * finalPixelCalc_neon(int y, int x,int height, int width, float sigma,int *pixels){

}

uint32_t * intermediatePixelCalc_neon(int y, int x,int height, int width, float sigma,int *pixels){
    int r=(jint)std::ceil((double)2*sigma);
    uint8x16x4_t sum;
    memset((uint8_t *)&sum,0,64);
    uint32_t *colorArr = (uint32_t *)malloc(64);

    for(int k=(-1*r);k<=r;k++){
        uint8x16x4_t temp= vectorMulGk_neon(Gk_neon(k,sigma),vectorLoad_neon(y+k,x, height,width,pixels));
        for(int i=3;i>=0;i--){
            sum.val[i] = vaddq_u8(temp.val[i],sum.val[i]);
        }
    }

    vst4q_u8((uint8_t *)colorArr,sum);
    return colorArr;
}


uint8x16x4_t vectorLoad_neon(int y,int x,int height, int width,int *pixels){

    uint8x16x4_t pixelChannels;
    uint8_t *emptyptr=(uint8_t *)malloc(64);
    memset(emptyptr,0,64);

    if(y<0||x<0||y>height||x>width){
        pixelChannels=vld4q_u8(emptyptr);
    }
    else if((width-x)<16){
        uint32_t *ptr=(uint32_t *)emptyptr;
        uint8_t len=width-x;
        for(int i=0;i<len;i++)
            ptr[i]=pixels[y*width+x];
        pixelChannels=vld4q_u8(emptyptr);
    }
    else{
        free(emptyptr);
        pixelChannels=vld4q_u8((uint8_t *)&pixels[y*width+x]);
    }
    return pixelChannels;
}

void pixelFiller_neon(int y, int x, int height, int width,int *tempPixels,  uint32_t *colorArr){

    if((width-x)<16){
        uint8_t len=width-x;
        for(int i=0;i<len;i++){
            tempPixels[y*width+x] = colorArr[i];
            x++;
        }
    }
    else if( x <= (width-16)){
        for(int i=0;i<16;i++){
            tempPixels[y*width+x] = colorArr[i];
            x++;
        }
    }
    free(colorArr);
}

void rotateImage_neon(int *orginalImage,int *roatedImage,int height, int width ){
    for(uint32_t y=0;y<height;y++){
        for(uint32_t x=0;x<width;x++){
            roatedImage[x*height+y] =orginalImage[y*width+x];
        }
    }
}