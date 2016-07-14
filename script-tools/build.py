# coding=utf8
###
 # Created By YanChunGuang
 # 编译打包
###
import os
import sys
import subprocess
import getopt
import re
import shutil

############## 输出调整 ##############
class Log:
    GREEN = '\033[32m'
    RED = '\033[91m'
    YELLOW = '\033[33m'
    BLUE = '\033[34m'
    PURPLE = '\033[35m'
    END = '\033[0m'


    @staticmethod
    def i(info):
        print Log.GREEN + info + Log.END

    @staticmethod
    def w(info):
        print Log.YELLOW + info + Log.END

    @staticmethod
    def e(info):
        print Log.RED + info + Log.END

    @staticmethod
    def blue(info):
        print Log.BLUE + info + Log.END

    @staticmethod
    def purple(info):
        print Log.PURPLE + info + Log.END

class CmdError(Exception):
  def __init__(self, err_args):
          super(CmdError, self).__init__(err_args)
          Log.e(err_args)

def _run_cmd(command):
  ret = os.system(command)
  if ret != 0:
      message = "Error running command"
      raise CmdError(message)

# check env

Log.blue('Check the environment.\n')

def _check_ndk_root_env():
    Log.i(">>Checking the environment NDK_ROOT, which will be used for building")

    try:
        NDK_ROOT = os.environ['NDK_ROOT']
    except Exception:
        Log.e("NDK_ROOT not defined. Please define NDK_ROOT in your environment.")
        sys.exit(1)

    return NDK_ROOT

def _check_python_bin_env():
    Log.i(">>Checking the environment PYTHON_BIN, which will be used for building...")
    try:
        PYTHON_BIN = os.environ['PYTHON_BIN']
    except Exception:
        Log.w("PYTHON_BIN not defined, use current python.")
        PYTHON_BIN = sys.executable

    return PYTHON_BIN

def _check_sdk_env():
    Log.i(">>Checking the environment ANDROID_SDK_ROOT, which will be used for building...")
    try:
        ANDROID_SDK_ROOT = os.environ['ANDROID_SDK_ROOT']
    except Exception:
        Log.e("ANDROID_SDK_ROOT not defined, Please define ANDROID_SDK_ROOT in your environment.")
        sys.exit(1)
    return ANDROID_SDK_ROOT

def _check_ant_env():
    Log.i(">>Checking the environment ANT_ROOT, which will be used for building...")
    try:
        ANT_ROOT = os.environ['ANT_ROOT']
    except Exception:
        Log.e("ANT_ROOT not defined, Please define ANT_ROOT in your environment.")
        sys.exit(1)
    return ANT_ROOT


PROJECT_ROOT = os.path.join(os.getcwd(),'..','..')
NDK_ROOT = _check_ndk_root_env()
PYTHON_BIN = _check_python_bin_env()
ANDROID_SDK_ROOT = _check_sdk_env()
ANT_ROOT = _check_ant_env()
ANDROID_ROOT = os.path.join(os.getcwd(),'..','runtime-src/proj.android')

Log.blue('\nCheck the environment success.\n')

Log.purple('PROJECT_ROOT==>'+PROJECT_ROOT)
Log.purple('NDK_ROOT==>'+NDK_ROOT)
Log.purple('PYTHON_BIN==>'+PYTHON_BIN)
Log.purple('ANDROID_SDK_ROOT==>'+ANDROID_SDK_ROOT)
Log.purple('ANT_ROOT==>'+ANT_ROOT + '\n')


def get_num_of_cpu():
    Log.i('>>The build process can be accelerated by running multiple concurrent job processes using the -j-option.')
    try:
        platform = sys.platform
        if platform == 'win32':
            if 'NUMBER_OF_PROCESSORS' in os.environ:
                return int(os.environ['NUMBER_OF_PROCESSORS'])
            else:
                return 1
        else:
            from numpy.distutils import cpuinfo
            return cpuinfo.cpu._getNCPUs()
    except Exception:
        Log.w("Can't know cpuinfo, use default 1 cpu")
        return 1


