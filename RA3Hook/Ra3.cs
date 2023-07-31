using Microsoft.Win32;
using System;
using System.Diagnostics;
using System.IO;
using System.Runtime.InteropServices;
using System.Runtime.InteropServices.ComTypes;
using System.Text;
using System.Windows;

namespace RA3Hook
{
    internal static class Ra3
    {
        public const string Ra3WindowClass = "41DAF790-16F5-4881-8754-59FD8CF3B8D2";

        public static string GetDefaultRa3Path()
        {
            try
            {
                using var key = RegistryKey.OpenBaseKey(RegistryHive.LocalMachine, RegistryView.Registry32);
                using var subkey = key.OpenSubKey(@"SOFTWARE\Electronic Arts\Electronic Arts\Red Alert 3", false);
                var ra3Folder = subkey.GetValue("Install Dir") as string;
                if (!string.IsNullOrEmpty(ra3Folder))
                {
                    //return System.IO.Path.Combine(ra3Folder, "RA3.exe");
                    return ra3Folder;
                }
                MessageBox.Show("游戏注册表项无效");
                return null;
            }
            catch (Exception e)
            {
                MessageBox.Show($"游戏路径获取失败，请手动选择游戏路径：{e}");
                return null;
            }
        }

        public static string GetRa3Language()
        {
            using var key1 = RegistryKey.OpenBaseKey(RegistryHive.CurrentUser, RegistryView.Registry32);
            using var subkey1 = key1.OpenSubKey(@"SOFTWARE\Electronic Arts\Electronic Arts\Red Alert 3", false);
            var ra3lang = subkey1.GetValue("Language") as string;
            if (!string.IsNullOrEmpty(ra3lang))
            {
                return ra3lang;
            }
            return "english";
        }
        public static string GetDefaultBattleNetPath()
        {
            using var key = RegistryKey.OpenBaseKey(RegistryHive.LocalMachine, RegistryView.Registry32);
            RegistryKey software = key.OpenSubKey(@"SOFTWARE\Microsoft\Windows\CurrentVersion\App Paths\Ra3.BattleNet.Client.exe", false);
            var Folder = software.GetValue("") as string;
            if (!string.IsNullOrEmpty(Folder))
            {
                DirectoryInfo info = new DirectoryInfo(Folder);
                string filePath = info.Parent.FullName;
                filePath = System.IO.Path.Combine(filePath, "contents\\NativeDll.dll");
                return filePath;
            }
            return null;
        }

        /// <summary>
        ///   按照红警3的窗口类型 ID 找到红警3的窗口
        /// </summary>
        /// <returns>
        ///   红警3的窗口句柄
        /// </returns>
        public static IntPtr FindRa3Window()
        {
            var ra3Window = FindWindowW(Ra3WindowClass, null);
            if (ra3Window == IntPtr.Zero)
            {
                // 假如之前的 ID 找不到的话，试试找一下岚依的魔改版红警3（
                var modifiedWindowClass = "7" + Ra3WindowClass[1..];
                ra3Window = FindWindowW(modifiedWindowClass, null);
            }
            return ra3Window;
        }

        /// <summary>
        ///   按照红警3的窗口句柄找到红警3的窗口
        /// </summary>
        /// <param name="ra3Window">
        ///   红警3的窗口句柄，可用 <see cref="FindRa3Window"/> 获取
        /// </param>
        /// <returns>
        ///   红警3的进程，或者 null
        /// </returns>
        public static Process FindRa3Process(IntPtr ra3Window)
        {
            // 根据窗口句柄找到进程 ID
            _ = GetWindowThreadProcessId(ra3Window, out var processId);
            var process = Process.GetProcessById(processId);
            bool shouldDisplose = true; // 是否应该释放进程
            try
            {
                var name = process.ProcessName.ToUpperInvariant();
                // 万一人家开的是凯恩之怒呢
                if (!name.Contains("RA3_"))
                {
                    MessageBox.Show("请先打开红警3");
                    return null;
                }
                // 现在确定是 RA3 了
                if (name.Contains("1.12"))
                {
                    shouldDisplose = false; // 进程作为返回值返回了，因此不应该释放进程
                    return process;
                }
                // 不过假如不是 1.12 的话，那有大问题
                else
                {
                    MessageBox.Show("游戏版本不是 1.12，请升级到 1.12 后再试。");
                    return null;
                }
            }
            finally
            {
                if (shouldDisplose)
                {
                    process.Dispose();
                }
            }
        }

