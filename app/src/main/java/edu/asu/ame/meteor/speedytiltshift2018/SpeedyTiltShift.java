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

    public static Bitmap tiltshift_java(Bitmap input, float sigma_far, float sigma_near, int a0, int a1, int a2, int a3){
        Bitmap outBmp = Bitmap.createBitmap(input.getWidth(), input.getHeight(), Bitmap.Config.ARGB_8888);
        //cannot write to input Bitmap, since it may be immutable
        //if you try, you may get a java.lang.IllegalStateException

        int[] pixels = new int[input.getHeight()*input.getWidth()];
        int[] pixelsinn = new int[input.getHeight()*input.getWidth()];
        int[] pixelsOut = new int[input.getHeight()*input.getWidth()];
        input.getPixels(pixels,0,input.getWidth(),0,0,input.getWidth(),input.getHeight());

        int R=0, G=0, B=0, R1=0, G1=0, B1=0;

        int r_far = (int)Math.ceil(2*sigma_far);
        int r_near = (int)Math.ceil(2*sigma_near);

        float sigma_one=0;

        double vectorG_norm=0;

        for(int y = 0; y < input.getHeight(); y++) {
            for (int x=0;x<input.getWidth();x++) {

                if(y<a0 && sigma_far>0.6){
                    sigma_one = sigma_far;
                    for (int k = -1*r_far; k <= r_far; k++) {
                        int val=getFinalpixelval(x+k, y, input);
                        B = val & 0xff;
                        G = (val >> 8) & 0xff;
                        R = (val >> 16) & 0xff;
                        vectorG_norm =  ((Math.exp(-((k * k) / (2 * sigma_one * sigma_one)))) * (1 / Math.sqrt(2 * 3.14 * sigma_one * sigma_one)));
                        B1 += vectorG_norm * B;
                        G1 += vectorG_norm * G;
                        R1 += vectorG_norm * R;
                    }
                }
                else if(y>=a0 && y<a1 && sigma_far>0.6){
                    sigma_one = sigma_far*((float)(a1-y)/(float)(a1-a0));
                    for (int k = -1*r_far; k <= r_far; k++) {
                        int val=getFinalpixelval(x+k, y, input);
                        B = val & 0xff;
                        G = (val >> 8) & 0xff;
                        R = (val >> 16) & 0xff;
                        vectorG_norm =  ((Math.exp(-((k * k) / (2 * sigma_one * sigma_one)))) * (1 / Math.sqrt(2 * 3.14 * sigma_one * sigma_one)));
                        B1 += vectorG_norm * B;
                        G1 += vectorG_norm * G;
                        R1 += vectorG_norm * R;
                    }
                }
                else if(y>a2 && y<=a3 && sigma_near>0.6){
                    sigma_one = sigma_near*((float)(y-a2)/(float)(a3-a2));
                    for (int k = -1*r_near; k <= r_near; k++) {
                        int val=getFinalpixelval(x+k, y, input);
                        B = val & 0xff;
                        G = (val >> 8) & 0xff;
                        R = (val >> 16) & 0xff;
                        vectorG_norm =  ((Math.exp(-((k * k) / (2 * sigma_one * sigma_one)))) * (1 / Math.sqrt(2 * 3.14 * sigma_one * sigma_one)));
                        B1 += vectorG_norm * B;
                        G1 += vectorG_norm * G;
                        R1 += vectorG_norm * R;
                    }
                }
                else if(y>a3 && sigma_near>0.6){
                    sigma_one = sigma_near;
                    for (int k = -1*r_near; k <= r_near; k++) {
                        int val=getFinalpixelval(x+k, y, input);
                        B = val & 0xff;
                        G = (val >> 8) & 0xff;
                        R = (val >> 16) & 0xff;
                        vectorG_norm =  ((Math.exp(-((k * k) / (2 * sigma_one * sigma_one)))) * (1 / Math.sqrt(2 * 3.14 * sigma_one * sigma_one)));
                        B1 += vectorG_norm * B;
                        G1 += vectorG_norm * G;
                        R1 += vectorG_norm * R;
                    }
                }
                else{
                    int val=getFinalpixelval(x, y, input);
                    B1 = val & 0xff;
                    G1 = (val >> 8) & 0xff;
                    R1 = (val >> 16) & 0xff;
                }

                int A = 0xff;
                int color =((A & 0xff) << 24) | ((R1 & 0xff) << 16) | ((G1 & 0xff) << 8 )| (B1 & 0xff);
                B1=0;G1=0;R1=0;
                pixelsinn[y*input.getWidth()+x]=color;
            }
        }

        B1=0;G1=0;R1=0;
        for(int y = 0; y < input.getHeight(); y++) {
            for (int x=0;x<input.getWidth();x++) {
                if(y<a0 && sigma_far>0.6){
                    sigma_one = sigma_far;
                    for (int k = -1*r_far; k <= r_far; k++) {
                        int val= getFinalpixelvalarr(x,y+k,input.getHeight(),input.getWidth(),pixelsinn);
                        B =  val & 0xff;
                        G = (val >> 8) & 0xff;
                        R = (val >> 16) & 0xff;
                        vectorG_norm =  ((Math.exp(-((k * k) / (2 * sigma_one * sigma_one)))) * (1 / Math.sqrt(2 * 3.14 * sigma_one * sigma_one)));
                        B1 += vectorG_norm * B;
                        G1 += vectorG_norm * G;
                        R1 += vectorG_norm * R;
                    }
                }
                else if(y>=a0 && y<a1 && sigma_far>0.6){
                    sigma_one = sigma_far*((float)(a1-y)/(float)(a1-a0));
                    for (int k = -1*r_far; k <= r_far; k++) {
                        int val= getFinalpixelvalarr(x,y+k,input.getHeight(),input.getWidth(),pixelsinn);
                        B =  val & 0xff;
                        G = (val >> 8) & 0xff;
                        R = (val >> 16) & 0xff;
                        vectorG_norm =  ((Math.exp(-((k * k) / (2 * sigma_one * sigma_one)))) * (1 / Math.sqrt(2 * 3.14 * sigma_one * sigma_one)));
                        B1 += vectorG_norm * B;
                        G1 += vectorG_norm * G;
                        R1 += vectorG_norm * R;
                    }
                }
                else if(y>a2 && y<=a3 && sigma_near>0.6){
                    sigma_one = sigma_near*((float)(y-a2)/(float)(a3-a2));
                    for (int k = -1*r_near; k <= r_near; k++) {
                        int val= getFinalpixelvalarr(x,y+k,input.getHeight(),input.getWidth(),pixelsinn);
                        B =  val & 0xff;
                        G = (val >> 8) & 0xff;
                        R = (val >> 16) & 0xff;
                        vectorG_norm =  ((Math.exp(-((k * k) / (2 * sigma_one * sigma_one)))) * (1 / Math.sqrt(2 * 3.14 * sigma_one * sigma_one)));
                        B1 += vectorG_norm * B;
                        G1 += vectorG_norm * G;
                        R1 += vectorG_norm * R;
                    }
                }
                else if(y>a3 && sigma_near>0.6){
                    sigma_one = sigma_near;
                    for (int k = -1*r_near; k <= r_near; k++) {
                        int val= getFinalpixelvalarr(x,y+k,input.getHeight(),input.getWidth(),pixelsinn);
                        B =  val & 0xff;
                        G = (val >> 8) & 0xff;
                        R = (val >> 16) & 0xff;
                        vectorG_norm =  ((Math.exp(-((k * k) / (2 * sigma_one * sigma_one)))) * (1 / Math.sqrt(2 * 3.14 * sigma_one * sigma_one)));
                        B1 += vectorG_norm * B;
                        G1 += vectorG_norm * G;
                        R1 += vectorG_norm * R;
                    }
                }
                else{
                    int val= getFinalpixelvalarr(x,y,input.getHeight(),input.getWidth(),pixelsinn);
                    B1 =  val & 0xff;
                    G1 = (val >> 8) & 0xff;
                    R1 = (val >> 16) & 0xff;
                }

                int A = 0xff;
                int color =((A & 0xff) << 24) | ((R1 & 0xff) << 16) | ((G1 & 0xff) << 8 )| (B1 & 0xff);
                B1=0;G1=0;R1=0;
                pixelsOut[y*input.getWidth()+x]=color;
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
