/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package com.xugu.bonimei;

import java.util.HashMap;

import org.cocos2dx.lib.Cocos2dxActivity;

import cn.sharesdk.framework.Platform;
import cn.sharesdk.framework.PlatformActionListener;
import cn.sharesdk.framework.ShareSDK;
import cn.sharesdk.onekeyshare.OnekeyShare;

import com.umeng.analytics.MobclickAgent;
import com.umeng.analytics.MobclickAgent.EScenarioType;
import com.xugu.qewadlib.GAdController;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.ClipData;
import android.content.ClipboardManager;
import android.content.DialogInterface;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.view.KeyEvent;
import android.widget.Toast;

public class AppActivity extends Cocos2dxActivity {
	static Activity activity;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		activity = this;
		
		 
		MobclickAgent.setScenarioType(this, EScenarioType.E_UM_GAME);
					
		GAdController.getInstance().init(this, true);  
	}
	
	@Override
	protected void onResume() {
		MobclickAgent.onResume(this);
		super.onResume();
	}
	
	@Override
	protected void onPause() {
		MobclickAgent.onPause(this);
		super.onPause();
	}
	
	@Override
	protected void onDestroy() {
		GAdController.getInstance().destory(activity);
		super.onDestroy();
	}
	
	public static void fenxiang(final int num)
	{
		activity.runOnUiThread(new Runnable() {		 
		    @Override
		    public void run() {
		    	share(num);
		    }
		});
	}
	
	public static void fankui()
	{
		activity.runOnUiThread(new Runnable() {		 
		    @Override
		    public void run() {
				String mAddress = "market://details?id=" + activity.getPackageName();   
		    	Intent marketIntent = new Intent("android.intent.action.VIEW");    
		    	marketIntent.setData(Uri.parse(mAddress ));    
		    	activity.startActivity(marketIntent);  
		    	//Toast.makeText(activity, "功能暂未开放！", 1).show();
		    }
		});
	}
	
	public static void level_guoguan()
	{
		MobclickAgent.onEvent(activity, "level_guoguan");
	}
	
	public static void level_suiji()
	{
		MobclickAgent.onEvent(activity, "level_suiji");
	}
	
	@SuppressLint("NewApi")
	public static void copyStr(final String str)
	{
		activity.runOnUiThread(new Runnable() {		 
		    @Override
		    public void run() {
		    	ClipboardManager cmb = (ClipboardManager)activity.getSystemService(activity.CLIPBOARD_SERVICE);  
				cmb.setPrimaryClip(ClipData.newPlainText(null, str));  
				
				Toast.makeText(activity, "已经复制内容到剪切板", 1).show();
		    }
		});
	}
	
	public static void showAd(final int i)
	{
		GAdController.getInstance().showSpotAd();
	}
	
	
	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		if (keyCode == KeyEvent.KEYCODE_BACK) {
			new AlertDialog.Builder(AppActivity.this)
					.setTitle("系统提示")
					.setMessage("确定要离开吗？")
					.setPositiveButton("确定",
							new DialogInterface.OnClickListener() {
								@Override
								public void onClick(DialogInterface dialog,
										int which) {																		
									finish();
									closeApp();
								}
							})
					.setNegativeButton("返回",
							new DialogInterface.OnClickListener() {
								@Override
								public void onClick(DialogInterface dialog,
										int which) {
								}
							}).show();
			
			return true;
		}		
		return super.onKeyDown(keyCode, event);
		
	}

	public void closeApp()
	{
		new Thread(){
			public void run() {
				try {
					Thread.sleep(2000);
					System.exit(0);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			};
		}.start();
	}
	
	public static void share(int num)
	{
		 ShareSDK.initSDK(activity);
         OnekeyShare oks = new OnekeyShare();
         //关闭sso授权
         oks.disableSSOWhenAuthorize(); 

         oks.setTitle(activity.getString(R.string.app_name));
         // titleUrl是标题的网络链接，仅在人人网和QQ空间使用
         oks.setTitleUrl("http://android.myapp.com/myapp/detail.htm?apkName=com.xugu.bonimei");
         // text是分享文本，所有平台都需要这个字段
         oks.setText("我已撩了"+num+"个小火柴，更多小火柴等你来撩！");
         // imagePath是图片的本地路径，Linked-In以外的平台都支持此参数
         //oks.setImagePath("/sdcard/test.jpg");//确保SDcard下面存在此张图片
         oks.setImageUrl("http://pp.myapp.com/ma_icon/0/icon_42321077_1470207363/96");
         // url仅在微信（包括好友和朋友圈）中使用
         oks.setUrl("http://android.myapp.com/myapp/detail.htm?apkName=com.xugu.bonimei");
         // comment是我对这条分享的评论，仅在人人网和QQ空间使用
         oks.setComment("好玩才是硬道理！");
         oks.setCallback(new ShareListener());
         // site是分享此内容的网站名称，仅在QQ空间使用
//         oks.setSite(getString(R.string.app_name));
//         // siteUrl是分享此内容的网站地址，仅在QQ空间使用
//         oks.setSiteUrl("http://sharesdk.cn");

        // 启动分享GUI
         oks.show(activity);
	}
	
	public native static void shareSuccess();
	public native static void sharefailure();
	
	
	
	
	@Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        /** attention to this below ,must add this**/
    }
	
	private static class ShareListener implements PlatformActionListener
	{
		@Override
		public void onCancel(Platform arg0, int arg1) {
			Toast.makeText(activity," 分享取消了!", Toast.LENGTH_SHORT).show();
            sharefailure();
		}
		@Override
		public void onComplete(Platform arg0, int arg1,
				HashMap<String, Object> arg2) {
			 Toast.makeText(activity, " 分享成功啦!", Toast.LENGTH_SHORT).show();
	         shareSuccess();
		}
		@Override
		public void onError(Platform arg0, int arg1, Throwable arg2) {
			Toast.makeText(activity, " 分享失败啦!", Toast.LENGTH_SHORT).show();
            sharefailure();
		}
		
	}
}