Log.i('cpu num = %d \n' % get_num_of_cpu())


def select_toolchain_version(ndk_root):
     ret_version = "4.8"

     version_file_path = os.path.join(ndk_root, "RELEASE.TXT")
     try:
         versionFile = open(version_file_path)
         lines = versionFile.readlines()
         versionFile.close()

         version_num = None
         version_char = None
         pattern = r'^[a-zA-Z]+(\d+)(\w)'
         for line in lines:
             str_line = line.lstrip()
             match = re.match(pattern, str_line)
             if match:
                 version_num = int(match.group(1))
                 version_char = match.group(2)
                 break

         if version_num is None:
             Log.i("Parse NDK version from file %s failed." % version_file_path)
         else:
             version_char = version_char.lower()
             if version_num > 10 or (version_num == 10 and cmp(version_char, 'c') >= 0):
                 ret_version = "4.9"

     except:
         Log.i("Parse NDK version from file %s failed." % version_file_path)

     Log.i("NDK_TOOLCHAIN_VERSION: %s" % ret_version)
     if ret_version == "4.8":
         Log.w(
             "Your application may crash when using c++ 11 regular expression with NDK_TOOLCHAIN_VERSION %s \n" % ret_version)

     return ret_version


def convert_path_to_cmd(path):
        """ Convert path which include space to correct style which bash(mac) and cmd(windows) can treat correctly.

            eg: on mac: convert '/usr/xxx/apache-ant 1.9.3' to '/usr/xxx/apache-ant\ 1.9.3'
            eg: on windows: convert '"c:\apache-ant 1.9.3"\bin' to '"c:\apache-ant 1.9.3\bin"'
        """
        ret = path
        if sys.platform == 'darwin':
            ret = path.replace("\ ", " ").replace(" ", "\ ")

        if sys.platform == 'win32':
            ret = "\"%s\"" % (path.replace("\"", ""))

        # print("!!!!! Convert %s to %s\n" % (path, ret))
        return ret

def convert_path_to_python(path):
    """ Convert path which include space to correct style which python can treat correctly.

        eg: on mac: convert '/usr/xxx/apache-ant\ 1.9.3' to '/usr/xxx/apache-ant 1.9.3'
        eg: on windows: convert '"c:\apache-ant 1.9.3"\bin' to 'c:\apache-ant 1.9.3\bin'
    """
    ret = path
    if sys.platform == 'darwin':
        ret = path.replace("\ ", " ")

    if sys.platform == 'win32':
        ret = ret.replace("\"", "")

    # print("!!!!! Convert %s to %s\n" % (path, ret))
    return ret

def get_target_config(proj_path):
    property_file = os.path.join(proj_path, "project.properties")
    if not os.path.isfile(property_file):
        raise CmdError('not find project.properties!!!')

    patten = re.compile(r'^target=(.+)')
    for line in open(property_file):
        str1 = line.replace(' ', '')
        str2 = str1.replace('\t', '')
        match = patten.match(str2)
        if match is not None:
            target = match.group(1)
            target_num = get_api_level(target)
            if target_num > 0:
                return target_num

    raise CmdError("在 project.properties 文件中未找到 'target'。")

def get_api_level(target_str, raise_error=True):
    special_targats_info = {
        "android-4.2" : 17,
        "android-L" : 20
    }

    if special_targats_info.has_key(target_str):
        ret = special_targats_info[target_str]
    else:
        match = re.match(r'android-(\d+)', target_str)
        if match is not None:
            ret = int(match.group(1))
        else:
            if raise_error:
                raise CmdError('%s 不是一個有效的 Android 目標平臺。' % (target_str))
            else:
                ret = -1

    return ret


