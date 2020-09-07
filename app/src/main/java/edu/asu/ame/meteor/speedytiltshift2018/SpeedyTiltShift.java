package edu.asu.ame.meteor.speedytiltshift2018;

/* Code is not the final version and needs bugs to be fixed - Viraj Savaliya*/

import android.graphics.Bitmap;

import java.util.Arrays;

public class SpeedyTiltShift {
    static SpeedyTiltShift Singleton = new SpeedyTiltShift();

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }


    public static Bitmap tiltshift_java(Bitmap input, float sigma_far, float sigma_near, int a0, int a1, int a2, int a3){
        Bitmap outBmp = Bitmap.createBitmap(input.getWidth(), input.getHeight(), Bitmap.Config.ARGB_8888);
        //cannot write to input Bitmap, since it may be immutable
        //if you try, you may get a java.lang.IllegalStateException

        int height_mat = input.getHeight();
        int width_mat = input.getWidth();

        int[] pixels = new int[height_mat*width_mat];
        int[] pixelsinn = new int[height_mat*width_mat];
        int[] pixelsOut = new int[height_mat*width_mat];

//        Arrays.setAll(pixels, null);
//        Arrays.setAll(pixelsinn, null);
//        Arrays.setAll(pixelsOut, null);

        input.getPixels(pixels,0,width_mat,0,0,width_mat,height_mat);
        /*
        for (int i=0;i<input.getWidth()*input.getHeight();i++){
            int B = pixels[i]&0xff;
            int G = (pixels[i]>>8)&0xff;
            int R = (pixels[i]>>16)&0xff;
            int A = 0xff;
            //G=0;
            int color = (A & 0xff) << 24 | (R & 0xff) << 16 | (G & 0xff) << 8 | (B & 0xff);
            pixelsOut[i]=color;
        }
        */


        int r_near = (int)Math.ceil(2*sigma_near);
        int r_far = (int)Math.ceil(2*sigma_far);

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
        double sigma_one=0;
        double factor=0;
        double fact = 0;
        double a_0 = 0;
        double a_1 = 0;
        double a_2 = 0;
        double a_3 = 0;

        for (int j=0;j<height_mat*width_mat;j++){

//            for (int i=0;i<width_mat;i++){

                if(j>(r_far-1)*width_mat && j>(r_far-1)*height_mat && j<(width_mat-r_far)*height_mat && sigma_far>0.6 && j<a0*width_mat){

                    sigma_one = sigma_far;
                    for(int k = -r_far; k < r_far+1; k++){
                        summation += ((Math.exp(-((k*k)/(2*sigma_one*sigma_one))))*(1/Math.sqrt(2*3.14*sigma_one*sigma_one)));
                    }

                    for(int k = -r_far; k < r_far+1; k++){
                        B = pixels[j+k]&0xff;
                        G = (pixels[j+k]>>8)&0xff;
                        R = (pixels[j+k]>>16)&0xff;
                        summationD = ((Math.exp(-((k*k)/(2*sigma_one*sigma_one))))*(1/Math.sqrt(2*3.14*sigma_one*sigma_one)));
                        normalD = summationD/summation;
                        D += normalD*B;
                        E += normalD*G;
                        F += normalD*R;
                    }

                }
                else if(j>((r_far-1)*height_mat) && j<(width_mat-r_far)*height_mat && sigma_far>0.6 && j>=a0*width_mat && j<a1*width_mat){

                    fact = (double)j;
                    a_0 = (double)a0;
                    a_1 = (double)a1;

                    factor = (a_1-fact)*(1/(a_1-a_0));

                    sigma_one = sigma_far*factor;
                    r_far1 = (int)Math.ceil(2*sigma_one);
                    for (int k = -r_far1; k < r_far1 + 1; k++) {
                        summation += ((Math.exp(-((k * k) / (2 * sigma_one * sigma_one)))) *
                                (1 / Math.sqrt(2 * 3.14 * sigma_one * sigma_one)));
                    }

                    for (int k = -r_far1; k < r_far1 + 1; k++) {
                        B = pixels[j + k] & 0xff;
                        G = (pixels[j + k] >> 8) & 0xff;
                        R = (pixels[j + k] >> 16) & 0xff;
                        summationD = ((Math.exp(-((k * k) / (2 * sigma_one * sigma_one)))) *
                                (1 / Math.sqrt(2 * 3.14 * sigma_one * sigma_one)));
                        normalD = summationD / summation;
                        D += normalD * B;
                        E += normalD * G;
                        F += normalD * R;
                    }


                }
                else if(j>(r_near-1)*height_mat && j<(width_mat-r_near)*height_mat && sigma_near>0.6 && j>a2*width_mat && j<=a3*width_mat){

                    fact = (double)j;
                    a_2 = (double)a2;
                    a_3 = (double)a3;
                    factor = (fact-a_2)*(1/(a_3-a_2));
                    sigma_one = sigma_near*factor;
                    r_near1 = (int)Math.ceil(2*sigma_one);

                    for (int k = -r_near1; k < r_near1 + 1; k++) {
                        summation += ((Math.exp(-((k * k) / (2 * sigma_one * sigma_one)))) *
                                (1 / Math.sqrt(2 * 3.14 * sigma_one * sigma_one)));
                    }

                    for (int k = -r_near1; k < r_near1 + 1; k++) {
                        B = pixels[j + k] & 0xff;
                        G = (pixels[j + k] >> 8) & 0xff;
                        R = (pixels[j + k] >> 16) & 0xff;
                        summationD = ((Math.exp(-((k * k) / (2 * sigma_one * sigma_one)))) *
                                (1 / Math.sqrt(2 * 3.14 * sigma_one * sigma_one)));
                        normalD = summationD / summation;
                        D += normalD * B;
                        E += normalD * G;
                        F += normalD * R;
                    }



                }
                else if(j<(height_mat-r_near)*width_mat && j>(r_near-1)*height_mat && j<(width_mat-r_near)*height_mat && sigma_near>0.6 && j>a3*width_mat  ){

                    sigma_one = sigma_near;
                    for(int k = -r_near; k < r_near+1; k++){
                        summation += ((Math.exp(-((k*k)/(2*sigma_one*sigma_one))))*(1/Math.sqrt(2*3.14*sigma_one*sigma_one)));
                    }

                    for(int k = -r_near; k < r_near+1; k++){
                        B = pixels[j+k]&0xff;
                        G = (pixels[j+k]>>8)&0xff;
                        R = (pixels[j+k]>>16)&0xff;
                        summationD = ((Math.exp(-((k*k)/(2*sigma_one*sigma_one))))*(1/Math.sqrt(2*3.14*sigma_one*sigma_one)));
                        normalD = summationD/summation;
                        D += normalD*B;
                        E += normalD*G;
                        F += normalD*R;
                    }

                }

                else{
                    D = pixels[j]&0xff;;
                    E = (pixels[j]>>8)&0xff;
                    F = (pixels[j]>>16)&0xff;
                }

                int A = 0xff;
                int color = (A & 0xff) << 24 | (F & 0xff) << 16 | (E & 0xff) << 8 | (D & 0xff);

                pixelsinn[j]=color;
                summation=0;
                D=0;
                E=0;
                F=0;

//            }

        }

        for (int j=0;j<height_mat*width_mat;j++){

//            for (int i=0;i<width_mat;i++){

                if(j>(r_far-1)*width_mat && j>(r_far-1)*height_mat && j<(width_mat-r_far)*height_mat && sigma_far>0.6 && j<a0*width_mat){
                    sigma_one = sigma_far;
                    for(int k = -r_far; k < r_far+1; k++){
                        summation += ((Math.exp(-((k*k)/(2*sigma_one*sigma_one))))*(1/Math.sqrt(2*3.14*sigma_one*sigma_one)));
                    }

                    for(int k = -r_far; k < r_far+1; k++){
                        B = pixelsinn[j+k]&0xff;
                        G = (pixelsinn[j+k]>>8)&0xff;
                        R = (pixelsinn[j+k]>>16)&0xff;
                        summationD = ((Math.exp(-((k*k)/(2*sigma_one*sigma_one))))*(1/Math.sqrt(2*3.14*sigma_one*sigma_one)));
                        normalD = summationD/summation;
                        D += normalD*B;
                        E += normalD*G;
                        F += normalD*R;
                    }

                }
                else if( j>(r_far-1)*height_mat && j<(width_mat-r_far)*height_mat && sigma_far>0.6 && j>=a0*width_mat && j<a1*width_mat){

                    fact = (double)j;
                    a_0 = (double)a0;
                    a_1 = (double)a1;
                    factor = (a_1-fact)*(1/(a_1-a_0));
                    sigma_one = sigma_far*factor;
                    r_far1 = (int)Math.ceil(2*sigma_one);

                    for (int k = -r_far1; k < r_far1 + 1; k++) {
                        summation += ((Math.exp(-((k * k) / (2 * sigma_one * sigma_one)))) *
                                (1 / Math.sqrt(2 * 3.14 * sigma_one * sigma_one)));
                    }

                    for (int k = -r_far1; k < r_far1 + 1; k++) {
                        B = pixelsinn[j + k] & 0xff;
                        G = (pixelsinn[j + k] >> 8) & 0xff;
                        R = (pixelsinn[j + k] >> 16) & 0xff;
                        summationD = ((Math.exp(-((k * k) / (2 * sigma_one * sigma_one)))) *
                                (1 / Math.sqrt(2 * 3.14 * sigma_one * sigma_one)));
                        normalD = summationD / summation;
                        D += normalD * B;
                        E += normalD * G;
                        F += normalD * R;
                    }

                }
                else if( j>(r_near-1)*height_mat && j<(width_mat-r_near)*height_mat && sigma_near>0.6 && j>a2*width_mat && j<=a3*width_mat){

                    fact = (double)j;
                    a_2 = (double)a2;
                    a_3 = (double)a3;
                    factor = (fact-a_2)*(1/(a_3-a_2));
                    sigma_one = sigma_near*factor;
                    r_near1 = (int)Math.ceil(2*sigma_one);

                    for (int k = -r_near1; k < r_near1 + 1; k++) {
                        summation += ((Math.exp(-((k * k) / (2 * sigma_one * sigma_one)))) *
                                (1 / Math.sqrt(2 * 3.14 * sigma_one * sigma_one)));
                    }

                    for (int k = -r_near1; k < r_near1 + 1; k++) {
                        B = pixelsinn[j + k] & 0xff;
                        G = (pixelsinn[j + k] >> 8) & 0xff;
                        R = (pixelsinn[j + k] >> 16) & 0xff;
                        summationD = ((Math.exp(-((k * k) / (2 * sigma_one * sigma_one)))) *
                                (1 / Math.sqrt(2 * 3.14 * sigma_one * sigma_one)));
                        normalD = summationD / summation;
                        D += normalD * B;
                        E += normalD * G;
                        F += normalD * R;
                    }

                }
                else if(j<(height_mat-r_near)*width_mat && j>(r_near-1)*height_mat && j<(width_mat-r_near)*height_mat && sigma_near>0.6 && j>a3*width_mat){


                    sigma_one = sigma_near;
                    for(int k = -r_near; k < r_near+1; k++){
                        summation += ((Math.exp(-((k*k)/(2*sigma_one*sigma_one))))*(1/Math.sqrt(2*3.14*sigma_one*sigma_one)));
                    }

                    for(int k = -r_near; k < r_near+1; k++){
                        B = pixelsinn[j+k]&0xff;
                        G = (pixelsinn[j+k]>>8)&0xff;
                        R = (pixelsinn[j+k]>>16)&0xff;
                        summationD = ((Math.exp(-((k*k)/(2*sigma_one*sigma_one))))*(1/Math.sqrt(2*3.14*sigma_one*sigma_one)));
                        normalD = summationD/summation;
                        D += normalD*B;
                        E += normalD*G;
                        F += normalD*R;
                    }


                }
                else{
                    D = pixels[j]&0xff;;
                    E = (pixels[j]>>8)&0xff;
                    F = (pixels[j]>>16)&0xff;
                }


                int A = 0xff;
                int color = (A & 0xff) << 24 | (F & 0xff) << 16 | (E & 0xff) << 8 | (D & 0xff);

                pixelsOut[j]=color;
                summation=0;
                D=0;
                E=0;
                F=0;

 //           }

        }

//        pixels = null;
//        pixelsinn = null;
//        pixelsOut = null;

        outBmp.setPixels(pixelsOut,0,width_mat,0,0,width_mat,height_mat);

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
