/*
#   clove
#
#   Copyright (C) 2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
package com.android.clove;

public class CloveJNILib {

     static {
         System.loadLibrary("clove");
     }

     public static native void load(int width, int height);
     public static native void step();
}
