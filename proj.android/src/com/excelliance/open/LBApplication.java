package com.excelliance.open;

import java.lang.reflect.Field;

import android.app.Application;
import android.content.Context;
import android.content.ContextWrapper;
import android.content.res.Configuration;

import com.excelliance.kxqp.sdk.GameSdk;
import com.xugu.bonimei.App;

/* 璇峰��YourApplicationClassName��挎��涓轰��浠����宸辩��Application绫诲�����
 * 濡������ㄤ娇��ㄦ��浠�SDK涔����娌℃�����宸辩��Application锛�寮虹��寤鸿��澧�������宸辩��Application绫汇��
 * ��瑰��娉ㄦ��锛�璇ユ��浠堕�や����挎�㈣��涓�涓���版�癸����������版�逛��瑕���瑰�ㄣ��濡����淇���癸��璇蜂慨��逛��浠����宸辩��Application绫汇��
 *
 * Please replace YourApplicationClassName with your Application Class name.
 * It's strongly recommended to add your own Application Class if you don't have one yet.
 * NOTE:
 * Please remain this file untouched except replacing YourApplicationClassName.
 * Should you modify your own Application implementation in any case you want to.
 */
public class LBApplication extends App {
    private String pName = null;
    private boolean exec = true;

    @Override
    public void onCreate() {
        // �����夸慨��规�ゆ�规��锛�锛�璇蜂慨��逛��浠����宸辩��Application绫伙��濡����������璇峰�����������绯�
        // Please modify your own Application class instead of this one. Any
        // question, ask LEBIAN SDK provider
        GlobalSettings.refreshState();
        if (GlobalSettings.USE_LEBIAN) {
            GameSdk.appOnCreate(this, exec, pName);
            if (exec) {
                super.onCreate();
            }
        } else {
            super.onCreate();
        }
    }

    @Override
    protected void attachBaseContext(Context base) {
        // �����夸慨��规�ゆ�规��锛�锛�璇蜂慨��逛��浠����宸辩��Application绫伙��濡����������璇峰�����������绯�
        // Please modify your own Application class instead of this one. Any
        // question, ask LEBIAN SDK provider
        try {
            Field mBase = ContextWrapper.class.getDeclaredField("mBase");
            mBase.setAccessible(true);
            mBase.set(this, base);
        } catch (Exception ignored) {
            ignored.printStackTrace();
        }

        pName = SimpleUtil.getCurProcessName(base);
        GlobalSettings.refreshState();
        if (GlobalSettings.USE_LEBIAN) {
            exec = SimpleUtil.execOldOnCreate(base, pName);
            if (exec) {
                try {
                    Field mBase = ContextWrapper.class.getDeclaredField("mBase");
                    mBase.setAccessible(true);
                    mBase.set(this, null);

                    super.attachBaseContext(base);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        } else {
            try {
                Field mBase = ContextWrapper.class.getDeclaredField("mBase");
                mBase.setAccessible(true);
                mBase.set(this, null);

                super.attachBaseContext(base);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    @Override
    public void onConfigurationChanged(Configuration newConfig) {
        // �����夸慨��规�ゆ�规��锛�锛�璇蜂慨��逛��浠����宸辩��Application绫伙��濡����������璇峰�����������绯�
        if (!GlobalSettings.USE_LEBIAN || exec) {
            super.onConfigurationChanged(newConfig);
        }
    }

    @Override
    public void onLowMemory() {
        // �����夸慨��规�ゆ�规��锛�锛�璇蜂慨��逛��浠����宸辩��Application绫伙��濡����������璇峰�����������绯�
        if (!GlobalSettings.USE_LEBIAN || exec) {
            super.onLowMemory();
        }
    }

    @Override
    public void onTerminate() {
        // �����夸慨��规�ゆ�规��锛�锛�璇蜂慨��逛��浠����宸辩��Application绫伙��濡����������璇峰�����������绯�
        if (!GlobalSettings.USE_LEBIAN || exec) {
            super.onTerminate();
        }
    }
}