        /// <summary>
        ///   注入 DLL
        /// </summary>
        /// <param name="process">
        ///   要注入的进程
        /// </param>
        /// <param name="dllPath">
        ///   DLL 的路径
        /// </param>
        /// <param name="customData">
        ///   自定义数据，会传给 DLL 的 NativeInjectionEntryPoint 函数
        /// </param>
        /// <exception cref="Exception">
        ///   注入失败时抛出
        /// </exception>
        public static bool InjectAndWaitForExit(Process process, string dllPath, byte[] customData, int delayTime)
        {
            var result = RhInjectLibrary(process.Id, 0, 0, dllPath, null, customData, customData.Length);
            if (result != 0)
            {
                throw new Exception("Failed to inject the game: " + Marshal.PtrToStringUni(RtlGetLastErrorString()));
            }
            //process.WaitForExit();
            process.WaitForExit(delayTime);
            return true;
        }

        public static string GetModPath()
        {
            DirectoryInfo info = new DirectoryInfo(AppDomain.CurrentDomain.BaseDirectory);
            string filePath = info.Parent.FullName;
            return filePath;
        }
        public static int PowerfulRA3Launch(string FileName, string Arguments, string BattleNetPath)
        {
            return PowerfulGameLaunch(FileName, Arguments, BattleNetPath);
        }

        [DllImport("LauncherDLL.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        private static extern int PowerfulGameLaunch(string FileName, string Arguments, string BattleNetPath);
        
        // P/Invoke
        [DllImport("EasyHook32.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        private static extern IntPtr RtlGetLastErrorString();

        [DllImport("EasyHook32.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        private static extern int RhInjectLibrary(int InTargetPID,
                                                  int InWakeUpTID,
                                                  int InInjectionOptions,
                                                  string InLibraryPath_x86,
                                                  string InLibraryPath_x64,
                                                  byte[] InPassThruBuffer,
                                                  int InPassThruSize);

        [DllImport("User32.dll", CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, SetLastError = true)]
        private static extern IntPtr FindWindowW(string className, string windowName);

        [DllImport("User32.dll", CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Unicode, SetLastError = true)]
        private static extern bool GetClassNameW(IntPtr window, char[] className, int maxCount);

        [DllImport("User32.dll", CallingConvention = CallingConvention.Winapi, SetLastError = true)]
        private static extern int GetWindowThreadProcessId(IntPtr window, out int processId);
    }

    internal static class INIfile
    {
        public static string ReadString(string section, string key, string def, string filePath)
        {
            StringBuilder str = new StringBuilder(1024);
            GetPrivateProfileString(section, key, def, str, 1024, filePath);
            return str.ToString();
        }
        public static uint ReadINT(string section, string key, int value, string filePath)
        {
            return GetPrivateProfileInt(section, key, value, filePath);
        }

        public static int Write(string section, string key, string value, string filePath)
        {
            return WritePrivateProfileString(section, key, value, filePath);
        }

        [DllImport("kernel32.dll", CharSet = CharSet.Auto)]
        private static extern uint GetPrivateProfileInt(string lpAppName, string lpKeyName, int nDefault, string lpFileName);

        [DllImport("kernel32.dll", CharSet = CharSet.Auto)]
        private static extern int GetPrivateProfileString(string lpAppName, string lpKeyName, string lpDefault, StringBuilder lpReturnedString, int nSize, string lpFileName);

        [DllImport("kernel32.dll", CharSet = CharSet.Auto)]
        private static extern int WritePrivateProfileString(string lpApplicationName, string lpKeyName, string lpString, string lpFileName);
    }
}