def select_default_android_platform(min_api_level):
    ''' select a default android platform in SDK_ROOT
    '''
    sdk_root = ANDROID_SDK_ROOT
    platforms_dir = os.path.join(sdk_root, "platforms")
    ret_num = -1
    ret_platform = ""
    if os.path.isdir(platforms_dir):
        for dir_name in os.listdir(platforms_dir):
            if not os.path.isdir(os.path.join(platforms_dir, dir_name)):
                continue

            num = get_api_level(dir_name, raise_error=False)
            if num >= min_api_level:
                if ret_num == -1 or ret_num > num:
                    ret_num = num
                    ret_platform = dir_name

    if ret_num != -1:
        return ret_platform
    else:
        return None

# check the selected android platform
def check_android_platform(sdk_root, android_platform, proj_path, auto_select):
    ret = android_platform
    min_platform = get_target_config(proj_path)
    if android_platform is None:
        # not specified platform, found one
        Log.w("未指定 Android 目標平臺版本，自動查找一個可用版本...")
        ret = select_default_android_platform(min_platform)
    else:
        # check whether it's larger than min_platform
        select_api_level = get_api_level(android_platform)
        if select_api_level < min_platform:
            if auto_select:
                # select one for project
                ret = select_default_android_platform(min_platform)
            else:
                # raise error
                raise CmdError("'%s' 工程的 Android 目標平臺版本應該大於等於 %d，但是指定的值為 %d。" % (proj_path, min_platform, select_api_level))

    if ret is None:
        raise CmdError("未找到工程 '%s' 可用的 Android 目標平臺。 Android 目標平臺版本應該大於或等於 %d。" % (proj_path, min_platform))

    ret_path = os.path.join(convert_path_to_python(sdk_root), "platforms", ret)
    if not os.path.isdir(ret_path):
        raise CmdError("在 Android SDK 中未找到檔案夾 '%s'。" %  ret)

    special_platforms_info = {
        "android-4.2" : "android-17"
    }
    if special_platforms_info.has_key(ret):
        ret = special_platforms_info[ret]

    return ret

def update_lib_projects(sdk_root, sdk_tool_path, android_platform, property_path):
    property_file = os.path.join(property_path, "project.properties")
    if not os.path.isfile(property_file):
        return

    patten = re.compile(r'^android\.library\.reference\.[\d]+=(.+)')
    for line in open(property_file):
        str1 = line.replace(' ', '')
        str2 = str1.replace('\t', '')
        match = patten.match(str2)
        if match is not None:
            # a lib project is found
            lib_path = match.group(1)
            abs_lib_path = os.path.join(property_path, lib_path)
            abs_lib_path = os.path.normpath(abs_lib_path)
            if os.path.isdir(abs_lib_path):
                target_str = check_android_platform(sdk_root, android_platform, abs_lib_path, True)
                command = "%s update lib-project -p %s -t %s" % (convert_path_to_cmd(sdk_tool_path), abs_lib_path, target_str)
                _run_cmd(command)

                update_lib_projects(sdk_root, sdk_tool_path, android_platform, abs_lib_path)


def update_project(use_studio,android_platform):
    if use_studio:
        manifest_path = os.path.join(ANDROID_ROOT, 'app')
    else:
        manifest_path = ANDROID_ROOT

    sdk_tool_path = os.path.join(ANDROID_SDK_ROOT, "tools", "android")

    # check the android platform
    target_str = check_android_platform(ANDROID_SDK_ROOT, android_platform, manifest_path, False)

    # update project
    command = "%s update project -t %s -p %s" % (convert_path_to_cmd(sdk_tool_path), target_str, manifest_path)
    _run_cmd(command)

    # update lib-projects
    property_path = manifest_path
    update_lib_projects(ANDROID_SDK_ROOT, sdk_tool_path, android_platform, property_path)

    if use_studio:
        # copy the local.properties to the app_android_root
        file_name = 'local.properties'
        src_path = os.path.normpath(os.path.join(manifest_path, file_name))
        dst_path = os.path.normpath(os.path.join(app_android_root, file_name))
        if src_path != dst_path:
            if os.path.isfile(dst_path):
                os.remove(dst_path)
            shutil.copy(src_path, dst_path)

