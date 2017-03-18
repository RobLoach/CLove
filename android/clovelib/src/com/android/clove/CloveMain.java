/*
#   clove
#
#   Copyright (C) 2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
package com.android.clove;

import android.app.Activity;
import android.os.Bundle;

public class CloveMain extends Activity {

	private CloveView mView;

	@Override
	protected void onCreate(Bundle icicle) {
		super.onCreate(icicle);
		mView = new CloveView(getApplication());
		setContentView(mView);
	}

	@Override
	protected void onPause() {
		super.onPause();
		mView.onPause();
	}

	@Override
	protected void onResume() {
		super.onResume();
		mView.onResume();
	}
}
