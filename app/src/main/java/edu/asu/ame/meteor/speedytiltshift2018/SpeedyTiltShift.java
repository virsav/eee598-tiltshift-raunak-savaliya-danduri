package edu.asu.ame.meteor.speedytiltshift2018;

import android.graphics.Bitmap;

public class SpeedyTiltShift {
    static SpeedyTiltShift Singleton = new SpeedyTiltShift();

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    public static int getFinalpixelval(int x,int y,Bitmap b1) {
        if(x<0||y<0||y>=b1.getHeight()||x>=b1.getWidth())
            return 0;
        else
            return b1.getPixel(x,y);
    }
    public static int getFinalpixelvalarr(int x, int y, int height, int width, int[] arr){
        if(x<0||y<0||y>=height||x>=width)
            return 0;
        else
            return arr[y*width+x];

    }
    public  static float Gk(int k,float sigma){
        return (float) ((Math.exp(-((k * k) / (2 * sigma * sigma)))) * (1 / Math.sqrt(2 * 3.14 * sigma * sigma)));
    }

    public static int finalpixelval(int x,int y,Bitmap b,int[] arr,float sigma,int flag){
        int R=0, G=0, B=0, R1=0, G1=0, B1=0,val=0;
        float vectorG_norm=0;
        int r = (int)Math.ceil(2*sigma);
        for (int k = -1*r; k <= r; k++) {
            if(flag==0){
                val=getFinalpixelval(x+k, y, b);
            }
            else if(flag==1){
                val=getFinalpixelvalarr(x, y+k, b.getHeight(),b.getWidth(),arr);
            }
            B = val & 0xff;
            G = (val >> 8) & 0xff;
            R = (val >> 16) & 0xff;
            vectorG_norm = Gk(k,sigma);
            B1 += vectorG_norm * B;
            G1 += vectorG_norm * G;
            R1 += vectorG_norm * R;
        }
        int A = 0xff;
        int color =((A & 0xff) << 24) | ((R1 & 0xff) << 16) | ((G1 & 0xff) << 8 )| (B1 & 0xff);
        return color;

    }
    public static float sigmacal(int y, float sigma_far, float sigma_near, int a0, int a1, int a2, int a3){
        float sigma_one=0;
        if(y<a0){
            sigma_one = sigma_far;
        }
        else if(y>=a0 && y<a1){
            sigma_one = sigma_far*((float)(a1-y)/(float)(a1-a0));
        }
        else if(y>a2 && y<=a3){
            sigma_one = sigma_near*((float)(y-a2)/(float)(a3-a2));
        }
        else if(y>a3){
            sigma_one = sigma_near;
        }
        return sigma_one;
    }
    public static Bitmap tiltshift_java(Bitmap input, float sigma_far, float sigma_near, int a0, int a1, int a2, int a3){
        Bitmap outBmp = Bitmap.createBitmap(input.getWidth(), input.getHeight(), Bitmap.Config.ARGB_8888);
        //cannot write to input Bitmap, since it may be immutable
        //if you try, you may get a java.lang.IllegalStateException
        int[] pixelsinn = new int[input.getHeight()*input.getWidth()];
        int[] pixelsOut = new int[input.getHeight()*input.getWidth()];


        float sigma_one=0;
        for(int y = 0; y < input.getHeight(); y++) {
            for (int x=0;x<input.getWidth();x++) {
                if(!(y>=a1 && y<=a2)){
                    sigma_one= sigmacal(y,sigma_far,sigma_near,a0,a1,a2,a3);
                    pixelsinn[y*input.getWidth()+x]= finalpixelval(x,y,input,pixelsinn,sigma_one,0);
                }
                else
                    pixelsinn[y*input.getWidth()+x]=input.getPixel(x,y);
            }
        }
        for(int y = 0; y < input.getHeight(); y++) {
            for (int x=0;x<input.getWidth();x++) {
                if(!(y>=a1 && y<=a2)){
                    sigma_one= sigmacal(y,sigma_far,sigma_near,a0,a1,a2,a3);
                    pixelsOut[y*input.getWidth()+x]= finalpixelval(x,y,input,pixelsinn,sigma_one,1);
                }
                else
                    pixelsOut[y*input.getWidth()+x]=input.getPixel(x,y);
            }
        }

        outBmp.setPixels(pixelsOut,0,input.getWidth(),0,0,input.getWidth(),input.getHeight());

        return outBmp;
    }

    public static Bitmap tiltshift_cpp(Bitmap input, float sigma_far, float sigma_near, int a0, int a1, int a2, int a3){
        Bitmap outBmp = Bitmap.createBitmap(input.getWidth(), input.getHeight(), Bitmap.Config.ARGB_8888);
        int[] pixels = new int[input.getHeight()*input.getWidth()];
        int[] pixelsOut = new int[input.getHeight()*input.getWidth()];
        input.getPixels(pixels,0,input.getWidth(),0,0,input.getWidth(),input.getHeight());

        tiltshiftcppnative(pixels,pixelsOut,input.getWidth(),input.getHeight(),sigma_far,sigma_near,a0,a1,a2,a3);

        outBmp.setPixels(pixelsOut,0,input.getWidth(),0,0,input.getWidth(),input.getHeight());
        return outBmp;
    }
    public static Bitmap tiltshift_neon(Bitmap input, float sigma_far, float sigma_near, int a0, int a1, int a2, int a3){
        Bitmap outBmp = Bitmap.createBitmap(input.getWidth(), input.getHeight(), Bitmap.Config.ARGB_8888);
        int[] pixels = new int[input.getHeight()*input.getWidth()];
        int[] pixelsOut = new int[input.getHeight()*input.getWidth()];
        input.getPixels(pixels,0,input.getWidth(),0,0,input.getWidth(),input.getHeight());

        tiltshiftneonnative(pixels,pixelsOut,input.getWidth(),input.getHeight(),sigma_far,sigma_near,a0,a1,a2,a3);

        outBmp.setPixels(pixelsOut,0,input.getWidth(),0,0,input.getWidth(),input.getHeight());
        return outBmp;
    }


    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public static native int tiltshiftcppnative(int[] inputPixels, int[] outputPixels, int width, int height, float sigma_far, float sigma_near, int a0, int a1, int a2, int a3);
    public static native int tiltshiftneonnative(int[] inputPixels, int[] outputPixels, int width, int height, float sigma_far, float sigma_near, int a0, int a1, int a2, int a3);

}