def copy_files(src, dst):
    for item in os.listdir(src):
        path = os.path.join(src, item)
        # Android can not package the file that ends with ".gz"
        if not item.startswith('.') and not item.endswith('.gz') and os.path.isfile(path):
            shutil.copy(path, dst)
        if os.path.isdir(path):
            new_dst = os.path.join(dst, item)
            os.mkdir(new_dst)
            copy_files(path, new_dst)

def copy_res():
    # remove app_android_root/assets if it exists
    assets_dir = os.path.join(ANDROID_ROOT, "assets")
    if os.path.isdir(assets_dir):
        shutil.rmtree(assets_dir)

    # copy resources
    os.mkdir(assets_dir)

    assets_res_dir = assets_dir + "/res";
    assets_scripts_dir = assets_dir + "/src";
    assets_jsb_dir = assets_dir + "/script";
    os.mkdir(assets_res_dir);
    os.mkdir(assets_scripts_dir);
    os.mkdir(assets_jsb_dir);


    shutil.copy(os.path.join(ANDROID_ROOT, "../../../main.js"), assets_dir)
    shutil.copy(os.path.join(ANDROID_ROOT, "../../../project.json"), assets_dir)

    resources_dir = os.path.join(ANDROID_ROOT, "../../../res")
    copy_files(resources_dir, assets_res_dir)

    resources_dir = os.path.join(ANDROID_ROOT, "../../../src")
    copy_files(resources_dir, assets_scripts_dir)

    resources_dir = os.path.join(ANDROID_ROOT, "../../cocos2d-x/cocos/scripting/js-bindings/script")
    copy_files(resources_dir, assets_jsb_dir)


def build_android(ndk_build_param,build_mode):
    cocos_root = os.path.join(ANDROID_ROOT, "..", "..", "cocos2d-x")
    ndk_path = os.path.join(NDK_ROOT, "ndk-build")
    ndk_toolchain_version = select_toolchain_version(NDK_ROOT)

    # windows should use ";" to seperate module paths
    platform = sys.platform
    if platform == 'win32':
        ndk_module_path = 'NDK_MODULE_PATH=%s/..;%s;%s/external;%s/cocos NDK_TOOLCHAIN_VERSION=%s' % (cocos_root, cocos_root, cocos_root, cocos_root, ndk_toolchain_version)
    else:
        ndk_module_path = 'NDK_MODULE_PATH=%s/..:%s:%s/external:%s/cocos NDK_TOOLCHAIN_VERSION=%s' % (cocos_root, cocos_root, cocos_root, cocos_root, ndk_toolchain_version)

    num_of_cpu = get_num_of_cpu()
    if ndk_build_param == None:
        command = '%s -j%d -C %s NDK_DEBUG=%d %s' % (ndk_path, num_of_cpu, ANDROID_ROOT, build_mode=='debug', ndk_module_path)
    else:
        command = '%s -j%d -C %s NDK_DEBUG=%d %s %s' % (ndk_path, num_of_cpu, ANDROID_ROOT, build_mode=='debug', ndk_build_param, ndk_module_path)
    Log.purple(command)
    if os.system(command) != 0:
        raise CmdError("Build dynamic library for project [ " + ANDROID_ROOT + " ] fails!")
    else:
        copy_res()
        ant_path = os.path.join(ANT_ROOT, 'ant')
        buildfile_path = os.path.join(ANDROID_ROOT, "build.xml")
        command = "%s clean %s -f %s -Dsdk.dir=%s" % (convert_path_to_cmd(ant_path),
                                                              build_mode, buildfile_path,
                                                              convert_path_to_cmd(ANDROID_SDK_ROOT))
        Log.purple(command)
        _run_cmd(command)
        #jarsigner -verbose -keystore androidguy-release.keystore ch02_showdatetime.apk androidguy


opts, args = getopt.getopt(sys.argv[1:], "-p:-m:")

platform = "android"
mode = "debug"

for name,value in opts:
    if name in ("-p"):
        platform = value
    if name in ("-m"):
        mode = value


if platform == "android":
    update_project(False,'android-19')
    build_android(None,mode)
else:
    Log.e("args error！！！");