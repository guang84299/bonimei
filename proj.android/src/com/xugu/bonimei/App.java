package com.xugu.bonimei;

import com.umeng.fb.push.FeedbackPush;
import com.umeng.socialize.PlatformConfig;
import com.xugu.qewadlib.GApplication;

import android.app.Application;
import android.os.Bundle;


/**
 * Created by ntop on 15/7/8.
 */
public class App extends GApplication {

    @Override
    public void onCreate() {
    	FeedbackPush.getInstance(this).init(false);
        super.onCreate();
    }

    //各个平台的配置，建议放在全局Application或者程序入口
    {
        //微信 wx12342956d1cab4f9,a5ae111de7d9ea137e88a5e02c07c94d
        PlatformConfig.setWeixin("wx68af018cb2c236e8", "f25e26dc443843bb9d18bace2f9a423b");
        //新浪微博
        //PlatformConfig.setSinaWeibo("3921700954", "04b48b094faeb16683c32669824ebdad");
        PlatformConfig.setQQZone("1105585716", "ZZ2ktInrWka16XiB");
 
    }
}
